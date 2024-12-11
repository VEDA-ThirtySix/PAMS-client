#include "tcpManager.h"
#include "jsonManager.h"
#include "logManager.h"
#include "metadata.h"
#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>
#include <QTimer>
#include <QRegularExpression>

// Singleton의 private 생성자 정의
TcpManager::TcpManager(QObject *parent)
    : QObject(parent)
    , tcpSocket(new QTcpSocket(this))
{
    qDebug() << "[TcpManager] Constructor initialized";
    setupSocket();

    contentLength = -1;
    headerParsed = false;

    connect(tcpSocket, &QTcpSocket::connected, this, []() {
        qDebug() << "[TcpManager] Socket connected";
    });

    connect(tcpSocket, &QTcpSocket::readyRead, this, &TcpManager::on_readyRead);

    connect(tcpSocket, &QTcpSocket::disconnected, this, []() {
        qDebug() << "[TcpManager] Socket disconnected";
    });

    connect(tcpSocket, &QTcpSocket::disconnected, this, &TcpManager::handleDisconnect);
}

TcpManager::~TcpManager()
{
    if (tcpSocket->state() == QAbstractSocket::ConnectedState) {
        tcpSocket->disconnectFromHost();
    }
}

void TcpManager::connectToServer(const QString& host, quint16 port) {
    if (tcpSocket->state() == QAbstractSocket::ConnectedState) {
        m_serverAddress = host;
        m_serverPort = port;

        tcpSocket->disconnectFromHost();
        qDebug() << "ConnetToServer -> disconnect";
    }

    tcpSocket->connectToHost(host, port);
}

bool TcpManager::parse_header() {
    int headerEnd = buffer.indexOf("\r\n\r\n");

    if (headerEnd == -1) {
        qDebug() << "Header end not found";
        return false;
    }

    QString header = QString::fromUtf8(buffer.left(headerEnd));
    qDebug() << "Found header:" << header;

    QRegularExpression rx("Content-Length: (\\d+)");
    QRegularExpressionMatch match = rx.match(header);

    if (match.hasMatch()) {
        contentLength = match.captured(1).toLongLong();
        buffer.remove(0, headerEnd + 4); // Remove the header part
        return true;
    }

    qDebug() << "Content-Length not found in header";
    return false;
}

void TcpManager::on_readyRead() {
    buffer.append(tcpSocket->readAll());

    qDebug() << "[TcpManager] Received data, total buffer size:" << buffer.size();

    if (!headerParsed) {
        headerParsed = parse_header();

        if (!headerParsed) {
            qDebug() << "Waiting for complete header";
            return;
        }
    }

    if (buffer.size() >= contentLength) {
        qDebug() << "Received complete data";

        JsonManager *jsonManager = new JsonManager(this);
        TimeInfo timeInfo = jsonManager->parse_data(buffer);
        QString plate = timeInfo.get_plate();

        QByteArray parsed_base64 = jsonManager->decode_base64(buffer);

        if (jsonManager->saveImageFromByteArray(parsed_base64, plate)) {
            qDebug() << "new JPG file:" << plate << ".jpg";
        } else {
            qDebug() << "Saving JPG File Failed";
        }

        LogManager *logManager = new LogManager(this);
        logManager->save_dataImage(timeInfo, parsed_base64);

        qDebug() << "PLATE: TimeInfo + BLOB done";

        qDebug() << "emit plateDataReceived";
        emit plateDataReceived(buffer); // Emit signal

        buffer.clear();
        contentLength = -1;
        headerParsed = false;

    } else {
        qDebug() << "Waiting for more data. Current:" << buffer.size()
        << "Expected:" << contentLength;
    }
}

void TcpManager::handleDisconnect()
{
    qDebug() << "[TcpManager] Socket disconnected";

    if (tcpSocket) {
        tcpSocket->close();
    }
}

void TcpManager::setupSocket() {
    tcpSocket->setSocketOption(QAbstractSocket::KeepAliveOption, 1);
}

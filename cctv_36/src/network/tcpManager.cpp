#include "tcpManager.h"
#include "jsonManager.h"
#include "logManager.h"
#include "metadata.h"
#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>
#include <QTimer>
#include <QRegularExpression>

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
    if(tcpSocket->state() == QAbstractSocket::ConnectedState) {
        tcpSocket->disconnectFromHost();
    }
}

void TcpManager::connectToServer(const QString& host, quint16 port) {
    if(tcpSocket->state() == QAbstractSocket::ConnectedState) {
        m_serverAddress = host;
        m_serverPort = port;

        tcpSocket->disconnectFromHost();
        qDebug() << "ConnetToServer -> disconnect";
    }
    tcpSocket->connectToHost(host, port);

}

bool TcpManager::parse_header() {
    // 헤더의 끝(\r\n\r\n)을 찾음
    int headerEnd = buffer.indexOf("\r\n\r\n");
    if (headerEnd == -1) {
        qDebug() << "Header end not found";
        return false;
    }

    // 헤더 문자열 추출
    QString header = QString::fromUtf8(buffer.left(headerEnd));
    qDebug() << "Found header:" << header;

    // Content-Length 찾기
    QRegularExpression rx("Content-Length: (\\d+)");
    QRegularExpressionMatch match = rx.match(header);
    if (match.hasMatch()) {
        contentLength = match.captured(1).toLongLong();

        // 헤더 부분을 버퍼에서 제거
        buffer.remove(0, headerEnd + 4); // 4 = \r\n\r\n
        return true;
    }

    qDebug() << "Content-Length not found in header";
    return false;
}



void TcpManager::on_readyRead() {
    buffer.append(tcpSocket->readAll());
    qDebug() << "[TcpManager] Received data, total buffer size:" << buffer.size();

    // 헤더 파싱이 안된 상태라면 파싱 시도
    if (!headerParsed) {
        headerParsed = parse_header();
        if (!headerParsed) {
            qDebug() << "Waiting for complete header";
            return; // 헤더가 완전히 수신되지 않음
        }
    }

    // 데이터가 모두 수신되었는지 확인
    if (buffer.size() >= contentLength) {
        qDebug() << "Received complete data";

        // 완전한 JSON 데이터 처리
        JsonManager *jsonManager = new JsonManager(this);
        TimeInfo timeInfo = jsonManager->parse_data(buffer);
        QString plate = timeInfo.get_plate();

        QByteArray parsed_base64 = jsonManager->decode_base64(buffer);
        if(jsonManager->saveImageFromByteArray(parsed_base64, plate)) {
            qDebug() << "new JPG file:" << plate << ".jpg";
        } else {
            qDebug() << "Saving JPG File Failed";
        }

        LogManager *logManager = new LogManager(this);
        logManager->save_dataImage(timeInfo, parsed_base64);
        qDebug() << "PLATE: TimeInfo + BLOB done";

        // 버퍼 초기화
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

    // 재연결 시도 또는 정리 작업 수행
    if (tcpSocket) {
        tcpSocket->close();
        // 필요한 경우 재연결 시도
        // reconnectToServer();
    }
}

// void TcpManager::reconnectToServer()
// {
//     const int MAX_RETRY_COUNT = 5;
//     const int RETRY_INTERVAL = 3000; // 3초

//     if (!tcpSocket) {
//         tcpSocket = new QTcpSocket(this);
//         connect(tcpSocket, &QTcpSocket::connected, this, []() {
//             qDebug() << "[TcpManager] Socket connected";
//         });        connect(tcpSocket, &QTcpSocket::errorOccurred, this, &TcpManager::handleError);
//         connect(tcpSocket, &QTcpSocket::disconnected, this, &TcpManager::handleDisconnect);
//     }

//     static int retryCount = 0;
//     if (retryCount < MAX_RETRY_COUNT) {
//         qDebug() << "[TcpManager]  ting to reconnect..." << "Attempt:" << retryCount + 1;

//         // 이전 연결 정보 사용
//         tcpSocket->connectToHost(m_serverAddress, m_serverPort);

//         if (tcpSocket->waitForConnected(RETRY_INTERVAL)) {
//             qDebug() << "[TcpManager] Reconnection successful";
//             retryCount = 0;  // 성공하면 재시도 카운트 리셋
//         } else {
//             retryCount++;
//             QTimer::singleShot(RETRY_INTERVAL, this, &TcpManager::reconnectToServer);
//         }
//     } else {
//         qDebug() << "[TcpManager] Max reconnection attempts reached";
//         retryCount = 0;  // 재시도 카운트 리셋
//     }
//}


// Qt TCP 소켓 keep-alive 설정 예시
void TcpManager::setupSocket() {
    tcpSocket->setSocketOption(QAbstractSocket::KeepAliveOption, 1);

    // 연결 오류 처리
    // connect(tcpSocket, &QTcpSocket::errorOccurred, this, [this](QAbstractSocket::SocketError error) {
    //     qDebug() << "Socket Error:" << error;
    //     reconnectToServer();
    // });
}

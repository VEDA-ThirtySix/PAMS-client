#include "tcpManager.h"
#include "metadata.h"
#include <QDebug>
#include <QByteArray>

TcpManager::TcpManager(QObject *parent)
    : QObject(parent)
    , tcpSocket(new QTcpSocket(this))
    , httpManager(nullptr)
{
    qDebug() << "DEBUG(TM): exec constructor";

    connect(tcpSocket, &QTcpSocket::connected, this, &TcpManager::on_connected);
    connect(tcpSocket, &QTcpSocket::readyRead, this, &TcpManager::on_readyRead);
    connect(tcpSocket, &QTcpSocket::disconnected, this, &TcpManager::on_disconnected);
}

TcpManager::~TcpManager()
{ }

bool TcpManager::connect_server(const QString& host, quint16 port) {
    //Connect Server with TCP
    tcpSocket->connectToHost(host, port);
    qDebug() << "DONE(TM): connect_server(host:port): " << host << ":" << port;

    //wait for TCP connection before HTTP POST request
    if(!tcpSocket->waitForConnected()) {
        qDebug() << "ERROR(TM): TCP Connection Failed";
        return false;
    }

    //Post JSON(INIT) to Server
    QUrl url(QString("http://%1:%2").arg(host).arg(port));
    httpManager = new HttpManager(this);
    ClientInfo clientInfo;
    clientInfo = httpManager->set_ClientInfo(url);
    connect(tcpSocket, &QTcpSocket::connected, this, [this, url, clientInfo] {
        qDebug() << "DEBUG(TM): TCP connection Checked";
        httpManager->post_initInfo(url, clientInfo);    //Synchronize(TCP, HTTP)
    });

    return true;
}

void TcpManager::disconnect_server() {
    tcpSocket->disconnectFromHost();
}

void TcpManager::on_connected() {
    qDebug() << "DONE(TM): on_connected";
}

void TcpManager::on_readyRead() {
    httpManager = new HttpManager(this);
    QByteArray responseArray = tcpSocket->readAll();
    qDebug() << "************ New Response Occurred ************";
    httpManager->handle_response(responseArray);
}

void TcpManager::on_disconnected() {
    qDebug() << "Disconnected from Server";
}

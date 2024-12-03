#include "tcpManager.h"
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
    tcpSocket->connectToHost(host, port);
    qDebug() << "DONE(TM): connect_server(host:port): " << host << ":" << port;
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
    httpManager->handle_response(responseArray);
}

void TcpManager::on_disconnected() {
    qDebug() << "Disconnected from Server";
}

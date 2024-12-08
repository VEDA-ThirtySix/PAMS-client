/*
#ifndef TCPMANAGER_H
#define TCPMANAGER_H

#include "httpManager.h"
#include <QObject>
#include <QString>
#include <QTcpSocket>

class TcpManager : public QObject {
    Q_OBJECT

public:
    TcpManager(QObject *parent);
    ~TcpManager();

    bool connect_server(const QString& host, quint16 port);
    void disconnect_server();

private:
    QTcpSocket *tcpSocket;
    HttpManager *httpManager;

private slots:
    void on_connected();
    void on_readyRead();
    void on_disconnected();
};

#endif // TCPMANAGER_H
*/

#ifndef TCPMANAGER_H
#define TCPMANAGER_H

#include <QObject>
#include <QString>
#include <QTcpSocket>
#include <QByteArray>

class TcpManager : public QObject {
    Q_OBJECT

public:
    explicit TcpManager(QObject *parent = nullptr);
    ~TcpManager();

    void connectToServer(const QString& host, quint16 port);
    bool parse_header();

private:
    QTcpSocket *tcpSocket;
    QString m_serverAddress;
    quint16 m_serverPort;
    QByteArray buffer;
    quint64 contentLength;
    bool headerParsed;

private slots:
    void on_readyRead();
    //void on_disconnected();
    void handleDisconnect();
    //void reconnectToServer();
    void setupSocket();

signals:
    void plateDataReceived(const QByteArray &data);
};

#endif // TCPMANAGER_H

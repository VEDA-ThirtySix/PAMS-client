#ifndef SSLSOCKET_H
#define SSLSOCKET_H

#include <QObject>
#include <QSslSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

class SSLClient : public QObject {
    Q_OBJECT

private:
    QSslSocket* socket;

public:
    explicit SSLClient(QObject* parent = nullptr);
    void connectToServer(const QString& host, quint16 port);

private slots:
    void onConnected();
    void onEncrypted();
    void onError(QAbstractSocket::SocketError error);
    void onSslErrors(const QList<QSslError>& errors);
    void onReadyRead();
    void processJsonData(const QByteArray& data);
};

#endif // SSLSOCKET_H

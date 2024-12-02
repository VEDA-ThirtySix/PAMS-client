#include "sslSocket.h"
#include <QSslConfiguration>

SSLClient::SSLClient(QObject* parent) : QObject(parent) {
    socket = new QSslSocket(this);

    connect(socket, &QSslSocket::connected, this, &SSLClient::onConnected);
    connect(socket, &QSslSocket::encrypted, this, &SSLClient::onEncrypted);
    connect(socket, &QSslSocket::errorOccurred, this, &SSLClient::onError);
    connect(socket, &QSslSocket::sslErrors, this, &SSLClient::onSslErrors);
    connect(socket, &QSslSocket::readyRead, this, &SSLClient::onReadyRead);
}

void SSLClient::connectToServer(const QString& host, quint16 port) {
    QSslConfiguration config = socket->sslConfiguration();
    config.setPeerVerifyMode(QSslSocket::VerifyNone);  // 테스트용
    socket->setSslConfiguration(config);

    socket->connectToHostEncrypted(host, port);
}

void SSLClient::onReadyRead() {
    QByteArray data = socket->readAll();
    processJsonData(data);
}

void SSLClient::processJsonData(const QByteArray& data) {
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
    if (!jsonDoc.isNull() && jsonDoc.isObject()) {
        QJsonObject jsonObj = jsonDoc.object();

        // JSON 데이터 처리
        QString name = jsonObj["name"].toString();
        int age = jsonObj["age"].toInt();
        QString city = jsonObj["city"].toString();

        QJsonArray hobbies = jsonObj["hobbies"].toArray();

        qDebug() << "Received data:";
        qDebug() << "Name:" << name;
        qDebug() << "Age:" << age;
        qDebug() << "City:" << city;
        qDebug() << "Hobbies:";
        for (const QJsonValue& hobby : hobbies) {
            qDebug() << "- " << hobby.toString();
        }
    }
}

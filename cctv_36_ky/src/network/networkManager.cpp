#include "networkManager.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkInterface>
#include <QNetworkAddressEntry>

NetworkManager::NetworkManager(QObject* parent)
    : QObject(parent)
    , accessManager(nullptr)
    , jsonManager(nullptr)
    , logManager(nullptr)
{ }

NetworkManager::~NetworkManager()
{ }

QUrl NetworkManager::set_config(const QString& url, const QString& port) {
    QString fullURL = url;
    if(!url.startsWith("http://") && !url.startsWith("https://")) {
        fullURL = "http://" + url;
    }
    QUrl serverURL = QUrl(fullURL);

    if(port.toInt() > 0) {
        serverURL.setPort(port.toInt());
    } else {
        qDebug() << "ERROR(NM): set_config >> Wrong Port Number";
    }

    if(!serverURL.isValid()) {
        qDebug() << "ERROR(NM): set_config >> Invalid URL Format: " << serverURL;
        return QUrl();
    } else {
        qDebug() << "DONE(NM): set_config >> set_config(network) Successful(URL): " << serverURL;
        return serverURL;
    }
}

bool NetworkManager::set_connection(const QUrl& url, const ClientInfo& clientInfo) {
    accessManager = new QNetworkAccessManager(this);
    /*
    // SSL/TLS 설정 (HTTPS 사용시)
    QNetworkRequest request(serverURL);
    QSslConfiguration config = QSslConfiguration::defaultConfiguration();
    config.setProtocol(QSsl::TlsV1_2);
    request.setSslConfiguration(config); */

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    jsonManager = new JSONManager(this);
    QByteArray jsonArray = jsonManager->send_request_init(clientInfo);
    connect(accessManager, &QNetworkAccessManager::finished, this, &NetworkManager::reply_finished);
    qDebug() << "DONE(NM): set_connection";

    accessManager->post(request, jsonArray);
    return true;
}
/*
void NetworkManager::post_initInfo(const QUrl& url, const ClientInfo& clientInfo) {
    jsonManager = new JSONManager(this);
    accessManager = new QNetworkAccessManager(this);

    QByteArray jsonArray = jsonManager->send_request_init(clientInfo);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    qDebug() << "DONE(NM): post_initInfo";

    accessManager->post(request, jsonArray);
}*/

void NetworkManager::post_userInfo(const QUrl& url, const BasicInfo& basicInfo) {
    jsonManager = new JSONManager(this);
    accessManager = new QNetworkAccessManager(this);

    QByteArray jsonArray = jsonManager->send_request_info(basicInfo);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    qDebug() << "DONE(NM): post_userInfo";

    accessManager->post(request, jsonArray);
}

void NetworkManager::post_clipInfo(const QUrl& url, const TimeInfo& timeInfo) {
    jsonManager = new JSONManager(this);
    accessManager = new QNetworkAccessManager(this);

    QByteArray jsonArray = jsonManager->send_request_clip(timeInfo);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    qDebug() << "DONE(NM): post_clipInfo";

    accessManager->post(request, jsonArray);
}

void NetworkManager::handle_response(QByteArray& jsonArray) {
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonArray);
    QJsonObject jsonObj = jsonDoc.object();

    jsonManager = new JSONManager(this);
    switch(jsonManager->check_response(jsonArray)) {
        case 1: //response: status = "success"
        {
            TimeInfo timeInfo;
            timeInfo = jsonManager->parse_response(jsonArray);
            qDebug() << "DONE(NM): Parse Response to TimeInfo";

            logManager = new LogManager(this);
            logManager->save_log(timeInfo);
            qDebug() << "DONE(NM): Save TimeInfo to DB";
        }
        case 2: //response: status = else
            qDebug() << "ERROR(NM): cannot parse response due to status(!success)";
        case 3: //response: wrong status
            qDebug() << "ERROR(NM): cannot parse response due to status(wrong status)";
    }
}

void NetworkManager::reply_finished(QNetworkReply* reply) {
    if(reply->error()) {
        qDebug() << "ERROR(NM): Connection Error: " << reply->errorString();
    } else {
        qDebug() << "DONE(NM): Connection Successful";
    }
    reply->deleteLater();
}

QString NetworkManager::getLocalIPInSameSubnet(const QUrl& serverURL) {
    // 서버 호스트 주소를 QHostAddress로 변환
    QHostAddress serverAddr(serverURL.host());

    // 모든 네트워크 인터페이스 순회
    for(const QNetworkInterface &interface: QNetworkInterface::allInterfaces()) {
        // 활성화된 인터페이스만 선택
        if(interface.flags().testFlag(QNetworkInterface::IsUp) &&
            interface.flags().testFlag(QNetworkInterface::IsRunning)) {

            // 해당 인터페이스의 IP 주소들을 확인
            for(const QNetworkAddressEntry &entry: interface.addressEntries()) {
                QHostAddress address = entry.ip();
                QHostAddress subnet = entry.netmask();

                // IPv4 주소이고 로컬호스트가 아닌 경우
                if(address.protocol() == QAbstractSocket::IPv4Protocol &&
                    !address.isLoopback()) {

                    // 서버 IP와 같은 서브넷인지 확인
                    if((address.toIPv4Address() & subnet.toIPv4Address()) ==
                        (serverAddr.toIPv4Address() & subnet.toIPv4Address())) {
                        return address.toString();
                    }
                }
            }
        }
    }
    return QString();
}

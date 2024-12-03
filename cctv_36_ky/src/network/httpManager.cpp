#include "httpManager.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkInterface>
#include <QNetworkAddressEntry>

HttpManager::HttpManager(QObject* parent)
    : QObject(parent)
    , accessManager(nullptr)
    , jsonManager(nullptr)
    , logManager(nullptr)
    , dbManager(nullptr)
{ }

HttpManager::~HttpManager()
{ }

QUrl HttpManager::set_config(const QString& url, const QString& port) {
    QString fullURL = url;
    if(!url.startsWith("http://") && !url.startsWith("https://")) {
        fullURL = "http://" + url;
    }
    QUrl serverURL = QUrl(fullURL);

    if(port.toInt() > 0) {
        serverURL.setPort(port.toInt());
    } else {
        qDebug() << "ERROR(HM): set_config >> Wrong Port Number";
    }

    if(!serverURL.isValid()) {
        qDebug() << "ERROR(HM): set_config >> Invalid URL Format: " << serverURL;
        return QUrl();
    } else {
        qDebug() << "DONE(HM): set_config >> set_config(network) Successful(URL): " << serverURL;
        return serverURL;
    }
}

bool HttpManager::post_initInfo(const QUrl& url, const ClientInfo& clientInfo) {
    accessManager = new QNetworkAccessManager(this);
    /*
    // SSL/TLS 설정 (HTTPS 사용시)
    QNetworkRequest request(serverURL);
    QSslConfiguration config = QSslConfiguration::defaultConfiguration();
    config.setProtocol(QSsl::TlsV1_2);
    request.setSslConfiguration(config); */

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    jsonManager = new JsonManager(this);
    QByteArray jsonArray = jsonManager->build_init(clientInfo);
    connect(accessManager, &QNetworkAccessManager::finished, this, &HttpManager::reply_finished);
    qDebug() << "DONE(HM): post_initInfo";

    accessManager->post(request, jsonArray);
    return true;
}

void HttpManager::post_userInfo(const QUrl& url, const BasicInfo& basicInfo) {
    jsonManager = new JsonManager(this);
    accessManager = new QNetworkAccessManager(this);

    QByteArray jsonArray = jsonManager->build_info(basicInfo);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    qDebug() << "DONE(HM): post_userInfo";

    accessManager->post(request, jsonArray);
}

void HttpManager::post_clipInfo(const QUrl& url, const TimeInfo& timeInfo) {
    jsonManager = new JsonManager(this);
    accessManager = new QNetworkAccessManager(this);

    QByteArray jsonArray = jsonManager->build_clip(timeInfo);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    qDebug() << "DONE(HM): post_clipInfo";

    accessManager->post(request, jsonArray);
}


void HttpManager::handle_response(QByteArray& jsonArray) {
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonArray);
    QJsonObject jsonObj = jsonDoc.object();

    jsonManager = new JsonManager(this);

    switch(jsonManager->parse_status(jsonArray)) {
        case 1: //response: All clear
        {
            //Parse Response(data)
            TimeInfo timeInfo;
            timeInfo = jsonManager->parse_data(jsonArray);
            qDebug() << "DONE(HM): Parse Response to TimeInfo";

            //Parse Response(image)
            QByteArray decoded;
            jsonManager->decode_base64(jsonArray);

            //Save data(TimeInfo + Image(blob)) to DB
            logManager = new LogManager(this);
            logManager->save_dataImage(timeInfo, decoded);
            qDebug() << "DONE(HM): Save TimeInfo to DB";

            //Save Plate Image file(.jpeg)
            dbManager = new DBManager(this);
            dbManager->save_jpeg(decoded);
            qDebug() << "DONE(HM): Save Image File(.jpeg)";
            break;
        }
        case 2: //for other response objectives
            qDebug() << "ERROR(HM): cannot parse response due to status(wrong code)";
        case 3: //for other response objectives
            qDebug() << "ERROR(HM): cannot parse response due to status(wrong message)";
        case 4:
            qDebug() << "ERROR(HM): cannot parse response due to status(wrong response)";
    }
}

void HttpManager::reply_finished(QNetworkReply* reply) {
    if(reply->error()) {
        qDebug() << "ERROR(HM): Connection Error: " << reply->errorString();
    } else {
        qDebug() << "DONE(HM): Connection Successful";
    }
    reply->deleteLater();
}

QString HttpManager::getLocalIPInSameSubnet(const QUrl& serverURL) {
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

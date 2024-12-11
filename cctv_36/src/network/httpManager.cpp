#include "httpManager.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>
#include <QNetworkInterface>
#include <QNetworkAddressEntry>
#include <QDateTime>

HttpManager::HttpManager(QObject* parent)
    : QObject(parent)
    , accessManager(nullptr)
    , jsonManager(nullptr)
    , logManager(nullptr)
    , dbManager(nullptr)
{

}

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

void HttpManager::post_initInfo(const QUrl& url, const ClientInfo& clientInfo) {
    jsonManager = new JsonManager(this);
    accessManager = new QNetworkAccessManager(this);

    QByteArray jsonArray = jsonManager->build_init(clientInfo);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Connection", "close");

    accessManager->post(request, jsonArray);
    qDebug() << "DONE(HM): post_clipInfo";
}

void HttpManager::post_userInfo(const QUrl& url, const BasicInfo& basicInfo) {
    jsonManager = new JsonManager(this);
    accessManager = new QNetworkAccessManager(this);

    QByteArray jsonArray = jsonManager->build_user(basicInfo);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Connection", "close");

    QNetworkReply *reply = accessManager->post(request, jsonArray);
    connect(reply, &QNetworkReply::finished, this, [=]() {
        if(reply->error() == QNetworkReply::NoError) {
            emit requestCompleted();
            qDebug() << "HttpManager: post userInfo Completed";
            qDebug() << "[HttpManager][INFO ] Response: >> " << reply->readAll();
        } else {
            emit requestFailed();
            qDebug() << "HttpManager: post userInfo Failed";
            qDebug() << "[HttpManager][ERROR] Response: " << reply->errorString();
        }
    });
}

void HttpManager::post_clipInfo(const QUrl& url, const TimeInfo& timeInfo) {
    jsonManager = new JsonManager(this);
    accessManager = new QNetworkAccessManager(this);

    QByteArray jsonArray = jsonManager->build_clip(timeInfo);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Connection", "close");

    accessManager->post(request, jsonArray);
    qDebug() << "DONE(HM): post_clipInfo";
}

void HttpManager::post_plateData(const QUrl& url) {
    m_url = url;
    jsonManager = new JsonManager(this);
    accessManager = new QNetworkAccessManager(this);

    QByteArray jsonArray = jsonManager->build_plate();
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Connection", "keep-alive");

    QNetworkReply *reply = accessManager->post(request, jsonArray);
    qDebug() << "DONE(HM): get_plateData";

    connect(reply, &QNetworkReply::finished, this, [=]() {
        on_replyFinished(reply);
    });
}

void HttpManager::on_replyFinished(QNetworkReply *reply) {
    if(reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        qDebug() << "Received plate data:" << responseData;

        // JSON 파싱하여 상태 확인
        QJsonDocument doc = QJsonDocument::fromJson(responseData);
        QJsonObject obj = doc.object();

        // 실제 데이터가 있을 때만 처리
        if (obj.contains("data")) {
            // 데이터 처리 로직
            qDebug() << "########################3";
            post_plateData(m_url);
        }
    }
    reply->deleteLater();
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
        DBManager::instance().save_jpeg(decoded);
        qDebug() << "DONE(HM): Save Image File(.jpeg)";
        break;
    }
    case 2:
        qDebug() << "ERROR(HM): cannot parse response due to status(wrong code)";
        break;
    case 3:
        qDebug() << "ERROR(HM): cannot parse response due to status(wrong message)";
        break;
    case 4:
        qDebug() << "ERROR(HM): cannot parse response due to status(wrong response)";
        break;
    }
}

ClientInfo HttpManager::set_ClientInfo(const QUrl& serverURL) {
    QDateTime currentTime = QDateTime::currentDateTime();

    ClientInfo clientInfo;
    clientInfo.set_name("ThirtySix");
    clientInfo.set_ipAddr(getLocalIPInSameSubnet(serverURL));
    clientInfo.set_connectTime(currentTime);

    return clientInfo;
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

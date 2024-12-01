#include "networkManager.h"
#include <QJsonDocument>
#include <QJsonObject>

NetworkManager::NetworkManager(QObject* parent)
    : QObject(parent)
    , accessManager(nullptr)
    , jsonManager(nullptr)
{ }

NetworkManager::~NetworkManager()
{ }

void NetworkManager::post_userInfo(const QUrl& url, const BasicInfo& basicInfo) {
    jsonManager = new JSONManager(this);
    accessManager = new QNetworkAccessManager(this);

    QByteArray jsonArray = jsonManager->send_request_info(basicInfo);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    accessManager->post(request, jsonArray);
}

void NetworkManager::post_videoClip(const QUrl& url, const TimeInfo& timeInfo) {
    jsonManager = new JSONManager(this);
    accessManager = new QNetworkAccessManager(this);

    QByteArray jsonArray = jsonManager->send_request_clip(timeInfo);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    accessManager->post(request, jsonArray);
}

bool NetworkManager::check_response(QByteArray& jsonArray) {
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonArray);
    QJsonObject jsonObj = jsonDoc.object();

    jsonManager = new JSONManager(this);
    QString status = jsonManager->check_response(jsonArray);
    if(status == "success") {
        qDebug() << "DONE(NM): message status: " << status;
        return true;
    } else {
        qDebug() << "DONE(NM): message status: " << status;
        return false;
    }
}

TimeInfo NetworkManager::handle_response(QByteArray& jsonArray) {
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonArray);
    QJsonObject jsonObj = jsonDoc.object();

    if(check_response(jsonArray)) {
        TimeInfo timeInfo;
        jsonManager = new JSONManager(this);
        timeInfo = jsonManager->parse_response(jsonArray);

        return timeInfo;
    } else {
        qDebug() << "ERROR(NM): cannot parse response due to status";
        return TimeInfo();  //return empty TimeInfo
    }
}

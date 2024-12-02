#include "jsonManager.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QDateTime>

JSONManager::JSONManager(QObject* parent)
    : QObject(parent)
{ }

JSONManager::~JSONManager()
{ }

QByteArray JSONManager::send_request_init(const ClientInfo& clientInfo) {
    QString name = clientInfo.get_name();
    QString ipAddr = clientInfo.get_ipAddr();
    QDateTime connectTime = clientInfo.get_connectTime();

    QJsonObject requestType;
    requestType["reqType"] = "init";
    QJsonObject clientInfoJson;
    clientInfoJson["cliName"] = name;
    clientInfoJson["ipAddr"] = ipAddr;
    clientInfoJson["connectTime"] = connectTime.toString();

    QJsonObject json;
    json["requestType"] = requestType;
    json["clientInfo"] = clientInfoJson;

    QJsonDocument jsonDoc(json);
    QByteArray jsonArray = jsonDoc.toJson();

    return jsonArray;
}


QByteArray JSONManager::send_request_info(const BasicInfo& basicInfo) {
    QString name = basicInfo.get_name();
    QString plate = basicInfo.get_plate();
    QString home = basicInfo.get_home();
    QString phone = basicInfo.get_phone();

    QJsonObject requestType;
    requestType["reqType"] = "user";
    QJsonObject basicInfoJson;
    basicInfoJson["name"] = name;
    basicInfoJson["plate"] = plate;
    basicInfoJson["home"] = home;
    basicInfoJson["phone"] = phone;

    QJsonObject json;
    json["requestType"] = requestType;
    json["basicInfo"] = basicInfoJson;

    QJsonDocument jsonDoc(json);
    QByteArray jsonArray = jsonDoc.toJson();

    return jsonArray;
}


QByteArray JSONManager::send_request_clip(const TimeInfo& timeInfo) {
    QString plate = timeInfo.get_plate();
    QDateTime time = timeInfo.get_time();
    QString type = timeInfo.get_type();

    QJsonObject requestType;
    requestType["reqType"] = "clip";
    QJsonObject timeInfoJson;
    timeInfoJson["plate"] = plate;
    timeInfoJson["time"] = time.toString();
    timeInfoJson["type"] = type;

    QJsonObject json;
    json["requestType"] = requestType;
    json["timeInfo"] = timeInfoJson;

    QJsonDocument jsonDoc(json);
    QByteArray jsonArray = jsonDoc.toJson();

    return jsonArray;
}

int JSONManager::check_response(const QByteArray& jsonArray) {
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonArray);
    QJsonObject jsonObj = jsonDoc.object();

    QString status;
    if(jsonObj.contains("status") && jsonObj["status"].isString()) {
        status = jsonObj["status"].toString();
        qDebug() << "DONE(JM): received correct status from server: " << status;
        if(status == "success") {
            qDebug() << "DONE(NM): message status: " << status;
            return 1;
        } else {
            qDebug() << "DONE(NM): message status: " << status;
            return 2;
        }
    } else {
        qWarning() << "ERROR(JM): received wrong status from server: " << status;
        return 3;
    }
}

TimeInfo JSONManager::parse_response(const QByteArray& jsonArray) {
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonArray);
    QJsonObject jsonObj = jsonDoc.object();

    QJsonObject timeInfoJson = jsonObj["timeInfo"].toObject();
    QString plate = timeInfoJson["plate"].toString();
    QString timeStr = timeInfoJson["time"].toString();
    QDateTime time = QDateTime::fromString(timeStr, Qt::TextDate);
    QString type = timeInfoJson["type"].toString();

    TimeInfo timeInfo;
    timeInfo.set_plate(plate);
    timeInfo.set_time(time);
    timeInfo.set_type(type);

    return timeInfo;
}

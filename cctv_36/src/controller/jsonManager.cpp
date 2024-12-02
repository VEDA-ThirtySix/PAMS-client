#include "jsonManager.h"
#include <QJsonObject>
#include <QJsonDocument>

JSONManager::JSONManager(QObject* parent)
    : QObject(parent)
{ }

JSONManager::~JSONManager()
{ }

QByteArray JSONManager::send_request_info(const BasicInfo& basicInfo) {
    QString name = basicInfo.get_name();
    QString plate = basicInfo.get_plate();
    QString home = basicInfo.get_home();
    QString phone = basicInfo.get_phone();

    QJsonObject basicInfoJson;
    basicInfoJson["name"] = name;
    basicInfoJson["plate"] = plate;
    basicInfoJson["home"] = home;
    basicInfoJson["phone"] = phone;

    QJsonObject json;
    json["basicInfo"] = basicInfoJson;

    QJsonDocument jsonDoc(json);
    QByteArray jsonArray = jsonDoc.toJson();

    return jsonArray;
}

QByteArray JSONManager::send_request_clip(const TimeInfo& timeInfo) {
    QString plate = timeInfo.get_plate();
    QDateTime time = timeInfo.get_time();
    QString type = timeInfo.get_type();

    QJsonObject timeInfoJson;
    timeInfoJson["plate"] = plate;
    timeInfoJson["time"] = time.toString();
    timeInfoJson["type"] = type;

    QJsonObject json;
    json["timeInfo"] = timeInfoJson;

    QJsonDocument jsonDoc(json);
    QByteArray jsonArray = jsonDoc.toJson();

    return jsonArray;
}

QString JSONManager::check_response(const QByteArray& jsonArray) {
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonArray);
    QJsonObject jsonObj = jsonDoc.object();

    QString status;
    if(jsonObj.contains("status") && jsonObj["status"].isString()) {
        status = jsonObj["status"].toString();
        qDebug() << "DONE(JM): received correct status from server: " << status;
    } else {
        qWarning() << "ERROR(JM): received wrong status from server: " << status;
    }

    return status;
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

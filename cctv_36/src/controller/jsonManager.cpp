#include "jsonManager.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QDateTime>

JsonManager::JsonManager(QObject* parent)
    : QObject(parent)
{ }

JsonManager::~JsonManager()
{ }

QByteArray JsonManager::build_init(const ClientInfo& clientInfo) {
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


QByteArray JsonManager::build_user(const BasicInfo& basicInfo) {
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


QByteArray JsonManager::build_clip(const TimeInfo& timeInfo) {
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

int JsonManager::parse_status(const QByteArray& jsonArray) {
    QJsonDocument request_doc = QJsonDocument::fromJson(jsonArray);
    QJsonObject request_obj = request_doc.object();

    QString code, message;
    bool codeValid = false;
    bool messageValid = false;

    if(request_obj.contains("status") && request_obj["status"].isObject()) {
        QJsonObject status_obj = request_obj["status"].toObject();
        //STATUS(code)
        if(status_obj.contains("code") && status_obj["code"].isString()) {
            code = status_obj["code"].toString();
            qDebug() << "DONE(JM): parse_status(code)";
            if(code == "success") {
                qDebug() << "DONE(JM): code: " << code;
                codeValid = true;
            } else {
                qDebug() << "DONE(JM): code: " << code;
                codeValid = false;
            }
        } else {
            qWarning() << "ERROR(JM): parse_status : Received Wrong Status(code)";
            codeValid = false;
        }

        //STATUS(message)
        if(status_obj.contains("message") && status_obj["message"].isString()) {
            message = status_obj["message"].toString();
            qDebug() << "DONE(JM): parse_status(message)";
            messageValid = true;
        } else {
            qWarning() << "ERROR(JM): parse_status : Received Wrong Status(message)";
            messageValid = false;
        }

    } else {
        qWarning() << "ERROR(JM): parse_status : Received Wrong Object";
        codeValid = false;
        messageValid = false;
    }

    if(codeValid && messageValid) {
        return 1;
    } else if(!codeValid && messageValid) {
        return 2;
    } else if(codeValid && !messageValid) {
        return 3;
    } else {
        return 4;
    }
}


TimeInfo JsonManager::parse_data(const QByteArray& jsonArray) {
    QJsonDocument request_doc = QJsonDocument::fromJson(jsonArray);
    QJsonObject request_obj = request_doc.object();

    QString plate, time, type;
    if(request_obj.contains("data") && request_obj["data"].isObject()) {
        QJsonObject data_obj = request_obj["data"].toObject();
        //DATA(plate)
        if(data_obj.contains("plate") && data_obj["plate"].isString()) {
            plate = data_obj["plate"].toString();
            qDebug() << "DONE(JM): plate : " << plate;
        } else {
            qWarning() << "ERROR(JM): parse_data - plate";
        }

        //DATA(time)
        if(data_obj.contains("time") && data_obj["time"].isString()) {
            time = data_obj["time"].toString();
            qDebug() << "DONE(JM): time : " << time;
        } else {
            qWarning() << "ERROR(JM) : parse_data - time";
        }

        //DATA(type)
        if(data_obj.contains("type") && data_obj["type"].isString()) {
            type = data_obj["type"].toString();
            qDebug() << "DONE(JM): type : " << type;
        } else {
            qWarning() << "ERROR(JM) : parse_data - type";
        }
    } else {
        qWarning() << "ERROR(JM) : parse_data - object";
    }

    QDateTime dateTime = QDateTime::fromString(time, "yyyy-MM-dd HH:mm:ss");
    qDebug() << "DONE(JM): time(QString to QDateTime) conversion : " << dateTime;

    TimeInfo timeInfo;
    timeInfo.set_plate(plate);
    timeInfo.set_time(dateTime);
    timeInfo.set_type(type);

    return timeInfo;
}

QByteArray JsonManager::decode_base64(const QByteArray& jsonArray) {
    QJsonDocument request_doc = QJsonDocument::fromJson(jsonArray);
    QJsonObject request_obj = request_doc.object();

    QByteArray encodedArray;
    if(request_obj.contains("image") && request_obj["image"].isObject()) {
        QJsonObject image_obj = request_obj["image"].toObject();
        //IMAGE(base64)
        if(image_obj.contains("base64") && image_obj["base64"].isString()) {
            encodedArray = image_obj["base64"].toString().toUtf8();
            qDebug() << "DONE(JM): base64 : " << encodedArray;
        } else {
            qWarning() << "ERROR(JM): parse_image - base64";
        }
    }

    QByteArray base64Array = encodedArray;
    QByteArray base64Data = base64Array.split(',').last();
    QByteArray imageArray = QByteArray::fromBase64(base64Data);

    return imageArray;
}

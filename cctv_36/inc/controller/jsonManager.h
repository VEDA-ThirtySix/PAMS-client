#ifndef JSONMANAGER_H
#define JSONMANAGER_H

#include "metadata.h"
#include <QObject>
#include <QByteArray>

/* JSON: HTTP BODY */

class JsonManager : public QObject {
    Q_OBJECT

public:
    JsonManager(QObject *parent);
    ~JsonManager();

    QByteArray build_init(const ClientInfo& clientInfo);
    QByteArray build_user(const BasicInfo& basicInfo);
    QByteArray build_clip(const TimeInfo& timeInfo);
    QByteArray build_plate();

    int parse_status(const QByteArray& jsonArray);
    TimeInfo parse_data(const QByteArray& jsonArray);
    QByteArray decode_base64(const QByteArray& jsonArray);
    bool saveImageFromByteArray(const QByteArray& imageData, const QString& filename);
};

#endif // JSONMANAGER_H

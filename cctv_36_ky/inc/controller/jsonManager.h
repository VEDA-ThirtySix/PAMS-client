#ifndef JSONMANAGER_H
#define JSONMANAGER_H

#include "metadata.h"
#include <QObject>
#include <QByteArray>

/* JSON: HTTP BODY */

class JSONManager : public QObject {
Q_OBJECT

public:
    JSONManager(QObject *parent);
    ~JSONManager();

    QByteArray build_init(const ClientInfo& clientInfo);
    QByteArray build_info(const BasicInfo& basicInfo);
    QByteArray build_clip(const TimeInfo& timeInfo);

    int check_response(const QByteArray& jsonArray);
    int parse_response(const QByteArray& jsonArray);    //[Core] parse status, return status code
    TimeInfo* parse_data(const QByteArray& dataArray);
    void decode_base64(const QByteArray& imageArray);
};

#endif // JSONMANAGER_H

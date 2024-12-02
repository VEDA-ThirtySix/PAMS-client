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

    QByteArray send_request_init(const ClientInfo& clientInfo);
    QByteArray send_request_info(const BasicInfo& basicInfo);
    QByteArray send_request_clip(const TimeInfo& timeInfo);

    int check_response(const QByteArray& jsonArray);
    TimeInfo parse_response(const QByteArray& jsonArray);
};

#endif // JSONMANAGER_H

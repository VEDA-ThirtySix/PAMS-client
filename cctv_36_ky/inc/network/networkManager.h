#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

//#include <QSslSocket>
#include "metadata.h"
#include "jsonManager.h"
#include "logManager.h"
#include <QObject>
#include <QUrl>
#include <QByteArray>
#include <QNetworkAccessManager>

class NetworkManager : public QObject {
    Q_OBJECT

public:
    NetworkManager(QObject *parent);
    ~NetworkManager();

    void post_userInfo(const QUrl& url, const BasicInfo& basicInfo);
    void post_videoClip(const QUrl& url, const TimeInfo& timeInfo);

    bool check_response(QByteArray& jsonArray);
    void handle_response(QByteArray& jsonArray);

private:
    QNetworkAccessManager *accessManager;
    JSONManager *jsonManager;
    LogManager *logManager;

};
#endif // NETWORKMANAGER_H

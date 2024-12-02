#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

//#include <QSslSocket>
#include "metadata.h"
#include "jsonManager.h"
#include "logManager.h"
#include <QObject>
#include <QByteArray>
#include <QNetworkAccessManager>

//BASE64 BESTSHOT
class NetworkManager : public QObject {
    Q_OBJECT

public:
    NetworkManager(QObject *parent);
    ~NetworkManager();

    QUrl set_config(const QString& url, const QString& port);
    bool set_connection(const QUrl& url, const ClientInfo& clientInfo);
    //void post_initInfo(const QUrl& url, const ClientInfo& clientInfo);
    void post_userInfo(const QUrl& url, const BasicInfo& basicInfo);
    void post_clipInfo(const QUrl& url, const TimeInfo& timeInfo);

    //void get_data(const QUrl& url);
    void handle_response(QByteArray& jsonArray);
    void reply_finished(QNetworkReply *reply);
    QString getLocalIPInSameSubnet(const QUrl& serverURL);

private:
    QNetworkAccessManager *accessManager;
    JSONManager *jsonManager;
    LogManager *logManager;

};
#endif // NETWORKMANAGER_H

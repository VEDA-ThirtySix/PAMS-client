#ifndef HTTPMANAGER_H
#define HTTPMANAGER_H

//#include <QSslSocket>
#include "metadata.h"
#include "jsonManager.h"
#include "logManager.h"
#include "dbManager.h"
#include <QObject>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QUrl>

//BASE64 BESTSHOT
class HttpManager : public QObject {
    Q_OBJECT

public:
    HttpManager(QObject *parent);
    ~HttpManager();

    QUrl set_config(const QString& url, const QString& port);
    bool post_initInfo(const QUrl& url, const ClientInfo& clientInfo);
    //void post_initInfo(const QUrl& url, const ClientInfo& clientInfo);
    void post_userInfo(const QUrl& url, const BasicInfo& basicInfo);
    void post_clipInfo(const QUrl& url, const TimeInfo& timeInfo);

    //void get_data(const QUrl& url);
    void handle_response(QByteArray& jsonArray);    //plateData: data(metadata), image(base64)
    ClientInfo set_ClientInfo(const QUrl& serverURL);
    QString getLocalIPInSameSubnet(const QUrl& serverURL);

private:
    QNetworkAccessManager *accessManager;
    JsonManager *jsonManager;
    LogManager *logManager;
    DBManager *dbManager;

};
#endif // HTTPMANAGER_H

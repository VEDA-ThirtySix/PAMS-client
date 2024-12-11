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
#include <QNetworkReply>
#include <QUrl>

//BASE64 BESTSHOT
class HttpManager : public QObject {
    Q_OBJECT

public:
    HttpManager(QObject *parent);
    ~HttpManager();

    QUrl set_config(const QString& url, const QString& port);
    void post_initInfo(const QUrl& url, const ClientInfo& clientInfo);
    void post_userInfo(const QUrl& url, const BasicInfo& basicInfo);
    void post_clipInfo(const QUrl& url, const TimeInfo& timeInfo);
    void post_plateData(const QUrl& url);

    //void get_data(const QUrl& url);
    void handle_response(QByteArray& jsonArray);    //plateData: data(metadata), image(base64)
    ClientInfo set_ClientInfo(const QUrl& serverURL);
    QString getLocalIPInSameSubnet(const QUrl& serverURL);

private:
    QNetworkAccessManager *accessManager;
    JsonManager *jsonManager;
    LogManager *logManager;
    DBManager *dbManager;
    QUrl m_url;

private slots:
    void on_replyFinished(QNetworkReply *reply);

signals:
    void requestCompleted();  // 요청 완료 시그널
    void requestFailed();     // 요청 실패 시그널
    void on_readyRead();
};
#endif // HTTPMANAGER_H

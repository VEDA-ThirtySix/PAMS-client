#ifndef PARSEJSON_H
#define PARSEJSON_H

#include <QObject>
#include <QByteArray>

class JSONManager : public QObject {
Q_OBJECT

public:
    JSONManager(QObject *parent);
    ~JSONManager();

    QByteArray sendRequest();

    QString parseResponse(const QByteArray& jsonArray);
};

#endif // PARSEJSON_H

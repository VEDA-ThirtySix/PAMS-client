#ifndef LOGMANAGER_H
#define LOGMANAGER_H

#include "metadata.h"
#include "dbManager.h"
#include <QObject>
#include <QByteArray>

class LogManager : public QObject {
    Q_OBJECT

public:
    LogManager(QObject *parent);
    ~LogManager();

    void save_dataImage(const TimeInfo& timeInfo, const QByteArray& imageArray);

private:
    DBManager *dbManager;
};

#endif // LOGMANAGER_H

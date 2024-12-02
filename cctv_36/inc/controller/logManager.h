#ifndef LOGMANAGER_H
#define LOGMANAGER_H

#include "metadata.h"
#include "dbManager.h"
#include <QObject>

class LogManager : public QObject {
    Q_OBJECT

public:
    LogManager(QObject *parent);
    ~LogManager();

    void save_log(const TimeInfo& timeInfo);

private:
    DBManager *dbManager;
};

#endif // LOGMANAGER_H

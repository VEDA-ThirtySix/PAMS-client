#include "logManager.h"

LogManager::LogManager(QObject *parent)
    : QObject(parent)
    , dbManager(nullptr)
{ }

LogManager::~LogManager()
{ }

void LogManager::save_log(const TimeInfo& timeInfo) {
    dbManager = new DBManager;

    dbManager->create_timeInfo(timeInfo);
    qDebug() << "DONE(LM): Create TimeInfo(plate): " << timeInfo.get_plate();
}

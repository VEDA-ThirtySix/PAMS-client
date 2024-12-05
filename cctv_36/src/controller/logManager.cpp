#include "logManager.h"
#include <QDebug>

LogManager::LogManager(QObject *parent)
    : QObject(parent)
    , dbManager(nullptr)
{ }

LogManager::~LogManager()
{ }

void LogManager::save_dataImage(const TimeInfo& timeInfo, const QByteArray& imageArray) {
    dbManager = new DBManager;

    dbManager->create_timeInfo(timeInfo, imageArray);
    qDebug() << "DONE(LM): Create TimeInfo(plate): " << timeInfo.get_plate();
}

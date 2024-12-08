#include "logManager.h"
#include <QDebug>

LogManager::LogManager(QObject *parent)
    : QObject(parent)
{ }

LogManager::~LogManager()
{ }

void LogManager::save_dataImage(const TimeInfo& timeInfo, const QByteArray& imageArray) {
    DBManager::instance().create_timeInfo(timeInfo, imageArray);
    qDebug() << "DONE(LM): Create TimeInfo(plate): " << timeInfo.get_plate();
}

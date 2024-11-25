#include "metadata.h"

BasicInfo::BasicInfo(const QString& plateNumber,
                     const QString& home,
                     const QString& phone)
    : m_plateNumber(plateNumber), m_home(home), m_phone(phone) { }


QString BasicInfo::get_plateNumber() const {
    return m_plateNumber;
}
QString BasicInfo::get_home() const {
    return m_home;
}
QString BasicInfo::get_phone() const {
    return m_phone;
}


void BasicInfo::set_plateNumber(const QString& plateNumber) {
    m_plateNumber = plateNumber;
}
void BasicInfo::set_home(const QString& home) {
    m_home = home;
}
void BasicInfo::set_phone(const QString& phone) {
    m_phone = phone;
}

/* ====================================================== */

TimeInfo::TimeInfo(const QDateTime& timeStamp,
                   const QString& eventType,
                   const QString& plateNumber)
    : m_timeStamp(timeStamp), m_eventType(eventType), m_plateNumber(plateNumber) { }

QDateTime TimeInfo::get_timeStamp() const {
    return m_timeStamp;
}
QString TimeInfo::get_eventType() const {
    return m_eventType;
}
QString TimeInfo::get_plateNumber() const {
    return m_plateNumber;
}

void TimeInfo::set_timeStamp(const QDateTime& timeStamp) {
    m_timeStamp = timeStamp;
}
void TimeInfo::set_eventType(const QString& eventType) {
    m_eventType = eventType;
}
void TimeInfo::set_plateNumber(const QString& plateNumber) {
    m_plateNumber = plateNumber;
}


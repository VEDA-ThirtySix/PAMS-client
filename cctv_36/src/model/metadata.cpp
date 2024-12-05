#include "metadata.h"

ClientInfo::ClientInfo()
    : m_name(QString()), m_ipAddr(QString()), m_connectTime(QDateTime())
{ }

ClientInfo::ClientInfo(const QString& name,
                       const QString& ipAddr,
                       const QDateTime& connectTime)
    : m_name(name), m_ipAddr(ipAddr), m_connectTime(connectTime)
{ }

QString ClientInfo::get_name() const {
    return m_name;
}
QString ClientInfo::get_ipAddr() const {
    return m_ipAddr;
}
QDateTime ClientInfo::get_connectTime() const {
    return m_connectTime;
}

void ClientInfo::set_name(const QString& name) {
    m_name = name;
}
void ClientInfo::set_ipAddr(const QString& ipAddr) {
    m_ipAddr = ipAddr;
}
void ClientInfo::set_connectTime(const QDateTime& connectTime) {
    m_connectTime = connectTime;
}

/* ====================================================== */

BasicInfo::BasicInfo()
    : m_name(QString()), m_plate(QString()), m_home(QString()),
    m_phone(QString()) {}

BasicInfo::BasicInfo(const QString& name,
                     const QString& plate,
                     const QString& home,
                     const QString& phone)
    : m_name(name), m_plate(plate), m_home(home), m_phone(phone)
{ }

QString BasicInfo::get_name() const {
    return m_name;
}
QString BasicInfo::get_plate() const {
    return m_plate;
}
QString BasicInfo::get_home() const {
    return m_home;
}
QString BasicInfo::get_phone() const {
    return m_phone;
}

void BasicInfo::set_name(const QString& name) {
    m_name = name;
}
void BasicInfo::set_plate(const QString& plate) {
    m_plate = plate;
}
void BasicInfo::set_home(const QString& home) {
    m_home = home;
}
void BasicInfo::set_phone(const QString& phone) {
    m_phone = phone;
}

/* ====================================================== */

TimeInfo::TimeInfo()
    : m_plate(QString()), m_time(QDateTime()), m_type(QString()) { }

TimeInfo::TimeInfo(const QString& plate,
                   const QDateTime& time,
                   const QString& type)
    : m_plate(plate), m_time(time), m_type(type) { }

QString TimeInfo::get_plate() const {
    return m_plate;
}
QDateTime TimeInfo::get_time() const {
    return m_time;
}
QString TimeInfo::get_type() const {
    return m_type;
}

void TimeInfo::set_plate(const QString& plate) {
    m_plate = plate;
}
void TimeInfo::set_time(const QDateTime& time) {
    m_time = time;
}
void TimeInfo::set_type(const QString& type) {
    m_type = type;
}


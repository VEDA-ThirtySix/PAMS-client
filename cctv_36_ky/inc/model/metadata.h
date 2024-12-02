#ifndef METADATA_H
#define METADATA_H

#include <QString>
#include <QDateTime>

/**
 *@ brief: 메타데이터 멤버변수 선언
 *@ type : 번호판(license), 동호수(home), 연락처(phone), 입출차시간
 *@ Note :
 *@ Author: KIYUN
 **/

class ClientInfo {
public:
    ClientInfo();
    explicit ClientInfo(const QString& name,
                        const QString& ipAddr,
                        const QDateTime& connectTime);

    QString get_name() const;
    QString get_ipAddr() const;
    QDateTime get_connectTime() const;

    void set_name(const QString& name);
    void set_ipAddr(const QString& ipAddr);
    void set_connectTime(const QDateTime& connectTime);

private:
    QString m_name;
    QString m_ipAddr;
    QDateTime m_connectTime;
};


class BasicInfo {
public:
    BasicInfo();
    explicit BasicInfo(const QString& name,
                       const QString& plate,
                       const QString& home,
                       const QString& phone);
    QString get_name()  const;
    QString get_plate() const;
    QString get_home()  const;
    QString get_phone() const;

    void set_name(const QString& name);
    void set_plate(const QString& plate);
    void set_home(const QString& home);
    void set_phone(const QString& phone);

private:
    QString m_name;
    QString m_plate;
    QString m_home;
    QString m_phone;
};


class TimeInfo {
public:
    TimeInfo();
    explicit TimeInfo(const QString& plate,
                      const QDateTime& time,
                      const QString& type);

    QString get_plate()  const;
    QDateTime get_time() const;
    QString get_type()   const;

    void set_plate(const QString& plate);
    void set_time(const QDateTime& time);
    void set_type(const QString& type);

private:
    QString m_plate;
    QDateTime m_time;
    QString m_type;
};

#endif // METADATA_H


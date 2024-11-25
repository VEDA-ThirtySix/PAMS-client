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
\
class BasicInfo {
public:
    explicit BasicInfo(const QString& plateNumber,
              const QString& home,
              const QString& phone);

    QString get_plateNumber() const;
    QString get_home() const;
    QString get_phone() const;

    void set_plateNumber(const QString& plateNumber);
    void set_home(const QString& home);
    void set_phone(const QString& phone);

private:
    QString m_plateNumber;
    QString m_home;
    QString m_phone;
};

class TimeInfo {
public:
    explicit TimeInfo(const QDateTime& timeStamp,
             const QString& eventType,
             const QString& plateNumber);

    QDateTime get_timeStamp() const;
    QString get_eventType() const;
    QString get_plateNumber() const;

    void set_timeStamp(const QDateTime& timeStamp);
    void set_eventType(const QString& eventType);
    void set_plateNumber(const QString& plateNumber);

private:
    QDateTime m_timeStamp;
    QString m_eventType;
    QString m_plateNumber;
};

#endif // METADATA_H

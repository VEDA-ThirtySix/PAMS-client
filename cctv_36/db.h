#ifndef DB_H
#define DB_H

/**
 *@ brief: 메타데이터 멤버변수 선언
 *@ type : 번호판(license), 동호수(home), 연락처(phone), 입출차시간
 *@ Note :
 *@ Author: KIYUN
 **/

#include "metadata.h"
#include <QObject>
#include <QtSql/QSqlDatabase>
#include <QMap>
#include <QList>

class DB : public QObject {
public:
    explicit DB(QObject *parent = nullptr);
    ~DB();

    bool open_database();
    void close_database();

    /* CRUD: BasicInfo */
    void create_basicInfo(const QString& plateNumber,
                            const QString& phone,
                            const QString& address);
    QString read_basicInfo(int num);
    QMap<QString, BasicInfo> readAll_basicInfo(int num, const QString& value);
    void update_basicInfo(int num, QString& value);
    void delete_basicInfo(const QString& plateNumber);

    /* CRUD: TimeInfo */
    /*
     * 등록_입차시간
     * 등록_출차시간
     * 조회_입차시간/출차시간(int command) ...
     * 타임스탬프 정렬 -> 시그널-슬롯 연결
     */

private:
    QMap<QString, BasicInfo> basicInfo;
    QList<TimeInfo> timeInfo;
};

#endif // DB_H

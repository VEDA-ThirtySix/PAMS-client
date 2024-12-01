#ifndef DB_H
#define DB_H

/** *********************************************************
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
    void create_basicInfo(const BasicInfo& basicInfo);
    BasicInfo read_basicInfo(int num, const QString& value);
    void update_basicInfo(const QString& plate, int num, const QString& value);
    void delete_basicInfo(const QString& plate);

    /* CRUD: TimeInfo */
    void create_timeInfo(const TimeInfo& timeInfo);
    TimeInfo read_timeInfo(int num, const QString& value);
    TimeInfo read_timeInfo_duaration(int num, const QDateTime& timeFrom, const QDateTime& timeTo);
    //void delete_timeInfo(const QString& plate);

    /* QList<TimeInfo> timeInfoList */
    //void addNewTimeInfo(const TimeInfo& newTimeInfo);

    /*
     * 등록_입차시간
     * 등록_출차시간
     * 조회_입차시간/출차시간(int command) ...
     * 타임스탬프 정렬 -> 시그널-슬롯 연결
     */

private:
    QSqlDatabase db;
    BasicInfo basicInfo;
    TimeInfo timeInfo;
    QList<TimeInfo> timeInfoList;
};

#endif // DB_H

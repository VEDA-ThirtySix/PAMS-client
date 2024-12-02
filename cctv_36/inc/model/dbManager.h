#ifndef DBMANAGER_H
#define DBMANAGER_H

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

class DBManager : public QObject {
public:
    explicit DBManager(QObject *parent = nullptr);
    ~DBManager();

    bool open_database();
    QSqlDatabase getDatabase() const;
    void close_database();

    /* CRUD: BasicInfo */
    void create_basicInfo(const BasicInfo& new_basicInfo);
    BasicInfo read_basicInfo(const QString& selected_plate);
    void update_basicInfo(const BasicInfo& editted_basicInfo);
    void delete_basicInfo(const QString& selected_plate);

    /* CRUD: TimeInfo */
    void create_timeInfo(const TimeInfo& timeInfo);
    TimeInfo read_timeInfo(const QString& selected_plate);
    qint64 get_duration(const QDateTime& from, const QDateTime& to);
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

#endif // DBMANAGER_H

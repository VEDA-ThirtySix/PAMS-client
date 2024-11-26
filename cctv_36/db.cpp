#include "db.h"
#include <QDebug>
#include <QtSql/QSqlQuery>

DB::DB(QObject* parent)
    : QObject(parent)
    , db(QSqlDatabase::addDatabase("QSQLITE"))
{
    open_database();
}

DB::~DB() { }

bool DB::open_database() {
    db.setDatabaseName("metadata.db");
    if(!db.open()) {
        qDebug() << "Error(DB): cannot open 'metadata.db'";
    }

    QSqlQuery query(db);

    /* 테이블(Basic) 생성 */
    if(!query.exec("CREATE TABLE IF NOT EXISTS Basic ("
                   "plate VARCHAR PRIMARY KEY,"
                   "home VARCHAR,"
                   "phone VARCHAR)")) {
        qDebug() << "ERROR(DB): Failed to open the table(Basic)";
    }

    /* 테이블(Time) 생성 */
    if(!query.exec("CREATE TABLE IF NOT EXISTS Time ("
                   "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                   "plate VARCHAR,"
                   "time DATETIME,"
                   "type VARCHAR,"
                   "FOREIGN KEY (plate) REFERENCES Basic(plate)"
                   "ON DELETE CASCADE)")) {
        qDebug() << "ERROR(DB): Failed to open the table(Time)";
    }

    qDebug() << "DONE(DB): open_database";
    return true;
}

void DB::close_database() {
    if(db.isOpen()) {
        db.close();
        qDebug() << "DONE(DB): close 'metadata.db";
    }
}


/* CRUD: BASIC_INFO */
void DB::create_basicInfo(const BasicInfo& basicInfo) {
    QSqlQuery query(db);
    query.prepare("INSERT INTO Basic (plate, home, phone) VALUES (:plate, :home, :phone)");
    query.bindValue(":plate", basicInfo.get_plate());
    query.bindValue(":home",  basicInfo.get_home());
    query.bindValue(":phone", basicInfo.get_phone());
    query.exec();
    qDebug() << "DONE(DB): Created basicInfo data(" << basicInfo.get_plate() << ")";
}

BasicInfo DB::read_basicInfo(int num, const QString& value) {
    QSqlQuery query(db);

    switch(num) {
    case 1:
        query.prepare("SELECT * FROM Basic WHERE plate = :value");
        query.bindValue(":value", value);

        basicInfo.set_plate(query.value(0).toString());
        basicInfo.set_home(query.value(1).toString());
        basicInfo.set_phone(query.value(2).toString());
        return basicInfo;
        qDebug() << "DONE(DB): Read basicInfo data with plate";

    case 2:
        query.prepare("SELECT * FROM Basic WHERE home = :value");
        query.bindValue(":value", value);

        basicInfo.set_plate(query.value(0).toString());
        basicInfo.set_home(query.value(1).toString());
        basicInfo.set_phone(query.value(2).toString());
        return basicInfo;
        qDebug() << "DONE(DB): Read basicInfo data with home";

    case 3:
        query.prepare("SELECT * FROM Basic WHERE phone = :value");
        query.bindValue(":value", value);

        basicInfo.set_plate(query.value(0).toString());
        basicInfo.set_home(query.value(1).toString());
        basicInfo.set_phone(query.value(2).toString());
        return basicInfo;
        qDebug() << "DONE(DB): Read basicInfo data with phone";
    }
    return BasicInfo();
    qDebug() << "WARNING(DB): read_BasicInfo > Wrong Argument input (plate:1, home:2, phone:3)";
}

void DB::update_basicInfo(const QString& plate, int num, const QString& value) {
    QSqlQuery query(db);
    switch(num) {
    case 1: //update home
        query.prepare("UPDATE Basic SET home = :value WHERE plate = :plate");
        break;
    case 2: //update phone
        query.prepare("UPDATE Basic SET phone = :value WHERE plate = :plate");
        break;
    }
    query.bindValue(":value", value);
    query.bindValue(":plate", plate);
    query.exec();
    qDebug() << "DONE(DB): Updated basicInfo data(" << plate << ")";
}

void DB::delete_basicInfo(const QString& plate) {
    QSqlQuery query(db);
    query.prepare("DELETE FROM Basic WHERE plate = :plate");
    query.bindValue(":plate", plate);
    query.exec();
    qDebug() << "DONE(DB): Deleted basicInfo data(" << plate << ")";
}



/* CRUD: TIME_INFO */
void DB::create_timeInfo(const TimeInfo& timeInfo) {
    QSqlQuery query(db);
    query.prepare("INSERT INTO Time (plate, time, type) VALUES (:plate, :time, :type)");
    query.bindValue(":plate", timeInfo.get_plate());
    query.bindValue(":time",  timeInfo.get_time());
    query.bindValue(":type",  timeInfo.get_type());
    query.exec();
    qDebug() << "DONE(DB): Created timeInfo data(" << timeInfo.get_plate() << ")";
}

TimeInfo DB::read_timeInfo(int num, const QString& value) {
    QSqlQuery query(db);

    switch(num) {
    case 1:
        query.prepare("SELECT * FROM Time WHERE plate = :value");
        query.bindValue(":value", value);

        timeInfo.set_plate(query.value(0).toString());
        timeInfo.set_time(query.value(1).toDateTime());
        timeInfo.set_type(query.value(2).toString());
        return timeInfo;
        qDebug() << "DONE(DB): Read timeInfo data with plate";

    case 2:
        query.prepare("SELECT * FROM Time WHERE type = :value");
        query.bindValue(":value", value);

        timeInfo.set_plate(query.value(0).toString());
        timeInfo.set_time(query.value(1).toDateTime());
        timeInfo.set_type(query.value(2).toString());
        return timeInfo;
        qDebug() << "DONE(DB): Read timeInfo data with type";
    }
    return TimeInfo();
    qDebug() << "WARNING(DB): read_timeInfo > Wrong Argument input (plate:1, type:2)";
}

TimeInfo DB::read_timeInfo_duaration(int num, const QDateTime& timeFrom, const QDateTime& timeTo) {
    QSqlQuery query(db);

}




/*
void DB::addNewTimeInfo(const TimeInfo& newTimeInfo) {
    timeInfoList.append(newTimeInfo);
}*/

/**
    *@ ********************************************************************
    *@ Name           : readAll_basicInfo                                       *
    *@ Description    : Receives input event from camera SDK              *
    *@ Arguments      : eventIn[IN]: Input event type                     *
    *@                : pData[IN]  : Data for input event                 *
    *@ Return Value   :                                              *
    *@ Notes          :                                                   *
    *@ Change History :                                                   *
    *@ ********************************************************************/

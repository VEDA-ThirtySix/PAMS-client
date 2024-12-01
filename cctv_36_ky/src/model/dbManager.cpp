#include "dbManager.h"
#include <QDebug>
#include <QtSql/QSqlQuery>

DBManager::DBManager(QObject* parent)
    : QObject(parent)
    , db(QSqlDatabase::addDatabase("QSQLITE"))
{
    open_database();
}

QSqlDatabase DBManager::getDatabase() const {
    return db;
}

DBManager::~DBManager()
{

}

bool DBManager::open_database() {
    db.setDatabaseName("metadata.db");
    if(!db.open()) {
        qDebug() << "Error(DB): cannot open 'metadata.db'";
        return false;
    }

    QSqlQuery query(db);

    /* 테이블(Basic) 생성 */
    if(!query.exec("CREATE TABLE IF NOT EXISTS Basic ("
                   "name VARCHAR PRIMARY KEY,"
                   "plate VARCHAR,"
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

    qDebug() << "DONE(DM): open_database";
    return true;
}

void DBManager::close_database() {
    if(db.isOpen()) {
        db.close();
        qDebug() << "DONE(DM): close 'metadata.db";
    }
}


/* CRUD: BASIC_INFO */
void DBManager::create_basicInfo(const BasicInfo& basicInfo) {
    QSqlQuery query(db);

    query.prepare("INSERT INTO Basic (name, plate, home, phone) VALUES (:name, :plate, :home, :phone)");
    query.bindValue(":name", basicInfo.get_name());
    query.bindValue(":plate", basicInfo.get_plate());
    query.bindValue(":home",  basicInfo.get_home());
    query.bindValue(":phone", basicInfo.get_phone());
    if(!query.exec()) {
        qDebug() << "ERROR(DB): create_basicInfo";
    }
    qDebug() << "DONE(DM): Created basicInfo data(" << basicInfo.get_plate() << ")";
}

BasicInfo DBManager::read_basicInfo(const QString& selected_plate) {
    QSqlQuery query(db);

    query.prepare("SELECT * FROM Basic WHERE plate = :selected_plate");
    query.bindValue("selected_plate", selected_plate);
    if(!query.exec() || !query.next()) {
        qDebug() << "Error(DB): read_basicInfo";
        return BasicInfo();
    }

    basicInfo.set_name(query.value(0).toString());
    basicInfo.set_plate(query.value(1).toString());
    basicInfo.set_home(query.value(2).toString());
    basicInfo.set_phone(query.value(3).toString());
    qDebug() << "DONE(DM): Read BasicInfo(plate):" << selected_plate;
    return basicInfo;
}


void DBManager::update_basicInfo(const BasicInfo& editted_basicInfo) {
    QSqlQuery query(db);

    query.prepare("UPDATE Basic SET name = :name, plate = :plate, home = :home, phone = :phone WHERE plate = :plate");
    query.bindValue(":name", editted_basicInfo.get_name());
    query.bindValue(":plate", editted_basicInfo.get_plate());
    query.bindValue(":home",  editted_basicInfo.get_home());
    query.bindValue(":phone", editted_basicInfo.get_phone());

    if (query.exec()) {
        qDebug() << "DONE(DM): Updated basicInfo data for plate:" << editted_basicInfo.get_plate();
    } else {
        qDebug() << "ERROR(DB): Failed to update basicInfo data:";
    }
}

/*
BasicInfo DBManager::read_basicInfo(int num, const QString& value) {
    QSqlQuery query(db);

    switch(num) {
    case 1:
        query.prepare("SELECT * FROM Basic WHERE plate = :value");
        query.bindValue(":value", value);
        query.exec();

        basicInfo.set_name(query.value(0).toString());
        basicInfo.set_plate(query.value(1).toString());
        basicInfo.set_home(query.value(2).toString());
        basicInfo.set_phone(query.value(3).toString());
        return basicInfo;
        qDebug() << "DONE(DM): Read basicInfo data with plate";

    case 2:
        query.prepare("SELECT * FROM Basic WHERE home = :value");
        query.bindValue(":value", value);

        basicInfo.set_name(query.value(0).toString());
        basicInfo.set_plate(query.value(1).toString());
        basicInfo.set_home(query.value(2).toString());
        basicInfo.set_phone(query.value(3).toString());
        return basicInfo;
        qDebug() << "DONE(DM): Read basicInfo data with home";

    case 3:
        query.prepare("SELECT * FROM Basic WHERE phone = :value");
        query.bindValue(":value", value);

        basicInfo.set_name(query.value(0).toString());
        basicInfo.set_plate(query.value(1).toString());
        basicInfo.set_home(query.value(2).toString());
        basicInfo.set_phone(query.value(3).toString());
        return basicInfo;
        qDebug() << "DONE(DM): Read basicInfo data with phone";
    }

    qDebug() << "WARNING(DB): read_BasicInfo > Wrong Argument input (plate:1, home:2, phone:3)";
    return BasicInfo();
}
*/


/*
void DBManager::update_basicInfo(const QString& plate, int num, const QString& value) {
    QSqlQuery query(db);

    switch(num) {
    case 1: //update name
        query.prepare("UPDATE Basic SET name= :value WHERE plate = :plate");
        break;
    case 2: //update home
        query.prepare("UPDATE Basic SET home = :value WHERE plate = :plate");
        break;
    case 3: //update phone
        query.prepare("UPDATE Basic SET phone = :value WHERE plate = :plate");
        break;
    }
    query.bindValue(":value", value);
    query.bindValue(":plate", plate);
    query.exec();
    qDebug() << "DONE(DM): Updated basicInfo data(" << plate << ")";
}

void DBManager::delete_basicInfo(const QString& plate) {
    QSqlQuery query(db);
    query.prepare("DELETE FROM Basic WHERE plate = :plate");
    query.bindValue(":plate", plate);
    query.exec();
    qDebug() << "DONE(DM): Deleted basicInfo data(" << plate << ")";
}
*/


/* CRUD: TIME_INFO */
void DBManager::create_timeInfo(const TimeInfo& timeInfo) {
    QSqlQuery query(db);
    query.prepare("INSERT INTO Time (plate, time, type) VALUES (:plate, :time, :type)");
    query.bindValue(":plate", timeInfo.get_plate());
    query.bindValue(":time",  timeInfo.get_time());
    query.bindValue(":type",  timeInfo.get_type());
    query.exec();
    qDebug() << "DONE(DM): Created timeInfo data(" << timeInfo.get_plate() << ")";
}

TimeInfo DBManager::read_timeInfo(int num, const QString& value) {
    QSqlQuery query(db);

    switch(num) {
    case 1:
        query.prepare("SELECT * FROM Time WHERE plate = :value");
        query.bindValue(":value", value);

        timeInfo.set_plate(query.value(0).toString());
        timeInfo.set_time(query.value(1).toDateTime());
        timeInfo.set_type(query.value(2).toString());
        return timeInfo;
        qDebug() << "DONE(DM): Read timeInfo data with plate";

    case 2:
        query.prepare("SELECT * FROM Time WHERE type = :value");
        query.bindValue(":value", value);

        timeInfo.set_plate(query.value(0).toString());
        timeInfo.set_time(query.value(1).toDateTime());
        timeInfo.set_type(query.value(2).toString());
        return timeInfo;
        qDebug() << "DONE(DM): Read timeInfo data with type";
    }
    return TimeInfo();
    qDebug() << "WARNING(DB): read_timeInfo > Wrong Argument input (plate:1, type:2)";
}






/*
void DBManager::addNewTimeInfo(const TimeInfo& newTimeInfo) {
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

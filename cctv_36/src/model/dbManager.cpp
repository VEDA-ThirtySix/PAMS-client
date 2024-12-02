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
    //db.setDatabaseName("metadata.db");
    db.setDatabaseName("/Users/taewonkim/GitHub/RaspberryPi-5-RTSP-Client/cctv_36/build/Qt_6_7_2_for_macOS-Debug/metadata.db"); // for MacOS
    if(!db.open()) {
        qDebug() << "Error(DB): Failed to Open Database: " << db;
        return false;
    }

    QSqlQuery query(db);

    /* 테이블(Basic) 생성 */
    if(!query.exec("CREATE TABLE IF NOT EXISTS Basic ("
                   "name VARCHAR PRIMARY KEY,"
                   "plate VARCHAR,"
                   "home VARCHAR,"
                   "phone VARCHAR)")) {
        qDebug() << "ERROR(DB): Failed to Open Table: Basic";
    }

    /* 테이블(Time) 생성 */
    if(!query.exec("CREATE TABLE IF NOT EXISTS Time ("
                   "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                   "plate VARCHAR,"
                   "time DATETIME,"
                   "type VARCHAR,"
                   "FOREIGN KEY (plate) REFERENCES Basic(plate)"
                   "ON DELETE CASCADE)")) {
        qDebug() << "ERROR(DB): Failed to Open table: Time";
    }

    qDebug() << "DONE(DM): Open Database" << db;
    return true;
}

void DBManager::close_database() {
    if(db.isOpen()) {
        db.close();
        qDebug() << "DONE(DM): Close Database: " << db;
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
        qDebug() << "ERROR(DB): Failed to Create BasicInfo";
    } else {
        qDebug() << "DONE(DM): Create BasicInfo(plate): " << basicInfo.get_plate();
    }
}

BasicInfo DBManager::read_basicInfo(const QString& selected_plate) {
    QSqlQuery query(db);

    query.prepare("SELECT * FROM Basic WHERE plate = :selected_plate");
    query.bindValue(":selected_plate", selected_plate);
    if(!query.exec() || !query.next()) {
        qDebug() << "Error(DB): Failed to Read BasicInfo";
        return BasicInfo();
    } else {
        basicInfo.set_name(query.value(0).toString());
        basicInfo.set_plate(query.value(1).toString());
        basicInfo.set_home(query.value(2).toString());
        basicInfo.set_phone(query.value(3).toString());
        qDebug() << "DONE(DM): Read BasicInfo(plate): " << selected_plate;
        return basicInfo;
    }
}


void DBManager::update_basicInfo(const BasicInfo& editted_basicInfo) {
    QSqlQuery query(db);

    query.prepare("UPDATE Basic SET name = :name, plate = :plate, home = :home, phone = :phone WHERE plate = :plate");
    query.bindValue(":name", editted_basicInfo.get_name());
    query.bindValue(":plate", editted_basicInfo.get_plate());
    query.bindValue(":home",  editted_basicInfo.get_home());
    query.bindValue(":phone", editted_basicInfo.get_phone());

    if(!query.exec()) {
        qDebug() << "ERROR(DB): Failed to Update BasicInfo";
    } else {
        qDebug() << "DONE(DM): Update BasicInfo(plate): " << editted_basicInfo.get_plate();
    }
}

void DBManager::delete_basicInfo(const QString& selected_plate) {
    QSqlQuery query(db);

    query.prepare("DELETE FROM Basic WHERE plate = :plate");
    query.bindValue(":plate", selected_plate);

    if(!query.exec()) {
        qDebug() << "ERROR(DM): Failed to Delete BasicInfo";
    } else {
        qDebug() << "DONE(DM): Delete BasicInfo(plate): " << selected_plate;
    }
}




/* CRUD: TIME_INFO */
void DBManager::create_timeInfo(const TimeInfo& timeInfo) {
    QSqlQuery query(db);
    query.prepare("INSERT INTO Time (plate, time, type) VALUES (:plate, :time, :type)");
    query.bindValue(":plate", timeInfo.get_plate());
    query.bindValue(":time",  timeInfo.get_time());
    query.bindValue(":type",  timeInfo.get_type());
    if(!query.exec()) {
        qDebug() << "ERROR(DM): Failed to Create TimeInfo";
    } else {
        qDebug() << "DONE(DM): Create TimeInfo(plate): " << timeInfo.get_plate() << ")";
    }
}

TimeInfo DBManager::read_timeInfo(const QString& selected_plate) {
    QSqlQuery query(db);

    query.prepare("SELECT * FROM Time WHERE plate = :selected_plate");
    query.bindValue(":selected_plate", selected_plate);
    if(!query.exec() || !query.next()) {
        qDebug() << "Error(DB): Failed to Read TimeInfso";
        return TimeInfo();
    } else {
        timeInfo.set_plate(query.value(0).toString());
        timeInfo.set_time(query.value(1).toDateTime());
        timeInfo.set_type(query.value(2).toString());
        qDebug() << "DONE(DM): Read BasicInfo(plate): " << selected_plate;
        return timeInfo;
    }
}

qint64 DBManager::get_duration(const QDateTime& from, const QDateTime& to) {
    qint64 seconds = from.secsTo(to);

    return seconds;
    /*
     * minutes = seconds / 60;
     * hours = seconds / 3600;
     * days = seconds / (3600 * 24);
     */
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

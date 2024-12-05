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
    //db.setDatabaseName("/Users/taewonkim/GitHub/RaspberryPi-5-RTSP-Client/cctv_36/build/Qt_6_7_2_for_macOS-Debug/metadata.db"); // for MacOS
    db.setDatabaseName(getDatabasePath());

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

QString DBManager::getDatabasePath() const {
    QString dbName = "metadata.db";

#ifdef Q_OS_MAC

    QDir dir(QCoreApplication::applicationDirPath()); // 현재 실행 파일 위치 (*.app/Contents/MacOS)

    dir.cdUp(); // MacOS -> Contents
    dir.cdUp(); // Contents -> cctv_36.app
    dir.cdUp(); // cctv_36.app -> Debug
    dir.cdUp(); // Debug -> build
    dir.cdUp(); // build -> cctv_36

    QString dbPath = dir.path() + QDir::separator() + dbName;

    // qDebug() << "Project Root:" << dir.path();
    // qDebug() << "Database Path:" << dbPath;

#else // Windows/Linux의 경우
    QString dbPath = QCoreApplication::applicationDirPath() + QDir::separator() + dbName;
#endif

    QDir dbDir(QFileInfo(dbPath).path());
    if (!dbDir.exists()) {
        dbDir.mkpath(".");
        qDebug() << "Created directory:" << dbDir.path();
    }

    return dbPath;
}

// void DBManager::insertExampleTimeData() {
//     QSqlQuery query(db);

//     // 현재 시간을 기준으로 예제 데이터 생성
//     QDateTime currentTime = QDateTime::currentDateTime();
//     QDateTime earlierTime = currentTime.addSecs(-3600); // 1시간 전

//     // 첫 번째 예제 데이터: 입차
//     TimeInfo timeInfo1("12가3456", earlierTime, "입차");
//     create_timeInfo(timeInfo1);

//     // 두 번째 예제 데이터: 출차
//     TimeInfo timeInfo2("12가3456", currentTime, "출차");
//     create_timeInfo(timeInfo2);
// }


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

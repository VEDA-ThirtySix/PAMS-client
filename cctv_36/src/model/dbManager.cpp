#include "dbManager.h"
#include <QDebug>
#include <QtSql/QSqlQuery>
#include <QFile>
#include <QDir>
#include <QCoreApplication>
#include <QSqlError>

DBManager& DBManager::instance() { // db 싱글톤 패턴 적용
    static DBManager instance;
    return instance;
}

DBManager::DBManager(QObject* parent)
    : QObject(parent)
    , db(QSqlDatabase::addDatabase("QSQLITE"))
{
    // if (open_database()) {
    //     qDebug() << "ERROR(DB): Failed to open database";
    // }
}

QSqlDatabase DBManager::getDatabase() const {
    return db;
}

DBManager::~DBManager()
{
    if (db.isOpen()){
        close_database();
    }
}

bool DBManager::open_database() {

    db.setDatabaseName(getDatabasePath());

    if(!db.open()) {
        qDebug() << "Error(DB): Failed to Open Database: " << db;
        return false;
    }

    QSqlQuery query(db);

    /* 테이블(Basic) 생성 */
    if(!query.exec("CREATE TABLE IF NOT EXISTS Basic ("
                    "name VARCHAR,"
                    "plate VARCHAR PRIMARY KEY,"
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
                    "image BLOB,"
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

    query.prepare("SELECT name, plate, home, phone FROM Basic WHERE plate = :selected_plate");
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

        qDebug() << "DONE(DM): Read BasicInfo - Name:" << basicInfo.get_name()
                 << "Plate:" << basicInfo.get_plate()
                 << "Home:" << basicInfo.get_home()
                 << "Phone:" << basicInfo.get_phone();

        return basicInfo;
    }
}


void DBManager::update_basicInfo(const BasicInfo& editted_basicInfo) {
    QSqlQuery query(db);

    // 트랜잭션 시작
    db.transaction();

    query.prepare("UPDATE Basic SET name = :name, home = :home, phone = :phone WHERE plate = :plate");
    query.bindValue(":name", editted_basicInfo.get_name());
    query.bindValue(":home", editted_basicInfo.get_home());
    query.bindValue(":phone", editted_basicInfo.get_phone());
    query.bindValue(":plate", editted_basicInfo.get_plate());

    if(!query.exec()) {
        qDebug() << "ERROR(DB): Failed to Update BasicInfo";
        qDebug() << "Error details:" << query.lastError().text();
        qDebug() << "Query was:" << query.lastQuery();
        qDebug() << "Values -"
                 << "Name:" << editted_basicInfo.get_name()
                 << "Plate:" << editted_basicInfo.get_plate()
                 << "Home:" << editted_basicInfo.get_home()
                 << "Phone:" << editted_basicInfo.get_phone();
        db.rollback();  // 실패시 롤백
        return;
    }

    if(query.numRowsAffected() == 0) {
        qDebug() << "ERROR(DB): No rows were updated";
        db.rollback();
        return;
    }

    // 성공적으로 완료되면 커밋
    if(db.commit()) {
        qDebug() << "DONE(DM): Update BasicInfo for plate:" << editted_basicInfo.get_plate();
    } else {
        qDebug() << "ERROR(DB): Failed to commit transaction:" << db.lastError().text();
        db.rollback();
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
void DBManager::create_timeInfo(const TimeInfo& timeInfo, const QByteArray& imageArray) {
    QSqlQuery query(db);
    query.prepare("INSERT INTO Time (plate, time, type, image) VALUES (:plate, :time, :type, :image)");
    query.bindValue(":plate", timeInfo.get_plate());
    query.bindValue(":time",  timeInfo.get_time());
    query.bindValue(":type",  timeInfo.get_type());
    query.bindValue(":image",  imageArray);

    if(!query.exec()) {
        qDebug() << "ERROR(DM): Failed to Create TimeInfo";
    } else {
        qDebug() << "DONE(DM): Create TimeInfo(plate): " << timeInfo.get_plate() << ")";
    }
}

TimeInfo DBManager::read_timeInfo(const QString& selected_plate) {
    QSqlQuery query(db);
    query.prepare("SELECT plate, time, type FROM Time WHERE plate = :selected_plate");
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

QByteArray DBManager::read_image(const QString& selected_plate) {
    QSqlQuery query(db);
    query.prepare("SELECT image FROM Time WHERE plate = :selected_plate");
    query.bindValue(":selected_plate", selected_plate);

    if(!query.exec() || !query.next()) {
        qDebug() << "Error(DB): Failed to Read TimeInfso";
        return QByteArray();
    } else {
        qDebug() << "DONE(DM): Read BasicInfo(plate): " << selected_plate;
        return query.value(0).toByteArray();
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

/* Save Plate Image */
void DBManager::save_jpeg(const QByteArray& imageArray) {
    QFile file("plate.jpg");
    if(file.open(QIODevice::WriteOnly)) {
        file.write(imageArray);
        file.close();
        qDebug() << "DONE(DM): Saved JPEG file(plate.jpg) Successfully";
    } else {
        qDebug() << "ERROR(DM): Saving JPEG file(plate.jpg) Failed";
    }
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


bool DBManager::find_plate(const QString& plate) {
    QSqlQuery query;

    query.prepare("SELECT EXISTS ("
                  "SELECT 1 FROM Time WHERE plate = :plate"
                  ")");
    query.bindValue(":plate", plate);

    if (!query.exec()) {
        qDebug() << "Error(DB): Query execution failed:" << query.lastError().text();
        return false;
    }

    if (query.next()) {
        return query.value(0).toBool();
    }

    return false;
}

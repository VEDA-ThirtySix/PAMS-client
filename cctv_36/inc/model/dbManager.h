#ifndef DBMANAGER_H
#define DBMANAGER_H

#include "metadata.h"
#include <QObject>
#include <QtSql/QSqlDatabase>
#include <QMap>
#include <QList>
#include <QImage>
#include <QByteArray>

class DBManager : public QObject {
public:
    static DBManager& instance(); // db 싱글톤 패턴 적용

    bool open_database();
    void close_database();

    QSqlDatabase getDatabase() const;

    /* CRUD: BasicInfo */
    void create_basicInfo(const BasicInfo& new_basicInfo);
    BasicInfo read_basicInfo(const QString& selected_plate);
    void update_basicInfo(const BasicInfo& editted_basicInfo);
    void delete_basicInfo(const QString& selected_plate);

    /* CRUD: TimeInfo */
    void create_timeInfo(const TimeInfo& timeInfo, const QByteArray& imageArray);
    TimeInfo read_timeInfo(const QString& selected_plate);
    QByteArray read_image(const QString& selected_plate);
    qint64 get_duration(const QDateTime& from, const QDateTime& to);
    bool find_plate(const QString& plate);

    /* Save Plate Image */
    void save_jpeg(const QByteArray& imageArray);

    QString getDatabasePath() const;

private:
    explicit DBManager(QObject *parent = nullptr); // 싱글톤을 위한 private 생성자
    ~DBManager();

    // 복사 방지
    DBManager(const DBManager&) = delete;
    DBManager& operator=(const DBManager&) = delete;

    QSqlDatabase db;
    BasicInfo basicInfo;
    TimeInfo timeInfo;
    QList<TimeInfo> timeInfoList;
};

#endif // DBMANAGER_H

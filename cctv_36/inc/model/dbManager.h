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
    void create_timeInfo(const TimeInfo& timeInfo, const QByteArray& imageArray);
    TimeInfo read_timeInfo(const QString& selected_plate);
    QByteArray read_image(const QString& selected_plate);
    qint64 get_duration(const QDateTime& from, const QDateTime& to);

    /* Save Plate Image */
    void save_jpeg(const QByteArray& imageArray);

private:
    QSqlDatabase db;
    BasicInfo basicInfo;
    TimeInfo timeInfo;
    QList<TimeInfo> timeInfoList;
};

#endif // DBMANAGER_H

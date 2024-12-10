#ifndef USERMANAGER_H
#define USERMANAGER_H

#include "metadata.h"
#include "dbManager.h"
#include <QObject>

class UserManager : public QObject {
    Q_OBJECT

public:
    explicit UserManager(QObject *parent = nullptr);
    ~UserManager();

    /* controller */
    bool initiallize();
    QSqlDatabase getDatabase() const;
    BasicInfo getCurrentInfo(QString& selected_plate) const;
    bool enrollUser(const BasicInfo& basicInfo);
    bool editUser(const BasicInfo& editted_basicInfo);
    bool deleteUser(const QString& plate);

private:
    DBManager* dbManager;

};

#endif // USERMANAGER_H

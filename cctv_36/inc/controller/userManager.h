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
    bool editUser(BasicInfo& basicInfo);
    //bool deleteUser(int type, const QString& value);

private:
    DBManager* dbManager;

};

#endif // USERMANAGER_H

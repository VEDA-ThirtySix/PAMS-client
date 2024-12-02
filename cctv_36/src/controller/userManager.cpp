#include "userManager.h"
#include <QDebug>

UserManager::UserManager(QObject *parent)
    : QObject(parent)
    , dbManager(new DBManager(this))    //dbdebug
    // dbManager(nullptr)
{ }

UserManager::~UserManager()
{
    delete dbManager;
}

bool UserManager::initiallize() {
    dbManager->open_database();
    qDebug() << "DONE(UM): initialize";
    return true;
}

QSqlDatabase UserManager::getDatabase() const {
    return dbManager->getDatabase();
}

BasicInfo UserManager::getCurrentInfo(QString& selected_plate) const {
    return dbManager->read_basicInfo(selected_plate);
}

bool UserManager::enrollUser(const BasicInfo& basicInfo) {
    dbManager = new DBManager(this);
    dbManager->create_basicInfo(basicInfo);
    qDebug() << "DONE(UM): enrollUser";
    return true;
}

bool UserManager::editUser(BasicInfo& editted_basicInfo) {
    dbManager->update_basicInfo(editted_basicInfo);
    qDebug() << "DONE(UM): editUser";
    return true;
}

/*
bool UserManager::deleteUser(int type, const QString& value) {

}
*/

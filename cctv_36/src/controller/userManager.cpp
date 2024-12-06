#include "userManager.h"
#include <QDebug>

UserManager::UserManager(QObject *parent)
    : QObject(parent)
{

}

UserManager::~UserManager()
{

}

bool UserManager::initiallize() {
    qDebug() << "DONE(UM): initialize";
    return true;
}

QSqlDatabase UserManager::getDatabase() const {
    return DBManager::instance().getDatabase();
}

BasicInfo UserManager::getCurrentInfo(QString& selected_plate) const {
    return DBManager::instance().read_basicInfo(selected_plate);
}

bool UserManager::enrollUser(const BasicInfo& basicInfo) {
    DBManager::instance().create_basicInfo(basicInfo);
    qDebug() << "DONE(UM): enrollUser";
    return true;
}

bool UserManager::editUser(BasicInfo& editted_basicInfo) {
    DBManager::instance().update_basicInfo(editted_basicInfo);
    qDebug() << "DONE(UM): editUser";
    return true;
}
/*
bool UserManager::deleteUser(int type, const QString& value) {

}
*/

#include "mainwindow.h"
#include "dbManager.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    if (!DBManager::instance().open_database()){
        qDebug() << "Failed to connect database";
        return -1;
    }
    MainWindow w;
    qDebug() << "hello";
    w.show();
    return a.exec();
}

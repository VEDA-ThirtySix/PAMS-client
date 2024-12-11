#include "mainwindow.h"
#include "dbManager.h"
#include "tcpManager.h"
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

    TcpManager& tcpManager = TcpManager::instance();
    tcpManager.connectToServer("192.168.68.120", 8088);

    w.show();
    return a.exec();
}

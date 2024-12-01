#include "mainwindow.h"
#include "sslSocket.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SSLClient client;
    client.connectToServer("localhost", 8443);

    MainWindow w;
    w.show();
    return a.exec();
}

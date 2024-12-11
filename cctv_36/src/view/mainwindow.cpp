#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "streaming.h"
#include "search.h"
#include "metadata.h"
#include "httpManager.h"

#include <QDateTime>
#include <QMessageBox>
#include <QImage>
#include <QTabBar>

#include <QMenu>
#include <QAction>
#include <QFileDialog>
#include <QIcon>
#include <QMenuBar>
#include <QVBoxLayout>
#include <QDebug>
#include <QNetworkInterface>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Streaming *streaming = new Streaming(this);
    Search *search = new Search(this);

    ui->tabWidget->addTab(streaming, "Streaming");
    ui->tabWidget->addTab(search, "Search");

    connect(ui->pushButton_apply, &QPushButton::clicked, this, [this, streaming, search]() {
        QString host = ui->lineEdit_host->text();
        m_host = host;

        streaming->get_host(host);
        streaming->rtsp_setting();
        search->get_host(host);
        search->build_QUrl();
        send_initData();
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::send_initData() {
    QString name = "cctv_36";
    QString ipAddr = getWiFiIPAddress();
    QDateTime currentTime = QDateTime::currentDateTime();
    ClientInfo clientInfo(name, ipAddr, currentTime);
    qDebug() << "MainWindow$    ipAddr   : " << ipAddr;
    qDebug() << "MainWindow$ current Time: " << currentTime.toString();

    QUrl url;
    url.setHost(m_host);
    url.setPort(8080);
    url.setScheme("http");
    qDebug() << "MainWindow$ m_url = " << url.toString();

    HttpManager* httpManager = new HttpManager(this);
    httpManager->post_initInfo(url, clientInfo);
    qDebug() << "MainWindow$ send_initData Successful";
}

QString MainWindow::getWiFiIPAddress() {
    QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();

    for (const QNetworkInterface &interface : interfaces) {
        for (const QNetworkAddressEntry &entry : interface.addressEntries()) {
            if (entry.ip().protocol() == QAbstractSocket::IPv4Protocol &&
                entry.ip().toString().startsWith("192.168.")) {
                return entry.ip().toString();
            }
        }
    }

    return QString(); // Wi-Fi 연결된 IP 주소를 찾지 못한 경우 빈 문자열 반환
}


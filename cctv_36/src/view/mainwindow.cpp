#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "streaming.h"
#include "search.h"

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
        // search->build_QUrl();
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}



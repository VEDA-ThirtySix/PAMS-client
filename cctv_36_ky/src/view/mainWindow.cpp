#include "mainWindow.h"
#include "ui_mainwindow.h"
#include "streamingWidget.h"
#include "searchWidget.h"

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
    SearchWidget *search = new SearchWidget(this);

    ui->tabWidget->addTab(streaming, "Streaming");
    ui->tabWidget->addTab(search, "Search");
}

MainWindow::~MainWindow()
{
    delete ui;
}



#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialog_enroll.h"
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
    connect(ui->pushButton_enroll, &QPushButton::clicked, this, &MainWindow::clicked_buttonEnroll);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::clicked_buttonEnroll() {
    EnrollDialog *enrollDialog = new EnrollDialog(this);
    enrollDialog->setAttribute(Qt::WA_DeleteOnClose);
    enrollDialog->exec();   //Enroll Dialog(Modal)
    qDebug() << "DONE(MW): Open Enroll Dialog";
}

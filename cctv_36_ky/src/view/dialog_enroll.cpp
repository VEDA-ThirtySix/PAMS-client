#include "dialog_enroll.h"
#include "ui_enroll.h"
#include "metadata.h"
#include <QDebug>

EnrollDialog::EnrollDialog(QUrl url, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::EnrollDialog)
    , userManager(new UserManager(this))
    , httpManager(new HttpManager(this))
    , m_url(url)
{
    ui->setupUi(this);

    connect(ui->pushButton_prev, &QPushButton::clicked, this, &EnrollDialog::clicked_buttonPrev);
    connect(ui->pushButton_enroll, &QPushButton::clicked, this, &EnrollDialog::clicked_buttonEnroll);
}

EnrollDialog::~EnrollDialog()
{
    delete ui;
}

void EnrollDialog::clicked_buttonEnroll() {
    QString newName = ui->lineEdit_name->text();
    QString newPlate = ui->lineEdit_plate->text();
    QString newHome = ui->lineEdit_home->text();
    QString newPhone = ui->lineEdit_phone->text();
    BasicInfo newBasicInfo(newName, newPlate, newHome, newPhone);

    if(!userManager->enrollUser(newBasicInfo)) {
        qDebug() << "ERROR: Local DB Enroll Failed!";
        return;
    }

    //httpManager = new HttpManager(this);
    connect(httpManager, &HttpManager::requestCompleted, this, [this]() {
        qDebug() << "DONE: HTTP Request Completed";
        emit dataSubmitted();
        this->accept();
    });

    connect(httpManager, &HttpManager::requestFailed, this, [this]() {
        qDebug() << "ERROR: HTTP Request Failed!";
        this->accept();
    });

    httpManager->post_userInfo(m_url, newBasicInfo);
}

void EnrollDialog::clicked_buttonPrev() {
    this->close();
    qDebug() << "Back to MainWindow ...";
}

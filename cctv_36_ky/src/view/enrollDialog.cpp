#include "enrollDialog.h"
#include "ui_enroll.h"
#include "metadata.h"
#include <QDebug>

EnrollDialog::EnrollDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::EnrollDialog)
    , userManager(new UserManager(this))
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
    if(userManager->enrollUser(newBasicInfo)) {
        qDebug() << "DONE: Enroll new Member: " << newName;
        emit dataSubmitted();
        this->accept();
    } else {
        qDebug() << "ERROR: Enroll Failed! ";
    }
}

void EnrollDialog::clicked_buttonPrev() {
    this->close();
    qDebug() << "Back to MainWindow ...";
}

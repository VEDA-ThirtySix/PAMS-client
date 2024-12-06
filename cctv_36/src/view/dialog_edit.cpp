#include "dialog_edit.h"
#include "ui_edit.h"
#include "metadata.h"
#include <QDebug>
#include <QMessageBox>

EditDialog::EditDialog(QString plate, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::EditDialog)
    , userManager(new UserManager(this))
    , m_selectedPlate(plate)
{
    ui->setupUi(this);

    BasicInfo current_info = userManager->getCurrentInfo(m_selectedPlate);

    ui->lineEdit_name->setText(current_info.get_name());
    ui->lineEdit_plate->setText(m_selectedPlate);
    ui->lineEdit_plate->setReadOnly(true);
    ui->lineEdit_home->setText(current_info.get_home());
    ui->lineEdit_phone->setText(current_info.get_phone());


    connect(ui->pushButton_prev, &QPushButton::clicked, this, &EditDialog::clicked_buttonPrev);
    connect(ui->pushButton_edit, &QPushButton::clicked, this, &EditDialog::clicked_buttonEdit);
}


EditDialog::~EditDialog()
{
    delete ui;
}

void EditDialog::clicked_buttonEdit() {
    QString editted_name = ui->lineEdit_name->text();
    QString current_plate = m_selectedPlate;
    QString editted_home = ui->lineEdit_home->text();
    QString editted_phone = ui->lineEdit_phone->text();

    // 빈 필드 체크
    if(editted_name.isEmpty() || current_plate.isEmpty()) {
        QMessageBox::warning(this, "입력 오류", "이름과 차량번호는 필수 입력사항입니다.");
        return;
    }

    BasicInfo editted_BasicInfo(editted_name, current_plate, editted_home, editted_phone);

    if(userManager->editUser(editted_BasicInfo)) {
        emit dataUpdated();
        this->close();
    } else {
        qDebug() << "EditDialog - 오류: 데이터베이스 업데이트 실패";
        QMessageBox::warning(this, "수정 실패", "데이터베이스 업데이트에 실패했습니다.");
    }
}

void EditDialog::clicked_buttonPrev() {
    this->close();
    qDebug() << "Back to MainWindow ...";
}

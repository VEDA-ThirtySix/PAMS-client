#include "dialog_edit.h"
#include "ui_edit.h"
#include "metadata.h"
#include <QDebug>
#include <QMessageBox>

EditDialog::EditDialog(QUrl url, QString plate, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::EditDialog)
    , userManager(new UserManager(this))
    , httpManager(new HttpManager(this))
    , m_url(url)
    , m_selectedPlate(plate)
{
    ui->setupUi(this);

    BasicInfo current_info = userManager->getCurrentInfo(m_selectedPlate);

    ui->lineEdit_plate->setText(m_selectedPlate);
    ui->lineEdit_plate->setReadOnly(true);

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

    if(editted_name.isEmpty() || current_plate.isEmpty()) {
        QMessageBox::warning(this, "입력 오류", "이름과 차량번호는 필수 입력사항입니다.");
        return;
    }

    BasicInfo editted_BasicInfo(editted_name, current_plate, editted_home, editted_phone);

    HttpManager *httpManager = new HttpManager(this);

    // HTTP 요청 완료 시그널 연결
    connect(httpManager, &HttpManager::requestCompleted, this, [this, editted_BasicInfo]() {
        if(userManager->editUser(editted_BasicInfo)) {
            emit dataUpdated();
            this->close();
        } else {
            qDebug() << "EditDialog - 오류: 데이터베이스 업데이트 실패";
            QMessageBox::warning(this, "수정 실패", "데이터베이스 업데이트에 실패했습니다.");
        }
        sender()->deleteLater();  // HttpManager 정리
    });

    // 요청 실패 처리
    connect(httpManager, &HttpManager::requestFailed, this, [this]() {
        QMessageBox::warning(this, "전송 실패", "서버 통신에 실패했습니다.");
        sender()->deleteLater();
    });

    httpManager->post_userInfo(m_url, editted_BasicInfo);
    qDebug() << "EditDialog_clicked_buttonEdit: post_userInfo 요청 시작";
    qDebug() << "Server URL: " << m_url;
}

void EditDialog::clicked_buttonPrev() {
    this->close();
    qDebug() << "Back to MainWindow ...";
}

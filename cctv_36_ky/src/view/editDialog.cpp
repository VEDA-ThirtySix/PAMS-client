#include "editDialog.h"
#include "ui_edit.h"
#include "metadata.h"
#include <QDebug>

EditDialog::EditDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::EditDialog)
    , userManager(new UserManager(this))
{
    ui->setupUi(this);
    connect(ui->pushButton_prev, &QPushButton::clicked, this, &EditDialog::clicked_buttonPrev);
    connect(ui->pushButton_edit, &QPushButton::clicked, this, &EditDialog::clicked_buttonEdit);
}


EditDialog::~EditDialog()
{
    delete ui;
}

void EditDialog::clicked_buttonEdit() {
    searchWidget = new SearchWidget(this);
    QString selected_plate = searchWidget->get_seletedData();

    QString editted_name = ui->lineEdit_name->text();
    QString editted_plate = ui->lineEdit_plate->text();
    QString editted_home = ui->lineEdit_home->text();
    QString editted_phone = ui->lineEdit_phone->text();

    BasicInfo current_info = userManager->getCurrentInfo(selected_plate);

    if(editted_name.isEmpty()) editted_name = current_info.get_name();
    if(editted_plate.isEmpty()) editted_plate = current_info.get_plate();
    if(editted_home.isEmpty()) editted_home = current_info.get_home();
    if(editted_phone.isEmpty()) editted_phone = current_info.get_phone();

    BasicInfo editted_BasicInfo(editted_name, editted_plate, editted_home, editted_phone);

    if(userManager->editUser(editted_BasicInfo)) {
        qDebug() << "DONE: Edit Member: " << editted_name;
        this->accept();
    } else {
        qDebug() << "ERROR: Enroll Failed! ";
    }
}

void EditDialog::clicked_buttonPrev() {
    this->close();
    qDebug() << "Back to MainWindow ...";
}

#include "networkDialog.h"
#include "ui_networkDialog.h"

NetworkDialog::NetworkDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::NetworkDialog)
    , httpManager(nullptr)
{
    ui->setupUi(this);

    connect(ui->pushButton_prev, &QPushButton::clicked, this, &NetworkDialog::clicked_buttonPrev);
    connect(ui->pushButton_apply, &QPushButton::clicked, this, &NetworkDialog::clicked_buttonApply);
}

NetworkDialog::~NetworkDialog()
{
    delete ui;
}

void NetworkDialog::clicked_buttonApply() {
    QString url = ui->lineEdit_url->text();
    QString port = ui->lineEdit_port->text();

    emit configChanged(url, port);
    this->accept();
}

void NetworkDialog::clicked_buttonPrev() {
    this->close();
    qDebug() << "DONE(ND): Back to Streaming Window ...";
}

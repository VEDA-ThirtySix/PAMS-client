/********************************************************************************
** Form generated from reading UI file 'edit.ui'
**
** Created by: Qt User Interface Compiler version 6.7.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EDIT_H
#define UI_EDIT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_EditDialog
{
public:
    QLineEdit *lineEdit_name;
    QPushButton *pushButton_name;
    QLineEdit *lineEdit_plate;
    QLineEdit *lineEdit_home;
    QLineEdit *lineEdit_phone;
    QPushButton *pushButton_plate;
    QPushButton *pushButton_home;
    QPushButton *pushButton_phone;
    QPushButton *pushButton_prev;
    QPushButton *pushButton_edit;
    QLabel *label_title;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_7;
    QLabel *label_8;
    QLabel *label_name;
    QLabel *label_plate;
    QLabel *label_home;
    QLabel *label_phone;

    void setupUi(QDialog *EditDialog)
    {
        if (EditDialog->objectName().isEmpty())
            EditDialog->setObjectName("EditDialog");
        EditDialog->resize(639, 609);
        lineEdit_name = new QLineEdit(EditDialog);
        lineEdit_name->setObjectName("lineEdit_name");
        lineEdit_name->setGeometry(QRect(240, 180, 251, 41));
        pushButton_name = new QPushButton(EditDialog);
        pushButton_name->setObjectName("pushButton_name");
        pushButton_name->setGeometry(QRect(500, 190, 88, 25));
        lineEdit_plate = new QLineEdit(EditDialog);
        lineEdit_plate->setObjectName("lineEdit_plate");
        lineEdit_plate->setGeometry(QRect(240, 230, 251, 41));
        lineEdit_home = new QLineEdit(EditDialog);
        lineEdit_home->setObjectName("lineEdit_home");
        lineEdit_home->setGeometry(QRect(240, 280, 251, 41));
        lineEdit_phone = new QLineEdit(EditDialog);
        lineEdit_phone->setObjectName("lineEdit_phone");
        lineEdit_phone->setGeometry(QRect(240, 330, 251, 41));
        pushButton_plate = new QPushButton(EditDialog);
        pushButton_plate->setObjectName("pushButton_plate");
        pushButton_plate->setGeometry(QRect(500, 240, 88, 25));
        pushButton_home = new QPushButton(EditDialog);
        pushButton_home->setObjectName("pushButton_home");
        pushButton_home->setGeometry(QRect(500, 290, 88, 25));
        pushButton_phone = new QPushButton(EditDialog);
        pushButton_phone->setObjectName("pushButton_phone");
        pushButton_phone->setGeometry(QRect(500, 340, 88, 25));
        pushButton_prev = new QPushButton(EditDialog);
        pushButton_prev->setObjectName("pushButton_prev");
        pushButton_prev->setGeometry(QRect(340, 450, 88, 25));
        pushButton_edit = new QPushButton(EditDialog);
        pushButton_edit->setObjectName("pushButton_edit");
        pushButton_edit->setGeometry(QRect(430, 450, 88, 25));
        label_title = new QLabel(EditDialog);
        label_title->setObjectName("label_title");
        label_title->setGeometry(QRect(280, 70, 81, 61));
        label_5 = new QLabel(EditDialog);
        label_5->setObjectName("label_5");
        label_5->setGeometry(QRect(200, 180, 31, 41));
        label_6 = new QLabel(EditDialog);
        label_6->setObjectName("label_6");
        label_6->setGeometry(QRect(200, 230, 31, 41));
        label_7 = new QLabel(EditDialog);
        label_7->setObjectName("label_7");
        label_7->setGeometry(QRect(200, 280, 31, 41));
        label_8 = new QLabel(EditDialog);
        label_8->setObjectName("label_8");
        label_8->setGeometry(QRect(200, 330, 31, 41));
        label_name = new QLabel(EditDialog);
        label_name->setObjectName("label_name");
        label_name->setGeometry(QRect(100, 190, 71, 31));
        label_plate = new QLabel(EditDialog);
        label_plate->setObjectName("label_plate");
        label_plate->setGeometry(QRect(100, 240, 71, 31));
        label_home = new QLabel(EditDialog);
        label_home->setObjectName("label_home");
        label_home->setGeometry(QRect(100, 290, 71, 31));
        label_phone = new QLabel(EditDialog);
        label_phone->setObjectName("label_phone");
        label_phone->setGeometry(QRect(100, 340, 81, 31));

        retranslateUi(EditDialog);

        QMetaObject::connectSlotsByName(EditDialog);
    } // setupUi

    void retranslateUi(QDialog *EditDialog)
    {
        EditDialog->setWindowTitle(QCoreApplication::translate("EditDialog", "Dialog", nullptr));
        lineEdit_name->setPlaceholderText(QCoreApplication::translate("EditDialog", "type your name", nullptr));
        pushButton_name->setText(QCoreApplication::translate("EditDialog", "PushButton", nullptr));
        lineEdit_plate->setPlaceholderText(QCoreApplication::translate("EditDialog", "type your plate number", nullptr));
        lineEdit_home->setPlaceholderText(QCoreApplication::translate("EditDialog", "type your home address", nullptr));
        lineEdit_phone->setPlaceholderText(QCoreApplication::translate("EditDialog", "type your phone number", nullptr));
        pushButton_plate->setText(QCoreApplication::translate("EditDialog", "PushButton", nullptr));
        pushButton_home->setText(QCoreApplication::translate("EditDialog", "PushButton", nullptr));
        pushButton_phone->setText(QCoreApplication::translate("EditDialog", "PushButton", nullptr));
        pushButton_prev->setText(QCoreApplication::translate("EditDialog", "PREV", nullptr));
        pushButton_edit->setText(QCoreApplication::translate("EditDialog", "EDIT", nullptr));
        label_title->setText(QCoreApplication::translate("EditDialog", "<html><head/><body><p><span style=\" font-size:26pt; font-weight:700;\">EDIT</span></p></body></html>", nullptr));
        label_5->setText(QCoreApplication::translate("EditDialog", "<html><head/><body><p><span style=\" font-size:18pt; font-weight:700;\">&gt;&gt;</span></p></body></html>", nullptr));
        label_6->setText(QCoreApplication::translate("EditDialog", "<html><head/><body><p><span style=\" font-size:18pt; font-weight:700;\">&gt;&gt;</span></p></body></html>", nullptr));
        label_7->setText(QCoreApplication::translate("EditDialog", "<html><head/><body><p><span style=\" font-size:18pt; font-weight:700;\">&gt;&gt;</span></p></body></html>", nullptr));
        label_8->setText(QCoreApplication::translate("EditDialog", "<html><head/><body><p><span style=\" font-size:18pt; font-weight:700;\">&gt;&gt;</span></p></body></html>", nullptr));
        label_name->setText(QCoreApplication::translate("EditDialog", "<html><head/><body><p><span style=\" font-size:18pt; font-weight:700;\">NAME</span></p></body></html>", nullptr));
        label_plate->setText(QCoreApplication::translate("EditDialog", "<html><head/><body><p><span style=\" font-size:18pt; font-weight:700;\">PLATE</span></p></body></html>", nullptr));
        label_home->setText(QCoreApplication::translate("EditDialog", "<html><head/><body><p><span style=\" font-size:18pt; font-weight:700;\">HOME</span></p></body></html>", nullptr));
        label_phone->setText(QCoreApplication::translate("EditDialog", "<html><head/><body><p><span style=\" font-size:18pt; font-weight:700;\">PHONE</span></p></body></html>", nullptr));
    } // retranslateUi

};

namespace Ui {
    class EditDialog: public Ui_EditDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EDIT_H

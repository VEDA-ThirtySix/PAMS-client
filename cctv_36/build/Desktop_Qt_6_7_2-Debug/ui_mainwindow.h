/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QTabWidget *tabWidget;
    QLabel *guideLabel;
    QLabel *teamLogo;
    QLabel *hanwhaLogo;
    QLineEdit *lineEdit_host;
    QPushButton *pushButton_apply;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1052, 564);
        MainWindow->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName("tabWidget");
        tabWidget->setGeometry(QRect(10, 80, 1031, 431));
        tabWidget->setStyleSheet(QString::fromUtf8("/* \355\203\255 \354\260\275 \354\230\201\354\227\255 */\n"
" QTabWidget::pane {\n"
"        background-color:rgb(255, 255, 255);\n"
"        border:none;\n"
"\n"
"    }\n"
"\n"
"/*\355\203\255 \353\262\204\355\212\274 \354\230\201\354\227\255*/\n"
"QTabBar::tab {\n"
"	font: 9pt \"Sans Serif\";\n"
" 	background-color:rgba(243,115,33,0.5);\n"
"    color: transparent;               /* \352\270\260\353\263\270 \355\205\215\354\212\244\355\212\270\353\245\274 \354\210\250\352\271\200 */\n"
"    padding: 0px;                    /* \352\270\260\353\263\270 \355\214\250\353\224\251 */\n"
"	margin-right:5px;\n"
"    width: 120px;                  /* \352\270\260\353\263\270 \353\204\210\353\271\204 */\n"
"    height: 30px;                 /* \352\270\260\353\263\270 \353\206\222\354\235\264 */\n"
"	background-repeat: no-repeat;\n"
"    background-position: center;\n"
"}\n"
"\n"
"/*search \355\203\255*/\n"
"QTabBar::tab:first {\n"
"	background-color:rgba(243,115,33,0.1);\n"
"    background-image: url(:/images/search5.p"
                        "ng);\n"
"	color:transparent;\n"
"}\n"
"QTabBar::tab:first:selected {\n"
"	background: none; /* \354\240\204\354\262\264 \353\260\260\352\262\275 \354\206\215\354\204\261 \354\264\210\352\270\260\355\231\224!!!!!!! \354\244\221\354\232\224!! */\n"
"	background-color:  rgba(243,115,33,0.5);\n"
"	background-image:transparent;\n"
"	color:rgb(0, 0, 0);\n"
"	font: 500 12pt \"Quicksand Medium\";\n"
"    padding: 0px;                    /* \354\204\240\355\203\235\353\220\234 \355\203\255 \355\214\250\353\224\251 */\n"
"}\n"
"\n"
"\n"
"/*streaming \355\203\255*/\n"
"QTabBar::tab:last {\n"
"    background-color:rgba(243,115,33,0.1);\n"
"    background-image: url(:/images/video2.png);\n"
"	color:transparent;\n"
"}\n"
"QTabBar::tab:last:selected {\n"
"	background: none; /* \354\240\204\354\262\264 \353\260\260\352\262\275 \354\206\215\354\204\261 \354\264\210\352\270\260\355\231\224 */\n"
"	background-color:  rgba(243,115,33,0.5);\n"
"	background-image:none;\n"
"	color:rgb(0, 0, 0);\n"
"	font: 500 12pt \"Quicksand Medium"
                        "\";\n"
"    padding: 0px;                    /* \354\204\240\355\203\235\353\220\234 \355\203\255 \355\214\250\353\224\251 */\n"
"}\n"
"QTabBar::tab:hover {\n"
"	background: none; /* \354\240\204\354\262\264 \353\260\260\352\262\275 \354\206\215\354\204\261 \354\264\210\352\270\260\355\231\224 */\n"
"    background-color:rgb(223, 223, 223);\n"
"	background-image:none;\n"
"	color:rgb(255, 255, 255);\n"
"	font: 500 10pt \"Quicksand Medium\";\n"
"\n"
"}"));
        tabWidget->setTabPosition(QTabWidget::TabPosition::North);
        guideLabel = new QLabel(centralwidget);
        guideLabel->setObjectName("guideLabel");
        guideLabel->setGeometry(QRect(0, 0, 1051, 41));
        QFont font;
        font.setFamilies({QString::fromUtf8("Quicksand Medium")});
        font.setPointSize(13);
        font.setBold(true);
        font.setItalic(false);
        guideLabel->setFont(font);
        guideLabel->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"font: 500 bold 13pt \"Quicksand Medium\";\n"
"background-color: rgba(243,115,33,0.5);\n"
"border-bottom:2px solid rgb(243,115,33);"));
        guideLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);
        teamLogo = new QLabel(centralwidget);
        teamLogo->setObjectName("teamLogo");
        teamLogo->setEnabled(true);
        teamLogo->setGeometry(QRect(900, 0, 151, 31));
        QFont font1;
        font1.setFamilies({QString::fromUtf8("UnDotum")});
        font1.setPointSize(9);
        font1.setBold(true);
        font1.setItalic(false);
        teamLogo->setFont(font1);
        teamLogo->setStyleSheet(QString::fromUtf8("background-color:transparent;\n"
"color: rgb(255, 255, 255);\n"
"\n"
"font: bold 9pt \"UnDotum\";"));
        hanwhaLogo = new QLabel(centralwidget);
        hanwhaLogo->setObjectName("hanwhaLogo");
        hanwhaLogo->setEnabled(true);
        hanwhaLogo->setGeometry(QRect(0, 0, 161, 31));
        hanwhaLogo->setFont(font1);
        hanwhaLogo->setStyleSheet(QString::fromUtf8("background-color:transparent;\n"
"color: rgb(255, 255, 255);\n"
"\n"
"font: bold 9pt \"UnDotum\";"));
        lineEdit_host = new QLineEdit(centralwidget);
        lineEdit_host->setObjectName("lineEdit_host");
        lineEdit_host->setGeometry(QRect(770, 50, 191, 25));
        pushButton_apply = new QPushButton(centralwidget);
        pushButton_apply->setObjectName("pushButton_apply");
        pushButton_apply->setGeometry(QRect(970, 50, 71, 25));
        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(-1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        guideLabel->setText(QCoreApplication::translate("MainWindow", "LicensePlate Detection CCTV App", nullptr));
        teamLogo->setText(QCoreApplication::translate("MainWindow", "Thirty-Six Stratagems ", nullptr));
        hanwhaLogo->setText(QCoreApplication::translate("MainWindow", " HanwhaVision", nullptr));
        lineEdit_host->setPlaceholderText(QCoreApplication::translate("MainWindow", "Enter HOST IP Address", nullptr));
        pushButton_apply->setText(QCoreApplication::translate("MainWindow", "APPLY", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H

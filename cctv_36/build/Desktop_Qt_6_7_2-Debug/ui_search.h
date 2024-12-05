/********************************************************************************
** Form generated from reading UI file 'search.ui'
**
** Created by: Qt User Interface Compiler version 6.7.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SEARCH_H
#define UI_SEARCH_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Search
{
public:
    QPushButton *filterButton;
    QLineEdit *searchInput;
    QPushButton *searchButton;
    QTableView *resultsTable;
    QLabel *imageLabel;
    QPushButton *pushButton_edit;
    QPushButton *pushButton_delete;
    QPushButton *pushButton_enroll;
    QLabel *textLabel;
    QPushButton *pushButton_connect;
    QLabel *label_url;

    void setupUi(QWidget *Search)
    {
        if (Search->objectName().isEmpty())
            Search->setObjectName("Search");
        Search->resize(1052, 473);
        filterButton = new QPushButton(Search);
        filterButton->setObjectName("filterButton");
        filterButton->setGeometry(QRect(400, 10, 51, 41));
        QFont font;
        font.setPointSize(10);
        filterButton->setFont(font);
        filterButton->setStyleSheet(QString::fromUtf8("image: url(:/images/list.png);\n"
"background-color: transparent;\n"
"border:1px solid transparent;\n"
"\n"
"\n"
"border-top-right-radius: 5px; /* \354\230\244\353\245\270\354\252\275 \354\234\204 \353\252\250\354\204\234\353\246\254 */\n"
"border-bottom-right-radius: 5px; /* \354\230\244\353\245\270\354\252\275 \354\225\204\353\236\230 \353\252\250\354\204\234\353\246\254 */\n"
"border-top-left-radius: 5px; /* \354\231\274\354\252\275 \354\234\204 \353\252\250\354\204\234\353\246\254 */\n"
"border-bottom-left-radius: 5px; /* \354\231\274\354\252\275 \354\225\204\353\236\230 \353\252\250\354\204\234\353\246\254 */"));
        searchInput = new QLineEdit(Search);
        searchInput->setObjectName("searchInput");
        searchInput->setGeometry(QRect(190, 10, 261, 41));
        searchInput->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"border:1px solid rgb(0, 0, 0);\n"
"\n"
"border-top-right-radius: 0px; /* \354\230\244\353\245\270\354\252\275 \354\234\204 \353\252\250\354\204\234\353\246\254 */\n"
"border-bottom-right-radius: 0px; /* \354\230\244\353\245\270\354\252\275 \354\225\204\353\236\230 \353\252\250\354\204\234\353\246\254 */\n"
"border-top-left-radius: 5px; /* \354\231\274\354\252\275 \354\234\204 \353\252\250\354\204\234\353\246\254 */\n"
"border-bottom-left-radius: 5px; /* \354\231\274\354\252\275 \354\225\204\353\236\230 \353\252\250\354\204\234\353\246\254 */"));
        searchButton = new QPushButton(Search);
        searchButton->setObjectName("searchButton");
        searchButton->setGeometry(QRect(450, 10, 41, 41));
        searchButton->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"border:1px solid rgb(0, 0, 0);\n"
"\n"
"\n"
"border-top-right-radius: 5px; /* \354\230\244\353\245\270\354\252\275 \354\234\204 \353\252\250\354\204\234\353\246\254 */\n"
"border-bottom-right-radius: 5px; /* \354\230\244\353\245\270\354\252\275 \354\225\204\353\236\230 \353\252\250\354\204\234\353\246\254 */\n"
"border-top-left-radius: 0; /* \354\231\274\354\252\275 \354\234\204 \353\252\250\354\204\234\353\246\254 */\n"
"border-bottom-left-radius: 0; /* \354\231\274\354\252\275 \354\225\204\353\236\230 \353\252\250\354\204\234\353\246\254 */"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/search5.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        searchButton->setIcon(icon);
        searchButton->setIconSize(QSize(20, 20));
        resultsTable = new QTableView(Search);
        resultsTable->setObjectName("resultsTable");
        resultsTable->setGeometry(QRect(0, 60, 661, 351));
        resultsTable->setStyleSheet(QString::fromUtf8("QTableView {\n"
"    background-color: rgb(255, 255, 255);\n"
"}\n"
"\n"
"QHeaderView::section {\n"
"    background-color: rgba(202,205,205,0.5);\n"
"    border: 1px solid rgba(0, 0, 0, 0.5);\n"
"    padding: 3px;\n"
"    font-weight: bold;\n"
"    text-align: center;\n"
"}"));
        resultsTable->setInputMethodHints(Qt::InputMethodHint::ImhNone);
        resultsTable->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);
        resultsTable->setTextElideMode(Qt::TextElideMode::ElideLeft);
        resultsTable->horizontalHeader()->setCascadingSectionResizes(false);
        imageLabel = new QLabel(Search);
        imageLabel->setObjectName("imageLabel");
        imageLabel->setGeometry(QRect(700, 70, 311, 171));
        imageLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);
        pushButton_edit = new QPushButton(Search);
        pushButton_edit->setObjectName("pushButton_edit");
        pushButton_edit->setGeometry(QRect(940, 350, 88, 25));
        pushButton_delete = new QPushButton(Search);
        pushButton_delete->setObjectName("pushButton_delete");
        pushButton_delete->setGeometry(QRect(940, 380, 88, 25));
        pushButton_enroll = new QPushButton(Search);
        pushButton_enroll->setObjectName("pushButton_enroll");
        pushButton_enroll->setGeometry(QRect(940, 320, 88, 25));
        textLabel = new QLabel(Search);
        textLabel->setObjectName("textLabel");
        textLabel->setGeometry(QRect(710, 250, 151, 161));
        pushButton_connect = new QPushButton(Search);
        pushButton_connect->setObjectName("pushButton_connect");
        pushButton_connect->setGeometry(QRect(940, 280, 88, 25));
        label_url = new QLabel(Search);
        label_url->setObjectName("label_url");
        label_url->setGeometry(QRect(710, 30, 291, 31));

        retranslateUi(Search);

        QMetaObject::connectSlotsByName(Search);
    } // setupUi

    void retranslateUi(QWidget *Search)
    {
        Search->setWindowTitle(QCoreApplication::translate("Search", "Form", nullptr));
        filterButton->setText(QString());
        searchButton->setText(QString());
        imageLabel->setText(QCoreApplication::translate("Search", "<html><head/><body><p align=\"center\"><span style=\" font-size:14pt; font-weight:700;\">Vehicle<br/>License Plate</span></p></body></html>", nullptr));
        pushButton_edit->setText(QCoreApplication::translate("Search", "EDIT", nullptr));
        pushButton_delete->setText(QCoreApplication::translate("Search", "DELETE", nullptr));
        pushButton_enroll->setText(QCoreApplication::translate("Search", "ENROLL", nullptr));
        textLabel->setText(QCoreApplication::translate("Search", "<html><head/><body><p align=\"center\"><span style=\" font-size:14pt; font-weight:700;\">UserInfo</span></p></body></html>", nullptr));
        pushButton_connect->setText(QCoreApplication::translate("Search", "CONNECT", nullptr));
        label_url->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Search: public Ui_Search {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SEARCH_H

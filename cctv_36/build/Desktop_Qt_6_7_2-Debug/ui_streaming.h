/********************************************************************************
** Form generated from reading UI file 'streaming.ui'
**
** Created by: Qt User Interface Compiler version 6.7.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STREAMING_H
#define UI_STREAMING_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Streaming
{
public:
    QFrame *streamingFrame;
    QLabel *streamingLabel;
    QFrame *bestshotFrame;
    QLabel *label;
    QLabel *label_3;
    QLabel *dateTimeLabel;
    QLabel *licensetitleLabel;
    QFrame *bestshotFrame_2;
    QPushButton *startButton;
    QPushButton *stopButton;
    QLabel *label_url;

    void setupUi(QWidget *Streaming)
    {
        if (Streaming->objectName().isEmpty())
            Streaming->setObjectName("Streaming");
        Streaming->resize(1052, 564);
        streamingFrame = new QFrame(Streaming);
        streamingFrame->setObjectName("streamingFrame");
        streamingFrame->setGeometry(QRect(0, 0, 691, 451));
        streamingFrame->setStyleSheet(QString::fromUtf8("background-color: transparent;\n"
"border:none;"));
        streamingFrame->setFrameShape(QFrame::Shape::NoFrame);
        streamingLabel = new QLabel(streamingFrame);
        streamingLabel->setObjectName("streamingLabel");
        streamingLabel->setGeometry(QRect(10, 80, 440, 250));
        streamingLabel->setStyleSheet(QString::fromUtf8("font: 300 bold 13pt \"Quicksand Light\";\n"
"color:  rgb(243,115,33);\n"
"background:rgba(243,115,33,0.2);\n"
"border:1px solid rgb(243,115,33);"));
        bestshotFrame = new QFrame(streamingFrame);
        bestshotFrame->setObjectName("bestshotFrame");
        bestshotFrame->setGeometry(QRect(460, 40, 231, 391));
        bestshotFrame->setStyleSheet(QString::fromUtf8("font: 500 13pt \"Quicksand Medium\";\n"
"color:  rgb(243,115,33);\n"
"background:none;\n"
"border:1px solid rgb(243,115,33);"));
        bestshotFrame->setFrameShape(QFrame::Shape::NoFrame);
        label = new QLabel(bestshotFrame);
        label->setObjectName("label");
        label->setGeometry(QRect(10, 230, 211, 51));
        label->setStyleSheet(QString::fromUtf8("background:none;\n"
"border:1px solid rgb(255, 255, 255);\n"
"font: 300 bold 13pt \"Quicksand Light\";\n"
"color:  rgb(243,115,33);\n"
"background:none;"));
        label_3 = new QLabel(bestshotFrame);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(10, 20, 211, 91));
        label_3->setStyleSheet(QString::fromUtf8("background:none;\n"
"border:1px solid rgb(255, 255, 255);\n"
"font: 300 bold 13pt \"Quicksand Light\";\n"
"color:  rgb(243,115,33);\n"
"background:none;"));
        dateTimeLabel = new QLabel(streamingFrame);
        dateTimeLabel->setObjectName("dateTimeLabel");
        dateTimeLabel->setGeometry(QRect(10, 10, 440, 31));
        dateTimeLabel->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"font: 500 12pt \"Quicksand Medium\";\n"
"background-color: rgba(243,115,33,0.5);\n"
"border:1px solid rgb(243,115,33);"));
        licensetitleLabel = new QLabel(streamingFrame);
        licensetitleLabel->setObjectName("licensetitleLabel");
        licensetitleLabel->setGeometry(QRect(460, 10, 231, 31));
        licensetitleLabel->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"font: 500 13pt \"Quicksand Medium\";\n"
"background-color: rgba(243,115,33,0.5);\n"
"border:1px solid rgb(243,115,33);"));
        bestshotFrame_2 = new QFrame(streamingFrame);
        bestshotFrame_2->setObjectName("bestshotFrame_2");
        bestshotFrame_2->setGeometry(QRect(10, 340, 441, 91));
        bestshotFrame_2->setStyleSheet(QString::fromUtf8("color:  rgb(243,115,33);\n"
"background:none;\n"
"border:1px solid rgb(243,115,33);"));
        bestshotFrame_2->setFrameShape(QFrame::Shape::NoFrame);
        startButton = new QPushButton(streamingFrame);
        startButton->setObjectName("startButton");
        startButton->setGeometry(QRect(10, 60, 221, 21));
        startButton->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"	color:  rgb(243,115,33);\n"
"	background:none;\n"
"	border:1px solid rgb(243,115,33);\n"
"	font: 500 9pt \"Quicksand Medium\";\n"
"}\n"
"QPushButton:selected{\n"
"	color:  rgb(243,115,33);\n"
"	background-color: rgba(243,115,33,0.3);\n"
"	border:1px solid rgb(243,115,33);\n"
"	font: 500 9pt \"Quicksand Medium\";\n"
"}\n"
"QPushButton:hover {\n"
"	color:  rgb(255, 255, 255);\n"
"	background-color:rgba(243,115,33,0.3);\n"
"	border:1px solid rgb(243,115,33);\n"
"	font: 500 8pt \"Quicksand Medium\";\n"
"}"));
        startButton->setIconSize(QSize(20, 20));
        stopButton = new QPushButton(streamingFrame);
        stopButton->setObjectName("stopButton");
        stopButton->setGeometry(QRect(230, 60, 220, 21));
        stopButton->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"	color:  rgb(243,115,33);\n"
"	background:none;\n"
"	border:1px solid rgb(243,115,33);\n"
"	font: 500 9pt \"Quicksand Medium\";\n"
"}\n"
"QPushButton:selected{\n"
"	color:  rgb(243,115,33);\n"
"	background-color: rgba(243,115,33,0.3);\n"
"	border:1px solid rgb(243,115,33);\n"
"	font: 500 9pt \"Quicksand Medium\";\n"
"}\n"
"QPushButton:hover {\n"
"	color:  rgb(255, 255, 255);\n"
"	background-color:rgba(243,115,33,0.3);\n"
"	border:1px solid rgb(243,115,33);\n"
"	font: 500 8pt \"Quicksand Medium\";\n"
"}"));
        label_url = new QLabel(streamingFrame);
        label_url->setObjectName("label_url");
        label_url->setGeometry(QRect(10, 40, 440, 21));
        label_url->setAlignment(Qt::AlignmentFlag::AlignCenter);
        streamingLabel->raise();
        dateTimeLabel->raise();
        licensetitleLabel->raise();
        bestshotFrame->raise();
        bestshotFrame_2->raise();
        startButton->raise();
        stopButton->raise();
        label_url->raise();

        retranslateUi(Streaming);

        QMetaObject::connectSlotsByName(Streaming);
    } // setupUi

    void retranslateUi(QWidget *Streaming)
    {
        Streaming->setWindowTitle(QCoreApplication::translate("Streaming", "Form", nullptr));
        streamingLabel->setText(QCoreApplication::translate("Streaming", "Streaming Video", nullptr));
        label->setText(QCoreApplication::translate("Streaming", "Detected Vehicle Image", nullptr));
        label_3->setText(QCoreApplication::translate("Streaming", "Vehicle1", nullptr));
        dateTimeLabel->setText(QCoreApplication::translate("Streaming", "Date - Time", nullptr));
        licensetitleLabel->setText(QCoreApplication::translate("Streaming", "LicensePlate", nullptr));
        startButton->setText(QCoreApplication::translate("Streaming", "Start", nullptr));
        stopButton->setText(QCoreApplication::translate("Streaming", "Stop", nullptr));
        label_url->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Streaming: public Ui_Streaming {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STREAMING_H

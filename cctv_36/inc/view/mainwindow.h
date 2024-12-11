#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QTimer>

#include <QTcpSocket>
#include <QBuffer>

#include <QDebug>
#include <QThread>
#include <QVector>

#include <QString>
#include <QMutex>
#include <QRegularExpression>
#include <QDateTime>

QT_BEGIN_NAMESPACE
namespace Ui {class MainWindow;}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void send_initData();
    QString getWiFiIPAddress();

private:
    Ui::MainWindow *ui;
    QString m_host;

private slots:

};
#endif // MAINWINDOW_H

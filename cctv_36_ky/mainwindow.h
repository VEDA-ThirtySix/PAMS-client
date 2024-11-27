#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QTimer>

#include <QTcpSocket>
#include <QBuffer>
#include "search.h"
#include "dbTable.h"    //KIYUN: 1126
#include <QSqlDatabase>

QT_BEGIN_NAMESPACE
namespace Ui {class MainWindow;}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void updateTable();

private slots:
    void updateDateTime();
    void readStream();
    void handleError(QAbstractSocket::SocketError socketError);
    void reconnectToStream();
    void onConnected();
    void onDisconnected();

private:
    Ui::MainWindow *ui;
    QTimer *timer;
    Search *searchManager;

    QTcpSocket *streamSocket;
    QByteArray frameBuffer;

    QSqlDatabase db;
    QTableView* m_tableView;
    DBtable* m_dbTable;

    const int FRAME_WIDTH = 320;
    const int FRAME_HEIGHT = 240;
    const int FRAME_SIZE = FRAME_WIDTH * FRAME_HEIGHT * 2;  // YUYV format

    void setupStreamingConnection();
    void processYUYVFrame(const QByteArray &frameData);

    void setupSearch();
};
#endif // MAINWINDOW_H

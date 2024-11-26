#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QTimer>

#include <QTcpSocket>
#include <QBuffer>
#include "search.h"

#include <QDebug>
#include <QThread>
#include <QVector>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
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

    QSqlDatabase db;  // 데이터베이스 객체
    QSqlQuery image_Query; //이미지 저장 테이블을 관리할 쿼리
    QMenu *windowMenu;

    const int FRAME_WIDTH = 320;
    const int FRAME_HEIGHT = 240;
    const int FRAME_SIZE = FRAME_WIDTH * FRAME_HEIGHT * 2;  // YUYV format

    bool errorMessageShown = false;  // 에러 메시지 표시 여부를 추적하는 플래그

    void setupStreamingConnection();
    void processYUYVFrame(const QByteArray &frameData);

    void setupSearch();


    void initDatabase();  // 데이터베이스 초기화 메서드
    void saveMessageToDatabase(const QString &message);  // 메시지를 데이터베이스에 저장하는 메서드
    bool saveToDatabase(const QString &tableName, const QMap<QString, QVariant> &data);    // 데이터베이스 저장 함수


};
#endif // MAINWINDOW_H

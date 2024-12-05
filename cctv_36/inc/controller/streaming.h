#ifndef STREAMING_H
#define STREAMING_H

#include <QWidget>
#include <QSqlQuery>
#include <QMainWindow>
#include <QLabel>
#include <QProcess>
#include <QTimer>
#include "ui_streaming.h"

class Streaming : public QWidget
{
    Q_OBJECT

public:
    explicit Streaming(QWidget *parent = nullptr);
    ~Streaming();

private:
    Ui::Streaming *ui;
    QTimer *timer;
    QProcess *ffmpegProcess;
    QTimer *frameTimer;
    QString rtspUrl;  // RTSP 서버 주소
    QByteArray *buffer;
    // 데이터베이스 관련 메서드
    void saveMessageToDatabase(const QString &message);  // 메시지를 데이터베이스에 저장하는 메서드
    bool saveToDatabase(const QString &tableName, const QMap<QString, QVariant> &data);    // 데이터베이스 저장 함수

    QByteArray incompleteBuffer; // 12.06 추가 : 중요!

private slots:
    // UI 업데이트
    void updateDateTime();

    // RTSP 및 FFmpeg 관련
    void setupAddressInput();
    bool eventFilter(QObject *watched, QEvent *event);
    void startFFmpeg();
    void stopFFmpeg();
    void captureFrame();
    void processOutput();

    // RTSP GUI 설정
    void on_setButton_clicked();    //RTSP IP주소 설정
    void rtsp_setting();            //
    void on_startButton_clicked();
    void on_stopButton_clicked();

    void setButtonStyle(QPushButton* button, bool isActive);
};

#endif // STREAMING_H

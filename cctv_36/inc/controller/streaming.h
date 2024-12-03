#ifndef STREAMING_H
#define STREAMING_H

#include <QWidget>
#include <QAbstractSocket>
#include <QTcpSocket>
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

    // 데이터베이스 관련 메서드
    void saveMessageToDatabase(const QString &message);  // 메시지를 데이터베이스에 저장하는 메서드
    bool saveToDatabase(const QString &tableName, const QMap<QString, QVariant> &data);    // 데이터베이스 저장 함수


private slots:
    // UI 업데이트
    void updateDateTime();
    void updateUIState(bool isRunning);

    // RTSP 및 FFmpeg 관련
    void setupAddressInput();
    bool eventFilter(QObject *watched, QEvent *event);
    void startFFmpeg();
    void stopFFmpeg();
    void captureFrame();
    void processOutput();

    // RTSP 설정
    void on_setButton_clicked();
    void rtsp_setting();
};

#endif // STREAMING_H

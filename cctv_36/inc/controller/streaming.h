#ifndef STREAMING_H
#define STREAMING_H

#include "ui_streaming.h"
#include <QWidget>

#include <QSqlQuery>
#include <QMainWindow>
#include <QLabel>
#include <QProcess>
#include <QTimer>
#include <QUrl>
#include <QByteArray>

class Streaming : public QWidget
{
    Q_OBJECT

public:
    explicit Streaming(QWidget *parent = nullptr);
    ~Streaming();

    // RTSP 설정
    void get_host(const QString& host);
    void rtsp_setting();
    void set_config(const QString& url, const QString& port);

private:
    Ui::Streaming *ui;
    QTimer *timer;
    QProcess *ffmpegProcess;
    QTimer *frameTimer;
    QString m_host;
    QString m_rtspUrl;  // RTSP 서버 주소
    QByteArray m_receivedBuffer;

    // 데이터베이스 관련 메서드
    //void saveMessageToDatabase(const QString &message);  // 메시지를 데이터베이스에 저장하는 메서드
    //bool saveToDatabase(const QString &tableName, const QMap<QString, QVariant> &data);    // 데이터베이스 저장 함수

    QByteArray incompleteBuffer; // 12.06 추가 : 중요!

private slots:
    // UI 업데이트
    void updateDateTime();

    // RTSP 및 FFmpeg 관련
    void startFFmpeg();
    void stopFFmpeg();
    void captureFrame();
    void processOutput();

    // 슬롯 정의
    void on_startButton_clicked();
    void on_stopButton_clicked();

    void carEntryState(bool state);
    void updateGateState(bool state);

    void setButtonStyle(QPushButton* button, bool isActive);

    void on_plateDataReceived(const QByteArray& buffer);
    void update_InfoLabel();
};

#endif // STREAMING_H

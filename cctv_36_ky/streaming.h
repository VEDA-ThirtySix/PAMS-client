#ifndef STREAMING_H
#define STREAMING_H

#include <QWidget>
#include "ui_streaming.h"

#include <QAbstractSocket>
#include <QTcpSocket>
#include <QSqlQuery>

class Streaming : public QWidget
{
    Q_OBJECT

public:
    explicit Streaming(QWidget *parent = nullptr);
    ~Streaming();

private:
    Ui::Streaming *ui;
    QTimer *timer;

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

    void saveMessageToDatabase(const QString &message);  // 메시지를 데이터베이스에 저장하는 메서드
    bool saveToDatabase(const QString &tableName, const QMap<QString, QVariant> &data);    // 데이터베이스 저장 함수


private slots:
    void updateDateTime();
    void readStream();
    void handleError(QAbstractSocket::SocketError socketError);
    void reconnectToStream();
    void onConnected();
    void onDisconnected();
};

#endif // STREAMING_H

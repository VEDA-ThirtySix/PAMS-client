#ifndef DIALOG_VIDEOCLIP_H
#define DIALOG_VIDEOCLIP_H

#include <QDialog>
#include <QProcess>
#include <QTimer>

namespace Ui {
class VideoClipDialog;
}

class VideoClipDialog : public QDialog
{
    Q_OBJECT

public:
    explicit VideoClipDialog(const QString &host, QWidget *parent = nullptr);
    ~VideoClipDialog();

private slots:
    void handleError();
    void captureFrame();
    void playPauseVideo();
    void stopVideo();
    void updatePosition(qint64 position);
    void reconnectStream();

private:
    void setupConnections();
    void startFFmpeg();
    void stopFFmpeg();
    void showErrorMessage(const QString &message);
    QString formatTime(qint64 milliseconds);
    void setupSlider();

    Ui::VideoClipDialog *ui;
    QProcess *ffmpegProcess;
    QTimer *frameTimer;
    QString m_rtspUrl;
    int reconnectAttempts;
    QByteArray incompleteBuffer;
    qint64 frameCount = 0;
    qint64 startTime;

    static const int MAX_RECONNECT_ATTEMPTS = 3;
    static const int RECONNECT_INTERVAL = 5000;
    static constexpr const char* PROTOCOL = "rtsp";
    static const int CLIP_PORT = 7554;
};

#endif // DIALOG_VIDEOCLIP_H

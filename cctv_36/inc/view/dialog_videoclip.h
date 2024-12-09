#ifndef DIALOG_VIDEOCLIP_H
#define DIALOG_VIDEOCLIP_H

#include <QDialog>
#include <QMediaPlayer>

namespace Ui {
class VideoClipDialog;
}

class VideoClipDialog : public QDialog
{
    Q_OBJECT

public:
    explicit VideoClipDialog(const QString &videoPath, QWidget *parent = nullptr);
    ~VideoClipDialog();

private slots:
    void handleError();
    void handlePlaybackStateChanged(QMediaPlayer::PlaybackState state);
    void playPauseVideo();
    void stopVideo();
    void updatePosition(qint64 position);
    void updateDuration(qint64 duration);
    void seek(int position);

private:
    void setupConnections();
    QString formatTime(qint64 milliseconds);

    Ui::VideoClipDialog *ui;
    QMediaPlayer *mediaPlayer;
};

#endif // DIALOG_VIDEOCLIP_H

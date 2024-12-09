#include "dialog_videoclip.h"
#include "ui_clip.h"
#include <QDebug>

VideoClipDialog::VideoClipDialog(const QString &videoPath, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::VideoClipDialog)
    , mediaPlayer(new QMediaPlayer(this))
{
    ui->setupUi(this);
    setupConnections();

    // 비디오 출력 설정
    mediaPlayer->setVideoOutput(ui->videoWidget);

    // 비디오 경로 설정 및 재생 시작
    mediaPlayer->setSource(QUrl::fromLocalFile(videoPath));
    mediaPlayer->play();
}

VideoClipDialog::~VideoClipDialog()
{
    delete ui;
}

void VideoClipDialog::setupConnections()
{
    // 미디어 플레이어 시그널 연결
    connect(mediaPlayer, &QMediaPlayer::errorOccurred, this, &VideoClipDialog::handleError);
    connect(mediaPlayer, &QMediaPlayer::playbackStateChanged, this, &VideoClipDialog::handlePlaybackStateChanged);
    connect(mediaPlayer, &QMediaPlayer::positionChanged, this, &VideoClipDialog::updatePosition);
    connect(mediaPlayer, &QMediaPlayer::durationChanged, this, &VideoClipDialog::updateDuration);

    // 컨트롤 버튼 연결
    connect(ui->playPauseButton, &QPushButton::clicked, this, &VideoClipDialog::playPauseVideo);
    connect(ui->stopButton, &QPushButton::clicked, this, &VideoClipDialog::stopVideo);
    connect(ui->positionSlider, &QSlider::sliderMoved, this, &VideoClipDialog::seek);
}

void VideoClipDialog::handleError()
{
    qDebug() << "미디어 플레이어 오류:" << mediaPlayer->error() << mediaPlayer->errorString();
}

void VideoClipDialog::handlePlaybackStateChanged(QMediaPlayer::PlaybackState state)
{
    if (state == QMediaPlayer::PlayingState) {
        ui->playPauseButton->setText("일시정지");
    } else {
        ui->playPauseButton->setText("재생");
    }
}

void VideoClipDialog::playPauseVideo()
{
    if (mediaPlayer->playbackState() == QMediaPlayer::PlayingState) {
        mediaPlayer->pause();
    } else {
        mediaPlayer->play();
    }
}

void VideoClipDialog::stopVideo()
{
    mediaPlayer->stop();
}

void VideoClipDialog::updatePosition(qint64 position)
{
    ui->positionSlider->setValue(position);
    ui->currentTimeLabel->setText(formatTime(position));
}

void VideoClipDialog::updateDuration(qint64 duration)
{
    ui->positionSlider->setRange(0, duration);
    ui->totalTimeLabel->setText(formatTime(duration));
}

void VideoClipDialog::seek(int position)
{
    mediaPlayer->setPosition(position);
}

QString VideoClipDialog::formatTime(qint64 milliseconds)
{
    int seconds = (milliseconds / 1000) % 60;
    int minutes = (milliseconds / 60000) % 60;
    int hours = (milliseconds / 3600000);

    if (hours > 0) {
        return QString("%1:%2:%3")
        .arg(hours, 2, 10, QChar('0'))
            .arg(minutes, 2, 10, QChar('0'))
            .arg(seconds, 2, 10, QChar('0'));
    }
    return QString("%1:%2")
        .arg(minutes, 2, 10, QChar('0'))
        .arg(seconds, 2, 10, QChar('0'));
}

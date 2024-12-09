#include "dialog_videoclip.h"
#include "ui_clip.h"
#include <QDebug>
#include <QMessageBox>
#include <QTimer>
#include <QLabel>

VideoClipDialog::VideoClipDialog(const QString &host, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::VideoClipDialog)
    , ffmpegProcess(new QProcess(this))
    , frameTimer(new QTimer(this))
    , reconnectAttempts(0)
{
    ui->setupUi(this);
    setupConnections();

    // RTSP URL 설정
    m_rtspUrl = QString("%1://%2:%3/")
                    .arg(PROTOCOL, host, QString::number(CLIP_PORT));

    qDebug() << "QDEBUG(SW)$ Clip RTSP URL:" << m_rtspUrl;

    // FFmpeg 프레임 타이머 설정
    frameTimer->setInterval(33); // ~30fps
    connect(frameTimer, &QTimer::timeout, this, &VideoClipDialog::captureFrame);

    // FFmpeg 시작
    startFFmpeg();
}

void VideoClipDialog::setupConnections()
{
    // 컨트롤 버튼 연결
    connect(ui->playPauseButton, &QPushButton::clicked, this, &VideoClipDialog::playPauseVideo);
    connect(ui->stopButton, &QPushButton::clicked, this, &VideoClipDialog::stopVideo);
}

void VideoClipDialog::startFFmpeg()
{
    if (ffmpegProcess->state() == QProcess::Running) {
        qDebug() << "FFmpeg is already running!";
        return;
    }

#ifdef Q_OS_MAC
    QString ffmpegPath = "/opt/homebrew/bin/ffmpeg";
#elif defined(Q_OS_LINUX)
    QString ffmpegPath = "/usr/bin/ffmpeg";
#endif

    QStringList arguments = {
        "-protocol_whitelist", "file,tcp,udp,rtp,rtsp",
        "-i", m_rtspUrl,
        "-vf", "fps=30",
        "-s", "800x600",
        "-f", "rawvideo",
        "-pix_fmt", "rgb24",
        "-loglevel", "debug",
        "-"
    };

    ffmpegProcess->start(ffmpegPath, arguments);

    if (!ffmpegProcess->waitForStarted()) {
        showErrorMessage("FFmpeg failed to start");
        return;
    }

    frameTimer->start();
}

void VideoClipDialog::stopFFmpeg()
{
    frameTimer->stop();
    if (ffmpegProcess->state() == QProcess::Running) {
        ffmpegProcess->terminate();
        if (!ffmpegProcess->waitForFinished(3000)) {
            ffmpegProcess->kill();
        }
    }
}

void VideoClipDialog::captureFrame()
{
    while (ffmpegProcess->bytesAvailable() > 0) {
        QByteArray frameData = ffmpegProcess->readAllStandardOutput();
        if (frameData.isEmpty()) return;

        incompleteBuffer.append(frameData);

        const int width = 800;
        const int height = 600;
        const int frameSize = width * height * 3;

        while (incompleteBuffer.size() >= frameSize) {
            QByteArray singleFrameData = incompleteBuffer.left(frameSize);
            incompleteBuffer.remove(0, frameSize);

            QImage image((const uchar*)singleFrameData.data(),
                         width, height, QImage::Format_RGB888);

            QPixmap scaledPixmap = QPixmap::fromImage(image).scaled(
                ui->ffmpegLabel->size(),
                Qt::KeepAspectRatioByExpanding,
                Qt::SmoothTransformation
                );

            ui->ffmpegLabel->setPixmap(scaledPixmap);
            updatePosition(frameTimer->interval() * frameCount++);
        }
    }
}

void VideoClipDialog::playPauseVideo()
{
    if (frameTimer->isActive()) {
        frameTimer->stop();
        ui->playPauseButton->setIcon(QIcon(":/images/play2.png"));
    } else {
        frameTimer->start();
        ui->playPauseButton->setIcon(QIcon(":/images/pause2.png"));
    }
}

void VideoClipDialog::stopVideo()
{
    frameTimer->stop();
    frameCount = 0;
    ui->playPauseButton->setIcon(QIcon(":/images/play2.png"));
    ui->positionSlider->setValue(0);
    ui->currentTimeLabel->setText("00:00");
    startFFmpeg();
}

void VideoClipDialog::updatePosition(qint64 position)
{
    if (!ui->positionSlider->isSliderDown()) {
        ui->positionSlider->setValue(position);
    }
    ui->currentTimeLabel->setText(formatTime(position));
}

QString VideoClipDialog::formatTime(qint64 milliseconds)
{
    int seconds = (milliseconds / 1000) % 60;
    int minutes = (milliseconds / 60000) % 60;
    return QString("%1:%2")
        .arg(minutes, 2, 10, QChar('0'))
        .arg(seconds, 2, 10, QChar('0'));
}

void VideoClipDialog::showErrorMessage(const QString &message)
{
    QMessageBox::critical(this, tr("Streaming Error"), message);
}

void VideoClipDialog::handleError()
{
    QString errorString = ffmpegProcess->errorString();
    qDebug() << "FFmpeg error:" << errorString;

    if (reconnectAttempts < MAX_RECONNECT_ATTEMPTS) {
        showErrorMessage(tr("Stream error: %1\nAttempting to reconnect...").arg(errorString));
        QTimer::singleShot(RECONNECT_INTERVAL, this, &VideoClipDialog::reconnectStream);
        reconnectAttempts++;
    } else {
        showErrorMessage(tr("Stream connection failed: %1\nMax reconnection attempts exceeded.").arg(errorString));
    }
}

void VideoClipDialog::reconnectStream()
{
    stopFFmpeg();
    startFFmpeg();
}

VideoClipDialog::~VideoClipDialog()
{
    stopFFmpeg();
    delete ui;
}

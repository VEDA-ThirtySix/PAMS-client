#include "streaming.h"
#include "ui_streaming.h"
#include <QDebug>
#include <QPixmap>
#include <QImage>
#include <QLayout>

#include <QTimer>
#include <QDateTime>
#include <QMessageBox>

Streaming::Streaming(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Streaming)
    , ffmpegProcess(new QProcess(this))
    , frameTimer(new QTimer(this))
    , timer(new QTimer(this))
    , rtspUrl("") // 초기 URL 비우기
{
    ui->setupUi(this);

    // SetButton 체크 가능하도록 설정
    ui->setButton->setCheckable(true);
    ui->addressInput->setPlaceholderText("Enter RTSP server address (e.g., 192.168.0.39)");

    setupAddressInput();
    if (ui->addressInput->text().isEmpty()) {
        ui->addressInput->setPlaceholderText("Enter RTSP server address (e.g., 192.168.0.39)");
    }
    // 타이머로 날짜/시간 업데이트
    connect(timer, &QTimer::timeout, this, &Streaming::updateDateTime);
    timer->start(1000); //1초마다 실행
    updateDateTime();// 초기 날짜/시간 표시

    // start/stop 버튼 클릭으로 스트리밍 시작/중지
    connect(ui->startButton, &QPushButton::clicked, this, &Streaming::startFFmpeg);
    connect(ui->stopButton, &QPushButton::clicked, this, &Streaming::stopFFmpeg);

    connect(frameTimer, &QTimer::timeout, this, &Streaming::captureFrame);
    //connect(ffmpegProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(captureFrame()));

    // Set 버튼 클릭으로 RTSP URL 설정
    connect(ui->setButton, &QPushButton::clicked, this, &Streaming::rtsp_setting);

    // AddressInput 내용 변경 시 SetButton 상태 해제
    connect(ui->addressInput, &QLineEdit::textChanged, this, [this]() {
        ui->setButton->setChecked(false);
    });
    // 초기 UI 상태 설정
    updateUIState(false); // Start 버튼 활성화, Stop 버튼 비활성화
}

Streaming::~Streaming()
{
    if (ffmpegProcess->state() == QProcess::Running) {
        ffmpegProcess->terminate();
        if (!ffmpegProcess->waitForFinished(3000)) { // 3초 대기
                        ffmpegProcess->kill(); // 종료 실패 시 강제 종료
                    }
    }
    delete ui;
}

void Streaming::updateDateTime()
{
    QDateTime current = QDateTime::currentDateTime();

    // 요일 한글 변환
    QMap<int, QString> weekdays;
    weekdays[1] = "MON";
    weekdays[2] = "TUE";
    weekdays[3] = "WED";
    weekdays[4] = "THU";
    weekdays[5] = "FRI";
    weekdays[6] = "SAT";
    weekdays[7] = "SUN";

    QString weekday = weekdays[current.date().dayOfWeek()];

    // 오전/오후 판단
    QString ampm = current.time().hour() < 12 ? "AM" : "PM";

    // 12시간제로 변환
    int hour = current.time().hour() % 12;
    if (hour == 0) hour = 12;

    // 날짜/시간 문자열 생성
    QString dateTimeStr = QString::asprintf("%04d.%02d.%02d %s | %s %02d:%02d:%02d",
                            current.date().year(),
                            current.date().month(),
                            current.date().day(),
                            qPrintable(weekday),
                            qPrintable(ampm),
                            hour,
                            current.time().minute(),
                            current.time().second());

    ui->dateTimeLabel->setText(dateTimeStr);
}


/*RTSP스트림을 가져오고, 파이프로 출력*/
void Streaming::startFFmpeg() {

    if (ffmpegProcess->state() == QProcess::Running) {
        qDebug() << "FFmpeg is already running!";
        return;
    }

    if (rtspUrl.isEmpty()) {
        //ui->messageInput->text().isEmpty()
        QMessageBox::warning(this, "Error", "RTSP URL is not set. Please set the address first.");
        return;
    }

    QString ffmpegPath = "/bin/ffmpeg"; // FFmpeg 실행 경로

    /*FFMPEG 실행시 전달되는 옵션*/
    QStringList arguments = {
        "-fflags", "discardcorrupt",        //   손상된 프레임 무시
        "-i",rtspUrl, //동적으로 설정된 RTSP URL
        "-vf", "fps=30",                    // 출력 프레임 속도 설정
        "-s", "680x360",                         // 출력 영상의 해상도 설정
        "-f", "image2pipe",                 // 이미지전송: 파이프 형식
        "-vcodec", "mjpeg",                 // MJPEG 형식으로 디코딩
        "-q:v", "2",    // 품질 설정 (1~31, 낮을수록 품질이 좋음)
        "-pix_fmt", "yuvj420p",  // 색 공간 최적화
        "pipe:1"                            // 출력 파이프
    };

    ffmpegProcess->start(ffmpegPath, arguments);

    if (!ffmpegProcess->waitForStarted()) {
        qDebug() << "FFmpeg failed to start!";
        return;
    }
    frameTimer->start(33); // 30fps (1000ms /33 = 30ms)
    qDebug() << "FFmpeg started with URL:" << rtspUrl;

    updateUIState(true); // Start 버튼 비활성화, Stop 버튼 활성화
}

void Streaming::captureFrame() {
    while(ffmpegProcess->canReadLine()){
        // FFmpeg 프로세스에서 데이터를 읽기
        QByteArray frameData = ffmpegProcess->readAllStandardOutput();
        if (frameData.isEmpty()) {
            qDebug() << "No frame data received!";
            return;
        }

        // 프레임데이터를 QPixmap으로 변환
        QPixmap pixmap;
        if (!pixmap.loadFromData(frameData, "JPEG")) {
            qDebug() << "Failed to load frame data!";
            return;
        }

        // QLabel 크기에 맞게 조정
        QPixmap scaledPixmap = pixmap.scaled(
            ui->streamingLabel->size(),
            Qt::KeepAspectRatio,
            Qt::SmoothTransformation
        );
        // QLabel에 표시
        ui->streamingLabel->setPixmap(scaledPixmap);
    }


}
/*FFmpeg을 종료하고 마지막 프레임을 고정*/
void Streaming::stopFFmpeg() {
    if (ffmpegProcess->state() == QProcess::Running) {
        frameTimer->stop(); // 프레임 캡처 중지
        ffmpegProcess->terminate(); // 프로세스 종료 요청
        if (!ffmpegProcess->waitForFinished(3000)) { // 3초 대기
            ffmpegProcess->kill(); // 강제 종료
        }
        qDebug() << "FFmpeg process stopped.";
    } else {
        qDebug() << "FFmpeg is not running.";
    }
    updateUIState(false); // Start 버튼 활성화, Stop 버튼 비활성화
}

/*FFmpeg 프로세스의 에러로그를 읽어서 출력*/
void Streaming::processOutput() {
    QByteArray errorOutput = ffmpegProcess->readAllStandardError();
    if (!errorOutput.isEmpty()) {
            qDebug() << "FFmpeg Error:" << errorOutput;
        }
}

/*startButton과 stopButton의 활성화 상태를 FFmpeg 프로세스의 실행 상태에 따라 동기화*/
void Streaming::updateUIState(bool isRunning) {
    //bool isRunning = (ffmpegProcess->state() == QProcess::Running);
    ui->startButton->setEnabled(!isRunning);
    ui->stopButton->setEnabled(isRunning);

    // QLineEdit 내용 유지 (SetButton 상태만 변경)
    if (!isRunning) {
        ui->setButton->setChecked(false); // 스트리밍이 멈추면 SetButton 상태 해제
    }
}

/*UI에서 입력된 RTSP주소 가져오기*/
void Streaming::rtsp_setting(){
    // QLineEdit에서 IP 주소 가져오기
    QString inputAddress = ui->addressInput->text().trimmed();
    if (inputAddress.isEmpty()) {
        ui->setButton->setChecked(false); // 잘못된 입력 시 SetButton 상태 해제
        QMessageBox* msgBox = new QMessageBox(QMessageBox::Warning, "Error", "Please enter a valid IP address.", QMessageBox::Ok, this);
        msgBox->setStyleSheet("QLabel { color : black; }"); // 글씨 색상 변경
        msgBox->exec();
        ui->setButton->setChecked(false); // 잘못된 입력 시 SetButton 상태 해제
        delete msgBox; // 메모리 관리
        return;
    }

    // RTSP URL 생성
    rtspUrl = QString("rtsp://%1:8554/stream1").arg(inputAddress);
    qDebug() << "RTSP URL set to:" << rtspUrl;

    // SetButton 클릭 상태 유지
    ui->setButton->setChecked(true);

    QMessageBox* msgBox = new QMessageBox(QMessageBox::Information, "Success", "RTSP URL updated successfully.", QMessageBox::Ok, this);
    msgBox->setStyleSheet("QLabel { color : black; }"); // 글씨 색상 변경
    msgBox->exec();
    delete msgBox; // 메모리 관리

}

void Streaming::on_setButton_clicked()
{
    rtsp_setting();

}

void Streaming::setupAddressInput() {
    ui->addressInput->installEventFilter(this); // 이벤트 필터 등록
}

bool Streaming::eventFilter(QObject *watched, QEvent *event) {
    if (watched == ui->addressInput) {
        if (event->type() == QEvent::FocusIn) { // 입력 필드에 포커스가 들어올 때
            ui->addressInput->clear();         // 텍스트 초기화
        }
    }
    return QWidget::eventFilter(watched, event); // 기본 동작 유지
}


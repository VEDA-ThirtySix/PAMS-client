#include "streaming.h"
#include "ui_streaming.h"
#include "metadata.h"
#include "tcpManager.h"
#include "jsonManager.h"
#include "userManager.h"
#include "dbManager.h"
#include <QDebug>
#include <QPixmap>
#include <QImage>
#include <QLayout>

#include <QTimer>
#include <QDateTime>
#include <QMessageBox>

#define protocol "rtsp"
#define port 8554

Streaming::Streaming(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Streaming)
    , timer(new QTimer(this))
    , ffmpegProcess(new QProcess(this))
    , frameTimer(new QTimer(this))
    , m_rtspUrl("") // 초기 URL 비우기
{
    ui->setupUi(this);
    ui->label_url->setAlignment(Qt::AlignCenter); //텍스트를 중앙에 배치
    // 타이머로 날짜/시간 업데이트
    connect(timer, &QTimer::timeout, this, &Streaming::updateDateTime);
    timer->start(1000); //1초마다 실행
    updateDateTime();// 초기 날짜/시간 표시

    connect(frameTimer, &QTimer::timeout, this, &Streaming::captureFrame);
    TcpManager& tcpManager = TcpManager::instance();
    connect(&tcpManager, &TcpManager::plateDataReceived, this, &Streaming::on_plateDataReceived);}

Streaming::~Streaming()
{
    if (ffmpegProcess->state() == QProcess::Running) {
        ffmpegProcess->terminate();
        if (!ffmpegProcess->waitForFinished(3000)) { // 3초 대기
                        ffmpegProcess->kill(); // 종료 실패 시 강제 종료
                    }
    }
    delete ui;
    //delete mainWindow;
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



/*RTSP스트림을 가져온다.*/
void Streaming::startFFmpeg() {

    if (ffmpegProcess->state() == QProcess::Running) {
        qDebug() << "FFmpeg is already running!";
        return;
    }

    if (m_rtspUrl.isEmpty()) {
        //ui->messageInput->text().isEmpty()
        QMessageBox::warning(this, "Error", "RTSP URL is not set. Please set the address first.");
        return;
    }

#ifdef Q_OS_MAC
    QString ffmpegPath = "/opt/homebrew/bin/ffmpeg"; // macOS의 FFmpeg 경로
#elif defined(Q_OS_LINUX)
    QString ffmpegPath = "/usr/bin/ffmpeg"; // Linux의 일반적인 FFmpeg 경로
#endif

    //FFMPEG 실행시 전달되는 옵션
    QStringList arguments = {
        "-protocol_whitelist","file,tcp,udp,rtp,rtsp",
        "-i",m_rtspUrl, //동적으로 설정된 RTSP URL
        "-vf", "fps=30",                    // 출력 프레임 속도 설정
        "-s", "800x600",                         // 출력 영상의 해상도 설정
        "-f","rawvideo",
        "-pix_fmt","rgb24",  // 픽셀 포맷: RGB888
        //"-pix_fmt", "yuvj420p",  // 색 공간 최적화
        "-loglevel","debug",
        "-"
    };


    ffmpegProcess->start(ffmpegPath, arguments);

    if (!ffmpegProcess->waitForStarted()) {
        qDebug() << "FFmpeg failed to start!";
        return;
    }
    frameTimer->start(33); // 30fps (1000ms /33 = 30ms)
    qDebug() << "FFmpeg started with URL:" << m_rtspUrl;

    setButtonStyle(ui->startButton, true);
    setButtonStyle(ui->stopButton, false);

}

void Streaming::captureFrame() {
    while (ffmpegProcess->bytesAvailable() > 0) {
        // FFmpeg에서 데이터 읽기
        QByteArray frameData = ffmpegProcess->readAllStandardOutput();
        if (frameData.isEmpty()) {
            qDebug() << "No frame data received!";
            return;
        }
        // 이전에 누적된 데이터와 합치기
        incompleteBuffer.append(frameData); // 클래스의 멤버변수

        // 한 프레임 크기 계산 (RGB888)
        const int width = 800;
        const int height = 600;
        const int bytesPerPixel = 3;
        const int frameSize = width * height * bytesPerPixel;

        // 버퍼가 프레임 크기 이상인지 확인
        while (incompleteBuffer.size() >= frameSize) {
            QByteArray singleFrameData = incompleteBuffer.left(frameSize);
            incompleteBuffer.remove(0, frameSize);

            // QImage 생성
            QImage image(reinterpret_cast<const uchar*>(singleFrameData.data()),
                         width, height, QImage::Format_RGB888);

            // QLabel에 맞게 크기 조정
            QPixmap scaledPixmap = QPixmap::fromImage(image).scaled(
                ui->streamingLabel->size(),
                Qt::KeepAspectRatioByExpanding,
                Qt::SmoothTransformation
            );

            // QLabel에 QPixmap 설정
            ui->streamingLabel->setPixmap(scaledPixmap);
        }
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

    setButtonStyle(ui->startButton, false);
    setButtonStyle(ui->stopButton, true);
}

/*FFmpeg 프로세스의 에러로그를 읽어서 출력*/
void Streaming::processOutput() {
    QByteArray errorOutput = ffmpegProcess->readAllStandardError();
    if (!errorOutput.isEmpty()) {
            qDebug() << "FFmpeg Error:" << errorOutput;
        }
}


/*UI에서 입력된 RTSP주소 가져오기*/
void Streaming::get_host(const QString& host) {
    m_host = host;
}

void Streaming::rtsp_setting(){

    // RTSP URL(QString) 생성
    m_rtspUrl = QString("%1://%2:%3/")
                    .arg(protocol)
                    .arg(m_host)
                    .arg(port);
    ui->label_url->setText(m_rtspUrl);
    ui->label_url->setStyleSheet("color:rgb(243,115,33);border:1px solid rgb(243,115,33);");
    qDebug() << "QDEBUG(SW)$ RTSP URL:" << m_rtspUrl;
}

void Streaming::on_startButton_clicked(){
    startFFmpeg();
    ui->car_on->raise(); // Test용
    updateGateState(true);
}

void Streaming::on_stopButton_clicked(){
    stopFFmpeg();
    ui->car_off->raise(); // Test용
    updateGateState(false);
}

/*START, STOP 버튼 true, false에 따른 스타일 지정 함수*/
void Streaming::setButtonStyle(QPushButton* button, bool isActive) {
    if (isActive) {
        button->setChecked(true);
        button->setStyleSheet("color: white; \
                              background-color:rgba(243,115,33,0.7); \
                              border:1px solid rgb(243,115,33); \
                              font: 500 9pt 'Quicksand Medium';");
    } else {
        button->setChecked(false);
        button->setStyleSheet("color: rgb(243,115,33); \
                              background:none; \
                              border:1px solid rgb(243,115,33); \
                              font: 500 9pt 'Quicksand Medium';");
    }
}

void Streaming::carEntryState(bool isActive){
    if (isActive) {
        ui->car_on->raise(); // 차량 진입 상태: car_on 라벨을 전면으로 표시
    } else {
        ui->car_off->raise(); // 차량 미진입 상태: car_off 라벨을 전면으로 표시
    }
}
void Streaming::updateGateState(bool state) {
    // 이미지 경로
    QString trueImagePath = ":/images/gate_open.png"; // true 상태의 이미지
    QString falseImagePath = ":/images/gate_close.png"; // false 상태의 이미지
    // 상태에 따라 이미지 로드
    QPixmap pixmap = state ? QPixmap(trueImagePath) : QPixmap(falseImagePath);
    // QLabel 크기를 가져오기
    QSize labelSize = ui->gate_state->size();
    // QLabel에 이미지를 설정 (픽셀 정보는 유지)
    ui->gate_state->setPixmap(pixmap.scaled(labelSize, Qt::KeepAspectRatio, Qt::FastTransformation));
    ui->gate_state->setScaledContents(false); // QLabel의 스케일 조정 비활성화
    
void Streaming::on_plateDataReceived(const QByteArray& buffer) {
    qDebug() << "on_plateDataReceived";
    m_receivedBuffer = buffer;
    update_InfoLabel();
}

void Streaming::update_InfoLabel() {
    qDebug() << "update_InfoLabel";
    TimeInfo timeInfo;
    JsonManager *jsonManager = new JsonManager(this);
    timeInfo = jsonManager->parse_data(m_receivedBuffer);
    QString received_plate = timeInfo.get_plate();
    qDebug() << "received_plate: " << received_plate;

    BasicInfo basicInfo;
    UserManager *userManager = new UserManager(this);
    basicInfo = userManager->getCurrentInfo(received_plate);

    QString name = basicInfo.get_name();
    QString plate = basicInfo.get_plate();
    QString home = basicInfo.get_home();
    QString phone = basicInfo.get_phone();
    qDebug() << "name : " << name;
    qDebug() << "plate: " << plate;

    DBManager& dbManager = DBManager::instance();
    bool isCustomer = dbManager.find_plate(plate);

    QString customerInfo;
    if(isCustomer) {
        qDebug() << "Customer Found!";
        customerInfo = QString(
           "[ 입주민 정보 ]\n\n"
           "  이름 : %1\n"
           "차량번호: %2\n"
           "  주소 : %3\n"
           "전화번호: %4"
           ).arg(name, plate, home, phone);
    } else {
        qDebug() << "Who are you?";
        customerInfo = "미등록 차량입니다";
    }

    ui->label->setText(customerInfo);
}



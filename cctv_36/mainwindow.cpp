#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "search.h"
#include <QDateTime>
#include <QMessageBox>
#include <QImage>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 타이머 설정
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateDateTime);
    timer->start(1000);

    streamSocket = new QTcpSocket(this);

    connect(streamSocket, &QTcpSocket::connected, this, &MainWindow::onConnected);
    connect(streamSocket, &QTcpSocket::disconnected, this, &MainWindow::onDisconnected);
    connect(streamSocket, &QTcpSocket::readyRead, this, &MainWindow::readStream);
    connect(streamSocket, &QAbstractSocket::errorOccurred, this, &MainWindow::handleError);

    ui->streamingLabel->setMinimumSize(FRAME_WIDTH, FRAME_HEIGHT);
    ui->streamingLabel->setAlignment(Qt::AlignCenter);

    setupStreamingConnection();

    setupSearch();

    // 초기 날짜/시간 표시
    updateDateTime();

}

MainWindow::~MainWindow()
{
    if (streamSocket->state() == QAbstractSocket::ConnectedState) {
            streamSocket->disconnectFromHost();
        }

    delete ui;
}


void MainWindow::setupStreamingConnection()
{
    const QString serverIP = "192.168.0.39";
    const quint16 serverPort = 5100;

    streamSocket->connectToHost(serverIP, serverPort);
}

void MainWindow::onConnected()
{
    qDebug() << "Connected to streaming server";
}

void MainWindow::onDisconnected()
{
    qDebug() << "Disconnected from streaming server";
    QTimer::singleShot(1000, this, &MainWindow::reconnectToStream);
}

void MainWindow::readStream()
{
    frameBuffer.append(streamSocket->readAll());

    // 프레임을 수신했는지 확인
    while (frameBuffer.size() >= FRAME_SIZE) {
        QByteArray frameData = frameBuffer.left(FRAME_SIZE);
        frameBuffer.remove(0, FRAME_SIZE);

        processYUYVFrame(frameData);
    }
}
void MainWindow::processYUYVFrame(const QByteArray &frameData)
{
    // YUYV 데이터를 RGB로 변환
    QImage image(FRAME_WIDTH, FRAME_HEIGHT, QImage::Format_RGB888);
    const uchar *data = reinterpret_cast<const uchar*>(frameData.constData());

    for (int y = 0; y < FRAME_HEIGHT; y++) {
        for (int x = 0; x < FRAME_WIDTH; x += 2) {
            int i = (y * FRAME_WIDTH + x) * 2;
            int y0 = data[i];
            int u = data[i + 1];
            int y1 = data[i + 2];
            int v = data[i + 3];

            // YUYV를 RGB로 변환하는 공식
            int r0 = qBound(0, static_cast<int>(y0 + 1.402 * (v - 128)), 255);
            int g0 = qBound(0, static_cast<int>(y0 - 0.344136 * (u - 128) - 0.714136 * (v - 128)), 255);
            int b0 = qBound(0, static_cast<int>(y0 + 1.772 * (u - 128)), 255);

            int r1 = qBound(0, static_cast<int>(y1 + 1.402 * (v - 128)), 255);
            int g1 = qBound(0, static_cast<int>(y1 - 0.344136 * (u - 128) - 0.714136 * (v - 128)), 255);
            int b1 = qBound(0, static_cast<int>(y1 + 1.772 * (u - 128)), 255);

            image.setPixelColor(x, y, QColor(r0, g0, b0));
            image.setPixelColor(x + 1, y, QColor(r1, g1, b1));
        }
    }

    ui->streamingLabel->setPixmap(QPixmap::fromImage(image));
}
void MainWindow::handleError(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        qDebug() << "Connection closed by server";
        break;
    case QAbstractSocket::HostNotFoundError:
        QMessageBox::warning(this, tr("Streaming Client"),
                             tr("The host was not found."));
        break;
    case QAbstractSocket::ConnectionRefusedError:
        QMessageBox::warning(this, tr("Streaming Client"),
                             tr("The connection was refused."));
        break;
    default:
        QMessageBox::warning(this, tr("Streaming Client"),
                             tr("Error: %1.")
                                 .arg(streamSocket->errorString()));
    }
}
void MainWindow::reconnectToStream()
{
    if (streamSocket->state() == QAbstractSocket::UnconnectedState) {
        setupStreamingConnection();
    }
}

void MainWindow::setupSearch()
{
    // Search 클래스 초기화 - Search 탭의 위젯들 연결
    searchManager = new Search(ui->searchInput,    // LineEdit
                               ui->searchButton,     // Search 버튼
                               ui->resultsList,      // 결과 리스트
                               this);
}

void MainWindow::updateDateTime()
{
    QDateTime current = QDateTime::currentDateTime();

    // 요일 한글 변환
    QMap<int, QString> weekdays;
    weekdays[1] = "월";
    weekdays[2] = "화";
    weekdays[3] = "수";
    weekdays[4] = "목";
    weekdays[5] = "금";
    weekdays[6] = "토";
    weekdays[7] = "일";

    QString weekday = weekdays[current.date().dayOfWeek()];

    // 오전/오후 판단
    QString ampm = current.time().hour() < 12 ? "AM" : "PM";

    // 12시간제로 변환
    int hour = current.time().hour() % 12;
    if (hour == 0) hour = 12;

    // 날짜/시간 문자열 생성
    QString dateTimeStr = QString::asprintf("%04d.%02d.%02d(%s) | %s %02d:%02d:%02d",
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

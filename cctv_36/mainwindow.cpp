#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "search.h"
#include <QDateTime>
#include <QMessageBox>
#include <QImage>
#include <QTabBar>

#include <QMenu>
#include <QAction>
#include <QFileDialog>
#include <QIcon>
#include <QMenuBar>
#include <QVBoxLayout>



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
    updateDateTime();// 초기 날짜/시간 표시

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
    const QString serverIP = "192.168.0.40";
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
        if (!errorMessageShown) {
            qDebug() << "The host was not found";
            QMessageBox::warning(this, tr("Streaming Client"), tr("The host was not found."));
            errorMessageShown = true;
        }
        break;
    case QAbstractSocket::ConnectionRefusedError:
        if (!errorMessageShown) {
            qDebug() << "The connection was refused";
            QMessageBox::warning(this, tr("Streaming Client"), tr("The connection was refused."));
            errorMessageShown = true;
        }
        break;
    default:
        break;
    }
    
    QTimer::singleShot(500, this, &MainWindow::reconnectToStream);
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
    searchManager = new Search(ui->searchInput,      // 검색어
                               ui->searchButton,     // 검색 버튼
                               ui->resultsTable,     // 데이터 테이블
                               ui->imageLabel,       // 차량번호판 이미지
                               ui->filterButton,     // 검색 필터링 버튼
                               ui->textLabel,
                               this);
}

void MainWindow::updateDateTime()
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

// /* 로컬 이미지를 데이터베이스에서 받아오기*/

// void MainWindow::initDatabase() {                               // 데이터베이스 초기화
//     db = QSqlDatabase::addDatabase("QSQLITE");              // SQLite 데이터베이스 사용
//     db.setDatabaseName("shyun_test.db");                       // 데이터베이스 파일 이름 설정 (build 폴더에 저장되어 있음)

//     if (!db.open()) {
//         qDebug() << "Error: Unable to open database." << db.lastError().text();
//         return;                                             // 데이터베이스가 열리지 않으면 종료
//     }

//     else {
//         QSqlQuery image;                                       // 메세지 테이블 처리할 쿼리
//         if (!image.exec("CREATE TABLE IF NOT EXISTS logins ("
//                         "id TEXT NOT NULL,"
//                         "image BLOB NOT NULL")) {
//             qDebug() << "Error creating logins table:" << image.lastError().text();
//         }
//         qDebug() << "Database initialized successfully.";
//     }
// }


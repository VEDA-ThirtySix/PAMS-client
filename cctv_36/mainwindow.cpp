#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "search.h"
#include <QDateTime>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 타이머 설정
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateDateTime);
    timer->start(1000);

    setupSearch();

    // 초기 날짜/시간 표시
    updateDateTime();

}

MainWindow::~MainWindow()
{
    delete ui;
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
    QString ampm = current.time().hour() < 12 ? "오전" : "오후";

    // 12시간제로 변환
    int hour = current.time().hour() % 12;
    if (hour == 0) hour = 12;

    // 날짜/시간 문자열 생성
    QString dateTimeStr = QString::asprintf("%04d.%02d.%02d(%s) \n %s %02d:%02d:%02d",
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

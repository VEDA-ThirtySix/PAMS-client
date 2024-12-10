#include "search.h"
#include "ui_search.h"
#include "dialog_edit.h"
#include "dialog_enroll.h"
#include "httpManager.h"
#include "dialog_videoclip.h"

#include <QMessageBox>
#include <QPixmap>
#include <QtWidgets/qmenu.h>
#include <QItemSelectionModel>  //KIYUN_1127
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QIdentityProxyModel>

#define protocol "http"
#define port 8080

class CustomProxyModel : public QIdentityProxyModel {
public:
    using QIdentityProxyModel::QIdentityProxyModel;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override {
        if (index.column() == 4 && role == Qt::DisplayRole) {
            return QString("plate%1.png").arg(index.row() + 1);
        }
        return QIdentityProxyModel::data(index, role);
    }
};

Search::Search(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Search)
    , userManager(new UserManager(this))
{
    ui->setupUi(this);

    m_currentSearchType = "차량번호"; // 기본 검색 타입

    setupConnections();
    setupCustomerTable();
    setupVideoTable();
    setupImage();
    updatePlaceholder();
    initializePath();

    connect(ui->pushButton_enroll, &QPushButton::clicked, this, &Search::clicked_buttonEnroll);
    connect(ui->pushButton_edit, &QPushButton::clicked, this, &Search::clicked_buttonEdit);
    //connect(ui->pushButton_delete, &QPushButton::clicked, this, &Search::clicked_buttonDelete);
    connect(ui->customerTable->selectionModel(), &QItemSelectionModel::selectionChanged, this, &Search::selectCustomerInfo);
    connect(ui->pushButton_delete, &QPushButton::clicked, this, &Search::clicked_buttonDelete);

}

Search::~Search() {
    delete ui;
    //delete mainWindow;
}

void Search::setupCustomerTable() {
    // DB 초기화
    //userManager->initiallize();

    // Basic 테이블 모델 설정
    m_db = userManager->getDatabase();
    qDebug() << "DONE(SM): got DB:" << m_db;

    QSqlQuery query1(m_db);
    query1.exec("SELECT count(*) FROM sqlite_master WHERE type='table' AND name='Basic'");
    if (query1.next()) {
        qDebug() << "Table exists:" << query1.value(0).toInt();
    }

    m_modelBasic = new QSqlTableModel(this, m_db);
    m_modelBasic->setTable("Basic");  //연결하려는 테이블명
    m_modelBasic->setEditStrategy(QSqlTableModel::OnManualSubmit);

    if(!m_modelBasic->select()) {
        qDebug() << "Error: " << m_modelBasic->lastError().text();
    }

    qDebug() << "Row count:" << m_modelBasic->rowCount();
    qDebug() << "Column count:" << m_modelBasic->columnCount();
    qDebug() << "Table: " << m_modelBasic->tableName();
    qDebug() << "is Valid: " << m_modelBasic->database().isValid();

    QSqlQuery query(m_db);
    if (query.exec("SELECT * FROM Basic")) {
        while (query.next()) {
            qDebug() << "Data:" << query.value(0).toString() << query.value(1).toString();
        }
    }

    // 열 헤더 설정
    m_modelBasic->setHeaderData(0, Qt::Horizontal, "NAME");
    m_modelBasic->setHeaderData(1, Qt::Horizontal, "PLATE");
    m_modelBasic->setHeaderData(2, Qt::Horizontal, "HOME");
    m_modelBasic->setHeaderData(3, Qt::Horizontal, "PHONE");


    ui->customerTable->setModel(m_modelBasic);
    ui->customerTable->resizeColumnsToContents();
    //m_resultsTable->hideColumn(0); // ID 컬럼 숨기기
    ui->customerTable->setEditTriggers(QAbstractItemView::NoEditTriggers); // 컬럼 수정 불가
    ui->customerTable->setSelectionBehavior(QAbstractItemView::SelectRows);


    // 열 너비 설정
    /*
    ui->resultsTable->setColumnWidth(2, 150); //HOME
    ui->resultsTable->setColumnWidth(3, 150); //PHONE   */
}

void Search::setupVideoTable() {

    m_db = DBManager::instance().getDatabase();

    m_modelTime = new QSqlTableModel(this, m_db);
    m_modelTime->setTable("Time");
    m_modelTime->setEditStrategy(QSqlTableModel::OnManualSubmit);

    if(!m_modelTime->select()) {
        qDebug() << "Time Table Error: " << m_modelTime->lastError().text();
    }

    // Time 테이블의 컬럼에 맞게 헤더 설정
    m_modelTime->setHeaderData(0, Qt::Horizontal, "ID");
    m_modelTime->setHeaderData(1, Qt::Horizontal, "PLATE");
    m_modelTime->setHeaderData(2, Qt::Horizontal, "TIME");
    m_modelTime->setHeaderData(3, Qt::Horizontal, "TYPE");
    m_modelTime->setHeaderData(4, Qt::Horizontal, "IMAGE");

    auto* proxyModel = new CustomProxyModel(this);
    proxyModel->setSourceModel(m_modelTime);
    ui->videoTable->setModel(proxyModel);
    ui->videoTable->resizeColumnsToContents();
    ui->videoTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->videoTable->setSelectionBehavior(QAbstractItemView::SelectRows);

    connect(ui->videoTable->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, [this](const QItemSelection &selected, const QItemSelection &deselected) {
                Q_UNUSED(deselected);

                if (selected.indexes().isEmpty()) {
                    ui->imageLabel_2->clear();
                    return;
                }

                int row = selected.indexes().first().row();
                QByteArray imageData = m_modelTime->data(m_modelTime->index(row, 4)).toByteArray();

                if (!imageData.isEmpty()) {
                    QImage image;
                    if (image.loadFromData(imageData)) {
                        QPixmap pixmap = QPixmap::fromImage(image);
                        ui->imageLabel_2->setPixmap(pixmap);
                        qDebug() << "DONE: Image loaded successfully";
                    } else {
                        ui->imageLabel_2->setText("이미지 로드 실패");
                        qDebug() << "ERROR: Failed to load image";
                    }
                } else {
                    ui->imageLabel_2->setText("이미지 없음");
                }
            });

    connect(ui->videoTable, &QTableView::doubleClicked, this, [this](const QModelIndex &index) {
        // if (m_host.isEmpty()) {
        //     QMessageBox warningBox;
        //     warningBox.setWindowTitle("연결 오류");
        //     warningBox.setText("호스트 주소가 설정되지 않았습니다. 먼저 주소를 설정해주세요.");
        //     warningBox.setStyleSheet(
        //         "QMessageBox { "
        //         "    background-color: white; "
        //         "    color: black; "
        //         "} "
        //         "QLabel { "
        //         "    color: black; "
        //         "} "
        //         "QPushButton { "
        //         "    background-color: rgba(243, 115, 33,0.5); "
        //         "    color: black; "
        //         "    width: 100px;"
        //         "    border: none; "
        //         "    padding: 5px; "
        //         "} "
        //         "QPushButton:hover { "
        //         "    background-color: rgba(190,190,190); "
        //         "} "
        //         );
        //     warningBox.exec();
        //     return;
        // }

        // Post Clip Request to User Server
        QModelIndex plateIndex = m_modelTime->index(index.row(), 1);    // PLATE column
        QModelIndex timeIndex = m_modelTime->index(index.row(), 2);     // TIME column
        QModelIndex typeIndex = m_modelTime->index(index.row(), 3);     // TYPE column
        QString plateValue = m_modelTime->data(plateIndex).toString();
        QDateTime timeValue = m_modelTime->data(timeIndex).toDateTime();
        QString typeValue = m_modelTime->data(typeIndex).toString();

        qDebug() << "VideoTable$ plate: " << plateValue;
        qDebug() << "VideoTable$ time : " << timeValue;
        qDebug() << "VideoTable$ type : " << typeValue;
        qDebug() << "VideoTable$ m_url: " << m_url;

        build_QUrl();
        TimeInfo timeInfo(plateValue, timeValue, typeValue);
        HttpManager *httpManager = new HttpManager(this);
        httpManager->post_clipInfo(m_url, timeInfo);

        // Create and show video dialog
        QString timestamp = timeValue.toString();
        VideoClipDialog *dialog = new VideoClipDialog(m_host, this);
        dialog->setAttribute(Qt::WA_DeleteOnClose);
        dialog->setWindowTitle(QString("Video Clip - %1").arg(timestamp));
        dialog->show();
    });




    setupCalendarWidget();
    connect(ui->calendarButton, &QPushButton::clicked, this, &Search::toggleCalendar);
}


void Search::setupImage()
{
    ui->imageLabel->setMinimumSize(320, 200);
    ui->imageLabel->setScaledContents(true);
    ui->imageLabel->setAlignment(Qt::AlignCenter);
    ui->imageLabel->setText("이미지 없음");

    ui->imageLabel_2->setMinimumSize(320, 200);
    ui->imageLabel_2->setScaledContents(true);
    ui->imageLabel_2->setAlignment(Qt::AlignCenter);
    ui->imageLabel_2->setText("이미지 없음");
}

/*
    m_db.transaction();
    for (const auto &example : examples) {
        // 주차 시간 계산
        qint64 seconds = example.entranceTime.secsTo(example.exitTime);
        int hours = seconds / 3600;
        int minutes = (seconds % 3600) / 60;
        int secs = seconds % 60;
        QString parkingDuration = QString("%1:%2:%3")
                                      .arg(hours, 2, 10, QLatin1Char('0'))
                                      .arg(minutes, 2, 10, QLatin1Char('0'))
                                      .arg(secs, 2, 10, QLatin1Char('0'));

        query.bindValue(0, example.name);
        query.bindValue(1, example.plateNumber);
        query.bindValue(2, example.entranceTime.toString("yyyy-MM-dd hh:mm:ss"));
        query.bindValue(3, example.exitTime.toString("yyyy-MM-dd hh:mm:ss"));
        query.bindValue(4, parkingDuration);
        query.exec();
    }
    m_db.commit();
    // 모델 새로고침
    m_model->select();
*/



void Search::setupConnections()
{
    // 검색 버튼 클릭 시 검색 수행
    connect(ui->searchButton, &QPushButton::clicked, this, &Search::performSearch);

    // Enter 키 입력 시 검색 수행
    connect(ui->searchInput, &QLineEdit::returnPressed, this, &Search::performSearch);

    // 텍스트 변경 시 실시간 검색
    connect(ui->searchInput, &QLineEdit::textChanged, this, &Search::handleSearchInput);

    // 테이블 뷰에 있는 컬럼 더블 클릭시 이벤트 연결
    // connect(ui->resultsTable, &QTableView::doubleClicked, this, &Search::handleDoubleClick);

    // 검색어 필터링
    connect(ui->filterButton, &QPushButton::clicked, this, &Search::showSearchMenu);
}

void Search::handleSearchInput(const QString &text)
{
    // 실시간 검색을 위한 로직
    if (text.length() >= 3) {  // 3글자 이상 입력시 자동 검색
        performSearch();
    } else if (text.isEmpty()){
        m_modelBasic->setFilter("");
        m_modelBasic->select();
    }
}

bool Search::initializePath() {
    projectDir = QDir::current();

    // images 디렉토리를 찾을 때까지 상위 디렉토리로 이동
    while (!projectDir.exists("images") && projectDir.cdUp()) {}

    if (projectDir.exists("images")) {
        imagesPath = projectDir.absoluteFilePath("images");
        qDebug() << "Found images directory at:" << imagesPath;
        return true;
    } else {
        qDebug() << "Warning: images 디렉토리를 찾을 수 없습니다.";
        return false;
    }
}

// void Search::handleDoubleClick(const QModelIndex &index) {
//     // qDebug() << "클릭된 컬럼 번호:" << index.column();

//     // QString basePath = "/home/kiyun/vFinal/rasp_project/cctv_36_ky/images";
//     // QString basePath = "/Users/taewonkim/GitHub/RaspberryPi-5-RTSP-Client/cctv_36/images"; // for taewon MacOS

//     // QString imagePath = QString("%1/image_%2.jpg")
//     //                         .arg(basePath)
//     //                         .arg(index.row() + 1);

//     QString imagePath = QDir(imagesPath).filePath(
//         QString("image_%1.jpg").arg(index.row() + 1)
//         );

//     /* 이미지와 데이터를 Qt에서 연결 -> 이미지를 db자체에서 연결(비트맵?이진화?) */
//     QPixmap image(imagePath);
//     if (image.isNull()) {
//         ui->imageLabel->setText("이미지 없음");
//         qDebug() << "이미지 로드 실패:" << imagePath;
//     } else {
//         ui->imageLabel->setPixmap(image);
//         qDebug() << "이미지 로드 성공:" << imagePath;
//     }

//     ui->resultsTable->selectRow(index.row());

// }

void Search::clearImage()
{
    ui->imageLabel->setPixmap(QPixmap());
    ui->imageLabel->setText("이미지 없음");
    ui->textLabel->setText("고객정보 없음");
}

void Search::performSearch()
{
    QString searchText = ui->searchInput->text();

    clearImage(); // 검색 시 이미지 초기화

    if (searchText.isEmpty()) {
        m_modelBasic->setFilter("");
    } else {
        QString filter;

        if (m_currentSearchType == "이름") {
            filter = QString("name LIKE '%%1%'").arg(searchText);
        }
        else if (m_currentSearchType == "차량번호") {
            filter = QString("plate LIKE '%%1%'").arg(searchText);
        }
        else if (m_currentSearchType == "시간") {
            // 일단은 입차시간을 검색하게 만듬
            filter = QString("entrance_time LIKE '%%1%'").arg(searchText);
        }
        m_modelBasic->setFilter(filter);
    }
    m_modelBasic->select();
}

void Search::updatePlaceholder()
{
    ui->searchInput->setPlaceholderText(QString("%1을(를) 입력하세요").arg(m_currentSearchType));
}

void Search::selectCustomerInfo(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(deselected);

    if (selected.indexes().isEmpty()) {
        ui->textLabel->setText("고객 정보");
        return;
    }

    int row = selected.indexes().first().row();

    // Get data from each row
    QString name = m_modelBasic->data(m_modelBasic->index(row, 0)).toString();
    QString plate = m_modelBasic->data(m_modelBasic->index(row, 1)).toString();
    QString home = m_modelBasic->data(m_modelBasic->index(row, 2)).toString();
    QString phone = m_modelBasic->data(m_modelBasic->index(row, 3)).toString();

    // Format the customer information
    QString customerInfo = QString(
                               "[ 고객 정보 ]\n\n"
                               "이름: %1\n"
                               "차량번호: %2\n"
                               "주소: %3\n"
                               "전화번호: %4"
                               ).arg(name, plate, home, phone);

    ui->textLabel->setText(customerInfo);


    QString imagePath = QDir(imagesPath).filePath(
        QString("image_%1.jpg").arg(row + 1)
        );

    /* 이미지와 데이터를 Qt에서 연결 -> 이미지를 db자체에서 연결(비트맵?이진화?) */
    QPixmap image(imagePath);
    if (image.isNull()) {
        ui->imageLabel->setText("이미지 없음");
        qDebug() << "이미지 로드 실패:" << imagePath;
    } else {
        ui->imageLabel->setPixmap(image);
        qDebug() << "이미지 로드 성공:" << imagePath;
    }

}


void Search::showSearchMenu()
{
    QMenu* menu = new QMenu;

    QAction* nameSearch = menu->addAction("이름으로 검색");
    QAction* plateSearch = menu->addAction("차량번호로 검색");
    QAction* timeSearch = menu->addAction("입차시간으로 검색");

    connect(nameSearch, &QAction::triggered, this, [this]() {
        clearImage();
        m_currentSearchType = "이름";
        ui->searchInput->clear();
        updatePlaceholder();
        m_modelBasic->setFilter("");
        m_modelBasic->select();
    });

    connect(plateSearch, &QAction::triggered, this, [this]() {
        clearImage();
        m_currentSearchType = "차량번호";
        ui->searchInput->clear();
        updatePlaceholder();
        m_modelBasic->setFilter("");
        m_modelBasic->select();
    });

    connect(timeSearch, &QAction::triggered, this, [this]() {
        clearImage();
        m_currentSearchType = "시간";
        ui->searchInput->clear();
        updatePlaceholder();
        m_modelBasic->setFilter("");
        m_modelBasic->select();
    });

    // filterButton 위치를 기준으로 메뉴 표시
    menu->exec(ui->filterButton->mapToGlobal(QPoint(0, ui->filterButton->height())));
}



/* KIYUN_1127 */
void Search::get_host(const QString& host) {
    m_host = host;
    qDebug() << "Host set to:" << m_host;
}

void Search::build_QUrl() {
    m_url.setHost(m_host);
    m_url.setPort(port);
    m_url.setScheme("http");
    QString url = m_url.toString();

    ui->label_url->setText(url);
    qDebug() << "build_QUrl$ m_url: " << m_url;
}

void Search::clicked_buttonEnroll() {
    build_QUrl();
    EnrollDialog *enrollDialog = new EnrollDialog(m_url, this);
    connect(enrollDialog, &EnrollDialog::dataSubmitted, this, &Search::refreshTable);

    enrollDialog->setAttribute(Qt::WA_DeleteOnClose);
    enrollDialog->exec();   //Enroll Dialog(Modal)
    qDebug() << "SUCCESS(SW): Open Enroll Dialog";
}


void Search::clicked_buttonEdit() {
    QString selectedPlate = get_selectedData();
    if(selectedPlate.isEmpty()) {
        //QMessageBox::warning(this, "수정 오류", "수정할 사용자를 선택해주세요.");
        QMessageBox warningBox;
        warningBox.setWindowTitle("수정 오류");
        warningBox.setText("수정할 사용자를 선택해주세요.");
        // 스타일 설정
        warningBox.setStyleSheet(
            "QMessageBox { "
            "    background-color: white; "                     // 전체 배경 흰색
            "    color: black; "
            "} "
            "QLabel { "
            "    color: black; "                    // 라벨 텍스트 주황색
            "} "
            "QPushButton { "
            "    background-color: rgba(243, 115, 33,0.5); "         // 버튼 배경 주황색
            "    color: black; "                                // 버튼 텍스트 흰색
            "    width :100px;"
            "    border: none; "                                // 버튼 테두리 없음
            "    padding: 5px; "                                // 버튼 안쪽 여백
            "} "
            "QPushButton:hover { "
            "    background-color:rgba(190,190,190);  "          // 버튼 호버 시 더 밝은 주황색
            "} "
            );
        warningBox.exec();

        return;
    }

    EditDialog *editDialog = new EditDialog(m_url, selectedPlate, this);
    editDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(editDialog, &EditDialog::dataUpdated, this, &Search::refreshTable);
    editDialog->exec();
}


QString Search::get_selectedData() {
    QModelIndex currentIndex = ui->customerTable->selectionModel()->currentIndex();
    if(!currentIndex.isValid()) {
        return QString();
    }
    return m_modelBasic->data(m_modelBasic->index(currentIndex.row(), 1)).toString();
}



void Search::clicked_buttonDelete() {
    QString selectedPlate = get_selectedData();
    if(selectedPlate.isEmpty()) {
        //QMessageBox::warning(this, "삭제 오류", "삭제할 데이터를 선택해주세요.");
        QMessageBox warningBox;
        warningBox.setWindowTitle("삭제 오류");
        warningBox.setText("삭제할 데이터를 선택해주세요.");
        warningBox.setStyleSheet(
            "QMessageBox { "
            "    background-color: white; "                     // 전체 배경 흰색
            "    color: black;"
            "} "
            "QLabel { "
            "    color: black; "                    // 라벨 텍스트 주황색
            "} "
            "QPushButton { "
            "    background-color: rgba(243, 115, 33,0.5);  "         // 버튼 배경 주황색
            "    color: black; "                                // 버튼 텍스트 흰색
            "    width :100px;"
            "    border: none; "                                // 버튼 테두리 없음
            "    padding: 5px; "                                // 버튼 안쪽 여백
            "} "
            "QPushButton:hover { "
            "    background-color:rgb(190,190,190);  "          // 버튼 호버 시 더 밝은 주황색
            "} "
            );
        warningBox.exec();
        return;
    }

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "삭제 확인", "선택한 데이터를 삭제하시겠습니까?", QMessageBox::Yes|QMessageBox::No);

    if(reply == QMessageBox::Yes) {
        if(userManager->deleteUser(selectedPlate)){
            refreshTable();
            clearImage();
            QMessageBox::information(this, "삭제 완료", "데이터가 성공적으로 삭제되었습니다.");
        } else {
            QMessageBox::warning(this, "삭제 실패", "데이터베이스 삭제에 실패했습니다.");
        }
    }

}
void Search::refreshTable() {
    qDebug() << "Search - refreshTable 시작";

    // 데이터베이스 재연결
    m_db = userManager->getDatabase();

    // 모델 재설정
    if(m_modelBasic) {
        delete m_modelBasic;
    }

    m_modelBasic = new QSqlTableModel(this, m_db);
    m_modelBasic->setTable("Basic");
    m_modelBasic->setEditStrategy(QSqlTableModel::OnManualSubmit);

    if(!m_modelBasic->select()) {
        qDebug() << "Search - 테이블 새로고침 실패:" << m_modelBasic->lastError().text();
        return;
    }
    qDebug() << "Search - 모델 select 성공";

    // 열 헤더 설정
    m_modelBasic->setHeaderData(0, Qt::Horizontal, "NAME");
    m_modelBasic->setHeaderData(1, Qt::Horizontal, "PLATE");
    m_modelBasic->setHeaderData(2, Qt::Horizontal, "HOME");
    m_modelBasic->setHeaderData(3, Qt::Horizontal, "PHONE");

    ui->customerTable->setModel(m_modelBasic);
    ui->customerTable->resizeColumnsToContents();

    connect(ui->customerTable->selectionModel(), &QItemSelectionModel::selectionChanged, this, &Search::selectCustomerInfo);

    qDebug() << "Search - refreshTable 완료";
}

void Search::setupCalendarWidget() {
    ui->calendarWidget->setGridVisible(true);
    ui->calendarWidget->setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);
    ui->calendarWidget->setHorizontalHeaderFormat(QCalendarWidget::SingleLetterDayNames);

    ui->calendarWidget->hide();

    // 달력 날짜 변경 시그널 연결
    connect(ui->calendarWidget, &QCalendarWidget::clicked,
            this, &Search::handleCalendarDateChanged);
}

void Search::toggleCalendar() {
    if (ui->calendarWidget->isHidden()) {
        ui->calendarWidget->show();
    } else {
        ui->calendarWidget->hide();
    }
}

void Search::handleCalendarDateChanged(const QDate& date) {

    QString startTime = date.toString("yyyy-MM-dd") + "T00:00:00.000";
    QString endTime = date.toString("yyyy-MM-dd") + "T23:59:59.999";

    // time 컬럼의 값이 선택된 날짜의 시작과 끝 사이에 있는 레코드만 필터링
    QString filter = QString("time >= '%1' AND time <= '%2'").arg(startTime, endTime);

    m_modelTime->setFilter(filter);

    if (!m_modelTime->select()) {
        qDebug() << "Failed to filter time data:" << m_modelTime->lastError().text();
    } else {
        qDebug() << "Successfully filtered for date:" << date.toString("yyyy-MM-dd");
        qDebug() << "Filter condition:" << filter;
        qDebug() << "Found" << m_modelTime->rowCount() << "records";
    }
}


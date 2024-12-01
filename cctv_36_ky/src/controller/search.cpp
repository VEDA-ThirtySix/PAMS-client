#include "search.h"
#include "ui_search.h"
#include "dialog_edit.h"
#include <QMessageBox>
#include <QPixmap>
#include <QtWidgets/qmenu.h>
#include <QItemSelectionModel>  //KIYUN_1127
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>

Search::Search(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Search)
    , userManager(new UserManager(this))
{
    ui->setupUi(this);
    m_currentSearchType = "차량번호"; // 기본 검색 타입

    setupConnections();
    setupTable();
    setupImage();
    lineEdit_test();
    updatePlaceholder();

    connect(ui->pushButton_edit, &QPushButton::clicked, this, &Search::clicked_buttonEdit);
    //connect(ui->pushButton_delete, &QPushButton::clicked, this, &Search::clicked_buttonDelete);

}

Search::~Search() {
    delete ui;
}

void Search::setupTable() {
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


    ui->resultsTable->setModel(m_modelBasic);
    ui->resultsTable->resizeColumnsToContents();
    //m_resultsTable->hideColumn(0); // ID 컬럼 숨기기
    ui->resultsTable->setEditTriggers(QAbstractItemView::NoEditTriggers); // 컬럼 수정 불가

    // 열 너비 설정
    /*
    ui->resultsTable->setColumnWidth(2, 150); //HOME
    ui->resultsTable->setColumnWidth(3, 150); //PHONE   */
}

void Search::setupImage()
{
    ui->imageLabel->setMinimumSize(320, 200);
    ui->imageLabel->setScaledContents(true);
    ui->imageLabel->setAlignment(Qt::AlignCenter);
    ui->imageLabel->setText("이미지 없음");
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
    connect(ui->resultsTable, &QTableView::doubleClicked, this, &Search::handleDoubleClick);

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

void Search::handleDoubleClick(const QModelIndex &index) {
    // qDebug() << "클릭된 컬럼 번호:" << index.column();

    QString basePath = "/home/kiyun/vFinal/rasp_project/cctv_36_ky/images";
    //QString basePath = "/Users/taewonkim/GitHub/PAMS-client/cctv_36/images";
    QString imagePath = QString("%1/image_%2.jpg")
                            .arg(basePath)
                            .arg(index.row() + 1);

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

void Search::clearImage()
{
    ui->imageLabel->setPixmap(QPixmap());
    ui->imageLabel->setText("이미지 없음");
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
QString Search::get_seletedData() {
    QModelIndex currentIndex = ui->resultsTable->selectionModel()->currentIndex();

    if(!currentIndex.isValid()) {
        qDebug() << "MSG: no selection";
        return QString();
    }

    int plateIndex_column = 1;

    QModelIndex plateIndex = ui->resultsTable->model()->index(currentIndex.row(), plateIndex_column);
    QString selected_plate = ui->resultsTable->model()->data(plateIndex).toString();

    qDebug() << "DONE(SE): selected column(plate): " << selected_plate;
    return selected_plate;
}


void Search::lineEdit_test() {
    ui->lineEdit_test->setText("hi");
}

void Search::clicked_buttonEdit() {
    EditDialog *editDialog = new EditDialog(this);
    editDialog->setAttribute(Qt::WA_DeleteOnClose);
    editDialog->exec();
    qDebug() << "DONE(MW): Open Edit Dialog";
}


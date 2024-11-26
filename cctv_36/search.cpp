#include "search.h"
#include <QMessageBox>
#include <QPixmap>
#include <QtWidgets/qmenu.h>

Search::Search(QLineEdit* searchInput,
               QPushButton* searchButton,
               QTableView* resultsTable,
               QLabel* imageLabel,
               QPushButton* filterButton,
               QObject *parent)
    : QObject(parent)
{
    m_searchInput = searchInput;
    m_searchButton = searchButton;
    m_resultsTable = resultsTable;
    m_filterButton = filterButton;
    m_imageLabel = imageLabel;
    m_currentSearchType = "차량번호"; // 기본 검색 타입

    setupConnections();
    setupDatabase();
    setupImage();

    updatePlaceholder();
}

bool Search::setupDatabase()
{
    // 데이터베이스 연결 설정
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    //m_db.setDatabaseName("vehicles.db");
    m_db.setDatabaseName("/Users/taewonkim/GitHub/PAMS-client/cctv_36/build/Qt_6_7_2_for_macOS-Debug/vehicles.db"); // for MacOS

    if (!m_db.open()) {
        QMessageBox::critical(nullptr, "Error", "데이터베이스 연결 실패!");
        return false;
    }

    QSqlQuery query;
    query.exec("DROP TABLE IF EXISTS vehicles");  // 테이블이 이미 존재하면 삭제

    // 테이블 생성
    query.exec("CREATE TABLE IF NOT EXISTS vehicles ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "name VARCHAR(20), "
               "plate_number VARCHAR(20), "
               "entrance_time TEXT, "
               "exit_time TEXT, "
               "parking_duration TEXT)");

    // 테이블 모델 설정
    m_model = new QSqlTableModel(this, m_db);
    m_model->setTable("vehicles");
    m_model->setEditStrategy(QSqlTableModel::OnManualSubmit);

    // 열 헤더 설정
    m_model->setHeaderData(1, Qt::Horizontal, "이름");
    m_model->setHeaderData(2, Qt::Horizontal, "차량번호");
    m_model->setHeaderData(3, Qt::Horizontal, "입차시간");
    m_model->setHeaderData(4, Qt::Horizontal, "출차시간");
    m_model->setHeaderData(5, Qt::Horizontal, "주차시간");

    m_resultsTable->setModel(m_model);
    m_resultsTable->hideColumn(0); // ID 컬럼 숨기기

    m_resultsTable->setEditTriggers(QAbstractItemView::NoEditTriggers); // 컬럼 수정 불가

    // 열 너비 설정
    m_resultsTable->setColumnWidth(3, 150);
    m_resultsTable->setColumnWidth(4, 150);
    m_resultsTable->setColumnWidth(5, 140);

    createExampleData();

    return true;
}

void Search::setupImage()
{
    m_imageLabel->setMinimumSize(320, 200);
    m_imageLabel->setScaledContents(true);
    m_imageLabel->setAlignment(Qt::AlignCenter);
    m_imageLabel->setText("이미지 없음");
}

void Search::createExampleData()
{

    // 기존 데이터 삭제
    QSqlQuery clearQuery;
    clearQuery.exec("DELETE FROM vehicles");

    QSqlQuery query;
    query.prepare("INSERT INTO vehicles (name, plate_number, entrance_time, exit_time, parking_duration) VALUES (?, ?, ?, ?, ?)");

    QDateTime current = QDateTime::currentDateTime();

    struct VehicleData {
        QString name;
        QString plateNumber;
        QDateTime entranceTime;
        QDateTime exitTime;
    };

    QList<VehicleData> examples = {
        {"김철수", "12가3456", current.addSecs(-3600), current},  // 1시간 주차
        {"이영희", "34나5678", current.addSecs(-7200), current},  // 2시간 주차
        {"박민수", "56다7890", current.addSecs(-18000), current}, // 5시간 주차
        {"정준호", "78라1234", current.addSecs(-25200), current}, // 7시간 주차
        {"최수진", "90마5678", current.addDays(-1), current},
        {"강동원", "12바9012", current.addDays(-2), current},
        {"조미영", "34사3456", current.addDays(-1), current.addDays(-1).addSecs(14400)},
        {"윤석진", "56아7890", current.addDays(-3), current.addDays(-2)},
        {"한지민", "78자1234", current.addDays(-4), current.addDays(-3)}
    };

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
}


void Search::setupConnections()
{
    // 검색 버튼 클릭 시 검색 수행
    connect(m_searchButton, &QPushButton::clicked, this, &Search::performSearch);

    // Enter 키 입력 시 검색 수행
    connect(m_searchInput, &QLineEdit::returnPressed, this, &Search::performSearch);

    // 텍스트 변경 시 실시간 검색
    connect(m_searchInput, &QLineEdit::textChanged, this, &Search::handleSearchInput);

    // 테이블 뷰에 있는 컬럼 더블 클릭시 이벤트 연결
    connect(m_resultsTable, &QTableView::doubleClicked, this, &Search::handleDoubleClick);

    // 검색어 필터링
    connect(m_filterButton, &QPushButton::clicked, this, &Search::showSearchMenu);

}

void Search::handleSearchInput(const QString &text)
{
    // 실시간 검색을 위한 로직
    if (text.length() >= 3) {  // 3글자 이상 입력시 자동 검색
        performSearch();
    } else if (text.isEmpty()){
        m_model->setFilter("");
        m_model->select();
    }
}

void Search::handleDoubleClick(const QModelIndex &index)

{

    // qDebug() << "클릭된 컬럼 번호:" << index.column();

    QString basePath = "/Users/taewonkim/GitHub/PAMS-client/cctv_36/images";
    QString imagePath = QString("%1/image_%2.jpg")
                            .arg(basePath)
                            .arg(index.row() + 1);

    QPixmap image(imagePath);
    if (image.isNull()) {
        m_imageLabel->setText("이미지 없음");
        qDebug() << "이미지 로드 실패:" << imagePath;
    } else {
        m_imageLabel->setPixmap(image);
        qDebug() << "이미지 로드 성공:" << imagePath;
    }
}

void Search::clearImage()
{
    m_imageLabel->setPixmap(QPixmap());
    m_imageLabel->setText("이미지 없음");
}

void Search::performSearch()
{
    QString searchText = m_searchInput->text();

    clearImage(); // 검색 시 이미지 초기화

    if (searchText.isEmpty()) {
        m_model->setFilter("");
    } else {
        QString filter;

        if (m_currentSearchType == "이름") {
            filter = QString("name LIKE '%%1%'").arg(searchText);
        }
        else if (m_currentSearchType == "차량번호") {
            filter = QString("plate_number LIKE '%%1%'").arg(searchText);
        }
        else if (m_currentSearchType == "시간") {
             // 일단은 입차시간을 검색하게 만듬
            filter = QString("entrance_time LIKE '%%1%'").arg(searchText);
        }
        m_model->setFilter(filter);
    }
    m_model->select();
}

void Search::updatePlaceholder()
{
    m_searchInput->setPlaceholderText(QString("%1을(를) 입력하세요").arg(m_currentSearchType));
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
        m_searchInput->clear();
        updatePlaceholder();
        m_model->setFilter("");
        m_model->select();
    });

    connect(plateSearch, &QAction::triggered, this, [this]() {
        clearImage();
        m_currentSearchType = "차량번호";
        m_searchInput->clear();
        updatePlaceholder();
        m_model->setFilter("");
        m_model->select();
    });

    connect(timeSearch, &QAction::triggered, this, [this]() {
        clearImage();
        m_currentSearchType = "시간";
        m_searchInput->clear();
        updatePlaceholder();
        m_model->setFilter("");
        m_model->select();
    });

    // filterButton 위치를 기준으로 메뉴 표시
    menu->exec(m_filterButton->mapToGlobal(QPoint(0, m_filterButton->height())));
}

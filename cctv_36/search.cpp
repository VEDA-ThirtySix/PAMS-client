#include "search.h"
#include <QMessageBox>
#include <QPixmap>

Search::Search(QLineEdit* searchInput,
               QPushButton* searchButton,
               QTableView* resultsTable,
               QLabel* imageLabel,
               QObject *parent)
    : QObject(parent)
{
    m_searchInput = searchInput;
    m_searchButton = searchButton;
    m_resultsTable = resultsTable;
    m_imageLabel = imageLabel;

    setupConnections();
    setupDatabase();
    setupImage();

    m_searchInput->setPlaceholderText("차량번호를 입력하세요");
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

    // 테이블 생성
    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS vehicles ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "plate_number VARCHAR(20), "
               "timestamp DATETIME)");

    // 테이블 모델 설정
    m_model = new QSqlTableModel(this, m_db);
    m_model->setTable("vehicles");
    m_model->setEditStrategy(QSqlTableModel::OnManualSubmit);

    // 열 헤더 설정
    m_model->setHeaderData(1, Qt::Horizontal, "차량번호");
    m_model->setHeaderData(2, Qt::Horizontal, "시간");

    m_resultsTable->setModel(m_model);
    m_resultsTable->hideColumn(0); // ID 컬럼 숨기기

    m_resultsTable->setEditTriggers(QAbstractItemView::NoEditTriggers); // 컬럼 수정 불가

    // 열 너비 설정
    m_resultsTable->setColumnWidth(1, 150);
    m_resultsTable->setColumnWidth(2, 200);

    createExampleData();

    return true;
}

void Search::setupImage()
{
    m_imageLabel->setMinimumSize(300, 200);
    m_imageLabel->setMaximumSize(300, 200);
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
    query.prepare("INSERT INTO vehicles (plate_number, timestamp) VALUES (?, ?)");

    QDateTime current = QDateTime::currentDateTime();
    QList<QPair<QString, QDateTime>> examples = {
        {QString("12가3456"), current.addSecs(-1800)},
        {QString("34나5678"), current.addSecs(-2700)},
        {QString("56다7890"), current.addSecs(-18000)},
        {QString("78라1234"), current.addSecs(-25200)},
        {QString("90마5678"), current.addDays(-1)},
        {QString("12바9012"), current.addDays(-1).addSecs(-14400)},
        {QString("34사3456"), current.addDays(-2)},
        {QString("56아7890"), current.addDays(-3)},
        {QString("78자1234"), current.addDays(-4)}
    };

    m_db.transaction();
    for (const auto &example : examples) {
        query.bindValue(0, example.first);
        query.bindValue(1, example.second.toString("yyyy-MM-dd hh:mm:ss"));
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

void Search::performSearch()
{
    QString searchText = m_searchInput->text();
    if (searchText.isEmpty()) {
        m_model->setFilter("");
    } else {
        QString filter = QString("plate_number LIKE '%%1%'").arg(searchText);
        m_model->setFilter(filter);
    }
    m_model->select();
}

#include "search.h"
#include <QMessageBox>

Search::Search(QLineEdit* searchInput,
               QPushButton* searchButton,
               QTableView* resultsTable,
               QObject *parent)
    : QObject(parent)
{
    m_searchInput = searchInput;
    m_searchButton = searchButton;
    m_resultsTable = resultsTable;

    setupConnections();
    setupDatabase();

    m_searchInput->setPlaceholderText("차량번호를 입력하세요");
}

bool Search::setupDatabase()
{
    // 데이터베이스 연결 설정
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName("vehicles.db");

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

    return true;
}

void Search::createExampleData()
{
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
    connect(m_searchButton, &QPushButton::clicked,
            this, &Search::performSearch);

    // Enter 키 입력 시 검색 수행
    connect(m_searchInput, &QLineEdit::returnPressed,
            this, &Search::performSearch);

    // 텍스트 변경 시 실시간 검색
    connect(m_searchInput, &QLineEdit::textChanged,
            this, &Search::handleSearchInput);
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

/*void Search::loadExampleData()
{
    // 현재 시간 기준으로 예시 데이터 생성
    QDateTime current = QDateTime::currentDateTime();

    // 다양한 시간대의 예시 데이터 생성
    QList<QPair<QString, QDateTime>> examples = {
        // 최근 1시간 내
        {QString("12가3456"), current.addSecs(-1800)},  // 30분 전
        {QString("34나5678"), current.addSecs(-2700)},  // 45분 전

        // 오늘 다른 시간대
        {QString("56다7890"), current.addSecs(-18000)}, // 5시간 전
        {QString("78라1234"), current.addSecs(-25200)}, // 7시간 전

        // 어제
        {QString("90마5678"), current.addDays(-1)},
        {QString("12바9012"), current.addDays(-1).addSecs(-14400)}, // 어제 4시간 전

        // 이번 주 다른 날들
        {QString("34사3456"), current.addDays(-2)},
        {QString("56아7890"), current.addDays(-3)},
        {QString("78자1234"), current.addDays(-4)}
    };

    // 리스트 위젯에 데이터 추가
    for (const auto &example : examples) {
        QString plateNumber = example.first;
        QDateTime timestamp = example.second;

        // 날짜/시간 포맷팅
        QString timeStr;
        qint64 secsTo = timestamp.secsTo(current);

        if (secsTo < 3600) {  // 1시간 이내
            int mins = secsTo / 60;
            timeStr = QString("%1분 전").arg(mins);
        } else if (secsTo < 86400) {  // 24시간 이내
            int hours = secsTo / 3600;
            timeStr = QString("%1시간 전").arg(hours);
        } else {  // 24시간 이상
            timeStr = timestamp.toString("yyyy-MM-dd hh:mm");
        }

        // 아이템 생성 및 추가
        QString displayText = QString("%1  |  %2").arg(plateNumber, timeStr);
        QListWidgetItem* item = new QListWidgetItem(displayText);

        // 아이템 스타일 설정
        item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);

        // 폰트 설정
        QFont font = item->font();
        font.setPointSize(10);
        item->setFont(font);

        m_resultsList->addItem(item);
    }
}*/

/*void Search::performSearch()
{
    QString searchText = m_searchInput->text();
    if (searchText.isEmpty()) {
        loadExampleData();  // 검색어가 비어있으면 전체 목록 표시
        return;
    }

    m_resultsList->clear();

    // 예시 데이터 필터링
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

    // 검색어에 맞는 결과만 필터링하여 표시
    for (const auto &example : examples) {
        if (example.first.contains(searchText, Qt::CaseInsensitive)) {
            QString timeStr;
            qint64 secsTo = example.second.secsTo(current);

            if (secsTo < 3600) {
                int mins = secsTo / 60;
                timeStr = QString("%1분 전").arg(mins);
            } else if (secsTo < 86400) {
                int hours = secsTo / 3600;
                timeStr = QString("%1시간 전").arg(hours);
            } else {
                timeStr = example.second.toString("yyyy-MM-dd hh:mm");
            }

            QString displayText = QString("%1  |  %2").arg(example.first, timeStr);
            QListWidgetItem* item = new QListWidgetItem(displayText);
            item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);

            QFont font = item->font();
            font.setPointSize(10);
            item->setFont(font);

            m_resultsList->addItem(item);
        }
    }
}*/

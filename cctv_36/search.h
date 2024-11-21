#ifndef SEARCH_H
#define SEARCH_H

#include <QObject>
#include <QLineEdit>
#include <QPushButton>
#include <QListWidget>
#include <QDateTime>
#include <QTableView>
#include <QLabel>

#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QSqlError>

class Search : public QObject
{
    Q_OBJECT

public:
    explicit Search(QLineEdit* searchInput,
                    QPushButton* searchButton,
                    QTableView* resultsTable,
                    QLabel* imageLabel,
                    QObject *parent = nullptr);

    void initializeDatabase();
    void createExampleData();

public slots:
    void performSearch();
    void handleSearchInput(const QString &text);
    void handleDoubleClick(const QModelIndex &index);

private:
    QLineEdit* m_searchInput;
    QPushButton* m_searchButton;
    QTableView* m_resultsTable;
    QSqlTableModel* m_model;
    QSqlDatabase m_db;
    QLabel* m_imageLabel;

    void setupConnections();
    bool setupDatabase();
    void setupImage();
};

#endif // SEARCH_H

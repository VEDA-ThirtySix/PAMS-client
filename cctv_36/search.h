#ifndef SEARCH_H
#define SEARCH_H

#include <QObject>
#include <QLineEdit>
#include <QPushButton>
#include <QListWidget>
#include <QDateTime>
#include <QTableView>
#include <QLabel>
#include <QMenu>

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
                    QPushButton* filterButton,
                    QLabel* textLabel,
                    QObject *parent = nullptr);

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
    QPushButton* m_filterButton;
    QString m_currentSearchType;
    QLabel* m_textLabel;

    void setupConnections();
    bool setupDatabase();
    void setupImage();
    void updatePlaceholder();
    void clearImageAndText();

signals:
    void dataSelected(const QString &name, const QString &plateNumber,
                      const QString &entranceTime, const QString &exitTime,
                      const QString &parkingDuration);

private slots:
    void showSearchMenu();

};

#endif // SEARCH_H

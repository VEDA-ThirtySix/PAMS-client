#ifndef SEARCH_H
#define SEARCH_H

#include <QObject>
#include <QLineEdit>
#include <QPushButton>
#include <QListWidget>
#include <QDateTime>

class Search : public QObject
{
    Q_OBJECT

public:
    explicit Search(QLineEdit* searchInput,
                    QPushButton* searchButton,
                    QListWidget* resultsList,
                    QObject *parent = nullptr);

public slots:
    void performSearch();
    void loadExampleData();
    void handleSearchInput(const QString &text);

private:
    QLineEdit* m_searchInput;
    QPushButton* m_searchButton;
    QListWidget* m_resultsList;

    void setupConnections();
};

#endif // SEARCH_H

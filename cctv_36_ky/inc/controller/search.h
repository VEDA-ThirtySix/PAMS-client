#ifndef SEARCH_H
#define SEARCH_H

#include "userManager.h"
#include "ui_search.h"

#include <QObject>
#include <QWidget>
#include <QSqlTableModel>

class Search : public QWidget
{
    Q_OBJECT

public:
    explicit Search(QWidget *parent = nullptr);
    ~Search();

    QString get_seletedData();   //KIYUN_1127


public slots:
    void performSearch();
    void handleSearchInput(const QString &text);
    void handleDoubleClick(const QModelIndex &index);
    void lineEdit_test();

private:
    Ui::Search *ui;
    UserManager *userManager;

    /* KIYUN 1129 */
    QString m_currentSearchType;
    QSqlTableModel* m_modelBasic;
    QSqlTableModel* m_modelTime;
    QSqlDatabase m_db;

    void setupConnections();
    void setupTable();
    void setupImage();
    void updatePlaceholder();
    void clearImage();

    void clicked_buttonEdit();
    void clicked_buttonDelete();

private slots:
    void showSearchMenu();

};

#endif // SEARCH_H

#ifndef SEARCH_H
#define SEARCH_H

#include "userManager.h"
#include "ui_search.h"

#include <QObject>
#include <QWidget>
#include <QSqlTableModel>
#include <QDir>

class Search : public QWidget
{
    Q_OBJECT

public:
    explicit Search(QWidget *parent = nullptr);
    ~Search();

    QString get_seletedData();   //KIYUN_1127
    void refreshTable();


public slots:
    void performSearch();
    void handleSearchInput(const QString &text);
    //void handleDoubleClick(const QModelIndex &index);
    //void lineEdit_test();

private:
    Ui::Search *ui;
    UserManager *userManager;

    /* KIYUN 1129 */
    QString m_currentSearchType;
    QSqlTableModel* m_modelBasic;
    QSqlTableModel* m_modelTime;
    QSqlDatabase m_db;
    QDir projectDir;
    QString imagesPath;
    bool initializePath();

    void setupConnections();
    void setupTable();
    void setupImage();
    void updatePlaceholder();
    void clearImage();


    void clicked_buttonEnroll();
    void clicked_buttonEdit();
    void clicked_buttonDelete();

private slots:
    void showSearchMenu();
    void selectCustomerInfo(const QItemSelection &selected, const QItemSelection &deselected);
};

#endif // SEARCH_H

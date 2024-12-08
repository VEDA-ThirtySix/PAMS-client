#ifndef SEARCH_H
#define SEARCH_H

#include "ui_search.h"
#include "userManager.h"

#include <QObject>
#include <QWidget>
#include <QSqlTableModel>
#include <QDir>
#include <QUrl>

class Search : public QWidget
{
    Q_OBJECT

public:
    explicit Search(QWidget *parent = nullptr);
    ~Search();

    QString get_selectedData();   //KIYUN_1127
    void refreshTable();

    //서버 연결 설정(metadata)
    void get_host(const QString& host);
    void build_QUrl();


public slots:
    void performSearch();
    void handleSearchInput(const QString &text);
    //void handleDoubleClick(const QModelIndex &index);

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

    QString m_host;
    QUrl m_url;

    QString currentName;
    QString currentPlate;
    QString currentHome;
    QString currentPhone;

    bool initializePath();

    void setupConnections();
    void setupCustomerTable();
    void setupVideoTable();
    void setupImage();
    void updatePlaceholder();
    void clearImage();

    void clicked_buttonConnect();
    void clicked_buttonEnroll();
    void clicked_buttonEdit();
    void clicked_buttonDelete();

    void setupCalendarWidget();


private slots:
    void showSearchMenu();
    void selectCustomerInfo(const QItemSelection &selected, const QItemSelection &deselected);
    void insertSampleTimeData();
    void handleCalendarDateChanged(const QDate& date);

};

#endif // SEARCH_H

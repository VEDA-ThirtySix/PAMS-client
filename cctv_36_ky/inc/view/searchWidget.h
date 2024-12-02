#ifndef SEARCHWIDGET_H
#define SEARCHWIDGET_H

#include "ui_search.h"
#include "userManager.h"

#include <QObject>
#include <QWidget>
#include <QSqlTableModel>
#include <QDir>

class SearchWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SearchWidget(QWidget *parent = nullptr);
    ~SearchWidget();

    QString get_seletedData();   //KIYUN_1127
    void refreshTable();
    bool initializePath();


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

    QDir projectDir;
    QString imagesPath;


    void updatePlaceholder();
    void clearImage();
    void setupConnections();
    void setupTable();
    void setupImage();


    void clicked_buttonEnroll();
    void clicked_buttonEdit();
    void clicked_buttonDelete();

private slots:
    void showSearchMenu();

};

#endif // SEARCHWIDGET_H

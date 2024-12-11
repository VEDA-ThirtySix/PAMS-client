#ifndef EDITDIALOG_H
#define EDITDIALOG_H

#include "userManager.h"
#include "httpManager.h"
#include "search.h"
#include <QDialog>
#include <QUrl>

QT_BEGIN_NAMESPACE
namespace Ui {
class EditDialog;
}
QT_END_NAMESPACE

class EditDialog : public QDialog {
    Q_OBJECT

public:
    //explicit EditDialog(QWidget *parent = nullptr);
    explicit EditDialog(QUrl url, QString plate, QWidget *parent = nullptr);
    ~EditDialog();

private:
    Ui::EditDialog *ui;
    UserManager *userManager;
    HttpManager *httpManager;
    QUrl m_url;
    QString m_selectedPlate;
    Search *search;

private slots:
    void clicked_buttonPrev();
    void clicked_buttonEdit();

signals:
    void dataUpdated();

};

#endif // EDITDIALOG_H

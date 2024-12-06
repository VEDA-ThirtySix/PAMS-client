#ifndef EDITDIALOG_H
#define EDITDIALOG_H

#include "userManager.h"
#include "search.h"
#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui {
class EditDialog;
}
QT_END_NAMESPACE

class EditDialog : public QDialog {
    Q_OBJECT

public:
    //explicit EditDialog(QWidget *parent = nullptr);
    explicit EditDialog(QString plate, QWidget *parent = nullptr);
    ~EditDialog();

    //void return_mainWindow();

private:
    Ui::EditDialog *ui;
    UserManager *userManager;
    QString m_selectedPlate;
    Search *search;
    void setupClearConnections();

private slots:
    void clicked_buttonPrev();
    void clicked_buttonEdit();

signals:
    void dataUpdated();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

};



#endif // EDITDIALOG_H

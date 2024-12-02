#ifndef DIALOG_EDIT_H
#define DIALOG_EDIT_H

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
    explicit EditDialog(QWidget *parent = nullptr);
    ~EditDialog();

    //void return_mainWindow();

private:
    Ui::EditDialog *ui;
    UserManager *userManager;
    Search *search;

private slots:
    void clicked_buttonPrev();
    void clicked_buttonEdit();
};

#endif // DIALOG_EDIT_H

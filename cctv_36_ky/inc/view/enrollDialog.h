#ifndef DIALOG_ENROLL_H
#define DIALOG_ENROLL_H

#include "userManager.h"
#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui {
class EnrollDialog;
}
QT_END_NAMESPACE

class EnrollDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EnrollDialog(QWidget *parent = nullptr);
    ~EnrollDialog();

private:
    Ui::EnrollDialog *ui;
    UserManager *userManager;

private slots:
    void clicked_buttonPrev();
    void clicked_buttonEnroll();

signals:
    void dataSubmitted();
};

#endif // DIALOG_ENROLL_H

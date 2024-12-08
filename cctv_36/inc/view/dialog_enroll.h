#ifndef DIALOG_ENROLL_H
#define DIALOG_ENROLL_H

#include "userManager.h"
#include "httpManager.h"
#include <QDialog>
#include <QUrl>

QT_BEGIN_NAMESPACE
namespace Ui {
class EnrollDialog;
}
QT_END_NAMESPACE

class EnrollDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EnrollDialog(QUrl url, QWidget *parent = nullptr);
    ~EnrollDialog();

private:
    Ui::EnrollDialog *ui;
    UserManager *userManager;
    HttpManager *httpManager;
    QUrl m_url;

private slots:
    void clicked_buttonPrev();
    void clicked_buttonEnroll();

signals:
    void dataSubmitted();
};

#endif // DIALOG_ENROLL_H

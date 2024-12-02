#ifndef NETWORKDIALOG_H
#define NETWORKDIALOG_H

#include "networkManager.h"

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui {
class NetworkDialog;
}
QT_END_NAMESPACE

class NetworkDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NetworkDialog(QWidget *parent = nullptr);
    ~NetworkDialog();

private:
    Ui::NetworkDialog *ui;
    NetworkManager *networkManager;

private slots:
    void clicked_buttonPrev();
    void clicked_buttonApply();

signals:
    void configChanged(const QString& url, const QString& port);
};

#endif // NETWORKDIALOG_H

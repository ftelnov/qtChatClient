#ifndef NOTIFICATIONDIALOG_H
#define NOTIFICATIONDIALOG_H

#include <QDialog>

namespace Ui {
class NotificationDialog;
}

class NotificationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NotificationDialog(QString message, QWidget *parent = nullptr);
    ~NotificationDialog();

private slots:
    void on_okButton_clicked();

private:
    Ui::NotificationDialog *ui;
};

#endif // NOTIFICATIONDIALOG_H

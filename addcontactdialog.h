#ifndef ADDCONTACTDIALOG_H
#define ADDCONTACTDIALOG_H

#include <QDialog>
#include <QNetworkReply>

namespace Ui {
class AddContactDialog;
}

class AddContactDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddContactDialog(QWidget *parent = nullptr);
    ~AddContactDialog();

private slots:
    void on_addContactButton_clicked();
    void addContactReplyFinished(QNetworkReply* reply);

private:
    Ui::AddContactDialog *ui;
    void pushNotification(QString message);
    QString baseUrl = "http://127.0.0.1:8000/";
};

#endif // ADDCONTACTDIALOG_H

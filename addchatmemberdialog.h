#ifndef ADDCHATMEMBERDIALOG_H
#define ADDCHATMEMBERDIALOG_H

#include <QDialog>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include "utils.h"
#include <QJsonDocument>

namespace Ui {
class AddChatMemberDialog;
}

class AddChatMemberDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddChatMemberDialog(QString hash, QWidget *parent = nullptr);
    ~AddChatMemberDialog();

private slots:
    void on_addMemberButton_clicked();
    void addMemberReplyFinished(QNetworkReply* reply);

private:
    Ui::AddChatMemberDialog *ui;
    QString hash;
};

#endif // ADDCHATMEMBERDIALOG_H

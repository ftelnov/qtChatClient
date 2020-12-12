#ifndef CHANGECHATNAMEDIALOG_H
#define CHANGECHATNAMEDIALOG_H

#include <QDialog>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include "utils.h"
#include <QJsonDocument>

namespace Ui {
class ChangeChatNameDialog;
}

class ChangeChatNameDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChangeChatNameDialog(QString hash, QWidget *parent = nullptr);
    ~ChangeChatNameDialog();

private slots:
    void on_changeNameButton_clicked();
    void changeNameReplyFinished(QNetworkReply* reply);

private:
    Ui::ChangeChatNameDialog *ui;
    QString hash;
};

#endif // CHANGECHATNAMEDIALOG_H

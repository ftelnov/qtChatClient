#include "addcontactdialog.h"
#include "ui_addcontactdialog.h"
#include "notificationdialog.h"
#include <QSettings>
#include <QNetworkReply>
#include <QJsonValue>
#include <QJsonValueRef>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "utils.h"

AddContactDialog::AddContactDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddContactDialog)
{
    ui->setupUi(this);
}

AddContactDialog::~AddContactDialog()
{
    delete ui;
}

void AddContactDialog::on_addContactButton_clicked()
{
    QSettings settings;
    QString nickname = this->ui->friendNickname->text();
    QMap<QString, QVariant> raw_data;
    raw_data["nickname"] = nickname;
    QNetworkRequest request = Utils::constructRequest("/contact/add/", Utils::getToken());
    QByteArray data = Utils::constructPostData(&raw_data);
    QNetworkAccessManager* accessManager = new QNetworkAccessManager(this);
    connect(accessManager, SIGNAL(finished(QNetworkReply*)), this,
        SLOT(addContactReplyFinished(QNetworkReply*)));
       accessManager->post(request, data);
    return;
}

void AddContactDialog::addContactReplyFinished(QNetworkReply* reply) {
    if(reply->error()) {
        Utils::pushNotification(this, "Something went wrong. We've got error: " + reply->errorString());
        return;
    }
    QJsonObject jsonReply = Utils::getJson(reply);
    if (jsonReply["result"] != 0) {
        Utils::pushNotification(this, jsonReply["error"].toString());
        return;
    }
    this->accept();
}

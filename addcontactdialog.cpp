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
    QString token = "Token " + settings.value("token").toString();
    QNetworkAccessManager* accessManager = new QNetworkAccessManager(this);
    QNetworkRequest request(QUrl(baseUrl + "api/contact/add/"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", token.toUtf8());
    QByteArray postData;
    postData.append(nickname);
    connect(accessManager, SIGNAL(finished(QNetworkReply*)), this,
        SLOT(addContactReplyFinished(QNetworkReply*)));
       accessManager->post(request, postData);
    return;
}

void AddContactDialog::addContactReplyFinished(QNetworkReply* reply) {
    if(reply->error()) {
        pushNotification("Something went wrong. We've got error: " + reply->errorString());
        return;
    }
    QByteArray buffer = reply->readAll();
    reply->deleteLater();
    QJsonDocument jsonDoc(QJsonDocument::fromJson(buffer));
    QJsonObject jsonReply = jsonDoc.object();
    if (jsonReply["result"] != 0) {
        pushNotification(jsonReply["error"].toString());
        return;
    }
    this->accept();
}

void AddContactDialog::pushNotification(QString message) {
    NotificationDialog dialog(message, this);
    dialog.exec();
}

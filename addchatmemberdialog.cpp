#include "addchatmemberdialog.h"
#include "ui_addchatmemberdialog.h"

AddChatMemberDialog::AddChatMemberDialog(QString hash, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddChatMemberDialog)
{
    ui->setupUi(this);
    this->hash = hash;
}

AddChatMemberDialog::~AddChatMemberDialog()
{
    delete ui;
}

void AddChatMemberDialog::on_addMemberButton_clicked()
{
    QString nickname = this->ui->newMemberNickname->text();
    QMap<QString, QVariant> raw_data;
    raw_data["username"] = nickname;
    raw_data["chash"] = this->hash;
    QNetworkRequest request = Utils::constructRequest("/chat/adduser/", Utils::getToken());
    QByteArray data = Utils::constructPostData(&raw_data);
    QNetworkAccessManager* accessManager = new QNetworkAccessManager(this);
    connect(accessManager, SIGNAL(finished(QNetworkReply*)), this,
        SLOT(addMemberReplyFinished(QNetworkReply*)));
    accessManager->post(request, data);
    return;
}


void AddChatMemberDialog::addMemberReplyFinished(QNetworkReply* reply) {
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

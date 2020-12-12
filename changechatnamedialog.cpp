#include "changechatnamedialog.h"
#include "ui_changechatnamedialog.h"

ChangeChatNameDialog::ChangeChatNameDialog(QString hash, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChangeChatNameDialog)
{
    ui->setupUi(this);
    this->hash = hash;
}

ChangeChatNameDialog::~ChangeChatNameDialog()
{
    delete ui;
}

void ChangeChatNameDialog::on_changeNameButton_clicked()
{
    QString name = this->ui->chatName->text();
    QMap<QString, QVariant> raw_data;
    raw_data["name"] = name;
    raw_data["chash"] = this->hash;
    QNetworkRequest request = Utils::constructRequest("/chat/changename/", Utils::getToken());
    QByteArray data = Utils::constructPostData(&raw_data);
    QNetworkAccessManager* accessManager = new QNetworkAccessManager(this);
    connect(accessManager, SIGNAL(finished(QNetworkReply*)), this,
        SLOT(changeNameReplyFinished(QNetworkReply*)));
    accessManager->post(request, data);
    return;
}


void ChangeChatNameDialog::changeNameReplyFinished(QNetworkReply *reply){
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

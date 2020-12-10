#include "chatpreview.h"
#include "ui_chatpreview.h"

ChatPreview::ChatPreview(QJsonObject chat, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChatPreview)
{
    ui->setupUi(this);
    this->setHash(chat.value("hash").toString());
    this->ui->chatName->setText(this->getHash());
    QJsonArray users = chat.value("members").toArray();
    int members_amount = users.size();
    QString members = "";
    for(int i = 0; i < members_amount; ++i) {
        members = members + users.at(i).toObject().value("username").toString() + " ";
    }
    this->ui->membersIndicator->setText(members);
}

void ChatPreview::setHash(QString hash) {
    this->hash=hash;
}

QString ChatPreview::getHash() {
    return this->hash;
}

void ChatPreview::setMessages(QJsonArray messages){
    this->messages = messages;
}

void ChatPreview::addMessage(QJsonObject message) {
    this->messages.append(message);
}

void ChatPreview::addMessages(QJsonArray messages) {
    int size = messages.size();
    for(int i = 0; i < size; ++i) {
       this->addMessage(messages[i].toObject());
    }
}

ChatPreview::~ChatPreview()
{
    delete ui;
}



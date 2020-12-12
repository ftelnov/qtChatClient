#include "chatpreview.h"
#include "ui_chatpreview.h"
#include "mainwindow.h"

ChatPreview::ChatPreview(QJsonObject chat, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChatPreview)
{
    ui->setupUi(this);
    this->setHash(chat.value("hash").toString());
    this->setName(chat.value("name").toString());
    this->setAccessibleName(this->getHash());
    this->ui->chatName->setText("Name: " + this->getName());
    int members_amount = chat.value("members").toArray().size();
    this->ui->membersIndicator->setText(QString::number(members_amount) + " members");
}

void ChatPreview::setHash(QString hash) {
    this->hash=hash;
}

void ChatPreview::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        emit chatClicked(this->getHash());
        event->accept();
        return;
    }
    event->ignore();
}

QString ChatPreview::getHash() {
    return this->hash;
}

QString ChatPreview::getName() {
    return this->name;
}

void ChatPreview::setName(QString name) {
    this->name = name;
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



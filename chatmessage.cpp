#include "chatmessage.h"
#include "ui_chatmessage.h"

ChatMessage::ChatMessage(int id, QString message, QString author, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChatMessage)
{
    ui->setupUi(this);
    this->id = id;
    this->message = message;
    this->author = author;
    this->ui->messageText->setText(message);
    this->ui->messageIndicator->setText("(" + author + ")");
}

void ChatMessage::setMessageText(QString text){
    this->message = text;
    this->ui->messageText->setText(text);
}

QString ChatMessage::getAuthor(){
    return this->author;
}

int ChatMessage::getId() {
    return this->id;
}

ChatMessage::~ChatMessage()
{
    delete ui;
}

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "addcontactdialog.h"
#include "questiondialog.h"
#include <QtDebug>
#include <QJsonArray>
#include <QApplication>
#include <QProcess>
#include "chatpreview.h"
#include <QListWidget>
#include <QListWidgetItem>
#include "utils.h"
#include "changechatnamedialog.h"
#include "addchatmemberdialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QSettings settings;
    QString token = Utils::getRawToken();
    if (token == "not_stated") {
       AuthDialog authdialog(this);
       int dialog_code = authdialog.exec();
       if (dialog_code == 0) {
           exit(0);
       }
    }
    QNetworkRequest request = Utils::constructRequest("/user/getme/", Utils::getToken());
    QNetworkAccessManager* accessManager = new QNetworkAccessManager(this);
    connect(accessManager, SIGNAL(finished(QNetworkReply*)), this,
        SLOT(getMeReplyFinished(QNetworkReply*)));
    accessManager->get(request);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::chatListRequest() {
    QNetworkRequest request = Utils::constructRequest("/user/chats/", Utils::getToken());
    QNetworkAccessManager* accessManager = new QNetworkAccessManager(this);
    connect(accessManager, SIGNAL(finished(QNetworkReply*)), this,
        SLOT(getChatList(QNetworkReply*)));
    accessManager->get(request);
}

void MainWindow::getMeReplyFinished(QNetworkReply *reply){
    if (reply->error()) {
        Utils::pushNotification(this, "Something went wrong. We've got error: " + reply->errorString());
        return;
    }
    QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    if (status_code.toInt() == 301) {
        QSettings settings;
        settings.setValue("token", "not_stated");
        qApp->quit();
        QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
        return;
    }
    QJsonObject data = Utils::getJson(reply);
    if (data["result"] != 0) {
        Utils::pushNotification(this, data["error"].toString());
        return;
    }
    chatListRequest();
}

void MainWindow::updateChatList(QJsonArray chats) {
    int size = chats.size();
    QLayout* layout = this->ui->previewsContainerWidget->layout();
    layout->removeItem(this->ui->previewsSpacer);
    QLayoutItem* child;
    while ((child = layout->takeAt(0)) != 0) {
       child->widget()->setParent(NULL);
       delete child;
    }
    for(int i = 0; i < size; i++) {
        QJsonObject chat = chats.at(i).toObject();
        if(selectedChatHash == "") {
            selectedChatHash = chat.value("hash").toString();
        }
        ChatPreview* chatPreview = new ChatPreview(chat, this->ui->previewsContainerWidget);
        connect(chatPreview, SIGNAL(chatClicked(QString)), this, SLOT(chatSwitched(QString)));
        layout->addWidget(chatPreview);
    }
    layout->addItem(this->ui->previewsSpacer);
    clearMessagesList();
    this->update();
    if (messagesTimer == nullptr) {
        messagesTimer = new QTimer(this);
        connect(messagesTimer, &QTimer::timeout, this, QOverload<>::of(&MainWindow::updateMessages));
        messagesTimer->start(1200);
    } else{
        if (size != 0 && !messagesTimer->isActive()) {
            messagesTimer->start();
        }
    }
}

void MainWindow::chatSwitched(QString chash){
    this->switchToAnotherChat(chash);
}

void MainWindow::switchToAnotherChat(QString chash) {
    messagesTimer->stop();
    selectedChatHash = chash;
    clearMessagesList();
    messagesTimer->start();
}


void MainWindow::clearMessagesList(){
    QLayout* layout = this->ui->messagesContainerWidget->layout();
    layout->removeItem(this->ui->messagesSpacer);
    QLayoutItem* child;
    while ((child = layout->takeAt(0)) != 0) {
       child->widget()->setParent(NULL);
       delete child;
    }
    layout->addItem(this->ui->messagesSpacer);
    lmid=-1;
}


void MainWindow::updateMessages() {
    QNetworkRequest request = Utils::constructRequest("/chat/messages/?lmid=" + QString::number(this->lmid) + "&chash=" + this->selectedChatHash, Utils::getToken());
    QNetworkAccessManager* accessManager = new QNetworkAccessManager(this);
    connect(accessManager, SIGNAL(finished(QNetworkReply*)), this,
        SLOT(getMessagesReplyFinished(QNetworkReply*)));
    accessManager->get(request);
}


void MainWindow::pushMessage(QLayout* layout, QJsonObject message){
    ChatMessage* messageContainer = new ChatMessage(message.value("id").toInt(), message.value("text").toString(), message.value("author").toString(), this->ui->messagesContainerWidget);
    layout->addWidget(messageContainer);
}


void MainWindow::getMessagesReplyFinished(QNetworkReply *reply) {
    if (reply->error()) {
        qDebug() << reply->errorString();
        return;
    }
    QJsonObject data = Utils::getJson(reply);
    if (data["result"] != 0) {
        switch(data["result"].toInt()){
            case -2:
                messagesTimer->stop();
                clearMessagesList();
                break;
        }
        return;
    }
    QJsonArray messagesList = data.value("data").toArray();
    int messages_amount = messagesList.size();
    if (messages_amount > 0) {
        QLayout* layout = this->ui->messagesContainerWidget->layout();
        layout->removeItem(this->ui->messagesSpacer);
        this->lmid = data.value("lmid").toInt();
        for(int i = 0; i < messages_amount; ++i) {
            pushMessage(layout, messagesList.at(i).toObject());
        }
        layout->addItem(this->ui->messagesSpacer);
        this->update();
    }
}

void MainWindow::leaveChatReplyFinished(QNetworkReply *reply){
    if (reply->error()) {
        Utils::pushNotification(this, "Something went wrong. We've got error: " + reply->errorString());
        return;
    }
    QJsonObject data = Utils::getJson(reply);
    if (data["result"] != 0) {
        Utils::pushNotification(this, data["error"].toString());
        return;
    }
    QLayout* layout = this->ui->previewsContainerWidget->layout();
    layout->removeItem(this->ui->previewsSpacer);
    QLayoutItem *child;
     while ((child = layout->takeAt(0)) != 0) {
        if (child->widget()->accessibleName() == selectedChatHash) {
            child->widget()->setParent(NULL);
            delete child;
            break;
        }
     }
     while ((child = layout->takeAt(0)) != 0) {
        if (!child->widget()->accessibleName().isEmpty()) {
            switchToAnotherChat(qobject_cast<ChatPreview*>(child->widget())->getHash());
            break;
        }
     }
    layout->addItem(this->ui->previewsSpacer);
}


void MainWindow::getChatList(QNetworkReply *reply){
    if (reply->error()) {
        Utils::pushNotification(this, "Something went wrong. We've got error: " + reply->errorString());
        return;
    }
    QJsonObject data = Utils::getJson(reply);
    if (data["result"] != 0) {
        Utils::pushNotification(this, data["error"].toString());
        return;
    }
    QJsonArray chats = data.value("data").toArray();
    this->updateChatList(chats);
}


void MainWindow::on_addContactButton_clicked()
{
    AddContactDialog dialog(this);
    dialog.exec();
}

void MainWindow::on_logoutButton_clicked()
{
    QuestionDialog dialog("Are you sure you want to continue?", this);
    int result = dialog.exec();
    if (result == 1) {
        QSettings settings;
        settings.setValue("token", "not_stated");
        qApp->quit();
        QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
    }
}

void MainWindow::on_sendMessageIcon_clicked()
{
    QString text = this->ui->messageInputField->text();
    if(text != " ") {
        QMap<QString, QVariant> raw_data;
        raw_data["text"] = text;
        raw_data["chash"] = selectedChatHash;
        QNetworkRequest request = Utils::constructRequest("/chat/messages/add/", Utils::getToken());
        QNetworkAccessManager* accessManager = new QNetworkAccessManager(this);
        accessManager->post(request, Utils::constructPostData(&raw_data));
    }
}

void MainWindow::on_leaveChatButton_clicked()
{
    QuestionDialog dialog("Are you sure want to leave?", this);
    int result = dialog.exec();
    if (result == 1) {
        QMap<QString, QVariant> raw_data;
        raw_data["chash"] = selectedChatHash;
        QNetworkRequest request = Utils::constructRequest("/chat/leave/", Utils::getToken());
        QNetworkAccessManager* accessManager = new QNetworkAccessManager(this);
        connect(accessManager, SIGNAL(finished(QNetworkReply*)), this,
            SLOT(leaveChatReplyFinished(QNetworkReply*)));
        accessManager->post(request, Utils::constructPostData(&raw_data));
    }
}

void MainWindow::on_updateChatListButton_clicked()
{
    chatListRequest();
}

void MainWindow::on_changeChatNameButton_clicked()
{
    ChangeChatNameDialog dialog(selectedChatHash, this);
    dialog.exec();
}



void MainWindow::on_addToChatButton_clicked(bool checked)
{
    AddChatMemberDialog dialog(selectedChatHash, this);
    dialog.exec();
}

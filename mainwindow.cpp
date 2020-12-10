#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "addcontactdialog.h"
#include "questiondialog.h"
#include <QtDebug>
#include <QJsonArray>
#include <QApplication>
#include <QProcess>
#include "chatpreview.h"
#include "utils.h"

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
    QNetworkRequest request = Utils::constructRequest("/user/chats/", Utils::getToken());
    QNetworkAccessManager* accessManager = new QNetworkAccessManager(this);
    connect(accessManager, SIGNAL(finished(QNetworkReply*)), this,
        SLOT(getChatList(QNetworkReply*)));
    accessManager->get(request);
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
    int size = chats.size();
    for(int i = 0; i < size; i++) {
        QJsonObject chat = chats.at(i).toObject();
        ChatPreview* chatPreview = new ChatPreview(chat, this->ui->chatsPreviews->layout()->widget());
        this->ui->chatsPreviews->layout()->addWidget(chatPreview);
    }
    this->ui->chatsPreviews->adjustSize();
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

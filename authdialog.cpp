#include "authdialog.h"
#include "ui_authdialog.h"
#include <QDebug>
#include "notificationdialog.h"
#include <QNetworkReply>
#include <QJsonValue>
#include <QJsonValueRef>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

AuthDialog::AuthDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AuthDialog)
{
    ui->setupUi(this);
    this->ui->passwordRepeatInput->hide();
    this->manager = new QNetworkAccessManager(this);
}

AuthDialog::~AuthDialog()
{
    delete ui;
}
void AuthDialog::on_authButton_clicked()
{
    if(this->is_signup) {
        QString nickname = this->ui->authNicknameInput->text();
        QString password = this->ui->passwordInput->text();
        QString repeatedPassword = this->ui->passwordRepeatInput->text();
        if (nickname.length() < 4) {
            pushNotification("Nickname lenght is invalid(should be >= 4)");
            return;
        }
        bool containsDigit = false;
        for (QChar sym: password) {
            if(sym.isDigit()){
                containsDigit = true;
                break;
            }
        }
        if(repeatedPassword != password) {
            pushNotification("Passwords do not match");
            return;
        }
        if(containsDigit && password.length() >= 8){
            QNetworkRequest request(QUrl(baseUrl + "api/signup/"));
            request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
            QByteArray postData;
            postData.append("nickname=" + nickname + "$");
            postData.append("password=" + password);
            connect(this->manager, SIGNAL(finished(QNetworkReply*)), this,
                SLOT(signupReplyFinished(QNetworkReply*)));
                this->manager->post(request, postData);
            return;
        }
        pushNotification("Password validation failed(should has lenght >= 8 and at least 1 digit)");
    } else {

    }
}

void AuthDialog::pushNotification(QString message) {
    NotificationDialog dialog(message, this);
    dialog.exec();
}

void AuthDialog::signupReplyFinished(QNetworkReply *reply) {
    QByteArray buffer = reply->readAll();
    QJsonDocument jsonDoc(QJsonDocument::fromJson(buffer));
    QJsonObject jsonReply = jsonDoc.object();
    if (jsonReply["result"] != 0) {
        pushNotification(jsonReply["error"].toString());
    } else {
        settings.setValue("token", jsonReply["token"].toString());
        this->accept();
    }
}

void AuthDialog::on_signUpCheckBox_stateChanged(int arg1)
{
    if (arg1) {
        this->ui->passwordRepeatInput->show();
    } else{
        this->ui->passwordRepeatInput->hide();
    }
    this->is_signup = !this->is_signup;
}

void AuthDialog::on_AuthDialog_finished(int result)
{
}

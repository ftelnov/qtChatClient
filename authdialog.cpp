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
#include "utils.h"

AuthDialog::AuthDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AuthDialog)
{
    ui->setupUi(this);
    this->ui->passwordRepeatInput->hide();
}

AuthDialog::~AuthDialog()
{
    delete ui;
}
void AuthDialog::on_authButton_clicked()
{
    QString nickname = this->ui->authNicknameInput->text();
    QString password = this->ui->passwordInput->text();
    if(this->is_signup) {
        QString repeatedPassword = this->ui->passwordRepeatInput->text();
        if (nickname.length() < 4) {
            Utils::pushNotification(this, "Nickname lenght is invalid(should be >= 4)");
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
            Utils::pushNotification(this, "Passwords do not match");
            return;
        }
        if(containsDigit && password.length() >= 8){
            QMap<QString, QVariant> r_data;
            r_data["nickname"] = nickname;
            r_data["password"] = password;
            QNetworkAccessManager* accessManager = new QNetworkAccessManager(this);           
            connect(accessManager, SIGNAL(finished(QNetworkReply*)), this,
                SLOT(signupReplyFinished(QNetworkReply*)));
               accessManager->post(Utils::constructRequest("/signup/", ""), Utils::constructPostData(&r_data));
            return;
        }
        Utils::pushNotification(this, "Password validation failed(should has lenght >= 8 and at least 1 digit)");
    } else {
        QNetworkAccessManager* accessManager = new QNetworkAccessManager(this);
        QMap<QString, QVariant> r_data;
        r_data["nickname"] = nickname;
        r_data["password"] = password;
        connect(accessManager, SIGNAL(finished(QNetworkReply*)), this,
            SLOT(signupReplyFinished(QNetworkReply*)));
           accessManager->post(Utils::constructRequest("/token/obtain/", ""), Utils::constructPostData(&r_data));
        return;
    }
}

void AuthDialog::signupReplyFinished(QNetworkReply *reply) {
    if(reply->error()) {
        Utils::pushNotification(this, "Something went wrong. We've got error: " + reply->errorString());
        return;
    }
    QJsonObject jsonReply = Utils::getJson(reply);
    if (jsonReply["result"] != 0) {
        Utils::pushNotification(this, jsonReply["error"].toString());
    } else {
        Utils::updateToken(jsonReply["token"].toString());
        this->accept();
    }
}

void AuthDialog::on_signUpCheckBox_stateChanged(int arg1)
{
    if (arg1) {
        this->ui->passwordRepeatInput->show();
        this->ui->authButton->setText("Sign Up");
    } else{
        this->ui->passwordRepeatInput->hide();
        this->ui->authButton->setText("Authenticate");
    }
    this->is_signup = !this->is_signup;
}

void AuthDialog::on_AuthDialog_finished(int result)
{
}

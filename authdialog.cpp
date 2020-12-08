#include "authdialog.h"
#include "ui_authdialog.h"
#include <QDebug>
#include "notificationdialog.h"

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
    if(this->is_signup) {
        QString nickname = this->ui->authNicknameInput->toPlainText();
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
        if(containsDigit && password.length() >= 8){
            if(repeatedPassword != password) {
                pushNotification("Passwords do not match");
                return;
            }
            return;
        }
        pushNotification("Password validation failed(should has lenght >= 8 and at least 1 digit)");
    }
}

void AuthDialog::pushNotification(QString message) {
    NotificationDialog dialog(message, this);
    dialog.exec();
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

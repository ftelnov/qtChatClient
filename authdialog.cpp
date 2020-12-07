#include "authdialog.h"
#include "ui_authdialog.h"
#include <QDebug>

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

#include "notificationdialog.h"
#include "ui_notificationdialog.h"

NotificationDialog::NotificationDialog(QString message, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NotificationDialog)
{
    ui->setupUi(this);
    this->ui->notificationText->setText(message);
    QApplication::processEvents();
    this->adjustSize();
}

NotificationDialog::~NotificationDialog()
{
    delete ui;
}

void NotificationDialog::on_okButton_clicked()
{
    this->accept();
}

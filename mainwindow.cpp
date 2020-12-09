#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "addcontactdialog.h"
#include <QtDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QSettings settings;
    QString token = settings.value("token", "not_stated").toString();
    if (token == "not_stated") {
       AuthDialog authdialog(this);
       int dialog_code = authdialog.exec();
       if (dialog_code == 0) {
           exit(0);
       }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_addContactButton_clicked()
{
    AddContactDialog dialog(this);
    dialog.exec();
}

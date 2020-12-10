#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "addcontactdialog.h"
#include "questiondialog.h"
#include <QtDebug>
#include <QApplication>
#include <QProcess>
#include "utils.h"

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

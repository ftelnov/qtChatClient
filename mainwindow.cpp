#include "mainwindow.h"
#include "ui_mainwindow.h"
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
       authdialog.setAttribute(Qt::WA_QuitOnClose);
       authdialog.exec();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}


#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("TheosPractise");
    QCoreApplication::setOrganizationDomain("somedomain.com");
    QCoreApplication::setApplicationName("qServer");
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

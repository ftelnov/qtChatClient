#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <authdialog.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_addContactButton_clicked();

    void on_logoutButton_clicked();

    void getMeReplyFinished(QNetworkReply* reply);

    void getChatList(QNetworkReply* reply);

private:
    Ui::MainWindow *ui;
    void updateChatList(QJsonArray chats);
};
#endif // MAINWINDOW_H

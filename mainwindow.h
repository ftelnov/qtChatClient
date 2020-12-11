#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <authdialog.h>
#include <QTimer>
#include <chatmessage.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void updateMessages();
    ~MainWindow();

private slots:
    void on_addContactButton_clicked();

    void on_logoutButton_clicked();

    void getMeReplyFinished(QNetworkReply* reply);

    void getChatList(QNetworkReply* reply);

    void getMessagesReplyFinished(QNetworkReply* reply);

    void on_sendMessageIcon_clicked();

private:
    Ui::MainWindow *ui;
    void updateChatList(QJsonArray chats);
    QString selectedChatHash = "";
    QTimer* messagesTimer = nullptr;
    void pushMessage(QLayout* layout, QJsonObject message);
    int lmid = -1;
};
#endif // MAINWINDOW_H

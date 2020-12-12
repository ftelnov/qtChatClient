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
    void switchToAnotherChat(QString chash);
    ~MainWindow();

private slots:
    void on_addContactButton_clicked();

    void on_logoutButton_clicked();

    void getMeReplyFinished(QNetworkReply* reply);

    void getChatList(QNetworkReply* reply);

    void getMessagesReplyFinished(QNetworkReply* reply);

    void leaveChatReplyFinished(QNetworkReply* reply);

    void on_sendMessageIcon_clicked();

    void chatSwitched(QString chash);

    void on_leaveChatButton_clicked();

    void on_updateChatListButton_clicked();

    void on_changeChatNameButton_clicked();

    void on_addToChatButton_clicked(bool checked);

private:
    Ui::MainWindow *ui;
    void updateChatList(QJsonArray chats);
    void clearMessagesList();
    void chatListRequest();
    QString selectedChatHash = "";
    QTimer* messagesTimer = nullptr;
    void pushMessage(QLayout* layout, QJsonObject message);
    QTimer* chatListTimer = nullptr;
    int lmid = -1;
};
#endif // MAINWINDOW_H

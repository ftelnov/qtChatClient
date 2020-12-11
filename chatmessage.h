#ifndef CHATMESSAGE_H
#define CHATMESSAGE_H

#include <QWidget>

namespace Ui {
class ChatMessage;
}

class ChatMessage : public QWidget
{
    Q_OBJECT

public:
    explicit ChatMessage(int id, QString message, QString author, QWidget *parent = nullptr);
    int getId();
    QString getMessageText();
    QString getAuthor();
    void setMessageText(QString text);

    ~ChatMessage();

private:
    Ui::ChatMessage *ui;
    int id;
    QString message;
    QString author;
};

#endif // CHATMESSAGE_H

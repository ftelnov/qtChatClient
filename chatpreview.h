#ifndef CHATPREVIEW_H
#define CHATPREVIEW_H

#include <QWidget>
#include <QJsonArray>
#include <QJsonObject>
#include <QLabel>
#include <QMouseEvent>
#include <QDebug>
#include <QStyleOption>
#include <QPainter>

namespace Ui {
class ChatPreview;
}

class ChatPreview : public QWidget
{
    Q_OBJECT

public:
    explicit ChatPreview(QJsonObject chat, QWidget *parent = nullptr);
    ~ChatPreview();
    QString getHash();
    void setHash(QString hash);
    QString getName();
    void setName(QString name);
    void setMessages(QJsonArray messages);
    void addMessages(QJsonArray messages);
    void setSelected(bool selected);
signals:
    void chatClicked(QString chash);

private:
    Ui::ChatPreview *ui;
    QString hash;
    QString name;
    bool selected = false;
    QJsonArray messages;
    int last_message_id = -1;
    void addMessage(QJsonObject message);
    void mousePressEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *e) override;
};

#endif // CHATPREVIEW_H

#ifndef UTILS_H
#define UTILS_H

#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>
#include <QNetworkRequest>
#include <QObject>
#include <QSettings>
#include <QScrollArea>

class Utils
{
public:
    Utils();
    static QNetworkRequest constructRequest(QString relativePath, QString token);
    static QByteArray constructPostData(QMap<QString, QVariant> *data);
    static QJsonObject getJson(QNetworkReply* reply);
    static QString getToken();
    static QString getRawToken();
    static void updateToken(QString token);
    static void pushNotification(QWidget* ui, QString message);
};

#endif // UTILS_H

#include "utils.h"
#include "notificationdialog.h"
#include "chatpreview.h"

Utils::Utils()
{
}


QNetworkRequest Utils::constructRequest(QString relativePath, QString token){
    QString baseUrl = "http://127.0.0.1:8000/api";
    QNetworkRequest request(QUrl(baseUrl + relativePath));
    request.setRawHeader("Authorization", token.toUtf8());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    return request;
}

QString Utils::getToken(){
    QSettings settings;
    return "Token " + settings.value("token").toString();
}

QString Utils::getRawToken() {
    QSettings settings;
    return settings.value("token").toString();
}


QByteArray Utils::constructPostData(QMap<QString, QVariant> *data){
    QJsonObject json;
    for(QString key: data->keys()) {
        json.insert(key, data->take(key).toJsonValue());
    }
    QJsonDocument json_doc;
    json_doc.setObject(json);
    QByteArray postData = json_doc.toJson();
    return postData;
}

QJsonObject Utils::getJson(QNetworkReply *reply) {
    QByteArray buffer = reply->readAll();
    QJsonDocument jsonDoc(QJsonDocument::fromJson(buffer));
    QJsonObject jsonReply = jsonDoc.object();
    return jsonReply;
}

void Utils::pushNotification(QWidget *ui, QString message) {
    NotificationDialog dialog(message, ui);
    dialog.exec();
}

void Utils::updateToken(QString token) {
    QSettings settings;
    settings.setValue("token", token);
}

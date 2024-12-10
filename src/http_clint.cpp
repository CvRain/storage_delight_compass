//
// Created by cvrain on 24-12-8.
//

#include "http_clint.hpp"

#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonObject>

HttpClient* HttpClient::getInstance() {
    qDebug() << "HttpClient::getInstance";

    static HttpClient instance;
    return &instance;
}

HttpClient::~HttpClient() {
    qDebug() << "HttpClient::~HttpClient";
    if (manager != nullptr) {
        manager->deleteLater();
    }
}

BaseResult HttpClient::login(const QString &userName, const QString &password) {
    qDebug() << "HttpClient::login";

    const auto url = QUrl{baseUrl + "/user/login"};
    QNetworkRequest request{url};
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject json;
    json.insert("name", userName);
    json.insert("password", password);

    const auto jsonData = QJsonDocument{json}.toJson();
    const auto reply = manager->post(request, jsonData);
    if (reply->error() != QNetworkReply::NoError) {
        BaseResult result;
        result.setResult(-1, "error", reply->errorString());
        return result;
    }
    const auto& jsonResponse = QJsonDocument::fromJson(reply->readAll());

    const auto code = jsonResponse.object()["code"].toInt();
    const auto& message = jsonResponse.object()["message"].toString();
    const auto& result = jsonResponse.object()["result"].toString();

    reply->deleteLater();
    BaseResult back;
    back.setResult(code, result, message);
    return back;
}

void HttpClient::replyHandler(const QNetworkReply *reply) {
    if (reply->error() == QNetworkReply::NoError) {
        qDebug() << "[success] Request " << reply->url();
        return;
    }
    qDebug() << "[failed] Request " << reply->url() << ": " << reply->error();
}

HttpClient::HttpClient()
    : baseUrl("http://localhost:10492/api"), manager(new QNetworkAccessManager(this)) {
    qDebug() << "HttpClient::HttpClient";

    const auto request = QNetworkRequest(QUrl{baseUrl + "/hello/say"});
    const auto reply = manager->get(request);
    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << "HttpClient::HttpClient: " << reply->errorString();
    }else {
        qDebug() << "HttpClient::HttpClient: " << reply->readAll();
    }
    reply->deleteLater();
}

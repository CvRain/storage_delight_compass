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

HttpClint* HttpClint::getInstance() {
    qDebug() << "HttpClient::getInstance";

    static HttpClint instance;
    return &instance;
}

HttpClint::~HttpClint() {
    qDebug() << "HttpClient::~HttpClient";
    if (manager != nullptr) {
        manager->deleteLater();
    }
}

void HttpClint::login(const QString &userName, const QString &password) {
    qDebug() << "HttpClient::login";

    const auto url = QUrl{baseUrl + "/user/login"};
    QNetworkRequest request{url};
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject json;
    json.insert("name", userName);
    json.insert("password", password);

    const auto jsonData = QJsonDocument{json}.toJson();
    manager->post(request, jsonData);
}

HttpClint::HttpClint()
    : baseUrl("http://localhost:10492/api"), manager(new QNetworkAccessManager(this)) {
    qDebug() << "HttpClient::HttpClient";

    const auto isConnected = connect(manager, &QNetworkAccessManager::finished, this, [](QNetworkReply *reply) {
        if (reply->error() != QNetworkReply::NoError) {
            qDebug() << "error: " << reply->errorString();
        }
        const auto response = reply->readAll();
        const auto jsonResponse = QJsonDocument::fromJson(response);
        qDebug() << "response: " << jsonResponse.toJson();
    });

    if (!isConnected) {
        qDebug() << "connect failed";
    }

    const auto request = QNetworkRequest(QUrl{baseUrl + "/hello/say"});
    manager->get(request);
}

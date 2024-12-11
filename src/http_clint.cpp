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

void HttpClient::login(const QString &userName, const QString &password) {
    qDebug() << "HttpClient::login";

    const auto data = QJsonObject{
        {"name", userName},
        {"password", password}
    };

    QNetworkRequest request(QUrl{baseUrl + "/user/login"});
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply *reply = manager->post(request, QJsonDocument(data).toJson());
    connect(reply, &QNetworkReply::finished, this, [this,reply]() {
        userLoginResponse(reply);
    });
}

void HttpClient::userRegister(const QString &userName, const QString &password, int role) {
    qDebug() << "HttpClient::userRegister";
    const auto data = QJsonObject{
        {"name", userName},
        {"password", password},
        {"role", role}
    };
    QNetworkRequest request(QUrl{baseUrl + "/user/add"});
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QNetworkReply *reply = manager->post(request, QJsonDocument(data).toJson());
    connect(reply, &QNetworkReply::finished, this, [this,reply]() {
        userRegisterResponse(reply);
    });
}

void HttpClient::userLoginResponse(QNetworkReply *reply) {
    qDebug() << "HttpClient::userLoginResponse";
    if (reply->error() == QNetworkReply::NoError) {
        const auto response = reply->readAll();
        const auto json = QJsonDocument::fromJson(response);

        const auto code = json["code"].toInt();
        const auto &message = json["message"].toString();
        const auto &result = json["result"].toString();

        emit userLogged(code, result, message);
    }
    else {
        qDebug() << "HttpClient::userLoginResponse: " << reply->errorString();
        emit requestFailed(reply->errorString());
    }
    reply->deleteLater();
}

void HttpClient::userRegisterResponse(QNetworkReply *reply) {
    qDebug() << "HttpClient::userRegisterResponse";
    if (reply->error() == QNetworkReply::NoError) {
        const auto response = reply->readAll();
        const auto json = QJsonDocument::fromJson(response);

        const auto code = json["code"].toInt();
        const auto &message = json["message"].toString();
        const auto &result = json["result"].toString();

        emit userRegistered(code, result, message);
    }
    else {
        qDebug() << "HttpClient::userRegisterResponse: " << reply->errorString();
        emit requestFailed(reply->errorString());
    }
}

HttpClient::HttpClient()
    : baseUrl("http://localhost:10492/api"), manager(new QNetworkAccessManager(this)) {
    qDebug() << "HttpClient::HttpClient";

    const auto request = QNetworkRequest(QUrl{baseUrl + "/hello/say"});
}

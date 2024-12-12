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

    static auto instance = HttpClient{};
    return &instance;
}

HttpClient::~HttpClient() {
    qDebug() << "HttpClient::~HttpClient";
    if (manager != nullptr) {
        manager->deleteLater();
        delete manager;
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

void HttpClient::userInfo(const QString &userInfo) {
    //curl --location --request GET 'http://localhost:10492/api/user/id/?user_id={}&search_id={}'
    qDebug() << "HttpClient::userInfo";
    const auto data = QJsonObject{
        {"user_id", userInfo},
        {"search_id", userInfo}
    };
    QNetworkRequest request(QUrl{baseUrl + "/user/id/"});
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply *reply = manager->get(request);
    connect(reply, &QNetworkReply::finished, this, [this,reply]() {
        userInfoResponse(reply);
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

        const auto& data = json["data"].toObject();
        const auto& token = data["token"].toString();

        UserManager::getInstance()->setLoginStatus(code == 200);
        UserManager::getInstance()->getUserInfo().setToken(token);
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

void HttpClient::userInfoResponse(QNetworkReply *reply) {
    qDebug() << "HttpClient::userInfoResponse";
    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << "HttpClient::userInfoResponse: " << reply->errorString();
        emit requestFailed(reply->errorString());
        return;
    }
    const auto response = reply->readAll();
    const auto jsonResponse = QJsonDocument::fromJson(response);

    const auto code = jsonResponse["code"].toInt();
    const auto &message = jsonResponse["message"].toString();
    const auto &result = jsonResponse["result"].toString();
    emit userInfoFetched(code, result, message);

    const auto& data = jsonResponse["data"].toObject();
    const auto& groupId = data["group_id"].toString();
    const auto& id = data["_id"].toString();
    const auto& name = data["name"].toString();
    const auto& role = data["role"].toInt();
    //添加用户信息
    UserManager::getInstance()->getUserInfo().setGroupId(groupId);
    UserManager::getInstance()->getUserInfo().setId(id);
    UserManager::getInstance()->getUserInfo().setName(name);
    UserManager::getInstance()->getUserInfo().setRole(role);

}

HttpClient::HttpClient()
    : baseUrl("http://localhost:10492/api"), manager(new QNetworkAccessManager(this)) {
    qDebug() << "HttpClient::HttpClient";
}

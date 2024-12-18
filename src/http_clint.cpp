//
// Created by cvrain on 24-12-8.
//

#include "http_clint.hpp"

#include <QDebug>

HttpClient* HttpClient::getInstance() {
    qDebug() << "HttpClient::getInstance";

    static auto instance = HttpClient{};
    return &instance;
}

HttpClient::~HttpClient() {
    qDebug() << "HttpClient::~HttpClient";
}

void HttpClient::login(const QString &userName, const QString &password) {
    qDebug() << "HttpClient::login";
    web::json::value requestData;
    requestData[_XPLATSTR("name")] = web::json::value::string(userName.toStdString());
    requestData[_XPLATSTR("password")] = web::json::value::string(password.toStdString());

    web::http::http_request request(web::http::methods::POST);
    request.set_request_uri(_XPLATSTR("/user/login"));
    request.headers().set_content_type(_XPLATSTR("application/json"));
    request.set_body(requestData);

    try {
        const auto response = client.request(request).get();
        const auto responseData = response.extract_json().get();

        const auto& responseCode = responseData.at(_XPLATSTR("code")).as_number().to_int32();
        const auto& responseMessage = responseData.at(_XPLATSTR("message")).as_string();
        const auto& responseResult = responseData.at(_XPLATSTR("result")).as_string();

        if (responseCode == 200) {
            const auto& responseToken = responseData.at(_XPLATSTR("data")).at(_XPLATSTR("token")).as_string();
            const auto& responseName = responseData.at(_XPLATSTR("data")).at(_XPLATSTR("name")).as_string();
            const auto& responseUserId = responseData.at(_XPLATSTR("data")).at(_XPLATSTR("user_id")).as_string();
        }
        emit userLogged(responseCode, QString{responseResult.data()}, QString{responseMessage.data()});
    }catch (const std::exception& e) {
        emit requestFailed(QString{e.what()});
    }

}

void HttpClient::userRegister(const QString &userName, const QString &password, int role) {
    qDebug() << "HttpClient::userRegister";
    web::json::value requestData;
    requestData[_XPLATSTR("name")] = web::json::value::string(userName.toStdString());
    requestData[_XPLATSTR("password")] = web::json::value::string(password.toStdString());
    requestData[_XPLATSTR("role")] = web::json::value::number(role);
    web::http::http_request request(web::http::methods::POST);
    request.set_request_uri(_XPLATSTR("/user/register"));
    request.headers().set_content_type(_XPLATSTR("application/json"));
    request.set_body(requestData);
    try {
        const auto response = client.request(request).get();
        const auto responseData = response.extract_json().get();

        const auto& responseCode = responseData.at(_XPLATSTR("code")).as_number().to_int32();
        const auto& responseMessage = responseData.at(_XPLATSTR("message")).as_string();
        const auto& responseResult = responseData.at(_XPLATSTR("result")).as_string();

        emit userRegistered(responseCode, QString{responseResult.data()}, QString{responseMessage.data()});
    }catch (const std::exception& e) {
        emit requestFailed(QString{e.what()});
    }

}

void HttpClient::userInfo(const QString &userInfo) {
    //curl --location --request GET 'http://localhost:10492/api/user/id/?user_id={}&search_id={}'
    qDebug() << "HttpClient::userInfo";

}


HttpClient::HttpClient()
    : baseUrl("http://localhost:10492/api"), client(_XPLATSTR(baseUrl)) {
    qDebug() << "HttpClient::HttpClient";
}

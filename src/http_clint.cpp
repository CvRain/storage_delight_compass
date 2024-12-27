//
// Created by cvrain on 24-12-8.
//

#include "http_clint.hpp"

#include <QDebug>

#include "types/storage_source.hpp"

class StorageSource;

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

        const auto &responseCode = responseData.at(_XPLATSTR("code")).as_number().to_int32();
        const auto &responseMessage = responseData.at(_XPLATSTR("message")).as_string();
        const auto &responseResult = responseData.at(_XPLATSTR("result")).as_string();

        if (responseCode == 200) {
            const auto &responseToken = responseData.at(_XPLATSTR("data")).at(_XPLATSTR("token")).as_string();
            const auto &responseName = responseData.at(_XPLATSTR("data")).at(_XPLATSTR("name")).as_string();
            const auto &responseUserId = responseData.at(_XPLATSTR("data")).at(_XPLATSTR("user_id")).as_string();

            UserManager::getInstance()->getUserInfo().setToken(responseToken.data());
            UserManager::getInstance()->getUserInfo().setName(responseName.data());
            UserManager::getInstance()->getUserInfo().setId(responseUserId.data());
        }
        emit userLogged(responseCode, QString{responseResult.data()}, QString{responseMessage.data()});
    }
    catch (const std::exception &e) {
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

    request.set_request_uri(_XPLATSTR("/user/add"));
    request.headers().set_content_type(_XPLATSTR("application/json"));
    request.set_body(requestData);

    try {
        const auto response = client.request(request).get();
        const auto responseData = response.extract_json().get();

        const auto &responseCode = responseData.at(_XPLATSTR("code")).as_number().to_int32();
        const auto &responseMessage = responseData.at(_XPLATSTR("message")).as_string();
        const auto &responseResult = responseData.at(_XPLATSTR("result")).as_string();

        emit userRegistered(responseCode, QString{responseResult.data()}, QString{responseMessage.data()});
    }
    catch (const std::exception &e) {
        emit requestFailed(QString{e.what()});
    }
}

/// <summary>
/// <code> curl --location --request GET 'http://localhost:10492/api/user/info'
/// --header 'Content-Type: application/json'
/// --data-raw '{
/// "user_id" : "67459983948109dbd1079573" ,
/// "search_id" : "67459983948109dbd1079573"
/// }'
/// </code>
/// </summary>

void HttpClient::userInfo() {
    qDebug() << "HttpClient::userInfo";

    web::json::value requestData;
    requestData[_XPLATSTR("user_id")] = web::json::value::string(UserManager::getInstance()->getId().toStdString());
    requestData[_XPLATSTR("search_id")] = web::json::value::string(UserManager::getInstance()->getId().toStdString());

    web::http::http_request request(web::http::methods::GET);
    request.set_request_uri(_XPLATSTR("/user/info"));
    request.headers().set_content_type(_XPLATSTR("application/json"));
    request.set_body(requestData);
    try {
        const auto response = client.request(request).get();
        const auto responseData = response.extract_json().get();

        const auto &responseCode = responseData.at(_XPLATSTR("code")).as_number().to_int32();
        const auto &responseMessage = responseData.at(_XPLATSTR("message")).as_string();
        const auto &responseResult = responseData.at(_XPLATSTR("result")).as_string();

        if (responseCode == 200) {
            const auto &groupId = responseData.at(_XPLATSTR("data")).at(_XPLATSTR("group_id")).as_string();
            const auto &role = responseData.at(_XPLATSTR("data")).at(_XPLATSTR("role")).as_number().to_int32();
            const auto &createTime = responseData.at(_XPLATSTR("data")).at(_XPLATSTR("create_time")).as_number().to_int32();

            UserManager::getInstance()->setGroupId(groupId.data());
            UserManager::getInstance()->setRole(role);
            UserManager::getInstance()->setCreateTime(createTime);
        }
        emit userInfoFetched(responseCode, QString{responseResult.data()}, QString{responseMessage.data()});
    }
    catch (const std::exception &e) {
        emit requestFailed(QString{e.what()});
    }
}

/*
 * curl --location --request GET 'http://localhost:10492/api/StorageSource/list' \
 * --header 'Content-Type: application/json' \
 * --data-raw '{
 *    "user_id":"67459983948109dbd1079573"
 * }'
 */
QList<StorageSource> HttpClient::storageSource() {
    qDebug() << "HttpClient::storageSource";

    web::json::value requestData;
    requestData[_XPLATSTR("user_id")] = web::json::value::string(UserManager::getInstance()->getId().toStdString());

    web::http::http_request request(web::http::methods::GET);
    request.set_request_uri(_XPLATSTR("/StorageSource/list"));
    request.headers().set_content_type(_XPLATSTR("application/json"));
    request.set_body(requestData);

    try {
        const auto response = client.request(request).get();
        const auto responseData = response.extract_json().get();

        const auto &responseCode = responseData.at(_XPLATSTR("code")).as_number().to_int32();
        const auto &responseMessage = responseData.at(_XPLATSTR("message")).as_string();
        const auto &responseResult = responseData.at(_XPLATSTR("result")).as_string();

        QList<StorageSource> sources;
        if (responseCode == 200) {
            for (const auto &responseDataArray = responseData.at(_XPLATSTR("data")).as_array();
                 const auto &item: responseDataArray) {
                const auto &itemData = item.as_object();
                StorageSource source;
                source.setId(itemData.at(_XPLATSTR("_id")).as_string().data());
                source.setName(itemData.at(_XPLATSTR("name")).as_string().data());
                source.setUrl(itemData.at(_XPLATSTR("url")).as_string().data());
                source.setAk(itemData.at(_XPLATSTR("access_key")).as_string().data());
                source.setSk(itemData.at(_XPLATSTR("secret_key")).as_string().data());
                qDebug() << "source:" << source.getId() << source.getName() << source.getUrl() << source.getAk()
                        << source.getSk();
                sources.append(source);
            }
        }
        emit requestFinish(responseCode, QString{responseResult.data()}, QString{responseMessage.data()});
        return sources;
    }
    catch (const std::exception &e) {
        qDebug() << e.what();
        emit requestFailed(QString{e.what()});
        return {};
    }
}

/// <summary>
/// 添加一个存储源
/// <code>
/// curl --location --request PUT 'http://localhost:10492/api/StorageSource/add'
/// --header 'Content-Type: application/json'
/// --data-raw '{
///     "name": "string",
///     "url": "string",
///     "access_key": "string",
///     "secret_key": "string",
///     "user_id": "string",
///     "is_https":false
/// }'
/// </code>
/// </summary>
CommonResponse HttpClient::addStorageSource(const StorageSource &source) {
    qDebug() << "HttpClient::addStorageSource";

    web::json::value requestData;
    requestData[_XPLATSTR("name")] = web::json::value::string(source.getName().toStdString());
    requestData[_XPLATSTR("url")] = web::json::value::string(source.getUrl().toStdString());
    requestData[_XPLATSTR("access_key")] = web::json::value::string(source.getAk().toStdString());
    requestData[_XPLATSTR("secret_key")] = web::json::value::string(source.getSk().toStdString());
    requestData[_XPLATSTR("user_id")] = web::json::value::string(UserManager::getInstance()->getId().toStdString());
    requestData[_XPLATSTR("is_https")] = web::json::value::boolean(source.getIsHttps());

    web::http::http_request request(web::http::methods::PUT);
    request.set_request_uri(_XPLATSTR("/StorageSource/add"));
    request.headers().set_content_type(_XPLATSTR("application/json"));
    request.set_body(requestData);
    try {
        const auto response = client.request(request).get();
        const auto responseData = response.extract_json().get();
        const auto &responseCode = responseData.at(_XPLATSTR("code")).as_number().to_int32();
        const auto &responseMessage = responseData.at(_XPLATSTR("message")).as_string();
        const auto &responseResult = responseData.at(_XPLATSTR("result")).as_string();

        emit requestFinish(responseCode, QString{responseResult.data()}, QString{responseMessage.data()});

        CommonResponse backResponse;
        backResponse.data = {};
        backResponse.code = responseCode;
        backResponse.message = responseMessage;
        backResponse.result = responseResult;
        return backResponse;
    }
    catch (const std::exception &e) {
        emit requestFailed(QString{e.what()});
        throw;
    }
}

/// <summary>
/// 删除一个存储源
/// <code>
/// curl --location --request DELETE 'http://localhost:10492/api/StorageSource/remove'
/// --header 'Content-Type: application/json'
/// --data-raw '{
///     "user_id": "670d399eb8679e9edc0a90c3",
///     "source_id": "6734670d840bd161d30c9799"
/// }'
/// </code>
/// </summary>
CommonResponse HttpClient::removeStorageSource(const std::string& storageId) {
    qDebug() << "HttpClient::removeStorageSource";

    const auto userId = UserManager::getInstance()->getId().toStdString();

    web::json::value requestData;
    requestData[_XPLATSTR("user_id")] = web::json::value::string(userId);
    requestData[_XPLATSTR("source_id")] = web::json::value::string(storageId);
    web::http::http_request request(web::http::methods::DEL);
    request.set_request_uri(_XPLATSTR("/StorageSource/remove"));
    request.headers().set_content_type(_XPLATSTR("application/json"));
    request.set_body(requestData);

    try {
        const auto response = client.request(request).get();
        const auto responseData = response.extract_json().get();
        const auto &responseCode = responseData.at(_XPLATSTR("code")).as_number().to_int32();
        const auto &responseMessage = responseData.at(_XPLATSTR("message")).as_string();
        const auto &responseResult = responseData.at(_XPLATSTR("result")).as_string();
        emit requestFinish(responseCode, QString{responseResult.data()}, QString{responseMessage.data()});
        CommonResponse backResponse;
        backResponse.data = {};
        backResponse.code = responseCode;
        backResponse.message = responseMessage;
        backResponse.result = responseResult;
        return backResponse;
    }
    catch (const std::exception &e) {
        emit requestFailed(QString{e.what()});
        throw;
    }
}

/// <summary>
/// 获取所有用户信息
/// <code>
/// curl --location --request GET 'http://localhost:10492/api/User/info/all'
/// </code>
/// </summary>
QList<UserInfo> HttpClient::userInfoList() {
    qDebug() << "HttpClient::userInfoList";
    try {
        const auto response = client.request(web::http::methods::GET, _XPLATSTR("/User/info/all")).get();
        const auto responseData = response.extract_json().get();
        const auto &responseCode = responseData.at(_XPLATSTR("code")).as_number().to_int32();
        const auto &responseMessage = responseData.at(_XPLATSTR("message")).as_string();
        const auto &responseResult = responseData.at(_XPLATSTR("result")).as_string();
        if (responseCode != 200) {
            emit requestFinish(responseCode, QString{responseResult.data()}, QString{responseMessage.data()});
            return {};
        }
        const auto data = responseData.at(_XPLATSTR("data")).as_array();
        QList<UserInfo> users;
        for (const auto& it: data) {
            UserInfo oneUser;
            oneUser.setId(it.at(_XPLATSTR("_id")).at(_XPLATSTR("$oid")).as_string().data());
            oneUser.setCreateTime(it.at(_XPLATSTR("create_time")).as_number().to_int32());
            oneUser.setGroupId(it.at(_XPLATSTR("group_id")).at(_XPLATSTR("$oid")).as_string().data());
            oneUser.setName(it.at(_XPLATSTR("name")).as_string().data());
            oneUser.setRole(it.at(_XPLATSTR("role")).as_number().to_int32());
            users.append(std::move(oneUser));
        }
        return users;
    }catch (const std::exception &e) {
        emit requestFailed(QString{e.what()});
        throw;
    }
}

QString HttpClient::removeUser(const std::string &user_id) {
    qDebug() << "HttpClient::removeUser";
    try {
        web::http::http_request request(web::http::methods::DEL);
        request.set_request_uri(_XPLATSTR("/User/remove"));
        request.headers().set_content_type(_XPLATSTR("application/json"));

        web::json::value requestData;
        requestData[_XPLATSTR("user_id")] = web::json::value::string(user_id);
        request.set_body(requestData);

        const auto response = client.request(request).get();
        const auto responseData = response.extract_json().get();
        const auto &responseCode = responseData.at(_XPLATSTR("code")).as_number().to_int32();
        const auto &responseMessage = responseData.at(_XPLATSTR("message")).as_string();

        if (responseCode != 200) {
            emit requestFinish(responseCode, QString{responseMessage.data()}, QString{responseMessage.data()});
            return {};
        }
        return responseData.at(_XPLATSTR("result")).as_string().data();
    }catch (const std::exception &e) {
        emit requestFailed(QString{e.what()});
        throw;
    }
}


HttpClient::HttpClient()
    : baseUrl("http://localhost:10492/api"), client(_XPLATSTR(baseUrl)) {
    qDebug() << "HttpClient::HttpClient";
}

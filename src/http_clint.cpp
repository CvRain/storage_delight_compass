//
// Created by cvrain on 24-12-8.
//

#include "http_clint.hpp"

#include <QDebug>
#include <QFile>
#include <filesystem>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QHttpMultiPart>
#include <QUrl>
#include <QFileInfo>

#include "types/storage_source.hpp"
#include <cpprest/filestream.h>
#include <cpprest/json.h>
#include <cpprest/http_headers.h>
#include <cpprest/streams.h>
#include <cpprest/containerstream.h>

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
            const auto &createTime = responseData.at(_XPLATSTR("data")).at(_XPLATSTR("create_time")).as_number().
                    to_int32();

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
CommonResponse HttpClient::removeStorageSource(const std::string &storageId) {
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
        for (const auto &it: data) {
            UserInfo oneUser;
            oneUser.setId(it.at(_XPLATSTR("_id")).at(_XPLATSTR("$oid")).as_string().data());
            oneUser.setCreateTime(it.at(_XPLATSTR("create_time")).as_number().to_int32());
            oneUser.setGroupId(it.at(_XPLATSTR("group_id")).at(_XPLATSTR("$oid")).as_string().data());
            oneUser.setName(it.at(_XPLATSTR("name")).as_string().data());
            oneUser.setRole(it.at(_XPLATSTR("role")).as_number().to_int32());
            users.append(std::move(oneUser));
        }
        return users;
    }
    catch (const std::exception &e) {
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
    }
    catch (const std::exception &e) {
        emit requestFailed(QString{e.what()});
        throw;
    }
}

/// <summary>
/// 获取群组信息
/// <code>
/// curl --location --request GET 'http://localhost:10492/api/Group/info' \
/// --header 'Content-Type: application/json' \
/// --data-raw '{
///     "user_id": "67459983948109dbd1079573",
///     "group_id": "67459983948109dbd1079575"
/// }'
/// </code>
/// </summary>
std::optional<GroupInfo> HttpClient::groupInfo() {
    qDebug() << "HttpClient::groupInfo";
    try {
        const auto userId = UserManager::getInstance()->getId().toStdString();
        const auto groupId = UserManager::getInstance()->getGroupId().toStdString();

        web::json::value requestData;
        requestData[_XPLATSTR("user_id")] = web::json::value::string(userId);
        requestData[_XPLATSTR("group_id")] = web::json::value::string(groupId);
        web::http::http_request request(web::http::methods::GET);

        request.set_request_uri(_XPLATSTR("/Group/info"));
        request.headers().set_content_type(_XPLATSTR("application/json"));
        request.set_body(requestData);

        const auto response = client.request(request).get();
        const auto responseData = response.extract_json().get();
        qDebug() << "HttpClient::groupInfo" << responseData.serialize().data();
        const auto &responseCode = responseData.at(_XPLATSTR("code")).as_number().to_int32();
        const auto &responseMessage = responseData.at(_XPLATSTR("message")).as_string();
        const auto &responseResult = responseData.at(_XPLATSTR("result")).as_string();

        if (responseCode != 200) {
            emit requestFinish(responseCode, QString{responseResult.data()}, QString{responseMessage.data()});
            return std::nullopt;
        }

        const auto &data = responseData.at(_XPLATSTR("data"));
        const auto &id = data.at(_XPLATSTR("_id")).as_string();
        const auto &name = data.at(_XPLATSTR("name")).as_string();
        const auto &buckets = data.at(_XPLATSTR("buckets")).as_array();
        const auto &membersId = data.at(_XPLATSTR("members_id")).as_array();

        QList<Bucket> bucketsList;
        for (const auto &it: buckets) {
            Bucket tempBucket{};
            tempBucket.bucketName = it.at(_XPLATSTR("bucket_name")).as_string().data();
            tempBucket.sourceId = it.at(_XPLATSTR("source_id")).as_string().data();
            qDebug() << "bucketName: " << tempBucket.bucketName << " sourceId: " << tempBucket.sourceId;
            bucketsList.append(std::move(tempBucket));
        }

        QList<QString> membersIdList;
        for (const auto &it: membersId) {
            qDebug() << "membersId: " << it.as_string().data();
            membersIdList.append(it.as_string().data());
        }

        GroupInfo groupInfo{};
        groupInfo.setId(id.data());
        groupInfo.setBuckets(bucketsList);
        groupInfo.setName(name.data());
        groupInfo.setMembersId(membersIdList);

        return groupInfo;
    }
    catch (const std::exception &e) {
        emit requestFailed(QString{e.what()});
        throw;
    }
}

/// <code>
/// curl --location --request GET 'http://localhost:10492/api/User/name'
/// --header 'Content-Type: application/json'
/// --data-raw '{
///     "user_ids": [
///         "67459983948109dbd1079573",
///         "67459983948109dbd1079573"
///     ]
/// }'
/// </code>
/// 返回
/// <code>
/// {
///     "code": 200,
///     "data": [{
///             "id": "67459983948109dbd1079573",
///             "name": "cvrain" }],
///     "message": "k200OK",
///     "result": "ok"
/// }
/// </code>
std::optional<std::pair<QStringList, QStringList>> HttpClient::groupMemberNames(const QStringList &names) {
    qDebug() << "HttpClient::groupMemberNames";
    try {
        web::json::value requestData;
        requestData[_XPLATSTR("user_ids")] = web::json::value::array();
        for (int i = 0; i < names.size(); i++) {
            requestData[_XPLATSTR("user_ids")][i] = web::json::value::string(names.at(i).toStdString());
        }

        web::http::http_request request(web::http::methods::GET);

        request.set_request_uri(_XPLATSTR("/User/name"));
        request.headers().set_content_type(_XPLATSTR("application/json"));
        request.set_body(requestData);

        const auto response = client.request(request).get();
        const auto responseData = response.extract_json().get();
        const auto &responseCode = responseData.at(_XPLATSTR("code")).as_number().to_int32();
        const auto &responseMessage = responseData.at(_XPLATSTR("message")).as_string();
        const auto &responseResult = responseData.at(_XPLATSTR("result")).as_string();

        if (responseCode != 200) {
            qDebug() << "HttpClient::groupMemberNames" << responseCode;
            emit requestFinish(responseCode, QString{responseResult.data()}, QString{responseMessage.data()});
            return std::nullopt;
        }

        const auto &data = responseData.at(_XPLATSTR("data")).as_array();
        QStringList memberNames;
        for (const auto &it: data) {
            memberNames.append(QString::fromStdString(it.at(_XPLATSTR("name")).as_string()));
        }
        QStringList memberIds;
        for (const auto &it: data) {
            memberIds.append(QString::fromStdString(it.at(_XPLATSTR("id")).as_string()));
        }
        //debug memberNames and memberIds size;
        qDebug() << "memberNames size: " << memberNames.size() << " memberIds size: " << memberIds.size();
        return std::make_pair(std::move(memberIds), std::move(memberNames));
    }
    catch (const std::exception &e) {
        emit requestFailed(QString{e.what()});
        throw;
    }
}

/// <summary>
/// <code>
/// curl --location --request PATCH 'http://localhost:10492/api/group/members/add'
/// --header 'Content-Type: application/json'
/// --data-raw '{
///     "user_id": "string",
///     "group_id": "string",
///     "member_id": [
///         "string"
///     ]
/// }'
/// </code>
/// </summary>
void HttpClient::addMember(const QString &memberId) {
    qDebug() << "HttpClient::addMember";
    try {
        web::json::value requestData;
        requestData[_XPLATSTR("user_id")] = web::json::value::string(
            UserManager::getInstance()->getUserInfo().getId().toStdString());
        qDebug() << "user_id: " << UserManager::getInstance()->getUserInfo().getId();

        requestData[_XPLATSTR("group_id")] = web::json::value::string(
            GroupInfoManager::getInstance()->getGroupInfo().getId().toStdString());

        requestData[_XPLATSTR("members_id")] = web::json::value::array();
        requestData[_XPLATSTR("members_id")][0] = web::json::value::string(memberId.toStdString());

        web::http::http_request request(web::http::methods::PATCH);
        request.set_request_uri(_XPLATSTR("/group/members/add"));
        request.headers().set_content_type(_XPLATSTR("application/json"));
        request.set_body(requestData);

        const auto response = client.request(request).get();
        const auto responseData = response.extract_json().get();

        const auto &responseCode = responseData.at(_XPLATSTR("code")).as_number().to_int32();
        const auto &responseMessage = responseData.at(_XPLATSTR("message")).as_string();
        const auto &responseResult = responseData.at(_XPLATSTR("result")).as_string();
        if (responseCode != 200) {
            qDebug() << "HttpClient::addMember error " << responseCode;
            qDebug() << "HttpClient::addMember error " << responseMessage;
            qDebug() << "HttpClient::addMember error " << responseResult;
            emit requestFinish(responseCode, QString{responseResult.data()}, QString{responseMessage.data()});
            return;
        }
        emit requestFinish(responseCode, QString{responseResult.data()}, QString{responseMessage.data()});
    }
    catch (const std::exception &e) {
        qDebug() << "HttpClient::addMember exception: " << e.what();
        emit requestFailed(QString{e.what()});
    }
}

/// <summary>
/// <code>
/// curl --location --request DELETE 'http://localhost:10492/api/group/members/remove'
/// --header 'Content-Type: application/json'
/// --data-raw '{
///     "user_id": "string",
///     "group_id": "string",
///     "members_id": [
///         "string"
///     ]
/// }'
/// </code>
/// </summary>
void HttpClient::removeMember(const QString &memberId) {
    qDebug() << "HttpClient::removeMember " << memberId;
    try {
        web::json::value requestData;
        requestData[_XPLATSTR("user_id")] = web::json::value::string(
            UserManager::getInstance()->getUserInfo().getId().toStdString());
        qDebug() << "user_id: " << UserManager::getInstance()->getUserInfo().getId();

        requestData[_XPLATSTR("group_id")] = web::json::value::string(
            GroupInfoManager::getInstance()->getGroupInfo().getId().toStdString());

        requestData[_XPLATSTR("members_id")] = web::json::value::array();
        requestData[_XPLATSTR("members_id")][0] = web::json::value::string(memberId.toStdString());

        web::http::http_request request(web::http::methods::DEL);

        request.set_request_uri(_XPLATSTR("/group/members/remove"));
        request.headers().set_content_type(_XPLATSTR("application/json"));
        request.set_body(requestData);

        const auto response = client.request(request).get();
        const auto responseData = response.extract_json().get();
        const auto &responseCode = responseData.at(_XPLATSTR("code")).as_number().to_int32();
        const auto &responseMessage = responseData.at(_XPLATSTR("message")).as_string();
        const auto &responseResult = responseData.at(_XPLATSTR("result")).as_string();

        if (responseCode != 200) {
            qDebug() << "HttpClient::removeMember error " << responseCode;
            qDebug() << "HttpClient::removeMember error" << responseMessage;
            qDebug() << "HttpClient::removeMember error" << responseResult;
            emit requestFinish(responseCode, QString{responseResult.data()}, QString{responseMessage.data()});
            return;
        }
        emit requestFinish(responseCode, QString{responseResult.data()}, QString{responseMessage.data()});
    }
    catch (const std::exception &e) {
        qDebug() << "HttpClient::removeMember exception: " << e.what();
        emit requestFailed(QString{e.what()});
    }
}

/// <summary>
/// <code>curl --location --request GET 'http://localhost:10492/api/Group/all'</code>
/// </summary>
QList<GroupInfo> HttpClient::allGroupInfo() {
    qDebug() << "HttpClient::allGroupInfo";
    try {
        web::http::http_request request(web::http::methods::GET);
        request.set_request_uri(_XPLATSTR("/group/all"));
        const auto response = client.request(request).get();

        const auto responseData = response.extract_json().get();
        const auto &responseCode = responseData.at(_XPLATSTR("code")).as_number().to_int32();
        const auto &responseMessage = responseData.at(_XPLATSTR("message")).as_string();
        const auto &responseResult = responseData.at(_XPLATSTR("result")).as_string();

        if (responseCode != 200) {
            qDebug() << "HttpClient::allGroupInfo error " << responseCode;
            qDebug() << "HttpClient::allGroupInfo error " << responseMessage;
            qDebug() << "HttpClient::allGroupInfo error " << responseResult;
            return {};
        }

        const auto &resultData = responseData.at(_XPLATSTR("data")).as_array();
        QList<GroupInfo> groupInfoList;
        for (const auto &data: resultData) {
            const auto &id = data.at(_XPLATSTR("_id")).as_string();
            const auto &buckets = data.at(_XPLATSTR("buckets")).as_array();
            const auto &members = data.at(_XPLATSTR("members_id")).as_array();
            const auto &ownerId = data.at(_XPLATSTR("owner_id")).as_string();
            const auto &name = data.at(_XPLATSTR("name")).as_string();

            QList<Bucket> bucketList;
            for (const auto &bucket: buckets) {
                bucketList.append(Bucket{
                    .sourceId = QString::fromStdString(bucket.at(_XPLATSTR("source_id")).as_string()),
                    .bucketName = QString::fromStdString(bucket.at(_XPLATSTR("bucket_name")).as_string()),
                });
            }

            QList<QString> membersId;
            for (const auto &member: members) {
                membersId.append(QString::fromStdString(member.as_string()));
            }

            GroupInfo groupInfo;
            groupInfo.setId(QString::fromStdString(id));
            groupInfo.setBuckets(bucketList);
            groupInfo.setMembersId(membersId);
            groupInfo.setOwnerId(QString::fromStdString(ownerId));
            groupInfo.setName(QString::fromStdString(name));
            groupInfoList.append(groupInfo);
        }
        return groupInfoList;
    }
    catch (const std::exception &e) {
        qDebug() << "HttpClient::allGroupInfo exception: " << e.what();
        emit requestFailed(QString{e.what()});
        return {};
    }
}

/// <summary>
/// <code>
/// curl --location --request GET 'http://localhost:10492/api/object/list'
/// --header 'Content-Type: application/json'
/// --data-raw '{
///     "user_id": "6772b2141193cedbb302d3d2",
///     "bucket_name": "another-bucket",
///     "source_id": "6772b3a31193cedbb302d46f"
/// }'
/// </code>
/// </summary>
QList<OneObject> HttpClient::getObjects(const std::string &bucketName, const std::string &sourceId) {
    qDebug() << "HttpClient::getObjects";
    qDebug() << "HttpClient::getObjects bucketName: " << QString::fromStdString(bucketName);
    qDebug() << "HttpClient::getObjects sourceId: " << QString::fromStdString(sourceId);
    try {
        web::json::value requestData;
        requestData[_XPLATSTR("user_id")] = web::json::value::string(
            UserManager::getInstance()->getId().toStdString());
        requestData[_XPLATSTR("bucket_name")] = web::json::value::string(bucketName);
        requestData[_XPLATSTR("source_id")] = web::json::value::string(sourceId);

        web::http::http_request request(web::http::methods::GET);
        request.set_request_uri(_XPLATSTR("/object/list"));
        request.headers().set_content_type(_XPLATSTR("application/json"));
        request.set_body(requestData);
        const auto response = client.request(request).get();
        const auto responseData = response.extract_json().get();
        const auto &responseCode = responseData.at(_XPLATSTR("code")).as_number().to_int32();
        const auto &responseMessage = responseData.at(_XPLATSTR("message")).as_string();
        const auto &responseResult = responseData.at(_XPLATSTR("result")).as_string();
        if (responseCode != 200) {
            qDebug() << "HttpClient::getObjects error " << responseCode;
            qDebug() << "HttpClient::getObjects error " << responseMessage;
            qDebug() << "HttpClient::getObjects error " << responseResult;
            return {};
        }

        const auto &resultData = responseData.at(_XPLATSTR("data")).as_array();
        QList<OneObject> objects;
        for (const auto &data: resultData) {
            OneObject object{};
            object.name = QString::fromStdString(data.at(_XPLATSTR("name")).as_string());
            object.size = data.at(_XPLATSTR("size")).as_number().to_int32();
            object.etag = QString::fromStdString(data.at(_XPLATSTR("etag")).as_string());
            object.lastModified = QString::fromStdString(data.at(_XPLATSTR("last_modified")).as_string());
            object.versionId = QString::fromStdString(data.at(_XPLATSTR("version_id")).as_string());
            objects.append(object);
        }
        return objects;
    }
    catch (const std::exception &e) {
        qDebug() << "HttpClient::getObjects exception: " << e.what();
        emit requestFailed(QString{e.what()});
        return {};
    }
}

/// <summary>
/// 下载存储源中的对象
/// 正常返回为二进制文件
/// 失败返回code, result, message
/// <code>
/// curl --location --request GET 'http://localhost:10492/api/object/download'
///          --header 'Content-Type: application/json'
///          --data-raw '{
///          "user_id": "6772b2141193cedbb302d3d2",
///          "source_id": "6772b3a31193cedbb302d46f",
///          "bucket_name": "another-bucket",
///          "object_name": "05.jpg"
///      }' --output 5.jpg
/// </code>
/// </summary>
void HttpClient::objectDownload(const QString &objectName, const QString &bucketName, const QString &sourceId) {
    qDebug() << "HttpClient::objectDownlaod";
    try {
        web::json::value requestData;
        requestData[_XPLATSTR("user_id")] = web::json::value::string(
            UserManager::getInstance()->getId().toStdString());
        requestData[_XPLATSTR("source_id")] = web::json::value::string(sourceId.toStdString());
        requestData[_XPLATSTR("bucket_name")] = web::json::value::string(bucketName.toStdString());
        requestData[_XPLATSTR("object_name")] = web::json::value::string(objectName.toStdString());

        web::http::http_request request(web::http::methods::GET);
        request.set_request_uri(_XPLATSTR("/object/download"));
        request.headers().set_content_type(_XPLATSTR("application/json"));
        request.set_body(requestData);

        // Send the request asynchronously
        const auto requestTask = client.request(request).then([&](const web::http::http_response &response) {
            if (response.status_code() == web::http::status_codes::OK) {
                // Handle successful response (binary file)
                const auto fileStream = std::make_shared<concurrency::streams::ostream>();
                return concurrency::streams::fstream::open_ostream(_XPLATSTR(objectName.toStdString())).then(
                    [=](const concurrency::streams::ostream &outFile) {
                        *fileStream = outFile;
                        return response.body().read_to_end(fileStream->streambuf());
                    }).then([=](size_t) {
                    emit objectDownloadFinish();
                    return fileStream->close();
                });
            }
            // Handle error response (JSON)
            return response.extract_json().then([=](const web::json::value &jsonResponse) {
                emit requestFailed(QString::fromStdString(jsonResponse.serialize()));
            });
        });
        requestTask.get();
    }
    catch (const std::exception &e) {
        qDebug() << "HttpClient::objectDownload exception: " << e.what();
        emit requestFailed(QString{e.what()});
    }
}

/// <summary>
/// <code>
/// curl --location --request POST 'http://localhost:10492/api/object/upload' \
/// --form 'source_id="673ea940b4f544a2c2092b3d"' \
/// --form 'user_id="67459983948109dbd1079573"' \
/// --form 'bucket_name="another-bucket"' \
/// --form 'files=@""'
/// </code>
/// </summary>
void HttpClient::objectUpload(const QString &filePath, const QString &bucketName, const QString &sourceId) {
    qDebug() << "HttpClient::objectUpload";

    try {
        auto *manager = new QNetworkAccessManager();

        const QUrl url(QString("http://localhost:10492/api/object/upload"));
        const QNetworkRequest request(url);

        auto *multipart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

        QHttpPart sourceIdPart;
        sourceIdPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"source_id\""));
        sourceIdPart.setBody(sourceId.toUtf8());
        multipart->append(sourceIdPart);

        QHttpPart userIdPart;
        userIdPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"user_id\""));
        userIdPart.setBody(UserManager::getInstance()->getId().toUtf8());
        multipart->append(userIdPart);

        QHttpPart bucketNamePart;
        bucketNamePart.setHeader(QNetworkRequest::ContentDispositionHeader,
                                 QVariant("form-data; name=\"bucket_name\""));
        bucketNamePart.setBody(bucketName.toUtf8());
        multipart->append(bucketNamePart);

        //检查filePath开头是否存在file://如果有则删除
        auto path = filePath;
        if (filePath.startsWith("file://")) {
            path = filePath.mid(7);
        }
        const auto file = new QFile(path);
        if (!file->open(QIODevice::ReadOnly)) {
            qDebug() << "HttpClient::objectUpload file open failed";
            emit requestFailed("File open failed");
            return;
        }

        QHttpPart filePart;
        const auto fileName = QFileInfo(*file).fileName();
        filePart.setHeader(QNetworkRequest::ContentDispositionHeader,
                           QVariant("form-data; name=\"files\"; filename=\"" + fileName + "\""));
        filePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/octet-stream"));
        //filePart.setBody(file->readAll());
        filePart.setBodyDevice(file);
        file->setParent(multipart);

        multipart->append(filePart);

        QNetworkReply *reply = manager->post(request, multipart);
        connect(reply, &QNetworkReply::finished, [=]() {
            if (reply->error() == QNetworkReply::NoError) {
                emit objectUploadFinish();
            }
            else {
                qDebug() << "HttpClient::objectUpload error: " << reply->errorString();
                emit requestFailed(reply->errorString());
            }
            reply->deleteLater();
            multipart->deleteLater();
        });
    }
    catch (const std::exception &e) {
        qDebug() << "HttpClient::objectUpload exception: " << e.what();
    }
}


/// <summary>
/// <code>
/// curl --location --request DELETE 'http://localhost:10492/api/Object/remove'
/// --header 'Content-Type: application/json'
/// --data-raw '{
///     "user_id": "6772b2141193cedbb302d3d2",
///     "source_id": "6772b3a31193cedbb302d46f",
///     "bucket_name": "another-bucket",
///     "object_name": "第0章.docx"
/// }'
/// </code>
/// </summary>
void HttpClient::objectRemove(const QString &objectName, const QString &bucketName, const QString &sourceId) {
    qDebug() << "HttpClient::objectRemove";
    try {
        web::http::http_request request(web::http::methods::DEL);
        request.set_request_uri(_XPLATSTR("/object/remove"));
        request.headers().set_content_type(_XPLATSTR("application/json"));

        web::json::value json;
        json[_XPLATSTR("user_id")] = web::json::value::string(
            _XPLATSTR(UserManager::getInstance()->getId().toStdString()));
        json[_XPLATSTR("source_id")] = web::json::value::string(_XPLATSTR(sourceId.toStdString()));
        json[_XPLATSTR("bucket_name")] = web::json::value::string(_XPLATSTR(bucketName.toStdString()));
        json[_XPLATSTR("object_name")] = web::json::value::string(_XPLATSTR(objectName.toStdString()));
        request.set_body(json);
        const auto requestTask = client.request(request).then([&](const web::http::http_response &response) {
            if (response.status_code() != web::http::status_codes::OK) {
                qDebug() << "HttpClient::objectRemove error:" << response.status_code();
                emit requestFailed(QString::fromStdString(response.extract_string().get()));
            }
            const auto responseData = response.extract_json().get();
            const auto &responseCode = responseData.at(_XPLATSTR("code")).as_number().to_int32();
            const auto &responseMessage = responseData.at(_XPLATSTR("message")).as_string();
            const auto &responseResult = responseData.at(_XPLATSTR("result")).as_string();
            if (responseCode != 200) {
                qDebug() << "HttpClient::objectRemove error " << responseCode;
                qDebug() << "HttpClient::objectRemove error " << responseMessage;
                qDebug() << "HttpClient::objectRemove error " << responseResult;
                emit requestFailed(QString::fromStdString(responseResult));
                return;
            }
            emit objectRemoveFinish();
        });
    }
    catch (const std::exception &e) {
        qDebug() << "HttpClient::objectRemove exception: " << e.what();
        emit requestFailed(QString::fromStdString(e.what()));
    }
}

/// <summary>
/// curl --location --request PATCH 'http://localhost:10492/api/Group/buckets/add' \
/// --header 'Content-Type: application/json' \
/// --data-raw '{
///     "user_id": "string",
///     "group_id": "string",
///     "source_id": "string",
///     "bucket_name": "string"
/// }'
/// </summary>
void HttpClient::addGroupBucket(const QString &bucketName, const QString &sourceId, const QString &groupId) {
    qDebug() << "HttpClient::addGroupBucket";
    qDebug() << "HttpClient::addGroupBucket bucketName:" << bucketName;
    qDebug() << "HttpClient::addGroupBucket sourceId:" << sourceId;
    qDebug() << "HttpClient::addGroupBucket groupId:" << groupId;

    try {
        web::http::http_request request(web::http::methods::PATCH);
        request.set_request_uri(_XPLATSTR("/group/buckets/add"));
        request.headers().set_content_type(_XPLATSTR("application/json"));
        web::json::value json;
        json[_XPLATSTR("user_id")] = web::json::value::string(
            _XPLATSTR(UserManager::getInstance()->getId().toStdString()));
        json[_XPLATSTR("group_id")] = web::json::value::string(groupId.toStdString());
        json[_XPLATSTR("source_id")] = web::json::value::string(_XPLATSTR(sourceId.toStdString()));
        json[_XPLATSTR("bucket_name")] = web::json::value::string(_XPLATSTR(bucketName.toStdString()));
        request.set_body(json);
        const auto requestTask = client.request(request).then([&](const web::http::http_response &response) {
            if (response.status_code() != web::http::status_codes::OK) {
                qDebug() << "HttpClient::addBucket error:" << response.status_code();
                emit requestFailed(QString::fromStdString(response.extract_string().get()));
            }
            const auto responseData = response.extract_json().get();
            const auto &responseCode = responseData.at(_XPLATSTR("code")).as_number().to_int32();
            const auto &responseMessage = responseData.at(_XPLATSTR("message")).as_string();
            const auto &responseResult = responseData.at(_XPLATSTR("result")).as_string();
            if (responseCode != 200) {
                qDebug() << "HttpClient::addBucket error " << responseCode;
                qDebug() << "HttpClient::addBucket error " << responseMessage;
                qDebug() << "HttpClient::addBucket error " << responseResult;
                emit requestFailed(QString::fromStdString(responseResult));
                return;
            }
            emit requestFinish(responseCode, QString::fromStdString(responseResult),
                               QString::fromStdString(responseMessage));
        });
        requestTask.get();
    }
    catch (const std::exception &e) {
        qDebug() << "HttpClient::addGroupBucket exception: " << e.what();
        emit requestFailed(QString::fromStdString(e.what()));
    }
}

/// <summary>
/// curl --location --request DELETE 'http://localhost:10492/api/Group/buckets/remove'
/// --header 'Content-Type: application/json'
/// --data-raw '{
///     "user_id": "67459983948109dbd1079573",
///     "group_id": "67459983948109dbd1079575",
///     "source_id": "673ea940b4f544a2c2092b3d",
///     "bucket_name": "another-bucket"
/// }'
/// </summary>
void HttpClient::removeGroupBucket(const QString &bucketName, const QString &sourceId, const QString &groupId) {
    qDebug() << "HttpClient::removeGroupBucket";
    qDebug() << "HttpClient::removeGroupBucket bucketName:" << bucketName;
    qDebug() << "HttpClient::removeGroupBucket sourceId:" << sourceId;
    qDebug() << "HttpClient::removeGroupBucket groupId:" << groupId;
    if (bucketName.isEmpty() || sourceId.isEmpty() || groupId.isEmpty()) {
        emit requestFailed(
            QString("HttpClient::removeGroupBucket error: bucketName, sourceId, groupId cannot be empty"));
        return;
    }

    try {
        web::http::http_request request(web::http::methods::DEL);
        request.set_request_uri(_XPLATSTR("/group/buckets/remove"));
        request.headers().set_content_type(_XPLATSTR("application/json"));

        web::json::value json;
        json[_XPLATSTR("user_id")] = web::json::value::string(
            _XPLATSTR(UserManager::getInstance()->getId().toStdString()));
        json[_XPLATSTR("group_id")] = web::json::value::string(_XPLATSTR(groupId.toStdString()));
        json[_XPLATSTR("source_id")] = web::json::value::string(_XPLATSTR(sourceId.toStdString()));
        json[_XPLATSTR("bucket_name")] = web::json::value::string(_XPLATSTR(bucketName.toStdString()));
        request.set_body(json);

        const auto requestTask = client.request(request).then([&](const web::http::http_response &response) {
            if (response.status_code() != web::http::status_codes::OK) {
                qDebug() << "HttpClient::removeGroupBucket error:" << response.status_code();
                emit requestFailed(QString::fromStdString(response.extract_string().get()));
            }
            const auto responseData = response.extract_json().get();
            const auto &responseCode = responseData.at(_XPLATSTR("code")).as_number().to_int32();
            const auto &responseMessage = responseData.at(_XPLATSTR("message")).as_string();
            const auto &responseResult = responseData.at(_XPLATSTR("result")).as_string();
            if (responseCode != 200) {
                qDebug() << "HttpClient::removeGroupBucket error " << responseCode;
                qDebug() << "HttpClient::removeGroupBucket error " << responseMessage;
                qDebug() << "HttpClient::removeGroupBucket error " << responseResult;
                emit requestFailed(QString::fromStdString(responseResult));
                return;
            }
            emit requestFinish(responseCode, QString::fromStdString(responseResult),
                               QString::fromStdString(responseMessage));
        });
        requestTask.get();
    }
    catch (const std::exception &e) {
        qDebug() << "HttpClient::addGroupBucket exception: " << e.what();
        emit requestFailed(QString::fromStdString(e.what()));
    }
}


void HttpClient::addBucket(const QString &bucketName, const QString &sourceId) {
    try {
    }
    catch (const std::exception &e) {
        qDebug() << "HttpClient::addGroupBucket exception: " << e.what();
        emit requestFailed(QString::fromStdString(e.what()));
    }
}

void HttpClient::removeBucket(const QString &bucketName, const QString &sourceId) {
    try {
    }
    catch (const std::exception &e) {
        qDebug() << "HttpClient::addGroupBucket exception: " << e.what();
        emit requestFailed(QString::fromStdString(e.what()));
    }
}


HttpClient::HttpClient()
    : baseUrl("http://localhost:10492/api"), client(_XPLATSTR(baseUrl)) {
    qDebug() << "HttpClient::HttpClient";
}

//
// Created by cvrain on 24-12-8.
//

#ifndef HTTP_CLINT_HPP
#define HTTP_CLINT_HPP
#define _TURN_OFF_PLATFORM_STRING

#include <cpprest/http_client.h>
#include <QObject>

#include "user_info.hpp"
#include "types/response.hpp"
#include "types/result.hpp"
#include "types/storage_source.hpp"

class StorageSource;
/**
 * @brief singleton http clint
 */
class HttpClient final : public QObject {
    Q_OBJECT

public:
    static HttpClient* getInstance();

    ~HttpClient() override;

    Q_INVOKABLE void login(const QString &userName, const QString &password);

    Q_INVOKABLE void userRegister(const QString &userName, const QString &password, int role);

    Q_INVOKABLE void userInfo();

    QList<StorageSource> storageSource();

    CommonResponse addStorageSource(const StorageSource& source);

    CommonResponse removeStorageSource(const std::string& storageId);

signals:
    void requestFailed(const QString &error);

    void requestFinish(int code, const QString &result, const QString &message);

    void userLogged(int code, const QString &result, const QString &message);

    void userRegistered(int code, const QString &result, const QString &message);

    void userInfoFetched(int code, const QString &result, const QString &message);

private:
    explicit HttpClient();

    const std::string baseUrl;
    web::http::client::http_client client;
};

#endif //HTTP_CLINT_HPP

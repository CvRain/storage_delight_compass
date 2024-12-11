//
// Created by cvrain on 24-12-8.
//

#ifndef HTTP_CLINT_HPP
#define HTTP_CLINT_HPP

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>
#include <QJsonObject>

#include "user_info.hpp"
#include "types/result.hpp"

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

    Q_INVOKABLE void userInfo(const QString &userInfo);

signals:
    void requestFailed(const QString &error);

    void userLogged(int code, const QString &result, const QString &message);

    void userRegistered(int code, const QString &result, const QString &message);

    void userInfoFetched(int code, const QString &result, const QString &message);

protected:
    void userLoginResponse(QNetworkReply *reply);

    void userRegisterResponse(QNetworkReply *reply);

    void userInfoResponse(QNetworkReply *userInfo);

private:
    explicit HttpClient();

    const QString baseUrl;
    QNetworkAccessManager *manager;
};

#endif //HTTP_CLINT_HPP

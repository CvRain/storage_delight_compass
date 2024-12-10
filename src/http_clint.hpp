//
// Created by cvrain on 24-12-8.
//

#ifndef HTTP_CLINT_HPP
#define HTTP_CLINT_HPP

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>

#include "types/result.hpp"

/**
 * @brief singleton http clint
 */
class HttpClient final : public QObject {
    Q_OBJECT
public:
    static HttpClient* getInstance();

    ~HttpClient() override;

    Q_INVOKABLE BaseResult login(const QString &userName, const QString &password);
protected:
    static void replyHandler(const QNetworkReply* reply);
private:
    explicit HttpClient();

    const QString baseUrl;
    QNetworkAccessManager *manager;
};


#endif //HTTP_CLINT_HPP

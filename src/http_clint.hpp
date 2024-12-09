//
// Created by cvrain on 24-12-8.
//

#ifndef HTTP_CLINT_HPP
#define HTTP_CLINT_HPP
#define _TURN_OFF_PLATFORM_STRING

#include <QNetworkAccessManager>
#include <QObject>

/**
 * @brief singleton http clint
 */
class HttpClint final : public QObject {
    Q_OBJECT
public:
    static HttpClint* getInstance();

    ~HttpClint() override;

    Q_INVOKABLE void login(const QString &userName, const QString &password);

private:
    explicit HttpClint();

    const QString baseUrl;
    QNetworkAccessManager *manager;
};


#endif //HTTP_CLINT_HPP

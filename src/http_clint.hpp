//
// Created by cvrain on 24-12-8.
//

#ifndef HTTP_CLINT_HPP
#define HTTP_CLINT_HPP
#define _TURN_OFF_PLATFORM_STRING

#include <cpprest/http_client.h>
#include <QObject>

#include "group_info.hpp"
#include "user_info.hpp"
#include "types/object_model.hpp"
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

    QList<UserInfo> userInfoList();

    QString removeUser(const std::string& user_id);

    std::optional<GroupInfo> groupInfo();

    std::optional<std::pair<QStringList, QStringList>> groupMemberNames(const QStringList& names);

    void addMember(const QString& memberId);

    void removeMember(const QString& memberId);

    QList<GroupInfo> allGroupInfo();

    QList<OneObject> getObjects(const std::string& bucketName, const std::string& sourceId);

    Q_INVOKABLE void objectDownload(const QString &objectName, const QString &bucketName, const QString &sourceId);

    Q_INVOKABLE void objectUpload(const QString &filePath, const QString &bucketName, const QString &sourceId);

    Q_INVOKABLE void objectRemove(const QString &objectName, const QString &bucketName, const QString &sourceId);

signals:
    void requestFailed(const QString &error);

    void requestFinish(int code, const QString &result, const QString &message);

    void userLogged(int code, const QString &result, const QString &message);

    void userRegistered(int code, const QString &result, const QString &message);

    void userInfoFetched(int code, const QString &result, const QString &message);

    void objectDownloadFinish();

    void objectUploadFinish();

    void objectRemoveFinish();

private:
    explicit HttpClient();

    const std::string baseUrl;
    web::http::client::http_client client;
};

#endif //HTTP_CLINT_HPP

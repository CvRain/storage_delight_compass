//
// Created by cvrain on 24-12-9.
//

#ifndef USER_INFO_HPP
#define USER_INFO_HPP

#include <QObject>
#include <QString>

class UserInfo : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString id READ getId WRITE setId NOTIFY userInfoChanged)
    Q_PROPERTY(QString name READ getName WRITE setName NOTIFY userInfoChanged)
    Q_PROPERTY(QString token READ getToken WRITE setToken NOTIFY userInfoChanged)
    Q_PROPERTY(QString groupId READ getGroupId WRITE setGroupId NOTIFY userInfoChanged)
    Q_PROPERTY(int role READ getRole WRITE setRole NOTIFY userInfoChanged)
    Q_PROPERTY(int createTime READ getCreateTime WRITE setCreateTime NOTIFY userInfoChanged)

public:
    explicit UserInfo(QObject *parent = nullptr);

    UserInfo(UserInfo&& source, QObject *parent = nullptr);

    UserInfo(const UserInfo& source, QObject *parent = nullptr);

    UserInfo& operator=(const UserInfo& source);

    [[nodiscard]] Q_INVOKABLE QString getId() const;

    [[nodiscard]] Q_INVOKABLE QString getToken() const;

    [[nodiscard]] Q_INVOKABLE QString getName() const;

    [[nodiscard]] Q_INVOKABLE QString getGroupId() const;

    [[nodiscard]] Q_INVOKABLE int getRole() const;

    [[nodiscard]] Q_INVOKABLE int getCreateTime() const;

    Q_INVOKABLE QString getCreateDate();

    Q_INVOKABLE void setId(const QString &userId);

    Q_INVOKABLE void setToken(const QString &userToken);

    Q_INVOKABLE void setName(const QString &userName);

    Q_INVOKABLE void setGroupId(const QString &groupId);

    Q_INVOKABLE void setRole(int role);

    Q_INVOKABLE void setCreateTime(int time);

signals:
    void userInfoChanged();

private:
    QString id;
    QString token;
    QString name;
    QString groupId;
    int role;
    int createTime;
};


class UserManager : public QObject {
    Q_OBJECT

public:
    static UserManager* getInstance();

    Q_INVOKABLE void setLoginStatus(bool status);

    Q_INVOKABLE UserInfo& getUserInfo();

    [[nodiscard]] Q_INVOKABLE QString getId() const;

    [[nodiscard]] Q_INVOKABLE QString getToken() const;

    [[nodiscard]] Q_INVOKABLE QString getName() const;

    [[nodiscard]] Q_INVOKABLE QString getGroupId() const;

    [[nodiscard]] Q_INVOKABLE int getRole() const;

    [[nodiscard]] Q_INVOKABLE int getCreateTime() const;

    [[nodiscard]] Q_INVOKABLE QString getCreateDate();

    Q_INVOKABLE void setId(const QString &userId);

    Q_INVOKABLE void setToken(const QString &userToken);

    Q_INVOKABLE void setName(const QString &userName);

    Q_INVOKABLE void setGroupId(const QString &groupId);

    Q_INVOKABLE void setRole(int role);

    Q_INVOKABLE void setCreateTime(int time);

signals:
    void loginStatusChanged(bool status);

    void userInfoChanged(const UserInfo &info);

private:
    explicit UserManager(QObject *parent = nullptr);

    bool isLogin{};
    UserInfo info;
};

#endif //USER_INFO_HPP

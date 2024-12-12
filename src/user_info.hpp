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

public:
    explicit UserInfo(QObject *parent = nullptr);

    [[nodiscard]] Q_INVOKABLE QString getId() const;

    [[nodiscard]] Q_INVOKABLE QString getToken() const;

    [[nodiscard]] Q_INVOKABLE QString getName() const;

    [[nodiscard]] Q_INVOKABLE QString getGroupId() const;

    [[nodiscard]] Q_INVOKABLE int getRole() const;

    Q_INVOKABLE void setId(const QString &userId);

    Q_INVOKABLE void setToken(const QString &userToken);

    Q_INVOKABLE void setName(const QString &userName);

    Q_INVOKABLE void setGroupId(const QString &groupId);

    Q_INVOKABLE void setRole(int role);
signals:
    void userInfoChanged();

private:
    QString id;
    QString token;
    QString name;
    QString groupId;
    int role;
};


class UserManager : public QObject {
    Q_OBJECT

public:
    static UserManager* getInstance();

    Q_INVOKABLE void setLoginStatus(bool status);
    UserInfo& getUserInfo();

signals:
    void loginStatusChanged(bool status);
    void userInfoChanged(const UserInfo& info);
private:
    explicit UserManager(QObject *parent = nullptr);

    bool isLogin{};
    UserInfo info;
};

#endif //USER_INFO_HPP

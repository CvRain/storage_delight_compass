//
// Created by cvrain on 24-12-27.
//

#ifndef GROUP_INFO_HPP
#define GROUP_INFO_HPP

#include <QObject>
#include <QList>
#include <QQmlListProperty>

#include "types/buckets.hpp"

class GroupInfo : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString id READ getId WRITE setId NOTIFY groupInfoChanged)
    Q_PROPERTY(QString name READ getName WRITE setName NOTIFY groupInfoChanged)
    Q_PROPERTY(QList<Bucket> buckets READ getBuckets WRITE setBuckets NOTIFY groupInfoChanged)
    Q_PROPERTY(QList<QString> membersId READ getMembersId NOTIFY groupInfoChanged)
    Q_PROPERTY(QStringList bucketsName READ getBucketsName NOTIFY groupInfoChanged)

public:
    explicit GroupInfo(QObject *parent = nullptr);

    GroupInfo(const GroupInfo &info, QObject *parent = nullptr);

    GroupInfo(GroupInfo &&info, QObject *parent = nullptr) noexcept;

    GroupInfo& operator=(const GroupInfo &info);

    [[nodiscard]] Q_INVOKABLE QString getId() const;

    [[nodiscard]] Q_INVOKABLE QString getName() const;

    [[nodiscard]] Q_INVOKABLE QList<Bucket> getBuckets() const;

    [[nodiscard]] Q_INVOKABLE Bucket getBucket(int index) const;

    [[nodiscard]] Q_INVOKABLE QStringList getBucketsName() const;

    [[nodiscard]] Q_INVOKABLE QList<QString> getMembersId() const;

    [[nodiscard]] Q_INVOKABLE QString getMemberId(int index) const;

    Q_INVOKABLE void setId(const QString &id);

    Q_INVOKABLE void setName(const QString &name);

    Q_INVOKABLE void setBuckets(const QList<Bucket> &buckets);

    Q_INVOKABLE void setMembersId(const QList<QString> &membersId);
signals:
    void groupInfoChanged();

private:
    QString id;
    QString name;
    QList<Bucket> buckets;
    QList<QString> membersId;
};

class GroupInfoManager : public QObject {
    Q_OBJECT

public:
    static GroupInfoManager* getInstance();

    Q_INVOKABLE void update();

    Q_INVOKABLE QString getName() const;

    GroupInfo getGroupInfo() const;

    Q_INVOKABLE static void addMember(const QString& memberId);

    Q_INVOKABLE static void removeMember(const QString& memberId);

signals:
    void groupInfoChanged();

    void groupError(const QString &error);

private:
    explicit GroupInfoManager(QObject *parent = nullptr);

    GroupInfo groupInfo;
};


#endif //GROUP_INFO_HPP

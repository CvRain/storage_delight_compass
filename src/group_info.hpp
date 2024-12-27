//
// Created by cvrain on 24-12-27.
//

#ifndef GROUP_INFO_HPP
#define GROUP_INFO_HPP

#include <QObject>
#include <QList>

#include "types/buckets.hpp"

class GroupInfo : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString id READ getId WRITE setId NOTIFY groupInfoChanged)
    Q_PROPERTY(QString name READ getName WRITE setName NOTIFY groupInfoChanged)
    Q_PROPERTY(QList<Bucket> buckets READ getBuckets WRITE setBuckets NOTIFY groupInfoChanged)
    Q_PROPERTY(QList<QString> membersId READ getMembersId NOTIFY groupInfoChanged)
    Q_PROPERTY(QStringList bucketsNameREAD getBucketsName NOTIFY groupInfoChanged)
public:
    explicit GroupInfo(QObject *parent = nullptr);

    GroupInfo(const GroupInfo &info, QObject *parent = nullptr);

    GroupInfo(GroupInfo &&info, QObject *parent = nullptr) noexcept;

    GroupInfo& operator=(const GroupInfo &info);

    Q_INVOKABLE QString getId() const;

    Q_INVOKABLE QString getName() const;

    Q_INVOKABLE QList<Bucket> getBuckets() const;

    Q_INVOKABLE Bucket getBucket(int index) const;

    Q_INVOKABLE QList<QString> getMembersId() const;

    Q_INVOKABLE QString getMemberId(int index) const;

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


#endif //GROUP_INFO_HPP

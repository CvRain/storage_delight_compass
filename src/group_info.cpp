//
// Created by cvrain on 24-12-27.
//

#include "group_info.hpp"

#include <ranges>
#include <QDebug>
#include "http_clint.hpp"

GroupInfo::GroupInfo(QObject *parent)
    : QObject(parent), id{}, name{}, ownerId{}, buckets{}, membersId{} {
}

GroupInfo::GroupInfo(const GroupInfo &info, QObject *parent)
    : QObject{parent}, id{info.id}, name{info.name}, ownerId{info.ownerId}, buckets{info.buckets},
      membersId{info.membersId} {
}

GroupInfo::GroupInfo(GroupInfo &&info, QObject *parent) noexcept
    : QObject{parent}, id{std::move(info.id)}, name{std::move(info.name)}, ownerId{std::move(info.ownerId)},
      buckets{std::move(info.buckets)}, membersId{std::move(info.membersId)} {
}

GroupInfo& GroupInfo::operator=(const GroupInfo &info) {
    id = info.id;
    name = info.name;
    buckets = info.buckets;
    membersId = info.membersId;
    ownerId = info.ownerId;
    return *this;
}

QString GroupInfo::getId() const {
    return id;
}

QString GroupInfo::getName() const {
    return name;
}

QList<Bucket> GroupInfo::getBuckets() const {
    return buckets;
}

Bucket GroupInfo::getBucket(const int index) const {
    return buckets[index];
}

QStringList GroupInfo::getBucketsName() const {
    QStringList names;
    std::ranges::for_each(buckets, [&names](const auto &bucket) {
        names.append(bucket.bucketName);
    });
    return names;
}

QList<QString> GroupInfo::getMembersId() const {
    return membersId;
}

QString GroupInfo::getMemberId(const int index) const {
    return membersId[index];
}

QString GroupInfo::getOwnerId() const {
    return ownerId;
}

void GroupInfo::setId(const QString &id) {
    this->id = id;
}

void GroupInfo::setName(const QString &name) {
    this->name = name;
}

void GroupInfo::setBuckets(const QList<Bucket> &buckets) {
    this->buckets = buckets;
}

void GroupInfo::setMembersId(const QList<QString> &membersId) {
    this->membersId = membersId;
}

void GroupInfo::setOwnerId(const QString &ownerId) {
    this->ownerId = ownerId;
}

GroupInfoManager* GroupInfoManager::getInstance() {
    qDebug() << "GroupInfoManager::getInstance()";
    static auto *manager = new GroupInfoManager();
    return manager;
}

void GroupInfoManager::update() {
    const auto result = HttpClient::getInstance()->groupInfo();
    if (not result.has_value()) {
        return;
    }
    this->groupInfo = result.value();
    qDebug() << "GroupInfoManager::update(): " << groupInfo.getName();
}

QString GroupInfoManager::getName() const {
    qDebug() << "GroupInfoManager::getName(): " << groupInfo.getName();
    return groupInfo.getName();
}

GroupInfo GroupInfoManager::getGroupInfo() const {
    return groupInfo;
}

void GroupInfoManager::addMember(const QString &memberId) {
    HttpClient::getInstance()->addMember(memberId);
}

void GroupInfoManager::removeMember(const QString &memberId) {
    HttpClient::getInstance()->removeMember(memberId);
}


GroupInfoManager::GroupInfoManager(QObject *parent) : QObject(parent) {
    qDebug() << "GroupInfoManager::GroupInfoManager()";
    update();
}

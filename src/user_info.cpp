//
// Created by cvrain on 24-12-9.
//

#include "user_info.hpp"
#include <QDate>

UserInfo::UserInfo(QObject *parent): QObject(parent), role(0), createTime(0) {
}

UserInfo::UserInfo(UserInfo &&source, QObject *parent) noexcept
    : QObject(parent),
      id(std::move(source.id)),
      token(std::move(source.token)),
      name(std::move(source.name)),
      groupId(std::move(source.groupId)),
      role(source.role),
      createTime(source.createTime) {
}

UserInfo::UserInfo(const UserInfo &source, QObject *parent)
    : QObject(parent),
      id(source.id),
      token(source.token),
      name(source.name),
      groupId(source.groupId),
      role(source.role),
      createTime(source.createTime) {
}

UserInfo & UserInfo::operator=(const UserInfo &source) {
    id = source.id;
    token = source.token;
    name = source.name;
    groupId = source.groupId;
    role = source.role;
    createTime = source.createTime;
    return *this;
}

QString UserInfo::getId() const {
    return id;
}

QString UserInfo::getToken() const {
    return token;
}

QString UserInfo::getName() const {
    return name;
}

QString UserInfo::getGroupId() const {
    return groupId;
}

int UserInfo::getRole() const {
    return role;
}

int UserInfo::getCreateTime() const {
    return createTime;
}

QString UserInfo::getCreateDate() {
    const auto dateTime = QDateTime::fromSecsSinceEpoch(createTime);
    const auto dateString = dateTime.toString("yyyy-MM-dd");
    return dateString;
}

void UserInfo::setId(const QString &userId) {
    id = userId;
}

void UserInfo::setToken(const QString &userToken) {
    token = userToken;
}

void UserInfo::setName(const QString &userName) {
    name = userName;
}

void UserInfo::setGroupId(const QString &groupId) {
    this->groupId = groupId;
}

void UserInfo::setRole(const int role) {
    this->role = role;
}

void UserInfo::setCreateTime(const int time) {
    this->createTime = time;
}

UserManager* UserManager::getInstance() {
    static auto *instance = new UserManager();
    return instance;
}

void UserManager::setLoginStatus(const bool status) {
    isLogin = status;
    emit loginStatusChanged(status);
}

UserInfo& UserManager::getUserInfo() {
    return this->info;
}

QString UserManager::getId() const {
    return info.getId();
}

QString UserManager::getToken() const {
    return info.getToken();
}

void UserManager::setId(const QString &userId) {
    info.setId(userId);
}

void UserManager::setToken(const QString &userToken) {
    info.setToken(userToken);
}

void UserManager::setName(const QString &userName) {
    info.setName(userName);
}

void UserManager::setGroupId(const QString &groupId) {
    info.setGroupId(groupId);
}

void UserManager::setRole(int role) {
    info.setRole(role);
}

void UserManager::setCreateTime(int time) {
    info.setCreateTime(time);
}

QString UserManager::getName() const {
    return info.getName();
}

QString UserManager::getGroupId() const {
    return info.getGroupId();
}

int UserManager::getRole() const {
    return info.getRole();
}

int UserManager::getCreateTime() const {
    return info.getCreateTime();
}

QString UserManager::getCreateDate() {
    return info.getCreateDate();
}

UserManager::UserManager(QObject *parent) : QObject(parent) {
}

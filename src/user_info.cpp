//
// Created by cvrain on 24-12-9.
//

#include "user_info.hpp"

UserInfo::UserInfo(QObject *parent): QObject(parent) {
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

UserManager * UserManager::getInstance() {
    static auto *instance = new UserManager();
    return instance;
}

void UserManager::setLoginStatus(const bool status) {
    isLogin = status;
    emit loginStatusChanged(status);
}

UserManager::UserManager(QObject *parent) : QObject(parent){
}

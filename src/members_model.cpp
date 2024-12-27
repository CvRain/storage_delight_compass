//
// Created by cvrain on 24-12-26.
//

#include "members_model.hpp"

#include "http_clint.hpp"

MembersModel::MembersModel(QObject *parent) : QAbstractListModel(parent) {
    qDebug() << "MembersModel::MembersModel";
    connect(HttpClient::getInstance(), &HttpClient::requestFailed, this, &MembersModel::requestFailed);
    connect(HttpClient::getInstance(), &HttpClient::requestFinish, this, &MembersModel::requestFinish);
    update();
}

int MembersModel::rowCount(const QModelIndex &parent) const {
    return static_cast<int>(members.size());
}

QVariant MembersModel::data(const QModelIndex &index, int role) const {
    if (index.row() < 0 || index.row() >= members.size()) {
        return {};
    }
    switch (role) {
        case IdRole:
            return members.at(index.row()).getId();
        case NameRole:
            return members.at(index.row()).getName();
        case CreateTimeRole:
            return members.at(index.row()).getCreateTime();
        default:
            return {};
    }
}

QVariantMap MembersModel::get(const int index) const {
    return {
        {"id", members.at(index).getId()},
        {"name", members.at(index).getName()},
        {"createTime", members.at(index).getCreateTime()}
    };
}

QHash<int, QByteArray> MembersModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name"; // 关键：将 NameRole 映射到 "name"
    roles[IdRole] = "id"; // 同样需要映射 IdRole
    roles[CreateTimeRole] = "createTime"; // 映射 CreateTimeRole
    return roles;
}

QModelIndex MembersModel::index(const int row, const int column, const QModelIndex &parent) const {
    return createIndex(row, column);
}

void MembersModel::addMember(const QVariant &memberData) {
    if (!memberData.canConvert<QVariantMap>()) {
        qWarning() << "Invalid source data";
        emit requestFailed("Invalid source data");
        return;
    }

    qDebug() << "MembersModel::addMember";
    auto dataMap = memberData.toMap();
    const auto &name = dataMap["name"].toString();
    const auto &password = dataMap["password"].toString();
    qDebug() << "add user name: " << name << " password: " << password;
    HttpClient::getInstance()->userRegister(name, password, 1);
    update();
}

void MembersModel::removeMember(const QString &memberId) {
    qDebug() << "MembersModel::removeMember";
    qDebug() << "remove user id: " << memberId;
    HttpClient::getInstance()->removeUser(memberId.toStdString());
    update();
}

void MembersModel::update() {
    beginResetModel();
    this->members = HttpClient::getInstance()->userInfoList();
    endResetModel();
    emit memberUpdated();
}

//
// Created by cvrain on 24-12-30.
//

#include "group_list_model.hpp"

#include <QObject>
#include "src/http_clint.hpp"

GroupListModel::GroupListModel(QObject *parent): QAbstractListModel(parent) {
    update();
}

QVariant GroupListModel::data(const QModelIndex &index, const int role) const {
    if (!index.isValid()) {
        return QVariant();
    }
    switch (role) {
        case IdRole:
            return QVariant(groupList[index.row()].getId());
        case NameRole:
            return QVariant(groupList[index.row()].getName());
        case OwnerRole:
            return QVariant(groupList[index.row()].getOwnerId());
        default:
            return QVariant();
    }
}

QHash<int, QByteArray> GroupListModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[IdRole] = "id";
    roles[NameRole] = "name";
    roles[OwnerRole] = "owner";
    return roles;
}

int GroupListModel::rowCount(const QModelIndex &parent) const {
    return groupList.size();
}

QModelIndex GroupListModel::index(const int row, const int column, const QModelIndex &parent) const {
    if (parent.isValid()) {
        return QModelIndex();
    }
    return createIndex(row, column);
}

QVariantMap GroupListModel::get(const int index) const {
    if (index < 0 || index >= groupList.size()) {
        return QVariantMap();
    }
    return QVariantMap{
        {"id", groupList.at(index).getId()},
        {"name", groupList.at(index).getName()},
        {"owner", groupList.at(index).getOwnerId()}
    };
}

void GroupListModel::update() {
    beginResetModel();
    this->groupList = HttpClient::getInstance()->allGroupInfo();
    endResetModel();
}

QVariantList GroupListModel::getGroupMembers(const int index) const {
    if (index < 0 || index >= groupList.size()) {
        return QVariantList();
    }
    const auto &members = groupList.at(index).getMembersId();
    return QVariantList(members.begin(), members.end());
}

BucketListModel GroupListModel::getBucketsModel(const int index) const {
    BucketListModel model;
    model.setBuckets(groupList.at(index).getBuckets());
    return model;
}

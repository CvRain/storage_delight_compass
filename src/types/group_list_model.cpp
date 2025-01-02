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
        return {};
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
        return {};
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

bool GroupListModel::isOwner(const int index) const {
    if (index < 0 || index >= groupList.size()) {
        return false;
    }
    const auto& currentGroup = groupList.at(index);
    return currentGroup.getOwnerId() == UserManager::getInstance()->getId();
}

bool GroupListModel::isMember(const int index) const {
    if (index < 0 || index >= groupList.size()) {
        return false;
    }
    const auto& currentGroup = groupList.at(index);
    return currentGroup.getMembersId().contains(UserManager::getInstance()->getId());
}

QVariantList GroupListModel::getGroupMembers(const int index) const {
    if (index < 0 || index >= groupList.size()) {
        return {};
    }
    const auto &members = groupList.at(index).getMembersId();
    QVariantList memberList;
    std::ranges::transform(members, std::back_inserter(memberList), [](const auto &member) {
        return QVariant(member);
    });
    return memberList;
}

QList<Bucket> GroupListModel::getBuckets(const int index) const {
    const auto& buckets =  groupList.at(index).getBuckets();
    for (const auto&[sourceId, bucketName] : buckets) {
        qDebug() << "name: " << bucketName;
        qDebug() << "sourceId: " << sourceId;
    }
    return buckets;
}

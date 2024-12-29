//
// Created by cvrain on 24-12-29.
//

#include "group_model.hpp"

#include "src/http_clint.hpp"

GroupModel::GroupModel(QObject *parent): QAbstractListModel(parent) {
    qDebug() << "GroupModel::GroupModel";
    update();
}

int GroupModel::rowCount(const QModelIndex &parent) const {
    return static_cast<int>(membersId.length());
}

QVariant GroupModel::data(const QModelIndex &index, const int role) const {
    switch (role) {
        case IdRole:
            return GroupInfoManager::getInstance()->getGroupInfo().getId();
        case NameRole:
            return GroupInfoManager::getInstance()->getGroupInfo().getName();
        case MembersIdRole:
            return membersId.at(index.row());
        case MembersNameRole:
            return membersName.at(index.row());
        default:
            return {};
    }
}

QVariantMap GroupModel::get(const int index) const {
    if (index < 0 || index >= membersId.size()) {
        return {
            {"id", "00000000"},
            {"name", "unknown"},
            {"membersId", QStringList{}},
            {"membersName", QStringList{}}
        };
    }
    return {
        {"id", GroupInfoManager::getInstance()->getGroupInfo().getId()},
        {"name", GroupInfoManager::getInstance()->getGroupInfo().getName()},
        {"membersId", membersId.at(index)},
        {"membersName", membersName.at(index)}
    };
}

QHash<int, QByteArray> GroupModel::roleNames() const {
    return {
        {IdRole, "id"},
        {NameRole, "name"},
        {MembersIdRole, "membersId"},
        {MembersNameRole, "membersName"}
    };
}

QModelIndex GroupModel::index(const int row, const int column, const QModelIndex &parent) const {
    return createIndex(row, column);
}

void GroupModel::update() {
    beginResetModel();
    GroupInfoManager::getInstance()->update();
    const auto result = HttpClient::getInstance()->groupMemberNames(
        GroupInfoManager::getInstance()->getGroupInfo().getMembersId());

    if (not result.has_value()) {
        qDebug() << "GroupModel::update" << "no result";
        endResetModel();
        return;
    }

    const auto [fst, snd] = result.value();
    membersId = fst;
    membersName = snd;

    std::ranges::for_each(membersId, [this](const auto& id) {
        qDebug() << "GroupModel::update" << id;
    });

    std::ranges::for_each(membersName, [this](const auto& name) {
        qDebug() << "GroupModel::update" << name;
    });

    endResetModel();
}

QString GroupModel::getName() {
    const auto name =  GroupInfoManager::getInstance()->getGroupInfo().getName();
    qDebug() << "GroupModel::getName" << name;
    return name.isEmpty() ? "unknown" : name;
}

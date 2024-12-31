//
// Created by cvrain on 24-12-30.
//

#ifndef GROUP_LIST_MODEL_HPP
#define GROUP_LIST_MODEL_HPP

#include <QAbstractListModel>
#include <QList>

#include "src/group_info.hpp"

class GroupListModel : public QAbstractListModel {
    Q_OBJECT

public:
    enum GroupRole {
        IdRole = Qt::UserRole + 1,
        NameRole,
        OwnerRole
    };
    explicit GroupListModel(QObject *parent = nullptr);
    [[nodiscard]] Q_INVOKABLE QVariant data(const QModelIndex &index, int role) const override;
    [[nodiscard]] Q_INVOKABLE QHash<int, QByteArray> roleNames() const override;
    [[nodiscard]] Q_INVOKABLE int rowCount(const QModelIndex &parent) const override;
    [[nodiscard]] QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    [[nodiscard]] Q_INVOKABLE QVariantMap get(int index) const;
    [[nodiscard]] Q_INVOKABLE QVariantList getGroupMembers(int index) const;
    [[nodiscard]] Q_INVOKABLE QList<Bucket> getBuckets(int index) const;
    Q_INVOKABLE void update();
    Q_INVOKABLE bool isOwner(int index) const;
    Q_INVOKABLE bool isMember(int index) const;
private:
    QList<GroupInfo> groupList;
};


#endif //GROUP_LIST_MODEL_HPP

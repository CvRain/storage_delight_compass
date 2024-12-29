//
// Created by cvrain on 24-12-29.
//

#ifndef GROUP_MODEL_HPP
#define GROUP_MODEL_HPP

#include <QAbstractListModel>

#include "src/group_info.hpp"

class GroupModel : public QAbstractListModel {
    Q_OBJECT

public:
    enum GroupRoles {
        IdRole = Qt::UserRole + 1,
        NameRole,
        MembersIdRole,
        MembersNameRole
    };

    explicit GroupModel(QObject *parent = nullptr);

    [[nodiscard]] int rowCount(const QModelIndex &parent) const override;

    [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;

    [[nodiscard]] Q_INVOKABLE QVariantMap get(int index) const;

    [[nodiscard]] QHash<int, QByteArray> roleNames() const override;

    [[nodiscard]] QModelIndex index(int row, int column, const QModelIndex &parent) const override;

    Q_INVOKABLE void update();

    Q_INVOKABLE static QString getName();

private:
    QStringList membersId{};
    QStringList membersName{};
};


#endif //GROUP_MODEL_HPP

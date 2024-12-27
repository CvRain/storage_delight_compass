//
// Created by cvrain on 24-12-26.
//

#ifndef MEMBERS_MODEL_HPP
#define MEMBERS_MODEL_HPP

#include <QAbstractListModel>

#include "user_info.hpp"

class MembersModel : public QAbstractListModel {
    Q_OBJECT

public:
    enum Roles{
        NameRole = Qt::UserRole + 1,
        IdRole,
        CreateTimeRole
    };

    explicit MembersModel(QObject *parent = nullptr);

    [[nodiscard]] int rowCount(const QModelIndex &parent) const override;

    [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;

    [[nodiscard]] Q_INVOKABLE QVariantMap get(int index) const;

    [[nodiscard]] QHash<int, QByteArray> roleNames() const override;

    [[nodiscard]] QModelIndex index(int row, int column, const QModelIndex &parent) const override;

    Q_INVOKABLE void addMember(const QVariant &memberData);

    Q_INVOKABLE void removeMember(const QString &memberId);

    Q_INVOKABLE void update();

signals:
    void requestFailed(const QString &error);

    void requestFinish(int code, const QString &result, const QString &message);

    void memberUpdated();

private:
    QList<UserInfo> members{};
};


#endif //MEMBERS_MODEL_HPP

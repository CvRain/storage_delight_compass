//
// Created by cvrain on 24-12-31.
//

#ifndef OBJECT_MODEL_HPP
#define OBJECT_MODEL_HPP

#include <QObject>
#include <QAbstractListModel>
#include <QString>

class OneObject :public QObject {
    Q_OBJECT

public:
    explicit OneObject(QObject *parent = nullptr);
    OneObject(const OneObject &sourceObject);
    OneObject& operator=(const OneObject &sourceObject);

    QString etag{};
    QString lastModified{};
    QString name{};
    int size{};
    QString versionId{};
};

class ObjectModel : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY(QString bucketName READ getBucketName WRITE setBucketName)
    Q_PROPERTY(QString sourceId READ getSourceId WRITE setSourceId)
public:
    enum {
        EtagRole = Qt::UserRole + 1,
        LastModifiedRole,
        NameRole,
        SizeRole,
        VersionIdRole
    };

    explicit ObjectModel(QObject *parent = nullptr);
    [[nodiscard]] Q_INVOKABLE QVariant data(const QModelIndex &index, int role) const override;
    [[nodiscard]] Q_INVOKABLE QHash<int, QByteArray> roleNames() const override;
    [[nodiscard]] Q_INVOKABLE int rowCount(const QModelIndex &parent) const override;
    [[nodiscard]] Q_INVOKABLE QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    [[nodiscard]] Q_INVOKABLE QString getBucketName() const;
    [[nodiscard]] Q_INVOKABLE QString getSourceId() const;

    Q_INVOKABLE void setObjects(const QList<OneObject> &objects);
    Q_INVOKABLE void update();
    Q_INVOKABLE void setBucketName(const QString &bucketName);
    Q_INVOKABLE void setSourceId(const QString &sourceId);
private:
    QList<OneObject> objects;
    QString bucketName{};
    QString sourceId{};
};


#endif //OBJECT_MODEL_HPP

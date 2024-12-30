//
// Created by cvrain on 24-12-27.
//

#ifndef BUCKETS_HPP
#define BUCKETS_HPP

#include <QAbstractListModel>
#include <QString>
#include <QObject>

class Bucket {
public:
    QString sourceId;
    QString bucketName;
};

class BucketListModel : public QAbstractListModel {
    Q_OBJECT

public:
    enum Roles {
        SourceIdRole = Qt::UserRole + 1,
        BucketNameRole,
    };

    explicit BucketListModel(QObject *parent = nullptr) : QAbstractListModel(parent) {
    }

    explicit BucketListModel(const QList<Bucket> &buckets, QObject *parent = nullptr) : QAbstractListModel(parent),
        buckets(buckets) {
    }

    BucketListModel(const BucketListModel &sourceBuckets) {
        this->buckets = sourceBuckets.getBuckets();
    }

    BucketListModel &operator=(const BucketListModel &sourceBuckets) {
        this->buckets = sourceBuckets.getBuckets();
        return *this;
    }

    [[nodiscard]] Q_INVOKABLE QList<Bucket> getBuckets() const;

    [[nodiscard]] Q_INVOKABLE Bucket getBucket(int index) const;

    [[nodiscard]] Q_INVOKABLE QString getSourceId(int index) const;

    [[nodiscard]] Q_INVOKABLE QString getBucketName(int index) const;

    [[nodiscard]] QHash<int, QByteArray> roleNames() const override;

    [[nodiscard]] int rowCount(const QModelIndex &parent) const override;

    [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;

    Q_INVOKABLE void setBuckets(const QList<Bucket> &buckets);

private:
    QList<Bucket> buckets;
};


#endif //BUCKETS_HPP

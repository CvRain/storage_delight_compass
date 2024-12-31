//
// Created by cvrain on 24-12-30.
//

#include "buckets.hpp"

QList<Bucket> BucketListModel::getBuckets() const {
    return buckets;
}

Bucket BucketListModel::getBucket(const int index) const {
    return buckets.at(index);
}

QString BucketListModel::getSourceId(const int index) const {
    return buckets.at(index).sourceId;
}

QString BucketListModel::getBucketName(const int index) const {
    return buckets.at(index).bucketName;
}

void BucketListModel::setBuckets(const QList<Bucket> &buckets) {
    beginResetModel();
    this->buckets = buckets;
    endResetModel();
}

QHash<int, QByteArray> BucketListModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[SourceIdRole] = "sourceId";
    roles[BucketNameRole] = "bucketName";
    return roles;
}

int BucketListModel::rowCount(const QModelIndex &parent) const {
    return static_cast<int>(buckets.size());
}

QVariant BucketListModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid()) {
        return {};
    }
    switch (role) {
        case SourceIdRole:
            return buckets.at(index.row()).sourceId;
        case BucketNameRole:
            return buckets.at(index.row()).bucketName;
        default:
            return {};
    }
}

int BucketListModel::length() const
{
    return static_cast<int>(buckets.size());
}

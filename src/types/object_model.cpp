//
// Created by cvrain on 24-12-31.
//

#include "object_model.hpp"

#include "src/http_clint.hpp"

OneObject::OneObject(QObject *parent) : QObject(parent) {
}

OneObject::OneObject(const OneObject &sourceObject) {
    etag = sourceObject.etag;
    lastModified = sourceObject.lastModified;
    name = sourceObject.name;
    size = sourceObject.size;
    versionId = sourceObject.versionId;
}

OneObject& OneObject::operator=(const OneObject &sourceObject) {
    etag = sourceObject.etag;
    lastModified = sourceObject.lastModified;
    name = sourceObject.name;
    size = sourceObject.size;
    versionId = sourceObject.versionId;
    return *this;
}

ObjectModel::ObjectModel(QObject *parent): QAbstractListModel(parent) {
    update();
}

QVariant ObjectModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid()) {
        return {};
    }
    switch (role) {
        case EtagRole:
            return objects.at(index.row()).etag;
        case LastModifiedRole:
            return objects.at(index.row()).lastModified;
        case NameRole:
            return objects.at(index.row()).name;
        case SizeRole:
            return objects.at(index.row()).size;
        case VersionIdRole:
            return objects.at(index.row()).versionId;
        default:
            return {};
    }
}

QHash<int, QByteArray> ObjectModel::roleNames() const {
    return {
            {EtagRole, "etag"},
            {LastModifiedRole, "lastModified"},
            {NameRole, "name"},
            {SizeRole, "size"},
            {VersionIdRole, "versionId"}
    };
}

int ObjectModel::rowCount(const QModelIndex &parent) const {
    return static_cast<int>(objects.size());
}

QModelIndex ObjectModel::index(const int row, const int column, const QModelIndex &parent) const {
    if (parent.isValid()) {
        return {};
    }
    return createIndex(row, column);
}

QString ObjectModel::getBucketName() const {
    return bucketName;
}

QString ObjectModel::getSourceId() const {
    return sourceId;
}

void ObjectModel::setObjects(const QList<OneObject> &objects) {
    beginResetModel();
    this->objects = objects;
    endResetModel();
}

void ObjectModel::update() {
    const auto result = HttpClient::getInstance()->getObjects(bucketName.toStdString(), sourceId.toStdString());
    setObjects(result);
}

void ObjectModel::setBucketName(const QString &bucketName) {
    this->bucketName = bucketName;
}

void ObjectModel::setSourceId(const QString &sourceId) {
    this->sourceId = sourceId;
}

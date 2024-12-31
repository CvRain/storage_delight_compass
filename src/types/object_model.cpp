//
// Created by cvrain on 24-12-31.
//

#include "object_model.hpp"

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
}

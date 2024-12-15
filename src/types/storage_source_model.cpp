//
// Created by cvrain on 24-12-13.
//

#include "storage_source_model.hpp"

StorageSourceModel::StorageSourceModel(QObject *parent)
: QObject(parent), currentIndex(0) {
    storageName << "MinIO" << "Qt Documentation";
    storageUrl << "https://min.io/" << "https://doc.qt.io/" ;
}


QStringList StorageSourceModel::getItems() const {
    return storageName;
}

int StorageSourceModel::getCurrentIndex() const {
    return currentIndex;
}

QString StorageSourceModel::getUrl(const int index) const {
    if (index >= 0 && index < storageUrl.size()) {
        return storageUrl[index];
    }
    return {};
}

QString StorageSourceModel::getName(const int index) const {
    if (index >= 0 && index < storageName.size()) {
        return storageName[index];
    }
    return {};
}

void StorageSourceModel::setCurrentIndex(const int index) {
    currentIndex = index;
}

void StorageSourceModel::append(const QString &item) {
    storageUrl.push_back(item);
    emit itemsChanged();
}

void StorageSourceModel::clear() {
    storageUrl.clear();
    emit itemsChanged();
}

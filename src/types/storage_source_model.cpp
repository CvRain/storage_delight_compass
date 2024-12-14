//
// Created by cvrain on 24-12-13.
//

#include "storage_source_model.hpp"

StorageSourceModel::StorageSourceModel(QObject *parent)
:QObject(parent), currentIndex(0){
    storageUrl.push_back("https://min.io/");
}

QStringList StorageSourceModel::getItems() const {
    return storageUrl;
}

int StorageSourceModel::getCurrentIndex() const {
    return currentIndex;
}

void StorageSourceModel::setCurrentIndex(const int index) {
    currentIndex = index;
}

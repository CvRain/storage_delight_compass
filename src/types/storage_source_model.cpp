//
// Created by cvrain on 24-12-13.
//

#include "storage_source_model.hpp"

#include <QDebug>
#include "storage_source.hpp"
#include "src/http_clint.hpp"

StorageSourceModel::StorageSourceModel(QObject *parent)
: QObject(parent), currentIndex(0) {
    this->sources = HttpClient::getInstance()->storageSource();
}

QStringList StorageSourceModel::getItems() const {
    QStringList storageName;
    for (const auto& it : sources) {
        storageName << it.getName();
    }
    return storageName;
}

int StorageSourceModel::getCurrentIndex() const {
    return currentIndex;
}

QString StorageSourceModel::getUrl(const int index) const {
    return sources.at(index).getUrl();
}

QString StorageSourceModel::getName(const int index) const {
    return sources.at(index).getName();
}

void StorageSourceModel::update() {
    qDebug() << "StorageSourceModel::update";
    this->sources = HttpClient::getInstance()->storageSource();
    currentIndex = 0;
}

void StorageSourceModel::setCurrentIndex(const int index) {
    if (index < 0 || index >= sources.size()) {
        return;
    }
    currentIndex = index;
}
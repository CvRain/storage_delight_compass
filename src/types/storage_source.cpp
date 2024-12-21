//
// Created by cvrain on 24-12-19.
//

#include "storage_source.hpp"

StorageSource::StorageSource(QObject *parent): QObject(parent) {
}

StorageSource::StorageSource(const StorageSource &source)
    : id(source.id), ak(source.ak), sk(source.sk), url(source.url), name(source.name), isHttps(source.isHttps){
}

StorageSource& StorageSource::operator=(const StorageSource &source) {
    id = source.id;
    ak = source.ak;
    sk = source.sk;
    url = source.url;
    name = source.name;
    isHttps = source.isHttps;
    return *this;
}

StorageSourceInstance* StorageSourceInstance::getInstance() {
    static StorageSourceInstance instance;
    return &instance;
}

StorageSource StorageSourceInstance::getStorageSource(const int index) const {
    return storageSources.at(index);
}

QList<StorageSource> StorageSourceInstance::getStorageSources() const {
    return storageSources;
}

void StorageSourceInstance::addStorageSource(const StorageSource &source) {
    storageSources.push_back(source);
}

void StorageSourceInstance::removeStorageSource(const int index) {
    storageSources.removeAt(index);
}

QString StorageSourceInstance::getId(const int index) const {
    return storageSources.at(index).getId();
}

QString StorageSourceInstance::getName(const int index) const {
    return storageSources.at(index).getName();
}

QString StorageSourceInstance::getUrl(const int index) const {
    return storageSources.at(index).getUrl();
}

QString StorageSourceInstance::getAk(const int index) const {
    return storageSources.at(index).getAk();
}

QString StorageSourceInstance::getSk(const int index) const {
    return storageSources.at(index).getSk();
}

qsizetype StorageSourceInstance::getStorageSourceCount() const {
    return storageSources.length();
}

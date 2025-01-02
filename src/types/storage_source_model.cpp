//
// Created by cvrain on 24-12-13.
//

#include "storage_source_model.hpp"

#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>

#include "storage_source.hpp"
#include "src/http_clint.hpp"

StorageSourceModel::StorageSourceModel(QObject *parent)
    : QObject(parent), currentIndex(0) {
    this->sources = HttpClient::getInstance()->storageSource();
}

QStringList StorageSourceModel::getItems() const {
    QStringList storageName;
    for (const auto &it: sources) {
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

QString StorageSourceModel::getId(const int index) const {
    return sources.at(index).getId();
}

int StorageSourceModel::size() const {
    return static_cast<int>(sources.size());
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

void StorageSourceModel::remove(int index) {
    if (UserManager::getInstance()->getRole() != 0){
        emit requestFailed("You do not have permission to do this");
        return;
    }

    const auto storageId = sources.at(index).getId();
    if (const auto result = HttpClient::getInstance()->removeStorageSource(storageId.toStdString());
        result.code != 200) {
        qDebug() << "remove storage source failed";
        emit requestFailed(result.message.data());
        return;
    }
}

void StorageSourceModel::append(const QVariant &sourceData) {
    if (!sourceData.canConvert<QVariantMap>()) {
        qWarning() << "Invalid source data";
        emit requestFailed("Invalid source data");
        return;
    }
    try {
        StorageSource source;
        const auto data = sourceData.toMap();
        source.setName(data["name"].toString());
        source.setAk(data["ak"].toString());
        source.setSk(data["sk"].toString());
        source.setIsHttps(data["isHttps"].toBool());

        const auto urlStr = data["url"].toString();
        //check if urlStr not has http:// or https://, and http://
        if (!urlStr.startsWith("http://") && !urlStr.startsWith("https://")) {
            source.setUrl("http://" + urlStr);
        } else {
            source.setUrl(urlStr);
        }
        this->add(source);
    }catch (const std::exception &e) {
        emit requestFailed(e.what());
    }
}

void StorageSourceModel::add(const StorageSource &source) {
    if (UserManager::getInstance()->getRole() != 0){
        emit requestFailed("You do not have permission to do this");
        return;
    }

    if (const auto result = HttpClient::getInstance()->addStorageSource(source); result.code != 200) {
        qDebug() << "add storage source failed";
        emit requestFailed(result.message.data());
        return;
    }
    this->update();
    emit itemsChanged();
}

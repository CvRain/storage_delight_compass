//
// Created by cvrain on 24-12-13.
//

#ifndef STORAGE_SOURCE_MODEL_HPP
#define STORAGE_SOURCE_MODEL_HPP

#include <QObject>
#include <QStringList>
#include <QVariant>

#include "storage_source.hpp"

class StorageSourceModel: public QObject{
Q_OBJECT
Q_PROPERTY(QStringList items READ getItems NOTIFY itemsChanged)
Q_PROPERTY(int currentIdex READ getCurrentIndex NOTIFY currentIndexChanged)

public:
    explicit StorageSourceModel(QObject* parent = nullptr);
    [[nodiscard]] Q_INVOKABLE QStringList getItems() const;
    [[nodiscard]] Q_INVOKABLE int getCurrentIndex() const;
    [[nodiscard]] Q_INVOKABLE QString getUrl(int index) const;
    [[nodiscard]] Q_INVOKABLE QString getName(int index) const;
    [[nodiscard]] Q_INVOKABLE int size() const;
    Q_INVOKABLE void update();
    Q_INVOKABLE void setCurrentIndex(int index);
    Q_INVOKABLE void remove(int index);
    Q_INVOKABLE void append(const QVariant& sourceData);
    void add(const StorageSource& source);

signals:
    void itemsChanged();
    void currentIndexChanged();
    void requestFailed(const QString& error);

private:
    QList<StorageSource> sources{};
    int currentIndex;
};

#endif //STORAGE_SOURCE_MODEL_HPP

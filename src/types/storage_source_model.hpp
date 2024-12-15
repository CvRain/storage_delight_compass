//
// Created by cvrain on 24-12-13.
//

#ifndef STORAGE_SOURCE_MODEL_HPP
#define STORAGE_SOURCE_MODEL_HPP

#include <QObject>
#include <QStringList>

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
    void Q_INVOKABLE setCurrentIndex(int index);
    void Q_INVOKABLE append(const QString &item);
    void Q_INVOKABLE clear();

signals:
    void itemsChanged();
    void currentIndexChanged();

private:
    QStringList storageUrl;
    QStringList storageName;
    int currentIndex;
};



#endif //STORAGE_SOURCE_MODEL_HPP

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
    [[nodiscard]] QStringList getItems() const;
    [[nodiscard]] int getCurrentIndex() const;
    void setCurrentIndex(int index);

signals:
    void itemsChanged();
    void currentIndexChanged();

private:
    QStringList storageUrl;
    int currentIndex;
};



#endif //STORAGE_SOURCE_MODEL_HPP

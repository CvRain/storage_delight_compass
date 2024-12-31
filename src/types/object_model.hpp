//
// Created by cvrain on 24-12-31.
//

#ifndef OBJECT_MODEL_HPP
#define OBJECT_MODEL_HPP

#include <QObject>
#include <QAbstractListModel>
#include <QString>

class OneObject : QObject {
    Q_OBJECT

public:
    explicit OneObject(QObject *parent = nullptr);
    OneObject(const OneObject &sourceObject);
    OneObject& operator=(const OneObject &sourceObject);

    QString etag{};
    QString lastModified{};
    QString name{};
    int size{};
    QString versionId{};
};

class ObjectModel : QAbstractListModel {
    Q_OBJECT
public:
    explicit ObjectModel(QObject *parent = nullptr);

};


#endif //OBJECT_MODEL_HPP

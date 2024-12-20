//
// Created by cvrain on 24-12-19.
//

#ifndef STORAGE_SOURCE_HPP
#define STORAGE_SOURCE_HPP

#include <QObject>
#include <QList>

class StorageSource: public QObject{
    Q_OBJECT
public:
    explicit StorageSource(QObject *parent = nullptr);

    StorageSource(const StorageSource &source);

    StorageSource& operator=(const StorageSource &source);

    [[nodiscard]] QString getId() const { return id; }

    [[nodiscard]] QString getAk() const { return ak; }

    [[nodiscard]] QString getSk() const { return sk; }

    [[nodiscard]] QString getUrl() const { return url; }

    [[nodiscard]] QString getName() const { return name; }

    void setId(const QString &id) { this->id = id; }

    void setAk(const QString &ak) { this->ak = ak; }

    void setSk(const QString &sk) { this->sk = sk; }

    void setUrl(const QString &url) { this->url = url; }

    void setName(const QString &name) { this->name = name; }

private:
    QString id;
    QString ak;
    QString sk;
    QString url;
    QString name;
};

class StorageSourceInstance: public QObject {
    Q_OBJECT
public:
    static StorageSourceInstance *getInstance();

    void addStorageSource(const StorageSource &source);
    void removeStorageSource(int index);
    [[nodiscard]] StorageSource getStorageSource(int index) const;
    [[nodiscard]] QList<StorageSource> getStorageSources() const;
    [[nodiscard]] Q_INVOKABLE QString getId(int index) const;
    [[nodiscard]] Q_INVOKABLE QString getName(int index) const;
    [[nodiscard]] Q_INVOKABLE QString getUrl(int index) const;
    [[nodiscard]] Q_INVOKABLE QString getAk(int index) const;
    [[nodiscard]] Q_INVOKABLE QString getSk(int index) const;
    [[nodiscard]] Q_INVOKABLE qsizetype getStorageSourceCount() const;

private:
    explicit StorageSourceInstance(QObject *parent = nullptr) : QObject(parent) {
    }

    QList<StorageSource> storageSources{};
};


#endif //STORAGE_SOURCE_HPP

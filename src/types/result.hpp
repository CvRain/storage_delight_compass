//
// Created by cvrain on 24-12-8.
//

#ifndef RESULT_HPP
#define RESULT_HPP

#include <QObject>

class BaseResult : public QObject {
    Q_OBJECT
    Q_PROPERTY(int code READ getCode NOTIFY resultChanged)
    Q_PROPERTY(QString result READ getResult NOTIFY resultChanged)
    Q_PROPERTY(QString message READ getMessage NOTIFY resultChanged)

public:
    explicit BaseResult(QObject *parent = nullptr) : QObject(parent) {
    }

    int getCode() const {
        return code;
    }

    QString getResult() const {
        return result;
    }

    QString getMessage() const {
        return message;
    }

    void setResult(const int code, const QString &result, const QString &message) {
        this->code = code;
        this->result = result;
        this->message = message;
        emit resultChanged();
    }

signals:
    void resultChanged();

private:
    int code{};
    QString result{};
    QString message{};
};

class ResponseResult final : public BaseResult {
};

#endif //RESULT_HPP

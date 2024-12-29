//
// Created by cvrain on 24-12-30.
//

#ifndef UTILS_HPP
#define UTILS_HPP

#include <QGuiApplication>
#include <QClipboard>

class Utils: public QObject {
    Q_OBJECT
public:
    Q_INVOKABLE static void copyToClipboard(const QString &text) {
        QGuiApplication::clipboard()->setText(text);
    }
};

#endif //UTILS_HPP

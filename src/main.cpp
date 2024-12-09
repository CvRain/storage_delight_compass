#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "types/result.hpp"
#include "http_clint.hpp"
#include "user_info.hpp"

int main(int argc, char *argv[]) {
    const QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    qmlRegisterSingletonInstance<HttpClint>("Storage.Service", 1, 0, "HttpClient", HttpClint::getInstance());
    qmlRegisterSingletonInstance<UserInfo>("Storage.User", 1, 0, "UserInfo", UserInfo::getInstance());
    qmlRegisterType<BaseResult>("Storage.Model", 1, 0, "BaseResult");

    engine.load("qrc:/qt/qml/storage_delight_compass/DelightUI/App.qml");

    return QGuiApplication::exec();
}

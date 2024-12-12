#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtWebEngineQuick/qtwebenginequickglobal.h>

#include "types/result.hpp"
#include "http_clint.hpp"
#include "user_info.hpp"

int main(int argc, char *argv[]) {
    const QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
    QtWebEngineQuick::initialize();
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    qmlRegisterSingletonInstance<HttpClient>("Storage.Service", 1, 0, "HttpClient", HttpClient::getInstance());
    qmlRegisterSingletonInstance<UserManager>("Storage.Service", 1, 0, "UserManager", UserManager::getInstance());

    qmlRegisterType<UserInfo>("Storage.Model", 1, 0, "UserInfo");
    qmlRegisterType<BaseResult>("Storage.Model", 1, 0, "BaseResult");

    engine.load("qrc:/qt/qml/storage_delight_compass/DelightUI/App.qml");

    return QGuiApplication::exec();
}

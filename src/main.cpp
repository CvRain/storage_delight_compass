#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtWebEngineQuick/qtwebenginequickglobal.h>

#include "group_info.hpp"
#include "types/result.hpp"
#include "types/storage_source_model.hpp"
#include "http_clint.hpp"
#include "members_model.hpp"
#include "user_info.hpp"
#include "utils.hpp"
#include "types/group_list_model.hpp"
#include "types/group_model.hpp"
#include "types/storage_source.hpp"

int main(int argc, char *argv[]) {

    QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
    QtWebEngineQuick::initialize();

    const QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    qmlRegisterSingletonInstance<HttpClient>("Storage.Service", 1, 0, "HttpClient", HttpClient::getInstance());
    qmlRegisterSingletonInstance<UserManager>("Storage.Service", 1, 0, "UserManager", UserManager::getInstance());
    qmlRegisterSingletonInstance<StorageSourceInstance>("Storage.Service", 1, 0, "StorageInstance", StorageSourceInstance::getInstance());
    qmlRegisterSingletonInstance<GroupInfoManager> ("Storage.Service", 1, 0, "GroupInfoManager", GroupInfoManager::getInstance());

    qmlRegisterType<UserInfo>("Storage.Model", 1, 0, "UserInfo");
    qmlRegisterType<BaseResult>("Storage.Model", 1, 0, "BaseResult");
    qmlRegisterType<StorageSourceModel>("Storage.Model", 1, 0, "SourceList");
    qmlRegisterType<StorageSource>("Storage.Model", 1, 0, "StorageSource");
    qmlRegisterType<MembersModel>("Storage.Model", 1, 0, "MembersModel");
    qmlRegisterType<GroupInfo>("Storage.Model", 1, 0, "GroupInfo");
    qmlRegisterType<GroupModel>("Storage.Model", 1, 0, "GroupModel");
    qmlRegisterType<GroupListModel>("Storage.Model", 1, 0, "GroupListModel");
    qmlRegisterType<Utils>("Storage.Model", 1, 0, "Utils");
    qmlRegisterType<Bucket>("Storage.Model", 1, 0, "Bucket");

    engine.load("qrc:/qt/qml/storage_delight_compass/DelightUI/App.qml");

    return QGuiApplication::exec();
}

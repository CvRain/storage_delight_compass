cmake_minimum_required(VERSION 3.16)

project(storage_delight_compass VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Quick)

qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(appstorage_delight_compass
        src/main.cpp
)

set(PAGE_QML_PATH DelightUI/Pages)
set(COMPONENT_QML_PATH DelightUI/Components)

set(QML_PAGES ${PAGE_QML_PATH}/Login.qml
        ${PAGE_QML_PATH}/Register.qml
        ${PAGE_QML_PATH}/Welcome.qml
        ${PAGE_QML_PATH}/Terminal.qml)

set(QML_COMPONENTS
    ${COMPONENT_QML_PATH}/Card.qml
    ${COMPONENT_QML_PATH}/InputBar.qml
    ${COMPONENT_QML_PATH}/BaseButton.qml)

set(QML_RESOURCES DelightUI/Components/qmldir DelightUI/Pages/qmldir DelightUI/qmldir)

qt_add_qml_module(appstorage_delight_compass
        URI storage_delight_compass
        VERSION 1.0
        QML_FILES
        DelightUI/App.qml ${QML_PAGES} ${QML_COMPONENTS}
        RESOURCES ${QML_RESOURCES}
        RESOURCES resources.qrc
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appstorage_delight_compass PROPERTIES
        #    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appstorage_delight_compass
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE TRUE
        WIN32_EXECUTABLE TRUE
)

target_link_libraries(appstorage_delight_compass
        PRIVATE Qt6::Quick
)

include(GNUInstallDirs)
install(TARGETS appstorage_delight_compass
        BUNDLE DESTINATION .
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

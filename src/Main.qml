import QtQuick
import QtQuick.Window
import QtQuick.Controls
import "./Qml/Pages"

Window {
    id: mainWindow
    width: 1920
    height: 1080
    visible: true
    title: qsTr("Hello World")

    StackView{
        id: stackView
        anchors.fill: parent
        initialItem: welcomePage
    }

    Component{
        id: welcomePage
        Loader{
            id: welcomePageLoader
            source:"qrc:/qt/qml/storage_delight_compass/src/Qml/Pages/Welcome.qml"
            Connections{
                target: welcomePageLoader.item
                function onPageChange(){
                    stackView.replace(terminalPage)
                    console.debug(stackView.depth)
                }
            }
        }
    }


    Component{
        id: terminalPage
        Loader{
            id: terminalPageLoader
            source:"qrc:/qt/qml/storage_delight_compass/src/Qml/Pages/Terminal.qml"
            Connections{
                target: terminalPageLoader.item
                function onPageChange(){
                    stackView.replace(welcomePage)
                    console.debug(stackView.depth)
                }
            }
        }
    }
}

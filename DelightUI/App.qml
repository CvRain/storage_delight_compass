import QtQuick
import QtQuick.Window
import QtQuick.Controls
import "./Pages"

pragma ComponentBehavior: Bound

ApplicationWindow {
    id: mainWindow
    width: 1366
    height: 768
    visible: true
    title: qsTr("storage delight compass")

    Component.onCompleted:{
        console.log(mainWindow.width)
    }

    StackView{
        id: stackView
        anchors.fill: parent
        initialItem: welcomePage
    }

    Component{
        id: welcomePage
        Welcome{
            id: welcomePageLoader
            width: mainWindow.width
            height: mainWindow.height
            anchors.fill: parent
            onPageChange:{
                stackView.replace(terminalPage)
            }
        }
    }

    Component{
        id: terminalPage
        Terminal{
            id: terminalPageLoader
            width: mainWindow.width
            height: mainWindow.height
            anchors.fill: parent

            Connections{
                target: terminalPageLoader
                function onPageChange(){
                    stackView.replace(welcomePage)
                }
            }
        }
    }
}

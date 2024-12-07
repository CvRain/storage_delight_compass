import QtQuick
import QtQuick.Window
import QtQuick.Controls

pragma ComponentBehavior: Bound

ApplicationWindow {
    id: mainWindow
    width: 1920
    height: 1080
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
        Loader{
            id: welcomePageLoader
            source:"./Pages/Welcome.qml"
            Connections{
                target: welcomePageLoader.item
                function onPageChange(){
                    stackView.replace(terminalPage)
                }
            }
        }
    }


    Component{
        id: terminalPage
        Loader{
            id: terminalPageLoader
            source:"./Pages/Terminal.qml"
            Connections{
                target: terminalPageLoader.item
                function onPageChange(){
                    stackView.replace(welcomePage)
                }
            }
        }
    }
}

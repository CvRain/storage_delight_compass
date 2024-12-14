import QtQuick
import QtQuick.Controls
import "../Components"

Rectangle {
    id: root
    width: 1920
    height: 1080
    color: "#e6e9ef"

    signal pageChange

    TopBar {
        id: topbar
        width: parent.width
        height: 100

        onUserLogout: {
            root.pageChange()
        }
    }

    Rectangle {
        id: sideBar
        width: 245
        height: parent.height - topbar.height
        color: "#e6e9ef"
        anchors.top: topbar.bottom
        anchors.left: parent.left

        Column {
            anchors.fill: sidebarBackground
            spacing: 0

            // Add multiple SideButtons here
            SideButton {
                text: qsTr("Object browser")
                width: sideBar.width
                icon: "qrc:/res/icon/save-line.svg"
            }
            SideButton {
                text: qsTr("Groups")
                width: sideBar.width
                icon: "qrc:/res/icon/group-line.svg"
            }
            SideButton{
                text: qsTr("Members")
                width: sideBar.width
                icon:"qrc:/res/icon/user-line.svg"
            }
            SideButton{
                text:qsTr("Personal info")
                width: sideBar.width
                icon:"qrc:/res/icon/settings-line.svg"
            }
            SideButton{
                text: qsTr("Console")
                width: sideBar.width
                icon: "qrc:/res/icon/terminal-line.svg"
            }
        }
    }

    StackView{
        id: stackView
        width: parent.width - sideBar.width
        height: parent.height - topbar.height

        anchors.top: topbar.bottom
        anchors.left: sideBar.right

        initialItem: consolePage
    }

    Component{
        id: consolePage
        Console{
            id: consolePageLoader
        }
    }

}

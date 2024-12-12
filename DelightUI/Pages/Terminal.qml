import QtQuick
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
        width: 380
        height: parent.height - topbar.height
        color: "#414559"
        anchors.top: topbar.bottom
        anchors.left: parent.left

        Column {
            anchors.fill: sidebarBackground
            spacing: 12

            // Add multiple SideButtons here
            SideButton {
                text: qsTr("Object browser")
                width: sideBar.width
            }
            SideButton {
                text: qsTr("Groups")
                width: sideBar.width
            }
            SideButton{
                text: qsTr("Members")
                width: sideBar.width
            }
            SideButton {
                text: qsTr("Sources terminal")
                width: sideBar.width
            }
            SideButton{
                text:qsTr("Personal info")
                width: sideBar.width
            }
        }
    }
}

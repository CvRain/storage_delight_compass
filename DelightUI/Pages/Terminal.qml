import QtQuick
import QtQuick.Controls
import Storage.Service
import Storage.Model
import "../Components"

Rectangle {
    id: root
    width: 1920
    height: 1080
    color: "#e6e9ef"

    signal pageChange

    Component.onCompleted: {
        HttpClient.userInfo();
    }

    TopBar {
        id: topbar
        width: parent.width
        height: 100
    }

    Alert{
        id: alert
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 35
        level: "warn"
        text: "test alert"
    }

    Rectangle {
        id: sideBar
        width: 245
        height: parent.height - topbar.height
        color: "#e6e9ef"
        anchors.top: topbar.bottom
        anchors.left: parent.left

        Column {
            id: column
            width: parent.width
            height: parent.height - userName - logout
            anchors.fill: sidebarBackground
            spacing: 0

            // Add multiple SideButtons here
            SideButton {
                text: qsTr("Object browser")
                width: sideBar.width
                icon: "qrc:/res/icon/save-line.svg"

                onClicked: {
                    stackView.replace(objectBrowerPage)
                }
            }
            SideButton {
                text: qsTr("Groups")
                width: sideBar.width
                icon: "qrc:/res/icon/group-line.svg"
                onClicked: {
                    stackView.replace(groupsPage)
                }
            }
            SideButton{
                text: qsTr("Members")
                width: sideBar.width
                icon:"qrc:/res/icon/user-line.svg"
                onClicked: {
                    stackView.replace(membersPage)
                }
            }
            SideButton{
                text:qsTr("Personal info")
                width: sideBar.width
                icon:"qrc:/res/icon/settings-line.svg"
                onClicked: {
                    stackView.replace(personalPage)
                }
            }
            SideButton{
                text: qsTr("Console")
                width: sideBar.width
                icon: "qrc:/res/icon/terminal-line.svg"
                onClicked: {
                    stackView.replace(consolePage)
                }
            }
        }

        Text {
            id: userName
            text: "unknown"
            width: parent.width - 5
            height: 25
            font.pixelSize: 16
            horizontalAlignment: Text.AlignLeft

            anchors.left: parent.left
            anchors.leftMargin: 5
            anchors.bottom: logout.top
            anchors.bottomMargin: 5

            Component.onCompleted: {
                alert.level = "info"
                alert.text = "Welcome " + UserManager.getName() + "id: " + UserManager.getId()
                alert.show()
                userName.text = UserManager.getName()
            }
        }
        Text{
            id: logout
            text: qsTr("logout")
            width: parent.width - 5
            height: 25
            horizontalAlignment: Text.AlignLeft

            anchors.left: parent.left
            anchors.leftMargin: 5
            anchors.bottom: parent.bottom

            MouseArea{
                anchors.fill: parent
                onClicked: {
                    root.pageChange()
                }
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
            alertInstance: alert
        }
    }

    Component{
        id: objectBrowerPage
        ObjectBrowser{
            id: objectBrowserPageLoader
        }
    }

    Component{
        id: groupsPage
        Groups{
            id: groupPageLoader
        }
    }

    Component{
        id: membersPage
        Members{
            id: memberPageLoader
        }
    }

    Component{
        id: personalPage
        Personal{
            id: personalPageLoader
        }
    }

}

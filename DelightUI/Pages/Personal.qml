import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Storage.Model
import Storage.Service
import "../Components"

Rectangle {
    signal logoutInvoked()

    id: root
    width: parent.width
    height: parent.height
    color: "white"

    Text{
        id: personalInfoText
        text: qsTr("Personal Info")
        color: "black"
        font.pixelSize: 22
        font.family: "Ubuntu Mono"

        anchors.left: topLine.left
        anchors.top: parent.top
        anchors.topMargin: 15
    }

    BaseButton{
        id: logoutButton
        height: 50
        width: 105
        text: qsTr("Logout")
        textColor: "white"
        color: "#dc8a78"
        font.pixelSize: 12

        anchors.right: topLine.right
        anchors.top: parent.top
        anchors.topMargin: 15
        anchors.verticalCenter: personalInfoText.verticalCenter

        onClicked: {
            root.logoutInvoked()
        }
    }

    Line{
        id: topLine
        color: "#8c8fa1"
        width: parent.width * 0.8

        anchors.top: personalInfoText.bottom
        anchors.topMargin: 15
        anchors.horizontalCenter: parent.horizontalCenter
    }

    Card{
        id: userNameCard
        width: parent.width * 0.5
        height: parent.width * 0.6
        border.color: "transparent"

        anchors.top: topLine.bottom
        anchors.topMargin: 35
        anchors.horizontalCenter: topLine.horizontalCenter

        GridLayout{
            id: cardGroup
            columns: 2
            columnSpacing: 8
            rowSpacing: 5

            width: parent.width * 0.8
            height: parent.height

            anchors.centerIn: parent

            Text{
                width: parent.width / 2 - 3
                text: qsTr("Name")
                font.pixelSize: 16
                font.bold: true
            }
            Text{
                id: nameText
                width: parent.width / 2 - 3
                text: qsTr("unknown")
                font.pixelSize: 16
            }
            Text{
                width: parent.width / 2 - 3
                text: qsTr("User id")
                font.pixelSize: 16
                font.bold: true
            }
            Text{
                id: idText
                width: parent.width / 2 - 3
                text: "0000000000000000"
                font.pixelSize: 16
            }
            Text{
                width: parent.width / 2 - 3
                text: qsTr("Group id")
                font.pixelSize: 16
                font.bold: true
            }
            Text{
                id: groupText
                width: parent.width / 2 - 3
                text: qsTr("Id")
                font.pixelSize: 16
            }
            Text{
                width: parent.width / 2 - 3
                text: qsTr("Role")
                font.pixelSize: 16
                font.bold: true
            }
            Text{
                id: roleText
                width: parent.width / 2 - 3
                text: qsTr("Id")
                font.pixelSize: 16
            }
            Text{
                width: parent.width / 2 - 3
                text: qsTr("Create time")
                font.pixelSize: 16
                font.bold: true
            }
            Text{
                id: createTimeText
                width: parent.width / 2 - 3
                text: qsTr("Id")
                font.pixelSize: 16
            }

            Component.onCompleted: {
                nameText.text = UserManager.getName()
                idText.text = UserManager.getId()
                groupText.text = UserManager.getGroupId()
                roleText.text = UserManager.getRole() === 0 ? "Admin" : "Common user"
                createTimeText.text = UserManager.getCreateDate()
            }
        }
    }

}

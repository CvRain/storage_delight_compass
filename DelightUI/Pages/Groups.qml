import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtWebEngine
import Storage.Model
import Storage.Service
import "../Components"

Rectangle {
    property Alert alertInstance

    id: root

    Row {
        id: topBarLeft
        width: parent.width / 2
        height: 50
        spacing: 15

        anchors.top: parent.top
        anchors.topMargin: 15
        anchors.left: topLine.left

        Text {
            id: membersInfoText
            color: "black"
            font.pixelSize: 22
            font.family: "Ubuntu Mono"
            text: groupListView.model.getName()

            anchors.verticalCenter: parent.verticalCenter
        }
    }

    Row {
        id: topBarRight
        width: root.width / 2
        height: 50
        spacing: 5

        anchors.top: topBarLeft.top
        anchors.right: topLine.right

        BaseButton {
            id: updateButton
            height: 30
            width: 120
            text: qsTr("update")
            textColor: "white"
            color: "#dc8a78"
            font.pixelSize: 18

            anchors.verticalCenter: parent.verticalCenter

            onClicked: {
                memberUpdate()
            }
        }

        BaseButton {
            id: addOneButton
            height: 30
            width: 120
            text: qsTr("add member")
            textColor: "white"
            color: "#dc8a78"
            font.pixelSize: 18

            anchors.verticalCenter: parent.verticalCenter

            onClicked: {
                memberAdd()
            }
        }

        BaseButton {
            id: removeButton
            height: 30
            width: 120
            text: qsTr("remove member")
            textColor: "white"
            color: "#dc8a78"
            font.pixelSize: 18

            anchors.verticalCenter: parent.verticalCenter

            onClicked: {
                memberRemove()
            }
        }
    }

    Line {
        id: topLine
        color: "#8c8fa1"
        width: parent.width * 0.8

        anchors.top: topBarLeft.bottom
        anchors.topMargin: 15
        anchors.horizontalCenter: parent.horizontalCenter
    }

    Component {
        id: groupInfoDelegate
        Item {
            id: wrapper
            width: root.width * 0.8
            height: 45

            MouseArea {
                anchors.fill: parent
                onClicked: wrapper.ListView.view.currentIndex = index
            }

            Rectangle {
                border.color: "#f2d5cf"
                border.width: 2
                radius: 15

                anchors.fill: parent
                color: wrapper.ListView.isCurrentItem ? "#f2d5cf" : "transparent"

                Text {
                    id: accountName
                    text: membersName
                    color: wrapper.ListView.isCurrentItem ? "white" : "black"
                    font.pixelSize: 16
                    font.bold: true
                    verticalAlignment: Text.AlignVCenter
                    font.family: "Ubuntu Mono"

                    anchors.left: parent.left
                    anchors.leftMargin: 10
                    anchors.verticalCenter: parent.verticalCenter
                }

                Text {
                    id: accountId
                    text: membersId
                    color: wrapper.ListView.isCurrentItem ? "white" : "black"
                    font.pixelSize: 16
                    font.bold: true
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignRight
                    font.family: "Ubuntu Mono"

                    anchors.right: parent.right
                    anchors.rightMargin: 10
                    anchors.leftMargin: 10
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
        }
    }

    GroupModel{
        id: groupModel
    }

    ListView {
        id: groupListView
        width: topLine.width
        height: parent.height - topBarLeft.height - 20
        spacing: 5

        delegate: groupInfoDelegate
        model: groupModel

        anchors.top: topLine.bottom
        anchors.topMargin: 15
        anchors.left: topLine.left

        onCurrentIndexChanged: {
            console.debug("current select group member: ", groupListView.model.getId(currentIndex))
        }
    }

    Component {
        id: groupRemoveComponent
        Dialog {
            id: groupAddDialog
            width: 300
            height: 150
            title: qsTr("Remove member")
            standardButtons: Dialog.Ok | Dialog.Cancel
            modal: true
            anchors.centerIn: parent

            background: Rectangle {
                width: groupRemoveComponent.width
                height: groupRemoveComponent.height
                radius: 5
            }

            Text {
                id: tips
                text: qsTr("Are you sure to remove")
                font.bold: true
                font.pixelSize: 16
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                anchors.centerIn: parent
            }

            Text {
                text: groupListView.model.getId(groupListView.currentIndex)
                font.bold: true
                font.pixelSize: 15
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                anchors.top: tips.bottom
                anchors.topMargin: 10
                anchors.horizontalCenter: tips.horizontalCenter
            }

            onAccepted: {
                let removeId = groupListView.model.getId(groupListView.currentIndex)
                console.debug("try to remove",removeId)
                groupListView.model.removeMember(removeId)
                memberUpdate()
            }
        }
    }

    Component {
        id: groupAddComponent
        Dialog {
            id: memberAddDialog

            width: 400
            height: 150
            title: qsTr("Add member")
            standardButtons: Dialog.Ok | Dialog.Cancel
            modal: true

            background: Rectangle {
                width: memberAddDialog.width
                height: memberAddDialog.height
                color: "white"
                radius: 5
            }

            ColumnLayout{
                anchors.fill: parent
                Text {
                    id: memberAddTip
                    height: 35
                    width: parent.width * 0.9
                    text: qsTr("Input member id")
                    font.bold: true
                    font.pixelSize: 16
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                InputBar{
                    id: memberAddInputBar
                    height: 35
                    width: parent.width * 0.9
                }
            }

            onAccepted: {
                groupListView.model.addMember(memberAddInputBar.text);
                memberUpdate()
            }

            anchors.centerIn: parent
        }
    }

    Component.onCompleted: {
        memberUpdate()
    }

    function memberUpdate() {
        groupListView.model.update()
    }

    function memberRemove() {
        var dialog = groupRemoveComponent.createObject(root)
        dialog.open()
    }

    function memberAdd() {
        var dialog = groupAddComponent.createObject(root)
        dialog.open()
    }
}

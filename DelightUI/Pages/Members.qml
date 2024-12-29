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
            text: qsTr("Members")
            color: "black"
            font.pixelSize: 22
            font.family: "Ubuntu Mono"

            anchors.verticalCenter: parent.verticalCenter
        }

        TextInput {
            id: memberIdText
            font.pixelSize: 18
            font.family: "Ubuntu Mono"
            readOnly: true
            width: parent.width

            MouseArea{
                anchors.fill: parent
                onClicked: {
                    utils.copyToClipboard(memberIdText.text)
                    alertInstance.text = "copy success"
                    alertInstance.level = "success"
                    alertInstance.show()
                }

            }

            anchors.verticalCenter: parent.verticalCenter
        }
    }

    Row {
        id: topBarRight
        width: root.width / 2
        height: 50
        spacing: 5

        anchors.top: parent.top
        anchors.topMargin: 15
        anchors.right: topLine.right

        BaseButton {
            id: updateButton
            height: 30
            width: 105
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
            width: 105
            text: qsTr("add")
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
            width: 105
            text: qsTr("remove")
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
        id: memberInfoDelegate
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
                    text: name
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
                    text: id
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

    ListView {
        id: memberListView
        width: topLine.width
        height: parent.height - topBarLeft.height - 20
        spacing: 5

        delegate: memberInfoDelegate
        model: MembersModel {
            id: memberModel

            onMemberUpdated: {
                members = model.members
            }
        }

        onCurrentIndexChanged: {
            if (currentIndex !== -1) {
                let id = model.get(currentIndex).id
                memberIdText.text = id
            } else {
                memberIdText.text = ""
            }
        }

        anchors.top: topLine.bottom
        anchors.topMargin: 15
        anchors.left: topLine.left
    }

    Component {
        id: memberRemoveComponent
        Dialog {
            id: memberRemoveDialog
            width: 300
            height: 150
            title: qsTr("Remove member")
            standardButtons: Dialog.Ok | Dialog.Cancel
            modal: true

            background: Rectangle {
                width: memberRemoveDialog.width
                height: memberRemoveDialog.height
                radius: 5
            }

            anchors.centerIn: parent

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
                text: memberListView.model.get(memberListView.currentIndex).name
                font.bold: true
                font.pixelSize: 15
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                anchors.top: tips.bottom
                anchors.topMargin: 10
                anchors.horizontalCenter: tips.horizontalCenter
            }

            onAccepted: {
                let removeId = memberListView.model.get(
                        memberListView.currentIndex).id
                console.debug("try to remove: ", removeId)
                memberListView.model.removeMember(removeId)
            }
        }
    }

    Component {
        id: memberAddComponent
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

            GridLayout {
                columns: 2
                columnSpacing: 5
                rowSpacing: 10

                Text {
                    id: userNameText
                    height: 35
                    text: qsTr("user name")
                }
                InputBar {
                    id: userNameInput
                    width: parent.width / 2
                    height: userNameText.height
                }
                Text {
                    id: userPasswordText
                    height: 35
                    text: qsTr("user password")
                }
                InputBar {
                    id: userPasswordInput
                    width: parent.width / 2
                    height: userPasswordText.height
                    echoMode: TextInput.Password
                }

                anchors.fill: parent
            }

            onAccepted: {
                var requestBody = {
                    "name": userNameInput.text,
                    "password": userPasswordInput.text
                }
                memberListView.model.addMember(requestBody)
            }

            anchors.centerIn: parent
        }
    }

    function memberUpdate() {
        alertInstance.text = "update"
        alertInstance.level = "info"
        alertInstance.show()
        memberListView.model.update()
    }

    function memberRemove() {
        if (UserManager.getRole() !== 0) {
            alertInstance.text = qsTr("No permission")
            alertInstance.level = "error"
            alertInstance.show()
            return
        }

        if(memberListView.model.get(memberListView.currentIndex) === 0){
            alertInstance.text = qsTr("Can not remove admin account")
            alertInstance.level = "warn"
            alertInstance.show()
            return
        }

        var dialog = memberRemoveComponent.createObject(root)
        dialog.open()
    }

    function memberAdd() {
        if (UserManager.getRole() !== 0) {
            alertInstance.text = qsTr("No permission")
            alertInstance.level = "error"
            alertInstance.show()
            return
        }

        var dialog = memberAddComponent.createObject(root)
        dialog.open()
    }

    Utils{
        id: utils
    }
}

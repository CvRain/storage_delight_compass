import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtWebEngine
import Storage.Model
import Storage.Service
import "../Components"

Rectangle {
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

        Text {
            id: memberIdText
            font.pixelSize: 18
            font.family: "Ubuntu Mono"

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
                if (memberListView.currentIndex !== -1) {
                    // 检查是否有选中项
                    let selectedName = memberListView.model.get(
                            memberListView.currentIndex).name
                    console.log("Update Button Clicked. Selected Name:",
                                selectedName)
                    // 在这里添加你的更新逻辑，例如发送信号、调用函数等
                } else {
                    console.log("No item selected.") // 如果没有选中项，则输出提示信息
                }
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
            }
        }
    }

    ListView {
        id: memberListView
        width: topLine.width
        height: parent.height - topBarLeft.height - 20
        spacing: 5

        delegate: memberInfoDelegate
        model: ListModel {
            ListElement {
                name: "cvrain"
            }
            ListElement {
                name: "test"
            }
            ListElement {
                name: "aaa"
            }
        }

        onCurrentIndexChanged: {
            if (currentIndex !== -1) {
                memberIdText.text = model.get(currentIndex).name
            } else {
                memberIdText.text = ""
            }
        }

        anchors.top: topLine.bottom
        anchors.topMargin: 15
        anchors.left: topLine.left
    }
}

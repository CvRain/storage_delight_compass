import QtQuick
import QtQuick.Controls
import QtWebEngine
import Storage.Model
import Storage.Service
import "../Components"

Rectangle {
    property Alert alertInstance
    property int selectGroup

    id: root

    Row {
        id: topBarLeft
        width: parent.width / 2
        height: 50
        spacing: 15

        anchors.top: parent.top
        anchors.topMargin: 15
        anchors.left: topLine.left

        BaseButton {
            id: returnSuperiorButton
            height: 30
            width: 120
            text: qsTr("Return")
            textColor: "white"
            color: "#dc8a78"
            font.pixelSize: 18

            anchors.verticalCenter: parent.verticalCenter

            onClicked: {
                if (stackView.depth !== 1) {
                    stackView.pop()
                }
            }
        }

        Text {
            id: membersInfoText
            text: qsTr("Object ")
            color: "black"
            font.pixelSize: 22
            font.family: "Ubuntu Mono"

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

    GroupListModel {
        id: groupListModel
    }

    Component {
        id: firstPage

        Item{
            ListView {
                id: firstPageListView
                width: topLine.width
                height: parent.height - topBarLeft.height - 20
                spacing: 5

                model: groupListModel
                delegate: groupInfoDelegate

                onCurrentIndexChanged: {
                    selectGroup = currentIndex
                }
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
                            text: owner
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

                            MouseArea {
                                anchors.fill: parent

                                onDoubleClicked: {
                                    console.log("Double-clicked item:",
                                                firstPageListView.currentIndex)
                                    stackView.push(bucketPage)
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    Component {
        id: bucketPage
        BucketPage {
            bucketIndex: selectGroup
            groupModel: groupListModel
        }
    }

    Component {
        id: objectPage
        ObjectPage {}
    }

    StackView {
        id: stackView
        width: topLine.width
        height: parent.height - topBarLeft.height - 20
        initialItem: firstPage

        anchors.top: topLine.bottom
        anchors.horizontalCenter: topLine.horizontalCenter
        anchors.topMargin: 10
    }
}

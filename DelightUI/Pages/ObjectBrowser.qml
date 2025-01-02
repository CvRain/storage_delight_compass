import QtQuick
import QtQuick.Controls
import QtWebEngine
import Storage.Model
import Storage.Service
import "../Components"

Rectangle {
    property Alert alertInstance
    property int selectGroup
    property string currentBucketName: ""
    property string currentSourceId: ""
    property string currentGroupId: ""

    id: root
    anchors.fill: parent

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
                    return
                }
                alertInstance.text = qsTr("you are in the root path")
                alertInstance.level = "info"
                alertInstance.show()
            }
        }

        Text {
            id: pageTitleText
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

        Item {
            ListView {
                id: firstPageListView
                width: topLine.width
                height: parent.height - topBarLeft.height - 20
                spacing: 5

                model: groupListModel
                delegate: groupInfoDelegate

                onCurrentIndexChanged: {
                    selectGroup = currentIndex
                    let groupId = firstPageListView.model.get(firstPageListView.currentIndex).id
                    console.debug("groupId: ", groupId)
                    currentGroupId = groupId
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
                                    enterBucket()
                                }

                                function enterBucket() {
                                    let currentIndex = firstPageListView.currentIndex
                                    console.log("enter item:", currentIndex)

                                    if (groupListModel.isOwner(currentIndex)
                                            || groupListModel.isMember(
                                                currentIndex)) {
                                        stackView.push(bucketPage)
                                        return
                                    }

                                    alertInstance.text = qsTr(
                                                "you are not owner nor memebr in this group")
                                    alertInstance.level = "warn"
                                    alertInstance.show()
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
            width: stackView.width
            height: stackView.height
            bucketIndex: selectGroup
            groupModel: groupListModel
            groupId: currentGroupId

            onBucketSelected: function (bucketName, sourceId) {
                currentBucketName = bucketName
                currentSourceId = sourceId
                console.debug("push objcetPage")
                console.debug("bucketName ", bucketName)
                console.debug("sourceId ", sourceId)

                var newObjectPage = objectPage.createObject(stackView)
                newObjectPage.initialized(bucketName, sourceId)
                stackView.push(newObjectPage)
            }
        }
    }

    Component {
        id: objectPage
        ObjectPage {
            bucketName: currentBucketName
            sourceId: currentSourceId
            alertInstance: root.alertInstance
        }
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

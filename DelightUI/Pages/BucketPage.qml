import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtWebEngine
import Storage.Model
import Storage.Service
import "../Components"

Item{
    id: root
    property int bucketIndex: 0
    property GroupListModel groupModel: GroupListModel{}

    signal bucketSelected(string sourceId, string bucketName)

    BucketListModel{
        id: bucketListModel
        Component.onCompleted: {
            console.debug("bucket group", bucketIndex)
            bucketListModel.setBuckets(groupModel.getBuckets(bucketIndex))
        }
    }

    ListView {
        id: firstPageListView
        width: topLine.width
        height: parent.height - topBarLeft.height - 20
        spacing: 5

        model: bucketListModel
        delegate: groupInfoDelegate
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
                    id: bucketNameText
                    text: bucketName
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
                    id: sourceIdText
                    text: sourceId
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
                            bucketSelected(bucketNameText.text, sourceIdText.text)
                            console.debug("selected: ", bucketNameText.text, "-", sourceIdText.text)
                        }
                    }
                }
            }
        }
    }
}

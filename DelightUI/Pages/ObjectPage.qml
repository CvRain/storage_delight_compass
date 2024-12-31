import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtWebEngine
import Storage.Model
import Storage.Service
import "../Components"

Item{
    id: root
    property string sourceId
    property string bucketName
    signal initialized(string bucketName, string sourceId)

    ObjectModel{
        id: objectModel
    }

    Connections {
        target: root // 确保连接到正确的对象
        onInitialized: function(bucketName, sourceId) {
            root.bucketName = bucketName;
            root.sourceId = sourceId;

            // 调用更新逻辑
            objectModel.setBucketName(bucketName);
            objectModel.setSourceId(sourceId);
            objectModel.update();

            console.debug("Initialized ObjectModel with:", bucketName, "-", sourceId);
        }
    }


    ListView {
        id: firstPageListView
        width: topLine.width
        height: parent.height - topBarLeft.height - 20
        spacing: 5

        model: objectModel
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
                    id: objectNameText
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
                    id: objectTag
                    text: etag
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
                        }
                    }
                }
            }
        }
    }
}

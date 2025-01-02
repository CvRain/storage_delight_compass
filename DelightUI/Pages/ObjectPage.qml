import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import QtWebEngine
import Storage.Model
import Storage.Service
import "../Components"

Item {
    id: root
    property string sourceId
    property string bucketName
    property Alert alertInstance
    signal initialized(string bucketName, string sourceId)

    ObjectModel {
        id: objectModel
    }

    Connections {
        target: root // 确保连接到正确的对象
        function onInitialized(bucketName, sourceId) {
            root.bucketName = bucketName
            root.sourceId = sourceId

            // 调用更新逻辑
            objectModel.setBucketName(bucketName)
            objectModel.setSourceId(sourceId)
            objectModel.update()

            console.debug("Initialized ObjectModel with:", bucketName,
                          "-", sourceId)
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
                        id: objectTagMouseArea
                        anchors.fill: parent
                        acceptedButtons: Qt.LeftButton | Qt.RightButton

                        onDoubleClicked: {
                            confirmationDialog.open()
                        }

                        onReleased: function (mouseEvent) {
                            if (mouseEvent.button === Qt.RightButton) {
                                contextMenu.popup()
                            }
                        }
                    }
                }
            }
        }
    }

    Menu {
        id: contextMenu

        MenuItem {
            text: qsTr("download")
            onTriggered: {
                confirmationDialog.open()
            }
        }

        MenuItem {
            text: qsTr("remove")
            onTriggered: {
                confirmDeleteDialog.open()
            }
        }

        MenuItem{
            text: qsTr("upload")
            onTriggered: {
                fileDialog.open()
            }
        }
    }

    // 确认对话框组件定义。
    Dialog {
        id: confirmationDialog

        title: "confirm download"
        modal: true

        ColumnLayout {
            spacing: 10

            Text {
                text: qsTr("Wanna to download?")
                wrapMode: Text.WrapAnywhere
            }

            RowLayout {
                spacing: 10

                Button {
                    text: qsTr("cancel")
                    onClicked: confirmationDialog.close()
                }

                Button {
                    text: qsTr("confirm")
                    onClicked: function () {
                        let currentIndex = firstPageListView.currentIndex
                        let sourceId = objectModel.sourceId
                        let bucketName = objectModel.bucketName
                        let objectName = firstPageListView.model.get(
                                currentIndex).name

                        console.debug("prepare download: ")
                        console.debug("sourceId", sourceId)
                        console.debug("bucketName", bucketName)
                        console.debug("objectName", objectName)

                        HttpClient.objectDownload(objectName,
                                                  bucketName, sourceId)
                        confirmationDialog.close()
                    }
                }
            }
        }
    }

    Dialog {
        id: confirmDeleteDialog
        width: 300
        height: 150
        title: qsTr("Remove member")
        standardButtons: Dialog.Ok | Dialog.Cancel
        modal: true

        background: Rectangle {
            width: confirmDeleteDialog.width
            height: confirmDeleteDialog.height
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

        onAccepted: {
            let fileName = firstPageListView.model.get(firstPageListView.currentIndex).name
            console.debug("prepare to remove: ", fileName)
            HttpClient.objectRemove(fileName, objectModel.bucketName, objectModel.sourceId)
        }
    }

    FileDialog {
        id: fileDialog

        title: qsTr("upload file")
        currentFolder: "./"
        nameFilters: ["All Files (*)"]

        onAccepted: {
            console.debug("selected file: ", fileDialog.selectedFile)
            HttpClient.objectUpload(fileDialog.selectedFile, objectModel.bucketName, objectModel.sourceId)
        }

        onRejected: {
            console.debug("user cancel")
        }
    }

    Connections {
        target: HttpClient
        function onObjectDownloadFinish(){
            console.log("onObjectDownloadFinish")

            alertInstance.text = "download finish"
            alertInstance.level = "success"
            alertInstance.show()
        }
        function onRequestFailed(error){
            console.log("onRequestFailed")

            alertInstance.text = error
            alertInstance.level = "warn"
            alertInstance.show()
        }

        function onObjectUploadFinish(){
            console.log("onObjectUploadFinish")

            alertInstance.text = "upload finish"
            alertInstance.level = "success"
            alertInstance.show()
            firstPageListView.model.update()
        }

        function onObjectRemoveFinish(){
            console.log("onObjectRemoveFinish")

            alertInstance.text = "remove finish"
            alertInstance.level = "success"
            alertInstance.show()
            firstPageListView.model.update()
        }
    }
}

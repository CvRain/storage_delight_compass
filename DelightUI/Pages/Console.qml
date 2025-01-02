import QtQuick
import QtQuick.Controls
import QtWebEngine
import QtCore
import Storage.Model
import Storage.Service
import "../Components"

Item {
    property Alert alertInstance
    property string savePath: ""

    id: root
    anchors.fill: parent

    Rectangle {
        id: bar
        width: root.width
        height: 40
        color: "#eff1f5"
        border.color: "#ccd0da"
        border.width: 2

        Row {
            id: topbarMenu
            anchors.left: bar.left
            anchors.top: bar.top
            anchors.verticalCenter: parent.verticalCenter
            spacing: 5
            width: parent.width - storageName
            height: parent.height - 5

            FlatComboBox {
                id: combox

                checkedColor: "#dc8a78"
                onActivated: {
                    webview.url = sourceList.getUrl(currentIndex)
                    storageName.text = sourceList.getUrl(0)
                }

                onCurrentIndexChanged: {
                    sourceList.setCurrentIndex(currentIndex)
                    storageName.text = sourceList.getUrl(0)
                }

                Component.onCompleted: {
                    storageUpdate()
                }
            }

            BaseButton {
                id: updateButton
                width: 65
                height: parent.height
                text: qsTr("update")
                textColor: "white"
                radius: 5
                color: combox.checkedColor
                onClicked: {
                    storageUpdate()
                }
            }

            BaseButton {
                id: removeItemButton
                width: 65
                height: parent.height
                text: qsTr("remove")
                textColor: "white"
                radius: 5
                color: combox.checkedColor
                onClicked: {
                    if (sourceList.size() === 0) {
                        alertInstance.left = "warn"
                        alertInstance.text = "no storage"
                        alertInstance.show()
                        return
                    }
                    var dialogInstance = removeStorageDialog.createObject(root)
                    dialogInstance.open()
                }
            }

            BaseButton {
                id: addItemButton
                width: 65
                height: parent.height
                text: qsTr("add")
                textColor: "white"
                radius: 5
                color: combox.checkedColor

                Connections {
                    target: addItemButton
                    function onClicked() {
                        console.debug("User role ", UserManager.getRole())
                        if (UserManager.getRole() !== 0) {
                            alertInstance.text = qsTr("No permission")
                            alertInstance.level = "warn"
                            alertInstance.show()
                            return
                        }
                        var dialogInstance = addStorageDialog.createObject(root)
                        dialogInstance.open()
                    }
                }
            }
        }



        Text {
            id: storageName
            font.pixelSize: 16
            color: "#dd7878"
            font.bold: true
            width: 100

            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignRight
            anchors.right: parent.right
            anchors.rightMargin: 25
            anchors.verticalCenter: parent.verticalCenter

            MouseArea{
                anchors.fill: parent
                onClicked: {
                    let storageId = sourceList.getId(combox.currentIndex);
                    utils.copyToClipboard(storageId)
                    alertInstance.text = "copy success: " + storageId
                    alertInstance.level = "success"
                    alertInstance.show()
                }

            }
        }
    }

    Component {
        id: addStorageDialog

        StorageSoreAppend {
            anchors.centerIn: parent

            onAccepted: {
                var sourceData = {
                    "ak": akText,
                    "sk": skText,
                    "isHttps": isHttp,
                    "url": urlText,
                    "name": nameText
                }
                sourceList.append(sourceData)
            }
        }
    }

    Component {
        id: removeStorageDialog
        StorageSoreRemove {
            anchors.centerIn: parent
            onAccepted: {
                if (sourceList.size() !== 0) {
                    sourceList.remove(sourceList.currentIdex)
                    return
                }
            }
        }
    }

    WebEngineView {
        id: webview
        width: parent.width
        height: parent.height - bar.height
        anchors.top: bar.bottom
        anchors.left: bar.left

        profile: WebEngineProfile {
            id: webprofile

            onDownloadRequested: function (downloadItem) {
                //当触发下载时，onDownloadRequested 事件会被调用，接收 WebEngineDownloadItem 对象来管理下载过程。
                console.log("Download requested: ", downloadItem.url)
                // 使用 FileDialog 让用户选择文件路径
                // folderDialog.open();

                //设置下载路径,会获取电脑标准的下载路径进行拼接
                var customSavePath = StandardPaths.writableLocation(
                            StandardPaths.DownloadLocation).toString().replace(
                            "file:///", "")
                ;
                savePath = customSavePath
                console.log("Custom save path: ", customSavePath)
                console.log("downloadDirectory path: ",
                            downloadItem.downloadDirectory)
                downloadItem.downloadDirectory = customSavePath
                console.log("downloadDirectory path: ",
                            downloadItem.downloadDirectory)
                downloadItem.accept()
            }

            onDownloadFinished: function (downloadItem) {
                if (downloadItem.state === WebEngineDownloadRequest.DownloadCancelled) {
                    console.log("下载成功 ")
                    dialogText.text = "下载成功,地址为:" + savePath
                    downloadCompleteDialog.open()
                } else if (downloadItem.state === WebEngineDownloadRequest.DownloadCancelled) {
                    console.log("下载失败")
                }

            }
        }
    }

    Dialog {
        id: downloadCompleteDialog
        title: "下载通知"
        standardButtons: Dialog.Ok
        anchors.centerIn: parent
        property var downloadItem: null
        onAccepted: {
            console.log("Dialog accepted")
        }
        onRejected: {
            console.log("Dialog closed")
        }

        Text {
            id: dialogText
            anchors.margins: 10
            anchors.fill: parent
            text: "下载信息将在这里显示"
        }
    }

    SourceList {
        id: sourceList

        onRequestFailed: {
            alertInstance.text = error
            alertInstance.left = "error"
            alertInstance.show()
        }
    }

    function storageUpdate() {
        sourceList.update()
        alertInstance.text = "update"
        alertInstance.level = "info"
        alertInstance.show()
        console.debug("storage source update")

        if (sourceList.size() === 0) {
            webview.url = "https://doc.qt.io/"
            storageName = "no storage source"
            alertInstance.text = "no storage source"
            alertInstance.level = "info"
            alertInstance.show()
        } else {
            combox.model = sourceList.items
            webview.url = sourceList.getUrl(0)
            storageName.text = sourceList.getUrl(0)
        }
    }

    Utils{
        id: utils
    }
}

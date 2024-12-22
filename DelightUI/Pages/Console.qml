import QtQuick
import QtQuick.Controls
import QtWebEngine
import Storage.Model
import Storage.Service
import "../Components"

Item {
    property Alert alertInstance

    id: root
    anchors.fill: parent

    Rectangle{
        id: bar
        width: root.width
        height: 40
        color: "#eff1f5"
        border.color: "#ccd0da"
        border.width: 2

        Row{
            id: topbarMenu
            anchors.left: bar.left
            anchors.top: bar.top
            spacing: 5
            width: parent.width - storageName
            height: parent.height

            FlatComboBox{
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

            BaseButton{
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

            BaseButton{
                id: removeItemButton
                width: 65
                height: parent.height
                text: qsTr("remove")
                textColor: "white"
                radius: 5
                color: combox.checkedColor
                onClicked: {
                    if(sourceList.size() === 0){
                        alertInstance.left = "warn"
                        alertInstance.text = "no storage"
                        alertInstance.show()
                        return;
                    }
                    var dialogInstance = removeStorageDialog.createObject(root)
                    dialogInstance.open()
                }
            }

            BaseButton{
                id: addItemButton
                width: 65
                height: parent.height
                text: qsTr("add")
                textColor: "white"
                radius: 5
                color: combox.checkedColor
                onClicked: {
                    var dialogInstance = addStorageDialog.createObject(root)
                    dialogInstance.open()
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
            anchors.verticalCenter: parent.verticalCente
        }
    }

    Component{
        id: addStorageDialog
        StorageSoreAppend{
            anchors.centerIn: parent
        }


    }

    Component{
        id: removeStorageDialog
        StorageSoreRemove{
            anchors.centerIn: parent
            onAccepted: {
                if(sourceList.size() !== 0){
                    sourceList.remove(sourceList.currentIdex)
                    return
                }
            }
        }
    }

    WebEngineView{
        id: webview
        width: parent.width
        height: parent.height - bar.height
        anchors.top: bar.bottom
        anchors.left: bar.left
    }

    SourceList{
        id: sourceList

        onRequestFailed: {
            alertInstance.level = "error"
            alertInstance.text = error
            alertInstance.show()
        }
    }

    function storageUpdate(){
        sourceList.update()
        alertInstance.text = "update"
        alertInstance.level = "info"
        alertInstance.show()
        console.debug("storage source update")

        if(sourceList.size() === 0){
            webview.url = "https://doc.qt.io/"
            storageName = "no storage source"
            alertInstance.text = "no storage source"
            alertInstance.level = "info"
            alertInstance.show()

        }else{
            model = sourceList.items
            webview.url = sourceList.getUrl(0)
            storageName.text = sourceList.getUrl(0)
        }
    }
}

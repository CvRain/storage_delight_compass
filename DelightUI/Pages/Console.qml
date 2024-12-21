import QtQuick
import QtQuick.Controls
import QtWebEngine
import Storage.Model
import Storage.Service
import "../Components"

Item {
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
                model: sourceList.items
                checkedColor: "#dc8a78"

                onActivated: {
                    webview.url = sourceList.getUrl(currentIndex)
                    storageName.text = sourceList.getUrl(0)
                }

                onCurrentIndexChanged: {
                    sourceList.setCurrentIndex(currentIndex)
                    storageName.text = sourceList.getUrl(0)
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
                    sourceList.update()
                }
            }

            BaseButton{
                id: removeItemButton
                width: 65
                text: qsTr("remove")
                textColor: "white"
                radius: 5
                color: combox.checkedColor
                onClicked: {
                }
            }

            BaseButton{
                id: addItemButton
                width: 65
                text: qsTr("add")
                textColor: "white"
                radius: 5
                color: combox.checkedColor
                onClicked: {

                }

            }

        }

        Text {
            id: storageName
            text: sourceList.getUrl(0)
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

    WebEngineView{
        id: webview
        width: parent.width
        height: parent.height - bar.height
        anchors.top: bar.bottom
        anchors.left: bar.left
        url: sourceList.getUrl(0)
    }

    SourceList{
        id: sourceList
    }
}

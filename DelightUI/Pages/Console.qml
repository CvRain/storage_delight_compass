import QtQuick
import QtQuick.Controls
import QtWebEngine
import Storage.Model
import "../Components"

Item {
    id: root
    width: parent.width
    height: parent.height

    Rectangle{
        id: bar
        width: root.width
        height: 40
        color: "#eff1f5"
        border.color: "#ccd0da"
        border.width: 2

        FlatComboBox{
            id: combox
            width: parent.width / 5
            height: parent.height
            model: sourceList.items
            anchors.left: combox.right
            anchors.top: combox.top
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

        Text {
            id: storageName
            text: sourceList.getUrl(0)
            font.pixelSize: 16
            color: "#dd7878"
            font.bold: true

            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignRight
            anchors.right: parent.right
            anchors.rightMargin: 15
            anchors.verticalCenter: parent.verticalCenter
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

import QtQuick
import QtQuick.Controls
import QtWebEngine
import Storage.Model
import Storage.Service
import "../Components"

Item {
    id: root
    width: parent.width
    height: parent.height

    Text {
        id: temp
        text: qsTr("ObjectBrowser")
        anchors.centerIn: parent
    }
}
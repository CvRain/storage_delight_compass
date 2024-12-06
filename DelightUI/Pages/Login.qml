import QtQuick
import QtQuick.Controls
import "../Components"

Card{
    Text{
        text: qsTr("Sign in")
        color: "#5c5f77"
        font.bold: true
        font.pixelSize: 28

        anchors.left: parent.left
        anchors.top: parent.top
        anchors.leftMargin: 35
        anchors.topMargin: 100
    }


}

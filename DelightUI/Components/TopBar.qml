import QtQuick
import QtQuick.Controls

Rectangle {
    signal userLogout

    id: root
    width: 1920
    height: 65
    color: "#eff1f5"

    Image {
        id: icon
        source: "qrc:/res/img/storage_delight_placeholder.png"
        height: parent.height * 0.9
        autoTransform: true
        fillMode: Image.PreserveAspectFit

        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: 5
    }
}

import QtQuick 2.15

Rectangle{
    property alias title: titleText

    id: root
    width: 450
    height: 600
    color: "#eff1f5"
    radius: 15
    border.width: 3
    border.color: "#e6e9ef"

    Text{
        id: titleText
        anchors.top: parent.top
        anchors.topMargin: 5
        anchors.left: parent.left
        anchors.leftMargin: 5
        visible: false
    }
}

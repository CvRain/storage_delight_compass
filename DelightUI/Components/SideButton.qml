import QtQuick 2.15

Rectangle {
    property color bgColorNormal: "#414559"
    property color bgColorHover: "#51576d"
    property color bgColorClick: "#626880"
    property color lineColorHover: "#dc8a78"
    property color lineColorClick: "#e64533"
    property alias text: barText.text
    property alias icon: barIcon.source

    signal entered()
    signal exited()
    signal clicked()
    signal released()

    id: root
    width: 80
    height: 50
    color: root.bgColorNormal

    Image {
        id: barIcon
        source: ""
        height: parent.height * 0.9
        width: parent.height * 0.9
        autoTransform: true
        fillMode: Image.PreserveAspectFit

        anchors.left: parent.left
        anchors.leftMargin: 5
        anchors.verticalCenter: parent.verticalCenter
    }

    Text {
        id: barText
        text: "edit it"
        color: "#eff1f5"
        font.pixelSize: 26
        font.bold: true
        verticalAlignment: Text.AlignVCenter

        anchors.left: barIcon.right
        anchors.leftMargin: 5
        anchors.verticalCenter: parent.verticalCenter
    }

    Rectangle {
        id: line
        width: 4
        height: parent.height
        color: "#5D5F61"
        visible: false // Border visibility controlled by mouse events
        opacity: 1
    }

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true

        onEntered: {
            root.color = bgColorHover
            line.color = lineColorHover
            line.visible = true
            root.entered()
        }

        onExited: {
            root.color = root.bgColorNormal
            line.color = root.bgColorNormal
            line.visible = false
            root.exited()
        }

        onClicked: {
            root.color = root.bgColorClick
            line.color = root.lineColorClick
            line.visible = true
            root.clicked()
        }

        onReleased: {
            root.color = root.bgColorNormal
            line.color = root.bgColorNormal
            line.visible = false
            root.released()
        }

    }
}

import QtQuick 2.0
import QtQuick.Controls 2.0

Switch {
    property color checkedColor: "#4c4f69"
    property color backgroundColor: "#e6e9ef"
    property color borderColor: "#5c5f77"
    property real indicatorWidth: 55
    property real indicatorHeight: 34

    id: root
    width: 110
    height: 68

    indicator: Rectangle {
        width: root.indicatorWidth
        height: root.indicatorHeight
        radius: height / 2
        color: root.checked ? root.checkedColor : root.backgroundColor
        border.width: 1
        border.color: root.checked ? root.backgroundColor :  root.checkedColor

        Rectangle {
            x: root.checked ? parent.width - width - 2 : 1
            width: root.checked ? parent.height - 4 : parent.height - 2
            height: width
            radius: width / 2
            anchors.verticalCenter: parent.verticalCenter
            color: root.checked ? root.backgroundColor: root.checkedColor
            border.color: "#D5D5D5"

            Behavior on x {
                NumberAnimation { duration: 200 }
            }
        }
    }
}

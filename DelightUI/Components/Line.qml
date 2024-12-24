import QtQuick

Item{
    signal entered
    signal exited
    signal clicked
    signal released

    property bool isRound: false
    property alias color: line.color

    id: root
    width: 100
    height: 2

    Rectangle{
        id: line
        width: parent.width
        height: parent.height
        radius: isRound ? parent.height / 2 : 0
        color: "#4c4f69"

        anchors.fill: parent
        MouseArea{
            hoverEnabled: true
            onEntered: {
                root.entered()
            }
            onExited: {
                root.exited()
            }

            onClicked: {
                root.clicked()
            }

            onReleased: {
                root.released()
            }
        }
    }
}

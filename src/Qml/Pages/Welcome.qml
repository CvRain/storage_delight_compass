import QtQuick

Rectangle{
    id: root
    width: 1920
    height: 1080
    color: "steelblue"

    signal pageChange()

    Rectangle{
        id: button
        width: 100
        height: 60
        color: "orange"
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        MouseArea{
            anchors.fill: parent
            onClicked:{
                root.pageChange()
                console.debug("[debug]: page change signal")
            }
        }
    }
}

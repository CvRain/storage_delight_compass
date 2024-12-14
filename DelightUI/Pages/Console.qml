import QtQuick
import QtQuick.Controls
import QtWebEngine

Item {
    id: root
    width: parent.width
    height: parent.height

    Rectangle{
        id: bar
        width: root.width
        height: 80

        ComboBox{
            id: combox
            width: parent.width / 5
            height: parent.height
            model: [""]
        }
    }
}

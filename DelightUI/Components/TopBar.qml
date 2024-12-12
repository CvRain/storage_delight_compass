import QtQuick
import QtQuick.Controls

Rectangle {
    signal userLogout

    id: root
    width: 1920
    height: 65
    color: "#232634"

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

    Text {
        id: barTitle
        text: "unknown user"
        font.pixelSize: 28
        color: "white"
        horizontalAlignment: Text.AlignRight
        verticalAlignment: Text.AlignVCenter
        width: parent.width / 5
        height: parent.height

        anchors.right: parent.right
        anchors.rightMargin: 15
        anchors.verticalCenter: parent.verticalCenter

        MouseArea {
            id: mouseArea
            anchors.fill: parent
            hoverEnabled: true
            acceptedButtons: Qt.LeftButton | Qt.RightButton

            onClicked: {
                if (mouse.button == Qt.RightButton) {
                    barMenu.popup()
                }
            }
        }
    }

    Menu {
        id: barMenu

        MenuItem {
            text: qsTr("logout")
            action: {
                root.userLogout()
            }
        }
    }
}

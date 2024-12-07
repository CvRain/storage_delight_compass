import QtQuick 2.15

Rectangle{
    signal clicked()
    signal entered()
    signal exited()

    property alias text: buttonText.text
    property alias textColor: buttonText.color

    id: root
    width: parent.width / 2
    height: 50
    radius: 15
    color:"#bcc0cc"

    Text{
        id: buttonText
        anchors.fill: parent
        text: qsTr("Sign in")
        font.pixelSize: 18
        font.bold: true
        color: "#4c4f69"
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter

    }

    MouseArea{
        anchors.fill: parent
        hoverEnabled: true

        onEntered:{
            root.color = "#acb0be"
            root.entered()
        }

        onExited:{
            root.color = "#bcc0cc"
            root.exited()
        }

        onClicked:{
            root.clicked()
        }
    }
}

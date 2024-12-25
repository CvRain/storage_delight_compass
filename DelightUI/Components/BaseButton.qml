import QtQuick 2.15

Rectangle{
    signal clicked()
    signal entered()
    signal exited()

    property alias text: buttonText.text
    property alias textColor: buttonText.color
    property alias font: buttonText.font

    id: root
    implicitWidth: parent.width / 2
    implicitHeight: 50
    radius: 15
    color:"#bcc0cc"

    Text{
        id: buttonText
        text: qsTr("Sign in")
        font.pixelSize: 18
        font.bold: true
        color: "#4c4f69"
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        anchors.centerIn: parent
        anchors.fill: parent
    }

    MouseArea{
        anchors.fill: parent
        hoverEnabled: true

        onEntered:{
            root.entered()
        }

        onExited:{
            root.exited()
        }

        onClicked:{
            root.clicked()
        }
    }
}

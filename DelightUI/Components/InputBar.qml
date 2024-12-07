import QtQuick 2.15

Rectangle{
    property color background: "#e6e9ef"
    property color borderColor: "#ccd0da"
    property int textMode: TextInput.Normal
    property alias text: textInput.text


    id: root
    width: 300
    height: 40
    color: background
    border.color: borderColor
    border.width: 2
    radius: 15

    TextInput{
        id: textInput
        width: parent.width - 15
        height: parent.height - 5
        font.pixelSize: 16
        verticalAlignment: TextInput.AlignVCenter
        horizontalAlignment:TextInput.AlignLeft
        maximumLength: 64
        echoMode: root.textMode
        wrapMode: textInput.wrapMode
        clip:true
        anchors.centerIn: parent
    }

    Rectangle{
        id: underline
        width: parent.width
        height: 3
        color:"#4c4f69"
        radius: 35

        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
    }
}

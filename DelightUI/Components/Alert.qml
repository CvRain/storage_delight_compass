import QtQuick 2.15

Rectangle{
    property alias icon: icon.source
    property alias text: title.text
    property string level: "info"
    property bool isVisable: false
    property int displayDuration: 5000  // Default duration in milliseconds

    id: root
    width: 550
    height: 35
    radius: 5
    color: "#d9f7b3"
    border.color: "#95de64"
    border.width: 1
    opacity: isVisable ? 1 : 0;

    NumberAnimation {
        id: opacityAnimation
        target: root
        property: "opacity"
        duration: 200
        easing.type: Easing.InOutQuad
    }

    Timer{
        id: hideTimer
        interval: displayDuration
        running: false
        repeat: false
        onTriggered: {
            isVisable = false
        }
    }

    Component.onCompleted: {
        updateAppearance()
    }

    function updateAppearance(){
        switch(level){
        case "success":
            root.color = "#f6ffed"
            root.border.color = "#b7eb8f"
            break
        case "warn":
            root.color = "#fffbe6"
            root.border.color = "#ffe58f"
            break
        case "error":
            root.color ="#fff2f0"
            root.border.color = "#ffccc7"
            break
        case "info":
            root.color = "#e6f4ff"
            root.border.color = "#91caff"
            break
        }
    }

    function show(duration){
        if(duration !== undefined){
            displayDuration = duration
        }
        isVisable = true
        hideTimer.start()
        opacityAnimation.from = 0
        opacityAnimation.to = 1
        opacityAnimation.start()
    }

    Image {
        id: icon
        height: root.height * 0.9
        width: root.height * 0.9
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: 15
    }

    Text{
        id: title
        width: parent.width - icon.width
        height: parent.height * 0.9
        color: "#4c4f69"
        font.pixelSize: 14
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: icon.right
        anchors.leftMargin: 5
    }

    onVisibleChanged: {
        if(!isVisable){
            opacityAnimation.from = 1
            opacityAnimation.to = 0
            opacityAnimation.start()
        }
    }
}
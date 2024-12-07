import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import "../Components"

Card {
    id: root
    height: 500

    signal loginClicked(string name, string password)
    signal registerClicked()

    Image {
        id: titleImage
        source: "qrc:/res/img/storage_delight_placeholder.png"
        width: parent.width / 3
        autoTransform: true
        fillMode: Image.PreserveAspectFit

        anchors.top: parent.top
        anchors.topMargin: 15
        anchors.left: parent.left
        anchors.leftMargin: 15
    }

    Text {
        id: title
        text: qsTr("Sign in")
        color: "#5c5f77"
        font.bold: true
        font.pixelSize: 28

        anchors.top: titleImage.bottom
        anchors.topMargin: 55
        anchors.horizontalCenter: parent.horizontalCenter
    }

    GridLayout {
        id: formLayout
        columns: 2

        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: title.bottom
        anchors.topMargin: 35

        Text {
            text: qsTr("Account:")
            font.pixelSize: 16
            font.bold: true
        }
        InputBar {
            id: userNameInput
            borderColor: "transparent"
            background: "transparent"
        }

        Text {
            text: qsTr("Password:")
            font.pixelSize: 16
            font.bold: true
        }
        InputBar {
            id: userPasswordInput
            borderColor: "transparent"
            background: "transparent"
            textMode: TextInput.Password
        }
    }

    Switch{
        id: switchRole
        width: 10
        height: 10

        anchors.top: formLayout.bottom
        anchors.topMargin: 35
        anchors.left: formLayout.left
    }

    Text{
        id: registerJump
        width: parent.width / 2
        height: 35
        text: qsTr("No account? Sign up here")
        color: "#6c6f85"
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignLeft
        font.pointSize: 10

        anchors.top: switchRole.bottom
        anchors.topMargin: 10
        anchors.right: root.right

        MouseArea{
            anchors.fill: parent
            hoverEnabled: true

            onEntered:{
                registerJump.color = "#5c5f77"
                registerJump.font.italic = true
            }

            onExited:{
                registerJump.color = "#6c6f85"
                registerJump.font.italic = false
            }

            onClicked:{
                root.registerClicked()
                console.debug("[debug] register page jump")
            }
        }
    }

    BaseButton{
        id: loginButton
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 55
        anchors.horizontalCenter: parent.horizontalCenter

        onClicked:{
            root.loginClicked(userNameInput.text, userPasswordInput.text)
            console.debug("[debug] user: ", userNameInput.text)
            console.debug("[debug] password: ", userPasswordInput.text)
        }
    }

}

import QtQuick
import QtQuick.Layouts
import "../Components"

Card {
    id: root
    height: 500

    signal registerClicked(string name, string password)
    signal loginPageJump()

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
        text: qsTr("Sign up")
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

    Text{
        id: registerJump
        width: parent.width / 2
        height: 35
        text: qsTr("Have account? login there")
        color: "#6c6f85"
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignLeft
        font.pointSize: 10

        anchors.top: formLayout.bottom
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
                root.loginPageJump()
            }
        }
    }

    BaseButton{
        id: loginButton
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 55
        anchors.horizontalCenter: parent.horizontalCenter
        text: qsTr("Sign up")

        onClicked:{
            root.registerClicked(userNameInput.text, userPasswordInput.text)
        }
    }

    function isNameInputEmpty(){
        return userNameInput.isEmpty()
    }

    function isPasswordInputEmpty(){
        return userPasswordInput.isEmpty()
    }

}

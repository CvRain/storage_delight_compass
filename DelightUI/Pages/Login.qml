import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import "../Components"

Card {
    id: root
    height: 500

    signal loginClicked(string name, string password, bool isAdmin)
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

    FlatSwitch{
        id: switchRole
        width: 40
        height: 40
        indicatorWidth: 40
        indicatorHeight: 20

        anchors.top: formLayout.bottom
        anchors.topMargin: 25
        anchors.left: formLayout.left

        Connections{
            target: switchRole

            function onCheckedChanged(){
                if(switchRole.checked === true){
                    switchRoleTip.color = switchRoleTip.activeColor
                    switchRoleTip.text = switchRoleTip.adminUser
                }else{
                    switchRoleTip.color = switchRoleTip.unactiveColor
                    switchRoleTip.text = switchRoleTip.commonUser
                }
            }
        }
    }

    Text{
        property string commonUser: qsTr("I am user")
        property string adminUser: qsTr("I am admin")
        property color unactiveColor: "#4c4f69"
        property color activeColor: "#dc8a78"

        id: switchRoleTip
        height: switchRole.height
        text: commonUser
        font.pixelSize: 14
        color: unactiveColor
        horizontalAlignment: Text.AlignLeft

        anchors.left: switchRole.right
        anchors.leftMargin: 7
        anchors.verticalCenter: switchRole.verticalCenter
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

        anchors.top: formLayout.bottom
        anchors.topMargin: 25
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
            root.loginClicked(userNameInput.text, userPasswordInput.text, switchRole.checked)
        }
    }


}

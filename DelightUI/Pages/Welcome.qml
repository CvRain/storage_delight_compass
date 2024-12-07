import QtQuick
import QtQuick.Controls

pragma ComponentBehavior: Bound

Rectangle {
    id: root
    width: 1920
    height: 1080
    color: "#e6e9ef"

    StackView {
        id: stackView
        width: parent.width
        height: parent.height
        initialItem: loginPage
    }

    Component {
        id: loginPage
        Login {
            id: loginLoader
            anchors.centerIn: parent
            onRegisterClicked: {
                stackView.replace(registerPage)
            }
        }
    }

    Component {
        id: registerPage
        Register {
            id: registerLoader
            anchors.centerIn: parent
            onLoginPageJump: {
                stackView.replace(loginPage)
            }
        }
    }
}

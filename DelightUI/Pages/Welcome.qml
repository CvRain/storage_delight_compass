import QtQuick
import QtQuick.Controls
import Storage.Service
import Storage.User
import Storage.Model

pragma ComponentBehavior: Bound

Rectangle {
    signal shouldClosePage(bool isShould)
    signal pageChange()

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

            Connections {
                target: loginLoader

                function onLoginClicked(name, password, isAdmin) {
                    console.debug("[debug] user: ", name)
                    console.debug("[debug] password: ", password)
                    console.debug('[debug] admin', isAdmin)
                    HttpClient.login(name, password)
                }

                function onRegisterClicked() {
                    stackView.replace(registerPage)
                }
            }
        }
    }

    Component {
        id: registerPage
        Register {
            id: registerLoader
            anchors.centerIn: parent

            Connections {
                target: registerLoader

                function onLoginPageJump(){
                    stackView.replace(loginPage)
                }

                function onRegisterClicked(name, password){
                    console.debug("[debug] user: ", name)
                    console.debug("[debug] password: ", password)
                }
            }
        }
    }
}

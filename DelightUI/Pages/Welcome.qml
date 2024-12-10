import QtQuick
import QtQuick.Controls
import Storage.Service 1.0
import Storage.Model
import Storage.User

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
                    var result = HttpClient.login(name, password)
                    console.log("code: ", result.code)
                    console.log("message: ", result.message)
                    console.log("result: ",result.result)
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

import QtQuick
import QtQuick.Controls
import Storage.Service 1.0
import Storage.Model
import Storage.User
import "../Components"

pragma ComponentBehavior: Bound

Rectangle {
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

    Alert{
        id: alert
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 35
        level: "warn"
        text: "test alert"
    }

    Component {
        id: loginPage
        Login {
            id: loginLoader
            anchors.centerIn: parent

            Connections {
                target: loginLoader

                function onLoginClicked(name, password, isAdmin) {
                    if (loginLoader.isUserNameInputEmpty() || loginLoader.isUserPasswordInputEmpty()){
                        alert.level = "warn"
                        alert.text = "nether nor password can't be empty"
                        alert.show(3000)
                        return
                    }
                    HttpClient.login(name, password)
                }

                function onRegisterClicked() {
                    stackView.replace(registerPage)
                }
            }
        }
    }

    Connections{
        target: HttpClient

        function onRequestFailed(errorString){
            alert.level = "error"
            alert.text = errorString
            alert.show()
        }

        function onUserLogged(code, result, message){
            if(code === 200 || code === "200"){
                alert.level ="success"
                alert.text = message + ": " + result
                alert.show(3000)
                pageChange()
            }
            else{
                alert.level = "warn"
            }
        }

        function onUserRegistered(code, result, message){
            if(code === 200 || code === "200"){
                alert.level ="success"
                alert.text = message + ": " + result
                alert.show(3000)
                pageChange()
            }
            else{
                alert.level = "warn"
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
                    if(registerLoader.isNameInputEmpty() || registerLoader.isPasswordInputEmpty()){
                        alert.level = "warn"
                        alert.text = "nether nor password can't be empty"
                        alert.show(3000)
                        return
                    }

                    HttpClient.userRegister(name, password, 1)
                }
            }
        }
    }
}

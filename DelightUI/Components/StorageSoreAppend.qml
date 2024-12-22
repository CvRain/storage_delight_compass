import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Dialog {
    id: dialog
    title: "输入参数"
    modal: true

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10

        Label {
            text: "URL:"
        }
        TextField {
            id: urlInput
        }

        Label {
            text: "Name:"
        }
        TextField {
            id: nameInput
        }

        Label {
            text: "AK:"
        }
        TextField {
            id: akInput
        }

        Label {
            text: "SK:"
        }
        TextField {
            id: skInput
        }

        CheckBox {
            id: isHttpInput
            text: "Is HTTP"
        }
    }

    standardButtons: Dialog.Ok | Dialog.Cancel

    onAccepted: {
        // var params = Qt.createQmlObject('import "." MyParams {};', root)
        // params.url = urlInput.text
        // params.name = nameInput.text
        // params.ak = akInput.text
        // params.sk = skInput.text
        // params.isHttp = isHttpInput.checked
        // myClass.myFunc(params)
        // dialog.close() // 关闭对话框
    }

    onRejected: {
        dialog.close() // 关闭对话框
    }
}

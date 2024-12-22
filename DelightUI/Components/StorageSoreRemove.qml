import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Dialog {
    id: dialog
    title: qsTr("remove storage source")
    modal: true

    Text {
        id: name
        text: qsTr("Are you sure to delete it?")
    }

    standardButtons: Dialog.Ok | Dialog.Cancel

    onAccepted: {

    }

    onRejected: {
        dialog.close() // 关闭对话框
    }
}

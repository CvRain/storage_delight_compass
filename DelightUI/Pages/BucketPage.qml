import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtWebEngine
import Storage.Model
import Storage.Service
import "../Components"

Item{
    id: root
    property int bucketIndex: 0
    property GroupListModel groupModel: GroupListModel{}
    property string groupId
    property int selectGroupIndex: 0

    signal bucketSelected(string sourceId, string bucketName)

    width: parent.width
    height: parent.height
    anchors.fill: parent

    BucketListModel{
        id: bucketListModel
        Component.onCompleted: {
            console.debug("bucket group", bucketIndex)
            bucketListModel.setBuckets(groupModel.getBuckets(bucketIndex))
        }
    }

    Row{
        id: buttonGroups
        width: parent.width / 4
        height: 30
        spacing: 10
        anchors.right: parent.right
        anchors.rightMargin: 15


        BaseButton{
            id: removeButcketButton
            text: qsTr("remove bucket")
            textColor: "white"
            color: "#dc8a78"
            font.pixelSize: 16
            height: buttonGroups.height

            onClicked: function(){
                if(groupModel.isOwner(selectGroupIndex) === false){
                    alertInstance.text = qsTr("You are not group owner")
                    alertInstance.level = "warn"
                    alertInstance.show()
                    return
                }

                if(firstPageListView.currentIndex !== -1){
                    var removeDialog = memberRemoveComponent.createObject(root)
                    removeDialog.open();
                }else{
                    alertInstance.text = qsTr("not select any item")
                    alertInstance.level = "warn"
                    alertInstance.show()
                    return
                }
            }
        }

        BaseButton{
            id: addBucketButton
            text: qsTr("new bucket")
            textColor: "white"
            color: "#dc8a78"
            font.pixelSize: 16
            height: buttonGroups.height

            onClicked: function(){
                if(groupModel.isOwner(selectGroupIndex) === false){
                    alertInstance.text = qsTr("You are not group owner")
                    alertInstance.level = "warn"
                    alertInstance.show()
                    return
                }
                var addBucketDialog = bucketSelectComponent.createObject(root)
                addBucketDialog.open()
            }
        }

        Component {
            id: memberRemoveComponent
            Dialog {
                id: memberRemoveDialog
                width: 300
                height: 150
                title: qsTr("Remove Group")
                standardButtons: Dialog.Ok | Dialog.Cancel
                modal: true

                background: Rectangle {
                    width: memberRemoveDialog.width
                    height: memberRemoveDialog.height
                    radius: 5
                }

                anchors.centerIn: parent

                Text {
                    id: tips
                    width: memberRemoveDialog.width - 10
                    text: qsTr("Are you sure to delete? It will be hard to recover")
                    font.bold: true
                    font.pixelSize: 16
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    anchors.centerIn: parent
                    lineHeight: 0.7
                    wrapMode: Text.WordWrap
                    fontSizeMode: Text.Fit
                }
                Text {
                    text: firstPageListView.model.getBucketName(firstPageListView.currentIndex)
                    font.bold: true
                    font.pixelSize: 15
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    anchors.top: tips.bottom
                    anchors.topMargin: 10
                    anchors.horizontalCenter: tips.horizontalCenter
                }

                onAccepted: function(){
                    let removeBucketName = firstPageListView.model.getBucketName(firstPageListView.currentIndex)
                    let removeSourceId = firstPageListView.model.getSourceId(firstPageListView.currentIndex)
                    HttpClient.removeGroupBucket(removeBucketName, removeSourceId, groupId)
                    firstPageListView.update()
                }
            }
        }
    }



    Component{
        id: bucketSelectComponent
        Dialog{
            width: 400
            height: 150
            title: qsTr("Add bucket")
            standardButtons: Dialog.Ok | Dialog.Cancel
            modal: true

            background: Rectangle {
                width: bucketSelectComponent.width
                height: bucketSelectComponent.height
                color: "white"
                radius: 5
            }

            GridLayout {
                columns: 2
                columnSpacing: 5
                rowSpacing: 10
                width: 150

                Text {
                    id: bucketNameText
                    height: 35
                    text: qsTr("Bucket name")
                }
                InputBar {
                    id: bucketNameInput
                    width: parent.width / 2
                    height: bucketNameText.height
                }
                Text {
                    id: sourceIdText
                    height: 35
                    text: qsTr("Source id")
                }
                InputBar {
                    id: sourceIdInput
                    width: parent.width / 2
                    height: sourceIdText.height
                }

                anchors.fill: parent
            }

            onAccepted: {
                HttpClient.addGroupBucket(bucketNameInput.text, sourceIdInput.text, groupId)
                firstPageListView.update()
            }

            anchors.centerIn: parent
        }
    }

    ListView {
        id: firstPageListView
        width: parent.width
        height: parent.height - 50
        spacing: 5

        model: bucketListModel
        delegate: groupInfoDelegate

        anchors.top: buttonGroups.bottom
        anchors.topMargin: 20
    }

    Component {
        id: groupInfoDelegate
        Item {
            id: wrapper
            width: root.width * 0.8
            height: 45


            MouseArea {
                anchors.fill: parent
                onClicked: wrapper.ListView.view.currentIndex = index
            }

            Rectangle {
                border.color: "#f2d5cf"
                border.width: 2
                radius: 15

                anchors.fill: parent
                color: wrapper.ListView.isCurrentItem ? "#f2d5cf" : "transparent"

                Text {
                    id: bucketNameText
                    width: parent.width
                    text: bucketName
                    color: wrapper.ListView.isCurrentItem ? "white" : "black"
                    font.pixelSize: 16
                    font.bold: true
                    verticalAlignment: Text.AlignVCenter
                    font.family: "Ubuntu Mono"

                    anchors.left: parent.left
                    anchors.leftMargin: 10
                    anchors.verticalCenter: parent.verticalCenter
                }

                Text {
                    id: sourceIdText
                    text: sourceId
                    width: parent.width
                    color: wrapper.ListView.isCurrentItem ? "white" : "black"
                    font.pixelSize: 16
                    font.bold: true
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignRight
                    font.family: "Ubuntu Mono"

                    anchors.right: parent.right
                    anchors.rightMargin: 10
                    anchors.leftMargin: 10
                    anchors.verticalCenter: parent.verticalCenter

                    MouseArea {
                        anchors.fill: parent

                        onDoubleClicked: {
                            console.debug("selected: ", bucketNameText.text, "-", sourceIdText.text)
                            bucketSelected(bucketNameText.text, sourceIdText.text)
                        }
                    }
                }
            }
        }
    }
}

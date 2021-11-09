import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15


Window {
    width: 600
    height: 200
    visible: true
    title: qsTr("Tortoise Stash")

    Rectangle {
        id: body

        anchors.fill: parent

        TextField {
            id: passwordInput

            anchors.left: parent.left
            anchors.top: parent.top
            anchors.right: passwordButton.left
            anchors.bottom: parent.bottom
            anchors.margins: 60

            color: "black"
            placeholderText: "Enter password..."
            font.pixelSize: 30
            maximumLength: 24
            selectByMouse: true
            focus: true

            echoMode: TextInput.Password
            passwordMaskDelay: 1000

        }

        Button {
            id: passwordButton

            width: body.width * 0.3

            anchors.top: parent.top
            anchors.right: parent.right
            anchors.bottom: parent.bottom

            text: "Encrypt!"
            font.pointSize: 25

            onClicked: {

            }
        }
    }
}

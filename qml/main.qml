import QtQuick
import demo
Window {
    id: main

    width: 1280
    height: 720
    visible: true
    color: "#000000"
    title: qsTr("Hello World")

    Player {
        width: parent.width - 16
        height: parent.height - 16
        anchors.centerIn: parent
    }
}

import QtQuick 2.6

Rectangle {
    color: "#d0d0d0"

    border {
        color: "black"
        width: 1
    }

    property alias text: label.text

    implicitHeight: label.height

    Text {
        id: label

        padding: 10
        elide: Text.ElideRight

        font {
            bold: true
            pixelSize: 14
        }
    }
}

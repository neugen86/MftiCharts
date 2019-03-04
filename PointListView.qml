import QtQuick 2.0
import QtQuick.Controls 2.0

import MyTypes 1.0

Control {
    id: control

    property SeriesModel series

    background: Rectangle {
        border.color: "gray"
        border.width: 1
    }

    topPadding: header.implicitHeight
    bottomPadding: footer.implicitHeight

    Row {
        id: header

        anchors.top: control.top

        ListColumnHeader {
            width: 100
            text: qsTr("X")
        }
        ListColumnHeader {
            width: 100
            text: qsTr("Y")
        }
        ListColumnHeader {
            width: 200
        }
    }

    Button {
        id: footer

        width: 400

        anchors.bottom: control.bottom

        text: qsTr("Append")

        onClicked: series.append(Qt.point(0, 0))
    }

    contentItem: ListView {
        id: view

        clip: true

        implicitWidth: 400

        model: control.series

        property point pastePoint
        property bool pasteMode: false

        delegate: PointListDelegate {}

        ScrollBar.vertical: ScrollBar {}
    }
}

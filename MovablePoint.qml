import QtQuick 2.0

import MyTypes 1.0

Item {
    id: control

    property real radius: 15

    property PointHandle handle

    property rect bound: Qt.rect(control.parent.x, control.parent.y,
                                 control.parent.width, control.parent.height)

    onXChanged: {
        if (handle) {
            handle_connections.enabled = false
            handle.x = toHandlePos(control.x)
            handle_connections.enabled = true
        }

        if (mouse_area.drag.active) {
            if (mouse_area.drag.axis !== Drag.XAxis) {
                mouse_area.drag.axis = Drag.XAxis
            }
        }

        updateVisible()
    }

    onYChanged: {
        if (handle) {
            handle_connections.enabled = false
            handle.y = toHandlePos(control.y)
            handle_connections.enabled = true
        }

        if (mouse_area.drag.active) {
            if (mouse_area.drag.axis !== Drag.YAxis) {
                mouse_area.drag.axis = Drag.YAxis
            }
        }

        updateVisible()
    }

    onHandleChanged: {
        if (handle) {
            control.x = fromHandlePos(handle.x)
            control.y = fromHandlePos(handle.y)
        }
    }

    Connections {
        id: handle_connections

        target: handle

        onXChanged: {
            control.x = fromHandlePos(handle.x)
        }

        onYChanged: {
            control.y = fromHandlePos(handle.y)
        }
    }

    function toHandlePos(value) {
        return value + control.radius
    }

    function fromHandlePos(value) {
        return value - control.radius
    }

    function updateVisible() {
        control.visible =
                control.x >= mouse_area.drag.minimumX &&
                control.y >= mouse_area.drag.minimumY &&
                control.x <= mouse_area.drag.maximumX &&
                control.y <= mouse_area.drag.maximumY

    }

    Rectangle {
        implicitWidth: radius * 2
        implicitHeight: radius * 2

        radius: control.radius
        color: mouse_area.pressed ? "red" : "yellow"

        readonly property bool handleActive: handle ? handle.active : false
        readonly property bool hightlighted: mouse_area.containsMouse || handleActive
        readonly property real borderWidth: hightlighted ? 5 : 1

        border {
            width: mouse_area.drag.active ? 0 : borderWidth
            color: "red"
        }

        MouseArea {
            id: mouse_area

            anchors.fill: parent

            hoverEnabled: true

            onReleased: {
                drag.axis = Drag.XAndYAxis
            }

            readonly property real minLeft: control.bound.x - control.radius
            readonly property real minTop: control.bound.y - control.radius

            drag {
                target: control

                axis: Drag.XAndYAxis

                minimumX: minLeft
                minimumY: minTop
                maximumX: control.bound.width + minLeft
                maximumY: control.bound.height + minTop

                onActiveChanged: {
                    control.z = drag.active ? 1 : 0
                }
            }
        }
    }
}

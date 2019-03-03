import QtQuick 2.0
import QtQuick.Controls 2.0

MouseArea {
    id: mouse_area

    hoverEnabled: true

    width: view.implicitWidth
    height: content.implicitHeight

    onContainsMouseChanged: {
        model.handle.active = containsMouse
    }

    Column {
        id: content

        Row {
            PointEditor {
                xValue: fromValue(model.x)
                yValue: fromValue(model.y)

                onXValueEdited: model.x = fromText(text)
                onYValueEdited: model.y = fromText(text)
            }

            Row {
                Button {
                    text: qsTr("Copy")
                    onClicked: {
                        view.pastePoint = model.point
                        console.log(view.pastePoint)
                        view.pasteMode = true
                    }
                }

                Button {
                    text: qsTr("Delete")
                    onClicked: series.remove(model.row)
                }

                visible: mouse_area.containsMouse && !view.pasteMode
            }
        }

        Row {
            PointEditor {
                xValue: fromValue(view.pastePoint.x)
                yValue: fromValue(view.pastePoint.y)

                enabled: false
            }

            Row {
                Button {
                    text: qsTr("Paste")
                    onClicked: {
                        series.insert(model.row + 1, view.pastePoint)
                        view.pasteMode = false
                    }
                }
                Button {
                    text: qsTr("Cancel")
                    onClicked: {
                        view.pasteMode = false
                    }
                }
            }

            visible: mouse_area.containsMouse && view.pasteMode
        }
    }

    function fromValue(value) {
        return value.toFixed(0)
    }

    function fromText(text) {
        var value = fromValue(Number(text))
        return isNaN(value) ? 0 : value
    }
}

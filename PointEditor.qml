import QtQuick 2.0
import QtQuick.Controls 2.0

Row {
    property string xValue
    property string yValue

    property int editorWidth: 100

    signal xValueEdited(string text)
    signal yValueEdited(string text)

    TextField {
        text: xValue
        implicitWidth: editorWidth
        onEditingFinished: xValueEdited(text)
    }

    TextField {
        text: yValue
        implicitWidth: editorWidth
        onEditingFinished: yValueEdited(text)
    }
}

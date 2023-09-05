import QtQuick 2.12
import TableModel 0.1

TableView {
    columnSpacing: 1
    rowSpacing: 1
    clip: true

    model: TableModel {}

    delegate: Rectangle {
        implicitWidth: 250
        implicitHeight: 40
        border.color: "black"
        border.width: 1
        color: "grey"
        Text {
            text: tabledata
            font.pointSize: 14
            anchors.centerIn: parent
        }
    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:0.33}D{i:34}
}
##^##*/

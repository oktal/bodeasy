// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
//import QtDesktop 0.1

Item {
    signal clicked(date d, int row)

    id: root
    width: 1400
    height: 600

    property bool highlightCurrentyDay: true

    onHeightChanged: updateCellHeight();
    onWidthChanged: updateCellWidth();

    Rectangle {
        id: monthHeader
        height: 20
        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            font.pixelSize: 25
            text: Qt.formatDate(currentDate, "MMMM yyyy")
        }
        anchors { left: parent.left; right: parent.right; top: parent.top; topMargin: 5 }
    }

    Row {
        id: header
        Repeater {
            model: [ "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday" ]
            Rectangle {
                width: grid.cellWidth
                height: 20
                Text {
                    text: modelData
                    color: "gray"
                    font.pixelSize: 10
                    anchors { left: parent.left; leftMargin: 10; bottom: parent.bottom; bottomMargin: 5 }
                }
            }
        }
        anchors { left: parent.left; top: monthHeader.bottom; topMargin: 10 }

    }

    GridView {
        id: grid
        objectName: "grid"
        model: calendarModel
        cellHeight: 110
        cellWidth: 150
        delegate: GridDelegate {
            objectName: "delegate"
            currentMonth: currentMonthNumber
            highlightCurrenytDay: root.highlightCurrentyDay
            onClicked: {
                root.clicked(d, row)
            }
        }
        anchors { left: parent.left; top: header.bottom; bottom: parent.bottom; right: header.right }
        height: 800

    }

    function updateCellHeight() {
        var minimumCellHeight = 50;
        var height = (root.height - header.height - monthHeader.height - 14) / 6;
        grid.cellHeight = height <= minimumCellHeight ? minimumCellHeight : height;
    }

    function updateCellWidth() {
        var minimumCellWidth = 90;
        var width = (root.width) / 7;
        grid.cellWidth = width <= minimumCellWidth ? minimumCellWidth : width;
    }

    function isoDateString(d){
       function pad(n){ return n < 10 ? '0' + n : n }
       return d.getUTCFullYear() + '-'
          + pad(d.getUTCMonth() + 1) + '-'
          + pad(d.getUTCDate()) + 'T'
          + pad(d.getUTCHours()) + ':'
          + pad(d.getUTCMinutes()) + ':'
          + pad(d.getUTCSeconds()) + 'Z'}

}


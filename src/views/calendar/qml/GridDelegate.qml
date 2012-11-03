// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1

Item {
    id: gridDelegate

    property int currentMonth
    property bool highlightCurrenytDay

    signal clicked(date d)

    function isToday() {
        var today = new Date();
        return today.getDate() === dayNumber && today.getMonth() === monthNumber - 1 &&
                today.getFullYear() === yearNumber;
    }


    Rectangle {
        id: gridRectangle
        width: grid.cellWidth
        height: grid.cellHeight
        border.color: "#002851"

        Rectangle {
            id: headerRect
            color: {
                var today = new Date();
                if (highlightCurrenytDay && isToday()) {
                    return "yellow";
                }
                else { return "#A3A3D1"; }
            }
            opacity: {
                var today = new Date();
                if (highlightCurrenytDay && isToday()) {
                    return 0.5;
                }
                else { return 0.6; }
            }
            anchors { left: parent.left; top: parent.top }
            width: parent.width
            height: 18
            Text {
                id: dayNumberText
                text: {
                    if (dayNumber === 1) { return Qt.formatDate(date, "d MMM") }
                    else { return Qt.formatDate(date, "d") }
                }
                color: {
                    if (monthNumber !== currentMonth) { return "gray" }
                    else { return "black"; }
                }

                font.bold: true
                anchors { top: parent.top; topMargin: 3; right: parent.right; rightMargin: 10 }
            }
        }

        Rectangle {
            id: contentRect
            color: {
                if (monthNumber !== currentMonth) { return "white" }
                else { return "#CBCBE4"; }
            }
            opacity: 0.5
            anchors { left: parent.left; top: headerRect.bottom; bottom: parent.bottom }
            width: grid.cellWidth
            Column {
                Repeater {
                    model: items

                    Rectangle {
                        property color backgroundColor: modelData.backgroundColor


                        objectName: "calendarItemRect"

                        id: itemRect
                        color: backgroundColor
                        opacity: 0.9
                        border.color: itemRectArea.selected ? "#000000" : Qt.darker(color)
                        border.width: 2
                        radius: 4
                        width: grid.cellWidth - 10
                        height: 20
                        Text {
                            text: modelData.subject
                            color: modelData.foreGroundColor
                            anchors { left: parent.left; leftMargin: 5; top: parent.top; topMargin: 5; right: parent.right }
                            font.pixelSize: 11
                            elide: Text.ElideRight
                        }

                        MouseArea {
                            id: itemRectArea
                            anchors.fill: parent
                            hoverEnabled: true

                            property bool selected: false

                            onEntered: {
                                itemRect.color = Qt.lighter(backgroundColor)
                            }
                            onExited: {
                                itemRect.color = backgroundColor
                            }
                            onClicked: {
                                itemRectArea.selected = true
                                gridDelegate.clicked(modelData.date)
                            }
                        }
                    }
                }
                spacing: 4
                anchors { left: parent.left; leftMargin: 5; top: parent.top; topMargin: 5 }
            }
        }
    }
}

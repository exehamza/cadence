import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: heatmap

    property var cells: []
    property var monthLabels: []
    property string hoveredSummary: "Hover any day square to inspect study hours"

    readonly property var palette: [
        "#161C28", // Level 0: 0 min
        "#0E4429", // Level 1: 1–25%
        "#006D32", // Level 2: 26–50%
        "#26A641", // Level 3: 51–75%
        "#39D353"  // Level 4: 76–100%
    ]

    implicitHeight: mainCol.implicitHeight

    ColumnLayout {
        id: mainCol
        anchors.left: parent.left
        anchors.right: parent.right
        spacing: 8

        // Top Bar: Hover Detail Pill + 5-Level Intensity Legend
        RowLayout {
            Layout.fillWidth: true

            Label {
                text: heatmap.hoveredSummary
                color: "#CBD5E1"
                font.pixelSize: 12
                Layout.fillWidth: true
            }

            RowLayout {
                spacing: 4
                Label { text: "Less"; color: "#64748B"; font.pixelSize: 11 }

                Repeater {
                    model: heatmap.palette
                    delegate: Rectangle {
                        required property string modelData
                        width: 12
                        height: 12
                        radius: 2.5
                        color: modelData
                        border.color: "#232B3D"
                        border.width: 0.5
                    }
                }

                Label { text: "More"; color: "#64748B"; font.pixelSize: 11 }
            }
        }

        // Scrollable 53-Week Contribution Matrix
        Flickable {
            id: gridFlick
            Layout.fillWidth: true
            Layout.preferredHeight: matrixRow.implicitHeight + 4
            contentWidth: matrixRow.implicitWidth
            contentHeight: matrixRow.implicitHeight
            clip: true
            boundsBehavior: Flickable.StopAtBounds

            onContentWidthChanged: {
                if (contentWidth > width) {
                    contentX = contentWidth - width;
                }
            }

            RowLayout {
                id: matrixRow
                spacing: 8

                // Weekday Y-Axis Labels
                ColumnLayout {
                    Layout.topMargin: 20
                    spacing: 4

                    Repeater {
                        model: ["Mon", "", "Wed", "", "Fri", "", "Sun"]
                        delegate: Item {
                            required property string modelData
                            width: 28
                            height: 14

                            Label {
                                anchors.verticalCenter: parent.verticalCenter
                                text: modelData
                                color: "#64748B"
                                font.pixelSize: 10
                            }
                        }
                    }
                }

                // Month Headers + 53x7 Square Grid
                ColumnLayout {
                    spacing: 6

                    // Month Header Row
                    Row {
                        spacing: 4
                        Repeater {
                            model: heatmap.monthLabels
                            delegate: Item {
                                required property string modelData
                                width: 14
                                height: 14

                                Label {
                                    text: modelData
                                    color: "#94A3B8"
                                    font.pixelSize: 10
                                    font.weight: Font.Medium
                                }
                            }
                        }
                    }

                    // 53 Weeks x 7 Days (Column-Major via Grid.TopToBottom)
                    Grid {
                        rows: 7
                        flow: Grid.TopToBottom
                        rowSpacing: 4
                        columnSpacing: 4

                        Repeater {
                            model: heatmap.cells

                            delegate: Rectangle {
                                required property var modelData
                                width: 14
                                height: 14
                                radius: 3
                                opacity: modelData.isFuture ? 0.22 : 1.0
                                color: heatmap.palette[modelData.level]
                                border.color: modelData.isToday ? "#38BDF8" : "#222A3B"
                                border.width: modelData.isToday ? 1.5 : 0.6

                                MouseArea {
                                    anchors.fill: parent
                                    enabled: !modelData.isFuture
                                    hoverEnabled: true
                                    onEntered: {
                                        if (modelData.minutes <= 0) {
                                            heatmap.hoveredSummary = modelData.formattedDate + " — No focus sessions logged";
                                        } else {
                                            heatmap.hoveredSummary = modelData.formattedDate
                                                + " — " + modelData.hours.toFixed(2)
                                                + " hrs (" + modelData.minutes + " min)";
                                        }
                                    }
                                    onExited: {
                                        heatmap.hoveredSummary = "Hover any day square to inspect study hours";
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
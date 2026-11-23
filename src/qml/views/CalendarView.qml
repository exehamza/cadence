import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Cadence 1.0
import "../components"

Item {
    id: calendarView

    RowLayout {
        anchors.fill: parent
        anchors.margins: 24
        spacing: 20

        // =====================================================================
        // LEFT / CENTER: Monthly & Weekly Unified Grid
        // =====================================================================
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.preferredWidth: 72
            radius: 16
            color: "#141822"
            border.color: "#232A3B"
            border.width: 1

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 20
                spacing: 14

                // Calendar Navigation Header
                RowLayout {
                    Layout.fillWidth: true
                    spacing: 10

                    Button {
                        text: "‹"
                        implicitWidth: 34
                        implicitHeight: 34
                        onClicked: CalendarVM.previousPeriod()
                    }

                    Button {
                        text: "Today"
                        implicitHeight: 34
                        onClicked: CalendarVM.goToToday()
                    }

                    Button {
                        text: "›"
                        implicitWidth: 34
                        implicitHeight: 34
                        onClicked: CalendarVM.nextPeriod()
                    }

                    Label {
                        text: CalendarVM.headerTitle
                        color: "#F8FAFC"
                        font.pixelSize: 20
                        font.weight: Font.DemiBold
                        leftPadding: 8
                        Layout.fillWidth: true
                    }

                    // Month / Week Switcher
                    Rectangle {
                        implicitWidth: viewSwitchRow.implicitWidth + 8
                        implicitHeight: 36
                        radius: 18
                        color: "#0F121A"
                        border.color: "#252D40"

                        RowLayout {
                            id: viewSwitchRow
                            anchors.centerIn: parent
                            spacing: 4

                            Repeater {
                                model: [
                                    { label: "Month", modeVal: CalendarVM.MonthView },
                                    { label: "Week",  modeVal: CalendarVM.WeekView }
                                ]
                                delegate: Rectangle {
                                    required property var modelData
                                    readonly property bool active: CalendarVM.viewMode === modelData.modeVal
                                    implicitWidth: modeLbl.implicitWidth + 22
                                    implicitHeight: 28
                                    radius: 14
                                    color: active ? "#4F46E5" : "transparent"

                                    Label {
                                        id: modeLbl
                                        anchors.centerIn: parent
                                        text: modelData.label
                                        color: active ? "#FFFFFF" : "#94A3B8"
                                        font.pixelSize: 12
                                        font.weight: Font.DemiBold
                                    }

                                    MouseArea {
                                        anchors.fill: parent
                                        cursorShape: Qt.PointingHandCursor
                                        onClicked: CalendarVM.viewMode = modelData.modeVal
                                    }
                                }
                            }
                        }
                    }
                }

                // Weekday Column Headers
                GridLayout {
                    Layout.fillWidth: true
                    columns: 7
                    columnSpacing: 6

                    Repeater {
                        model: ["Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"]
                        delegate: Label {
                            required property string modelData
                            Layout.fillWidth: true
                            text: modelData
                            color: "#64748B"
                            font.pixelSize: 12
                            font.weight: Font.DemiBold
                            horizontalAlignment: Text.AlignHCenter
                        }
                    }
                }

                // Calendar Cells Grid (42 cells in MonthView, 7 tall columns in WeekView)
                GridLayout {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    columns: 7
                    columnSpacing: 6
                    rowSpacing: 6

                    Repeater {
                        model: CalendarVM.dayCells

                        delegate: Rectangle {
                            required property var modelData
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            radius: 10
                            color: modelData.isSelected
                                   ? "#1E2638"
                                   : (modelData.isCurrentMonth || CalendarVM.viewMode === CalendarVM.WeekView
                                      ? "#10141E"
                                      : "#0C0F16")
                            border.color: modelData.isSelected
                                          ? "#6366F1"
                                          : (modelData.isToday ? "#38BDF8" : "#1E2536")
                            border.width: modelData.isSelected || modelData.isToday ? 1.5 : 1

                            MouseArea {
                                anchors.fill: parent
                                cursorShape: Qt.PointingHandCursor
                                onClicked: CalendarVM.selectedDateIso = modelData.dateIso
                            }

                            ColumnLayout {
                                anchors.fill: parent
                                anchors.margins: 7
                                spacing: 4

                                RowLayout {
                                    Layout.fillWidth: true

                                    Rectangle {
                                        width: 24
                                        height: 24
                                        radius: 12
                                        color: modelData.isToday ? "#38BDF8" : "transparent"

                                        Label {
                                            anchors.centerIn: parent
                                            text: modelData.dayNumber
                                            color: modelData.isToday
                                                   ? "#090D16"
                                                   : (modelData.isCurrentMonth || CalendarVM.viewMode === CalendarVM.WeekView
                                                      ? "#F8FAFC"
                                                      : "#475569")
                                            font.pixelSize: 12
                                            font.weight: modelData.isToday ? Font.Bold : Font.Medium
                                        }
                                    }

                                    Item { Layout.fillWidth: true }

                                    Label {
                                        visible: modelData.items.length > 0
                                        text: modelData.items.length
                                        color: "#64748B"
                                        font.pixelSize: 10
                                    }
                                }

                                // Unified Color-Coded Pills
                                Repeater {
                                    model: {
                                        const maxVisible = CalendarVM.viewMode === CalendarVM.WeekView ? 12 : 3;
                                        return modelData.items.slice(0, maxVisible);
                                    }

                                    delegate: Rectangle {
                                        required property var modelData
                                        Layout.fillWidth: true
                                        height: CalendarVM.viewMode === CalendarVM.WeekView ? 34 : 19
                                        radius: 5
                                        color: Qt.rgba(Qt.color(modelData.colorHex).r,
                                                       Qt.color(modelData.colorHex).g,
                                                       Qt.color(modelData.colorHex).b,
                                                       0.22)
                                        border.color: modelData.colorHex
                                        border.width: 1

                                        RowLayout {
                                            anchors.fill: parent
                                            anchors.leftMargin: 6
                                            anchors.rightMargin: 5
                                            spacing: 4

                                            Label {
                                                text: modelData.title
                                                color: "#F8FAFC"
                                                font.pixelSize: 10
                                                font.weight: Font.Medium
                                                font.strikeout: modelData.isCompleted
                                                elide: Text.ElideRight
                                                Layout.fillWidth: true
                                            }
                                        }
                                    }
                                }

                                Label {
                                    visible: CalendarVM.viewMode === CalendarVM.MonthView && modelData.items.length > 3
                                    text: "+" + (modelData.items.length - 3) + " more"
                                    color: "#94A3B8"
                                    font.pixelSize: 10
                                }

                                Item { Layout.fillHeight: true }
                            }
                        }
                    }
                }
            }
        }

        // =====================================================================
        // RIGHT PANE: Selected Day Agenda Inspector
        // =====================================================================
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.preferredWidth: 28
            radius: 16
            color: "#141822"
            border.color: "#232A3B"
            border.width: 1

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 20
                spacing: 14

                RowLayout {
                    Layout.fillWidth: true
                    ColumnLayout {
                        Layout.fillWidth: true
                        spacing: 2

                        Label {
                            text: "Day Agenda"
                            color: "#94A3B8"
                            font.pixelSize: 12
                            font.weight: Font.DemiBold
                        }
                        Label {
                            text: CalendarVM.selectedDateFormatted
                            color: "#F8FAFC"
                            font.pixelSize: 16
                            font.weight: Font.DemiBold
                            elide: Text.ElideRight
                            Layout.fillWidth: true
                        }
                    }

                    Button {
                        text: "+ Event"
                        onClicked: {
                            addEventModal.defaultDateIso = CalendarVM.selectedDateIso;
                            addEventModal.open();
                        }
                    }
                }

                Rectangle { Layout.fillWidth: true; height: 1; color: "#232A3B" }

                ListView {
                    id: agendaList
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    clip: true
                    spacing: 10
                    model: CalendarVM.selectedDayItems

                    Label {
                        anchors.centerIn: parent
                        visible: agendaList.count === 0
                        text: "Nothing scheduled for this day."
                        color: "#64748B"
                        font.pixelSize: 13
                    }

                    delegate: Rectangle {
                        required property var modelData
                        width: agendaList.width
                        height: 68
                        radius: 10
                        color: "#181E2C"
                        border.color: "#263044"

                        RowLayout {
                            anchors.fill: parent
                            anchors.leftMargin: 12
                            anchors.rightMargin: 10
                            spacing: 10

                            Rectangle {
                                width: 4
                                height: 42
                                radius: 2
                                color: modelData.colorHex
                            }

                            ColumnLayout {
                                Layout.fillWidth: true
                                spacing: 3

                                Label {
                                    text: modelData.title
                                    color: modelData.isCompleted ? "#64748B" : "#F8FAFC"
                                    font.pixelSize: 14
                                    font.weight: Font.Medium
                                    font.strikeout: modelData.isCompleted
                                    elide: Text.ElideRight
                                    Layout.fillWidth: true
                                }

                                Label {
                                    text: modelData.categoryLabel + " • " + modelData.timeLabel
                                          + (modelData.subtitle.length > 0 ? " • " + modelData.subtitle : "")
                                    color: "#94A3B8"
                                    font.pixelSize: 11
                                    elide: Text.ElideRight
                                    Layout.fillWidth: true
                                }
                            }

                            ToolButton {
                                visible: !modelData.isReminder
                                text: "✕"
                                onClicked: CalendarVM.removeStandaloneEvent(modelData.itemId)
                                contentItem: Label {
                                    text: "✕"
                                    color: "#F87171"
                                    horizontalAlignment: Text.AlignHCenter
                                    verticalAlignment: Text.AlignVCenter
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    AddEventDialog {
        id: addEventModal
    }
}
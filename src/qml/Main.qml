import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Cadence 1.0
import "views"
import "components"

ApplicationWindow {
    id: root
    width: 1280
    height: 820
    minimumWidth: 980
    minimumHeight: 660
    visible: true
    title: "Cadence — Study & Productivity"
    color: "#0F1117"

    property int activeTab: 0
    readonly property bool isWindowFullscreen: root.visibility === Window.FullScreen

    function toggleFullscreen() {
        if (root.visibility === Window.FullScreen) {
            root.showNormal();
        } else {
            root.showFullScreen();
        }
    }

    Shortcut {
        sequences: ["F11"]
        onActivated: root.toggleFullscreen()
    }

    Shortcut {
        sequences: ["Escape"]
        enabled: root.isWindowFullscreen
        onActivated: root.showNormal()
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        // Top Navigation Bar (auto-hides during distraction-free Fullscreen Focus)
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 56
            visible: !root.isWindowFullscreen
            color: "#0B0E14"
            border.color: "#1E2536"
            border.width: 1

            RowLayout {
                anchors.fill: parent
                anchors.leftMargin: 24
                anchors.rightMargin: 24
                spacing: 14

                Label {
                    text: "CADENCE"
                    color: "#F8FAFC"
                    font.pixelSize: 15
                    font.weight: Font.Bold
                    font.letterSpacing: 1.8
                }

                Item { Layout.preferredWidth: 12 }

                // Workspace Switcher
                Repeater {
                    model: [
                        { label: "Focus Timer", tabIdx: 0 },
                        { label: "Planner",     tabIdx: 1 },
                        { label: "Calendar",    tabIdx: 2 },
                        { label: "Analytics",   tabIdx: 3 }
                    ]
                    delegate: Rectangle {
                        required property var modelData
                        readonly property bool selected: root.activeTab === modelData.tabIdx
                        implicitWidth: navText.implicitWidth + 28
                        implicitHeight: 34
                        radius: 8
                        color: selected ? "#1E2638" : "transparent"
                        border.color: selected ? "#334155" : "transparent"

                        Label {
                            id: navText
                            anchors.centerIn: parent
                            text: modelData.label
                            color: selected ? "#FFFFFF" : "#94A3B8"
                            font.pixelSize: 13
                            font.weight: selected ? Font.DemiBold : Font.Normal
                        }

                        MouseArea {
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            onClicked: root.activeTab = modelData.tabIdx
                        }
                    }
                }

                Item { Layout.fillWidth: true }

                // Live Background Timer Pill when browsing Planner/Calendar/Analytics
                Rectangle {
                    visible: root.activeTab !== 0 && (FocusTimer.isRunning || FocusTimer.isPaused)
                    implicitWidth: livePillRow.implicitWidth + 20
                    implicitHeight: 32
                    radius: 16
                    color: "#182234"
                    border.color: FocusTimer.activeSubjectColor

                    RowLayout {
                        id: livePillRow
                        anchors.centerIn: parent
                        spacing: 8

                        Rectangle {
                            width: 8
                            height: 8
                            radius: 4
                            color: FocusTimer.isRunning ? "#10B981" : "#F59E0B"
                        }

                        Label {
                            text: FocusTimer.formattedTime + " • " + FocusTimer.activeSubjectName
                            color: "#F8FAFC"
                            font.pixelSize: 12
                            font.weight: Font.DemiBold
                        }
                    }

                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: root.activeTab = 0
                    }
                }

                Button {
                    text: "Subjects (" + SubjectModel.count + ")"
                    onClicked: subjectManagerModal.open()
                }
            }
        }

        // Persistent Workspace Stack
        StackLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            currentIndex: root.activeTab

            TimerView {
                isFullscreen: root.isWindowFullscreen
                onToggleFullscreenRequested: root.toggleFullscreen()
            }

            PlannerView {}

            CalendarView {}

            AnalyticsView {}
        }
    }

    SubjectManagerDialog {
        id: subjectManagerModal
    }
}
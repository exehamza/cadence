import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Cadence 1.0
import "../components"

Item {
    id: analyticsView

    Flickable {
        anchors.fill: parent
        anchors.margins: 24
        contentHeight: dashboardCol.implicitHeight
        clip: true

        ColumnLayout {
            id: dashboardCol
            width: parent.width
            spacing: 20

            // =================================================================
            // ROW 1: Top KPI Summary Cards
            // =================================================================
            RowLayout {
                Layout.fillWidth: true
                spacing: 16

                Repeater {
                    model: [
                        {
                            title: "FOCUS TIME (SELECTED RANGE)",
                            value: AnalyticsVM.totalHoursInRange.toFixed(1) + " hrs",
                            accent: "#6366F1"
                        },
                        {
                            title: "CURRENT STUDY STREAK",
                            value: AnalyticsVM.currentStreakDays + (AnalyticsVM.currentStreakDays === 1 ? " day" : " days"),
                            accent: "#10B981"
                        },
                        {
                            title: "LONGEST STREAK (1Y)",
                            value: AnalyticsVM.longestStreakDays + (AnalyticsVM.longestStreakDays === 1 ? " day" : " days"),
                            accent: "#38BDF8"
                        },
                        {
                            title: "ACTIVE STUDY DAYS (1Y)",
                            value: AnalyticsVM.activeDaysLastYear + " days",
                            accent: "#F59E0B"
                        }
                    ]

                    delegate: Rectangle {
                        required property var modelData
                        Layout.fillWidth: true
                        height: 84
                        radius: 14
                        color: "#141822"
                        border.color: "#232A3B"

                        ColumnLayout {
                            anchors.fill: parent
                            anchors.margins: 16
                            spacing: 6

                            RowLayout {
                                spacing: 6
                                Rectangle {
                                    width: 8
                                    height: 8
                                    radius: 4
                                    color: modelData.accent
                                }
                                Label {
                                    text: modelData.title
                                    color: "#94A3B8"
                                    font.pixelSize: 11
                                    font.weight: Font.Bold
                                    font.letterSpacing: 0.7
                                }
                            }

                            Label {
                                text: modelData.value
                                color: "#F8FAFC"
                                font.pixelSize: 24
                                font.weight: Font.DemiBold
                                font.features: { "tnum": 1 }
                            }
                        }
                    }
                }
            }

            // =================================================================
            // ROW 2: Subject Time Distribution (Left) + Exam Countdown (Right)
            // =================================================================
            RowLayout {
                Layout.fillWidth: true
                spacing: 20

                // Subject Distribution Card
                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredWidth: 60
                    height: 330
                    radius: 16
                    color: "#141822"
                    border.color: "#232A3B"

                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: 20
                        spacing: 14

                        RowLayout {
                            Layout.fillWidth: true

                            Label {
                                text: "Subject Time Distribution"
                                color: "#F8FAFC"
                                font.pixelSize: 18
                                font.weight: Font.DemiBold
                                Layout.fillWidth: true
                            }

                            // Timeframe Selector Pills
                            Rectangle {
                                implicitWidth: tfRow.implicitWidth + 8
                                implicitHeight: 32
                                radius: 16
                                color: "#0F121A"
                                border.color: "#252D40"

                                RowLayout {
                                    id: tfRow
                                    anchors.centerIn: parent
                                    spacing: 4

                                    Repeater {
                                        model: [
                                            { label: "All Time", idx: 0 },
                                            { label: "30 Days",  idx: 1 },
                                            { label: "7 Days",   idx: 2 }
                                        ]
                                        delegate: Rectangle {
                                            required property var modelData
                                            readonly property bool selected: AnalyticsVM.timeframeIndex === modelData.idx
                                            implicitWidth: tfLbl.implicitWidth + 18
                                            implicitHeight: 24
                                            radius: 12
                                            color: selected ? "#4F46E5" : "transparent"

                                            Label {
                                                id: tfLbl
                                                anchors.centerIn: parent
                                                text: modelData.label
                                                color: selected ? "#FFFFFF" : "#94A3B8"
                                                font.pixelSize: 11
                                                font.weight: Font.DemiBold
                                            }

                                            MouseArea {
                                                anchors.fill: parent
                                                cursorShape: Qt.PointingHandCursor
                                                onClicked: AnalyticsVM.timeframeIndex = modelData.idx
                                            }
                                        }
                                    }
                                }
                            }
                        }

                        SubjectDonutChart {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            distributionModel: AnalyticsVM.subjectDistribution
                            totalHours: AnalyticsVM.totalHoursInRange
                        }
                    }
                }

                // Live Exam Countdown Widget Card
                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredWidth: 40
                    height: 330
                    radius: 16
                    color: "#141822"
                    border.color: "#232A3B"

                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: 20
                        spacing: 14

                        RowLayout {
                            Layout.fillWidth: true
                            Label {
                                text: "Exam Countdown"
                                color: "#F8FAFC"
                                font.pixelSize: 18
                                font.weight: Font.DemiBold
                                Layout.fillWidth: true
                            }
                            Label {
                                text: AnalyticsVM.upcomingExams.length + " upcoming"
                                color: "#94A3B8"
                                font.pixelSize: 12
                            }
                        }

                        ListView {
                            id: examList
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            clip: true
                            spacing: 10
                            model: AnalyticsVM.upcomingExams

                            Label {
                                anchors.centerIn: parent
                                visible: examList.count === 0
                                text: "No upcoming examinations.\nAdd an Examination reminder in Planner to track its countdown."
                                color: "#64748B"
                                font.pixelSize: 13
                                wrapMode: Text.WordWrap
                                width: parent.width - 24
                                horizontalAlignment: Text.AlignHCenter
                            }

                            delegate: ExamCountdownCard {
                                required property var modelData
                                width: examList.width
                                examData: modelData
                            }
                        }
                    }
                }
            }

            // =================================================================
            // ROW 3: Annual GitHub-Style Activity Heatmap
            // =================================================================
            Rectangle {
                Layout.fillWidth: true
                implicitHeight: heatmapCol.implicitHeight + 40
                radius: 16
                color: "#141822"
                border.color: "#232A3B"

                ColumnLayout {
                    id: heatmapCol
                    anchors.fill: parent
                    anchors.margins: 20
                    spacing: 12

                    Label {
                        text: "Annual Study Consistency"
                        color: "#F8FAFC"
                        font.pixelSize: 18
                        font.weight: Font.DemiBold
                    }

                    ActivityHeatmap {
                        Layout.fillWidth: true
                        cells: AnalyticsVM.heatmapCells
                        monthLabels: AnalyticsVM.heatmapMonthLabels
                    }
                }
            }
        }
    }
}
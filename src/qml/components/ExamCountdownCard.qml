import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: card

    required property var examData

    readonly property color accent: examData.isOverdue
        ? "#EF4444"
        : (examData.isUrgent ? "#F59E0B" : examData.colorHex)

    height: 86
    radius: 12
    color: "#10141E"
    border.color: examData.isUrgent || examData.isOverdue ? accent : "#242D40"
    border.width: examData.isUrgent || examData.isOverdue ? 1.5 : 1

    RowLayout {
        anchors.fill: parent
        anchors.leftMargin: 14
        anchors.rightMargin: 14
        spacing: 12

        // Left Subject Color Bar
        Rectangle {
            width: 4
            height: 54
            radius: 2
            color: card.accent
        }

        // Exam Title & Date Metadata
        ColumnLayout {
            Layout.fillWidth: true
            spacing: 4

            RowLayout {
                spacing: 6
                Rectangle {
                    implicitWidth: subLbl.implicitWidth + 12
                    implicitHeight: 18
                    radius: 4
                    color: Qt.rgba(Qt.color(card.accent).r, Qt.color(card.accent).g, Qt.color(card.accent).b, 0.20)

                    Label {
                        id: subLbl
                        anchors.centerIn: parent
                        text: card.examData.subjectName
                        color: card.accent
                        font.pixelSize: 10
                        font.weight: Font.Bold
                    }
                }
            }

            Label {
                text: card.examData.title
                color: "#F8FAFC"
                font.pixelSize: 14
                font.weight: Font.DemiBold
                elide: Text.ElideRight
                Layout.fillWidth: true
            }

            Label {
                text: card.examData.dueDateFormatted
                color: "#94A3B8"
                font.pixelSize: 11
            }
        }

        // Live Days / Hours / Minutes Countdown Pill
        Rectangle {
            implicitWidth: countdownRow.implicitWidth + 20
            implicitHeight: 52
            radius: 10
            color: Qt.rgba(Qt.color(card.accent).r, Qt.color(card.accent).g, Qt.color(card.accent).b, 0.14)
            border.color: Qt.rgba(Qt.color(card.accent).r, Qt.color(card.accent).g, Qt.color(card.accent).b, 0.45)

            RowLayout {
                id: countdownRow
                anchors.centerIn: parent
                spacing: 10

                Label {
                    visible: card.examData.isOverdue
                    text: "DUE NOW"
                    color: "#FCA5A5"
                    font.pixelSize: 12
                    font.weight: Font.Bold
                }

                // Days Block
                ColumnLayout {
                    visible: !card.examData.isOverdue
                    spacing: 0
                    Label {
                        Layout.alignment: Qt.AlignHCenter
                        text: card.examData.daysLeft
                        color: "#FFFFFF"
                        font.pixelSize: 18
                        font.weight: Font.Bold
                        font.features: { "tnum": 1 }
                    }
                    Label {
                        Layout.alignment: Qt.AlignHCenter
                        text: "DAYS"
                        color: "#94A3B8"
                        font.pixelSize: 9
                        font.weight: Font.Bold
                    }
                }

                Label {
                    visible: !card.examData.isOverdue
                    text: ":"
                    color: "#64748B"
                    font.pixelSize: 16
                    font.weight: Font.Bold
                }

                // Hours Block
                ColumnLayout {
                    visible: !card.examData.isOverdue
                    spacing: 0
                    Label {
                        Layout.alignment: Qt.AlignHCenter
                        text: String(card.examData.hoursLeft).padStart(2, "0")
                        color: "#FFFFFF"
                        font.pixelSize: 18
                        font.weight: Font.Bold
                        font.features: { "tnum": 1 }
                    }
                    Label {
                        Layout.alignment: Qt.AlignHCenter
                        text: "HRS"
                        color: "#94A3B8"
                        font.pixelSize: 9
                        font.weight: Font.Bold
                    }
                }

                Label {
                    visible: !card.examData.isOverdue
                    text: ":"
                    color: "#64748B"
                    font.pixelSize: 16
                    font.weight: Font.Bold
                }

                // Minutes Block
                ColumnLayout {
                    visible: !card.examData.isOverdue
                    spacing: 0
                    Label {
                        Layout.alignment: Qt.AlignHCenter
                        text: String(card.examData.minutesLeft).padStart(2, "0")
                        color: "#FFFFFF"
                        font.pixelSize: 18
                        font.weight: Font.Bold
                        font.features: { "tnum": 1 }
                    }
                    Label {
                        Layout.alignment: Qt.AlignHCenter
                        text: "MIN"
                        color: "#94A3B8"
                        font.pixelSize: 9
                        font.weight: Font.Bold
                    }
                }
            }
        }
    }
}
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: gauge

    property real progress: 0.0
    property string timeText: "25:00"
    property string phaseLabel: "FOCUS"
    property string modeLabel: "POMODORO"
    property string subjectName: "Unassigned"
    property color accentColor: "#6366F1"

    property real animatedProgress: progress
    Behavior on animatedProgress {
        NumberAnimation {
            duration: 220
            easing.type: Easing.OutCubic
        }
    }

    onAnimatedProgressChanged: ringCanvas.requestPaint()
    onAccentColorChanged: ringCanvas.requestPaint()
    onWidthChanged: ringCanvas.requestPaint()

    implicitWidth: 330
    implicitHeight: 330

    Canvas {
        id: ringCanvas
        anchors.fill: parent
        antialiasing: true
        renderStrategy: Canvas.Threaded

        onPaint: {
            const ctx = getContext("2d");
            ctx.reset();

            const cx = width / 2;
            const cy = height / 2;
            const lineWidth = 10;
            const radius = Math.min(cx, cy) - lineWidth - 6;
            const startAngle = -Math.PI / 2;
            const endAngle = startAngle + (Math.PI * 2 * Math.max(0, Math.min(1, gauge.animatedProgress)));

            // Subtle outer track
            ctx.beginPath();
            ctx.arc(cx, cy, radius, 0, Math.PI * 2, false);
            ctx.strokeStyle = Qt.rgba(1, 1, 1, 0.12);
            ctx.lineWidth = lineWidth;
            ctx.stroke();

            // Active progress arc
            if (gauge.animatedProgress > 0.001) {
                ctx.beginPath();
                ctx.arc(cx, cy, radius, startAngle, endAngle, false);
                ctx.strokeStyle = gauge.accentColor;
                ctx.lineWidth = lineWidth;
                ctx.lineCap = "round";
                ctx.stroke();
            }
        }
    }

    ColumnLayout {
        anchors.centerIn: parent
        spacing: 8

        Rectangle {
            Layout.alignment: Qt.AlignHCenter
            implicitWidth: phaseRow.implicitWidth + 20
            implicitHeight: 26
            radius: 13
            color: Qt.rgba(gauge.accentColor.r, gauge.accentColor.g, gauge.accentColor.b, 0.18)
            border.color: Qt.rgba(gauge.accentColor.r, gauge.accentColor.g, gauge.accentColor.b, 0.5)
            border.width: 1

            RowLayout {
                id: phaseRow
                anchors.centerIn: parent
                spacing: 6

                Rectangle {
                    width: 7
                    height: 7
                    radius: 3.5
                    color: gauge.accentColor
                }

                Label {
                    text: gauge.phaseLabel + " • " + gauge.modeLabel
                    color: "#F8FAFC"
                    font.pixelSize: 11
                    font.weight: Font.Bold
                    font.letterSpacing: 1.1
                }
            }
        }

        Label {
            Layout.alignment: Qt.AlignHCenter
            text: gauge.timeText
            color: "#FFFFFF"
            font.pixelSize: 64
            font.weight: Font.Light
            font.features: { "tnum": 1 }
        }

        Label {
            Layout.alignment: Qt.AlignHCenter
            text: gauge.subjectName
            color: "#CBD5E1"
            font.pixelSize: 14
            font.weight: Font.Medium
            elide: Text.ElideRight
            maximumLineCount: 1
        }
    }
}
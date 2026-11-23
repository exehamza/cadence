import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: root

    property var distributionModel: []
    property real totalHours: 0.0
    property int hoveredIndex: -1

    onDistributionModelChanged: donutCanvas.requestPaint()
    onHoveredIndexChanged: donutCanvas.requestPaint()
    onWidthChanged: donutCanvas.requestPaint()

    RowLayout {
        anchors.fill: parent
        spacing: 24

        // Left: Hardware-Accelerated Canvas Donut Ring
        Item {
            Layout.preferredWidth: 230
            Layout.preferredHeight: 230
            Layout.alignment: Qt.AlignVCenter

            Canvas {
                id: donutCanvas
                anchors.fill: parent
                antialiasing: true
                renderStrategy: Canvas.Threaded

                onPaint: {
                    const ctx = getContext("2d");
                    ctx.reset();

                    const cx = width / 2;
                    const cy = height / 2;
                    const baseThickness = 24;
                    const radius = Math.min(cx, cy) - baseThickness - 6;

                    // Base ring track
                    ctx.beginPath();
                    ctx.arc(cx, cy, radius, 0, Math.PI * 2, false);
                    ctx.strokeStyle = "#1E2536";
                    ctx.lineWidth = baseThickness;
                    ctx.stroke();

                    const items = root.distributionModel;
                    if (!items || items.length === 0 || root.totalHours <= 0.0001) {
                        return;
                    }

                    let activeSlices = 0;
                    for (let i = 0; i < items.length; ++i) {
                        if (items[i].hours > 0) activeSlices++;
                    }

                    const gapAngle = activeSlices > 1 ? 0.025 : 0.0;
                    let startAngle = -Math.PI / 2;

                    for (let i = 0; i < items.length; ++i) {
                        const share = items[i].percentage / 100.0;
                        if (share <= 0.0001) continue;

                        const sweep = Math.max(0.01, (Math.PI * 2 * share) - gapAngle);
                        const endAngle = startAngle + sweep;
                        const isHovered = (root.hoveredIndex === i);

                        ctx.beginPath();
                        ctx.arc(cx, cy, radius, startAngle, endAngle, false);
                        ctx.strokeStyle = items[i].colorHex;
                        ctx.lineWidth = isHovered ? baseThickness + 5 : baseThickness;
                        ctx.lineCap = "butt";
                        ctx.stroke();

                        startAngle += (Math.PI * 2 * share);
                    }
                }
            }

            // Center Readout
            ColumnLayout {
                anchors.centerIn: parent
                spacing: 2

                Label {
                    Layout.alignment: Qt.AlignHCenter
                    text: {
                        if (root.hoveredIndex >= 0 && root.hoveredIndex < root.distributionModel.length) {
                            return root.distributionModel[root.hoveredIndex].hours.toFixed(1) + "h";
                        }
                        return root.totalHours.toFixed(1) + "h";
                    }
                    color: "#F8FAFC"
                    font.pixelSize: 28
                    font.weight: Font.DemiBold
                    font.features: { "tnum": 1 }
                }

                Label {
                    Layout.alignment: Qt.AlignHCenter
                    Layout.maximumWidth: 130
                    text: {
                        if (root.hoveredIndex >= 0 && root.hoveredIndex < root.distributionModel.length) {
                            return root.distributionModel[root.hoveredIndex].name;
                        }
                        return "TOTAL FOCUS";
                    }
                    color: "#94A3B8"
                    font.pixelSize: 11
                    font.weight: Font.Bold
                    font.letterSpacing: 0.8
                    elide: Text.ElideRight
                }
            }
        }

        // Right: Interactive Color-Coded Subject Breakdown Bars
        ListView {
            id: legendList
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            spacing: 10
            model: root.distributionModel

            Label {
                anchors.centerIn: parent
                visible: legendList.count === 0
                text: "No subjects created yet. Add a subject and log a focus session to see analytics."
                color: "#64748B"
                font.pixelSize: 13
                wrapMode: Text.WordWrap
                width: parent.width - 24
                horizontalAlignment: Text.AlignHCenter
            }

            delegate: Rectangle {
                required property var modelData
                required property int index
                width: legendList.width
                height: 46
                radius: 9
                color: root.hoveredIndex === index ? "#1C2333" : "#10141E"
                border.color: root.hoveredIndex === index ? modelData.colorHex : "#1D2434"

                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    onEntered: root.hoveredIndex = index
                    onExited: if (root.hoveredIndex === index) root.hoveredIndex = -1
                }

                ColumnLayout {
                    anchors.fill: parent
                    anchors.leftMargin: 12
                    anchors.rightMargin: 12
                    anchors.topMargin: 7
                    anchors.bottomMargin: 7
                    spacing: 5

                    RowLayout {
                        Layout.fillWidth: true
                        spacing: 8

                        Rectangle {
                            width: 10
                            height: 10
                            radius: 5
                            color: modelData.colorHex
                        }

                        Label {
                            text: modelData.name
                            color: "#F8FAFC"
                            font.pixelSize: 13
                            font.weight: Font.Medium
                            elide: Text.ElideRight
                            Layout.fillWidth: true
                        }

                        Label {
                            text: modelData.hours.toFixed(1) + " hrs (" + Math.round(modelData.percentage) + "%)"
                            color: "#CBD5E1"
                            font.pixelSize: 12
                            font.features: { "tnum": 1 }
                        }
                    }

                    // Proportional Progress Bar
                    Rectangle {
                        Layout.fillWidth: true
                        height: 5
                        radius: 2.5
                        color: "#1B2232"

                        Rectangle {
                            width: parent.width * Math.min(1.0, modelData.percentage / 100.0)
                            height: parent.height
                            radius: 2.5
                            color: modelData.colorHex
                        }
                    }
                }
            }
        }
    }
}
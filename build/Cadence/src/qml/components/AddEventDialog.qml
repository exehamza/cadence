import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Cadence 1.0

Popup {
    id: eventDialog
    modal: true
    focus: true
    anchors.centerIn: Overlay.overlay
    width: Math.min(440, parent ? parent.width - 48 : 440)
    padding: 22

    property string defaultDateIso: ""
    property string selectedColor: "#6366F1"
    readonly property var colorSwatches: [
        "#6366F1", "#EC4899", "#10B981", "#F59E0B", "#06B6D4", "#8B5CF6"
    ]

    onOpened: {
        titleField.clear();
        locationField.clear();
        dateField.text = defaultDateIso;
        startTimeField.text = "09:00";
        endTimeField.text = "10:00";
        allDayCheck.checked = false;
        subjectCombo.currentIndex = 0;
        titleField.forceActiveFocus();
    }

    background: Rectangle {
        color: "#161B26"
        radius: 16
        border.color: "#2A3244"
        border.width: 1
    }

    contentItem: ColumnLayout {
        spacing: 14

        RowLayout {
            Layout.fillWidth: true
            Label {
                text: "New Calendar Event"
                color: "#F8FAFC"
                font.pixelSize: 18
                font.weight: Font.DemiBold
                Layout.fillWidth: true
            }
            ToolButton {
                text: "✕"
                onClicked: eventDialog.close()
                contentItem: Label {
                    text: "✕"
                    color: "#94A3B8"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }
        }

        TextField {
            id: titleField
            Layout.fillWidth: true
            placeholderText: "Event title (e.g., CS Quiz, Party Day, Study Group)"
            color: "#F8FAFC"
            placeholderTextColor: "#64748B"
            background: Rectangle {
                radius: 8
                color: "#0F1117"
                border.color: titleField.activeFocus ? "#6366F1" : "#2A3244"
            }
        }

        TextField {
            id: locationField
            Layout.fillWidth: true
            placeholderText: "Location or notes (optional)"
            color: "#F8FAFC"
            placeholderTextColor: "#64748B"
            background: Rectangle {
                radius: 8
                color: "#0F1117"
                border.color: "#2A3244"
            }
        }

        RowLayout {
            Layout.fillWidth: true
            spacing: 10

            TextField {
                id: dateField
                Layout.preferredWidth: 130
                placeholderText: "YYYY-MM-DD"
                color: "#F8FAFC"
                background: Rectangle { radius: 8; color: "#0F1117"; border.color: "#2A3244" }
            }

            CheckBox {
                id: allDayCheck
                text: "All Day"
                contentItem: Label {
                    text: allDayCheck.text
                    color: "#CBD5E1"
                    font.pixelSize: 13
                    leftPadding: allDayCheck.indicator.width + 6
                    verticalAlignment: Text.AlignVCenter
                }
            }
        }

        RowLayout {
            Layout.fillWidth: true
            visible: !allDayCheck.checked
            spacing: 10

            Label { text: "From:"; color: "#94A3B8"; font.pixelSize: 13 }
            TextField {
                id: startTimeField
                Layout.preferredWidth: 80
                placeholderText: "09:00"
                color: "#F8FAFC"
                background: Rectangle { radius: 8; color: "#0F1117"; border.color: "#2A3244" }
            }

            Label { text: "To:"; color: "#94A3B8"; font.pixelSize: 13 }
            TextField {
                id: endTimeField
                Layout.preferredWidth: 80
                placeholderText: "10:00"
                color: "#F8FAFC"
                background: Rectangle { radius: 8; color: "#0F1117"; border.color: "#2A3244" }
            }
        }

        // Optional Subject Link
        RowLayout {
            Layout.fillWidth: true
            spacing: 10

            Label {
                text: "Subject:"
                color: "#94A3B8"
                font.pixelSize: 13
            }

            ComboBox {
                id: subjectCombo
                Layout.fillWidth: true
                model: {
                    let list = [{ id: -1, name: "None (Custom Color)" }];
                    for (let i = 0; i < SubjectModel.rowCount(); ++i) {
                        const idx = SubjectModel.index(i, 0);
                        list.push({
                            id: SubjectModel.data(idx, SubjectModel.IdRole),
                            name: SubjectModel.data(idx, SubjectModel.NameRole)
                        });
                    }
                    return list;
                }
                textRole: "name"
            }
        }

        // Custom color swatches when no Subject is linked
        RowLayout {
            visible: subjectCombo.currentIndex === 0
            spacing: 10
            Label { text: "Color:"; color: "#94A3B8"; font.pixelSize: 13 }
            Repeater {
                model: eventDialog.colorSwatches
                delegate: Rectangle {
                    required property string modelData
                    width: 22
                    height: 22
                    radius: 11
                    color: modelData
                    border.color: eventDialog.selectedColor === modelData ? "#FFFFFF" : "transparent"
                    border.width: 2
                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: eventDialog.selectedColor = modelData
                    }
                }
            }
        }

        RowLayout {
            Layout.fillWidth: true
            Item { Layout.fillWidth: true }
            Button {
                text: "Cancel"
                onClicked: eventDialog.close()
            }
            Button {
                text: "Add Event"
                enabled: titleField.text.trim().length > 0 && dateField.text.trim().length === 10
                onClicked: {
                    const chosenSubjectId = subjectCombo.model[subjectCombo.currentIndex].id;
                    if (CalendarVM.addStandaloneEvent(
                            titleField.text,
                            locationField.text,
                            eventDialog.selectedColor,
                            dateField.text.trim(),
                            startTimeField.text.trim(),
                            endTimeField.text.trim(),
                            allDayCheck.checked,
                            chosenSubjectId)) {
                        eventDialog.close();
                    }
                }
            }
        }
    }
}
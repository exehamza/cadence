import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Cadence 1.0

Popup {
    id: dialog
    modal: true
    focus: true
    anchors.centerIn: Overlay.overlay
    width: Math.min(500, parent ? parent.width - 48 : 500)
    padding: 22

    property string selectedColor: "#6366F1"
    property var editingSubjectId: -1
    readonly property var colorSwatches: [
        "#6366F1", "#EC4899", "#10B981", "#F59E0B",
        "#06B6D4", "#8B5CF6", "#EF4444", "#14B8A6"
    ]

    function resetForm() {
        editingSubjectId = -1;
        nameInput.clear();
        weeklyGoalSpin.value = 0;
        selectedColor = "#6366F1";
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
                text: "Manage Subjects"
                color: "#F8FAFC"
                font.pixelSize: 18
                font.weight: Font.DemiBold
                Layout.fillWidth: true
            }
            ToolButton {
                text: "✕"
                onClicked: {
                    dialog.resetForm();
                    dialog.close();
                }
                contentItem: Label {
                    text: "✕"
                    color: "#94A3B8"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }
        }

        ListView {
            id: subList
            Layout.fillWidth: true
            Layout.preferredHeight: Math.min(240, Math.max(64, contentHeight))
            clip: true
            spacing: 6
            model: SubjectModel

            Label {
                anchors.centerIn: parent
                visible: subList.count === 0
                text: "No subjects created yet."
                color: "#64748B"
                font.pixelSize: 13
            }

            delegate: Rectangle {
                width: subList.width
                height: 46
                radius: 10
                color: dialog.editingSubjectId === model.subjectId ? "#232C40" : "#11151F"
                border.color: dialog.editingSubjectId === model.subjectId ? model.colorHex : "#1E2534"

                RowLayout {
                    anchors.fill: parent
                    anchors.leftMargin: 12
                    anchors.rightMargin: 8
                    spacing: 10

                    Rectangle {
                        width: 12
                        height: 12
                        radius: 6
                        color: model.colorHex
                    }

                    Label {
                        text: model.name
                        color: "#F8FAFC"
                        font.pixelSize: 14
                        font.weight: Font.Medium
                        Layout.fillWidth: true
                        elide: Text.ElideRight
                    }

                    Label {
                        text: model.totalHours.toFixed(1) + "h"
                        color: "#94A3B8"
                        font.pixelSize: 12
                    }

                    ToolButton {
                        text: "Edit"
                        onClicked: {
                            dialog.editingSubjectId = model.subjectId;
                            nameInput.text = model.name;
                            dialog.selectedColor = model.colorHex;
                            weeklyGoalSpin.value = model.targetWeeklyMinutes;
                        }
                        contentItem: Label {
                            text: "Edit"
                            color: "#38BDF8"
                            font.pixelSize: 12
                        }
                    }

                    ToolButton {
                        text: "✕"
                        onClicked: {
                            if (dialog.editingSubjectId === model.subjectId) {
                                dialog.resetForm();
                            }
                            SubjectModel.removeSubject(model.subjectId);
                        }
                        contentItem: Label {
                            text: "✕"
                            color: "#F87171"
                            font.pixelSize: 12
                        }
                    }
                }
            }
        }

        Rectangle { Layout.fillWidth: true; height: 1; color: "#262E3F" }

        Label {
            text: dialog.editingSubjectId > 0 ? "Edit Subject" : "Add New Subject"
            color: "#94A3B8"
            font.pixelSize: 12
            font.weight: Font.DemiBold
        }

        RowLayout {
            Layout.fillWidth: true
            spacing: 8

            TextField {
                id: nameInput
                Layout.fillWidth: true
                placeholderText: "Subject name (e.g., CS2115)"
                color: "#F8FAFC"
                placeholderTextColor: "#64748B"
                background: Rectangle {
                    radius: 8
                    color: "#0F1117"
                    border.color: nameInput.activeFocus ? dialog.selectedColor : "#2A3244"
                }
            }

            Label {
                text: "Weekly Goal (m):"
                color: "#94A3B8"
                font.pixelSize: 12
            }

            SpinBox {
                id: weeklyGoalSpin
                from: 0
                to: 3600
                stepSize: 30
                value: 0
            }
        }

        RowLayout {
            Layout.fillWidth: true
            spacing: 10

            Repeater {
                model: dialog.colorSwatches
                delegate: Rectangle {
                    required property string modelData
                    width: 22
                    height: 22
                    radius: 11
                    color: modelData
                    border.color: dialog.selectedColor === modelData ? "#FFFFFF" : "transparent"
                    border.width: 2

                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: dialog.selectedColor = modelData
                    }
                }
            }

            Item { Layout.fillWidth: true }

            Button {
                visible: dialog.editingSubjectId > 0
                text: "Cancel"
                onClicked: dialog.resetForm()
            }

            Button {
                text: dialog.editingSubjectId > 0 ? "Save" : "Create"
                enabled: nameInput.text.trim().length > 0
                onClicked: {
                    if (dialog.editingSubjectId > 0) {
                        SubjectModel.editSubject(dialog.editingSubjectId, nameInput.text, dialog.selectedColor, weeklyGoalSpin.value);
                    } else {
                        SubjectModel.addSubject(nameInput.text, dialog.selectedColor, weeklyGoalSpin.value);
                    }
                    dialog.resetForm();
                }
            }
        }
    }
}
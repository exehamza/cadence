import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Cadence 1.0

Popup {
    id: dialog
    modal: true
    focus: true
    anchors.centerIn: Overlay.overlay
    width: Math.min(440, parent ? parent.width - 48 : 440)
    padding: 22

    property string selectedNewColor: "#6366F1"
    readonly property var colorSwatches: [
        "#6366F1", "#EC4899", "#10B981", "#F59E0B",
        "#06B6D4", "#8B5CF6", "#EF4444", "#14B8A6"
    ]

    background: Rectangle {
        color: "#161B26"
        radius: 16
        border.color: "#2A3244"
        border.width: 1
    }

    contentItem: ColumnLayout {
        spacing: 16

        RowLayout {
            Layout.fillWidth: true
            Label {
                text: "Select Active Subject"
                color: "#F8FAFC"
                font.pixelSize: 18
                font.weight: Font.DemiBold
                Layout.fillWidth: true
            }
            ToolButton {
                text: "✕"
                onClicked: dialog.close()
                contentItem: Label {
                    text: "✕"
                    color: "#94A3B8"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }
        }

        // Existing Subjects List
        ListView {
            id: subjectList
            Layout.fillWidth: true
            Layout.preferredHeight: Math.min(220, Math.max(56, contentHeight))
            clip: true
            spacing: 6
            model: SubjectModel

            Label {
                anchors.centerIn: parent
                visible: subjectList.count === 0
                text: "No subjects yet — create your first one below."
                color: "#64748B"
                font.pixelSize: 13
            }

            delegate: ItemDelegate {
                id: rowItem
                width: subjectList.width
                height: 44
                onClicked: {
                    FocusTimer.selectSubject(model.subjectId, model.name, model.colorHex);
                    dialog.close();
                }

                background: Rectangle {
                    radius: 10
                    color: FocusTimer.activeSubjectId === model.subjectId
                           ? "#252D40"
                           : (rowItem.hovered ? "#1E2534" : "#121620")
                    border.color: FocusTimer.activeSubjectId === model.subjectId ? model.colorHex : "transparent"
                    border.width: 1
                }

                contentItem: RowLayout {
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
                        text: model.totalHours.toFixed(1) + " hrs"
                        color: "#94A3B8"
                        font.pixelSize: 12
                    }
                }
            }
        }

        Rectangle {
            Layout.fillWidth: true
            height: 1
            color: "#262E3F"
        }

        // Quick Add New Subject
        Label {
            text: "Create New Subject"
            color: "#94A3B8"
            font.pixelSize: 12
            font.weight: Font.DemiBold
        }

        RowLayout {
            Layout.fillWidth: true
            spacing: 8

            TextField {
                id: newSubjectInput
                Layout.fillWidth: true
                placeholderText: "e.g., CS2115 Computer Organization"
                color: "#F8FAFC"
                placeholderTextColor: "#64748B"
                background: Rectangle {
                    radius: 8
                    color: "#0F1117"
                    border.color: newSubjectInput.activeFocus ? dialog.selectedNewColor : "#2A3244"
                }
                onAccepted: createBtn.clicked()
            }

            Button {
                id: createBtn
                text: "Add"
                enabled: newSubjectInput.text.trim().length > 0
                onClicked: {
                    if (SubjectModel.addSubject(newSubjectInput.text, dialog.selectedNewColor, 0)) {
                        const lastIdx = SubjectModel.rowCount() - 1;
                        const idx = SubjectModel.index(lastIdx, 0);
                        FocusTimer.selectSubject(
                            SubjectModel.data(idx, SubjectModel.IdRole),
                            SubjectModel.data(idx, SubjectModel.NameRole),
                            SubjectModel.data(idx, SubjectModel.ColorHexRole)
                        );
                        newSubjectInput.clear();
                        dialog.close();
                    }
                }
            }
        }

        RowLayout {
            Layout.alignment: Qt.AlignHCenter
            spacing: 10

            Repeater {
                model: dialog.colorSwatches
                delegate: Rectangle {
                    required property string modelData
                    width: 22
                    height: 22
                    radius: 11
                    color: modelData
                    border.color: dialog.selectedNewColor === modelData ? "#FFFFFF" : "transparent"
                    border.width: 2

                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: dialog.selectedNewColor = modelData
                    }
                }
            }
        }
    }
}
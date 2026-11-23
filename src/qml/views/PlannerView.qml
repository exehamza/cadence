import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Cadence 1.0

Item {
    id: plannerView

    readonly property string todayIso: Qt.formatDate(new Date(), "yyyy-MM-dd")
    readonly property string tomorrowIso: {
        let d = new Date();
        d.setDate(d.getDate() + 1);
        return Qt.formatDate(d, "yyyy-MM-dd");
    }

    property var activeSubGoalParentId: -1

    function ensureChecklistSelected() {
        if (ChecklistItemModel.activeChecklistId <= 0 && ChecklistModel.rowCount() > 0) {
            const idx = ChecklistModel.index(0, 0);
            ChecklistItemModel.activeChecklistId = ChecklistModel.data(idx, ChecklistModel.IdRole);
        }
    }

    Component.onCompleted: ensureChecklistSelected()

    Connections {
        target: ChecklistModel
        function onCountChanged() {
            plannerView.ensureChecklistSelected();
        }
    }

    RowLayout {
        anchors.fill: parent
        anchors.margins: 24
        spacing: 20

        // =====================================================================
        // LEFT PANE: Categorized Reminders + "Push to Tomorrow"
        // =====================================================================
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.preferredWidth: 52
            radius: 16
            color: "#141822"
            border.color: "#232A3B"
            border.width: 1

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 20
                spacing: 14

                // Header + Filter Bar
                RowLayout {
                    Layout.fillWidth: true
                    spacing: 8

                    Label {
                        text: "Reminders & Deadlines"
                        color: "#F8FAFC"
                        font.pixelSize: 18
                        font.weight: Font.DemiBold
                        Layout.fillWidth: true
                    }

                    CheckBox {
                        id: showCompletedToggle
                        checked: ReminderFilter.showCompleted
                        text: "Show Done"
                        onToggled: ReminderFilter.showCompleted = checked
                        contentItem: Label {
                            text: showCompletedToggle.text
                            color: "#94A3B8"
                            font.pixelSize: 12
                            leftPadding: showCompletedToggle.indicator.width + 6
                            verticalAlignment: Text.AlignVCenter
                        }
                    }
                }

                // Category Filter Pills
                RowLayout {
                    spacing: 6
                    Repeater {
                        model: [
                            { label: "All",          val: "all" },
                            { label: "Tasks",        val: "task" },
                            { label: "Assignments",  val: "assignment" },
                            { label: "Examinations", val: "examination" }
                        ]
                        delegate: Rectangle {
                            required property var modelData
                            readonly property bool active: ReminderFilter.categoryFilter === modelData.val
                            implicitWidth: catLbl.implicitWidth + 22
                            implicitHeight: 30
                            radius: 15
                            color: active ? "#4F46E5" : "#1C2230"
                            border.color: active ? "#6366F1" : "#283044"

                            Label {
                                id: catLbl
                                anchors.centerIn: parent
                                text: modelData.label
                                color: active ? "#FFFFFF" : "#94A3B8"
                                font.pixelSize: 12
                                font.weight: Font.Medium
                            }

                            MouseArea {
                                anchors.fill: parent
                                cursorShape: Qt.PointingHandCursor
                                onClicked: ReminderFilter.categoryFilter = modelData.val
                            }
                        }
                    }
                }

                // Quick-Add Reminder Form
                Rectangle {
                    Layout.fillWidth: true
                    implicitHeight: addFormCol.implicitHeight + 20
                    radius: 12
                    color: "#0F121A"
                    border.color: "#22293A"

                    ColumnLayout {
                        id: addFormCol
                        anchors.fill: parent
                        anchors.margins: 10
                        spacing: 8

                        RowLayout {
                            Layout.fillWidth: true
                            spacing: 8

                            TextField {
                                id: remTitleInput
                                Layout.fillWidth: true
                                placeholderText: "Add task, assignment, or exam..."
                                color: "#F8FAFC"
                                placeholderTextColor: "#64748B"
                                background: Rectangle {
                                    radius: 8
                                    color: "#161B26"
                                    border.color: remTitleInput.activeFocus ? "#6366F1" : "#273043"
                                }
                                onAccepted: addRemBtn.clicked()
                            }

                            ComboBox {
                                id: remCategoryBox
                                Layout.preferredWidth: 126
                                model: [
                                    { label: "Task",        val: "task" },
                                    { label: "Assignment",  val: "assignment" },
                                    { label: "Examination", val: "examination" }
                                ]
                                textRole: "label"
                            }
                        }

                        RowLayout {
                            Layout.fillWidth: true
                            spacing: 8

                            ComboBox {
                                id: remSubjectBox
                                Layout.fillWidth: true
                                model: {
                                    let items = [{ id: -1, name: "No Subject" }];
                                    for (let i = 0; i < SubjectModel.rowCount(); ++i) {
                                        const idx = SubjectModel.index(i, 0);
                                        items.push({
                                            id: SubjectModel.data(idx, SubjectModel.IdRole),
                                            name: SubjectModel.data(idx, SubjectModel.NameRole)
                                        });
                                    }
                                    return items;
                                }
                                textRole: "name"
                            }

                            TextField {
                                id: remDateInput
                                Layout.preferredWidth: 112
                                text: plannerView.todayIso
                                placeholderText: "YYYY-MM-DD"
                                color: "#F8FAFC"
                                font.pixelSize: 12
                                background: Rectangle { radius: 8; color: "#161B26"; border.color: "#273043" }
                            }

                            TextField {
                                id: remTimeInput
                                Layout.preferredWidth: 68
                                text: "23:59"
                                placeholderText: "HH:mm"
                                color: "#F8FAFC"
                                font.pixelSize: 12
                                background: Rectangle { radius: 8; color: "#161B26"; border.color: "#273043" }
                            }

                            Button {
                                id: addRemBtn
                                text: "Add"
                                enabled: remTitleInput.text.trim().length > 0
                                onClicked: {
                                    const dateStr = remDateInput.text.trim();
                                    let epochSec = 0;
                                    if (dateStr.length === 10) {
                                        const timeStr = remTimeInput.text.trim().length === 5
                                            ? remTimeInput.text.trim()
                                            : "09:00";
                                        const parsedDt = Date.fromLocaleString(Qt.locale(), dateStr + " " + timeStr, "yyyy-MM-dd HH:mm");
                                        if (!isNaN(parsedDt.getTime())) {
                                            epochSec = Math.floor(parsedDt.getTime() / 1000);
                                        }
                                    }
                                    const subId = remSubjectBox.model[remSubjectBox.currentIndex].id;
                                    const catVal = remCategoryBox.model[remCategoryBox.currentIndex].val;

                                    if (ReminderModel.addReminder(remTitleInput.text, "", catVal, subId, epochSec, dateStr)) {
                                        remTitleInput.clear();
                                    }
                                }
                            }
                        }
                    }
                }

                // Reminders ListView
                ListView {
                    id: remindersList
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    clip: true
                    spacing: 8
                    model: ReminderFilter

                    Label {
                        anchors.centerIn: parent
                        visible: remindersList.count === 0
                        text: "No reminders in this view."
                        color: "#64748B"
                        font.pixelSize: 13
                    }

                    delegate: Rectangle {
                        width: remindersList.width
                        height: 64
                        radius: 11
                        color: model.isCompleted ? "#10141D" : "#181E2C"
                        border.color: model.category === "examination" && !model.isCompleted
                                      ? "#F43F5E"
                                      : "#252E42"
                        border.width: 1

                        RowLayout {
                            anchors.fill: parent
                            anchors.leftMargin: 12
                            anchors.rightMargin: 10
                            spacing: 10

                            // Subject Color Accent Strip
                            Rectangle {
                                width: 4
                                height: 38
                                radius: 2
                                color: model.subjectColorHex
                            }

                            CheckBox {
                                checked: model.isCompleted
                                onToggled: ReminderModel.toggleCompleted(model.reminderId)
                            }

                            ColumnLayout {
                                Layout.fillWidth: true
                                spacing: 3

                                RowLayout {
                                    spacing: 6
                                    Label {
                                        text: model.title
                                        color: model.isCompleted ? "#64748B" : "#F8FAFC"
                                        font.pixelSize: 14
                                        font.weight: Font.Medium
                                        font.strikeout: model.isCompleted
                                        elide: Text.ElideRight
                                        Layout.fillWidth: true
                                    }
                                }

                                RowLayout {
                                    spacing: 8

                                    // Category Tag
                                    Rectangle {
                                        implicitWidth: catTagText.implicitWidth + 12
                                        implicitHeight: 18
                                        radius: 4
                                        color: model.category === "examination"
                                               ? "#3B1522"
                                               : (model.category === "assignment" ? "#1E293B" : "#1B2436")

                                        Label {
                                            id: catTagText
                                            anchors.centerIn: parent
                                            text: model.category.toUpperCase()
                                            color: model.category === "examination"
                                                   ? "#FDA4AF"
                                                   : (model.category === "assignment" ? "#7DD3FC" : "#94A3B8")
                                            font.pixelSize: 10
                                            font.weight: Font.Bold
                                        }
                                    }

                                    Label {
                                        visible: model.subjectName.length > 0
                                        text: model.subjectName
                                        color: model.subjectColorHex
                                        font.pixelSize: 11
                                        font.weight: Font.DemiBold
                                    }

                                    Label {
                                        visible: model.dueDate.length > 0
                                        text: {
                                            if (model.dueDate === plannerView.todayIso) return "• Today";
                                            if (model.dueDate === plannerView.tomorrowIso) return "• Tomorrow";
                                            return "• " + model.dueDate;
                                        }
                                        color: (!model.isCompleted && model.dueDate < plannerView.todayIso && model.dueDate.length > 0)
                                               ? "#FB7185"
                                               : "#94A3B8"
                                        font.pixelSize: 11
                                    }
                                }
                            }

                            // One-Click "Push to Tomorrow" Action
                            Button {
                                visible: !model.isCompleted
                                text: "→ Tomorrow"
                                implicitHeight: 28
                                onClicked: ReminderModel.pushToTomorrow(model.reminderId)
                                background: Rectangle {
                                    radius: 6
                                    color: "#222B3F"
                                    border.color: "#334155"
                                }
                                contentItem: Label {
                                    text: "→ Tomorrow"
                                    color: "#E2E8F0"
                                    font.pixelSize: 11
                                    font.weight: Font.Medium
                                    leftPadding: 8
                                    rightPadding: 8
                                    verticalAlignment: Text.AlignVCenter
                                }
                            }

                            ToolButton {
                                text: "✕"
                                onClicked: ReminderModel.removeReminder(model.reminderId)
                                contentItem: Label {
                                    text: "✕"
                                    color: "#64748B"
                                    horizontalAlignment: Text.AlignHCenter
                                    verticalAlignment: Text.AlignVCenter
                                }
                            }
                        }
                    }
                }
            }
        }

        // =====================================================================
        // RIGHT PANE: Long-Term Nested Checklists
        // =====================================================================
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.preferredWidth: 48
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
                    Label {
                        text: "Long-Term Checklists"
                        color: "#F8FAFC"
                        font.pixelSize: 18
                        font.weight: Font.DemiBold
                        Layout.fillWidth: true
                    }
                }

                // New Top-Level Checklist Bar
                RowLayout {
                    Layout.fillWidth: true
                    spacing: 8

                    TextField {
                        id: newListTitleInput
                        Layout.fillWidth: true
                        placeholderText: "New list (e.g., CS Books to Read)..."
                        color: "#F8FAFC"
                        placeholderTextColor: "#64748B"
                        background: Rectangle {
                            radius: 8
                            color: "#0F121A"
                            border.color: newListTitleInput.activeFocus ? "#10B981" : "#273043"
                        }
                        onAccepted: createListBtn.clicked()
                    }

                    Button {
                        id: createListBtn
                        text: "+ List"
                        enabled: newListTitleInput.text.trim().length > 0
                        onClicked: {
                            if (ChecklistModel.addChecklist(newListTitleInput.text, -1)) {
                                newListTitleInput.clear();
                                const lastRow = ChecklistModel.rowCount() - 1;
                                const idx = ChecklistModel.index(lastRow, 0);
                                ChecklistItemModel.activeChecklistId = ChecklistModel.data(idx, ChecklistModel.IdRole);
                            }
                        }
                    }
                }

                // Horizontal Checklist Selector Tabs
                ListView {
                    id: checklistTabs
                    Layout.fillWidth: true
                    Layout.preferredHeight: 38
                    orientation: ListView.Horizontal
                    spacing: 8
                    clip: true
                    model: ChecklistModel

                    delegate: Rectangle {
                        readonly property bool selected: ChecklistItemModel.activeChecklistId === model.checklistId
                        implicitWidth: tabRow.implicitWidth + 24
                        implicitHeight: 36
                        radius: 10
                        color: selected ? "#1E293B" : "#0F121A"
                        border.color: selected ? "#38BDF8" : "#232B3D"

                        RowLayout {
                            id: tabRow
                            anchors.centerIn: parent
                            spacing: 8

                            Label {
                                text: model.title
                                color: selected ? "#F8FAFC" : "#94A3B8"
                                font.pixelSize: 13
                                font.weight: selected ? Font.DemiBold : Font.Normal
                            }

                            Label {
                                text: model.completedCount + "/" + model.totalCount
                                color: "#38BDF8"
                                font.pixelSize: 11
                                font.weight: Font.Bold
                            }

                            MouseArea {
                                Layout.preferredWidth: 14
                                Layout.preferredHeight: 14
                                cursorShape: Qt.PointingHandCursor
                                onClicked: {
                                    const deletingActive = (ChecklistItemModel.activeChecklistId === model.checklistId);
                                    ChecklistModel.removeChecklist(model.checklistId);
                                    if (deletingActive) {
                                        ChecklistItemModel.activeChecklistId = -1;
                                        plannerView.ensureChecklistSelected();
                                    }
                                }
                                Label {
                                    anchors.centerIn: parent
                                    text: "✕"
                                    color: "#64748B"
                                    font.pixelSize: 10
                                }
                            }
                        }

                        MouseArea {
                            anchors.fill: parent
                            z: -1
                            cursorShape: Qt.PointingHandCursor
                            onClicked: {
                                plannerView.activeSubGoalParentId = -1;
                                ChecklistItemModel.activeChecklistId = model.checklistId;
                            }
                        }
                    }
                }

                // Add Root Item to Active Checklist
                RowLayout {
                    Layout.fillWidth: true
                    visible: ChecklistItemModel.activeChecklistId > 0
                    spacing: 8

                    TextField {
                        id: rootGoalInput
                        Layout.fillWidth: true
                        placeholderText: "Add top-level goal or milestone..."
                        color: "#F8FAFC"
                        placeholderTextColor: "#64748B"
                        background: Rectangle {
                            radius: 8
                            color: "#0F121A"
                            border.color: rootGoalInput.activeFocus ? "#38BDF8" : "#273043"
                        }
                        onAccepted: addRootGoalBtn.clicked()
                    }

                    Button {
                        id: addRootGoalBtn
                        text: "Add Goal"
                        enabled: rootGoalInput.text.trim().length > 0
                        onClicked: {
                            if (ChecklistItemModel.addItem(rootGoalInput.text, -1)) {
                                rootGoalInput.clear();
                            }
                        }
                    }
                }

                // Hierarchical Nested Checklist Items
                ListView {
                    id: treeList
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    clip: true
                    spacing: 6
                    model: ChecklistItemModel

                    Label {
                        anchors.centerIn: parent
                        visible: ChecklistItemModel.activeChecklistId <= 0
                        text: "Create a checklist above to organize long-term goals."
                        color: "#64748B"
                        font.pixelSize: 13
                    }

                    delegate: ColumnLayout {
                        width: treeList.width
                        spacing: 4

                        Rectangle {
                            Layout.fillWidth: true
                            Layout.leftMargin: model.depth * 24
                            height: 44
                            radius: 9
                            color: model.depth === 0 ? "#181E2C" : "#121722"
                            border.color: model.depth === 0 ? "#273147" : "#1D2434"

                            RowLayout {
                                anchors.fill: parent
                                anchors.leftMargin: 10
                                anchors.rightMargin: 8
                                spacing: 8

                                Label {
                                    visible: model.depth > 0
                                    text: "↳"
                                    color: "#475569"
                                    font.pixelSize: 13
                                }

                                CheckBox {
                                    checked: model.isCompleted
                                    onToggled: ChecklistItemModel.toggleItem(model.itemId)
                                }

                                Label {
                                    text: model.title
                                    color: model.isCompleted ? "#64748B" : "#F8FAFC"
                                    font.pixelSize: model.depth === 0 ? 14 : 13
                                    font.weight: model.depth === 0 ? Font.Medium : Font.Normal
                                    font.strikeout: model.isCompleted
                                    elide: Text.ElideRight
                                    Layout.fillWidth: true
                                }

                                Button {
                                    text: "+ Sub"
                                    implicitHeight: 24
                                    onClicked: {
                                        if (plannerView.activeSubGoalParentId === model.itemId) {
                                            plannerView.activeSubGoalParentId = -1;
                                        } else {
                                            plannerView.activeSubGoalParentId = model.itemId;
                                            subGoalField.clear();
                                            subGoalField.forceActiveFocus();
                                        }
                                    }
                                    background: Rectangle {
                                        radius: 5
                                        color: "#1E2638"
                                    }
                                    contentItem: Label {
                                        text: "+ Sub"
                                        color: "#38BDF8"
                                        font.pixelSize: 11
                                        leftPadding: 7
                                        rightPadding: 7
                                        verticalAlignment: Text.AlignVCenter
                                    }
                                }

                                ToolButton {
                                    text: "✕"
                                    onClicked: ChecklistItemModel.removeItem(model.itemId)
                                    contentItem: Label {
                                        text: "✕"
                                        color: "#64748B"
                                        font.pixelSize: 12
                                        horizontalAlignment: Text.AlignHCenter
                                        verticalAlignment: Text.AlignVCenter
                                    }
                                }
                            }
                        }

                        // Inline Sub-Goal Creator Row
                        RowLayout {
                            Layout.fillWidth: true
                            Layout.leftMargin: (model.depth + 1) * 24
                            visible: plannerView.activeSubGoalParentId === model.itemId
                            spacing: 6

                            TextField {
                                id: subGoalField
                                Layout.fillWidth: true
                                placeholderText: "Sub-goal under \"" + model.title + "\"..."
                                color: "#F8FAFC"
                                font.pixelSize: 12
                                background: Rectangle {
                                    radius: 7
                                    color: "#0F121A"
                                    border.color: "#38BDF8"
                                }
                                onAccepted: addSubBtn.clicked()
                            }

                            Button {
                                id: addSubBtn
                                text: "Add"
                                enabled: subGoalField.text.trim().length > 0
                                onClicked: {
                                    if (ChecklistItemModel.addItem(subGoalField.text, model.itemId)) {
                                        subGoalField.clear();
                                        plannerView.activeSubGoalParentId = -1;
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
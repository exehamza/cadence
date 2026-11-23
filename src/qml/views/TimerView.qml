import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import QtMultimedia
import Cadence 1.0
import "../components"

Item {
    id: timerView

    property bool isFullscreen: false
    property bool drawerOpen: false
    signal toggleFullscreenRequested()

    readonly property color effectiveAccentColor: FocusTimer.phase === FocusTimer.Break
        ? "#10B981"
        : FocusTimer.activeSubjectColor

    // =========================================================================
    // 1. Hardware-Accelerated Backdrop Layer (Solid Color / Image / Looping .mp4)
    // =========================================================================
    Rectangle {
        anchors.fill: parent
        color: FocusTimer.backgroundColor
    }

    Image {
        anchors.fill: parent
        visible: FocusTimer.backgroundType === FocusTimer.Image
                 && FocusTimer.backgroundMediaUrl.toString().length > 0
        source: FocusTimer.backgroundType === FocusTimer.Image ? FocusTimer.backgroundMediaUrl : ""
        fillMode: Image.PreserveAspectCrop
        asynchronous: true
        cache: false
    }

    MediaPlayer {
        id: bgVideoPlayer
        readonly property bool activeVideo: FocusTimer.backgroundType === FocusTimer.Video
                                            && FocusTimer.backgroundMediaUrl.toString().length > 0
        source: activeVideo ? FocusTimer.backgroundMediaUrl : ""
        loops: MediaPlayer.Infinite
        audioOutput: AudioOutput {
            muted: true
            volume: 0.0
        }
        videoOutput: bgVideoSurface

        onActiveVideoChanged: {
            if (activeVideo) play();
            else stop();
        }
        onSourceChanged: {
            if (activeVideo) play();
        }
    }

    VideoOutput {
        id: bgVideoSurface
        anchors.fill: parent
        visible: bgVideoPlayer.activeVideo
        fillMode: VideoOutput.PreserveAspectCrop
    }

    // Contrast scrim so HUD remains readable over bright images/videos
    Rectangle {
        anchors.fill: parent
        visible: FocusTimer.backgroundType !== FocusTimer.SolidColor
        color: "#090C12"
        opacity: 0.56
    }

    // =========================================================================
    // 2. Ambient Focus Audio Engine (Auto-fades & pauses during Breaks)
    // =========================================================================
    AudioOutput {
        id: focusAudioOutput
        volume: FocusTimer.shouldPlayFocusAudio ? FocusTimer.audioVolume : 0.0

        Behavior on volume {
            NumberAnimation {
                duration: 600
                easing.type: Easing.InOutQuad
                onRunningChanged: {
                    if (!running && !FocusTimer.shouldPlayFocusAudio) {
                        focusAudioPlayer.pause();
                    }
                }
            }
        }
    }

    MediaPlayer {
        id: focusAudioPlayer
        source: FocusTimer.audioTrackUrl
        loops: MediaPlayer.Infinite
        audioOutput: focusAudioOutput
    }

    Connections {
        target: FocusTimer
        function onShouldPlayFocusAudioChanged() {
            if (FocusTimer.shouldPlayFocusAudio) {
                focusAudioPlayer.play();
            }
        }
        function onFocusSessionLogged(subjectName, minutesLogged) {
            toastBanner.show("Logged " + minutesLogged + " min to " + subjectName);
        }
    }

    // =========================================================================
    // 3. Top Distraction-Free Control Bar
    // =========================================================================
    RowLayout {
        id: topBar
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 24
        spacing: 12

        // Mode Switcher (Pomodoro | Flowtime)
        Rectangle {
            implicitWidth: modeRow.implicitWidth + 8
            implicitHeight: 38
            radius: 19
            color: Qt.rgba(0.08, 0.10, 0.15, 0.82)
            border.color: "#273043"
            border.width: 1

            RowLayout {
                id: modeRow
                anchors.centerIn: parent
                spacing: 4

                Repeater {
                    model: [
                        { label: "Pomodoro", modeVal: FocusTimer.Pomodoro },
                        { label: "Flowtime", modeVal: FocusTimer.Flowtime }
                    ]
                    delegate: Rectangle {
                        required property var modelData
                        readonly property bool selected: FocusTimer.mode === modelData.modeVal
                        implicitWidth: modeText.implicitWidth + 24
                        implicitHeight: 30
                        radius: 15
                        color: selected ? timerView.effectiveAccentColor : "transparent"

                        Label {
                            id: modeText
                            anchors.centerIn: parent
                            text: modelData.label
                            color: selected ? "#FFFFFF" : "#94A3B8"
                            font.pixelSize: 13
                            font.weight: Font.DemiBold
                        }

                        MouseArea {
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            onClicked: FocusTimer.mode = modelData.modeVal
                        }
                    }
                }
            }
        }

        // Active Subject Selector Pill
        Rectangle {
            implicitWidth: subPillRow.implicitWidth + 24
            implicitHeight: 38
            radius: 19
            color: Qt.rgba(0.08, 0.10, 0.15, 0.82)
            border.color: FocusTimer.activeSubjectColor
            border.width: 1

            RowLayout {
                id: subPillRow
                anchors.centerIn: parent
                spacing: 8

                Rectangle {
                    width: 10
                    height: 10
                    radius: 5
                    color: FocusTimer.activeSubjectColor
                }

                Label {
                    text: FocusTimer.activeSubjectName
                    color: "#F8FAFC"
                    font.pixelSize: 13
                    font.weight: Font.Medium
                }

                Label {
                    text: "▾"
                    color: "#94A3B8"
                    font.pixelSize: 12
                }
            }

            MouseArea {
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                onClicked: subjectPicker.open()
            }
        }

        Item { Layout.fillWidth: true }

        // Backdrop & Audio Drawer Button
        Button {
            text: timerView.drawerOpen ? "Close Studio" : "Backdrop & Audio"
            onClicked: timerView.drawerOpen = !timerView.drawerOpen
            background: Rectangle {
                radius: 10
                color: timerView.drawerOpen
                       ? timerView.effectiveAccentColor
                       : Qt.rgba(0.08, 0.10, 0.15, 0.82)
                border.color: "#273043"
            }
            contentItem: Label {
                text: parent.text
                color: "#F8FAFC"
                font.pixelSize: 13
                font.weight: Font.Medium
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                leftPadding: 12
                rightPadding: 12
            }
        }

        // Fullscreen Toggle Button
        Button {
            text: timerView.isFullscreen ? "Exit Fullscreen" : "Fullscreen"
            onClicked: timerView.toggleFullscreenRequested()
            background: Rectangle {
                radius: 10
                color: Qt.rgba(0.08, 0.10, 0.15, 0.82)
                border.color: "#273043"
            }
            contentItem: Label {
                text: parent.text
                color: "#F8FAFC"
                font.pixelSize: 13
                font.weight: Font.Medium
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                leftPadding: 12
                rightPadding: 12
            }
        }
    }

    // =========================================================================
    // 4. Center Focus Gauge & Transport Controls
    // =========================================================================
    ColumnLayout {
        anchors.centerIn: parent
        spacing: 28

        CircularTimerGauge {
            Layout.alignment: Qt.AlignHCenter
            progress: FocusTimer.progress
            timeText: FocusTimer.formattedTime
            phaseLabel: FocusTimer.phase === FocusTimer.Focus ? "FOCUS" : "BREAK"
            modeLabel: FocusTimer.mode === FocusTimer.Pomodoro ? "POMODORO" : "FLOWTIME"
            subjectName: FocusTimer.activeSubjectName
            accentColor: timerView.effectiveAccentColor
        }

        // Primary Transport Row
        RowLayout {
            Layout.alignment: Qt.AlignHCenter
            spacing: 14

            // Start / Pause Button
            Button {
                implicitWidth: 148
                implicitHeight: 48
                onClicked: FocusTimer.toggleStartPause()

                background: Rectangle {
                    radius: 24
                    color: timerView.effectiveAccentColor
                }
                contentItem: Label {
                    text: FocusTimer.isRunning ? "Pause" : (FocusTimer.isPaused ? "Resume" : "Start Focus")
                    color: "#FFFFFF"
                    font.pixelSize: 15
                    font.weight: Font.DemiBold
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }

            // Complete & Log Session Button (or Skip Break)
            Button {
                visible: FocusTimer.phase === FocusTimer.Break
                         || FocusTimer.elapsedSeconds > 0
                implicitWidth: 148
                implicitHeight: 48
                onClicked: FocusTimer.completeAndLogSession()

                background: Rectangle {
                    radius: 24
                    color: Qt.rgba(0.11, 0.14, 0.21, 0.9)
                    border.color: "#334155"
                    border.width: 1
                }
                contentItem: Label {
                    text: FocusTimer.phase === FocusTimer.Break ? "Skip Break" : "Complete & Log"
                    color: "#F8FAFC"
                    font.pixelSize: 14
                    font.weight: Font.Medium
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }

            // Reset Button
            Button {
                visible: FocusTimer.elapsedSeconds > 0 || FocusTimer.phase === FocusTimer.Break
                implicitWidth: 92
                implicitHeight: 48
                onClicked: FocusTimer.reset()

                background: Rectangle {
                    radius: 24
                    color: Qt.rgba(0.11, 0.14, 0.21, 0.72)
                    border.color: "#263042"
                }
                contentItem: Label {
                    text: "Reset"
                    color: "#94A3B8"
                    font.pixelSize: 14
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }
        }

        // Flowtime Proportional Break Offer Card
        Rectangle {
            Layout.alignment: Qt.AlignHCenter
            visible: FocusTimer.hasPendingBreakOffer
            implicitWidth: offerRow.implicitWidth + 32
            implicitHeight: 54
            radius: 14
            color: Qt.rgba(0.06, 0.18, 0.14, 0.92)
            border.color: "#10B981"
            border.width: 1

            RowLayout {
                id: offerRow
                anchors.centerIn: parent
                spacing: 14

                Label {
                    text: "Earned a " + FocusTimer.suggestedBreakMinutes + " min proportional break"
                    color: "#ECFDF5"
                    font.pixelSize: 13
                    font.weight: Font.Medium
                }

                Button {
                    text: "Start Break"
                    onClicked: FocusTimer.acceptFlowtimeBreak()
                    background: Rectangle {
                        radius: 8
                        color: "#10B981"
                    }
                    contentItem: Label {
                        text: "Start Break"
                        color: "#052E16"
                        font.pixelSize: 12
                        font.weight: Font.Bold
                        leftPadding: 10
                        rightPadding: 10
                    }
                }

                ToolButton {
                    text: "✕"
                    onClicked: FocusTimer.dismissFlowtimeBreak()
                    contentItem: Label {
                        text: "✕"
                        color: "#A7F3D0"
                    }
                }
            }
        }
    }

    // =========================================================================
    // 5. Slide-Out Studio Drawer (Durations, Backdrop & Audio)
    // =========================================================================
    Rectangle {
        id: studioDrawer
        width: 340
        anchors.top: topBar.bottom
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.margins: 24
        radius: 16
        color: Qt.rgba(0.08, 0.10, 0.15, 0.95)
        border.color: "#273043"
        border.width: 1
        visible: opacity > 0.01
        opacity: timerView.drawerOpen ? 1.0 : 0.0

        Behavior on opacity {
            NumberAnimation { duration: 180 }
        }

        Flickable {
            anchors.fill: parent
            anchors.margins: 18
            contentHeight: drawerColumn.implicitHeight
            clip: true

            ColumnLayout {
                id: drawerColumn
                width: parent.width
                spacing: 16

                Label {
                    text: "Timer Intervals"
                    color: "#F8FAFC"
                    font.pixelSize: 15
                    font.weight: Font.DemiBold
                }

                RowLayout {
                    Layout.fillWidth: true
                    Label {
                        text: "Pomodoro Focus (min)"
                        color: "#CBD5E1"
                        font.pixelSize: 13
                        Layout.fillWidth: true
                    }
                    SpinBox {
                        from: 1
                        to: 180
                        value: FocusTimer.pomodoroFocusMinutes
                        onValueModified: FocusTimer.pomodoroFocusMinutes = value
                    }
                }

                RowLayout {
                    Layout.fillWidth: true
                    Label {
                        text: "Pomodoro Break (min)"
                        color: "#CBD5E1"
                        font.pixelSize: 13
                        Layout.fillWidth: true
                    }
                    SpinBox {
                        from: 1
                        to: 60
                        value: FocusTimer.pomodoroBreakMinutes
                        onValueModified: FocusTimer.pomodoroBreakMinutes = value
                    }
                }

                RowLayout {
                    Layout.fillWidth: true
                    Label {
                        text: "Flowtime Break Divisor (1/N)"
                        color: "#CBD5E1"
                        font.pixelSize: 13
                        Layout.fillWidth: true
                    }
                    SpinBox {
                        from: 2
                        to: 10
                        value: FocusTimer.flowtimeBreakDivisor
                        onValueModified: FocusTimer.flowtimeBreakDivisor = value
                    }
                }

                Rectangle { Layout.fillWidth: true; height: 1; color: "#232B3B" }

                Label {
                    text: "Focus Backdrop"
                    color: "#F8FAFC"
                    font.pixelSize: 15
                    font.weight: Font.DemiBold
                }

                // Solid Color Presets
                RowLayout {
                    spacing: 10
                    Repeater {
                        model: ["#0F1117", "#0B192C", "#13111C", "#0D1F1A", "#1C1412"]
                        delegate: Rectangle {
                            required property string modelData
                            width: 28
                            height: 28
                            radius: 14
                            color: modelData
                            border.color: (FocusTimer.backgroundType === FocusTimer.SolidColor
                                           && FocusTimer.backgroundColor === modelData)
                                          ? "#FFFFFF" : "#334155"
                            border.width: 2

                            MouseArea {
                                anchors.fill: parent
                                cursorShape: Qt.PointingHandCursor
                                onClicked: {
                                    FocusTimer.backgroundColor = modelData;
                                    FocusTimer.backgroundType = FocusTimer.SolidColor;
                                }
                            }
                        }
                    }
                }

                RowLayout {
                    Layout.fillWidth: true
                    spacing: 8

                    Button {
                        text: "Image..."
                        Layout.fillWidth: true
                        onClicked: imageFileDialog.open()
                    }
                    Button {
                        text: "Looping .mp4..."
                        Layout.fillWidth: true
                        onClicked: videoFileDialog.open()
                    }
                }

                Button {
                    visible: FocusTimer.backgroundType !== FocusTimer.SolidColor
                    text: "Clear Media Backdrop"
                    Layout.fillWidth: true
                    onClicked: FocusTimer.clearBackgroundMedia()
                }

                Rectangle { Layout.fillWidth: true; height: 1; color: "#232B3B" }

                Label {
                    text: "Ambient Focus Audio"
                    color: "#F8FAFC"
                    font.pixelSize: 15
                    font.weight: Font.DemiBold
                }

                Label {
                    text: FocusTimer.audioTrackName
                    color: "#94A3B8"
                    font.pixelSize: 12
                    elide: Text.ElideMiddle
                    Layout.fillWidth: true
                }

                RowLayout {
                    Layout.fillWidth: true
                    spacing: 8

                    Button {
                        text: "Select Audio..."
                        Layout.fillWidth: true
                        onClicked: audioFileDialog.open()
                    }
                    Button {
                        text: FocusTimer.audioMuted ? "Unmute" : "Mute"
                        enabled: FocusTimer.audioTrackUrl.toString().length > 0
                        onClicked: FocusTimer.audioMuted = !FocusTimer.audioMuted
                    }
                }

                RowLayout {
                    Layout.fillWidth: true
                    Label {
                        text: "Volume"
                        color: "#CBD5E1"
                        font.pixelSize: 12
                    }
                    Slider {
                        Layout.fillWidth: true
                        from: 0.0
                        to: 1.0
                        value: FocusTimer.audioVolume
                        onMoved: FocusTimer.audioVolume = value
                    }
                }
            }
        }
    }

    // Toast Notification Banner
    Rectangle {
        id: toastBanner
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottomMargin: 28
        implicitWidth: toastLabel.implicitWidth + 32
        implicitHeight: 40
        radius: 20
        color: "#1E293B"
        border.color: "#38BDF8"
        opacity: 0.0
        visible: opacity > 0.01

        function show(msg) {
            toastLabel.text = msg;
            toastAnim.restart();
        }

        SequentialAnimation {
            id: toastAnim
            NumberAnimation { target: toastBanner; property: "opacity"; to: 1.0; duration: 180 }
            PauseAnimation { duration: 2400 }
            NumberAnimation { target: toastBanner; property: "opacity"; to: 0.0; duration: 300 }
        }

        Label {
            id: toastLabel
            anchors.centerIn: parent
            color: "#F8FAFC"
            font.pixelSize: 13
            font.weight: Font.Medium
        }
    }

    SubjectPickerDialog {
        id: subjectPicker
    }

    FileDialog {
        id: imageFileDialog
        title: "Choose Background Image"
        nameFilters: ["Images (*.png *.jpg *.jpeg *.webp)"]
        onAccepted: {
            FocusTimer.backgroundMediaUrl = selectedFile;
            FocusTimer.backgroundType = FocusTimer.Image;
        }
    }

    FileDialog {
        id: videoFileDialog
        title: "Choose Looping MP4 Video"
        nameFilters: ["MP4 Videos (*.mp4 *.webm *.mkv)"]
        onAccepted: {
            FocusTimer.backgroundMediaUrl = selectedFile;
            FocusTimer.backgroundType = FocusTimer.Video;
        }
    }

    FileDialog {
        id: audioFileDialog
        title: "Choose Ambient Audio Track"
        nameFilters: ["Audio Files (*.mp3 *.wav *.ogg *.flac *.m4a)"]
        onAccepted: {
            FocusTimer.audioTrackUrl = selectedFile;
            FocusTimer.audioMuted = false;
        }
    }
}
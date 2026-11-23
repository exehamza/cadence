#include "viewmodels/FocusTimerViewModel.hpp"

#include <QDate>
#include <QDateTime>
#include <QFileInfo>
#include <QSettings>
#include <algorithm>
#include <cmath>

namespace cadence {

FocusTimerViewModel::FocusTimerViewModel(DatabaseManager& db,
                                         SubjectModel& subjectModel,
                                         QObject* parent)
    : QObject(parent),
      m_db(db),
      m_subjectModel(subjectModel) {
    m_tickTimer.setInterval(250);
    m_tickTimer.setTimerType(Qt::CoarseTimer);
    connect(&m_tickTimer, &QTimer::timeout, this, &FocusTimerViewModel::onTick);

    loadSettings();

    // Auto-select first available subject if none persisted yet
    if (m_activeSubjectId <= 0 && m_subjectModel.rowCount() > 0) {
        const QModelIndex first = m_subjectModel.index(0, 0);
        selectSubject(
            m_subjectModel.data(first, SubjectModel::IdRole).toLongLong(),
            m_subjectModel.data(first, SubjectModel::NameRole).toString(),
            m_subjectModel.data(first, SubjectModel::ColorHexRole).toString()
        );
    }
}

qint64 FocusTimerViewModel::currentElapsedMs() const noexcept {
    if (!m_isRunning) {
        return m_accumulatedMs;
    }
    return m_accumulatedMs + m_segmentClock.elapsed();
}

qint64 FocusTimerViewModel::activeTargetDurationMs() const noexcept {
    if (m_phase == Break) {
        const int breakSecs = (m_customBreakDurationSeconds > 0)
            ? m_customBreakDurationSeconds
            : (m_pomodoroBreakMinutes * 60);
        return static_cast<qint64>(breakSecs) * 1000LL;
    }

    if (m_mode == Pomodoro) {
        return static_cast<qint64>(m_pomodoroFocusMinutes) * 60LL * 1000LL;
    }
    return 0; // Flowtime focus counts up indefinitely
}

int FocusTimerViewModel::elapsedSeconds() const noexcept {
    return static_cast<int>(currentElapsedMs() / 1000LL);
}

int FocusTimerViewModel::remainingSeconds() const noexcept {
    const qint64 targetMs = activeTargetDurationMs();
    if (targetMs <= 0) return 0;

    const qint64 remMs = std::max<qint64>(0, targetMs - currentElapsedMs());
    return static_cast<int>((remMs + 999LL) / 1000LL);
}

QString FocusTimerViewModel::formattedTime() const {
    const int totalSecs = (m_mode == Pomodoro || m_phase == Break)
        ? remainingSeconds()
        : elapsedSeconds();

    const int hours   = totalSecs / 3600;
    const int minutes = (totalSecs % 3600) / 60;
    const int seconds = totalSecs % 60;

    if (hours > 0) {
        return QStringLiteral("%1:%2:%3")
            .arg(hours, 2, 10, QLatin1Char('0'))
            .arg(minutes, 2, 10, QLatin1Char('0'))
            .arg(seconds, 2, 10, QLatin1Char('0'));
    }

    return QStringLiteral("%1:%2")
        .arg(minutes, 2, 10, QLatin1Char('0'))
        .arg(seconds, 2, 10, QLatin1Char('0'));
}

double FocusTimerViewModel::progress() const noexcept {
    const qint64 targetMs = activeTargetDurationMs();
    if (targetMs > 0) {
        const double ratio = static_cast<double>(currentElapsedMs()) / static_cast<double>(targetMs);
        return std::clamp(ratio, 0.0, 1.0);
    }

    // In Flowtime count-up mode, sweep the ring smoothly once per minute
    const qint64 cycleMs = currentElapsedMs() % 60000LL;
    return static_cast<double>(cycleMs) / 60000.0;
}

QString FocusTimerViewModel::audioTrackName() const noexcept {
    if (!m_audioTrackUrl.isValid() || m_audioTrackUrl.isEmpty()) {
        return QStringLiteral("No audio selected");
    }
    const QString localPath = m_audioTrackUrl.isLocalFile()
        ? m_audioTrackUrl.toLocalFile()
        : m_audioTrackUrl.toString();
    return QFileInfo(localPath).fileName();
}

bool FocusTimerViewModel::shouldPlayFocusAudio() const noexcept {
    return m_isRunning
        && m_phase == Focus
        && !m_audioMuted
        && m_audioTrackUrl.isValid()
        && !m_audioTrackUrl.isEmpty();
}

void FocusTimerViewModel::setMode(TimerMode newMode) {
    if (m_mode == newMode) return;
    reset();
    m_mode = newMode;
    emit modeChanged();
    emit timeChanged();
}

void FocusTimerViewModel::setPomodoroFocusMinutes(int minutes) {
    const int clamped = std::clamp(minutes, 1, 180);
    if (m_pomodoroFocusMinutes == clamped) return;
    m_pomodoroFocusMinutes = clamped;
    saveSettings();
    emit configChanged();
    emit timeChanged();
}

void FocusTimerViewModel::setPomodoroBreakMinutes(int minutes) {
    const int clamped = std::clamp(minutes, 1, 60);
    if (m_pomodoroBreakMinutes == clamped) return;
    m_pomodoroBreakMinutes = clamped;
    saveSettings();
    emit configChanged();
    emit timeChanged();
}

void FocusTimerViewModel::setFlowtimeBreakDivisor(int divisor) {
    const int clamped = std::clamp(divisor, 2, 10);
    if (m_flowtimeBreakDivisor == clamped) return;
    m_flowtimeBreakDivisor = clamped;
    saveSettings();
    emit configChanged();
}

void FocusTimerViewModel::setBackgroundType(BackgroundType type) {
    if (m_backgroundType == type) return;
    m_backgroundType = type;
    saveSettings();
    emit mediaSettingsChanged();
}

void FocusTimerViewModel::setBackgroundColor(const QString& hexColor) {
    if (m_backgroundColor == hexColor) return;
    m_backgroundColor = hexColor;
    saveSettings();
    emit mediaSettingsChanged();
}

void FocusTimerViewModel::setBackgroundMediaUrl(const QUrl& url) {
    if (m_backgroundMediaUrl == url) return;
    m_backgroundMediaUrl = url;
    saveSettings();
    emit mediaSettingsChanged();
}

void FocusTimerViewModel::setAudioTrackUrl(const QUrl& url) {
    if (m_audioTrackUrl == url) return;
    m_audioTrackUrl = url;
    saveSettings();
    emit mediaSettingsChanged();
    emit audioPlaybackStateChanged();
}

void FocusTimerViewModel::setAudioVolume(double volume) {
    const double clamped = std::clamp(volume, 0.0, 1.0);
    if (std::abs(m_audioVolume - clamped) < 0.001) return;
    m_audioVolume = clamped;
    saveSettings();
    emit mediaSettingsChanged();
}

void FocusTimerViewModel::setAudioMuted(bool muted) {
    if (m_audioMuted == muted) return;
    m_audioMuted = muted;
    saveSettings();
    emit mediaSettingsChanged();
    emit audioPlaybackStateChanged();
}

void FocusTimerViewModel::start() {
    if (m_isRunning) return;

    if (m_suggestedBreakSeconds > 0) {
        dismissFlowtimeBreak();
    }

    if (m_accumulatedMs == 0) {
        m_sessionStartedEpoch = QDateTime::currentSecsSinceEpoch();
    }

    m_segmentClock.start();
    m_isRunning = true;
    m_tickTimer.start();

    emit runningChanged();
    emit audioPlaybackStateChanged();
}

void FocusTimerViewModel::pause() {
    if (!m_isRunning) return;

    m_accumulatedMs += m_segmentClock.elapsed();
    m_isRunning = false;
    m_tickTimer.stop();

    emit runningChanged();
    emit timeChanged();
    emit audioPlaybackStateChanged();
}

void FocusTimerViewModel::toggleStartPause() {
    if (m_isRunning) {
        pause();
    } else {
        start();
    }
}

void FocusTimerViewModel::reset() {
    m_tickTimer.stop();
    m_isRunning = false;
    m_accumulatedMs = 0;
    m_sessionStartedEpoch = 0;
    m_customBreakDurationSeconds = 0;

    if (m_phase != Focus) {
        m_phase = Focus;
        emit phaseChanged();
    }

    emit runningChanged();
    emit timeChanged();
    emit audioPlaybackStateChanged();
}

void FocusTimerViewModel::completeAndLogSession() {
    if (m_phase != Focus) {
        skipBreak();
        return;
    }

    const int studiedSecs = elapsedSeconds();
    m_tickTimer.stop();
    m_isRunning = false;
    m_accumulatedMs = 0;

    if (studiedSecs > 0) {
        logCompletedFocusSeconds(studiedSecs);
    }

    if (m_mode == Flowtime) {
        if (studiedSecs >= 10) {
            m_suggestedBreakSeconds = std::max(60, studiedSecs / m_flowtimeBreakDivisor);
            emit breakOfferChanged();
        }
        emit runningChanged();
        emit timeChanged();
        emit audioPlaybackStateChanged();
    } else {
        transitionToPhase(Break);
    }
}

void FocusTimerViewModel::acceptFlowtimeBreak() {
    if (m_suggestedBreakSeconds <= 0) return;
    m_customBreakDurationSeconds = m_suggestedBreakSeconds;
    m_suggestedBreakSeconds = 0;
    emit breakOfferChanged();

    transitionToPhase(Break);
    start();
}

void FocusTimerViewModel::dismissFlowtimeBreak() {
    if (m_suggestedBreakSeconds == 0) return;
    m_suggestedBreakSeconds = 0;
    emit breakOfferChanged();
}

void FocusTimerViewModel::skipBreak() {
    m_customBreakDurationSeconds = 0;
    transitionToPhase(Focus);
}

void FocusTimerViewModel::selectSubject(qint64 subjectId, const QString& name, const QString& colorHex) {
    if (m_activeSubjectId == subjectId && m_activeSubjectName == name && m_activeSubjectColor == colorHex) {
        return;
    }
    m_activeSubjectId    = subjectId;
    m_activeSubjectName  = subjectId > 0 ? name : QStringLiteral("Unassigned");
    m_activeSubjectColor = subjectId > 0 ? colorHex : QStringLiteral("#6366F1");
    saveSettings();
    emit activeSubjectChanged();
}

void FocusTimerViewModel::clearBackgroundMedia() {
    m_backgroundMediaUrl.clear();
    m_backgroundType = SolidColor;
    saveSettings();
    emit mediaSettingsChanged();
}

void FocusTimerViewModel::clearAudioTrack() {
    m_audioTrackUrl.clear();
    saveSettings();
    emit mediaSettingsChanged();
    emit audioPlaybackStateChanged();
}

void FocusTimerViewModel::onTick() {
    if (!m_isRunning) return;

    emit timeChanged();

    const qint64 targetMs = activeTargetDurationMs();
    if (targetMs > 0 && currentElapsedMs() >= targetMs) {
        m_tickTimer.stop();
        m_isRunning = false;
        m_accumulatedMs = 0;

        if (m_phase == Focus) {
            const int loggedSecs = static_cast<int>(targetMs / 1000LL);
            logCompletedFocusSeconds(loggedSecs);
            transitionToPhase(Break);
            start(); // Auto-start break countdown
        } else {
            m_customBreakDurationSeconds = 0;
            emit breakFinished();
            transitionToPhase(Focus);
        }
    }
}

void FocusTimerViewModel::logCompletedFocusSeconds(int durationSeconds) {
    if (durationSeconds <= 0 || m_activeSubjectId <= 0) return;

    const std::int64_t endEpoch = QDateTime::currentSecsSinceEpoch();
    const std::int64_t startEpoch = (m_sessionStartedEpoch > 0)
        ? m_sessionStartedEpoch
        : (endEpoch - durationSeconds);

    StudySession session;
    session.subjectId       = m_activeSubjectId;
    session.mode            = (m_mode == Pomodoro) ? SessionMode::Pomodoro : SessionMode::Flowtime;
    session.startedAt       = startEpoch;
    session.endedAt         = endEpoch;
    session.durationSeconds = durationSeconds;
    session.studyDate       = QDate::currentDate().toString(Qt::ISODate);

    if (m_db.recordStudySession(session).has_value()) {
        m_subjectModel.appendStudiedSeconds(m_activeSubjectId, durationSeconds);
        emit focusSessionLogged(m_activeSubjectName, std::max(1, durationSeconds / 60));
    }
}

void FocusTimerViewModel::transitionToPhase(TimerPhase nextPhase) {
    m_accumulatedMs = 0;
    m_sessionStartedEpoch = 0;
    if (m_phase != nextPhase) {
        m_phase = nextPhase;
        emit phaseChanged();
    }
    emit runningChanged();
    emit timeChanged();
    emit audioPlaybackStateChanged();
}

void FocusTimerViewModel::loadSettings() {
    QSettings settings;
    m_pomodoroFocusMinutes = settings.value(QStringLiteral("timer/focusMinutes"), 25).toInt();
    m_pomodoroBreakMinutes = settings.value(QStringLiteral("timer/breakMinutes"), 5).toInt();
    m_flowtimeBreakDivisor = settings.value(QStringLiteral("timer/flowtimeDivisor"), 5).toInt();

    m_backgroundType     = static_cast<BackgroundType>(settings.value(QStringLiteral("media/bgType"), 0).toInt());
    m_backgroundColor    = settings.value(QStringLiteral("media/bgColor"), QStringLiteral("#0F1117")).toString();
    m_backgroundMediaUrl = settings.value(QStringLiteral("media/bgUrl")).toUrl();
    m_audioTrackUrl      = settings.value(QStringLiteral("media/audioUrl")).toUrl();
    m_audioVolume        = settings.value(QStringLiteral("media/audioVolume"), 0.65).toDouble();
    m_audioMuted         = settings.value(QStringLiteral("media/audioMuted"), false).toBool();

    const qint64 savedSubjectId = settings.value(QStringLiteral("timer/subjectId"), -1).toLongLong();
    if (savedSubjectId > 0) {
        for (int i = 0; i < m_subjectModel.rowCount(); ++i) {
            const QModelIndex idx = m_subjectModel.index(i, 0);
            if (m_subjectModel.data(idx, SubjectModel::IdRole).toLongLong() == savedSubjectId) {
                m_activeSubjectId    = savedSubjectId;
                m_activeSubjectName  = m_subjectModel.data(idx, SubjectModel::NameRole).toString();
                m_activeSubjectColor = m_subjectModel.data(idx, SubjectModel::ColorHexRole).toString();
                break;
            }
        }
    }
}

void FocusTimerViewModel::saveSettings() const {
    QSettings settings;
    settings.setValue(QStringLiteral("timer/focusMinutes"), m_pomodoroFocusMinutes);
    settings.setValue(QStringLiteral("timer/breakMinutes"), m_pomodoroBreakMinutes);
    settings.setValue(QStringLiteral("timer/flowtimeDivisor"), m_flowtimeBreakDivisor);
    settings.setValue(QStringLiteral("timer/subjectId"), static_cast<qlonglong>(m_activeSubjectId));

    settings.setValue(QStringLiteral("media/bgType"), static_cast<int>(m_backgroundType));
    settings.setValue(QStringLiteral("media/bgColor"), m_backgroundColor);
    settings.setValue(QStringLiteral("media/bgUrl"), m_backgroundMediaUrl);
    settings.setValue(QStringLiteral("media/audioUrl"), m_audioTrackUrl);
    settings.setValue(QStringLiteral("media/audioVolume"), m_audioVolume);
    settings.setValue(QStringLiteral("media/audioMuted"), m_audioMuted);
}

} // namespace cadence
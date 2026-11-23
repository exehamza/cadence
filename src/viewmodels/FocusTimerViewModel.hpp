#pragma once

#include "core/Types.hpp"
#include "db/DatabaseManager.hpp"
#include "models/SubjectModel.hpp"

#include <QElapsedTimer>
#include <QObject>
#include <QString>
#include <QTimer>
#include <QUrl>
#include <cstdint>

namespace cadence {

class FocusTimerViewModel : public QObject {
    Q_OBJECT

    // Timer State & Mode
    Q_PROPERTY(TimerMode mode READ mode WRITE setMode NOTIFY modeChanged)
    Q_PROPERTY(TimerPhase phase READ phase NOTIFY phaseChanged)
    Q_PROPERTY(bool isRunning READ isRunning NOTIFY runningChanged)
    Q_PROPERTY(bool isPaused READ isPaused NOTIFY runningChanged)

    // Clock & Progress Readouts
    Q_PROPERTY(int elapsedSeconds READ elapsedSeconds NOTIFY timeChanged)
    Q_PROPERTY(int remainingSeconds READ remainingSeconds NOTIFY timeChanged)
    Q_PROPERTY(QString formattedTime READ formattedTime NOTIFY timeChanged)
    Q_PROPERTY(double progress READ progress NOTIFY timeChanged)

    // Pomodoro & Flowtime Configuration
    Q_PROPERTY(int pomodoroFocusMinutes READ pomodoroFocusMinutes WRITE setPomodoroFocusMinutes NOTIFY configChanged)
    Q_PROPERTY(int pomodoroBreakMinutes READ pomodoroBreakMinutes WRITE setPomodoroBreakMinutes NOTIFY configChanged)
    Q_PROPERTY(int flowtimeBreakDivisor READ flowtimeBreakDivisor WRITE setFlowtimeBreakDivisor NOTIFY configChanged)

    // Flowtime Proportional Break Offer
    Q_PROPERTY(bool hasPendingBreakOffer READ hasPendingBreakOffer NOTIFY breakOfferChanged)
    Q_PROPERTY(int suggestedBreakMinutes READ suggestedBreakMinutes NOTIFY breakOfferChanged)

    // Linked Subject
    Q_PROPERTY(qint64 activeSubjectId READ activeSubjectId NOTIFY activeSubjectChanged)
    Q_PROPERTY(QString activeSubjectName READ activeSubjectName NOTIFY activeSubjectChanged)
    Q_PROPERTY(QString activeSubjectColor READ activeSubjectColor NOTIFY activeSubjectChanged)

    // Backdrop & Ambient Audio Preferences
    Q_PROPERTY(BackgroundType backgroundType READ backgroundType WRITE setBackgroundType NOTIFY mediaSettingsChanged)
    Q_PROPERTY(QString backgroundColor READ backgroundColor WRITE setBackgroundColor NOTIFY mediaSettingsChanged)
    Q_PROPERTY(QUrl backgroundMediaUrl READ backgroundMediaUrl WRITE setBackgroundMediaUrl NOTIFY mediaSettingsChanged)
    Q_PROPERTY(QUrl audioTrackUrl READ audioTrackUrl WRITE setAudioTrackUrl NOTIFY mediaSettingsChanged)
    Q_PROPERTY(QString audioTrackName READ audioTrackName NOTIFY mediaSettingsChanged)
    Q_PROPERTY(double audioVolume READ audioVolume WRITE setAudioVolume NOTIFY mediaSettingsChanged)
    Q_PROPERTY(bool audioMuted READ audioMuted WRITE setAudioMuted NOTIFY mediaSettingsChanged)
    Q_PROPERTY(bool shouldPlayFocusAudio READ shouldPlayFocusAudio NOTIFY audioPlaybackStateChanged)

public:
    enum TimerMode {
        Pomodoro = 0,
        Flowtime = 1
    };
    Q_ENUM(TimerMode)

    enum TimerPhase {
        Focus = 0,
        Break = 1
    };
    Q_ENUM(TimerPhase)

    enum BackgroundType {
        SolidColor = 0,
        Image      = 1,
        Video      = 2
    };
    Q_ENUM(BackgroundType)

    explicit FocusTimerViewModel(DatabaseManager& db,
                                 SubjectModel& subjectModel,
                                 QObject* parent = nullptr);

    // Getters
    [[nodiscard]] TimerMode mode() const noexcept { return m_mode; }
    [[nodiscard]] TimerPhase phase() const noexcept { return m_phase; }
    [[nodiscard]] bool isRunning() const noexcept { return m_isRunning; }
    [[nodiscard]] bool isPaused() const noexcept { return !m_isRunning && currentElapsedMs() > 0; }

    [[nodiscard]] int elapsedSeconds() const noexcept;
    [[nodiscard]] int remainingSeconds() const noexcept;
    [[nodiscard]] QString formattedTime() const;
    [[nodiscard]] double progress() const noexcept;

    [[nodiscard]] int pomodoroFocusMinutes() const noexcept { return m_pomodoroFocusMinutes; }
    [[nodiscard]] int pomodoroBreakMinutes() const noexcept { return m_pomodoroBreakMinutes; }
    [[nodiscard]] int flowtimeBreakDivisor() const noexcept { return m_flowtimeBreakDivisor; }

    [[nodiscard]] bool hasPendingBreakOffer() const noexcept { return m_suggestedBreakSeconds > 0; }
    [[nodiscard]] int suggestedBreakMinutes() const noexcept {
        return std::max(1, (m_suggestedBreakSeconds + 30) / 60);
    }

    [[nodiscard]] qint64 activeSubjectId() const noexcept { return m_activeSubjectId; }
    [[nodiscard]] QString activeSubjectName() const noexcept { return m_activeSubjectName; }
    [[nodiscard]] QString activeSubjectColor() const noexcept { return m_activeSubjectColor; }

    [[nodiscard]] BackgroundType backgroundType() const noexcept { return m_backgroundType; }
    [[nodiscard]] QString backgroundColor() const noexcept { return m_backgroundColor; }
    [[nodiscard]] QUrl backgroundMediaUrl() const noexcept { return m_backgroundMediaUrl; }
    [[nodiscard]] QUrl audioTrackUrl() const noexcept { return m_audioTrackUrl; }
    [[nodiscard]] QString audioTrackName() const noexcept;
    [[nodiscard]] double audioVolume() const noexcept { return m_audioVolume; }
    [[nodiscard]] bool audioMuted() const noexcept { return m_audioMuted; }
    [[nodiscard]] bool shouldPlayFocusAudio() const noexcept;

    // Setters
    void setMode(TimerMode newMode);
    void setPomodoroFocusMinutes(int minutes);
    void setPomodoroBreakMinutes(int minutes);
    void setFlowtimeBreakDivisor(int divisor);

    void setBackgroundType(BackgroundType type);
    void setBackgroundColor(const QString& hexColor);
    void setBackgroundMediaUrl(const QUrl& url);
    void setAudioTrackUrl(const QUrl& url);
    void setAudioVolume(double volume);
    void setAudioMuted(bool muted);

    // Invokable Transport & Session Commands
    Q_INVOKABLE void start();
    Q_INVOKABLE void pause();
    Q_INVOKABLE void toggleStartPause();
    Q_INVOKABLE void reset();
    Q_INVOKABLE void completeAndLogSession();
    Q_INVOKABLE void acceptFlowtimeBreak();
    Q_INVOKABLE void dismissFlowtimeBreak();
    Q_INVOKABLE void skipBreak();

    Q_INVOKABLE void selectSubject(qint64 subjectId, const QString& name, const QString& colorHex);
    Q_INVOKABLE void clearBackgroundMedia();
    Q_INVOKABLE void clearAudioTrack();

signals:
    void modeChanged();
    void phaseChanged();
    void runningChanged();
    void timeChanged();
    void configChanged();
    void breakOfferChanged();
    void activeSubjectChanged();
    void mediaSettingsChanged();
    void audioPlaybackStateChanged();

    void focusSessionLogged(const QString& subjectName, int minutesLogged);
    void breakFinished();

private slots:
    void onTick();

private:
    [[nodiscard]] qint64 currentElapsedMs() const noexcept;
    [[nodiscard]] qint64 activeTargetDurationMs() const noexcept;
    void logCompletedFocusSeconds(int durationSeconds);
    void transitionToPhase(TimerPhase nextPhase);
    void loadSettings();
    void saveSettings() const;

    DatabaseManager& m_db;
    SubjectModel&    m_subjectModel;

    QTimer        m_tickTimer;
    QElapsedTimer m_segmentClock;

    TimerMode  m_mode{Pomodoro};
    TimerPhase m_phase{Focus};
    bool       m_isRunning{false};

    qint64       m_accumulatedMs{0};
    std::int64_t m_sessionStartedEpoch{0};

    int m_pomodoroFocusMinutes{25};
    int m_pomodoroBreakMinutes{5};
    int m_flowtimeBreakDivisor{5};

    int m_customBreakDurationSeconds{0};
    int m_suggestedBreakSeconds{0};

    std::int64_t m_activeSubjectId{-1};
    QString      m_activeSubjectName{QStringLiteral("Unassigned")};
    QString      m_activeSubjectColor{QStringLiteral("#6366F1")};

    BackgroundType m_backgroundType{SolidColor};
    QString        m_backgroundColor{QStringLiteral("#0F1117")};
    QUrl           m_backgroundMediaUrl;
    QUrl           m_audioTrackUrl;
    double         m_audioVolume{0.65};
    bool           m_audioMuted{false};
};

} // namespace cadence
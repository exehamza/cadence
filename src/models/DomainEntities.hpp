#pragma once

#include "core/Types.hpp"

#include <QString>
#include <cstdint>
#include <optional>

namespace cadence {

struct Subject {
    std::int64_t id{0};
    QString      name;
    QString      colorHex{QStringLiteral("#4F46E5")};
    std::int64_t totalSecondsStudied{0};
    int          targetWeeklyMinutes{0};
    bool         archived{false};

    [[nodiscard]] double totalHours() const noexcept {
        return static_cast<double>(totalSecondsStudied) / 3600.0;
    }
};

struct StudySession {
    std::int64_t id{0};
    std::int64_t subjectId{0};
    SessionMode  mode{SessionMode::Pomodoro};
    std::int64_t startedAt{0};
    std::int64_t endedAt{0};
    int          durationSeconds{0};
    QString      studyDate; // 'YYYY-MM-DD'
    QString      notes;
};

struct Reminder {
    std::int64_t                id{0};
    std::optional<std::int64_t> subjectId;
    QString                     subjectName;
    QString                     subjectColorHex{QStringLiteral("#64748B")};
    QString                     title;
    QString                     description;
    ReminderCategory            category{ReminderCategory::Task};
    std::optional<std::int64_t> dueAt;
    QString                     dueDate; // 'YYYY-MM-DD' or empty
    bool                        isCompleted{false};
};

struct Checklist {
    std::int64_t                id{0};
    std::optional<std::int64_t> subjectId;
    QString                     title;
    int                         sortOrder{0};
    int                         completedCount{0};
    int                         totalCount{0};
};

struct ChecklistItem {
    std::int64_t                id{0};
    std::int64_t                checklistId{0};
    std::optional<std::int64_t> parentItemId;
    QString                     title;
    bool                        isCompleted{false};
    int                         sortOrder{0};
    int                         depth{0};
    bool                        hasChildren{false};
};

struct CalendarEvent {
    std::int64_t                id{0};
    std::optional<std::int64_t> subjectId;
    QString                     title;
    QString                     location;
    QString                     colorHex{QStringLiteral("#4F46E5")};
    std::int64_t                startAt{0};
    std::int64_t                endAt{0};
    bool                        isAllDay{false};
    QString                     eventDate; // 'YYYY-MM-DD'
};

// --- Phase 4 Analytics DTOs ---

struct SubjectStudyStat {
    std::int64_t subjectId{0};
    QString      subjectName;
    QString      colorHex{QStringLiteral("#4F46E5")};
    std::int64_t totalSeconds{0};
    int          targetWeeklyMinutes{0};
};

struct DailyStudyStat {
    QString      dateIso; // 'YYYY-MM-DD'
    std::int64_t totalSeconds{0};
};

struct UpcomingExam {
    std::int64_t reminderId{0};
    QString      title;
    QString      subjectName;
    QString      colorHex{QStringLiteral("#F43F5E")};
    std::int64_t dueAtEpoch{0};
    QString      dueDateIso;
};

} // namespace cadence
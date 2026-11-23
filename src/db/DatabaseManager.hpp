#pragma once

#include "models/DomainEntities.hpp"

#include <QSqlDatabase>
#include <QString>
#include <optional>
#include <vector>

namespace cadence {

class DatabaseManager {
public:
    DatabaseManager() = default;
    ~DatabaseManager();

    DatabaseManager(const DatabaseManager&)            = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;

    [[nodiscard]] bool initialize(const QString& dbFilePath);

    // --- Subjects ---
    [[nodiscard]] std::vector<Subject> fetchAllSubjects(bool includeArchived = false) const;
    [[nodiscard]] std::optional<Subject> createSubject(const QString& name,
                                                       const QString& colorHex,
                                                       int targetWeeklyMinutes = 0);
    [[nodiscard]] bool updateSubject(std::int64_t id,
                                     const QString& name,
                                     const QString& colorHex,
                                     int targetWeeklyMinutes);
    [[nodiscard]] bool deleteSubject(std::int64_t id);

    // --- Study Sessions ---
    [[nodiscard]] std::optional<std::int64_t> recordStudySession(const StudySession& session);

    // --- Reminders ---
    [[nodiscard]] std::vector<Reminder> fetchAllReminders() const;
    [[nodiscard]] std::vector<Reminder> fetchRemindersInRange(const QString& startDateIso,
                                                              const QString& endDateIso) const;
    [[nodiscard]] std::optional<Reminder> createReminder(const Reminder& draft);
    [[nodiscard]] bool setReminderCompleted(std::int64_t id, bool completed);
    [[nodiscard]] std::optional<Reminder> pushReminderToTomorrow(std::int64_t id);
    [[nodiscard]] bool deleteReminder(std::int64_t id);

    // --- Standalone Calendar Events ---
    [[nodiscard]] std::vector<CalendarEvent> fetchCalendarEventsInRange(const QString& startDateIso,
                                                                        const QString& endDateIso) const;
    [[nodiscard]] std::optional<CalendarEvent> createCalendarEvent(const CalendarEvent& draft);
    [[nodiscard]] bool deleteCalendarEvent(std::int64_t id);

    // --- Checklists & Nested Items ---
    [[nodiscard]] std::vector<Checklist> fetchAllChecklists() const;
    [[nodiscard]] std::optional<Checklist> createChecklist(const QString& title,
                                                           std::optional<std::int64_t> subjectId);
    [[nodiscard]] bool deleteChecklist(std::int64_t checklistId);

    [[nodiscard]] std::vector<ChecklistItem> fetchChecklistTree(std::int64_t checklistId) const;
    [[nodiscard]] std::optional<ChecklistItem> createChecklistItem(
        std::int64_t checklistId,
        std::optional<std::int64_t> parentItemId,
        const QString& title);
    [[nodiscard]] bool setChecklistItemCompleted(std::int64_t itemId, bool completed);
    [[nodiscard]] bool deleteChecklistItem(std::int64_t itemId);

    // --- Phase 4 Analytics Aggregations ---
    [[nodiscard]] std::vector<SubjectStudyStat> fetchSubjectStudyDistribution(
        const QString& startDateIso = QString()) const;
    [[nodiscard]] std::vector<DailyStudyStat> fetchDailyStudyActivity(
        const QString& startDateIso,
        const QString& endDateIso) const;
    [[nodiscard]] std::vector<UpcomingExam> fetchUpcomingExaminations() const;

private:
    class ScopedTransaction {
    public:
        explicit ScopedTransaction(QSqlDatabase& db);
        ~ScopedTransaction();
        [[nodiscard]] bool commit();

    private:
        QSqlDatabase& m_db;
        bool          m_active{false};
        bool          m_committed{false};
    };

    [[nodiscard]] bool applyPragmas();
    [[nodiscard]] bool runSchemaMigrations();

    QSqlDatabase m_db;
    QString      m_connectionName{QStringLiteral("cadence_primary_sqlite")};
};

} // namespace cadence
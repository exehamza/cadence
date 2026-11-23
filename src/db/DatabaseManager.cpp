cat << 'EOF' > src/db/DatabaseManager.cpp
#include "db/DatabaseManager.hpp"

#include <QDate>
#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>
#include <functional>
#include <unordered_map>

namespace cadence {

namespace {

constexpr const char* kFallbackSchemaSql = R"SQL(
PRAGMA foreign_keys = ON;

CREATE TABLE IF NOT EXISTS subjects (
    id                    INTEGER PRIMARY KEY AUTOINCREMENT,
    name                  TEXT    NOT NULL UNIQUE COLLATE NOCASE,
    color_hex             TEXT    NOT NULL DEFAULT '#4F46E5',
    target_weekly_minutes INTEGER NOT NULL DEFAULT 0,
    created_at            INTEGER NOT NULL,
    archived              INTEGER NOT NULL DEFAULT 0
);

CREATE TABLE IF NOT EXISTS study_sessions (
    id               INTEGER PRIMARY KEY AUTOINCREMENT,
    subject_id       INTEGER NOT NULL,
    mode             TEXT    NOT NULL CHECK(mode IN ('pomodoro', 'flowtime')),
    started_at       INTEGER NOT NULL,
    ended_at         INTEGER NOT NULL,
    duration_seconds INTEGER NOT NULL CHECK(duration_seconds > 0),
    study_date       TEXT    NOT NULL,
    notes            TEXT    NOT NULL DEFAULT '',
    FOREIGN KEY (subject_id) REFERENCES subjects(id) ON DELETE CASCADE
);

CREATE INDEX IF NOT EXISTS idx_sessions_study_date ON study_sessions(study_date);
CREATE INDEX IF NOT EXISTS idx_sessions_subject_id ON study_sessions(subject_id);

CREATE TABLE IF NOT EXISTS reminders (
    id           INTEGER PRIMARY KEY AUTOINCREMENT,
    subject_id   INTEGER NULL,
    title        TEXT    NOT NULL,
    description  TEXT    NOT NULL DEFAULT '',
    category     TEXT    NOT NULL CHECK(category IN ('task', 'assignment', 'examination')),
    due_at       INTEGER NULL,
    due_date     TEXT    NULL,
    is_completed INTEGER NOT NULL DEFAULT 0,
    completed_at INTEGER NULL,
    created_at   INTEGER NOT NULL,
    FOREIGN KEY (subject_id) REFERENCES subjects(id) ON DELETE SET NULL
);

CREATE INDEX IF NOT EXISTS idx_reminders_due_date     ON reminders(due_date);
CREATE INDEX IF NOT EXISTS idx_reminders_category_due ON reminders(category, due_at);

CREATE TABLE IF NOT EXISTS calendar_events (
    id         INTEGER PRIMARY KEY AUTOINCREMENT,
    subject_id INTEGER NULL,
    title      TEXT    NOT NULL,
    location   TEXT    NOT NULL DEFAULT '',
    color_hex  TEXT    NULL,
    start_at   INTEGER NOT NULL,
    end_at     INTEGER NOT NULL,
    is_all_day INTEGER NOT NULL DEFAULT 0,
    event_date TEXT    NOT NULL,
    FOREIGN KEY (subject_id) REFERENCES subjects(id) ON DELETE SET NULL
);

CREATE INDEX IF NOT EXISTS idx_events_date ON calendar_events(event_date);

CREATE TABLE IF NOT EXISTS checklists (
    id         INTEGER PRIMARY KEY AUTOINCREMENT,
    subject_id INTEGER NULL,
    title      TEXT    NOT NULL,
    sort_order INTEGER NOT NULL DEFAULT 0,
    created_at INTEGER NOT NULL,
    FOREIGN KEY (subject_id) REFERENCES subjects(id) ON DELETE SET NULL
);

CREATE TABLE IF NOT EXISTS checklist_items (
    id             INTEGER PRIMARY KEY AUTOINCREMENT,
    checklist_id   INTEGER NOT NULL,
    parent_item_id INTEGER NULL,
    title          TEXT    NOT NULL,
    is_completed   INTEGER NOT NULL DEFAULT 0,
    sort_order     INTEGER NOT NULL DEFAULT 0,
    FOREIGN KEY (checklist_id)   REFERENCES checklists(id)      ON DELETE CASCADE,
    FOREIGN KEY (parent_item_id) REFERENCES checklist_items(id) ON DELETE CASCADE
);

CREATE INDEX IF NOT EXISTS idx_checklist_items_tree
    ON checklist_items(checklist_id, parent_item_id, sort_order);
)SQL";

} // namespace

DatabaseManager::ScopedTransaction::ScopedTransaction(QSqlDatabase& db)
    : m_db(db) {
    m_active = m_db.transaction();
    if (!m_active) {
        qWarning() << "[DB] Failed to begin transaction:" << m_db.lastError().text();
    }
}

DatabaseManager::ScopedTransaction::~ScopedTransaction() {
    if (m_active && !m_committed) {
        m_db.rollback();
    }
}

bool DatabaseManager::ScopedTransaction::commit() {
    if (!m_active) return false;
    m_committed = m_db.commit();
    if (!m_committed) {
        qWarning() << "[DB] Transaction commit failed:" << m_db.lastError().text();
    }
    return m_committed;
}

DatabaseManager::~DatabaseManager() {
    if (m_db.isOpen()) {
        m_db.close();
    }
    m_db = QSqlDatabase();
    QSqlDatabase::removeDatabase(m_connectionName);
}

bool DatabaseManager::initialize(const QString& dbFilePath) {
    const QFileInfo fileInfo(dbFilePath);
    if (!QDir().mkpath(fileInfo.absolutePath())) {
        qCritical() << "[DB] Unable to create data directory:" << fileInfo.absolutePath();
        return false;
    }

    m_db = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"), m_connectionName);
    m_db.setDatabaseName(dbFilePath);

    if (!m_db.open()) {
        qCritical() << "[DB] Failed to open SQLite database:" << m_db.lastError().text();
        return false;
    }

    return applyPragmas() && runSchemaMigrations();
}

bool DatabaseManager::applyPragmas() {
    QSqlQuery q(m_db);
    const char* pragmas[] = {
        "PRAGMA journal_mode = WAL;",
        "PRAGMA synchronous = NORMAL;",
        "PRAGMA foreign_keys = ON;",
        "PRAGMA temp_store = MEMORY;",
        "PRAGMA cache_size = -4000;"
    };

    for (const char* stmt : pragmas) {
        if (!q.exec(QString::fromLatin1(stmt))) {
            qCritical() << "[DB] PRAGMA failed:" << stmt << q.lastError().text();
            return false;
        }
    }
    return true;
}

bool DatabaseManager::runSchemaMigrations() {
    QString sqlScript;
    QFile schemaFile(QStringLiteral(":/qt/qml/Cadence/resources/sql/schema.sql"));
    if (schemaFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        sqlScript = QString::fromUtf8(schemaFile.readAll()).trimmed();
    }

    if (sqlScript.isEmpty()) {
        sqlScript = QString::fromUtf8(kFallbackSchemaSql);
    }

    QString cleanedSql;
    const QStringList lines = sqlScript.split(QLatin1Char('\n'));
    for (const QString& line : lines) {
        const qsizetype commentIdx = line.indexOf(QStringLiteral("--"));
        cleanedSql += (commentIdx >= 0 ? line.left(commentIdx) : line) + QLatin1Char('\n');
    }

    const QStringList statements = cleanedSql.split(QLatin1Char(';'), Qt::SkipEmptyParts);

    ScopedTransaction tx(m_db);
    QSqlQuery q(m_db);

    for (const QString& rawStmt : statements) {
        const QString trimmed = rawStmt.trimmed();
        if (trimmed.isEmpty()) continue;

        if (!q.exec(trimmed)) {
            qCritical() << "[DB] Schema statement failed:" << trimmed
                        << "\nError:" << q.lastError().text();
            return false;
        }
    }

    return tx.commit();
}

// ============================================================================
// Subjects
// ============================================================================

std::vector<Subject> DatabaseManager::fetchAllSubjects(bool includeArchived) const {
    std::vector<Subject> results;
    QSqlQuery q(m_db);

    const QString sql = includeArchived
        ? QStringLiteral(
              "SELECT s.id, s.name, s.color_hex, s.target_weekly_minutes, s.archived, "
              "       COALESCE(SUM(ss.duration_seconds), 0) AS total_seconds "
              "FROM subjects s "
              "LEFT JOIN study_sessions ss ON ss.subject_id = s.id "
              "GROUP BY s.id "
              "ORDER BY s.name ASC;")
        : QStringLiteral(
              "SELECT s.id, s.name, s.color_hex, s.target_weekly_minutes, s.archived, "
              "       COALESCE(SUM(ss.duration_seconds), 0) AS total_seconds "
              "FROM subjects s "
              "LEFT JOIN study_sessions ss ON ss.subject_id = s.id "
              "WHERE s.archived = 0 "
              "GROUP BY s.id "
              "ORDER BY s.name ASC;");

    if (!q.exec(sql)) {
        qWarning() << "[DB] fetchAllSubjects failed:" << q.lastError().text();
        return results;
    }

    while (q.next()) {
        Subject s;
        s.id                  = q.value(0).toLongLong();
        s.name                = q.value(1).toString();
        s.colorHex            = q.value(2).toString();
        s.totalSecondsStudied = q.value(5).toLongLong();
        s.targetWeeklyMinutes = q.value(3).toInt();
        s.archived            = q.value(4).toInt() != 0;
        results.push_back(std::move(s));
    }
    return results;
}

std::optional<Subject> DatabaseManager::createSubject(const QString& name,
                                                      const QString& colorHex,
                                                      int targetWeeklyMinutes) {
    QSqlQuery q(m_db);
    q.prepare(QStringLiteral(
        "INSERT INTO subjects (name, color_hex, target_weekly_minutes, created_at) "
        "VALUES (:name, :color, :target, :created);"));

    const std::int64_t now = QDateTime::currentSecsSinceEpoch();
    q.bindValue(QStringLiteral(":name"), name.trimmed());
    q.bindValue(QStringLiteral(":color"), colorHex);
    q.bindValue(QStringLiteral(":target"), targetWeeklyMinutes);
    q.bindValue(QStringLiteral(":created"), static_cast<qlonglong>(now));

    if (!q.exec()) {
        qWarning() << "[DB] createSubject failed:" << q.lastError().text();
        return std::nullopt;
    }

    Subject s;
    s.id                  = q.lastInsertId().toLongLong();
    s.name                = name.trimmed();
    s.colorHex            = colorHex;
    s.totalSecondsStudied = 0;
    s.targetWeeklyMinutes = targetWeeklyMinutes;
    s.archived            = false;
    return s;
}

bool DatabaseManager::updateSubject(std::int64_t id,
                                    const QString& name,
                                    const QString& colorHex,
                                    int targetWeeklyMinutes) {
    QSqlQuery q(m_db);
    q.prepare(QStringLiteral(
        "UPDATE subjects "
        "SET name = :name, color_hex = :color, target_weekly_minutes = :target "
        "WHERE id = :id;"));
    q.bindValue(QStringLiteral(":name"), name.trimmed());
    q.bindValue(QStringLiteral(":color"), colorHex);
    q.bindValue(QStringLiteral(":target"), targetWeeklyMinutes);
    q.bindValue(QStringLiteral(":id"), static_cast<qlonglong>(id));

    if (!q.exec()) {
        qWarning() << "[DB] updateSubject failed:" << q.lastError().text();
        return false;
    }
    return q.numRowsAffected() > 0;
}

bool DatabaseManager::deleteSubject(std::int64_t id) {
    QSqlQuery q(m_db);
    q.prepare(QStringLiteral("DELETE FROM subjects WHERE id = :id;"));
    q.bindValue(QStringLiteral(":id"), static_cast<qlonglong>(id));
    if (!q.exec()) {
        qWarning() << "[DB] deleteSubject failed:" << q.lastError().text();
        return false;
    }
    return q.numRowsAffected() > 0;
}

// ============================================================================
// Study Sessions
// ============================================================================

std::optional<std::int64_t> DatabaseManager::recordStudySession(const StudySession& session) {
    if (session.durationSeconds <= 0) return std::nullopt;

    QSqlQuery q(m_db);
    q.prepare(QStringLiteral(
        "INSERT INTO study_sessions "
        "(subject_id, mode, started_at, ended_at, duration_seconds, study_date, notes) "
        "VALUES (:sub_id, :mode, :start, :end, :dur, :date, :notes);"));

    const auto modeSv = toSqlString(session.mode);
    q.bindValue(QStringLiteral(":sub_id"), static_cast<qlonglong>(session.subjectId));
    q.bindValue(QStringLiteral(":mode"), QString::fromUtf8(modeSv.data(), static_cast<qsizetype>(modeSv.size())));
    q.bindValue(QStringLiteral(":start"), static_cast<qlonglong>(session.startedAt));
    q.bindValue(QStringLiteral(":end"), static_cast<qlonglong>(session.endedAt));
    q.bindValue(QStringLiteral(":dur"), session.durationSeconds);
    q.bindValue(QStringLiteral(":date"), session.studyDate);
    q.bindValue(QStringLiteral(":notes"), session.notes);

    if (!q.exec()) {
        qWarning() << "[DB] recordStudySession failed:" << q.lastError().text();
        return std::nullopt;
    }
    return q.lastInsertId().toLongLong();
}

// ============================================================================
// Reminders
// ============================================================================

std::vector<Reminder> DatabaseManager::fetchAllReminders() const {
    std::vector<Reminder> results;
    QSqlQuery q(m_db);

    const QString sql = QStringLiteral(
        "SELECT r.id, r.subject_id, COALESCE(s.name, ''), COALESCE(s.color_hex, '#64748B'), "
        "       r.title, r.description, r.category, r.due_at, COALESCE(r.due_date, ''), "
        "       r.is_completed "
        "FROM reminders r "
        "LEFT JOIN subjects s ON s.id = r.subject_id "
        "ORDER BY r.is_completed ASC, r.due_at IS NULL ASC, r.due_at ASC, r.id DESC;");

    if (!q.exec(sql)) {
        qWarning() << "[DB] fetchAllReminders failed:" << q.lastError().text();
        return results;
    }

    while (q.next()) {
        const QByteArray catBytes = q.value(6).toString().toUtf8();
        Reminder r;
        r.id              = q.value(0).toLongLong();
        r.subjectId       = q.value(1).isNull() ? std::nullopt
                                                : std::optional<std::int64_t>(q.value(1).toLongLong());
        r.subjectName     = q.value(2).toString();
        r.subjectColorHex = q.value(3).toString();
        r.title           = q.value(4).toString();
        r.description     = q.value(5).toString();
        r.category        = reminderCategoryFromSql(
            std::string_view(catBytes.constData(), static_cast<std::size_t>(catBytes.size())));
        r.dueAt           = q.value(7).isNull() ? std::nullopt
                                                : std::optional<std::int64_t>(q.value(7).toLongLong());
        r.dueDate         = q.value(8).toString();
        r.isCompleted     = q.value(9).toInt() != 0;
        results.push_back(std::move(r));
    }
    return results;
}

std::vector<Reminder> DatabaseManager::fetchRemindersInRange(const QString& startDateIso,
                                                             const QString& endDateIso) const {
    std::vector<Reminder> results;
    QSqlQuery q(m_db);
    q.prepare(QStringLiteral(
        "SELECT r.id, r.subject_id, COALESCE(s.name, ''), COALESCE(s.color_hex, '#64748B'), "
        "       r.title, r.description, r.category, r.due_at, r.due_date, r.is_completed "
        "FROM reminders r "
        "LEFT JOIN subjects s ON s.id = r.subject_id "
        "WHERE r.due_date IS NOT NULL AND r.due_date >= :start AND r.due_date <= :end "
        "ORDER BY r.due_date ASC, r.due_at ASC, r.id ASC;"));
    q.bindValue(QStringLiteral(":start"), startDateIso);
    q.bindValue(QStringLiteral(":end"), endDateIso);

    if (!q.exec()) {
        qWarning() << "[DB] fetchRemindersInRange failed:" << q.lastError().text();
        return results;
    }

    while (q.next()) {
        const QByteArray catBytes = q.value(6).toString().toUtf8();
        Reminder r;
        r.id              = q.value(0).toLongLong();
        r.subjectId       = q.value(1).isNull() ? std::nullopt
                                                : std::optional<std::int64_t>(q.value(1).toLongLong());
        r.subjectName     = q.value(2).toString();
        r.subjectColorHex = q.value(3).toString();
        r.title           = q.value(4).toString();
        r.description     = q.value(5).toString();
        r.category        = reminderCategoryFromSql(
            std::string_view(catBytes.constData(), static_cast<std::size_t>(catBytes.size())));
        r.dueAt           = q.value(7).isNull() ? std::nullopt
                                                : std::optional<std::int64_t>(q.value(7).toLongLong());
        r.dueDate         = q.value(8).toString();
        r.isCompleted     = q.value(9).toInt() != 0;
        results.push_back(std::move(r));
    }
    return results;
}

std::optional<Reminder> DatabaseManager::createReminder(const Reminder& draft) {
    QSqlQuery q(m_db);
    q.prepare(QStringLiteral(
        "INSERT INTO reminders "
        "(subject_id, title, description, category, due_at, due_date, is_completed, created_at) "
        "VALUES (:sub_id, :title, :desc, :cat, :due_at, :due_date, 0, :created);"));

    const auto catSv = toSqlString(draft.category);
    q.bindValue(QStringLiteral(":sub_id"),
                draft.subjectId ? QVariant(static_cast<qlonglong>(*draft.subjectId)) : QVariant());
    q.bindValue(QStringLiteral(":title"), draft.title.trimmed());
    q.bindValue(QStringLiteral(":desc"), draft.description);
    q.bindValue(QStringLiteral(":cat"), QString::fromUtf8(catSv.data(), static_cast<qsizetype>(catSv.size())));
    q.bindValue(QStringLiteral(":due_at"),
                draft.dueAt ? QVariant(static_cast<qlonglong>(*draft.dueAt)) : QVariant());
    q.bindValue(QStringLiteral(":due_date"),
                draft.dueDate.isEmpty() ? QVariant() : QVariant(draft.dueDate));
    q.bindValue(QStringLiteral(":created"), static_cast<qlonglong>(QDateTime::currentSecsSinceEpoch()));

    if (!q.exec()) {
        qWarning() << "[DB] createReminder failed:" << q.lastError().text();
        return std::nullopt;
    }

    Reminder created = draft;
    created.id = q.lastInsertId().toLongLong();
    created.title = draft.title.trimmed();
    created.isCompleted = false;
    return created;
}

bool DatabaseManager::setReminderCompleted(std::int64_t id, bool completed) {
    QSqlQuery q(m_db);
    q.prepare(QStringLiteral(
        "UPDATE reminders "
        "SET is_completed = :done, completed_at = :comp_at "
        "WHERE id = :id;"));

    q.bindValue(QStringLiteral(":done"), completed ? 1 : 0);
    q.bindValue(QStringLiteral(":comp_at"),
                completed ? QVariant(static_cast<qlonglong>(QDateTime::currentSecsSinceEpoch())) : QVariant());
    q.bindValue(QStringLiteral(":id"), static_cast<qlonglong>(id));

    return q.exec() && q.numRowsAffected() > 0;
}

std::optional<Reminder> DatabaseManager::pushReminderToTomorrow(std::int64_t id) {
    QSqlQuery selectQ(m_db);
    selectQ.prepare(QStringLiteral(
        "SELECT r.id, r.subject_id, COALESCE(s.name, ''), COALESCE(s.color_hex, '#64748B'), "
        "       r.title, r.description, r.category, r.due_at, COALESCE(r.due_date, ''), r.is_completed "
        "FROM reminders r "
        "LEFT JOIN subjects s ON s.id = r.subject_id "
        "WHERE r.id = :id;"));
    selectQ.bindValue(QStringLiteral(":id"), static_cast<qlonglong>(id));

    if (!selectQ.exec() || !selectQ.next()) {
        return std::nullopt;
    }

    const QDate tomorrowDate = QDate::currentDate().addDays(1);
    const QString newDueDate = tomorrowDate.toString(Qt::ISODate);

    std::int64_t newDueAt = 0;
    if (!selectQ.value(7).isNull()) {
        const QDateTime oldDt = QDateTime::fromSecsSinceEpoch(selectQ.value(7).toLongLong());
        const QDateTime shiftedDt(tomorrowDate, oldDt.time());
        newDueAt = shiftedDt.toSecsSinceEpoch();
    } else {
        newDueAt = QDateTime(tomorrowDate, QTime(9, 0)).toSecsSinceEpoch();
    }

    QSqlQuery updateQ(m_db);
    updateQ.prepare(QStringLiteral(
        "UPDATE reminders SET due_at = :due_at, due_date = :due_date WHERE id = :id;"));
    updateQ.bindValue(QStringLiteral(":due_at"), static_cast<qlonglong>(newDueAt));
    updateQ.bindValue(QStringLiteral(":due_date"), newDueDate);
    updateQ.bindValue(QStringLiteral(":id"), static_cast<qlonglong>(id));

    if (!updateQ.exec() || updateQ.numRowsAffected() == 0) {
        return std::nullopt;
    }

    const QByteArray catBytes = selectQ.value(6).toString().toUtf8();
    Reminder updated;
    updated.id              = selectQ.value(0).toLongLong();
    updated.subjectId       = selectQ.value(1).isNull() ? std::nullopt
                                                        : std::optional<std::int64_t>(selectQ.value(1).toLongLong());
    updated.subjectName     = selectQ.value(2).toString();
    updated.subjectColorHex = selectQ.value(3).toString();
    updated.title           = selectQ.value(4).toString();
    updated.description     = selectQ.value(5).toString();
    updated.category        = reminderCategoryFromSql(
        std::string_view(catBytes.constData(), static_cast<std::size_t>(catBytes.size())));
    updated.dueAt           = newDueAt;
    updated.dueDate         = newDueDate;
    updated.isCompleted     = selectQ.value(9).toInt() != 0;
    return updated;
}

bool DatabaseManager::deleteReminder(std::int64_t id) {
    QSqlQuery q(m_db);
    q.prepare(QStringLiteral("DELETE FROM reminders WHERE id = :id;"));
    q.bindValue(QStringLiteral(":id"), static_cast<qlonglong>(id));
    return q.exec() && q.numRowsAffected() > 0;
}

// ============================================================================
// Standalone Calendar Events
// ============================================================================

std::vector<CalendarEvent> DatabaseManager::fetchCalendarEventsInRange(const QString& startDateIso,
                                                                       const QString& endDateIso) const {
    std::vector<CalendarEvent> results;
    QSqlQuery q(m_db);
    q.prepare(QStringLiteral(
        "SELECT e.id, e.subject_id, e.title, e.location, "
        "       COALESCE(NULLIF(e.color_hex, ''), s.color_hex, '#4F46E5') AS resolved_color, "
        "       e.start_at, e.end_at, e.is_all_day, e.event_date "
        "FROM calendar_events e "
        "LEFT JOIN subjects s ON s.id = e.subject_id "
        "WHERE e.event_date >= :start AND e.event_date <= :end "
        "ORDER BY e.event_date ASC, e.is_all_day DESC, e.start_at ASC, e.id ASC;"));
    q.bindValue(QStringLiteral(":start"), startDateIso);
    q.bindValue(QStringLiteral(":end"), endDateIso);

    if (!q.exec()) {
        qWarning() << "[DB] fetchCalendarEventsInRange failed:" << q.lastError().text();
        return results;
    }

    while (q.next()) {
        CalendarEvent ev;
        ev.id        = q.value(0).toLongLong();
        ev.subjectId = q.value(1).isNull() ? std::nullopt
                                           : std::optional<std::int64_t>(q.value(1).toLongLong());
        ev.title     = q.value(2).toString();
        ev.location  = q.value(3).toString();
        ev.colorHex  = q.value(4).toString();
        ev.startAt   = q.value(5).toLongLong();
        ev.endAt     = q.value(6).toLongLong();
        ev.isAllDay  = q.value(7).toInt() != 0;
        ev.eventDate = q.value(8).toString();
        results.push_back(std::move(ev));
    }
    return results;
}

std::optional<CalendarEvent> DatabaseManager::createCalendarEvent(const CalendarEvent& draft) {
    QSqlQuery q(m_db);
    q.prepare(QStringLiteral(
        "INSERT INTO calendar_events "
        "(subject_id, title, location, color_hex, start_at, end_at, is_all_day, event_date) "
        "VALUES (:sub_id, :title, :loc, :color, :start, :end, :all_day, :edate);"));

    q.bindValue(QStringLiteral(":sub_id"),
                draft.subjectId ? QVariant(static_cast<qlonglong>(*draft.subjectId)) : QVariant());
    q.bindValue(QStringLiteral(":title"), draft.title.trimmed());
    q.bindValue(QStringLiteral(":loc"), draft.location.trimmed());
    q.bindValue(QStringLiteral(":color"), draft.colorHex.isEmpty() ? QVariant() : QVariant(draft.colorHex));
    q.bindValue(QStringLiteral(":start"), static_cast<qlonglong>(draft.startAt));
    q.bindValue(QStringLiteral(":end"), static_cast<qlonglong>(draft.endAt));
    q.bindValue(QStringLiteral(":all_day"), draft.isAllDay ? 1 : 0);
    q.bindValue(QStringLiteral(":edate"), draft.eventDate);

    if (!q.exec()) {
        qWarning() << "[DB] createCalendarEvent failed:" << q.lastError().text();
        return std::nullopt;
    }

    CalendarEvent created = draft;
    created.id    = q.lastInsertId().toLongLong();
    created.title = draft.title.trimmed();
    return created;
}

bool DatabaseManager::deleteCalendarEvent(std::int64_t id) {
    QSqlQuery q(m_db);
    q.prepare(QStringLiteral("DELETE FROM calendar_events WHERE id = :id;"));
    q.bindValue(QStringLiteral(":id"), static_cast<qlonglong>(id));
    return q.exec() && q.numRowsAffected() > 0;
}

// ============================================================================
// Checklists & Nested Items
// ============================================================================

std::vector<Checklist> DatabaseManager::fetchAllChecklists() const {
    std::vector<Checklist> results;
    QSqlQuery q(m_db);

    const QString sql = QStringLiteral(
        "SELECT c.id, c.subject_id, c.title, c.sort_order, "
        "       COALESCE(SUM(CASE WHEN ci.is_completed = 1 THEN 1 ELSE 0 END), 0) AS done_cnt, "
        "       COUNT(ci.id) AS total_cnt "
        "FROM checklists c "
        "LEFT JOIN checklist_items ci ON ci.checklist_id = c.id "
        "GROUP BY c.id "
        "ORDER BY c.sort_order ASC, c.id ASC;");

    if (!q.exec(sql)) {
        qWarning() << "[DB] fetchAllChecklists failed:" << q.lastError().text();
        return results;
    }

    while (q.next()) {
        Checklist c;
        c.id             = q.value(0).toLongLong();
        c.subjectId      = q.value(1).isNull() ? std::nullopt
                                               : std::optional<std::int64_t>(q.value(1).toLongLong());
        c.title          = q.value(2).toString();
        c.sortOrder      = q.value(3).toInt();
        c.completedCount = q.value(4).toInt();
        c.totalCount     = q.value(5).toInt();
        results.push_back(std::move(c));
    }
    return results;
}

std::optional<Checklist> DatabaseManager::createChecklist(const QString& title,
                                                          std::optional<std::int64_t> subjectId) {
    QSqlQuery q(m_db);
    q.prepare(QStringLiteral(
        "INSERT INTO checklists (subject_id, title, sort_order, created_at) "
        "VALUES (:sub_id, :title, (SELECT COALESCE(MAX(sort_order) + 1, 0) FROM checklists), :created);"));

    q.bindValue(QStringLiteral(":sub_id"),
                subjectId ? QVariant(static_cast<qlonglong>(*subjectId)) : QVariant());
    q.bindValue(QStringLiteral(":title"), title.trimmed());
    q.bindValue(QStringLiteral(":created"), static_cast<qlonglong>(QDateTime::currentSecsSinceEpoch()));

    if (!q.exec()) {
        qWarning() << "[DB] createChecklist failed:" << q.lastError().text();
        return std::nullopt;
    }

    Checklist c;
    c.id        = q.lastInsertId().toLongLong();
    c.subjectId = subjectId;
    c.title     = title.trimmed();
    return c;
}

bool DatabaseManager::deleteChecklist(std::int64_t checklistId) {
    QSqlQuery q(m_db);
    q.prepare(QStringLiteral("DELETE FROM checklists WHERE id = :id;"));
    q.bindValue(QStringLiteral(":id"), static_cast<qlonglong>(checklistId));
    return q.exec() && q.numRowsAffected() > 0;
}

std::vector<ChecklistItem> DatabaseManager::fetchChecklistTree(std::int64_t checklistId) const {
    QSqlQuery q(m_db);
    q.prepare(QStringLiteral(
        "SELECT id, checklist_id, parent_item_id, title, is_completed, sort_order "
        "FROM checklist_items "
        "WHERE checklist_id = :cid "
        "ORDER BY sort_order ASC, id ASC;"));
    q.bindValue(QStringLiteral(":cid"), static_cast<qlonglong>(checklistId));

    std::vector<ChecklistItem> flatRaw;
    if (!q.exec()) {
        qWarning() << "[DB] fetchChecklistTree failed:" << q.lastError().text();
        return flatRaw;
    }

    while (q.next()) {
        ChecklistItem item;
        item.id           = q.value(0).toLongLong();
        item.checklistId  = q.value(1).toLongLong();
        item.parentItemId = q.value(2).isNull() ? std::nullopt
                                                : std::optional<std::int64_t>(q.value(2).toLongLong());
        item.title        = q.value(3).toString();
        item.isCompleted  = q.value(4).toInt() != 0;
        item.sortOrder    = q.value(5).toInt();
        flatRaw.push_back(std::move(item));
    }

    std::unordered_map<std::int64_t, std::vector<std::size_t>> childrenMap;
    std::vector<std::size_t> rootIndices;
    childrenMap.reserve(flatRaw.size());

    for (std::size_t i = 0; i < flatRaw.size(); ++i) {
        if (flatRaw[i].parentItemId.has_value()) {
            childrenMap[*flatRaw[i].parentItemId].push_back(i);
        } else {
            rootIndices.push_back(i);
        }
    }

    std::vector<ChecklistItem> orderedTree;
    orderedTree.reserve(flatRaw.size());

    std::function<void(std::size_t, int)> dfs = [&](std::size_t idx, int depth) {
        ChecklistItem item = flatRaw[idx];
        item.depth = depth;
        const auto it = childrenMap.find(item.id);
        item.hasChildren = (it != childrenMap.end() && !it->second.empty());
        orderedTree.push_back(std::move(item));

        if (it != childrenMap.end()) {
            for (std::size_t childIdx : it->second) {
                dfs(childIdx, depth + 1);
            }
        }
    };

    for (std::size_t rootIdx : rootIndices) {
        dfs(rootIdx, 0);
    }

    return orderedTree;
}

std::optional<ChecklistItem> DatabaseManager::createChecklistItem(
    std::int64_t checklistId,
    std::optional<std::int64_t> parentItemId,
    const QString& title) {

    QSqlQuery q(m_db);
    q.prepare(QStringLiteral(
        "INSERT INTO checklist_items (checklist_id, parent_item_id, title, is_completed, sort_order) "
        "VALUES (:cid, :pid, :title, 0, "
        "        (SELECT COALESCE(MAX(sort_order) + 1, 0) FROM checklist_items WHERE checklist_id = :cid2));"));

    q.bindValue(QStringLiteral(":cid"), static_cast<qlonglong>(checklistId));
    q.bindValue(QStringLiteral(":pid"),
                parentItemId ? QVariant(static_cast<qlonglong>(*parentItemId)) : QVariant());
    q.bindValue(QStringLiteral(":title"), title.trimmed());
    q.bindValue(QStringLiteral(":cid2"), static_cast<qlonglong>(checklistId));

    if (!q.exec()) {
        qWarning() << "[DB] createChecklistItem failed:" << q.lastError().text();
        return std::nullopt;
    }

    ChecklistItem item;
    item.id           = q.lastInsertId().toLongLong();
    item.checklistId  = checklistId;
    item.parentItemId = parentItemId;
    item.title        = title.trimmed();
    return item;
}

bool DatabaseManager::setChecklistItemCompleted(std::int64_t itemId, bool completed) {
    QSqlQuery q(m_db);
    q.prepare(QStringLiteral("UPDATE checklist_items SET is_completed = :done WHERE id = :id;"));
    q.bindValue(QStringLiteral(":done"), completed ? 1 : 0);
    q.bindValue(QStringLiteral(":id"), static_cast<qlonglong>(itemId));
    return q.exec() && q.numRowsAffected() > 0;
}

bool DatabaseManager::deleteChecklistItem(std::int64_t itemId) {
    QSqlQuery q(m_db);
    q.prepare(QStringLiteral("DELETE FROM checklist_items WHERE id = :id;"));
    q.bindValue(QStringLiteral(":id"), static_cast<qlonglong>(itemId));
    return q.exec() && q.numRowsAffected() > 0;
}

// ============================================================================
// Phase 4: Analytics Aggregations
// ============================================================================

std::vector<SubjectStudyStat> DatabaseManager::fetchSubjectStudyDistribution(
    const QString& startDateIso) const {
    std::vector<SubjectStudyStat> results;
    QSqlQuery q(m_db);

    if (startDateIso.isEmpty()) {
        q.prepare(QStringLiteral(
            "SELECT s.id, s.name, s.color_hex, s.target_weekly_minutes, "
            "       COALESCE(SUM(ss.duration_seconds), 0) AS total_secs "
            "FROM subjects s "
            "LEFT JOIN study_sessions ss ON ss.subject_id = s.id "
            "WHERE s.archived = 0 "
            "GROUP BY s.id "
            "ORDER BY total_secs DESC, s.name ASC;"));
    } else {
        q.prepare(QStringLiteral(
            "SELECT s.id, s.name, s.color_hex, s.target_weekly_minutes, "
            "       COALESCE(SUM(ss.duration_seconds), 0) AS total_secs "
            "FROM subjects s "
            "LEFT JOIN study_sessions ss "
            "       ON ss.subject_id = s.id AND ss.study_date >= :start_date "
            "WHERE s.archived = 0 "
            "GROUP BY s.id "
            "ORDER BY total_secs DESC, s.name ASC;"));
        q.bindValue(QStringLiteral(":start_date"), startDateIso);
    }

    if (!q.exec()) {
        qWarning() << "[DB] fetchSubjectStudyDistribution failed:" << q.lastError().text();
        return results;
    }

    while (q.next()) {
        SubjectStudyStat stat;
        stat.subjectId           = q.value(0).toLongLong();
        stat.subjectName         = q.value(1).toString();
        stat.colorHex            = q.value(2).toString();
        stat.targetWeeklyMinutes = q.value(3).toInt();
        stat.totalSeconds        = q.value(4).toLongLong();
        results.push_back(std::move(stat));
    }
    return results;
}

std::vector<DailyStudyStat> DatabaseManager::fetchDailyStudyActivity(
    const QString& startDateIso,
    const QString& endDateIso) const {
    std::vector<DailyStudyStat> results;
    QSqlQuery q(m_db);
    q.prepare(QStringLiteral(
        "SELECT study_date, COALESCE(SUM(duration_seconds), 0) "
        "FROM study_sessions "
        "WHERE study_date >= :start_date AND study_date <= :end_date "
        "GROUP BY study_date "
        "ORDER BY study_date ASC;"));
    q.bindValue(QStringLiteral(":start_date"), startDateIso);
    q.bindValue(QStringLiteral(":end_date"), endDateIso);

    if (!q.exec()) {
        qWarning() << "[DB] fetchDailyStudyActivity failed:" << q.lastError().text();
        return results;
    }

    while (q.next()) {
        DailyStudyStat day;
        day.dateIso      = q.value(0).toString();
        day.totalSeconds = q.value(1).toLongLong();
        results.push_back(std::move(day));
    }
    return results;
}

std::vector<UpcomingExam> DatabaseManager::fetchUpcomingExaminations() const {
    std::vector<UpcomingExam> results;
    QSqlQuery q(m_db);
    q.prepare(QStringLiteral(
        "SELECT r.id, r.title, COALESCE(s.name, 'General'), COALESCE(s.color_hex, '#F43F5E'), "
        "       r.due_at, COALESCE(r.due_date, '') "
        "FROM reminders r "
        "LEFT JOIN subjects s ON s.id = r.subject_id "
        "WHERE r.category = 'examination' "
        "  AND r.is_completed = 0 "
        "  AND r.due_at IS NOT NULL "
        "ORDER BY r.due_at ASC;"));

    if (!q.exec()) {
        qWarning() << "[DB] fetchUpcomingExaminations failed:" << q.lastError().text();
        return results;
    }

    while (q.next()) {
        UpcomingExam exam;
        exam.reminderId  = q.value(0).toLongLong();
        exam.title       = q.value(1).toString();
        exam.subjectName = q.value(2).toString();
        exam.colorHex    = q.value(3).toString();
        exam.dueAtEpoch  = q.value(4).toLongLong();
        exam.dueDateIso  = q.value(5).toString();
        results.push_back(std::move(exam));
    }
    return results;
}

} // namespace cadence
EOF
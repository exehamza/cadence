// src/viewmodels/CalendarViewModel.cpp
#include "viewmodels/CalendarViewModel.hpp"

#include <QDateTime>
#include <QLocale>
#include <QTime>
#include <QVariantMap>
#include <algorithm>
#include <unordered_map>

namespace cadence {

CalendarViewModel::CalendarViewModel(DatabaseManager& db, QObject* parent)
    : QObject(parent),
      m_db(db),
      m_anchorDate(QDate::currentDate()),
      m_selectedDate(QDate::currentDate()) {
    rebuildGrid();
}

void CalendarViewModel::setViewMode(ViewMode mode) {
    if (m_viewMode == mode) return;
    m_viewMode = mode;
    m_anchorDate = m_selectedDate;
    emit viewModeChanged();
    rebuildGrid();
}

QString CalendarViewModel::headerTitle() const {
    const QLocale locale;
    if (m_viewMode == MonthView) {
        return locale.toString(m_anchorDate, QStringLiteral("MMMM yyyy"));
    }

    const int offset = m_anchorDate.dayOfWeek() - 1;
    const QDate weekStart = m_anchorDate.addDays(-offset);
    const QDate weekEnd   = weekStart.addDays(6);

    if (weekStart.month() == weekEnd.month()) {
        return QStringLiteral("%1 %2 – %3, %4")
            .arg(locale.monthName(weekStart.month(), QLocale::ShortFormat))
            .arg(weekStart.day())
            .arg(weekEnd.day())
            .arg(weekEnd.year());
    }
    return QStringLiteral("%1 %2 – %3 %4, %5")
        .arg(locale.monthName(weekStart.month(), QLocale::ShortFormat))
        .arg(weekStart.day())
        .arg(locale.monthName(weekEnd.month(), QLocale::ShortFormat))
        .arg(weekEnd.day())
        .arg(weekEnd.year());
}

QString CalendarViewModel::selectedDateIso() const {
    return m_selectedDate.toString(Qt::ISODate);
}

void CalendarViewModel::setSelectedDateIso(const QString& dateIso) {
    const QDate parsed = QDate::fromString(dateIso, Qt::ISODate);
    if (!parsed.isValid() || parsed == m_selectedDate) return;

    m_selectedDate = parsed;
    if (m_viewMode == MonthView && (parsed.month() != m_anchorDate.month() || parsed.year() != m_anchorDate.year())) {
        m_anchorDate = QDate(parsed.year(), parsed.month(), 1);
    }
    emit selectedDateChanged();
    rebuildGrid();
}

QString CalendarViewModel::selectedDateFormatted() const {
    return QLocale().toString(m_selectedDate, QStringLiteral("dddd, MMM d, yyyy"));
}

void CalendarViewModel::refresh() {
    rebuildGrid();
}

void CalendarViewModel::nextPeriod() {
    if (m_viewMode == MonthView) {
        m_anchorDate = m_anchorDate.addMonths(1);
    } else {
        m_anchorDate = m_anchorDate.addDays(7);
        m_selectedDate = m_selectedDate.addDays(7);
        emit selectedDateChanged();
    }
    rebuildGrid();
}

void CalendarViewModel::previousPeriod() {
    if (m_viewMode == MonthView) {
        m_anchorDate = m_anchorDate.addMonths(-1);
    } else {
        m_anchorDate = m_anchorDate.addDays(-7);
        m_selectedDate = m_selectedDate.addDays(-7);
        emit selectedDateChanged();
    }
    rebuildGrid();
}

void CalendarViewModel::goToToday() {
    const QDate today = QDate::currentDate();
    m_anchorDate = today;
    if (m_selectedDate != today) {
        m_selectedDate = today;
        emit selectedDateChanged();
    }
    rebuildGrid();
}

bool CalendarViewModel::addStandaloneEvent(const QString& title,
                                           const QString& location,
                                           const QString& colorHex,
                                           const QString& dateIso,
                                           const QString& startTimeStr,
                                           const QString& endTimeStr,
                                           bool isAllDay,
                                           qint64 subjectId) {
    if (title.trimmed().isEmpty()) return false;

    const QDate eventDate = QDate::fromString(dateIso, Qt::ISODate);
    if (!eventDate.isValid()) return false;

    QTime startTime = QTime::fromString(startTimeStr, QStringLiteral("HH:mm"));
    QTime endTime   = QTime::fromString(endTimeStr, QStringLiteral("HH:mm"));

    if (isAllDay || !startTime.isValid()) {
        startTime = QTime(0, 0);
        endTime   = QTime(23, 59);
    } else if (!endTime.isValid() || endTime <= startTime) {
        endTime = startTime.addSecs(3600);
    }

    CalendarEvent draft;
    draft.subjectId = subjectId > 0 ? std::optional<std::int64_t>(subjectId) : std::nullopt;
    draft.title     = title;
    draft.location  = location;
    draft.colorHex  = (subjectId > 0) ? QString() : colorHex;
    draft.startAt   = QDateTime(eventDate, startTime).toSecsSinceEpoch();
    draft.endAt     = QDateTime(eventDate, endTime).toSecsSinceEpoch();
    draft.isAllDay  = isAllDay;
    draft.eventDate = dateIso;

    if (!m_db.createCalendarEvent(draft).has_value()) {
        return false;
    }

    rebuildGrid();
    return true;
}

bool CalendarViewModel::removeStandaloneEvent(qint64 eventId) {
    if (!m_db.deleteCalendarEvent(eventId)) return false;
    rebuildGrid();
    return true;
}

void CalendarViewModel::rebuildGrid() {
    QDate gridStart;
    int cellCount = 42;

    if (m_viewMode == MonthView) {
        const QDate firstOfMonth(m_anchorDate.year(), m_anchorDate.month(), 1);
        const int mondayOffset = firstOfMonth.dayOfWeek() - 1;
        gridStart = firstOfMonth.addDays(-mondayOffset);
        cellCount = 42;
    } else {
        const int mondayOffset = m_anchorDate.dayOfWeek() - 1;
        gridStart = m_anchorDate.addDays(-mondayOffset);
        cellCount = 7;
    }

    const QDate gridEnd = gridStart.addDays(cellCount - 1);
    const QString startIso = gridStart.toString(Qt::ISODate);
    const QString endIso   = gridEnd.toString(Qt::ISODate);

    const auto reminders = m_db.fetchRemindersInRange(startIso, endIso);
    const auto events    = m_db.fetchCalendarEventsInRange(startIso, endIso);

    std::unordered_map<QString, QVariantList> itemsByDate;
    itemsByDate.reserve(static_cast<std::size_t>(cellCount));

    // 1. Unified Sync: Map dated Reminders (Tasks, Assignments, Examinations)
    for (const Reminder& r : reminders) {
        QString catBadge = QStringLiteral("Task");
        if (r.category == ReminderCategory::Assignment) {
            catBadge = QStringLiteral("Assignment");
        } else if (r.category == ReminderCategory::Examination) {
            catBadge = QStringLiteral("Exam");
        }

        QString timeText = QStringLiteral("Due");
        qint64 sortEpoch = 0;
        if (r.dueAt.has_value() && *r.dueAt > 0) {
            sortEpoch = static_cast<qint64>(*r.dueAt);
            const QDateTime dt = QDateTime::fromSecsSinceEpoch(sortEpoch);
            timeText = dt.time().toString(QStringLiteral("HH:mm"));
        }

        QVariantMap entry;
        entry.insert(QStringLiteral("itemId"),        static_cast<qlonglong>(r.id));
        entry.insert(QStringLiteral("isReminder"),    true);
        entry.insert(QStringLiteral("title"),         r.title);
        entry.insert(QStringLiteral("subtitle"),      r.subjectName.isEmpty() ? catBadge : r.subjectName);
        entry.insert(QStringLiteral("categoryLabel"), catBadge);
        entry.insert(QStringLiteral("timeLabel"),     timeText);
        entry.insert(QStringLiteral("colorHex"),      r.subjectColorHex);
        entry.insert(QStringLiteral("isCompleted"),   r.isCompleted);
        entry.insert(QStringLiteral("sortEpoch"),     sortEpoch);

        itemsByDate[r.dueDate].append(entry);
    }

    // 2. Standalone Calendar Events
    for (const CalendarEvent& ev : events) {
        QString timeText = QStringLiteral("All Day");
        if (!ev.isAllDay && ev.startAt > 0) {
            const QDateTime startDt = QDateTime::fromSecsSinceEpoch(ev.startAt);
            const QDateTime endDt   = QDateTime::fromSecsSinceEpoch(ev.endAt);
            timeText = QStringLiteral("%1 – %2")
                .arg(startDt.time().toString(QStringLiteral("HH:mm")),
                     endDt.time().toString(QStringLiteral("HH:mm")));
        }

        QVariantMap entry;
        entry.insert(QStringLiteral("itemId"),        static_cast<qlonglong>(ev.id));
        entry.insert(QStringLiteral("isReminder"),    false);
        entry.insert(QStringLiteral("title"),         ev.title);
        entry.insert(QStringLiteral("subtitle"),      ev.location.isEmpty() ? QStringLiteral("Event") : ev.location);
        entry.insert(QStringLiteral("categoryLabel"), QStringLiteral("Event"));
        entry.insert(QStringLiteral("timeLabel"),     timeText);
        entry.insert(QStringLiteral("colorHex"),      ev.colorHex);
        entry.insert(QStringLiteral("isCompleted"),   false);
        entry.insert(QStringLiteral("sortEpoch"),     static_cast<qlonglong>(ev.startAt));

        itemsByDate[ev.eventDate].append(entry);
    }

    // Sort each day's unified items chronologically
    for (auto& [dateKey, list] : itemsByDate) {
        std::sort(list.begin(), list.end(), [](const QVariant& a, const QVariant& b) {
            return a.toMap().value(QStringLiteral("sortEpoch")).toLongLong()
                 < b.toMap().value(QStringLiteral("sortEpoch")).toLongLong();
        });
    }

    const QDate today = QDate::currentDate();
    const QLocale locale;
    QVariantList newCells;
    newCells.reserve(cellCount);

    for (int i = 0; i < cellCount; ++i) {
        const QDate d = gridStart.addDays(i);
        const QString iso = d.toString(Qt::ISODate);

        QVariantMap cell;
        cell.insert(QStringLiteral("dateIso"),        iso);
        cell.insert(QStringLiteral("dayNumber"),      d.day());
        cell.insert(QStringLiteral("dayOfWeekShort"), locale.dayName(d.dayOfWeek(), QLocale::ShortFormat));
        cell.insert(QStringLiteral("isCurrentMonth"), d.month() == m_anchorDate.month());
        cell.insert(QStringLiteral("isToday"),        d == today);
        cell.insert(QStringLiteral("isSelected"),     d == m_selectedDate);
        cell.insert(QStringLiteral("items"),          itemsByDate[iso]);

        newCells.append(cell);
    }

    m_dayCells = std::move(newCells);

    // Populate Agenda Inspector for selectedDateIso (even if outside current grid)
    const QString selIso = m_selectedDate.toString(Qt::ISODate);
    if (itemsByDate.find(selIso) != itemsByDate.end()) {
        m_selectedDayItems = itemsByDate[selIso];
    } else {
        m_selectedDayItems.clear();
    }

    emit gridChanged();
}

} // namespace cadence
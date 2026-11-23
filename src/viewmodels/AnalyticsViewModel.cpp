#include "viewmodels/AnalyticsViewModel.hpp"

#include <QDate>
#include <QDateTime>
#include <QLocale>
#include <QVariantMap>
#include <algorithm>
#include <unordered_map>

namespace cadence {

AnalyticsViewModel::AnalyticsViewModel(DatabaseManager& db, QObject* parent)
    : QObject(parent), m_db(db) {
    m_countdownTimer.setInterval(60000);
    m_countdownTimer.setTimerType(Qt::VeryCoarseTimer);
    connect(&m_countdownTimer, &QTimer::timeout, this, &AnalyticsViewModel::updateExamCountdowns);

    refresh();
}

void AnalyticsViewModel::setTimeframeIndex(int idx) {
    const int clamped = std::clamp(idx, 0, 2);
    if (m_timeframeIndex == clamped) return;
    m_timeframeIndex = clamped;
    emit timeframeChanged();
    rebuildSubjectDistribution();
    emit analyticsChanged();
}

void AnalyticsViewModel::refresh() {
    rebuildSubjectDistribution();
    rebuildActivityHeatmap();
    reloadUpcomingExams();
    emit analyticsChanged();
}

void AnalyticsViewModel::rebuildSubjectDistribution() {
    QString startDateIso;
    const QDate today = QDate::currentDate();
    if (m_timeframeIndex == 1) {
        startDateIso = today.addDays(-29).toString(Qt::ISODate);
    } else if (m_timeframeIndex == 2) {
        startDateIso = today.addDays(-6).toString(Qt::ISODate);
    }

    const auto stats = m_db.fetchSubjectStudyDistribution(startDateIso);

    std::int64_t grandTotalSecs = 0;
    for (const auto& s : stats) {
        grandTotalSecs += s.totalSeconds;
    }

    m_totalHoursInRange = static_cast<double>(grandTotalSecs) / 3600.0;

    QVariantList list;
    list.reserve(static_cast<int>(stats.size()));

    for (const auto& s : stats) {
        const double hours = static_cast<double>(s.totalSeconds) / 3600.0;
        const double pct = (grandTotalSecs > 0)
            ? (static_cast<double>(s.totalSeconds) * 100.0 / static_cast<double>(grandTotalSecs))
            : 0.0;

        QVariantMap row;
        row.insert(QStringLiteral("subjectId"),  static_cast<qlonglong>(s.subjectId));
        row.insert(QStringLiteral("name"),       s.subjectName);
        row.insert(QStringLiteral("colorHex"),   s.colorHex);
        row.insert(QStringLiteral("seconds"),    static_cast<qlonglong>(s.totalSeconds));
        row.insert(QStringLiteral("hours"),      hours);
        row.insert(QStringLiteral("percentage"), pct);
        list.push_back(std::move(row));
    }

    m_subjectDistribution = std::move(list);
}

void AnalyticsViewModel::rebuildActivityHeatmap() {
    const QDate today = QDate::currentDate();
    // Align grid to 53 full Monday–Sunday weeks ending on the current week's Sunday
    const int daysToSunday = 7 - today.dayOfWeek();
    const QDate gridEnd    = today.addDays(daysToSunday);
    const QDate gridStart  = gridEnd.addDays(-(53 * 7 - 1));

    const auto dailyRows = m_db.fetchDailyStudyActivity(
        gridStart.toString(Qt::ISODate),
        today.toString(Qt::ISODate)
    );

    std::unordered_map<QString, std::int64_t> secondsByDate;
    secondsByDate.reserve(dailyRows.size());

    std::int64_t maxDailySecs = 0;
    int activeDays = 0;

    for (const auto& row : dailyRows) {
        secondsByDate[row.dateIso] = row.totalSeconds;
        if (row.totalSeconds > 0) {
            ++activeDays;
            maxDailySecs = std::max(maxDailySecs, row.totalSeconds);
        }
    }

    m_activeDaysLastYear = activeDays;

    // Compute Current Streak & Longest Streak over trailing 365 days
    int currentStreak = 0;
    QDate cursor = today;
    // Allow streak to stay alive if user hasn't studied yet today but studied yesterday
    if (secondsByDate[cursor.toString(Qt::ISODate)] <= 0) {
        cursor = cursor.addDays(-1);
    }
    while (cursor >= gridStart && secondsByDate[cursor.toString(Qt::ISODate)] > 0) {
        ++currentStreak;
        cursor = cursor.addDays(-1);
    }
    m_currentStreakDays = currentStreak;

    int longestStreak = 0;
    int runningStreak = 0;
    for (QDate d = gridStart; d <= today; d = d.addDays(1)) {
        if (secondsByDate[d.toString(Qt::ISODate)] > 0) {
            ++runningStreak;
            longestStreak = std::max(longestStreak, runningStreak);
        } else {
            runningStreak = 0;
        }
    }
    m_longestStreakDays = longestStreak;

    // Dynamic quartile thresholds (minimum 1 hour baseline so 10-min sessions don't look maxed out)
    const double scaleCeiling = std::max<double>(3600.0, static_cast<double>(maxDailySecs));

    const QLocale locale;
    QVariantList cells;
    cells.reserve(53 * 7);

    QVariantList monthHeaders;
    monthHeaders.reserve(53);
    int lastLabeledMonth = -1;

    for (int week = 0; week < 53; ++week) {
        const QDate weekMonday = gridStart.addDays(week * 7);
        if (weekMonday.month() != lastLabeledMonth && weekMonday <= today) {
            monthHeaders.push_back(locale.monthName(weekMonday.month(), QLocale::ShortFormat));
            lastLabeledMonth = weekMonday.month();
        } else {
            monthHeaders.push_back(QString());
        }

        for (int dayOfWeek = 0; dayOfWeek < 7; ++dayOfWeek) {
            const QDate cellDate = weekMonday.addDays(dayOfWeek);
            const QString iso = cellDate.toString(Qt::ISODate);
            const bool isFuture = cellDate > today;
            const std::int64_t secs = isFuture ? 0 : secondsByDate[iso];

            int level = 0;
            if (secs > 0) {
                const double ratio = static_cast<double>(secs) / scaleCeiling;
                if (ratio <= 0.25)      level = 1;
                else if (ratio <= 0.50) level = 2;
                else if (ratio <= 0.75) level = 3;
                else                    level = 4;
            }

            QVariantMap cell;
            cell.insert(QStringLiteral("dateIso"),       iso);
            cell.insert(QStringLiteral("formattedDate"), locale.toString(cellDate, QStringLiteral("MMM d, yyyy")));
            cell.insert(QStringLiteral("hours"),         static_cast<double>(secs) / 3600.0);
            cell.insert(QStringLiteral("minutes"),       static_cast<int>(secs / 60));
            cell.insert(QStringLiteral("level"),         level);
            cell.insert(QStringLiteral("isFuture"),      isFuture);
            cell.insert(QStringLiteral("isToday"),       cellDate == today);
            cells.push_back(std::move(cell));
        }
    }

    m_heatmapCells       = std::move(cells);
    m_heatmapMonthLabels = std::move(monthHeaders);
}

void AnalyticsViewModel::reloadUpcomingExams() {
    m_rawExams = m_db.fetchUpcomingExaminations();
    updateExamCountdowns();

    if (m_rawExams.empty()) {
        m_countdownTimer.stop();
    } else if (!m_countdownTimer.isActive()) {
        m_countdownTimer.start();
    }
}

void AnalyticsViewModel::updateExamCountdowns() {
    const std::int64_t nowEpoch = QDateTime::currentSecsSinceEpoch();
    const QLocale locale;

    QVariantList cards;
    cards.reserve(static_cast<int>(m_rawExams.size()));

    for (const auto& exam : m_rawExams) {
        const std::int64_t deltaSecs = exam.dueAtEpoch - nowEpoch;
        const bool isOverdue = deltaSecs <= 0;
        const std::int64_t absSecs = std::max<std::int64_t>(0, deltaSecs);

        const int days    = static_cast<int>(absSecs / 86400);
        const int hours   = static_cast<int>((absSecs % 86400) / 3600);
        const int minutes = static_cast<int>((absSecs % 3600) / 60);
        const int totalHours = static_cast<int>(absSecs / 3600);

        const QDateTime dueDt = QDateTime::fromSecsSinceEpoch(exam.dueAtEpoch);
        const QString formattedDue = locale.toString(dueDt, QStringLiteral("ddd, MMM d • HH:mm"));

        QVariantMap card;
        card.insert(QStringLiteral("reminderId"),       static_cast<qlonglong>(exam.reminderId));
        card.insert(QStringLiteral("title"),            exam.title);
        card.insert(QStringLiteral("subjectName"),      exam.subjectName);
        card.insert(QStringLiteral("colorHex"),         exam.colorHex);
        card.insert(QStringLiteral("dueDateFormatted"), formattedDue);
        card.insert(QStringLiteral("daysLeft"),         days);
        card.insert(QStringLiteral("hoursLeft"),        hours);
        card.insert(QStringLiteral("minutesLeft"),      minutes);
        card.insert(QStringLiteral("totalHoursLeft"),   totalHours);
        card.insert(QStringLiteral("isOverdue"),        isOverdue);
        card.insert(QStringLiteral("isUrgent"),         !isOverdue && totalHours < 48);
        cards.push_back(std::move(card));
    }

    m_upcomingExams = std::move(cards);
    emit countdownChanged();
}

} // namespace cadence
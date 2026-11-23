#pragma once

#include "db/DatabaseManager.hpp"

#include <QObject>
#include <QString>
#include <QTimer>
#include <QVariantList>
#include <vector>

namespace cadence {

class AnalyticsViewModel : public QObject {
    Q_OBJECT

    // 0 = All Time, 1 = Last 30 Days, 2 = Last 7 Days
    Q_PROPERTY(int timeframeIndex READ timeframeIndex WRITE setTimeframeIndex NOTIFY timeframeChanged)

    // Summary Metrics
    Q_PROPERTY(double totalHoursInRange READ totalHoursInRange NOTIFY analyticsChanged)
    Q_PROPERTY(int activeDaysLastYear READ activeDaysLastYear NOTIFY analyticsChanged)
    Q_PROPERTY(int currentStreakDays READ currentStreakDays NOTIFY analyticsChanged)
    Q_PROPERTY(int longestStreakDays READ longestStreakDays NOTIFY analyticsChanged)

    // Visualization Datasets
    Q_PROPERTY(QVariantList subjectDistribution READ subjectDistribution NOTIFY analyticsChanged)
    Q_PROPERTY(QVariantList heatmapCells READ heatmapCells NOTIFY analyticsChanged)
    Q_PROPERTY(QVariantList heatmapMonthLabels READ heatmapMonthLabels NOTIFY analyticsChanged)
    Q_PROPERTY(QVariantList upcomingExams READ upcomingExams NOTIFY countdownChanged)

public:
    explicit AnalyticsViewModel(DatabaseManager& db, QObject* parent = nullptr);

    [[nodiscard]] int timeframeIndex() const noexcept { return m_timeframeIndex; }
    void setTimeframeIndex(int idx);

    [[nodiscard]] double totalHoursInRange() const noexcept { return m_totalHoursInRange; }
    [[nodiscard]] int activeDaysLastYear() const noexcept { return m_activeDaysLastYear; }
    [[nodiscard]] int currentStreakDays() const noexcept { return m_currentStreakDays; }
    [[nodiscard]] int longestStreakDays() const noexcept { return m_longestStreakDays; }

    [[nodiscard]] QVariantList subjectDistribution() const noexcept { return m_subjectDistribution; }
    [[nodiscard]] QVariantList heatmapCells() const noexcept { return m_heatmapCells; }
    [[nodiscard]] QVariantList heatmapMonthLabels() const noexcept { return m_heatmapMonthLabels; }
    [[nodiscard]] QVariantList upcomingExams() const noexcept { return m_upcomingExams; }

    Q_INVOKABLE void refresh();

signals:
    void timeframeChanged();
    void analyticsChanged();
    void countdownChanged();

private slots:
    void updateExamCountdowns();

private:
    void rebuildSubjectDistribution();
    void rebuildActivityHeatmap();
    void reloadUpcomingExams();

    DatabaseManager& m_db;
    QTimer           m_countdownTimer;

    int    m_timeframeIndex{0};
    double m_totalHoursInRange{0.0};
    int    m_activeDaysLastYear{0};
    int    m_currentStreakDays{0};
    int    m_longestStreakDays{0};

    QVariantList              m_subjectDistribution;
    QVariantList              m_heatmapCells;
    QVariantList              m_heatmapMonthLabels;
    std::vector<UpcomingExam> m_rawExams;
    QVariantList              m_upcomingExams;
};

} // namespace cadence
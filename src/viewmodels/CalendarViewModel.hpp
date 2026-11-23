// src/viewmodels/CalendarViewModel.hpp
#pragma once

#include "db/DatabaseManager.hpp"

#include <QDate>
#include <QObject>
#include <QString>
#include <QVariantList>

namespace cadence {

class CalendarViewModel : public QObject {
    Q_OBJECT

    Q_PROPERTY(ViewMode viewMode READ viewMode WRITE setViewMode NOTIFY viewModeChanged)
    Q_PROPERTY(QString headerTitle READ headerTitle NOTIFY gridChanged)
    Q_PROPERTY(QString selectedDateIso READ selectedDateIso WRITE setSelectedDateIso NOTIFY selectedDateChanged)
    Q_PROPERTY(QString selectedDateFormatted READ selectedDateFormatted NOTIFY selectedDateChanged)
    Q_PROPERTY(QVariantList dayCells READ dayCells NOTIFY gridChanged)
    Q_PROPERTY(QVariantList selectedDayItems READ selectedDayItems NOTIFY gridChanged)

public:
    enum ViewMode {
        MonthView = 0,
        WeekView  = 1
    };
    Q_ENUM(ViewMode)

    explicit CalendarViewModel(DatabaseManager& db, QObject* parent = nullptr);

    [[nodiscard]] ViewMode viewMode() const noexcept { return m_viewMode; }
    void setViewMode(ViewMode mode);

    [[nodiscard]] QString headerTitle() const;
    [[nodiscard]] QString selectedDateIso() const;
    void setSelectedDateIso(const QString& dateIso);

    [[nodiscard]] QString selectedDateFormatted() const;
    [[nodiscard]] QVariantList dayCells() const noexcept { return m_dayCells; }
    [[nodiscard]] QVariantList selectedDayItems() const noexcept { return m_selectedDayItems; }

    Q_INVOKABLE void refresh();
    Q_INVOKABLE void nextPeriod();
    Q_INVOKABLE void previousPeriod();
    Q_INVOKABLE void goToToday();

    Q_INVOKABLE bool addStandaloneEvent(const QString& title,
                                        const QString& location,
                                        const QString& colorHex,
                                        const QString& dateIso,
                                        const QString& startTimeStr,
                                        const QString& endTimeStr,
                                        bool isAllDay,
                                        qint64 subjectId);
    Q_INVOKABLE bool removeStandaloneEvent(qint64 eventId);

signals:
    void viewModeChanged();
    void selectedDateChanged();
    void gridChanged();

private:
    void rebuildGrid();

    DatabaseManager& m_db;
    ViewMode         m_viewMode{MonthView};
    QDate            m_anchorDate;
    QDate            m_selectedDate;
    QVariantList     m_dayCells;
    QVariantList     m_selectedDayItems;
};

} // namespace cadence
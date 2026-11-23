// src/models/ReminderFilterModel.hpp
#pragma once

#include <QSortFilterProxyModel>
#include <QString>

namespace cadence {

class ReminderFilterModel : public QSortFilterProxyModel {
    Q_OBJECT
    Q_PROPERTY(QString categoryFilter READ categoryFilter WRITE setCategoryFilter NOTIFY filterChanged)
    Q_PROPERTY(bool showCompleted READ showCompleted WRITE setShowCompleted NOTIFY filterChanged)
    Q_PROPERTY(int filteredCount READ rowCount NOTIFY filteredCountChanged)

public:
    explicit ReminderFilterModel(QObject* parent = nullptr);

    [[nodiscard]] QString categoryFilter() const noexcept { return m_categoryFilter; }
    void setCategoryFilter(const QString& category);

    [[nodiscard]] bool showCompleted() const noexcept { return m_showCompleted; }
    void setShowCompleted(bool show);

signals:
    void filterChanged();
    void filteredCountChanged();

protected:
    [[nodiscard]] bool filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const override;

private:
    QString m_categoryFilter{QStringLiteral("all")};
    bool    m_showCompleted{true};
};

} // namespace cadence
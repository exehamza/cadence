// src/models/ReminderFilterModel.cpp
#include "models/ReminderFilterModel.hpp"
#include "models/ReminderModel.hpp"

namespace cadence {

ReminderFilterModel::ReminderFilterModel(QObject* parent)
    : QSortFilterProxyModel(parent) {
    setDynamicSortFilter(true);

    connect(this, &QAbstractItemModel::rowsInserted, this, &ReminderFilterModel::filteredCountChanged);
    connect(this, &QAbstractItemModel::rowsRemoved,  this, &ReminderFilterModel::filteredCountChanged);
    connect(this, &QAbstractItemModel::modelReset,   this, &ReminderFilterModel::filteredCountChanged);
    connect(this, &QAbstractItemModel::layoutChanged, this, &ReminderFilterModel::filteredCountChanged);
}

void ReminderFilterModel::setCategoryFilter(const QString& category) {
    const QString normalized = category.trimmed().toLower();
    if (m_categoryFilter == normalized) return;
    m_categoryFilter = normalized;
    invalidateFilter();
    emit filterChanged();
    emit filteredCountChanged();
}

void ReminderFilterModel::setShowCompleted(bool show) {
    if (m_showCompleted == show) return;
    m_showCompleted = show;
    invalidateFilter();
    emit filterChanged();
    emit filteredCountChanged();
}

bool ReminderFilterModel::filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const {
    const QAbstractItemModel* src = sourceModel();
    if (!src) return false;

    const QModelIndex idx = src->index(sourceRow, 0, sourceParent);
    if (!idx.isValid()) return false;

    if (!m_showCompleted) {
        const bool completed = src->data(idx, ReminderModel::IsCompletedRole).toBool();
        if (completed) return false;
    }

    if (m_categoryFilter != QStringLiteral("all") && !m_categoryFilter.isEmpty()) {
        const QString cat = src->data(idx, ReminderModel::CategoryRole).toString();
        if (cat != m_categoryFilter) return false;
    }

    return true;
}

} // namespace cadence
// src/models/ChecklistModel.cpp
#include "models/ChecklistModel.hpp"

#include <algorithm>

namespace cadence {

ChecklistModel::ChecklistModel(DatabaseManager& db, QObject* parent)
    : QAbstractListModel(parent), m_db(db) {
    reload();
}

int ChecklistModel::rowCount(const QModelIndex& parent) const {
    return parent.isValid() ? 0 : static_cast<int>(m_lists.size());
}

QVariant ChecklistModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || index.row() < 0 || index.row() >= rowCount()) {
        return {};
    }

    const Checklist& c = m_lists[static_cast<std::size_t>(index.row())];
    switch (role) {
        case IdRole:             return static_cast<qlonglong>(c.id);
        case SubjectIdRole:      return c.subjectId ? static_cast<qlonglong>(*c.subjectId) : -1LL;
        case TitleRole:          return c.title;
        case CompletedCountRole: return c.completedCount;
        case TotalCountRole:     return c.totalCount;
        default:                 return {};
    }
}

QHash<int, QByteArray> ChecklistModel::roleNames() const {
    return {
        {IdRole,             "checklistId"},
        {SubjectIdRole,      "subjectId"},
        {TitleRole,          "title"},
        {CompletedCountRole, "completedCount"},
        {TotalCountRole,     "totalCount"}
    };
}

void ChecklistModel::reload() {
    beginResetModel();
    m_lists = m_db.fetchAllChecklists();
    endResetModel();
    emit countChanged();
}

bool ChecklistModel::addChecklist(const QString& title, qint64 subjectId) {
    if (title.trimmed().isEmpty()) return false;

    const auto subOpt = subjectId > 0 ? std::optional<std::int64_t>(subjectId) : std::nullopt;
    auto created = m_db.createChecklist(title, subOpt);
    if (!created.has_value()) return false;

    const int row = rowCount();
    beginInsertRows(QModelIndex(), row, row);
    m_lists.push_back(std::move(*created));
    endInsertRows();
    emit countChanged();
    return true;
}

bool ChecklistModel::removeChecklist(qint64 checklistId) {
    if (!m_db.deleteChecklist(checklistId)) return false;

    const auto it = std::find_if(m_lists.begin(), m_lists.end(),
                                 [checklistId](const Checklist& c) { return c.id == checklistId; });
    if (it == m_lists.end()) return false;

    const int row = static_cast<int>(std::distance(m_lists.begin(), it));
    beginRemoveRows(QModelIndex(), row, row);
    m_lists.erase(it);
    endRemoveRows();
    emit countChanged();
    return true;
}

// ============================================================================
// ChecklistItemModel
// ============================================================================

ChecklistItemModel::ChecklistItemModel(DatabaseManager& db, ChecklistModel& parentLists, QObject* parent)
    : QAbstractListModel(parent), m_db(db), m_parentLists(parentLists) {}

void ChecklistItemModel::setActiveChecklistId(qint64 checklistId) {
    if (m_activeChecklistId == checklistId) return;
    m_activeChecklistId = checklistId;
    emit activeChecklistIdChanged();
    reload();
}

int ChecklistItemModel::rowCount(const QModelIndex& parent) const {
    return parent.isValid() ? 0 : static_cast<int>(m_items.size());
}

QVariant ChecklistItemModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || index.row() < 0 || index.row() >= rowCount()) {
        return {};
    }

    const ChecklistItem& item = m_items[static_cast<std::size_t>(index.row())];
    switch (role) {
        case IdRole:           return static_cast<qlonglong>(item.id);
        case ParentItemIdRole: return item.parentItemId ? static_cast<qlonglong>(*item.parentItemId) : -1LL;
        case TitleRole:        return item.title;
        case IsCompletedRole:  return item.isCompleted;
        case DepthRole:        return item.depth;
        case HasChildrenRole:  return item.hasChildren;
        default:               return {};
    }
}

QHash<int, QByteArray> ChecklistItemModel::roleNames() const {
    return {
        {IdRole,           "itemId"},
        {ParentItemIdRole, "parentItemId"},
        {TitleRole,        "title"},
        {IsCompletedRole,  "isCompleted"},
        {DepthRole,        "depth"},
        {HasChildrenRole,  "hasChildren"}
    };
}

void ChecklistItemModel::reload() {
    beginResetModel();
    if (m_activeChecklistId > 0) {
        m_items = m_db.fetchChecklistTree(m_activeChecklistId);
    } else {
        m_items.clear();
    }
    endResetModel();
}

bool ChecklistItemModel::addItem(const QString& title, qint64 parentItemId) {
    if (m_activeChecklistId <= 0 || title.trimmed().isEmpty()) return false;

    const auto parentOpt = parentItemId > 0 ? std::optional<std::int64_t>(parentItemId) : std::nullopt;
    if (!m_db.createChecklistItem(m_activeChecklistId, parentOpt, title).has_value()) {
        return false;
    }

    reload();
    m_parentLists.reload();
    return true;
}

bool ChecklistItemModel::toggleItem(qint64 itemId) {
    const auto it = std::find_if(m_items.begin(), m_items.end(),
                                 [itemId](const ChecklistItem& i) { return i.id == itemId; });
    if (it == m_items.end()) return false;

    const bool nextState = !it->isCompleted;
    if (!m_db.setChecklistItemCompleted(itemId, nextState)) return false;

    it->isCompleted = nextState;
    const int row = static_cast<int>(std::distance(m_items.begin(), it));
    const QModelIndex idx = index(row);
    emit dataChanged(idx, idx, {IsCompletedRole});
    m_parentLists.reload();
    return true;
}

bool ChecklistItemModel::removeItem(qint64 itemId) {
    if (!m_db.deleteChecklistItem(itemId)) return false;
    reload();
    m_parentLists.reload();
    return true;
}

} // namespace cadence
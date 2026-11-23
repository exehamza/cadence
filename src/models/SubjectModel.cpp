// src/models/SubjectModel.cpp
#include "models/SubjectModel.hpp"

#include <algorithm>

namespace cadence {

SubjectModel::SubjectModel(DatabaseManager& db, QObject* parent)
    : QAbstractListModel(parent), m_db(db) {
    reload();
}

int SubjectModel::rowCount(const QModelIndex& parent) const {
    return parent.isValid() ? 0 : static_cast<int>(m_items.size());
}

QVariant SubjectModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || index.row() < 0 || index.row() >= rowCount()) {
        return {};
    }

    const Subject& s = m_items[static_cast<std::size_t>(index.row())];
    switch (role) {
        case IdRole:                  return static_cast<qlonglong>(s.id);
        case NameRole:                return s.name;
        case ColorHexRole:            return s.colorHex;
        case TotalSecondsRole:        return static_cast<qlonglong>(s.totalSecondsStudied);
        case TotalHoursRole:          return s.totalHours();
        case TargetWeeklyMinutesRole: return s.targetWeeklyMinutes;
        default:                      return {};
    }
}

QHash<int, QByteArray> SubjectModel::roleNames() const {
    return {
        {IdRole,                  "subjectId"},
        {NameRole,                "name"},
        {ColorHexRole,            "colorHex"},
        {TotalSecondsRole,        "totalSeconds"},
        {TotalHoursRole,          "totalHours"},
        {TargetWeeklyMinutesRole, "targetWeeklyMinutes"}
    };
}

void SubjectModel::reload() {
    beginResetModel();
    m_items = m_db.fetchAllSubjects(false);
    endResetModel();
    emit countChanged();
}

bool SubjectModel::addSubject(const QString& name, const QString& colorHex, int targetWeeklyMinutes) {
    if (name.trimmed().isEmpty()) return false;

    auto created = m_db.createSubject(name, colorHex, targetWeeklyMinutes);
    if (!created.has_value()) return false;

    const int insertRow = rowCount();
    beginInsertRows(QModelIndex(), insertRow, insertRow);
    m_items.push_back(std::move(*created));
    endInsertRows();
    emit countChanged();
    return true;
}

bool SubjectModel::editSubject(qint64 id, const QString& name, const QString& colorHex, int targetWeeklyMinutes) {
    if (!m_db.updateSubject(id, name, colorHex, targetWeeklyMinutes)) return false;

    const auto it = std::find_if(m_items.begin(), m_items.end(),
                                 [id](const Subject& s) { return s.id == id; });
    if (it == m_items.end()) return false;

    it->name                = name.trimmed();
    it->colorHex            = colorHex;
    it->targetWeeklyMinutes = targetWeeklyMinutes;

    const int row = static_cast<int>(std::distance(m_items.begin(), it));
    const QModelIndex idx = index(row);
    emit dataChanged(idx, idx, {NameRole, ColorHexRole, TargetWeeklyMinutesRole});
    return true;
}

bool SubjectModel::removeSubject(qint64 id) {
    if (!m_db.deleteSubject(id)) return false;

    const auto it = std::find_if(m_items.begin(), m_items.end(),
                                 [id](const Subject& s) { return s.id == id; });
    if (it == m_items.end()) return false;

    const int row = static_cast<int>(std::distance(m_items.begin(), it));
    beginRemoveRows(QModelIndex(), row, row);
    m_items.erase(it);
    endRemoveRows();
    emit countChanged();
    return true;
}

void SubjectModel::appendStudiedSeconds(std::int64_t subjectId, int seconds) {
    const auto it = std::find_if(m_items.begin(), m_items.end(),
                                 [subjectId](const Subject& s) { return s.id == subjectId; });
    if (it == m_items.end()) return;

    it->totalSecondsStudied += seconds;
    const int row = static_cast<int>(std::distance(m_items.begin(), it));
    const QModelIndex idx = index(row);
    emit dataChanged(idx, idx, {TotalSecondsRole, TotalHoursRole});
}

} // namespace cadence
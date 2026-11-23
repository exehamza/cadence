// src/models/ChecklistModel.hpp
#pragma once

#include "db/DatabaseManager.hpp"
#include "models/DomainEntities.hpp"

#include <QAbstractListModel>
#include <vector>

namespace cadence {

// Manages top-level Long-Term Checklists (e.g., "CS Books to Read")
class ChecklistModel : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)

public:
    enum Roles {
        IdRole = Qt::UserRole + 1,
        SubjectIdRole,
        TitleRole,
        CompletedCountRole,
        TotalCountRole
    };
    Q_ENUM(Roles)

    explicit ChecklistModel(DatabaseManager& db, QObject* parent = nullptr);

    [[nodiscard]] int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    [[nodiscard]] QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    [[nodiscard]] QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void reload();
    Q_INVOKABLE bool addChecklist(const QString& title, qint64 subjectId = -1);
    Q_INVOKABLE bool removeChecklist(qint64 checklistId);

signals:
    void countChanged();

private:
    DatabaseManager&       m_db;
    std::vector<Checklist> m_lists;
};

// Manages flattened hierarchical sub-items for the currently selected Checklist
class ChecklistItemModel : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY(qint64 activeChecklistId READ activeChecklistId WRITE setActiveChecklistId NOTIFY activeChecklistIdChanged)

public:
    enum Roles {
        IdRole = Qt::UserRole + 1,
        ParentItemIdRole,
        TitleRole,
        IsCompletedRole,
        DepthRole,
        HasChildrenRole
    };
    Q_ENUM(Roles)

    explicit ChecklistItemModel(DatabaseManager& db, ChecklistModel& parentLists, QObject* parent = nullptr);

    [[nodiscard]] qint64 activeChecklistId() const noexcept { return m_activeChecklistId; }
    void setActiveChecklistId(qint64 checklistId);

    [[nodiscard]] int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    [[nodiscard]] QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    [[nodiscard]] QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void reload();
    Q_INVOKABLE bool addItem(const QString& title, qint64 parentItemId = -1);
    Q_INVOKABLE bool toggleItem(qint64 itemId);
    Q_INVOKABLE bool removeItem(qint64 itemId);

signals:
    void activeChecklistIdChanged();

private:
    DatabaseManager&           m_db;
    ChecklistModel&            m_parentLists;
    std::int64_t               m_activeChecklistId{-1};
    std::vector<ChecklistItem> m_items;
};

} // namespace cadence
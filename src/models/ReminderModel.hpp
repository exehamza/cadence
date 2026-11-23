// src/models/ReminderModel.hpp
#pragma once

#include "db/DatabaseManager.hpp"
#include "models/DomainEntities.hpp"

#include <QAbstractListModel>
#include <vector>

namespace cadence {

class ReminderModel : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)

public:
    enum Roles {
        IdRole = Qt::UserRole + 1,
        SubjectIdRole,
        SubjectNameRole,
        SubjectColorHexRole,
        TitleRole,
        DescriptionRole,
        CategoryRole,
        DueAtRole,
        DueDateRole,
        IsCompletedRole
    };
    Q_ENUM(Roles)

    explicit ReminderModel(DatabaseManager& db, QObject* parent = nullptr);

    [[nodiscard]] int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    [[nodiscard]] QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    [[nodiscard]] QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void reload();
    Q_INVOKABLE bool addReminder(const QString& title,
                                 const QString& description,
                                 const QString& categoryStr, // "task" | "assignment" | "examination"
                                 qint64 subjectId,           // <= 0 if unlinked
                                 qint64 dueAtEpoch,          // <= 0 if undated
                                 const QString& dueDateIso);
    Q_INVOKABLE bool toggleCompleted(qint64 id);
    Q_INVOKABLE bool pushToTomorrow(qint64 id);
    Q_INVOKABLE bool removeReminder(qint64 id);

signals:
    void countChanged();
    void reminderScheduleChanged(); // Notifies CalendarViewModel in Phase 3

private:
    DatabaseManager&      m_db;
    std::vector<Reminder> m_items;
};

} // namespace cadence
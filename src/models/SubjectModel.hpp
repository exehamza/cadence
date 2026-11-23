// src/models/SubjectModel.hpp
#pragma once

#include "db/DatabaseManager.hpp"
#include "models/DomainEntities.hpp"

#include <QAbstractListModel>
#include <vector>

namespace cadence {

class SubjectModel : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)

public:
    enum Roles {
        IdRole = Qt::UserRole + 1,
        NameRole,
        ColorHexRole,
        TotalSecondsRole,
        TotalHoursRole,
        TargetWeeklyMinutesRole
    };
    Q_ENUM(Roles)

    explicit SubjectModel(DatabaseManager& db, QObject* parent = nullptr);

    [[nodiscard]] int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    [[nodiscard]] QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    [[nodiscard]] QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void reload();
    Q_INVOKABLE bool addSubject(const QString& name, const QString& colorHex, int targetWeeklyMinutes = 0);
    Q_INVOKABLE bool editSubject(qint64 id, const QString& name, const QString& colorHex, int targetWeeklyMinutes);
    Q_INVOKABLE bool removeSubject(qint64 id);

    // Called by FocusTimerViewModel (Phase 2) when a focus session finishes
    void appendStudiedSeconds(std::int64_t subjectId, int seconds);

signals:
    void countChanged();

private:
    DatabaseManager&     m_db;
    std::vector<Subject> m_items;
};

} // namespace cadence
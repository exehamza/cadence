#include "db/DatabaseManager.hpp"
#include "models/ChecklistModel.hpp"
#include "models/ReminderFilterModel.hpp"
#include "models/ReminderModel.hpp"
#include "models/SubjectModel.hpp"
#include "viewmodels/AnalyticsViewModel.hpp"
#include "viewmodels/CalendarViewModel.hpp"
#include "viewmodels/FocusTimerViewModel.hpp"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QStandardPaths>

int main(int argc, char* argv[]) {
    QGuiApplication app(argc, argv);
    app.setOrganizationName(QStringLiteral("Cadence"));
    app.setApplicationName(QStringLiteral("Cadence"));

    const QString dataDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    const QString dbPath  = dataDir + QStringLiteral("/cadence.sqlite3");

    cadence::DatabaseManager dbManager;
    if (!dbManager.initialize(dbPath)) {
        return 1;
    }

    cadence::SubjectModel        subjectModel(dbManager);
    cadence::ReminderModel       reminderModel(dbManager);
    cadence::ReminderFilterModel reminderFilterModel;
    reminderFilterModel.setSourceModel(&reminderModel);

    cadence::ChecklistModel      checklistModel(dbManager);
    cadence::ChecklistItemModel  checklistItemModel(dbManager, checklistModel);
    cadence::FocusTimerViewModel focusTimerViewModel(dbManager, subjectModel);
    cadence::CalendarViewModel   calendarViewModel(dbManager);
    cadence::AnalyticsViewModel  analyticsViewModel(dbManager);

    // Keep Calendar synchronized with Reminder and Subject mutations
    QObject::connect(&reminderModel, &cadence::ReminderModel::reminderScheduleChanged,
                     &calendarViewModel, &cadence::CalendarViewModel::refresh);
    QObject::connect(&subjectModel, &QAbstractItemModel::dataChanged,
                     &calendarViewModel, &cadence::CalendarViewModel::refresh);
    QObject::connect(&subjectModel, &QAbstractItemModel::rowsRemoved,
                     &calendarViewModel, &cadence::CalendarViewModel::refresh);

    // Keep Analytics Dashboard synchronized with Focus Sessions, Exams & Subjects
    QObject::connect(&focusTimerViewModel, &cadence::FocusTimerViewModel::focusSessionLogged,
                     &analyticsViewModel, &cadence::AnalyticsViewModel::refresh);
    QObject::connect(&reminderModel, &cadence::ReminderModel::reminderScheduleChanged,
                     &analyticsViewModel, &cadence::AnalyticsViewModel::refresh);
    QObject::connect(&subjectModel, &QAbstractItemModel::dataChanged,
                     &analyticsViewModel, &cadence::AnalyticsViewModel::refresh);
    QObject::connect(&subjectModel, &QAbstractItemModel::rowsRemoved,
                     &analyticsViewModel, &cadence::AnalyticsViewModel::refresh);

    qmlRegisterSingletonInstance("Cadence", 1, 0, "SubjectModel",       &subjectModel);
    qmlRegisterSingletonInstance("Cadence", 1, 0, "ReminderModel",      &reminderModel);
    qmlRegisterSingletonInstance("Cadence", 1, 0, "ReminderFilter",     &reminderFilterModel);
    qmlRegisterSingletonInstance("Cadence", 1, 0, "ChecklistModel",     &checklistModel);
    qmlRegisterSingletonInstance("Cadence", 1, 0, "ChecklistItemModel", &checklistItemModel);
    qmlRegisterSingletonInstance("Cadence", 1, 0, "FocusTimer",         &focusTimerViewModel);
    qmlRegisterSingletonInstance("Cadence", 1, 0, "CalendarVM",         &calendarViewModel);
    qmlRegisterSingletonInstance("Cadence", 1, 0, "AnalyticsVM",        &analyticsViewModel);

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection
    );

    engine.loadFromModule("Cadence", "Main");
    return app.exec();
}
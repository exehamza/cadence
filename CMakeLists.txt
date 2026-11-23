cmake_minimum_required(VERSION 3.21)

project(Cadence VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS
    Core
    Gui
    Quick
    QuickControls2
    Sql
    Multimedia
)

qt_standard_project_setup(REQUIRES 6.5)
qt_policy(SET QTP0004 NEW)

qt_add_executable(CadenceApp
    src/main.cpp
    src/core/Types.hpp
    src/db/DatabaseManager.hpp
    src/db/DatabaseManager.cpp
    src/models/DomainEntities.hpp
    src/models/SubjectModel.hpp
    src/models/SubjectModel.cpp
    src/models/ReminderModel.hpp
    src/models/ReminderModel.cpp
    src/models/ReminderFilterModel.hpp
    src/models/ReminderFilterModel.cpp
    src/models/ChecklistModel.hpp
    src/models/ChecklistModel.cpp
    src/viewmodels/FocusTimerViewModel.hpp
    src/viewmodels/FocusTimerViewModel.cpp
    src/viewmodels/CalendarViewModel.hpp
    src/viewmodels/CalendarViewModel.cpp
    src/viewmodels/AnalyticsViewModel.hpp
    src/viewmodels/AnalyticsViewModel.cpp
)

qt_add_qml_module(CadenceApp
    URI Cadence
    VERSION 1.0
    QML_FILES
        src/qml/Main.qml
        src/qml/views/TimerView.qml
        src/qml/views/PlannerView.qml
        src/qml/views/CalendarView.qml
        src/qml/views/AnalyticsView.qml
        src/qml/components/CircularTimerGauge.qml
        src/qml/components/SubjectPickerDialog.qml
        src/qml/components/SubjectManagerDialog.qml
        src/qml/components/AddEventDialog.qml
        src/qml/components/SubjectDonutChart.qml
        src/qml/components/ActivityHeatmap.qml
        src/qml/components/ExamCountdownCard.qml
    RESOURCES
        resources/sql/schema.sql
)

target_include_directories(CadenceApp PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(CadenceApp PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Quick
    Qt6::QuickControls2
    Qt6::Sql
    Qt6::Multimedia
)

if(MSVC)
    target_compile_options(CadenceApp PRIVATE /W4 /permissive-)
else()
    target_compile_options(CadenceApp PRIVATE -Wall -Wextra -Wpedantic)
endif()

set_target_properties(CadenceApp PROPERTIES
    OUTPUT_NAME cadence
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)
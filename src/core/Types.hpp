#pragma once

#include <cstdint>
#include <string_view>

namespace cadence {

enum class SessionMode : std::uint8_t {
    Pomodoro = 0,
    Flowtime = 1
};

enum class ReminderCategory : std::uint8_t {
    Task        = 0,
    Assignment  = 1,
    Examination = 2
};

[[nodiscard]] constexpr std::string_view toSqlString(SessionMode mode) noexcept {
    switch (mode) {
        case SessionMode::Pomodoro: return "pomodoro";
        case SessionMode::Flowtime: return "flowtime";
    }
    return "pomodoro";
}

[[nodiscard]] constexpr SessionMode sessionModeFromSql(std::string_view raw) noexcept {
    if (raw == "flowtime") return SessionMode::Flowtime;
    return SessionMode::Pomodoro;
}

[[nodiscard]] constexpr std::string_view toSqlString(ReminderCategory category) noexcept {
    switch (category) {
        case ReminderCategory::Task:        return "task";
        case ReminderCategory::Assignment:  return "assignment";
        case ReminderCategory::Examination: return "examination";
    }
    return "task";
}

[[nodiscard]] constexpr ReminderCategory reminderCategoryFromSql(std::string_view raw) noexcept {
    if (raw == "assignment")  return ReminderCategory::Assignment;
    if (raw == "examination") return ReminderCategory::Examination;
    return ReminderCategory::Task;
}

} // namespace cadence
#include "../include/Validator.h"
#include <chrono>

bool Validator::ValidateTime() {
    const auto now = std::chrono::system_clock::now();
    const auto time_t_now = std::chrono::system_clock::to_time_t(now);
    const std::tm* tm_now = std::localtime(&time_t_now);

    if (tm_now->tm_wday == 0 || tm_now->tm_wday == 6) {
        return false;
    }

    std::tm start_tm = *tm_now;
    start_tm.tm_hour = 11;
    start_tm.tm_min = 0;
    start_tm.tm_sec = 0;
    const auto start_time = std::chrono::system_clock::from_time_t(std::mktime(&start_tm));

    std::tm end_tm = *tm_now;
    end_tm.tm_hour = 20;
    end_tm.tm_min = 0;
    end_tm.tm_sec = 0;

    if (auto end_time = std::chrono::system_clock::from_time_t(std::mktime(&end_tm)); start_time <= end_time) {
        return (now >= start_time) && (now <= end_time);
    } else {
        return (now >= start_time) || (now <= end_time);
    }
}

#include "../include/Validator.h"
#include <chrono>
#include "../include/Api/Api.h"
#include "../include/Config.h"
#include "../include/Database/Insert.h"

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

void Validator::ValidateConns() {
    const Config cfg;
    const auto& env = cfg.GetEnv();

    const auto conns = Api::GetConnections(env.LoginArca, env.PasswordArca);

    if (conns.empty()) {
        std::clog << "ERROR: No instances came from the api, skipping.\n";
        return;
    }

    const auto insert = Insert(env.DbUrlV3);


    for (const auto& conn : conns) {
        if (conn.Status == false) {
            insert.SetBanned(conn.InstanceNumber);
        }
    }
}

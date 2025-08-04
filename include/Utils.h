#ifndef INCLUDE_UTILS_H
#define INCLUDE_UTILS_H

#include <string>

class Utils {
public:
    Utils() = delete;
    static std::string FormatPhone(const std::string& phone) {
        if (phone.length() >= 2 && phone.starts_with("55")) {
            return phone;
        }
        return "55" + phone;
    }
};

#endif //INCLUDE_UTILS_H

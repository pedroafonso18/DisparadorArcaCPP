#ifndef INCLUDE_API_API_H
#define INCLUDE_API_API_H

#include <iostream>
#include "Models.h"

class Api {
public:
    Api() = delete;
    static bool SendMessageApi(Request* req);
    static const Instance* ValidateConns(const std::string& ApiToken);
};

#endif //INCLUDE_API_API_H

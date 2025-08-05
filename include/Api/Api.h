#ifndef INCLUDE_API_API_H
#define INCLUDE_API_API_H

#include <iostream>
#include <vector>

#include "Models.h"

class Api {
private:
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);
    static std::string Login(const std::string& email, const std::string& password);
public:
    Api() = delete;
    static bool SendMessageApi(Request* req);
    static std::vector<Instance_t> GetConnections(const std::string& email, const std::string& password);
};

#endif //INCLUDE_API_API_H

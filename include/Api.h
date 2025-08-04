#ifndef INCLUDE_API_H
#define INCLUDE_API_H

#include <iostream>

class Api {
private:
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
        size_t totalSize = size * nmemb;
        auto* resp = static_cast<std::string*>(userp);
        resp->append(static_cast<char*>(contents), totalSize);
        return totalSize;
    }
public:
    typedef struct {
        std::string ApiKey;
        std::string Number;
        std::string Body;
    } Request;
    Api() = delete;
    static bool SendMessageApi(Request* req);
};

#endif //INCLUDE_API_H

#ifndef INCLUDE_API_H
#define INCLUDE_API_H

#include <iostream>

class Api {
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

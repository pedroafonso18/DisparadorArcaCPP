#ifndef INCLUDE_API_MODELS_H
#define INCLUDE_API_MODELS_H

#include <string>

typedef struct {
    std::string ApiKey;
    std::string Number;
    std::string Body;
} Request;

typedef struct {
    std::string InstanceName;
    std::string InstanceNumber;
    bool Status;
    int Id;
} Instance;

#endif //INCLUDE_API_MODELS_H

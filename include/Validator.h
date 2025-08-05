#ifndef INCLUDE_VALIDATOR_H
#define INCLUDE_VALIDATOR_H

class Validator {
public:
    Validator() = delete;
    static bool ValidateTime();
    static void ValidateConns();
    static void SendMessage();
};

#endif //INCLUDE_VALIDATOR_H
#ifndef INCLUDE_CONFIG_H
#define INCLUDE_CONFIG_H

#include <string>
#include <memory>

class Config {
private:
    typedef struct{
        std::string DbUrl;
        std::string DbUrlV3;
        std::string DbUrlTrafego;
        std::string LoginArca;
        std::string PasswordArca;
    } Env;
    std::unique_ptr<Env> env;
public:
    Config();
    [[nodiscard]] const Env& GetEnv() const;
};


#endif
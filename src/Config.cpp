#include "../include/Config.h"
#include "../third_party/dotenv.h"
#include <memory>

Config::Config() : env(std::make_unique<Env>()){
    const std::string root_path = "../.env";
    dotenv::init(root_path.c_str());
}

const Config::Env &Config::GetEnv() const {
    env->DbUrl = dotenv::getenv("DB_URL");
    env->DbUrlTrafego = dotenv::getenv("DB_URL_TRAFEGO");
    env->DbUrlV3 = dotenv::getenv("DB_URL_V3");
    env->LoginArca = dotenv::getenv("LOGIN_ARCA");
    env->PasswordArca = dotenv::getenv("PASSWORD_ARCA");

    return *env;
}

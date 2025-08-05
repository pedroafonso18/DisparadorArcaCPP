#include "../include/Config.h"
#include "../third_party/dotenv.h"
#include <memory>
#include <iostream>

Config::Config() : env(std::make_unique<Env>()){
    std::clog << "[Config::Config] Initializing dotenv with ../.env" << std::endl;
    const std::string root_path = "../.env";
    dotenv::init(root_path.c_str());
}

const Config::Env &Config::GetEnv() const {
    std::clog << "[Config::GetEnv] Loading environment variables" << std::endl;
    env->DbUrl = dotenv::getenv("DB_URL");
    env->DbUrlTrafego = dotenv::getenv("DB_URL_TRAFEGO");
    env->DbUrlV3 = dotenv::getenv("DB_URL_V3");
    env->LoginArca = dotenv::getenv("LOGIN_ARCA");
    env->PasswordArca = dotenv::getenv("PASSWORD_ARCA");
    std::clog << "[Config::GetEnv] DB_URL: " << env->DbUrl << std::endl;
    std::clog << "[Config::GetEnv] DB_URL_TRAFEGO: " << env->DbUrlTrafego << std::endl;
    std::clog << "[Config::GetEnv] DB_URL_V3: " << env->DbUrlV3 << std::endl;
    std::clog << "[Config::GetEnv] LOGIN_ARCA: " << env->LoginArca << std::endl;
    // Do not log password for security
    return *env;
}

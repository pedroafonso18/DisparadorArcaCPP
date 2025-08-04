cmake_minimum_required(VERSION 3.31)
project(DisparadorArca)

set(CMAKE_CXX_STANDARD 20)

add_executable(DisparadorArca src/main.cpp
        include/Config.h
        third_party/dotenv.h
        src/Config.cpp
        include/Utils.h
        include/Api.h
        src/Api.cpp
        third_party/json.h)

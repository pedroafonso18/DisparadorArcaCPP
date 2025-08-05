cmake_minimum_required(VERSION 3.31)
project(DisparadorArca)

set(CMAKE_CXX_STANDARD 20)

set(MSYS_ROOT "C:/msys64/mingw64")

find_package(CURL REQUIRED)
find_package(OpenSSL REQUIRED)

set(CURL_ROOT_DIR "${MSYS_ROOT}")
set(OPENSSL_ROOT_DIR "${MSYS_ROOT} ${MSYS_ROOT}/include")

include_directories(${CURL_INCLUDE_DIRS} ${OPENSSL_INCLUDE_DIR})

add_executable(DisparadorArca src/main.cpp
        include/Config.h
        third_party/dotenv.h
        src/Config.cpp
        include/Utils.h
        include/Api/Api.h
        src/Api.cpp
        third_party/json.h
        include/Database/Database.h
        include/Database/Fetch.h
        include/Database/Models.h
        include/Database/Insert.h
        include/Validator.h
        include/Api/Models.h
        src/Database/Insert.cpp
        src/Database/Fetch.cpp
        src/Validator.cpp)

target_link_libraries(DisparadorArca PRIVATE pqxx pq ${OPENSSL_LIBRARIES} ${CURL_LIBRARIES}
)
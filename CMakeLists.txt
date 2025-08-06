cmake_minimum_required(VERSION 3.22.1)
project(DisparadorArca)

set(CMAKE_CXX_STANDARD 20)

if(WIN32)
    set(MSYS_ROOT "C:/msys64/mingw64")

    set(OPENSSL_ROOT_DIR "${MSYS_ROOT}")
    set(OPENSSL_INCLUDE_DIR "${OPENSSL_ROOT_DIR}/include")

    set(ZLIB_ROOT "${MSYS_ROOT}")
    set(ZLIB_INCLUDE_DIR "${ZLIB_ROOT}/include")
    set(ZLIB_LIBRARY "${ZLIB_ROOT}/lib/libz.dll.a")
endif()

find_package(CURL REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(ZLIB REQUIRED)

include_directories(
        ${CURL_INCLUDE_DIRS}
        ${OPENSSL_INCLUDE_DIR}
        ${ZLIB_INCLUDE_DIR}
)

add_executable(DisparadorArca
        src/main.cpp
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
        src/Validator.cpp
)

if(WIN32)
    target_link_libraries(DisparadorArca PRIVATE pqxx pq ${OPENSSL_LIBRARIES} ${CURL_LIBRARIES} ${ZLIB_LIBRARY})
else()
    target_link_libraries(DisparadorArca PRIVATE pqxx pq ${OPENSSL_LIBRARIES} ${CURL_LIBRARIES} ${ZLIB_LIBRARIES})
endif()

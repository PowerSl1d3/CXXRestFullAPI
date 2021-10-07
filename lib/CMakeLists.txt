cmake_minimum_required(VERSION 3.16)
project(CXXRestFullAPI)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenSSL)

if(OPENSSL_FOUND)
    include_directories(${OPENSSL_INCLUDE_DIR})
endif()

add_library(dataBase SHARED
        dataBase.cpp
        taskDataBase.cpp
        userDataBase.cpp
        connection.cpp
        sha256.cpp
        cryptographer.cpp)

target_include_directories(dataBase PUBLIC ../inc)
target_link_libraries(dataBase PUBLIC mysqlcppconn)

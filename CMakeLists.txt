cmake_minimum_required(VERSION 3.16)
project(CXXRestFullAPI)

set(CMAKE_CXX_STANDARD 17)

find_package(Threads)
find_package(ZLIB)
find_package(OpenSSL)

if(OPENSSL_FOUND)
    include_directories(${OPENSSL_INCLUDE_DIR})
endif()

find_package(Boost REQUIRED)

add_executable(CXXRestFullAPI main.cpp DataBase.h userDataBase.h userDataBase.cpp taskDataBase.cpp taskDataBase.h dataBase.cpp dataBase.h)

target_link_libraries(CXXRestFullAPI PUBLIC ${Boost_LIBRARIES} pthread mysqlcppconn)

cmake_minimum_required(VERSION 3.16)
project(CXXRestFullAPI)

set(CMAKE_CXX_STANDARD 17)

find_package(Threads)
find_package(ZLIB)
find_package(OpenSSL)

if(OPENSSL_FOUND)
    include_directories(${OPENSSL_INCLUDE_DIR})
endif()

add_subdirectory(lib)

find_package(Boost REQUIRED COMPONENTS program_options)

add_executable(CXXRestFullAPI main.cpp)

target_link_libraries(CXXRestFullAPI PUBLIC
        ${Boost_LIBRARIES}
        pthread
        dataBase)

target_include_directories(CXXRestFullAPI PUBLIC inc)

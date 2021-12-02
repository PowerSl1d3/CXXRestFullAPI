cmake_minimum_required(VERSION 3.16)
project(CXXRestFullAPI VERSION 0.0.1)

find_package(Crow REQUIRED)
find_package(Threads)
find_package(ZLIB)
find_package(OpenSSL)
if(OPENSSL_FOUND)
    include_directories(${OPENSSL_INCLUDE_DIR})
endif()
find_package(Boost COMPONENTS program_options unit_test_framework REQUIRED)
add_subdirectory(lib)

add_executable(CXXRestFullAPI main.cpp)

add_executable(Test test.cpp)

set_target_properties(CXXRestFullAPI Test PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        COMPILE_OPTIONS "-Wpedantic;-Wall"
        )

set_target_properties(Test PROPERTIES
        COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
        )

target_link_libraries(CXXRestFullAPI PUBLIC
        ${Boost_LIBRARIES}
        Crow::Crow
        pthread
        dataBase)

target_link_libraries(Test PUBLIC
        ${Boost_LIBRARIES}
        mysqlcppconn
        )

target_include_directories(CXXRestFullAPI PUBLIC inc)

install(TARGETS CXXRestFullAPI RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})

set(CPACK_PACKAGE_CONTACT manikryte@yandex.ru)

set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)

include(CPack)

enable_testing()

add_test(CXX_RestFullAPI_test Test)
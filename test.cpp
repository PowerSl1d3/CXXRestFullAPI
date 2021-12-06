//
// Created by olegaksenenko on 11/30/21.
//
#define BOOST_TEST_MODULE Test

#include <boost/test/unit_test.hpp>

#include <mysql_connection.h>
#include <mysql_driver.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

#include <memory>

BOOST_AUTO_TEST_SUITE(restfull_api)

BOOST_AUTO_TEST_CASE(check_mysql_connection_is_valid) {
    std::unique_ptr<sql::mysql::MySQL_Driver,
    std::function<void(sql::mysql::MySQL_Driver*)>> driver(sql::mysql::get_mysql_driver_instance(),
                                                           [](sql::mysql::MySQL_Driver*){});
    //BOOST_CHECK(driver);
    std::unique_ptr<sql::Connection> con(driver->connect("tcp://127.0.0.1:3306",
                                                             "root",
                                                             "H1l6KWq8N1"));
    //BOOST_CHECK(con);
}

BOOST_AUTO_TEST_SUITE_END()
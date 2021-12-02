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

BOOST_AUTO_TEST_CASE(example) {
        BOOST_CHECK(1 == 1);
}

BOOST_AUTO_TEST_CASE(example2) {
    BOOST_CHECK(2 == 2);
}

BOOST_AUTO_TEST_SUITE_END()
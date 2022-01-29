//
// Created by olegaksenenko on 10/5/21.
//

#ifndef CXXRESTFULLAPI_CONNECTION_H
#define CXXRESTFULLAPI_CONNECTION_H

#include <memory>

#include <mysql_connection.h>
#include <mysql_driver.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

class connection {
private:
    sql::mysql::MySQL_Driver* driver;
    std::shared_ptr<sql::Connection> con;
public:
    connection(const std::string& hostname, const std::string& username, const std::string& password);
    std::shared_ptr<sql::Connection> getConnection();
};

#endif //CXXRESTFULLAPI_CONNECTION_H

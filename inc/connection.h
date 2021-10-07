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
    const std::unique_ptr<sql::mysql::MySQL_Driver> driver;
    const std::unique_ptr<sql::Connection> con;
    std::shared_ptr<sql::Statement> stmt;
public:
    connection(const std::string& username, const std::string& password);
    std::shared_ptr<sql::Statement> getStatement();
};

#endif //CXXRESTFULLAPI_CONNECTION_H

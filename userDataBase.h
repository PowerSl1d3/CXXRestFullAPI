//
// Created by olegaksenenko on 10/2/21.
//

#ifndef CXXRESTFULLAPI_USERDATABASE_H
#define CXXRESTFULLAPI_USERDATABASE_H

#include <mysql_connection.h>
#include <mysql_driver.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

class userDataBase {
private:
    sql::mysql::MySQL_Driver *driver;
    sql::Connection *con;
    sql::Statement *stmt;
    sql::ResultSet *res;
public:
    userDataBase();
    void createUser(const std::string& username, const std::string& password, int task_id);
    sql::ResultSet* getUserTaskId(const std::string& username, const std::string& password);
};

#endif //CXXRESTFULLAPI_USERDATABASE_H

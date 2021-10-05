//
// Created by olegaksenenko on 10/2/21.
//

#ifndef CXXRESTFULLAPI_TASKDATABASE_H
#define CXXRESTFULLAPI_TASKDATABASE_H

#include <string>

#include <mysql_connection.h>
#include <mysql_driver.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

class taskDataBase {
private:
    sql::mysql::MySQL_Driver* driver;
    sql::Connection* con;
    sql::Statement* stmt;
    sql::ResultSet* res;
public:
    taskDataBase();
    void createTask(const int userTaskId, const std::string& text);
    sql::ResultSet* getToDo(const int userTaskId);
    void deleteToDo(const int userTaskId, const int todoId);
    void updateToDo(const int todoId, const int userTaskId, const std::string& text);
};
#endif //CXXRESTFULLAPI_TASKDATABASE_H

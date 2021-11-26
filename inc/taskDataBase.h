//
// Created by olegaksenenko on 10/2/21.
//

#ifndef CXXRESTFULLAPI_TASKDATABASE_H
#define CXXRESTFULLAPI_TASKDATABASE_H

#include <string>
#include <memory>

#include <mysql_connection.h>
#include <mysql_driver.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

class taskDataBase {
private:
    const std::shared_ptr<sql::Connection> con_;
    const std::unique_ptr<sql::Statement> stmt_;
    std::unique_ptr<sql::PreparedStatement> preparedStatement_;
public:
    taskDataBase(std::shared_ptr<sql::Connection>);
    void createTask(const int userId, const std::string& text);
    std::unique_ptr<sql::ResultSet> getToDo(const int userId);
    void deleteToDo(const int userTaskId, const int todoId);
    void updateToDo(const int todoId, const int userTaskId, const std::string& text);
};
#endif //CXXRESTFULLAPI_TASKDATABASE_H

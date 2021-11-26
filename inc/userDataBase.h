//
// Created by olegaksenenko on 10/2/21.
//

#ifndef CXXRESTFULLAPI_USERDATABASE_H
#define CXXRESTFULLAPI_USERDATABASE_H

#include <memory>

#include <mysql_connection.h>
#include <mysql_driver.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

class userDataBase {
private:
    const std::shared_ptr<sql::Connection> con_;
    const std::unique_ptr<sql::Statement> stmt_;
    std::unique_ptr<sql::PreparedStatement> preparedStatement_;
public:
    userDataBase(const std::shared_ptr<sql::Connection>&);
    //TODO: change method to new architecture of data base
    bool checkUserAlreadyExist(const std::string& username, const std::string& password);
    void createUser(const std::string& username, const std::string& password);
    std::unique_ptr<sql::ResultSet> getUserId(const std::string& username, const std::string& password);
};

#endif //CXXRESTFULLAPI_USERDATABASE_H

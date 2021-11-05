//
// Created by olegaksenenko on 10/2/21.
//

#include "userDataBase.h"

userDataBase::userDataBase(const std::shared_ptr<sql::Connection>& con) :
        con_(con),
        stmt_(con->createStatement())
{
    stmt_->execute("USE mysql");
    stmt_->execute("DROP TABLE IF EXISTS todoUser");
    stmt_->execute("CREATE TABLE todoUser(id INT PRIMARY KEY AUTO_INCREMENT,"
                  " username VARCHAR(30), password VARCHAR(256));");
}

bool userDataBase::checkUserAlreadyExist(const std::string& username, const std::string& password) {
    const std::string statement = "SELECT * FROM todoUser WHERE username = ? AND password = ?";
    preparedStatement_.reset(con_->prepareStatement(statement));
    preparedStatement_->setString(1, username);
    preparedStatement_->setString(2, password);
    return preparedStatement_->executeQuery()->rowsCount();
}

void userDataBase::createUser(const std::string& username, const std::string& password) {
    const std::string statement = "INSERT INTO todoUser(username, password) VALUES (?, ?)";
    preparedStatement_.reset(con_->prepareStatement(statement));
    preparedStatement_->setString(1, username);
    preparedStatement_->setString(2, password);
    preparedStatement_->execute();
}

std::unique_ptr<sql::ResultSet> userDataBase::getUserId(const std::string& username, const std::string& password) {
    const std::string statement = "SELECT id FROM todoUser WHERE username = ? AND password = ?";
    preparedStatement_.reset(con_->prepareStatement(statement));
    preparedStatement_->setString(1, username);
    preparedStatement_->setString(2, password);
    return std::unique_ptr<sql::ResultSet>(preparedStatement_->executeQuery());
}
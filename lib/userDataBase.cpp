//
// Created by olegaksenenko on 10/2/21.
//

#include "userDataBase.h"

userDataBase::userDataBase(std::shared_ptr<sql::Statement> stmt_) :
stmt(stmt_)
{
    stmt->execute("USE mysql");
    stmt->execute("DROP TABLE IF EXISTS todoUser");
    stmt->execute("CREATE TABLE todoUser(id INT PRIMARY KEY AUTO_INCREMENT,"
                  " username VARCHAR(30), password VARCHAR(256));");
}

bool userDataBase::checkUserAlreadyExist(const std::string& username, const std::string& password) {
    return stmt->executeQuery("SELECT * FROM todoUser WHERE username = '" + username +
    "' AND password = '" + password + "'")->rowsCount();
}

void userDataBase::createUser(const std::string& username, const std::string& password) {
    stmt->execute("INSERT INTO todoUser(username, password)"
                  " VALUES ('"
                  + username +
                  "', '"
                  + password +
                  "')");
}

std::unique_ptr<sql::ResultSet> userDataBase::getUserId(const std::string& username, const std::string& password) {
    return std::unique_ptr<sql::ResultSet>(
    stmt->executeQuery("SELECT id FROM todoUser WHERE username = '"
                              + username +
                              "' AND password = '"
                              + password +
                              "'")
    );
}
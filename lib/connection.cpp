//
// Created by olegaksenenko on 10/5/21.
//

#include "connection.h"

connection::connection(const std::string& username, const std::string& password) :
driver(sql::mysql::get_mysql_driver_instance()),
con(driver->connect("tcp://127.0.0.1:3306", username, password)),
stmt(con->createStatement())
{}

std::shared_ptr<sql::Statement> connection::getStatement() {
    return stmt;
}
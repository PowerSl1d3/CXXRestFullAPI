//
// Created by olegaksenenko on 10/5/21.
//

#include "connection.h"

connection::connection(const std::string& hostname, const std::string& username, const std::string& password) :
driver(sql::mysql::get_mysql_driver_instance()),
con(driver->connect(hostname, username, password))
{}

std::shared_ptr<sql::Connection> connection::getConnection() {
    return con;
}
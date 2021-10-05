//
// Created by olegaksenenko on 10/2/21.
//

#include "userDataBase.h"

userDataBase::userDataBase() {
    driver = sql::mysql::get_mysql_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "root", "H1l6KWq8N1");
    res = nullptr;

    stmt = con->createStatement();
    stmt->execute("USE mysql");
    stmt->execute("DROP TABLE IF EXISTS todoUser");
    stmt->execute("CREATE TABLE todoUser(id INT PRIMARY KEY AUTO_INCREMENT,"
                  " username VARCHAR(30), password VARCHAR(30), task_id INT);");
    stmt->execute("INSERT INTO todoUser(username, password, task_id)"
                  " VALUES ('admin', 'admin', 1)");
    stmt->execute("INSERT INTO todoUser(username, password, task_id)"
                  " VALUES ('smarty', 'smarty', 2)");
}

void userDataBase::createUser(const std::string& username, const std::string& password, int task_id) {
    stmt->execute("INSERT INTO todoUser(username, password, task_id)"
                  " VALUES ('"
                  + username +
                  "', '"
                  + password +
                  "', '"
                  + std::to_string(task_id) +
                  "')");
}

sql::ResultSet* userDataBase::getUserTaskId(const std::string& username, const std::string& password) {
    return stmt->executeQuery("SELECT task_id FROM todoUser WHERE username = '"
                              + username +
                              "' AND password = '"
                              + password +
                              "'");
}
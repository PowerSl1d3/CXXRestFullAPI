//
// Created by olegaksenenko on 10/2/21.
//

#include "taskDataBase.h"

taskDataBase::taskDataBase() {
    driver = sql::mysql::get_mysql_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "root", "H1l6KWq8N1");

    stmt = con->createStatement();
    stmt->execute("USE mysql");
    stmt->execute("DROP TABLE IF EXISTS todoTask");
    stmt->execute("CREATE TABLE todoTask(id INT PRIMARY KEY AUTO_INCREMENT,"
                  " user_task_id INT, text VARCHAR(255));");
    stmt->execute("INSERT INTO todoTask(user_task_id, text)"
                  " VALUES (1, 'create my CXX RestFullApi')");
    stmt->execute("INSERT INTO todoTask(user_task_id, text)"
                  " VALUES (1, 'tell Roma what I did')");
    stmt->execute("INSERT INTO todoTask(user_task_id, text)"
                  " VALUES (2, 'Do Androids Dream of Electric Sheep?')");
}

void taskDataBase::createTask(const int userTaskId, const std::string& text) {
    stmt->execute("INSERT INTO todoTask(user_task_id, text) VALUES ('"
                  + std::to_string(userTaskId) +
                  "', '"
                  + text +
                  +"')");
}

sql::ResultSet* taskDataBase::getToDo(const int userTaskId) {
    return stmt->executeQuery("SELECT id, text FROM todoTask WHERE user_task_id = " + std::to_string(userTaskId));
}

void taskDataBase::deleteToDo(const int userTaskId, const int todoId) {
    stmt->execute("DELETE FROM todoTask WHERE id = " + std::to_string(todoId)
    + " AND user_task_id = " + std::to_string(userTaskId));
}

void taskDataBase::updateToDo(const int todoId, const int userTaskId, const std::string &text) {
    stmt->execute("UPDATE todoTask SET text = '"
    + text +
    "' WHERE id = " + std::to_string(todoId) + " AND user_task_id = " + std::to_string(userTaskId));
}

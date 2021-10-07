//
// Created by olegaksenenko on 10/2/21.
//

#include "taskDataBase.h"

taskDataBase::taskDataBase(std::shared_ptr<sql::Statement> stmt_):
stmt(stmt_)
{
    stmt->execute("USE mysql");
    stmt->execute("DROP TABLE IF EXISTS todoTask");
    stmt->execute("CREATE TABLE todoTask(id INT PRIMARY KEY AUTO_INCREMENT,"
                  " user_id INT, text VARCHAR(256));");
}

void taskDataBase::createTask(const int userId, const std::string& text) {
    stmt->execute("INSERT INTO todoTask(user_id, text) VALUES ('"
                  + std::to_string(userId) +
                  "', '"
                  + text +
                  +"')");
}

std::unique_ptr<sql::ResultSet> taskDataBase::getToDo(const int userId) {
    return std::unique_ptr<sql::ResultSet>(
            stmt->executeQuery("SELECT id, text FROM todoTask WHERE user_id = "
            + std::to_string(userId))
    );
}

void taskDataBase::deleteToDo(const int userTaskId, const int todoId) {
    stmt->execute("DELETE FROM todoTask WHERE id = " + std::to_string(todoId)
    + " AND user_id = " + std::to_string(userTaskId));
}

void taskDataBase::updateToDo(const int todoId, const int userTaskId, const std::string &text) {
    stmt->execute("UPDATE todoTask SET text = '"
    + text +
    "' WHERE id = " + std::to_string(todoId) + " AND user_id = " + std::to_string(userTaskId));
}

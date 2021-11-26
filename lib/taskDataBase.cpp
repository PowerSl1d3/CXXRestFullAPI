//
// Created by olegaksenenko on 10/2/21.
//

#include "taskDataBase.h"

taskDataBase::taskDataBase(std::shared_ptr<sql::Connection> con):
        con_(con),
        stmt_(con->createStatement())
{
    stmt_->execute("USE mysql");
    stmt_->execute("DROP TABLE IF EXISTS todoTask");
    stmt_->execute("CREATE TABLE todoTask(id INT PRIMARY KEY AUTO_INCREMENT,"
                  " user_id INT, text VARCHAR(256));");
}

void taskDataBase::createTask(const int userId, const std::string& text) {
    const std::string statement = "INSERT INTO todoTask(user_id, text) VALUES (?, ?)";
    preparedStatement_.reset(con_->prepareStatement(statement));
    preparedStatement_->setInt(1, userId);
    preparedStatement_->setString(2, text);
    preparedStatement_->execute();
}

std::unique_ptr<sql::ResultSet> taskDataBase::getToDo(const int userId) {
    const std::string statement = "SELECT id, text FROM todoTask WHERE user_id = ?";
    preparedStatement_.reset(con_->prepareStatement(statement));
    preparedStatement_->setInt(1, userId);
    return std::unique_ptr<sql::ResultSet>(preparedStatement_->executeQuery());
}

void taskDataBase::deleteToDo(const int userTaskId, const int todoId) {
    const std::string statement = "DELETE FROM todoTask WHERE id = ? AND user_id = ?";
    preparedStatement_.reset(con_->prepareStatement(statement));
    preparedStatement_->setInt(1, todoId);
    preparedStatement_->setInt(2, userTaskId);
    preparedStatement_->execute();
}

void taskDataBase::updateToDo(const int todoId, const int userTaskId, const std::string &text) {
    const std::string statement = "UPDATE todoTask SET text = ? WHERE id = ? AND user_id = ?";
    preparedStatement_.reset(con_->prepareStatement(statement));
    preparedStatement_->setString(1, text);
    preparedStatement_->setInt(2, todoId);
    preparedStatement_->setInt(3, userTaskId);
    preparedStatement_->execute();
}

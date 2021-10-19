//
// Created by olegaksenenko on 10/9/21.
//

#include "fileDataBase.h"

fileDataBase::fileDataBase(std::shared_ptr<sql::Connection> con):
        con_(con),
        stmt_(con->createStatement())
{
    stmt_->execute("USE mysql");
    stmt_->execute("DROP TABLE IF EXISTS todoFile");
    stmt_->execute("CREATE TABLE todoFile(id INT PRIMARY KEY AUTO_INCREMENT, "
                  "user_id INT, filename VARCHAR(256), type VARCHAR(256));");
}

void fileDataBase::createFile(const int userId,
                              const std::string &filename,
                              const std::string& file_type) {
    const std::string statement = "INSERT INTO todoFile(user_id, filename, type) VALUES (?, ?, ?)";
    preparedStatement_.reset(con_->prepareStatement(statement));
    preparedStatement_->setInt(1, userId);
    preparedStatement_->setString(2, filename);
    preparedStatement_->setString(3, file_type);
    preparedStatement_->execute();
}

std::unique_ptr<sql::ResultSet> fileDataBase::getFiles(const int userId) {
    const std::string statement = "SELECT id, filename, type FROM todoFile WHERE user_id = ?";
    preparedStatement_.reset(con_->prepareStatement(statement));
    preparedStatement_->setInt(1, userId);
    return std::unique_ptr<sql::ResultSet>(preparedStatement_->executeQuery());
}

std::unique_ptr<sql::ResultSet> fileDataBase::getFile(const int userId,
                                                      const std::string& filename,
                                                      const std::string& file_type) {
    const std::string statement = "SELECT id, filename, type FROM todoFile WHERE user_id = ? AND filename = ? AND type = ?";
    preparedStatement_.reset(con_->prepareStatement(statement));
    preparedStatement_->setInt(1, userId);
    preparedStatement_->setString(2, filename);
    preparedStatement_->setString(3, file_type);
    return std::unique_ptr<sql::ResultSet>(preparedStatement_->executeQuery());
}

void fileDataBase::deleteFile(const int userTaskId,
                              const std::string &filename,
                              const std::string& file_type) {
    const std::string statement = "DELETE FROM todoFile WHERE user_id = ? AND filename = ? AND type = ?";
    preparedStatement_.reset(con_->prepareStatement(statement));
    preparedStatement_->setInt(1, userTaskId);
    preparedStatement_->setString(2, filename);
    preparedStatement_->setString(3, file_type);
    preparedStatement_->execute();
}

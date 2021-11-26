//
// Created by olegaksenenko on 10/9/21.
//

#ifndef CXXRESTFULLAPI_FILEDATABASE_H
#define CXXRESTFULLAPI_FILEDATABASE_H

#include <string>
#include <memory>

#include <mysql_connection.h>
#include <mysql_driver.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

class fileDataBase {
private:
    const std::shared_ptr<sql::Connection> con_;
    const std::unique_ptr<sql::Statement> stmt_;
    std::unique_ptr<sql::PreparedStatement> preparedStatement_;
public:
    fileDataBase(std::shared_ptr<sql::Connection>);
    void createFile(const int userId, const std::string& filename, const std::string& file_type);
    std::unique_ptr<sql::ResultSet> getFiles(const int userId);
    //TODO: add MIME types of files
    std::unique_ptr<sql::ResultSet> getFile(const int userId,
                                            const std::string& filename,
                                            const std::string& file_type);
    void deleteFile(const int userTaskId,
                    const std::string& filename,
                    const std::string& file_type);
};
/*
 * CREATE TABLE todoFile(id INT PRIMARY KEY AUTO_INCREMENT, user_id INT, name VARCHAR(256));
 */
#endif //CXXRESTFULLAPI_FILEDATABASE_H

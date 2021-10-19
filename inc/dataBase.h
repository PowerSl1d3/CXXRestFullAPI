//
// Created by olegaksenenko on 10/2/21.
//

#ifndef CXXRESTFULLAPI_DATABASE_H
#define CXXRESTFULLAPI_DATABASE_H

#include <random>
#include <functional>
#include <string>
#include <vector>
#include <utility>
#include <memory>
#include <unordered_set>
#include <filesystem>
#include <tuple>

#include "connection.h"
#include "userDataBase.h"
#include "taskDataBase.h"
#include "fileDataBase.h"

class dataBase {
private:
    std::unique_ptr<connection> pSQLCon;
    std::unique_ptr<userDataBase> uDB;
    std::unique_ptr<taskDataBase> tDB;
    std::unique_ptr<fileDataBase> fDB;
    const std::string filesRoot;
public:
    dataBase(const std::string& hostname,
             const std::string& username,
             const std::string& password,
             const std::string& filesRoot_);
    /**
     *
     * @param username
     * @param password
     * @return token - string that identify unique user
     */
    void createUser(const std::string& username, const std::string& password);
    void checkToken(const std::string& username, const std::string& password);
    std::vector<std::pair<int, std::string>> getToDo(const std::string& username, const std::string& password);
    void postToDo(const std::string& username, const std::string& password, const std::string& text);
    void deleteToDo(const std::string& username, const std::string& password, const int todoId);
    void updateTodo(const std::string& username, const std::string& password, const int todoId, const std::string& text);
    void postFile(const std::string& username,
                  const std::string& password,
                  const std::string& filename,
                  const std::string& file_type);
    std::vector<std::tuple<int, std::string, std::string>> getFiles(const std::string& username,const std::string& password);
    std::string getFile(const std::string& username,
                        const std::string& password,
                        const std::string& filename,
                        const std::string& file_type);
    void deleteFile(const std::string& username,
                    const std::string& password,
                    const std::string& filename,
                    const std::string& file_type);
};

#endif //CXXRESTFULLAPI_DATABASE_H

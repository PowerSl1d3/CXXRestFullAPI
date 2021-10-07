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

#include "connection.h"
#include "userDataBase.h"
#include "taskDataBase.h"

class dataBase {
private:
    std::unique_ptr<connection> pSQLCon;
    std::unique_ptr<userDataBase> uDB;
    std::unique_ptr<taskDataBase> tDB;
public:
    dataBase(const std::string& username, const std::string& password);
    /**
     *
     * @param username
     * @param password
     * @return token - string that identify unique user
     */
     //TODO: delete check if user delete not his todo
     //TODO: UPDATE TOOO
    void createUser(const std::string& username, const std::string& password);
    void checkToken(const std::string& username, const std::string& password);
    std::vector<std::pair<int, std::string>> getToDo(const std::string& username, const std::string& password);
    void postToDo(const std::string& username, const std::string& password, const std::string& text);
    void deleteToDo(const std::string& username, const std::string& password, const int todoId);
    void updateTodo(const std::string& username, const std::string& password, const int todoId, const std::string& text);
};

#endif //CXXRESTFULLAPI_DATABASE_H

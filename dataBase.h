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

#include "userDataBase.h"
#include "taskDataBase.h"

class dataBase {
private:
    userDataBase uDB;
    taskDataBase tDB;
    //Section for generating user_task_id
    std::random_device device;
    std::mt19937 generator;
    std::uniform_int_distribution<int> distribution;
    std::function<int(void)> getRandomNumber;
    //Section for generating user_task_id
    std::string tokenize(const std::string& username, const std::string& password) const;
    std::pair<std::string, std::string> deTokenize(const std::string& token) const;
public:
    dataBase();
    /**
     *
     * @param username
     * @param password
     * @return token - string that identify unique user
     */
     //TODO: response only token when user are exist
    std::string createUser(const std::string& username, const std::string& password);
    std::vector<std::pair<int, std::string>> getToDo(const std::string& token);
    void postToDo(const std::string& token, const std::string& text);
    void deleteToDo(const std::string& token, const int todoId);
    void updateTodo(const int todoId, const std::string& token, const std::string& text);
};

#endif //CXXRESTFULLAPI_DATABASE_H

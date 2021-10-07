//
// Created by olegaksenenko on 10/2/21.
//

#include "dataBase.h"

dataBase::dataBase(const std::string& username, const std::string& password):
pSQLCon(std::make_unique<connection>(username, password)),
uDB(std::make_unique<userDataBase>(pSQLCon->getStatement())),
tDB(std::make_unique<taskDataBase>(pSQLCon->getStatement()))
{}

void dataBase::createUser(const std::string& username, const std::string& password) {
    if (!uDB->checkUserAlreadyExist(username, password)) {
        uDB->createUser(username, password);
    }
}

void dataBase::checkToken(const std::string &username, const std::string &password) {
    if (!uDB->checkUserAlreadyExist(username, password)) {
        throw std::runtime_error("Incorrect token.");
    }
}

std::vector<std::pair<int, std::string>> dataBase::getToDo(const std::string& username, const std::string& password) {
    std::unique_ptr<sql::ResultSet> taskIdQuery = uDB->getUserId(username, password);
    taskIdQuery->next();
    int taskId = taskIdQuery->getInt("id");
    std::unique_ptr<sql::ResultSet> taskListQuery = tDB->getToDo(taskId);
    if (!taskListQuery->rowsCount()) {
        return {};
    }
    std::vector<std::pair<int, std::string>> taskList;
    taskList.reserve(taskListQuery->rowsCount());
    while (taskListQuery->next()) {
        taskList.emplace_back(taskListQuery->getInt("id"), taskListQuery->getString("text"));
    }

    return taskList;
}

void dataBase::postToDo(const std::string& username, const std::string& password, const std::string& text) {
    std::unique_ptr<sql::ResultSet> taskIdQuery = uDB->getUserId(username, password);
    taskIdQuery->next();
    int taskId = taskIdQuery->getInt("id");

    tDB->createTask(taskId, text);
}

void dataBase::deleteToDo(const std::string& username, const std::string& password, const int todoId) {
    std::unique_ptr<sql::ResultSet> taskIdQuery = uDB->getUserId(username, password);
    taskIdQuery->next();
    int taskId = taskIdQuery->getInt("id");
    std::unique_ptr<sql::ResultSet> taskListQuery = tDB->getToDo(taskId);
    if (!taskListQuery->rowsCount()) {
        throw std::runtime_error("User haven't tasks.");
    }
    std::unordered_set<int> uniqueSetOfId;
    while (taskListQuery->next()) {
        uniqueSetOfId.insert(taskListQuery->getInt("id"));
    }
    if (!uniqueSetOfId.count(todoId)) {
        throw std::runtime_error("User haven't tasks with this id.");
    }
    tDB->deleteToDo(taskId, todoId);
}

void dataBase::updateTodo(const std::string& username, const std::string& password, const int todoId, const std::string& text) {
    std::unique_ptr<sql::ResultSet> taskIdQuery = uDB->getUserId(username, password);
    taskIdQuery->next();
    int taskId = taskIdQuery->getInt("id");
    std::unique_ptr<sql::ResultSet> taskListQuery = tDB->getToDo(taskId);
    if (!taskListQuery->rowsCount()) {
        throw std::runtime_error("User haven't tasks.");
    }
    std::unordered_set<int> uniqueSetOfId;
    while (taskListQuery->next()) {
        uniqueSetOfId.insert(taskListQuery->getInt("id"));
    }
    if (!uniqueSetOfId.count(todoId)) {
        throw std::runtime_error("User haven't tasks with this id.");
    }
    tDB->updateToDo(todoId, taskId, text);
}

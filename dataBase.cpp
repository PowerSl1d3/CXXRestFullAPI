//
// Created by olegaksenenko on 10/2/21.
//

#include "dataBase.h"

dataBase::dataBase() {
    generator = std::mt19937(device());
    distribution = std::uniform_int_distribution<int>(1, 1'000'000);
    getRandomNumber = std::bind(this->distribution, this->generator);
}

std::string dataBase::tokenize(const std::string& username, const std::string& password) const {
    std::string token = username + '_' + password;
    for (char& ch : token) {
        ++ch;
    }
    return token;
}

std::pair<std::string, std::string> dataBase::deTokenize(const std::string &token) const {
    auto deToken = token;
    for (char& ch : deToken) {
        --ch;
    }
    size_t separator_pos = deToken.find('_');
    std::string::iterator it = deToken.begin() +
            (separator_pos == std::basic_string<char>::npos ?
            throw std::runtime_error("Incorrect token.") : separator_pos);
    return {{deToken.begin(), it}, {(it + 1), deToken.end()}};
}

std::string dataBase::createUser(const std::string& username, const std::string& password) {
    int user_task_id = getRandomNumber();
    uDB.createUser(username, password, user_task_id);
    return tokenize(username, password);
}

std::vector<std::pair<int, std::string>> dataBase::getToDo(const std::string& token) {
    const auto& [username, password] = deTokenize(token);
    sql::ResultSet* taskIdQuery = uDB.getUserTaskId(username, password);
    if (!taskIdQuery->rowsCount()) {
        throw std::runtime_error("User does not exist.");
    }
    int taskId;
    while (taskIdQuery->next()) {
        taskId = taskIdQuery->getInt("task_id");
    }
    sql::ResultSet* taskListQuery = tDB.getToDo(taskId);
    std::vector<std::pair<int, std::string>> taskList;
    taskList.reserve(taskListQuery->rowsCount());
    while (taskListQuery->next()) {
        taskList.emplace_back(taskListQuery->getInt("id"), taskListQuery->getString("text"));
    }
    return taskList;
}

void dataBase::postToDo(const std::string& token, const std::string& text) {
    const auto& [username, password] = deTokenize(token);
    sql::ResultSet* taskIdQuery = uDB.getUserTaskId(username, password);
    int taskId;
    while (taskIdQuery->next()) {
        taskId = taskIdQuery->getInt("task_id");
    }
    tDB.createTask(taskId, text);
}

void dataBase::deleteToDo(const std::string& token, const int todoId) {
    const auto& [username, password] = deTokenize(token);
    sql::ResultSet* taskIdQuery = uDB.getUserTaskId(username, password);
    int userTaskId;
    while (taskIdQuery->next()) {
        userTaskId = taskIdQuery->getInt("task_id");
    }
    tDB.deleteToDo(userTaskId, todoId);
}

void dataBase::updateTodo(const int todoId, const std::string& token, const std::string& text) {
    const auto& [username, password] = deTokenize(token);
    sql::ResultSet* taskIdQuery = uDB.getUserTaskId(username, password);
    int userTaskId;
    while (taskIdQuery->next()) {
        userTaskId = taskIdQuery->getInt("task_id");
    }
    tDB.updateToDo(todoId, userTaskId, text);
}

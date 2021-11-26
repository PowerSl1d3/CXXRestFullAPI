//
// Created by olegaksenenko on 10/2/21.
//

#include "dataBase.h"

dataBase::dataBase(const std::string& hostname,
                   const std::string& username,
                   const std::string& password,
                   const std::string& filesRoot_):
pSQLCon(std::make_unique<connection>(hostname, username, password)),
uDB(std::make_unique<userDataBase>(pSQLCon->getConnection())),
tDB(std::make_unique<taskDataBase>(pSQLCon->getConnection())),
fDB(std::make_unique<fileDataBase>(pSQLCon->getConnection())),
filesRoot(filesRoot_)
{
    std::filesystem::remove_all(filesRoot);
    std::filesystem::create_directory(filesRoot);
}

void dataBase::createUser(const std::string& username, const std::string& password) {
    if (!uDB->checkUserAlreadyExist(username, password)) {
        uDB->createUser(username, password);
    }
    std::filesystem::create_directory(std::filesystem::path(filesRoot)/username);
}

void dataBase::checkToken(const std::string &username, const std::string &password) {
    if (!uDB->checkUserAlreadyExist(username, password)) {
        throw std::runtime_error("Incorrect token.");
    }
}

std::vector<std::pair<int, std::string>> dataBase::getToDo(const std::string& username,
                                                           const std::string& password) {
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

void dataBase::postToDo(const std::string& username,
                        const std::string& password,
                        const std::string& text) {
    std::unique_ptr<sql::ResultSet> taskIdQuery = uDB->getUserId(username, password);
    taskIdQuery->next();
    int taskId = taskIdQuery->getInt("id");

    tDB->createTask(taskId, text);
}

void dataBase::deleteToDo(const std::string& username,
                          const std::string& password,
                          const int todoId) {
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

void dataBase::updateTodo(const std::string& username,
                          const std::string& password,
                          const int todoId,
                          const std::string& text) {
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

void dataBase::postFile(const std::string& username,
                        const std::string& password,
                        const std::string& filename,
                        const std::string& file_type) {
    std::unique_ptr<sql::ResultSet> fileIdQuery = uDB->getUserId(username, password);
    fileIdQuery->next();
    int fileId = fileIdQuery->getInt("id");
    fDB->createFile(fileId, filename, file_type);
}

std::vector<std::tuple<int, std::string, std::string>> dataBase::getFiles(const std::string &username,
                                                            const std::string &password) {
    std::unique_ptr<sql::ResultSet> fileIdQuery = uDB->getUserId(username, password);
    fileIdQuery->next();
    int fileId = fileIdQuery->getInt("id");
    std::unique_ptr<sql::ResultSet> fileListQuery = fDB->getFiles(fileId);
    if (!fileListQuery->rowsCount()) {
        throw std::runtime_error("User haven't files.");
    }
    std::vector<std::tuple<int, std::string, std::string>> fileList;
    fileList.reserve(fileListQuery->rowsCount());
    while (fileListQuery->next()) {
        fileList.emplace_back(fileListQuery->getInt("id"),
                              fileListQuery->getString("filename"),
                              fileListQuery->getString("type"));
    }

    return fileList;
}

std::string dataBase::getFile(const std::string &username,
                              const std::string &password,
                              const std::string &filename,
                              const std::string& file_type) {
    std::unique_ptr<sql::ResultSet> fileIdQuery = uDB->getUserId(username, password);
    fileIdQuery->next();
    int fileId = fileIdQuery->getInt("id");
    std::unique_ptr<sql::ResultSet> fileListQuery = fDB->getFile(fileId, filename, file_type);
    fileListQuery->next();
    if (!fileListQuery->rowsCount()) {
        throw std::runtime_error("User haven't files.");
    }
    return fileListQuery->getString("filename");
}

void dataBase::deleteFile(const std::string &username,
                          const std::string &password,
                          const std::string &filename,
                          const std::string& file_type) {
    std::unique_ptr<sql::ResultSet> fileIdQuery = uDB->getUserId(username, password);
    fileIdQuery->next();
    int fileId = fileIdQuery->getInt("id");
    fDB->deleteFile(fileId, filename, file_type);
}

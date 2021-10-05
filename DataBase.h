//
// Created by olegaksenenko on 10/2/21.
//

#ifndef CXXRESTFULLAPI_DATABASE_H
#define CXXRESTFULLAPI_DATABASE_H

#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <utility>

class DataBase {
private:
    typedef std::tuple<size_t, std::string, std::string, size_t> row_t;
    typedef std::vector<row_t> UserDataBase;
    std::map<std::string, std::string> username2password;
    std::map<std::string, std::vector<std::string>> todoId2todolist;
    void loadDB() {}
    std::string encrypt(const std::string&) const;
    std::string decrypt(const std::string&) const;
public:
    DataBase();
    /**
     *
     * @param username
     * @param password
     * @return token
     */
    std::string createUser(const std::string& username, const std::string& password);
    void saveDB() const;
    ~DataBase();
};

DataBase::DataBase() {
    std::ifstream ifs("DataBase.scv");
    std::string row;
    while (ifs >> row) {
        size_t pos = row.find(',');
        username2password[std::string(row, pos + 1)] = std::string(row, pos + 1, row.size());
    }
}

std::string DataBase::encrypt(const std::string& raw) const {
    std::string en;
    for (size_t i = 0; i < raw.size(); ++i) {
        en.push_back(raw[i] + 1);
    }
    return en;
}

std::string DataBase::decrypt(const std::string& encrypted) const {
    std::string dec;
    for (size_t i = 0; i < encrypted.size(); ++i) {
        dec.push_back(encrypted[i] - 1);
    }
    return dec;
}

std::string DataBase::createUser(const std::string &username, const std::string &password) {
    username2password[username] = password;
    return encrypt(username + "_" + password);
}

void DataBase::saveDB() const {
    std::ofstream ofs("DataBase.scv");
    for (const auto& [username, password] : username2password) {
        ofs << username << "," << password << "\n";
    }
}

DataBase::~DataBase() {
    saveDB();
}

#endif //CXXRESTFULLAPI_DATABASE_H

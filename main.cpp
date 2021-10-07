#include <iostream>
#include <sstream>

#include <boost/program_options.hpp>

#include "userDataBase.h"
#include "taskDataBase.h"
#include "dataBase.h"
#include "sha256.h"
#include "cryptographer.h"

#define CROW_MAIN
#include <crow_all.h>

#define TEST_DATABASE

std::pair<std::string, std::string> parseDecryptedToken(const std::string& token) {
    size_t separator_pos = token.find('_');
    std::string::const_iterator it = token.cbegin() +
                               (separator_pos == std::basic_string<char>::npos ?
                                throw std::runtime_error("Incorrect token.") : separator_pos);
    return {{token.begin(), it}, {(it + 1), token.end()}};
}

int main(int argc, char* argv[]) {

    namespace opt = boost::program_options;

    opt::options_description desc("All options");

    desc.add_options()
            ("username,u", opt::value<std::string>()->required()->default_value("root"), "Username for sql server")
            ("password,p", opt::value<std::string>()->required(), "Password for sql server")
            ("help,h", "Produce help message");

    opt::variables_map vm;

    try {
        opt::store(opt::parse_config_file<char>("sql.cfg", desc), vm
        );
    } catch (const opt::reading_file& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    try {
        opt::notify(vm);
    } catch (const opt::required_option& e) {

        std::cerr << e.what() << std::endl;
        return 2;

    }

    if (vm.count("help")) {

        std::cout << desc << std::endl;
        return 1;

    }

    crow::SimpleApp app; //define your crow application
    dataBase db(vm["username"].as<std::string>(), vm["password"].as<std::string>());
#ifdef TEST_DATABASE
#define TEST_DATABASE
    db.createUser("admin", sha256::compute("admin"));
    std::string admin_password = sha256::compute("admin");
    db.createUser("smarty", sha256::compute("smarty"));
    std::string smarty_password = sha256::compute("smarty");
    db.postToDo("admin", admin_password, "create my CXX RestFullApi");
    db.postToDo("admin", admin_password, "tell Roma what I did");
    db.postToDo("smarty", smarty_password, "Do Androids Dream of Electric Sheep?");
#endif

    //define your endpoint at the root directory
    CROW_ROUTE(app, "/")([](){
        return "Hello world!";
    });

    CROW_ROUTE(app, "/user").methods(crow::HTTPMethod::POST)([&db](const crow::request& req){
        auto x = crow::json::load(req.body);

        if (!x) {
            crow::response(404);
        }

        crow::json::wvalue token;
        db.createUser(x["username"].s(), sha256::compute(x["password"].s()));
        token["token"] = cryptographer::encrypt(std::string(x["username"].s()) +
                "_" + sha256::compute(x["password"].s()));

        return crow::response(token);
    });

    CROW_ROUTE(app, "/todo").methods(crow::HTTPMethod::GET)([&db](const crow::request& req){
        auto x = crow::json::load(req.body);

        if (!x) {
            crow::response(404);
        }

        std::vector<std::pair<int, std::string>> todoList;

        try {
            const auto& [username, password] = parseDecryptedToken(
                    cryptographer::decrypt(x["token"].s())
                    );
            db.checkToken(username, password);
            todoList = db.getToDo(username, password);
        } catch (const std::runtime_error& e) {
            return crow::response(404, e.what());
        }

        std::ostringstream oss;

        for (const auto& [id, text] : todoList) {
            oss << id << ' ' << text << "\n";
        }

        return crow::response(oss.str());
    });

    CROW_ROUTE(app, "/todo").methods(crow::HTTPMethod::POST)([&db](const crow::request& req){
        auto x = crow::json::load(req.body);

        if (!x) {
            crow::response(404);
        }

        try {
            const auto& [username, password] = parseDecryptedToken(
                    cryptographer::decrypt(x["token"].s())
            );
            db.checkToken(username, password);
            db.postToDo(username, password, x["text"].s());
        } catch (const std::runtime_error& e) {
            return crow::response(404, e.what());
        }

        return crow::response("Task successfully created!");
    });

    CROW_ROUTE(app, "/todo/<int>").methods(crow::HTTPMethod::PUT)([&db](const crow::request& req, int id){
        auto x = crow::json::load(req.body);

        if (!x) {
            crow::response(404);
        }

        try {
            const auto& [username, password] = parseDecryptedToken(
                    cryptographer::decrypt(x["token"].s())
            );
            db.checkToken(username, password);
            db.updateTodo(username, password, id, x["text"].s());
        } catch (const std::runtime_error& e) {
            return crow::response(404, e.what());
        }

        return crow::response("Task updated!");
    });

    CROW_ROUTE(app, "/todo/<int>").methods(crow::HTTPMethod::DELETE)([&db](const crow::request& req, int id){

        auto x = crow::json::load(req.body);

        if (!x) {
            crow::response(404);
        }

        try {
            const auto& [username, password] = parseDecryptedToken(
                    cryptographer::decrypt(x["token"].s())
            );
            db.checkToken(username, password);
            db.deleteToDo(username, password, id);
        } catch (const std::runtime_error& e) {
            return crow::response(404, e.what());
        }

        return crow::response("Task deleted!");
    });

    app.bindaddr("127.0.0.1")
    .port(8000)
    .run();

    return 0;
}

#include <iostream>
#include <sstream>

//#include "DataBase.h"
#include "userDataBase.h"
#include "taskDataBase.h"
#include "dataBase.h"

#define CROW_MAIN
#include <crow_all.h>

int main() {

    crow::SimpleApp app; //define your crow application
    dataBase db;

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
        token["token"] = db.createUser(x["username"].s(), x["password"].s());

        return crow::response(token);
    });

    CROW_ROUTE(app, "/todo").methods(crow::HTTPMethod::GET)([&db](const crow::request& req){
        auto x = crow::json::load(req.body);

        if (!x) {
            crow::response(404);
        }

        std::vector<std::pair<int, std::string>> todoList;

        try {
            todoList = db.getToDo(x["token"].s());
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

        db.postToDo(x["token"].s(), x["text"].s());

        return "Task successfully created!";
    });

    CROW_ROUTE(app, "/todo/<int>").methods(crow::HTTPMethod::PUT)([&db](const crow::request& req, int id){
        auto x = crow::json::load(req.body);

        if (!x) {
            crow::response(404);
        }

        db.updateTodo(id, x["token"].s(), x["text"].s());

        return "Task updated!";
    });

    CROW_ROUTE(app, "/todo/<int>").methods(crow::HTTPMethod::DELETE)([&db](const crow::request& req, int id){

        auto x = crow::json::load(req.body);

        if (!x) {
            crow::response(404);
        }

        db.deleteToDo(x["token"].s(), id);

        return "Task deleted!";
    });

    //set the port, set the app to run on multiple threads, and run the app
    app.bindaddr("127.0.0.1")
    .port(8000)
    .run();

    //auto x = db.getToDo("admin", "admin");
    //db.postToDo("smarty", "smarty", "Hello world!");
    //db.updateTodo(4, "new todo");

    return 0;
}

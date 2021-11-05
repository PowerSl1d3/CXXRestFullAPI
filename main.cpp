#include <iostream>
#include <sstream>
#include <filesystem>

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
    if (separator_pos == std::basic_string<char>::npos) {
        throw std::runtime_error("Incorrect token.");
    }
    std::string::const_iterator it = token.cbegin() +  separator_pos;
    return {{token.begin(), it}, {(it + 1), token.end()}};
}

int main(int argc, char* argv[]) {
    namespace opt = boost::program_options;

    opt::options_description desc("All options");

    desc.add_options()
            ("hostname,h", opt::value<std::string>()->required(), "MySQL hostname connection")
            ("username,u", opt::value<std::string>()->required()->default_value("root"), "Username for sql server")
            ("password,p", opt::value<std::string>()->required(), "Password for sql server")
            ("help,h", "Produce help message");

    opt::variables_map vm;

    try {
        opt::store(opt::parse_config_file<char>("sql.cfg", desc), vm);
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

    crow::SimpleApp app;
    dataBase db(
            vm["hostname"].as<std::string>(),
            vm["username"].as<std::string>(),
            vm["password"].as<std::string>(),
            "files");
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
            const auto& [username, password] = parseDecryptedToken(cryptographer::decrypt(x["token"].s()));
            db.checkToken(username, password);
            todoList = db.getToDo(username, password);
        } catch (const std::runtime_error& e) {
            return crow::response(404, e.what());
        }

        std::ostringstream oss;

        for (const auto& [id, text] : todoList) {
            oss << id << '#' << text << "\n";
        }

        return crow::response(oss.str());
    });

    CROW_ROUTE(app, "/todo").methods(crow::HTTPMethod::POST)([&db](const crow::request& req){
        auto x = crow::json::load(req.body);

        if (!x) {
            crow::response(404);
        }

        try {
            const auto& [username, password] = parseDecryptedToken(cryptographer::decrypt(x["token"].s()));
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
            const auto& [username, password] = parseDecryptedToken(cryptographer::decrypt(x["token"].s()));
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
            const auto& [username, password] = parseDecryptedToken(cryptographer::decrypt(x["token"].s()));
            db.checkToken(username, password);
            db.deleteToDo(username, password, id);
        } catch (const std::runtime_error& e) {
            return crow::response(404, e.what());
        }

        return crow::response("Task deleted!");
    });

    CROW_ROUTE(app, "/files").methods(crow::HTTPMethod::POST)([&db](const crow::request& req){
        const std::string token = req.headers.find("token")->second;
        const std::string filename = req.headers.find("filename")->second;
        const std::string file_type = req.headers.find("Content-Type")->second;

        if (token.empty()) {
            return crow::response(404);
        }

        const auto& [username, password] = parseDecryptedToken(cryptographer::decrypt(token));

        // Check file size: if is to big - terminate saving file
        if (req.body.size() > 10'485'760) {
            return crow::response(404, "File is too big");
        }

        auto file = crow::json::load(req.body);

        if (!file) {
            crow::response(404);
        }

        std::ofstream ofs("files/" + username + "/" + filename);
        ofs << file["content"].s();
        ofs.close();

        db.postFile(username, password, filename, file_type);

        return crow::response("File successful got!");
    });

    CROW_ROUTE(app, "/files").methods(crow::HTTPMethod::GET)([&db](const crow::request& req){
        // TODO(smarty): throw error if user haven't files
        const std::string token = req.headers.find("token")->second;

        if (token.empty()) {
            return crow::response(404);
        }

        const auto& [username, password] = parseDecryptedToken(cryptographer::decrypt(token));

        auto fileList = db.getFiles(username, password);
        std::ostringstream oss;

        for (const auto& [id, filename, file_extension] : fileList) {
            oss << id << '#' << filename << '#' << file_extension << '\n';
        }

        return crow::response(oss.str());
    });

    CROW_ROUTE(app, "/files/<string>").methods(crow::HTTPMethod::GET)([&db](const crow::request& req,
            const std::string& filename){
        const std::string token = req.headers.find("token")->second;
        const std::string file_type = req.headers.find("Content-Type")->second;

        if (token.empty()) {
            return crow::response(404);
        }

        const auto& [username, password] = parseDecryptedToken(cryptographer::decrypt(token));

        std::string file = db.getFile(username, password, filename, file_type);
        std::ifstream ifs("files/" + username + "/" + file);
        std::ostringstream buffer;
        buffer << ifs.rdbuf();

        return crow::response(buffer.str());
    });

    CROW_ROUTE(app, "/files/<string>").methods(crow::HTTPMethod::DELETE)([&db](const crow::request& req,
            const std::string& filename){
        const std::string token = req.headers.find("token")->second;
        const std::string file_type = req.headers.find("Content-Type")->second;

        if (token.empty()) {
            return crow::response(404);
        }

        const auto& [username, password] = parseDecryptedToken(cryptographer::decrypt(token));

        db.deleteFile(username, password, filename, file_type);
        std::filesystem::remove("files/" + username + "/" + filename);

        return crow::response("File successful deleted!");
    });

    app.bindaddr("127.0.0.1")
    .port(8000)
    .run();

    return 0;
}

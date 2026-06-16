#include "send.hpp"
#include "configure.hpp"

#include <iostream>
#include <iterator>
#include <string>
#include <string_view>
#include <unordered_map>
#include <functional>
#include <vector>

static constexpr std::string_view VERSION = "1.0.0";

struct Args {
    std::string command;
    std::vector<std::string> positional;
    std::unordered_map<std::string, std::string> flags;
};

typedef std::function<void(const Args&)> Handler;

Args parse_args(int argc, char* argv[]) {
    Args args;
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg.starts_with("--")) {
            std::string key = arg.substr(2);
            if (i + 1 < argc && argv[i + 1][0] != '-') {
                args.flags[key] = argv[++i];
            } else {
                args.flags[key] = "true";
            }
        } else if (args.command.empty()) {
            args.command = arg;
        } else {
            args.positional.push_back(arg);
        }
    }
    return args;
}

void cmd_hello_from_imo(const Args&) {
    Sender sender = get_hello_from_imo();

    json_object* obj = sender("http://localhost:8080/api/hello");

    if (obj) {
        std::cout << json_object_to_json_string_ext(
            obj,
            JSON_C_TO_STRING_PRETTY
        ) << std::endl;

        json_object_put(obj);
    } else {
        std::cout << "Server may be down, try again later" << std::endl;
    }
}

void cmd_register(const Args& args) {

    if(!is_config_dir_exist()) {
        create_config_dir();
        std::cout << "configuration directory created at " << get_config_dir() << ", you can register now" << std::endl;
        std::cout << "please don't edit configuration files manually" << std::endl;
        return;
    }else{
        std::string user_id = get_config_user_id();
        if(user_id != "") {
            std::cerr << "you already have an account, no need to register" << std::endl;
            return;
        }
    }

    auto it = args.flags.find("username");

    if (it == args.flags.end()) {
        std::cout << "username not found" << std::endl;
        return;
    }

    std::string username = it->second;

    Sender sender = register_new_user(username);

    json_object* obj = sender("http://localhost:8080/api/user/register");

    if (obj) {
        std::cout << json_object_to_json_string_ext(
            obj,
            JSON_C_TO_STRING_PRETTY
        ) << std::endl;

        json_object* data_obj;
        if (json_object_object_get_ex(obj, "data", &data_obj)) {

            json_object* id_obj;
            if (json_object_object_get_ex(data_obj, "id", &id_obj)) {

                const char* id = json_object_get_string(id_obj);

                create_config_json(id);
            }
        }

        json_object_put(obj);
    } else {
        std::cout << "Server may be down, try again later" << std::endl;
    }
}

void cmd_upload_file(const Args& args) {

    if(!is_config_dir_exist()) {
        create_config_dir();
        std::cout << "configuration directory created at " << get_config_dir() << ", you need to register first" << std::endl;
        std::cout << "please don't edit configuration files manually" << std::endl;
        return;
    }

    std::string user_id = get_config_user_id();

    if(user_id == "") {
        std::cout << "please register first\n" << std::endl;
        return;
    }

    auto it = args.flags.find("file");

    std::cout << "file path: " << it->second << std::endl;

    return;
}

void cmd_help(const Args&) {
    std::cout << "Usage: imo-tool <command> [options]\n\n"
              << "Commands:\n"
              << "  help     Show this help message\n"
              << "  version  Show version\n"
              << "  hello    Test send request to imo server"
              << "  upload   upload file to server"
              << "\nOptions:\n"
              << "  --<key> <value>   Pass a named flag\n";
}

void cmd_version(const Args&) {
    std::cout << "imo-cli " << VERSION << "\n";
}

int main(int argc, char* argv[]) {
    Args args = parse_args(argc, argv);

    std::unordered_map<std::string, Handler> commands = {
        {"help",    cmd_help},
        {"version", cmd_version},
        {"hello", cmd_hello_from_imo},
        {"register", cmd_register},
        {"upload", cmd_upload_file},
    };

    if (args.command.empty() || args.command == "help") {
        cmd_help(args);
        return 0;
    }

    auto it = commands.find(args.command);
    if (it == commands.end()) {
        std::cerr << "Unknown command: " << args.command << "\n";
        std::cerr << "Run 'imo-tool help' for usage.\n";
        return 1;
    }

    it->second(args);
    return 0;
}

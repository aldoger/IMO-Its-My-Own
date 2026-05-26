#include "send.h"

#include <iostream>
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

void cmd_hello(const Args&) {
    Sender sender = get_hello_world();

    json_object* obj = sender("https://jsonplaceholder.typicode.com/todos/1");

    if (obj) {
        std::cout << json_object_to_json_string_ext(
            obj,
            JSON_C_TO_STRING_PRETTY
        ) << std::endl;

        json_object_put(obj);
    }
}

void cmd_help(const Args&) {
    std::cout << "Usage: imo-tool <command> [options]\n\n"
              << "Commands:\n"
              << "  help     Show this help message\n"
              << "  version  Show version\n"
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
        {"hello", cmd_hello}
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

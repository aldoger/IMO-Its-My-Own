#include <iostream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <functional>
#include <vector>

static constexpr std::string_view VERSION = "0.1.0";

struct Args {
    std::string command;
    std::vector<std::string> positional;
    std::unordered_map<std::string, std::string> flags;
};

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

void cmd_help(const Args&) {
    std::cout << "Usage: imo-tool <command> [options]\n\n"
              << "Commands:\n"
              << "  help     Show this help message\n"
              << "  version  Show version\n"
              << "\nOptions:\n"
              << "  --<key> <value>   Pass a named flag\n";
}

void cmd_version(const Args&) {
    std::cout << "imo-tool " << VERSION << "\n";
}

int main(int argc, char* argv[]) {
    Args args = parse_args(argc, argv);

    using Handler = std::function<void(const Args&)>;
    std::unordered_map<std::string, Handler> commands = {
        {"help",    cmd_help},
        {"version", cmd_version},
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

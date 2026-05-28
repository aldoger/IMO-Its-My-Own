#include "configure.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <json-c/json.h>

namespace fs = std::filesystem;

bool is_config_dir_exist() {
    return fs::exists(get_config_dir());
}

void create_config_dir() {
    std::string dir = get_config_dir();
    try {
        fs::create_directories(dir);
        std::cout << "directory: " << dir << " created" << std::endl;
    }
    catch (const fs::filesystem_error& e) {
        std::cerr << e.what() << '\n';
    }
}

void create_config_json(const std::string& user_id) {
    std::string path = get_config_dir() + "/config.json";

    json_object* root = json_object_new_object();
    json_object_object_add(root, "user_id", json_object_new_string(user_id.c_str()));

    const char* json_str = json_object_to_json_string_ext(root, JSON_C_TO_STRING_PRETTY);

    std::ofstream file(path);
    if (!file) {
        std::cerr << "failed to open " << path << '\n';
        json_object_put(root);
        return;
    }

    file << json_str;
    json_object_put(root);
}

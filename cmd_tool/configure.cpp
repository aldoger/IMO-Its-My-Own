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

        std::string filePath = dir + "/config.json";
        std::ofstream configFile(filePath);
        if (!configFile.is_open()) {
            std::cerr << "Error: could not create config file at " << filePath << std::endl;
            return;
        }

        json_object* root = json_object_new_object();
        const char* json_str = json_object_to_json_string_ext(root, JSON_C_TO_STRING_PRETTY);
        configFile << json_str;
        configFile.close();
        json_object_put(root);

        std::cout << "Config file created at: " << filePath << std::endl;
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

std::string get_config_user_id() {
    if (!is_config_dir_exist()) {
        create_config_dir();

        std::cerr
            << "please register first before uploading your files"
            << std::endl;

        return "";
    }

    std::string path = get_config_dir() + "/config.json";

    json_object* root = json_object_from_file(path.c_str());

    if (root == nullptr) {
        std::cerr << "Failed to parse JSON\n";
        return "";
    }

    json_object* userIdObj;

    if (!json_object_object_get_ex(
            root,
            "user_id",
            &userIdObj
        )) {

        json_object_put(root);
        return "";
    }

    const char* user_id =
        json_object_get_string(userIdObj);

    std::string result =
        user_id ? user_id : "";

    json_object_put(root);

    return result;
}

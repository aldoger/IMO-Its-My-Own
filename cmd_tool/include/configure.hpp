#ifndef CONFIGURE_H
#define CONFIGURE_H

#include <cstdlib>
#include <string>

inline std::string get_config_dir() {

#ifdef _WIN32

    const char* appdata = std::getenv("APPDATA");

    return appdata
        ? std::string(appdata) + "\\imo"
        : "Config\\imo";

#else

    const char* home = std::getenv("HOME");

    return home
        ? std::string(home) + "/.config/imo"
        : ".config/imo";

#endif
}

bool is_config_dir_exist();
void create_config_dir();
void create_config_json(const std::string& user_id);
std::string get_config_user_id();

#endif

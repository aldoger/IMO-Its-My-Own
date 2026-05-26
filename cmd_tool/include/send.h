#ifndef SEND_H
#define SEND_H

#include <json-c/json.h>
#include <functional>
#include <string_view>

typedef std::function<json_object*(std::string_view path)> Sender;

Sender get_hello_world();

#endif
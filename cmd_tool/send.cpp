#include "send.hpp"

#include <curl/curl.h>
#include <string>

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

Sender register_new_user(std::string* username) {
    return [username](std::string_view path) -> json_object* {
        CURL* curl = curl_easy_init();

        if (!curl) {
            return nullptr;
        }

        struct curl_slist* headers = nullptr;

        headers = curl_slist_append(
            headers,
            "Content-Type: application/json"
        );

        json_object* root = json_object_new_object();

        json_object_object_add(
            root,
            "username",
            json_object_new_string(username->c_str())
        );

        const char* json_string =
            json_object_to_json_string(root);

        std::string response;
        std::string url(path);

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_string);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        CURLcode res = curl_easy_perform(curl);

        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);

        if (res != CURLE_OK) {
            json_object_put(root);
            return nullptr;
        }

        json_object* parsed = json_tokener_parse(response.c_str());

        return parsed;
    };
}

Sender get_hello_from_imo() {
    return [](std::string_view path) -> json_object* {
        CURL* curl = curl_easy_init();

        if (!curl) {
            return nullptr;
        }

        std::string response;
        std::string url = std::string(path);

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        CURLcode res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        if (res != CURLE_OK) {
            return nullptr;
        }

        return json_tokener_parse(response.c_str());
    };
}

Sender upload_my_file(std::string *filePath) {
    
}
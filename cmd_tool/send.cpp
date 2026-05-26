#include "send.h"

#include <curl/curl.h>
#include <string>

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

Sender get_hello_world() {
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
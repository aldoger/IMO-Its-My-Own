#include "send.hpp"
#include "configure.hpp"

#include <curl/curl.h>
#include <string>

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

Sender register_new_user(const std::string& username) {
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
            json_object_new_string(username.c_str())
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

Sender upload_my_file(const std::string& filePath, const std::string& fileName) {
    std::string user_id = get_config_user_id();

    if (user_id.empty()) {
        return nullptr;
    }

    return [filePath, user_id, fileName](std::string_view path) -> json_object* {
        CURL* curl = curl_easy_init();

        if (!curl) {
            return nullptr;
        }

        std::string response;
        std::string url(path);

        curl_mime* mime = curl_mime_init(curl);

        if (!mime) {
            curl_easy_cleanup(curl);
            return nullptr;
        }

        curl_mimepart* part =
            curl_mime_addpart(mime);

        curl_mime_name(part, "file");
        curl_mime_filedata(part, filePath.c_str());

        part = curl_mime_addpart(mime);

        curl_mime_name(part, "user_id");
        curl_mime_data(
            part,
            user_id.c_str(),
            CURL_ZERO_TERMINATED
        );

        curl_mime_name(part, "file_name");
        curl_mime_data(
            part,
            fileName.c_str(),
            CURL_ZERO_TERMINATED
        );

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_MIMEPOST, mime);

        curl_easy_setopt(
            curl,
            CURLOPT_WRITEFUNCTION,
            WriteCallback
        );

        curl_easy_setopt(
            curl,
            CURLOPT_WRITEDATA,
            &response
        );

        CURLcode res = curl_easy_perform(curl);

        json_object* result = nullptr;

        if (res == CURLE_OK) {
            result =
                json_tokener_parse(response.c_str());
        }

        curl_mime_free(mime);
        curl_easy_cleanup(curl);

        return result;
    };
}

Sender download_my_file(const std::string& filePath, const std::string& fileName) {
    std::string user_id = get_config_user_id();

    if (user_id.empty()) {
        return nullptr;
    }

    return [filePath, user_id, fileName](std::string_view path) -> json_object* {
        CURL* curl = curl_easy_init();

        if (!curl) {
            return nullptr;
        }

        std::string response;
        std::string url(path);

        json_object* body = json_object_new_object();
        json_object_object_add(body, "user_id", json_object_new_string(user_id.c_str()));
        json_object_object_add(body, "file_name", json_object_new_string(fileName.c_str()));
        const char* body_str = json_object_to_json_string(body);

        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, "Content-Type: application/json");

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body_str);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        CURLcode res = curl_easy_perform(curl);

        json_object* result = nullptr;

        if (res == CURLE_OK) {
            result = json_tokener_parse(response.c_str());
        }

        json_object_put(body);
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);

        return result;
    };
}

Sender delete_my_file(const std::string& fileName) {
    std::string user_id = get_config_user_id();

    if (user_id.empty()) {
        return nullptr;
    }

    return [user_id, fileName](std::string_view path) -> json_object* {
        CURL* curl = curl_easy_init();

        if (!curl) {
            return nullptr;
        }

        std::string response;
        std::string url(path);

        json_object* body = json_object_new_object();
        json_object_object_add(body, "user_id", json_object_new_string(user_id.c_str()));
        json_object_object_add(body, "file_name", json_object_new_string(fileName.c_str()));
        const char* body_str = json_object_to_json_string(body);

        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, "Content-Type: application/json");

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body_str);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        CURLcode res = curl_easy_perform(curl);

        json_object* result = nullptr;

        if (res == CURLE_OK) {
            result = json_tokener_parse(response.c_str());
        }

        json_object_put(body);
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);

        return result;
    };
}

Sender get_my_file_list() {
    std::string user_id = get_config_user_id();

    if (user_id.empty()) {
        return nullptr;
    }

    return [user_id](std::string_view path) -> json_object* {
        CURL* curl = curl_easy_init();

        if (!curl) {
            return nullptr;
        }

        std::string response;
        std::string url(path);

        json_object* body = json_object_new_object();
        json_object_object_add(body, "user_id", json_object_new_string(user_id.c_str()));
        const char* body_str = json_object_to_json_string(body);

        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, "Content-Type: application/json");

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body_str);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        CURLcode res = curl_easy_perform(curl);

        json_object* result = nullptr;

        if (res == CURLE_OK) {
            result = json_tokener_parse(response.c_str());
        }

        json_object_put(body);
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);

        return result;
    };
}

Sender share_my_file_with_user(const std::string& fileName, const std::string& targetUserName) {
    return [fileName, targetUserName](std::string_view path) -> json_object* {
        CURL* curl = curl_easy_init();

        if (!curl) {
            return nullptr;
        }

        std::string response;
        std::string url(path);

        json_object* body = json_object_new_object();
        json_object_object_add(body, "file_name", json_object_new_string(fileName.c_str()));
        json_object_object_add(body, "target_user_name", json_object_new_string(targetUserName.c_str()));
        const char* body_str = json_object_to_json_string(body);

        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, "Content-Type: application/json");

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body_str);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        CURLcode res = curl_easy_perform(curl);

        json_object* result = nullptr;

        if (res == CURLE_OK) {
            result = json_tokener_parse(response.c_str());
        }

        json_object_put(body);
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);

        return result;
    };
}
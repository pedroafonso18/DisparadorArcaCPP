#include "../include/Api/Api.h"
#include <curl/curl.h>
#include "../third_party/json.h"
#include <format>

size_t Api::WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t totalSize = size * nmemb;
    auto* resp = static_cast<std::string*>(userp);
    resp->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}

bool Api::SendMessageApi(Request *req) {
    const std::string url = "https://bk.atendimento-meuconsig.com.br/api/messages/send";
    CURL* curl = curl_easy_init();
    if (!curl) {
        std::clog << "ERROR: Failed to initialize CURL\n";
        return false;
    }

    nlohmann::json req_body = {
        {"number", req->Number},
        {"body", req->Body},
        {"queueId", 2},
        {"sendSignature", false},
        {"closeTicket", true},
        {"userId", ""}
    };

    std::string str_bod = req_body.dump();
    struct curl_slist* headers = nullptr;
    const std::string auth = std::format("Authorization: {}", req->ApiKey);
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, auth.c_str());

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, str_bod.c_str());

    CURLcode res = curl_easy_perform(curl);
    long http_code = 0;
    if (res == CURLE_OK) {
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
    } else {
        std::clog << "ERROR: curl_easy_perform failed: " << curl_easy_strerror(res) << "\n";
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    return (res == CURLE_OK && http_code >= 200 && http_code < 300);
}

std::string Api::Login(const std::string &email, const std::string &password) {
    const std::string url = "https://bk.atendimento-meuconsig.com.br/auth/login";
    CURL* curl = curl_easy_init();
    if (!curl) {
        std::clog << "ERROR: Failed to initialize CURL\n";
        return "";
    }
    nlohmann::json req_body = {
        {"email", email},
        {"password", password}
    };

    std::string respBody;
    std::string str_bod = req_body.dump();
    struct curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, str_bod.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &respBody);

    CURLcode res = curl_easy_perform(curl);
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK) {
        std::clog << "ERROR: curl_easy_perform failed: " << curl_easy_strerror(res) << "\n";
        return "";
    }

    try {
        auto json_resp = nlohmann::json::parse(respBody);
        if (json_resp.contains("token")) {
            return json_resp["token"].get<std::string>();
        } else {
            std::clog << "ERROR: No token in response\n";
            return "";
        }
    } catch (const std::exception& e) {
        std::clog << "ERROR: Failed to parse JSON: " << e.what() << "\n";
        return "";
    }
}

std::vector<Instance_t> Api::GetConnections(const std::string &email, const std::string &password) {
    const std::string url = "https://bk.atendimento-meuconsig.com.br/whatsapp?companyId=2&session=0";
    const std::string token = Login(email, password);
    std::string respBody;

    CURL* curl = curl_easy_init();
    if (!curl) {
        std::clog << "ERROR: Failed to initialize CURL\n";
        return {};
    }

    struct curl_slist* headers = nullptr;
    const std::string auth = std::format("Authorization: Bearer {}", token);
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, auth.c_str());

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &respBody);

    CURLcode res = curl_easy_perform(curl);
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK) {
        std::clog << "ERROR: curl_easy_perform failed: " << curl_easy_strerror(res) << "\n";
        return {};
    }

    std::vector<Instance_t> instances;

    try {
        auto json_resp = nlohmann::json::parse(respBody);
        if (!json_resp.is_array()) {
            std::clog << "ERROR: Expected array of connections in JSON\n";
            return {};
        }

        for (const auto& item : json_resp) {
            Instance_t inst;
            inst.Id = item.value("id", 0);
            inst.InstanceName = item.value("name", "");
            inst.InstanceNumber = item.value("number", "");
            inst.Status = item.value("status", "") == "CONNECTED";
            instances.push_back(inst);
        }

    } catch (const std::exception& e) {
        std::clog << "ERROR: Failed to parse JSON: " << e.what() << "\n";
        return {};
    }

    return instances;
}

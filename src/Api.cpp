#include "../include/Api.h"
#include <curl/curl.h>
#include "../third_party/json.h"
#include <format>

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

    std::string respBody;
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, str_bod.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &respBody);

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

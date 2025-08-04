#include "../include/Api.h"
#include <curl/curl.h>

bool Api::SendMessageApi(Request *req) {
    const std::string url = "https://bk.atendimento-meuconsig.com.br/api/messages/send";
    CURL* c;

}

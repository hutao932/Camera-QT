#ifndef __HT_XEYE_BASE_H__
#define __HT_XEYE_BASE_H__

#include <iostream>
#include <memory>
#include <string>
#include "http.h"
#include "json/json.h"
#include "curl.h"

namespace http_curl {

static const char* CURL_ERROR_CODE = "curl_error_code";
static const std::map<std::string, std::string> null;

class CurlHttpBase
{
    public:
    HttpClient client;
    Json::CharReaderBuilder crbuilder;
    CurlHttpBase(){};

    void setConnectionTimeoutInMillis(int connect_timeout)
    {
        this->client.setConnectTimeout(connect_timeout);
    }

    void setSocketTimeoutInMillis(int socket_timeout)
    {
        this->client.setSocketTimeout(socket_timeout);
    }

    void setDebug(bool debug)
    {
        this->client.setDebug(debug);
    }

    Json::Value request(
        std::string url,
        std::map<std::string, std::string> const & params,
        Json::Value const & data,
        std::map<std::string, std::string> const & headers)
    {
        std::string response;
        Json::Value obj;

        auto temp_params = params;
        int status_code = this->client.post(url, &temp_params, data, &headers, &response);

        if (status_code != CURLcode::CURLE_OK) {
            obj[CURL_ERROR_CODE] = status_code;
            return obj;
        }

        std::string error;
        std::unique_ptr<Json::CharReader> reader(crbuilder.newCharReader());
        reader->parse(response.data(), response.data() + response.size(), &obj, &error);
        std::cout << status_code << std::endl;
        // std::cout << response.data() <<std::endl;
        // TODO: wrong error handling logic
        return obj;
    }

    Json::Value get(
        std::string url,
        std::map<std::string, std::string> const & params,
        std::map<std::string, std::string> const & headers)
    {
        std::string response;
        Json::Value obj;

        auto temp_params = params;

        int status_code = this->client.get(url, &temp_params, &headers, &response);

        if (status_code != CURLcode::CURLE_OK) {
            obj[CURL_ERROR_CODE] = status_code;
            return obj;
        }

        std::string error;
        std::unique_ptr<Json::CharReader> reader(crbuilder.newCharReader());
        reader->parse(response.data(), response.data() + response.size(), &obj, &error);
        std::cout << error << std::endl;
        std::cout << status_code << std::endl;
        // std::cout << response.data();
        // TODO: wrong error handling logic
        return obj;
    }
};

}
#endif

#ifndef __HT_XEYE_HTTP_H__
#define __HT_XEYE_HTTP_H__

#include "curl.h"
#include <sstream>
#include <string>
#include <map>
#include "json/json.h"

namespace http_curl {

inline size_t onWriteData(void* buffer, size_t size, size_t nmemb, void* userp) {
    std::string* str = dynamic_cast<std::string*>((std::string*)userp);
    str->append((char*)buffer, size * nmemb);
    return nmemb;
}

class HttpClient {
public:
    HttpClient() = default;

    HttpClient(const HttpClient&) = delete;
    HttpClient& operator=(const HttpClient&) = delete;

    void setConnectTimeout(int connect_timeout) {
        this->connect_timeout = connect_timeout;
    }

    void setSocketTimeout(int socket_timeout) {
        this->socket_timeout = socket_timeout;
    }

    void setDebug(bool debug) {
        this->debug = debug;
    }

    int get(
        std::string url,
        std::map<std::string, std::string> const* params,
        std::map<std::string, std::string> const* headers,
        std::string* response) const {
        CURL* curl = curl_easy_init();
        struct curl_slist* slist = NULL;

        if (headers) {
            this->appendHeaders(*headers, &slist);
        }

        if (params) {
            this->appendUrlParams(*params, &url);
        }

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, onWriteData);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*) response);
        curl_easy_setopt(curl, CURLOPT_NOSIGNAL, true);
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT_MS, this->connect_timeout);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, this->socket_timeout);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);
        curl_easy_setopt(curl, CURLOPT_VERBOSE, this->debug);

        int status_code = curl_easy_perform(curl);

        curl_easy_cleanup(curl);
        curl_slist_free_all(slist);

        return status_code;
    }

    int post(
        std::string url,
        std::map<std::string, std::string> const* params,
        const std::string& body,
        std::map<std::string, std::string> const* headers,
        std::string* response) const {
        struct curl_slist* slist = NULL;
        static CURLSH* share_handle = NULL;

        if (!share_handle) {
            share_handle = curl_share_init();
            curl_share_setopt(share_handle, CURLSHOPT_SHARE, CURL_LOCK_DATA_DNS);
        }

        CURL* curl = curl_easy_init();

        if (headers) {
            this->appendHeaders(*headers, &slist);
        }

        if (params) {
            this->appendUrlParams(*params, &url);
        }

        curl_easy_setopt(curl, CURLOPT_SHARE, share_handle);
        curl_easy_setopt(curl, CURLOPT_DNS_CACHE_TIMEOUT, 518400L);

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist);
        curl_easy_setopt(curl, CURLOPT_POST, true);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, body.size());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, onWriteData);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*) response);
        curl_easy_setopt(curl, CURLOPT_NOSIGNAL, true);
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT_MS, this->connect_timeout);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, this->socket_timeout);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);
        curl_easy_setopt(curl, CURLOPT_VERBOSE, this->debug);

        int status_code = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        curl_slist_free_all(slist);

        return status_code;
    }

    int post(
        std::string url,
        std::map<std::string, std::string> const* params,
        std::map<std::string, std::string> const& data,
        std::map<std::string, std::string> const* headers,
        std::string* response) const {

        std::string body;
        this->makeUrlencodedForm(data, &body);
        return this->post(std::move(url), params, body, headers, response);
    }

    int post(
        std::string url,
        std::map<std::string, std::string> const* params,
        Json::Value const& data,
        std::map<std::string, std::string> const* headers,
        std::string* response) const {
        std::string body;
        // Warning: use foramt string type
        // Json::StreamWriterBuilder swb;
        // std::unique_ptr<Json::StreamWriter> writer(swb.newStreamWriter());
        // std::ostringstream os;
        // writer->write(data, &os);
        // body = os.str();
        Json::FastWriter writer;
        body = writer.write(data);
        return this->post(url.c_str(), params, body, headers, response);
    }

    int post(
        std::string url,
        std::map<std::string, std::string> const* params,
        std::map<std::string, std::string> const* headers,
        std::string* response) const {
        const static std::string EMPTY_STRING;
        return this->post(std::move(url), params, EMPTY_STRING, headers, response);
    }
private:
    bool debug = false;
    int connect_timeout = 100000;
    int socket_timeout = 100000;

    void makeUrlencodedForm(std::map<std::string, std::string> const& params,
                            std::string* content) const {
        content->clear();
        std::map<std::string, std::string>::const_iterator it;

        for (it = params.begin(); it != params.end(); it++) {
            char* key = curl_escape(it->first.c_str(), (int) it->first.size());
            char* value = curl_escape(it->second.c_str(), (int) it->second.size());
            *content += key;
            *content += '=';
            *content += value;
            *content += '&';
            curl_free(key);
            curl_free(value);
        }
    }

    void appendUrlParams(std::map<std::string, std::string> const& params, std::string* url) const {
        if (params.empty()) {
            return;
        }

        std::string content;
        this->makeUrlencodedForm(params, &content);
        bool url_has_param = false;

        for (const auto& ch : *url) {
            if (ch == '?') {
                url_has_param = true;
                break;
            }
        }

        if (url_has_param) {
            url->append("&");
        } else {
            url->append("?");
        }

        url->append(content);
    }

    void appendHeaders(std::map<std::string, std::string> const& headers, curl_slist** slist) const {
        std::map<std::string, std::string>::const_iterator it;

        for (it = headers.begin(); it != headers.end(); it++) {
            std::ostringstream ostr;
            ostr << it->first << ":" << it->second;
            *slist = curl_slist_append(*slist, ostr.str().c_str());
            // print http head
            // std::cout << ostr.str().c_str() << std::endl;
        }
    }
};

}
#endif

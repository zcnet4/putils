#pragma once

#include <string>
#include <unordered_map>

namespace kia
{
    namespace packets
    {
        struct Log
        {
            std::string msg;
        };

        struct HttpRequest
        {
            int clientFd;
            std::string method;
            std::string uri;
            std::string httpVersion;
            std::unordered_map<std::string, std::string> headers;
            std::unordered_map<std::string, std::string> params;
            std::string body;
        };

        struct HttpResponse
        {
            int clientFd;
            std::string httpVersion;
            std::string statusCode;
            std::string reasonPhrase;
            std::unordered_map<std::string, std::string> headers;
            std::string body;
        };

        struct IncomingMessage
        {
            int clientFd;
            std::string msg;
        };

        struct OutgoingMessage
        {
            int clientFd;
            std::string msg;
        };
    }

    inline packets::HttpResponse success(const packets::HttpRequest &origin, std::string_view statusCode = "200", std::string_view reasonPhrase = "")
    {
        return packets::HttpResponse{ origin.clientFd, origin.httpVersion, statusCode.data(), reasonPhrase.data(), {}, ""};
    }

    inline packets::HttpResponse error(const packets::HttpRequest &origin, std::string_view statusCode = "400", std::string_view reasonPhrase = "")
    {
        return packets::HttpResponse{ origin.clientFd, origin.httpVersion, statusCode.data(), reasonPhrase.data(), {}, ""};
    }

}

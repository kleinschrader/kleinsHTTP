#ifndef HTTPPARSER_H
#define HTTPPARSER_H

#include <iostream>
#include <regex>
#include <map>

#include "../packet/packet.h"
#include "../connection/connection.h"

namespace kleins {
    class httpParser
    {
    private:
        packet* data;
        connection* connsocket;
        std::map<std::string,const std::function<void(httpParser*)>> functionTable;
    public:
        httpParser(packet* httpdata, connection* conn);
        ~httpParser();

        bool parse();

        void on(const std::string& method,const std::string& uri, const std::function<void(httpParser*)> callback);

        void respond(const std::string& status, const std::list<std::string>& responseHeaders, const std::string& body);

        std::string requestline;
        std::string header;
        std::string body;

        std::string method;
        std::string uri;
    };    
}

#endif
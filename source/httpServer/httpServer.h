#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <list>

#include "../socketBase/socketBase.h"
#include "../connectionBase/connectionBase.h"
#include "../packet/packet.h"
#include "../httpParser/httpParser.h"

namespace kleins
{
    class httpServer
    {
    private:
        std::list<std::unique_ptr<socketBase>> sockets;

        std::map<std::string,const std::function<void(httpParser*)>> functionTable;


        void newConnection(connectionBase* conn);
    public:
        httpServer(/* args */);
        ~httpServer();

        void on(const std::string& method,const std::string& uri, const std::function<void(httpParser*)> callback);


        bool addSocket(socketBase* socket);
    };

}

#endif
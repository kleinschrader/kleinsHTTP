#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include "../socketBase/socketBase.h"

namespace kleins
{
    class httpServer
    {
    private:
        /* data */
    public:
        httpServer(/* args */);
        ~httpServer();

        void addSocket(socketBase* socket);
    };

}

#endif
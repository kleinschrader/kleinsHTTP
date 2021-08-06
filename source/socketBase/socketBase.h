#ifndef SOCKETBASE_H
#define SOCKETBASE_H

#include <sys/socket.h>
#include <future>
#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <thread>

#include "../connection/connection.h"


namespace kleins {

    class socketBase
    {
    protected:
    
        static void tickLoop(socketBase* socket);
        std::thread* tickThread;

        virtual bool tick() = 0;

        void startTicks();

    public:
        socketBase();
        ~socketBase();

        std::function<void(connection*)> newConnectionCallback;

        virtual std::future<bool> init() = 0;
    };
    
}

#endif
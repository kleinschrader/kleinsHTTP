#ifndef CONNECTIONBASE_H
#define CONNECTIONBASE_H

#include <thread>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <list>
#include <future>
#include <unistd.h>

#include "../packet/packet.h"

namespace kleins {
    class connectionBase
    {
    private:
        static void ownTickLoop(connectionBase* conn);
        std::thread* tickThread = 0;

    public:
        connectionBase();
        ~connectionBase();

        virtual bool getAlive() = 0;

        void startOwnTickLoop();

        virtual void tick() = 0;

        virtual void sendData(const char* data, int datalength) = 0;

        virtual void close_socket() = 0;

        void join();

        std::function<void(std::unique_ptr<packet>)> onRecieveCallback;
    };
}

#endif
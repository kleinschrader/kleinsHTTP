#ifndef CONNECTION_H
#define CONNECTION_H

#include <thread>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <list>
#include <future>
#include <unistd.h>

#include "../packet/packet.h"

namespace kleins {
    class connection
    {
    private:
        int connectionfd;

        static void ownTickLoop(connection* conn);
        std::thread* tickThread = 0;

    public:
        connection(int connectionid);
        ~connection();

        bool getAlive();

        void startOwnTickLoop();

        void tick();

        void sendData(const char* data, int datalength);

        void close_socket();

        void join();

        std::function<void(std::unique_ptr<packet>)> onRecieveCallback;
    };
}

#endif
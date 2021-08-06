#ifndef CONNECTION_H
#define CONNECTION_H

#include <thread>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <list>
#include <future>
#include <unistd.h>

#include "../connectionBase/connectionBase.h"
#include "../packet/packet.h"

namespace kleins {
    class tcpConnection : public connectionBase
    {
    private:
        int connectionfd;

    public:
        tcpConnection(int connectionid);
        ~tcpConnection();

        virtual bool getAlive();
        virtual void tick();
        virtual void sendData(const char* data, int datalength);
        virtual void close_socket();

        std::function<void(std::unique_ptr<packet>)> onRecieveCallback;
    };
};

#endif
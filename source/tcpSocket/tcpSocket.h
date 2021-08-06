#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <sys/socket.h>
#include <future>
#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <thread>

#include "../connection/connection.h"
#include "../socketBase/socketBase.h"

namespace kleins {

    class tcpSocket : private socketBase
    {
    private:
        int socketfd;
        int opt;
        struct sockaddr_in address;
        int addrlen;

        bool tick();

    public:
        tcpSocket(const char* listenAddress, const int listenPort);
        ~tcpSocket();

        std::function<void(connection*)> newConnectionCallback;

        std::future<bool> init();
    };
    
}

#endif
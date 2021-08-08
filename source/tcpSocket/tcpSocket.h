#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <sys/socket.h>
#include <future>
#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <thread>


#include "../tcpConnection/tcpConnection.h"
#include "../socketBase/socketBase.h"



namespace kleins {
    class tcpSocket : public socketBase
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

        std::future<bool> init();
    };
    
};

#endif
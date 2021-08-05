#include <sys/socket.h>
#include <future>
#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <thread>

#include "../connection/connection.h"


namespace kleins {

    class tcpSocket
    {
    private:
        int socketfd;
        int opt;
        struct sockaddr_in address;
        int addrlen;

        static void connectionLoop(tcpSocket* socket);
        std::thread* connectionThread;

    public:
        tcpSocket(const char* listenAddress, const int listenPort);
        ~tcpSocket();

        std::function<void(connection*)> newConnectionCallback;

        std::future<bool> init();
    };
    
}
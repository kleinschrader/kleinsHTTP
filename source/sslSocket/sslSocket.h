#ifndef SSLSOCKET_T
#define SSLSOCKET_T

#include <sys/socket.h>
#include <future>
#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <thread>
#include <openssl/ssl.h>
#include <openssl/err.h>

#include "../sslConnection/sslConnection.h"
#include "../socketBase/socketBase.h"



namespace kleins {
    class sslSocket : public socketBase
    {
    private:
        int socketfd;
        int opt;
        struct sockaddr_in address;
        int addrlen;

        SSL_CTX *ctx;

        bool tick();

    public:
        sslSocket(const char* listenAddress, const int listenPort, const char* pathToCertificate, const char* pathToKey);
        ~sslSocket();

        std::future<bool> init();
    };
    
};

#endif
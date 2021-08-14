#ifndef CONNECTION_H
#define CONNECTION_H

#include <thread>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <list>
#include <future>
#include <unistd.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#include "../connectionBase/connectionBase.h"
#include "../packet/packet.h"

namespace kleins {
    class sslConnection : public connectionBase
    {
    private:
        int connectionfd;
        SSL_CTX* ctx;
        SSL* ossl;
        bool initOk = true;
    public:
        sslConnection(int connectionid, SSL_CTX* sslcontext);
        ~sslConnection();

        virtual bool getAlive();
        virtual void tick();
        virtual void sendData(const char* data, int datalength);
        virtual void close_socket();
    };
};

#endif
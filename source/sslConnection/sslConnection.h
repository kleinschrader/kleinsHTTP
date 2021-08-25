#ifndef CONNECTION_H
#define CONNECTION_H

#include <future>
#include <iostream>
#include <list>
#include <openssl/err.h>
#include <openssl/ssl.h>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>

#ifndef SINGLE_HEADER
#include "../connectionBase/connectionBase.h"
#include "../packet/packet.h"
#endif

namespace kleins {
class sslConnection : public connectionBase {
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
}; // namespace kleins

#endif

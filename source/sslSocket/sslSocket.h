#ifndef SSLSOCKET_T
#define SSLSOCKET_T

#include <arpa/inet.h>
#include <future>
#include <iostream>
#include <netinet/in.h>
#include <openssl/err.h>
#include <openssl/ssl.h>
#include <sys/socket.h>
#include <thread>

#ifndef SINGLE_HEADER
#include "../socketBase/socketBase.h"
#include "../sslConnection/sslConnection.h"
#endif

namespace kleins {
class sslSocket : public socketBase {
private:
  int socketfd;
  int opt;
  struct sockaddr_in address;
  int addrlen;

  SSL_CTX* ctx;

  bool tick();

public:
  sslSocket(const char* listenAddress, const int listenPort, const char* pathToCertificate, const char* pathToKey);
  ~sslSocket();

  std::future<bool> init();
};

}; // namespace kleins

#endif

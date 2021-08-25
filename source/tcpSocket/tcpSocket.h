#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <arpa/inet.h>
#include <future>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <thread>

#ifndef SINGLE_HEADER
#include "../socketBase/socketBase.h"
#include "../tcpConnection/tcpConnection.h"
#endif

namespace kleins {
class tcpSocket : public socketBase {
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

}; // namespace kleins

#endif

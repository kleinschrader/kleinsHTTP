#ifndef CONNECTION_H
#define CONNECTION_H

#include <future>
#include <iostream>
#include <list>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>

#ifndef SINGLE_HEADER
#include "../connectionBase/connectionBase.h"
#include "../packet/packet.h"
#endif

namespace kleins {
class tcpConnection : public connectionBase {
private:
  int connectionfd;

public:
  tcpConnection(int connectionid);
  ~tcpConnection();

  virtual bool getAlive();
  virtual void tick();
  virtual void sendData(const char* data, int datalength);
  virtual void close_socket();
};
}; // namespace kleins

#endif

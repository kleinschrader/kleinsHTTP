#ifndef SOCKETBASE_H
#define SOCKETBASE_H

#include <arpa/inet.h>
#include <future>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <thread>

#ifndef SINGLE_HEADER
#include "../connectionBase/connectionBase.h"
#endif

namespace kleins {

class socketBase {
protected:
  static void tickLoop(socketBase* socket);
  std::thread* tickThread;

  virtual bool tick() = 0;

public:
  socketBase();
  ~socketBase();

  void startTicks();

  std::function<void(connectionBase*)> newConnectionCallback;
  virtual std::future<bool> init() = 0;
};

} // namespace kleins

#endif

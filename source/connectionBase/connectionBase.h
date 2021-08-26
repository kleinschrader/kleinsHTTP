#ifndef CONNECTIONBASE_H
#define CONNECTIONBASE_H

#include <chrono>
#include <future>
#include <iostream>
#include <list>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>

#ifndef SINGLE_HEADER
#include "../packet/packet.h"
#endif

namespace kleins {
class connectionBase {
private:
  static void ownTickLoop(connectionBase* conn);
  std::thread* tickThread = 0;
  unsigned int timeout = 30000;

  std::chrono::time_point<std::chrono::steady_clock> lastPacket;

public:
  connectionBase();
  ~connectionBase();

  virtual bool getAlive() = 0;

  void startOwnTickLoop();

  virtual void tick() = 0;

  virtual void sendData(const char* data, int datalength) = 0;

  virtual void close_socket() = 0;

  void join();

  void setTimeout(unsigned int timeoutInMS = 30000);

  void resetTimeoutTimer();
  bool getTimeout();

  std::function<void(std::unique_ptr<packet>)> onRecieveCallback;
};
} // namespace kleins

#endif

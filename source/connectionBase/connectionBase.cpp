#include "connectionBase.h"

kleins::connectionBase::connectionBase() {
}

kleins::connectionBase::~connectionBase() {
  if (tickThread) {
    delete tickThread;
  }
}

void kleins::connectionBase::startOwnTickLoop() {
  tickThread = new std::thread(ownTickLoop, this);
}

void kleins::connectionBase::ownTickLoop(connectionBase* connection) {
  while (connection->getAlive()) {
    connection->tick();
    usleep(2000);
  }
}

void kleins::connectionBase::join() {
  if (tickThread != 0) {
    if (tickThread->joinable()) {
      tickThread->join();
    }
  }
}

void kleins::connectionBase::setTimeout(unsigned int timeoutin) {
  timeout = timeoutin;
}

void kleins::connectionBase::resetTimeoutTimer() {
  lastPacket = std::chrono::steady_clock::now();
}

bool kleins::connectionBase::getTimeout() {
  if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - lastPacket).count() > timeout) {
    return true;
  }
  return false;
}
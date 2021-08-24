#include "socketBase.h"

kleins::socketBase::socketBase() {
}

kleins::socketBase::~socketBase() {
  delete tickThread;
}

void kleins::socketBase::tickLoop(socketBase* socket) {
  while (socket->tick()) {
  };
}

void kleins::socketBase::startTicks() {
  tickThread = new std::thread(tickLoop, this);
}
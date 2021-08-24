#include "tcpConnection.h"

kleins::tcpConnection::tcpConnection(int connectionid) {
  connectionfd = connectionid;
  resetTimeoutTimer();
}

kleins::tcpConnection::~tcpConnection() {
  close_socket();
  join();
}

bool kleins::tcpConnection::getAlive() {
  int error = 0;
  socklen_t len = sizeof(error);
  int retval = getsockopt(connectionfd, SOL_SOCKET, SO_ERROR, &error, &len);

  return (error | retval) == 0;
}

void kleins::tcpConnection::tick() {
  packet* packetBuffer = new packet;

  packetBuffer->data.resize(4096);
  packetBuffer->size = recv(connectionfd, (char*)&packetBuffer->data[0], 4096, MSG_DONTWAIT);

  if (packetBuffer->size == -1) {
    delete packetBuffer;
    usleep(20000);

    if (getTimeout()) {
      close_socket();
    }

    return;
  }

  resetTimeoutTimer();

  packetBuffer->data.resize(packetBuffer->size);

  this->onRecieveCallback(std::unique_ptr<packet>(packetBuffer));
}

void kleins::tcpConnection::sendData(const char* data, int datalength) {
  send(connectionfd, data, datalength, 0);
}

void kleins::tcpConnection::close_socket() {
  close(connectionfd);
}
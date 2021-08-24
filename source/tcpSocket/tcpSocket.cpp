#include "tcpSocket.h"

kleins::tcpSocket::tcpSocket(const char* listenAddress, const int listenPort) {
  address.sin_family = AF_INET;

  address.sin_port = htons(listenPort);
  inet_aton(listenAddress, (in_addr*)&address.sin_addr.s_addr);
}

kleins::tcpSocket::~tcpSocket() {
  close(socketfd);
}

bool kleins::tcpSocket::tick() {
  int newConnection;

  newConnection = accept(socketfd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
  tcpConnection* conn = new tcpConnection(newConnection);
  newConnectionCallback(conn);

  return newConnection;
}

std::future<bool> kleins::tcpSocket::init() {
  auto init_async = [this]() {
    socketfd = socket(AF_INET, SOCK_STREAM, 0);

    if (!socketfd) {
      std::cerr << "Error creating socket file descriptor" << std::endl;
      return false;
    }

    if (setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT | SO_KEEPALIVE, &opt, sizeof(opt))) {
      std::cerr << "Error setting socket opts" << std::endl;
      return false;
    }

    if (bind(socketfd, (struct sockaddr*)&address, sizeof(address)) < 0) {
      std::cerr << "Error binding socket" << std::endl;
      return false;
    }
    if (listen(socketfd, 3) < 0) {
      std::cerr << "Error listening on socket" << std::endl;
      return false;
    }

    addrlen = sizeof(address);

    return true;
  };

  return std::async(std::launch::async, init_async);
}
#include "sslSocket.h"

kleins::sslSocket::sslSocket(const char* listenAddress, const int listenPort, const char* pathToCertificate, const char* pathToKey) {
  address.sin_family = AF_INET;

  address.sin_port = htons(listenPort);
  inet_aton(listenAddress, (in_addr*)&address.sin_addr.s_addr);

  SSL_load_error_strings();
  OpenSSL_add_ssl_algorithms();

  const SSL_METHOD* method;

  method = SSLv23_server_method();

  ctx = SSL_CTX_new(method);
  if (!ctx) {
    perror("Unable to create SSL context");
    ERR_print_errors_fp(stderr);
    exit(EXIT_FAILURE);
  }

  SSL_CTX_set_ecdh_auto(ctx, 1);

  if (SSL_CTX_use_certificate_file(ctx, pathToCertificate, SSL_FILETYPE_PEM) <= 0) {
    ERR_print_errors_fp(stderr);
    exit(EXIT_FAILURE);
  }

  if (SSL_CTX_use_PrivateKey_file(ctx, pathToKey, SSL_FILETYPE_PEM) <= 0) {
    ERR_print_errors_fp(stderr);
    exit(EXIT_FAILURE);
  }
}

kleins::sslSocket::~sslSocket() {
  close(socketfd);
}

bool kleins::sslSocket::tick() {
  int newConnection;

  newConnection = accept(socketfd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
  sslConnection* conn = new sslConnection(newConnection, ctx);
  newConnectionCallback(conn);

  return newConnection;
}

std::future<bool> kleins::sslSocket::init() {
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
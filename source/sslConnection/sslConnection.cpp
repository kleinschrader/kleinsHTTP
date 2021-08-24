#include "sslConnection.h"

kleins::sslConnection::sslConnection(int connectionid, SSL_CTX* sslcontext) {
  connectionfd = connectionid;
  ctx = sslcontext;
  ossl = SSL_new(ctx);
  SSL_set_fd(ossl, connectionfd);
  if (SSL_accept(ossl) <= 0) {
    ERR_print_errors_fp(stderr);
    initOk = false;
  }
  resetTimeoutTimer();
}

kleins::sslConnection::~sslConnection() {
  close_socket();
  join();
}

bool kleins::sslConnection::getAlive() {
  if (!initOk) {
    return false;
  }

  int error = 0;
  socklen_t len = sizeof(error);
  int retval = getsockopt(connectionfd, SOL_SOCKET, SO_ERROR, &error, &len);

  return (error | retval) == 0;
}

void kleins::sslConnection::tick() {
  packet* packetBuffer = new packet;

  packetBuffer->data.resize(4096);
  SSL_read_ex(ossl, (char*)&packetBuffer->data[0], 4096, (size_t*)&packetBuffer->size);

  if (!packetBuffer->size) {
    usleep(20000);
    delete packetBuffer;

    if (getTimeout()) {
      close_socket();
    }

    return;
  }

  resetTimeoutTimer();

  packetBuffer->data.resize(packetBuffer->size);

  this->onRecieveCallback(std::unique_ptr<packet>(packetBuffer));
}

void kleins::sslConnection::sendData(const char* data, int datalength) {
  SSL_write(ossl, data, datalength);
}

void kleins::sslConnection::close_socket() {
  SSL_shutdown(ossl);
  SSL_free(ossl);
  close(connectionfd);
}
#include "../../libkleinsHTTP.h"

int main() {
  kleins::httpServer server;
  server.startMetricsServer(9090);

  server.printVersion();

  server.addSocket(new kleins::tcpSocket("0.0.0.0", 8080));

  server.on(kleins::httpMethod::GET, "/", [](kleins::httpParser* data) { data->respond("200", {}, "Hello!"); });

  server.on(kleins::httpMethod::GET, "/hi", [](kleins::httpParser* data) { data->respond("200", {}, "Hi!"); });

  for (;;) {
    usleep(90000);
  }
}

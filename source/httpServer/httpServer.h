#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <filesystem>
#include <fstream>
#include <list>

#include "../connectionBase/connectionBase.h"
#include "../httpParser/httpParser.h"
#include "../packet/packet.h"
#include "../socketBase/socketBase.h"

namespace kleins {
class httpServer {
private:
  std::list<std::unique_ptr<socketBase>> sockets;

  std::map<std::string, const std::function<void(httpParser *)>> functionTable;
  std::map<std::string, std::string> fileLookup;

  void newConnection(connectionBase *conn);

  static std::map<std::string, std::string> mimeLookup;

public:
  httpServer(/* args */);
  ~httpServer();

  void on(const std::string &method, const std::string &uri,
          const std::function<void(httpParser *)> callback);

  void serve(const std::string &uri, const std::string &path);

  void serveDirectory(const std::string &baseuri, const std::string &path,
                      const std::string indexFile = "index.html");

  bool addSocket(socketBase *socket);
};

} // namespace kleins

#endif
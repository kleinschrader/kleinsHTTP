#ifndef HTTPPARSER_H
#define HTTPPARSER_H

#include <iostream>
#include <map>
#include <regex>
#include <list>


#ifndef SINGLE_HEADER
#include "../connectionBase/connectionBase.h"
#include "../httpServer/httpServer.h"
#include "../packet/packet.h"
#include "../sessionBase/sessionBase.h"
#endif

namespace kleins {
class httpServer;

class httpParser {
private:
  packet* data;
  connectionBase* connsocket;
  httpServer* server;
  std::map<std::string, const std::function<void(httpParser*)>> functionTable;

  inline void parseRequestline();
  inline void parseHeaders();

  void parseURLencodedData(const char* rawData);
  void parseURLencodedData(std::string& rawData);
  void parseURLencodedData(const char* rawData, const int length);

public:
  httpParser(packet* httpdata, connectionBase* conn, httpServer* srv);
  ~httpParser();

  bool parse();

  template <class T>
  sessionBase* startSession();

  void on(const std::string& method, const std::string& uri, const std::function<void(httpParser*)> callback);
  void on(const std::string& ref, const std::function<void(httpParser*)> callback);

  void respond(const std::string& status, const std::list<std::string>& responseHeaders, const std::string& body, const std::string& mimeType = "text/html");

  std::string requestline;
  std::string header;
  std::string body;

  const std::string* sessionKey = 0;

  std::string method;
  std::string path;
  std::map<std::string, std::string> parameters;
  std::map<std::string, std::string> headers;
};
} // namespace kleins

#endif

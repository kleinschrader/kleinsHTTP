#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <filesystem>
#include <fstream>
#include <list>
#include <openssl/rand.h>

#ifndef SINGLE_HEADER
#include "../connectionBase/connectionBase.h"
#include "../counterMetric/counterMetric.h"
#include "../gaugeMetric/gaugeMetric.h"
#include "../histogramMetric/histogramMetric.h"
#include "../httpParser/httpParser.h"
#include "../packet/packet.h"
#include "../sessionBase/sessionBase.h"
#include "../socketBase/socketBase.h"
#include "../tcpSocket/tcpSocket.h"
#endif

#ifndef BUILD_VERSION
#define BUILD_VERSION "UNKNOWN"
#endif

namespace kleins {

class httpParser;

typedef enum httpMethod { GET, HEAD, POST, PUT, DELETE, CONNECT, OPTIONS, TRACE, PATCH } httpMethod;

/**
 * @brief An httpserver that will take care of connection managment, serving of static files and api endpoints
 * 
 * 
 * 
 */
class httpServer {
private:
  std::map<std::string, sessionBase*> sessions;

  std::thread* sessionCleanupThread;
  static void cleanUpSessionLoop(httpServer* server);

  bool keepRunning = true;

  std::list<std::unique_ptr<socketBase>> sockets;

  std::map<std::string, const std::function<void(httpParser*)>> functionTable;
  std::map<std::string, std::string> fileLookup;

  void newConnection(connectionBase* conn);

  static std::map<std::string, std::string> mimeLookup;
  static std::map<httpMethod, std::string> methodLookup;

  httpServer* mServer = 0;

  metrics::counterMetric* metric_totalAcccess = 0;
  metrics::histogramMetric* metric_access = 0;
  metrics::counterMetric* metric_totalSessions = 0;
  metrics::gaugeMetric* metric_activeSessions = 0;

public:
  /**
   * @brief httpServer constructor
   * 
   */
  httpServer(/* args */);

  /**
   * @brief Destroy the http Server object
   * 
   */
  ~httpServer();

  /**
   * @brief Add an endpoint to the httpServer
   * 
   * @param method The method of the endpoint
   * @param uri The url to respond to on ('/', 'api/hello') 
   * @param callback The callback function that gets triggered when a client acces it.
   * 
   * Example:
   * 
   * \code{.cpp}
   * server.on(kleins::httpMethod::GET,"/hello",[](httpParser* parser){
   *    data->respond("200",{},"Hello!");
   * });
   * \endcode
   */
  void on(httpMethod method, const std::string& uri, const std::function<void(httpParser*)> callback);

  /**
   * @brief Serve a localfile under a path
   * 
   * @param uri The url the file should be provided under
   * @param path The local path of the file
   */
  void serve(const std::string& uri, const std::string& path);

  /**
   * @brief Automaticly serve a directory recursivly
   * 
   * @param baseuri The parent path of all the files ('/','static/')
   * @param path The path of the local files to be served
   * @param indexFile Set the name of the index files that will also be avaliable under /
   */
  void serveDirectory(const std::string& baseuri, const std::string& path, const std::string indexFile = "index.html");

  /**
   * @brief Add a socket to listen on
   * 
   * @param socket A pointer to a socket that should be used. \n It should be noted that it the socket will be delted when the server object gets deleted.
   * @return true 
   * @return false 
   */
  bool addSocket(socketBase* socket);

  /**
   * @brief Print the version of kleinsHTTP to the stdout
   * 
   */
  void printVersion();

  void startMetricsServer(uint16_t port);

  metrics::counterMetric* metric_notfound = 0;

  template <class T> sessionBase* startSession(std::string& authKey);
};

}; // namespace kleins

#endif

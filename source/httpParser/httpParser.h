#ifndef HTTPPARSER_H
#define HTTPPARSER_H

#include <iostream>
#include <map>
#include <regex>

#include "../connectionBase/connectionBase.h"
#include "../packet/packet.h"

namespace kleins {

/**
 * @brief The httpParser that implements most of the login for the server.
 * 
 * The httpParser takes care of the following tasks:
 *  
 * - Parsing the http request.
 * - Routing the http Request
 * - Respond with 404 on missing route.
 * - Formating a response and sending it to the socket.
 */
class httpParser {
private:
  packet* data;
  connectionBase* connsocket;
  std::map<std::string, const std::function<void(httpParser*)>> functionTable;

  inline void parseRequestline();
  inline void parseHeaders();

  void parseURLencodedData(const char* rawData);
  void parseURLencodedData(std::string& rawData);
  void parseURLencodedData(const char* rawData, const int length);

public:
/**
 * @brief Construct a new httpParser object
 * 
 * @param httpdata a pointer to the data just recieved
 * @param conn a pointer to the connection just used
 */
  httpParser(packet* httpdata, connectionBase* conn);

  /**
   * @brief Destroy the http Parser object
   * 
   */
  ~httpParser();

  /**
   * @brief This function is responsible for parsing the http request header. 
   * 
   * It is the job of httpServer call this, you may encounter errors if you call it manually.
   * 
   * @return true Valid HTTP Request
   * @return false Invalid HTTP Request
   */
  bool parse();

  /**
   * @brief Adds a callback to route
   * 
   * This is also set my the httpServer.
   * 
   * @param method The method to listen to (GET, POST, DELETE, etc...)
   * @param uri The path to listen on ("/", "example", etc...)
   * @param callback A lambda `[](httpParser*){void}` to call if the route is hit
   */
  void on(const std::string& method, const std::string& uri, const std::function<void(httpParser*)> callback);

  /**
   * @brief Adds a callback to route
   * 
   * This is also set my the httpServer.
   * 
   * @param ref This is essentialy the same as above except the uri is append to the method.
   * @param callback A lambda `[](httpParser*){void}` to call if the route is hit
   */
  void on(const std::string& ref, const std::function<void(httpParser*)> callback);

  /**
   * @brief Generate a response and send it to the connected client
   * 
   * @param status A string of response code of the httpRequest.
   * @param responseHeaders A list of responseHeader to be set. It should be noted that the header should alread follow the "Value: Key" standard.
   * @param body The body to send back.
   * @param mimeType The mimetype to use for the reply. "text/html" is the default but if you encouter issues "text/plain also works"
   */
  void respond(const std::string& status, const std::list<std::string>& responseHeaders, const std::string& body, const std::string& mimeType = "text/html");


  /**
   * @brief The complete requestline in the request ("GET / HTTP/1.1")
   * 
   */
  std::string requestline;
  /**
   * @brief The entire header of the request.
   * 
   */
  std::string header;

  /**
   * @brief A key value map of header
   * 
   */
  std::map<std::string, std::string> headers;

  /**
   * @brief The body of the request.
   * 
   */
  std::string body;

  /**
   * @brief The method used (GET, POST) of the request.
   * 
   */
  std::string method;

  /**
   * @brief The url requested.
   * 
   */
  std::string path;

  /**
   * @brief A key value of the paramets sent along the request.
   * 
   * This will only be set from URLENCODED or FORM-URLENCODED parameters
   */
  std::map<std::string, std::string> parameters;
};
} // namespace kleins

#endif
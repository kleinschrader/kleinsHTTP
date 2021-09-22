#ifndef SINGLE_HEADER
#include "httpParser/httpParser.h"
#include "httpServer/httpServer.h"
#endif

template <class T> kleins::sessionBase* kleins::httpParser::startSession() {
  kleins::sessionBase* sb;
  if (headers["Cookie"].length() > 19) {
    std::string cookie = headers["Cookie"].substr(19);
    sb = server->startSession<T>(cookie);
  } else {
    sb = server->startSession<T>(headers["Cookie"]);
  }
  sessionKey = sb->sessionKey;
  return sb;
}

template <class T> kleins::sessionBase* kleins::httpServer::startSession(std::string& authKey) {
  try {
    auto it = sessions.at(authKey);
    return it;
  } catch (const std::out_of_range& oor) {
  }

  const char* keychars = "1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

  char* newKey = new char[33];
  newKey[32] = 0;

  RAND_bytes((unsigned char*)newKey, 32);

  for (int i = 0; i < 32; i++) {
    newKey[i] = keychars[((unsigned int)newKey[i]) % 62];
  };

  auto sb = new T();

  auto ins = sessions.insert(std::make_pair(std::string(newKey), sb));

  metric_totalSessions->inc();
  metric_activeSessions->set(metric_activeSessions->get() + 1);

  sb->sessionKey = &ins.first->first;

  return sb;
}

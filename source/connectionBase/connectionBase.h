#ifndef CONNECTIONBASE_H
#define CONNECTIONBASE_H

#include <chrono>
#include <future>
#include <iostream>
#include <list>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>

#include "../packet/packet.h"

namespace kleins {
/**
 * @brief This is the base class for all things client connections
 * 
 * You should not use this on its own but rather inherit it.
 */
class connectionBase {
private:
  static void ownTickLoop(connectionBase* conn);
  std::thread* tickThread = 0;
  unsigned int timeout = 30000;

  std::chrono::time_point<std::chrono::steady_clock> lastPacket;

public:
  /**
 * @brief Construct a new connection Base object
 * 
 */
  connectionBase();

  /**
   * @brief Destroy the connection Base object
   * 
   */
  ~connectionBase();

  /**
   * @brief Return if the connection is still alive
   * 
   * @return true Connection is alive
   * @return false Connection is dead
   */
  virtual bool getAlive() = 0;

  /**
   * @brief This will start a loop to continusly call tick.
   * 
   * It will sleep 2ms between calls to keep the cpu happy.
   */
  void startOwnTickLoop();

  /**
   * @brief This function will read the connection and call the callback function if data is read.
   * 
   */
  virtual void tick() = 0;

  /**
   * @brief Sends data to the client
   * 
   * @param data The data that should be send
   * @param datalength The length of data
   */
  virtual void sendData(const char* data, int datalength) = 0;

  /**
   * @brief Closes the connection.
   * 
   */
  virtual void close_socket() = 0;

  /**
   * @brief Join the calling thread with the tickloop
   * 
   * This will essantially wait till the socket is closed.
   * 
   */
  void join();

  /**
   * @brief Set the Timeout of the socket.
   * 
   * If the timeout expires it will close the connection.
   * The timeout is reset everytime data is recieved
   * 
   * @param timeoutInMS The time to wait for the client is milliseconds.
   */
  void setTimeout(unsigned int timeoutInMS = 30000);

  /**
   * @brief Resets the timeout timer.
   * 
   */
  void resetTimeoutTimer();

  /**
   * @brief Checks if the timeout has been hit
   * 
   * @return true Timeout has been hit
   * @return false Timeout has not been hit
   */
  bool getTimeout();

  /**
   * @brief Set this callback to whatever you want to do with the data.
   * 
   */
  std::function<void(std::unique_ptr<packet>)> onRecieveCallback;
};
} // namespace kleins

#endif

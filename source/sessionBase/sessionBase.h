#ifndef SESSIONBASE_H
#define SESSIONBASE_H

#include <chrono>
#include <string>

namespace kleins {
/**
 * @brief a base class for storing session data.
 * 
 * To use this, start the httpServer with the autosession flag and inherit this class to modify it how you like.
 */
class sessionBase {
public:
  /**
     * @brief Default constructor.
     * 
     * @param timeout When the session should expire in minutes from now
     * 
     * Sets the time when this session expires.
     */
  sessionBase(unsigned int timeout = 480);

  /**
     * @brief Destroy the session Base object
     * 
     * Does nothing
     */
  ~sessionBase();

  /**
     * @brief The session key this session uses.
     * 
     */
  const std::string* sessionKey;

  /**
     * @brief The time at which this session expires
     * 
     */
  std::chrono::time_point<std::chrono::system_clock> expireTime;
};
} // namespace kleins

#endif
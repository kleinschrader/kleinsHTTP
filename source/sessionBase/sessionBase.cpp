#include "sessionBase.h"

kleins::sessionBase::sessionBase(unsigned int timeout) {
  expireTime = std::chrono::system_clock::now() + std::chrono::minutes(timeout);
}

kleins::sessionBase::~sessionBase() {
}
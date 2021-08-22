#ifndef PACKET_H
#define PACKET_H

#include <string>

namespace kleins {
struct packet {
public:
  std::string data;
  int size;
};
} // namespace kleins

#endif
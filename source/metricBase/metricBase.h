#ifndef METRICBASE_H
#define METRICBASE_H

#include <memory>

namespace kleins {

namespace metrics {

class metricBase {
protected:
  const char* helpString = "";
  const char* nameString = "";

public:
  metricBase(/* args */);
  ~metricBase();

  virtual const char* getType() = 0;

  void setHelp(char* help);
  void setName(char* name);

  virtual std::unique_ptr<char*> construct() = 0;
};

} // namespace metrics

} // namespace kleins

#endif
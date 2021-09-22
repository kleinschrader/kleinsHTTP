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

  void setHelp(const char* help);
  void setName(const char* name);

  virtual std::unique_ptr<char*> construct() = 0;
};

} // namespace metrics

} // namespace kleins

#endif
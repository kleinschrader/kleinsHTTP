#ifndef GAUGEMETRIC_H
#define GAUGEMETRIC_H

#include <cassert>
#include <stdio.h>

#ifndef SINGLE_HEADER
#include "../metricBase/metricBase.h"
#endif

namespace kleins {

namespace metrics {

class gaugeMetric : public metricBase {
private:
  uint64_t counterValue = 0;

public:
  gaugeMetric(const char* name, const char* help);
  ~gaugeMetric();

  uint64_t get();
  void set(uint64_t value);

  virtual const char* getType();

  virtual std::unique_ptr<char*> construct();
};

} // namespace metrics

} // namespace kleins

#endif
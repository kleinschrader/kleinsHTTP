#ifndef HISTOGRAMMETRIC_H
#define HISTOGRAMMETRIC_H

#include <cassert>
#include <cstring>
#include <map>
#include <sstream>
#include <stdio.h>
#include <string>

#ifndef SINGLE_HEADER
#include "../metricBase/metricBase.h"
#endif

namespace kleins {

namespace metrics {

class counterBucketMetricData {
private:
  uint64_t counterValue = 0;

public:
  uint64_t get();
  void set(uint64_t value);
  void inc(uint64_t value = 1);
  void reset();
};

class histogramMetric : public metricBase {
private:
  std::map<std::string, counterBucketMetricData*> counterValues;

public:
  histogramMetric(const char* name, const char* help);
  ~histogramMetric();

  counterBucketMetricData* operator[](std::string& input);
  counterBucketMetricData* operator[](const char* input);

  void addBucket(const char* bucketName);

  virtual const char* getType();

  virtual std::unique_ptr<char*> construct();
};

} // namespace metrics

} // namespace kleins

#endif
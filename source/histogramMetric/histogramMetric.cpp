#include "histogramMetric.h"

uint64_t kleins::metrics::counterBucketMetricData::get() {
  return counterValue;
}

void kleins::metrics::counterBucketMetricData::set(uint64_t value) {
  assert(("Attemped to decrease counter value", value < counterValue));

  counterValue = value;
}

void kleins::metrics::counterBucketMetricData::inc(uint64_t value) {
  counterValue += value;
}

void kleins::metrics::counterBucketMetricData::reset() {
  counterValue = 0;
}

kleins::metrics::histogramMetric::histogramMetric(const char* name, const char* help) {
  setName(name);
  setHelp(help);
}

kleins::metrics::histogramMetric::~histogramMetric() {
}

const char* kleins::metrics::histogramMetric::getType() {
  return "counter";
}

void kleins::metrics::histogramMetric::addBucket(const char* bucketName) {
  counterValues.insert(std::make_pair(std::string(bucketName), new counterBucketMetricData));
}

kleins::metrics::counterBucketMetricData* kleins::metrics::histogramMetric::operator[](std::string& input) {
  for (auto x : counterValues) {
    if (x.first == input) {
      return x.second;
    }
  }
  assert("A Bucket was accessed that never existed");
  return 0;
}

kleins::metrics::counterBucketMetricData* kleins::metrics::histogramMetric::operator[](const char* input) {
  for (auto x : counterValues) {
    if (x.first == input) {
      return x.second;
    }
  }
  assert("A Bucket was accessed that never existed");
  return 0;
}

std::unique_ptr<char*> kleins::metrics::histogramMetric::construct() {
  std::stringstream ss;
  ss << "# HELP " << nameString << ' ' << helpString << "\n"
     << "# TYPE " << nameString << ' ' << getType() << "\n";

  for (auto x : counterValues) {
    ss << nameString << '{' << x.first << "} " << x.second->get() << "\n";
  }
  int totalLength = ss.str().length();

  char* outputBuffer = new char[totalLength];
  std::memcpy(outputBuffer, ss.str().c_str(), totalLength);

  return std::make_unique<char*>(outputBuffer);
};
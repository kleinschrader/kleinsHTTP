#include "gaugeMetric.h"

kleins::metrics::gaugeMetric::gaugeMetric(const char* name, const char* help) {
  setName(name);
  setHelp(help);
}

kleins::metrics::gaugeMetric::~gaugeMetric() {
}

const char* kleins::metrics::gaugeMetric::getType() {
  return "gauge";
}

std::unique_ptr<char*> kleins::metrics::gaugeMetric::construct() {
  char* targetBuffer = new char[4096];

  snprintf(targetBuffer, 4096, "# HELP %s %s\n# TYPE %s %s\n%s %i\n", nameString, helpString, nameString, getType(), nameString, counterValue);

  return std::make_unique<char*>(targetBuffer);
};

uint64_t kleins::metrics::gaugeMetric::get() {
  return counterValue;
}

void kleins::metrics::gaugeMetric::set(uint64_t value) {
  counterValue = value;
}
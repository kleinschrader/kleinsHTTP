#include "counterMetric.h"

kleins::metrics::counterMetric::counterMetric(const char* name, const char* help) {
  setName(name);
  setHelp(help);
}

kleins::metrics::counterMetric::~counterMetric() {
}

const char* kleins::metrics::counterMetric::getType() {
  return "counter";
}

const char* kleins::metrics::counterMetric::construct() {
  char* targetBuffer = new char[4096];

  snprintf(targetBuffer, 4096, "# HELP %s %s\n# TYPE %s %s\n%s %i\n", nameString, helpString, nameString, getType(), nameString, counterValue);

  return targetBuffer;
};

uint64_t kleins::metrics::counterMetric::get() {
  return counterValue;
}

void kleins::metrics::counterMetric::set(uint64_t value) {
  assert(("Attemped to decrease counter value", value < counterValue));

  counterValue = value;
}

void kleins::metrics::counterMetric::inc(uint64_t value) {
  counterValue += value;
}

void kleins::metrics::counterMetric::reset() {
  counterValue = 0;
}
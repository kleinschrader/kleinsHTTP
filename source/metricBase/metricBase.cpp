#include "metricBase.h"

kleins::metrics::metricBase::metricBase(/* args */) {
}

kleins::metrics::metricBase::~metricBase() {
}

void kleins::metrics::metricBase::setHelp(const char* help) {
  helpString = help;
}

void kleins::metrics::metricBase::setName(const char* name) {
  nameString = name;
}
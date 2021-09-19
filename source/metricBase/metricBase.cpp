#include "metricBase.h"

kleins::metrics::metricBase::metricBase(/* args */) {
}

kleins::metrics::metricBase::~metricBase() {
}

void kleins::metrics::metricBase::setHelp(char* help) {
  helpString = help;
}

void kleins::metrics::metricBase::setName(char* name) {
  nameString = name;
}
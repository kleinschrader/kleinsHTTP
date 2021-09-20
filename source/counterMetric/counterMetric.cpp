#include "counterMetric.h"


kleins::metrics::counterMetric::counterMetric(/* args */)
{
}

kleins::metrics::counterMetric::~counterMetric()
{
}

const char* kleins::metrics::counterMetric::getType() {
    return "counter";
}

std::unique_ptr<char*> kleins::metrics::counterMetric::construct() {
    char* targetBuffer = new char[4096];

    snprintf(targetBuffer,4096,"# HELP %s %s\r\n# TYPE %s %s\r\n%s %i",nameString, helpString, nameString, getType(), nameString, counterValue);

    return std::make_unique<char*>(targetBuffer);
};

uint64_t kleins::metrics::counterMetric::get() {
    return counterValue;
}

void kleins::metrics::counterMetric::set(uint64_t value){
    assert(("Attemped to decrease counter value",value<counterValue));

    counterValue = value;
}

void kleins::metrics::counterMetric::inc(uint64_t value = 1){
    counterValue += value;
}

void kleins::metrics::counterMetric::reset() {
    counterValue = 0;
}
#include "metricsServer.h"

kleins::metrics::metricsServer::metricsServer(/* args */) {
  on(GET, "/metrics", [this](httpParser* parser) {
    std::stringstream ss;

    for (auto x : metrics) {
      const char* metricValue = x->construct();
      ss << metricValue;
      delete[] metricValue;
    }

    parser->respond("200", {"Cache-Control: no-cache"}, ss.str(), "text/plain");
  });
}

kleins::metrics::metricsServer::~metricsServer() {
}

void kleins::metrics::metricsServer::addMetric(metricBase* metric) {
  metrics.push_back(metric);
}
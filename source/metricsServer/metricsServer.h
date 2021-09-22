#ifndef METRICSERVER_H
#define METRICSERVER_H

#ifndef SINGLE_HEADER
#include "../httpServer/httpServer.h"
#include "../metricBase/metricBase.h"

#endif

namespace kleins {

namespace metrics {

class metricsServer : public kleins::httpServer {
private:
  /* data */

  std::list<metricBase*> metrics;

public:
  void addMetric(metricBase* metric);

  metricsServer(/* args */);
  ~metricsServer();
};

} // namespace metrics
} // namespace kleins

#endif
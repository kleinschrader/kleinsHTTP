#ifndef METRICSERVER_H
#define METRICSERVER_H

#ifndef SINGLE_HEADER
#include "../httpServer/httpServer.h"
#endif

namespace kleins {

namespace metrics {

class metricsServer : public httpServer {
private:
  /* data */
public:
  metricsServer(/* args */);
  ~metricsServer();
};

} // namespace metrics
} // namespace kleins

#endif
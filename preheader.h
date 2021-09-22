#define SINGLE_HEADER

namespace kleins {
    namespace metrics {
        class metricBase;
        class counterMetric;
        class histogramMetric;
        class gaugeMetric;
        class metricServer;
    };
    class packet;
    class connectionBase;
    class tcpConnection;
    class httpParser;
    class socketBase;
    class sessionBase;
}

#include "telemetry_comms.h"




int main() {
    TelemetryComms* tc = TelemetryComms::getInstance();

    tc->start(0, 115200);

    while(1) {
    }
}

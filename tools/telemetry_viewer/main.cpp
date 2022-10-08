#include "telemetry_comms.h"

#include <spphost/api.h>
#include <spphost/defs.h>

#include "include/telemetry/spp_property_list.h"


#include <iostream>
#include <chrono>
#include <thread>

int main() {
    TelemetryComms* tc = TelemetryComms::getInstance();

//    tc->start(0, 115200);
    tc->start();

    std::cout << "started" << std::endl;

    SppHostEngine_t *spp = tc->getSpp();

    while(1) {
        std::this_thread::sleep_for(std::chrono::milliseconds(5)); 
    }

    SppStream_t s_status;
    SppStream_t s_telem;
    SppHostStartStream(spp, &spp->clients[0], PROP_status_ID, 100, SPP_STREAM_READ, &s_status);
    SppHostStartStream(spp, &spp->clients[0], PROP_telemetry_ID, 100, SPP_STREAM_READ, &s_telem);

    while(1) {
    }
}

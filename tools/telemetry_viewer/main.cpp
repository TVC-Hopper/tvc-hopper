#include "telemetry_comms.h"

#include <spphost/api.h>
#include <spphost/defs.h>

#include "include/telemetry/spp_property_list.h"


#include <iostream>
#include <chrono>
#include <thread>

int main(int argc, char** argv) {
    TelemetryComms* tc = TelemetryComms::getInstance();


    if (argv[2][0] == 'e') {
        std::cout << "Starting emulated" << std::endl;
        tc->start();
    } else {
        tc->start(argv[1], 115200);
    }

    std::cout << "started" << std::endl;

    SppHostEngine_t *spp = tc->getSpp();

    while(1) {
        std::this_thread::sleep_for(std::chrono::milliseconds(5)); 
    }
}

#ifndef TOOLS_TELEMETRY_COMMS_H
#define TOOLS_TELEMETRY_COMMS_H

#include <spphost/defs.h>
#include <spphost/api.h>
#include <stcp/stcp.h>

#include "tcp_server.h"
#include "rs232.h"


class TelemetryComms {
public:
    static TelemetryComms* getInstance();

    void start(int comport, int baud);

    int getComport();

    SppHostEngine_t* getSpp();
    StcpEngine_t* getStcp();

    SppStream_t* getNextStream();

private:
    void acceptClient();


private:
    TelemetryComms();
    ~TelemetryComms();
    static TelemetryComms* instance_;

    TcpServer server_;
    server_observer_t listener_;

    static const int BUFFER_SIZE = 100;
    const uint8_t address_raw = 0x0;
    SppPropertyDefinition_t prop_defs_buffer[BUFFER_SIZE];
    uint8_t address_buffer[SPP_CONNECTIONS_MAX];
    SppStream_t streams_[SPP_STREAM_MAX_COUNT];
    size_t stream_count_{0};

    int comport_;
    int baud_;

    SppAddress_t address;
    StcpEngine_t stcp_;
    SppHostEngine_t spp_;

};

#endif

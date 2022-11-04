#ifndef TOOLS_TELEMETRY_COMMS_H
#define TOOLS_TELEMETRY_COMMS_H

#include <spphost/defs.h>
#include <spphost/api.h>
#include <stcp/stcp.h>

#include <unordered_map>

#include <mutex>

#include "tcp_server.h"
#include "rs232.h"

struct PropValue {
    std::mutex mx{};
    SppPropertyDefinition_t* def{nullptr};
    uint32_t timestamp;
    std::vector<uint8_t> buffer;
};


class TelemetryComms {
public:
    static TelemetryComms* getInstance();

    void start(const char* comport, int baud);
    void start();

    int getComport();

    SppHostEngine_t* getSpp();
    StcpEngine_t* getStcp();
    TcpServer* getServer();

    bool isEmulated();

    int getEmulatorSock();
    int getViewerSock();

    SppStream_t* getNextStream();
    PropValue* getValue(uint16_t id);

    int acceptClient();


private:
    TelemetryComms();
    ~TelemetryComms();
    static TelemetryComms* instance_;

    void startListener();
    void waitForViewer();

    TcpServer server_;
    server_observer_t listener_;
    std::thread serial_listener_;
    std::thread wait_for_viewer_;

    std::unordered_map<uint16_t, PropValue> prop_values_;


    static const int BUFFER_SIZE = 100;
    const uint8_t address_raw = 0x0;
    SppPropertyDefinition_t prop_defs_buffer[BUFFER_SIZE];
    uint8_t address_buffer[SPP_CONNECTIONS_MAX];
    SppStream_t streams_[SPP_STREAM_MAX_COUNT];
    size_t stream_count_{0};

    bool is_data_src_emulated_;
    int emulator_fd_ = 0;
    int viewer_fd_ = 0;

    int comport_;
    int baud_;

    SppAddress_t address;
    StcpEngine_t stcp_;
    SppHostEngine_t spp_;

};

#endif

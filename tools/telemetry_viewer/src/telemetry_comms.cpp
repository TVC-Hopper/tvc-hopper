#include "telemetry_comms.h"

#include <stdint.h>
#include <iostream>
#include <sstream>
#include <future>
#include <utility>
#include <thread>

#include <mutex>


#include <spphost/defs.h>
#include <spphost/api.h>
#include <stcp/stcp.h>

#include "spp_property_list.h"
#include "rs232.h"

TelemetryComms* TelemetryComms::instance_ = nullptr;
uint8_t default_addr = 0x1;
SppAddress_t default_client{ &default_addr };

static bool areAddressesEqual(SppAddress_t* a, SppAddress_t* b, void* instance_data);
static StcpStatus_t handleStcpPacket(void* bytes, uint16_t len, void* instance_data);
static SPP_STATUS_T sendSPPPacket(uint8_t *bytes, uint16_t len, void* instance_data);
static StcpStatus_t sendStcpPacket(void *bytes, uint16_t len, void* instance_data);
static SPP_STATUS_T onValueResponse(SppAddress_t *client, uint16_t id, void* value, void* instance_data);
static void onStatusResponse(SPP_STATUS_T status, void* instance_data);
static void onStreamResponse(uint32_t timestamp, SppStream_t* stream, void* instance_data);
static void onIncomingMsg(const std::string &clientIP, const uint8_t * msg, size_t size);
static void onClientDisconnected(const std::string &ip, const uint8_t * msg, size_t len);

void onIncomingMsg(const std::string &clientIP, const uint8_t * msg, size_t size) {
    SppHostEngine_t* spp = TelemetryComms::getInstance()->getSpp();
    std::string msg_str = (char*)msg;
    size_t cmd_end = msg_str.find('/');
    std::string cmd = msg_str.substr(0, cmd_end);

    if (cmd == "emdat") {
        size_t start = cmd.length() + 1;
        for (size_t i = start; i < size - 1; ++i) {
            if (msg[i] == STCP_FOOTER && msg[i + 1] == STCP_FOOTER) {
                StcpEngine_t* stcp = TelemetryComms::getInstance()->getStcp();
                SPP_STATUS_T ret = StcpHandleMessage(stcp, (uint8_t*)(msg + start), (i + 2 - start));
                std::cout << "Got msg, ret=" << (int)ret << std::endl;

                start = i + cmd.length() + 3;
                i++; // skip last footer
            }
        }

        return;
    }

    std::string body = msg_str.substr(cmd_end + 1, msg_str.length());
    int id_end = body.find('/');
    std::string id_str = body.substr(0, id_end);
    uint16_t id = std::stoi(id_str);


    if (cmd == "str") {
        uint16_t period = std::stoi(body.substr(id_end + 1, body.length()));
        std::cout << "stream request " << id << " " << period << std::endl;
        SppStream_t* s = TelemetryComms::getInstance()->getNextStream();
        SppHostStartStream(spp, &default_client, id, period, SPP_STREAM_READ, s);
    } else if (cmd == "get") {
        std::cout << "get request " << id << std::endl;
        std::cout << (int)SppHostGetValue(spp, &default_client, id) << std::endl;
    } else if (cmd =="val") {
        std::cout << "value request " << id << std::endl;
        PropValue* value = TelemetryComms::getInstance()->getValue(id);

        uint16_t type = value->def->type;

        std::ostringstream oss;
        oss << id << "/";

        std::unique_lock<std::mutex> ul(value->mx);
        if (type == SPP_PROP_T_BOOL) {
            oss << *((bool*)&value->buffer[0]);
        } else if (type == SPP_PROP_T_U32) {
            oss << *((uint32_t*)&value->buffer[0]);
        } else if (type == SPP_PROP_T_I32) {
            oss << *((int32_t*)&value->buffer[0]);
        } else if (type == SPP_PROP_T_I16) {
            oss << *((uint16_t*)&value->buffer[0]);
        } else if (type == SPP_PROP_T_I16) {
            oss << *((int16_t*)&value->buffer[0]);
        } else if (type == SPP_PROP_T_ARR) {
            for (auto &i : value->buffer) {
                oss << i;
            }
            std::cout << "arr: " << oss.str() << std::endl;
        }
        ul.unlock();
        
        std::string response = "val/" + oss.str();
        TelemetryComms* tc = TelemetryComms::getInstance();
        tc->getServer()->sendToClient(tc->getViewerSock(), (uint8_t*)response.c_str(), response.length());
    } else if (cmd == "set") {
        std::string value_str = body.substr(id_end + 1, body.length());
        std::cout << "set request " << id << " " << value_str << std::endl;
        SppPropertyDefinition_t *def;
        SppHostGetDefinition(spp, &default_client, id, &def);

        uint8_t value[def->size];

        if (def->type == SPP_PROP_T_BOOL) {
            value[0] = std::stoi(value_str);
        } else if (def->type == SPP_PROP_T_U32) {
            uint32_t value_tmp = std::stoul(value_str);
            memcpy(value, &value_tmp, sizeof(value_tmp));
        } else if (def->type == SPP_PROP_T_I32) {
            int32_t value_tmp = std::stoi(value_str);
            memcpy(value, &value_tmp, sizeof(value_tmp));
        }
            
        
        SppHostSetValue(spp, &default_client, id, value);
    }
}

void onClientDisconnected(const std::string &ip, const uint8_t* msg, size_t len) {
    std::cout << "Client: " << ip << " disconnected. Reason: " << (char*)msg << "\n";
}
int TelemetryComms::getEmulatorSock() {
    return emulator_fd_;
}

int TelemetryComms::getViewerSock() {
    return viewer_fd_;
}

bool TelemetryComms::isEmulated() {
    return is_data_src_emulated_;
}

TcpServer* TelemetryComms::getServer() {
    return &server_;
}

SppStream_t* TelemetryComms::getNextStream() {
    return &streams_[stream_count_++];
}

PropValue* TelemetryComms::getValue(uint16_t id) {
    if (prop_values_.find(id) == prop_values_.end()) {
        SppHostGetDefinition(getSpp(), &default_client, id, &prop_values_[id].def);
        prop_values_[id].buffer = std::vector<uint8_t>(prop_values_[id].def->size, 0);
    }
    return &prop_values_[id];
}

int TelemetryComms::acceptClient() {
    try {
        std::cout << "waiting for incoming client...\n";
        Client* c = server_.acceptClient(0);
        std::cout << "accepted new client with IP: " << c->getIp()<< "\n" <<
                  "== updated list of accepted clients ==" << "\n";
        server_.printClients();

        return c->getSock();
    } catch (const std::runtime_error &error) {
        std::cout << "Accepting client failed: " << error.what() << "\n";
        return -1;
    }
}

SPP_STATUS_T onValueResponse(SppAddress_t *client, uint16_t id, void* value, void* instance_data) {

    PropValue *pv = TelemetryComms::getInstance()->getValue(id);
    switch(id) {
        case PROP_start_ID:
        {
            std::cout << *((bool*)value) << std::endl;
            std::lock_guard<std::mutex> lg(pv->mx);
            memcpy(&pv->buffer[0], (uint8_t*)value, pv->def->size);
            break;
        }
        case PROP_stop_ID:
        {
            std::cout << *((bool*)value) << std::endl;
            std::lock_guard<std::mutex> lg(pv->mx);
            memcpy(&pv->buffer[0], (uint8_t*)value, pv->def->size);
            break;
        }
        case PROP_status_ID:
        {
            std::cout << *((uint32_t*)value) << std::endl;
            std::lock_guard<std::mutex> lg(pv->mx);
            memcpy(&pv->buffer[0], (uint8_t*)value, pv->def->size);
            break;
        }
        case PROP_telemetry_ID:
        {
            uint8_t* data = (uint8_t*)value;
            for (int i = 0; i < 10; ++i) {
                std::cout << data[i] << " ";
            }
            std::cout << std::endl;
            std::lock_guard<std::mutex> lg(pv->mx);
            memcpy(&pv->buffer[0], (uint8_t*)value, pv->def->size);
            break;
        }
        default:
        {
            return SPP_STATUS_UNKNOWN_PROPERTY;
        }
    }

    return SPP_STATUS_OK;
}

void onStatusResponse(SPP_STATUS_T status, void* instance_data) {
    // STUB
}

void onStreamResponse(uint32_t timestamp, SppStream_t* stream, void* instance_data) {
    std::cout << timestamp << " ";
    onValueResponse(nullptr, stream->def->id, stream->value, instance_data);
}

TelemetryComms* TelemetryComms::getInstance() {
    if (!instance_) {
        instance_ = new TelemetryComms();
    }

    return instance_;
}


TelemetryComms::TelemetryComms() {

    address = SppAddress_t{ (void*)&address_raw };


    SppHostEngineInitParams_t ip;
    ip.callbacks = SppHostCallbacks_t{
        areAddressesEqual,
        sendSPPPacket,
        onValueResponse,
        onStatusResponse,
        onStreamResponse
    };
    ip.address_buffer = address_buffer;
    ip.address_length = sizeof(uint8_t);
    ip.defs_buffer = prop_defs_buffer;
    ip.defs_buffer_size = BUFFER_SIZE;
    ip.host_address = address;
    ip.instance_data = nullptr;

    SppHostEngineInit(&ip, &spp_);

    stcp_.callbacks = StcpCallbacks_t{
        .Send = sendStcpPacket,
        .HandleMessage = handleStcpPacket
    };

    stcp_.instance_data = nullptr;

    pipe_ret_t start_ret = server_.start(65123);
    if (start_ret.isSuccessful()) {
        std::cout << "Server setup succeeded\n";
    } else {
        std::cout << "ERROR: server setup failed: " << start_ret.message() << "\n";
    }

    listener_.incomingPacketHandler = onIncomingMsg;
    listener_.disconnectionHandler = onClientDisconnected;
    listener_.wantedIP = "127.0.0.1";

    server_.subscribe(listener_);
}


void TelemetryComms::start() {
    // start with emulator connection
    is_data_src_emulated_ = true;

    // accept emulator
    emulator_fd_ = acceptClient();

    // std::promise<int> viewer_fd_promise;
    // std::future<int> fd_future = viewer_fd_promise.get_future();

    while(1) {
        viewer_fd_ = TelemetryComms::getInstance()->acceptClient();
    }
}

void TelemetryComms::start(int comport, int baud) {
    // start with hardware connection
    is_data_src_emulated_ = false;
    comport_ = comport;
    baud_ = baud;

    char mode[] = {'8', 'N', '1', 0};

    if (RS232_OpenComport(comport, baud, mode, 0)) {
        std::cout << "ERROR: failed to open com port " << comport << std::endl;
    }

    acceptClient();

    // TODO: start listener for serial communications
}

TelemetryComms::~TelemetryComms() {
    if (!is_data_src_emulated_) {
        RS232_CloseComport(comport_);
    }
    delete instance_;
}

int TelemetryComms::getComport() {
    return comport_;
}

SppHostEngine_t* TelemetryComms::getSpp() {
    return &spp_;
}

StcpEngine_t* TelemetryComms::getStcp() {
    return &stcp_;
}

StcpStatus_t handleStcpPacket(void* bytes, uint16_t len, void* instance_data) {
    SppHostEngine_t* spp = TelemetryComms::getInstance()->getSpp();
    SPP_STATUS_T ret = SppHostProcessMessage(spp, (uint8_t*)bytes, len);

    if (ret != SPP_STATUS_OK) {
        //for (int i = 0; i < len; ++i) printf("\\x%.2x", ((uint8_t*)bytes)[i]);
        //std::cout << std::endl;
        std::cout << (int)ret << std::endl;
        return STCP_STATUS_UNDEFINED_ERROR;
    } else {
        return STCP_STATUS_SUCCESS;
    }
}

StcpStatus_t sendStcpPacket(void *bytes, uint16_t len, void* instance_data) {
    TelemetryComms* tc = TelemetryComms::getInstance();

    if (tc->isEmulated()) {
       tc->getServer()->sendToClient(tc->getEmulatorSock(), (uint8_t*)bytes, len);
    } else {
        int com = tc->getComport();
        RS232_SendBuf(com, (uint8_t*)bytes, len);
    }

    return STCP_STATUS_SUCCESS;
}

SPP_STATUS_T sendSPPPacket(uint8_t *bytes, uint16_t len, void* instance_data) {
    StcpEngine_t* stcp = TelemetryComms::getInstance()->getStcp();
    StcpWrite(stcp, bytes, len);
    return SPP_STATUS_OK;
}

bool areAddressesEqual(SppAddress_t* a, SppAddress_t* b, void* instance_data) {
    if (!a || !b) {
        return false;
    }
    if (!a->address || !b->address) {
        return false;
    }
    uint8_t a_u8 = (*((uint8_t*)a->address));
    uint8_t b_u8 = (*((uint8_t*)b->address));

    return a_u8 == b_u8;
}

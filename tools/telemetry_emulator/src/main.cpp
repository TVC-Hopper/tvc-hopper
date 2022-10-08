
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <chrono>
#include <thread>
#include <sstream>

#include <iostream>
#include <csignal>

#include <stcp/stcp.h>
#include <sppclient/api.h>
#include <sppclient/defs.h>

extern "C" {
#include "spp_property_list.h"
}

#include "tcp_client.h"

static SPP_STATUS_T StcpSendPacket(void* buffer, uint16_t len, void* instance_data);
static SPP_STATUS_T SppSendPacket(uint8_t* bytes, uint16_t len, void* instance_data);
static StcpStatus_t HandleStcpPacket(void* buffer, uint16_t length, void* instance_data);
static SPP_STATUS_T SetValue(uint16_t id, void* value, void* instance_data);
static SPP_STATUS_T GetValue(uint16_t id, void* value, void* instance_data);
static void engineSetup();


static void onIncomingMsg(const uint8_t * msg, size_t size);
static void onDisconnection(const pipe_ret_t & ret);
static void sig_exit(int s);

SppClientEngine_t spp;
StcpEngine_t stcp;
TcpClient tcp;

static uint8_t broadcast_addr_raw = 0x00;
static uint8_t addr_raw = 0x01;
static uint8_t property_list_id = 0x00;
static uint8_t client_address_buffer[3];

static SppAddress_t broadcast_addr = { &broadcast_addr_raw };
static SppAddress_t address = { &addr_raw };


static bool prop_start = false;
static bool prop_stop = false;
static uint32_t prop_status = 0;
static uint8_t prop_telem[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};


int main() {

    //register to SIGINT to close client when user press ctrl+c
	signal(SIGINT, sig_exit);

    // configure and register observer
    client_observer_t observer;
	observer.wantedIP = "127.0.0.1";
	observer.incomingPacketHandler = onIncomingMsg;
	observer.disconnectionHandler = onDisconnection;
	tcp.subscribe(observer);

	// connect client to an open server
	bool connected = false;
    while (!connected) {
        pipe_ret_t connectRet = tcp.connectTo("127.0.0.1", 65123);
        connected = connectRet.isSuccessful();
        if (connected) {
            std::cout << "Client connected successfully\n";
        } else {
            std::cout << "Client failed to connect: " << connectRet.message() << "\n"
                      << "Make sure the server is open and listening\n\n";
            sleep(2);
            std::cout << "Retrying to connect...\n";
        }
	};

    engineSetup();

    using namespace std::chrono;
    uint32_t last = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
	while(1)
	{
        uint32_t now = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
        SppProcessStreams(&spp, now, now - last);
        last = now;
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
	}

	return 0;
}

// observer callback. will be called for every new message received by the server
static void onIncomingMsg(const uint8_t * msg, size_t size) {
    std::cout << "got response" << std::endl;
    for (int i = 0; i < size; ++i) printf("\\x%.2x", msg[i]);
    StcpHandleMessage(&stcp, (uint8_t*)msg, size);
}

// observer callback. will be called when server disconnects
static void onDisconnection(const pipe_ret_t & ret) {
	std::cout << "Server disconnected: " << ret.message() << "\n";
}


// on sig_exit, close client
static void sig_exit(int s)
{
	std::cout << "Closing client...\n";
	pipe_ret_t finishRet = tcp.close();
	if (finishRet.isSuccessful()) {
		std::cout << "Client closed.\n";
	} else {
		std::cout << "Failed to close client.\n";
	}
	exit(0);
}

static void engineSetup() {
    SppClientEngineInitParams_t spp_ip;
    spp_ip.broadcast_address = broadcast_addr;
    spp_ip.client_address = address;
    spp_ip.host_address_buffer = &client_address_buffer;
    spp_ip.property_count = SPP_PROP_COUNT;
    spp_ip.property_list_id = property_list_id;
    spp_ip.address_length = sizeof(uint8_t);
    spp_ip.properties = SppGetPropertyList();

    SppClientCallbacks_t spp_callbacks = {
        .Send = SppSendPacket,
        .SetValue = SetValue,
        .GetValue = GetValue,
    };

    spp_ip.callbacks = spp_callbacks;
    spp_ip.instance_data = NULL;

    SppClientEngineInit(&spp_ip, &spp);

    StcpCallbacks_t stcp_callbacks = {
        .Send = StcpSendPacket,
        .HandleMessage = HandleStcpPacket,
    };


    stcp.callbacks = stcp_callbacks;
    stcp.instance_data = NULL;

    SppClientStart(&spp);
}

static SPP_STATUS_T StcpSendPacket(void* buffer, uint16_t len, void* instance_data) {
    std::ostringstream oss;

    oss << "emdat/";

    uint8_t msg[len + 6];

    memcpy(msg, oss.str().c_str(), oss.str().length());
    memcpy(msg + 6, (uint8_t*)buffer, len);

    pipe_ret_t sendRet = tcp.sendMsg(msg, len + 6);

    if (!sendRet.isSuccessful()) {
        std::cout << "Failed to send message: " << sendRet.message() << "\n";
    } else {
        for (int i = 0; i < len + 6; ++i) printf("\\x%.2x", msg[i]);
        std::cout << std::endl << (len + 6) << std::endl;
        std::cout << "message was sent successfuly\n";
    }

    return SPP_STATUS_OK;
}


static SPP_STATUS_T SetValue(uint16_t id, void* value, void* instance_data) {
    switch(id) {
        case PROP_start_ID:
        {
            memcpy(&prop_start, value, sizeof(prop_start));
            break;
        }
        case PROP_stop_ID:
        {
            memcpy(&prop_stop, value, sizeof(prop_stop));
            break;
        }
        case PROP_status_ID:
        {
            memcpy(&prop_status, value, sizeof(prop_status));
            break;
        }
        case PROP_telemetry_ID:
        {
            memcpy(prop_telem, value, sizeof(prop_telem));
            break;
        }
        default:
        {
            return SPP_STATUS_UNKNOWN_PROPERTY;
        }
    }

    return SPP_STATUS_OK;
}

static SPP_STATUS_T GetValue(uint16_t id, void* value, void* instance_data) {
    std::cout << "getting " << id << std::endl;
    switch(id) {
        case PROP_start_ID:
        {
            memcpy(value, &prop_start, sizeof(bool));
            break;
        }
        case PROP_stop_ID:
        {
            memcpy(value, &prop_stop, sizeof(bool));
            break;
        }
        case PROP_status_ID:
        {
            memcpy(value, &prop_status, sizeof(uint32_t));
            break;
        }
        case PROP_telemetry_ID:
        {
            memcpy(value, prop_telem, 10);
            break;
        }
        default:
        {
            return SPP_STATUS_UNKNOWN_PROPERTY;
        }
    }

    return SPP_STATUS_OK;
}

static StcpStatus_t HandleStcpPacket(void* buffer, uint16_t length, void* instance_data) {
    SppClientProcessMessage(&spp, (uint8_t*)buffer, length);

    return STCP_STATUS_SUCCESS;
}

static SPP_STATUS_T SppSendPacket(uint8_t* bytes, uint16_t len, void* instance_data) {
   StcpWrite(&stcp, bytes, len);

   return SPP_STATUS_OK;
}


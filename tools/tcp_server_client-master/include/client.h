#pragma once

#include <string>
#include <thread>
#include <functional>
#include <mutex>
#include <atomic>

#include "pipe_ret_t.h"
#include "client_event.h"
#include "file_descriptor.h"


class Client {

    using client_event_handler_t = std::function<void(const Client&, ClientEvent, const uint8_t*, size_t)>;

private:
    FileDescriptor _sockfd;
    std::string _ip = "";
    std::atomic<bool> _isConnected;
    std::thread * _receiveThread = nullptr;
    client_event_handler_t _eventHandlerCallback;

    void setConnected(bool flag) { _isConnected = flag; }

    void receiveTask();

    void terminateReceiveThread();

public:
    Client(int);

    bool operator ==(const Client & other) const ;

    void setIp(const std::string & ip) { _ip = ip; }
    std::string getIp() const { return _ip; }

    int getSock() const { return _sockfd.get(); }

    void setEventsHandler(const client_event_handler_t & eventHandler) { _eventHandlerCallback = eventHandler; }
    void publishEvent(ClientEvent clientEvent, const uint8_t * msg, size_t size);


    bool isConnected() const { return _isConnected; }

    void startListen();

    void send(const uint8_t * msg, size_t msgSize) const;

    void close();

    void print() const;

};



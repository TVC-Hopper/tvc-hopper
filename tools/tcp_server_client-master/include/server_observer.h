#pragma once

#include <string>
#include <functional>
#include "client.h"

struct server_observer_t {
	std::string wantedIP = "";
	std::function<void(const std::string &clientIP, const uint8_t * msg, size_t size)> incomingPacketHandler;
	std::function<void(const std::string &ip, const uint8_t * msg, size_t size)> disconnectionHandler;
};


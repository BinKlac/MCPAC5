#pragma once

#include "MINT.h"

enum NetworkFilterResult {
	PassThrough,
	ReturnError,
	ReturnFakeData
};

class NetworkFilter {
public:
	static NetworkFilterResult ProcessDisconnect(SOCKET s);
	static NetworkFilterResult ProcessConnect(SOCKET s, const sockaddr* name);
	static NetworkFilterResult ProcessSend(SOCKET s, const char* buf, size_t len);
	static NetworkFilterResult ProcessRecv(SOCKET s, const char* buf, size_t len);
};


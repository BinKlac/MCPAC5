#pragma once

#include "HookAPIDefines.h"
#include "NetworkFilter.h"

typedef struct _HookAddressList {
	pfnconnect connect = nullptr;
	pfnsend send = nullptr;
	pfnrecv recv = nullptr;
	pfnclosesocket closesocket = nullptr;
	pfnWSAConnect WSAConnect = nullptr;
	pfnWSASend WSASend = nullptr;
	pfnWSARecv WSARecv = nullptr;
}HookAddressList, * PHookAddressList;

class APIHooker {
private:
	static bool IsHookInitized;
	static HookAddressList mHookAddressList;

public:
	static const PHookAddressList getHookAddressList(void);

private:
	static bool InitizeJVMHook(void);
	static bool InitizeAPIHook(void);

public:
	static bool InitizeHook(void);
	static bool ClearAPIHook(void);
};


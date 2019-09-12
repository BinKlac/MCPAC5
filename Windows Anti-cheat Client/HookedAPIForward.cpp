#include "HookAPIDefines.h"
#include "NetworkFilter.h"
#include "APIHooker.h"

int PASCAL Hooked_send(SOCKET s, const char* buf, int len, int flags) {
	switch (NetworkFilter::ProcessSend(s, buf, len)) {
		case NetworkFilterResult::ReturnError:
			WSASetLastError(WSAEINTR);
			return SOCKET_ERROR;
		case NetworkFilterResult::ReturnFakeData:
			return len;
		default:
			return APIHooker::getHookAddressList()->send(s, buf, len, flags);
	}
}

int PASCAL Hooked_recv(SOCKET s, char* buf, int len, int flags) {
	auto OrigianalRet = APIHooker::getHookAddressList()->recv(s, buf, len, flags);

	switch (NetworkFilter::ProcessRecv(s, buf, len)) {
		case NetworkFilterResult::ReturnError:
			WSASetLastError(WSAEINTR);
			return SOCKET_ERROR;
		case NetworkFilterResult::ReturnFakeData:
			ZeroMemory(buf, len);
			return 0;
		default:
			return OrigianalRet;
	}
}

int  PASCAL Hooked_connect(SOCKET s, sockaddr* name, int namelen) {
	switch (NetworkFilter::ProcessConnect(s, name)) {
		case NetworkFilterResult::ReturnError:
			WSASetLastError(WSAEINTR);
			return SOCKET_ERROR;
		default:
			return APIHooker::getHookAddressList()->connect(s, name, namelen);
	}
}

int PASCAL Hooked_closesocket(SOCKET s) {
	switch (NetworkFilter::ProcessDisconnect(s)) {
		default:
			return APIHooker::getHookAddressList()->closesocket(s);
	}
}

int WSAAPI Hooked_WSASend(SOCKET s, LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesSent, DWORD dwFlags, LPWSAOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine) {
	DWORD NumberOfDataSent = 0;
	PCHAR BufferOfDataSent = nullptr;
	NetworkFilterResult CriticalResult = NetworkFilterResult::PassThrough;
	BOOLEAN NeedFree = false;

	if (dwBufferCount == 1) {
		NumberOfDataSent = lpBuffers[0].len;
		BufferOfDataSent = lpBuffers[0].buf;
	} else {
		PCHAR BufferOfDataSent = reinterpret_cast<PCHAR>(malloc(NumberOfDataSent));

		if (BufferOfDataSent != nullptr) {
			NeedFree = true;
			NumberOfDataSent = 0;

			for (DWORD i = 0; i < dwBufferCount; i++) {
				memcpy(BufferOfDataSent + NumberOfDataSent, lpBuffers[i].buf, lpBuffers[i].len);
				NumberOfDataSent += lpBuffers[i].len;
			}
		}
	}

	if (BufferOfDataSent != nullptr) {
		CriticalResult = NetworkFilter::ProcessSend(s, BufferOfDataSent, NumberOfDataSent);
	}

	if (NeedFree) {
		free(BufferOfDataSent);
	}

	switch (CriticalResult) {
		case NetworkFilterResult::ReturnError:
			WSASetLastError(WSAEINTR);
			return SOCKET_ERROR;
		case NetworkFilterResult::ReturnFakeData:
			*lpNumberOfBytesSent = NumberOfDataSent;
			return 0;
		default:
			return APIHooker::getHookAddressList()->WSASend(s, lpBuffers, dwBufferCount, lpNumberOfBytesSent, dwFlags, lpOverlapped, lpCompletionRoutine);
	}
}

int WSAAPI Hooked_WSARecv(SOCKET s, LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesRecvd, LPDWORD lpFlags, LPWSAOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine) {
	auto OrigianalRet = APIHooker::getHookAddressList()->WSARecv(s, lpBuffers, dwBufferCount, lpNumberOfBytesRecvd, lpFlags, lpOverlapped, lpCompletionRoutine);

	DWORD NumberOfDataReceived = *lpNumberOfBytesRecvd;
	PCHAR BufferOfDataReceived = nullptr;
	NetworkFilterResult CriticalResult = NetworkFilterResult::PassThrough;
	BOOLEAN NeedFree = false;

	if (dwBufferCount == 1) {
		BufferOfDataReceived = lpBuffers[0].buf;
	} else {
		BufferOfDataReceived = reinterpret_cast<PCHAR>(malloc(NumberOfDataReceived));

		if (BufferOfDataReceived != nullptr) {
			NeedFree = true;

			for (DWORD i = 0; i < dwBufferCount; i++) {
				if (lpBuffers[i].len <= NumberOfDataReceived) {
					memcpy(BufferOfDataReceived + ((*lpNumberOfBytesRecvd) - NumberOfDataReceived), lpBuffers[i].buf, NumberOfDataReceived);
					NumberOfDataReceived = 0;
				} else {
					memcpy(BufferOfDataReceived + ((*lpNumberOfBytesRecvd) - NumberOfDataReceived), lpBuffers[i].buf, lpBuffers[i].len);
					NumberOfDataReceived = NumberOfDataReceived - lpBuffers[i].len;
				}
			}

			NumberOfDataReceived = *lpNumberOfBytesRecvd;
		}
	}

	if (BufferOfDataReceived != nullptr) {
		CriticalResult = NetworkFilter::ProcessRecv(s, BufferOfDataReceived, NumberOfDataReceived);
	}

	if (NeedFree) {
		free(BufferOfDataReceived);
	}

	switch (CriticalResult) {
		case NetworkFilterResult::ReturnError:
			WSASetLastError(WSAEINTR);
			return SOCKET_ERROR;
		case NetworkFilterResult::ReturnFakeData:
			for (DWORD i = 0; i < dwBufferCount; i++) {
				ZeroMemory(lpBuffers[i].buf, lpBuffers[i].len);
			}

			*lpNumberOfBytesRecvd = 1;
			return 0;
		default:
			return OrigianalRet;
	}
}

int WSAAPI Hooked_WSAConnect(SOCKET s, const sockaddr* name, int namelen, LPWSABUF lpCallerData, LPWSABUF lpCalleeData, LPQOS lpSQOS, LPQOS lpGQOS) {
	switch (NetworkFilter::ProcessConnect(s, name)) {
		case NetworkFilterResult::ReturnError:
			WSASetLastError(WSAEINTR);
			return SOCKET_ERROR;
		default:
			return APIHooker::getHookAddressList()->WSAConnect(s, name, namelen, lpCallerData, lpCalleeData, lpSQOS, lpGQOS);
	}
}
#pragma once

#include <winsock2.h>
#include "MINT.h"

//jvm!attachapi hook
typedef BOOL(*PFNEmptyFunctiom)();

//for windows socket hook
typedef int(PASCAL* pfnconnect)(SOCKET s, sockaddr* name, int namelen);
typedef int(PASCAL* pfnsend)(SOCKET s, const char* buf, int len, int flags);
typedef int(PASCAL* pfnrecv)(SOCKET s, char* buf, int len, int flags);
typedef int(PASCAL* pfnclosesocket)(SOCKET s);
typedef int(WSAAPI* pfnWSAConnect)(SOCKET s, const sockaddr* name, int namelen, LPWSABUF lpCallerData, LPWSABUF lpCalleeData, LPQOS lpSQOS, LPQOS lpGQOS);
typedef int(WSAAPI* pfnWSASend)(SOCKET s, LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesSent, DWORD dwFlags, LPWSAOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);
typedef int(WSAAPI* pfnWSARecv)(SOCKET s, LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesRecvd, LPDWORD lpFlags, LPWSAOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);

int WSAAPI Hooked_WSASend(SOCKET s, LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesSent, DWORD dwFlags, LPWSAOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);
int WSAAPI Hooked_WSAConnect(SOCKET s, const sockaddr* name, int namelen, LPWSABUF lpCallerData, LPWSABUF lpCalleeData, LPQOS lpSQOS, LPQOS lpGQOS);
int WSAAPI Hooked_WSARecv(SOCKET s, LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesRecvd, LPDWORD lpFlags, LPWSAOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);
int PASCAL Hooked_send(SOCKET s, const char* buf, int len, int flags);
int PASCAL Hooked_recv(SOCKET s, char* buf, int len, int flags);
int PASCAL Hooked_connect(SOCKET s, sockaddr* name, int namelen);
int PASCAL Hooked_closesocket(SOCKET s);

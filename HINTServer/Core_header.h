#pragma once

// SDKDDKVer.h를 포함하면 최고 수준의 가용성을 가진 Windows 플랫폼이 정의됩니다.

// 이전 Windows 플랫폼에 대해 응용 프로그램을 빌드하려는 경우에는 SDKDDKVer.h를 포함하기 전에
// WinSDKVer.h를 포함하고 _WIN32_WINNT 매크로를 지원하려는 플랫폼으로 설정하십시오.

#include <SDKDDKVer.h>


// SDKDDKVer.h를 포함하면 최고 수준의 가용성을 가진 Windows 플랫폼이 정의됩니다.

// 이전 Windows 플랫폼에 대해 응용 프로그램을 빌드하려는 경우에는 SDKDDKVer.h를 포함하기 전에
// WinSDKVer.h를 포함하고 _WIN32_WINNT 매크로를 지원하려는 플랫폼으로 설정하십시오.


#pragma comment(lib, "Log.lib")
#pragma comment(lib, "Database.lib")

#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"mswsock.lib")  // AcceptEx()

#pragma warning(disable:4996)		// strncpy :: C4996 Error

#include <SDKDDKVer.h>

#include <iostream>
#include <tchar.h>

#include <winsock2.h>
#include <ws2tcpip.h>
#include <mswsock.h>
#include <windows.h>

#include <wchar.h>
#include <string>
#include <string.h>
#include <utility>

#include <vector>
#include <assert.h>

enum IO_TYPE
{
	IO_ACCEPT,
	IO_READ,
	IO_WRITE
};

typedef struct _OVERLAPPED_EX
{
	OVERLAPPED	Overlapped;
	IO_TYPE		IoType;
	VOID		*Object;
} OVERLAPPED_EX;

#define MAXBUFFERLENGTH		4096
#ifdef _SERVER_SIDE
#define MAXQUEUELENGTH	50
#else
#define MAXQUEUELENGTH	500
#endif

//----------------------------------------------------------------------//
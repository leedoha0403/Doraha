#pragma once

// SDKDDKVer.h�� �����ϸ� �ְ� ������ ���뼺�� ���� Windows �÷����� ���ǵ˴ϴ�.

// ���� Windows �÷����� ���� ���� ���α׷��� �����Ϸ��� ��쿡�� SDKDDKVer.h�� �����ϱ� ����
// WinSDKVer.h�� �����ϰ� _WIN32_WINNT ��ũ�θ� �����Ϸ��� �÷������� �����Ͻʽÿ�.

#include <SDKDDKVer.h>
#pragma once

// SDKDDKVer.h�� �����ϸ� �ְ� ������ ���뼺�� ���� Windows �÷����� ���ǵ˴ϴ�.

// ���� Windows �÷����� ���� ���� ���α׷��� �����Ϸ��� ��쿡�� SDKDDKVer.h�� �����ϱ� ����
// WinSDKVer.h�� �����ϰ� _WIN32_WINNT ��ũ�θ� �����Ϸ��� �÷������� �����Ͻʽÿ�.

#include <SDKDDKVer.h>


// SDKDDKVer.h�� �����ϸ� �ְ� ������ ���뼺�� ���� Windows �÷����� ���ǵ˴ϴ�.

// ���� Windows �÷����� ���� ���� ���α׷��� �����Ϸ��� ��쿡�� SDKDDKVer.h�� �����ϱ� ����
// WinSDKVer.h�� �����ϰ� _WIN32_WINNT ��ũ�θ� �����Ϸ��� �÷������� �����Ͻʽÿ�.


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
//----------------------------------------------------------------------//


#pragma comment(lib, "Log.lib")
#pragma comment(lib, "Database.lib")
#pragma comment(lib, "Core.lib")

#pragma warning(disable:4996)		// strncpy :: C4996 Error

////////////////////////////////////////////////////////
// Log
//#include "stdafx.h"
#include "Log.h"

//Databas
//#include "targetver.h"
//#include "DATABASE_CODE.h"
#include "DataBase.h"

// Core
#include "stdafx.h"
#include "CCriticalSection.h"
#include "CMultiThreadSync.h"
#include "CMemoryPool.h"
#include "CStream.h"
#include "CCircularQueue.h"
#include "CNetworkSession.h"
#include "CPacketSession.h"
#include "CIocp.h"


//#include "../../Core/Core/EventSelect.h"
//#include "../../Core/Core/ClientSession.h"

// TCP Protocol
//#include "../Packet/TcpProtocol.h"
//
//#include "../Packet/TCP_Structure.h"
//#include "../Packet/TCP_WritePacket.h"
//#include "../Packet/TCP_ReadPacket.h"

////////////////////////////////////////////////////////
#include <list>

#define DEFAULT_PORT			9600
#define MAX_SENSOR				12

#define WRITE_FAIL_U_PACKET(PROTOCOL, ERROR_CODE) \
	connectedUser->WritePacket(PROTOCOL, \
	WriteBuffer, \
	WRITE_##PROTOCOL(WriteBuffer, \
	ERROR_CODE));\
	MULTI_LOG3() << "# Write packet : "; MULTI_LOG3() << #PROTOCOL; //MULTI_LOG3() << ERROR_CODE)//;

#define WRITE_FAIL_U_PACKET2(OBJECT, PROTOCOL, ERROR_CODE) \
	OBJECT->WritePacket(PROTOCOL, \
	WriteBuffer, \
	WRITE_##PROTOCOL(WriteBuffer, \
	ERROR_CODE));\
	MULTI_LOG3() << "# Write packet : "; MULTI_LOG3() << #PROTOCOL; //MULTI_LOG3() << ERROR_CODE)//;

#define READ_PACKET(PROTOCOL)\
	S_##PROTOCOL Data;\
	READ_##PROTOCOL(packet, Data);

typedef enum SensorIocpStatus
{
	SENSOR_STATUS_BEGIN,
	
	SENSOR_WAIT_TO_SETUP,	

	SENSOR_ARDUINO_SETUP_SUCC,
	SENSOR_MOBLIE_SETUP_SUCC,

	SENSOR_ALL_SETUP_SUCC,

	SENSOR_STATUS_END,

	SENSOR_ERROR				= 100,
	SENSOR_ARDUINO_ERROR,
	SENSOR_MOBILE_ERROR,
}SENSOR_STATUS;
#define		MAX_MOBILE_SET		1			// �ƽøذ�
#define		MAX_ARDUINO_SET		4 
 
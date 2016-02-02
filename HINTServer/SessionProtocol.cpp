#include "stdafx.h"

#include "CCriticalSection.h"
#include "CMultiThreadSync.h"
#include "CCircularQueue.h"
#include "CNetworkSession.h"
#include "CPacketSession.h"
#include "CIocp.h"

#include "ConnectSession.h"
#include "ConnectSessionManager.h"

#include "HINTIocp.h"

#include "../Packet/gHeader.h"
#include "../Packet/TcpProtocol.h"
#include "../Packet/TCP_Structure.h"
#include "../Packet/TCP_ReadPacket.h"
#include "../Packet/TCP_WritePacket.h"

VOID CHINTIocp::onPT_SESSION_VALUE( CConnectedSession *ConnectSession, BYTE* packet )
{
	BYTE	WriteBuffer[MAXBUFFERLENGTH]	= {0,};

	READ_PACKET(PT_SENSOR_DATA);

	Data.Type;

}
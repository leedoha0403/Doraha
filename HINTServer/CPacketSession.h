#pragma once
#include "CNetworkSession.h"

//typedef struct _READ_PACKET_INFO
//{
//	CHAR	RemoteAddress[14];
//	USHORT	RemotePort;
//	DWORD	PacketNumber;
//} READ_PACKET_INFO;


extern class CPacketSession :	public CNetworkSession
{
public:
	CPacketSession(void);
	virtual ~CPacketSession(void);

private:
	BYTE									mPacketBuffer[MAXBUFFERLENGTH * 3];
	INT									mRemainLength;
	DWORD								mCurrentPacketNumber;
	
	DWORD								mLastReadPacketNumber;

	CCircularQueue						WriteQueue;
	
public:
	BOOL	Begin(VOID);
	BOOL	End(VOID);
	BOOL	InitializePacket(VOID);

	BOOL	ReadPacketForIocp(DWORD readLength);
	BOOL	ReadPacketForEventSelect(VOID);

	BOOL	WritePacket(DWORD protocol, const BYTE *packet, DWORD packetLength);
	BOOL	WriteComplete(VOID);

	BOOL	GetPacket(DWORD &protocol, BYTE *packet, DWORD &packetLength);
	BOOL	GetPacket(LPSTR remoteAddress, USHORT remotePort, DWORD &protocol, BYTE *packet, DWORD &packetLength);
};


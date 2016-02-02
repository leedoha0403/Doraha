#include "stdafx.h"

#include "CCriticalSection.h"
#include "CMultiThreadSync.h"
#include "CCircularQueue.h"
#include "CNetworkSession.h"
//#include "CPacketSession.h"

#include "ConnectSession.h"


////////////////////////////////////////////////////////////////////////////////////////


CPacketSessions::CPacketSessions(VOID)
{
	memset(mPacketBuffer, 0, sizeof(mPacketBuffer));

	mRemainLength			= 0;

	mLastReadPacketNumber	= 0;
}

CPacketSessions::~CPacketSessions(VOID)
{
}

BOOL CPacketSessions::Begin(VOID)
{
	CThreadSync Sync;

	memset(mPacketBuffer, 0, sizeof(mPacketBuffer));

	mRemainLength			= 0;
	mLastReadPacketNumber	= 0;

	if (!WriteQueue.Begin())
		return FALSE;

	return CNetworkSession::Begin();
}

BOOL CPacketSessions::End(VOID)
{
	CThreadSync Sync;

	if (!WriteQueue.End())
		return FALSE;

	return CNetworkSession::End();
}

BOOL CPacketSessions::InitializePacket(VOID)
{
	CThreadSync Sync;

	memset(mPacketBuffer, 0, sizeof(mPacketBuffer));

	mRemainLength			= 0;
	mLastReadPacketNumber	= 0;

	return TRUE;
}

BOOL CPacketSessions::GetPacket(BYTE *packet, DWORD &packetLength)
{
	CThreadSync Sync;

	if (!packet)
		return FALSE;

	if (mRemainLength < sizeof(DWORD))
		return FALSE;

	memcpy(packet, mPacketBuffer, packetLength);
	
	/*	
	memcpy(mType,	mPacketBuffer ,					sizeof(CHAR)*3 );
	memcpy(mValue0, mPacketBuffer + sizeof(CHAR)*3,	sizeof(CHAR)*3 );
	memcpy(mValue1, mPacketBuffer + sizeof(CHAR)*6,	sizeof(CHAR)*3 );
	*/

	mRemainLength = 0;

	return TRUE;
}

// ReadPacketForIocp는 FALSE가 떨어질때 까지 while문을 돌린다.
BOOL CPacketSessions::ReadPacketForIocp(DWORD readLength)
{
	CThreadSync Sync;

	if (!CNetworkSession::ReadForIocp(mPacketBuffer + mRemainLength, readLength))
		return FALSE;

	mRemainLength	+= readLength;
	return TRUE;
}

// ReadPacketForEventSelect는 FALSE가 떨어질때 까지 while문을 돌린다.
BOOL CPacketSessions::ReadPacketForEventSelect(VOID)
{
	CThreadSync Sync;

	DWORD ReadLength = 0;

	if (!CNetworkSession::ReadForEventSelect(mPacketBuffer + mRemainLength, ReadLength))
		return FALSE;

	mRemainLength	+= ReadLength;

	//return getPacket(protocol, packet, packetLength);
	return TRUE;
}

BOOL CPacketSessions::WritePacket(BYTE *packet, DWORD packetLength)
{
	CThreadSync Sync;

	if (!packet)
		return FALSE;

	DWORD PacketLength = packetLength;

	if (PacketLength >= MAXBUFFERLENGTH)
		return FALSE;

	//// 패킷에 인덱스를 붙여 순서 보정을 해 주어야 한다.
	//memcpy(TempBuffer, &PacketLength, sizeof(CHAR)*3);
	//
	//memcpy(TempBuffer /*ORDER*/, 
	//	&mType, sizeof(CHAR)*3);
	//
	//memcpy(TempBuffer + 
	//	sizeof(CHAR)*3/*ORDER*/, 
	//	&mValue0,sizeof(CHAR)*3);
	//
	//memcpy(TempBuffer + 
	//	sizeof(CHAR)*3/*ORDER*/ +
	//	sizeof(CHAR)*3/*VALUE0*/,  
	//	&mValue1,sizeof(CHAR)*3);
	//
	//// WriteQueue를 이용해서 패킷이 전송 완료가 되었을까지 메모리를 살려둔다.
	////BYTE *WriteData = WriteQueue.Push(this, TempBuffer, PacketLength);

	return CNetworkSession::Write(packet, PacketLength);
}

BOOL CPacketSessions::WriteComplete(VOID)
{
	CThreadSync Sync;

	// WriteQueue에서 Pop을 해 주면 된다.
	return WriteQueue.Pop();
}
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

CConnectedSession::CConnectedSession(void)
{
	ZeroMemory( mVirtualAddress, sizeof(mVirtualAddress) );
	ZeroMemory( mRealAddress, sizeof(mRealAddress) );

	mVirtualPort = 0;
	mRealPort = 0;

	ZeroMemory( mSensorID, sizeof(mSensorID) );
	ZeroMemory( mType, sizeof(mType) );
	ZeroMemory( mValue0, sizeof(mValue0) );
	ZeroMemory( mValue1, sizeof(mValue1) );

	mIsConnected = FALSE;
}


CConnectedSession::~CConnectedSession(void)
{
}

BOOL	CConnectedSession::Begin( VOID )
{
	CThreadSync Sync;
	return TRUE;
}
BOOL	CConnectedSession::End( VOID )
{
	CThreadSync Sync;
	return CPacketSessions::End();
}
BOOL	CConnectedSession::InitializePacket( VOID )
{
	CThreadSync Sync;
	return CPacketSessions::InitializePacket();
}

BOOL CConnectedSession::Reload( SOCKET listenSocket )
{
	CThreadSync Sync;

	End();
	
	if( ! Begin() )
		return FALSE;

	if( ! CNetworkSession::Accept( listenSocket  ))
	//if( ! CNetworkSession::Accept( NULL ))
	{
		return FALSE;
	}
	return TRUE;
}

std::string	 CConnectedSession::GetSensorID()
{
	CThreadSync Sync;
	std::string temp;
	int nSize = wcslen( mSensorID )*2 +1;    
	wcstombs( (char*)temp.c_str() , mSensorID, nSize);
	return temp;
}
std::string	 CConnectedSession::GetSensorType()
{
	CThreadSync Sync;
	std::string temp;
	int nSize = wcslen( mType )*2 +1;    
	wcstombs( (char*)temp.c_str() , mType, nSize);
	return temp;
}
std::string	 CConnectedSession::GetSensorValue0()
{
	CThreadSync Sync;
	std::string temp;
	int nSize = wcslen( mValue0 )*2 +1;    
	wcstombs( (char*)temp.c_str() , mValue0, nSize);
	return temp;
}
std::string	 CConnectedSession::GetSensorValue1()
{
	CThreadSync Sync;
	std::string temp;
	int nSize = wcslen( mValue1 )*2 +1;    
	wcstombs( (char*)temp.c_str() , mValue1, nSize);
	return temp;
}
BOOL CConnectedSession::GetConnect()
{
	CThreadSync Sync;
	return mIsConnected;
}

BOOL CConnectedSession::SetSensorID( std::string temp )
{
	CThreadSync Sync;
	mbstowcs( mSensorID, temp.c_str(), strlen(temp.c_str()) + 1);
	return TRUE;
}
BOOL CConnectedSession::SetSensorType( std::string temp )
{
	CThreadSync Sync;
	mbstowcs( mType, temp.c_str(), strlen(temp.c_str()) + 1);
	return TRUE;
}
BOOL CConnectedSession::SetSensorValue0( std::string temp )
{
	CThreadSync Sync;
	mbstowcs( mValue0, temp.c_str(), strlen(temp.c_str()) + 1);
	return TRUE;
}
BOOL CConnectedSession::SetSensorValue1( std::string temp )
{
	CThreadSync Sync;
	mbstowcs( mValue1, temp.c_str(), strlen(temp.c_str()) + 1);
	return TRUE;
}
BOOL CConnectedSession::SetConnect( bool connect )
{
	CThreadSync Sync;
	mIsConnected = connect;
	return TRUE;
}
#pragma once

#pragma warning(disable:4996)		// strncpy :: C4996 Error

class CPacketSessions : public CNetworkSession
{
public:
	CPacketSessions(void);
	virtual ~CPacketSessions(void);

private:
	BYTE								mPacketBuffer[MAXBUFFERLENGTH * 3];
	INT									mRemainLength;
	
	DWORD								mLastReadPacketNumber;

	CCircularQueue						WriteQueue;

public:
	BOOL	Begin(VOID);
	BOOL	End(VOID);
	BOOL	InitializePacket(VOID);

	BOOL	ReadPacketForIocp(DWORD readLength);
	BOOL	ReadPacketForEventSelect(VOID);

	BOOL	WritePacket(BYTE *packet, DWORD packetLength);
	BOOL	WriteComplete(VOID);

	BOOL	GetPacket(BYTE *packet, DWORD &packetLength);
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CConnectedSession :
	public CPacketSessions
{
public:
	CConnectedSession(void);
	virtual ~CConnectedSession(void);

private:
	//------------ Packet ------------//
	TCHAR			mSensorID[3];
	TCHAR			mType[3];
	TCHAR			mValue0[3];
	TCHAR			mValue1[3];
	//----------------------------------//
	TCHAR			mVirtualAddress[32];
	TCHAR			mRealAddress[32];

	USHORT			mVirtualPort;
	USHORT			mRealPort;
	//------------------------------------//
	BOOL				mIsConnected;

public:
	BOOL				Begin( VOID );
	BOOL				End( VOID );
	BOOL				InitializePacket( VOID );
	
	// 접속 종료 -> 클리어 -> Accpet상태로 변경
	BOOL				Reload( SOCKET listenSocket );
public:
	inline BOOL SetVirtualInformation( LPTSTR virtualAddress, USHORT virtualPort )
	{
		CThreadSync Sync;
		if( ! virtualAddress || virtualPort <= 0 )	return FALSE;
		_tcsncpy( mVirtualAddress, virtualAddress, 32 );
		mVirtualPort = virtualPort;
		return TRUE;
	}
	inline BOOL SetRealInformation( LPTSTR realAddress, USHORT realProt )
	{
		CThreadSync Sync;
		if( ! realAddress || realProt <= 0 )	return FALSE;
		_tcsncpy( mVirtualAddress, realAddress, 32 );
		mVirtualPort = realProt;
		return TRUE;
	}

	std::string			GetSensorID();
	std::string			GetSensorType();
	std::string			GetSensorValue0();
	std::string			GetSensorValue1();
	BOOL				GetConnect();

	BOOL				SetSensorID( std::string temp );
	BOOL				SetSensorType( std::string temp );
	BOOL				SetSensorValue0( std::string temp );
	BOOL				SetSensorValue1( std::string temp );
	BOOL				SetConnect( bool connect );
};


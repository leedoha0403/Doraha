#pragma once

#pragma warning(disable:4996)		// strncpy :: C4996 Error

class CPacketSensor : public CNetworkSession
{
public:
	CPacketSensor(void);
	virtual ~CPacketSensor(void);

private:
	BYTE								mPacketBuffer[MAXBUFFERLENGTH * 3];
	INT									mRemainLength;
	
	DWORD								mLastReadPacketNumber;

	CCircularQueue						WriteQueue;
	/* -------------------------------------------------------------- */
	CHAR			mType[3];
	CHAR			mValue0[3];
	CHAR			mValue1[3];
	/* -------------------------------------------------------------- */

public:
	BOOL	Begin(VOID);
	BOOL	End(VOID);
	BOOL	InitializePacket(VOID);

	BOOL	ReadPacketForIocp(DWORD readLength);
	BOOL	ReadPacketForEventSelect(VOID);

	BOOL	WritePacket(BYTE *packet, DWORD packetLength);
	BOOL	WriteComplete(VOID);

	BOOL	GetPacket(BYTE *packet, DWORD &packetLength);
	/* -------------------------------------------------------------- */
	std::string			GetSensorType();
	std::string			GetSensorValue0();
	std::string			GetSensorValue1();
	BOOL				SetSensorType()
	{
		memset(mType, 0, sizeof(mType));
		memset(mValue0, 0, sizeof(mValue0));
		memset(mValue1, 0, sizeof(mValue1));
		return TRUE;
	}
	/* -------------------------------------------------------------- */
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class CConnectSensor :
	public CPacketSensor
{
public:
	CConnectSensor(void);
	virtual ~CConnectSensor(void);

private:
	//----------------------------------//
	TCHAR				mVirtualAddress[32];
	TCHAR				mRealAddress[32];

	USHORT				mVirtualPort;
	USHORT				mRealPort;
	//------------------------------------//
	BOOL				mIsConnected;

	std::string			mSensorKind;		// 센서의 종류
public:
	BOOL				Begin( VOID );
	BOOL				End( VOID );
	BOOL				InitializePacket( VOID );
	
	// 접속 종료 -> 클리어 -> Accpet상태로 변경
	BOOL				Reload( SOCKET listenSocket );

protected:
	BOOL				SetSensorKind(std::string str)
	{
		if( mSensorKind.empty() )
			return FALSE;
		if( str.empty() )
			return FALSE;
		
		mSensorKind = str.substr(0,3);

		return TRUE;
	}
public:
	DWORD				GetProtocol();

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
	BOOL				GetConnect();
	BOOL				SetConnect( bool connect );
	std::string			GetSensorKind()
	{
		return mSensorKind;
	}
};


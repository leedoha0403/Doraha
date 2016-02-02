#include "stdafx.h"

#include "CCriticalSection.h"
#include "CMultiThreadSync.h"
#include "CCircularQueue.h"
#include "CNetworkSession.h"

#include "ConnectSensor.h"
#include "ProtocolHeader.h"

////////////////////////////////////////////////////////////////////////////////////////

CPacketSensor::CPacketSensor(VOID)
{
	memset(mPacketBuffer, 0, sizeof(mPacketBuffer));

	mRemainLength			= 0;

	mLastReadPacketNumber	= 0;
}

CPacketSensor::~CPacketSensor(VOID)
{
}

BOOL CPacketSensor::Begin(VOID)
{
	CThreadSync Sync;

	memset(mPacketBuffer, 0, sizeof(mPacketBuffer));

	mRemainLength			= 0;
	mLastReadPacketNumber	= 0;


	if (!WriteQueue.Begin())
		return FALSE;

	return CNetworkSession::Begin();
}

BOOL CPacketSensor::End(VOID)
{
	CThreadSync Sync;

	if (!WriteQueue.End())
		return FALSE;

	return CNetworkSession::End();
}

BOOL CPacketSensor::InitializePacket(VOID)
{
	CThreadSync Sync;

	memset(mPacketBuffer, 0, sizeof(mPacketBuffer));

	mRemainLength			= 0;
	mLastReadPacketNumber	= 0;

	return TRUE;
}

BOOL CPacketSensor::GetPacket(BYTE *packet, DWORD &packetLength)
{
	CThreadSync Sync;

	if (!packet)
		return FALSE;

	if (mRemainLength < sizeof(DWORD))
		return FALSE;

	memcpy(packet, mPacketBuffer, packetLength);
	
	memcpy(mType,	mPacketBuffer ,					sizeof(CHAR)*3 );
	memcpy(mValue0, mPacketBuffer + sizeof(CHAR)*3,	sizeof(CHAR)*3 );
	memcpy(mValue1, mPacketBuffer + sizeof(CHAR)*6,	sizeof(CHAR)*3 );

	mRemainLength = 0;

	return TRUE;
}

// ReadPacketForIocp는 FALSE가 떨어질때 까지 while문을 돌린다.
BOOL CPacketSensor::ReadPacketForIocp(DWORD readLength)
{
	CThreadSync Sync;

	if (!CNetworkSession::ReadForIocp(mPacketBuffer + mRemainLength, readLength))
		return FALSE;

	mRemainLength	+= readLength;
	return TRUE;
}

// ReadPacketForEventSelect는 FALSE가 떨어질때 까지 while문을 돌린다.
BOOL CPacketSensor::ReadPacketForEventSelect(VOID)
{
	CThreadSync Sync;

	DWORD ReadLength = 0;

	if (!CNetworkSession::ReadForEventSelect(mPacketBuffer + mRemainLength, ReadLength))
		return FALSE;

	mRemainLength	+= ReadLength;

	//return getPacket(protocol, packet, packetLength);
	return TRUE;
}

BOOL CPacketSensor::WritePacket(BYTE *packet, DWORD packetLength)
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

BOOL CPacketSensor::WriteComplete(VOID)
{
	CThreadSync Sync;

	// WriteQueue에서 Pop을 해 주면 된다.
	return WriteQueue.Pop();
}

std::string	 CPacketSensor::GetSensorType()
{
	CThreadSync Sync;
	std::string temp;
	temp = mType;
	return temp.substr(0,3);
}
std::string	 CPacketSensor::GetSensorValue0()
{
	CThreadSync Sync;
	std::string temp;
	temp = mValue0;
	return temp.substr(0,3);
}
std::string	 CPacketSensor::GetSensorValue1()
{
	CThreadSync Sync;
	std::string temp;
	temp = mValue1;
	return temp.substr(0,3);
}

/////////////////////////////////////////////////////////////////////////////////////////

CConnectSensor::CConnectSensor(void)
{
	ZeroMemory( mVirtualAddress, sizeof(mVirtualAddress) );
	ZeroMemory( mRealAddress, sizeof(mRealAddress) );

	mVirtualPort = 0;
	mRealPort = 0;

	mIsConnected = FALSE;
}


CConnectSensor::~CConnectSensor(void)
{
}

BOOL	CConnectSensor::Begin( VOID )
{
	CThreadSync Sync;
	return TRUE;
}
BOOL	CConnectSensor::End( VOID )
{
	CThreadSync Sync;
	return CPacketSensor::End();
}
BOOL	CConnectSensor::InitializePacket( VOID )
{
	CThreadSync Sync;
	return CPacketSensor::InitializePacket();
}

BOOL CConnectSensor::Reload( SOCKET listenSocket )
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
DWORD CConnectSensor::GetProtocol()
{	
	CThreadSync Sync;
	std::string datatype = this->GetSensorType();

	if (datatype.find("xxx") != std::string::npos) 
	{
		// SETTING TYPE
		std::string senserType = this->GetSensorValue0();

		if( senserType.find("xx1") != std::string::npos )
		{
			mSensorKind = "xx1";
			return PT_SETUP_LED;
		}
		else if( senserType.find("xx2") != std::string::npos )
		{
			mSensorKind = "xx2";
			return PT_SETUP_LOCK;
		}
		else if( senserType.find("xx3") != std::string::npos )
		{
			mSensorKind = "xx3";
			return PT_SETUP_RFID;
		}
		else if( senserType.find("xx4") != std::string::npos )
		{
			mSensorKind = "xx4";
			return PT_SETUP_PRESSURE;
		}
	}
	//else if( datatype.find("2xx") != std::string::npos )
	//{
	//	// RESPONSE TYPE
	//
	//	std::string senserType = this->GetSensorValue0();
	//
	//	if( mSensorKind.find("xxx") != std::string::npos )
	//	{
	//		mSensorKind = "xxx";
	//	}
	//	if( mSensorKind.find("xx1") != std::string::npos )	// 설정된 SensorKind 판별
	//	{
	//		return PT_RESPONSE_LED;
	//	}
	//	else if( mSensorKind.find("xx2") != std::string::npos )
	//	{
	//		return PT_RESPONSE_LOCK;
	//	}
	//	else if( mSensorKind.find("xx3") != std::string::npos )
	//	{
	//		return PT_RESPONSE_RFID;
	//	}
	//	else if( mSensorKind.find("xx4") != std::string::npos )
	//	{
	//		return PT_RESPONSE_PRESSURE;
	//	}
	//}
	else if( datatype.find("1xx") != std::string::npos )
	{
		// VALUE TYPE
		
		if( mSensorKind.find("xx1") != std::string::npos )	// 설정된 SensorKind 판별
		{
			return PT_DATA_LED;
		}
		else if( mSensorKind.find("xx2") != std::string::npos )
		{
			return PT_DATA_LOCK;
		}
		else if( mSensorKind.find("xx3") != std::string::npos )
		{
			return PT_DATA_RFID;
		}
		else if( mSensorKind.find("xx4") != std::string::npos )
		{
			return PT_DATA_RPESSURE;
		}
	}
	else if( datatype.find("111") != std::string::npos )
	{
		if( this->GetSensorValue1().find("111") != std::string::npos )
		{
			mSensorKind = "000";
			return PT_MOBILE_CONNECT;
		}
		if( this->GetSensorValue1().find("112") != std::string::npos )
		{
			return PT_SCIENCE_OPEN;
		}
		if( this->GetSensorValue1().find("113") != std::string::npos )
		{
			return PT_SCIENCE_CLEAR;
		}
		else if( this->GetSensorValue1().find("114") != std::string::npos )
		{
			return PT_DIARY_OPEN;
		}
	}
}

BOOL CConnectSensor::GetConnect()
{
	CThreadSync Sync;
	return mIsConnected;
}

BOOL CConnectSensor::SetConnect( bool connect )
{
	CThreadSync Sync;
	mIsConnected = connect;
	return TRUE;
}
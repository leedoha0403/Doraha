
#include "stdafx.h"

#include "CCriticalSection.h"
#include "CMultiThreadSync.h"
#include "CCircularQueue.h"
#include "CNetworkSession.h"
#include "CPacketSession.h"
#include "CIocp.h"

#include "ConnectSensor.h"
#include "ConnectSensorManager.h"

#include "HINTSensorIocp.h"
#include "ProtocolHeader.h"
#include "HINTSensorControler.h"

DWORD WINAPI ReceiveChangeMessage( LPVOID parameter )
{
	CHINTSensorControler *Owner = (CHINTSensorControler*) parameter;
	Owner->ReceiveChangeMessage();

	return 0;
}
CHINTSensorControler::CHINTSensorControler(void)
{
	mInputData = false;
}
CHINTSensorControler::~CHINTSensorControler(void)
{
}
BOOL CHINTSensorControler::Begin(VOID)
{
	return CHINTSensorIocp::Begin();
}
BOOL CHINTSensorControler::End(VOID)
{
	if (mReciveThread)
	{
		WaitForSingleObject(mReciveThread, INFINITE);
		CloseHandle(mReciveThread);
	}
	return CHINTSensorIocp::End();
}
BOOL CHINTSensorControler::Begin( DWORD port )
{
	mReciveThread		= CreateThread(NULL, 0, ::ReceiveChangeMessage, this, 0, NULL);
	if (!mReciveThread)
	{
		End();

		return FALSE;
	}
	return CHINTSensorIocp::Begin( port );
}

VOID CHINTSensorControler::ReceiveChangeMessage( VOID )
{
	while (1)
	{
		while (!mInputData)
		{
			static char temp = NULL; 
			
			if( GetKeyState( VK_F1 ) < 0 )
			{
				mInputData = true;
				memset( mReceiveData, 0, sizeof(mReceiveData) ) ;
			}
		}
		
		MULTI_LOG3() << "ORDER << ";
		std::cin >> mReceiveData;

		if( strcmp( mReceiveData, "exit") == 0 )
			return;
		
		MULTI_LOG3() << "ORDER >> " << mReceiveData << std::endl;
		//PT_SENSOR_VALUE_SEND( mReceiveData );

		mInputData = false;
	}
	return;
}

VOID CHINTSensorControler::OnIoRead(VOID *object, DWORD dataLength)
{
	if( mInputData )	 // 글자가 입력되는 동안
	{
		CConnectSensor *ConnectedSensor = (CConnectSensor*) object;
		
		BYTE	Packet[MAXBUFFERLENGTH]					= {0,};
		DWORD	PacketLength							= 0;
		
		BYTE	WriteBuffer[MAXBUFFERLENGTH]			= {0,};
		
		if (ConnectedSensor->ReadPacketForIocp(dataLength))
		{
			if( ConnectedSensor->GetPacket( Packet, PacketLength))
			{
				// 각 패킷에 대한 처리
			}
		
		if (!ConnectedSensor->InitializeReadForIocp())
			ConnectedSensor->End();
		}
	}
	else
		CHINTSensorIocp::OnIoRead( object, dataLength );
}

VOID CHINTSensorControler::OnIoWrote(VOID *object, DWORD dataLength)
{
	CHINTSensorIocp::OnIoWrote( object, dataLength );
}
VOID CHINTSensorControler::OnIoConnected(VOID *object)
{
	CHINTSensorIocp::OnIoConnected( object );
}
VOID CHINTSensorControler::OnIoDisconnected(VOID *object)
{
	CHINTSensorIocp::OnIoDisconnected( object );
}
//----------------------------------------------------------------------------//
#include "stdafx.h"

#include "CCriticalSection.h"
#include "CMultiThreadSync.h"
#include "CCircularQueue.h"
#include "CNetworkSession.h"
#include "CPacketSession.h"
#include "CIocp.h"

#include "ConnectSensor.h"
#include "ConnectSensorManager.h"

//#include "BundleOfStageData.h"
//#include "StageData.h"

#include "HINTSensorIocp.h"
#include "ProtocolHeader.h"


DWORD WINAPI KeepThreadCallbackSensor(LPVOID parameter)
{
	CHINTSensorIocp *Owner = (CHINTSensorIocp*) parameter;
	Owner->KeepThreadCallbackSensor();

	return 0;
}
DWORD WINAPI SensorThreadCallbackSensor(LPVOID parameter )
{
	CHINTSensorIocp *Owner = (CHINTSensorIocp*) parameter;
	Owner->SensorThreadCallbackSensor();

	return 0;
}
CHINTSensorIocp::CHINTSensorIocp(void)
{
}


CHINTSensorIocp::~CHINTSensorIocp(void)
{
}

VOID CHINTSensorIocp::KeepThreadCallbackSensor(VOID)
{
	DWORD KeepAlive = 0xFFFF;

	while (TRUE)
	{
		DWORD Result = WaitForSingleObject(mKeepThreadDestroyEvent, 30000);

		if (Result == WAIT_OBJECT_0)
			return;

		//mConnectedSensorManager.WriteAll(0x3000000, (BYTE*) &KeepAlive, sizeof(DWORD));
	}
}

VOID CHINTSensorIocp::SensorThreadCallbackSensor(VOID)
{
	DWORD KeepAlive = 0xFFFF;

	while (TRUE)
	{
		// 지속적으로 Current MessageCheck
		mConnectedSensorManager.CheckCurrentStatus();
	}
}

VOID CHINTSensorIocp::CheckTimerCallbackSensor(VOID)
{
	
}

BOOL CHINTSensorIocp::Begin(VOID)
{
	if (!CIocp::Begin())
	{	
		MULTI_LOG3() << "CIocp::Begin" << std::endl;

		End();

		return FALSE;
	}

	if (!mListenSession.Begin())
	{
		MULTI_LOG3() << "ListenSession.Begin" << std::endl;

		End();

		return FALSE;
	}

	if (!mListenSession.Bind())
	{
		MULTI_LOG3() << "ListenSession.TcpBind" << std::endl;

		End();
		
		return FALSE;
	}

	if (!mListenSession.Listen(DEFAULT_PORT, MAX_SENSOR))
	{
		MULTI_LOG3() << "ListenSession.Listen" << std::endl ;

		End();

		return FALSE;
	}

	if (!CIocp::RegisterSocketToIocp(mListenSession.GetSocket(), (ULONG_PTR) &mListenSession))
	{
		MULTI_LOG3() << "CIocp::RegisterSocketToIocp" << std::endl;

		End();

		return FALSE;
	}

	if (!mConnectedSensorManager.Begin(MAX_SENSOR, mListenSession.GetSocket()))
	{
		MULTI_LOG3() << "ConnectedUserManager.Begin" << std::endl;

		End();

		return FALSE;
	}

	if (!mConnectedSensorManager.AcceptAll())
	{
		MULTI_LOG3() << "ConnectedUserManager.AllAccept" << std::endl;

		End();

		return FALSE;
	}

	mKeepThreadDestroyEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (!mKeepThreadDestroyEvent)
	{
		End();

		return FALSE;
	}

	mKeepThreadHandle		= CreateThread(NULL, 0, ::KeepThreadCallbackSensor, this, 0, NULL);
	if (!mKeepThreadHandle)
	{
		End();

		return FALSE;
	}

	mSensorThreadHandle		= CreateThread(NULL, 0, ::SensorThreadCallbackSensor, this, 0, NULL);
	if (!mSensorThreadHandle)
	{
		End();

		return FALSE;
	}
	

	//mCheckTimerThreadHandle		= CreateThread(NULL, 0, ::, this, 0, NULL);
	//if (!mSensorThreadHandle)
	//{
	//	End();
	//
	//	return FALSE;
	//}

	return TRUE;
}

BOOL CHINTSensorIocp::End(VOID)
{
	if (mKeepThreadDestroyEvent && mKeepThreadHandle)
	{
		SetEvent(mKeepThreadDestroyEvent);

		WaitForSingleObject(mKeepThreadHandle, INFINITE);

		CloseHandle(mKeepThreadDestroyEvent);
		CloseHandle(mKeepThreadHandle);
	}

	if( mSensorThreadHandle )
	{
		WaitForSingleObject(mSensorThreadHandle, INFINITE);
		CloseHandle(mSensorThreadHandle);
	}
	CIocp::End();

	mConnectedSensorManager.End();
	mListenSession.End();

	return TRUE;
}


BOOL CHINTSensorIocp::Begin( DWORD port )
{
	if (!CIocp::Begin())
	{	
		MULTI_LOG3() << "CIocp::Begin" << std::endl;

		End();

		return FALSE;
	}

	if (!mListenSession.Begin())
	{
		MULTI_LOG3() << "ListenSession.Begin" << std::endl;

		End();

		return FALSE;
	}

	if (!mListenSession.Bind())
	{
		MULTI_LOG3() << "ListenSession.TcpBind" << std::endl;

		End();

		return FALSE;
	}

	if (!mListenSession.Listen(port, MAX_SENSOR))
	{
		MULTI_LOG3() << "ListenSession.Listen" << std::endl ;

		End();

		return FALSE;
	}

	if (!CIocp::RegisterSocketToIocp(mListenSession.GetSocket(), (ULONG_PTR) &mListenSession))
	{
		MULTI_LOG3() << "CIocp::RegisterSocketToIocp" << std::endl;

		End();

		return FALSE;
	}

	if (!mConnectedSensorManager.Begin(MAX_SENSOR, mListenSession.GetSocket()))
	{
		MULTI_LOG3() << "ConnectedUserManager.Begin" << std::endl;

		End();

		return FALSE;
	}

	if (!mConnectedSensorManager.AcceptAll())
	{
		MULTI_LOG3() << "ConnectedUserManager.AllAccept" << std::endl;

		End();

		return FALSE;
	}

	mKeepThreadDestroyEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (!mKeepThreadDestroyEvent)
	{
		End();

		return FALSE;
	}

	mKeepThreadHandle		= CreateThread(NULL, 0, ::KeepThreadCallbackSensor, this, 0, NULL);
	if (!mKeepThreadHandle)
	{
		End();

		return FALSE;
	}

	mSensorThreadHandle		= CreateThread(NULL, 0, ::SensorThreadCallbackSensor, this, 0, NULL);
	if (!mKeepThreadHandle)
	{
		End();

		return FALSE;
	}
	return TRUE;
}

VOID CHINTSensorIocp::OnIoRead(VOID *object, DWORD dataLength)
{
	CConnectSensor *ConnectedSensor = (CConnectSensor*) object;

	BYTE	Packet[MAXBUFFERLENGTH]					= {0,};
	DWORD	PacketLength							= 0;

	BYTE	WriteBuffer[MAXBUFFERLENGTH]			= {0,};

	if (ConnectedSensor->ReadPacketForIocp(dataLength))
	{
		if( ConnectedSensor->GetPacket( Packet, dataLength))
		{
			//MULTI_LOG3() << (char*)Packet << std::endl;

			if( ConnectedSensor->GetProtocol() == PT_SETUP_LED ){ 
				mConnectedSensorManager.SetSocketArduino( ConnectedSensor );
				onPT_LED_SENSOR_SETUP( ConnectedSensor, Packet );
			}
			else if( ConnectedSensor->GetProtocol() == PT_SETUP_LOCK ){
				mConnectedSensorManager.SetSocketArduino( ConnectedSensor );
				onPT_LOCK_SENSOR_SETUP( ConnectedSensor, Packet );
			}
			else if( ConnectedSensor->GetProtocol() == PT_SETUP_RFID ){
				mConnectedSensorManager.SetSocketArduino( ConnectedSensor );
				onPT_RFID_SENSOR_SETUP( ConnectedSensor, Packet );
			}
			else if( ConnectedSensor->GetProtocol() == PT_SETUP_PRESSURE ){
				mConnectedSensorManager.SetSocketArduino( ConnectedSensor );
				onPT_PRESSURE_SENSOR_SETUP( ConnectedSensor, Packet );
			}
			if( ConnectedSensor->GetProtocol() == PT_DATA_LED ){
				onPT_LED_SENSOR_DATA( ConnectedSensor, Packet );
			}
			else if( ConnectedSensor->GetProtocol() == PT_DATA_LOCK ){
				onPT_LOCK_SENSOR_DATA( ConnectedSensor, Packet );
			}
			else if( ConnectedSensor->GetProtocol() == PT_DATA_RFID ){
				onPT_RFID_SENSOR_DATA( ConnectedSensor, Packet );
			}
			else if( ConnectedSensor->GetProtocol() == PT_DATA_RPESSURE ){
				onPT_PRESSURE_SENSOR_DATA( ConnectedSensor, Packet );
			}

			if( ConnectedSensor->GetProtocol() == PT_RESPONSE_LED ){
				onPT_LED_SENSOR_RESPONSE( ConnectedSensor, Packet );
			}
			else if( ConnectedSensor->GetProtocol() == PT_RESPONSE_LOCK ){
				onPT_LOCK_SENSOR_RESPONSE( ConnectedSensor, Packet );
			}
			else if( ConnectedSensor->GetProtocol() == PT_RESPONSE_RFID ){
				onPT_RFID_SENSOR_RESPONSE( ConnectedSensor, Packet );
			}
			else if( ConnectedSensor->GetProtocol() == PT_RESPONSE_PRESSURE ){
				onPT_PRESSURE_SENSOR_RESPONSE( ConnectedSensor, Packet );
			}

			if( ConnectedSensor->GetProtocol() == PT_MOBILE_CONNECT ){
				//111111111
				mConnectedSensorManager.SetSocketMobile( ConnectedSensor );
				onPT_MOBILE_CONNECT( ConnectedSensor, Packet );
			}
			else if( ConnectedSensor->GetProtocol() == PT_SCIENCE_OPEN ){
				//111111112
				onPT_SCIENCE_OPEN( ConnectedSensor, Packet );
			}
			else if( ConnectedSensor->GetProtocol() == PT_SCIENCE_CLEAR ){
				//111111113
				onPT_SCIENCE_CLEAR( ConnectedSensor, Packet );
			}
			else if( ConnectedSensor->GetProtocol() == PT_DIARY_OPEN ){
				//111111114
				onPT_DIARY_OPEN( ConnectedSensor, Packet );
			}
		}
	}

	if (!ConnectedSensor->InitializeReadForIocp())
		ConnectedSensor->End();
}

VOID CHINTSensorIocp::OnIoWrote(VOID *object, DWORD dataLength)
{
	CConnectSensor *ConnectedSensor = (CConnectSensor*) object;

	ConnectedSensor->WriteComplete();

#ifdef _FULL_DEBUG
	//CLog::WriteLog(_T("# Client data wrote : 0x%x(0x%x)(%d)\n"), ConnectedUser, ConnectedUser->GetSocket(), dataLength);
#endif
}

VOID CHINTSensorIocp::OnIoConnected(VOID *object)
{
	CConnectSensor *ConnectedSensor = (CConnectSensor*) object;

	TCHAR	RemoteAddress[32]	= {0,};
	USHORT	RemotePort			= 0;

	ConnectedSensor->GetRemoteAddressAfterAccept(RemoteAddress, RemotePort);
	ConnectedSensor->SetRealInformation(RemoteAddress, RemotePort);

	if (!CIocp::RegisterSocketToIocp(ConnectedSensor->GetSocket(), (ULONG_PTR) ConnectedSensor))
	{
		MULTI_LOG3() << "! OnIoConnected : CIocp::RegisterSocketToIocp" << std::endl ;

		End();

		return;
	}

	ConnectedSensor->SetConnect(TRUE);

	if (!ConnectedSensor->InitializeReadForIocp())
	{
		if (!ConnectedSensor->Reload(mListenSession.GetSocket()))
		{
			MULTI_LOG3() << "! OnIoConnected : ConnectedUser->Reload" << std::endl ;

			End();

			return;
		}
	}

	mConnectedSensorManager.IncreaseConnectedUserCount();

	MULTI_LOG3() << "# New client connected : ";
	MULTI_LOG3() << ConnectedSensor;
	MULTI_LOG3() << ConnectedSensor->GetSocket() << std::endl;
}

VOID CHINTSensorIocp::OnIoDisconnected(VOID *object)
{
	CConnectSensor *ConnectedSensor = (CConnectSensor*) object;

	if (!ConnectedSensor->Reload(mListenSession.GetSocket()))
	{
		MULTI_LOG3() << "! OnIoDisconnected : ConnectedUser->Reload" << std::endl;

		End();

		return;
	}

	mConnectedSensorManager.DecreaseConnectedUserCount();

	ConnectedSensor->SetConnect(FALSE);

	ConnectedSensor->InitializePacket();
	MULTI_LOG3() << "# Client disconnected : ";
	MULTI_LOG3() << ConnectedSensor;
	MULTI_LOG3() << ConnectedSensor->GetSocket() << std::endl;
}


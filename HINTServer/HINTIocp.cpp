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

DWORD WINAPI KeepThreadCallback(LPVOID parameter)
{
	CHINTIocp *Owner = (CHINTIocp*) parameter;
	Owner->KeepThreadCallback();

	return 0;
}

CHINTIocp::CHINTIocp(void)
{
}


CHINTIocp::~CHINTIocp(void)
{
}

VOID CHINTIocp::KeepThreadCallback(VOID)
{
	DWORD KeepAlive = 0xFFFF;

	while (TRUE)
	{
		DWORD Result = WaitForSingleObject(mKeepThreadDestroyEvent, 30000);

		if (Result == WAIT_OBJECT_0)
			return;

		mConnectedSessionManager.WriteAll(0x3000000, (BYTE*) &KeepAlive, sizeof(DWORD));
	}
}

BOOL CHINTIocp::Begin(VOID)
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

	if (!mConnectedSessionManager.Begin(MAX_SENSOR, mListenSession.GetSocket()))
	{
		MULTI_LOG3() << "ConnectedUserManager.Begin" << std::endl;

		End();

		return FALSE;
	}

	if (!mConnectedSessionManager.AcceptAll())
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

	mKeepThreadHandle		= CreateThread(NULL, 0, ::KeepThreadCallback, this, 0, NULL);
	if (!mKeepThreadHandle)
	{
		End();

		return FALSE;
	}
	return TRUE;
}

BOOL CHINTIocp::End(VOID)
{
	if (mKeepThreadDestroyEvent && mKeepThreadHandle)
	{
		SetEvent(mKeepThreadDestroyEvent);

		WaitForSingleObject(mKeepThreadHandle, INFINITE);

		CloseHandle(mKeepThreadDestroyEvent);
		CloseHandle(mKeepThreadHandle);
	}

	CIocp::End();

	mConnectedSessionManager.End();
	mListenSession.End();

	// 데이터베이스는 가장 마지막에 종료
	//mDataBase.End();

	return TRUE;
}


BOOL CHINTIocp::Begin( DWORD port )
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

	if (!mConnectedSessionManager.Begin(MAX_SENSOR, mListenSession.GetSocket()))
	{
		MULTI_LOG3() << "ConnectedUserManager.Begin" << std::endl;

		End();

		return FALSE;
	}

	if (!mConnectedSessionManager.AcceptAll())
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

	mKeepThreadHandle		= CreateThread(NULL, 0, ::KeepThreadCallback, this, 0, NULL);
	if (!mKeepThreadHandle)
	{
		End();

		return FALSE;
	}
	return TRUE;
}

VOID CHINTIocp::OnIoRead(VOID *object, DWORD dataLength)
{
 	CConnectedSession *ConnectedSensor = (CConnectedSession*) object;

	DWORD	Protocol								= 0;
	BYTE	Packet[MAXBUFFERLENGTH]					= {0,};
	DWORD	PacketLength							= 0;

	BYTE	WriteBuffer[MAXBUFFERLENGTH]	= {0,};

	if (ConnectedSensor->ReadPacketForIocp(dataLength))
	{
		while (ConnectedSensor->GetPacket(Packet, dataLength))
		{
			MULTI_LOG3() << (char*)Packet << std::endl;
		}
	}
	 
	if (!ConnectedSensor->InitializeReadForIocp())
		ConnectedSensor->End();
}
VOID CHINTIocp::OnIoWrote(VOID *object, DWORD dataLength)
{
	CConnectedSession *ConnectedSensor = (CConnectedSession*) object;

	ConnectedSensor->WriteComplete();

#ifdef _FULL_DEBUG
	//CLog::WriteLog(_T("# Client data wrote : 0x%x(0x%x)(%d)\n"), ConnectedUser, ConnectedUser->GetSocket(), dataLength);
#endif
}
VOID CHINTIocp::OnIoConnected(VOID *object)
{
	CConnectedSession *ConnectedSensor = (CConnectedSession*) object;

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

	mConnectedSessionManager.IncreaseConnectedUserCount();

	MULTI_LOG3() << "# New client connected : ";
	MULTI_LOG3() << ConnectedSensor;
	MULTI_LOG3() << ConnectedSensor->GetSocket() << std::endl;
}
VOID CHINTIocp::OnIoDisconnected(VOID *object)
{
	CConnectedSession *ConnectedSensor = (CConnectedSession*) object;

	if (!ConnectedSensor->Reload(mListenSession.GetSocket()))
	{
		MULTI_LOG3() << "! OnIoDisconnected : ConnectedUser->Reload" << std::endl;

		End();

		return;
	}

	mConnectedSessionManager.DecreaseConnectedUserCount();

	ConnectedSensor->SetConnect(FALSE);

	ConnectedSensor->InitializePacket();
	MULTI_LOG3() << "# Client disconnected : ";
	MULTI_LOG3() << ConnectedSensor;
	MULTI_LOG3() << ConnectedSensor->GetSocket() << std::endl;
}
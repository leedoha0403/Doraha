#include "stdafx.h"

#include "CCriticalSection.h"
#include "CMultiThreadSync.h"
#include "CCircularQueue.h"
#include "CNetworkSession.h"
#include "CPacketSession.h"

#include "ConnectSession.h"
#include "ConnectSessionManager.h"


ConnectSessionManager::ConnectSessionManager(void)
{
	mMaxSensorCount = 0;
	mCurrentSensorCount = 0;
}
ConnectSessionManager::~ConnectSessionManager(void)
{
}
BOOL ConnectSessionManager::Begin( DWORD maxUserCount, SOCKET listenSocket )
{
	CThreadSync Sync;

	if( maxUserCount <= 0 || ! listenSocket )
		return FALSE;

	mMaxSensorCount = maxUserCount;
	mCurrentSensorCount = 0;
	mListenSocket = listenSocket;

	for( DWORD i = 0 ; i < mMaxSensorCount ; ++i )
	{
		CConnectedSession *connectedSensor = new CConnectedSession;

		if( connectedSensor->Begin() )
			mConnectSessionVector.push_back( connectedSensor );
		else
		{
			End();
			return FALSE;
		}
	}
	return TRUE;
}
BOOL ConnectSessionManager::End( VOID )
{
	CThreadSync Sync;

	for( DWORD i = 0 ; i < mConnectSessionVector.size() ; ++i )
	{
		CConnectedSession *connectedUser = mConnectSessionVector[i];
		connectedUser->End();
		delete connectedUser;
	}

	mConnectSessionVector.clear();
	return TRUE;
}

BOOL ConnectSessionManager::AcceptAll(VOID)
{
	CThreadSync Sync;

	for( DWORD i = 0 ; i < mConnectSessionVector.size() ; ++i )
	{
		CConnectedSession *connectedUser = mConnectSessionVector[i];

		if( ! connectedUser->Accept( mListenSocket ) )
		{
			End();
			return FALSE;
		}
	}
	return TRUE;
}
BOOL ConnectSessionManager::WriteAll( DWORD protocol, BYTE *data, DWORD dataLength )
{
	CThreadSync Sync;

	if( ! data )
		return FALSE;

	for( DWORD i = 0 ; i < mConnectSessionVector.size() ; ++i )
	{
		CConnectedSession *connectedUser = mConnectSessionVector[i];

		if( connectedUser->GetConnect() )
		{
			if( ! connectedUser->WritePacket( data, dataLength ))
				connectedUser->End();
		}
	}

	return TRUE;
}

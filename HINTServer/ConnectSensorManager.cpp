#include "stdafx.h"

#include "CCriticalSection.h"
#include "CMultiThreadSync.h"
#include "CCircularQueue.h"
#include "CNetworkSession.h"
#include "CPacketSession.h"

#include "ConnectSensor.h"
#include "ConnectSensorManager.h"

#include <set>
#include <algorithm>

ConnectSesnsorManager::ConnectSesnsorManager(void)
{
	mMaxSensorCount = 0;
	mCurrentSensorCount = 0;
}
ConnectSesnsorManager::~ConnectSesnsorManager(void)
{
}
BOOL ConnectSesnsorManager::Begin( DWORD maxUserCount, SOCKET listenSocket )
{
	CThreadSync Sync;

	if( maxUserCount <= 0 || ! listenSocket )
		return FALSE;

	mMaxSensorCount = maxUserCount;
	mCurrentSensorCount = 0;
	mListenSocket = listenSocket;

	mStatus = SENSOR_WAIT_TO_SETUP;

	for( DWORD i = 0 ; i < mMaxSensorCount ; ++i )
	{
		CConnectSensor *connectedSensor = new CConnectSensor;

		if( connectedSensor->Begin() )
			mConnectSensorVector.push_back( connectedSensor );
		else
		{
			End();
			return FALSE;
		}
	}
	return TRUE;
}
BOOL ConnectSesnsorManager::End( VOID )
{
	CThreadSync Sync;
	
	mStatus = SENSOR_WAIT_TO_SETUP;

	for( DWORD i = 0 ; i < mConnectSensorVector.size() ; ++i )
	{
		CConnectSensor *connectedUser = mConnectSensorVector[i];
		connectedUser->End();
		delete connectedUser;
	}

	mConnectSensorVector.clear();
	return TRUE;
}

BOOL ConnectSesnsorManager::AcceptAll(VOID)
{
	CThreadSync Sync;

	for( DWORD i = 0 ; i < mConnectSensorVector.size() ; ++i )
	{
		CConnectSensor *connectedUser = mConnectSensorVector[i];

		if( ! connectedUser->Accept( mListenSocket ) )
		{
			End();
			return FALSE;
		}
	}
	return TRUE;
}
BOOL ConnectSesnsorManager::WriteAll(BYTE *data, DWORD dataLength )
{
	CThreadSync Sync;

	if( ! data )
		return FALSE;

	for( DWORD i = 0 ; i < mConnectSensorVector.size() ; ++i )
	{
		CConnectSensor *connectedUser = mConnectSensorVector[i];

		if( connectedUser->GetConnect() )
		{
			if( ! connectedUser->WritePacket( data, dataLength ))
				connectedUser->End();
		}
	}

	return TRUE;
}
//
//BOOL ConnectSesnsorManager::WriteArduino(BYTE *data, DWORD dataLength )
//{
//	CThreadSync Sync;
//
//	if( ! data )
//		return FALSE;
//
//	for( DWORD i = 0 ; i < mSocketArduino.size() ; ++i )
//	{
//		CConnectSensor *connectedUser = mSocketArduino[i];
//
//		if( connectedUser->GetConnect() )
//		{
//			if( ! connectedUser->WritePacket( data, dataLength ))
//				connectedUser->End();
//		}
//	}
//
//	return TRUE;
//}
//

BOOL ConnectSesnsorManager::WriteMobile(BYTE *data, DWORD dataLength )
{
	CThreadSync Sync;

	if( ! data )
		return FALSE;

	for( std::list<CConnectSensor*>::iterator iter = mSocketMobile.begin();
		iter != mSocketMobile.end() ; ++iter )
	{
		if( (*iter)->GetConnect() )
		{
			if( ! (*iter)->WritePacket( data, dataLength ))
				(*iter)->End();
		}
	}

	return TRUE;
}

std::vector<CConnectSensor*> ConnectSesnsorManager::GetSensorVector()
{
	CThreadSync Sync;
	return mConnectSensorVector;
}

CConnectSensor*	ConnectSesnsorManager::FindConnectSensor(std::string sensorKind)
{
	for( std::vector<CConnectSensor*>::iterator iter = mConnectSensorVector.begin()
		; iter != mConnectSensorVector.end() ; ++iter )
	{
		std::string datatype = (*iter)->GetSensorKind();

		if (datatype.find( sensorKind ) != std::string::npos) 
		{
			MULTI_LOG3() << "SensorKind : " << sensorKind << std::endl;
			return (*iter);
		}
	}

	return NULL;
}

BOOL ConnectSesnsorManager::SetSocketArduino( CConnectSensor* socket )
{
	CThreadSync Sync;
	
	if( socket == NULL )
		return FALSE;

	std::list<CConnectSensor*>::iterator iter;
	iter = std::find( mSocketArduino.begin(), mSocketArduino.end(), socket );
	if( iter != mSocketArduino.end() )
		return FALSE;						 // 이미 등록되어있는 정보
	
	mSocketArduino.push_back( socket );
	return TRUE;
}

BOOL ConnectSesnsorManager::SetSocketMobile( CConnectSensor* socket )
{
	CThreadSync Sync;
	
	if( socket == NULL )
		return FALSE;
	
	std::list<CConnectSensor*>::iterator iter;
	iter = std::find( mSocketMobile.begin(), mSocketMobile.end(), socket );
	if( iter != mSocketMobile.end() )
	{	
		MULTI_LOG3() << "# Do not Regist Socket Mobile : " << socket << std::endl;
		return FALSE;						 // 이미 등록되어있는 정보
	}
	MULTI_LOG3() << "# Regist Socket Mobile : " << socket << std::endl;
	mSocketMobile.push_back( socket );
	return TRUE;
}

std::list<CConnectSensor*> ConnectSesnsorManager::GetSocketArduino()
{
	CThreadSync Sync;
	return mSocketArduino;
}

std::list<CConnectSensor*> ConnectSesnsorManager::GetSocketMobile()
{
	CThreadSync Sync;
	return mSocketMobile;
}

BOOL ConnectSesnsorManager::SetStatus( INT _status )
{
	CThreadSync Sync;

	if( SENSOR_STATUS_BEGIN > _status || SENSOR_STATUS_END < _status )
		return FALSE;

	mStatus = _status;
}

INT ConnectSesnsorManager::GetStatus()
{
	CThreadSync Sync;
	return mStatus;
}

INT	ConnectSesnsorManager::CheckCurrentStatus()
{
	CThreadSync Sync;

	if( mSocketArduino.size() == MAX_ARDUINO_SET)			// 우선 센서 한 set가 연결되어야 함
	{
		////////////////////////////////////////////////////////////////////////////////
		// 각각 다른 기기에서 보냄을 확인
		std::set<SOCKET> TempArduinoSocket;

		for( std::list<CConnectSensor*>::iterator iter = mSocketArduino.begin();
			iter != mSocketArduino.end() ; ++iter )
		{
			TempArduinoSocket.insert( (*iter)->GetSocket() );		// 아두이노 socket이 단일임을 판별 (set)
		}

		if( TempArduinoSocket.size() == MAX_ARDUINO_SET )			// set으로 SOCK의 단일을 판별
		{
			if( SENSOR_WAIT_TO_SETUP == mStatus ) 
			{
				mStatus = SENSOR_ARDUINO_SETUP_SUCC;
				MULTI_LOG3() << "	+ SENSOR_ARDUINO_SETUP_SUCC " << std::endl;
			}
			else if( SENSOR_MOBLIE_SETUP_SUCC == mStatus )
			{
				mStatus = SENSOR_ALL_SETUP_SUCC;
				MULTI_LOG3() << "	+ SENSOR_ALL_SETUP_SUCC " << std::endl; 
			}
		}
		else
		{
			mStatus = SENSOR_ARDUINO_ERROR;
			MULTI_LOG3() << "	+ SENSOR_ARDUINO_ERROR " << std::endl; 
			mSocketArduino.clear();
			return SENSOR_ARDUINO_ERROR;
		}
		////////////////////////////////////////////////////////////////////////////////
		// xx1, xx2, xx3, xx4 가 중복되지 않음을 판별
		bool TempArduino[MAX_ARDUINO_SET] = {false,};
		int tempardunoCount = 0;

		for( std::list<CConnectSensor*>::iterator iter = mSocketArduino.begin();
			iter != mSocketArduino.end() ; ++iter , ++tempardunoCount)
		{
			if( false == TempArduino[tempardunoCount])
			{
				TempArduino[tempardunoCount] = true;
			}
			else
			{
				mStatus = SENSOR_ARDUINO_ERROR;
				MULTI_LOG3() << "	+ SENSOR_ARDUINO_ERROR " << std::endl; 
				return SENSOR_ARDUINO_ERROR;
			}
		}
		////////////////////////////////////////////////////////////////////////////////
	}
	else if( mSocketArduino.size() > MAX_ARDUINO_SET )
	{
		mSocketArduino.clear();
	}

	if( mSocketMobile.size() == MAX_MOBILE_SET )	// 모바일 최대의 개수 접속 확인
	{
		std::set<SOCKET> TempSocketMobile;

		for( std::list<CConnectSensor*>::iterator iter = mSocketMobile.begin();
			iter != mSocketMobile.end() ; ++iter )
		{
			TempSocketMobile.insert( (*iter)->GetSocket() );	// 모바일 socket이 단일임을 판별 (set)
		}

		if( TempSocketMobile.size() == MAX_MOBILE_SET )			// set으로 SOCK의 단일을 판별
		{
			if( SENSOR_WAIT_TO_SETUP == mStatus ) 
			{
				mStatus = SENSOR_MOBLIE_SETUP_SUCC;
				MULTI_LOG3() << "	+ SENSOR_MOBLIE_SETUP_SUCC " << std::endl; 
			}
			else if( SENSOR_ARDUINO_SETUP_SUCC == mStatus )
			{
				mStatus = SENSOR_ALL_SETUP_SUCC;
				MULTI_LOG3() << "	+ SENSOR_ALL_SETUP_SUCC " << std::endl;
			}
		}
		else
		{
			mStatus = SENSOR_MOBILE_ERROR;
			MULTI_LOG3() << "	+ SENSOR_MOBILE_ERROR " << std::endl; 
			return SENSOR_MOBILE_ERROR;
		}
	}
	return mStatus;
}
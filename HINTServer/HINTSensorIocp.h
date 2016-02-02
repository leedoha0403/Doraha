#pragma once

#include "BundleOfStageData.h"

class CHINTSensorIocp	: public CIocp
{
public:
	CHINTSensorIocp(void);
	virtual ~CHINTSensorIocp(void);

protected:
	CNetworkSession				mListenSession;
	ConnectSesnsorManager		mConnectedSensorManager;
	
	HANDLE						mKeepThreadHandle;
	HANDLE						mKeepThreadDestroyEvent;

	HANDLE						mSensorThreadHandle;
	HANDLE						mSensorThreadDestoryEvent;

	HANDLE						mCheckTimerThreadHandle;
	HANDLE						mCheckTimerThreadDeatoryEvent;

	CBundleOfStageData			mBunddleOfStageData;

public:	
	BOOL	Begin(VOID);
	BOOL	End(VOID);
	
	BOOL	Begin( DWORD port );
	
	VOID	KeepThreadCallbackSensor(VOID);
	VOID	SensorThreadCallbackSensor(VOID);
	VOID	CheckTimerCallbackSensor(VOID);
protected:
	VOID OnIoRead(VOID *object, DWORD dataLength);
	VOID OnIoWrote(VOID *object, DWORD dataLength);
	VOID OnIoConnected(VOID *object);
	VOID OnIoDisconnected(VOID *object);
	////////////////////////////////////////////////////////////////////////////////
	DWORD GetProtocol(CConnectSensor *ConnectSession)	
	{
		return ConnectSession->GetProtocol(); 
	}
	////////////////////////////////////////////////////////////////////////////////

protected: //------------------------- Protocol Packet -----------------------------//
	VOID	onPT_SENSOR_READY				( CConnectSensor *ConnectSession, BYTE* packet );
	VOID	onPT_SENSOR_VALUE				( CConnectSensor *ConnectSession, BYTE* packet );
	
	VOID	onPT_LED_SENSOR_SETUP			( CConnectSensor *ConnectSession, BYTE* packet );
	VOID	onPT_LED_SENSOR_DATA			( CConnectSensor *ConnectSession, BYTE* packet );
	VOID	onPT_LED_SENSOR_RESPONSE		( CConnectSensor *ConnectSession, BYTE* packet );
	
	VOID	onPT_LOCK_SENSOR_SETUP			( CConnectSensor *ConnectSession, BYTE* packet );
	VOID	onPT_LOCK_SENSOR_DATA			( CConnectSensor *ConnectSession, BYTE* packet );
	VOID	onPT_LOCK_SENSOR_RESPONSE		( CConnectSensor *ConnectSession, BYTE* packet );
	
	VOID	onPT_RFID_SENSOR_SETUP			( CConnectSensor *ConnectSession, BYTE* packet );
	VOID	onPT_RFID_SENSOR_DATA			( CConnectSensor *ConnectSession, BYTE* packet );
	VOID	onPT_RFID_SENSOR_RESPONSE		( CConnectSensor *ConnectSession, BYTE* packet );

	VOID	onPT_PRESSURE_SENSOR_SETUP		( CConnectSensor *ConnectSession, BYTE* packet );
	VOID	onPT_PRESSURE_SENSOR_DATA		( CConnectSensor *ConnectSession, BYTE* packet );
	VOID	onPT_PRESSURE_SENSOR_RESPONSE	( CConnectSensor *ConnectSession, BYTE* packet );

	//////////////////////////////////////////////////////////////////////////////////////////
	VOID	onPT_MOBILE_CONNECT				( CConnectSensor *ConnectSession, BYTE* packet );
	VOID	onPT_SCIENCE_OPEN				( CConnectSensor *ConnectSession, BYTE* packet );
	VOID	onPT_SCIENCE_CLEAR				( CConnectSensor *ConnectSession, BYTE* packet );
	VOID	onPT_DIARY_OPEN					( CConnectSensor *ConnectSession, BYTE* packet );

	//////////////////////////////////////////////////////////////////////////////////////////
	VOID	sendPT_GIVE_USB					( CConnectSensor *ConnectSession );		// 아두이노에게(LED)
	VOID	sendPT_OPEN_INTERVIEW			();		// 모바일에게
	VOID	sendPT_GIVE_BINARYCODE			( CConnectSensor *ConnectSession );		// 아두이노에게(LOCK)
	VOID	sendPT_CREATE_BINARYCODE		();		// 모바일에게(다이어리)
};


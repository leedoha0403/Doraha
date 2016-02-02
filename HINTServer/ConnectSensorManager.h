#pragma once
#include <set>

class CConnectSensor;
class ConnectSesnsorManager		: public CMultiThreadSync<ConnectSesnsorManager>
{
public:
	ConnectSesnsorManager(void);
	virtual ~ConnectSesnsorManager(void);

protected:
	SOCKET mListenSocket;
	std::vector<CConnectSensor*>		mConnectSensorVector;
	//std::vector<CConnectSensor*>		mConnectSessionVector;
	
	std::list<CConnectSensor*>		mSocketArduino;
	std::list<CConnectSensor*>		mSocketMobile;

	DWORD	mMaxSensorCount;
	DWORD	mCurrentSensorCount;
	
	INT		mStatus;

public:
	BOOL Begin( DWORD maxUserCount, SOCKET listenSocket );
	BOOL End( VOID );

	BOOL AcceptAll(VOID);
	BOOL WriteAll( BYTE *byte, DWORD dataLength );

	//BOOL WriteArduino(BYTE *byte, DWORD dataLength );
	BOOL WriteMobile(BYTE *byte, DWORD dataLength );

	inline VOID IncreaseConnectedUserCount( VOID )
	{
		InterlockedIncrement( (LONG*)&mCurrentSensorCount);
	}
	inline VOID DecreaseConnectedUserCount( VOID )
	{
		InterlockedDecrement( (LONG*)&mCurrentSensorCount);
	}

public:
	std::vector<CConnectSensor*> GetSensorVector();
	CConnectSensor* FindConnectSensor(std::string sensorKind);

	BOOL SetSocketArduino( CConnectSensor* socket );
	BOOL SetSocketMobile( CConnectSensor* socket );

	std::list<CConnectSensor*> GetSocketArduino();	
	std::list<CConnectSensor*> GetSocketMobile();

	BOOL SetStatus( INT _status );
	INT GetStatus();

	INT	CheckCurrentStatus();						 // 현재 센서들의 상태 판별 및 mStatus 저장 / 반환
};


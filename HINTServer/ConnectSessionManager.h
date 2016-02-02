#pragma once
class CConnctedSession;
class ConnectSessionManager		: public CMultiThreadSync<ConnectSessionManager>
{
public:
	ConnectSessionManager(void);
	virtual ~ConnectSessionManager(void);

private:
	SOCKET mListenSocket;
	std::vector<CConnectedSession*>		mConnectSessionVector;

	DWORD	mMaxSensorCount;
	DWORD	mCurrentSensorCount;

public:
	BOOL Begin( DWORD maxUserCount, SOCKET listenSocket );
	BOOL End( VOID );

	BOOL AcceptAll(VOID);
	BOOL WriteAll( DWORD protocol, BYTE *byte, DWORD dataLength );

	inline VOID IncreaseConnectedUserCount( VOID )
	{
		InterlockedIncrement( (LONG*)&mCurrentSensorCount);
	}
	inline VOID DecreaseConnectedUserCount( VOID )
	{
		InterlockedDecrement( (LONG*)&mCurrentSensorCount);
	}
};


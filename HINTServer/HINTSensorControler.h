#pragma once
class CHINTSensorIocp;

class CHINTSensorControler : public CHINTSensorIocp
{
public:
	CHINTSensorControler(void);
	virtual ~CHINTSensorControler(void);

public:
	BOOL				Begin(VOID);
	BOOL				End(VOID);
	BOOL				Begin( DWORD port );

protected:
	HANDLE				mReciveThread;
public:
	VOID				ReceiveChangeMessage( VOID );

private:
	CHAR				mReceiveData[MAXBUFFERLENGTH];
	BOOL				mInputData;
	
public:
	CHAR*				GetReceiveData()
	{
		return mReceiveData;
	}

public:
	VOID OnIoRead(VOID *object, DWORD dataLength);
	VOID OnIoWrote(VOID *object, DWORD dataLength);
	VOID OnIoConnected(VOID *object);
	VOID OnIoDisconnected(VOID *object);

	CConnectSensor* ConnecetSession(BYTE* sensorType);
	
	//------------------------- Protocol Packet -----------------------------//
	VOID	onPT_SENSOR_VALUE( CConnectSensor *ConnectSession, BYTE* packet );

	//------------------------- Protocol Packet -----------------------------//
	VOID	PT_SENSOR_VALUE_SEND( CHAR* packet );
};
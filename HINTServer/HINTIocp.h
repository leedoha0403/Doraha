#pragma once

class CHINTIocp :	public CIocp
{
public:
	CHINTIocp(void);
	virtual ~CHINTIocp(void);
private:
	CNetworkSession				mListenSession;
	ConnectSessionManager		mConnectedSessionManager;

	HANDLE						mKeepThreadHandle;
	HANDLE						mKeepThreadDestroyEvent;
public:	
	BOOL	Begin(VOID);
	BOOL	End(VOID);
	
	BOOL	Begin( DWORD port );
	
	VOID	KeepThreadCallback(VOID);

protected:
	VOID OnIoRead(VOID *object, DWORD dataLength);
	VOID OnIoWrote(VOID *object, DWORD dataLength);
	VOID OnIoConnected(VOID *object);
	VOID OnIoDisconnected(VOID *object);

//private: //------------------------- Protocol Packet -----------------------------//
//	VOID	onPT_SENSOR_READY( CConnectedSession *ConnectSession, BYTE* packet );
	VOID	onPT_SESSION_VALUE( CConnectedSession *ConnectSession, BYTE* packet );
};


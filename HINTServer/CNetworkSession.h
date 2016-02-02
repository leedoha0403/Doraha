#pragma once

extern class CNetworkSession : public CMultiThreadSync<CNetworkSession>
{
public:
	CNetworkSession(VOID);
	virtual ~CNetworkSession(VOID);

private:
	OVERLAPPED_EX	mAcceptOverlapped;
	OVERLAPPED_EX	mReadOverlapped;
	OVERLAPPED_EX	mWriteOverlapped;

	BYTE			mReadBuffer[MAXBUFFERLENGTH];	//30
	
	SOCKET			mSocket;
public:
	BOOL			Begin(VOID);
	BOOL			End(VOID);
	
	BOOL			Listen(USHORT port, INT backLog);
	BOOL			Accept(SOCKET listenSocket);
	BOOL			Bind(VOID);

	BOOL			GetLocalIP( WCHAR* pIP );
	BOOL			GetLocalPort(VOID);

	BOOL			Write(BYTE *data, DWORD &dataLength);

	
	BOOL			InitializeReadForIocp(VOID);
	BOOL			ReadForIocp(BYTE *data, DWORD &dataLength);
	BOOL			ReadForEventSelect(BYTE *data, DWORD &dataLength);

	BOOL			Connect(LPSTR address, USHORT port );

	SOCKET			GetSocket(VOID);
	BOOL			GetRemoteAddressAfterAccept(LPTSTR remoteAddress, USHORT &remotePort);

	SOCKET*			GetSocketPointer(VOID)
	{
					return &mSocket;
	}
};
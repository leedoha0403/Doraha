#pragma once
#pragma warning(disable:4996)		// strncpy :: C4996 Error

typedef struct _QUEUE_DATA
{
	VOID	*Object;
	BYTE	Data[MAXBUFFERLENGTH];
	DWORD	DataLength;

	CHAR	RemoteAddress[32];
	USHORT	RemotePort;

	DWORD	Protocol;
} QUEUE_DATA;

extern class CCircularQueue : public CMultiThreadSync<CCircularQueue>
{
public:
	CCircularQueue(VOID);
	~CCircularQueue(VOID);

private:
	QUEUE_DATA	mQueue[MAXQUEUELENGTH];
	DWORD		mQueueHead;
	DWORD		mQueueTail;

public:
	BOOL		Begin(VOID);
	BOOL		End(VOID);

	BYTE*		Push(VOID *object, BYTE *data, DWORD dataLength);
	BYTE*		Push(VOID *object, BYTE *data, DWORD dataLength, LPCSTR remoteAddress, USHORT remotePort);
	BYTE*		Push(VOID *object, DWORD protocol, BYTE *data, DWORD dataLength);
	BYTE*		Push(VOID *object, DWORD protocol, BYTE *data, DWORD dataLength, LPCSTR remoteAddress, USHORT remotePort);

	BOOL		Pop(VOID **object, BYTE *data, DWORD &dataLength);
	BOOL		Pop(VOID **object, BYTE *data, DWORD &dataLength, LPSTR remoteAddress, USHORT &remotePort);
	BOOL		Pop(VOID **object, DWORD &protocol, BYTE *data, DWORD &dataLength);
	BOOL		Pop(VOID **object, DWORD &protocol, BYTE *data, DWORD &dataLength, LPSTR remoteAddress, USHORT &remotePort);
	BOOL		Pop(VOID);

	BOOL		IsEmpty(VOID);
};
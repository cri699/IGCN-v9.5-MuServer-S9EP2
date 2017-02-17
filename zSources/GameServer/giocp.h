// Giocp.h
//------------------------------------------
// Decompiled by Deathway
// Date : 2007-03-09
//------------------------------------------

#ifndef GIOCP_H
#define GIOCP_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define HIDE_ERRORL10
#define MAX_EXSENDBUF_SIZE 14048
#define MAX_IO_BUFFER_SIZE		32768
#define MAX_IO_THREAD_HANDLES	16

struct _PER_IO_CONTEXT
{ 
	WSAOVERLAPPED Overlapped; // 0
	WSABUF wsabuf; 
	unsigned char Buffer[MAX_IO_BUFFER_SIZE]; // 1C
	unsigned char BufferSecond[MAX_IO_BUFFER_SIZE]; // 178C
	int nSecondOfs; // 2EFC
	int nTotalBytes;	// 2F00
	int nSentBytes; // 2F04
	int IOOperation; // 2F08
	int nWaitIO; // 2F0C

}; 


struct _PER_SOCKET_CONTEXT
{
	SOCKET m_socket;	// 0
	int nIndex;	// 4
	_PER_IO_CONTEXT IOContext[2];	// 8
	int dwIOCount;	// 5E28
}; 

class CIOCP
{
public:

	void GiocpInit();
	void GiocpDelete();
	bool CreateGIocp(int server_port);
	void DestroyGIocp();
	bool CreateListenSocket();
	bool RecvDataParse(_PER_IO_CONTEXT * lpIOContext, int uIndex);
	bool DataSend(int aIndex, unsigned char* lpMsg, DWORD dwSize, bool Encrypt = true);
	bool IoSendSecond(_PER_SOCKET_CONTEXT * lpPerSocketContext);
	bool IoMoreSend(_PER_SOCKET_CONTEXT * lpPerSocketContext);
	bool UpdateCompletionPort(SOCKET sd, int ClientIndex, BOOL bAddToList);
	void CloseClient(_PER_SOCKET_CONTEXT * lpPerSocketContext, int result);
	void CloseClient(int index);
	void ResponErrorCloseClient(int index);

private:

	unsigned char* ExSendBuf;
	int g_ServerPort;
	HANDLE g_IocpThreadHandle;
	HANDLE g_ThreadHandles[MAX_IO_THREAD_HANDLES];
	CRITICAL_SECTION criti;
	enum SOCKET_FLAG;
	DWORD g_dwThreadCount;
	HANDLE g_CompletionPort;	
	SOCKET g_Listen;

	static void IocpServerWorkerEP(void *pThis)
	{
			CIOCP *pt = (CIOCP*)pThis;
			pt->IocpServerWorker(pThis);
	}

	static void ServerWorkerThreadEP(void *pThis)
	{
			CIOCP *pt = (CIOCP*)pThis;
			pt->ServerWorkerThread();
	}

	DWORD WINAPI IocpServerWorker(void * p);
	DWORD WINAPI ServerWorkerThread();

};

extern CIOCP IOCP;

#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


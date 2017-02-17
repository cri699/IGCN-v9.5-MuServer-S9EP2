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

#include "CQueue.h"

enum eSERVER_TYPE
{
	ST_NONE = 0,
	ST_JOINSERVER = 1,
	ST_DATASERVER = 2,
	ST_EXDATASERVER = 3,
};

#define MAX_IO_BUFFER_SIZE		100000
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

typedef struct tagIocpServerParameter
{
	DWORD dwServerCount;
	eSERVER_TYPE eServerType;

} IocpServerParameter;

struct _PER_SOCKET_CONTEXT
{
	SOCKET m_socket;	// 0
	int nIndex;	// 4
	_PER_IO_CONTEXT IOContext[2];	// 8
	int dwIOCount;	// 5E28
}; 

struct PacketQueue
{
	PacketQueue()
	{
		this->nSize = 0;
		this->headcode = 0;
		this->uindex = -1;
		this->iSessionId = -1;
	}

	boost::shared_ptr<unsigned char[]> pData;
	unsigned short nSize;
	unsigned char headcode;
	int uindex;
	int iSessionId;
};

extern CRITICAL_SECTION scriti;

BOOL IocpServerStart();
BOOL CreateGIocp(int server_port, eSERVER_TYPE eServerType);
void DestroyGIocp();
BOOL CreateListenSocket(DWORD dwServerCountIndex);
unsigned long __stdcall IocpServerWorker(DWORD pIocpServerParameter);
unsigned long __stdcall ServerWorkerThread(HANDLE CompletionPortID);
BOOL RecvDataParse(_PER_IO_CONTEXT * lpIOContext, int uIndex);
BOOL DataSend(int aIndex, unsigned char* lpMsg, DWORD dwSize, char* szFunction);
BOOL IoSendSecond(_PER_SOCKET_CONTEXT * lpPerSocketContext);
BOOL IoMoreSend(_PER_SOCKET_CONTEXT * lpPerSocketContext);
BOOL UpdateCompletionPort(SOCKET sd, int ClientIndex, DWORD dwServerTypeCount);
void CloseClient(_PER_SOCKET_CONTEXT * lpPerSocketContext, int result);
void CloseClient(int index);
void ResponErrorCloseClient(int index);

extern CQueue m_DSQueue1;
extern CQueue m_DSQueue2;
extern CQueue m_DSQueue3;
extern CQueue m_DSQueue4;
extern CQueue m_DSQueue5;
extern CQueue m_JSQueue;
extern CQueue m_EXDSQueue;

#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


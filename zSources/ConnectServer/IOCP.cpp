// Giocp.cpp
//------------------------------------------
// Decompiled by Deathway
// Date : 2007-03-09
//------------------------------------------

// GS-N 0.99.60T 0x00473020 - Status : Completed :)
//	GS-N	1.00.18	JPN	0x00489FD0	-	Completed
#include "stdafx.h"
#include "IOCP.h"
#include "TLog.h"
#include "Engine.h"
#include "protocol.h"
#include "ConnectServer.h"

void CIOCP::GiocpInit()
{
	ExSendBuf=new unsigned char[MAX_EXSENDBUF_SIZE];
}

void CIOCP::GiocpDelete()
{
	delete[] ExSendBuf;
}

bool CIOCP::CreateGIocp(int server_port)
{
	unsigned long ThreadID;
	
	g_ServerPort=server_port;

	InitializeCriticalSection(&criti);

	g_IocpThreadHandle=CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)IocpServerWorkerEP, this, 0, &ThreadID);

	if ( g_IocpThreadHandle == 0 )
	{
		g_Log.AddC(TColor::Red, "CreateThread() failed with error %d", GetLastError());
		return 0;
	}
	else
	{
		return 1;
	}	
}

void CIOCP::DestroyGIocp()
{
	closesocket(g_Listen);

	for (DWORD dwCPU=0; dwCPU < g_dwThreadCount;dwCPU++ )
	{
		TerminateThread( g_ThreadHandles[dwCPU] , 0);
	}

	TerminateThread(g_IocpThreadHandle, 0);

	if ( g_CompletionPort != NULL )
	{
		CloseHandle(g_CompletionPort);
		g_CompletionPort=NULL;
	} 
	
}

bool CIOCP::CreateListenSocket()
{
	sockaddr_in InternetAddr;
	int nRet;

	g_Listen=WSASocket(AF_INET, SOCK_STREAM, IPPROTO_IP, NULL, 0, WSA_FLAG_OVERLAPPED);

	if ( g_Listen == -1 )
	{
		g_Log.AddC(TColor::Red, "WSASocket() failed with error %d", WSAGetLastError() );
		return 0;
	}
	else
	{
		InternetAddr.sin_family=AF_INET;
		InternetAddr.sin_addr.S_un.S_addr=htonl(0);
		InternetAddr.sin_port=htons(g_ServerPort);
		nRet=bind(g_Listen, (sockaddr*)&InternetAddr, 16);
		
		if ( nRet == -1 )
		{
			g_Log.MsgBox("BIND ERROR - Port in use already");
			return 0 ;
		}
		else
		{
			nRet=listen(g_Listen, 5);
			if (nRet == -1)
			{
				g_Log.AddC(TColor::Red, "listen() failed with error %d", WSAGetLastError());
				return 0;
			}
			else
			{
				return 1;
			}
		}
	} 
}

DWORD CIOCP::IocpServerWorker(void * p)
{
	SYSTEM_INFO SystemInfo;
	DWORD ThreadID;
	SOCKET Accept;
	int nRet;
	int ClientIndex;
	sockaddr_in cAddr;
	in_addr cInAddr;
	int cAddrlen;
	_PER_SOCKET_CONTEXT * lpPerSocketContext;
	int RecvBytes;
	unsigned long Flags;
	
	

	cAddrlen=16;
	lpPerSocketContext=0;
	Flags=0;

	GetSystemInfo(&SystemInfo);

	g_dwThreadCount = SystemInfo.dwNumberOfProcessors * 2;

	if ( g_dwThreadCount >= MAX_IO_THREAD_HANDLES )
	{
		g_dwThreadCount = MAX_IO_THREAD_HANDLES;
		g_Log.AddC(TColor::Orange, "[NOTICE]: IOCP Thread Handles set to 16");
	}

	__try
	{

		g_CompletionPort=CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);

		if ( g_CompletionPort == NULL )
		{
			g_Log.AddC(TColor::Red, "CreateIoCompletionPort failed with error: %d", GetLastError());
			__leave;
		}

		for (int n = 0; n<g_dwThreadCount; n++)
		{
			HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ServerWorkerThreadEP, this, 0, &ThreadID);

			if ( hThread == 0 )
			{
				g_Log.AddC(TColor::Red, "CreateThread() failed with error %d", GetLastError() );
				__leave;
			}

			g_ThreadHandles[n] = hThread;

			CloseHandle(hThread);
		}

		if ( CreateListenSocket() == 0 )
		{

		}
		else
		{
			while ( true )
			{
				Accept = WSAAccept(g_Listen, (sockaddr*)&cAddr, &cAddrlen, NULL, 0 );

				if ( Accept == -1 )
				{
					EnterCriticalSection(&criti);
					g_Log.AddC(TColor::Red, "WSAAccept() failed with error %d", WSAGetLastError() );
					LeaveCriticalSection(&criti);
					continue;
				}

				EnterCriticalSection(&criti);
				memcpy(&cInAddr, &cAddr.sin_addr  , sizeof(cInAddr) );

				ClientIndex = UserAdd(Accept, inet_ntoa(cInAddr) );

				if ( ClientIndex == -1 )
				{
					//g_Log.AddC(TColor::Red, "error-L2: ClientIndex = -1");
					closesocket(Accept);
					LeaveCriticalSection(&criti);
					continue;
				}

				if (UpdateCompletionPort(Accept, ClientIndex, 1) == 0 )
				{
					g_Log.AddC(TColor::Red, "error-L1: %d %d CreateIoCompletionPort failed with error %d", Accept, ClientIndex, GetLastError() );
					closesocket(Accept);
					LeaveCriticalSection(&criti);
					continue;
				}

				
				memset(&Users[ClientIndex].PerSocketContext->IOContext[0].Overlapped, 0, sizeof(WSAOVERLAPPED));
				memset(&Users[ClientIndex].PerSocketContext->IOContext[1].Overlapped, 0, sizeof(WSAOVERLAPPED));

				Users[ClientIndex].PerSocketContext->IOContext[0].wsabuf.buf = (char*)&Users[ClientIndex].PerSocketContext->IOContext[0].Buffer;
				Users[ClientIndex].PerSocketContext->IOContext[0].wsabuf.len = MAX_IO_BUFFER_SIZE;
				Users[ClientIndex].PerSocketContext->IOContext[0].nTotalBytes = 0;
				Users[ClientIndex].PerSocketContext->IOContext[0].nSentBytes = 0;
				Users[ClientIndex].PerSocketContext->IOContext[0].nWaitIO = 0;
				Users[ClientIndex].PerSocketContext->IOContext[0].nSecondOfs = 0;
				Users[ClientIndex].PerSocketContext->IOContext[0].IOOperation = 0;
				Users[ClientIndex].PerSocketContext->IOContext[1].wsabuf.buf = (char*)Users[ClientIndex].PerSocketContext->IOContext[0].Buffer;
				Users[ClientIndex].PerSocketContext->IOContext[1].wsabuf.len = MAX_IO_BUFFER_SIZE;
				Users[ClientIndex].PerSocketContext->IOContext[1].nTotalBytes = 0;
				Users[ClientIndex].PerSocketContext->IOContext[1].nSentBytes = 0;
				Users[ClientIndex].PerSocketContext->IOContext[1].nWaitIO = 0;
				Users[ClientIndex].PerSocketContext->IOContext[1].nSecondOfs = 0;
				Users[ClientIndex].PerSocketContext->IOContext[1].IOOperation = 1;
				Users[ClientIndex].PerSocketContext->m_socket = Accept;
				Users[ClientIndex].PerSocketContext->nIndex = ClientIndex;

				nRet = WSARecv(Accept, &Users[ClientIndex].PerSocketContext->IOContext[0].wsabuf , 1, (unsigned long*)&RecvBytes, &Flags, 
						&Users[ClientIndex].PerSocketContext->IOContext[0].Overlapped, NULL);

				if ( nRet == -1 )
				{
					if ( WSAGetLastError() != WSA_IO_PENDING )
					{
						g_Log.AddC(TColor::Red, "error-L1: WSARecv() failed with error %d", WSAGetLastError() );
						Users[ClientIndex].PerSocketContext->IOContext[0].nWaitIO = 4;
						CloseClient(Users[ClientIndex].PerSocketContext, 0);
						LeaveCriticalSection(&criti);
						continue;
					}
				}

				Users[ClientIndex].PerSocketContext->IOContext[0].nWaitIO  = 1;
				Users[ClientIndex].PerSocketContext->dwIOCount++;

				LeaveCriticalSection(&criti);
				SCConnectResultSend(ClientIndex, 1);
			}
		}
	}
	__finally
	{
		
		if ( g_CompletionPort != NULL )
		{
			for ( int i = 0 ; i < g_dwThreadCount ; i++ )
			{
				PostQueuedCompletionStatus( g_CompletionPort , 0, 0, 0);
			}
		}

		if ( g_CompletionPort != NULL )
		{
			CloseHandle(g_CompletionPort);
			g_CompletionPort = NULL;
		}
		if ( g_Listen != INVALID_SOCKET )
		{
			closesocket( g_Listen);
			g_Listen = INVALID_SOCKET;
		}
	}

	return 1;
		
}

DWORD CIOCP::ServerWorkerThread()
{
	HANDLE CompletionPort;
	DWORD dwIoSize;
	unsigned long RecvBytes;
	unsigned long Flags;
	DWORD dwSendNumBytes;
	BOOL bSuccess;
	int nRet;
#ifdef _WIN64
	ULONGLONG ClientIndex;
#else
	ULONG ClientIndex;
#endif
	_PER_SOCKET_CONTEXT * lpPerSocketContext;
	LPOVERLAPPED lpOverlapped;
	_PER_IO_CONTEXT * lpIOContext;
	
	

	CompletionPort=this->g_CompletionPort;
	dwSendNumBytes=0;
	bSuccess=0;
	lpPerSocketContext=0;
	lpOverlapped=0;
	lpIOContext=0;
	
	while ( true )
	{
		bSuccess=GetQueuedCompletionStatus( CompletionPort, &dwIoSize, &ClientIndex, &lpOverlapped, -1); // WAIT_FOREVER

		if (bSuccess == 0)
		{
			if (lpOverlapped != 0)
			{
				int aError = GetLastError();
				if ( (aError != ERROR_NETNAME_DELETED) && (aError != ERROR_CONNECTION_ABORTED) && (aError != ERROR_OPERATION_ABORTED) && (aError != ERROR_SEM_TIMEOUT) && (aError != ERROR_HOST_UNREACHABLE) )
				{
					EnterCriticalSection(&criti);
					g_Log.AddC(TColor::Red, "Error Thread: GetQueueCompletionStatus( %d )", GetLastError());
					LeaveCriticalSection(&criti);
					return 0;
				}
			}
		}

		EnterCriticalSection(&criti);

		if (ClientIndex < 0 || ClientIndex >= MAX_USER)
		{
			LeaveCriticalSection(&criti);
			continue;
		}

		lpPerSocketContext=Users[ClientIndex].PerSocketContext;
		lpPerSocketContext->dwIOCount --;
				
		if ( dwIoSize == 0 )
		{
			CloseClient(lpPerSocketContext, 0);
			LeaveCriticalSection(&criti);
			continue;
		}

		lpIOContext = (_PER_IO_CONTEXT *)lpOverlapped;

		if ( lpIOContext == 0 )
		{
			continue;
		}

		if ( lpIOContext->IOOperation == 1 )
		{
			lpIOContext->nSentBytes += dwIoSize;

			if ( lpIOContext->nSentBytes >= lpIOContext->nTotalBytes )
			{
				lpIOContext->nWaitIO = 0;
						
				if ( lpIOContext->nSecondOfs > 0)
				{
					IoSendSecond(lpPerSocketContext);
				}
			}
			else
			{
				IoMoreSend(lpPerSocketContext);
			}
			
		}
		else if ( lpIOContext->IOOperation == 0 )
		{
			RecvBytes = 0;
			lpIOContext->nSentBytes += dwIoSize;

			if ( RecvDataParse(lpIOContext, lpPerSocketContext->nIndex ) == 0 )
			{
				g_Log.AddC(TColor::Red, "error-L1: Socket Header error %d, %d", WSAGetLastError(), lpPerSocketContext->nIndex);
				CloseClient(lpPerSocketContext, 0);
				LeaveCriticalSection(&criti);
				continue;
			}

			lpIOContext->nWaitIO = 0;
			Flags = 0;
			memset(&lpIOContext->Overlapped, 0, sizeof (WSAOVERLAPPED));
			lpIOContext->wsabuf.len = MAX_IO_BUFFER_SIZE - lpIOContext->nSentBytes;
			lpIOContext->wsabuf.buf = (char*)&lpIOContext->Buffer[lpIOContext->nSentBytes];
			lpIOContext->IOOperation = 0;

			nRet = WSARecv(lpPerSocketContext->m_socket, &lpIOContext->wsabuf, 1, &RecvBytes, &Flags,
						&lpIOContext->Overlapped, NULL);

			if ( nRet == -1 )
			{
				if ( WSAGetLastError() != WSA_IO_PENDING)
				{
					g_Log.AddC(TColor::Red, "WSARecv() failed with error %d", WSAGetLastError() );
					CloseClient(lpPerSocketContext, 0);
					LeaveCriticalSection(&criti);
					continue;
				}
			}

			lpPerSocketContext->dwIOCount ++;
			lpIOContext->nWaitIO = 1;
		}
		LeaveCriticalSection(&criti);
		

	}

	return 1;
}


bool CIOCP::RecvDataParse(_PER_IO_CONTEXT * lpIOContext, int uIndex)	
{
	unsigned char* recvbuf;
	int lOfs;
	int size;
	BYTE headcode;
	BYTE xcode;

	if ( lpIOContext->nSentBytes < 3 )
	{
		return true;
	}

	lOfs=0;
	size=0;
	xcode=0;
	recvbuf = lpIOContext->Buffer;
	
	while ( true )
	{
		if ( recvbuf[lOfs] == 0xC1 ||
			 recvbuf[lOfs] == 0xC3 )
		{
			unsigned char * pBuf;

			pBuf = &recvbuf[lOfs];
			size = pBuf[1];
			headcode = pBuf[2];
			xcode = recvbuf[lOfs];
		}
		else if ( recvbuf[lOfs] == 0xC2 ||
			      recvbuf[lOfs] == 0xC4 )
		{
			unsigned char * pBuf;

			pBuf = &recvbuf[lOfs];
			size = pBuf[1] * 256;
			size |= pBuf[2];
			headcode = pBuf[3];
			xcode = recvbuf[lOfs];
		}

		else
		{
			g_Log.AddC(TColor::Red, "error-L1: Header error (%s %d)lOfs:%d, size:%d", __FILE__, __LINE__, lOfs, lpIOContext->nSentBytes);
			lpIOContext->nSentBytes = 0;
			return false;
		}

		if ( size <= 0 )
		{
			g_Log.AddC(TColor::Red, "error-L1: size %d",
				size);

			return false;
		}

		if ( size <= lpIOContext->nSentBytes )
		{
			Users[uIndex].PacketCount++;

			if (Users[uIndex].PacketCount >= g_MaxPacketPerSec && strcmp(Users[uIndex].IP, g_WhiteListIP))
			{
				g_Log.AddC(TColor::Red, "[ANTI-FLOOD] Packets Per Second: %d / %d, IP: %d", Users[uIndex].PacketCount, g_MaxPacketPerSec, Users[uIndex].IP);
				this->CloseClient(uIndex);
				return false;
			}

			CSProtocolCore(headcode, &recvbuf[lOfs], size, uIndex, 0, 0);

			lOfs += size;
			lpIOContext->nSentBytes  -= size;

			if ( lpIOContext->nSentBytes <= 0 )
			{
				break;
			}
		}
		else if ( lOfs > 0 )
		{
			if ( lpIOContext->nSentBytes < 1 )
			{
				g_Log.AddC(TColor::Red, "error-L1: recvbuflen 1 %s %d", __FILE__, __LINE__);
				break;
			}

			if ( lpIOContext->nSentBytes < MAX_IO_BUFFER_SIZE ) 
			{
				memcpy(recvbuf, &recvbuf[lOfs], lpIOContext->nSentBytes);
				g_Log.AddC(TColor::Red, "Message copy %d", lpIOContext->nSentBytes);
			}
			break;
		
		}
		else
		{
			break;
		}
		
	}

	return true;
}

bool CIOCP::DataSend(int aIndex, unsigned char* lpMsg, DWORD dwSize, bool Encrypt)
{
	unsigned long SendBytes;
	_PER_SOCKET_CONTEXT * lpPerSocketContext;
	unsigned char * SendBuf;
	BYTE BUFFER[65535];
	

	if ( aIndex < 0 )
	{
		return true;
	}

	EnterCriticalSection(&criti);

	if ( ((aIndex < 0)? FALSE : (aIndex > 1000-1)? FALSE : TRUE )  == FALSE )
	{
		g_Log.AddC(TColor::Red, "error-L2: Index(%d) %x %x %x ", dwSize, lpMsg[0], lpMsg[1], lpMsg[2]);
		LeaveCriticalSection(&criti);
		return false;
	}

	SendBuf = lpMsg;

	if ( Users[aIndex].ConnectionState == 0  )
	{
		LeaveCriticalSection(&criti);
		return false;
	}

	lpPerSocketContext = Users[aIndex].PerSocketContext;

	if ( dwSize > sizeof(lpPerSocketContext->IOContext[0].Buffer))
	{
		g_Log.AddC(TColor::Red, "Error: Max msg(%d) %s %d", dwSize, __FILE__, __LINE__);
		CloseClient(aIndex);
		LeaveCriticalSection(&criti);
		return false;
	}

	_PER_IO_CONTEXT  * lpIoCtxt;

	lpIoCtxt = &lpPerSocketContext->IOContext[1];

	if ( lpIoCtxt->nWaitIO > 0 )
	{
		if ( ( lpIoCtxt->nSecondOfs + dwSize ) > MAX_IO_BUFFER_SIZE-1 )
		{
			g_Log.AddC(TColor::Red, "(%d)error-L2: MAX BUFFER OVER %d %d %d", aIndex, lpIoCtxt->nTotalBytes, lpIoCtxt->nSecondOfs, dwSize);
			lpIoCtxt->nWaitIO = 0;
			CloseClient(aIndex);
			LeaveCriticalSection(&criti);
			return true;
		}

		memcpy( &lpIoCtxt->BufferSecond[lpIoCtxt->nSecondOfs], SendBuf, dwSize);
		lpIoCtxt->nSecondOfs += dwSize;
		LeaveCriticalSection(&criti);
		return true;
	}

	lpIoCtxt->nTotalBytes = 0;
	
	if ( lpIoCtxt->nSecondOfs > 0 )
	{
		memcpy(lpIoCtxt->Buffer, lpIoCtxt->BufferSecond, lpIoCtxt->nSecondOfs);
		lpIoCtxt->nTotalBytes = lpIoCtxt->nSecondOfs;
		lpIoCtxt->nSecondOfs = 0;
	}

	if ( (lpIoCtxt->nTotalBytes+dwSize) > MAX_IO_BUFFER_SIZE-1 )
	{
		g_Log.AddC(TColor::Red, "(%d)error-L2: MAX BUFFER OVER %d %d", aIndex, lpIoCtxt->nTotalBytes, dwSize);
		lpIoCtxt->nWaitIO = 0;
		CloseClient(aIndex);
		LeaveCriticalSection(&criti);
		return false;
	}

	memcpy( &lpIoCtxt->Buffer[lpIoCtxt->nTotalBytes], SendBuf, dwSize);
	lpIoCtxt->nTotalBytes += dwSize;
	lpIoCtxt->wsabuf.buf = (char*)&lpIoCtxt->Buffer;
	lpIoCtxt->wsabuf.len = lpIoCtxt->nTotalBytes;
	lpIoCtxt->nSentBytes = 0;
	lpIoCtxt->IOOperation = 1;
	

	if ( WSASend( Users[aIndex].socket, &lpIoCtxt->wsabuf , 1, &SendBytes, 0, &lpIoCtxt->Overlapped, NULL) == -1 )
	{

		if ( WSAGetLastError() != WSA_IO_PENDING )	
		{
			lpIoCtxt->nWaitIO = 0;
			

			if ( lpIoCtxt->wsabuf.buf[0] == 0xC1 )
			{
				g_Log.AddC(TColor::Red, "(%d)WSASend(%d) failed with error [%x][%x] %d %s ", __LINE__, aIndex, (BYTE)lpIoCtxt->wsabuf.buf[0],
					(BYTE)lpIoCtxt->wsabuf.buf[2], WSAGetLastError(), Users[aIndex].IP);
			}
			else if ( lpIoCtxt->wsabuf.buf[0] == 0xC2 )
			{
				g_Log.AddC(TColor::Red, "(%d)WSASend(%d) failed with error [%x][%x] %d %s ", __LINE__, aIndex, (BYTE)lpIoCtxt->wsabuf.buf[0],
					(BYTE)lpIoCtxt->wsabuf.buf[3], WSAGetLastError(), Users[aIndex].IP);
			}
			CloseClient(aIndex);
			LeaveCriticalSection(&criti);
			return false;
		}
	}
	else
	{
		lpPerSocketContext->dwIOCount ++;
	}
	
	
	lpIoCtxt->nWaitIO = 1;
	LeaveCriticalSection(&criti);
	return true;
}



bool CIOCP::IoSendSecond(_PER_SOCKET_CONTEXT * lpPerSocketContext)
{
	unsigned long SendBytes;
	int aIndex;
	_PER_IO_CONTEXT * lpIoCtxt;

	EnterCriticalSection(&criti);
	aIndex = lpPerSocketContext->nIndex;
	lpIoCtxt = &lpPerSocketContext->IOContext[1];

	if ( lpIoCtxt->nWaitIO > 0 )
	{
		LeaveCriticalSection(&criti);
		return false;
	}

	lpIoCtxt->nTotalBytes = 0;
	if ( lpIoCtxt->nSecondOfs > 0 )
	{
		memcpy(lpIoCtxt->Buffer, lpIoCtxt->BufferSecond, lpIoCtxt->nSecondOfs);
		lpIoCtxt->nTotalBytes = lpIoCtxt->nSecondOfs;
		lpIoCtxt->nSecondOfs = 0;
	}
	else
	{
		LeaveCriticalSection(&criti);
		return false;
	}

	lpIoCtxt->wsabuf.buf = (char*)&lpIoCtxt->Buffer;
	lpIoCtxt->wsabuf.len = lpIoCtxt->nTotalBytes;
	lpIoCtxt->nSentBytes = 0;
	lpIoCtxt->IOOperation = 1;

	if ( WSASend(Users[aIndex].socket, &lpIoCtxt->wsabuf, 1, &SendBytes, 0, &lpIoCtxt->Overlapped, NULL) == -1 )
	{
		if ( WSAGetLastError() != WSA_IO_PENDING )
		{
			g_Log.AddC(TColor::Red, "WSASend(%d) failed with error %d %s ", __LINE__, WSAGetLastError(), Users[aIndex].IP);
			CloseClient(aIndex);
			LeaveCriticalSection(&criti);
			return false;
		}
	}
	else
	{
		lpPerSocketContext->dwIOCount ++;
	}
	
	lpIoCtxt->nWaitIO = 1;
	LeaveCriticalSection(&criti);
	
	return true;
}

bool CIOCP::IoMoreSend(_PER_SOCKET_CONTEXT * lpPerSocketContext)
{
	unsigned long SendBytes;
	int aIndex;
	_PER_IO_CONTEXT * lpIoCtxt;

	EnterCriticalSection(&criti);
	aIndex = lpPerSocketContext->nIndex;
	lpIoCtxt = &lpPerSocketContext->IOContext[1];

	if ( (lpIoCtxt->nTotalBytes - lpIoCtxt->nSentBytes) < 0 )
	{
		LeaveCriticalSection(&criti);
		return false;
	}

	lpIoCtxt->wsabuf.buf = (char*)&lpIoCtxt->Buffer[lpIoCtxt->nSentBytes];
	lpIoCtxt->wsabuf.len = lpIoCtxt->nTotalBytes - lpIoCtxt->nSentBytes;
	lpIoCtxt->IOOperation = 1;

	if ( WSASend(Users[aIndex].socket, &lpIoCtxt->wsabuf, 1, &SendBytes, 0, &lpIoCtxt->Overlapped, NULL) == -1 )
	{
		if ( WSAGetLastError() != WSA_IO_PENDING )
		{
			g_Log.AddC(TColor::Red, "WSASend(%d) failed with error %d %s ", __LINE__, WSAGetLastError(), Users[aIndex].IP);
			CloseClient(aIndex);
			LeaveCriticalSection(&criti);
			return false;
		}
	}
	else
	{
		lpPerSocketContext->dwIOCount ++;
	}
	
	
	lpIoCtxt->nWaitIO = 1;
	LeaveCriticalSection(&criti);
	return true;
}

bool CIOCP::UpdateCompletionPort(SOCKET sd, int ClientIndex, BOOL bAddToList)
{
	_PER_SOCKET_CONTEXT * lpPerSocketContext = NULL;

	HANDLE cp = CreateIoCompletionPort((HANDLE) sd, g_CompletionPort, ClientIndex, 0);

	if ( cp == 0 )
	{
		g_Log.AddC(TColor::Red, "CreateIoCompletionPort: %d", GetLastError() );
		return FALSE;
	}

	Users[ClientIndex].PerSocketContext->dwIOCount = 0;
	return TRUE;
}

void CIOCP::CloseClient(_PER_SOCKET_CONTEXT * lpPerSocketContext, int result)
{
	int index = -1;
	index = lpPerSocketContext->nIndex ;

	if ( index >= 0 && index < 1000 )
	{
		if ( Users[index].socket != INVALID_SOCKET )
		{
			if (closesocket(Users[index].socket) == -1 )
			{
				if ( WSAGetLastError() != WSAENOTSOCK )
				{
					return;
				}
			}
				Users[index].socket = INVALID_SOCKET;
		}
			UserDelete(index);
	
	}
}


void CIOCP::CloseClient(int index)
{
	if ( index < 0 || index > 1000-1 )
	{
		g_Log.AddC(TColor::Red, "error-L1 : CloseClient index error");
		return;
	}

	if ( Users[index].ConnectionState == 0 )
	{
		g_Log.AddC(TColor::Red, "error-L1 : CloseClient connect error");
		return;
	}

	EnterCriticalSection(&criti);

	if ( Users[index].socket != INVALID_SOCKET )
	{
		closesocket(Users[index].socket );
		Users[index].socket = INVALID_SOCKET;
	}

	else
	{
	//	closesocket(Users[index].m_socket );
	}

	LeaveCriticalSection(&criti);
}

void CIOCP::ResponErrorCloseClient(int index)
{
	if ( index < 0 || index > 1000-1 )
	{
		g_Log.AddC(TColor::Red, "error-L1 : CloseClient index error");
		return;
	}

	if ( Users[index].ConnectionState == 0 )
	{
		g_Log.AddC(TColor::Red, "error-L1 : CloseClient connect error");
		return;
	}

	EnterCriticalSection(&criti);
	closesocket(Users[index].socket);
	Users[index].socket = INVALID_SOCKET;
	UserDelete(index);
	LeaveCriticalSection(&criti);
}


//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


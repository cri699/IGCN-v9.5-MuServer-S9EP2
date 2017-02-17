// Giocp.cpp
//------------------------------------------
// Decompiled by Deathway
// Date : 2007-03-09
//------------------------------------------

// GS-N 0.99.60T 0x00473020 - Status : Completed :)
//	GS-N	1.00.18	JPN	0x00489FD0	-	Completed
#include "stdafx.h"
#include "giocp.h"
#include "TLog.h"
#include "DataServer.h"
#include "ServerEngine.h"
#include "DelayHandler.h"

#define szModule "GIocp"

DWORD g_dwServerCount=0;
HANDLE g_CompletionPort[MAX_SERVER_TYPE];
DWORD g_dwThreadCount[MAX_SERVER_TYPE];
CRITICAL_SECTION criti[MAX_SERVER_TYPE];
CRITICAL_SECTION scriti;
HANDLE g_ThreadHandles[MAX_SERVER_TYPE][MAX_IO_THREAD_HANDLES];
int g_ServerPort[MAX_SERVER_TYPE];
HANDLE g_IocpThreadHandle[MAX_SERVER_TYPE];
SOCKET g_Listen[MAX_SERVER_TYPE] = {INVALID_SOCKET,
									INVALID_SOCKET,
									INVALID_SOCKET,
									INVALID_SOCKET};

CQueue m_DSQueue1;
CQueue m_DSQueue2;
CQueue m_DSQueue3;
CQueue m_DSQueue4;
CQueue m_DSQueue5;
CQueue m_JSQueue;
CQueue m_EXDSQueue;

DWORD WINAPI ServerRecvWorkerThread_DS_Q1(DWORD p);
DWORD WINAPI ServerRecvWorkerThread_DS_Q2(DWORD p);
DWORD WINAPI ServerRecvWorkerThread_DS_Q3(DWORD p);
DWORD WINAPI ServerRecvWorkerThread_DS_Q4(DWORD p);
DWORD WINAPI ServerRecvWorkerThread_DS_Q5(DWORD p);
DWORD WINAPI ServerRecvWorkerThread_JS(DWORD p);
DWORD WINAPI ServerRecvWorkerThread_EXDS(DWORD p);

BOOL IocpServerStart()
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2,2), &wsaData);

	InitializeCriticalSection(&scriti);

	if(g_DSMode == FALSE)
	{
		g_Log.Add("[IOCP] Using normal functionality");

		if (g_UseJoinServer == TRUE && CreateGIocp(g_JoinServerListPort, ST_JOINSERVER) == TRUE )
			g_Log.Add("[IOCP] ST_JOINSERVER Start! PORT [ %d ]", g_JoinServerListPort);

		Sleep(100);

		if (g_UseDataServer == TRUE && CreateGIocp(g_DataServerListPort, ST_DATASERVER) == TRUE )
			g_Log.Add("[IOCP] ST_DATASERVER Start! PORT [ %d ]", g_DataServerListPort);

		Sleep(100);

		if (g_UseExDataServer == TRUE && CreateGIocp(g_ExDataServerListPort, ST_EXDATASERVER) == TRUE )
			g_Log.Add("[IOCP] ST_EXDATASERVER Start! PORT [ %d ]", g_ExDataServerListPort);

		Sleep(100);
	}

	else
	{
		if (g_UseDataServer == TRUE && CreateGIocp(g_DataServerListPort, ST_DATASERVER) == TRUE )
			g_Log.Add("[IOCP] ST_DATASERVER Start! PORT [ %d ]", g_DataServerListPort);

		Sleep(100);

		g_Log.Add("[IOCP] Using DataServer-ONLY functionality");

	}

	return TRUE;
}


BOOL CreateGIocp(int server_port, eSERVER_TYPE eServerType)
{
	DWORD dwPar = 0;
	dwPar = g_dwServerCount & 0xFFFF;
	dwPar |= (eServerType & 0xFFFF ) << 16;

	DWORD ThreadID;
	g_ServerPort[g_dwServerCount] = server_port;
	g_IocpThreadHandle[g_dwServerCount] = CreateThread( NULL, 0,
											(LPTHREAD_START_ROUTINE)IocpServerWorker,
											(LPVOID)dwPar, 0, &ThreadID);

	if ( g_IocpThreadHandle[g_dwServerCount] == NULL )
	{
		g_Log.Add("[IOCP] CreateThread() failed with error %d - ServerCount : %d", GetLastError(), g_dwServerCount);
		return FALSE;
	}
	
	g_dwServerCount++;

	if ( g_dwServerCount >= MAX_SERVER_TYPE )
	{
		MessageBox(NULL, "[IOCP] MAX_SERVER_TYPE reached!", "Error", MB_ICONERROR);
		return FALSE;
	}

	return TRUE;
}

void DestroyGIocp()
{
	for(DWORD I=0;I<g_dwServerCount;I++)
	{
		closesocket(g_Listen[g_dwServerCount]);

		for (DWORD dwCPU=0; dwCPU < g_dwThreadCount[g_dwServerCount];dwCPU++ )
			TerminateThread(g_ThreadHandles[g_dwServerCount][dwCPU] , 0);

		TerminateThread(g_IocpThreadHandle[g_dwServerCount], 0);

		if ( g_CompletionPort[g_dwServerCount] != NULL )
		{
			CloseHandle(g_CompletionPort[g_dwServerCount]);
			g_CompletionPort[g_dwServerCount]=NULL;
		} 
	}

	DeleteCriticalSection(&scriti);
}

BOOL CreateListenSocket(DWORD dwServerCountIndex)
{
	sockaddr_in InternetAddr;
	int nRet;

	g_Listen[dwServerCountIndex] = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_IP, NULL, 0, WSA_FLAG_OVERLAPPED);

	if ( g_Listen[dwServerCountIndex] == INVALID_SOCKET )
	{
		g_Log.Add("WSASocket() failed with error %d", WSAGetLastError() );
		return FALSE;
	}

	InternetAddr.sin_family=AF_INET;
	InternetAddr.sin_addr.S_un.S_addr=htonl(0);
	InternetAddr.sin_port=htons(g_ServerPort[dwServerCountIndex]);
	nRet=::bind(g_Listen[dwServerCountIndex], (sockaddr*)&InternetAddr, 16);
	
	if ( nRet == -1 )
	{
		g_Log.AddMSB("bind error : eDataServer can't be launched twice");
		SendMessage(ghWnd, WM_CLOSE, 0,0);	// Kill aplication
		return FALSE;
	}

	nRet=listen(g_Listen[dwServerCountIndex], 5);
	if (nRet == -1)
	{
		g_Log.Add("listen() failed with error %d", WSAGetLastError());
		return FALSE;
	}

	return TRUE;
}

unsigned long __stdcall IocpServerWorker(DWORD pIocpServerParameter)
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
	int eType;
	
	cAddrlen=16;
	lpPerSocketContext=0;
	Flags=0;
	DWORD dwServerCountIndex = pIocpServerParameter & 0xFFFF;
	eType = pIocpServerParameter >> 16;

	InitializeCriticalSection(&criti[eType]);
	GetSystemInfo(&SystemInfo);

	g_dwThreadCount[dwServerCountIndex] = SystemInfo.dwNumberOfProcessors * 2;

	if ( g_dwThreadCount[dwServerCountIndex] > MAX_IO_THREAD_HANDLES )
	{
		g_dwThreadCount[dwServerCountIndex] = MAX_IO_THREAD_HANDLES;
		g_Log.AddC(TColor::Orange, "[WARNING]: IOCP Thread Handles set to 16");
	}

	__try
	{

		g_CompletionPort[dwServerCountIndex] = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);

		if ( g_CompletionPort[dwServerCountIndex] == NULL )
		{
			g_Log.Add("CreateIoCompletionPort failed with error: %d", GetLastError());
			__leave;
		}

		for ( DWORD n = 0; n<g_dwThreadCount[dwServerCountIndex]; n++ )
		{
			HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ServerWorkerThread, (LPVOID)g_CompletionPort[dwServerCountIndex], 0, &ThreadID);

			if ( hThread == NULL )
			{
				g_Log.Add("CreateThread() failed with error %d", GetLastError() );
				__leave;
			}

			g_ThreadHandles[dwServerCountIndex][n] = hThread;
			CloseHandle(hThread);
		}

		if ( eType == ST_DATASERVER )
		{
			HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ServerRecvWorkerThread_DS_Q1, (LPVOID)eType, 0, &ThreadID);

			if ( hThread == NULL )
			{
				g_Log.Add("CreateThread() failed with error %d", GetLastError() );
				__leave;
			}

			hQueue1 = hThread;

			if ( SetThreadPriority(hThread, THREAD_PRIORITY_HIGHEST) == FALSE )
			{
				g_Log.Add("SetThreadPriority() failed with error %d", GetLastError());
			}

			hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ServerRecvWorkerThread_DS_Q2, (LPVOID)eType, 0, &ThreadID);

			if ( hThread == NULL )
			{
				g_Log.Add("CreateThread() (2) failed with error %d", GetLastError() );
				__leave;
			}

			hQueue2 = hThread;

			hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ServerRecvWorkerThread_DS_Q3, (LPVOID)eType, 0, &ThreadID);

			if ( hThread == NULL )
			{
				g_Log.Add("CreateThread() (3) failed with error %d", GetLastError() );
				__leave;
			}

			hQueue3 = hThread;

			hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ServerRecvWorkerThread_DS_Q4, (LPVOID)eType, 0, &ThreadID);

			if ( hThread == NULL )
			{
				g_Log.Add("CreateThread() (4) failed with error %d", GetLastError() );
				__leave;
			}

			hQueue4 = hThread;

			if (SetThreadPriority(hThread, THREAD_PRIORITY_ABOVE_NORMAL) == FALSE)
			{
				g_Log.Add("SetThreadPriority() failed with error %d", GetLastError());
			}

			hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ServerRecvWorkerThread_DS_Q5, (LPVOID)eType, 0, &ThreadID);

			if (hThread == NULL)
			{
				g_Log.Add("CreateThread() (5) failed with error %d", GetLastError());
				__leave;
			}

			hQueue5 = hThread;
		}

		else if ( eType == ST_JOINSERVER )
		{
			HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ServerRecvWorkerThread_JS, (LPVOID)eType, 0, &ThreadID);

			if ( hThread == NULL )
			{
				g_Log.Add("CreateThread() failed with error %d", GetLastError() );
				__leave;
			}

			hJSQueue = hThread;
		}

		else if ( eType == ST_EXDATASERVER )
		{
			HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ServerRecvWorkerThread_EXDS, (LPVOID)eType, 0, &ThreadID);

			if ( hThread == NULL )
			{
				g_Log.Add("CreateThread() failed with error %d", GetLastError() );
				__leave;
			}
			
			hEXDSQueue = hThread;
		}

		if ( CreateListenSocket(dwServerCountIndex) == FALSE )
		{

		}
		else
		{
			while ( true )
			{
				Accept = WSAAccept(g_Listen[dwServerCountIndex], (sockaddr*)&cAddr, &cAddrlen, NULL, 0 );

				if ( Accept == -1 )
				{
					EnterCriticalSection(&criti[eType]);
					g_Log.Add("WSAAccept() failed with error %d", WSAGetLastError() );
					LeaveCriticalSection(&criti[eType]);
					continue;
				}

				EnterCriticalSection(&criti[eType]);
				memcpy(&cInAddr, &cAddr.sin_addr  , sizeof(cInAddr) );

				EnterCriticalSection(&scriti);

				ClientIndex = gObjServerAddSearch();

				if ( ClientIndex == -1 )
				{
					g_Log.AddC(TColor::Red,"error-L2 : ServerIndex = -1");
					closesocket(Accept);
					LeaveCriticalSection(&scriti);
					LeaveCriticalSection(&criti[eType]);
					continue;
				}

				if (UpdateCompletionPort(Accept, ClientIndex, dwServerCountIndex) == 0 )
				{
					g_Log.AddC(TColor::Red,"error-L1 : %d %d CreateIoCompletionPort failed with error %d", Accept, ClientIndex, GetLastError() );
					closesocket(Accept);
					LeaveCriticalSection(&scriti);
					LeaveCriticalSection(&criti[eType]);
					continue;
				}

				if (gObjServerAdd(Accept, inet_ntoa(cInAddr), ClientIndex, eSERVER_TYPE(eType) ) == -1 )
				{
					g_Log.AddC(TColor::Red,"error-L1 : %d %d gObjAdd() failed with error %d", Accept, ClientIndex, GetLastError() );
					LeaveCriticalSection(&scriti);
					LeaveCriticalSection(&criti[eType]);
					closesocket(Accept);
					continue;
				}

				g_Server[ClientIndex].m_ConnectPort = g_ServerPort[dwServerCountIndex];

				LeaveCriticalSection(&scriti);
				
				memset(&g_Server[ClientIndex].PerSocketContext->IOContext[0].Overlapped, 0, sizeof(WSAOVERLAPPED));
				memset(&g_Server[ClientIndex].PerSocketContext->IOContext[1].Overlapped, 0, sizeof(WSAOVERLAPPED));

				g_Server[ClientIndex].PerSocketContext->IOContext[0].wsabuf.buf = (char*)&g_Server[ClientIndex].PerSocketContext->IOContext[0].Buffer;
				g_Server[ClientIndex].PerSocketContext->IOContext[0].wsabuf.len = MAX_IO_BUFFER_SIZE;
				g_Server[ClientIndex].PerSocketContext->IOContext[0].nTotalBytes = 0;
				g_Server[ClientIndex].PerSocketContext->IOContext[0].nSentBytes = 0;
				g_Server[ClientIndex].PerSocketContext->IOContext[0].nWaitIO = 0;
				g_Server[ClientIndex].PerSocketContext->IOContext[0].nSecondOfs = 0;
				g_Server[ClientIndex].PerSocketContext->IOContext[0].IOOperation = 0;
				g_Server[ClientIndex].PerSocketContext->IOContext[1].wsabuf.buf = (char*)g_Server[ClientIndex].PerSocketContext->IOContext[0].Buffer;
				g_Server[ClientIndex].PerSocketContext->IOContext[1].wsabuf.len = MAX_IO_BUFFER_SIZE;
				g_Server[ClientIndex].PerSocketContext->IOContext[1].nTotalBytes = 0;
				g_Server[ClientIndex].PerSocketContext->IOContext[1].nSentBytes = 0;
				g_Server[ClientIndex].PerSocketContext->IOContext[1].nWaitIO = 0;
				g_Server[ClientIndex].PerSocketContext->IOContext[1].nSecondOfs = 0;
				g_Server[ClientIndex].PerSocketContext->IOContext[1].IOOperation = 1;
				g_Server[ClientIndex].PerSocketContext->m_socket = Accept;
				g_Server[ClientIndex].PerSocketContext->nIndex = ClientIndex;

				nRet = WSARecv(Accept, &g_Server[ClientIndex].PerSocketContext->IOContext[0].wsabuf , 1, (unsigned long*)&RecvBytes, &Flags, 
						&g_Server[ClientIndex].PerSocketContext->IOContext[0].Overlapped, NULL);

				if ( nRet == -1 )
				{
					if ( WSAGetLastError() != WSA_IO_PENDING )
					{
						g_Log.AddC(TColor::Red,"error-L1 : WSARecv() failed with error %d", WSAGetLastError() );
						g_Server[ClientIndex].PerSocketContext->IOContext[0].nWaitIO = 4;
						CloseClient(g_Server[ClientIndex].PerSocketContext, 0);
						LeaveCriticalSection(&criti[eType]);
						continue;
					}
				}

				g_Server[ClientIndex].PerSocketContext->IOContext[0].nWaitIO  = 1;
				g_Server[ClientIndex].PerSocketContext->dwIOCount++;

				LeaveCriticalSection(&criti[eType]);
				//SCPJoinResultSend(ClientIndex, 1);
			}

		}
	}
	__finally
	{
		
		if ( g_CompletionPort[dwServerCountIndex] != NULL )
		{
			for ( DWORD i = 0 ; i < g_dwThreadCount[dwServerCountIndex] ; i++ )
			{
				PostQueuedCompletionStatus( g_CompletionPort[dwServerCountIndex] , 0, 0, 0);
			}
		}

		if ( g_CompletionPort[dwServerCountIndex] != NULL )
		{
			CloseHandle(g_CompletionPort[dwServerCountIndex]);
			g_CompletionPort[dwServerCountIndex] = NULL;
		}
		if ( g_Listen[dwServerCountIndex] != INVALID_SOCKET )
		{
			closesocket( g_Listen[dwServerCountIndex]);
			g_Listen[dwServerCountIndex] = INVALID_SOCKET;
		}

		DeleteCriticalSection(&criti[eType]);
	}
	return TRUE;
}

unsigned long __stdcall ServerWorkerThread(HANDLE CompletionPortID)
{
	HANDLE CompletionPort;
	DWORD dwIoSize;
	unsigned long RecvBytes;
	unsigned long Flags;
	DWORD dwSendNumBytes;
	BOOL bSuccess;
	int nRet;
#ifndef _WIN64
	DWORD ClientIndex;
#else
	UINT64 ClientIndex;
#endif
	_PER_SOCKET_CONTEXT * lpPerSocketContext;
	LPOVERLAPPED lpOverlapped;
	_PER_IO_CONTEXT * lpIOContext;
	int eType;
	
	CompletionPort=CompletionPortID;
	dwSendNumBytes=0;
	bSuccess=0;
	lpPerSocketContext=0;
	lpOverlapped=0;
	lpIOContext=0;
	
	while ( true )
	{
		bSuccess=GetQueuedCompletionStatus( CompletionPort, &dwIoSize, &ClientIndex, &lpOverlapped, -1); // WAIT_FOREVER

		if (bSuccess == FALSE)
		{
			if (lpOverlapped != NULL)
			{
				DWORD aError = GetLastError();
				if ( (aError != ERROR_NETNAME_DELETED) && (aError != ERROR_CONNECTION_ABORTED) && (aError != ERROR_OPERATION_ABORTED) )
				{
					//EnterCriticalSection(&criti[dwServerCountIndex]);
					g_Log.Add("Error Thread : GetQueueCompletionStatus( %d )", GetLastError());
					//LeaveCriticalSection(&criti[dwServerCountIndex]);
					return 0;
				}
			}
		}

		eType = g_Server[ClientIndex].m_Type;

		if (eType <= ST_NONE || eType > ST_EXDATASERVER)
		{
			g_Log.Add("Error - eType is wrong (%d) %s %d", eType, __FILE__, __LINE__);
			continue;
		}

		EnterCriticalSection(&criti[eType]);

		lpPerSocketContext=g_Server[ClientIndex].PerSocketContext;
		lpPerSocketContext->dwIOCount --;
				
		if ( dwIoSize == 0 )
		{
			g_Log.Add("Connection Closed, dwIoSize == 0 (Index:%d)", lpPerSocketContext->nIndex);
			CloseClient(lpPerSocketContext, 0);
			LeaveCriticalSection(&criti[eType]);
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
				g_Log.Add("error-L1 : Socket Header error %d, %d", WSAGetLastError(), lpPerSocketContext->nIndex);
				CloseClient(lpPerSocketContext, 0);
				LeaveCriticalSection(&criti[eType]);
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
					g_Log.Add("WSARecv() failed with error %d", WSAGetLastError() );
					CloseClient(lpPerSocketContext, 0);
					LeaveCriticalSection(&criti[eType]);
					continue;
				}
			}

			lpPerSocketContext->dwIOCount ++;
			lpIOContext->nWaitIO = 1;
		}
		LeaveCriticalSection(&criti[eType]);
		

	}

	return 1;
}


BOOL RecvDataParse(_PER_IO_CONTEXT * lpIOContext, int uIndex)	
{
	unsigned char* recvbuf;
	int lOfs;
	int size;
	BYTE headcode;
	BYTE xcode;

	// Check If Recv Data has More thatn 3 BYTES
	if ( lpIOContext->nSentBytes < 3 )
	{
		return TRUE;
	}

	// Initialize Variables
	lOfs=0;
	size=0;
	xcode=0;
	recvbuf = lpIOContext->Buffer;

	// Start Loop
	while ( true )
	{
		// Select packets with
		// C1 or C2 as HEader
		if ( recvbuf[lOfs] == 0xC1 ||
			 recvbuf[lOfs] == 0xC3 )
		{
			size = recvbuf[lOfs+1];
			headcode = recvbuf[lOfs+2];
			xcode = recvbuf[lOfs];
		}
		else if ( recvbuf[lOfs] == 0xC2 ||
			      recvbuf[lOfs] == 0xC4 )
		{
			size = recvbuf[lOfs+1] * 256;
			size |= recvbuf[lOfs+2];
			headcode = recvbuf[lOfs+3];
			xcode = recvbuf[lOfs];
		}

		// Check Size is leess thant 0
		if ( size <= 0 )
		{
			g_Log.Add("error-L1 : size %d",size);
			return false;
		}

		// Check if Size is On Range
		if ( size <= lpIOContext->nSentBytes )
		{
			if ( g_Server[uIndex].m_Type == ST_DATASERVER )
			{
				if(headcode == 0x52 || headcode == 0x53 || headcode == 0x55)
				{
					m_DSQueue3.AddToQueue((LPBYTE)(recvbuf + lOfs), size, headcode, uIndex, g_DelayHandler.GetQuerySessionId());
				}

				else if(headcode == 0x03 || headcode == 0x08 || headcode == 0x09 || headcode == 0x12)
				{
					m_DSQueue2.AddToQueue((LPBYTE)(recvbuf + lOfs), size, headcode, uIndex, g_DelayHandler.GetQuerySessionId());
				}

				else if(headcode == 0x01 || headcode == 0x04 || headcode == 0x05 || headcode == 0x06 || headcode == 0x07 || headcode == 0x11 || headcode == 0x60 || headcode == 0xAE || headcode == 0xDA || headcode == 0xFA || headcode == 0xFB || headcode == 0xC4)
				{
					m_DSQueue4.AddToQueue((LPBYTE)(recvbuf + lOfs), size, headcode, uIndex, g_DelayHandler.GetQuerySessionId());
				}

				else if (headcode == 0xF6 || headcode == 0xF7)
				{
					m_DSQueue5.AddToQueue((LPBYTE)(recvbuf + lOfs), size, headcode, uIndex, g_DelayHandler.GetQuerySessionId());
				}

				else
				{
					m_DSQueue1.AddToQueue((LPBYTE)(recvbuf + lOfs), size, headcode, uIndex, g_DelayHandler.GetQuerySessionId());
				}
			}

			else if ( g_Server[uIndex].m_Type == ST_JOINSERVER )
			{
				m_JSQueue.AddToQueue((LPBYTE)(recvbuf + lOfs), size, headcode, uIndex, g_DelayHandler.GetQuerySessionId());
			}

			else if ( g_Server[uIndex].m_Type == ST_EXDATASERVER )
			{
				m_EXDSQueue.AddToQueue((LPBYTE)(recvbuf + lOfs), size, headcode, uIndex, g_DelayHandler.GetQuerySessionId());
			}

			g_ServerInfoDisplayer.SetState(SERVER_ACTIVE);

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
				g_Log.Add("error-L1 : recvbuflen 1 %s %d", __FILE__, __LINE__);
				break;
			}

			if ( lpIOContext->nSentBytes < MAX_IO_BUFFER_SIZE ) 
			{
				memcpy(recvbuf, &recvbuf[lOfs], lpIOContext->nSentBytes);
				g_Log.Add("Message copy %d", lpIOContext->nSentBytes);
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







BOOL DataSend(int aIndex, unsigned char* lpMsg, DWORD dwSize, char* szFunction)
{
	unsigned long SendBytes;
	_PER_SOCKET_CONTEXT * lpPerSocketContext;
	unsigned char * SendBuf;

	if ( aIndex < 0 || aIndex > (g_dwMaxServerGroups-1) )
	{
		g_Log.Add("error-L2 : Index(%d) %x %x %x (%s) ", dwSize, lpMsg[0], lpMsg[1], lpMsg[2], szFunction);
		return false;
	}

	int eType = g_Server[aIndex].m_Type;

	if (eType != ST_DATASERVER && eType != ST_EXDATASERVER && eType != ST_JOINSERVER)
	{
		return false;
	}

	EnterCriticalSection(&criti[eType]);

	SendBuf = lpMsg;

	if ( SendBuf[0] != 0xC1 && SendBuf[0] != 0xC2 )
	{
		g_Log.Add("error : header error");
	}

	if ( g_Server[aIndex].m_State < SS_CONNECTED )
	{
		LeaveCriticalSection(&criti[eType]);
		return FALSE;
	}

	lpPerSocketContext= g_Server[aIndex].PerSocketContext;

	if ( dwSize > sizeof(lpPerSocketContext->IOContext[0].Buffer))
	{
		g_Log.Add("Error : Max msg(%d) %s %d", dwSize, __FILE__, __LINE__);
		CloseClient(aIndex);
		LeaveCriticalSection(&criti[eType]);
		return false;
	}

	_PER_IO_CONTEXT  * lpIoCtxt;

	lpIoCtxt = &lpPerSocketContext->IOContext[1];

	if ( lpIoCtxt->nWaitIO > 0 )
	{
		if ( ( lpIoCtxt->nSecondOfs + dwSize ) > MAX_IO_BUFFER_SIZE-1 )
		{
			g_Log.Add("(%d)error-L2 MAX BUFFER OVER %d %d %d",
				aIndex, lpIoCtxt->nTotalBytes, lpIoCtxt->nSecondOfs, dwSize);

			lpIoCtxt->nWaitIO = 0;
			CloseClient(aIndex);
			LeaveCriticalSection(&criti[eType]);
			return true;
		}

		memcpy( &lpIoCtxt->BufferSecond[lpIoCtxt->nSecondOfs], SendBuf, dwSize);
		lpIoCtxt->nSecondOfs += dwSize;
		LeaveCriticalSection(&criti[eType]);
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
		g_Log.Add("(%d)error-L2 MAX BUFFER OVER %d %d %d",
				aIndex, lpIoCtxt->nTotalBytes, lpIoCtxt->nSecondOfs, dwSize);

		lpIoCtxt->nWaitIO = 0;
		CloseClient(aIndex);
		LeaveCriticalSection(&criti[eType]);
		return FALSE;
	}

	memcpy( &lpIoCtxt->Buffer[lpIoCtxt->nTotalBytes], SendBuf, dwSize);
	lpIoCtxt->nTotalBytes += dwSize;
	lpIoCtxt->wsabuf.buf = (char*)&lpIoCtxt->Buffer;
	lpIoCtxt->wsabuf.len = lpIoCtxt->nTotalBytes;
	lpIoCtxt->nSentBytes = 0;
	lpIoCtxt->IOOperation = 1;
	

	if ( WSASend( g_Server[aIndex].m_Socket, &lpIoCtxt->wsabuf , 1, &SendBytes, 0, &lpIoCtxt->Overlapped, NULL) == -1 )
	{

		if ( WSAGetLastError() != WSA_IO_PENDING )	
		{
			lpIoCtxt->nWaitIO = 0;
			

			if ( lpIoCtxt->wsabuf.buf[0] == 0xC1 )
			{
				g_Log.Add("(%d)WSASend(%d) failed with error [%x][%x] %d %s ", __LINE__, aIndex, (BYTE)lpIoCtxt->wsabuf.buf[0],
					(BYTE)lpIoCtxt->wsabuf.buf[2], WSAGetLastError(), g_Server[aIndex].m_ServerIp);
			}
			else if ( lpIoCtxt->wsabuf.buf[0] == 0xC2 )
			{
				g_Log.Add("(%d)WSASend(%d) failed with error [%x][%x] %d %s ", __LINE__, aIndex, (BYTE)lpIoCtxt->wsabuf.buf[0],
					(BYTE)lpIoCtxt->wsabuf.buf[3], WSAGetLastError(), g_Server[aIndex].m_ServerIp);
			}
			CloseClient(aIndex);
			LeaveCriticalSection(&criti[eType]);
			return false;
		}
	}
	else
	{
		lpPerSocketContext->dwIOCount ++;
	}
	
	
	lpIoCtxt->nWaitIO = 1;
	LeaveCriticalSection(&criti[eType]);
	return true;
}



BOOL IoSendSecond(_PER_SOCKET_CONTEXT * lpPerSocketContext)
{
	unsigned long SendBytes;
	int aIndex;
	_PER_IO_CONTEXT * lpIoCtxt;

	aIndex = lpPerSocketContext->nIndex;

	int eType = g_Server[aIndex].m_Type;

	EnterCriticalSection(&criti[eType]);

	lpIoCtxt = &lpPerSocketContext->IOContext[1];

	if ( lpIoCtxt->nWaitIO > 0 )
	{
		LeaveCriticalSection(&criti[eType]);
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
		LeaveCriticalSection(&criti[eType]);
		return false;
	}

	lpIoCtxt->wsabuf.buf = (char*)&lpIoCtxt->Buffer;
	lpIoCtxt->wsabuf.len = lpIoCtxt->nTotalBytes;
	lpIoCtxt->nSentBytes = 0;
	lpIoCtxt->IOOperation = 1;

	if ( WSASend(g_Server[aIndex].m_Socket, &lpIoCtxt->wsabuf, 1, &SendBytes, 0, &lpIoCtxt->Overlapped, NULL) == -1 )
	{
		if ( WSAGetLastError() != WSA_IO_PENDING )
		{
			g_Log.Add("WSASend(%d) failed with error %d %s ", __LINE__, WSAGetLastError(), g_Server[aIndex].m_ServerIp);
			CloseClient(aIndex);
			LeaveCriticalSection(&criti[eType]);
			return false;
		}
	}
	else
	{
		lpPerSocketContext->dwIOCount ++;
	}
	
	lpIoCtxt->nWaitIO = 1;
	LeaveCriticalSection(&criti[eType]);
	
	return true;
}

BOOL IoMoreSend(_PER_SOCKET_CONTEXT * lpPerSocketContext)
{
	unsigned long SendBytes;
	int aIndex;
	_PER_IO_CONTEXT * lpIoCtxt;

	aIndex = lpPerSocketContext->nIndex;

	int eType = g_Server[aIndex].m_Type;

	EnterCriticalSection(&criti[eType]);
	lpIoCtxt = &lpPerSocketContext->IOContext[1];

	if ( (lpIoCtxt->nTotalBytes - lpIoCtxt->nSentBytes) < 0 )
	{
		LeaveCriticalSection(&criti[eType]);
		return false;
	}

	lpIoCtxt->wsabuf.buf = (char*)&lpIoCtxt->Buffer[lpIoCtxt->nSentBytes];
	lpIoCtxt->wsabuf.len = lpIoCtxt->nTotalBytes - lpIoCtxt->nSentBytes;
	lpIoCtxt->IOOperation = 1;

	if ( WSASend(g_Server[aIndex].m_Socket, &lpIoCtxt->wsabuf, 1, &SendBytes, 0, &lpIoCtxt->Overlapped, NULL) == -1 )
	{
		if ( WSAGetLastError() != WSA_IO_PENDING )
		{
			g_Log.Add("WSASend(%d) failed with error %d %s ", __LINE__, WSAGetLastError(), g_Server[aIndex].m_ServerIp);
			CloseClient(aIndex);
			LeaveCriticalSection(&criti[eType]);
			return false;
		}
	}
	else
	{
		lpPerSocketContext->dwIOCount ++;
	}
	
	
	lpIoCtxt->nWaitIO = 1;
	LeaveCriticalSection(&criti[eType]);
	return true;
}

BOOL UpdateCompletionPort(SOCKET sd, int ClientIndex, DWORD dwServerTypeCount)
{
	_PER_SOCKET_CONTEXT * lpPerSocketContext = NULL;
	HANDLE cp = CreateIoCompletionPort((HANDLE) sd, g_CompletionPort[dwServerTypeCount], ClientIndex, 0);

	if ( cp == NULL )
	{
		g_Log.Add("CreateIoCompletionPort: %d", GetLastError() );
		return FALSE;
	}

	g_Server[ClientIndex].PerSocketContext->dwIOCount = 0;
	return TRUE;
}

void CloseClient(_PER_SOCKET_CONTEXT * lpPerSocketContext, int result)
{
	int index = -1;
	index = lpPerSocketContext->nIndex ;

	if ( index >= 0 && index <= g_dwMaxServerGroups )
	{
		if(g_Server[index].m_Type == ST_JOINSERVER)
		{
			g_Server[index].m_ProtocolCore(g_Server[index].m_ServerCode,0xFF,NULL,-1);
		}

		if ( g_Server[index].m_Socket != INVALID_SOCKET )
		{
			if (closesocket(g_Server[index].m_Socket) == -1 )
			{
				if ( WSAGetLastError() != WSAENOTSOCK )
				{
					return;
				}
			}

			g_Server[index].m_Socket = INVALID_SOCKET;
		}

		gObjServerDel(index);
	}
}


void CloseClient(int index)
{
	if ( index < 0 || index >= g_dwMaxServerGroups )
	{
		g_Log.Add("error-L1 : CloseClient index error");
		return;
	}

	if ( g_Server[index].m_State == SS_CLOSED )
	{
		g_Log.Add("error-L1 : CloseClient connect error");
		return;
	}

	EnterCriticalSection(&criti[g_Server[index].m_Type]);

	if ( g_Server[index].m_Socket != INVALID_SOCKET )
	{
		closesocket(g_Server[index].m_Socket );
		g_Server[index].m_Socket = INVALID_SOCKET;
	}
	else
	{
		g_Log.Add("error-L1 : CloseClient INVALID_SOCKET");
	}

	LeaveCriticalSection(&criti[g_Server[index].m_Type]);
}

void ResponErrorCloseClient(int index)
{
	if ( index < 0 || index >= g_dwMaxServerGroups )
	{
		g_Log.Add("error-L1 : CloseClient index error");
		return;
	}

	if ( g_Server[index].m_State == SS_CLOSED )
	{
		g_Log.Add("error-L1 : CloseClient connect error");
		return;
	}

	EnterCriticalSection(&criti[g_Server[index].m_Type]);
	closesocket(g_Server[index].m_Socket);
	g_Server[index].m_Socket = INVALID_SOCKET;

	if ( g_Server[index].m_Socket == INVALID_SOCKET )
	{
		g_Log.Add("error-L1 : CloseClient INVALID_SOCKET");
	}

	gObjServerDel(index);
	LeaveCriticalSection(&criti[g_Server[index].m_Type]);
}

DWORD WINAPI ServerRecvWorkerThread_DS_Q1(DWORD p)
{
	BYTE RecvData[MAX_BUFF_SIZE];
	unsigned int nSize = 0;
	BYTE headcode;
	int  uindex;
	int	 loopN;

	int  iSessionId;

	int  nCount = 0; //1C
	int eType = p;

	while (TRUE)
	{
		EnterCriticalSection(&criti[eType]);
		loopN = m_DSQueue1.GetCount();

		if (loopN > MAX_NODE - 1)
		{
			loopN = MAX_NODE - 1;
			g_Log.Add("error : Q1 Loop MAX %s %d", __FILE__, __LINE__);
		}
		LeaveCriticalSection(&criti[eType]);

		if (loopN > 0)
		{
			if (m_DSQueue1.GetFromQueue((LPBYTE)RecvData, &nSize, &headcode, &uindex, &iSessionId) == TRUE)
			{
				if (g_Server[uindex].m_ProtocolCore != NULL)
				{
					g_Server[uindex].m_ProtocolCore(uindex, headcode, RecvData, nSize);
				}

				else
				{
					g_Log.Add("[DelayHandler] ProtocolCore IS NULL -> Index : %d", uindex);
				}
			}
		}
		Sleep(1);
	}
	return TRUE;
}

DWORD WINAPI ServerRecvWorkerThread_DS_Q2(DWORD p)
{
	BYTE RecvData[MAX_BUFF_SIZE];
	unsigned int nSize = 0;
	BYTE headcode;
	int  uindex;
	int	 loopN;

	int  iSessionId;

	int  nCount = 0; //1C
	int eType = p;

	while (TRUE)
	{
		EnterCriticalSection(&criti[eType]);
		loopN = m_DSQueue2.GetCount();

		if (loopN > MAX_NODE - 1)
		{
			loopN = MAX_NODE - 1;
			g_Log.Add("error : Q2 Loop MAX %s %d", __FILE__, __LINE__);
		}
		LeaveCriticalSection(&criti[eType]);

		if (loopN > 0)
		{
			if (m_DSQueue2.GetFromQueue((LPBYTE)RecvData, &nSize, &headcode, &uindex, &iSessionId) == TRUE)
			{
				if (g_Server[uindex].m_ProtocolCore != NULL)
				{
					g_Server[uindex].m_ProtocolCore(uindex, headcode, RecvData, nSize);
				}

				else
				{
					g_Log.Add("[DelayHandler] ProtocolCore IS NULL -> Index : %d", uindex);
				}
			}
		}
		Sleep(1);
	}
	return TRUE;
}

DWORD WINAPI ServerRecvWorkerThread_DS_Q3(DWORD p)
{
	BYTE RecvData[MAX_BUFF_SIZE];
	unsigned int nSize = 0;
	BYTE headcode;
	int  uindex;
	int	 loopN;

	int  iSessionId;

	int  nCount = 0; //1C
	int eType = p;

	while (TRUE)
	{
		EnterCriticalSection(&criti[eType]);
		loopN = m_DSQueue3.GetCount();

		if (loopN > MAX_NODE - 1)
		{
			loopN = MAX_NODE - 1;
			g_Log.Add("error : Q3 Loop MAX %s %d", __FILE__, __LINE__);
		}
		LeaveCriticalSection(&criti[eType]);

		if (loopN > 0)
		{
			if (m_DSQueue3.GetFromQueue((LPBYTE)RecvData, &nSize, &headcode, &uindex, &iSessionId) == TRUE)
			{
				if (g_Server[uindex].m_ProtocolCore != NULL)
				{
					g_Server[uindex].m_ProtocolCore(uindex, headcode, RecvData, nSize);
				}

				else
				{
					g_Log.Add("[DelayHandler] ProtocolCore IS NULL -> Index : %d", uindex);
				}
			}
		}
		Sleep(1);
	}
	return TRUE;
}

DWORD WINAPI ServerRecvWorkerThread_DS_Q4(DWORD p)
{
	BYTE RecvData[MAX_BUFF_SIZE];
	unsigned int nSize = 0;
	BYTE headcode;
	int  uindex;
	int	 loopN;

	int  iSessionId;

	int  nCount = 0; //1C
	int eType = p;

	while (TRUE)
	{
		EnterCriticalSection(&criti[eType]);
		loopN = m_DSQueue4.GetCount();

		if (loopN > MAX_NODE - 1)
		{
			loopN = MAX_NODE - 1;
			g_Log.Add("error : Q4 Loop MAX %s %d", __FILE__, __LINE__);
		}
		LeaveCriticalSection(&criti[eType]);

		if (loopN > 0)
		{
			if (m_DSQueue4.GetFromQueue((LPBYTE)RecvData, &nSize, &headcode, &uindex, &iSessionId) == TRUE)
			{
				if (g_Server[uindex].m_ProtocolCore != NULL)
				{
					g_Server[uindex].m_ProtocolCore(uindex, headcode, RecvData, nSize);
				}

				else
				{
					g_Log.Add("[DelayHandler] ProtocolCore IS NULL -> Index : %d", uindex);
				}
			}
		}
		Sleep(1);
	}
	return TRUE;
}

DWORD WINAPI ServerRecvWorkerThread_DS_Q5(DWORD p)
{
	BYTE RecvData[MAX_BUFF_SIZE];
	unsigned int nSize = 0;
	BYTE headcode;
	int  uindex;
	int	 loopN;

	int  iSessionId;

	int  nCount = 0; //1C
	int eType = p;

	while (TRUE)
	{
		EnterCriticalSection(&criti[eType]);
		loopN = m_DSQueue5.GetCount();

		if (loopN > MAX_NODE - 1)
		{
			loopN = MAX_NODE - 1;
			g_Log.Add("error : Q5 Loop MAX %s %d", __FILE__, __LINE__);
		}
		LeaveCriticalSection(&criti[eType]);

		if (loopN > 0)
		{
			if (m_DSQueue5.GetFromQueue((LPBYTE)RecvData, &nSize, &headcode, &uindex, &iSessionId) == TRUE)
			{
				if (g_Server[uindex].m_ProtocolCore != NULL)
				{
					g_Server[uindex].m_ProtocolCore(uindex, headcode, RecvData, nSize);
				}

				else
				{
					g_Log.Add("[DelayHandler] ProtocolCore IS NULL -> Index : %d", uindex);
				}
			}
		}
		Sleep(1);
	}
	return TRUE;
}

DWORD WINAPI ServerRecvWorkerThread_JS(DWORD p)
{
	BYTE RecvData[MAX_BUFF_SIZE];
	unsigned int nSize = 0;
	BYTE headcode;
	int  uindex;
	int	 loopN;

	int  iSessionId;

	int  nCount = 0; //1C
	int eType = p;

	while (TRUE)
	{
		EnterCriticalSection(&criti[eType]);
		loopN = m_JSQueue.GetCount();

		if (loopN > MAX_NODE - 1)
		{
			loopN = MAX_NODE - 1;
			g_Log.Add("error : JSQ Loop MAX %s %d", __FILE__, __LINE__);
		}
		LeaveCriticalSection(&criti[eType]);

		if (loopN > 0)
		{
			if (m_JSQueue.GetFromQueue((LPBYTE)RecvData, &nSize, &headcode, &uindex, &iSessionId) == TRUE)
			{
				if (g_Server[uindex].m_ProtocolCore != NULL)
				{
					g_Server[uindex].m_ProtocolCore(uindex, headcode, RecvData, nSize);
				}

				else
				{
					g_Log.Add("[DelayHandler] ProtocolCore IS NULL -> Index : %d", uindex);
				}
			}
		}
		Sleep(1);
	}
	return TRUE;
}

DWORD WINAPI ServerRecvWorkerThread_EXDS(DWORD p)
{
	BYTE RecvData[MAX_BUFF_SIZE];
	unsigned int nSize = 0;
	BYTE headcode;
	int  uindex;
	int	 loopN;

	int  iSessionId;

	int  nCount = 0; //1C
	int eType = p;

	while (TRUE)
	{
		EnterCriticalSection(&criti[eType]);
		loopN = m_EXDSQueue.GetCount();

		if (loopN > MAX_NODE - 1)
		{
			loopN = MAX_NODE - 1;
			g_Log.Add("error : EXDSQ Loop MAX %s %d", __FILE__, __LINE__);
		}
		LeaveCriticalSection(&criti[eType]);

		if (loopN > 0)
		{
			if (m_EXDSQueue.GetFromQueue((LPBYTE)RecvData, &nSize, &headcode, &uindex, &iSessionId) == TRUE)
			{
				if (g_Server[uindex].m_ProtocolCore != NULL)
				{
					g_Server[uindex].m_ProtocolCore(uindex, headcode, RecvData, nSize);
				}

				else
				{
					g_Log.Add("[DelayHandler] ProtocolCore IS NULL -> Index : %d", uindex);
				}
			}
		}
		Sleep(1);
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


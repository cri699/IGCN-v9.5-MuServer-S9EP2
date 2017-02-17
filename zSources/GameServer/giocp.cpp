// Giocp.cpp
//------------------------------------------
// Decompiled by Deathway
// Date : 2007-03-09
//------------------------------------------

// GS-N 0.99.60T 0x00473020 - Status : Completed :)
//	GS-N	1.00.18	JPN	0x00489FD0	-	Completed
#include "stdafx.h"
#include "spe.h"
#include "giocp.h"
#include "TLog.h"
#include "GameMain.h"
#include "GameServer.h"
#include "user.h"
#include "configread.h"
#include "PacketEncrypt.h"

CIOCP IOCP;

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
		g_Log.Add("CreateThread() failed with error %d", GetLastError());
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
		g_Log.Add("WSASocket() failed with error %d", WSAGetLastError() );
		return 0;
	}
	else
	{
		InternetAddr.sin_family=AF_INET;
		InternetAddr.sin_addr.S_un.S_addr=htonl(0);
		InternetAddr.sin_port=htons(g_ServerPort);
		nRet=::bind(g_Listen, (sockaddr*)&InternetAddr, 16);
		
		if ( nRet == -1 )
		{
			g_Log.MsgBox("Bind error - server cannot be launched twice. Please terminate game server and restart.");
			SendMessage(ghWnd, WM_CLOSE, 0,0);	// Kill aplication
			return 0 ;
		}
		else
		{
			nRet=listen(g_Listen, 5);
			if (nRet == -1)
			{
				g_Log.Add("listen() failed with error %d", WSAGetLastError());
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

	if ( g_dwThreadCount > MAX_IO_THREAD_HANDLES )
	{
		g_dwThreadCount = MAX_IO_THREAD_HANDLES;
		g_Log.AddC(TColor::Orange, "[WARNING]: IOCP Thread Handles set to 16");
	}

	__try
	{

		g_CompletionPort=CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);

		if ( g_CompletionPort == NULL )
		{
			g_Log.Add("CreateIoCompletionPort failed with error: %d", GetLastError());
			__leave;
		}

		for (int n = 0; n<g_dwThreadCount; n++)
		{
			HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ServerWorkerThreadEP, this, 0, &ThreadID);

			if ( hThread == 0 )
			{
				g_Log.Add("CreateThread() failed with error %d", GetLastError() );
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
					g_Log.Add("WSAAccept() failed with error %d", WSAGetLastError() );
					LeaveCriticalSection(&criti);
					continue;
				}

				EnterCriticalSection(&criti);
				memcpy(&cInAddr, &cAddr.sin_addr  , sizeof(cInAddr) );

				ClientIndex = gObjAddSearch(Accept, inet_ntoa(cInAddr) );

				if ( ClientIndex == -1 || !ObjectMaxRange(ClientIndex) )
				{
					g_Log.AddC(TColor::Red,  "error-L2 : ClientIndex = -1");
					closesocket(Accept);
					LeaveCriticalSection(&criti);
					continue;
				}

				if (UpdateCompletionPort(Accept, ClientIndex, 1) == 0 )
				{
					g_Log.AddC(TColor::Azure, "error-L1 : %d %d CreateIoCompletionPort failed with error %d", Accept, ClientIndex, GetLastError() );
					closesocket(Accept);
					LeaveCriticalSection(&criti);
					continue;
				}

				if (gObjAdd(Accept, inet_ntoa(cInAddr), ClientIndex) == -1 )
				{
					g_Log.AddC(TColor::Azure, "error-L1 : %d %d gObjAdd() failed with error %d", Accept, ClientIndex, GetLastError() );
					LeaveCriticalSection(&criti);
					closesocket(Accept);
					continue;
				}
				
				memset(&gObj[ClientIndex].PerSocketContext->IOContext[0].Overlapped, 0, sizeof(WSAOVERLAPPED));
				memset(&gObj[ClientIndex].PerSocketContext->IOContext[1].Overlapped, 0, sizeof(WSAOVERLAPPED));

				gObj[ClientIndex].PerSocketContext->IOContext[0].wsabuf.buf = (char*)&gObj[ClientIndex].PerSocketContext->IOContext[0].Buffer;
				gObj[ClientIndex].PerSocketContext->IOContext[0].wsabuf.len = MAX_IO_BUFFER_SIZE;
				gObj[ClientIndex].PerSocketContext->IOContext[0].nTotalBytes = 0;
				gObj[ClientIndex].PerSocketContext->IOContext[0].nSentBytes = 0;
				gObj[ClientIndex].PerSocketContext->IOContext[0].nWaitIO = 0;
				gObj[ClientIndex].PerSocketContext->IOContext[0].nSecondOfs = 0;
				gObj[ClientIndex].PerSocketContext->IOContext[0].IOOperation = 0;
				gObj[ClientIndex].PerSocketContext->IOContext[1].wsabuf.buf = (char*)gObj[ClientIndex].PerSocketContext->IOContext[1].Buffer;
				gObj[ClientIndex].PerSocketContext->IOContext[1].wsabuf.len = MAX_IO_BUFFER_SIZE;
				gObj[ClientIndex].PerSocketContext->IOContext[1].nTotalBytes = 0;
				gObj[ClientIndex].PerSocketContext->IOContext[1].nSentBytes = 0;
				gObj[ClientIndex].PerSocketContext->IOContext[1].nWaitIO = 0;
				gObj[ClientIndex].PerSocketContext->IOContext[1].nSecondOfs = 0;
				gObj[ClientIndex].PerSocketContext->IOContext[1].IOOperation = 1;
				gObj[ClientIndex].PerSocketContext->m_socket = Accept;
				gObj[ClientIndex].PerSocketContext->nIndex = ClientIndex;

				nRet = WSARecv(Accept, &gObj[ClientIndex].PerSocketContext->IOContext[0].wsabuf , 1, (unsigned long*)&RecvBytes, &Flags, 
						&gObj[ClientIndex].PerSocketContext->IOContext[0].Overlapped, NULL);

				if ( nRet == -1 )
				{
					if ( WSAGetLastError() != WSA_IO_PENDING )
					{
						g_Log.AddC(TColor::Azure, "error-L1 : WSARecv() failed with error %d", WSAGetLastError() );
						gObj[ClientIndex].PerSocketContext->IOContext[0].nWaitIO = 4;
						CloseClient(gObj[ClientIndex].PerSocketContext, 0);
						LeaveCriticalSection(&criti);
						continue;
					}
				}

				gObj[ClientIndex].PerSocketContext->IOContext[0].nWaitIO  = 1;
				gObj[ClientIndex].PerSocketContext->dwIOCount++;

				LeaveCriticalSection(&criti);
				GSProtocol.SCPJoinResultSend(ClientIndex, 1);
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

		if(gObj[ClientIndex].m_socket == INVALID_SOCKET)
		{
			continue;
		}
		if (bSuccess == 0)
		{
			if (lpOverlapped != 0)
			{
				int aError = GetLastError();
				if ( (aError != ERROR_NETNAME_DELETED) && (aError != ERROR_CONNECTION_ABORTED) && (aError != ERROR_OPERATION_ABORTED) && (aError != ERROR_SEM_TIMEOUT) && (aError != ERROR_HOST_UNREACHABLE) )
				{
					EnterCriticalSection(&criti);
					g_Log.Add("Error Thread : GetQueueCompletionStatus( %d )", GetLastError());
					LeaveCriticalSection(&criti);
					return 0;
				}
			}
		}

		EnterCriticalSection(&criti);

		lpPerSocketContext=gObj[ClientIndex].PerSocketContext;
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
			LeaveCriticalSection(&criti);
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
				LeaveCriticalSection(&criti);
				continue;
			}

			lpIOContext->nWaitIO = 0;
			Flags = 0;
			memset(&lpIOContext->Overlapped, 0, sizeof (WSAOVERLAPPED));
			lpIOContext->wsabuf.len = MAX_IO_BUFFER_SIZE - lpIOContext->nSentBytes;
			lpIOContext->wsabuf.buf = (char*)&lpIOContext->Buffer[lpIOContext->nSentBytes];
			lpIOContext->IOOperation = 0;
			if( lpPerSocketContext->m_socket != INVALID_SOCKET)
			{
				try
				{
				nRet = WSARecv(lpPerSocketContext->m_socket, &lpIOContext->wsabuf, 1, &RecvBytes, &Flags,
						&lpIOContext->Overlapped, NULL);
				}
				catch( const std::exception &e)
				{
					char text[1000];
					sprintf(text,"Exception %s was detected",e.what());
					MessageBoxA(0,text,0,0);
						///LeaveCriticalSection(&criti);
						//continue;
				}
			}
			else
			{
				LeaveCriticalSection(&criti);
				continue;
			}

			if ( nRet == -1 )
			{
				if ( WSAGetLastError() != WSA_IO_PENDING)
				{
					g_Log.Add("WSARecv() failed with error %d", WSAGetLastError() );
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

#define USERIJ 
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

	int recvsize = lpIOContext->nSentBytes;

	unsigned char byDec[9216];
	
	while ( true )
	{
		if ( recvbuf[lOfs] == 0xC1 ||
			 recvbuf[lOfs] == 0xC3 )
		{
			PBMSG_HEAD* lphead = (PBMSG_HEAD*)(recvbuf + lOfs);
			size = lphead->size;
			headcode = lphead->headcode;
			xcode = recvbuf[lOfs];
		}
		else if ( recvbuf[lOfs] == 0xC2 ||
			      recvbuf[lOfs] == 0xC4 )
		{
			PWMSG_HEAD* lphead = (PWMSG_HEAD*)(recvbuf + lOfs);
			size = ((WORD)(lphead->sizeH) << 8);
			size |= (WORD)(lphead->sizeL);
			headcode = lphead->headcode;
			xcode				= recvbuf[lOfs];
		}

		else
		{
			g_Log.Add("error-L1 : Header error (%s %d)lOfs:%d, size:%d",
											__FILE__, __LINE__,
											lOfs,
											lpIOContext->nSentBytes);

			lpIOContext->nSentBytes = 0;
			return FALSE;
		}

		if ( size <= 0 )
		{
			g_Log.Add("error-L1 : size %d",
				size);

			return false;
		}

		if ( size <= lpIOContext->nSentBytes )
		{
			if ( xcode == 0xC3 )
			{
				int ret = g_PacketEncrypt.Decrypt(&byDec[2], &recvbuf[lOfs + 2], size - 2);

				if ( ret < 0 )
				{
					g_Log.AddC(TColor::Red, "[%s][Packet-Decrypt BYTE] Error: ret < 0 %x/%x/%x)", gObj[uIndex].m_PlayerData->Ip_addr,recvbuf[lOfs], recvbuf[lOfs+1], recvbuf[lOfs+2]);
				}

				else
				{
					unsigned char* pDecBuf = byDec;

					headcode = pDecBuf[2];
					byDec[0] = 0xC1;
					byDec[1] = ret + 2;
					gObj[uIndex].m_PlayerData->PacketsPerSecond++;

					if(gObj[uIndex].m_PlayerData->PacketsPerSecond >= g_ConfigRead.data.common.PacketLimit)
					{
						g_Log.AddC(TColor::Red, "[ANTI-HACK] Packets Per Second: %d / %d", gObj[uIndex].m_PlayerData->PacketsPerSecond, g_ConfigRead.data.common.PacketLimit);
						GSProtocol.GCSendDisableReconnect(uIndex);
						//IOCP.CloseClient(uIndex);
						return 0;
					}

					CStreamPacketEngine_Server PacketStream;
					PacketStream.Clear();

					if (PacketStream.AddData(byDec, ret + 2) == 0)
					{

						g_Log.AddC(TColor::Red,  "error-L1 : CStreamPacketEngine Adding Error : ip = %s account:%s name:%s HEAD:%x (%s,%d) State:%d",
							gObj[uIndex].m_PlayerData->Ip_addr,
							gObj[uIndex].AccountID, gObj[uIndex].Name,
							headcode,
							__FILE__, __LINE__,
							gObj[uIndex].Connected);

						return 0;
					}

					if ( PacketStream.ExtractPacket(byDec) != 0 )
					{
						g_Log.AddC(TColor::Red,  "error-L1 : CStreamPacketEngine ExtractPacket Error : ip = %s account:%s name:%s HEAD:%x (%s,%d) State:%d",
							gObj[uIndex].m_PlayerData->Ip_addr, gObj[uIndex].AccountID, gObj[uIndex].Name, headcode, __FILE__, __LINE__, gObj[uIndex].Connected);

						return 0;
					}

					GSProtocol.ProtocolCore(headcode, byDec, ret, uIndex, 1);

				}
			}

			else if ( xcode == 0xC4 )
			{
				int ret = g_PacketEncrypt.Decrypt(&byDec[3], &recvbuf[lOfs + 3], size - 3);

				if ( ret < 0 )
				{
					g_Log.AddC(TColor::Red, "[Packet-Decrypt WORD] Error: ret < 0 %x/%x/%x)", recvbuf[lOfs], recvbuf[lOfs+1], recvbuf[lOfs+2]);
				}

				else
				{
					unsigned char* pDecBuf = byDec;
					
					headcode = pDecBuf[3];
					byDec[0] = 0xC2;
					WORD size = (ret &0xFFFF)+3;
					byDec[1] = SET_NUMBERH(size);
					byDec[2] = SET_NUMBERL(size);

					gObj[uIndex].m_PlayerData->PacketsPerSecond++;
					if(gObj[uIndex].m_PlayerData->PacketsPerSecond >= g_ConfigRead.data.common.PacketLimit)
					{
						g_Log.AddC(TColor::Red, "[ANTI-HACK] Packets Per Second: %d / %d", gObj[uIndex].m_PlayerData->PacketsPerSecond, g_ConfigRead.data.common.PacketLimit);
						GSProtocol.GCSendDisableReconnect(uIndex);
						//IOCP.CloseClient(uIndex);
						return 0;
					}

					CStreamPacketEngine_Server PacketStream;

					PacketStream.Clear();
					if ( PacketStream.AddData(byDec, ret+3) == 0 )
					{
						g_Log.AddC(TColor::Red,  "error-L1 : CStreamPacketEngine Adding Error : ip = %s account:%s name:%s HEAD:%x (%s,%d) State:%d",
							gObj[uIndex].m_PlayerData->Ip_addr, gObj[uIndex].AccountID, gObj[uIndex].Name, headcode, __FILE__, __LINE__, gObj[uIndex].Connected);
						return false;
					}

					if ( PacketStream.ExtractPacket(byDec) != 0 )
					{
						g_Log.AddC(TColor::Red,  "error-L1 : CStreamPacketEngine ExtractPacket Error : ip = %s account:%s name:%s HEAD:%x (%s,%d) State:%d",
							gObj[uIndex].m_PlayerData->Ip_addr, gObj[uIndex].AccountID, gObj[uIndex].Name, headcode, __FILE__, __LINE__, gObj[uIndex].Connected);
						return false;
					}
	
					GSProtocol.ProtocolCore(headcode, byDec, ret, uIndex, 1);
				}
			}
			else
			{
				CStreamPacketEngine_Server ps;
				ps.Clear();

				if ( ps.AddData(&recvbuf[lOfs], size) == 0 )
				{
					g_Log.AddC(TColor::Red,  "error-L1 : CStreamPacketEngine Adding Error : ip = %s account:%s name:%s HEAD:%x (%s,%d) State:%d",
						gObj[uIndex].m_PlayerData->Ip_addr, gObj[uIndex].AccountID, gObj[uIndex].Name, headcode, __FILE__, __LINE__, gObj[uIndex].Connected);
					return 0;
				}

				if ( ps.ExtractPacket(byDec) != 0 )
				{
					g_Log.AddC(TColor::Red,  "error-L1 : CStreamPacketEngine ExtractPacket Error : ip = %s account:%s name:%s HEAD:%x (%s,%d) State:%d",
						gObj[uIndex].m_PlayerData->Ip_addr, gObj[uIndex].AccountID, gObj[uIndex].Name, headcode, __FILE__, __LINE__, gObj[uIndex].Connected);
					return 0;
				}

				gObj[uIndex].m_PlayerData->PacketsPerSecond++;
				if(gObj[uIndex].m_PlayerData->PacketsPerSecond >= g_ConfigRead.data.common.PacketLimit)
				{
					g_Log.AddC(TColor::Red, "[ANTI-HACK] Packets Per Second: %d / %d", gObj[uIndex].m_PlayerData->PacketsPerSecond, g_ConfigRead.data.common.PacketLimit);
					GSProtocol.GCSendDisableReconnect(uIndex);
					//IOCP.CloseClient(uIndex);
					return 0;
				}

				GSProtocol.ProtocolCore(headcode, byDec, size, uIndex, 0); // here
			}

			lOfs += size; // wait
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

bool CIOCP::DataSend(int aIndex, unsigned char* lpMsg, DWORD dwSize, bool Encrypt)
{
	unsigned long SendBytes;
	_PER_SOCKET_CONTEXT * lpPerSocketContext;
	unsigned char * SendBuf;
	BYTE BUFFER[65535];

	if (aIndex < g_ConfigRead.server.GetObjectStartUserIndex())
	{
		return true;
	}

	EnterCriticalSection(&criti);

	if (ObjectMaxRange(aIndex) == FALSE)
	{
		g_Log.Add("error-L2 : Index(%d) %x %x %x ", dwSize, lpMsg[0], lpMsg[1], lpMsg[2]);
		LeaveCriticalSection(&criti);
		return false;
	}

	if (gStalkProtocol)
	{
		if (gStalkProtocolId[0] == gObj[aIndex].AccountID[0])
		{
			if (gStalkProtocolId[1] == gObj[aIndex].AccountID[1])
			{
				if (!strcmp(gStalkProtocolId, gObj[aIndex].AccountID))
				{
					g_Log.AddHeadHex("DATA SEND", lpMsg, dwSize);
				}
			}
		}
	}

	if (lpMsg[0] != 0xC1 && lpMsg[0] != 0xC2 &&
		lpMsg[0] != 0xC3 && lpMsg[0] != 0xC4)
	{
		g_Log.AddC(TColor::Red, "[ERROR] Trying to send packet without HEADER (%s)(%s)", gObj[aIndex].AccountID, gObj[aIndex].Name);
		LeaveCriticalSection(&criti);
		return FALSE;
	}

#ifdef EMU_NOCRYPT
	if (lpMsg[0] == 0xC3)
		lpMsg[0] = 0xC1;
	if (lpMsg[0] == 0xC4)
		lpMsg[0] = 0xC2;
#endif

#ifdef C3C4_DISABLECRYPT
	if (lpMsg[0] == 0xC3)
		lpMsg[0] = 0xC1;
	if (lpMsg[0] == 0xC4)
		lpMsg[0] = 0xC2;
#endif

	if (lpMsg[0] == 0xC3 || lpMsg[0] == 0xC4)
	{
		int ret;
		BYTE btsize;

		if (lpMsg[0] == 0xC3)
		{
			btsize = lpMsg[1];
			ret = g_PacketEncrypt.Encrypt(&ExSendBuf[2], &lpMsg[1], dwSize - 1);
			ExSendBuf[0] = 0xC3;
			ExSendBuf[1] = ret + 2;
			SendBuf = ExSendBuf;
			dwSize = ret + 2;
			lpMsg[1] = btsize;
		}
		else
		{
			btsize = lpMsg[2];
			ret = g_PacketEncrypt.Encrypt(&ExSendBuf[3], &lpMsg[2], dwSize - 2);
			ExSendBuf[0] = 0xC4;
			ExSendBuf[1] = SET_NUMBERH(ret + 3);
			ExSendBuf[2] = SET_NUMBERL(ret + 3);
			SendBuf = ExSendBuf;
			dwSize = ret + 3;
		}
	}

	else
	{
		SendBuf = lpMsg;
	}
	

	if ( gObj[aIndex].Connected < PLAYER_CONNECTED )
	{
		LeaveCriticalSection(&criti);
		return false;
	}

	lpPerSocketContext= gObj[aIndex].PerSocketContext;

	if ( dwSize > sizeof(lpPerSocketContext->IOContext[0].Buffer))
	{
		g_Log.Add("Error : Max msg(%d) %s %d", dwSize, __FILE__, __LINE__);
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
			g_Log.Add("(%d)error-L2 MAX BUFFER OVER %d %d %d [%s][%s]", aIndex, lpIoCtxt->nTotalBytes, lpIoCtxt->nSecondOfs, dwSize, gObj[aIndex].AccountID, gObj[aIndex].Name);
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
		g_Log.Add("(%d)error-L2 MAX BUFFER OVER %d %d [%s][%s]", aIndex, lpIoCtxt->nTotalBytes, dwSize, gObj[aIndex].AccountID, gObj[aIndex].Name);
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
	

	if ( WSASend( gObj[aIndex].m_socket, &lpIoCtxt->wsabuf , 1, &SendBytes, 0, &lpIoCtxt->Overlapped, NULL) == -1 )
	{

		if ( WSAGetLastError() != WSA_IO_PENDING )	
		{
			lpIoCtxt->nWaitIO = 0;
			

			if ( lpIoCtxt->wsabuf.buf[0] == 0xC1 )
			{
				g_Log.Add("(%d)WSASend(%d) failed with error [%x][%x] %d %s ", __LINE__, aIndex, (BYTE)lpIoCtxt->wsabuf.buf[0],
					(BYTE)lpIoCtxt->wsabuf.buf[2], WSAGetLastError(), gObj[aIndex].m_PlayerData->Ip_addr);
			}
			else if ( lpIoCtxt->wsabuf.buf[0] == 0xC2 )
			{
				g_Log.Add("(%d)WSASend(%d) failed with error [%x][%x] %d %s ", __LINE__, aIndex, (BYTE)lpIoCtxt->wsabuf.buf[0],
					(BYTE)lpIoCtxt->wsabuf.buf[3], WSAGetLastError(), gObj[aIndex].m_PlayerData->Ip_addr);
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

	if ( WSASend(gObj[aIndex].m_socket, &lpIoCtxt->wsabuf, 1, &SendBytes, 0, &lpIoCtxt->Overlapped, NULL) == -1 )
	{
		if ( WSAGetLastError() != WSA_IO_PENDING )
		{
			g_Log.Add("WSASend(%d) failed with error %d %s ", __LINE__, WSAGetLastError(), gObj[aIndex].m_PlayerData->Ip_addr);
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

	if ( WSASend(gObj[aIndex].m_socket, &lpIoCtxt->wsabuf, 1, &SendBytes, 0, &lpIoCtxt->Overlapped, NULL) == -1 )
	{
		if ( WSAGetLastError() != WSA_IO_PENDING )
		{
			g_Log.Add("WSASend(%d) failed with error %d %s ", __LINE__, WSAGetLastError(), gObj[aIndex].m_PlayerData->Ip_addr);
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
		g_Log.Add("CreateIoCompletionPort: %d", GetLastError() );
		return FALSE;
	}

	gObj[ClientIndex].PerSocketContext->dwIOCount = 0;
	return TRUE;
}

void CIOCP::CloseClient(_PER_SOCKET_CONTEXT * lpPerSocketContext, int result)
{
	int index = -1;
	index = lpPerSocketContext->nIndex ;

	if (index >= g_ConfigRead.server.GetObjectStartUserIndex() && index < g_ConfigRead.server.GetObjectMax())
	{
		if (gObj[index].m_bOff)
		{
			return;
		}

		if (gObj[index].m_socket != INVALID_SOCKET)
		{
			if (closesocket(gObj[index].m_socket) == -1)
			{
				if (WSAGetLastError() != WSAENOTSOCK)
				{
					return;
				}
			}
			gObj[index].m_socket = INVALID_SOCKET;
		}

		gObjDel(index);
	}
}


void CIOCP::CloseClient(int index) // go to game
{
	if ( index < 0 || index > g_ConfigRead.server.GetObjectMax()-1 )
	{
		g_Log.Add("error-L1 : CloseClient index error");
		return;
	}

	if ( gObj[index].Connected == PLAYER_EMPTY )
	{
		g_Log.Add("error-L1 : CloseClient connect error");
		return;
	}

	EnterCriticalSection(&criti);

	if ( gObj[index].m_socket != INVALID_SOCKET && gObj[index].m_bOff == false )
	{
		closesocket(gObj[index].m_socket );
		gObj[index].m_socket = INVALID_SOCKET;
	}

	else
	{
	//	closesocket(gObj[index].m_socket );
	}

	LeaveCriticalSection(&criti);
}

void CIOCP::ResponErrorCloseClient(int index)
{
	if ( index < 0 || index > g_ConfigRead.server.GetObjectMax()-1 )
	{
		g_Log.Add("error-L1 : CloseClient index error");
		return;
	}

	if ( gObj[index].Connected == PLAYER_EMPTY )
	{
		g_Log.Add("error-L1 : CloseClient connect error");
		return;
	}

	EnterCriticalSection(&criti);

	closesocket(gObj[index].m_socket);
	gObj[index].m_socket = INVALID_SOCKET;
	gObjDel(index);

	LeaveCriticalSection(&criti);
}


//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


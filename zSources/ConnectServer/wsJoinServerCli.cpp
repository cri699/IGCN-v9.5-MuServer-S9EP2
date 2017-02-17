//////////////////////////////////////////////////////////////////////
// wsJoinServerCli.cpp
// ------------------------------
// Decompiled by Deathway
// Date : 2007-03-09
// ------------------------------

// GS-N 0.99.60T Status : Completed
//	GS-N	1.00.18	JPN	0x0048D170	-	Completed

#include "stdafx.h"
#include "wsJoinServerCli.h"
#include "TLog.h"

wsJoinServerCli::wsJoinServerCli()
{

}

wsJoinServerCli::~wsJoinServerCli()
{
	return;
}

BOOL wsJoinServerCli::SetProtocolCore(WsProtocolCore pc)
{
	this->wsProtocolCore = pc;
	return TRUE;
}
	
BOOL wsJoinServerCli::Connect(LPSTR ip_addr, WORD port, DWORD WinMsgNum)
{
	hostent* host;
	sockaddr_in addr;
	int nResult;

	host = 0;

	if ( this->m_hWnd  == 0 )
	{
		return FALSE;
	}
	addr.sin_family = AF_INET;
	addr.sin_port =  htons(port);
	addr.sin_addr.S_un.S_addr = -1;//inet_addr(ip_addr);

	if ( addr.sin_addr.S_un.S_addr == -1 )
	{
		host = gethostbyname(ip_addr);
		if ( host == 0 )
		{
			return FALSE;
		}

		memcpy(&addr.sin_addr.S_un.S_addr, *host->h_addr_list  , host->h_length  );	
	}

	int ReceiveTimeout = 3000; 
	int e = setsockopt(this->m_socket, SOL_SOCKET, SO_RCVTIMEO, (char*)&ReceiveTimeout, sizeof(int));

	nResult=connect(this->m_socket, (sockaddr*)&addr, 16);

	if (nResult== -1)
	{
		if ( WSAGetLastError() != WSAEWOULDBLOCK)
		{
			this->Close(this->m_socket );
			return FALSE;
		}
	}

	nResult=WSAAsyncSelect(this->m_socket, this->m_hWnd , WinMsgNum, FD_READ|FD_WRITE|FD_CLOSE);

	if ( nResult == -1 )
	{
		closesocket(this->m_socket );
		return FALSE;
	}
	this->m_SendBufLen=0;
	this->SetConnect(TRUE);

	return TRUE;
}


BOOL wsJoinServerCli::DataSend(PCHAR buf, int len)
{
	int nResult;
	int nLeft;
	int nDx;	// Degeneration X :)
	LPBYTE sendbuf;
	int* sendbuflen;
	
	if ( len <= 0 )
	{
		return 0;
	}

	if ( this->m_socket  == INVALID_SOCKET )	// 
	{
		return 0;
	}

	nDx = 0;

	sendbuf = this->m_SendBuf;
	sendbuflen= &len;	// Strange, dangeorus

	if ( this->m_SendBufLen   > 0 )
	{
		if ( (this->m_SendBufLen +len) < 819200 )
		{
			memcpy( &this->m_SendBuf[this->m_SendBufLen]  , buf, len);	// Review this later

			this->m_SendBufLen +=len;
			return 1;
		}
		this->Close();
		return FALSE;
	}

	nLeft= *sendbuflen;

	while ( true )
	{
		nResult=send(this->m_socket, (char*)&buf[nDx], (*sendbuflen - nDx), 0);

		if ( nResult == -1)
		{
			if ( WSAGetLastError() != WSAEWOULDBLOCK )
			{
				*sendbuflen=0;
				this->Close(this->m_socket );
				return FALSE;
			}

			if ( (*sendbuflen + nLeft) > 819200 )
			{
				*sendbuflen = 0;
				this->Close(this->m_socket);
				return FALSE;
			}

			if ( nDx >= 0 )
			{
				memcpy( &this->m_SendBuf[this->m_SendBufLen],  &buf[nDx], nLeft);
				
				this->m_SendBufLen +=nLeft;

				return TRUE;
			}
		}
		else if ( nResult == 0 )
		{
			break;
		}

		if ( nResult >0 )
		{
			nDx += nResult;
			nLeft -= nResult;
			*sendbuflen -= nResult;
		}
		if ( nLeft <= 0)
		{
			break;
		}
	}
	return TRUE;
}



BOOL wsJoinServerCli::FDWRITE_MsgDataSend()
{
	int nResult;
	int nDx;	// Degeneration X :)
	LPBYTE sendbuf;
	int* sendbuflen;

	nDx=0;
	sendbuf=&this->m_SendBuf[0];
	sendbuflen=&this->m_SendBufLen ;

	if (this->m_SendBufLen < 1)
	{
		return TRUE;
	}

	while (*sendbuflen>0)
	{
		nResult=send(this->m_socket, (PCHAR)&sendbuf[nDx], *sendbuflen-nDx, 0);
		if (nResult == -1)
		{
			if ( WSAGetLastError() != WSAEWOULDBLOCK)
			{
				this->Close();
				return FALSE;
			}
			else
			{
				break;
			}
		}
		if (nResult <= 0)
		{
			break;
		}

		if (nResult > 0)
		{
			nDx += nResult;
			*sendbuflen -= nResult;
		}
	}
	return TRUE;
}


int wsJoinServerCli::DataRecv()
{
	int nResult;
	unsigned char* recvbuf;
	int* recvbuflen;
	int lOfs;
	int size;
	BYTE headcode;

	recvbuf= (UCHAR*)&this->m_RecvBuf[0]   ;
	recvbuflen=&this->m_RecvBufLen ;
	nResult=recv(this->m_socket , (CHAR*)&recvbuf[*recvbuflen], (819200-*recvbuflen), 0);

	if ( nResult == 0 )
	{

		return 1;
	}
	else if (nResult == -1 )
	{
		if ( WSAGetLastError() == WSAEWOULDBLOCK )
		{
			return 1;
		}
		return 1;
	}

	*recvbuflen += nResult;

	if (*recvbuflen<3)
	{
		return 3;
	}

	lOfs=0;
	size=0;

	while ( true )
	{
		size = *(WORD*)&recvbuf[lOfs+1];
		headcode = recvbuf[lOfs];

		if ( size <= 0 )
		{
			return 0;
		}

		if ( size <= *recvbuflen )
		{
			this->wsProtocolCore(headcode, &recvbuf[lOfs], size);
			lOfs += size;
			*recvbuflen -= size;
			
			if ( *recvbuflen <= 0)
			{
				break;
			}
		}
		
		else if ( lOfs > 0 )
		{
			if ( *recvbuflen < 1 ) 
			{
				break;
			}

			memcpy(recvbuf, &recvbuf[lOfs], *recvbuflen);
			break;
		}
		else
		{
			break;
		}
	}
	return 0;
}




//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


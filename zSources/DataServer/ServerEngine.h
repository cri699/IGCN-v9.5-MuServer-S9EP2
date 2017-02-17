// ServerEngine.h: interface for the CServerEngine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SERVERENGINE_H__7F7AC7E2_C979_41EA_95BF_375D4A6729C5__INCLUDED_)
#define AFX_SERVERENGINE_H__7F7AC7E2_C979_41EA_95BF_375D4A6729C5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "giocp.h"

enum eSERVER_STATE
{
	SS_CLOSED,
	SS_CONNECTED,
	SS_LOGGED,
	SS_GUILD,
	SS_CHAT,
};

typedef void (*WsProtocolCore)(int, BYTE, LPBYTE, int);

#define MAX_SERVER_TYPE (ST_EXDATASERVER+1)

struct tagSERVER_ENGINE
{
	int m_Index;
	SOCKET m_Socket;
	eSERVER_STATE m_State;
	eSERVER_TYPE m_Type;
	char m_ServerIp[16];
	struct _PER_SOCKET_CONTEXT * PerSocketContext;
	WsProtocolCore m_ProtocolCore;
	WORD m_ServerCode;
	WORD m_ServerGroup;
	unsigned short m_ConnectPort;
};

void gObjServerInit();
int gObjServerAddSearch();
int gObjServerAdd(SOCKET Socket, char * Ip, int ServerIndex, eSERVER_TYPE eServerType);
void gObjServerDel(int aIndex);
void LoadAllowableIpList(LPSTR filename);

void gObjConnectStatSyncThread();

struct IPList
{
	char m_IP[16];
};

extern IPList g_IpList[25];
extern int ipcount;
extern int servercount;
extern tagSERVER_ENGINE * g_Server;

#endif // !defined(AFX_SERVERENGINE_H__7F7AC7E2_C979_41EA_95BF_375D4A6729C5__INCLUDED_)


//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


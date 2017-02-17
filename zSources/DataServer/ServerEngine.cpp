// ServerEngine.cpp: implementation of the CServerEngine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ServerEngine.h"
#include "DataServer.h"
#include "TLog.h"
#include "Sprotocol.h"
#include "DSprotocol.h"
#include "EDSprotocol.h"

#define szModule "ServerEngine"

IPList g_IpList[25];
tagSERVER_ENGINE * g_Server = NULL;
int ipcount = 0;
int servercount = 0;

void gObjServerInit()
{
	g_Server = new tagSERVER_ENGINE[g_dwMaxServerGroups]; 

	if ( g_Server == NULL )
	{
		g_Log.AddMSB("[Server Engine] Memory Allocation Error");
		return;
	}

	memset(g_Server, 0, sizeof(tagSERVER_ENGINE) * g_dwMaxServerGroups);

	for(int i=0;i<g_dwMaxServerGroups;i++)
	{
		g_Server[i].m_Index = -1;
		g_Server[i].m_Socket = INVALID_SOCKET;
		g_Server[i].m_State = SS_CLOSED;
		g_Server[i].m_Type = ST_NONE;
		g_Server[i].m_ProtocolCore = NULL;
		g_Server[i].PerSocketContext = new _PER_SOCKET_CONTEXT;
		g_Server[i].m_ServerCode = -1;
	}

	servercount = 0;
	if(g_iConnectStatSyncEnable == TRUE)
	{
		DWORD ThreadId;
		CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(gObjConnectStatSyncThread), nullptr, 0, &ThreadId);
	}
}

int gObjServerAddSearch()
{
	if ( g_Server == NULL )
		return -1;

	for(int i=0;i<g_dwMaxServerGroups;i++)
	{
		if ( g_Server[i].m_Index == -1 )
		{
			return i;
		}
	}

	return -1;
}

int gObjServerAdd(SOCKET Socket, char * Ip, int ServerIndex, eSERVER_TYPE eServerType)
{
	g_Server[ServerIndex].m_Index = ServerIndex;
	memcpy(g_Server[ServerIndex].m_ServerIp, Ip, 16);
	g_Server[ServerIndex].m_State = SS_CONNECTED;
	g_Server[ServerIndex].m_Type = eServerType;
	g_Server[ServerIndex].m_Socket = Socket;

	switch ( g_Server[ServerIndex].m_Type )
	{
		case ST_JOINSERVER:
			g_Server[ServerIndex].m_ProtocolCore = CLoginServerProtocol::ProtocolCore;
			break;
		case ST_DATASERVER:
			g_Server[ServerIndex].m_ProtocolCore = CDataServerProtocol::ProtocolCore;
			break;
		case ST_EXDATASERVER:
			g_Server[ServerIndex].m_ProtocolCore = CExDataServerProtocol::ProtocolCore;
			break;
	}
	
	g_Log.Add("[Server Engine] Connect : Index : %d - IP : %s - ServerType : %d",
		ServerIndex, Ip, eServerType);

	servercount++;
	return ServerIndex;
}


void gObjServerDel(int aIndex)
{

	EnterCriticalSection(&scriti);

	if(g_Server[aIndex].m_Type == ST_JOINSERVER)
	{
		m_JSProtocol.DisconnectServer(g_Server[aIndex].m_ServerCode);
	}

	g_Server[aIndex].m_Index = -1;
	g_Server[aIndex].m_ServerCode = -1;
	g_Server[aIndex].m_Socket = INVALID_SOCKET;
	g_Server[aIndex].m_ServerIp[0];
	g_Server[aIndex].m_State = SS_CLOSED;
	//g_Server[aIndex].m_Type = ST_NONE;
	g_Server[aIndex].m_ProtocolCore = NULL;
	g_Server[aIndex].m_ConnectPort = -1;
	servercount--;

	LeaveCriticalSection(&scriti);
}

void LoadAllowableIpList(LPSTR filename)
{
	memset(g_IpList,0,sizeof(g_IpList));

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(filename);

	if (res.status != pugi::status_ok)
	{
		g_Log.MsgBox("LoadAllowedIpList() error - file load error (%s)", res.description());
		return;
	}

	pugi::xml_node main = file.child("AllowedIPList");
	int count = 0;

	for (pugi::xml_node ip = main.child("IP"); ip; ip = ip.next_sibling())
	{
		strcpy(g_IpList[count].m_IP, ip.attribute("Address").as_string());
		count++;
	}

	ipcount = count;
	g_Log.Add("%s File Loaded successfuly", filename);
}
//memb___id

void gObjConnectStatSyncProc()
{
	LPSTR szAccount;
	for(int i=0;i<m_JSProtocol.m_UserData.m_MuLoginUserDataCount;i++)
	{
		szAccount = m_JSProtocol.m_UserData.GetAccountID(i);
		m_DSProtocol.m_MembStatDB.ExecQuery("SELECT * from MEMB_STAT WHERE memb___id = '%s'",szAccount);
		m_DSProtocol.m_MembStatDB.Fetch();
		int iConnectStat = m_DSProtocol.m_MembStatDB.GetAsInteger("ConnectStat");
		m_DSProtocol.m_MembStatDB.Close();
		if(iConnectStat == FALSE)
		{
			m_JSProtocol.DisconnectPlayer(m_JSProtocol.m_UserData.GetAccountID(i));
			m_DSProtocol.m_MembStatDB.ExecQuery("EXEC WZ_DISCONNECT_MEMB '%s'", szAccount);
			g_Log.Add("[%s] Force Disconnect ConnectStat = 0 but Player is Online");
		}
	}
}

void gObjConnectStatSyncThread()
{
	while(true)
	{
		gObjConnectStatSyncProc();
		Sleep(2000);
	}
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


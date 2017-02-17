//////////////////////////////////////////////////////////////////////
// LProtocol.cpp
#include "StdAfx.h"
#include "LProtocol.h"
#include "giocp.h"
#include "Sprotocol.h"
#include "ServerEngine.h"
#include "TLog.h"
#include "readscript.h"
#include "WzUdp.h"
#include "prodef.h"
#define szModule "LProtocol"
CConServ g_ConnectServer;
P_ADD_NEWS gObjNews[MAX_NEWS]; 
std::vector<ServerList> g_CServerList;
std::map<int, ServerInfo> g_ConnectedServers;
WzUdp g_CSSocket;
static char Title[12];
bool newssent = false;
CConServ::CConServ()
{

}

CConServ::~CConServ()
{

}

void CConServ::ConnectResultSend(int aIndex)
{
	PMSG_HELLO pMsg = {0};
	pMsg.h.c = 0xC1;
	pMsg.h.headcode = 0x00;
	pMsg.h.size = sizeof(pMsg);
	pMsg.result = 0x01;

	DataSend(aIndex,(LPBYTE)&pMsg,sizeof(pMsg));;
	/*if(!newssent)
	{
		newssent = true;
		
	}*/
	
}

void CConServ::ServerListSend(int aIndex)
{
	PMSG_SERVERSLIST_COUNT * pMsg;///(0xC2, 0xF4, 0x06);
	PMSG_SERVERLIST_SERVER * pServer;
	BYTE cBUFF[65535];
	std::map<int, ServerInfo>::iterator It;

	int Count = 0;

	for(std::vector<ServerList>::iterator It2 = g_CServerList.begin(); It2 != g_CServerList.end(); It2++)
	{
		It = g_ConnectedServers.find(It2->btServerCode);

		if(It == g_ConnectedServers.end())
			continue;

		if(GetTickCount() - It->second.TickCount > 5000)
		{
			g_ConnectedServers.erase(It);
			continue;
		}

		pServer = (PMSG_SERVERLIST_SERVER *)(cBUFF + sizeof(PMSG_SERVERSLIST_COUNT) + (Count*sizeof(PMSG_SERVERLIST_SERVER)));
		
		pServer->ServerCode	= It2->btServerCode;
		pServer->Percent	= It->second.Percent;

		Count++;
	}

	WORD PacketSize = sizeof(PMSG_SERVERSLIST_COUNT) + (Count * sizeof(PMSG_SERVERLIST_SERVER));

	pMsg = (PMSG_SERVERSLIST_COUNT *)(cBUFF);

	pMsg->h.c = 0xC2;
	pMsg->h.headcode = 0xF4;
	pMsg->h.subcode = 0x06;

	pMsg->CountH		= HIBYTE(Count);
	pMsg->CountL		= LOBYTE(Count);

	pMsg->h.sizeL	= LOBYTE(PacketSize);
	pMsg->h.sizeH	= HIBYTE(PacketSize);

	DataSend(aIndex, cBUFF, PacketSize);

//	this->SendNews(aIndex);
}

void CConServ::GetServerInfo(int aIndex, USHORT id)
{
	GS_CONNECT_INFO pMsg	= {0};

	pMsg.h.c = 0xC1;
	pMsg.h.headcode = 0xF4;
	pMsg.h.size = sizeof(pMsg);
	pMsg.SubHead			= 0x03;

	for(std::vector<ServerList>::iterator It = g_CServerList.begin(); It != g_CServerList.end(); It++)
	{
		if(It->btServerCode == id)
		{
			char * IP = It->cIp;
			memcpy(&pMsg.IP, IP , 16);

			pMsg.Port = It->wServerPort;

			DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));

			g_Log.Add(0, szModule, __FUNCTION__, "INDEX : %d; SERVER SELECTED (%d) (%s:%d)", aIndex, id, pMsg.IP, pMsg.Port);
			
			break;
		}
	}
}

void CConServ::GetServerList(int aIndex)
{
	if(g_ConnectedServers.size() > 0)
	{	
		this->ServerListSend(aIndex);
		if(g_Server[aIndex].newsSent == 0)
		{
			this->SendNews(aIndex);
			g_Server[aIndex].newsSent = 1;
		}
	}
	else
	{
		g_Log.Add(0, szModule, __FUNCTION__, "INDEX : %d; no active servers", aIndex);
		CloseClient(aIndex);
	//	gObjServerDel(aIndex);
	}	
}
void CConServ::LoadServerList(LPSTR szFile)
{
	if ( (SMDFile = fopen(szFile, "r")) == NULL )
	{
		MessageBoxA(0,"CConServer::LoadServerList() error","CRITICAL ERROR",0);
		ExitProcess(1);
		return;
	}

	SMDToken Token;
	int aIndex;
	ServerList m_Server;
	g_CServerList.clear();
	g_ConnectedServers.clear();
	while(true)
	{
		Token = GetToken();

		if(Token == END)
			break;

		if(Token == NUMBER)
		{
			aIndex = TokenNumber;

			while(true)
			{
				Token = GetToken();

				if(strcmp("end",TokenString) == 0)
					break;

				m_Server.btServerCode = TokenNumber;

				Token = GetToken();
				strcpy(m_Server.cIp,TokenString);

				Token = GetToken();

				m_Server.wServerPort = TokenNumber;

				g_CServerList.push_back(m_Server);
			}
		}
	}
	g_Log.Add(0, szModule, __FUNCTION__, "Loaded %d servers", g_CServerList.size());
}
int newscount = 0;
void CConServ::LoadNewsFile(LPSTR szFile)
{
	for(int i = 0; i < 10; i++ )
	{
		gObjNews[i].dateColor = 0;
		gObjNews[i].day = 0;
		gObjNews[i].month = 0;
		gObjNews[i].textColor = 0;
		gObjNews[i].textLen = 0;
		memset(gObjNews[i].title,0,sizeof(gObjNews[i].title));
		gObjNews[i].titleColor = 0;
		gObjNews[i].year = 0;
	}

	 if ( (SMDFile = fopen(szFile, "r")) == NULL )
		 {
			MessageBoxA(0,"CQuestManager::LoadFile() error","CRITICAL ERROR",0);
			ExitProcess(1);
			return;
		 }
		SMDToken Token;
		int aIndex = 0;
		int iIndex = 0;
		// Date RGB
		BYTE dateR = 0;
		BYTE dateG = 0;
		BYTE dateB = 0;
		// Title RGB
		BYTE TitleR = 0;
		BYTE TitleG = 0;
		BYTE TitleB = 0;
		// Text RGB
		BYTE TextR = 0;
		BYTE TextG = 0;
		BYTE TextB = 0;

		//Date

		BYTE Day;
		BYTE Month;
		WORD Year;

		char szTitle[40] = {0};
		char szText[2048] = {0};

		while(true)
		 {
			 Token = GetToken();
			 if(Token == END)
				break;
			 if(Token == NUMBER)
			 {
				 aIndex = TokenNumber;

				 while(true)
				 {
					 Token = GetToken();
					 if(strcmp("end",TokenString) == 0)
						 break;

					 if(aIndex == 0)
					 {
						iIndex = TokenNumber;

						Token = GetToken();
						dateR = TokenNumber;

						Token = GetToken();
						dateG = TokenNumber;

						Token = GetToken();
						dateB = TokenNumber;

						Token = GetToken();
						TitleR = TokenNumber;

						Token = GetToken();
						TitleG = TokenNumber;

						Token = GetToken();
						TitleB = TokenNumber;

						Token = GetToken();
						TextR = TokenNumber;

						Token = GetToken();
						TextG = TokenNumber;

						Token = GetToken();
						TextB = TokenNumber;

						Token = GetToken();
						memcpy(szTitle,TokenString,sizeof(szTitle));

						Token = GetToken();
						memcpy(szText,TokenString,sizeof(szText));

						Token = GetToken();
						Day = TokenNumber;

						Token = GetToken();
						Month = TokenNumber;

						Token = GetToken();
						Year = TokenNumber;
						// store data
						gObjNews[iIndex].dateColor = RGB(dateR,dateG,dateB);
						gObjNews[iIndex].titleColor = RGB(TitleR,TitleG,TitleB);
						gObjNews[iIndex].textColor = RGB(TextR,TextG,TextB);

						strcpy_s(gObjNews[iIndex].Text,szText);
						strcpy_s(gObjNews[iIndex].title,szTitle);
						gObjNews[iIndex].day = Day;
						gObjNews[iIndex].month = Month;
						gObjNews[iIndex].year = Year;
						newscount++;
					 }
					 else if(aIndex == 1)
					 {
						 memcpy(Title, TokenString, sizeof(Title));
					 }
				 }
			 }
		 }

		 fclose(SMDFile);
		 g_Log.Add(0,szModule,"::LoadNewsFile()"," File Loaded successfuly");


}

void CConServ::SendNews(int aIndex)
{

	PMSG_SEND_TITLE pTitle;
	pTitle.h.c = 0xC1;
	pTitle.h.headcode = 0xFA;
	pTitle.h.subcode = 0x00;
	memcpy(pTitle.ServerName, Title, sizeof(pTitle.ServerName));

	DataSend(aIndex, (LPBYTE)&pTitle, pTitle.h.size);

	PMSG_SEND_NEWS pMsg = {0};

	pMsg.h.c = 0xC2;
	pMsg.h.headcode = 0xFA;
	pMsg.h.subcode = 0x01;
	
	BYTE buffer[7000];

	for(int i=0;i<newscount;i++)
	{
		pMsg.Day = gObjNews[i].day;
		pMsg.Month = gObjNews[i].month;
		pMsg.year = gObjNews[i].year;

		strcpy_s(pMsg.title,gObjNews[i].title);

		pMsg.dateColor = gObjNews[i].dateColor;
		pMsg.titleColor = gObjNews[i].titleColor;
		pMsg.textColor = gObjNews[i].textColor;

		
		int textlen = strlen(gObjNews[i].Text);
		pMsg.textLen = textlen;
		pMsg.h.sizeH = SET_NUMBERH( sizeof(PMSG_SEND_NEWS)+textlen);
		pMsg.h.sizeL = SET_NUMBERL( sizeof(PMSG_SEND_NEWS)+textlen);
		memcpy(buffer, &pMsg, sizeof(PMSG_SEND_NEWS));
		memcpy(&buffer[sizeof(PMSG_SEND_NEWS)], gObjNews[i].Text, textlen);

		DataSend(aIndex, (LPBYTE)buffer, sizeof(PMSG_SEND_NEWS)+textlen);
	}
	g_Log.Add(0,szModule,"::SendNews()","News Sent. Amount : %d",newscount);
}



void LProtocolCore(int aIndex, BYTE hCode, LPBYTE aRecv, int aLen)
{
	switch (hCode)
	{
		case 0xF4:
		{
			switch(aRecv[3])
			{
				case CS_SERVER_SELECT:
					g_ConnectServer.GetServerInfo(aIndex,MAKEWORD(aRecv[4],aRecv[5]));
					break;
				case CS_CLIENT_CONNECT:
					g_ConnectServer.GetServerList(aIndex);
					break;
			}
		}
		break;
	}
}

void LSProtocolCore(BYTE hCode, LPBYTE aRecv, int aLen)
{
	switch ( hCode )
	{
		case 0x01:
			AddServer(reinterpret_cast<PMSG_SERVERINFO *>(aRecv));
			break;
		case 0x02:
			break;
	}
}

void AddServer(PMSG_SERVERINFO * pMsg)
{
	std::map<int, ServerInfo>::iterator It;

	It = g_ConnectedServers.find(pMsg->ServerCode);

	if(It == g_ConnectedServers.end())
	{
		ServerInfo m_Server;
		
		m_Server.UserCount = pMsg->UserCount;
		m_Server.MaxUserCount = pMsg->MaxUserCount;
		m_Server.Percent = pMsg->Percent;
		m_Server.TickCount = GetTickCount();

		g_ConnectedServers.insert(std::pair<int, ServerInfo>(pMsg->ServerCode, m_Server));
	}
	else
	{
		It->second.UserCount = pMsg->UserCount;
		It->second.MaxUserCount = pMsg->MaxUserCount;
		It->second.Percent = pMsg->Percent;
		It->second.TickCount = GetTickCount();
	}
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


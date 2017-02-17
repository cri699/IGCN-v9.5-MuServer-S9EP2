//////////////////////////////////////////////////////////////////////
// protocol.cpp
#include "stdafx.h"
#include "protocol.h"
#include "IOCP.h"
#include "ServerData.h"
#include "ConnectServer.h"
#include "TLog.h"
#include "Engine.h"
#include "winutil.h"

void UDPInit()
{
	g_UDPPort.Close();
	g_UDPPort.CreateSocket();
	g_UDPPort.RecvSet(g_ConnectServerUDP);
	g_UDPPort.SetProtocolCore(UDPProtocolCore);
	g_UDPPort.Run();
}

void CSProtocolCore(BYTE protoNum, BYTE *aRecv, int aLen, int aIndex, bool Encrypt, int serial)
{

	switch ( protoNum )
	{
		case 0xF4:
			{
				switch(aRecv[3])
				{
					case CS_CLIENT_CONNECT:
						SCSendServerList(aIndex);
						break;
					case CS_SERVER_SELECT:
						SCSendServerInfo(aIndex, (PMSG_SERVER_SELECT *)aRecv);
						break;
				}
			}
			break;

		case 0x05:
			SCSendAutoUpdateData(aIndex, (PMSG_CLIENTVERSION *)aRecv);
			break;
	}
}

void UDPProtocolCore(BYTE hCode, LPBYTE aRecv, int aLen)
{
	switch ( hCode )
	{
		case 0x01:
			UDPSetServerInfo((PMSG_SERVERINFO *)aRecv);
			break;
	}
}

void UDPSetServerInfo(PMSG_SERVERINFO * aRecv)
{
	for(int i=0;i<100;i++)
	{
		if(m_ServerData.m_Servers[i].Code == aRecv->ServerCode)
		{
			m_ServerData.m_Servers[i].MaxUserCount = aRecv->MaxUserCount;
			m_ServerData.m_Servers[i].Percent = aRecv->Percent;
			m_ServerData.m_Servers[i].PlayType = aRecv->PlayType;
			m_ServerData.m_Servers[i].UserCount = aRecv->UserCount;
			m_ServerData.m_Servers[i].TickCount = GetTickCount();
		}
	}
}

void SCSendServerList(int aIndex)
{
	PMSG_SERVERSLIST_COUNT * pMsg;///(0xC2, 0xF4, 0x06);
	PMSG_SERVERLIST_SERVER * pServer;
	BYTE cBUFF[65535];

	if(Users[aIndex].News == 0)
	{
		SCSendNews(aIndex);
		Users[aIndex].News = 1;
	}

	int Count = 0;

	for(int i=0;i<100;i++) // MAX_SERVER - 100
	{
		pServer = (PMSG_SERVERLIST_SERVER *)(cBUFF + sizeof(PMSG_SERVERSLIST_COUNT) + (Count*sizeof(PMSG_SERVERLIST_SERVER)));

		if(m_ServerData.m_Servers[i].Visible == true &&
			m_ServerData.m_Servers[i].Port != 0 && m_ServerData.m_Servers[i].TickCount != 0)
		{
			pServer->wServerCode = m_ServerData.m_Servers[i].Code;
			pServer->btPercent = m_ServerData.m_Servers[i].Percent;
			pServer->btPlayType = m_ServerData.m_Servers[i].PlayType;

			Count++;
		}
	}

	if(Count == 0)
	{
		g_Log.AddC(TColor::Red, "[Server] No active Game Servers found - disconnect: (Index: [%d])", aIndex);
		IOCP.CloseClient(aIndex);
		return;
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

	IOCP.DataSend(aIndex, cBUFF, PacketSize);

	g_Log.Add("[Server] Sent Server List COUNT: [%d] (Index: [%d])", Count, aIndex);
}

void SCSendServerInfo(int aIndex, PMSG_SERVER_SELECT * aRecv)
{
	PMSG_CONNECT_INFO pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.headcode = 0xF4;
	pMsg.SubHead = 0x03;
	pMsg.h.size = sizeof(pMsg);
	
	for(int i=0;i<100;i++)
	{
		if(m_ServerData.m_Servers[i].Code == aRecv->ServerCode)
		{
			pMsg.Port = m_ServerData.m_Servers[i].Port;
			memcpy(&pMsg.IP, m_ServerData.m_Servers[i].IP, sizeof(pMsg.IP));

			IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);

			g_Log.Add("[Server] Connecting to Server: %s (IP: [%s] PORT: [%d]) (Players: [%d] / Max: [%d])",
				m_ServerData.m_Servers[i].Name, m_ServerData.m_Servers[i].IP, m_ServerData.m_Servers[i].Port,
				m_ServerData.m_Servers[i].UserCount, m_ServerData.m_Servers[i].MaxUserCount);

			return;
		}
	}
}

void SCConnectResultSend(int aIndex, BYTE btResult)
{
	PMSG_RESULT pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.headcode = 0x00;
	pMsg.result = btResult;
	pMsg.h.size = sizeof(pMsg);

	IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

void SCSendNews(int aIndex)
{
	PMSG_SEND_TITLE pTitle;
	pTitle.h.c = 0xC1;
	pTitle.h.headcode = 0xFA;
	pTitle.h.subcode = 0x00;
	memcpy(pTitle.ServerName, m_ServerData.szTitle, sizeof(pTitle.ServerName));

	IOCP.DataSend(aIndex, (LPBYTE)&pTitle, pTitle.h.size);

	PMSG_SEND_NEWS pMsg = {0};

	pMsg.h.c = 0xC2;
	pMsg.h.headcode = 0xFA;
	pMsg.h.subcode = 0x01;
	
	BYTE buffer[7000];

	for(int i=0;i<10;i++)
	{
		if(m_ServerData.m_News[i].Used == false)
			continue;

		pMsg.Day = m_ServerData.m_News[i].day;
		pMsg.Month = m_ServerData.m_News[i].month;
		pMsg.year = m_ServerData.m_News[i].year;

		strcpy(pMsg.title,m_ServerData.m_News[i].title);

		pMsg.dateColor = m_ServerData.m_News[i].dateColor;
		pMsg.titleColor = m_ServerData.m_News[i].titleColor;
		pMsg.textColor = m_ServerData.m_News[i].textColor;

		
		int textlen = strlen(m_ServerData.m_News[i].Text);
		pMsg.textLen = textlen;
		pMsg.h.sizeH = SET_NUMBERH( sizeof(PMSG_SEND_NEWS)+textlen);
		pMsg.h.sizeL = SET_NUMBERL( sizeof(PMSG_SEND_NEWS)+textlen);
		memcpy(buffer, &pMsg, sizeof(PMSG_SEND_NEWS));
		memcpy(&buffer[sizeof(PMSG_SEND_NEWS)], m_ServerData.m_News[i].Text, textlen);

		IOCP.DataSend(aIndex, (LPBYTE)buffer, sizeof(PMSG_SEND_NEWS)+textlen);
	}
}

void SCSendAutoUpdateData(int aIndex, PMSG_CLIENTVERSION *aRecv)
{
	unsigned int MainVersion, HeadVersion, SubVersion;

	sscanf_s(g_ClientVersion, "%u.%u.%u", &MainVersion, &HeadVersion, &SubVersion);

	if(aRecv->ClientMainVersion == MainVersion && aRecv->ClientHeadVersion == HeadVersion && aRecv->ClientSubVersion == SubVersion)
	{
		PMSG_VERSIONOK pMsg;
		
		pMsg.h.c = 0xC1;
		pMsg.h.headcode = 0x02;
		pMsg.h.size = sizeof(pMsg);
		pMsg.VersionOK = 1;

		IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
	}

	else
	{
		PMSG_AUTOUPDATE pMsg;

		pMsg.h.c = 0xC1;
		pMsg.h.headcode = 0x05;
		pMsg.h.size = sizeof(pMsg);

		pMsg.MainVersion = MainVersion;
		pMsg.HeadVersion = HeadVersion;
		pMsg.SubVersion = SubVersion;
		pMsg.FTPPort = g_FTPPort;
		memcpy(pMsg.HostURL, g_HostURL, sizeof(pMsg.HostURL));
		memcpy(pMsg.FTPLogin, g_FTPLogin, sizeof(pMsg.FTPLogin));
		memcpy(pMsg.FTPPassword, g_FTPPassword, sizeof(pMsg.FTPPassword));
		memcpy(pMsg.VersionFile, g_VersionFile, sizeof(pMsg.VersionFile));

		unsigned char Keys[3] = {0xFC, 0xCF, 0xAB};

		for(short i = 0; i <= 3;++i)
		{
			switch(i)
			{
				case 0 : for(short j = 0; j < 100; ++j) { pMsg.HostURL[j] ^= Keys[j%3]; } break;
				case 1 : for(short j = 0; j < 20; ++j) { pMsg.FTPLogin[j] ^= Keys[j%3]; } break;
				case 2 : for(short j = 0; j < 20; ++j) { pMsg.FTPPassword[j] ^= Keys[j%3]; } break;
				case 3 : for(short j = 0; j < 20; ++j) { pMsg.VersionFile[j] ^= Keys[j%3]; } break;
			}
		}

		IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
	}
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


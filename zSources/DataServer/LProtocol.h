//////////////////////////////////////////////////////////////////////
// LProtocol.h
//--------------------------------------------//
//-- Connect Server Source					--//
//-- coded by drakelv (c) 2011				--//
//--------------------------------------------//
#ifndef LPROTOCOL_H
#define LPROTOCOL_H

#include "WzUdp.h"

enum 
{
	CS_SERVER_SELECT	= 0x03,
	CS_CLIENT_CONNECT	= 0x06
};

struct PMSG_HELLO
{
	PBMSG_HEAD		h;
	UCHAR			result;
};

struct GS_CONNECT_INFO
{
	PBMSG_HEAD		h;
	UCHAR			SubHead;
	char			IP[16];
	USHORT			Port;
};

#pragma pack (1)
struct PMSG_SERVERSLIST_COUNT
{
	PWMSG_HEAD2		h;
	BYTE			CountH;
	BYTE			CountL;
};

struct PMSG_SERVERLIST_SERVER
{
	WORD			ServerCode;
	WORD			Percent;
};

#pragma pack ()
struct PMSG_SERVERINFO
{
	PBMSG_HEAD h;	// C1:01
	short ServerCode;	// 4
	BYTE Percent;	// 6
	short UserCount;	// 8
	short AccountCount;	// A
	short PCbangCount;	// C
	short MaxUserCount;	// E
};

/*this is how supron want it to look like, [C2][lenHI[lenLO][0xFA][0x01]

0x01 - byte responsible for our internal protocol
00 means adding news

before you will send first news send packet  {0xC1, 0x04, 0xFA, 0x00} - to reset current news windows, it is needed to do not make newses double

00 - reset
01 - added*/

#define MAX_NEWS 10
#pragma pack(push, 1)
struct PMSG_SEND_NEWS
{
	PWMSG_HEAD2 h;
	BYTE Day;
	BYTE Month;
	WORD year;

	COLORREF dateColor;
	COLORREF titleColor;
	COLORREF textColor;
	char title[40];
	WORD textLen;
	//char Text[2048];
};
#pragma pack(pop)

struct PMSG_SEND_TITLE
{
	PBMSG_HEAD2 h;
	char ServerName[12];
};

struct P_ADD_NEWS{
	// normal data
	BYTE day;
	BYTE month;
	WORD year;

	// text color etc
	COLORREF dateColor;
	COLORREF titleColor;
	COLORREF textColor;

	char title[40];
	
	// text lenght
	WORD textLen;
	char Text[2048];
	// rest of packet is text
	// lenght of packet is: header + structure + text

}; extern P_ADD_NEWS gObjNews[MAX_NEWS]; 

struct CONNECT_OBJECT
{
	DWORD lPacketTime;
	int COUNT;
	char ip[16];
};
#pragma pack()

struct ServerList
{
	ServerList()
	{
		this->btServerCode = 0;
		this->wServerPort = 0;
		memset(this->cIp,0,16);
	}
	BYTE btServerCode;
	WORD wServerPort;
	char cIp[16];
}; extern ServerList g_ServList[50];

struct ServerInfo
{
	WORD UserCount;
	WORD MaxUserCount;
	WORD Percent;
	DWORD TickCount;
};

class CConServ
{
public:
	CConServ();
	~CConServ();

	void ConnectResultSend(int aIndex);
	void ServerListSend(int aIndex);
	void GetServerList(int aIndex);
	void GetServerInfo(int aIndex, USHORT ServerId);
	void LoadNewsFile(LPSTR szFile);
	void SendNews(int aIndex);
	void LoadServerList(LPSTR szFile);
	CONNECT_OBJECT cObj[1000];

}; extern CConServ g_ConnectServer;
void LProtocolCore(int aIndex, BYTE hCode, LPBYTE aRecv, int aLen);
void AddServer(PMSG_SERVERINFO * pMsg);
void LSProtocolCore(BYTE hCode, LPBYTE aRecv, int aLen);
extern WzUdp g_CSSocket;
#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


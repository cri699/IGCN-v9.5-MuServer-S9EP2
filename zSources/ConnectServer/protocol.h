//////////////////////////////////////////////////////////////////////
// protocol.h
#ifndef PROTOCOL_H
#define PROTOCOL_H

#include "prodef.h"

enum CS_PACKETS
{
	CS_SERVER_SELECT	= 0x03,
	CS_CLIENT_CONNECT	= 0x06
};

struct PMSG_CONNECT_INFO
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
	WORD			wServerCode;
	BYTE			btPercent;
	BYTE			btPlayType;
};

struct PMSG_SERVER_SELECT
{
	PBMSG_HEAD2		h;
	WORD			ServerCode;
};

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

#pragma pack ()

struct PMSG_RESULT
{
	PBMSG_HEAD		h;
	UCHAR			result;
};

struct PMSG_SERVERINFO
{
	PBMSG_HEAD h;	// C1:01
	short ServerCode;	// 4
	BYTE Percent;	// 6
	BYTE PlayType;
	short UserCount;	// 8
	short AccountCount;	// A
	short MaxUserCount;	// E
};

#pragma pack (1)
struct PMSG_AUTOUPDATE
{
	PBMSG_HEAD h;
	BYTE MainVersion;
	BYTE HeadVersion;
	BYTE SubVersion;
	char HostURL[100];
	WORD FTPPort;
	char FTPLogin[20];
	char FTPPassword[20];
	char VersionFile[20];
};
#pragma pack ()

struct PMSG_CLIENTVERSION
{
	PBMSG_HEAD h;
	char ClientMainVersion;
	char ClientHeadVersion;
	char ClientSubVersion;
};

struct PMSG_VERSIONOK
{
	PBMSG_HEAD h;
	char VersionOK;
};

struct PMSG_LAUNCHER
{
	PWMSG_HEAD h;
	char FTP[200];
	char HASH[200];
};

struct PMSG_LAUNCHERNOLICENSE
{
	PBMSG_HEAD h;
};

struct PMSG_LAUNCHERERROR
{
	PBMSG_HEAD h;
	BYTE ERRORCODE;
};

void CSProtocolCore(BYTE protoNum, BYTE *aRecv, int aLen, int aIndex, bool Encrypt, int serial);
void UDPProtocolCore(BYTE hCode, LPBYTE aRecv, int aLen);
void UDPSetServerInfo(PMSG_SERVERINFO * aRecv);
void SCSendServerList(int aIndex);
void SCSendServerInfo(int aIndex, PMSG_SERVER_SELECT * aRecv);
void SCConnectResultSend(int aIndex, BYTE btResult);
void SCSendNews(int aIndex);
void SCSendAutoUpdateData(int aIndex, PMSG_CLIENTVERSION * aRecv);
void UDPInit();

#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


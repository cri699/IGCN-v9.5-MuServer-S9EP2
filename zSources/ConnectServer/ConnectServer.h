//////////////////////////////////////////////////////////////////////
// ConnectServer.h
#pragma once

#include "resource.h"
#include "WzUdp.h"
#include "wsJoinServerCli.h"
#include "IOCP.h"

extern HWND hWnd;
extern CIOCP IOCP;
extern WORD g_ConnectServerPort;
extern WORD g_ConnectServerUDP;
extern DWORD g_MaxConnectionsPerIP;
extern DWORD g_MaxPacketPerSec;
extern WzUdp g_UDPPort;
extern WORD g_FTPPort;
extern char g_HostURL[100];
extern char g_FTPLogin[20];
extern char g_FTPPassword[20];
extern char g_VersionFile[20];
extern char g_ClientVersion[9];
extern char FTPLauncher[200];
extern char HashLauncher[200];
extern char g_WhiteListIP[16];
#define	WM_LOG_PAINT	(0x101)
#define WM_LOG_DATE_CHANGE	(0x102)
#define WM_SERVER_CHECK		(0x103)

#define SET_NUMBERH(x) ( (BYTE)((DWORD)(x)>>(DWORD)8) )
#define SET_NUMBERL(x) ( (BYTE)((DWORD)(x) & 0xFF) )
#define SET_NUMBERHW(x) ( (WORD)((DWORD)(x)>>(DWORD)16) )
#define SET_NUMBERLW(x) ( (WORD)((DWORD)(x) & 0xFFFF) )


//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


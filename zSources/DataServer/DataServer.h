//////////////////////////////////////////////////////////////////////
// DataServer.h

#if !defined(AFX_EDATASERVER_H__A7C14824_5DD7_49C9_BDFB_C8D813D08CB9__INCLUDED_)
#define AFX_EDATASERVER_H__A7C14824_5DD7_49C9_BDFB_C8D813D08CB9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"
#include "DSprotocol.h"
#include "Sprotocol.h"
#include "EDSprotocol.h"
//#include "Fprotocol.h"

#define SET_NUMBERH(x) ( (BYTE)((DWORD)(x)>>(DWORD)8) )
#define SET_NUMBERL(x) ( (BYTE)((DWORD)(x) & 0xFF) )
#define SET_NUMBERHW(x) ( (WORD)((DWORD)(x)>>(DWORD)16) )
#define SET_NUMBERLW(x) ( (WORD)((DWORD)(x) & 0xFFFF) )

#define	WM_LOG_PAINT	(0x101)
#define WM_LOG_DATE_CHANGE	(0x102)
#define WM_FIRST_PROC 1000

extern HWND ghWnd;
extern int g_dwMaxServerGroups;
extern WORD g_FSGateServerListPort;
extern WORD g_JoinServerListPort;
extern WORD g_DataServerListPort;
extern WORD g_ExDataServerListPort;
extern TCHAR g_MuOnlineDNS[64];
extern TCHAR g_MeMuOnlineDNS[64];
extern TCHAR g_EventServerDNS[64];
extern TCHAR g_RankingServerDNS[64];
extern TCHAR g_UserID[64];
extern TCHAR g_Password[64];
extern BOOL g_PwEncrypt;
extern BOOL g_DSMode;
extern BOOL g_UseJoinServer;
extern BOOL g_UseDataServer;
extern BOOL g_UseExDataServer;
extern TCHAR g_ServerName[64];
extern DWORD g_GensRankingUpdateTime;
extern DWORD g_GensLeaveAfterDays;
extern DWORD g_MachineIDConnectionLimitPerGroup;
bool IniteDataServer();
void eDataServerClose();
extern CDataServerProtocol m_DSProtocol;
extern CLoginServerProtocol m_JSProtocol;
extern CExDataServerProtocol m_EXDSProtocol;
extern TCHAR szWANIP[150];
extern HANDLE hQueue1, hQueue2, hQueue3, hQueue4, hQueue5, hJSQueue, hEXDSQueue;
extern WORD g_MagumsaCreateMinLevel;
extern WORD g_DarkLordCreateMinLevel;
extern WORD g_GrowLancerCreateMinLevel;
extern int g_iShowAllQueriesInDS;
extern int g_iConnectStatSyncEnable;
BOOL CALLBACK JSDisconnect (HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

#endif // !defined(AFX_EDATASERVER_H__A7C14824_5DD7_49C9_BDFB_C8D813D08CB9__INCLUDED_)


//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


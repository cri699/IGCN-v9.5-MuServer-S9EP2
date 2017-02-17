// GameServer.h
// ------------------------------
// Decompiled by Deathway
// Date : 2007-03-09
// ------------------------------
#ifndef	GAMESERVER_H

#define	GAMESERVER_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "RingAttackEvent.h"
#include "XMasAttackEvent.h"

//***************************************************
// GLOBAL DEFINE SETTIN
//***************************************************

#define LOG_INMEDIATLY	0

// GameServer Use M:\\ Drive?
// 1 : GS use M:\ Drive		   
// 0 : GS use ..\data\ path
//---------------------------------------------
#define USE_M_DRIVE 0			

// GameServer Version
//--------------------------

#define GAMESERVER_VERSION "9.5.1.15"
#define DLL_VERSION "9.5.1.15"
// Protocol Trace
//--------------------------
#define TRACE_PROTOCOL	0

//***************************************************

#define WM_GM_SERVER_MSG_PROC			0x401
#define WM_GM_CLIENT_MSG_PROC			0x402
#define WM_GM_JOIN_CLIENT_MSG_PROC		0x403
#define WM_START_SERVER					0x405
#define WM_GM_DATA_CLIENT_MSG_PROC		0x40A
#define WM_GM_EXDATA_CLIENT_MSG_PROC	0x40E
#define WM_GM_CONNECT_SERVER_MSG		0x40F
#define WM_GM_AUTH_SERVER_MSG_PROC		0x410

// TIMER Messages
#define	WM_LOG_PAINT					0x64
#define WM_FIRST_MSG_PROCESS			0x65
#define WM_AI_MONSTER_PROC				0x1F4
#define WM_SECOND_MSG_PROCESS			0x3E8
#define WM_SET_DATE						0x3E9
#define WM_GS_CLOSE						0x3EA
#define WM_LOG_DATE_CHANGE				0x3EB
#define WM_EVENT_RUN_PROC				0x3EC
#define WM_MOVE_MONSTER_PROC			0x3ED
#define WM_MONSTER_AND_MSG_PROC			0x3EE
#define WM_CONNECT_DATASERVER			0x3EF
#define WM_GUILD_LIST_REQUEST			0x3F0
#define WM_AI_MONSTER_MOVE_PROC			0x3F2
#define WM_MATH_AUTHEN					0x3F3
#define WM_VIEWPORT_PROCESS				0x3F4
#define WM_BATTLECORE_PROCESS			0x3F6
#define WM_EVENTSETBASE_PROCESS			0x3F7

//*********** DO NOT CHANGE THIS *********************
#ifndef USE_M_DRIVE
#error "You must define USE_M_DRIVE"
#else
#if (USE_M_DRIVE == 1)
#define FINAL_PATH "M:\\"
#define COMMONSERVER_PATH "M:\\commonserver.cfg"
#else
#define FINAL_PATH "..\\data\\"
#define COMMONSERVER_PATH "..\\data\\commonserver.cfg"
#endif
#endif

extern int gServerReady;
extern int gCloseMsg;
extern int gCloseMsgTime;
extern int gCurPaintType;
extern int gCurPaintMapNumber;
extern CRingAttackEvent g_RingAttackEvent;
extern CXMasAttackEvent g_XMasAttackEvent;

extern HINSTANCE hInst; // Current Instance
extern HWND ghWnd, ghInfoBar;
extern HANDLE ghConnServThread;

extern char szTitle[100];
extern char szWindowClass[64];

#define MAX_LENGTH_SN 100

LRESULT CALLBACK LicenseInfo(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

BOOL AllServerStart(void); // Start all Servers

#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


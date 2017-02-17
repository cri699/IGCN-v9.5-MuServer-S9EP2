// DataServer.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "resource.h"
#include "DataServer.h"
#include "ServerEngine.h"
#include "DSprotocol.h"
#include "Sprotocol.h"
#include "TLog.h"
#include "Giocp.h"
#include "TServerInfoDisplayer.h"
#include "EDSprotocol.h"
#include "winutil.h"
#include "MapServerManager.h"
#include "ItemSerial.h"

#define MAX_LOADSTRING 100
// Global Variables:
HINSTANCE hInst;								// current instance
HWND ghWnd;
HWND ghGSListView;
HWND ghPlayerWnd;
HWND ghPlayerListView;
HWND ghQueueBar;
TCHAR szWANIP[150];
TCHAR szTitle[MAX_LOADSTRING];								// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];								// The title bar text
int g_dwMaxServerGroups=GetPrivateProfileInt("SETTINGS", "MAX_SERVER", 10, ".\\DataServer.ini") * MAX_SERVER_TYPE;	
WORD g_JoinServerListPort = GetPrivateProfileInt("SETTINGS", "JoinServerPort", 55970, ".\\DataServer.ini");
WORD g_DataServerListPort = GetPrivateProfileInt("SETTINGS", "DataServerPort", 55960, ".\\DataServer.ini");
WORD g_ExDataServerListPort = GetPrivateProfileInt("SETTINGS", "ExDataServerPort", 55906, ".\\DataServer.ini");
BOOL g_PwEncrypt = GetPrivateProfileInt("SQL", "PasswordEncryptType", 1, ".\\DataServer.ini");
BOOL g_DSMode = GetPrivateProfileInt("SETTINGS", "DataServerOnlyMode", 0, ".\\DataServer.ini");
BOOL g_UseJoinServer = GetPrivateProfileInt("SETTINGS", "UseJoinServer", 1, ".\\DataServer.ini");
BOOL g_UseDataServer = GetPrivateProfileInt("SETTINGS", "UseDataServer", 1, ".\\DataServer.ini");
BOOL g_UseExDataServer = GetPrivateProfileInt("SETTINGS", "UseExDataServer", 1, ".\\DataServer.ini");
DWORD g_GensRankingUpdateTime = 2;
DWORD g_GensLeaveAfterDays = GetPrivateProfileInt("GensSystem", "GensReJoinDaysLimit", 7, ".\\DataServer.ini");
DWORD g_MachineIDConnectionLimitPerGroup = GetPrivateProfileInt("SETTINGS", "MachineIDConnectionLimitPerGroup", 3, ".\\DataServer.ini");
WORD g_MagumsaCreateMinLevel = GetPrivateProfileInt("SETTINGS", "MagicGladiatorCreateMinLevel", 220, ".\\DataServer.ini");
WORD g_DarkLordCreateMinLevel = GetPrivateProfileInt("SETTINGS", "DarkLordCreateMinLevel", 250, ".\\DataServer.ini");
WORD g_GrowLancerCreateMinLevel = GetPrivateProfileInt("SETTINGS", "GrowLancerCreateMinLevel", 200, ".\\DataServer.ini");
int g_iShowAllQueriesInDS  = GetPrivateProfileInt("SETTINGS", "DisplayAllQueries", 1, ".\\DataServer.ini");
int g_iConnectStatSyncEnable =  GetPrivateProfileInt("SETTINGS", "MembStatSync", 0, ".\\DataServer.ini");;

TCHAR g_MuOnlineDNS[64];
TCHAR g_MeMuOnlineDNS[64];
TCHAR g_EventServerDNS[64];
TCHAR g_RankingServerDNS[64];
TCHAR g_UserID[64];
TCHAR g_Password[64];
TCHAR g_ServerName[64];
TCHAR g_AWHostPass[32];
TCHAR g_MapSvrFilePath[96];

CDataServerProtocol m_DSProtocol;
CLoginServerProtocol m_JSProtocol;
CExDataServerProtocol m_EXDSProtocol;

HANDLE hQueue1, hQueue2, hQueue3, hQueue4, hQueue5, hJSQueue, hEXDSQueue;

// Foward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	ManagePlayer(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK AuthWindow(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_EDATASERVER, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	InitCommonControls();
	_set_printf_count_output(TRUE);
	CreateDirectory("LOG", NULL);
	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_EDATASERVER);

	HWND QueueBar = CreateWindowExA(0,STATUSCLASSNAME,NULL, WS_CHILD | WS_VISIBLE, 0, 0, 0, 0,ghWnd,(HMENU)130,hInstance,NULL);
	ghQueueBar = QueueBar;

	int iQueueBarWidths[] = {130, 260, 390, 520, 650, 780, 910, 1040, 1170, 1300, 1430, 1560, -1};
	SendMessage(QueueBar, SB_SETPARTS, 8, (LPARAM)iQueueBarWidths);
	
	g_Log.LogInit(true, 256);
	g_Log.AddC(TColor::Green, "Initializing...");
	g_Log.AddC(TColor::Orange, "DataServer(e) version: %s Compiled at: %s %s", DATASERVER_VERSION, __DATE__, __TIME__);
	g_Log.LogDateChange();
	//GetPrivateProfileString(
	LoadAllowableIpList("./IGC_AllowedIPList.xml");
	GetPrivateProfileString("SETTINGS", "MapServerInfoPath", "..\\IGCData\\IGC_MapServerInfo.xml", g_MapSvrFilePath, sizeof(g_MapSvrFilePath), ".\\DataServer.ini");
	GetPrivateProfileString("SETTINGS","WanIP","127.0.0.1", szWANIP, 150, ".\\DataServer.ini");
	memcpy(szWANIP,ValidateAndResolveIP(szWANIP),15);
	g_MapServerManager.LoadMapData(g_MapSvrFilePath);
	SendMessage(ghWnd, WM_TIMER, WM_LOG_PAINT, NULL);

	gObjServerInit();
	IniteDataServer();
	IocpServerStart();

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return msg.wParam;
}

bool IniteDataServer()
{
	SetTimer(ghWnd, WM_LOG_PAINT, 1000, NULL);
	SetTimer(ghWnd, WM_LOG_DATE_CHANGE, 300, NULL);
	SetTimer(ghWnd, WM_FIRST_PROC, 1000, NULL);
	GetPrivateProfileString("SQL", "MuOnlineDB", "MuOnline", g_MuOnlineDNS, sizeof(g_MuOnlineDNS), ".\\DataServer.ini");
	GetPrivateProfileString("SQL", "MeMuOnlineDB", "MuOnline", g_MeMuOnlineDNS, sizeof(g_MeMuOnlineDNS), ".\\DataServer.ini");
	GetPrivateProfileString("SQL", "EventDB", "MuEvent", g_EventServerDNS, sizeof(g_EventServerDNS), ".\\DataServer.ini");
	GetPrivateProfileString("SQL", "RankingDB", "MuRanking", g_RankingServerDNS, sizeof(g_RankingServerDNS), ".\\DataServer.ini");
	GetPrivateProfileString("SQL", "User", "sa", g_UserID, sizeof(g_UserID), ".\\DataServer.ini");
	GetPrivateProfileString("SQL", "Pass", "darknes", g_Password, sizeof(g_Password), ".\\DataServer.ini");
	GetPrivateProfileString("SQL", "SQLServerName", "(local)", g_ServerName, sizeof(g_ServerName), ".\\DataServer.ini");

	m_DSProtocol.Init();
	m_JSProtocol.Init();
	m_EXDSProtocol.Init();
	g_ItemSerial.Init();

	char szText[400];

	if (g_DSMode == TRUE)
	{
		wsprintf(szText, "[%s] IGC.DataServer [DS: %d][MD5: %d][MainDB: %s][AccountDB: %s][EventsDB: %s][RankingDB: %s] [Season 9]", DATASERVER_VERSION, g_DataServerListPort, g_PwEncrypt, g_MuOnlineDNS, g_MeMuOnlineDNS, g_EventServerDNS, g_RankingServerDNS);
	}
	else if (!g_UseDataServer && !g_UseExDataServer && g_UseJoinServer == TRUE && !g_DSMode)
	{
		wsprintf(szText, "[%s] IGC.JoinServer [JS: %d][MD5: %d][AccountDB: %s][Season 9]", DATASERVER_VERSION, g_JoinServerListPort, g_PwEncrypt, g_MeMuOnlineDNS);
	}
	else if (!g_UseDataServer && g_UseExDataServer == TRUE && !g_UseJoinServer && !g_DSMode)
	{
		wsprintf(szText, "[%s] IGC.ExDataServer [ExDS: %d][MD5: %d][MainDB: %s][AccountDB: %s][EventsDB: %s][RankingDB: %s] [Season 9]", DATASERVER_VERSION, g_ExDataServerListPort, g_PwEncrypt, g_MuOnlineDNS, g_MeMuOnlineDNS, g_EventServerDNS, g_RankingServerDNS);
	}
	else
	{
		wsprintf(szText, "[%s] IGC.DataServer(e) [DS: %d][ExDS: %d][JS: %d][MD5: %d][MainDB: %s][AccountDB: %s][EventsDB: %s][RankingDB: %s] [Season 9]", DATASERVER_VERSION, g_DataServerListPort, g_ExDataServerListPort, g_JoinServerListPort, g_PwEncrypt, g_MuOnlineDNS, g_MeMuOnlineDNS, g_EventServerDNS, g_RankingServerDNS);
	}

	SetWindowText(ghWnd, szText);

	g_Log.AddC(TColor::Yellow, "Current Global Connections Limit (HWiD) per ServerGroup: %d", g_MachineIDConnectionLimitPerGroup);

	return true;

}

void eDataServerClose()
{

	DestroyGIocp();

	if ( hQueue1 != NULL )
	{
		TerminateThread(hQueue1, 0);
	}

	if ( hQueue2 != NULL )
	{
		TerminateThread(hQueue2, 0);
	}

	if ( hQueue3 != NULL )
	{
		TerminateThread(hQueue3, 0);
	}

	if ( hQueue4 != NULL )
	{
		TerminateThread(hQueue4, 0);
	}

	if (hQueue5 != NULL)
	{
		TerminateThread(hQueue5, 0);
	}

	if ( hEXDSQueue != NULL )
	{
		TerminateThread(hEXDSQueue, 0);
	}

	if ( hJSQueue != NULL )
	{
		TerminateThread(hJSQueue, 0);
	}

}

void UpdateInfoBar(HWND hWnd)
{
	HDC hDC;
	hDC = GetDC(hWnd);

	char szTempText[85];

	wsprintf(szTempText, "DS Queue (1): %d", m_DSQueue1.GetCount_NoLock());
	SendMessage(ghQueueBar, SB_SETTEXT, 0, (LPARAM)szTempText);

	wsprintf(szTempText, "DS Queue (2): %d", m_DSQueue2.GetCount_NoLock());
	SendMessage(ghQueueBar, SB_SETTEXT, 1, (LPARAM)szTempText);

	wsprintf(szTempText, "DS Queue (3): %d", m_DSQueue3.GetCount_NoLock());
	SendMessage(ghQueueBar, SB_SETTEXT, 2, (LPARAM)szTempText);

	wsprintf(szTempText, "DS Queue (4): %d", m_DSQueue4.GetCount_NoLock());
	SendMessage(ghQueueBar, SB_SETTEXT, 3, (LPARAM)szTempText);

	wsprintf(szTempText, "DS Queue (5): %d", m_DSQueue5.GetCount_NoLock());
	SendMessage(ghQueueBar, SB_SETTEXT, 4, (LPARAM)szTempText);

	wsprintf(szTempText, "JS Queue: %d", m_JSQueue.GetCount_NoLock());
	SendMessage(ghQueueBar, SB_SETTEXT, 5, (LPARAM)szTempText);

	wsprintf(szTempText, "EXDB Queue: %d", m_EXDSQueue.GetCount_NoLock());
	SendMessage(ghQueueBar, SB_SETTEXT, 6, (LPARAM)szTempText);

	wsprintf(szTempText, "Item Count: %I64d", g_ItemSerial.GetSerialCount());
	SendMessage(ghQueueBar, SB_SETTEXT, 7, (LPARAM)szTempText);

	ReleaseDC(hWnd, hDC);

}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage is only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_EDATASERVER);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= (LPCSTR)IDC_EDATASERVER;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HANDLE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ghWnd = hWnd;

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;

	switch (message) 
	{
		case WM_SIZE:
			SendMessage(ghQueueBar, WM_SIZE, 0, 0);
			break;
		case WM_COMMAND:
			wmId    = LOWORD(wParam); 
			wmEvent = HIWORD(wParam); 
			// Parse the menu selections:
			switch (wmId)
			{
				case IDM_ABOUT:
				   DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
				   break;
				case ID_JSDC_MENU:
					if (g_UseJoinServer == TRUE && g_DSMode == FALSE)
					{
						DialogBox(hInst, MAKEINTRESOURCE(IDD_JSDC_DIALOG), hWnd, (DLGPROC)JSDisconnect);
					}
					else
					{
						g_Log.AddC(TColor::Red, "Option available only with enabled Join Server");
						return false;
					}
					break;
				case ID_OPTIONS_RESETERROR:
					g_ServerInfoDisplayer.ResetSQLError();
					g_Log.AddC(TColor::Yellow, "Error Status Display cleared");
					break;
				case ID_GENS_REFRESHRANKING:
					m_EXDSProtocol.GensManualRefreshRanking(0);
					break;
				case ID_GENS_RELOADLEAVEDELAY:
					 g_GensLeaveAfterDays = GetPrivateProfileInt("GensSystem", "GensReJoinDaysLimit", 7, ".\\DataServer.ini");
					 g_Log.AddC(TColor::Yellow, "Gens rejoin day limit  reloaded");
					 break;
				//case ID_OPTIONS_PLAYERLIST:
					//ghPlayerWnd = CreateDialog(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, ManagePlayer);
					//ShowWindow(ghPlayerWnd, SW_SHOW);
				//	break;
				case ID_OPTIONS_RELOADHWIDLIMIT:
					g_MachineIDConnectionLimitPerGroup = GetPrivateProfileInt("SETTINGS", "MachineIDConnectionLimitPerGroup", 3, ".\\DataServer.ini");
					g_Log.AddC(TColor::Yellow, "Current Global Connections Limit (HWiD) per ServerGroup: %d", g_MachineIDConnectionLimitPerGroup);
					break;
				case IDM_EXIT:
				   DestroyWindow(hWnd);
				   break;
				default:
				   return DefWindowProc(hWnd, message, wParam, lParam);
			}
			break;
		case WM_TIMER:
			switch ( wParam )
			{
				case WM_LOG_PAINT:
					{
						RECT rect;
						HDC hdc = GetDC(hWnd);
						GetClientRect(hWnd, &rect);
						FillRect(hdc, &rect, (HBRUSH)GetStockObject(0));
						ReleaseDC(hWnd, hdc);

						g_Log.LogTextPaint(hWnd);

						g_ServerInfoDisplayer.Run(hWnd);
						UpdateInfoBar(ghQueueBar);
					}

					break;
				case WM_LOG_DATE_CHANGE:
					g_Log.LogDateChange();
					break;
				case WM_FIRST_PROC:
					m_EXDSProtocol.GensRankingProcess();
					m_JSProtocol.CheckVIPTimeProc();
					break;
			}
			break;
		case WM_DESTROY:
			eDataServerClose();
			PostQuitMessage(0);
			break;
		case WM_CLOSE:
			if ( MessageBox(NULL, "Are you sure to Close?", "Confirmation", MB_YESNO|MB_ICONQUESTION) == IDYES )
				DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}

// Mesage handler for about box.
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_INITDIALOG:
				return TRUE;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
			{
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			break;
	}
    return FALSE;
}

// Handler for disconnect box.
BOOL CALLBACK JSDisconnect (HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch(iMessage)
	{
		case WM_COMMAND:
		{
			switch (LOWORD(wParam)) 
			{
			case IDC_JSDC_BUTTON:
				{
					if ( g_UseJoinServer == FALSE )
					{
						g_Log.AddC(TColor::Red, "[JoinServer] - not used");
						EndDialog(hWnd, 0);
						break;
					}

					char szAccount[11];
					int sLen = GetDlgItemText(hWnd,IDC_JSDC_ACCOUNT,szAccount,sizeof(szAccount));
					szAccount[10] = 0;

					if(sLen <= 0 || sLen > sizeof(szAccount))
					{
						g_Log.AddC(TColor::Red,"[JoinServer] could not read account name");
					}
					else
					{
						if ( m_JSProtocol.DisconnectPlayer(szAccount) == TRUE )
						{
							g_Log.AddC(TColor::Blue,"[JoinServer] Account(%s) disconnected",szAccount);
						}

						else
						{
							g_Log.AddC(TColor::Red,"[JoinServer] Account(%s) not found online",szAccount);
						}
					}
				}
				EndDialog(hWnd, 0);
				break;
			}
		}
		return true;
		case WM_CLOSE:
		{
			EndDialog(hWnd, 0);
			return true;
		}
	}
return false;
}

LRESULT CALLBACK ManagePlayer(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {

    case WM_INITDIALOG:
			INITCOMMONCONTROLSEX ictrlex;
			ictrlex.dwSize       = sizeof(INITCOMMONCONTROLSEX);
			ictrlex.dwICC        = ICC_TREEVIEW_CLASSES ;
			InitCommonControlsEx(&ictrlex);

			RECT rcl;
			GetClientRect(hDlg, &rcl);
			ghPlayerListView = CreateWindowEx(0, WC_LISTVIEW, NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL | LVS_REPORT | LVS_SORTASCENDING, 
				10, 5, rcl.right - rcl.left - 20, rcl.bottom - rcl.top - 50, hDlg, NULL, hInst, NULL);

			ListView_SetExtendedListViewStyle(ghPlayerListView, LVS_EX_FULLROWSELECT);

			LVCOLUMN lvc;
			lvc.mask = LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

			lvc.iSubItem = 0;
			lvc.cx = 100;
			lvc.pszText = "Account ID";
			ListView_InsertColumn(ghPlayerListView, 0, &lvc);

			lvc.iSubItem = 1;
			lvc.cx = 100;
			lvc.pszText = "IP";
			ListView_InsertColumn(ghPlayerListView, 1, &lvc);

			lvc.iSubItem = 2;
			lvc.cx = 200;
			lvc.pszText = "Server";
			ListView_InsertColumn(ghPlayerListView, 2, &lvc);

        return TRUE;

    case WM_COMMAND:

        if (LOWORD(wParam) == IDOK)
        { 
            EndDialog(hDlg, LOWORD(wParam));
            return TRUE;
		}

        break;

    }
    return FALSE;
}



//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


// ConnectServer.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "ConnectServer.h"
#include "IOCP.h"
#include "TLog.h"
#include "Engine.h"
#include "WzUdp.h"
#include "protocol.h"
#include "ServerData.h"
#include "wsJoinServerCli.h"
#include "mdump.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
HWND hWnd;										// handle window
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
WORD g_ConnectServerPort;
WORD g_ConnectServerUDP;
DWORD g_MaxConnectionsPerIP;
DWORD g_MaxPacketPerSec;
WzUdp g_UDPPort;
WORD g_FTPPort;
char g_HostURL[100];
char g_FTPLogin[20];
char g_FTPPassword[20];
char g_VersionFile[20];
char g_ClientVersion[9];
char g_WhiteListIP[16];
CIOCP IOCP;

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	AuthBox(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_IGCCS, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	MiniDumper::MiniDumper("IGC.ConnectServer");

	CreateDirectory("LOG", NULL);
	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_IGCCS));

	g_Log.LogInit(TRUE, 256);
	g_Log.AddC(TColor::Red, "Initializing...");
	UserInit();
	IOCP.GiocpInit();

	g_Log.Add("Connect Server version: %s Compiled at: %s %s", CONNECTSERVER_VERSION, __DATE__, __TIME__);

	SetTimer(hWnd, WM_LOG_PAINT, 1000, NULL);
	SetTimer(hWnd, WM_LOG_DATE_CHANGE, 300, NULL);
	SetTimer(hWnd, WM_SERVER_CHECK, 1000, NULL);

	g_ConnectServerPort = GetPrivateProfileInt("Config", "TCP_PORT", 44405, ".\\ConnectServer.ini");
	g_ConnectServerUDP = GetPrivateProfileInt("Config", "UDP_PORT", 55667, ".\\ConnectServer.ini");
	g_MaxConnectionsPerIP = GetPrivateProfileInt("Config", "MaxConnectionsPerIP", 5, ".\\ConnectServer.ini");
	g_MaxPacketPerSec = GetPrivateProfileInt("Config", "MaxPacketsPerSecond", 50, ".\\ConnectServer.ini");
	GetPrivateProfileString("Config", "LauncherProxyWhiteListIP", "127.0.0.1", g_WhiteListIP, 16, ".\\ConnectServer.ini");
	GetPrivateProfileString("AutoUpdate", "Version", "1.05.91", g_ClientVersion, 9, ".\\ConnectServer.ini");
	GetPrivateProfileString("AutoUpdate", "VersionFile", "version.dat", g_VersionFile, 20, ".\\ConnectServer.ini");
	GetPrivateProfileString("AutoUpdate", "HostURL", "192.168.1.2", g_HostURL, 100, ".\\ConnectServer.ini");
	GetPrivateProfileString("AutoUpdate", "FTPLogin", "muonline", g_FTPLogin, 20, ".\\ConnectServer.ini");
	GetPrivateProfileString("AutoUpdate", "FTPPasswd", "muonline", g_FTPPassword, 20, ".\\ConnectServer.ini");

	g_FTPPort = GetPrivateProfileInt("AutoUpdate", "FTPPort", 21, ".\\ConnectServer.ini");
	//g_Log.Add("Current Auto Update Version: %s", g_ClientVersion);

	m_ServerData.LoadServerFile("IGC_ServerList.xml");
	m_ServerData.LoadNewsFile("News.dat");

	char szText[100];
	wsprintf(szText, "[%s] IGC.ConnectServer [TCP-Port: %d][UDP-Port: %d][TCP-Auth Port: %d]", CONNECTSERVER_VERSION, g_ConnectServerPort, g_ConnectServerUDP);
	SetWindowText(hWnd, szText);

	IOCP.CreateGIocp(g_ConnectServerPort);
	UDPInit();

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		} 
	}

	return (int) msg.wParam;
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
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
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_IGCCS));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_IGCCS);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
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

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ::hWnd = hWnd;

   ShowWindow(hWnd, SW_NORMAL);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
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
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case ID_RELOAD_SERVERS:
			m_ServerData.LoadServerFile("IGC_ServerList.xml");
			break;
		case ID_RELOAD_NEWS:
			m_ServerData.LoadNewsFile("News.dat");
			g_Log.Add("%s file reloaded", "News.dat");
			break;
		case ID_RELOAD_AUTOUPDATE:
			GetPrivateProfileStringA("AutoUpdate", "Version", "1.05.91", g_ClientVersion, 9, ".\\ConnectServer.ini");
			GetPrivateProfileStringA("AutoUpdate", "VersionFile", "version.dat", g_VersionFile, 20, ".\\ConnectServer.ini");
			GetPrivateProfileStringA("AutoUpdate", "HostURL", "192.168.1.2", g_HostURL, 100, ".\\ConnectServer.ini");
			GetPrivateProfileStringA("AutoUpdate", "FTPLogin", "muonline", g_FTPLogin, 20, ".\\ConnectServer.ini");
			GetPrivateProfileStringA("AutoUpdate", "FTPPasswd", "muonline", g_FTPPassword, 20, ".\\ConnectServer.ini");
			g_FTPPort = GetPrivateProfileIntA("AutoUpdate", "FTPPort", 21, ".\\ConnectServer.ini");
			//g_Log.Add("Current Auto Update Version: %s", g_ClientVersion);
			break;
		case ID_RELOAD_ANTIFLOODOPTIONS:
				g_MaxConnectionsPerIP = GetPrivateProfileInt("Config", "MaxConnectionsPerIP", 5, ".\\ConnectServer.ini");
				g_Log.Add("Max Connections Per IP - %d", g_MaxConnectionsPerIP);
				g_MaxPacketPerSec = GetPrivateProfileInt("Config", "MaxPacketsPerSecond", 50, ".\\ConnectServer.ini");
				g_Log.Add("Max Packet Per Second - %d", g_MaxPacketPerSec);
			break;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
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
				}
				break;
			case WM_LOG_DATE_CHANGE:
				g_Log.LogDateChange();
				break;

			case WM_SERVER_CHECK:
				m_ServerData.Run();
				UserRun();
				break;
		}
		break;
	case WM_DESTROY:
		IOCP.GiocpDelete();
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

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}



//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


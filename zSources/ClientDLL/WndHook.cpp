//////////////////////////////////////////////////////////////////////
// WndHook.cpp
#include "stdafx.h"
#include "WndHook.h"
#include "Camera.h"
#include "offsets.h"
#include "HookManager.h"
#include "ReconnectSystem.h"

bool bMButton = false;
int iX = 0, iY = 0;

int g_UsedResolution;
WNDPROC muWindowProc = (WNDPROC)MU_WND_PROC;
HWND MuWindow;
bool gLauncherProxyEnabled;
bool gLauncherProxyReconnectInitiated;
HWND LauncherWindowHandle;

#pragma optimize("g", on)
LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	MuWindow = hwnd;
	switch (uMsg)
	{
		case WM_CLOSE:
			ExitProcess(0);
			//if (g_UserQuit == 0)
			//{
			//	if (MessageBox(hwnd, "Are you sure you want to Exit the game?", "MuOnline Client", MB_OKCANCEL) == IDOK)
			//		ExitProcess(0);
			//	else
			//		return 0;
			//}
			break;
		case WM_LBUTTONDOWN:
		{
			if (g_ReconnectProcess > 0 && g_ReconnectProcess < 6){ // prevent walk while reconnecting
				return true;
			}

			//	iX = GET_X_LPARAM(lParam);
			//iY = GET_Y_LPARAM(lParam);
		//	g_NewsSystem->CheckLinkClick();
			//	if (g_Gui->LButtonDown(iX, iY))
			//	return true;
		}
		break;
		case 0x0500:
			{
				gLauncherProxyEnabled = true;
				LauncherWindowHandle = (HWND)wParam;
			}
			break;
		case 0x0501:
			{
				gLauncherProxyReconnectInitiated = true;
			}
			break;
		//case WM_KEYDOWN:
			//if (VK_F8)
				//g_Camera->Reset();
			// F8Press();
			//break;
		case WM_MBUTTONDOWN:
			bMButton = true;
			break;
		case WM_MBUTTONUP:
			bMButton = false;
			break;
		case WM_MOUSEMOVE:
			static int LastX, LastY;
			static int MoveX, MoveY;

			iX = GET_X_LPARAM(lParam);
			iY = GET_Y_LPARAM(lParam);

			MoveX = iX - LastX;
			MoveY = iY - LastY;

			LastX = iX;
			LastY = iY;
			
	//		g_Gui->OnMouseMove(iX, iY, MoveX, MoveY);

			//g_CustomMenu->CheckMousePos();

#ifdef USE_CAMERA == 1
			if (bMButton){
				g_Camera->MouseMove((float)MoveX, (float)MoveY);
				//closesocket(*g_MuSocket);
			}
#endif
			break;
/*		case WM_LBUTTONDOWN:
			iX = GET_X_LPARAM(lParam);
			iY = GET_Y_LPARAM(lParam);
			
			g_NewsSystem->CheckLinkClick();

			if (g_Gui->LButtonDown(iX, iY))
				return true;
			break;
		case WM_LBUTTONUP:
			iX = GET_X_LPARAM(lParam);
			iY = GET_Y_LPARAM(lParam);

			if (g_Gui->LButtonUp(iX, iY))
				return true;
			break;
		case WM_LBUTTONDBLCLK:
			iX = GET_X_LPARAM(lParam);
			iY = GET_Y_LPARAM(lParam);

			if (g_Gui->LButtonDowni(iX, iY))
				return true;
			break;
		case WM_KEYDOWN:
			if (VK_F8)
				g_Camera->Reset();
				//F8Press();
			break;
		case WM_MBUTTONDOWN:
			bMButton = true;
			break;
		case WM_MBUTTONUP:
			bMButton = false;
			break;
		case WM_MOUSEMOVE:
			static int LastX, LastY;
			static int MoveX, MoveY;

			iX = GET_X_LPARAM(lParam);
			iY = GET_Y_LPARAM(lParam);

				MoveX = iX-LastX;
				MoveY = iY-LastY;

				LastX = iX;
				LastY = iY;

			// g_Gui->OnMouseMove(iX, iY, MoveX, MoveY);

			// g_CustomMenu->CheckMousePos();

			if(bMButton)
				g_Camera->MouseMove((float)MoveX, (float)MoveY);
			break;
		case WM_MOUSEWHEEL:
		 int zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
			// g_NewsSystem->ProcessMouseWheel(zDelta/18);
			break; */
	}
	return muWindowProc(hwnd, uMsg, wParam, lParam);
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


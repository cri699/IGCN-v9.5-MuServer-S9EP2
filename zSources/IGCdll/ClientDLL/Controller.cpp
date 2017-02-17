//////////////////////////////////////////////////////////////////////
// Controller.cpp
#include "stdafx.h"
#include "Controller.h"
//#include "Interface.h"
#include "CameraV2.h"
#include "offsets.h"

// ----------------------------------------------------------------------------------------------

Controller	g_Controller;
// ----------------------------------------------------------------------------------------------

bool Controller::Load()
{
	if( !this->m_MouseHook )
	{
		this->m_MouseHook = SetWindowsHookEx(WH_MOUSE, this->Mouse, g_Controller.m_Instance, GetCurrentThreadId());
		// ----
		if( !this->m_MouseHook )
		{
			return false;
		}
	}
	// ----
/*	if( !this->m_KeyboardHook ) // cause chat typing cursor problem
	{
		this->m_KeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, this->Keyboard, g_Controller.m_Instance, NULL);
		// ----
		if( !this->m_KeyboardHook )
		{
			return false;
		}
	}
	// ---- */
	return true;
}
// ----------------------------------------------------------------------------------------------

LRESULT Controller::Mouse(int Code, WPARAM wParam, LPARAM lParam)
{
	if( GetForegroundWindow() != pGameWindow )
	{
		return CallNextHookEx(g_Controller.m_MouseHook, Code, wParam, lParam);
	}
	// ----
	MOUSEHOOKSTRUCTEX * Mouse	= (MOUSEHOOKSTRUCTEX*)lParam;
	// ----
	g_Camera.Run(Mouse, wParam);
	// ----
	switch(wParam)
	{
	case WM_LBUTTONUP:
	case WM_LBUTTONDOWN:
		{
			//Interface events (Buttons)
		}
		break;
	}
	// ----
	return CallNextHookEx(g_Controller.m_MouseHook, Code, wParam, lParam);
}
// ----------------------------------------------------------------------------------------------

LRESULT Controller::Keyboard(int Code, WPARAM wParam, LPARAM lParam)
{
	if( (Code == HC_ACTION) && (wParam == WM_KEYDOWN))
	{
		KBDLLHOOKSTRUCT Hook = *((KBDLLHOOKSTRUCT*)lParam);
		// ----
		/*if( Hook.vkCode == VK_END )	
		{
			if( NewWindowProc == NULL )
			{
				NewWindowProc = SetWindowLong(pGameWindow, GWL_WNDPROC, (long)WindowProc);
			}

			WindowState();
		}*/
		// ----
		if( GetForegroundWindow() != pGameWindow )
		{
			return CallNextHookEx(g_Controller.m_KeyboardHook, Code, wParam, lParam);
		}
		// ----
		switch(Hook.vkCode)
		{
			// --
		case VK_F11:
			{
				g_Camera.Init();
			}
			break;
			// --
		case VK_F12:
			{
				g_Camera.Switch();
			}
			break;
		}
	} 
	return CallNextHookEx(g_Controller.m_KeyboardHook, Code, wParam, lParam);
}
// ----------------------------------------------------------------------------------------------

LRESULT Controller::Window(HWND Window, DWORD Message, WPARAM wParam, LPARAM lParam)
{
	/*switch(Message)
	{
	case TRAYMODE_ICON_MESSAGE:
		{
			switch(lParam)
			{
			case WM_LBUTTONDBLCLK:
				{
					gTrayMode.SwitchState();
				}
				break;
			}
		}
		break;
	}
	// ----
	return CallWindowProc((WNDPROC)gTrayMode.TempWindowProc, Window, Message, wParam, lParam);*/
	return 0;
}
// ----------------------------------------------------------------

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// Controller.h
#pragma once
// ----------------------------------------------------------------------------------------------

class Controller
{
public:
	bool	Load();
	// ----
	static	LRESULT WINAPI Mouse(int Code, WPARAM wParam, LPARAM lParam);
	static	LRESULT WINAPI Keyboard(int Code, WPARAM wParam, LPARAM lParam);
	static	LRESULT WINAPI Window(HWND Window, DWORD Message, WPARAM wParam, LPARAM lParam);
	// ----
	HINSTANCE	m_Instance;
private:
	HHOOK		m_MouseHook;
	HHOOK		m_KeyboardHook;
	// ----
}; extern Controller g_Controller;
// ----------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


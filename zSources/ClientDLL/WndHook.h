//////////////////////////////////////////////////////////////////////
// WndHook.h
#ifndef WND_HOOK_H
#define WND_HOOK_H
//004D804D   C745 D4 FF6E4D00 MOV DWORD PTR SS:[EBP-2C],main_f.004D6EF>

#define MU_WND_PROC_HOOK 0x004D8AC7+3 // S9
#define MU_WND_PROC 0x004D791F // S9

extern WNDPROC muWndProc;
extern HWND MuWindow;
extern HWND LauncherWindowHandle;
extern bool gLauncherProxyEnabled;
extern bool gLauncherProxyReconnectInitiated;
LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


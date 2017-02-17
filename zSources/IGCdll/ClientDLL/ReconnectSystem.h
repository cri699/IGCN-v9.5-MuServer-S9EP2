//////////////////////////////////////////////////////////////////////
// ReconnectSystem.h
#ifndef ANTIDC_H
#define ANTIDC_H

#define MU_FONT_ADDRESS 0x123CA9C // S9
//0080F98F   55               PUSH EBP
#define RENDER_HP_BAR_CALLBACK 0x0082141C// S9


extern int g_UserQuit;
extern int g_ReconnectProcess;
extern int g_MuBotEnabled;
extern HANDLE hReconnectThread;
extern HANDLE hCGLiveThread;

extern BYTE Login[200];
extern BYTE bCharacter[16];
extern BYTE bServer[6];

void HookDCFunc();
bool ConnectToServer(char* ip, WORD port);
void ReconnectThread();
void CGLiveThread();
void RenderLoadingBar();
void HookExitCharSelectFunc();
void HookExitFunc();
extern BYTE *reconnectBuf;
#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


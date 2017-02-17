//////////////////////////////////////////////////////////////////////
// ReconnectSystem.cpp
#include "stdafx.h"
#include "ReconnectSystem.h"
//#include "ProtocolCore.h"
#include <winsock2.h>
#include <ws2tcpip.h>
//#include "Connection.h"
//#include "Miscs.h"
#include "Text.h"
#include "Interface.h"
#include "EncDec.h"
#include "ServerInfo.h"
#include "HookManager.h"
#include "protocol.h"
#include "giocp.h"
#include "offsets.h"
#include "WndHook.h"
BYTE Login[200];
BYTE bCharacter[16];
BYTE bServer[6];

HFONT* muFont = (HFONT*)MU_FONT_ADDRESS;
HANDLE hReconnectThread;
HANDLE hCGLiveThread;
int g_UserQuit = 0;
int g_ReconnectProcess = 0;
int g_MuBotEnabled = -1;
BYTE *reconnectBuf;
DWORD dwTickReconnect = 0;

char * ValidateAndReturnCSIP(char *input)
{
	struct sockaddr_in sa;

    int result = inet_ptonXP(AF_INET, input, &(sa.sin_addr));
	if(result == 0)
	{
		// Resolve DNS
		hostent * host = gethostbyname(input);

		if(!host)
		{
			return input;
		}

		in_addr * address = (in_addr * )host->h_addr;
		char* ip_address = inet_ntoaXP(* address);
		return ip_address;
	}
	else
	{
		return input;
	}
}

bool ConnectToServer(char* ip, WORD port)
{
	char text[1000];

	WSADATA wsaData;
	int iResult2 = WSAStartup(MAKEWORD(2, 2), &wsaData);
	*g_MuSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (*g_MuSocket == INVALID_SOCKET) {
		sprintf(text, "socket function failed with error: %ld\n", WSAGetLastError());
		WSACleanup();
		//	MessageBoxA(0, text, 0, 0);
		return false;
	}
	sockaddr_in clientService;
	clientService.sin_family = AF_INET;
	clientService.sin_addr.s_addr = inet_addr(ip);
	clientService.sin_port = htons(port);

	int iResult = connect(*g_MuSocket, (SOCKADDR *)& clientService, sizeof(clientService));
	if (iResult == SOCKET_ERROR) {
		sprintf(text, "connect function failed with error: %ld\n", WSAGetLastError());
		//MessageBoxA(0, text, 0, 0);
		iResult = closesocket(*g_MuSocket);
		if (iResult == SOCKET_ERROR)
			sprintf(text, "closesocket function failed with error: %ld\n", WSAGetLastError());
		//MessageBoxA(0, text, 0, 0);
		WSACleanup();
		return false;

	}
	//HWND windowHandle = FindWindowA(NULL, "MU");
	WSAAsyncSelect(*g_MuSocket, MuWindow, WM_USER, FD_WRITE | FD_READ);

	return true; 
}
void __declspec(naked) HookDCFunc()
{
	//004E1394   C705 106A1E01 01>MOV DWORD PTR DS : [11E6A10], 1

	//004E139E   68 08EB0101      PUSH main_f.0101EB08                     ; ASCII "> Connection closed. "



	if (g_UserQuit == 1)
	{
		_asm{
			MOV DWORD PTR DS : [0x123D09C], 1; // S9
			mov EDX, 0x004E1BA8; // S9
			jmp EDX;

		}
	}
	else
	{
		if (g_ReconnectProcess == 0 || g_ReconnectProcess == 6)
		{
			// this is where it close connection (hook)I must add it here? ye, add Close and Open
			closesocket(*g_MuSocket);
			g_ReconnectProcess = 1;
			hReconnectThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ReconnectThread, NULL, NULL, NULL);
		}
		//	OnConnect();
		_asm {
			//004E1403   833D 18F31A01 05 CMP DWORD PTR DS:[11AF318],5

			MOV EDX, 0x004E1C3F; // S9
			JMP EDX;

		}
	}
}

void CGLiveThread() // not used, so not ported for s9
{
	//004D35F0  /$ 55             PUSH EBP

	//004D360E  |. 833D 04177908 >CMP DWORD PTR DS:[8791704],0
	while (true)
	{
		MemSet(0x8791704, 1, 1);
		_asm
		{
			mov edx, 0x004D35F0;
			call edx;
		}
		Sleep(18000);
	}
}

void ReconnectThread() // can u attach debugger? yes
{
	bool bConnect = false;
	int ReconnectPhase = 0;
	DWORD dwPhaseTick = GetTickCount();
	if(gLauncherProxyEnabled == true)
	{
		SendMessageA(LauncherWindowHandle,0x0502,NULL,NULL);
		while(!gLauncherProxyReconnectInitiated)
		{
			Sleep(1);
		}
	}
	while (g_ReconnectProcess != 6)
	{
		/*if(dwTickReconnect != 0 && GetTickCount() - dwTickReconnect > 55000)
		{
			MessageBoxA(0,"Reconnect failed.. Please restart Game Client","Info",0);
			ExitProcess(1);
		}*/

		if(GetTickCount() - dwPhaseTick > 20000)
		{
			if(g_ReconnectProcess < 6)
			{
				dwPhaseTick = GetTickCount();
				ReconnectPhase += 1;
				g_ReconnectProcess = 1;
				closesocket(*g_MuSocket);
				if(ReconnectPhase > 15)
				{
					MessageBoxA(0,"Reconnect failed.. Please restart Game Client","Info",0);
					ExitProcess(1);
				}
			}
		}

		if (g_ReconnectProcess == 1)
		{
			dwTickReconnect = GetTickCount();
			g_ReconnectProcess = 2;
			MemSet(0x1205338, 5, 1); // set to render ingame, S9
			//	g_ServerInfo->GetIp();
			do
			{
				char * ip = g_ServerInfo->GetIp();
				bConnect = ConnectToServer(ip, g_ServerInfo->GetPort());
				Sleep(1000);
			} while (!bConnect);

			BYTE packet[4] = { 0xC1, 0x04, 0xF4, 0x06 };
			send(*g_MuSocket, (const char*)packet, 4, 0);
			Sleep(1000);
			//BYTE selectgs[6] = { 0xC1, 0x06, 0xF4, 0x03, 0x00, 0x00 };
			send(*g_MuSocket, (const char*)bServer, 6, 0);
			Sleep(1000);

		}
		if (g_ReconnectProcess == 3)
		{
			PMSG_CONNECT_INFO* packet = (PMSG_CONNECT_INFO*)reconnectBuf;
			closesocket(*g_MuSocket);
			g_ReconnectProcess = 4;
			bConnect = ConnectToServer(ValidateAndReturnCSIP(packet->IP), packet->Port);
			Sleep(1000);
			//if(!bConnect){
			//	g_ReconnectProcess = 1;}
			//else
			//{
				
			//°///}

		}
		if (g_ReconnectProcess == 5)
		{
			SendPacket(Login, sizeof(Login), 1, 0);
			//send(*g_MuSocket, (const char*)Login, 0xAB, 0);
			Sleep(2000);
			BYTE CharListPacket[4] = { 0xC1, 0x04, 0xF3, 0x0D };
			send(*g_MuSocket, (const char*)CharListPacket, sizeof(CharListPacket), 0);

			Sleep(2000);
			//BYTE packetz[14] = { 0xc1, 0x0e, 0xf3, 0x0e, 0x77, 0x27, 0x86, 0x56, 0x9c, 0xaf, 0x6e, 0xa2, 0xc4, 0xa3 };
			//SendPacket(bCharacter, sizeof(bCharacter), 0, 0
			send(*g_MuSocket, (const char*)bCharacter, 0x0f, 0);
			g_ReconnectProcess = 6;
			MemSet(0x8B9797C, 1, 1); // S9
			Sleep(2000);
			CGEnableMUBot(g_MuBotEnabled);
			gLauncherProxyReconnectInitiated = false;
			TerminateThread(hReconnectThread, 0);
		}
		Sleep(1);
	}
}
DWORD dwLastLoadTick = 0;
float Perc = 0.00f;
void RenderLoadingBar()
{
	if (g_ReconnectProcess == 0 || g_ReconnectProcess == 6)
		return;
	// 78F4F0 (resource id, X, Y, (size in pixels?), 8.0)
	typedef int(*tRenderLoadingBar)(int, float, float, float, float);
	//007CB14F   55               PUSH EBP


	tRenderLoadingBar RenderLoadBar = (tRenderLoadingBar)0x7CB14F;
	//0x7A65u, v5, v7, 160.0, 18.0);
	//int screenvec[2];
//	muGetWindowSize(screenvec);

	//float m_fFramePosX = (screenvec[0]/2) - 209.0f;
	//float m_fFramePosY = (screenvec[1]/100.0f*20.0f);
	float m_fFramePosX = 320.0f - 104.5f;
	float m_fFramePosY = 480.0f / 100.0f*15.0f;
	RenderLoadBar(0x7B14u, m_fFramePosX, m_fFramePosY, 209.0f, 80.0);
	RenderLoadBar(0x7A65u, m_fFramePosX + 24.5f, m_fFramePosY + 36.0f, 160.0f, 18.0);

	if (GetTickCount() - dwLastLoadTick > 75)
	{
		if (Perc < 150.0f)
		{
			Perc += 1.00f;
		}
		else
		{
			Perc = 0.00f;
		}
		dwLastLoadTick = GetTickCount();
	}
	RenderLoadBar(0x7A66u, m_fFramePosX + 24.5f + 5.0f, m_fFramePosY + 5.0f + 36.0f, Perc, 8.0);

	//7B14
	g_Text->SetFont(muFont[0]);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	char rectext[100] = { 0 };
	sprintf(rectext, "Reconnecting. Please wait...");
	g_Text->Out(rectext, m_fFramePosX + 72, m_fFramePosY + 25);	//level

}
/*
007A7370   55               PUSH EBP
007A7371   8BEC             MOV EBP,ESP
007A7373   6A FF            PUSH -1
007A7375   68 5192CD00      PUSH main.00CD9251                       ; SE handler installation
007A737A  |. 64:A1 00000000     MOV EAX,DWORD PTR FS:[0]

*/
//009C6995   68 68641E01      PUSH main.011E6468

void __declspec(naked) HookExitCharSelectFunc()
{
	//0043F244   68 C0A30101      PUSH main.0101A3C0                       ; ASCII "> Menu - Exit game."
	g_UserQuit = 1;
	_asm {
		PUSH 0x0106A400; // s9
		//0043F249   68 68641E01      PUSH main.011E6468
		mov edx,0x43F374; // s9
		jmp edx;
	}

}
void __declspec(naked) HookExitFunc()
{
	//009EDCA0   68 00CB2301      PUSH main.0123CB00

	g_UserQuit = 1;
	_asm{
		PUSH 0x0108AE08; // S9
		MOV EDX, 0x009EDCA0; // S9
		JMP EDX;
	}
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


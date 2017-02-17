//////////////////////////////////////////////////////////////////////
// packetsend_hooks.cpp
#include "stdafx.h"
#include "packetsend_hooks.h"
#include "EncDec.h"
#include "offsets.h"
#include "Camera.h"
#include "protocol.h"
#include "ServerInfo.h"
#include "ReconnectSystem.h"

BYTE hook[6];
BYTE hook2[6];
BYTE wsasendhook[6];
BYTE hookGLenable[6];
BYTE hookGLTex2d[6];
BYTE hookGLparam[6];
BYTE hookdebugstring[6];
BYTE jmp[6] = { 0xe9,0x00, 0x00, 0x00, 0x00 ,0xc3 };  
DWORD pPrevious;
CRITICAL_SECTION nSend_criti;

DWORD HookFunction(LPCSTR lpModule, LPCSTR lpFuncName, LPVOID lpFunction, unsigned char *lpBackup)
{  
      DWORD dwAddr = (DWORD)GetProcAddress(GetModuleHandle(lpModule), (char *)lpFuncName);
      ReadProcessMemory(GetCurrentProcess(), (LPVOID)dwAddr, lpBackup, 6, 0);
      DWORD dwCalc = ((DWORD)lpFunction - dwAddr - 5);
      VirtualProtect((void*) dwAddr, 6, PAGE_EXECUTE_READWRITE, &pPrevious);
      memcpy(&jmp[1], &dwCalc, 4);
      WriteProcessMemory(GetCurrentProcess(), (LPVOID)dwAddr, jmp, 6, 0);
      VirtualProtect((void*) dwAddr, 6, pPrevious, &pPrevious);
      FlushInstructionCache(GetCurrentProcess(),0,0);
      return dwAddr;
}   

BOOL UnHookFunction(LPCSTR lpModule, LPCSTR lpFuncName, unsigned char *lpBackup)
{
DWORD dwAddr = (DWORD)GetProcAddress(GetModuleHandle(lpModule), (char *)lpFuncName);

if (WriteProcessMemory(GetCurrentProcess(), (LPVOID)dwAddr, lpBackup, 6, 0))
        return TRUE;
        FlushInstructionCache(GetCurrentProcess(),0,0);

return FALSE;  
}

//This function works like GetProcAddress 
//This function is useful in the event that GetProcAddress is not imported
//or when you are concerned it may be hooked
FARPROC GetProcedureAddress(HANDLE hModule, char* pszProcName)
{
	IMAGE_DOS_HEADER* pdhDosHeader = (IMAGE_DOS_HEADER*)hModule;
	//Check if valid PE
	if (pdhDosHeader->e_magic != IMAGE_DOS_SIGNATURE) return 0;

	IMAGE_NT_HEADERS* pndNTHeader = (IMAGE_NT_HEADERS*)(pdhDosHeader->e_lfanew + (long)hModule);
	if (pndNTHeader->Signature != IMAGE_NT_SIGNATURE) return 0;

	//Traverse the export table to see if we can find the export
	IMAGE_EXPORT_DIRECTORY* iedExports = (IMAGE_EXPORT_DIRECTORY*)(pndNTHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress + (long)hModule);
	long* pNames = (long*)(iedExports->AddressOfNames + (long)hModule);
	short wOrdinalIndex = -1;
	for (int i = 0; i < iedExports->NumberOfFunctions; i++)
	{
		char* pszFunctionName = (char *)(pNames[i] + (long)hModule);

		if (lstrcmpi(pszFunctionName, pszProcName) == 0)
		{
			wOrdinalIndex = i;
			break;
		}
	}

	if (wOrdinalIndex == -1) return 0;

	//wIndex now holds the index of the function name in the names array, which is the index of the ordinal.
	//The ordinal also acts as the index of the address
	short* pOrdinals = (short*)(iedExports->AddressOfNameOrdinals + (long)hModule);
	unsigned long* pAddresses = (unsigned long*)(iedExports->AddressOfFunctions + (long)hModule);

	short wAddressIndex = pOrdinals[wOrdinalIndex];
	return (FARPROC)(pAddresses[wAddressIndex] + (long)hModule);
}

int __stdcall nWSASend(SOCKET s, LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesSent, DWORD dwFlags, LPWSAOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine){
	UnHookFunction("ws2_32.dll", "WSASend", wsasendhook);

	int result = WSASend(s, lpBuffers, dwBufferCount, lpNumberOfBytesSent, dwFlags, lpOverlapped, lpCompletionRoutine);

	//result = send(s,buf,len,flags);
	HookFunction("ws2_32.dll", "WSAsend", (LPVOID) nWSASend, wsasendhook);
	return result;
}

struct PMSG_PIDSEND
{
	PBMSG_HEAD h; // C1 : F4 
	BYTE subcode; // 3D
	DWORD pid;
};

int __stdcall nSend(SOCKET s, const char *buf, int len,int flags){
	EnterCriticalSection(&nSend_criti);
	UnHookFunction("ws2_32.dll", "send", hook);

	int result;
	
	int hc = buf[0]&0xFF;
	int proto = buf[2]&0xFF;
	int subproto = buf[3]&0xFF;
	int sz = buf[1] & 0xFF;
	if(hc == 0xC1 || hc == 0xC3)
	{
		if(proto == 0xF4 && hc == 0xC1)
		{ 
			if(g_ServerInfo->GetLauncher())
			{
				PMSG_PIDSEND pMsg;
				pMsg.h.c = 0xC1;
				pMsg.h.headcode = 0xF4;
				pMsg.h.size = sizeof(pMsg);
				pMsg.subcode = 0x3D;
				pMsg.pid = GetCurrentProcessId();
				send(s,(const char*)&pMsg,sizeof(pMsg),flags);
			}
			if (proto == 0xF4 && hc == 0xC1 && sz == 0x06 && subproto == 0x03)
			{
				memcpy(bServer, buf, buf[1]);
			}
			result = send(s,buf,len,flags);
		}

		else
		{
			result = send(s,buf, len, flags);
		}
	}
	
	else if(hc == 0xC2 || hc == 0xC4)
	{
		result = send(s, buf, len, flags);
	}
	

	//result = send(s,buf,len,flags)
	HookFunction("ws2_32.dll", "send", (LPVOID) nSend, hook);
	LeaveCriticalSection(&nSend_criti);
	return result;
}

void __stdcall nglEnable(GLenum cap)
{
	UnHookFunction("OpenGL32.dll","glEnable",hookGLenable);

	glEnable(cap);	
	glDisable(GL_FOG);
	
//	g_Sky->Render();

	//Cam zpos
	// 1159.000f
	// 335.000f

//	glEnable(GL_FOG);
	// 1600 - 2200
	// 900 - 1400
	if(*g_Map == -1 || *g_Map == 0x4a || *g_Map == 0x49 || *g_Map == 0x4E || *g_Map == 34 || *g_Map == 33 ||
		*g_Map == 0x5D || *g_Map == 0x5E)
	{
		
	}
	else
	{
		float FogCol[3]={0.8f,0.8f,0.8f}; // Define a nice light grey
		float* CamPos = (float*)CAMERA_POSITION_ADDR;
		float startFog = (1.177f*CamPos[2])+505.657f;
		float endFog = (1.03f*CamPos[2])+1054.95f;
		glEnable (GL_FOG);
		// Set the fog color
		glFogi(GL_FOG_MODE, GL_LINEAR);
		glFogf(GL_FOG_DENSITY, 2.0f);
		glFogfv(GL_FOG_COLOR,FogCol); 
		glFogf(GL_FOG_START, startFog);
		glFogf(GL_FOG_END, endFog);

		if (cap == GL_BLEND || cap == GL_TEXTURE_2D || cap == GL_DEPTH_TEST)   
		{
			glDisable(GL_FOG);  
		}

	}
	

	HookFunction("OpenGL32.dll","glEnable", (LPVOID)nglEnable, hookGLenable);
}

void __stdcall nOutputDebugStringA(LPCSTR lpOutput)
{
	UnHookFunction("kernel32.dll", "OutputDebugStringA", hookdebugstring);

	HookFunction("kernel32.dll", "OutputDebugStringA", (LPVOID)nOutputDebugStringA, hookdebugstring);
}
	
void DoHooks()
{
	
	/* typedef void(*GW)();

	HMODULE hModule = LoadLibrary(TEXT("IGCPatcher.DLL"));
	
	GW GetWelcomeMessage = (GW)GetProcAddress(hModule, "TestEx");
	MessageBoxA(0, 0, 0, 0);
	GetWelcomeMessage(); */
	InitializeCriticalSection(&nSend_criti);

	HookFunction("ws2_32.dll", "send", (LPVOID) nSend, hook);
	HookFunction("ws2_32.dll", "WSASend", (LPVOID) nWSASend, wsasendhook);
	HookFunction("kernel32.dll", "OutputDebugStringA", (LPVOID) nOutputDebugStringA, hookdebugstring);
	/* HookFunction("OpenGL32.dll","glEnable", (LPVOID)nglEnable, hookGLenable);
	HookFunction("OpenGL32.dll","glTexImage2D", (LPVOID)nglTexImage2D, hookGLTex2d);
	HookFunction("OpenGL32.dll","glTexParameteri",(LPVOID)nglTexParameteri, hookGLparam);
	HookFunction("ws2_32.dll", "recv", (LPVOID) Mine_Recv, hook2); */
}

	/* float maximumAnistropy;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maximumAnistropy);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maximumAnistropy); */


//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


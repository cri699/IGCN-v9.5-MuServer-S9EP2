//////////////////////////////////////////////////////////////////////
// AntiHack.cpp
#include "stdafx.h"
#include "AntiHack.h"
#include "packetsend_hooks.h"
#include "offsets.h"
#include "protocol.h"
#include "giocp.h"
// Globals
LPBYTE _GetTickCount = (LPBYTE)GetProcedureAddress(GetModuleHandle("kernel32.dll"), "GetTickCount");
BYTE originalqueryperformancecounter[5] = { 0x8B, 0xFF, 0x55, 0x8B, 0xEC };
HANDLE hCheckThread = NULL;
// functions
void CheckSH()
{
	Sleep(50);
}
void AHCheckGetTickHook()
{
	if (_GetTickCount != NULL)
	{
		if (_GetTickCount[0] == 0xE9 || _GetTickCount[0] == 0x90)
		{
			ExitProcess(0);
		}
	}
}

//004E134D   C745 98 AA000000 MOV DWORD PTR SS:[EBP-68],0AA
//004E1325   6A 70            PUSH 70


void AHCheckSwapBufferMod()
{
	BYTE Val1[1];
	BYTE Val2[1];
	DWORD OldProtect;

	
	DWORD AH_SWAP_BUFFER_OFFSET1 = 0x004E1B57+3; // S9
	DWORD AH_SWAP_BUFFER_OFFSET2 = 0x004E1B2F+1; // S9

	VirtualProtect((LPVOID)(AH_SWAP_BUFFER_OFFSET1), 4, PAGE_EXECUTE_READWRITE, &OldProtect);
	VirtualProtect((LPVOID)(AH_SWAP_BUFFER_OFFSET2), 1, PAGE_EXECUTE_READWRITE, &OldProtect);

	ReadProcessMemory(GetCurrentProcess(), (LPVOID)AH_SWAP_BUFFER_OFFSET1,Val1, 1, 0);
	ReadProcessMemory(GetCurrentProcess(), (LPVOID)AH_SWAP_BUFFER_OFFSET1,Val2, 1, 0);

	if(Val1[0] != Val2[0])
	{
		GCSendAntihackBreach(90000);
		int ofs = rand()%0xFFFFFFFF;
		_asm
		{
			mov edx,ofs;
			jmp edx;
		}
	}
	if(Val1[0] != 0x28 && Val2[0] != 0x28)
	{
		GCSendAntihackBreach(90000);
		int ofs = rand()%0xFFFFFFFF;
		_asm
		{
			mov edx,ofs;
			jmp edx;
		}
	}

	
}

HMODULE GetCurrentModule()
{
	HMODULE hModule = NULL;
	GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPCTSTR)GetCurrentModule, &hModule);

	return hModule;
}

int PrintModules( DWORD processID )
{
    HMODULE hMods[1024];
    HANDLE hProcess;
    DWORD cbNeeded;
    unsigned int i;

    // Print the process identifier.

    printf( "\nProcess ID: %u\n", processID );

    // Get a handle to the process.

    hProcess = OpenProcess( PROCESS_QUERY_INFORMATION |
                            PROCESS_VM_READ,
                            FALSE, processID );
    if (NULL == hProcess)
        return 1;

   // Get a list of all the modules in this process.

    if( EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded))
    {
        for ( i = 0; i < (cbNeeded / sizeof(HMODULE)); i++ )
        {
            TCHAR szModName[MAX_PATH];

            // Get the full path to the module's file.

            if ( GetModuleFileNameEx( hProcess, hMods[i], szModName,
                                      sizeof(szModName) / sizeof(TCHAR)))
            {
                // Print the module name and handle value.

                _tprintf( TEXT("\t%s (0x%08X)\n"), szModName, hMods[i] );
            }
        }
    }
    
    // Release the handle to the process.

    CloseHandle( hProcess );

    return 0;
}

int CheckHitHack()
{
	DWORD dwHitAddr1 = *(DWORD*)0x7f7e904;
	DWORD dwHitAddr2 = *(DWORD*)0x9ca0aec;

	BYTE ValHitHack[4];
	BYTE ValHitHack2[4];

	dwHitAddr1 += 0x4D4;
	dwHitAddr2 += 0x4D4;

	//ReadProcessMemory(GetCurrentProcess(), (LPVOID)dwHitAddr1,ValHitHack, 4, 0);
	//ReadProcessMemory(GetCurrentProcess(), (LPVOID)dwHitAddr2,ValHitHack2, 4, 0);
	//0x7d2b7cda
	
	PMSG_SEND_HITHACK_INFO pMsg;
	
	pMsg.h.set((LPBYTE)&pMsg,0xFA,0x15,sizeof(pMsg));
	pMsg.dwHitHackVal1 = *(DWORD*)dwHitAddr1;
	if(dwHitAddr2 != 0x4D4)
	{
		pMsg.dwHitHackVal2 = *(DWORD*)dwHitAddr2;
	}
	DataSend((LPBYTE)&pMsg, pMsg.h.size);
	/*
	if(ValHitHack[0] == 0xDA && ValHitHack[1] == 0x7C && ValHitHack[2] == 0x2B && ValHitHack[3] == 0x7D)
	{
		GCSendAntihackBreach(85000);
		int ofs = rand()%0xFFFFFFFF;
		_asm
		{
			mov edx,ofs;
			jmp edx;
		}		
	}

	if(ValHitHack2[0] == 0xDA && ValHitHack2[1] == 0x7C && ValHitHack2[2] == 0x2B && ValHitHack2[3] == 0x7D)
	{
		GCSendAntihackBreach(85000);
		int ofs = rand()%0xFFFFFFFF;
		_asm
		{
			mov edx,ofs;
			jmp edx;
		}		
	}*/

	return 0;
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// HookManager.cpp
#include "stdafx.h"
#include "HookManager.h"
#include "Exception.h"
//#include "IgcDll.h"
// "ErrorLog.h"

CHookManager HookManager;

CHookManager::CHookManager()
{
	m_Count = 0;
	m_LastEntry = -1;
}

void CHookManager::XorLine(char* buff, int len)
{
	BYTE xorTable[] = {0xA1, 0x11, 0x8B, 0x0F, 0x28, 0x17, 0x95, 0xEE, 0x1B, 0xCD, 0x58, 0x31, 0x0E, 0x4D, 0x7E, 0x41, 0x21, 0xFD, 0xFE, 0x7C, 0xC1, 0xC2, 0xAF, 0x3C, 0xFA, 0x14};
	for (int i = 0; i < len; i++)
	{
		buff[i]^=xorTable[i%5];
	}
}

void CHookManager::DumpList(std::string file)
{
	FILE* hFile = fopen(file.c_str(), "wb");
	if (!hFile)
		return;

	SYSTEMTIME time;
	GetLocalTime(&time);

	char line[1024];
	int lineLen;

	memset(line, 0, 1024);
	sprintf(line, "%d-%d-%d %d:%d Hook Manager Dump\r\n\r\n", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute);
	lineLen = strlen(line);
	XorLine(line, lineLen);
	fwrite(&lineLen, 1, 2, hFile);
	fwrite(line, 1, lineLen, hFile);

	memset(line, 0, 1024);
	sprintf(line, "Enter: %04X\r\n", m_LastEntry);
	lineLen = strlen(line);
	XorLine(line, lineLen);
	fwrite(&lineLen, 1, 2, hFile);
	fwrite(line, 1, lineLen, hFile);

	memset(line, 0, 1024);
	sprintf(line, "Hooks dump list:\r\n");
	lineLen = strlen(line);
	XorLine(line, lineLen);
	fwrite(&lineLen, 1, 2, hFile);
	fwrite(line, 1, lineLen, hFile);

	std::list<HOOK_STRUCT>::iterator it;
	for (it = m_List.begin(); it != m_List.end(); it++)
	{
		memset(line, 0, 1024);
		sprintf(line, "- %s;\t0x%04X\t0x%00000000X\r\n", (*it).Type.c_str(), (*it).Id, ((*it).Addr)^0x3ED4A519);
		lineLen = strlen(line);
		XorLine(line, lineLen);
		fwrite(&lineLen, 1, 2, hFile);
		fwrite(line, 1, lineLen, hFile);
	}
	fclose(hFile);

}

void CHookManager::RegisterHook(DWORD Addr, DWORD Type)
{
	HOOK_STRUCT hook;
	hook.Addr = Addr;
	hook.Id = m_Count;
	m_Count++;
	switch(Type)
	{
		case 0:
			hook.Type = "Jmp";
			break;
		case 1:
			hook.Type = "Call";
			break;
		case 2:
			hook.Type = "Class";
			break;
	}
	m_List.push_back(hook);

}

void CHookManager::MakeJmpHook(DWORD Addr, void* pProc)
{
	try {
		DWORD memlen = 10;		// MOV DWORD PTR...
		memlen+=5;				// JMP pProc;

		DWORD NewAddr = (DWORD)VirtualAlloc(NULL, memlen, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
		if (!NewAddr)
		{
			throw CException("Virtual Alloc error: %d", GetLastError());
		}

		DWORD offs = 0;

		BYTE aMovMem[] = {0xC7, 0x05};
		memcpy((LPVOID)(NewAddr+offs), aMovMem, 2);
		offs+=2;

		DWORD* pEntry = &m_LastEntry;
		memcpy((LPVOID)(NewAddr+offs), &pEntry, 4);
		offs+=4;

		memcpy((LPVOID)(NewAddr+offs), &m_Count, 4);
		offs+=4;

		DWORD OldProtect;
		VirtualProtect((LPVOID)Addr, 5, PAGE_EXECUTE_READWRITE, &OldProtect);
		__asm
		{
			MOV ECX, Addr;
			ADD ECX, 5;
			MOV EDX, NewAddr;
			SUB EDX, ECX;
			MOV EAX, Addr;
			MOV DWORD PTR DS:[EAX+1], EDX;
			MOV BYTE PTR DS:[EAX], 0xE9;
		}

		VirtualProtect((LPVOID)Addr, 5, OldProtect, &OldProtect);
		__asm
		{
			MOV ECX, NewAddr;
			ADD ECX, 5;
			ADD ECX, offs;
			MOV EDX, pProc;
			SUB EDX, ECX;
			MOV EAX, NewAddr;
			ADD EAX, offs;
			MOV DWORD PTR DS:[EAX+1], EDX;
			MOV BYTE PTR DS:[EAX], 0xE9;
		}
		RegisterHook(Addr, 0);

	}
	catch(std::exception& e)
	{
//		CErrorLog::OutException(e.what());
	}
}

void CHookManager::MakeCallback(DWORD Addr, void* pFunc, int Args, int MemSize, bool saveEcx)
{
	//MessageBox(0, "STOP", "STOP", MB_OK);

	try {
		BYTE aStartFunc[3] = {0x55, 0x8b, 0xec};
		BYTE aEndFunc[3] = {0x8b, 0xe5, 0x5d};

		BYTE aStartFuncWClass[4] = {0x55, 0x51, 0x8b, 0xec};
		BYTE aEndFuncWClass[4] = {0x8b, 0xe5, 0x59, 0x5d};
		DWORD Func = (DWORD)pFunc;
		//BYTE* buff = new BYTE[Mem];
		//memcpy(buff, (LPVOID)Addr, Mem);

		// Calculate memory size
		int memlen = 0;
		memlen+=10;			// MOV DWORD PTR DS[], Id;
		memlen+=3;			// PUSH EBP / MOV EPB,ESP
		memlen+=(3*Args);	// PUSH DWORD PTR SS:[EBP+8] etc...
		memlen+=5;			// CALL Func
		memlen+=3;			// MOV ESP, EBP / POP EBP
		memlen+=MemSize;		// Memory to copy
		memlen+=5;			// JMP to oryginal function

		if (saveEcx)
		{
			memlen+=2;		// Push/pop exc
		}

		DWORD newAddr = (DWORD)VirtualAlloc(NULL, memlen, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
		if (!newAddr)
		{
			throw CException("VirtualAlloc error: %d", GetLastError());
		}

		DWORD offset = newAddr;

		BYTE aMovMem[] = {0xC7, 0x05};
		memcpy((LPVOID)(offset), aMovMem, 2);
		offset+=2;

		DWORD* pEntry = &m_LastEntry;
		memcpy((LPVOID)(offset), &pEntry, 4);
		offset+=4;

		memcpy((LPVOID)(offset), &m_Count, 4);
		offset+=4;

		if (saveEcx)
		{
			memcpy((LPVOID)offset, aStartFuncWClass, 4);
			offset+=4;
		}
		else
		{
			memcpy((LPVOID)offset, aStartFunc, 3);
			offset+=3;
		}

		if (saveEcx)
		{
			for (int i = 0; i < Args; i++)
			{
				*(WORD*)offset = 0x75FF;
				BYTE arg = (0xC+(4*(Args-i)));
				*(BYTE*)(offset+2) = arg;
				offset+=3;
			}
		}
		else
		{
			for (int i = 1; i <= Args; i++)
			{
				*(WORD*)offset = 0x75FF;
				BYTE arg = (8+(4*(Args-i)));
				*(BYTE*)(offset+2) = arg;
				offset+=3;
			}
		}

		*(BYTE*)offset = 0xE8;
		DWORD calladdr = (Func-(offset+5));
		*(DWORD*)(offset+1) = calladdr;
		offset+=5;

		if (saveEcx)
		{
			memcpy((LPVOID)offset, aEndFuncWClass, 4);
			offset+=4;
		}
		else
		{
			memcpy((LPVOID)offset, aEndFunc, 3);
			offset+=3;
		}

		memcpy((LPVOID)offset, (LPVOID)Addr, MemSize);
		offset+=MemSize;

		*(BYTE*)offset = 0xE9;
		DWORD jmpaddr = (Addr-(offset+5)+MemSize);
		*(DWORD*)(offset+1) = jmpaddr;
		offset+=5;

		//MakeJmpHook(Addr, (LPVOID)newAddr);
		DWORD OldProtect;
		VirtualProtect((LPVOID)Addr, 5, PAGE_EXECUTE_READWRITE, &OldProtect);
		__asm
		{
			MOV ECX, Addr;
			ADD ECX, 5;
			MOV EDX, newAddr;
			SUB EDX, ECX;
			MOV EAX, Addr;
			MOV DWORD PTR DS:[EAX+1], EDX;
			MOV BYTE PTR DS:[EAX], 0xE9;
		}
		VirtualProtect((LPVOID)Addr, 5, OldProtect, &OldProtect);
		RegisterHook(Addr, 1);

	}
	catch (std::exception& e)
	{

	}
}

void CHookManager::MakeClassCallback(DWORD Addr, void* pMethod, void* pObject, int Args, int MemSize, bool saveEcx)
{
	try {
		BYTE aStartFunc[3] = {0x55, 0x8b, 0xec};
		BYTE aEndFunc[3] = {0x8b, 0xe5, 0x5d};

		BYTE aStartFuncWClass[4] = {0x55, 0x51, 0x8b, 0xec};
		BYTE aEndFuncWClass[4] = {0x8b, 0xe5, 0x59, 0x5d};
		DWORD Method = (DWORD)pMethod;
		//BYTE* buff = new BYTE[Mem];
		//memcpy(buff, (LPVOID)Addr, Mem);

		// Calculate memory size
		int memlen = 0;
		memlen+=10;			// MOV DWORD PTR DS[], Id;
		memlen+=3;			// PUSH EBP / MOV EPB,ESP
		memlen+=(3*Args);	// PUSH DWORD PTR SS:[EBP+8] etc...
		memlen+=5;			// MOV ECX, ...
		memlen+=5;			// CALL Func
		memlen+=3;			// MOV ESP, EBP / POP EBP
		memlen+=MemSize;		// Memory to copy
		memlen+=5;			// JMP to oryginal function

		if (saveEcx)
		{
			memlen+=2;		// Push/pop exc
		}

		DWORD newAddr = (DWORD)VirtualAlloc(NULL, memlen, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
		if (!newAddr)
		{
			throw CException("VirtualAlloc error: %d", GetLastError());
		}

		DWORD offset = newAddr;

		BYTE aMovMem[] = {0xC7, 0x05};
		memcpy((LPVOID)(offset), aMovMem, 2);
		offset+=2;

		DWORD* pEntry = &m_LastEntry;
		memcpy((LPVOID)(offset), &pEntry, 4);
		offset+=4;

		memcpy((LPVOID)(offset), &m_Count, 4);
		offset+=4;

		if (saveEcx)
		{
			memcpy((LPVOID)offset, aStartFuncWClass, 4);
			offset+=4;
		}
		else
		{
			memcpy((LPVOID)offset, aStartFunc, 3);
			offset+=3;
		}

		if (saveEcx)
		{
			for (int i = 0; i < Args; i++)
			{
				*(WORD*)offset = 0x75FF;
				BYTE arg = (0xC+(4*(Args-i)));
				*(BYTE*)(offset+2) = arg;
				offset+=3;
			}
		}
		else
		{
			for (int i = 1; i <= Args; i++)
			{
				*(WORD*)offset = 0x75FF;
				BYTE arg = (8+(4*(Args-i)));
				*(BYTE*)(offset+2) = arg;
				offset+=3;
			}
		}

		*(BYTE*)offset = 0xB9;
		*(DWORD*)(offset+1) = (DWORD)pObject;
		offset+=5;

		*(BYTE*)offset = 0xE8;
		DWORD calladdr = (Method-(offset+5));
		*(DWORD*)(offset+1) = calladdr;
		offset+=5;

		if (saveEcx)
		{
			memcpy((LPVOID)offset, aEndFuncWClass, 4);
			offset+=4;
		}
		else
		{
			memcpy((LPVOID)offset, aEndFunc, 3);
			offset+=3;
		}

		memcpy((LPVOID)offset, (LPVOID)Addr, MemSize);
		offset+=MemSize;

		*(BYTE*)offset = 0xE9;
		DWORD jmpaddr = (Addr-(offset+5)+MemSize);
		*(DWORD*)(offset+1) = jmpaddr;
		offset+=5;

		//MakeJmpHook(Addr, (LPVOID)newAddr);
		DWORD OldProtect;
		VirtualProtect((LPVOID)Addr, 5, PAGE_EXECUTE_READWRITE, &OldProtect);

		__asm
		{
			MOV ECX, Addr;
			ADD ECX, 5;
			MOV EDX, newAddr;
			SUB EDX, ECX;
			MOV EAX, Addr;
			MOV DWORD PTR DS:[EAX+1], EDX;
			MOV BYTE PTR DS:[EAX], 0xE9;
		}
		VirtualProtect((LPVOID)Addr, 5, OldProtect, &OldProtect);
		RegisterHook(Addr, 1);

	}
	catch(std::exception& e)
	{

	}
}

void ChangeAddress(DWORD Addr, DWORD AddrNew)
{

	DWORD OldProtect;
	VirtualProtect((LPVOID)Addr, 4, PAGE_EXECUTE_READWRITE, &OldProtect);
	__asm
	{
		MOV EAX, Addr;
		MOV EDX, AddrNew;
		MOV DWORD PTR DS:[EAX], EDX;
	}
	VirtualProtect((LPVOID)Addr, 4, OldProtect, &OldProtect);
}

void MemSet(DWORD Addr, int mem, int size)
{
	DWORD OldProtect;
	VirtualProtect((LPVOID)Addr, size, PAGE_EXECUTE_READWRITE, &OldProtect);
	
	memset((void*)Addr, mem, size);

	VirtualProtect((LPVOID)Addr, size, OldProtect, &OldProtect);
}

void SetMemory(DWORD Addr, void* mem, int size)
{

	DWORD OldProtect;
	VirtualProtect((LPVOID)Addr, size, PAGE_EXECUTE_READWRITE, &OldProtect);
	
	memcpy((void*)Addr, mem, size);

	VirtualProtect((LPVOID)Addr, size, OldProtect, &OldProtect);

}

void ModifyValueInt(DWORD Addr, int Val)
{
	DWORD OldProtect;
	VirtualProtect((LPVOID)Addr, 4, PAGE_EXECUTE_READWRITE, &OldProtect);

	//memcpy((void*)Addr, (void*)Val, 4);
	*(DWORD*)Addr = Val;
	VirtualProtect((LPVOID)Addr, 4, OldProtect, &OldProtect);
}

DWORD WriteMemory(const LPVOID lpAddress, const LPVOID lpBuf, const UINT uSize)
{
    DWORD dwErrorCode   = 0;
    DWORD dwOldProtect  = 0;
	// ----
    int iRes = VirtualProtect(lpAddress, uSize, PAGE_EXECUTE_READWRITE, & dwOldProtect);
	// ----
    if( iRes == 0 )
    {
        dwErrorCode = GetLastError();
        return dwErrorCode;
    }
	// ----
    memcpy(lpAddress, lpBuf, uSize);
	// ----
    DWORD dwBytes   = 0;
	// ----
    iRes = VirtualProtect(lpAddress, uSize, dwOldProtect, & dwBytes);
	// ----
    if ( iRes == 0 )
    {
        dwErrorCode = GetLastError();
        return dwErrorCode;
    }
	// ----
    return 0x00;
}

DWORD SetRange(const LPVOID dwAddress, const USHORT wCount, const BYTE btValue)
{
	BYTE * lpBuf	= new BYTE[wCount];
	// ----
	memset(lpBuf, btValue, wCount);
	// ----
	return WriteMemory( dwAddress, (LPVOID) lpBuf, wCount);
}

void HookThis(DWORD FuncOffset,DWORD JmpOffset)
{
	DWORD OldProtect;
	VirtualProtect((LPVOID)JmpOffset, 4, PAGE_EXECUTE_READWRITE, &OldProtect);
	*(DWORD*)(JmpOffset) = 0xE9;
	*(DWORD*)(JmpOffset+1) = FuncOffset - (JmpOffset+5);
}


//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


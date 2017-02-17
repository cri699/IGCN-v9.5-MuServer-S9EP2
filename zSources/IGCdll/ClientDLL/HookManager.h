//////////////////////////////////////////////////////////////////////
// HookManager.h
#ifndef HOOK_MANAGER_H
#define HOOK_MANAGER_H

struct HOOK_STRUCT{
	DWORD Id;
	DWORD Addr;
	std::string Type;
};

class CHookManager{
	DWORD m_LastEntry;
	DWORD m_Count;
	std::list<HOOK_STRUCT> m_List;
public:
	CHookManager();
	void XorLine(char* buff, int len);
	void DumpList(std::string file);
	void RegisterHook(DWORD Addr, DWORD Type);

	void MakeJmpHook(DWORD Addr, void* pProc);
	void MakeCallback(DWORD Addr, void* pFunc, int Args, int MemSize, bool saveEcx);
	void MakeClassCallback(DWORD Addr, void* pMethod, void* pObject, int Args, int MemSize, bool saveEcx);
};

extern CHookManager HookManager;

void ChangeAddress(DWORD Addr, DWORD AddrNew);
void MemSet(DWORD Addr, int mem, int size);
void ChangeAddress(DWORD Addr, DWORD AddrNew);
void SetMemory(DWORD Addr, void* mem, int size);
void ModifyValueInt(DWORD Addr, int Val);
DWORD WriteMemory(const LPVOID lpAddress, const LPVOID lpBuf, const UINT uSize);
DWORD SetRange(const LPVOID dwAddress, const USHORT wCount, const BYTE btValue);
void HookThis(DWORD FuncOffset,DWORD JmpOffset);
#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


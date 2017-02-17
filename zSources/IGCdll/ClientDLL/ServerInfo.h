//////////////////////////////////////////////////////////////////////
// ServerInfo.h
#ifndef SERVER_BMD_H
#define SERVER_BMD_H

#include "ResourceLoader.h"
//#include <SRSA.h>

#pragma pack(push, 1)

struct SERVER_BMD_FIELDS{
	WORD m_Port;
	char m_Serial[16];
	BYTE m_Version[5];
	bool m_AllowMulti;
	bool m_AllowXShop;
	bool m_UseLauncher;
	char m_LauncherFile[17];
	DWORD m_Charset;
	WORD m_Hostlen;
};
#pragma pack(pop)

class CServerInfo : public CResourceLoader{
	std::string m_Ip;
	char m_Serial[16];
	BYTE m_Version[5];
	WORD m_Port;
	WORD m_ChatServerPort;

	int m_PasswordSize;
	BYTE* m_Password;
	DWORD m_PasswordCrc;
	//CPrivateRsaKey* m_PassKey;
	bool m_AllowMulti;
	bool m_AllowXShop;
	bool m_UseLauncher;
	std::string m_LauncherFile;
	bool m_UsePatcher;
	std::string m_PathcherFile;
	DWORD m_Charset;
	bool m_BlockVirtualMachine;
public:
	void Load(std::string file);
	void LoadNew(std::string file);
	std::string Type(){	return "ServerInfo";}
	char* GetIp();
	WORD GetPort();
	WORD GetChatServerPort();
	char* GetSerial();
	BYTE* GetVersion();
	bool GetMulti();
	DWORD GetPassCrc();
	//CPrivateRsaKey* GetPassKey();
	bool GetXShop();
	bool GetLauncher();
	bool GetPatcher();
	bool GetBlockVM();
	std::string GetLauncherFile();
	std::string GetPatcherFile();
	DWORD GetCharset();
};

extern CServerInfo* g_ServerInfo;
int inet_ptonXP(int af, const char *src, void *dst);
char * inet_ntoaXP(struct in_addr ina);

#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


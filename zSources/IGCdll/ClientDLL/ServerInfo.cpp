//////////////////////////////////////////////////////////////////////
// ServerInfo.cpp
#include "stdafx.h"
#include "ServerInfo.h"
#include "EncDec.h"
#include "cryptopp\crc.h"
#include "Exception.h"
#include <fstream>
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma optimize("gpsy", on)

BYTE xorKey[4] = { 0xFF, 0xFA, 0x5, 0x88 };

CServerInfo* g_ServerInfo = new CServerInfo;

int inet_ptonXP(int af, const char *src, void *dst)
{
  struct sockaddr_storage ss;
  int size = sizeof(ss);
  char src_copy[INET6_ADDRSTRLEN+1];

  ZeroMemory(&ss, sizeof(ss));
  /* stupid non-const API */
  strncpy (src_copy, src, INET6_ADDRSTRLEN+1);
  src_copy[INET6_ADDRSTRLEN] = 0;

  if (WSAStringToAddress(src_copy, af, NULL, (struct sockaddr *)&ss, &size) == 0) {
    switch(af) {
      case AF_INET:
    *(struct in_addr *)dst = ((struct sockaddr_in *)&ss)->sin_addr;
    return 1;
      case AF_INET6:
    *(struct in6_addr *)dst = ((struct sockaddr_in6 *)&ss)->sin6_addr;
    return 1;
    }
  }
  return 0;
}

char * inet_ntoaXP(struct in_addr ina)
{
        static char buf[4*sizeof "123"];
        unsigned char *ucp = (unsigned char *)&ina;

        sprintf(buf, "%d.%d.%d.%d",
                ucp[0] & 0xff,
                ucp[1] & 0xff,
                ucp[2] & 0xff,
                ucp[3] & 0xff);
        return buf;
}

void CServerInfo::Load(std::string file)
{
	char ver[5];
	char szTemp[250];
	LPSTR filedec = "/ServerInfo.bmd";

	/*FILE* hFile = fopen(file.c_str(), "rb");
	if (!hFile)
	{
		MessageBoxA(0, "ServerInfo.bmd is missing", "ERROR", 0);
		ExitProcess(0);
	}

	fseek(hFile, 0, SEEK_END);
	int size = ftell(hFile);
	fseek(hFile, 0, SEEK_SET);

	BYTE* buf = new BYTE[size];
	fread(buf, 1, size, hFile);
	fclose(hFile);

	for (int i = 0; i < size; i++)
	{
		buf[i] ^= xorKey[i % 4];
	}

	std::ofstream decFile;
	decFile.open(filedec);
	decFile << buf;
	decFile.close();*/
	GetPrivateProfileStringA("Connection", "IP", "192.168.0.168", szTemp, 250, filedec);
	m_Ip = std::string(szTemp);
	m_Port = GetPrivateProfileInt("Connection", "Port", 44405, filedec);
	m_ChatServerPort = GetPrivateProfileIntA("Connection","ChatPort", 56960, filedec);
	GetPrivateProfileStringA("Main", "Version", "1.05.25", szTemp, 8, filedec);
	sscanf(szTemp, "%c.%c%c.%c%c", &ver[0], &ver[1], &ver[2], &ver[3], &ver[4]);
	for (int i = 0; i < 5; i++)
	{
		ver[i] = ver[i] + (i + 1);
	}
	memcpy(m_Version, ver, 5);

	GetPrivateProfileStringA("Main", "Serial", "PoweredByIGCN800", szTemp, 17, filedec);
	memcpy(m_Serial, szTemp, 16);
	m_UseLauncher = GetPrivateProfileIntA("Launcher", "Enabled", 0, filedec);
	GetPrivateProfileStringA("Launcher", "ExeName", "Launcher.exe", szTemp, 255, filedec);
	m_LauncherFile = std::string(szTemp);
	m_AllowXShop = GetPrivateProfileIntA("Gameplay", "SellXShopQuestItems", 0, filedec);
	m_Charset = GetPrivateProfileIntA("Gameplay", "CodePage", 0xFDE9, filedec);/** 1258vn0x4B0 0x4E4 */
	m_BlockVirtualMachine = GetPrivateProfileInt("Gameplay", "BlockVirtualMachine", 0, filedec);
	m_UsePatcher = GetPrivateProfileIntA("Patcher", "Enabled", 0, filedec);
	GetPrivateProfileStringA("Patcher", "ExeName", "Patcher.exe", szTemp, 255, filedec);
	m_PathcherFile = std::string(szTemp);
	
	/**
	WritePrivateProfileStringA("Connection", "IP", m_Ip,".\\Config.ini");
	   WritePrivateProfileIntA("Connection", "Port", m_Port,".\\Config.ini");
	   WritePrivateProfileIntA("Connection", "ChatPort", m_ChatServerPort,".\\Config.ini");
	WritePrivateProfileStringA("Main", "Version", m_Version,".\\Config.ini");
	WritePrivateProfileStringA("Main", "Serial", m_Serial,".\\Config.ini");
	   WritePrivateProfileIntA("Launcher", "Enabled", m_UseLauncher,".\\Config.ini");
	WritePrivateProfileStringA("Launcher", "ExeName", m_LauncherFile,".\\Config.ini");
	WritePrivateProfileStringA("Gameplay", "SellXShopQuestItems", m_AllowXShop,".\\Config.ini");
	   WritePrivateProfileIntA("Gameplay", "CodePage", m_Charset,".\\Config.ini");
	   WritePrivateProfileIntA("Gameplay", "BlockVirtualMachine", m_BlockVirtualMachine,".\\Config.ini");
	   WritePrivateProfileIntA("Patcher", "Enabled", m_UsePatcher,".\\Config.ini");
	WritePrivateProfileStringA("Patcher", "ExeName", m_PathcherFile,".\\Config.ini");
	
	*/
	DeleteFileA(filedec);
}

char* CServerInfo::GetIp()
{
	char Test[200];
	struct sockaddr_in sa;

    int result = inet_ptonXP(AF_INET, (char*)m_Ip.data(), &(sa.sin_addr));
	if(result == 0)
	{
		// Resolve DNS
		hostent * host = gethostbyname((char*)m_Ip.data());

		if(!host)
		{
			return (char*)m_Ip.data();
		}

		in_addr * address = (in_addr * )host->h_addr;
		char* ip_address = inet_ntoaXP(* address);
		return ip_address;
	}
	else
	{
		return (char*)m_Ip.data();
	}

}

WORD CServerInfo::GetPort()
{
	return m_Port;
}

char* CServerInfo::GetSerial()
{
	return m_Serial;
}

BYTE* CServerInfo::GetVersion()
{
	return m_Version;
}

bool CServerInfo::GetMulti()
{
	return m_AllowMulti;
}

DWORD CServerInfo::GetPassCrc()
{
	return m_PasswordCrc;
}

/*
CPrivateRsaKey* CServerInfo::GetPassKey()
{
	return m_PassKey;
}*/

bool CServerInfo::GetXShop()
{
	return m_AllowXShop;
}

bool CServerInfo::GetLauncher()
{
	return m_UseLauncher;
}

std::string CServerInfo::GetLauncherFile()
{
	return m_LauncherFile;
}

bool CServerInfo::GetPatcher()
{
	return m_UsePatcher;
}

std::string CServerInfo::GetPatcherFile()
{
	return m_PathcherFile;
}

DWORD CServerInfo::GetCharset()
{
	return m_Charset;
}

WORD CServerInfo::GetChatServerPort()
{
	return m_ChatServerPort;
}

bool CServerInfo::GetBlockVM()
{
	return m_BlockVirtualMachine;
}

#pragma optimize("gpsy", off)

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


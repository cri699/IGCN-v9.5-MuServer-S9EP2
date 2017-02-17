//////////////////////////////////////////////////////////////////////
// ServerData.h
#pragma once

struct SERVER_INFO
{
	char Name[50];
	char IP[16];
	WORD Port;
	WORD Code;
	bool Visible;
	WORD UserCount;
	WORD MaxUserCount;
	WORD Percent;
	DWORD TickCount;
	BYTE PlayType;
	bool Launcher;
	char FTPAddress[200];
	char HashCode[200];

	SERVER_INFO()
	{
		this->Port = 0;
		this->Code = 0;
		memset(IP, 0x00, sizeof(IP));
		memset(Name, 0x00, sizeof(Name));
		this->Visible = false;
		this->UserCount = 0;
		this->MaxUserCount = 0;
		this->Percent = 0;
		this->TickCount = 0;
		this->PlayType = 0;
	}
};

struct NEWS_INFO
{
	BYTE day;
	BYTE month;
	WORD year;

	// text color etc
	COLORREF dateColor;
	COLORREF titleColor;
	COLORREF textColor;

	char title[40];
	
	// text lenght
	WORD textLen;
	char Text[2048];
	bool Used;

	NEWS_INFO()
	{
		Used = false;
	}
	// rest of packet is text
	// lenght of packet is: header + structure + text
};

class CServerData
{
public:
	CServerData(void);
	virtual ~CServerData(void);

	void LoadServerFile(LPSTR lpszFile);
	void LoadNewsFile(LPSTR lpszFile);
	void Run();

	SERVER_INFO m_Servers[100];
	NEWS_INFO m_News[10];

	char szTitle[40];
};

extern CServerData m_ServerData;

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


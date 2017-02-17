//////////////////////////////////////////////////////////////////////
// ServerData.cpp
#include "StdAfx.h"
#include "ServerData.h"
#include "readscript.h"
#include "TLog.h"

CServerData m_ServerData;

CServerData::CServerData(void)
{
}

CServerData::~CServerData(void)
{
}

void CServerData::LoadServerFile(LPSTR lpszFile)
{
	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(lpszFile);

	if(res.status != pugi::status_ok)
	{
		g_Log.MsgBox("%s load fail (%s)", lpszFile, res.description());
		return;
	}

	pugi::xml_node main = file.child("ServerList");
	int count = 0;

	for(pugi::xml_node server = main.child("Server"); server; server = server.next_sibling())
	{
		this->m_Servers[count].MaxUserCount = 0;
		this->m_Servers[count].UserCount = 0;
		this->m_Servers[count].Percent = 0;
		this->m_Servers[count].TickCount = 0;

		this->m_Servers[count].Code = server.attribute("Code").as_int();
		strcpy(this->m_Servers[count].Name, server.attribute("Name").as_string());
		strcpy(this->m_Servers[count].IP, server.attribute("IP").as_string());
		this->m_Servers[count].Port = server.attribute("Port").as_int();
		this->m_Servers[count].Visible = server.attribute("Visible").as_int();

		g_Log.AddC(TColor::Green, "[Server Data] Loaded Server(s) info CODE: (%d), NAME: (%s), IP: (%s), PORT: (%d), VISIBLE: (%d), INDEX: (%d)",
			this->m_Servers[count].Code, this->m_Servers[count].Name, this->m_Servers[count].IP, this->m_Servers[count].Port, this->m_Servers[count].Visible, count);

		count++;
		

		if(count >= 100)
		{
			break;
		}
	}
}
					
void CServerData::LoadNewsFile(LPSTR lpszFile)
{
	SMDFile = fopen(lpszFile, "r");

	if(SMDFile == NULL)
	{
		g_Log.MsgBox("%s load fail", lpszFile);
		return;
	}

	int Token, Type, count = 0;

	int Index;
	// Date RGB
	BYTE dateR = 0;
	BYTE dateG = 0;
	BYTE dateB = 0;
	// Title RGB
	BYTE TitleR = 0;
	BYTE TitleG = 0;
	BYTE TitleB = 0;
	// Text RGB
	BYTE TextR = 0;
	BYTE TextG = 0;
	BYTE TextB = 0;
	//Date
	BYTE Day;
	BYTE Month;
	WORD Year;

	char szTitle[40] = {0};
	char szText[2048] = {0};

	while(true)
	{
		Token = GetToken();

		if(Token == END)
		{
			break;
		}

		if(Token == NUMBER)
		{
			Type = TokenNumber;

			if(Type == 0)
			{
				while(true)
				{
					Token = GetToken();
					if(strcmp(TokenString, "end") == 0)
						break;

					Index = TokenNumber;

					Token = GetToken();
					dateR = TokenNumber;

					Token = GetToken();
					dateG = TokenNumber;

					Token = GetToken();
					dateB = TokenNumber;
					
					Token = GetToken();
					TitleR = TokenNumber;

					Token = GetToken();
					TitleG = TokenNumber;

					Token = GetToken();
					TitleB = TokenNumber;

					Token = GetToken();
					TextR = TokenNumber;

					Token = GetToken();
					TextG = TokenNumber;

					Token = GetToken();
					TextB = TokenNumber;

					Token = GetToken();
					strcpy(szTitle,TokenString);

					Token = GetToken();
					strcpy(szText,TokenString);

					Token = GetToken();
					Day = TokenNumber;

					Token = GetToken();
					Month = TokenNumber;

					Token = GetToken();
					Year = TokenNumber;

					this->m_News[Index].dateColor = RGB(dateR,dateG,dateB);
					this->m_News[Index].titleColor = RGB(TitleR,TitleG,TitleB);
					this->m_News[Index].textColor = RGB(TextR,TextG,TextB);

					strcpy(this->m_News[Index].Text,szText);
					strcpy(this->m_News[Index].title,szTitle);
					this->m_News[Index].day = Day;
					this->m_News[Index].month = Month;
					this->m_News[Index].year = Year;
					this->m_News[Index].Used = true;
					count++;
				}
			}
			else if(Type == 1)
			{
				Token = GetToken();
				strcpy(this->szTitle, TokenString);
			}
		}
	}
}

void CServerData::Run()
{
	for(int i=0;i<100;i++)
	{
		if(GetTickCount() - this->m_Servers[i].TickCount >= 5000 && this->m_Servers[i].TickCount != 0)
		{
			this->m_Servers[i].TickCount = 0;
			this->m_Servers[i].MaxUserCount = 0;
			this->m_Servers[i].UserCount = 0;
			this->m_Servers[i].Percent = 0;
		}
	}
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


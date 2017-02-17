//////////////////////////////////////////////////////////////////////
// ProhibitedSymbols.cpp
#include "StdAfx.h"
#include "ProhibitedSymbols.h"
#include "TLog.h"
#include <regex>

CProhibitedSymbols g_prohibitedSymbols;

void CProhibitedSymbols::LoadSymbolFile(LPSTR szFile)
{
	this->m_ProhibitedSymbols.clear();
	_PROSYM_STRUCT data;

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(szFile);

	if (res.status != pugi::status_ok)
	{
		g_Log.MsgBox("%s file loading error - %s", szFile, res.description());
		return;
	}

	pugi::xml_node main_section = file.child("ProhibitedSymbols");
	this->m_nMode = main_section.attribute("Mode").as_int();
	pugi::xml_node symbol_section = main_section.child("Symbols");
	for (pugi::xml_node restrictsymbol = symbol_section.child("Restrict"); restrictsymbol; restrictsymbol = restrictsymbol.next_sibling())
	{
		DWORD dwNum = MultiByteToWideChar(CP_UTF8, 0, restrictsymbol.attribute("Symbol").as_string(), -1, NULL, 0);
		wchar_t* pwText;
		pwText = new wchar_t[dwNum];
		MultiByteToWideChar(CP_UTF8, 0, restrictsymbol.attribute("Symbol").as_string(), -1, pwText, dwNum);
		std::wstring wStr = pwText;
		data.system[0] = restrictsymbol.attribute("Chat").as_int();
		data.system[1] = restrictsymbol.attribute("CharName").as_int();
		data.system[2] = restrictsymbol.attribute("GuildName").as_int();

		this->m_ProhibitedSymbols.insert(std::pair<wchar_t, _PROSYM_STRUCT>(*wStr.c_str(), data));
	}
	pugi::xml_node regex_section = main_section.child("Regex");
	for (pugi::xml_node regextype = regex_section.child("Type"); regextype; regextype = regextype.next_sibling())
	{
		int nSystem = regextype.attribute("System").as_int();
		memcpy(this->m_sRegEx[nSystem].Regex, regextype.attribute("Regex").as_string(), 200);
		//this->regex[nSystem] = regextype.attribute("Regex").as_string();
	}
	g_Log.Add("Loaded %d prohibited symbols", this->m_ProhibitedSymbols.size());
	return;
}

bool CProhibitedSymbols::Validate(char* string, int len, BYTE Type)
{

	DWORD dwNum = MultiByteToWideChar(CP_UTF8, 0, string, -1, NULL, 0);
	char *psText;
	wchar_t *pwText;
	std::wstring wstr;
	if (!dwNum)
	{

	}
	else
	{
		
		pwText = new wchar_t[dwNum];
		MultiByteToWideChar(CP_UTF8, 0, string, -1, pwText, dwNum);

		dwNum = WideCharToMultiByte(CP_UTF8, 0, pwText, -1, NULL, 0, NULL, NULL);

		wstr = pwText;
		delete[] pwText;

	}
	

	if (this->m_nMode == 0)
	{
		for (int i = 0; i < wstr.length(); i++)
		{
			std::map<wchar_t, _PROSYM_STRUCT>::iterator it = this->m_ProhibitedSymbols.find(wstr.at(i)); // this is a problem right? yes xd
			if (it != this->m_ProhibitedSymbols.end() && it->second.system[Type] == 1)
			{
				return false;
			}
		}

		return true;
	}
	else
	{
		dwNum = MultiByteToWideChar(CP_UTF8, 0, this->m_sRegEx[Type].Regex, -1, NULL, 0);
		wchar_t *pwRegex;
		pwRegex = new wchar_t[dwNum];
		MultiByteToWideChar(CP_UTF8, 0, this->m_sRegEx[Type].Regex, -1, pwRegex, dwNum);
		std::wregex r(pwRegex);
		bool match = std::regex_match(wstr.c_str(), r);
		delete[] pwRegex;
		if (match)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


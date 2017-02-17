//////////////////////////////////////////////////////////////////////
// Lang.cpp
#include "stdafx.h"
#include "Lang.h"
#include "TLog.h"
#include "GameMain.h"
#include "configread.h"

CLanguage Lang;

void CLanguage::Init()
{
	this->m_LangData.Init();

	char * szFile = g_ConfigRead.GetPath("IGC_LangBase.xml"); 

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(szFile);

	if (res.status != pugi::status_ok)
	{
		g_Log.MsgBox("Error - %s file load failed! (%s)", szFile, res.description());
		ExitProcess(0);
	}

	pugi::xml_node main = file.child("LanguageSystem");
	this->m_LangData.DefaultLang = main.attribute("DefaultLang").as_int();

	for (pugi::xml_node lang = main.child("Lang"); lang; lang = lang.next_sibling())
	{
		Language m_Lang;
		
		m_Lang.LangID = lang.attribute("ID").as_int();
		m_Lang.Enable = lang.attribute("Enable").as_bool();
		m_Lang.Codepage = lang.attribute("Codepage").as_int();
		strcpy(m_Lang.FileName, lang.attribute("FileName").as_string());
		
		if (m_Lang.Enable == true)
		{
			if(this->LoadLanguage(m_Lang) == false)
			{
				g_Log.AddC(TColor::DarkOrange, "[Language] Load %s language failed", m_Lang.FileName);
				continue;
			}

			else
			{
				g_Log.AddC(TColor::DarkOrange, "[Language] Load %s language definition", m_Lang.FileName);
			}
		}

		this->m_LangData.m_mapLanguages.insert(std::pair<int, Language>(m_Lang.LangID, m_Lang));
	}
}

bool CLanguage::LoadLanguage(Language & Lang)
{
	pugi::xml_document file;
	char szTemp[256];

	sprintf(szTemp, "..\\IGCData\\Langs\\%s", Lang.FileName);
	pugi::xml_parse_result res = file.load_file(szTemp);

	if (res.status != pugi::status_ok)
	{
		g_Log.MsgBox("Error - %s file load failed! (%s)", szTemp, res.description());
		return false;
	}

	pugi::xml_node main = file.child("Language");
	pugi::xml_node message = main.child("Message");

	for (pugi::xml_node msg = message.child("Msg"); msg; msg = msg.next_sibling())
	{
		int iMsg = msg.attribute("ID").as_int();
		std::string szText = msg.attribute("Text").as_string();

		Lang.m_vtText.insert(std::pair<int, std::string>(iMsg, szText));
	}

	pugi::xml_node map = main.child("Map");

	for (pugi::xml_node msg = map.child("Msg"); msg; msg = msg.next_sibling())
	{
		int iMsg = msg.attribute("ID").as_int();
		std::string szText = msg.attribute("Text").as_string();

		Lang.m_vtMap.insert(std::pair<int, std::string>(iMsg, szText));
	}
}

char *CLanguage::GetText(int LangID, int TextID)
{
	int iLangID;

	if (LangID == 0)
	{
		iLangID = this->m_LangData.DefaultLang;
	}

	else
	{
		iLangID = LangID;
	}

	char szError[64];
	sprintf(szError, "NONE (%d)", TextID);

	std::map<int, Language>::iterator It = this->m_LangData.m_mapLanguages.find(iLangID);

	if (It == this->m_LangData.m_mapLanguages.end())
	{
		return szError;
	}

	try
	{
		return (char *)It->second.m_vtText.at(TextID).c_str();
	}

	catch (std::out_of_range err)
	{
		return szError;
	}
}

char *CLanguage::GetMap(int LangID, int MapID)
{
	int iLangID;

	if (LangID == 0)
	{
		iLangID = this->m_LangData.DefaultLang;
	}

	else
	{
		iLangID = LangID;
	}

	char szError[64];
	sprintf(szError, "NONE (%d)", MapID);

	std::map<int, Language>::iterator It = this->m_LangData.m_mapLanguages.find(iLangID);

	if (It == this->m_LangData.m_mapLanguages.end())
	{
		return szError;
	}

	try
	{
		return (char *)It->second.m_vtMap.at(MapID).c_str();
	}

	catch (std::out_of_range err)
	{
		return szError;
	}
}

WORD CLanguage::GetCodePage(int LangID)
{
	int iLangID;

	if (LangID == 0)
	{
		iLangID = this->m_LangData.DefaultLang;
	}

	else
	{
		iLangID = LangID;
	}

	std::map<int, Language>::iterator It = this->m_LangData.m_mapLanguages.find(iLangID);

	if (It == this->m_LangData.m_mapLanguages.end())
	{
		return 0;
	}

	return It->second.Codepage;

}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


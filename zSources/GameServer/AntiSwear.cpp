//////////////////////////////////////////////////////////////////////
// AntiSwear.cpp
#include "StdAfx.h"
#include "AntiSwear.h"
#include "TLog.h"

CAntiSwear SwearFilter;

CAntiSwear::CAntiSwear(void)
{
}

CAntiSwear::~CAntiSwear(void)
{
}

bool CAntiSwear::LoadFile(const char* File)
{
	this->m_SwearWords.clear();

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(File);

	if (res.status != pugi::status_ok)
	{
		g_Log.MsgBox("%s file load failed - %s", File, res.description());
		return false;
	}

	pugi::xml_node main_section = file.child("ProhibitedWords");
	std::string Swear;

	for (pugi::xml_node word = main_section.child("Restrict"); word; word = word.next_sibling())
	{
		Swear = word.attribute("Word").as_string();
		this->m_SwearWords.push_back(Swear);
	}

	g_Log.Add("Loaded %d prohibited words", this->m_SwearWords.size());
	return true;
}

bool CAntiSwear::CompareText(const char* Text)
{
	std::vector<std::string>::iterator It;
	for(It = this->m_SwearWords.begin(); It != this->m_SwearWords.end(); ++It)
	{
		if(strstr(Text, It->c_str()) != 0)
		{
			return true;
		}
	}
	return false;
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


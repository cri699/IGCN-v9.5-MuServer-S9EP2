//////////////////////////////////////////////////////////////////////
// NewsSystem.cpp
#include "stdafx.h"
#include "NewsSystem.h"
#include "TNotice.h"
#include "TLog.h"

CNewsSystem g_NewsSystem;

CNewsSystem::CNewsSystem(void)
{

}

CNewsSystem::~CNewsSystem(void)
{

}

bool CNewsSystem::LoadFile(LPSTR lpFile)
{
	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(lpFile);

	if(res.status != pugi::status_ok)
	{
		g_Log.MsgBox("%s load fail (%s)", lpFile, res.description());
		return false;
	}
	
	this->m_i64Tick = GetTickCount64();
	this->m_iMaxGUID = 0;
	this->m_NewsData.clear();

	NEWS_DATA m_News;

	pugi::xml_node main = file.child("NewsSystem");

	this->m_bEnable = main.attribute("Enable").as_int();
	this->m_iMinuteDelay = main.attribute("DisplayInterval").as_int();

	for (pugi::xml_node news = main.child("News"); news; news = news.next_sibling())
	{
		m_News.GUID = news.attribute("ID").as_int();

		for (pugi::xml_node line = news.child("Line"); line; line = line.next_sibling())
		{
			int iLine = line.attribute("Number").as_int();

			if (iLine < 0 || iLine > 2)
			{
				g_Log.MsgBox("NewsSystem: NewsID is WRONG - must be 0,1 or 2");
				continue;
			}

			m_News.News[iLine] = line.attribute("Text").as_string();
		}
		
		this->m_NewsData.push_back(m_News);
	}

	g_Log.Add("%d news load!", m_NewsData.size());
	return true;
}

void CNewsSystem::Run()
{
	if (this->m_bEnable == false)
	{
		return;
	}

	if((GetTickCount64() - m_i64Tick) < (this->m_iMinuteDelay*60000))
	{
		return;
	}

	PMSG_NOTICE pNotice;

	for(std::vector<NEWS_DATA>::iterator It = this->m_NewsData.begin(); It != this->m_NewsData.end(); It++)
	{
		if(It->GUID == this->m_iMaxGUID)
		{
			TNotice::MakeNoticeMsg(&pNotice, 0, (char *)It->News[0].c_str());
			TNotice::SendNoticeToAllUser(&pNotice);

			TNotice::MakeNoticeMsg(&pNotice, 0, (char *)It->News[1].c_str());
			TNotice::SendNoticeToAllUser(&pNotice);

			TNotice::MakeNoticeMsg(&pNotice, 0, (char *)It->News[2].c_str());
			TNotice::SendNoticeToAllUser(&pNotice);

			this->m_iMaxGUID++;
			this->m_i64Tick = GetTickCount64();
			
			if(this->m_iMaxGUID == this->m_NewsData.size())
				this->m_iMaxGUID = 0;

			return;
		}
	}

	this->m_iMaxGUID = 0;
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


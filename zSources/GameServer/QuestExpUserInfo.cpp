//////////////////////////////////////////////////////////////////////
// QuestExpUserInfo.cpp
#include "StdAfx.h"
#include "QuestExpUserInfo.h"
#include "TLog.h"

void UserQuestInfo::Init()
{
    this->m_iAskCnt = 0;
    this->m_iEp = 0;
    this->m_iQS = 0;
    this->m_wProgState = 0;
    this->m_lStartDate = 0;
    this->m_lEndDate = 0;
}

void UserQuestInfo::Clear()
{
	this->m_iAskCnt = 0;
	this->m_iEp = 0;
	this->m_iQS = 0;
	this->m_wProgState = 0;
	this->m_lStartDate = 0;
	this->m_lEndDate = 0;

	for (int i = 0; i < MAX_QUESTEXP_ASK_INFO; i++)
	{
		this->m_UserQuestAskInfo[i].Clear();
	}
}

void UserQuestInfo::QuestAskInfoClear()
{
	this->m_iAskCnt = 0;
	this->m_wProgState = 0;
	this->m_lStartDate = 0;
	this->m_lEndDate = 0;

	for (int i = 0; i < MAX_QUESTEXP_ASK_INFO; i++)
	{
		this->m_UserQuestAskInfo[i].Clear();
	}
}

bool UserQuestInfo::SetEpisode(int iEp)
{
	if (IsEpisode(iEp) == false)
	{
		g_Log.Add("[QuestExp] - Error - (UserQuestInfo::SetEpisode) EP[%d]", iEp);
		return false;
	}

	this->m_iEp = iEp;
	return true;
}

int UserQuestInfo::GetEpisode()
{
	return this->m_iEp;
}

bool UserQuestInfo::SetQuestSwitch(int iQS)
{
	if (IsQuestSwitch(iQS) == false)
	{
		g_Log.Add("[QuestExp] - Error - (UserQuestInfo::SetQuestSwitch) QS[%d]", iQS);
		return false;
	}

	this->m_iQS = iQS;
	return true;
}

int UserQuestInfo::GetQuestSwitch()
{
	return this->m_iQS;
}

void UserQuestInfo::SetAskCnt(int iAsk)
{
	if (IsQuestAskInfo(iAsk) == false)
	{
		g_Log.Add("[QuestExp] - Error - iAsk [%d] [%s] Line:[%d]", iAsk, __FILE__, __LINE__);
		return;
	}

	this->m_iAskCnt = iAsk;
}

int UserQuestInfo::GetAskCnt()
{
	if (IsQuestAskInfo(this->m_iAskCnt) == false)
	{
		g_Log.Add("[QuestExp] - Error - m_iAskCnt [%d] [%s] Line:[%d]", this->m_iAskCnt, __FILE__, __LINE__);
		return 0;
	}

	return this->m_iAskCnt;
}

void UserQuestInfo::SetStartDate(time_t iStartDate)
{
    this->m_lStartDate = iStartDate;
}

time_t UserQuestInfo::GetStartDate()
{
	return this->m_lStartDate;
}

void UserQuestInfo::SetEndDate(time_t iEndDate)
{
	this->m_lEndDate = iEndDate;
}

time_t UserQuestInfo::GetEndDate()
{
	return this->m_lEndDate;
}

void UserQuestInfo::SetQuestProgState(WORD wProgState)
{
	this->m_wProgState = wProgState;
}

WORD UserQuestInfo::GetQuestProgState()
{
	return this->m_wProgState;
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


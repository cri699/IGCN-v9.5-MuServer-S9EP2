//////////////////////////////////////////////////////////////////////
// QuestExpUserInfo.h
#ifndef QUESTEXPUSERINFO_H
#define QUESTEXPUSERINFO_H

#include "QuestExpDefine.h"

#pragma once

class UserQuestAskInfo
{
public:
    UserQuestAskInfo()
	{ 
		Init(); 
	}

    ~UserQuestAskInfo()
	{

	}

    void Init()
	{
		this->m_iQuestType = 0;
		this->m_bComplete = false;
		this->m_iValue = 0;
		this->m_iIndexID = 0;
	}

    void Clear()
	{
		this->m_iQuestType = 0;
		this->m_bComplete = false;
		this->m_iValue = 0;
		this->m_iIndexID = 0;
	}

    int GetQuestType() { return this->m_iQuestType; }
    void SetQuestType(int iQuestType) { this->m_iQuestType = iQuestType; }

    bool IsComplete() { return this->m_bComplete; }
    void SetComplete(bool bComplete) { this->m_bComplete = bComplete; }

    int GetValue() { return this->m_iValue; }
    void SetValue(int iValue) { this->m_iValue = iValue; }

    int GetIndexID() { return this->m_iIndexID; }
    void SetIndexID(int iIndexID) { this->m_iIndexID = iIndexID; }

protected:
	int m_iQuestType;
	bool m_bComplete;
	int m_iValue;
	int m_iIndexID;
};

class UserQuestReward
{
public:
	UserQuestReward()
	{
		this->m_iQuestRewardType = 0;
		this->m_iValue = 0;
		this->m_iIndexID = 0;
	}

	~UserQuestReward()
	{

	}

    int GetQuestRewardType() { return this->m_iQuestRewardType; }
    void SetQuestRewardType(int iQuestRewardType) { this->m_iQuestRewardType = iQuestRewardType; }

    int GetValue() { return this->m_iValue; }
    void SetValue(int iValue) { this->m_iValue = iValue; }

    int GetIndexID() { return this->m_iIndexID; }
    void SetIndexID(int iIndexID) { this->m_iIndexID = iIndexID; }

protected:
	int m_iQuestRewardType;
	int m_iValue;
	int m_iIndexID;
};

class UserQuestInfo
{
public:

	UserQuestInfo()
	{
		this->Init();
	}

	~UserQuestInfo()
	{

	}

	void Init();
	void Clear();
	void QuestAskInfoClear();

	bool SetEpisode(int iEp);
	int GetEpisode();

	bool SetQuestSwitch(int iQS);
	int GetQuestSwitch();

	void SetAskCnt(int iAskCnt);
	int GetAskCnt();

	void SetStartDate(time_t iStartDate);
	time_t GetStartDate();

	void SetEndDate(time_t iEndDate);
	time_t GetEndDate();

	void SetQuestProgState(WORD wProgState);
	WORD GetQuestProgState();

	UserQuestAskInfo		m_UserQuestAskInfo[MAX_QUESTEXP_ASK_INFO];

private:
	int						m_iEp;
	int						m_iQS;
	int						m_iAskCnt;
	WORD					m_wProgState;
	time_t					m_lStartDate;
	time_t					m_lEndDate;
};

#endif


//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


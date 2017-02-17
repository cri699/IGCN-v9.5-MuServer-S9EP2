//////////////////////////////////////////////////////////////////////
// MuunInfo.cpp
#include "stdafx.h"
#include "MuunInfo.h"

CMuunInfo::CMuunInfo()
{
	this->m_iOptIndex = -1;
}

CMuunInfo::~CMuunInfo()
{

}
void CMuunInfo::SetIndex(int iIndex)
{
	this->m_iIndex = iIndex;
}

int CMuunInfo::GetIndex()
{
	return this->m_iIndex;
}

void CMuunInfo::SetItemNum(int iItemNum)
{
	this->m_iItemNum = iItemNum;
}

int CMuunInfo::GetItemNum()
{
	return this->m_iItemNum;
}

void CMuunInfo::SetMuunType(int iMuunType)
{
	this->m_iMuunType = iMuunType;
}

int CMuunInfo::GetMuunType()
{
	return this->m_iMuunType;
}

void CMuunInfo::SetMuunRank(int iMuunRank)
{
	this->m_iMuunRank = iMuunRank;
}

int CMuunInfo::GetMuunRank()
{
	return this->m_iMuunRank;
}

void CMuunInfo::SetOptIndex(int iOptIndex)
{
	this->m_iOptIndex = iOptIndex;
}

int CMuunInfo::GetOptIndex()
{
	return this->m_iOptIndex;
}

void CMuunInfo::SetAddOptType(int iAddOptType)
{
	this->m_iAddOptType = iAddOptType;
}

int CMuunInfo::GetAddOptType()
{
	return this->m_iAddOptType;
}

void CMuunInfo::SetAddOptVal(int iAddOptVal)
{
	this->m_iAddOptVal = iAddOptVal;
}

int CMuunInfo::GetAddOptVal()
{
	return this->m_iAddOptVal;
}

void CMuunInfo::SetAddOptStart(time_t lAddOptStart)
{
	this->m_lAddOptStart = lAddOptStart;
}

time_t CMuunInfo::GetAddOptStart()
{
	return this->m_lAddOptStart;
}

void CMuunInfo::SetAddOptEnd(time_t lAddOptEnd)
{
	this->m_lAddOptEnd = lAddOptEnd;
}

time_t CMuunInfo::GetAddOptEnd()
{
	return this->m_lAddOptEnd;
}

void CMuunInfo::SetEvolutionMuunNum(int iEvolutionMuunNum)
{
	this->m_iEvolutionMuunNum = iEvolutionMuunNum;
}

int CMuunInfo::GetEvolutionMuunNum()
{
	return this->m_iEvolutionMuunNum;
}

void CMuunInfo::SetDelayTime(int iDelayTime)
{
	this->m_iDelayTime = iDelayTime;
}

int CMuunInfo::GetDelayTime()
{
	return this->m_iDelayTime;
}

void CMuunInfo::Clear()
{
	this->m_iIndex = 0;
	this->m_iItemNum = 0;
	this->m_iMuunType = 0;
	this->m_iMuunRank = 0;
	this->m_iOptIndex = 0;
	this->m_iAddOptType = 0;
	this->m_iAddOptVal = 0;
	this->m_lAddOptStart = 0;
	this->m_lAddOptEnd = 0;
	this->m_iEvolutionMuunNum = -1;
	this->m_iDelayTime = 0;
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


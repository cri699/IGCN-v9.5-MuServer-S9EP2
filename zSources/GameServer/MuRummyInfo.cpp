//////////////////////////////////////////////////////////////////////
// MuRummyInfo.cpp
#include "stdafx.h"
#include "MuRummyInfo.h"

CMuRummyInfo::CMuRummyInfo()
{
	this->Init();
}

CMuRummyInfo::~CMuRummyInfo() 
{
	if (this->m_pCCardInfo != NULL)
	{
		delete [] this->m_pCCardInfo;
	}

	if (this->m_pCSpecialCardInfo != NULL)
	{
		delete [] this->m_pCSpecialCardInfo;
	}

	if (this->m_pSlotCardIndex != NULL)
	{
		delete [] this->m_pSlotCardIndex;
	}

	if (this->m_pSlotStatus != NULL)
	{
		delete [] this->m_pSlotStatus;
	}
}

void CMuRummyInfo::Init()
{
	this->m_pCCardInfo = NULL;
	this->m_pCSpecialCardInfo = NULL;
	this->m_pSlotCardIndex = NULL;
	this->m_pSlotStatus = NULL;
	this->m_iScore = 0;
	this->m_iCardDeckCnt = 0;
	this->m_iSpecialCardCnt = 0;
	this->m_bPlayCard = false;
	this->m_bSpecialCardSet = false;
	this->m_bSpecialGame = false;
	this->m_bMuRummyInfoLoad = false;

	if (this->m_pCCardInfo == NULL)
	{
		this->m_pCCardInfo = new CMuRummyCardInfo[24];
	}

	if (this->m_pCSpecialCardInfo == NULL)
	{
		this->m_pCSpecialCardInfo = new CMuRummyCardInfo[3];
	}

	if (this->m_pSlotCardIndex == NULL)
	{
		this->m_pSlotCardIndex = new BYTE[10];
	}

	if (this->m_pSlotStatus == NULL)
	{
		this->m_pSlotStatus = new BYTE[10];
	}
}

void CMuRummyInfo::Clear()
{
	for (int i = 0; i < 24; i++)
	{
		this->m_pCCardInfo[i].SetColor(0);
		this->m_pCCardInfo[i].SetNumber(0);
		this->m_pCCardInfo[i].SetState(0);
		this->m_pCCardInfo[i].SetSlotNum(-1);
	}

	for (int i = 0; i < 3; i++)
	{
		this->m_pCSpecialCardInfo[i].SetColor(0);
		this->m_pCSpecialCardInfo[i].SetNumber(0);
		this->m_pCSpecialCardInfo[i].SetState(0);
		this->m_pCSpecialCardInfo[i].SetSlotNum(-1);
	}

	for (int i = 0; i < 10; i++)
	{
		this->m_pSlotCardIndex[i] = -1;
	}

	for (int i = 0; i < 10; i++)
	{
		this->m_pSlotStatus[i] = 0;
	}

	this->m_iScore = 0;
	this->m_iCardDeckCnt = 0;
	this->m_bPlayCard = false;
	this->m_bWaitReward = false;
	this->m_bSpecialGame = false;
	this->m_bSpecialCardSet = false;
	this->m_iSpecialCardCnt = 0;
}

CMuRummyCardInfo * CMuRummyInfo::GetCardInfo()
{
	if (this->m_pCCardInfo)
	{
		return this->m_pCCardInfo;
	}

	return NULL;
}

CMuRummyCardInfo * CMuRummyInfo::GetSpecialCardInfo()
{
	if (this->m_pCSpecialCardInfo)
	{
		return this->m_pCSpecialCardInfo;
	}

	return NULL;
}

BYTE * CMuRummyInfo::GetSlotIndexInfo()
{
	if (this->m_pSlotCardIndex)
	{
		return this->m_pSlotCardIndex;
	}

	return NULL;
}

BYTE * CMuRummyInfo::GetSlotStatus()
{
	if (this->m_pSlotStatus)
	{
		return this->m_pSlotStatus;
	}

	return NULL;
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


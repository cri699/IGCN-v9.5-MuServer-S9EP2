//////////////////////////////////////////////////////////////////////
// MuRummyInfo.h

#include "MuRummyCardInfo.h"

#pragma once
class CMuRummyInfo
{
public:
	CMuRummyInfo();
	virtual ~CMuRummyInfo();

	void Init();
	void Clear();

	CMuRummyCardInfo * GetCardInfo();
	CMuRummyCardInfo * GetSpecialCardInfo();
	BYTE * GetSlotIndexInfo();
	BYTE * GetSlotStatus();

	void SetScore(int iScore) { this->m_iScore = iScore; }
	void SetCardDeckCnt(int iCardDeckCnt) { this->m_iCardDeckCnt = iCardDeckCnt; }
	void SetSpecialCardCnt(int iSpecialCardCnt) { this->m_iSpecialCardCnt = iSpecialCardCnt; }
	void SetGetPlayCard(bool bPlayCard) { this->m_bPlayCard = bPlayCard; }
	void SetMuRummyInfoLoad(bool bMuRummyInfoLoad) { this->m_bMuRummyInfoLoad = bMuRummyInfoLoad; }
	void SetWaitReward(bool bWaitReward) { this->m_bWaitReward = bWaitReward; }
	void SetSpecialGame(bool bGame) { this->m_bSpecialGame = bGame; }
	void SetSpecialCardSet(bool bSet) { this->m_bSpecialCardSet = bSet; }
	int GetScore() { return this->m_iScore; }
	int GetCardDeckCnt() { return this->m_iCardDeckCnt; }
	int GetSpecialCardCnt() { return this->m_iSpecialCardCnt; }
	bool IsGetPlayCard() { return this->m_bPlayCard; }
	bool IsMuRummyInfoLoad() { return this->m_bMuRummyInfoLoad; }
	bool IsWaitReward() { return this->m_bWaitReward; }
	bool IsSpecialGame() { return this->m_bSpecialGame; }
	bool IsSpecialCardSet() { return this->m_bSpecialCardSet; }

private:
	int							m_iScore;
	int							m_iCardDeckCnt;
	int							m_iSpecialCardCnt;
	bool						m_bPlayCard;
	bool						m_bSpecialCardSet;
	bool						m_bMuRummyInfoLoad;
	bool						m_bWaitReward;
	bool						m_bSpecialGame;
	CMuRummyCardInfo*			m_pCCardInfo;
	CMuRummyCardInfo*			m_pCSpecialCardInfo;
	BYTE*						m_pSlotCardIndex;
	BYTE*						m_pSlotStatus;
};



//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


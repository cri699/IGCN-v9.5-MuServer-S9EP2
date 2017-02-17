//////////////////////////////////////////////////////////////////////
// MuRummyMng.h
#ifndef MURUMMYMNG_H
#define MURUMMYMNG_H

#include "MuRummyInfo.h"
#include "protocol.h"

struct _tagMuRummyCardInfo
{
	_tagMuRummyCardInfo()
	{
		this->btColor = -1;
		this->btNumber = -1;
		this->btSlotNum = -1;
	}

	BYTE btColor;
	BYTE btNumber;
	BYTE btSlotNum;
};

struct _tagPMSG_REQ_MURUMMY_EVENT_OPEN
{
	PBMSG_HEAD2 h;
};

struct _tagPMSG_ANS_MURUMMY_EVENT_OPEN
{
	PBMSG_HEAD2 h;
	BYTE btResult;
	BYTE btEventTime1;
	BYTE btEventTime2;
	BYTE btEventTime3;
	BYTE btEventTime4;
};

struct _tagPMSG_REQ_MURUMMY_INFO
{
	PBMSG_HEAD2 h;
	BYTE btGameType;
};

struct _tagPMSG_ANS_MURUMMY_INFO
{
	_tagPMSG_ANS_MURUMMY_INFO()
	{
		this->btTotScoreH = 0;
		this->btTotScoreL = 0;
		this->btCardDeckCnt = 0;
		this->SpecialCardCnt = 0;
		this->btIsSpecialCard = FALSE;
		memset(this->btSlotStatus, 0x00, sizeof(this->btSlotStatus));
	}

	PBMSG_HEAD2 h;
	BYTE btTotScoreH;
	BYTE btTotScoreL;
	BYTE btCardDeckCnt;
	BYTE SpecialCardCnt;
	BYTE btUnk;
	BYTE btIsSpecialCard;
	BYTE btSlotStatus[10];
	_tagMuRummyCardInfo stMuRummyCardInfo[6];
};

struct _tagPMSG_REQ_REVEAL_CARD
{
	PBMSG_HEAD2 h;
};

struct _tagPMSG_ANS_REVEAL_CARD
{
	PBMSG_HEAD2 h;
	_tagMuRummyCardInfo stMuRummyCardInfo[6];
	BYTE btCardDeckCnt;
};

struct _tagPMSG_REQ_CARD_MOVE
{
	PBMSG_HEAD2 h;
	BYTE sSlot;
	BYTE tSlot;
};

struct _tagPMSG_ANS_CARD_MOVE
{
	PBMSG_HEAD2 h;
	BYTE sSlot;
	BYTE tSlot;
	BYTE btColor;
	BYTE btNumber;
};

struct _tagPMSG_REQ_CARD_REMOVE
{
	PBMSG_HEAD2 h;
	BYTE btSlot;
};

struct _tagPMSG_ANS_CARD_REMOVE
{
	PBMSG_HEAD2 h;
	BYTE btResult;
};

struct _tagPMSG_REQ_CARD_MATCH
{
	PBMSG_HEAD2 h;
};

struct _tagMuRummyCardUpdateDS
{
	_tagMuRummyCardUpdateDS()
	{
		this->btSlotNum = -1;
		this->btSeq = -1;
		this->btStatus = -1;
	}

	BYTE btSlotNum;
	BYTE btSeq;
	BYTE btStatus;
};

struct _tagPMSG_ANS_CARD_MATCH
{
	PBMSG_HEAD2 h;
	BYTE btScoreH;
	BYTE btScoreL;
	BYTE btTotScoreH;
	BYTE btTotScoreL;
	BYTE btResult;
};

struct _tagPMSG_REQ_MURUMMY_END
{
	PBMSG_HEAD2 h;
};

struct _tagPMSG_ANS_MURUMMY_END
{
	PBMSG_HEAD2 h;
	BYTE btResult;
};

struct _tagPMSG_SEND_SLOTCARD_LIST
{
	PBMSG_HEAD2 h;
	_tagMuRummyCardInfo stMuRummyCardInfo[6];
};

struct _tagPMSG_SEND_MURUMMY_MSG
{
	PBMSG_HEAD2 h;
	BYTE btNotiIndex;
	BYTE btValueH;
	BYTE btValueL;
};

struct _tagPMSG_REQ_MURUMMY_SELECT_DS
{
	PBMSG_HEAD2 h;
	char AccountID[11];
	char Name[11];
	WORD aIndex;
};

struct _tagMuRummyCardInfoDS
{
	_tagMuRummyCardInfoDS()
	{
		this->btColor = -1;
		this->btNumber = -1;
		this->btSlotNum = -1;
		this->btSeq = -1;
		this->btStatus = -1;
	}

	BYTE btColor;
	BYTE btNumber;
	BYTE btSlotNum;
	BYTE btSeq;
	BYTE btStatus;
};

struct _tagPMSG_ANS_MURUMMY_SELECT_DS
{
	PBMSG_HEAD2 h;
	WORD aIndex;
	WORD wScore;
	BYTE btResult;
	_tagMuRummyCardInfoDS stMuRummyCardInfoDS[24];
};

struct _tagPMSG_REQ_MURUMMY_INSERT_DS
{
	PBMSG_HEAD2 h;
	char AccountID[11];
	char Name[11];
	WORD aIndex;
	_tagMuRummyCardInfoDS stMuRummyCardInfoDS[24];
};

struct _tagPMSG_REQ_MURUMMY_SCORE_UPDATE_DS
{
	PBMSG_HEAD2 h;
	char AccountID[11];
	char Name[11];
	WORD wScore;
	_tagMuRummyCardUpdateDS stCardUpdateDS[3];
};

struct _tagPMSG_REQ_MURUMMY_UPDATE_DS
{
	PBMSG_HEAD2 h;
	char AccountID[11];
	char Name[11];
	BYTE btSlotNum;
	BYTE btStatus;
	BYTE btSequence;
};

struct _tagPMSG_REQ_MURUMMY_DELETE_DS
{
	PBMSG_HEAD2 h;
	char AccountID[11];
	char Name[11];
};

struct _tagPMSG_REQ_MURUMMY_SLOTUPDATE_DS
{
	PBMSG_HEAD2 h;
	char AccountID[11];
	char Name[11];
	_tagMuRummyCardUpdateDS stCardUpdateDS;
};

struct _tagPMSG_REQ_MURUMMY_INFO_UPDATE_DS
{
	PBMSG_HEAD2 h;
	char AccountID[11];
	char Name[11];
	WORD wScore;
	_tagMuRummyCardUpdateDS stMuRummyCardUpdateDS[24];
};

struct _tagPMSG_REQ_MURUMMY_LOG_INSERT_DS
{
	PBMSG_HEAD2 h;
	char AccountID[11];
	char Name[11];
	WORD wScore;
};

#pragma once
class CMuRummyMng
{
public:
	CMuRummyMng();
	virtual ~CMuRummyMng();

	bool IsMuRummyEventOn() { return this->m_bMuRummyEventOn; }
	int GetMuRummyEventItemDropRate() { return this->m_iMuRummyEventItemDropRate; }

	void LoadPreFixData(char *lpszFileName);

	int GetRewardScore1() { return this->m_iRewardScore1; }
	int GetRewardScore2() { return this->m_iRewardScore2; }

	void SetCardDeck(CMuRummyInfo *pMuRummyInfo);
	void SetSpecialCard(CMuRummyInfo *pMuRummyInfo);
	void SetSlotStatus(CMuRummyInfo *pMuRummyInfo);
	void CardShuffle(CMuRummyCardInfo *pCCardInfo);
	bool SetPlayCardInfo(CMuRummyInfo *pMuRummyInfo, _tagMuRummyCardInfo *pOutCardInfo);
	bool FillEmptySlotCard(CMuRummyInfo *pMuRummyInfo, _tagMuRummyCardInfo *pOutCardInfo, OBJECTSTRUCT *lpObj);
	BYTE GetTempCardShowSlot(BYTE *pSlotCardIndex);
	BYTE CardSlotMove(CMuRummyInfo *pCMuRummyInfo, int sSlot, int tSlot, CMuRummyCardInfo *pCOutCardInfo, OBJECTSTRUCT *lpObj);
	BYTE CardSlotReMove(CMuRummyInfo *pCMuRummyInfo, int sSlot);
	BYTE CardMatchCheck(CMuRummyInfo *pCMuRummyInfo, int *iOutScore, _tagMuRummyCardUpdateDS *pOutCardUpdateDS, OBJECTSTRUCT *lpObj);
	void CardSortAscending(CMuRummyCardInfo *pCCardInfo);
	int IsCardSameColor(CMuRummyCardInfo *pCCardInfo);
	int IsCardNumber(CMuRummyCardInfo *pCCardInfo);
	BYTE GetColorName(int iColorNum);

	void CGReqMuRummyEventOpen(_tagPMSG_REQ_MURUMMY_EVENT_OPEN *lpMsg, int aIndex);
	void CGReqMuRummyStart(_tagPMSG_REQ_MURUMMY_INFO *lpMsg, int aIndex);
	void CGReqCardReveal(_tagPMSG_REQ_REVEAL_CARD *lpMsg, int aIndex);
	void CGReqCardMove(_tagPMSG_REQ_CARD_MOVE *lpMsg, int aIndex);
	void CGReqCardReMove(_tagPMSG_REQ_CARD_REMOVE *lpMsg, int aIndex);
	void CGReqCardMatch(_tagPMSG_REQ_CARD_MATCH *lpMsg, int aIndex);
	void CGReqMuRummyEnd(_tagPMSG_REQ_MURUMMY_END *lpMsg, int aIndex);

	void GCSendCardList(int aIndex);
	void GCSendMsg(int aIndex, BYTE btNotiIndex, int iValue);

	void CheatSetCardColorAsc(CMuRummyInfo *pMuRummyInfo, int aIndex);
	void CheatSetCardNumAsc(CMuRummyInfo *pMuRummyInfo, int aIndex);
	void CheatSetScore(CMuRummyInfo *pMuRummyInfo, int iScore, int aIndex);

	void GDReqCardInfo(OBJECTSTRUCT *lpObj);
	void GDAnsCardInfo(_tagPMSG_ANS_MURUMMY_SELECT_DS *lpMsg);
	void GDReqCardInfoInsert(OBJECTSTRUCT *lpObj);
	void GDReqScoreUpdate(OBJECTSTRUCT *lpObj, WORD wScore, _tagMuRummyCardUpdateDS *pCardUpdateDS);
	void GDReqCardInfoUpdate(OBJECTSTRUCT *lpObj, CMuRummyCardInfo *pCCardInfo, int iSeq);
	void GDReqScoreDelete(OBJECTSTRUCT *lpObj);
	void GDReqSlotInfoUpdate(OBJECTSTRUCT *lpObj, BYTE btSeq, BYTE btSlotNum, BYTE btStatus);
	void GDReqMuRummyInfoUpdate(OBJECTSTRUCT *lpObj);
	void GDReqMuRummyDBLog(OBJECTSTRUCT *lpObj, int iScore);

	void Slot3Log(OBJECTSTRUCT *lpObj, int iLogType);
	void Slot5Log(OBJECTSTRUCT *lpObj, int iLogType);

private:
	int m_iRewardScore1;
	int m_iRewardScore2;
	bool m_bMuRummyEventOn;
	int m_iMuRummyEventItemDropRate;
	bool m_bMuRummyEventDBLog;
};

extern CMuRummyMng g_CMuRummyMng;

#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


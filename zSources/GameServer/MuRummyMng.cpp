//////////////////////////////////////////////////////////////////////
// MuRummyMng.cpp
#include "stdafx.h"
#include "MuRummyMng.h"
#include "LargeRand.h"
#include "GameMain.h"
#include "user.h"
#include "winutil.h"
#include "BagManager.h"
#include "DSProtocol.h"
#include "TLog.h"
#include "IniReader.h"
#include "configread.h"

CMuRummyMng g_CMuRummyMng;

CMuRummyMng::CMuRummyMng()
{
	this->m_iRewardScore1 = 0;
	this->m_iRewardScore2 = 0;
}

CMuRummyMng::~CMuRummyMng()
{
}

void CMuRummyMng::LoadPreFixData(char *lpszFileName)
{
	CIniReader ReadMuRummy(lpszFileName);

	this->m_bMuRummyEventOn = ReadMuRummy.ReadInt("MuRummy", "EventEnable", 0);
	this->m_iMuRummyEventItemDropRate = ReadMuRummy.ReadInt("MuRummy", "CardItemDropRate", 0);
	this->m_bMuRummyEventDBLog = ReadMuRummy.ReadInt("MuRummy", "IsSaveLogInDatabase", 0);
	this->m_iRewardScore1 = ReadMuRummy.ReadInt("MuRummy", "RewardScore_First", 0);
	this->m_iRewardScore2 = ReadMuRummy.ReadInt("MuRummy", "RewardScore_Second", 0);

	if (this->m_iRewardScore1 <= 0 || this->m_iRewardScore2 <= 0)
	{
		if (this->IsMuRummyEventOn() == true)
		{
			g_Log.MsgBox("MuRummy Error - Check Score configuration (can't be zero)");
		}
	}
}

void CMuRummyMng::SetCardDeck(CMuRummyInfo * pMuRummyInfo)
{
	int nCardArr = 0;

	if (!pMuRummyInfo)
	{
		return;
	}

	CMuRummyCardInfo * pCCardInfo = pMuRummyInfo->GetCardInfo();

	if (!pCCardInfo)
	{
		return;
	}

	for (int nCardColor = 0; nCardColor < 3; nCardColor++)
	{
		for (int nCardNum = 0; nCardNum < 8; nCardNum++)
		{
			if (nCardArr < 0 || nCardArr > 23)
			{
				return;
			}

			pCCardInfo[nCardArr].SetColor(nCardColor + 1);
			pCCardInfo[nCardArr].SetNumber(nCardNum + 1);
			pCCardInfo[nCardArr].SetState(0);
			pCCardInfo[nCardArr].SetSlotNum(0);

			nCardArr++;
		}
	}

	BYTE * pSlotIndexInfo = pMuRummyInfo->GetSlotIndexInfo();

	if (!pSlotIndexInfo)
	{
		return;
	}

	for (int i = 0; i < 10; i++)
	{
		pSlotIndexInfo[i] = -1;
	}
	
	pMuRummyInfo->SetCardDeckCnt(24);
}

void CMuRummyMng::SetSpecialCard(CMuRummyInfo *pMuRummyInfo)
{
	if (!pMuRummyInfo)
	{
		return;
	}

	if (pMuRummyInfo->IsSpecialGame() == false)
	{
		pMuRummyInfo->SetSpecialCardCnt(0);
		return;
	}

	CMuRummyCardInfo * pCSpecialCardInfo = pMuRummyInfo->GetSpecialCardInfo();

	for (int i = 0; i < 3; i++)
	{
		pCSpecialCardInfo[i].SetColor(4);
		pCSpecialCardInfo[i].SetNumber(1 + rand()%11);
		pCSpecialCardInfo[i].SetSlotNum(0);
		pCSpecialCardInfo[i].SetState(0);
	}

	pMuRummyInfo->SetSpecialCardCnt(3);
}

void CMuRummyMng::SetSlotStatus(CMuRummyInfo *pMuRummyInfo)
{
	if (!pMuRummyInfo)
	{
		return;
	}

	BYTE * pSlotStatus = pMuRummyInfo->GetSlotStatus();

	if (!pSlotStatus)
	{
		return;
	}

	for (int i = 0; i < 8; i++)
	{
		pSlotStatus[i] = 0;
	}

	if (pMuRummyInfo->IsSpecialGame() == true)
	{
		pSlotStatus[8] = 0;
		pSlotStatus[9] = 0;
	}

	else
	{
		pSlotStatus[8] = 1;
		pSlotStatus[9] = 1;
	}
}

void CMuRummyMng::CardShuffle(CMuRummyCardInfo *pCCardInfo)
{
	CMuRummyCardInfo CardInfoTmp;
	CMuRummyCardInfo *pCardInfo_1;
	CMuRummyCardInfo *pCardInfo_2;
	CMuRummyCardInfo *pCardInfo_3;
	CMuRummyCardInfo *pCardInfo_4;

	for (int nCnt = 0; nCnt < 24; nCnt++)
	{
		int iIndex = nCnt + GetLargeRand() % (24 - nCnt);

		if (iIndex < 0 || iIndex > 23)
		{
			return;
		}

		pCardInfo_1 = &pCCardInfo[nCnt];
		CardInfoTmp.m_iColor = pCardInfo_1->m_iColor;
		CardInfoTmp.m_iNumber = pCardInfo_1->m_iNumber;
		CardInfoTmp.m_iState = pCardInfo_1->m_iState;
		CardInfoTmp.m_iSlotNum = pCardInfo_1->m_iSlotNum;

		pCardInfo_2 = &pCCardInfo[iIndex];
		pCardInfo_3 = &pCCardInfo[nCnt];

		pCardInfo_3->m_iColor = pCardInfo_2->m_iColor;
		pCardInfo_3->m_iNumber = pCardInfo_2->m_iNumber;
		pCardInfo_3->m_iState = pCardInfo_2->m_iState;
		pCardInfo_3->m_iSlotNum = pCardInfo_2->m_iSlotNum;

		pCardInfo_4 = &pCCardInfo[iIndex];

		pCardInfo_4->m_iColor = CardInfoTmp.m_iColor;
		pCardInfo_4->m_iNumber = CardInfoTmp.m_iNumber;
		pCardInfo_4->m_iState = CardInfoTmp.m_iState;
		pCardInfo_4->m_iSlotNum = CardInfoTmp.m_iSlotNum;
	}
}

bool CMuRummyMng::SetPlayCardInfo(CMuRummyInfo *pMuRummyInfo, _tagMuRummyCardInfo *pOutCardInfo)
{
	int nCardCnt = 0;

	if (!pMuRummyInfo)
	{
		return false;
	}

	CMuRummyCardInfo *pCCardInfo = pMuRummyInfo->GetCardInfo();

	if (!pCCardInfo)
	{
		g_Log.Add("[MuRummy][Error] pCCardInfo is NULL [%s, %d]", __FILE__, __LINE__);
		return false;
	}

	BYTE* pSlotIndexInfo = pMuRummyInfo->GetSlotIndexInfo();

	if (!pSlotIndexInfo)
	{
		g_Log.Add("[MuRummy][Error] pSlotIndexInfo is NULL [%s, %d]", __FILE__, __LINE__);
		return false;
	}

	for (int nCnt = 0; nCnt < 10; nCnt++)
	{
		int nCardArr = pSlotIndexInfo[nCnt];

		if (nCardArr >= 0 && nCardArr <= 23)
		{
			if (pCCardInfo[nCardArr].GetState() == 1)
			{
				if (nCardCnt < 0 || nCardCnt > 4)
				{
					return false;
				}

				pOutCardInfo[nCardCnt].btColor = pCCardInfo[nCardArr].GetColor();
				pOutCardInfo[nCardCnt].btNumber = pCCardInfo[nCardArr].GetNumber();
				pOutCardInfo[nCardCnt].btSlotNum = pCCardInfo[nCardArr].GetSlotNum();

				nCardCnt++;
			}
		}
	}

	return true;
}

void CMuRummyMng::CGReqMuRummyEventOpen(_tagPMSG_REQ_MURUMMY_EVENT_OPEN *lpMsg, int aIndex)
{
	_tagPMSG_ANS_MURUMMY_EVENT_OPEN pMsg;

	pMsg.btResult = -1;
	PHeadSubSetB((LPBYTE)&pMsg, 0x4D, 0x0F, sizeof(pMsg));

	if (ObjectMaxRange(aIndex) == false)
	{
		return;
	}

	if (!gObjIsConnected(aIndex))
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->CloseCount != -1)
	{
		return;
	}

	if (lpObj->m_IfState.use && (
		lpObj->m_IfState.type == 6 ||
		lpObj->m_IfState.type == 3 ||
		lpObj->m_IfState.type == 12))
	{
		g_Log.Add("[MuRummy][Error] [%s][%s] : not event time", lpObj->AccountID, lpObj->Name);
		return;
	}

	if (lpObj->m_bPShopOpen == true || lpObj->ChaosLock == TRUE)
	{
		g_Log.Add("[MuRummy][Error] [%s][%s] : not event time", lpObj->AccountID, lpObj->Name);
		return;
	}

	CMuRummyInfo * pMuRummyInfo = lpObj->m_PlayerData->m_pCMuRummyInfo;

	if (!pMuRummyInfo)
	{
		g_Log.Add("[MuRummy][Error] pMuRummyInfo is NULL [%s][%s] [%s, %d]",
			lpObj->AccountID, lpObj->Name, __FILE__, __LINE__);
		return;
	}

	if (pMuRummyInfo->IsWaitReward() == true)
	{
		g_Log.Add("[MuRummy][DebugLog][%s][%s] Waiting Item Reward(Req Open)", lpObj->AccountID, lpObj->Name);
		GSProtocol.GCServerMsgStringSend(Lang.GetText(0,561), aIndex, 1);
		return;
	}

	if (this->IsMuRummyEventOn() == true)
	{
		time_t tmEndTime;
		DWORD dw32EndTime;
		SystemTimeToUnixTime(&g_ConfigRead.data.common.EventEndDate, &tmEndTime);
		dw32EndTime = (DWORD)tmEndTime;

		pMsg.btEventTime1 = SET_NUMBERH(SET_NUMBERHW(dw32EndTime));
		pMsg.btEventTime2 = SET_NUMBERL(SET_NUMBERHW(dw32EndTime));
		pMsg.btEventTime3 = SET_NUMBERH(SET_NUMBERLW(dw32EndTime));
		pMsg.btEventTime4 = SET_NUMBERL(SET_NUMBERLW(dw32EndTime));

		if (pMuRummyInfo->IsGetPlayCard() == true)
		{
			pMsg.btResult = 2;
			IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
			this->CGReqMuRummyStart(0, aIndex);
		}

		else
		{
			pMsg.btResult = 1;
			IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
		}

		g_Log.Add("[MuRummy] [%s][%s] : Mini Game Open", lpObj->AccountID, lpObj->Name);
	}
}

void CMuRummyMng::CGReqMuRummyStart(_tagPMSG_REQ_MURUMMY_INFO *lpMsg, int aIndex)
{
	_tagPMSG_ANS_MURUMMY_INFO pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0x4D, 0x10, sizeof(pMsg));

	if (ObjectMaxRange(aIndex) == false)
	{
		return;
	}

	if (!gObjIsConnected(aIndex))
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if (!lpObj)
	{
		g_Log.Add("[MuRummy][Error] lpObj is NULL [%s, %d]", __FILE__, __LINE__);
		return;
	}

	if (lpObj->CloseType != -1)
	{
		return;
	}

	CMuRummyInfo *pMuRummyInfo = lpObj->m_PlayerData->m_pCMuRummyInfo;

	if (!pMuRummyInfo)
	{
		g_Log.Add("[MuRummy][Error] pMuRummyInfo is NULL [%s][%s] [%s, %d]", lpObj->AccountID, lpObj->Name, __FILE__, __LINE__);
		return;
	}

	if (!pMuRummyInfo->IsMuRummyInfoLoad())
	{
		g_Log.Add("[MuRummy][Error] IsMuRummyInfoLoad fail [%s][%s]", lpObj->AccountID, lpObj->Name);
		return;
	}

	_tagMuRummyCardInfo stCardInfo[5];
	bool bNewGame = false;

	if (pMuRummyInfo->IsGetPlayCard() == true)
	{
		if (this->SetPlayCardInfo(pMuRummyInfo, stCardInfo) == true)
		{
			g_Log.Add("[%s][%s][Mu Rummy] Event Open", lpObj->AccountID, lpObj->Name);
			g_Log.Add("[%s][%s][Mu Rummy] Event Card Cnt(%d)", lpObj->AccountID, lpObj->Name, pMuRummyInfo->GetCardDeckCnt());
			g_Log.Add("[%s][%s][Mu Rummy] Event Point(%d)", lpObj->AccountID, lpObj->Name, pMuRummyInfo->GetScore());

			this->Slot5Log(lpObj, 1);
			this->Slot3Log(lpObj, 1);
			memcpy(pMsg.stMuRummyCardInfo, stCardInfo, sizeof(stCardInfo));
			memcpy(pMsg.btSlotStatus, pMuRummyInfo->GetSlotStatus(), sizeof(pMsg.btSlotStatus));
		}

		else
		{
			g_Log.Add("[MuRummy][Error] SetPlayCardInfo fail [%s][%s]", lpObj->AccountID, lpObj->Name);
		}
	}

	else
	{
		if (lpMsg->btGameType == 1)
		{
			MsgOutput(aIndex, Lang.GetText(0,13));
			return;
		}

		bool bItem = false;

		for (int x = 0; x < EVENT_INVENTORY_SIZE; x++)
		{
			if (lpObj->pEventInventory[x].IsItem() == TRUE && lpObj->pEventInventory[x].m_Type == ITEMGET(14, 216))
			{
				g_Log.Add("[%s][%s][Mu Rummy] Event Start", lpObj->AccountID, lpObj->Name);
				g_Log.Add("[%s][%s][Mu Rummy] Event Card register success (%I64d)", lpObj->AccountID, lpObj->Name, lpObj->pEventInventory[x].m_Number);
				gObjEventInventoryDeleteItem(aIndex, x);
				GSProtocol.GCEventInventoryItemDeleteSend(aIndex, x, 1);
				bItem = true;
				break;
			}
		}

		if (!bItem)
		{
			g_Log.Add("[%s][%s][Mu Rummy] Event Start Fail (0)", lpObj->AccountID, lpObj->Name);
			this->GCSendMsg(aIndex, 0, 0);
			return;
		}

		pMuRummyInfo->Clear();
		pMuRummyInfo->SetGetPlayCard(true);
		pMuRummyInfo->SetSpecialGame(false);
		this->SetCardDeck(pMuRummyInfo);
		this->SetSlotStatus(pMuRummyInfo);
		this->CardShuffle(pMuRummyInfo->GetCardInfo());
		this->GDReqCardInfoInsert(lpObj);
		this->SetPlayCardInfo(pMuRummyInfo, stCardInfo);
		memcpy(pMsg.stMuRummyCardInfo, stCardInfo, sizeof(stCardInfo));
		memcpy(pMsg.btSlotStatus, pMuRummyInfo->GetSlotStatus(), sizeof(pMsg.btSlotStatus));
		bNewGame = true;
	}

	pMsg.SpecialCardCnt = 0;
	pMsg.btIsSpecialCard = 0;
	pMsg.btCardDeckCnt = pMuRummyInfo->GetCardDeckCnt();
	pMsg.btTotScoreH = HIBYTE(pMuRummyInfo->GetScore());
	pMsg.btTotScoreL = LOBYTE(pMuRummyInfo->GetScore());
	IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);

	if (bNewGame == true)
	{
		this->GCSendMsg(aIndex, 1, 0);
	}
}

void CMuRummyMng::CGReqCardReveal(_tagPMSG_REQ_REVEAL_CARD *lpMsg, int aIndex)
{
	_tagPMSG_ANS_REVEAL_CARD pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0x4D, 0x11, sizeof(pMsg));

	if (ObjectMaxRange(aIndex) == false)
	{
		return;
	}

	if (!gObjIsConnected(aIndex))
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if (!lpObj)
	{
		g_Log.Add("[MuRummy][Error] lpObj is NULL [%s, %d]", __FILE__, __LINE__);
		return;
	}

	if (lpObj->CloseType != -1)
	{
		return;
	}

	CMuRummyInfo * pMuRummyInfo = lpObj->m_PlayerData->m_pCMuRummyInfo;

	if (!pMuRummyInfo)
	{
		g_Log.Add("[MuRummy][Error] pMuRummyInfo is NULL [%s][%s] [%s, %d]", lpObj->AccountID, lpObj->Name, __FILE__, __LINE__);
		return;
	}

	CMuRummyCardInfo * pCCardInfo = pMuRummyInfo->GetCardInfo();

	if (!pCCardInfo)
	{
		g_Log.Add("[MuRummy][Error] pCCardInfo is NULL [%s][%s] [%s, %d]", lpObj->AccountID, lpObj->Name, __FILE__, __LINE__);
		return;
	}

	if (!pMuRummyInfo->GetCardDeckCnt())
	{
		this->GCSendMsg(aIndex, 2, 0);
		return;
	}

	BYTE *pSlotIndexInfo = pMuRummyInfo->GetSlotIndexInfo();

	if (!pSlotIndexInfo)
	{
		g_Log.Add("[MuRummy][Error] pSlotIndexInfo is NULL [%s][%s] [%s, %d]", lpObj->AccountID, lpObj->Name, __FILE__, __LINE__);
		return;
	}

	BYTE btSlotNum = this->GetTempCardShowSlot(pSlotIndexInfo);

	if (btSlotNum == 255)
	{
		this->GCSendMsg(aIndex, 2, 0);
	}

	else if (btSlotNum == 254)
	{
		this->GCSendMsg(aIndex, 3, 0);
	}

	else
	{
		_tagMuRummyCardInfo stCardInfo[5];
		int iBeforeCardCnt = pMuRummyInfo->GetCardDeckCnt();
		bool bRet = this->FillEmptySlotCard(pMuRummyInfo, stCardInfo, lpObj);

		if (bRet)
		{
			g_Log.Add("[%s][%s][Mu Rummy] Event Card Cnt(%d) (%d)", lpObj->AccountID, lpObj->Name, iBeforeCardCnt, pMuRummyInfo->GetCardDeckCnt());
			this->Slot5Log(lpObj, 0);

			memcpy(pMsg.stMuRummyCardInfo, stCardInfo, sizeof(stCardInfo));
			pMsg.btCardDeckCnt = pMuRummyInfo->GetCardDeckCnt();
			IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
		}
	}
}

bool CMuRummyMng::FillEmptySlotCard(CMuRummyInfo *pMuRummyInfo, _tagMuRummyCardInfo *pOutCardInfo, OBJECTSTRUCT *lpObj)
{
	CMuRummyCardInfo * pCCardInfo = pMuRummyInfo->GetCardInfo();

	if (!pCCardInfo)
	{
		g_Log.Add("[MuRummy][Error] pCCardInfo is NULL [%s][%s] [%s, %d]", lpObj->AccountID, lpObj->Name, __FILE__, __LINE__);
		return false;
	}

	BYTE *pSlotIndexInfo = pMuRummyInfo->GetSlotIndexInfo();

	if (!pSlotIndexInfo)
	{
		g_Log.Add("[MuRummy][Error] pSlotIndexInfo is NULL [%s][%s] [%s, %d]", lpObj->AccountID, lpObj->Name, __FILE__, __LINE__);
		return false;
	}

	int nCardArr = 0;

	for (int i = 0; i < 5; i++)
	{
		if (pSlotIndexInfo[i] == 255)
		{
			if (nCardArr < 0 || nCardArr > 4)
			{
				return false;
			}

			int nCardDeckCnt = pMuRummyInfo->GetCardDeckCnt();
			int nCardSeq = 24 - nCardDeckCnt;

			if (24 - nCardDeckCnt >= 24)
			{
				g_Log.Add("[MuRummy][Error] Card Cnt Over [%d] [%s, %d]", nCardSeq, __FILE__, __LINE__);
				return false;
			}

			pCCardInfo[nCardSeq].SetSlotNum(i);
			pCCardInfo[nCardSeq].SetState(1);
			pSlotIndexInfo[i] = nCardSeq;

			pOutCardInfo[nCardArr].btColor = pCCardInfo[nCardSeq].GetColor();
			pOutCardInfo[nCardArr].btNumber = pCCardInfo[nCardSeq].GetNumber();
			pOutCardInfo[nCardArr].btSlotNum = pCCardInfo[nCardSeq].GetSlotNum();
			pMuRummyInfo->SetCardDeckCnt(nCardDeckCnt - 1);
			nCardArr++;

			if (pMuRummyInfo->GetCardDeckCnt() < 0)
			{
				g_Log.Add("[MuRummy][Error] CardDeck cnt [%d] [%s, %d]", pMuRummyInfo->GetCardDeckCnt(), __FILE__, __LINE__);
				return false;
			}

			this->GDReqSlotInfoUpdate(lpObj, nCardSeq, pCCardInfo[nCardSeq].GetSlotNum(), pCCardInfo[nCardSeq].GetState());

			if (!pMuRummyInfo->GetCardDeckCnt())
			{
				return true;
			}
		}
	}

	return true;
}

BYTE CMuRummyMng::GetTempCardShowSlot(BYTE *pSlotCardIndex)
{
	int iSlotCnt = 0;

	for (int i = 0; i < 8; i++)
	{
		if (pSlotCardIndex[i] != 255)
		{
			if (i >= 5)
			{
				return -2;
			}

			iSlotCnt++;
		}
	}

	if (iSlotCnt >= 5)
	{
		return -1;
	}

	for (int i = 0; i < 5; i++)
	{
		if (pSlotCardIndex[i] == 255)
		{
			return i;
		}
	}

	return -1;
}

void CMuRummyMng::CGReqCardMove(_tagPMSG_REQ_CARD_MOVE *lpMsg, int aIndex)
{
	_tagPMSG_ANS_CARD_MOVE pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0x4D, 0x12, sizeof(pMsg));

	if (ObjectMaxRange(aIndex) == false)
	{
		return;
	}

	if (!gObjIsConnected(aIndex))
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if (!lpObj)
	{
		g_Log.Add("[MuRummy][Error] lpObj is NULL [%s, %d]", __FILE__, __LINE__);
		return;
	}

	if (lpObj->CloseType != -1)
	{
		return;
	}

	int sSlot = lpMsg->sSlot;
	int tSlot = lpMsg->tSlot;

	if (sSlot >= 0)
	{
		if (sSlot < 8 && tSlot >= 0 && tSlot < 8 && sSlot != tSlot)
		{
			CMuRummyInfo * pCMuRummyInfo = lpObj->m_PlayerData->m_pCMuRummyInfo;

			if (!pCMuRummyInfo)
			{
				g_Log.Add("[MuRummy][Error] pCMuRummyInfo is NULL [%s][%s] [%s, %d]", lpObj->AccountID, lpObj->Name, __FILE__, __LINE__);
				return;
			}

			CMuRummyCardInfo pCOutCardInfo;
			BYTE Ret = this->CardSlotMove(pCMuRummyInfo, sSlot, tSlot, &pCOutCardInfo, lpObj);

			if (Ret == 255)
			{
				return;
			}

			else if (Ret == 254)
			{
				sSlot = 255;
			}

			pMsg.sSlot = sSlot;
			pMsg.tSlot = pCOutCardInfo.GetSlotNum();
			pMsg.btColor = pCOutCardInfo.GetColor();
			pMsg.btNumber = pCOutCardInfo.GetNumber();
			IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
		}
	}
}

BYTE CMuRummyMng::CardSlotMove(CMuRummyInfo *pCMuRummyInfo, int sSlot, int tSlot, CMuRummyCardInfo *pCOutCardInfo, OBJECTSTRUCT *lpObj)
{
	CMuRummyCardInfo *pCCardInfo = pCMuRummyInfo->GetCardInfo();

	if (!pCCardInfo)
	{
		g_Log.Add("[MuRummy][Error] pCCardInfo is NULL [%s][%s] [%s, %d]", lpObj->AccountID, lpObj->Name, __FILE__, __LINE__);
		return -1;
	}

	BYTE *pSlotIndexInfo = pCMuRummyInfo->GetSlotIndexInfo();

	if (!pSlotIndexInfo)
	{
		g_Log.Add("[MuRummy][Error] pSlotIndexInfo is NULL [%s][%s] [%s, %d]", lpObj->AccountID, lpObj->Name, __FILE__, __LINE__);
		return -1;
	}

	BYTE sCardArr = pSlotIndexInfo[sSlot];
	BYTE tCardArr = pSlotIndexInfo[tSlot];

	if (sCardArr == 255)
	{
		return -1;
	}

	if (tCardArr != 255)
	{
		g_Log.Add("[MuRummy][Error] slot Not Empty [%s][%s] %d ", lpObj->AccountID, lpObj->Name, tCardArr);
		return -2;
	}

	if (sCardArr < 0 || sCardArr > 23)
	{
		g_Log.Add("[MuRummy][Error] CardInfo Index is Overflow [%s][%s][%d] [%s, %d]", lpObj->AccountID, lpObj->Name, sCardArr, __FILE__, __LINE__);
		return -2;
	}

	if (pCCardInfo[sCardArr].GetState() != 1)
	{
		return -1;
	}

	if (sSlot < 5 && tSlot >= 5)
	{
		g_Log.Add("[%s][%s][Mu Rummy] Slot[5] - Move Slot[%d] %c : %d",
			lpObj->AccountID, lpObj->Name,
			pCCardInfo[sCardArr].GetSlotNum() + 1,
			this->GetColorName(pCCardInfo[sCardArr].GetColor() - 1),
			pCCardInfo[sCardArr].GetNumber());
	}

	if (sSlot >= 5 && tSlot < 5)
	{
		g_Log.Add("[%s][%s][Mu Rummy] Slot[3] - Move Slot[%d] %c : %d",
			lpObj->AccountID, lpObj->Name,
			pCCardInfo[sCardArr].GetSlotNum() - 4,
			this->GetColorName(pCCardInfo[sCardArr].GetColor() - 1),
			pCCardInfo[sCardArr].GetNumber());
	}

	pSlotIndexInfo[sSlot] = -1;
	pSlotIndexInfo[tSlot] = sCardArr;
	pCCardInfo[sCardArr].SetSlotNum(tSlot);
	memcpy(pCOutCardInfo, &pCCardInfo[sCardArr], sizeof(CMuRummyCardInfo));

	if (sSlot < 5 && tSlot >= 5)
	{
		g_Log.Add("[%s][%s][Mu Rummy] Slot[3] - Register Slot[%d] %c : %d",
			lpObj->AccountID, lpObj->Name,
			pCCardInfo[sCardArr].GetSlotNum() - 4,
			this->GetColorName(pCCardInfo[sCardArr].GetColor() - 1),
			pCCardInfo[sCardArr].GetNumber());

		this->Slot3Log(lpObj, 0);
	}

	if (sSlot >= 5 && tSlot < 5)
	{
		g_Log.Add("[%s][%s][Mu Rummy] Slot[5] - Register Slot[%d] %c : %d",
			lpObj->AccountID, lpObj->Name,
			pCCardInfo[sCardArr].GetSlotNum() + 1,
			this->GetColorName(pCCardInfo[sCardArr].GetColor() - 1),
			pCCardInfo[sCardArr].GetNumber());

		this->Slot5Log(lpObj, 0);
	}

	return 0;
}

void CMuRummyMng::CGReqCardReMove(_tagPMSG_REQ_CARD_REMOVE *lpMsg, int aIndex)
{
	_tagPMSG_ANS_CARD_REMOVE pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0x4D, 0x13, sizeof(pMsg));

	if (ObjectMaxRange(aIndex) == false)
	{
		return;
	}

	if (!gObjIsConnected(aIndex))
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if (!lpObj)
	{
		g_Log.Add("[MuRummy][Error] lpObj is NULL [%s, %d]", __FILE__, __LINE__);
		return;
	}

	if (lpObj->CloseType != -1)
	{
		return;
	}

	int sSlot = lpMsg->btSlot;

	if (sSlot >= 8)
	{
		g_Log.Add("[MuRummy][Error] dump fail Slot Num [%d] [%s][%s] [%s, %d]",
			sSlot, lpObj->AccountID, lpObj->Name, __FILE__, __LINE__);
		return;
	}

	CMuRummyInfo * pCMuRummyInfo = lpObj->m_PlayerData->m_pCMuRummyInfo;

	if (!pCMuRummyInfo)
	{
		g_Log.Add("[MuRummy][Error] pCMuRummyInfo is NULL [%s][%s] [%s, %d]", lpObj->AccountID, lpObj->Name, __FILE__, __LINE__);
		return;
	}

	BYTE Ret = this->CardSlotReMove(pCMuRummyInfo, sSlot);

	if (Ret == 255)
	{
		g_Log.Add("[MuRummy][Error] dump Card fail [%s][%s] ", lpObj->AccountID, lpObj->Name);
		return;
	}

	CMuRummyCardInfo * pCCardInfo = pCMuRummyInfo->GetCardInfo();

	if (!pCCardInfo)
	{
		g_Log.Add("[MuRummy][Error] pCCardInfo is NULL [%s][%s] [%s, %d]", lpObj->AccountID, lpObj->Name, __FILE__, __LINE__);
		return;
	}

	this->GDReqCardInfoUpdate(lpObj, &pCCardInfo[Ret], Ret);

	pMsg.btResult = sSlot;
	IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);

	this->GCSendMsg(aIndex, 4, 0);
	g_Log.Add("[%s][%s][Mu Rummy] Slot[5] - Junk Slot[%d] %c : %d",
		lpObj->AccountID,
		lpObj->Name,
		sSlot,
		this->GetColorName(pCCardInfo[Ret].GetColor() - 1),
		pCCardInfo[Ret].GetNumber());

	this->Slot5Log(lpObj, 0);
}

BYTE CMuRummyMng::CardSlotReMove(CMuRummyInfo *pCMuRummyInfo, int sSlot)
{
	CMuRummyCardInfo *pCCardInfo = pCMuRummyInfo->GetCardInfo();

	if (!pCCardInfo)
	{
		g_Log.Add("[MuRummy][Error] pCCardInfo is NULL SlotNum [%d] [%s, %d]",
			sSlot, __FILE__, __LINE__);

		return -1;
	}

	BYTE *pSlotIndexInfo = pCMuRummyInfo->GetSlotIndexInfo();

	if (!pSlotIndexInfo)
	{
		g_Log.Add("[MuRummy][Error] pCSlotIndexInfo is NULL SlotNum [%d] [%s, %d]",
			sSlot, __FILE__, __LINE__);

		return -1;
	}

	if (pSlotIndexInfo[sSlot] == 255)
	{
		g_Log.Add("[MuRummy][Error] Slot is Empty SlotNum [%d] [%s, %d]",
			sSlot, __FILE__, __LINE__);

		return -1;
	}

	BYTE sCardArr = pSlotIndexInfo[sSlot];

	if (pCCardInfo[sCardArr].GetSlotNum() != sSlot)
	{
		g_Log.Add("[MuRummy][Error] Slot Num diff [%s, %d]", __FILE__, __LINE__);
		return -1;
	}

	pCCardInfo[sCardArr].SetSlotNum(-1);
	pCCardInfo[sCardArr].SetState(2);
	pSlotIndexInfo[sSlot] = -1;
	return sCardArr;
}

void CMuRummyMng::CGReqCardMatch(_tagPMSG_REQ_CARD_MATCH *lpMsg, int aIndex)
{
	_tagPMSG_ANS_CARD_MATCH pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0x4D, 0x14, sizeof(pMsg));

	if (ObjectMaxRange(aIndex) == false)
	{
		return;
	}

	if (!gObjIsConnected(aIndex))
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if (!lpObj)
	{
		g_Log.Add("[MuRummy][Error] lpObj is NULL [%s, %d]", __FILE__, __LINE__);
		return;
	}

	CMuRummyInfo * pCMuRummyInfo = lpObj->m_PlayerData->m_pCMuRummyInfo;

	if (!pCMuRummyInfo)
	{
		g_Log.Add("[MuRummy][Error] pCMuRummyInfo is NULL [%s][%s] [%s, %d]",
			lpObj->AccountID, lpObj->Name, __FILE__, __LINE__);
		return;
	}

	int nScore = 0;
	_tagMuRummyCardUpdateDS stCardUpdateDS[3];
	
	BYTE btRet = this->CardMatchCheck(pCMuRummyInfo, &nScore, stCardUpdateDS, lpObj);

	if (!pCMuRummyInfo->GetCardInfo())
	{
		g_Log.Add("[MuRummy][Error] pCCardInfo is NULL [%s, %d]", __FILE__, __LINE__);
		return;
	}

	if (btRet)
	{
		this->GCSendMsg(aIndex, 6, 0);
		g_Log.Add("[%s][%s][Mu Rummy] Slot[3] - Combine Fail", lpObj->AccountID, lpObj->Name);
	}

	else
	{
		pCMuRummyInfo->SetScore(nScore + pCMuRummyInfo->GetScore());
		pMsg.btResult = 1;
		pMsg.btScoreH = HIBYTE(nScore);
		pMsg.btScoreL = LOBYTE(nScore);
		pMsg.btTotScoreH = HIBYTE(pCMuRummyInfo->GetScore());
		pMsg.btTotScoreL = LOBYTE(pCMuRummyInfo->GetScore());
		IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
		this->GCSendMsg(aIndex, 5, 0);
		this->GDReqScoreUpdate(lpObj, pCMuRummyInfo->GetScore(), stCardUpdateDS);

		g_Log.Add("[%s][%s][Mu Rummy] Slot[3] - Combine Success Gain Point(%d) (%d)",
			lpObj->AccountID,
			lpObj->Name,
			nScore,
			pCMuRummyInfo->GetScore());
	}

	this->GCSendCardList(aIndex);
}

BYTE CMuRummyMng::CardMatchCheck(CMuRummyInfo *pCMuRummyInfo, int *iOutScore, _tagMuRummyCardUpdateDS *pOutCardUpdateDS, OBJECTSTRUCT *lpObj)
{
	CMuRummyCardInfo *pCCardInfo = pCMuRummyInfo->GetCardInfo();

	if (!pCCardInfo)
	{
		g_Log.Add("[MuRummy][Error] pCCardInfo is NULL [%s, %d]", __FILE__, __LINE__);
		return -1;
	}

	BYTE *pSlotIndexInfo = pCMuRummyInfo->GetSlotIndexInfo();

	if (!pSlotIndexInfo)
	{
		g_Log.Add("[MuRummy][Error] pCSlotIndexInfo is NULL [%s, %d]", __FILE__, __LINE__);
		return -1;
	}

	CMuRummyCardInfo CCardInfoTmp[3];
	CMuRummyCardInfo * pTempCard_1;
	CMuRummyCardInfo * pTempCard_2;

	int nChkCnt = 0;

	for (int nCnt = 5; nCnt < 8; nCnt++)
	{
		if (pSlotIndexInfo[nCnt] != 255)
		{
			pTempCard_1 = &pCCardInfo[pSlotIndexInfo[nCnt]];
			pTempCard_2 = &CCardInfoTmp[nChkCnt];

			pTempCard_2->m_iColor = pTempCard_1->m_iColor;
			pTempCard_2->m_iNumber = pTempCard_1->m_iNumber;
			pTempCard_2->m_iState = pTempCard_2->m_iState;
			pTempCard_2->m_iSlotNum = pTempCard_2->m_iSlotNum;

			nChkCnt++;
		}
	}

	if (nChkCnt != 3)
	{
		return -1;
	}

	this->Slot3Log(lpObj, 1);
	
	int nRetCardColor = this->IsCardSameColor(CCardInfoTmp);
	int nRetCardNum = this->IsCardNumber(CCardInfoTmp);

	bool bMatch = false;

	if (nRetCardColor == 1 && nRetCardNum == 3)
	{
		*iOutScore = 10 * (CCardInfoTmp[0].GetNumber() - 1) + 10;
		bMatch = true;
	}

	if (nRetCardColor == 2 && nRetCardNum == 3)
	{
		*iOutScore = 10 * (CCardInfoTmp[0].GetNumber() - 1) + 50;
		bMatch = true;
	}

	if (nRetCardColor == 1 && nRetCardNum == 4)
	{
		*iOutScore = 10 * (CCardInfoTmp[0].GetNumber() - 1) + 20;
		bMatch = true;
	}

	if (bMatch)
	{
		int iCnt = 0;

		for (int i = 5; i < 8; i++)
		{
			if (pSlotIndexInfo[i] != 255)
			{
				int nArrNum = pSlotIndexInfo[i];

				if (nArrNum < 0 || nArrNum > 23)
				{
					g_Log.Add("[MuRummy][Error] Combine CardInfo Index is Overflow [%s][%s][%d] [%s, %d]",
						lpObj->AccountID, lpObj->Name, nArrNum, __FILE__, __LINE__);

					return -2;
				}

				pCCardInfo[nArrNum].SetSlotNum(-1);
				pCCardInfo[nArrNum].SetState(3);
				pSlotIndexInfo[i] = -1;
				pOutCardUpdateDS[iCnt].btSeq = nArrNum;
				pOutCardUpdateDS[iCnt].btSlotNum = pCCardInfo[nArrNum].GetSlotNum();
				pOutCardUpdateDS[iCnt].btStatus = pCCardInfo[nArrNum].GetState();
				iCnt++;
			}
		}

		return 0;
	}

	int nSlotNum = 5;

	for (int n = 0; n < 5; n++)
	{
		if (pSlotIndexInfo[n] == 255)
		{
			if (nSlotNum < 0 || nSlotNum > 7)
			{
				return -2;
			}

			pSlotIndexInfo[n] = pSlotIndexInfo[nSlotNum];
			int nArrNum = pSlotIndexInfo[n];

			if (nArrNum < 0 || nArrNum > 23)
			{
				g_Log.Add("[MuRummy][Error] CardInfo Index is Overflow [%s][%s][%d] [%s, %d]",
					lpObj->AccountID, lpObj->Name, nArrNum, __FILE__, __LINE__);

				return -2;
			}

			pCCardInfo[nArrNum].SetSlotNum(n);
			pSlotIndexInfo[nSlotNum] = -1;
			nSlotNum++;
		}
	}

	*iOutScore = 0;
	return -1;
}

void CMuRummyMng::CardSortAscending(CMuRummyCardInfo *pCCardInfo)
{
	CMuRummyCardInfo CCardInfoTmp;
	CMuRummyCardInfo *pCardInfo_1;
	CMuRummyCardInfo *pCardInfo_2;
	CMuRummyCardInfo *pCardInfo_3;
	CMuRummyCardInfo *pCardInfo_4;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 3 - i - 1; j++)
		{
			int nCardNum = pCCardInfo[j].GetNumber();

			if (nCardNum > pCCardInfo[j + 1].GetNumber())
			{
				pCardInfo_1 = &pCCardInfo[j];

				CCardInfoTmp.m_iColor = pCardInfo_1->m_iColor;
				CCardInfoTmp.m_iNumber = pCardInfo_1->m_iNumber;
				CCardInfoTmp.m_iState = pCardInfo_1->m_iState;
				CCardInfoTmp.m_iSlotNum = pCardInfo_1->m_iSlotNum;

				pCardInfo_2 = &pCCardInfo[j + 1];
				pCardInfo_3 = &pCCardInfo[j];

				pCardInfo_3->m_iColor = pCardInfo_2->m_iColor;
				pCardInfo_3->m_iNumber = pCardInfo_2->m_iNumber;
				pCardInfo_3->m_iState = pCardInfo_2->m_iState;
				pCardInfo_3->m_iSlotNum = pCardInfo_2->m_iSlotNum;

				pCardInfo_4 = &pCCardInfo[j + 1];

				pCardInfo_4->m_iColor = CCardInfoTmp.m_iColor;
				pCardInfo_4->m_iNumber = CCardInfoTmp.m_iNumber;
				pCardInfo_4->m_iState = CCardInfoTmp.m_iState;
				pCardInfo_4->m_iSlotNum = CCardInfoTmp.m_iSlotNum;
			}
		}
	}
}

int CMuRummyMng::IsCardSameColor(CMuRummyCardInfo *pCCardInfo)
{
	for (int i = 0; i < 2; i++)
	{
		if (pCCardInfo[i].GetColor() != pCCardInfo[i + 1].GetColor())
		{
			return 1;
		}
	}

	return 2;
}

int CMuRummyMng::IsCardNumber(CMuRummyCardInfo *pCCardInfo)
{
	for (int i = 0;; i++)
	{
		if (i >= 2)
		{
			return 4;
		}

		if (pCCardInfo[i].GetNumber() != pCCardInfo[i + 1].GetNumber())
		{
			break;
		}
	}

	this->CardSortAscending(pCCardInfo);

	for (int j = 0; j < 2; j++)
	{
		if (pCCardInfo[j + 1].GetNumber() - pCCardInfo[j].GetNumber() != 1)
		{
			return 0;
		}
	}

	return 3;
}

void CMuRummyMng::CGReqMuRummyEnd(_tagPMSG_REQ_MURUMMY_END *lpMsg, int aIndex)
{
	if (ObjectMaxRange(aIndex) == false)
	{
		return;
	}

	if (!gObjIsConnected(aIndex))
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if (!lpObj)
	{
		g_Log.Add("[MuRummy][Error] lpObj is NULL [%s, %d]", __FILE__, __LINE__);
		return;
	}

	if (lpObj->CloseType != -1)
	{
		return;
	}

	CMuRummyInfo *pCMuRummyInfo = lpObj->m_PlayerData->m_pCMuRummyInfo;

	if (!pCMuRummyInfo)
	{
		g_Log.Add("[MuRummy][Error] pCMuRummyInfo is NULL [%s][%s] [%s, %d]",
			lpObj->AccountID, lpObj->Name, __FILE__, __LINE__);
		return;
	}

	if (pCMuRummyInfo->IsGetPlayCard() == false)
	{
		g_Log.Add("[MuRummy][Error] MuRummy not started [%s][%s] [%s, %d]",
			lpObj->AccountID, lpObj->Name, __FILE__, __LINE__);
		return;
	}

	if (pCMuRummyInfo->IsWaitReward() == true)
	{
		g_Log.Add("[MuRummy][DebugLog][%s][%s] Waiting Item Reward(Req End)", lpObj->AccountID, lpObj->Name);
		GSProtocol.GCServerMsgStringSend(Lang.GetText(0,561), aIndex, 1);
		return;
	}

	if (pCMuRummyInfo->GetScore() < this->GetRewardScore1() || CheckInventoryEmptySpace(lpObj, 4, 2))
	{
		if (pCMuRummyInfo->GetScore() >= this->GetRewardScore1())
		{
			if (pCMuRummyInfo->GetScore() < this->GetRewardScore1() || (pCMuRummyInfo->GetScore() >= this->GetRewardScore2()))
			{
				if (pCMuRummyInfo->GetScore() >= this->GetRewardScore2())
				{
					CItem Item;
					time_t Duration;

					if (g_BagManager.GetItemFromBag(aIndex, BAG_EVENT, EVENTBAG_MURUMMY_2, aIndex, Item, Duration) == false)
					{
						return;
					}

					LPITEM_ATTRIBUTE lpAttr = GetItemAttr(Item.m_Type);

					if (lpAttr == NULL)
					{
						return;
					}

					g_BagManager.AddCashCoin(aIndex, BAG_EVENT, EVENTBAG_MURUMMY_2, aIndex);
					ItemSerialCreateSend(aIndex, lpAttr->ItemKindA == 12 ? 220 : 225, 0, 0, Item.m_Type, Item.m_Level, Item.m_Durability, Item.m_Option1, Item.m_Option2, Item.m_Option3, aIndex, Item.m_NewOption, Item.m_SetOption, Duration, 0, 0);
				}
			}

			else
			{
				CItem Item;
				time_t Duration;

				if (g_BagManager.GetItemFromBag(aIndex, BAG_EVENT, EVENTBAG_MURUMMY_1, aIndex, Item, Duration) == false)
				{
					return;
				}

				LPITEM_ATTRIBUTE lpAttr = GetItemAttr(Item.m_Type);

				if (lpAttr == NULL)
				{
					return;
				}

				g_BagManager.AddCashCoin(aIndex, BAG_EVENT, EVENTBAG_MURUMMY_1, aIndex);
				ItemSerialCreateSend(aIndex, lpAttr->ItemKindA == 12 ? 220 : 225, 0, 0, Item.m_Type, Item.m_Level, Item.m_Durability, Item.m_Option1, Item.m_Option2, Item.m_Option3, aIndex, Item.m_NewOption, Item.m_SetOption, Duration, 0, 0);
			}

			pCMuRummyInfo->SetWaitReward(1);
		}

		else
		{
			int nMoney = lpObj->m_PlayerData->Money + 500000;

			if (gObjCheckMaxZen(aIndex, 500000))
			{
				this->GCSendMsg(aIndex, 10, 0);
				lpObj->m_PlayerData->Money += 500000;
				GSProtocol.GCMoneySend(aIndex, lpObj->m_PlayerData->Money);

				_tagPMSG_ANS_MURUMMY_END pMsg;
				pMsg.btResult = 1;
				PHeadSubSetB((LPBYTE)&pMsg, 0x4D, 0x15, sizeof(pMsg));
				IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);

				this->GCSendMsg(aIndex, 8, 0);

				g_Log.Add("[%s][%s][Mu Rummy] Event Reward Result [Point : %d, RewardZen : %d]", lpObj->AccountID, lpObj->Name, pCMuRummyInfo->GetScore(), 500000);
				g_Log.Add("[%s][%s][Mu Rummy] Event end", lpObj->AccountID, lpObj->Name);

				this->GDReqMuRummyDBLog(lpObj, pCMuRummyInfo->GetScore());
				pCMuRummyInfo->Clear();
				this->GDReqScoreDelete(lpObj);
			}

			else
			{
				this->GCSendMsg(aIndex, 11, 0);
				g_Log.Add("[%s][%s][Mu Rummy] Event Reward fail (0)", lpObj->AccountID, lpObj->Name);
			}
		}
	}

	else
	{
		this->GCSendMsg(aIndex, 7, 0);
		g_Log.Add("[%s][%s][Mu Rummy] Event Reward fail (1)", lpObj->AccountID, lpObj->Name);
	}
}

void CMuRummyMng::GCSendCardList(int aIndex)
{
	_tagPMSG_SEND_SLOTCARD_LIST pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0x4D, 0x16, sizeof(pMsg));

	if (ObjectMaxRange(aIndex) == false)
	{
		return;
	}

	if (!gObjIsConnected(aIndex))
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if (!lpObj)
	{
		g_Log.Add("[MuRummy][Error] lpObj is NULL [%s, %d]", __FILE__, __LINE__);
		return;
	}

	CMuRummyInfo *pCMuRummyInfo = lpObj->m_PlayerData->m_pCMuRummyInfo;

	if (!pCMuRummyInfo)
	{
		g_Log.Add("[MuRummy][Error] pCMuRummyInfo is NULL [%s][%s] [%s, %d]",
			lpObj->AccountID, lpObj->Name, __FILE__, __LINE__);
		return;
	}

	CMuRummyCardInfo * pCCardInfo = pCMuRummyInfo->GetCardInfo();

	if (!pCCardInfo)
	{
		g_Log.Add("[MuRummy][Error] pCCardInfo is NULL [%s, %d]",
			__FILE__, __LINE__);

		return;
	}

	BYTE *pSlotIndexInfo = pCMuRummyInfo->GetSlotIndexInfo();

	if (!pSlotIndexInfo)
	{
		g_Log.Add("[MuRummy][Error] pCSlotIndexInfo is NULL SlotNum [%s, %d]",
			__FILE__, __LINE__);

		return;
	}

	int nSlotNum = 0;

	for (int i = 0; i < 8; i++)
	{
		if (pSlotIndexInfo[i] != 255)
		{
			if (nSlotNum < 0 || nSlotNum > 4)
			{
				return;
			}

			int nArrNum = pSlotIndexInfo[i];
			pMsg.stMuRummyCardInfo[nSlotNum].btColor = pCCardInfo[nArrNum].GetColor();
			pMsg.stMuRummyCardInfo[nSlotNum].btNumber = pCCardInfo[nArrNum].GetNumber();
			pMsg.stMuRummyCardInfo[nSlotNum].btSlotNum = pCCardInfo[nArrNum].GetSlotNum();
			nSlotNum++;
		}
	}

	IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

void CMuRummyMng::GCSendMsg(int aIndex, BYTE btNotiIndex, int iValue)
{
	_tagPMSG_SEND_MURUMMY_MSG pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0x4D, 0x17, sizeof(pMsg));

	if (ObjectMaxRange(aIndex) == false)
	{
		return;
	}

	if (!gObjIsConnected(aIndex))
	{
		return;
	}

	pMsg.btNotiIndex = btNotiIndex;
	pMsg.btValueH = HIBYTE(iValue);
	pMsg.btValueL = LOBYTE(iValue);

	IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

void CMuRummyMng::GDReqCardInfo(LPOBJ lpObj)
{
	if (ObjectMaxRange(lpObj->m_Index) == false)
	{
		return;
	}

	if (gObjIsConnected(lpObj->m_Index) == false)
	{
		return;
	}

	if (this->IsMuRummyEventOn() == true)
	{
		_tagPMSG_REQ_MURUMMY_SELECT_DS pMsg;
		memcpy(pMsg.AccountID, lpObj->AccountID, MAX_ACCOUNT_LEN + 1);
		memcpy(pMsg.Name, lpObj->Name, MAX_ACCOUNT_LEN + 1);
		pMsg.aIndex = lpObj->m_Index;
		pMsg.h.c = 0xC1;
		pMsg.h.size = sizeof(pMsg);
		pMsg.h.headcode = 0xE8;
		pMsg.h.subcode = 0x00;
		wsDataCli.DataSend((char *)&pMsg, sizeof(pMsg));
	}
}

void CMuRummyMng::GDAnsCardInfo(_tagPMSG_ANS_MURUMMY_SELECT_DS *lpMsg)
{
	int aIndex = lpMsg->aIndex;

	if (ObjectMaxRange(aIndex) == false)
	{
		return;
	}

	if (!gObjIsConnected(aIndex))
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if (!lpObj)
	{
		g_Log.Add("[MuRummy][Error] lpObj is NULL [%s, %d]", __FILE__, __LINE__);
		return;
	}

	CMuRummyInfo * pMuRummyInfo = lpObj->m_PlayerData->m_pCMuRummyInfo;

	if (!pMuRummyInfo)
	{
		g_Log.Add("[MuRummy][Error] pMuRummyInfo is NULL [%s][%s] [%s, %d]",
			lpObj->AccountID, lpObj->Name, __FILE__, __LINE__);
		return;
	}

	CMuRummyCardInfo * pCCardInfo = pMuRummyInfo->GetCardInfo();

	if (!pCCardInfo)
	{
		g_Log.Add("[MuRummy][Error] pCCardInfo is NULL [%s, %d]",
			__FILE__, __LINE__);

		return;
	}

	BYTE *pSlotIndexInfo = pMuRummyInfo->GetSlotIndexInfo();

	if (!pSlotIndexInfo || this->IsMuRummyEventOn() == false)
	{
		return;
	}

	GSProtocol.GCSendEventBanner(lpObj->m_Index, 0);

	if (lpMsg->btResult)
	{
		pMuRummyInfo->SetScore(lpMsg->wScore);

		for (int i = 0; i < 8; i++)
		{
			pSlotIndexInfo[i] = -1;
		}

		int iCardCnt = 0;

		for (int nCardArr = 0; nCardArr < 24; nCardArr++)
		{
			int nArr = lpMsg->stMuRummyCardInfoDS[nCardArr].btSeq;
			int iSlotNum = lpMsg->stMuRummyCardInfoDS[nCardArr].btSlotNum;

			pCCardInfo[nArr].SetColor(lpMsg->stMuRummyCardInfoDS[nCardArr].btColor);
			pCCardInfo[nArr].SetNumber(lpMsg->stMuRummyCardInfoDS[nCardArr].btNumber);
			pCCardInfo[nArr].SetState(lpMsg->stMuRummyCardInfoDS[nCardArr].btStatus);
			pCCardInfo[nArr].SetSlotNum(iSlotNum);

			if (iSlotNum > -1 && iSlotNum < 8 && pCCardInfo[nArr].GetState() == 1)
				pSlotIndexInfo[iSlotNum] = nArr;

			if (!pCCardInfo[nArr].GetState())
			{
				iCardCnt++;
			}

			g_Log.Add("[MuRummy] MuRummy Card Info Load [%s][%s] Seq:[%d] [%d][%c][%d][%d]",
				lpObj->AccountID,
				lpObj->Name,
				nArr,
				pCCardInfo[nArr].GetSlotNum(),
				this->GetColorName(pCCardInfo[nArr].GetColor() - 1),
				pCCardInfo[nArr].GetNumber(),
				pCCardInfo[nArr].GetState());
		}

		g_Log.Add("[MuRummy] MuRummy Card Info Load [%s][%s] CardDeckCnt:[%d]",
			lpObj->AccountID, lpObj->Name, iCardCnt);

		pMuRummyInfo->SetCardDeckCnt(iCardCnt);
		pMuRummyInfo->SetGetPlayCard(1);
		pMuRummyInfo->SetMuRummyInfoLoad(1);
	}

	else
	{
		pMuRummyInfo->SetGetPlayCard(0);
		pMuRummyInfo->SetMuRummyInfoLoad(1);
	}
}

void CMuRummyMng::GDReqCardInfoInsert(OBJECTSTRUCT *lpObj)
{
	if (ObjectMaxRange(lpObj->m_Index) == false)
	{
		return;
	}

	if (gObjIsConnected(lpObj->m_Index) == false)
	{
		return;
	}

	if (this->IsMuRummyEventOn() == false)
	{
		return;
	}

	CMuRummyInfo * pMuRummyInfo = lpObj->m_PlayerData->m_pCMuRummyInfo;

	if (!pMuRummyInfo)
	{
		g_Log.Add("[MuRummy][Error] pMuRummyInfo is NULL [%s][%s] [%s, %d]", lpObj->AccountID, lpObj->Name, __FILE__, __LINE__);
		return;
	}

	if (pMuRummyInfo->IsGetPlayCard() == false)
	{
		return;
	}

	CMuRummyCardInfo * pCCardInfo = pMuRummyInfo->GetCardInfo();

	if (!pCCardInfo)
	{
		g_Log.Add("[MuRummy][Error] pCCardInfo is NULL [%s, %d]", __FILE__, __LINE__);

		return;
	}

	BYTE *pSlotIndexInfo = pMuRummyInfo->GetSlotIndexInfo();

	if (!pSlotIndexInfo)
	{
		g_Log.Add("[MuRummy][Error] pSlotIndexInfo is NULL [%s][%s] [%s, %d]", lpObj->AccountID, lpObj->Name, __FILE__, __LINE__);
		return;
	}

	_tagPMSG_REQ_MURUMMY_INSERT_DS pMsg;
	memcpy(pMsg.AccountID, lpObj->AccountID, MAX_ACCOUNT_LEN + 1);
	memcpy(pMsg.Name, lpObj->Name, MAX_ACCOUNT_LEN + 1);

	for (int i = 0; i < 24; i++)
	{
		pMsg.stMuRummyCardInfoDS[i].btColor = pCCardInfo[i].GetColor();
		pMsg.stMuRummyCardInfoDS[i].btNumber = pCCardInfo[i].GetNumber();
		pMsg.stMuRummyCardInfoDS[i].btSlotNum = pCCardInfo[i].GetSlotNum();
		pMsg.stMuRummyCardInfoDS[i].btStatus = pCCardInfo[i].GetState();
		pMsg.stMuRummyCardInfoDS[i].btSeq = i;
	}

	pMsg.aIndex = lpObj->m_Index;
	pMsg.h.c = 0xC1;
	pMsg.h.size = sizeof(pMsg);
	pMsg.h.headcode = 0xE8;
	pMsg.h.subcode = 0x01;

	wsDataCli.DataSend((char *)&pMsg, pMsg.h.size);
}

void CMuRummyMng::GDReqScoreUpdate(LPOBJ lpObj, WORD wScore, _tagMuRummyCardUpdateDS *pCardUpdateDS)
{
	_tagPMSG_REQ_MURUMMY_SCORE_UPDATE_DS pMsg;
	memcpy(pMsg.AccountID, lpObj->AccountID, MAX_ACCOUNT_LEN + 1);
	memcpy(pMsg.Name, lpObj->Name, MAX_ACCOUNT_LEN + 1);
	memcpy(pMsg.stCardUpdateDS, pCardUpdateDS, sizeof(pMsg.stCardUpdateDS));

	pMsg.wScore = wScore;
	pMsg.h.c = 0xC1;
	pMsg.h.size = sizeof(pMsg);
	pMsg.h.headcode = 0xE8;
	pMsg.h.subcode = 0x02;

	wsDataCli.DataSend((char *)&pMsg, pMsg.h.size);
}

void CMuRummyMng::GDReqCardInfoUpdate(OBJECTSTRUCT *lpObj, CMuRummyCardInfo *pCCardInfo, int iSeq)
{
	_tagPMSG_REQ_MURUMMY_UPDATE_DS pMsg;
	memcpy(pMsg.AccountID, lpObj->AccountID, MAX_ACCOUNT_LEN + 1);
	memcpy(pMsg.Name, lpObj->Name, MAX_ACCOUNT_LEN + 1);

	pMsg.btSlotNum = pCCardInfo->GetSlotNum();
	pMsg.btStatus = pCCardInfo->GetState();
	pMsg.btSequence = iSeq;
	pMsg.h.c = 0xC1;
	pMsg.h.size = sizeof(pMsg);
	pMsg.h.headcode = 0xE8;
	pMsg.h.subcode = 0x03;
	
	wsDataCli.DataSend((char *)&pMsg, sizeof(pMsg));
}

void CMuRummyMng::GDReqScoreDelete(OBJECTSTRUCT *lpObj)
{
	_tagPMSG_REQ_MURUMMY_DELETE_DS pMsg;
	memcpy(pMsg.AccountID, lpObj->AccountID, MAX_ACCOUNT_LEN + 1);
	memcpy(pMsg.Name, lpObj->Name, MAX_ACCOUNT_LEN + 1);

	pMsg.h.c = 0xC1;
	pMsg.h.size = sizeof(pMsg);
	pMsg.h.headcode = 0xE8;
	pMsg.h.subcode = 0x04;

	wsDataCli.DataSend((char *)&pMsg, sizeof(pMsg));
}

void CMuRummyMng::GDReqSlotInfoUpdate(OBJECTSTRUCT *lpObj, BYTE btSeq, BYTE btSlotNum, BYTE btStatus)
{
	_tagPMSG_REQ_MURUMMY_SLOTUPDATE_DS pMsg;

	if (btSlotNum < 0 || btSlotNum > 4)
	{
		g_Log.Add("[MuRummy][Error] btSlotNum[%d]  [%s][%s] [%s, %d]",
			btSlotNum, lpObj->AccountID, lpObj->Name, __FILE__, __LINE__);
		return;
	}

	pMsg.stCardUpdateDS.btSeq = btSeq;
	pMsg.stCardUpdateDS.btStatus = btStatus;
	pMsg.stCardUpdateDS.btSlotNum = btSlotNum;
	memcpy(pMsg.AccountID, lpObj->AccountID, MAX_ACCOUNT_LEN + 1);
	memcpy(pMsg.Name, lpObj->Name, MAX_ACCOUNT_LEN + 1);

	pMsg.h.c = 0xC1;
	pMsg.h.size = sizeof(pMsg);
	pMsg.h.headcode = 0xE8;
	pMsg.h.subcode = 0x05;

	wsDataCli.DataSend((char *)&pMsg, pMsg.h.size);
}

void CMuRummyMng::GDReqMuRummyInfoUpdate(OBJECTSTRUCT *lpObj)
{
	if (ObjectMaxRange(lpObj->m_Index) == false)
	{
		return;
	}

	if (gObjIsConnected(lpObj->m_Index) == false)
	{
		return;
	}

	if (this->IsMuRummyEventOn() == false)
	{
		return;
	}

	CMuRummyInfo * pMuRummyInfo = lpObj->m_PlayerData->m_pCMuRummyInfo;

	if (!pMuRummyInfo)
	{
		g_Log.Add("[MuRummy][Error] pMuRummyInfo is NULL [%s][%s] [%s, %d]", lpObj->AccountID, lpObj->Name, __FILE__, __LINE__);
		return;
	}

	if (pMuRummyInfo->IsGetPlayCard() == false)
	{
		return;
	}

	CMuRummyCardInfo * pCCardInfo = pMuRummyInfo->GetCardInfo();

	if (!pCCardInfo)
	{
		g_Log.Add("[MuRummy][Error] pCCardInfo is NULL [%s, %d]", __FILE__, __LINE__);

		return;
	}

	_tagPMSG_REQ_MURUMMY_INFO_UPDATE_DS pMsg;
	memcpy(pMsg.AccountID, lpObj->AccountID, MAX_ACCOUNT_LEN + 1);
	memcpy(pMsg.Name, lpObj->Name, MAX_ACCOUNT_LEN + 1);
	pMsg.wScore = pMuRummyInfo->GetScore();

	for (int i = 0; i < 24; i++)
	{
		pMsg.stMuRummyCardUpdateDS[i].btSlotNum = pCCardInfo[i].GetSlotNum();
		pMsg.stMuRummyCardUpdateDS[i].btStatus = pCCardInfo[i].GetState();
		pMsg.stMuRummyCardUpdateDS[i].btSeq = i;

		if (pCCardInfo[i].GetColor() != 255)
		{
			g_Log.Add("[MuRummy] MuRummy Info Save [%s][%s] [%d][%c][%d][%d] ",
				lpObj->AccountID, lpObj->Name,
				pCCardInfo[i].GetSlotNum(),
				this->GetColorName(pCCardInfo[i].GetColor() - 1),
				pCCardInfo[i].GetNumber(),
				pCCardInfo[i].GetState());
		}
	}

	pMsg.h.c = 0xC1;
	pMsg.h.size = sizeof(pMsg);
	pMsg.h.headcode = 0xE8;
	pMsg.h.subcode = 0x06;

	wsDataCli.DataSend((char *)&pMsg, pMsg.h.size);
}

void CMuRummyMng::CheatSetCardColorAsc(CMuRummyInfo *pMuRummyInfo, int aIndex)
{
	int nCardArr = 0;

	if (!pMuRummyInfo)
	{
		g_Log.Add("[MuRummy][Error] pMuRummyInfo is NULL [%s][%s] [%s, %d]", gObj[aIndex].AccountID, gObj[aIndex].Name, __FILE__, __LINE__);
		return;
	}

	CMuRummyCardInfo * pCCardInfo = pMuRummyInfo->GetCardInfo();

	if (!pCCardInfo)
	{
		g_Log.Add("[MuRummy][Error] pCCardInfo is NULL [%s, %d]", __FILE__, __LINE__);

		return;
	}
	
	for (int nCardNum = 0; nCardNum < 8; nCardNum++)
	{
		for (int nCardColor = 0; nCardColor < 3; nCardColor++)
		{
			if (nCardArr < 0 || nCardArr > 23)
			{
				return;
			}

			pCCardInfo[nCardArr].SetColor(nCardColor + 1);
			pCCardInfo[nCardArr].SetNumber(nCardNum + 1);
			pCCardInfo[nCardArr].SetState(0);
			pCCardInfo[nCardArr].SetSlotNum(0);

			nCardArr++;
		}
	}

	BYTE *pSlotIndexInfo = pMuRummyInfo->GetSlotIndexInfo();

	if (!pSlotIndexInfo)
	{
		return;
	}

	for (int i = 0; i < 8; i++)
	{
		pSlotIndexInfo[i] = -1;
	}

	pMuRummyInfo->SetCardDeckCnt(24);

	_tagPMSG_ANS_MURUMMY_INFO pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0x4D, 0x10, sizeof(pMsg));
	
	pMuRummyInfo->SetScore(0);
	pMsg.btCardDeckCnt = pMuRummyInfo->GetCardDeckCnt();
	pMsg.btTotScoreH = HIBYTE(pMuRummyInfo->GetScore());
	pMsg.btTotScoreL = LOBYTE(pMuRummyInfo->GetScore());

	IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

void CMuRummyMng::CheatSetCardNumAsc(CMuRummyInfo *pMuRummyInfo, int aIndex)
{
	this->SetCardDeck(pMuRummyInfo);

	_tagPMSG_ANS_MURUMMY_INFO pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0x4D, 0x10, sizeof(pMsg));

	IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

void CMuRummyMng::CheatSetScore(CMuRummyInfo *pMuRummyInfo, int iScore, int aIndex)
{
	_tagPMSG_ANS_CARD_MATCH pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0x4D, 0x14, sizeof(pMsg));
	
	pMuRummyInfo->SetScore(iScore);

	pMsg.btResult = 1;
	pMsg.btScoreH = HIBYTE(iScore);
	pMsg.btScoreL = LOBYTE(iScore);
	pMsg.btTotScoreH = HIBYTE(pMuRummyInfo->GetScore());
	pMsg.btTotScoreL = LOBYTE(pMuRummyInfo->GetScore());

	IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
	this->GCSendMsg(aIndex, 5, 0);
}

BYTE CMuRummyMng::GetColorName(int iColorNum)
{
	char szColor[4];

	szColor[0] = 0x52;
	szColor[1] = 0x42;
	szColor[2] = 0x59;
	szColor[3] = 0x45;

	if (iColorNum < 3 && iColorNum >= 0)
	{
		return szColor[iColorNum];
	}

	return szColor[3];
}

void CMuRummyMng::Slot5Log(OBJECTSTRUCT *lpObj, int iLogType)
{
	if (ObjectMaxRange(lpObj->m_Index) == false)
	{
		return;
	}

	CMuRummyInfo * pMuRummyInfo = lpObj->m_PlayerData->m_pCMuRummyInfo;

	if (!pMuRummyInfo)
	{
		g_Log.Add("[MuRummy][Error] pMuRummyInfo is NULL [%s][%s] [%s, %d]", lpObj->AccountID, lpObj->Name, __FILE__, __LINE__);
		return;
	}

	CMuRummyCardInfo * pCCardInfo = pMuRummyInfo->GetCardInfo();

	if (!pCCardInfo)
	{
		g_Log.Add("[MuRummy][Error] pCCardInfo is NULL [%s][%s] [%s, %d]", lpObj->AccountID, lpObj->Name, __FILE__, __LINE__);
		return;
	}

	BYTE *pSlotIndexInfo = pMuRummyInfo->GetSlotIndexInfo();

	if (!pSlotIndexInfo)
	{
		g_Log.Add("[MuRummy][Error] pSlotIndexInfo is NULL [%s][%s] [%s, %d]", lpObj->AccountID, lpObj->Name, __FILE__, __LINE__);
		return;
	}

	char szSlotInfo[1024];
	memset(szSlotInfo, 0x00, sizeof(szSlotInfo));
	char szTemp[1024];
	memset(szTemp, 0x00, sizeof(szTemp));

	_tagMuRummyCardInfo stMuRummyCardInfo[5];

	for (int n = 0; n < 5; n++)
	{
		if (pSlotIndexInfo[n] == 255)
		{
			strcat(szTemp, "None");
		}

		else
		{
			sprintf(szSlotInfo, "%c(%d)", this->GetColorName(pCCardInfo[pSlotIndexInfo[n]].GetColor() - 1), pCCardInfo[pSlotIndexInfo[n]].GetNumber());
			strcat(szTemp, szSlotInfo);
		}

		if (n != 4)
		{
			strcat(szTemp, " ,");
		}
	}

	if (iLogType)
	{
		g_Log.Add("[%s][%s][Mu Rummy] Slot[5] - Register Result [ %s ]", lpObj->AccountID, lpObj->Name, szTemp);
	}

	else
	{
		g_Log.Add("[%s][%s][Mu Rummy] Slot[5] - [ %s ]", lpObj->AccountID, lpObj->Name, szTemp);
	}
}

void CMuRummyMng::Slot3Log(OBJECTSTRUCT *lpObj, int iLogType)
{
	if (ObjectMaxRange(lpObj->m_Index) == false)
	{
		return;
	}

	CMuRummyInfo * pMuRummyInfo = lpObj->m_PlayerData->m_pCMuRummyInfo;

	if (!pMuRummyInfo)
	{
		g_Log.Add("[MuRummy][Error] pMuRummyInfo is NULL [%s][%s] [%s, %d]", lpObj->AccountID, lpObj->Name, __FILE__, __LINE__);
		return;
	}

	CMuRummyCardInfo * pCCardInfo = pMuRummyInfo->GetCardInfo();

	if (!pCCardInfo)
	{
		g_Log.Add("[MuRummy][Error] pCCardInfo is NULL [%s][%s] [%s, %d]", lpObj->AccountID, lpObj->Name, __FILE__, __LINE__);
		return;
	}

	BYTE *pSlotIndexInfo = pMuRummyInfo->GetSlotIndexInfo();

	if (!pSlotIndexInfo)
	{
		g_Log.Add("[MuRummy][Error] pSlotIndexInfo is NULL [%s][%s] [%s, %d]", lpObj->AccountID, lpObj->Name, __FILE__, __LINE__);
		return;
	}

	char szSlotInfo[1024];
	memset(szSlotInfo, 0x00, sizeof(szSlotInfo));
	char szTemp[1024];
	memset(szTemp, 0x00, sizeof(szTemp));

	_tagMuRummyCardInfo stMuRummyCardInfo[5];

	for (int n = 5; n < 8; n++)
	{
		if (pSlotIndexInfo[n] == 255)
		{
			strcat(szTemp, "None");
		}

		else
		{
			sprintf(szSlotInfo, "%c(%d)", this->GetColorName(pCCardInfo[pSlotIndexInfo[n]].GetColor() - 1), pCCardInfo[pSlotIndexInfo[n]].GetNumber());
			strcat(szTemp, szSlotInfo);
		}

		if (n != 7)
		{
			strcat(szTemp, " ,");
		}
	}

	if (iLogType)
	{
		g_Log.Add("[%s][%s][Mu Rummy] Slot[3] - Register Result [ %s ]", lpObj->AccountID, lpObj->Name, szTemp);
	}

	else
	{
		g_Log.Add("[%s][%s][Mu Rummy] Slot[3] - [ %s ]", lpObj->AccountID, lpObj->Name, szTemp);
	}
}

void CMuRummyMng::GDReqMuRummyDBLog(OBJECTSTRUCT *lpObj, int iScore)
{
	if (ObjectMaxRange(lpObj->m_Index) == false)
	{
		return;
	}

	if (!gObjIsConnected(lpObj->m_Index))
	{
		return;
	}

	if (this->IsMuRummyEventOn() == false)
	{
		return;
	}

	if (this->m_bMuRummyEventDBLog == false)
	{
		return;
	}

	if (iScore)
	{
		_tagPMSG_REQ_MURUMMY_LOG_INSERT_DS pMsg;

		memcpy(pMsg.AccountID, lpObj->AccountID, MAX_ACCOUNT_LEN + 1);
		memcpy(pMsg.Name, lpObj->Name, MAX_ACCOUNT_LEN + 1);
		pMsg.wScore = iScore;

		pMsg.h.c = 0xC1;
		pMsg.h.size = sizeof(pMsg);
		pMsg.h.headcode = 0xE8;
		pMsg.h.subcode = 0x07;

		wsDataCli.DataSend((char *)&pMsg, pMsg.h.size);
	}
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


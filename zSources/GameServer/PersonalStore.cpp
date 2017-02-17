//////////////////////////////////////////////////////////////////////
// PersonalStore.cpp
#include "stdafx.h"
#include "PersonalStore.h"
#include "user.h"
#include "GameMain.h"
#include "winutil.h"
#include "TLog.h"
#include "configread.h"
#include "NewPVP.h"
#include "ChaosCastle.h"
#include "GensSystem.h"
#include "PentagramSystem.h"
#include "DSProtocol.h"
#include "CashShop.h"
#include "MineSystem.h"
#include "BotSystem.h"

CPersonalStore g_PersonalStore;

CPersonalStore::CPersonalStore()
{

}

CPersonalStore::~CPersonalStore()
{

}

void CPersonalStore::CGPShopReqSetItemPrice(PMSG_REQ_PSHOP_SETITEMPRICE * lpMsg, int aIndex)
{

	if (gDoPShopOpen == FALSE)
	{
		CGPShopAnsSetItemPrice(aIndex, 0, lpMsg->btItemPos);
		return;
	}

	if (!gObjIsConnected(aIndex))
	{
		g_Log.Add("[PShop] ERROR : Index is not CONNECTED : %d", aIndex);
		return;
	}

	if (gObj[aIndex].Type != OBJ_USER)
	{
		g_Log.Add("[PShop] ERROR : Index is not CHARACTER : %d", aIndex);
		return;
	}

	if (gObj[aIndex].Level <= 5)
	{
		g_Log.Add("[PShop] [%s][%s] ERROR : Level is Under 6 : %d",
			gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].Level);
		CGPShopAnsSetItemPrice(aIndex, 5, lpMsg->btItemPos);

		return;
	}

	if (INVENTORY_RANGE(lpMsg->btItemPos) == FALSE)
	{
		g_Log.Add("[PShop] [%s][%s] ERROR : Item Position Out of Bound : %d",
			gObj[aIndex].AccountID, gObj[aIndex].Name, lpMsg->btItemPos);
		CGPShopAnsSetItemPrice(aIndex, 2, lpMsg->btItemPos);

		return;
	}

	if (gObj[aIndex].Inventory1[lpMsg->btItemPos].IsItem() == FALSE)
	{
		g_Log.Add("[PShop] [%s][%s] ERROR : Item Does Not Exist : %d",
			gObj[aIndex].AccountID, gObj[aIndex].Name, lpMsg->btItemPos);
		CGPShopAnsSetItemPrice(aIndex, 3, lpMsg->btItemPos);

		return;
	}

	CItem * sitem = &gObj[aIndex].Inventory1[lpMsg->btItemPos];
	int iItemPrice = MAKE_NUMBERDW(MAKE_NUMBERW(lpMsg->sItemPrice4, lpMsg->sItemPrice3), MAKE_NUMBERW(lpMsg->sItemPrice2, lpMsg->sItemPrice1));

	WORD wBlessPrice = MAKE_NUMBERW(lpMsg->sJewelOfBlessPrice2, lpMsg->sJewelOfBlessPrice1);
	WORD wSoulPrice = MAKE_NUMBERW(lpMsg->sJewelOfSoulPrice2, lpMsg->sJewelOfSoulPrice1);
	WORD wChaosPrice = MAKE_NUMBERW(lpMsg->sJewelOfChaosPrice2, lpMsg->sJewelOfChaosPrice1);

	if (gObjCheckSerial0ItemList(sitem) != FALSE)
	{
		MsgOutput(aIndex, Lang.GetText(0,259));
		GSProtocol.GCReFillSend(aIndex, gObj[aIndex].Life, 0xFD, 1, gObj[aIndex].iShield);

		g_Log.Add("[ANTI-HACK][Serial 0 Item] [PShop] (%s)(%s) Item(%s) Pos(%d)",
			gObj[aIndex].AccountID, gObj[aIndex].Name, sitem->GetName(), lpMsg->btItemPos);

		return;
	}

	if (gObjInventorySearchSerialNumber(&gObj[aIndex], sitem->GetNumber()) == FALSE)
	{
		GSProtocol.GCReFillSend(aIndex, gObj[aIndex].Life, 0xFD, 1, gObj[aIndex].iShield);
		g_Log.Add("error-L2 : CopyItem [%s][%s] return %s %d", gObj[aIndex].AccountID, gObj[aIndex].Name, __FILE__, __LINE__);

		return;
	}

	if (iItemPrice <= 0 && wBlessPrice <= 0 && wSoulPrice <= 0 && wChaosPrice <= 0)
	{
		g_Log.Add("[PShop] [%s][%s] ERROR : Personal Shop Item Price <= 0 : %d and sBlessJewelPrice <= 0 : %d and sSoulJewelPrice <= 0 : %d and sChaosJewelPrice <= 0 : %d ",
			gObj[aIndex].AccountID, gObj[aIndex].Name, sitem->m_iPShopValue, sitem->m_wPShopBlessValue, sitem->m_wPShopSoulValue, sitem->m_wPShopChaosValue);
		CGPShopAnsSetItemPrice(aIndex, 4, lpMsg->btItemPos);

		return;
	}

	if ((gObj[aIndex].Penalty & 4) == 4 || (gObj[aIndex].Penalty & 8) == 8)
	{
		g_Log.Add("[PShop] [%s][%s] ERROR : Item Price Change - Item Block", gObj[aIndex].AccountID, gObj[aIndex].Name, sitem->m_iPShopValue);	// Deathway fix Here - Last parameter can cause crash
		CGPShopAnsSetItemPrice(aIndex, 6, lpMsg->btItemPos);

		return;
	}

	sitem->m_iPShopValue = iItemPrice;
	sitem->m_wPShopBlessValue = wBlessPrice;
	sitem->m_wPShopSoulValue = wSoulPrice;
	sitem->m_wPShopChaosValue = wChaosPrice;

	ItemAttribute[sitem->m_Type].Name[31] = 0;	// Zero String Terminated

	g_Log.Add("[PShop] [%s][%s] Changing Item Price : %d, BlessJewelPrice : %d, SoulJewelPrice : %d, ChaosJewelPrice : %d (Type:%d (%s), Lev:%d, Serial:%I64d, Dur:%d",
		gObj[aIndex].AccountID, gObj[aIndex].Name, sitem->m_iPShopValue, sitem->m_wPShopBlessValue,
		sitem->m_wPShopSoulValue, sitem->m_wPShopChaosValue, sitem->m_Type,
		ItemAttribute[sitem->m_Type].Name, sitem->m_Level, sitem->m_Number,
		sitem->m_Durability);

	CGPShopAnsSetItemPrice(aIndex, 1, lpMsg->btItemPos);
}



void CPersonalStore::CGPShopAnsSetItemPrice(int aIndex, BYTE btResult, BYTE btItemPos)
{
	PMSG_ANS_PSHOP_SETITEMPRICE pMsg;

	PHeadSubSetB((LPBYTE)&pMsg, 0x3F, 0x01, sizeof(pMsg));
	pMsg.btResult = btResult;
	pMsg.btItemPos = btItemPos;

	IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}


void CPersonalStore::CGPShopReqOpen(PMSG_REQ_PSHOP_OPEN * lpMsg, int aIndex)
{
	if (::gDoPShopOpen == FALSE)
		return;

	if (!gObjIsConnected(aIndex))
	{
		g_Log.Add("[PShop] ERROR : Index is not CONNECTED : %d", aIndex);
		return;
	}

	if (gObj[aIndex].Type != OBJ_USER)
	{
		g_Log.Add("[PShop] ERROR : Index is not CHARACTER : %d", aIndex);
		return;
	}

	if (gObj[aIndex].m_PlayerData->m_bSecurityCheck == false)
	{
		MsgOutput(aIndex, Lang.GetText(0,394));
		this->CGPShopAnsOpen(aIndex, 3);
		return;
	}

	if (g_NewPVP.IsDuel(gObj[aIndex]))
	{
		GSProtocol.GCServerMsgStringSend(Lang.GetText(0,313), aIndex, 1);
		return;
	}

	if (g_NewPVP.IsObserver(gObj[aIndex]))
	{
		GSProtocol.GCServerMsgStringSend(Lang.GetText(0,314), aIndex, 1);
		return;
	}

	if (gObj[aIndex].Level <= 5)
	{
		g_Log.Add("[PShop] [%s][%s] ERROR : Level is Under 6 : %d",
			gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].Level);
		this->CGPShopAnsOpen(aIndex, 2);

		return;
	}

	if ((gObj[aIndex].Penalty & 4) == 4 || (gObj[aIndex].Penalty & 8) == 8)
	{
		g_Log.Add("[PShop] [%s][%s] ERROR : Cant't Open Shop - Item Block", gObj[aIndex].AccountID, gObj[aIndex].Name);
		this->CGPShopAnsOpen(aIndex, 3);

		return;
	}

	if (CC_MAP_RANGE(gObj[aIndex].MapNumber))
	{
		GSProtocol.GCServerMsgStringSend(Lang.GetText(0,115), aIndex, 1);
		this->CGPShopAnsOpen(aIndex, 0);
		return;
	}

	if (gObj[aIndex].MapNumber == MAP_INDEX_CHAOSCASTLE_SURVIVAL)
	{
		GSProtocol.GCServerMsgStringSend(Lang.GetText(0,563), aIndex, 1);
		this->CGPShopAnsOpen(aIndex, 0);
		return;
	}

	if (gObj[aIndex].m_PlayerData->m_JoinUnityBattle == TRUE || g_ConfigRead.server.GetServerType() == SERVER_BATTLECORE)
	{
		GSProtocol.GCServerMsgStringSend(Lang.GetText(0,621), aIndex, 1);
		this->CGPShopAnsOpen(aIndex, 0);
		return;
	}
	
	if (gObj[aIndex].m_PlayerData->m_bIsMining)
	{
		this->CGPShopAnsOpen(aIndex, 4);
		return;
	}

	if (g_MineSystem.IsEquipPickax(aIndex))
	{
		this->CGPShopAnsOpen(aIndex, 4);
		return;
	}

	if (g_GensSystem.IsMapBattleZone(gObj[aIndex].MapNumber))
	{
		this->CGPShopAnsOpen(aIndex, 0);
		return;
	}

	bool bEnablePShopOpen = false;

	if (gObj[aIndex].m_IfState.use == FALSE)
		bEnablePShopOpen = true;
	else
	{
		if (gObj[aIndex].m_IfState.use == TRUE)
		{
			switch (gObj[aIndex].m_IfState.type)
			{
			case 8:
				bEnablePShopOpen = true; break;
			}

			if (!bEnablePShopOpen)
			{
				g_Log.Add("[PShop] [%s][%s] ERROR : m_IfState.type is Using : %d", gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].m_IfState.type);
			}
		}
		else
		{
			g_Log.Add("[PShop] [%s][%s] ERROR : Unknown m_IfState.type : %d", gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].m_IfState.type);
			return;
		}
	}

	if (!::gObjFixInventoryPointer(aIndex))
	{
		g_Log.Add("[Fix Inv.Ptr] False Location - %s, %d", __FILE__, __LINE__);
	}

	if (gObj[aIndex].pTransaction == 1)
	{
		g_Log.Add("[PShop] [%s][%s] ERROR : Transaction == 1, IF_TYPE : %d", gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].m_IfState.type);
		bEnablePShopOpen = false;
	}

	if (bEnablePShopOpen)
	{
		if (gObj[aIndex].m_bPShopOpen == false)
		{
			gObj[aIndex].m_bPShopOpen = true;
			memcpy(gObj[aIndex].m_szPShopText, lpMsg->szPShopText, sizeof(lpMsg->szPShopText));
			this->GDAllSavePShopItemValue(&gObj[aIndex]);
			g_Log.Add("[PShop] [%s][%s] Personal Shop Opened", gObj[aIndex].AccountID, gObj[aIndex].Name);
		}
		else
		{
			memcpy(gObj[aIndex].m_szPShopText, lpMsg->szPShopText, sizeof(lpMsg->szPShopText));

			PMSG_ANS_PSHOP_TEXT_CHANGED pMsg;

			PHeadSubSetB((LPBYTE)&pMsg, 0x3F, 0x10, sizeof(pMsg));
			pMsg.NumberH = SET_NUMBERH(aIndex);
			pMsg.NumberL = SET_NUMBERL(aIndex);
			memcpy(pMsg.btPShopText, lpMsg->szPShopText, sizeof(pMsg.btPShopText));
			memcpy(pMsg.btName, gObj[aIndex].Name, sizeof(pMsg.btName));

			GSProtocol.MsgSendV2(&gObj[aIndex], (LPBYTE)&pMsg, pMsg.h.size);

			g_Log.Add("[PShop] [%s][%s] Personal Shop Already Opened - Changing PShop Name",
				gObj[aIndex].AccountID, gObj[aIndex].Name);
		}
	}
	else
	{
		CGPShopAnsOpen(aIndex, 0);
		return;
	}

	CGPShopAnsOpen(aIndex, 1);
}

void CPersonalStore::CGPShopAnsOpen(int aIndex, BYTE btResult)
{
	PMSG_ANS_PSHOP_OPEN pMsg;

	PHeadSubSetB((LPBYTE)&pMsg, 0x3F, 0x02, sizeof(pMsg));
	pMsg.btResult = btResult;

	IOCP.DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));
}



void CPersonalStore::CGPShopReqClose(int aIndex)
{
	if (!gObjIsConnected(aIndex))
	{
		g_Log.Add("[PShop] ERROR : Index is not CONNECTED : %d", aIndex);
		return;
	}

	if (gObj[aIndex].Type != OBJ_USER)
	{
		g_Log.Add("[PShop] ERROR : Index is not CHARACTER : %d", aIndex);
		return;
	}

	if (g_NewPVP.IsDuel(gObj[aIndex]))
	{
		GSProtocol.GCServerMsgStringSend(Lang.GetText(0,313), aIndex, 1);
		return;
	}

	if (g_NewPVP.IsObserver(gObj[aIndex]))
	{
		GSProtocol.GCServerMsgStringSend(Lang.GetText(0,314), aIndex, 1);
		return;
	}

	g_Log.Add("[PShop] [%s][%s] Requested to Close PShop", gObj[aIndex].AccountID, gObj[aIndex].Name);
	gObj[aIndex].m_bPShopOpen = false;
	memset(gObj[aIndex].m_szPShopText, 0, sizeof(gObj[aIndex].m_szPShopText));
	CGPShopAnsClose(aIndex, 1);
}

void CPersonalStore::CGPShopAnsClose(int aIndex, BYTE btResult)
{
	g_Log.Add("[PShop] [%s][%s] Close PShop", gObj[aIndex].AccountID, gObj[aIndex].Name);

	PMSG_ANS_PSHOP_CLOSE pMsg;

	PHeadSubSetB((LPBYTE)&pMsg, 0x3F, 0x03, sizeof(pMsg));
	pMsg.btResult = btResult;
	pMsg.NumberH = SET_NUMBERH(aIndex);
	pMsg.NumberL = SET_NUMBERL(aIndex);

	IOCP.DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));

	if (btResult == 1)
		GSProtocol.MsgSendV2(&gObj[aIndex], (LPBYTE)&pMsg, pMsg.h.size);
}



void CPersonalStore::CGPShopReqBuyList(PMSG_REQ_BUYLIST_FROM_PSHOP * lpMsg, int aSourceIndex)
{
	if (::gDoPShopOpen == FALSE)
		return;

	if (!ObjectMaxRange(MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL)))
		return;

	if (gObjIsConnected(MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL)) == FALSE)
	{
		g_Log.Add("[PShop] [%s][%s] PShop List Request Failed : Seller did Not CONNECTED (%d)",
			gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name, MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL));
		this->CGPShopAnsBuyList(aSourceIndex, -1, 2, 0);
		return;
	}

	LPOBJ lpObj = &gObj[MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL)];


	if(lpObj->m_bOffLevel == true)
	{
		this->CGPShopAnsBuyList(aSourceIndex, -1, 2, 0);
		return;
	}
	if (lpObj->Type != OBJ_USER)
	{
		g_Log.Add("[PShop] [%s][%s] PShop List Request Failed : Seller is Not CHARACTER (%d)"
			, gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name, MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL));
		this->CGPShopAnsBuyList(aSourceIndex, -1, 2, 0);
		return;
	}

	if (gObj[aSourceIndex].Type != OBJ_USER)
	{
		g_Log.Add("[PShop] [%s][%s] PShop List Request Failed : Buyer is Not CHARACTER (%d)"
			, gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name, MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL));
		this->CGPShopAnsBuyList(aSourceIndex, -1, 2, 0);
		return;
	}

	if(lpObj->m_PlayerData->ISBOT == true && g_BotSystem.GetBotType(lpObj->m_Index) == BOT_SHOP)
	{
		GSProtocol.GCAlterPShopVault(aSourceIndex,g_BotSystem.m_BotData[lpObj->m_PlayerData->wBotIndex].m_Shop.iCoinType);
	}
	else if(lpObj->m_bOff)
	{
		GSProtocol.GCAlterPShopVault(aSourceIndex,g_ConfigRead.offtrade.CoinType);
	}
	else
	{
		GSProtocol.GCAlterPShopVault(aSourceIndex,0);
	}
	if (lpObj->CloseCount >= 0)
	{
		g_Log.Add("[PShop] [%s][%s] PShop Item Buy Request Failed : Seller is Closing (%d)"
			, gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name, MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL));
		this->CGPShopAnsBuyItem(aSourceIndex, -1, 0, 2);
		return;
	}

	if (gObj[aSourceIndex].CloseCount >= 0)
	{
		g_Log.Add("[PShop] [%s][%s] PShop Item Buy Request Failed : Buyer is Closing (%d)",
			gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name, MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL));
		this->CGPShopAnsBuyItem(aSourceIndex, -1, 0, 2);
		return;
	}

	if (g_NewPVP.IsDuel(gObj[aSourceIndex]))
	{
		GSProtocol.GCServerMsgStringSend(Lang.GetText(0,313), aSourceIndex, 1);
		return;
	}

	if (g_NewPVP.IsObserver(gObj[aSourceIndex]))
	{
		GSProtocol.GCServerMsgStringSend(Lang.GetText(0,314), aSourceIndex, 1);
		return;
	}

	if (gObj[aSourceIndex].m_bMapSvrMoveQuit == true || gObj[aSourceIndex].m_bMapSvrMoveReq == true || gObj[aSourceIndex].m_bMapSvrMoveReq_2 == true ||
		lpObj->m_bMapSvrMoveQuit == true || lpObj->m_bMapSvrMoveReq == true || lpObj->m_bMapSvrMoveReq_2 == true)
	{
		g_Log.Add("[PShop][SuspectCheating] Map Of Move request was trying to Shop List. Buyer ID[%s]Name[%s] Seller Name[%s]",
			gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name, lpObj->Name);

		return;
	}

	if (gObj[aSourceIndex].m_PlayerData->m_JoinUnityBattle == TRUE ||
		lpObj->m_PlayerData->m_JoinUnityBattle == TRUE ||
		g_ConfigRead.server.GetServerType() == SERVER_BATTLECORE)
	{
		GSProtocol.GCServerMsgStringSend("Not Available in Battle Core", aSourceIndex, 1);
		return;
	}

	if (lpObj->m_Index == aSourceIndex)
	{
		g_Log.Add("[PShop] [%s][%s] PShop List Requested to Him(/Her)Self",
			gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name);
		this->CGPShopAnsBuyList(aSourceIndex, -1, 2, 0);
		return;
	}

	if (lpObj->m_bPShopOpen == false)
	{
		g_Log.Add("[PShop] [%s][%s] PShop List Request Failed : [%s][%s] Did not Open PShop",
			gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name,
			lpObj->AccountID, lpObj->Name);
		this->CGPShopAnsBuyList(aSourceIndex, -1, 3, 0);
		return;
	}

	if ((lpObj->Penalty & 4) == 4 || (lpObj->Penalty & 8) == 8)
	{
		g_Log.Add("[PShop] [%s][%s] PShop List Request Failed : [%s][%s] is in Item Block",
			gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name,
			lpObj->AccountID, lpObj->Name);
		this->CGPShopAnsBuyList(aSourceIndex, -1, 4, 0);
		return;
	}

	if (!::gObjFixInventoryPointer(aSourceIndex))
	{
		g_Log.Add("[Fix Inv.Ptr] False Location - %s, %d", __FILE__, __LINE__);
	}

	if (gObj[aSourceIndex].pTransaction == 1)
	{
		g_Log.Add("[PShop] [%s][%s] PShop List Request Failed : Requester Transaction == 1, IF_TYPE : %d",
			gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name, gObj[aSourceIndex].m_IfState.type);
		return;
	}

	g_Log.Add("[PShop] [%s][%s] is Receiving PShop List From [%s][%s]",
		gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name,
		lpObj->AccountID, lpObj->Name);

	gObj[aSourceIndex].m_bPShopWantDeal = true;
	gObj[aSourceIndex].m_iPShopDealerIndex = lpObj->m_Index;
	memcpy(gObj[aSourceIndex].m_szPShopDealerName, lpObj->Name, MAX_ACCOUNT_LEN);
	this->CGPShopAnsBuyList(aSourceIndex, lpObj->m_Index, 1, false);


}

void CPersonalStore::CGPShopAnsBuyList(int aSourceIndex, int aTargetIndex, BYTE btResult, bool bResend)
{
	if (gDoPShopOpen == FALSE)
		return;

	PMSG_ANS_BUYLIST_FROM_PSHOP pMsgILC;	// Packet Message Item List Count
	PMSG_BUYLIST_FROM_PSHOP pMsgIL;
	BYTE sendBuf[1024];
	int sOfs = sizeof(pMsgILC);
	int pMsgILSize = sizeof(pMsgIL);

	if (btResult != 1)
	{
		LPOBJ lpObj = &gObj[aSourceIndex];

		pMsgILC.h.c = 0xC2;
		pMsgILC.h.headcode = 0x3F;

		if (bResend == false)
			pMsgILC.h.subcode = 0x05;
		else
			pMsgILC.h.subcode = 0x13;

		pMsgILC.h.sizeH = SET_NUMBERH(sOfs);
		pMsgILC.h.sizeL = SET_NUMBERL(sOfs);
		pMsgILC.btCount = 0;
		pMsgILC.Result = btResult;
		pMsgILC.NumberH = SET_NUMBERH(aTargetIndex);
		pMsgILC.NumberL = SET_NUMBERL(aTargetIndex);
		memset(pMsgILC.btName, 0, sizeof(pMsgILC.btName));
		memset(pMsgILC.szPShopText, 0, sizeof(pMsgILC.szPShopText));
		memcpy(sendBuf, &pMsgILC, sizeof(pMsgILC));

		IOCP.DataSend(aSourceIndex, sendBuf, sOfs);
	}
	else
	{
		LPOBJ lpObjSource = &gObj[aSourceIndex];
		LPOBJ lpObjTarget = &gObj[aTargetIndex];
		int iCount = 0;

		for (int n = PSHOP_START_RANGE; n<PSHOP_END_RANGE; n++)
		{
			if (lpObjTarget->Inventory1[n].IsItem() == TRUE)
			{
				pMsgIL.Pos = n;
				ItemByteConvert(pMsgIL.ItemInfo, lpObjTarget->Inventory1[n]);
				pMsgIL.PShopItemValue = lpObjTarget->Inventory1[n].m_iPShopValue;
				pMsgIL.BlessValue = lpObjTarget->Inventory1[n].m_wPShopBlessValue;
				pMsgIL.ChaosValue = lpObjTarget->Inventory1[n].m_wPShopChaosValue;
				pMsgIL.SoulValue = lpObjTarget->Inventory1[n].m_wPShopSoulValue;
				memcpy(&sendBuf[sOfs], &pMsgIL, pMsgILSize);
				iCount++;
				sOfs += pMsgILSize;

				g_Log.Add("ITEM TYPE: %d", lpObjTarget->Inventory1[n].m_Type);
			}
		}

		pMsgILC.h.c = 0xC2;
		pMsgILC.h.headcode = 0x3F;

		if (bResend == false)
			pMsgILC.h.subcode = 0x05;
		else
			pMsgILC.h.subcode = 0x13;

		pMsgILC.h.sizeH = SET_NUMBERH(sOfs);
		pMsgILC.h.sizeL = SET_NUMBERL(sOfs);
		pMsgILC.btCount = iCount;
		pMsgILC.Result = btResult;
		pMsgILC.NumberH = SET_NUMBERH(aTargetIndex);
		pMsgILC.NumberL = SET_NUMBERL(aTargetIndex);
		memcpy(pMsgILC.btName, lpObjTarget->Name, sizeof(pMsgILC.btName));
		memcpy(pMsgILC.szPShopText, lpObjTarget->m_szPShopText, sizeof(pMsgILC.szPShopText));
		memcpy(sendBuf, &pMsgILC, sizeof(pMsgILC));

		IOCP.DataSend(aSourceIndex, sendBuf, sOfs);

		g_PentagramSystem.GCPShopPentagramJewelViewInfo(aTargetIndex, aSourceIndex);

	}
}

void CPersonalStore::CGPShopReqBuyItem(LPBYTE lpRecv, int aSourceIndex)
{
	if (gDoPShopOpen == FALSE)
		return;

	PMSG_REQ_BUYITEM_FROM_PSHOP * lpMsg = (PMSG_REQ_BUYITEM_FROM_PSHOP *)lpRecv;

	if (gObjIsConnected(MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL)) == FALSE)
	{
		g_Log.Add("[PShop] [%s][%s] PShop Item Buy Request Failed : Seller did Not CONNECTED (%d)",
			gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name, MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL));
		g_PersonalStore.CGPShopAnsBuyItem(aSourceIndex, -1, 0, 2);
		return;
	}

	LPOBJ lpObj = &gObj[MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL)];

	if (lpObj->Type != OBJ_USER)
	{
		g_Log.Add("[PShop] [%s][%s] PShop Item Buy Request Failed : Seller is Not CHARACTER (%d)",
			gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name, MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL));
		g_PersonalStore.CGPShopAnsBuyItem(aSourceIndex, -1, 0, 2);
		return;
	}

	if (lpObj->CloseCount >= 0)
	{
		g_Log.Add("[PShop] [%s][%s] PShop Item Buy Request Failed : Seller is Closing (%d)",
			gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name, MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL));
		g_PersonalStore.CGPShopAnsBuyItem(aSourceIndex, -1, 0, 2);
		return;
	}

	if (gObj[aSourceIndex].CloseCount >= 0)
	{
		g_Log.Add("[PShop] [%s][%s] PShop Item Buy Request Failed : Buyer is Closing (%d)",
			gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name, MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL));
		g_PersonalStore.CGPShopAnsBuyItem(aSourceIndex, -1, 0, 2);
		return;
	}

	if (g_NewPVP.IsDuel(gObj[aSourceIndex]))
	{
		GSProtocol.GCServerMsgStringSend(Lang.GetText(0,313), aSourceIndex, 1);
		return;
	}

	if (g_NewPVP.IsObserver(gObj[aSourceIndex]))
	{
		GSProtocol.GCServerMsgStringSend(Lang.GetText(0,314), aSourceIndex, 1);
		return;
	}

	if (lpObj->m_bPShopOpen == false)
	{
		g_Log.Add("[PShop] [%s][%s] PShop Item Buy Request Failed : [%s][%s] Did not Open PShop",
			gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name,
			lpObj->AccountID, lpObj->Name);
		g_PersonalStore.CGPShopAnsBuyItem(aSourceIndex, lpObj->m_Index, 0, 3);
		return;
	}

	char szName[MAX_ACCOUNT_LEN + 1] = { 0 };
	memcpy(szName, lpMsg->btName, sizeof(lpMsg->btName));
	szName[MAX_ACCOUNT_LEN] = 0;
	int iITEM_LOG_TYPE;
	int iITEM_LOG_LEVEL;
	int iITEM_LOG_DUR;
	int iITEM_LOG_SERIAL;
	int iITEM_LOG_BJPRICE;
	int iITEM_LOG_SJPRICE;
	int iITEM_LOG_CJPRICE;

	if (strcmp(szName, lpObj->Name))
	{
		g_Log.Add("[PShop] [%s][%s] PShop Item Buy Request Failed : Name Mismatch [%s] - [%s]",
			gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name,
			szName, lpObj->Name);
		g_PersonalStore.CGPShopAnsBuyItem(aSourceIndex, lpObj->m_Index, 0, 6);

		return;
	}

	if (lpObj->CloseType != -1)
	{
		g_Log.Add("[PShop] [%s][%s] PShop Item Buy Request Failed : [%s][%s] is Closing Connection",
			gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name,
			lpObj->AccountID, lpObj->Name);
		g_PersonalStore.CGPShopAnsBuyItem(aSourceIndex, lpObj->m_Index, 0, 2);

		return;
	}

	if ((lpObj->Penalty & 4) == 4 || (lpObj->Penalty & 8) == 8)
	{
		g_Log.Add("[PShop] [%s][%s] PShop Item Buy Request Failed : [%s][%s] is in Item Block",
			gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name,
			lpObj->AccountID, lpObj->Name);
		g_PersonalStore.CGPShopAnsBuyItem(aSourceIndex, lpObj->m_Index, 0, 9);

		return;
	}

	if ((gObj[aSourceIndex].Penalty & 4) == 4 || (gObj[aSourceIndex].Penalty & 8) == 8)
	{
		g_Log.Add("[PShop] [%s][%s] PShop Item Buy Request Failed : [%s][%s] is in Item Block",
			gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name,
			lpObj->AccountID, lpObj->Name);
		g_PersonalStore.CGPShopAnsBuyItem(aSourceIndex, lpObj->m_Index, 0, 9);

		return;
	}

	EnterCriticalSection(&lpObj->m_critPShopTrade);

	if (lpObj->m_bPShopTransaction == true)
	{
		g_Log.Add("[PShop] [%s][%s] PShop Item Buy Request Failed : [%s][%s] Already Trade With Other",
			gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name,
			lpObj->AccountID, lpObj->Name);
		g_PersonalStore.CGPShopAnsBuyItem(aSourceIndex, lpObj->m_Index, 0, 4);
		LeaveCriticalSection(&lpObj->m_critPShopTrade);

	}
	else
	{
		lpObj->m_bPShopTransaction = true;
		LeaveCriticalSection(&lpObj->m_critPShopTrade);

		if (gObjCheckSerial0ItemList(&lpObj->Inventory1[lpMsg->btItemPos]) != FALSE)
		{
			MsgOutput(lpObj->m_Index, Lang.GetText(0,259));
			GSProtocol.GCReFillSend(aSourceIndex, gObj[aSourceIndex].Life, 0xFD, 1, gObj[aSourceIndex].iShield);

			g_Log.Add("[ANTI-HACK][Serial 0 Item] [PShop Buy] (%s)(%s) Item(%s) Pos(%d)",
				lpObj->AccountID, lpObj->Name, lpObj->Inventory1[lpMsg->btItemPos].GetName(), lpMsg->btItemPos);

			return;
		}

		if (::gObjInventorySearchSerialNumber(&gObj[aSourceIndex], lpObj->Inventory1[lpMsg->btItemPos].GetNumber()) == FALSE)
		{
			GSProtocol.GCReFillSend(aSourceIndex, gObj[aSourceIndex].Life, 0xFD, 1, gObj[aSourceIndex].iShield);
			g_Log.Add("error-L2 : CopyItem [%s][%s] return %s %d", gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name,
				__FILE__, __LINE__);

			return;
		}

		if (::gObjInventorySearchSerialNumber(lpObj, lpObj->Inventory1[lpMsg->btItemPos].GetNumber()) == FALSE)
		{
			GSProtocol.GCReFillSend(lpObj->m_Index, lpObj->Life, 0xFD, 1, lpObj->iShield);
			g_Log.Add("error-L2 : CopyItem [%s][%s] return %s %d",
				lpObj->AccountID, lpObj->Name,
				__FILE__, __LINE__);

			return;
		}

		if (g_ConfigRead.data.common.BlockTradeFFFFFFFFItems == true)
		{
			if (lpObj->Inventory1[lpMsg->btItemPos].m_Number == (UINT64)-1)
			{
				GSProtocol.GCReFillSend(lpObj->m_Index, lpObj->Life, 0xFD, 1, lpObj->iShield);
				//MsgOutput(lpObj->m_Index, Lang.GetText(0,601));
				return;
			}
		}

		if (gObjFixInventoryPointer(aSourceIndex) == false)
		{
			g_Log.Add("[Fix Inv.Ptr] False Location - %s, %d",
				__FILE__, __LINE__);
		}

		if (gObj[aSourceIndex].pTransaction == 1)
		{
			g_Log.Add("[PShop] [%s][%s] PShop Item Buy Request Failed : Requester Transaction == 1, IF_TYPE : %d",
				gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name, gObj[aSourceIndex].m_IfState.type);
			return;
		}

		iITEM_LOG_TYPE = 0;
		iITEM_LOG_LEVEL = 0;
		iITEM_LOG_DUR = 0;
		iITEM_LOG_SERIAL = 0;

		__try
		{
			if (PSHOP_RANGE(lpMsg->btItemPos) == FALSE)
			{
				g_PersonalStore.CGPShopAnsBuyItem(aSourceIndex, lpObj->m_Index, 0, 5);
				return;
			}

			if (lpObj->m_bMapSvrMoveQuit == true)
			{
				g_PersonalStore.CGPShopAnsBuyItem(aSourceIndex, -1, 0, 2);
				return;
			}

			if (lpObj->Inventory1[lpMsg->btItemPos].IsItem() == TRUE)
			{
				if (lpObj->Inventory1[lpMsg->btItemPos].m_iPShopValue <= 0 && lpObj->Inventory1[lpMsg->btItemPos].m_wPShopBlessValue <= 0
					&& lpObj->Inventory1[lpMsg->btItemPos].m_wPShopSoulValue <= 0 && lpObj->Inventory1[lpMsg->btItemPos].m_wPShopChaosValue <= 0)
				{
					g_Log.Add("[PShop] [%s][%s] PShop Item Buy Request Failed : [%s][%s] Did Not Choose Item Price",
						gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name,
						lpObj->AccountID, lpObj->Name);
					g_PersonalStore.CGPShopAnsBuyItem(aSourceIndex, lpObj->m_Index, 0, 6);

					return;
				}

				if (lpObj->m_bOff == false && lpObj->m_PlayerData->ISBOT == false)
				{
					if (gObj[aSourceIndex].m_PlayerData->Money < lpObj->Inventory1[lpMsg->btItemPos].m_iPShopValue)
					{
						g_Log.Add("[PShop] [%s][%s] PShop Item Buy Request Failed : [%s][%s] Lack of Zen",
							gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name,
							lpObj->AccountID, lpObj->Name);
						g_PersonalStore.CGPShopAnsBuyItem(aSourceIndex, lpObj->m_Index, 0, 7);

						return;
					}
				}
				else
				{
					if (!lpObj->m_bOff && lpObj->m_PlayerData->ISBOT == false)
					{
						if (gObj[aSourceIndex].m_PlayerData->Money < lpObj->Inventory1[lpMsg->btItemPos].m_iPShopValue)
						{
							g_Log.Add("[PShop] [%s][%s] PShop Item Buy Request Failed : [%s][%s] Lack of Zen",
								gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name,
								lpObj->AccountID, lpObj->Name);
							g_PersonalStore.CGPShopAnsBuyItem(aSourceIndex, lpObj->m_Index, 0, 7);

							return;
						}
					}
					else if(lpObj->m_bOff && lpObj->m_PlayerData->ISBOT == false)
					{
						switch (g_ConfigRead.offtrade.CoinType)
						{
						case 0:
						{
							if (gObj[aSourceIndex].m_PlayerData->Money < lpObj->Inventory1[lpMsg->btItemPos].m_iPShopValue)
							{
								g_Log.Add("[PShop] [%s][%s] PShop Item Buy Request Failed : [%s][%s] Lack of Zen",
									gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name,
									lpObj->AccountID, lpObj->Name);
								g_PersonalStore.CGPShopAnsBuyItem(aSourceIndex, lpObj->m_Index, 0, 7);

								return;
							}
						}
							break;
						case 1:
						{
							if (gObj[aSourceIndex].m_PlayerData->m_WCoinC < lpObj->Inventory1[lpMsg->btItemPos].m_iPShopValue)
							{
								g_Log.Add("[PShop] [%s][%s] PShop Item Buy Request Failed : [%s][%s] Lack of Zen",
									gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name,
									lpObj->AccountID, lpObj->Name);
								g_PersonalStore.CGPShopAnsBuyItem(aSourceIndex, lpObj->m_Index, 0, 7);

								return;
							}
						}
							break;
						case 2:
						{
							if (gObj[aSourceIndex].m_PlayerData->m_WCoinP < lpObj->Inventory1[lpMsg->btItemPos].m_iPShopValue)
							{
								g_Log.Add("[PShop] [%s][%s] PShop Item Buy Request Failed : [%s][%s] Lack of Zen",
									gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name,
									lpObj->AccountID, lpObj->Name);
								g_PersonalStore.CGPShopAnsBuyItem(aSourceIndex, lpObj->m_Index, 0, 7);

								return;
							}
						}
							break;
						case 3:
						{
							if (gObj[aSourceIndex].m_PlayerData->m_GoblinPoint < lpObj->Inventory1[lpMsg->btItemPos].m_iPShopValue)
							{
								g_Log.Add("[PShop] [%s][%s] PShop Item Buy Request Failed : [%s][%s] Lack of Zen",
									gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name,
									lpObj->AccountID, lpObj->Name);
								g_PersonalStore.CGPShopAnsBuyItem(aSourceIndex, lpObj->m_Index, 0, 7);

								return;
							}
						}
							break;
						}
					}
					else
					{
						switch (g_BotSystem.m_BotData[lpObj->m_PlayerData->wBotIndex].m_Shop.iCoinType)
						{
						case 0:
							{
								if (gObj[aSourceIndex].m_PlayerData->Money < lpObj->Inventory1[lpMsg->btItemPos].m_iPShopValue)
								{
									g_Log.Add("[PShop] [%s][%s] PShop Item Buy Request Failed : [%s][%s] Lack of Zen",
										gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name,
										lpObj->AccountID, lpObj->Name);
									g_PersonalStore.CGPShopAnsBuyItem(aSourceIndex, lpObj->m_Index, 0, 7);

									return;
								}
							}
							break;
						case 1:
							{
								if (gObj[aSourceIndex].m_PlayerData->m_WCoinC < lpObj->Inventory1[lpMsg->btItemPos].m_iPShopValue)
								{
									g_Log.Add("[PShop] [%s][%s] PShop Item Buy Request Failed : [%s][%s] Lack of Zen",
										gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name,
										lpObj->AccountID, lpObj->Name);
									g_PersonalStore.CGPShopAnsBuyItem(aSourceIndex, lpObj->m_Index, 0, 7);

									return;
								}
							}
							break;
						case 2:
							{
								if (gObj[aSourceIndex].m_PlayerData->m_WCoinP < lpObj->Inventory1[lpMsg->btItemPos].m_iPShopValue)
								{
									g_Log.Add("[PShop] [%s][%s] PShop Item Buy Request Failed : [%s][%s] Lack of Zen",
										gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name,
										lpObj->AccountID, lpObj->Name);
									g_PersonalStore.CGPShopAnsBuyItem(aSourceIndex, lpObj->m_Index, 0, 7);

									return;
								}
							}
							break;
						case 3:
							{
								if (gObj[aSourceIndex].m_PlayerData->m_GoblinPoint < lpObj->Inventory1[lpMsg->btItemPos].m_iPShopValue)
								{
									g_Log.Add("[PShop] [%s][%s] PShop Item Buy Request Failed : [%s][%s] Lack of Zen",
										gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name,
										lpObj->AccountID, lpObj->Name);
									g_PersonalStore.CGPShopAnsBuyItem(aSourceIndex, lpObj->m_Index, 0, 7);

									return;
								}
							}
							break;
								}
					}
				}

				if (lpObj->Inventory1[lpMsg->btItemPos].m_Type != lpMsg->iItemType
					|| lpObj->Inventory1[lpMsg->btItemPos].m_iPShopValue != lpMsg->PShopItemMoneyValue
					|| lpObj->Inventory1[lpMsg->btItemPos].m_wPShopBlessValue != lpMsg->PShopItemBlessJewelValue
					|| lpObj->Inventory1[lpMsg->btItemPos].m_wPShopSoulValue != lpMsg->PShopItemSoulJewelValue
					|| lpObj->Inventory1[lpMsg->btItemPos].m_wPShopChaosValue != lpMsg->PShopItemChaosJewelValue
					)
				{
					g_Log.Add("[PShop] [%s][%s] PShop Item Buy Request Failed : [%s][%s] Client Item Different than Server Item (Server - ItemType:%d, Pos:%d, MV:%d, BJV:%d, SJV:%d, CJV:%d), (Client - ItemType:%d, Pos:%d, MV:%d, BJV:%d, SJV:%d, CJV:%d)",
						lpObj->AccountID, lpObj->Name, gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name,
						lpObj->Inventory1[lpMsg->btItemPos].m_Type, lpMsg->btItemPos, lpObj->Inventory1[lpMsg->btItemPos].m_iPShopValue, lpObj->Inventory1[lpMsg->btItemPos].m_wPShopBlessValue, lpObj->Inventory1[lpMsg->btItemPos].m_wPShopSoulValue, lpObj->Inventory1[lpMsg->btItemPos].m_wPShopChaosValue,
						lpMsg->iItemType, lpMsg->btItemPos, lpMsg->PShopItemMoneyValue, lpMsg->PShopItemBlessJewelValue, lpMsg->PShopItemSoulJewelValue, lpMsg->PShopItemChaosJewelValue);
					g_PersonalStore.CGPShopAnsBuyItem(aSourceIndex, lpObj->m_Index, 0, 18);

					return;
				}

				short sBlessJewelCount = 0;
				short s10BlessJewelCount = 0;
				short s20BlessJewelCount = 0;
				short s30BlessJewelCount = 0;
				short sSoulJewelCount = 0;
				short s10SoulJewelCount = 0;
				short s20SoulJewelCount = 0;
				short s30SoulJewelCount = 0;
				short sChaosJewelCount = 0;
				short s10ChaosJewelCount = 0;
				short s20ChaosJewelCount = 0;
				short s30ChaosJewelCount = 0;
				short sBlessJewelTotalCount = 0;
				short sSoulJewelTotalCount = 0;
				short sChaosJewelTotalCount = 0;

				if (gCountBlessSoulChaosJewel(aSourceIndex, sBlessJewelCount, s10BlessJewelCount, s20BlessJewelCount, s30BlessJewelCount,
					sSoulJewelCount, s10SoulJewelCount, s20SoulJewelCount, s30SoulJewelCount,
					sChaosJewelCount, s10ChaosJewelCount, s20ChaosJewelCount, s30ChaosJewelCount))
				{
					sBlessJewelTotalCount = 30 * s30BlessJewelCount + 20 * s20BlessJewelCount + 10 * s10BlessJewelCount + sBlessJewelCount;
					sSoulJewelTotalCount = 30 * s30SoulJewelCount + 20 * s20SoulJewelCount + 10 * s10SoulJewelCount + sSoulJewelCount;
					sChaosJewelTotalCount = 30 * s30ChaosJewelCount + 20 * s20ChaosJewelCount + 10 * s10ChaosJewelCount + sChaosJewelCount;

					if (sBlessJewelTotalCount < lpObj->Inventory1[lpMsg->btItemPos].m_wPShopBlessValue)
					{
						g_Log.Add("[PShop] [%s][%s] PShop Item Buy Request Failed : [%s][%s] Lack of BlessJewel",
							gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name, lpObj->AccountID, lpObj->Name);
						g_PersonalStore.CGPShopAnsBuyItem(aSourceIndex, lpObj->m_Index, 0, 11);

						return;
					}

					if (sSoulJewelTotalCount < lpObj->Inventory1[lpMsg->btItemPos].m_wPShopSoulValue)
					{
						g_Log.Add("[PShop] [%s][%s] PShop Item Buy Request Failed : [%s][%s] Lack of SoulJewel",
							gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name, lpObj->AccountID, lpObj->Name);
						g_PersonalStore.CGPShopAnsBuyItem(aSourceIndex, lpObj->m_Index, 0, 12);

						return;
					}

					if (sChaosJewelTotalCount < lpObj->Inventory1[lpMsg->btItemPos].m_wPShopChaosValue)
					{
						g_Log.Add("[PShop] [%s][%s] PShop Item Buy Request Failed : [%s][%s] Lack of ChaosJewel",
							gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name, lpObj->AccountID, lpObj->Name);
						g_PersonalStore.CGPShopAnsBuyItem(aSourceIndex, lpObj->m_Index, 0, 13);

						return;
					}
				}

				short sBlessJewelNeedCount = 0;
				short s10BlessJewelNeedCount = 0;
				short s20BlessJewelNeedCount = 0;
				short s30BlessJewelNeedCount = 0;
				short sSoulJewelNeedCount = 0;
				short s10SoulJewelNeedCount = 0;
				short s20SoulJewelNeedCount = 0;
				short s30SoulJewelNeedCount = 0;
				short sChaosJewelNeedCount = 0;
				short s10ChaosJewelNeedCount = 0;
				short s20ChaosJewelNeedCount = 0;
				short s30ChaosJewelNeedCount = 0;

				if (lpObj->Inventory1[lpMsg->btItemPos].m_wPShopBlessValue > 0
					|| lpObj->Inventory1[lpMsg->btItemPos].m_wPShopSoulValue > 0
					|| lpObj->Inventory1[lpMsg->btItemPos].m_wPShopChaosValue > 0)
				{
					int iInvalidValueTestResult = gJewelBuyItemValueTest(aSourceIndex, lpObj->m_Index,
						lpObj->Inventory1[lpMsg->btItemPos].m_wPShopBlessValue, lpObj->Inventory1[lpMsg->btItemPos].m_wPShopSoulValue, lpObj->Inventory1[lpMsg->btItemPos].m_wPShopChaosValue,
						sBlessJewelCount, s10BlessJewelCount, s20BlessJewelCount, s30BlessJewelCount,
						sSoulJewelCount, s10SoulJewelCount, s20SoulJewelCount, s30SoulJewelCount,
						sChaosJewelCount, s10ChaosJewelCount, s20ChaosJewelCount, s30ChaosJewelCount,
						sBlessJewelNeedCount, s10BlessJewelNeedCount, s20BlessJewelNeedCount, s30BlessJewelNeedCount,
						sSoulJewelNeedCount, s10SoulJewelNeedCount, s20SoulJewelNeedCount, s30SoulJewelNeedCount,
						sChaosJewelNeedCount, s10ChaosJewelNeedCount, s20ChaosJewelNeedCount, s30ChaosJewelNeedCount);

					if (iInvalidValueTestResult == 14)
					{
						g_Log.Add("[PShop] [%s][%s] PShop Item Buy Request Failed : [%s][%s] Not Share BlessJewel",
							gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name, lpObj->AccountID, lpObj->Name);
						g_PersonalStore.CGPShopAnsBuyItem(aSourceIndex, lpObj->m_Index, 0, 14);

						return;
					}

					if (iInvalidValueTestResult == 15)
					{
						g_Log.Add("[PShop] [%s][%s] PShop Item Buy Request Failed : [%s][%s] Not Share SoulJewel",
							gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name, lpObj->AccountID, lpObj->Name);
						g_PersonalStore.CGPShopAnsBuyItem(aSourceIndex, lpObj->m_Index, 0, 15);

						return;
					}

					if (iInvalidValueTestResult == 16)
					{
						g_Log.Add("[PShop] [%s][%s] PShop Item Buy Request Failed : [%s][%s] Not Share ChaosJewel",
							gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name, lpObj->AccountID, lpObj->Name);
						g_PersonalStore.CGPShopAnsBuyItem(aSourceIndex, lpObj->m_Index, 0, 16);

						return;
					}
				}

				DWORD dwCost = lpObj->Inventory1[lpMsg->btItemPos].m_iPShopValue;

				if (dwCost > 0 && gObjCheckMaxZen(lpObj->m_Index, dwCost) == FALSE)
				{
					g_Log.Add("[PShop] [%s][%s] PShop Item Buy Request Failed : [%s][%s] Exceeding Zen of the Host",
						gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name,
						lpObj->AccountID, lpObj->Name);
					g_PersonalStore.CGPShopAnsBuyItem(aSourceIndex, lpObj->m_Index, 0, 9);

					return;
				}

				if (g_PentagramSystem.IsPentagramItem(&lpObj->Inventory1[lpMsg->btItemPos]) == true)
				{
					if (lpObj->Inventory1[lpMsg->btItemPos].m_Durability < 1.0)
					{
						g_Log.Add("[PShop][%s][%s] PentagramItem Trans Count Not Exist!!",
							gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name);

						return;
					}

					if (!gTransPentagramJewelInfoTest(aSourceIndex, lpObj->m_Index, lpObj->Inventory1[lpMsg->btItemPos]))
					{
						g_Log.Add("[PShop][%s][%s] PentagramJewel Free DB Not Exist!!",
							gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name);

						g_PersonalStore.CGPShopAnsBuyItem(aSourceIndex, lpObj->m_Index, 0, 8);
						return;
					}

					lpObj->Inventory1[lpMsg->btItemPos].m_Durability -= 1.0;
				}

				if (lpObj->Inventory1[lpMsg->btItemPos].m_wPShopBlessValue > 0 ||
					lpObj->Inventory1[lpMsg->btItemPos].m_wPShopSoulValue > 0 ||
					lpObj->Inventory1[lpMsg->btItemPos].m_wPShopChaosValue > 0)
				{
					int iJewelPutTest = gJewelInventoryPutTest(aSourceIndex, lpObj->m_Index,
						lpObj->Inventory1[lpMsg->btItemPos].m_wPShopBlessValue,
						lpObj->Inventory1[lpMsg->btItemPos].m_wPShopSoulValue,
						lpObj->Inventory1[lpMsg->btItemPos].m_wPShopChaosValue);

					if (iJewelPutTest == 100)
					{
						if (g_PentagramSystem.IsPentagramItem(&lpObj->Inventory1[lpMsg->btItemPos]) == TRUE)
						{
							lpObj->Inventory1[lpMsg->btItemPos].m_Durability += 1.0;
						}

						g_Log.Add("[PShop] [%s][%s] PShop Item Buy Request Failed : [%s][%s] Seller Inventory is Full",
							gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name, lpObj->AccountID, lpObj->Name);
						g_PersonalStore.CGPShopAnsBuyItem(aSourceIndex, lpObj->m_Index, 0, 17);

						return;
					}
				}

				BYTE btNewItemPos = 0;
				btNewItemPos = ::gObjOnlyInventoryInsertItem(aSourceIndex, lpObj->Inventory1[lpMsg->btItemPos]);

				if (btNewItemPos == 0xFF)
				{
					g_Log.Add("[PShop] [%s][%s] PShop Item Buy Request Failed : [%s][%s] No Room to Buy Item",
						gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name,
						lpObj->AccountID, lpObj->Name);
					g_PersonalStore.CGPShopAnsBuyItem(aSourceIndex, lpObj->m_Index, 0, 8);

					return;
				}

				if (g_PentagramSystem.IsPentagramItem(&lpObj->Inventory1[lpMsg->btItemPos]))
				{
					int nJewelCount = 0;
					gTransPentagramJewelInfo(aSourceIndex, btNewItemPos, lpObj->m_Index, lpObj->Inventory1[lpMsg->btItemPos], &nJewelCount);
					g_Log.Add("[PShop] [%s][%s] PShop Item Buy : Result Trade Count - %d --> %d, JewelCount = %d",
						gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name,
						(int)lpObj->Inventory1[lpMsg->btItemPos].m_Durability + 1.0,
						(int)lpObj->Inventory1[lpMsg->btItemPos].m_Durability,
						nJewelCount);
				}

				gJewelInventoryPut(aSourceIndex, lpObj->m_Index,
					lpObj->Inventory1[lpMsg->btItemPos].m_wPShopBlessValue,
					lpObj->Inventory1[lpMsg->btItemPos].m_wPShopSoulValue,
					lpObj->Inventory1[lpMsg->btItemPos].m_wPShopChaosValue,
					sBlessJewelNeedCount, s10BlessJewelNeedCount, s20BlessJewelNeedCount, s30BlessJewelNeedCount,
					sSoulJewelNeedCount, s10SoulJewelNeedCount, s20SoulJewelNeedCount, s30SoulJewelNeedCount,
					sChaosJewelNeedCount, s10ChaosJewelNeedCount, s20ChaosJewelNeedCount, s30ChaosJewelNeedCount);

				iITEM_LOG_TYPE = lpObj->Inventory1[lpMsg->btItemPos].m_Type;
				iITEM_LOG_LEVEL = lpObj->Inventory1[lpMsg->btItemPos].m_Level;
				iITEM_LOG_DUR = lpObj->Inventory1[lpMsg->btItemPos].m_Durability;
				iITEM_LOG_SERIAL = lpObj->Inventory1[lpMsg->btItemPos].m_Number;
				iITEM_LOG_BJPRICE = lpObj->Inventory1[lpMsg->btItemPos].m_wPShopBlessValue;
				iITEM_LOG_SJPRICE = lpObj->Inventory1[lpMsg->btItemPos].m_wPShopSoulValue;
				iITEM_LOG_CJPRICE = lpObj->Inventory1[lpMsg->btItemPos].m_wPShopChaosValue;
				if(lpObj->m_PlayerData->ISBOT == false)
				{
					g_PersonalStore.gObjInventoryItemSet_PShop(lpObj->m_Index, lpMsg->btItemPos, -1);
					lpObj->Inventory1[lpMsg->btItemPos].Clear();
					GSProtocol.GCInventoryItemDeleteSend(lpObj->m_Index, lpMsg->btItemPos, TRUE);
				}
				if (lpObj->m_bOff && lpObj->m_PlayerData->ISBOT == false)
				{
					switch (g_ConfigRead.offtrade.CoinType)
					{
					case 0:
					{
						gObj[aSourceIndex].m_PlayerData->Money -= dwCost;

						lpObj->m_PlayerData->Money += (dwCost - round(dwCost * 0.01));
					}
						break;
					case 1:
					{
						gObj[aSourceIndex].m_PlayerData->m_WCoinC -= dwCost;
						::GDReqInGameShopPointAdd(aSourceIndex, 0, 0.00 - dwCost);
						::GDReqInGameShopPointAdd(lpObj->m_Index, 0, dwCost);
					}
						break;
					case 2:
					{
						gObj[aSourceIndex].m_PlayerData->m_WCoinP -= dwCost;
						::GDReqInGameShopPointAdd(aSourceIndex, 1, 0.00 - dwCost);
						::GDReqInGameShopPointAdd(lpObj->m_Index, 1, dwCost);
					}
						break;
					case 3:
					{
						gObj[aSourceIndex].m_PlayerData->m_GoblinPoint -= dwCost;
						::GDReqInGameShopPointAdd(aSourceIndex, 2, 0.00 - dwCost);
						::GDReqInGameShopPointAdd(lpObj->m_Index, 2, dwCost);
					}
						break;
					}
				}
				else if(lpObj->m_PlayerData->ISBOT == TRUE && g_BotSystem.GetBotType(lpObj->m_Index) == BOT_SHOP)
				{
					switch(g_BotSystem.m_BotData[lpObj->m_PlayerData->wBotIndex].m_Shop.iCoinType)
					{
						case 0:
							{
								gObj[aSourceIndex].m_PlayerData->Money -= dwCost;

								//lpObj->m_PlayerData->Money += (dwCost - round(dwCost * 0.01));
							}
							break;
						case 1:
							{
								gObj[aSourceIndex].m_PlayerData->m_WCoinC -= dwCost;
								::GDReqInGameShopPointAdd(aSourceIndex, 0, 0.00 - dwCost);
								//::GDReqInGameShopPointAdd(lpObj->m_Index, 0, dwCost);
							}
							break;
						case 2:
							{
								gObj[aSourceIndex].m_PlayerData->m_WCoinP -= dwCost;
								::GDReqInGameShopPointAdd(aSourceIndex, 1, 0.00 - dwCost);
							//	::GDReqInGameShopPointAdd(lpObj->m_Index, 1, dwCost);
							}
							break;
						case 3:
							{
								gObj[aSourceIndex].m_PlayerData->m_GoblinPoint -= dwCost;
								::GDReqInGameShopPointAdd(aSourceIndex, 2, 0.00 - dwCost);
							//	::GDReqInGameShopPointAdd(lpObj->m_Index, 2, dwCost);
							}
							break;
					}
				}
				else
				{
					gObj[aSourceIndex].m_PlayerData->Money -= dwCost;
					lpObj->m_PlayerData->Money += (dwCost - round(dwCost * 0.01)); // tax
				}

				GSProtocol.GCMoneySend(aSourceIndex, gObj[aSourceIndex].m_PlayerData->Money);
				GSProtocol.GCMoneySend(lpObj->m_Index, lpObj->m_PlayerData->Money);
				if(lpObj->m_PlayerData->ISBOT == false)
				{
					g_CashShop.GCCashPoint(&gObj[lpObj->m_Index]);
				}
				g_CashShop.GCCashPoint(&gObj[aSourceIndex]);
				g_PersonalStore.CGPShopAnsBuyItem(aSourceIndex, lpObj->m_Index, btNewItemPos, 1);
				if(lpObj->m_PlayerData->ISBOT == false && g_BotSystem.GetBotType(lpObj->m_Index) == BOT_SHOP)
				{
					g_PersonalStore.CGPShopAnsSoldItem(lpObj->m_Index, aSourceIndex, lpMsg->btItemPos);
				}
				g_Log.Add("[PShop] [%s][%s][%s] PShop Item Buy Request Succeed : [%s][%s][%s](ZenPrice = %d(ZenCommision:%d), BJPrice = %d(BJCommision:%d), SJPrice = %d(SJCommision:%d), CJPrice = %d(CJCommision:%d), ItemType:%d(%s), ItemLevel : %d, ItemDur : %d, Serial : %u, BuyKind : %d",
					gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name, gObj[aSourceIndex].m_PlayerData->Ip_addr,
					lpObj->AccountID, lpObj->Name, lpObj->m_PlayerData->Ip_addr, dwCost, round(dwCost * 0.01),
					iITEM_LOG_BJPRICE, round(iITEM_LOG_BJPRICE *0.03),
					iITEM_LOG_SJPRICE, round(iITEM_LOG_SJPRICE *0.03),
					iITEM_LOG_CJPRICE, round(iITEM_LOG_CJPRICE *0.03),
					iITEM_LOG_TYPE, ItemAttribute[iITEM_LOG_TYPE].Name, iITEM_LOG_LEVEL, iITEM_LOG_DUR, iITEM_LOG_SERIAL, lpMsg->btBuyKind);

				if(lpObj->m_PlayerData->ISBOT == true && g_BotSystem.GetBotType(lpObj->m_Index) == BOT_SHOP)
				{
					BotShopLog->Output("[BOT SHOP] [%s][%s][%s] Buy %s from %s for %d",gObj[aSourceIndex].m_PlayerData->Ip_addr, gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name, ItemAttribute[iITEM_LOG_TYPE].Name, lpObj->Name, dwCost);
				}
				if (g_PersonalStore.PShop_CheckInventoryEmpty(lpObj->m_Index) == true)
				{
					g_Log.Add("[PShop] [%s][%s] Sold All Items - Auto Closing PShop",
						lpObj->AccountID, lpObj->Name);
					GSProtocol.GCServerMsgStringSend(Lang.GetText(0,113), lpObj->m_Index, 1);
					lpObj->m_bPShopOpen = false;
					memset(lpObj->m_szPShopText, 0, sizeof(lpObj->m_szPShopText));
					g_PersonalStore.CGPShopAnsClose(lpObj->m_Index, 1);
					lpObj->m_bOff = false;
				}
				else
				{
					lpObj->m_bPShopItemChange = true;
				}

			}
		}
		__finally
		{
			lpObj->m_bPShopTransaction = false;
		}
	}
}

void CPersonalStore::CGPShopAnsBuyItem(int aSourceIndex, int aTargetIndex, int iItemPos, BYTE btResult)
{
	PMSG_ANS_BUYITEM_FROM_PSHOP pMsg;

	PHeadSubSetB((LPBYTE)&pMsg, 0x3F, 0x06, sizeof(pMsg));
	pMsg.NumberH = SET_NUMBERH(aTargetIndex);
	pMsg.NumberL = SET_NUMBERL(aTargetIndex);
	pMsg.btItemPos = iItemPos;
	pMsg.Result = btResult;
	ItemByteConvert(pMsg.cItemInfo, gObj[aSourceIndex].Inventory1[iItemPos]);

	IOCP.DataSend(aSourceIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

void CPersonalStore::CGPShopReqCloseDeal(PMSG_REQ_PSHOPDEAL_CLOSE * lpMsg, int aIndex)
{
	if (!gObjIsConnected(aIndex))
		return;

	gObj[aIndex].m_bPShopWantDeal = false;
	gObj[aIndex].m_iPShopDealerIndex = -1;
	memset(gObj[aIndex].m_szPShopDealerName, 0, MAX_ACCOUNT_LEN);
}



void CPersonalStore::CGPShopAnsSoldItem(int aSourceIndex, int aTargetIndex, int iItemPos)
{
	PMSG_ANS_SOLDITEM_FROM_PSHOP pMsg;

	PHeadSubSetB((LPBYTE)&pMsg, 0x3F, 0x08, sizeof(pMsg));
	pMsg.btItemPos = iItemPos;
	memcpy(pMsg.btName, gObj[aTargetIndex].Name, MAX_ACCOUNT_LEN);

	IOCP.DataSend(aSourceIndex, (LPBYTE)&pMsg, pMsg.h.size);
}



void CPersonalStore::CGPShopAnsDealerClosedShop(int aSourceIndex, int aTargetIndex)
{
	PMSG_REQ_DEALER_CLOSED_SHOP pMsg;

	PHeadSubSetB((LPBYTE)&pMsg, 0x3F, 0x12, sizeof(pMsg));
	pMsg.NumberH = SET_NUMBERH(aTargetIndex);
	pMsg.NumberL = SET_NUMBERL(aTargetIndex);

	IOCP.DataSend(aSourceIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

void CPersonalStore::PShop_ViewportListRegenarate(short aIndex)
{
	if (gObjIsConnected(aIndex) == 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->Type != OBJ_USER)
	{
		return;
	}

	if (lpObj->RegenOk > 0)
	{
		return;
	}

	if (lpObj->CloseCount > -1)
	{
		return;
	}

	BYTE cBUFFER[6000];
	int iVpOpenCount = 0;
	int iVpAddCount = 0;

	PMSG_ANS_PSHOP_VIEWPORT_NOTIFY * lpMsg = (PMSG_ANS_PSHOP_VIEWPORT_NOTIFY *)&cBUFFER;
	PMSG_PSHOP_VIEWPORT_NOTIFY * lpMsgBody = (PMSG_PSHOP_VIEWPORT_NOTIFY *)&cBUFFER[6];

	int iPShopOpenIndex[MAX_VIEWPORT];

	if (lpObj->m_bPShopRedrawAbs != 0)
	{
		memset(lpObj->m_iVpPShopPlayer, 0x00, sizeof(lpObj->m_iVpPShopPlayer));
		lpObj->m_wVpPShopPlayerCount = 0;
		lpObj->m_bPShopRedrawAbs = 0;
	}

	for (int n = 0; n < MAX_VIEWPORT; n++)
	{
		int tObjIndex = lpObj->VpPlayer[n].number;

		if (!ObjectMaxRange(tObjIndex))
		{
			continue;
		}

		if (lpObj->VpPlayer[n].state == 2 && lpObj->VpPlayer[n].type == OBJ_USER && gObj[tObjIndex].m_bPShopOpen == 1)
		{
			iPShopOpenIndex[iVpOpenCount] = tObjIndex;
			iVpOpenCount++;

			if (lpObj->m_wVpPShopPlayerCount > 0 && lpObj->m_wVpPShopPlayerCount <= MAX_VIEWPORT)
			{
				int found = 0;

				for (int i = 0; i < lpObj->m_wVpPShopPlayerCount; i++)
				{
					if (lpObj->m_iVpPShopPlayer[i] == tObjIndex)
					{
						found = 1;
						break;
					}
				}

				if (found == 0)
				{
					lpMsgBody[iVpAddCount].btNumberL = SET_NUMBERL(WORD(tObjIndex));
					lpMsgBody[iVpAddCount].btNumberH = SET_NUMBERH(WORD(tObjIndex));
					memcpy(lpMsgBody[iVpAddCount].szPShopText, gObj[tObjIndex].m_szPShopText, sizeof(gObj[tObjIndex].m_szPShopText));
					iVpAddCount++;
				}
			}
			else if (lpObj->m_wVpPShopPlayerCount == 0)
			{
				lpMsgBody[iVpAddCount].btNumberL = SET_NUMBERL(WORD(tObjIndex));
				lpMsgBody[iVpAddCount].btNumberH = SET_NUMBERH(WORD(tObjIndex));
				memcpy(lpMsgBody[iVpAddCount].szPShopText, gObj[tObjIndex].m_szPShopText, sizeof(gObj[tObjIndex].m_szPShopText));
				iVpAddCount++;
			}
			else
			{
				g_Log.Add("[PShop] ERROR : lpObj->m_wVpPShopPlayerCount is OUT of BOUND : %d", lpObj->m_wVpPShopPlayerCount);
			}
		}
	}

	memset(lpObj->m_iVpPShopPlayer, 0x00, sizeof(lpObj->m_iVpPShopPlayer));
	lpObj->m_wVpPShopPlayerCount = 0;

	if (iVpOpenCount > 0 && iVpOpenCount <= MAX_VIEWPORT)
	{
		for (int n = 0; n < iVpOpenCount; n++)
		{
			lpObj->m_iVpPShopPlayer[n] = iPShopOpenIndex[n];
		}
		lpObj->m_wVpPShopPlayerCount = iVpOpenCount;
	}
	else if (iVpOpenCount != 0)
	{
		g_Log.Add("[PShop] ERROR : iVpOpenCount is OUT of BOUND: %d", iVpOpenCount);
	}

	if (iVpAddCount > 0 && iVpAddCount <= MAX_VIEWPORT)
	{
		lpMsg->btCount = iVpAddCount;
		PHeadSubSetW((unsigned char *)lpMsg, 0x3F, 0, sizeof(PMSG_ANS_PSHOP_VIEWPORT_NOTIFY) + sizeof(PMSG_PSHOP_VIEWPORT_NOTIFY)*iVpAddCount);
		IOCP.DataSend(aIndex, (unsigned char *)lpMsg, ((lpMsg->h.sizeL & 0xFF) & 0xFF | ((lpMsg->h.sizeH & 0xFF) & 0xFF) << 8) & 0xFFFF);
	}
	else if (iVpAddCount != 0)
	{
		g_Log.Add("[PShop] ERROR : iVpAddCount is OUT of BOUND: %d", iVpAddCount);
	}

	if (gObj[aIndex].m_bPShopWantDeal == 1)
	{
		int iDealerIndex = gObj[aIndex].m_iPShopDealerIndex;

		if (ObjectMaxRange(iDealerIndex) == true && gObjIsConnected(iDealerIndex) != 0)
		{
			if (gObj[iDealerIndex].Type == OBJ_USER)
			{
				if (gObj[iDealerIndex].m_bPShopOpen == 0)
				{
					gObj[aIndex].m_bPShopWantDeal = 0;
					gObj[aIndex].m_iPShopDealerIndex = -1;
					memset(gObj[aIndex].m_szPShopDealerName, 0, sizeof(gObj[aIndex].m_szPShopDealerName));

					this->CGPShopAnsDealerClosedShop(aIndex, iDealerIndex);
				}
				else if (gObj[iDealerIndex].CloseCount >= 0 || gObj[aIndex].CloseCount >= 0)
				{
					gObj[aIndex].m_bPShopWantDeal = 0;
					gObj[aIndex].m_iPShopDealerIndex = -1;
					memset(gObj[aIndex].m_szPShopDealerName, 0, sizeof(gObj[aIndex].m_szPShopDealerName));

					this->CGPShopAnsDealerClosedShop(aIndex, iDealerIndex);
				}
				else if (gObj[iDealerIndex].m_bPShopItemChange == 1)
				{
					this->CGPShopAnsBuyList(aIndex, iDealerIndex, 1, 1);
				}
			}
			else
			{
				gObj[aIndex].m_bPShopWantDeal = 0;
				gObj[aIndex].m_iPShopDealerIndex = -1;
				memset(gObj[aIndex].m_szPShopDealerName, 0, sizeof(gObj[aIndex].m_szPShopDealerName));

				this->CGPShopAnsDealerClosedShop(aIndex, iDealerIndex);
			}
		}
		else
		{
			gObj[aIndex].m_bPShopWantDeal = 0;
			gObj[aIndex].m_iPShopDealerIndex = -1;
			memset(gObj[aIndex].m_szPShopDealerName, 0, sizeof(gObj[aIndex].m_szPShopDealerName));

			this->CGPShopAnsDealerClosedShop(aIndex, iDealerIndex);
		}
	}
}

bool CPersonalStore::PShop_CheckInventoryEmpty(short aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	for (int i = PSHOP_START_RANGE; i < PSHOP_END_RANGE; i++)
	{
		if (lpObj->Inventory1[i].IsItem() == 1)
		{
			return false;
		}
	}
	return true;
}

void CPersonalStore::gObjInventoryItemSet_PShop(int aIndex, int itempos, BYTE set_byte)
{
	int width;
	int height;
	if (itempos < INVETORY_WEAR_SIZE)
	{
		return;
	}
	else if (itempos >(INVENTORY_SIZE - 1))
	{
		return;
	}

	if (gObj[aIndex].Inventory1[itempos].GetSize((int&)width, (int &)height) == 0)
	{
		g_Log.Add("error: Item does not exist %s %d", __FILE__, __LINE__);
		return;
	}

	gObjInventoryItemBoxSet_PShop(aIndex, itempos, width, height, set_byte);
}



void CPersonalStore::gObjInventoryItemBoxSet_PShop(int aIndex, int itempos, int xl, int yl, BYTE set_byte)
{
	int itemposx = (itempos - INVETORY_WEAR_SIZE) % 8;
	int itemposy = (itempos - INVETORY_WEAR_SIZE) / 8;

	int xx, yy;

	if (!CheckOutOfInventory(aIndex, itemposy, yl))
	{
		return;
	}

	for (int y = 0; y < yl; y++)
	{
		yy = itemposy + y;

		for (int x = 0; x < xl; x++)
		{
			xx = itemposx + x;

			if (InventoryExtentCheck(xx, yy, 8, MAX_INVENTORY_H2))
			{
				*(BYTE*)(gObj[aIndex].InventoryMap1 + (itemposy + y) * 8 + (itemposx + x)) = set_byte;
			}
			else
			{
				g_Log.Add("error : %s %d", __FILE__, __LINE__);
				return;
			}
		}
	}
}

void CPersonalStore::CGReqSearchItemInPShop(PMSG_REQ_SEARCH_ITEM_PSHOP *lpMsg, int aIndex)
{
	if (!lpMsg)
	{
		return;
	}

	if (!ObjectMaxRange(aIndex))
	{
		return;
	}

	if (lpMsg->sSearchItem == -1)
	{
		this->GCPShop_AllInfo(aIndex, lpMsg->iLastCount);
	}

	else
	{
		this->GCPShop_SearchItem(aIndex, lpMsg->sSearchItem, lpMsg->iLastCount);
	}
}

void CPersonalStore::CGReqPShopLog(PMSG_REQ_PSHOP_LOG *lpMsg, int aIndex)
{
	if (!lpMsg)
	{
		return;
	}

	if (!ObjectMaxRange(aIndex))
	{
		return;
	}

	if (!ObjectMaxRange(lpMsg->iTargetIndex))
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];
	LPOBJ lpTargetObj = &gObj[lpMsg->iTargetIndex];

	if (lpMsg->btLogKind == 0)
	{
		g_Log.Add("[PShopLog] [%s][%s] Whisper To [%s][%s]",
			lpObj->AccountID, lpObj->Name, lpTargetObj->AccountID, lpTargetObj->Name);
	}

	else if (lpMsg->btLogKind == 1)
	{
		g_Log.Add("[PShopLog] [%s][%s] Mail To [%s][%s]",
			lpObj->AccountID, lpObj->Name, lpTargetObj->AccountID, lpTargetObj->Name);
	}
}

void CPersonalStore::GDRequestPShopItemValue(OBJECTSTRUCT* lpObj, char *szAccountID)
{
	if (g_ConfigRead.server.GetServerType() == SERVER_BATTLECORE)
	{
		return;
	}

	PMSG_REQ_PSHOPITEMVALUE_INFO pMsg;
	memcpy(pMsg.AccountId, szAccountID, MAX_ACCOUNT_LEN + 1);
	memcpy(pMsg.szName, lpObj->Name, MAX_ACCOUNT_LEN + 1);
	pMsg.iUserIndex = lpObj->m_Index;

	PHeadSetB((LPBYTE)&pMsg, 0xE9, sizeof(pMsg));
	wsDataCli.DataSend((char *)&pMsg, pMsg.h.size); 
}

void CPersonalStore::GDUpdatePShopItemValue(OBJECTSTRUCT *lpObj)
{
	return;
}

void CPersonalStore::GDAllSavePShopItemValue(OBJECTSTRUCT *lpObj)
{
	PMSG_UPDATE_PSHOPITEMVALUE_INFO pMsg;
	int iItemCnt = 0;
	bool bExistPShopItem = false;

	for (int i = PSHOP_START_RANGE; i < PSHOP_END_RANGE; i++)
	{
		if (lpObj->pInventory[i].IsItem() == TRUE)
		{
			bExistPShopItem = true;
			pMsg.PShopItemValueInfo[iItemCnt].nPShopItemInvenNum = i;
			pMsg.PShopItemValueInfo[iItemCnt].ItemSerial = lpObj->pInventory[i].m_Number;
			pMsg.PShopItemValueInfo[iItemCnt].nMoney = lpObj->pInventory[i].m_iPShopValue;
			pMsg.PShopItemValueInfo[iItemCnt].sBlessJewelValue = lpObj->pInventory[i].m_wPShopBlessValue;
			pMsg.PShopItemValueInfo[iItemCnt].sSoulJewelValue = lpObj->pInventory[i].m_wPShopSoulValue;
			pMsg.PShopItemValueInfo[iItemCnt].sChaosJewelValue = lpObj->pInventory[i].m_wPShopChaosValue;

			iItemCnt++;
		}
	}

	if (bExistPShopItem == true)
	{
		memcpy(pMsg.AccountId, lpObj->AccountID, MAX_ACCOUNT_LEN + 1);
		memcpy(pMsg.szName, lpObj->Name, MAX_ACCOUNT_LEN + 1);
		pMsg.btItemCnt = iItemCnt;

		PHeadSetW((LPBYTE)&pMsg, 0xEB, sizeof(pMsg));

		wsDataCli.DataSend((char *)&pMsg, sizeof(pMsg));
	}
}

void CPersonalStore::GDDelPShopItemValue(OBJECTSTRUCT *lpObj, int nPShopItemInvenNum)
{
	PMSG_DEL_PSHOPITEM pMsg;

	memcpy(pMsg.AccountId, lpObj->AccountID, MAX_ACCOUNT_LEN + 1);
	memcpy(pMsg.szName, lpObj->Name, MAX_ACCOUNT_LEN + 1);
	pMsg.nPShopItemInvenNum = nPShopItemInvenNum;

	PHeadSetB((LPBYTE)&pMsg, 0xEC, sizeof(pMsg));
	wsDataCli.DataSend((char *)&pMsg, pMsg.h.size);
}

void CPersonalStore::GDMovePShopItem(OBJECTSTRUCT* lpObj, int nOldPShopItemInvenNum, int nNewPShopItemInvenNum)
{
	PMSG_MOVE_PSHOPITEM pMsg;

	memcpy(pMsg.AccountId, lpObj->AccountID, MAX_ACCOUNT_LEN + 1);
	memcpy(pMsg.szName, lpObj->Name, MAX_ACCOUNT_LEN + 1);

	pMsg.nOldPShopItemInvenNum = nOldPShopItemInvenNum;
	pMsg.nNewPShopItemInvenNum = nNewPShopItemInvenNum;

	PHeadSetB((LPBYTE)&pMsg, 0xED, sizeof(pMsg));
	wsDataCli.DataSend((char *)&pMsg, pMsg.h.size);
}

void CPersonalStore::GDAnsPShopItemValue(PMSG_ANS_PSHOPITEMVALUE_INFO *lpMsg)
{
	int iItemCnt = lpMsg->btItemCnt;
	int iObjIndex = lpMsg->iUserIndex;

	if (!ObjectMaxRange(iObjIndex))
	{
		return;
	}

	if (iItemCnt < 0 || iItemCnt > 31)
	{
		return;
	}

	LPOBJ lpObj = &gObj[iObjIndex];
	
	for (int i = 0; i < iItemCnt; i++)
	{
		if (lpMsg->PShopItemValueInfo[i].nPShopItemInvenNum >= 204 && lpObj->pInventory[lpMsg->PShopItemValueInfo[i].nPShopItemInvenNum].IsItem() == TRUE)
		{
			if (lpObj->pInventory[lpMsg->PShopItemValueInfo[i].nPShopItemInvenNum].m_Number == lpMsg->PShopItemValueInfo[i].ItemSerial)
			{
				lpObj->pInventory[lpMsg->PShopItemValueInfo[i].nPShopItemInvenNum].m_iPShopValue = lpMsg->PShopItemValueInfo[i].nMoney;
				lpObj->pInventory[lpMsg->PShopItemValueInfo[i].nPShopItemInvenNum].m_wPShopBlessValue = lpMsg->PShopItemValueInfo[i].sBlessJewelValue;
				lpObj->pInventory[lpMsg->PShopItemValueInfo[i].nPShopItemInvenNum].m_wPShopSoulValue = lpMsg->PShopItemValueInfo[i].sSoulJewelValue;
				lpObj->pInventory[lpMsg->PShopItemValueInfo[i].nPShopItemInvenNum].m_wPShopChaosValue = lpMsg->PShopItemValueInfo[i].sChaosJewelValue;
			}
		}
	}

	this->GCPShopItemValueInfo(iObjIndex);
}

void CPersonalStore::GCPShopItemValueInfo(int aIndex)
{
	if (!ObjectMaxRange(aIndex))
	{
		return;
	}

	if (!gObjIsConnected(aIndex))
	{
		return;
	}

	BYTE sendbuf[4096];
	memset(sendbuf, 0x00, sizeof(sendbuf));

	PMSG_PSHOPITEMVALUE_INFO_COUNT pMsg;
	PSHOP_ITEMVALUE_INFO PShopItemValueInfo[32];

	int lOfs = sizeof(pMsg);
	int iItemCnt = 0;

	LPOBJ lpObj = &gObj[aIndex];

	for (int i = PSHOP_START_RANGE; i < PSHOP_END_RANGE; i++)
	{
		if (lpObj->pInventory[i].IsItem() == TRUE)
		{
			PShopItemValueInfo[iItemCnt].nPShopItemInvenNum = i;
			PShopItemValueInfo[iItemCnt].Empty = 0;
			PShopItemValueInfo[iItemCnt].nMoney = lpObj->pInventory[i].m_iPShopValue;
			PShopItemValueInfo[iItemCnt].sBlessJewelValue = lpObj->pInventory[i].m_wPShopBlessValue;
			PShopItemValueInfo[iItemCnt].sSoulJewelValue = lpObj->pInventory[i].m_wPShopSoulValue;
			PShopItemValueInfo[iItemCnt].sChaosJewelValue = lpObj->pInventory[i].m_wPShopChaosValue;

			memcpy(&sendbuf[lOfs], &PShopItemValueInfo[iItemCnt], sizeof(PSHOP_ITEMVALUE_INFO));
			lOfs += sizeof(PSHOP_ITEMVALUE_INFO);
			iItemCnt++;
		}
	}

	pMsg.Result = iItemCnt > 0;
	pMsg.btMoneyCommisionRate = 1;
	pMsg.btJewelCommisionRate = 3;
	pMsg.btItemCnt = iItemCnt;

	PHeadSubSetW((LPBYTE)&pMsg, 0xEC, 0x32, lOfs);
	memcpy(sendbuf, &pMsg, sizeof(pMsg));

	IOCP.DataSend(aIndex, sendbuf, lOfs);
}

void CPersonalStore::GCPShop_AllInfo(short aIndex, int iLastUserCount)
{
	if (!ObjectMaxRange(aIndex))
	{
		return;
	}

	if (!gObjIsConnected(aIndex))
	{
		return;
	}

	BYTE sendbuf[4096];
	memset(sendbuf, 0x00, sizeof(sendbuf));

	PMSG_SEARCH_ITEM_PSHOP_COUNT pMsg;
	PMSG_SEARCH_ITEM_PSHOP SearchItemPShop[1000];

	int lOfs = sizeof(PMSG_SEARCH_ITEM_PSHOP_COUNT);
	int iAddCnt = 0;

	for (int n = g_ConfigRead.server.GetObjectStartUserIndex(); n < g_ConfigRead.server.GetObjectMax(); n++)
	{
		LPOBJ lpObj = &gObj[n];

		if (gObj[n].Connected == PLAYER_PLAYING && lpObj->Type == OBJ_USER &&
			(lpObj->MapNumber == MAP_INDEX_RORENCIA || lpObj->MapNumber == MAP_INDEX_DEVIAS || lpObj->MapNumber == MAP_INDEX_NORIA ||
			lpObj->MapNumber == MAP_INDEX_ELBELAND || g_ConfigRead.server.GetServerType() == SERVER_MARKET))
		{
			if (aIndex != lpObj->m_Index)
			{
				if ((MapC[lpObj->MapNumber].GetAttr(lpObj->X, lpObj->Y) & 1) == 1)
				{
					if (lpObj->m_bPShopOpen == true)
					{
						if (iAddCnt >= iLastUserCount)
						{
							SearchItemPShop[iAddCnt].btNumberL = LOBYTE(lpObj->m_Index);
							SearchItemPShop[iAddCnt].btNumberH = HIBYTE(lpObj->m_Index);
							memcpy(SearchItemPShop[iAddCnt].szName, lpObj->Name, MAX_ACCOUNT_LEN + 1);
							memcpy(SearchItemPShop[iAddCnt].szPShopText, lpObj->m_szPShopText, 37);

							memcpy(&sendbuf[lOfs], &SearchItemPShop[iAddCnt], sizeof(PMSG_SEARCH_ITEM_PSHOP));
							lOfs += sizeof(PMSG_SEARCH_ITEM_PSHOP);
						}

						iAddCnt++;

						if (iLastUserCount < iAddCnt)
						{
							if (!(iAddCnt % 50))
							{
								pMsg.iPShopCnt = iAddCnt;
								pMsg.btContinueFlag = TRUE;

								PHeadSubSetW((LPBYTE)&pMsg, 0xEC, 0x31, lOfs);
								memcpy(sendbuf, &pMsg, sizeof(pMsg));

								IOCP.DataSend(aIndex, sendbuf, lOfs);
								return;
							}
						}
					}
				}
			}
		}
	}

	if (iAddCnt)
	{
		pMsg.iPShopCnt = iAddCnt;
		pMsg.btContinueFlag = 0;
	}

	else
	{
		pMsg.iPShopCnt = -1;
		pMsg.btContinueFlag = 0;
	}

	PHeadSubSetW((LPBYTE)&pMsg, 0xEC, 0x31, lOfs);
	memcpy(sendbuf, &pMsg, sizeof(pMsg));

	IOCP.DataSend(aIndex, sendbuf, lOfs);
}

void CPersonalStore::GCPShop_SearchItem(short aIndex, WORD sSearchItem, int iLastUserCount)
{
	if (!ObjectMaxRange(aIndex))
	{
		return;
	}

	if (!gObjIsConnected(aIndex))
	{
		return;
	}

	BYTE sendbuf[4096];
	memset(sendbuf, 0x00, sizeof(sendbuf));

	PMSG_SEARCH_ITEM_PSHOP_COUNT pMsg;
	PMSG_SEARCH_ITEM_PSHOP SearchItemPShop[1000];

	int lOfs = sizeof(PMSG_SEARCH_ITEM_PSHOP_COUNT);
	int iAddCnt = 0;

	for (int n = g_ConfigRead.server.GetObjectStartUserIndex(); n < g_ConfigRead.server.GetObjectMax(); n++)
	{
		LPOBJ lpObj = &gObj[n];

		if (gObj[n].Connected == PLAYER_PLAYING && lpObj->Type == OBJ_USER &&
			(lpObj->MapNumber == MAP_INDEX_RORENCIA || lpObj->MapNumber == MAP_INDEX_DEVIAS || lpObj->MapNumber == MAP_INDEX_NORIA ||
			lpObj->MapNumber == MAP_INDEX_ELBELAND || g_ConfigRead.server.GetServerType() == SERVER_MARKET))
		{
			if (aIndex != lpObj->m_Index)
			{
				if ((MapC[lpObj->MapNumber].GetAttr(lpObj->X, lpObj->Y) & 1) == 1)
				{
					if (lpObj->m_bPShopOpen == true)
					{
						if (this->PShop_CheckExistItemInInventory(n, sSearchItem) == TRUE)
						{
							if (iAddCnt >= iLastUserCount)
							{
								SearchItemPShop[iAddCnt].btNumberL = LOBYTE(lpObj->m_Index);
								SearchItemPShop[iAddCnt].btNumberH = HIBYTE(lpObj->m_Index);
								memcpy(SearchItemPShop[iAddCnt].szName, lpObj->Name, MAX_ACCOUNT_LEN + 1);
								memcpy(SearchItemPShop[iAddCnt].szPShopText, lpObj->m_szPShopText, 37);

								memcpy(&sendbuf[lOfs], &SearchItemPShop[iAddCnt], sizeof(PMSG_SEARCH_ITEM_PSHOP));
								lOfs += sizeof(PMSG_SEARCH_ITEM_PSHOP);
							}

							iAddCnt++;

							if (iLastUserCount < iAddCnt)
							{
								if (!(iAddCnt % 50))
								{
									pMsg.iPShopCnt = iAddCnt;
									pMsg.btContinueFlag = TRUE;

									PHeadSubSetW((LPBYTE)&pMsg, 0xEC, 0x31, lOfs);
									memcpy(sendbuf, &pMsg, sizeof(pMsg));

									IOCP.DataSend(aIndex, sendbuf, lOfs);
									return;
								}
							}
						}
					}
				}
			}
		}
	}

	if (iAddCnt)
	{
		pMsg.iPShopCnt = iAddCnt;
		pMsg.btContinueFlag = 0;
	}

	else
	{
		pMsg.iPShopCnt = -1;
		pMsg.btContinueFlag = 0;
	}

	PHeadSubSetW((LPBYTE)&pMsg, 0xEC, 0x31, lOfs);
	memcpy(sendbuf, &pMsg, sizeof(pMsg));

	IOCP.DataSend(aIndex, sendbuf, lOfs);
}

bool CPersonalStore::PShop_CheckExistItemInInventory(short aIndex, WORD sItemType)
{
	if (!ObjectMaxRange(aIndex))
	{
		return false;
	}

	if (!gObjIsConnected(aIndex))
	{
		return false;
	}

	for (int i = PSHOP_START_RANGE; i < PSHOP_END_RANGE; i++)
	{
		if (gObj[aIndex].Inventory1[i].m_Type == sItemType)
		{
			return true;
		}
	}

	return false;
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


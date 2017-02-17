//////////////////////////////////////////////////////////////////////
// GremoryCase.cpp
#include "stdafx.h"
#include "GremoryCase.h"
#include "winutil.h"
#include "TLog.h"
#include "GameMain.h"
#include "DSProtocol.h"
#include "ItemSocketOptionSystem.h"
#include "PentagramSystem.h"
#include "MuunSystem.h"
#include "configread.h"

CGremoryCase g_GremoryCase;

CGremoryCase::CGremoryCase(void)
{
}

CGremoryCase::~CGremoryCase(void)
{
}

void CGremoryCase::CheckStorageExpiredItems()
{
	time_t CurrTime = time(NULL);

	for (int i = g_ConfigRead.server.GetObjectStartUserIndex(); i < g_ConfigRead.server.GetObjectMax(); i++)
	{
		if (gObjIsConnected(i) == TRUE)
		{
			if (gObj[i].Type == OBJ_USER)
			{
				if (gObj[i].m_PlayerData->m_bGremoryCaseDBInfo == true)
				{
					for (int j = 0; j < MAX_GREMORYCASE_STORAGE_TYPES; j++)
					{
						for (int k = 0; k < MAX_GREMORYCASE_STORAGE_ITEMS; k++)
						{
							if (gObj[i].m_PlayerData->m_GremoryCaseData[j][k].btRewardInventory != 0)
							{
								if (gObj[i].m_PlayerData->m_GremoryCaseData[j][k].dwExpireTime < CurrTime)
								{
									this->GDReqDeleteItemFromGremoryCase(i, gObj[i].m_PlayerData->m_GremoryCaseData[j][k].ItemInfo.m_Type, gObj[i].m_PlayerData->m_GremoryCaseData[j][k].dwItemGUID, gObj[i].m_PlayerData->m_GremoryCaseData[j][k].dwAuthCode);
									this->GCSendDeleteItemFromGremoryCase(i, j, k);

									g_Log.Add("[Gremory Case] (%s)(%s) Deleted Expired Item (Storage Type: %d) (ItemGUID: %d) (AuthCode: %d)",
										gObj[i].AccountID, gObj[i].Name, j, gObj[i].m_PlayerData->m_GremoryCaseData[j][k].dwItemGUID, gObj[i].m_PlayerData->m_GremoryCaseData[j][k].dwAuthCode);
									gObj[i].m_PlayerData->m_GremoryCaseData[j][k].Clear();

								}
							}
						}
					}
				}
			}
		}
	}
}

void CGremoryCase::GDReqStorageItemList(int iIndex)
{
	if (g_ConfigRead.server.GetServerType() == SERVER_BATTLECORE)
	{
		return;
	}

	gObj[iIndex].m_PlayerData->m_bGremoryCaseDBInfo = false;

	_stReqGremoryCaseItemList pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0x4F, 0x00, sizeof(pMsg));

	pMsg.iIndex = iIndex;
	memcpy(pMsg.szAccountID, gObj[iIndex].AccountID, MAX_ACCOUNT_LEN+1);
	memcpy(pMsg.szName, gObj[iIndex].Name, MAX_ACCOUNT_LEN+1);

	wsDataCli.DataSend((char *)&pMsg, pMsg.h.size);

	g_Log.Add("[Gremory Case] (%s)(%s) Request to load Gremory Case item list.", gObj[iIndex].AccountID, gObj[iIndex].Name);
}

void CGremoryCase::DGAnsStorageItemList(LPBYTE lpRecv)
{
	_stAnsGremoryCaseItemList * lpMsg = (_stAnsGremoryCaseItemList *)(lpRecv);
	_stGremoryCaseItem * lpItem;

	int iIndex = lpMsg->iIndex;

	if ( !ObjectMaxRange(iIndex) )
	{
		return;
	}

	LPOBJ lpObj = &gObj[iIndex];

	if ( lpObj->Type != OBJ_USER )
	{
		return;
	}

	int iCharacterStorageCount = 0;
	int iServerStorageCount = 0;

	BYTE SocketOption[5] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
	BYTE SocketOptionIndex = 0xFF;

	for (int i = 0; i < lpMsg->btCount; i++ )
	{
		lpItem = (_stGremoryCaseItem *)(lpRecv + sizeof(_stAnsGremoryCaseItemList) + (i * sizeof(_stGremoryCaseItem)));

		for (int j = 0; j < MAX_GREMORYCASE_STORAGE_ITEMS; j++)
		{
			if ( lpObj->m_PlayerData->m_GremoryCaseData[lpItem->btStorageType-1][j].btRewardInventory == 0)
			{
				lpObj->m_PlayerData->m_GremoryCaseData[lpItem->btStorageType-1][j].Clear();
				lpObj->m_PlayerData->m_GremoryCaseData[lpItem->btStorageType-1][j].ItemInfo.Clear();
				lpObj->m_PlayerData->m_GremoryCaseData[lpItem->btStorageType-1][j].btRewardInventory = lpItem->btStorageType;
				lpObj->m_PlayerData->m_GremoryCaseData[lpItem->btStorageType-1][j].btRewardSource = lpItem->btRewardSource;
				lpObj->m_PlayerData->m_GremoryCaseData[lpItem->btStorageType-1][j].dwItemGUID = lpItem->btItemGUID;
				lpObj->m_PlayerData->m_GremoryCaseData[lpItem->btStorageType-1][j].dwAuthCode = lpItem->dwAuthCode;
				lpObj->m_PlayerData->m_GremoryCaseData[lpItem->btStorageType-1][j].dwReceiveDate = lpItem->iReceiveDate;
				lpObj->m_PlayerData->m_GremoryCaseData[lpItem->btStorageType-1][j].dwExpireTime = lpItem->iExpireDate;

				lpObj->m_PlayerData->m_GremoryCaseData[lpItem->btStorageType-1][j].ItemInfo.m_Type = lpItem->wItemID;
				lpObj->m_PlayerData->m_GremoryCaseData[lpItem->btStorageType-1][j].ItemInfo.m_Level = lpItem->btItemLevel;
				lpObj->m_PlayerData->m_GremoryCaseData[lpItem->btStorageType-1][j].ItemInfo.m_Durability = ItemGetDurability(lpItem->wItemID, lpItem->btItemLevel, lpItem->btItemExcOption, lpItem->btItemSetOption);

				if (g_SocketOptionSystem.IsEnableSocketItem(&lpObj->m_PlayerData->m_GremoryCaseData[lpItem->btStorageType-1][j].ItemInfo) == true)
				{
					if (lpItem->btItemSocketCount == 0)
					{
						g_SocketOptionSystem.MakeSocketSlot(&lpObj->m_PlayerData->m_GremoryCaseData[lpItem->btStorageType-1][j].ItemInfo, rand() % 5 + 1);
					}

					else
					{
						g_SocketOptionSystem.MakeSocketSlot(&lpObj->m_PlayerData->m_GremoryCaseData[lpItem->btStorageType-1][j].ItemInfo, lpItem->btItemSocketCount);
					}

					SocketOptionIndex = -1;
				}

				else if (g_PentagramSystem.IsPentagramItem(lpObj->m_PlayerData->m_GremoryCaseData[lpItem->btStorageType-1][j].ItemInfo.m_Type) == true)
				{
					SocketOptionIndex = lpItem->btItemMainAttribute;

					if (lpItem->btItemSocketCount == 0)
					{
						g_PentagramSystem.GetMakePentagramSlotCountNKind(SocketOption, lpObj->m_PlayerData->m_GremoryCaseData[lpItem->btStorageType-1][j].ItemInfo.m_Type);
					}

					else
					{
						BYTE Slot[5] = { 0, 0, 0, 0, 0 };

						for (int k = 0; k < lpItem->btItemSocketCount; k++)
						{
							Slot[k] = 1;
						}

						g_PentagramSystem.MakePentagramSocketSlot(&lpObj->m_PlayerData->m_GremoryCaseData[lpItem->btStorageType-1][j].ItemInfo, Slot[0], Slot[1], Slot[2], Slot[3], Slot[4]);
					}
				}

				else if (g_CMuunSystem.IsStoneofEvolution(lpObj->m_PlayerData->m_GremoryCaseData[lpItem->btStorageType-1][j].ItemInfo.m_Type) == TRUE)
				{
					int iMuunEvolutionItemID = ITEMGET(lpItem->wMuunEvoItemType, lpItem->btMuunEvoItemIndex);
				
					SocketOption[0] = iMuunEvolutionItemID >> 8;
					SocketOption[1] = ITEM_GET_INDEX(iMuunEvolutionItemID);
				}

				lpObj->m_PlayerData->m_GremoryCaseData[lpItem->btStorageType-1][j].ItemInfo.Convert(lpItem->wItemID, lpItem->btItemSkill,
					lpItem->btItemLuck, lpItem->btItemOption, lpItem->btItemExcOption, lpItem->btItemSetOption, 0, SocketOption, SocketOptionIndex, 0, 3);

				if ( lpItem->btStorageType == GC_STORAGE_SERVER )
				{
					iServerStorageCount++;
				}

				else if ( lpItem->btStorageType == GC_STORAGE_CHARACTER )
				{
					iCharacterStorageCount++;
				}

				break;
			}		
		}
	}

	lpObj->m_PlayerData->m_bGremoryCaseDBInfo = true;
	g_Log.Add("[Gremory Case] (%s)(%s) Received item list (Character:%d) (Server:%d)",
		lpObj->AccountID, lpObj->Name, iCharacterStorageCount, iServerStorageCount);

	this->GCSendStorageItemList(iIndex);
	this->CheckIsInStorageItemAboutToExpire(iIndex);
	this->CheckIsStorageFull(iIndex);
}

void CGremoryCase::GDReqAddItemToGremoryCase(int iIndex, _stGremoryCaseItem stItem, int iDaysToExpire)
{
	LPOBJ lpObj = &gObj[iIndex];

	if ( lpObj->Type != OBJ_USER ) 
	{
		return;
	}

	if ( lpObj->m_PlayerData->m_bGremoryCaseDBInfo == false )
	{
		return;
	}

	if (stItem.btStorageType != GC_STORAGE_SERVER && stItem.btStorageType != GC_STORAGE_CHARACTER)
	{
		return;
	}

	_stReqAddItemToGremoryCase pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0x4F, 0x01, sizeof(pMsg));

	pMsg.iIndex = iIndex;
	memcpy(pMsg.szAccountID, gObj[iIndex].AccountID, MAX_ACCOUNT_LEN+1);
	memcpy(pMsg.szName, gObj[iIndex].Name, MAX_ACCOUNT_LEN+1);

	pMsg.m_GremoryCaseItem.btStorageType = stItem.btStorageType;
	pMsg.m_GremoryCaseItem.btRewardSource = stItem.btRewardSource;
	pMsg.m_GremoryCaseItem.btItemGUID = 0;
	pMsg.m_GremoryCaseItem.dwAuthCode = 0;
	pMsg.m_GremoryCaseItem.wItemID = stItem.wItemID;
	pMsg.m_GremoryCaseItem.btItemLevel = stItem.btItemLevel;
	pMsg.m_GremoryCaseItem.btItemDurability = stItem.btItemDurability;
	pMsg.m_GremoryCaseItem.btItemSkill = stItem.btItemSkill;
	pMsg.m_GremoryCaseItem.btItemLuck = stItem.btItemLuck;
	pMsg.m_GremoryCaseItem.btItemOption = stItem.btItemOption;
	pMsg.m_GremoryCaseItem.btItemExcOption = stItem.btItemExcOption;
	pMsg.m_GremoryCaseItem.btItemSetOption = stItem.btItemSetOption;
	pMsg.m_GremoryCaseItem.btItemSocketCount = stItem.btItemSocketCount;
	pMsg.m_GremoryCaseItem.btItemMainAttribute = stItem.btItemMainAttribute;
	pMsg.m_GremoryCaseItem.wMuunEvoItemType = stItem.wMuunEvoItemType;
	pMsg.m_GremoryCaseItem.btMuunEvoItemIndex = stItem.btMuunEvoItemIndex;
	pMsg.m_GremoryCaseItem.iReceiveDate = time(NULL);
	pMsg.m_GremoryCaseItem.iExpireDate = time(NULL) + (iDaysToExpire * 24 * 60 * 60);

	wsDataCli.DataSend((char *)&pMsg, pMsg.h.size);
	g_Log.Add("[Gremory Case] (%s)(%s) Request to Add item to Storage (Storage Type: %d) (Reward Source: %d) (ItemID: %d) (ExpireDays: %d)",
		lpObj->AccountID, lpObj->Name, stItem.btStorageType, stItem.btRewardSource, stItem.wItemID, iDaysToExpire);
}

void CGremoryCase::DGAnsAddItemToGremoryCase(_stAnsAddItemToGremoryCase * lpMsg)
{
	int iIndex = lpMsg->iIndex;

	if ( !ObjectMaxRange(iIndex) )
	{
		return;
	}

	LPOBJ lpObj = &gObj[iIndex];

	if ( lpObj->Type != OBJ_USER )
	{
		return;
	}

	if ( lpMsg->btResult == 1 )
	{
		g_Log.Add("[Gremory Case] (%s)(%s) Failed to Add Item to Storage - DB Error", lpObj->AccountID, lpObj->Name);
		return;
	}

	int iItemArrayIndex = -1;
	BYTE SocketOption[5] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
	BYTE SocketOptionIndex = 0xFF;

	for (int i = 0; i < MAX_GREMORYCASE_STORAGE_ITEMS; i++)
	{
		if (lpObj->m_PlayerData->m_GremoryCaseData[lpMsg->m_GremoryCaseItem.btStorageType-1][i].btRewardInventory == 0)
		{
			iItemArrayIndex = i;
			lpObj->m_PlayerData->m_GremoryCaseData[lpMsg->m_GremoryCaseItem.btStorageType-1][i].Clear();
			lpObj->m_PlayerData->m_GremoryCaseData[lpMsg->m_GremoryCaseItem.btStorageType-1][i].ItemInfo.Clear();
			lpObj->m_PlayerData->m_GremoryCaseData[lpMsg->m_GremoryCaseItem.btStorageType-1][i].btRewardInventory = lpMsg->m_GremoryCaseItem.btStorageType;
			lpObj->m_PlayerData->m_GremoryCaseData[lpMsg->m_GremoryCaseItem.btStorageType-1][i].btRewardSource = lpMsg->m_GremoryCaseItem.btRewardSource;
			lpObj->m_PlayerData->m_GremoryCaseData[lpMsg->m_GremoryCaseItem.btStorageType-1][i].dwItemGUID = lpMsg->m_GremoryCaseItem.btItemGUID;
			lpObj->m_PlayerData->m_GremoryCaseData[lpMsg->m_GremoryCaseItem.btStorageType-1][i].dwAuthCode = lpMsg->m_GremoryCaseItem.dwAuthCode;
			lpObj->m_PlayerData->m_GremoryCaseData[lpMsg->m_GremoryCaseItem.btStorageType-1][i].dwReceiveDate = lpMsg->m_GremoryCaseItem.iReceiveDate;
			lpObj->m_PlayerData->m_GremoryCaseData[lpMsg->m_GremoryCaseItem.btStorageType-1][i].dwExpireTime = lpMsg->m_GremoryCaseItem.iExpireDate;

			lpObj->m_PlayerData->m_GremoryCaseData[lpMsg->m_GremoryCaseItem.btStorageType-1][i].ItemInfo.m_Type = lpMsg->m_GremoryCaseItem.wItemID;
			lpObj->m_PlayerData->m_GremoryCaseData[lpMsg->m_GremoryCaseItem.btStorageType-1][i].ItemInfo.m_Level = lpMsg->m_GremoryCaseItem.btItemLevel;
			lpObj->m_PlayerData->m_GremoryCaseData[lpMsg->m_GremoryCaseItem.btStorageType-1][i].ItemInfo.m_Durability = ItemGetDurability(lpMsg->m_GremoryCaseItem.wItemID, lpMsg->m_GremoryCaseItem.btItemLevel, lpMsg->m_GremoryCaseItem.btItemExcOption, lpMsg->m_GremoryCaseItem.btItemSetOption);

			if (g_SocketOptionSystem.IsEnableSocketItem(&lpObj->m_PlayerData->m_GremoryCaseData[lpMsg->m_GremoryCaseItem.btStorageType-1][i].ItemInfo) == true)
			{
				if (lpMsg->m_GremoryCaseItem.btItemSocketCount == 0)
				{
					g_SocketOptionSystem.MakeSocketSlot(&lpObj->m_PlayerData->m_GremoryCaseData[lpMsg->m_GremoryCaseItem.btStorageType-1][i].ItemInfo, rand() % 5 + 1);
				}

				else
				{
					g_SocketOptionSystem.MakeSocketSlot(&lpObj->m_PlayerData->m_GremoryCaseData[lpMsg->m_GremoryCaseItem.btStorageType-1][i].ItemInfo, lpMsg->m_GremoryCaseItem.btItemSocketCount);
				}

				SocketOptionIndex = -1;
			}

			else if (g_PentagramSystem.IsPentagramItem(lpObj->m_PlayerData->m_GremoryCaseData[lpMsg->m_GremoryCaseItem.btStorageType-1][i].ItemInfo.m_Type) == true)
			{
				SocketOptionIndex = lpMsg->m_GremoryCaseItem.btItemMainAttribute;

				if (lpMsg->m_GremoryCaseItem.btItemSocketCount == 0)
				{
					g_PentagramSystem.GetMakePentagramSlotCountNKind(SocketOption, lpObj->m_PlayerData->m_GremoryCaseData[lpMsg->m_GremoryCaseItem.btStorageType-1][i].ItemInfo.m_Type);
				}

				else
				{
					BYTE Slot[5] = { 0, 0, 0, 0, 0 };

					for (int i = 0; i < lpMsg->m_GremoryCaseItem.btItemSocketCount; i++)
					{
						Slot[i] = 1;
					}

					g_PentagramSystem.MakePentagramSocketSlot(&lpObj->m_PlayerData->m_GremoryCaseData[lpMsg->m_GremoryCaseItem.btStorageType-1][i].ItemInfo, Slot[0], Slot[1], Slot[2], Slot[3], Slot[4]);
				}
			}

			else if (g_CMuunSystem.IsStoneofEvolution(lpObj->m_PlayerData->m_GremoryCaseData[lpMsg->m_GremoryCaseItem.btStorageType-1][i].ItemInfo.m_Type) == TRUE)
			{
				int iMuunEvolutionItemID = ITEMGET(lpMsg->m_GremoryCaseItem.wMuunEvoItemType, lpMsg->m_GremoryCaseItem.btMuunEvoItemIndex);
				
				SocketOption[0] = iMuunEvolutionItemID >> 8;
				SocketOption[1] = ITEM_GET_INDEX(iMuunEvolutionItemID);
			}

			lpObj->m_PlayerData->m_GremoryCaseData[lpMsg->m_GremoryCaseItem.btStorageType-1][i].ItemInfo.Convert(lpMsg->m_GremoryCaseItem.wItemID, lpMsg->m_GremoryCaseItem.btItemSkill,
				lpMsg->m_GremoryCaseItem.btItemLuck, lpMsg->m_GremoryCaseItem.btItemOption, lpMsg->m_GremoryCaseItem.btItemExcOption, lpMsg->m_GremoryCaseItem.btItemSetOption, 0, SocketOption, SocketOptionIndex, 0, 3);
			break;
		}
	}
	
	if (iItemArrayIndex == -1)
	{
		g_Log.Add("[Gremory Case] (%s)(%s) Failed to Add Item to Storage - No space in array", lpObj->AccountID, lpObj->Name);
		return;
	}

	g_Log.Add("[Gremory Case] (%s)(%s) Success to Add Item to Storage (Storage: %d) (Source: %d) (ItemGUID: %d) (AuthCode: %d)",
		lpObj->AccountID, lpObj->Name, lpMsg->m_GremoryCaseItem.btStorageType, lpMsg->m_GremoryCaseItem.btRewardSource, lpMsg->m_GremoryCaseItem.btItemGUID, lpMsg->m_GremoryCaseItem.dwAuthCode);

	this->GCSendAddItemToGremoryCase(iIndex, lpMsg->m_GremoryCaseItem.btStorageType-1, iItemArrayIndex);
}

void CGremoryCase::GDReqCheckItemUseGremoryCase(int iIndex, WORD wItemID, DWORD dwItemGUID, DWORD dwAuthCode)
{
	_stReqCheckUseItemGremoryCase pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0x4F, 0x02, sizeof(pMsg));

	pMsg.iIndex = iIndex;
	memcpy(pMsg.szAccountID, gObj[iIndex].AccountID, MAX_ACCOUNT_LEN+1);
	memcpy(pMsg.szName, gObj[iIndex].Name, MAX_ACCOUNT_LEN+1);

	pMsg.wItemID = wItemID;
	pMsg.dwItemGUID = dwItemGUID;
	pMsg.dwAuthCode = dwAuthCode;

	wsDataCli.DataSend((char *)&pMsg, pMsg.h.size);
	g_Log.Add("[Gremory Case] (%s)(%s) Request to Use Item (ItemID: %d) (ItemGUID: %d) (AuthCode: %d)",
		gObj[iIndex].AccountID, gObj[iIndex].Name, wItemID, dwItemGUID, dwAuthCode);
}

void CGremoryCase::DGAnsCheckItemUseGremoryCase(_stAnsCheckUseItemGremoryCase * lpMsg)
{
	int iIndex = lpMsg->iIndex;

	if ( !ObjectMaxRange(iIndex) )
	{
		return;
	}

	LPOBJ lpObj = &gObj[iIndex];

	if ( lpObj->Type != OBJ_USER )
	{
		return;
	}

	PMSG_ADD_GREMORYCASE_ITEM_TO_INVENTORY_ANS pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0x4F, 0x02, sizeof(pMsg));

	if ( lpObj->m_PlayerData->m_bGremoryCaseDBInfo == false )
	{
		pMsg.btResult = 1;
		IOCP.DataSend(iIndex, (LPBYTE)&pMsg, pMsg.h.size);

		g_Log.Add("[Gremory Case] (%s)(%s) Failed to Add Item to Inventory - Data not loaded from DB", lpObj->AccountID, lpObj->Name);
		return;
	}

	int iStorageType = -1;
	int iItemArrayIndex = -1;

	for (int i = 0; i < MAX_GREMORYCASE_STORAGE_TYPES; i++)
	{
		for (int j = 0; j < MAX_GREMORYCASE_STORAGE_ITEMS; j++)
		{
			if ( lpObj->m_PlayerData->m_GremoryCaseData[i][j].btRewardInventory != 0)
			{
				if ( lpObj->m_PlayerData->m_GremoryCaseData[i][j].ItemInfo.m_Type == lpMsg->wItemID &&
					lpObj->m_PlayerData->m_GremoryCaseData[i][j].dwItemGUID == lpMsg->dwItemGUID &&
					lpObj->m_PlayerData->m_GremoryCaseData[i][j].dwAuthCode == lpMsg->dwAuthCode)
				{
					iStorageType = i;
					iItemArrayIndex = j;
					break;
				}
			}
		}
	}

	if ( iStorageType == -1 || iItemArrayIndex == -1 )
	{
		pMsg.btResult = 2;
		IOCP.DataSend(iIndex, (LPBYTE)&pMsg, pMsg.h.size);

		g_Log.Add("[Gremory Case] (%s)(%s) Failed to Add Item to Inventory - Item not found", lpObj->AccountID, lpObj->Name);
		return;
	}

	if ( lpMsg->btResult == 0 )
	{
		pMsg.btResult = 3;
		IOCP.DataSend(iIndex, (LPBYTE)&pMsg, pMsg.h.size);

		g_Log.Add("[Gremory Case] (%s)(%s) Failed to Add Item to Inventory - Item not Exists in DB", lpObj->AccountID, lpObj->Name);
		return;
	}

	LPITEM_ATTRIBUTE p = GetItemAttr(lpMsg->wItemID);
	bool IsInventoryEmptySpace;

	if ( p->ItemKindA == 11 )
	{
		IsInventoryEmptySpace = CheckEventInventoryEmptySpace(lpObj, p->Height, p->Width);
	}

	else if ( p->ItemKindA == 12 )
	{
		IsInventoryEmptySpace = gObjChkMuunInventoryEmpty(lpObj);
	}

	else
	{
		IsInventoryEmptySpace = CheckInventoryEmptySpace(lpObj, p->Height, p->Width);
	}

	if ( IsInventoryEmptySpace == false )
	{
		pMsg.btResult = 4;
		IOCP.DataSend(iIndex, (LPBYTE)&pMsg, pMsg.h.size);

		g_Log.Add("[Gremory Case] (%s)(%s) Failed to Add Item to Inventory - Not enough empty space", lpObj->AccountID, lpObj->Name);
		return;
	}

	if (lpObj->m_IfState.use)
	{
		pMsg.btResult = 5;
		IOCP.DataSend(iIndex, (LPBYTE)&pMsg, pMsg.h.size);

		g_Log.Add("[Gremory Case] (%s)(%s) Failed to Add Item to Inventory - IfState check error", lpObj->AccountID, lpObj->Name);
		return;
	}

	if (p->ItemKindA == 11)
	{
		ItemSerialCreateSend(iIndex, 226, 0, 0, lpObj->m_PlayerData->m_GremoryCaseData[iStorageType][iItemArrayIndex].ItemInfo.m_Type, lpObj->m_PlayerData->m_GremoryCaseData[iStorageType][iItemArrayIndex].ItemInfo.m_Level,
			lpObj->m_PlayerData->m_GremoryCaseData[iStorageType][iItemArrayIndex].ItemInfo.m_Durability, lpObj->m_PlayerData->m_GremoryCaseData[iStorageType][iItemArrayIndex].ItemInfo.m_Option1,
			lpObj->m_PlayerData->m_GremoryCaseData[iStorageType][iItemArrayIndex].ItemInfo.m_Option2, lpObj->m_PlayerData->m_GremoryCaseData[iStorageType][iItemArrayIndex].ItemInfo.m_Option3,
			iIndex, lpObj->m_PlayerData->m_GremoryCaseData[iStorageType][iItemArrayIndex].ItemInfo.m_NewOption, lpObj->m_PlayerData->m_GremoryCaseData[iStorageType][iItemArrayIndex].ItemInfo.m_SetOption, 0,
			lpObj->m_PlayerData->m_GremoryCaseData[iStorageType][iItemArrayIndex].ItemInfo.m_SocketOption, lpObj->m_PlayerData->m_GremoryCaseData[iStorageType][iItemArrayIndex].ItemInfo.m_BonusSocketOption);
			
	}

	else if (p->ItemKindA == 12)
	{
		BYTE SocketOption[5] = { -1, -1, -1, -1, -1 };

		ItemSerialCreateSend(iIndex, 224, 0, 0, lpObj->m_PlayerData->m_GremoryCaseData[iStorageType][iItemArrayIndex].ItemInfo.m_Type, lpObj->m_PlayerData->m_GremoryCaseData[iStorageType][iItemArrayIndex].ItemInfo.m_Level,
			lpObj->m_PlayerData->m_GremoryCaseData[iStorageType][iItemArrayIndex].ItemInfo.m_Durability, lpObj->m_PlayerData->m_GremoryCaseData[iStorageType][iItemArrayIndex].ItemInfo.m_Option1,
			lpObj->m_PlayerData->m_GremoryCaseData[iStorageType][iItemArrayIndex].ItemInfo.m_Option2, lpObj->m_PlayerData->m_GremoryCaseData[iStorageType][iItemArrayIndex].ItemInfo.m_Option3,
			iIndex, lpObj->m_PlayerData->m_GremoryCaseData[iStorageType][iItemArrayIndex].ItemInfo.m_NewOption, lpObj->m_PlayerData->m_GremoryCaseData[iStorageType][iItemArrayIndex].ItemInfo.m_SetOption, 0,
			SocketOption, -1);
	}

	else
	{
		ItemSerialCreateSend(iIndex, 235, 0, 0, lpObj->m_PlayerData->m_GremoryCaseData[iStorageType][iItemArrayIndex].ItemInfo.m_Type, lpObj->m_PlayerData->m_GremoryCaseData[iStorageType][iItemArrayIndex].ItemInfo.m_Level,
			lpObj->m_PlayerData->m_GremoryCaseData[iStorageType][iItemArrayIndex].ItemInfo.m_Durability, lpObj->m_PlayerData->m_GremoryCaseData[iStorageType][iItemArrayIndex].ItemInfo.m_Option1,
			lpObj->m_PlayerData->m_GremoryCaseData[iStorageType][iItemArrayIndex].ItemInfo.m_Option2, lpObj->m_PlayerData->m_GremoryCaseData[iStorageType][iItemArrayIndex].ItemInfo.m_Option3,
			iIndex, lpObj->m_PlayerData->m_GremoryCaseData[iStorageType][iItemArrayIndex].ItemInfo.m_NewOption, lpObj->m_PlayerData->m_GremoryCaseData[iStorageType][iItemArrayIndex].ItemInfo.m_SetOption, 0,
			lpObj->m_PlayerData->m_GremoryCaseData[iStorageType][iItemArrayIndex].ItemInfo.m_SocketOption, lpObj->m_PlayerData->m_GremoryCaseData[iStorageType][iItemArrayIndex].ItemInfo.m_BonusSocketOption);
	}

	pMsg.btResult = 0;
	pMsg.btStorageType = iStorageType+1;
	pMsg.wItemID = lpMsg->wItemID;
	pMsg.dwItemGUID = lpMsg->dwItemGUID;
	pMsg.dwAuthCode = lpMsg->dwAuthCode;

	this->GDReqDeleteItemFromGremoryCase(iIndex, lpMsg->wItemID, lpMsg->dwItemGUID, lpMsg->dwAuthCode);
	lpObj->m_PlayerData->m_GremoryCaseData[iStorageType][iItemArrayIndex].Clear();

	IOCP.DataSend(iIndex, (LPBYTE)&pMsg, pMsg.h.size);

	g_Log.Add("[Gremory Case] (%s)(%s) Success to Add Item to Inventory (ItemID: %d) (ItemGUID: %d) (AuthCode: %d)",
		lpObj->AccountID, lpObj->Name, lpMsg->wItemID, lpMsg->dwItemGUID, lpMsg->dwAuthCode);
}

void CGremoryCase::GDReqDeleteItemFromGremoryCase(int iIndex, WORD wItemID, DWORD dwItemGUID, DWORD dwAuthCode)
{
	_stReqDeleteItemFromGremoryCase pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0x4F, 0x03, sizeof(pMsg));

	memcpy(pMsg.szAccountID, gObj[iIndex].AccountID, MAX_ACCOUNT_LEN+1);
	memcpy(pMsg.szName, gObj[iIndex].Name, MAX_ACCOUNT_LEN+1);

	pMsg.wItemID = wItemID;
	pMsg.dwItemGUID = dwItemGUID;
	pMsg.dwAuthCode = dwAuthCode;

	wsDataCli.DataSend((char *)&pMsg, pMsg.h.size);
	g_Log.Add("[Gremory Case] (%s)(%s) Request to Delete Item (ItemID: %d) (ItemGUID: %d) (AuthCode: %d)",
		gObj[iIndex].AccountID, gObj[iIndex].Name, wItemID, dwItemGUID, dwAuthCode);
}

void CGremoryCase::GCSendStorageItemList(int iIndex)
{
	LPOBJ lpObj = &gObj[iIndex];

	if ( lpObj->Type != OBJ_USER ) 
	{
		return;
	}

	if ( lpObj->m_PlayerData->m_bGremoryCaseDBInfo == false )
	{
		return;
	}

	BYTE BUFFER[8192];
	PMSG_RECEIVE_GREMORYCASE_ITEMLIST * lpMsg = (PMSG_RECEIVE_GREMORYCASE_ITEMLIST *)(BUFFER);
	PMSG_GREMORYCASE_ITEM * lpItem;

	lpMsg->btCount = 0;

	for (int i = 0; i < MAX_GREMORYCASE_STORAGE_TYPES; i++)
	{
		for (int j = 0; j < MAX_GREMORYCASE_STORAGE_ITEMS; j++)
		{
			if (lpObj->m_PlayerData->m_GremoryCaseData[i][j].btRewardInventory != 0)
			{
				lpItem = (PMSG_GREMORYCASE_ITEM *)(BUFFER + sizeof(PMSG_RECEIVE_GREMORYCASE_ITEMLIST) + (lpMsg->btCount * sizeof(PMSG_GREMORYCASE_ITEM)));

				lpItem->btRewardInventory = lpObj->m_PlayerData->m_GremoryCaseData[i][j].btRewardInventory;
				lpItem->btRewardSource = lpObj->m_PlayerData->m_GremoryCaseData[i][j].btRewardSource;
				lpItem->dwItemGUID = lpObj->m_PlayerData->m_GremoryCaseData[i][j].dwItemGUID;
				lpItem->dwAuthCode = lpObj->m_PlayerData->m_GremoryCaseData[i][j].dwAuthCode;
				lpItem->dwExpireTime = lpObj->m_PlayerData->m_GremoryCaseData[i][j].dwExpireTime;		
				ItemByteConvert(lpItem->btItemInfo, lpObj->m_PlayerData->m_GremoryCaseData[i][j].ItemInfo);

				lpMsg->btCount++;
			}
		}
	}

	int PacketSize = sizeof(PMSG_RECEIVE_GREMORYCASE_ITEMLIST) + (lpMsg->btCount * sizeof(PMSG_GREMORYCASE_ITEM));
	PHeadSubSetW((LPBYTE)lpMsg, 0x4F, 0x00, PacketSize);

	IOCP.DataSend(iIndex, BUFFER, PacketSize);
}

void CGremoryCase::GCSendAddItemToGremoryCase(int iIndex, BYTE btStorageType, BYTE btItemArrayIndex)
{
	if (!ObjectMaxRange(iIndex))
	{
		return;
	}

	if (!gObjIsConnected(iIndex))
	{
		return;
	}

	LPOBJ lpObj = &gObj[iIndex];

	if ( lpObj->Type != OBJ_USER ) 
	{
		return;
	}

	PMSG_RECEIVE_GREMORYCASE_ITEM pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0x4F, 0x01, sizeof(pMsg));

	pMsg.m_ReceivedItem.btRewardInventory = lpObj->m_PlayerData->m_GremoryCaseData[btStorageType][btItemArrayIndex].btRewardInventory;
	pMsg.m_ReceivedItem.btRewardSource = lpObj->m_PlayerData->m_GremoryCaseData[btStorageType][btItemArrayIndex].btRewardSource;
	pMsg.m_ReceivedItem.dwItemGUID = lpObj->m_PlayerData->m_GremoryCaseData[btStorageType][btItemArrayIndex].dwItemGUID;
	pMsg.m_ReceivedItem.dwAuthCode = lpObj->m_PlayerData->m_GremoryCaseData[btStorageType][btItemArrayIndex].dwAuthCode;
	pMsg.m_ReceivedItem.dwExpireTime = lpObj->m_PlayerData->m_GremoryCaseData[btStorageType][btItemArrayIndex].dwExpireTime;		
	ItemByteConvert(pMsg.m_ReceivedItem.btItemInfo, lpObj->m_PlayerData->m_GremoryCaseData[btStorageType][btItemArrayIndex].ItemInfo);

	IOCP.DataSend(iIndex, (LPBYTE)&pMsg, pMsg.h.size);

	this->GCSendStorageItemList(iIndex);
}

void CGremoryCase::CGReqGetItemFromGremoryCase(PMSG_ADD_GREMORYCASE_ITEM_TO_INVENTORY_REQ * lpMsg, int iIndex)
{
	if (!ObjectMaxRange(iIndex))
	{
		return;
	}

	if (!gObjIsConnected(iIndex))
	{
		return;
	}

	LPOBJ lpObj = &gObj[iIndex];

	if ( lpObj->Type != OBJ_USER ) 
	{
		return;
	}

	this->GDReqCheckItemUseGremoryCase(iIndex, lpMsg->wItemID, lpMsg->dwItemGUID, lpMsg->dwAuthCode);
}

void CGremoryCase::GCSendDeleteItemFromGremoryCase(int iIndex, BYTE btStorageType, int iItemArrayIndex)
{
	PMSG_REMOVE_ITEM_FROM_GREMORYCASE pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0x4F, 0x03, sizeof(pMsg));

	pMsg.btStorageType = btStorageType+1;
	pMsg.wItemID = gObj[iIndex].m_PlayerData->m_GremoryCaseData[btStorageType][iItemArrayIndex].ItemInfo.m_Type;
	pMsg.dwItemGUID = gObj[iIndex].m_PlayerData->m_GremoryCaseData[btStorageType][iItemArrayIndex].dwItemGUID;
	pMsg.dwAuthCode =  gObj[iIndex].m_PlayerData->m_GremoryCaseData[btStorageType][iItemArrayIndex].dwAuthCode;

	IOCP.DataSend(iIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

void CGremoryCase::CGReqOpenGremoryCase(int iIndex)
{
	if (!ObjectMaxRange(iIndex))
	{
		return;
	}

	if (!gObjIsConnected(iIndex))
	{
		return;
	}

	LPOBJ lpObj = &gObj[iIndex];

	if ( lpObj->Type != OBJ_USER ) 
	{
		return;
	}

	PMSG_GREMORYCASE_OPEN_ANS pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0x4F, 0x05, sizeof(pMsg));

	if ( lpObj->m_PlayerData->m_bGremoryCaseDBInfo == false )
	{
		pMsg.btResult = 1;
		IOCP.DataSend(iIndex, (LPBYTE)&pMsg, pMsg.h.size);

		g_Log.Add("[Gremory Case] (%s)(%s) Open failed - No info from DB.", lpObj->AccountID, lpObj->Name);
		return;
	}

	pMsg.btResult = 0;
	IOCP.DataSend(iIndex, (LPBYTE)&pMsg, pMsg.h.size);

	g_Log.Add("[Gremory Case] (%s)(%s) Open Gremory Case.", lpObj->AccountID, lpObj->Name);
}

void CGremoryCase::CheckIsInStorageItemAboutToExpire(int iIndex)
{
	LPOBJ lpObj = &gObj[iIndex];

	if ( lpObj->Type != OBJ_USER ) 
	{
		return;
	}

	if ( lpObj->m_PlayerData->m_bGremoryCaseDBInfo == false )
	{
		return;
	}

	bool bItemFound = false;

	time_t TimeCheck = time(NULL) + (7 * 24 * 60 * 60); // add 7 days to date

	for (int i = 0; i < MAX_GREMORYCASE_STORAGE_TYPES; i++)
	{
		for (int j = 0; j < MAX_GREMORYCASE_STORAGE_ITEMS; j++)
		{
			if (lpObj->m_PlayerData->m_GremoryCaseData[i][j].btRewardInventory != 0)
			{
				if (lpObj->m_PlayerData->m_GremoryCaseData[i][j].dwExpireTime < TimeCheck)
				{
					bItemFound = true;
					break;
				}
			}
		}
	}

	if (bItemFound == false)
	{
		return;
	}

	PMSG_GREMORYCASE_SENT_NOTICE pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0x4F, 0x04, sizeof(pMsg));
	pMsg.btNoticeIndex = 1;

	IOCP.DataSend(iIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

void CGremoryCase::CheckIsStorageFull(int iIndex)
{
	LPOBJ lpObj = &gObj[iIndex];

	if ( lpObj->Type != OBJ_USER ) 
	{
		return;
	}

	if ( lpObj->m_PlayerData->m_bGremoryCaseDBInfo == false )
	{
		return;
	}

	BYTE btStorageState = 0;
	int iCharacterStorageCount = 0, iServerStorageCount = 0;

	for (int i = 0; i < MAX_GREMORYCASE_STORAGE_TYPES; i++)
	{
		for (int j = 0; j < MAX_GREMORYCASE_STORAGE_ITEMS; j++)
		{
			if (lpObj->m_PlayerData->m_GremoryCaseData[i][j].btRewardInventory != 0)
			{
				if (lpObj->m_PlayerData->m_GremoryCaseData[i][j].btRewardInventory == GC_STORAGE_SERVER)
				{
					iServerStorageCount++;
				}

				else if (lpObj->m_PlayerData->m_GremoryCaseData[i][j].btRewardInventory == GC_STORAGE_CHARACTER)
				{
					iCharacterStorageCount++;
				}
			}
		}
	}

	if (iServerStorageCount >= MAX_GREMORYCASE_STORAGE_ITEMS || iCharacterStorageCount >= MAX_GREMORYCASE_STORAGE_ITEMS)
	{
		btStorageState = 1;
	}

	else if ((iServerStorageCount >= MAX_GREMORYCASE_STORAGE_ITEMS-5 && iServerStorageCount < MAX_GREMORYCASE_STORAGE_ITEMS) || (iCharacterStorageCount >= MAX_GREMORYCASE_STORAGE_ITEMS-5 && iCharacterStorageCount < MAX_GREMORYCASE_STORAGE_ITEMS))
	{
		btStorageState = 2;
	}

	if ( btStorageState == 0 )
	{
		return;
	}

	PMSG_GREMORYCASE_SENT_NOTICE pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0x4F, 0x04, sizeof(pMsg));
	pMsg.btNoticeIndex = btStorageState == 2 ? 5 : 6;

	IOCP.DataSend(iIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


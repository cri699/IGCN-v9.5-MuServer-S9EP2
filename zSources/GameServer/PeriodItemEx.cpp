//////////////////////////////////////////////////////////////////////
// PeriodItemEx.cpp
// Decompiled by Dudas [IGC] July 2013
// Status: 100%

#include "stdafx.h"
#include "PeriodItemEx.h"
#include "BuffEffectSlot.h"
#include "ItemAddOption.h"
#include "user.h"
#include "ObjUseSkill.h"
#include "ObjCalCharacter.h"
#include "DSProtocol.h"
#include "GameMain.h"
#include "TLog.h"
#include "protocol.h"
#include "configread.h"

CPeriodItemEx g_PeriodItemEx;

CPeriodItemEx::CPeriodItemEx(void)
{
}

CPeriodItemEx::~CPeriodItemEx(void)
{
	if ( this->m_PeriodData != NULL )
	{
		delete [] this->m_PeriodData;
	}
}

void CPeriodItemEx::Initialize()
{
	if ( this->m_PeriodData != NULL )
	{
		delete [] this->m_PeriodData;
	}

	this->m_PeriodData = new PERIOD_DATA[g_ConfigRead.server.GetObjectMaxUser()];

	for (int i=0; i<g_ConfigRead.server.GetObjectMaxUser(); i++)
	{
		this->m_PeriodData[i].Clear();
	}
}

int CPeriodItemEx::AddPeriodInfo(LPOBJ lpObj)
{
	if ( lpObj->Type != OBJ_USER )
	{
		return -1;
	}

	for ( int i=0; i<g_ConfigRead.server.GetObjectMaxUser(); i++ )
	{
		if ( this->m_PeriodData[i].btUsedInfo == FALSE )
		{
			this->m_PeriodData[i].Clear();

			this->m_PeriodData[i].btUsedInfo = TRUE;
			this->m_PeriodData[i].m_Obj = lpObj;
			this->m_PeriodData[i].dwUserGuid = lpObj->DBNumber;
			this->m_PeriodData[i].wUserIndex = lpObj->m_Index;
			this->m_PeriodData[i].btItemCount = 0;
			memcpy(this->m_PeriodData[i].szAccountID, lpObj->AccountID, MAX_ACCOUNT_LEN+1);
			memcpy(this->m_PeriodData[i].szCharacterName, lpObj->Name, MAX_ACCOUNT_LEN+1);
			
			lpObj->m_PlayerData->m_iPeriodItemEffectIndex = i;

			return i;
		}
	}

	return -1;
}

BOOL CPeriodItemEx::RemovePeriodInfo(LPOBJ lpObj)
{
	if ( lpObj->Type != OBJ_USER )
	{
		return FALSE;
	}

	int PeriodItemIndex = lpObj->m_PlayerData->m_iPeriodItemEffectIndex;

	if ( this->IsCorrectUser(lpObj) == FALSE )
	{
		return FALSE;
	}

	if ( PeriodItemIndex < 0 || PeriodItemIndex >= g_ConfigRead.server.GetObjectMaxUser())
	{
		return FALSE;
	}

	if ( this->m_PeriodData[PeriodItemIndex].btUsedInfo == FALSE )
	{
		return FALSE;
	}

	this->m_PeriodData[PeriodItemIndex].Clear();

	return TRUE;
}

BOOL CPeriodItemEx::IsCorrectUser(LPOBJ lpObj)
{
	if ( lpObj->Type != OBJ_USER )
	{
		return FALSE;
	}

	int PeriodItemIndex = lpObj->m_PlayerData->m_iPeriodItemEffectIndex;

	if ( PeriodItemIndex >= 0 )
	{
		if ( this->m_PeriodData[PeriodItemIndex].btUsedInfo == TRUE && this->m_PeriodData[PeriodItemIndex].dwUserGuid == lpObj->DBNumber )
		{
			return TRUE;
		}
	}

	return FALSE;
}

int CPeriodItemEx::AddPeriodItemData(LPOBJ lpObj, WORD ItemType, UINT64 Serial, DWORD Duration)
{
	if ( lpObj->Type != OBJ_USER) 
	{
		return -1;
	}

	int PeriodItemIndex = lpObj->m_PlayerData->m_iPeriodItemEffectIndex;

	if ( this->IsCorrectUser(lpObj) == FALSE )
	{
		return -1;
	}

	for ( int i = 0; i < MAX_PERIOD_ITEMS; i++ )
	{
		if ( this->m_PeriodData[PeriodItemIndex].m_ItemInfo[i].btUsedInfo == FALSE )
		{
			time_t ExpireDate = this->GetExpireDate(Duration);
			this->m_PeriodData[PeriodItemIndex].m_ItemInfo[i].btUsedInfo = TRUE;
			this->m_PeriodData[PeriodItemIndex].m_ItemInfo[i].btItemType = PERIODITEM_NORMAL;
			this->m_PeriodData[PeriodItemIndex].m_ItemInfo[i].wItemCode = ItemType;
			this->m_PeriodData[PeriodItemIndex].m_ItemInfo[i].Serial = Serial;
			this->m_PeriodData[PeriodItemIndex].m_ItemInfo[i].lItemBuyDate = this->GetCurrentDate();
			this->m_PeriodData[PeriodItemIndex].m_ItemInfo[i].lItemExpireDate = ExpireDate;
			this->m_PeriodData[PeriodItemIndex].m_ItemInfo[i].dwDuration = Duration;
			this->m_PeriodData[PeriodItemIndex].btItemCount++;

			int itempos = this->GetItemFromInventory(lpObj, ItemType, Serial);
			
			if ( itempos == -1 )
			{
				this->m_PeriodData[PeriodItemIndex].m_ItemInfo[i].Clear();
				this->m_PeriodData[PeriodItemIndex].btItemCount--;	
				return -2;
			}
			
			else
			{
				if ( lpObj->pInventory[itempos].IsPeriodItem() == FALSE )
				{
					lpObj->pInventory[itempos].SetPeriodItem();
				}

				return i;
			}		
		}
	}

	return -1;
}

int CPeriodItemEx::AddPeriodItemData(LPOBJ lpObj, WORD ItemType, UINT64 Serial, DWORD Duration, time_t ExpireDate)
{
	if ( lpObj->Type != OBJ_USER) 
	{
		return -1;
	}

	int PeriodItemIndex = lpObj->m_PlayerData->m_iPeriodItemEffectIndex;

	if ( this->IsCorrectUser(lpObj) == FALSE )
	{
		return -1;
	}

	for ( int i=0; i<30 ; i++ )
	{
		if ( this->m_PeriodData[PeriodItemIndex].m_ItemInfo[i].btUsedInfo == FALSE )
		{
			this->m_PeriodData[PeriodItemIndex].m_ItemInfo[i].btUsedInfo = TRUE;
			this->m_PeriodData[PeriodItemIndex].m_ItemInfo[i].btItemType = PERIODITEM_NORMAL;
			this->m_PeriodData[PeriodItemIndex].m_ItemInfo[i].wItemCode = ItemType;
			this->m_PeriodData[PeriodItemIndex].m_ItemInfo[i].Serial = Serial;
			this->m_PeriodData[PeriodItemIndex].m_ItemInfo[i].lItemExpireDate = ExpireDate;
			this->m_PeriodData[PeriodItemIndex].m_ItemInfo[i].dwDuration = Duration;
			this->m_PeriodData[PeriodItemIndex].btItemCount++;

			int itempos = this->GetItemFromInventory(lpObj, ItemType, Serial);

			if ( itempos != -1 )
			{
				lpObj->pInventory[itempos].SetPeriodItem();
			}

			return i;
		}
	}

	return -1;
}

BOOL CPeriodItemEx::RemovePeriodItemData(LPOBJ lpObj, BYTE ItemType, WORD ItemIndex, UINT64 Serial)
{
	if ( !lpObj )
	{
		return FALSE;
	}

	if ( lpObj->Connected < PLAYER_LOGGED )
	{
		return FALSE;
	}

	if ( lpObj->Type != OBJ_USER )
	{
		return FALSE;
	}

	if ( ItemType == PERIODITEM_BUFF )
	{	
		LPBUFF_EFFECT_DATE lpEffect = g_BuffEffectSlot.GetEffectDataFromItemCode(ItemIndex);

		if ( lpEffect == NULL )
		{
			return FALSE;
		}

		gObjRemoveBuffEffect(lpObj, lpEffect->BuffIndex);
	}

	else if ( ItemType == PERIODITEM_NORMAL )
	{
		if ( Serial == 0 )
		{
			return FALSE;
		}

		int pos = this->GetItemFromInventory(lpObj, ItemIndex, Serial);

		if ( pos == -1 )
		{
			return FALSE;
		}

		this->SetDisableItemToExpire(lpObj, pos);
	}

	ITEM_DATA m_ItemData;
	
	m_ItemData.btItemType = ItemType;
	m_ItemData.wItemCode = ItemIndex;
	m_ItemData.Serial = Serial;

	this->RequestPeriodItemDelete(lpObj, &m_ItemData);

	return TRUE;
}

BOOL CPeriodItemEx::RemovePeriodItemDataByForce(LPOBJ lpObj, WORD ItemType, UINT64 Serial)
{
	if ( !lpObj )
	{
		return FALSE;
	}

	if ( lpObj->Connected < PLAYER_LOGGED )
	{
		return FALSE;
	}

	if ( lpObj->Type != OBJ_USER )
	{
		return FALSE;
	}

	if ( Serial == 0 )
	{
		return FALSE;
	}

	int iInventoryPosition = this->GetItemFromInventory(lpObj, ItemType, Serial);

	if ( iInventoryPosition == -1 )
	{
		return FALSE;
	}

	for (int i = 0; i < g_ConfigRead.server.GetObjectMaxUser(); i++)
	{
		if ( this->m_PeriodData[i].btUsedInfo == TRUE )
		{
			if ( this->m_PeriodData[i].m_Obj->DBNumber == lpObj->DBNumber )
			{
				for ( int iItemSlotIndex = 0; iItemSlotIndex < 30; iItemSlotIndex++ )
				{
					if ( this->m_PeriodData[i].m_ItemInfo[iItemSlotIndex].Serial == Serial )
					{
						g_Log.Add("[PeriodItemEx][Remove PeriodItem By Force] Remove PeriodItem Data By Force. User Id : %s(%d), Name : %s, Item : %s(%d), Type : %d, Serial : %I64d, ExpireDate : %I64d",
							lpObj->AccountID, lpObj->DBNumber, lpObj->Name, ItemAttribute[this->m_PeriodData[i].m_ItemInfo[iItemSlotIndex].wItemCode].Name,
							this->m_PeriodData[i].m_ItemInfo[iItemSlotIndex].wItemCode,
							this->m_PeriodData[i].m_ItemInfo[iItemSlotIndex].btItemType,
							this->m_PeriodData[i].m_ItemInfo[iItemSlotIndex].Serial,
							this->m_PeriodData[i].m_ItemInfo[iItemSlotIndex].lItemExpireDate);

						ITEM_DATA ItemData;

						ItemData.btItemType = 2;
						ItemData.wItemCode = ItemType;
						ItemData.Serial = Serial;
						this->RequestPeriodItemDelete(lpObj, &ItemData);
						this->m_PeriodData[i].m_ItemInfo[iItemSlotIndex].Clear();
						gObjCalCharacter.CalcCharacter(lpObj->m_Index);
						return TRUE;
					}
				}
			}
		}
	}

	return FALSE;
}

void CPeriodItemEx::CheckPeriodData()
{
	for(int i=0;i<g_ConfigRead.server.GetObjectMaxUser();i++)
	{
		if ( this->m_PeriodData[i].btUsedInfo == TRUE )
		{
			if ( this->m_PeriodData[i].m_Obj )
			{
				LPOBJ lpObj = this->m_PeriodData[i].m_Obj;

				if ( lpObj->Type == OBJ_USER )
				{
					if ( lpObj->Connected >= PLAYER_LOGGED )
					{
						for ( int j=0; j<30; j++ )
						{
							if ( this->m_PeriodData[i].m_ItemInfo[j].btUsedInfo == TRUE )
							{
								if ( this->CheckExpireDate(this->m_PeriodData[i].m_ItemInfo[j].lItemExpireDate) == TRUE )
								{
									g_Log.Add("[PeriodItemEx][Remove PeriodItem] Expire PeriodItem Data. User Id : %s(%d), Name : %s, Item : %s(%d), Type : %d, Serial : %I64d, ExpireDate : %I64d",
										lpObj->AccountID, lpObj->DBNumber, lpObj->Name, ItemAttribute[this->m_PeriodData[i].m_ItemInfo[j].wItemCode].Name, this->m_PeriodData[i].m_ItemInfo[j].wItemCode,
										this->m_PeriodData[i].m_ItemInfo[j].btItemType, this->m_PeriodData[i].m_ItemInfo[j].Serial, this->m_PeriodData[i].m_ItemInfo[j].lItemExpireDate);

									this->RemovePeriodItemData(lpObj, this->m_PeriodData[i].m_ItemInfo[j].btItemType, this->m_PeriodData[i].m_ItemInfo[j].wItemCode, this->m_PeriodData[i].m_ItemInfo[j].Serial);
									this->m_PeriodData[i].m_ItemInfo[j].Clear();
									this->m_PeriodData[i].btItemCount--;
									gObjUseSkill.SkillChangeUse(lpObj->m_Index);
									gObjCalCharacter.CalcCharacter(lpObj->m_Index);
								}
							}
						}
					}
				}
			}
		}
	}
}

time_t CPeriodItemEx::GetCurrentDate()
{
	return time(NULL);
}

time_t CPeriodItemEx::GetExpireDate(DWORD Duration)
{
	time_t expiretime = time(NULL);

	expiretime += Duration;

	return expiretime;
}

BOOL CPeriodItemEx::CheckExpireDate(time_t ExpireDate)
{
	return this->GetCurrentDate() > ExpireDate;
}

time_t CPeriodItemEx::GetLeftDate(time_t ExpireDate)
{
	time_t currtime = time(NULL);
	time_t difftime = ExpireDate - currtime;

	return difftime;
}

BOOL CPeriodItemEx::SetPeriodItemInfo(LPOBJ lpObj, WORD wItemCode, UINT64 Serial, DWORD dwDuration)
{
	if ( lpObj == NULL )
	{
		return FALSE;
	}

	if ( lpObj->Connected < PLAYER_LOGGED )
	{
		return FALSE;
	}

	if ( lpObj->Type != OBJ_USER )
	{
		return FALSE;
	}

	ITEM_DATA m_ItemData;

	

	if ( Serial != 0 )
	{
		int pos = this->GetItemFromInventory(lpObj, wItemCode, Serial);

		if ( pos == -1 )
		{
			return FALSE;
		}

		lpObj->pInventory[pos].SetPeriodItem();
		this->AddPeriodItemData(lpObj, wItemCode, Serial, dwDuration);
		m_ItemData.Clear();
		m_ItemData.wItemCode = wItemCode;
		m_ItemData.btItemType = PERIODITEM_NORMAL;
		m_ItemData.Serial = lpObj->pInventory[pos].GetNumber();		
		m_ItemData.btEffectCategory = 0;
		m_ItemData.btEffectType1 = 0;
		m_ItemData.btEffectType2 = 0;
		m_ItemData.dwDuration = dwDuration;
		m_ItemData.lItemBuyDate = this->GetCurrentDate();
		m_ItemData.lItemExpireDate = this->GetExpireDate(dwDuration);
	}

	else
	{
		LPBUFF_EFFECT_DATE lpBuffEffect = g_BuffEffectSlot.GetEffectDataFromItemCode(wItemCode);
		LPITEMEFFECT lpItemEffect = g_ItemAddOption.GetItemEffectData(wItemCode);

		if ( !lpBuffEffect || !lpItemEffect )
		{
			return FALSE;
		}

		if ( !gObjAddBuffEffectForInGameShop(lpObj, wItemCode, dwDuration) )
		{
			return FALSE;
		}

		m_ItemData.Clear();
		m_ItemData.wItemCode = wItemCode;
		m_ItemData.btItemType = PERIODITEM_BUFF;
		m_ItemData.Serial = 0;
		m_ItemData.btEffectCategory = lpBuffEffect->BuffEffectType;
		m_ItemData.btEffectType1 = (BYTE)lpItemEffect->wEffectType1;
		m_ItemData.btEffectType2 = (BYTE)lpItemEffect->wEffectType2;
		m_ItemData.dwDuration = dwDuration;
		m_ItemData.lItemBuyDate = this->GetCurrentDate();
		m_ItemData.lItemExpireDate = this->GetExpireDate(dwDuration);
	}

	

	this->RequestPeriodItemInsert(lpObj, &m_ItemData, dwDuration);
	return TRUE;
}

BOOL CPeriodItemEx::SetDisableItemToExpire(LPOBJ lpObj, int pos)
{
	if ( !lpObj )
	{
		return FALSE;
	}

	if ( lpObj->Connected < PLAYER_LOGGED )
	{
		return FALSE;
	}

	if ( lpObj->Type != OBJ_USER )
	{
		return FALSE;
	}

	if ( pos == -1 )
	{
		return FALSE;
	}

	if ( lpObj->pInventory[pos].IsPeriodItem() == FALSE )
	{
		return FALSE;
	}

	lpObj->pInventory[pos].SetPeriodItemExpire();
	GCItemListSend(lpObj->m_Index);
	this->OnRequestPeriodItemList(lpObj);

	return TRUE;
}

void CPeriodItemEx::SetExpireNotMatchedData(LPOBJ lpObj)
{
	if ( lpObj->Type != OBJ_USER )
	{
		return;
	}

	if ( lpObj->m_PlayerData->m_iPeriodItemEffectIndex == -1 )
	{
		return;
	}

	int ItemCount = 0;

	for (int i=0;i<INVENTORY_SIZE;i++)
	{
		if ( lpObj->pInventory[i].IsItem() == TRUE &&
			lpObj->pInventory[i].IsPeriodItem() == TRUE &&
			lpObj->pInventory[i].IsPeriodItemExpire() == FALSE )
		{
			BOOL bItemFound = FALSE;

			for (int j=0;j<30;j++)
			{
				if ( this->m_PeriodData[lpObj->m_PlayerData->m_iPeriodItemEffectIndex].m_ItemInfo[j].btUsedInfo == TRUE &&
					this->m_PeriodData[lpObj->m_PlayerData->m_iPeriodItemEffectIndex].m_ItemInfo[j].wItemCode == lpObj->pInventory[i].m_Type &&
					this->m_PeriodData[lpObj->m_PlayerData->m_iPeriodItemEffectIndex].m_ItemInfo[j].Serial == lpObj->pInventory[i].m_Number )
				{
					bItemFound = TRUE;
				}
			}

			if ( bItemFound == FALSE )
			{
				lpObj->pInventory[i].SetPeriodItemExpire();
				ItemCount++;
			}
		}
	}

	if ( ItemCount > 0 )
	{
		GCItemListSend(lpObj->m_Index);
	}
}

void CPeriodItemEx::RequestPeriodItemInfo(LPOBJ lpObj)
{
	PMSG_REQ_PERIODITEMEX_SELECT pMsg;

	pMsg.dwUserGuid = lpObj->DBNumber;
	pMsg.wUserIndex = lpObj->m_Index;
	memcpy(pMsg.chCharacterName, lpObj->Name, MAX_ACCOUNT_LEN+1);
	PHeadSubSetB((LPBYTE)&pMsg, 0xD0, 0x03, sizeof(pMsg));

	wsDataCli.DataSend((char *)&pMsg, sizeof(pMsg));
}

void CPeriodItemEx::RequestPeriodItemInsert(LPOBJ lpObj, ITEM_DATA * lpItemData, DWORD dwDuration)
{
	PMSG_REQ_PERIODITEMEX_INSERT pMsg;

	pMsg.dwUserGuid = lpObj->DBNumber;
	pMsg.wUserIndex = lpObj->m_Index;
	memcpy(pMsg.chCharacterName, lpObj->Name, MAX_ACCOUNT_LEN+1);

	pMsg.btItemType = lpItemData->btItemType;
	pMsg.wItemCode = lpItemData->wItemCode;
	pMsg.btEffectCategory = lpItemData->btEffectCategory;
	pMsg.btEffectType1 = lpItemData->btEffectType1;
	pMsg.btEffectType2 = lpItemData->btEffectType2;
	pMsg.Serial = lpItemData->Serial;
	pMsg.dwDuration = dwDuration;
	pMsg.lBuyDate = this->GetCurrentDate();
	pMsg.lExpireDate = this->GetExpireDate(dwDuration);

	PHeadSubSetB((LPBYTE)&pMsg, 0xD0, 0x01, sizeof(pMsg));
	g_Log.Add("[PeriodItemEx][Insert PeriodItem] Request Insert PeriodItem Data. User Id : %s(%d), Name : %s, Item : %s(%d), Type : %d, Serial : %I64d, Duration : %d",
		lpObj->AccountID, lpObj->DBNumber, lpObj->Name, ItemAttribute[lpItemData->wItemCode].Name, lpItemData->wItemCode, lpItemData->btItemType, lpItemData->Serial, dwDuration);

	wsDataCli.DataSend((char *)&pMsg, sizeof(pMsg));
}

void CPeriodItemEx::RequestPeriodItemDelete(LPOBJ lpObj, ITEM_DATA * lpItemData)
{
	PMSG_REQ_PERIODITEMEX_DELETE pMsg;

	pMsg.dwUserGuid = lpObj->DBNumber;
	pMsg.wUserIndex = lpObj->m_Index;
	memcpy(pMsg.chCharacterName, lpObj->Name, MAX_ACCOUNT_LEN+1);

	pMsg.btItemType = lpItemData->btItemType;
	pMsg.wItemCode = lpItemData->wItemCode;
	pMsg.Serial = lpItemData->Serial;

	PHeadSubSetB((LPBYTE)&pMsg, 0xD0, 0x02, sizeof(pMsg));
	g_Log.Add("[PeriodItemEx][Delete PeriodItem] Request Delete PeriodItem Data. User Id : %s(%d), Name : %s, Item : %s(%d), Type : %d, Serial : %I64d",
		lpObj->AccountID, lpObj->DBNumber, lpObj->Name, ItemAttribute[lpItemData->wItemCode].Name, lpItemData->wItemCode, lpItemData->btItemType, lpItemData->Serial);

	wsDataCli.DataSend((char *)&pMsg, sizeof(pMsg));
}

void CPeriodItemEx::DGPeriodItemExInsert(PMSG_ANS_PERIODITEMEX_INSERT * lpMsg)
{
	int UserIndex = lpMsg->wUserIndex;
	int pos = 0;

	if ( ObjectMaxRange(UserIndex) == FALSE )
	{
		return;
	}

	LPOBJ lpObj = &gObj[UserIndex];

	if ( lpObj->Connected < PLAYER_LOGGED )
	{
		return;
	}

	if ( lpObj->Type != OBJ_USER )
	{
		return;
	}

	if ( lpMsg->btResultCode == 0 )
	{
		g_Log.Add( "[PeriodItemEx][Insert PeriodItem] OnRequest Insert PeriodItem Data. User Id : %s(%d), Name : %s, Item : %s(%d), Type : %d, Serial : %I64d, Duration : %d, Result : %d",
			lpObj->AccountID, lpObj->DBNumber, lpObj->Name, ItemAttribute[lpMsg->wItemCode].Name, lpMsg->wItemCode, lpMsg->btItemtype, lpMsg->Serial, lpMsg->dwDuration, lpMsg->btResultCode);

		if (lpMsg->btItemtype == 2)
		{
			this->SendPeriodItemInfoOnce(lpObj, lpMsg->Serial);
		}
	}
}

void CPeriodItemEx::DGPeriodItemExDelete(PMSG_ANS_PERIODITEMEX_DELETE * lpMsg)
{
	int UserIndex = lpMsg->wUserIndex;

	if ( ObjectMaxRange(UserIndex) == FALSE )
	{
		return;
	}

	LPOBJ lpObj = &gObj[UserIndex];

	if ( lpObj->Connected < PLAYER_LOGGED )
	{
		return;
	}

	if ( lpObj->Type != OBJ_USER )
	{
		return;
	}

	if ( lpMsg->btResultCode == 0 )
	{
		g_Log.Add("[PeriodItemEx][Delete PeriodItem] OnRequest Delete PeriodItem Data. User Id : %s(%d), Name : %s, Result : %d",
			lpObj->AccountID, lpObj->DBNumber, lpObj->Name, lpMsg->btResultCode);
	}
}

void CPeriodItemEx::DGPeriodItemExExpiredItemList(PMSG_ANS_PERIODITEMEX_EXPIRED_ITEMLIST * lpMsg)
{
	int UserIndex = lpMsg->wUserIndex;

	if ( ObjectMaxRange(UserIndex) == FALSE )
	{
		return;
	}

	LPOBJ lpObj = &gObj[UserIndex];

	if ( lpObj->Connected < PLAYER_LOGGED )
	{
		return;
	}

	if ( lpObj->Type != OBJ_USER )
	{
		return;
	}

	int pos = -1;

	for ( int i=0; i<lpMsg->btExpiredItemCount;i++ )
	{
		pos = this->GetItemFromInventory(lpObj, lpMsg->wItemCode[i], lpMsg->Serial[i]);

		if ( pos != -1 )
		{
			g_Log.Add("[PeriodItemEx][SetExpiredItemList] OnRequest Expired Item Set Disable. User Id : %s(%d), Name : %s, Item : %s(%d), Serial : %I64d",
				lpObj->AccountID, lpObj->DBNumber, lpObj->Name, ItemAttribute[lpMsg->wItemCode[i]].Name, lpMsg->wItemCode[i], lpMsg->Serial[i]);

			this->SetDisableItemToExpire(lpObj, pos);
		}
	}
}

void CPeriodItemEx::DGPeriodItemExListCount(PMSG_ANS_PERIODITEMEX_LIST_COUNT * lpMsg)
{
	int UserIndex = lpMsg->wUserIndex;

	if ( ObjectMaxRange(UserIndex) == FALSE )
	{
		return;
	}

	LPOBJ lpObj = &gObj[UserIndex];

	if ( lpObj->Connected < PLAYER_LOGGED )
	{
		return;
	}

	if ( lpObj->Type != OBJ_USER )
	{
		return;
	}

	if ( lpMsg->dwUserGuid == lpObj->DBNumber )
	{
		g_Log.Add("[PeriodItemEx][SelectItemListCount] End PeriodItem List Receive User Id : %s(%d), Name : %s",
			lpObj->AccountID, lpObj->DBNumber, lpObj->Name);

		this->SetExpireNotMatchedData(lpObj);
		this->OnRequestPeriodItemList(lpObj);
	}
}

void CPeriodItemEx::DGPeriodItemExList(PMSG_ANS_PERIODITEMEX_LIST * lpMsg)
{
	int UserIndex = lpMsg->wUserIndex;

	if ( ObjectMaxRange(UserIndex) == FALSE )
	{
		return;
	}

	LPOBJ lpObj = &gObj[UserIndex];

	if ( lpObj->Connected < PLAYER_LOGGED )
	{
		return;
	}

	if ( lpObj->Type != OBJ_USER )
	{
		return;
	}

	time_t LeftDate = this->GetLeftDate(lpMsg->lItemExpireDate);

	if ( lpMsg->btItemType == PERIODITEM_BUFF )
	{
		if ( gObjAddBuffEffectForInGameShop(lpObj, lpMsg->wItemCode, LeftDate) == true )
		{
			g_Log.Add("[PeriodItemEx][SelectItemList] User Id : %s(%d), Name : %s, Item Code : %d(%s), PeriodType : %d, Serial : %I64d, Duration : %d, ExpireDate : %d",
				lpObj->AccountID, lpObj->DBNumber, lpObj->Name, lpMsg->wItemCode, ItemAttribute[lpMsg->wItemCode].Name, lpMsg->btItemType, lpMsg->Serial, LeftDate, lpMsg->lItemExpireDate);
		}

		else
		{
			g_Log.Add("[PeriodItemEx][SelectItemList] Failed User Id : %s(%d), Name : %s, Item Code : %d(%s), PeriodType : %d, Serial : %I64d, Duration : %d, ExpireDate : %d",
				lpObj->AccountID, lpObj->DBNumber, lpObj->Name, lpMsg->wItemCode, ItemAttribute[lpMsg->wItemCode].Name, lpMsg->btItemType, lpMsg->Serial, LeftDate, lpMsg->lItemExpireDate);
		}
	}

	else if ( lpMsg->btItemType == PERIODITEM_NORMAL )
	{
		if ( this->AddPeriodItemData(lpObj, lpMsg->wItemCode, lpMsg->Serial, LeftDate) != -1 )
		{
			g_Log.Add("[PeriodItemEx][SelectItemList] User Id : %s(%d), Name : %s, Item Code : %d(%s), PeriodType : %d, Serial : %I64d, Duration : %d, ExpireDate : %d",
				lpObj->AccountID, lpObj->DBNumber, lpObj->Name, lpMsg->wItemCode, ItemAttribute[lpMsg->wItemCode].Name, lpMsg->btItemType, lpMsg->Serial, LeftDate, lpMsg->lItemExpireDate);
		}

		else
		{
			g_Log.Add("[PeriodItemEx][SelectItemList] Failed User Id : %s(%d), Name : %s, Item Code : %d(%s), PeriodType : %d, Serial : %I64d, Duration : %d, ExpireDate : %d",
				lpObj->AccountID, lpObj->DBNumber, lpObj->Name, lpMsg->wItemCode, ItemAttribute[lpMsg->wItemCode].Name, lpMsg->btItemType, lpMsg->Serial, LeftDate, lpMsg->lItemExpireDate);
		}
	}
}

int CPeriodItemEx::GetItemFromInventory(LPOBJ lpObj, WORD wItemCode, UINT64 Serial)
{
	int pos = -1;

	if ( lpObj == NULL ) return -1;
	if ( lpObj->Connected < PLAYER_LOGGED ) return -1;
	if ( lpObj->Type != OBJ_USER ) return -1;

	for ( int i=0;i<INVENTORY_SIZE;i++ )
	{
		if ( lpObj->pInventory[i].IsItem() == TRUE &&
			lpObj->pInventory[i].m_Type == wItemCode &&
			lpObj->pInventory[i].GetNumber() == Serial )
		{
			pos = i;
			break;
		}
	}

	return pos;
}

void CPeriodItemEx::OnRequestPeriodItemList(LPOBJ lpObj)
{
	PMSG_PROPERTYITEMCOUNT pMsg1;
	PMSG_PROPERTYITEMINFO pMsg2;

	if ( lpObj->Type != OBJ_USER )
		return;

	int PeriodIndex = lpObj->m_PlayerData->m_iPeriodItemEffectIndex;

	if (PeriodIndex == -1)
		return;

	PHeadSubSetB((LPBYTE)&pMsg1, 0xD2, 0x11, sizeof(pMsg1));
	PHeadSubSetB((LPBYTE)&pMsg2, 0xD2, 0x12, sizeof(pMsg2));
	
	pMsg1.btCount = this->m_PeriodData[PeriodIndex].btItemCount;

	IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg1, sizeof(pMsg1));

	if ( this->m_PeriodData[PeriodIndex].btUsedInfo == TRUE )
	{
		for ( int i=0; i<30; i++ )
		{
			if ( this->m_PeriodData[PeriodIndex].m_ItemInfo[i].btUsedInfo == TRUE )
			{
				pMsg2.ItemID = this->m_PeriodData[PeriodIndex].m_ItemInfo[i].wItemCode;
				pMsg2.Pos = this->GetItemFromInventory(lpObj, this->m_PeriodData[PeriodIndex].m_ItemInfo[i].wItemCode, this->m_PeriodData[PeriodIndex].m_ItemInfo[i].Serial);
				pMsg2.Time = this->m_PeriodData[PeriodIndex].m_ItemInfo[i].lItemExpireDate;

				IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg2, sizeof(pMsg2));
			}
		}
	}
}

void CPeriodItemEx::OnRequestPeriodItemListOnce(LPOBJ lpObj, ITEM_DATA * lpItemData)
{
	if ( lpObj == NULL ) return;
	if ( lpItemData == NULL ) return;

	PMSG_PROPERTYITEMINFO pMsg;

	PHeadSubSetB((LPBYTE)&pMsg, 0xD2, 0x12, sizeof(pMsg));

	pMsg.ItemID = lpItemData->wItemCode;
	pMsg.Pos = this->GetItemFromInventory(lpObj, lpItemData->wItemCode, lpItemData->Serial);
	pMsg.Time = lpItemData->lItemExpireDate;

	IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
}

BOOL CPeriodItemEx::SendPeriodItemInfoOnce(LPOBJ lpObj, CItem * Item)
{
	if ( lpObj->Type != OBJ_USER )
	{
		return FALSE;
	}

	if ( Item == NULL )
	{
		return FALSE;
	}

	int PeriodIndex = lpObj->m_PlayerData->m_iPeriodItemEffectIndex;

	if ( PeriodIndex == -1 )
	{
		return FALSE;
	}

	if ( this->m_PeriodData[PeriodIndex].btUsedInfo == TRUE && this->m_PeriodData[PeriodIndex].dwUserGuid == lpObj->DBNumber )
	{
		for ( int i=0; i<30; i++ )
		{
			if ( this->m_PeriodData[PeriodIndex].m_ItemInfo[i].btUsedInfo == TRUE && this->m_PeriodData[PeriodIndex].m_ItemInfo[i].Serial == Item->GetNumber() )
			{
				this->OnRequestPeriodItemListOnce(lpObj, &this->m_PeriodData[PeriodIndex].m_ItemInfo[i]);
				return TRUE;
			}
		}
	}

	return FALSE;
}

BOOL CPeriodItemEx::SendPeriodItemInfoOnce(LPOBJ lpObj, UINT64 Serial)
{
	if ( lpObj->Type != OBJ_USER )
	{
		return FALSE;
	}

	if ( Serial == 0 )
	{
		return FALSE;
	}

	int PeriodIndex = lpObj->m_PlayerData->m_iPeriodItemEffectIndex;

	if ( PeriodIndex == -1 )
	{
		return FALSE;
	}

	if ( this->m_PeriodData[PeriodIndex].btUsedInfo == TRUE && this->m_PeriodData[PeriodIndex].dwUserGuid == lpObj->DBNumber )
	{
		for ( int i=0; i<30; i++ )
		{
			if ( this->m_PeriodData[PeriodIndex].m_ItemInfo[i].btUsedInfo == TRUE && this->m_PeriodData[PeriodIndex].m_ItemInfo[i].Serial == Serial )
			{
				this->OnRequestPeriodItemListOnce(lpObj, &this->m_PeriodData[PeriodIndex].m_ItemInfo[i]);
				return TRUE;
			}
		}
	}

	return FALSE;
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


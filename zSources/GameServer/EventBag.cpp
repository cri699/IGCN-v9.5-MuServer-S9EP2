//////////////////////////////////////////////////////////////////////
// EventBag.cpp
#include "stdafx.h"
#include "EventBag.h"
#include "TLog.h"
#include "user.h"
#include "LuaBag.h"
#include "MapClass.h"
#include "GameMain.h"

CEventBag::CEventBag()
{
}

CEventBag::~CEventBag()
{
}

void CEventBag::SetBagInfo(int EventID, int iParam2)
{
	this->EventID = EventID;
}

bool CEventBag::CheckCondition(int aIndex, int EventID, int iParam2)
{
	if (rand() % 10000 >= this->m_BagData.dwBagUseRate)
	{
		return false;
	}

	if (this->EventID != EventID)
	{
		return false;
	}

	return true;
}

bool CEventBag::IsBag(int aIndex, int EventID, int iParam2)
{
	if (this->EventID != EventID)
	{
		return false;
	}

	return true;
}

bool CEventBag::UseBag(int aIndex, int iMonsterIndex)
{
	LPOBJ lpObj = &gObj[aIndex];
	LPOBJ lpMonsterObj = &gObj[iMonsterIndex];

	if (rand() % 10000 >= this->m_BagData.dwItemDropRate)
	{
		MapC[gObj[aIndex].MapNumber].MoneyItemDrop(this->m_BagData.dwDropMoney, lpObj->X, lpObj->Y);

		g_Log.Add("[%s] Drop Zen [%s][%s] [%d Zen]", this->m_BagData.szBagName, lpObj->AccountID, lpObj->Name, this->m_BagData.dwDropMoney);
		return true;
	}

	BAG_ITEM m_Item;
	BAG_SECTION_ITEMS m_ItemSection;
	BAG_SECTION_DROP m_DropSection;

	int iResult = this->GetDropSection(aIndex, m_DropSection);

	if (iResult == FALSE)
	{
		return false;
	}

	iResult = this->GetItemsSection(m_DropSection, m_ItemSection);

	if (iResult == FALSE)
	{
		return false;
	}

	if (m_ItemSection.btItemDropCount <= 0)
	{
		g_Log.AddC(TColor::Red, "[ERROR] Bag %s has ItemDropCount <= 0 - check XML file)", this->m_BagData.szBagName);
		return false;
	}

	if (m_ItemSection.btItemDropCount == 1)
	{
		if (rand()%10000 < this->m_BagData.dwRandomSetItemDropRate)
		{
			MakeRewardSetItem(aIndex, lpMonsterObj->X, lpMonsterObj->Y, 1, lpObj->MapNumber);
			g_Log.Add("[%s] Drop SetItem", this->m_BagData.szBagName);
			return true;
		}

		if (this->GetItem(m_ItemSection, m_Item) == FALSE)
		{
			g_Log.AddC(TColor::Red, "[ERROR] Bag %s -> GetItem() == FALSE -> probably no item in %d section", this->m_BagData.szBagName, m_ItemSection.btSectionID);
			return false;
		}

		bool bResult = gLuaBag.DropCommonBag(aIndex, lpMonsterObj->MapNumber, lpMonsterObj->X, lpMonsterObj->Y, &m_Item);

		if (bResult == false)
		{
			g_Log.Add("[%s] Drop Fail [%s][%s]", this->m_BagData.szBagName, lpObj->AccountID, lpObj->Name);
			return false;
		}

		g_Log.Add("[%s] Drop Item [%s][%s] : (%d)(%d/%d) Item:(%s)%d", this->m_BagData.szBagName,
			lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y, ItemAttribute[ITEMGET(m_Item.btItemType, m_Item.wItemIndex)].Name, ITEMGET(m_Item.btItemType, m_Item.wItemIndex));

		return true;
	}

	for (int i = 0; i < m_ItemSection.btItemDropCount; i++)
	{		
		BYTE cDropX = lpMonsterObj->X;
		BYTE cDropY = lpMonsterObj->Y;

		if (!gObjGetRandomItemDropLocation(lpMonsterObj->MapNumber, cDropX, cDropY, 4, 4, 10))
		{
			cDropX = lpMonsterObj->X;
			cDropY = lpMonsterObj->Y;
		}
	
		if (rand()%10000 < this->m_BagData.dwRandomSetItemDropRate)
		{
			MakeRewardSetItem(aIndex, cDropX, cDropY, 1, lpObj->MapNumber);
			g_Log.Add("[%s] Drop SetItem", this->m_BagData.szBagName);
			continue;
		}

		if (this->GetItem(m_ItemSection, m_Item) == FALSE)
		{
			g_Log.AddC(TColor::Red, "[ERROR] Bag %s -> GetItem() == FALSE -> probably no item in %d section", this->m_BagData.szBagName, m_ItemSection.btSectionID);
			return false;
		}

		bool bResult = gLuaBag.DropMonsterBag(aIndex, iMonsterIndex, lpMonsterObj->MapNumber, cDropX, cDropY, &m_Item);

		if (bResult == false)
		{
			g_Log.Add("[%s] Drop Fail [%s][%s]", this->m_BagData.szBagName, lpObj->AccountID, lpObj->Name);
			return false;
		}

		g_Log.Add("[%s] Drop Item [%s][%s] : (%d)(%d/%d) Item:(%s)%d", this->m_BagData.szBagName,
			lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y, ItemAttribute[ITEMGET(m_Item.btItemType, m_Item.wItemIndex)].Name, ITEMGET(m_Item.btItemType, m_Item.wItemIndex));
	}
	return true;
}

bool CEventBag::UseBag_GremoryCase(int aIndex, int iMonsterIndex, BYTE btStorageType, BYTE btRewardSource, int iExpireDays)
{
	LPOBJ lpObj = &gObj[aIndex];
	LPOBJ lpMonsterObj = &gObj[iMonsterIndex];

	if (rand() % 10000 >= this->m_BagData.dwItemDropRate)
	{
		MapC[gObj[aIndex].MapNumber].MoneyItemDrop(this->m_BagData.dwDropMoney, lpObj->X, lpObj->Y);

		g_Log.Add("[%s] Drop Zen [%s][%s] [%d Zen]", this->m_BagData.szBagName, lpObj->AccountID, lpObj->Name, this->m_BagData.dwDropMoney);
		return true;
	}

	BAG_ITEM m_Item;
	BAG_SECTION_ITEMS m_ItemSection;
	BAG_SECTION_DROP m_DropSection;

	int iResult = this->GetDropSection(aIndex, m_DropSection);

	if (iResult == FALSE)
	{
		return false;
	}

	iResult = this->GetItemsSection(m_DropSection, m_ItemSection);

	if (iResult == FALSE)
	{
		return false;
	}

	if (m_ItemSection.btItemDropCount <= 0)
	{
		g_Log.AddC(TColor::Red, "[ERROR] Bag %s has ItemDropCount <= 0 - check XML file)", this->m_BagData.szBagName);
		return false;
	}

	if (m_ItemSection.btItemDropCount == 1)
	{
		if (rand()%10000 < this->m_BagData.dwRandomSetItemDropRate)
		{
			_stGremoryCaseItem stItem;
			MakeRewardSetItem(aIndex, lpMonsterObj->X, lpMonsterObj->Y, 1, lpObj->MapNumber, stItem);

			stItem.btStorageType = btStorageType;
			stItem.btRewardSource = btRewardSource;
			g_GremoryCase.GDReqAddItemToGremoryCase(aIndex, stItem, iExpireDays);

			g_Log.Add("[%s] Send SetItem to Gremory Case", this->m_BagData.szBagName);
			return true;
		}

		if (this->GetItem(m_ItemSection, m_Item) == FALSE)
		{
			g_Log.AddC(TColor::Red, "[ERROR] Bag %s -> GetItem() == FALSE -> probably no item in %d section", this->m_BagData.szBagName, m_ItemSection.btSectionID);
			return false;
		}

		_stGremoryCaseItem stItem;

		bool bResult = gLuaBag.MakeItemFromBagForGremoryCase(aIndex, &m_Item, stItem);

		if (bResult == false)
		{
			g_Log.Add("[%s] Send Item Fail [%s][%s]", this->m_BagData.szBagName, lpObj->AccountID, lpObj->Name);
			return false;
		}

		stItem.btStorageType = btStorageType;
		stItem.btRewardSource = btRewardSource;
		g_GremoryCase.GDReqAddItemToGremoryCase(aIndex, stItem, iExpireDays);

		g_Log.Add("[%s] Send Item to Gremory Case [%s][%s] : (%d)(%d/%d) Item:(%s)%d", this->m_BagData.szBagName,
			lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y, ItemAttribute[ITEMGET(m_Item.btItemType, m_Item.wItemIndex)].Name, ITEMGET(m_Item.btItemType, m_Item.wItemIndex));

		return true;
	}

	for (int i = 0; i < m_ItemSection.btItemDropCount; i++)
	{		
		BYTE cDropX = lpMonsterObj->X;
		BYTE cDropY = lpMonsterObj->Y;

		if (!gObjGetRandomItemDropLocation(lpMonsterObj->MapNumber, cDropX, cDropY, 4, 4, 10))
		{
			cDropX = lpMonsterObj->X;
			cDropY = lpMonsterObj->Y;
		}
	
		if (rand()%10000 < this->m_BagData.dwRandomSetItemDropRate)
		{
			_stGremoryCaseItem stItem;
			MakeRewardSetItem(aIndex, cDropX, cDropY, 1, lpObj->MapNumber, stItem);

			stItem.btStorageType = btStorageType;
			stItem.btRewardSource = btRewardSource;
			g_GremoryCase.GDReqAddItemToGremoryCase(aIndex, stItem, iExpireDays);

			g_Log.Add("[%s] Send SetItem to Gremory Case", this->m_BagData.szBagName);
			continue;
		}

		if (this->GetItem(m_ItemSection, m_Item) == FALSE)
		{
			g_Log.AddC(TColor::Red, "[ERROR] Bag %s -> GetItem() == FALSE -> probably no item in %d section", this->m_BagData.szBagName, m_ItemSection.btSectionID);
			return false;
		}

		_stGremoryCaseItem stItem;

		bool bResult = gLuaBag.MakeItemFromBagForGremoryCase(aIndex, &m_Item, stItem);

		if (bResult == false)
		{
			g_Log.Add("[%s] Send Item Fail [%s][%s]", this->m_BagData.szBagName, lpObj->AccountID, lpObj->Name);
			return false;
		}

		stItem.btStorageType = btStorageType;
		stItem.btRewardSource = btRewardSource;
		g_GremoryCase.GDReqAddItemToGremoryCase(aIndex, stItem, iExpireDays);

		g_Log.Add("[%s] Send Item to Gremory Case [%s][%s] : (%d)(%d/%d) Item:(%s)%d", this->m_BagData.szBagName,
			lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y, ItemAttribute[ITEMGET(m_Item.btItemType, m_Item.wItemIndex)].Name, ITEMGET(m_Item.btItemType, m_Item.wItemIndex));
	}

	return true;
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


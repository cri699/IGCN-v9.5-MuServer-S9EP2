//////////////////////////////////////////////////////////////////////
// Bag.h
#ifndef BAG_H
#define BAG_H

#pragma once

#include "zzzitem.h"

struct BAG_ITEM
{
	BYTE btItemType;
	WORD wItemIndex;
	BYTE btItemMinLevel;
	BYTE btItemMaxLevel;
	BYTE btItemDur;
	char btIsSkill;
	char btIsLuck;
	char btIsOption;
	char btIsExItem;
	char btIsSetItem;
	char btIsSocketItem;
	char btIsElementalItem;
	BYTE btMuunEvolutionItemType;
	WORD wMuunEvolutionItemIndex;
	DWORD dwDuration;
};

struct BAG_SECTION_ITEMS
{
	BYTE btSectionID;	
	BYTE btItemDropCount;
	DWORD dwSectionUseRate;
	std::vector<BAG_ITEM> m_vtItems;
};

struct BAG_SECTION_DROP
{
	WORD wUseSectionMinLevel;
	WORD wUseSectionMaxLevel;
	BYTE btUseSectionAllowedClass[7];
	BYTE btUseSectionAllowedMapNum;
	std::map<int, BAG_SECTION_ITEMS> m_mapSectionItems;
};

struct BAG_DATA
{
	bool m_bLoadBag;
	char szBagName[255];
	DWORD dwItemDropRate;
	DWORD dwRandomSetItemDropRate;
	DWORD dwDropMoney;
	int iBagUseEffect;
	DWORD dwBagUseRate;
	bool bAddCoinEnable;
	BYTE btAddCoinType;
	DWORD dwAddCoinValue;
	bool bSummonBookEnable;
	DWORD dwSummonBookDropRate;
	BYTE btSummonBookItemType;
	WORD wSummonBookItemIndex;
	std::vector<BAG_SECTION_DROP> m_vtSectionDrop;
};

extern const char * szFileClassName[];

class CBag
{
public:
	CBag();
	virtual ~CBag();

	void InitBag();

	bool LoadBag(const char* lpFileName);
	virtual bool IsBag(int aIndex, int iParam1, int iParam2) = 0;
	virtual bool UseBag(int aIndex, int iParam) = 0;
	virtual bool UseBag_GremoryCase(int aIndex, int iParam, BYTE btStorageType, BYTE btRewardSource, int iExpireDays) = 0;
	virtual bool CheckCondition(int aIndex, int iParam1, int iParam2) = 0;
	virtual void SetBagInfo(int iParam1, int iParam2) = 0;

	int GetDropSection(int aIndex, BAG_SECTION_DROP &pRetDrop);
	int GetItemsSection(BAG_SECTION_DROP & pDropSec, BAG_SECTION_ITEMS &pRetItems);
	int GetItem(BAG_SECTION_ITEMS &pItemsSec, BAG_ITEM & pRetItem);
	int GetReadyItemToUse(int aIndex, CItem & pItem, time_t & DurationItem);
	static int GetItemLevel(int MinLevel, int MaxLevel);
	void MakeBagEffectUse(int aIndex, int MonsterIndex);
	void AddCoin(int aIndex);
	void DropSummonItem(int aIndex);

	int IsBagUseEffect() { return this->m_BagData.iBagUseEffect; }

protected:
	BAG_DATA m_BagData;
};

#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// BagManager.h
#ifndef BAGMANAGER_H
#define BAGMANAGER_H

#include "LuaBag.h"
#include "Bag.h"
#include "CommonBag.h"
#include "MonsterBag.h"
#include "EventBag.h"

enum BAG_TYPES
{
	BAG_COMMON = 0,
	BAG_MONSTER = 1,
	BAG_EVENT = 2
};

enum EVENTBAG_TYPE
{
	EVENTBAG_ARCA = 0,
	EVENTBAG_NEWPVP = 1,
	EVENTBAG_BC1 = 6,
	EVENTBAG_BC2 = 7,
	EVENTBAG_BC3 = 8,
	EVENTBAG_BC4 = 9,
	EVENTBAG_BC5 = 10,
	EVENTBAG_BC6 = 11,
	EVENTBAG_BC7 = 12,
	EVENTBAG_BC8 = 13,
	EVENTBAG_CC1 = 14,
	EVENTBAG_LMS = 15,
	EVENTBAG_SANTAFIRST = 16,
	EVENTBAG_SANTASECOND = 17,
	EVENTBAG_SANTATHIRD = 18,
	EVENTBAG_WARRIORRING_1 = 19,
	EVENTBAG_WARRIORRING_2 = 20,
	EVENTBAG_CHERRYBLOSSOM_GOLD = 21,
	EVENTBAG_LUCKYCOIN10 = 22,
	EVENTBAG_LUCKYCOIN20 = 23,
	EVENTBAG_LUCKYCOIN30 = 24,
	EVENTBAG_LORDMIX = 25,
	EVENTBAG_KUNDUN = 26,
	EVENTBAG_ACHERONGUARDIAN = 27,
	EVENTBAG_MURUMMY_1 = 28,
	EVENTBAG_MURUMMY_2 = 29,
	EVENTBAG_EGG_PETITEM = 30,
	EVENTBAG_EGG_LUCKYBAG = 31,
	EVENTBAG_EGG_FIREGHOST = 32,
	EVENTBAG_EGG_GOLDGOBLIN = 33,
	EVENTBAG_ITR_1 = 34,
	EVENTBAG_ITR_2 = 35,
	EVENTBAG_ITR_3 = 36,
	EVENTBAG_ITR_4 = 37,
	EVENTBAG_ITR_5 = 38,
	EVENTBAG_ITR_6 = 39,
	EVENTBAG_GOLDCOLOSSUS_GOBLIN = 40,
	EVENTBAG_GOLDCOLOSSUS_TITAN = 41,
	EVENTBAG_GOLDCOLOSSUS_TANTALUS = 42,
	EVENTBAG_GOLDCOLOSSUS_EROHIM = 43,
	EVENTBAG_GOLDCOLOSSUS_HELLMAINE = 44,
	EVENTBAG_GOLDCOLOSSUS_KUNDUN = 45,
	EVENTBAG_LORDSILVESTER = 46,
	EVENTBAG_CHERRYBLOSSOM_WHITE = 47,
	EVENTBAG_CHERRYBLOSSOM_RED = 48,
	EVENTBAG_CC2 = 49,
	EVENTBAG_CC3 = 50,
	EVENTBAG_CC4 = 51,
	EVENTBAG_CC5 = 52,
	EVENTBAG_CC6 = 53,
	EVENTBAG_CC7 = 54
};

#pragma once
class CBagManager
{
public:
	CBagManager();
	virtual ~CBagManager();

	void InitBagManager();

	bool AddItemBag(int BagType, int iParam1, int iParam2, const char* lpFileName);
	void DeleteItemBags();

	bool IsBag(int aIndex, int BagType, int iParam1, int iParam2);
	bool SearchAndUseBag(int aIndex, int BagType, int iParam1, int iParam2);
	bool GetItemFromBag(int aIndex, int BagType, int iParam1, int iParam2, CItem & Item, time_t & DurationItem);
	bool UseBag_GremoryCase(int aIndex, int BagType, int iParam1, int iParam2, BYTE btStorageType, BYTE btRewardSource, int iExpireDays);

	void AddCashCoin(int aIndex, int BagType, int iParam1, int iParam2);

	std::multimap<int, CBag*>	m_multimapBags;
	CRITICAL_SECTION m_criti;
};

extern CBagManager g_BagManager;

#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


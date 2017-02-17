//////////////////////////////////////////////////////////////////////
// CItemDrop.h
#ifndef ITEMDROP_H
#define ITEMDROP_H

#include "user.h"

struct ITEMDROP_ITEM
{
	BYTE btItemType;
	WORD wItemIndex;
	BYTE btItemMinLevel;
	BYTE btItemMaxLevel;
	BYTE btItemDurability;
	BYTE btIsSkill;
	BYTE btIsLuck;
	BYTE btIsOption;
	BYTE btIsExc;
	BYTE btIsSetItem;
	BYTE btSocketCount;
	BYTE btIsElementalItem;
	BYTE btMuunEvoItemType;
	WORD wMuunEvoItemIndex;
	DWORD dwItemDropRate;
	DWORD dwPeriodDuration;
};

struct ITEMDROP_MONSTER
{
	ITEMDROP_MONSTER()
	{
		this->m_vtItems.clear();
	}

	WORD wMonsterID;
	WORD wMonsterMinLevel;
	WORD wMonsterMaxLevel;
	BYTE btMonsterMapNumber;
	DWORD dwItemDropRate;
	BYTE btItemCount;
	bool bIsCoinReward;
	BYTE btCoinType;
	DWORD dwCoinValue;
	WORD wPlayerMinLevel;
	WORD wPlayerMaxLevel;
	std::vector<boost::shared_ptr<ITEMDROP_ITEM>> m_vtItems;
};

struct ZEN_DROP
{
	BYTE btMapNumber;
	DWORD dwMinMoney;
	DWORD dwMaxMoney;
};

class CItemDrop
{
public:
	CItemDrop(void);
	virtual ~CItemDrop(void);

	bool LoadFile(const char* szFile);
	bool LoadZenDropFile(const char* szFile);
	bool DropItem(OBJECTSTRUCT* lpUser, OBJECTSTRUCT* lpMonster);
	bool IsZenDropActive(BYTE btMapNumber);
	DWORD GetZenAmount(BYTE btMapNumber, WORD wMonsterLevel);

private:

	inline int GetItemLevel(int min, int max)
	{
		int diff;
		__try
		{
			diff = max-min;
			int level = rand()%(diff+1);
	
			return level += min;
		}

		__except(diff=1, 1)
		{
			return min;
		}
	}

	ITEMDROP_ITEM ** GetItem(OBJECTSTRUCT *lpUser, OBJECTSTRUCT* lpMonster, int & ItemCount);

	boost::shared_ptr<ITEMDROP_MONSTER> GetMonsterData(OBJECTSTRUCT *lpUser, OBJECTSTRUCT* lpMonster);
	boost::shared_ptr<ITEMDROP_ITEM> GetItemFromMonster(boost::shared_ptr<ITEMDROP_MONSTER> itemdrop_monster_ptr);

	void AddCoinReward(boost::shared_ptr<ITEMDROP_MONSTER> itemdrop_monster_ptr, OBJECTSTRUCT *lpUser);

	std::vector<boost::shared_ptr<ITEMDROP_MONSTER>> m_vtMonsterList;
	std::map<int, ZEN_DROP> m_mapZenDrop;
	DWORD m_dwDropUseRate;

	bool m_bZenDropEnable;
	bool m_bMultiplyZenDropByMonLevel;
	DWORD m_dwMultiplyZenChanceRate;
};

extern CItemDrop ItemDrop;
#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


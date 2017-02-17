//////////////////////////////////////////////////////////////////////
// MonsterItemMng.h
#ifndef MONSTERITEMMNG_H
#define MONSTERITEMMNG_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "zzzitem.h"

#define MAX_ITEM_IN_MONSTER 1000
#define MAX_LEVEL_MONSTER 255
#define MAX_MAGIC_IN_MONSTER 100

struct __tagMONSTER_ITEM_DROP_PRATE_
{
	int m_Level;
	int m_MagicBook;
	int m_BlessRate;
	int m_SoulRate;
	int m_ChaosItemRate;
	int m_LifeRate;
	int m_CreationRate;
	int m_NormalItemRate;
	int m_TotalRate;
};

class CMonsterItemMng
{
public:
	CMonsterItemMng();
	virtual ~CMonsterItemMng();

	void LoadMonsterItemDropRate();

	void Init();
	void Clear();

	BYTE InsertItem(int monsterlevel, int type, int index, int itemlevel, int op1, int op2, int op3);
	CItem * GetItem(int monsterlevel);

	void gObjGiveItemSearch(int monsterlevel, int maxlevel);
	void MakeJewelItem();
	void gObjGiveItemSearchEx(int monsterlevel, int maxlevel);
	void MagicBookGiveItemSearch(int monsterlevel, int maxlevel);
	void NormalGiveItemSearch(int monsterlevel,  int maxlevel);
	int CheckMonsterDropItem(int type, int index);
	CItem * GetItemEx(int monsterlevel);
	void NormalGiveItemSearchEx(int monsterlevel, int maxlevel);
	__tagMONSTER_ITEM_DROP_PRATE_ m_MonsterItemDropRate[MAX_LEVEL_MONSTER];
	CItem *m_MonsterInvenItems[MAX_LEVEL_MONSTER];
	int m_iMonsterInvenItemCount[MAX_LEVEL_MONSTER];
	char MonsterName[255];
	int m_bLoad;
	int m_iMagicBookItmeCount[MAX_LEVEL_MONSTER];
	CItem *m_MagicBookItems[MAX_LEVEL_MONSTER];
	CItem *m_JewelOfBlessItem;
	CItem *m_JewelOfSoulItem;
	CItem *m_JewelOfChaosItem;
	CItem *m_JewelOfLifeItem;
	CItem *m_JewelOfCreationItem;
}; extern CMonsterItemMng g_MonsterItemMng;

#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


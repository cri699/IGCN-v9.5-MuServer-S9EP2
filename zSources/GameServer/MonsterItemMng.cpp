//////////////////////////////////////////////////////////////////////
// MonsterItemMng.cpp
#include "stdafx.h"
#include "MonsterItemMng.h"
#include "user.h"
#include "MonsterAttr.h"
#include "GameMain.h"
#include "TLog.h"
#include "LargeRand.h"
#include "CashShop.h"
#include "LuckyItemManager.h"
#include "configread.h"

CMonsterItemMng g_MonsterItemMng;

CMonsterItemMng::CMonsterItemMng()
{

}

CMonsterItemMng::~CMonsterItemMng()
{
	this->Clear();
}

void CMonsterItemMng::Init()
{
	this->Clear();

	for (int j = 0; j < MAX_LEVEL_MONSTER; j++)
	{
		this->m_MonsterInvenItems[j] = new CItem[MAX_ITEM_IN_MONSTER];
		this->m_MagicBookItems[j] = new CItem[MAX_MAGIC_IN_MONSTER];
	}

	for (int i = 0; i < MAX_MONSTER_TYPE; i++)
	{
		LPMONSTER_ATTRIBUTE lpm = gMAttr.GetAttr(i);

		if (!lpm)
		{
			continue;
		}

		if (lpm->m_Level == 0)
		{
			continue;
		}

		this->gObjGiveItemSearchEx(lpm->m_Level, lpm->m_MaxItemLevel);
	}

	this->LoadMonsterItemDropRate();
	this->MakeJewelItem();
}

void CMonsterItemMng::Clear()
{
	for (int i = 0; i < MAX_LEVEL_MONSTER; i++)
	{
		delete this->m_MonsterInvenItems[i];
		delete this->m_MagicBookItems[i];
	}

	memset(this->m_iMagicBookItmeCount, 0, sizeof(this->m_iMagicBookItmeCount));
	memset(this->m_iMonsterInvenItemCount, 0, sizeof(this->m_iMonsterInvenItemCount));

	if (this->m_JewelOfBlessItem)
	{
		delete this->m_JewelOfBlessItem;
		this->m_JewelOfBlessItem = nullptr;
	}
	if (this->m_JewelOfSoulItem)
	{
		delete this->m_JewelOfSoulItem;
		this->m_JewelOfSoulItem = nullptr;
	}
	if (this->m_JewelOfChaosItem)
	{
		delete this->m_JewelOfChaosItem;
		this->m_JewelOfChaosItem = nullptr;
	}
	if (this->m_JewelOfLifeItem)
	{
		delete this->m_JewelOfLifeItem;
		this->m_JewelOfLifeItem = nullptr;
	}
	if (this->m_JewelOfCreationItem)
	{
		delete this->m_JewelOfCreationItem;
		this->m_JewelOfCreationItem = nullptr;
	}
}

BYTE CMonsterItemMng::InsertItem(int monsterlevel, int type, int index, int itemlevel, int op1, int op2, int op3)
{
	if (monsterlevel < 0 || monsterlevel >= MAX_LEVEL_MONSTER)
	{
		return -1;
	}

	int itemcount = this->m_iMonsterInvenItemCount[monsterlevel];

	if (itemcount >= MAX_ITEM_IN_MONSTER)
	{
		return -1;
	}

	CItem *item = &this->m_MonsterInvenItems[monsterlevel][itemcount];

	item->m_Level = itemlevel;
	item->Convert(ITEMGET(type, index), op1, op2, op3, 0, 0, 0, 0, -1, 0, 3);

	if (type != 13 || index != 10)
	{
		item->m_Durability = item->m_BaseDurability;
	}
	
	else
	{
		item->m_Durability = (rand() % 100) + 100.0;
	}

	this->m_iMonsterInvenItemCount[monsterlevel]++;
	return 0;
}

CItem * CMonsterItemMng::GetItem(int monsterlevel)
{
	if (monsterlevel < 0 || monsterlevel >= MAX_LEVEL_MONSTER)
	{
		return nullptr;
	}

	int itemcount = this->m_iMonsterInvenItemCount[monsterlevel];

	if (itemcount <= 0)
	{
		return nullptr;
	}

	return &this->m_MonsterInvenItems[monsterlevel][rand() % itemcount];
}

void CMonsterItemMng::gObjGiveItemSearch(int monsterlevel, int maxlevel)
{
	// unused
}

void CMonsterItemMng::LoadMonsterItemDropRate()
{
	this->m_bLoad = FALSE;
	
	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(g_ConfigRead.GetPath("IGC_MonsterItemDropRate.xml"));

	if (res.status != pugi::status_ok)
	{
		g_Log.MsgBox("File Read Error: IGC_MonsterItemDropRate.xml (%s)", res.description());
		return;
	}

	int nLevel = 0;
	pugi::xml_node droprate = file.child("DropRate");

	for (pugi::xml_node monster = droprate.child("Monster"); monster; monster = monster.next_sibling())
	{
		int nLevel = monster.attribute("Level").as_int();

		if (nLevel >= MAX_LEVEL_MONSTER)
		{
			g_Log.MsgBox("Error - Wrong Monster Level (%d) (Maximum is %d) (%s)", nLevel, MAX_LEVEL_MONSTER-1, __FUNCTION__);
			continue;
		}

		this->m_MonsterItemDropRate[nLevel].m_Level = nLevel;
		this->m_MonsterItemDropRate[nLevel].m_MagicBook = monster.attribute("MagicBook").as_float() * 10000000.0;
		this->m_MonsterItemDropRate[nLevel].m_BlessRate = monster.attribute("JewelOfBless").as_float() * 10000000.0;
		this->m_MonsterItemDropRate[nLevel].m_SoulRate = monster.attribute("JewelOfSoul").as_float() * 10000000.0;
		this->m_MonsterItemDropRate[nLevel].m_ChaosItemRate = monster.attribute("JewelOfChaos").as_float() * 10000000.0;
		this->m_MonsterItemDropRate[nLevel].m_LifeRate = monster.attribute("JewelOfLife").as_float() * 10000000.0;
		this->m_MonsterItemDropRate[nLevel].m_CreationRate = monster.attribute("JewelOfCreation").as_float() * 10000000.0;
		this->m_MonsterItemDropRate[nLevel].m_NormalItemRate = monster.attribute("Items").as_float() * 10000000.0;

		this->m_MonsterItemDropRate[nLevel].m_TotalRate = this->m_MonsterItemDropRate[nLevel].m_NormalItemRate
			+ this->m_MonsterItemDropRate[nLevel].m_LifeRate
			+ this->m_MonsterItemDropRate[nLevel].m_CreationRate
			+ this->m_MonsterItemDropRate[nLevel].m_ChaosItemRate
			+ this->m_MonsterItemDropRate[nLevel].m_SoulRate
			+ this->m_MonsterItemDropRate[nLevel].m_BlessRate
			+ this->m_MonsterItemDropRate[nLevel].m_MagicBook;
	}
		
	this->m_bLoad = TRUE;
}

void CMonsterItemMng::MakeJewelItem()
{
	this->m_JewelOfBlessItem = new CItem;
	this->m_JewelOfSoulItem = new CItem;
	this->m_JewelOfChaosItem = new CItem;
	this->m_JewelOfLifeItem = new CItem;
	this->m_JewelOfCreationItem = new CItem;

	this->m_JewelOfBlessItem->Convert(ITEMGET(14, 13), 0, 0, 0, 0, 0, 0, 0, -1, 0, 3);
	this->m_JewelOfBlessItem->m_Level = 0;
	this->m_JewelOfBlessItem->m_Durability = this->m_JewelOfBlessItem->m_BaseDurability;

	this->m_JewelOfSoulItem->Convert(ITEMGET(14, 14), 0, 0, 0, 0, 0, 0, 0, -1, 0, 3);
	this->m_JewelOfSoulItem->m_Level = 0;
	this->m_JewelOfSoulItem->m_Durability = this->m_JewelOfSoulItem->m_BaseDurability;

	this->m_JewelOfChaosItem->Convert(ITEMGET(12, 15), 0, 0, 0, 0, 0, 0, 0, -1, 0, 3);
	this->m_JewelOfChaosItem->m_Level = 0;
	this->m_JewelOfChaosItem->m_Durability = this->m_JewelOfChaosItem->m_BaseDurability;

	this->m_JewelOfLifeItem->Convert(ITEMGET(14, 16), 0, 0, 0, 0, 0, 0, 0, -1, 0, 3);
	this->m_JewelOfLifeItem->m_Level = 0;
	this->m_JewelOfLifeItem->m_Durability = this->m_JewelOfLifeItem->m_BaseDurability;

	this->m_JewelOfCreationItem->Convert(ITEMGET(14, 22), 0, 0, 0, 0, 0, 0, 0, -1, 0, 3);
	this->m_JewelOfCreationItem->m_Level = 0;
	this->m_JewelOfCreationItem->m_Durability = this->m_JewelOfCreationItem->m_BaseDurability;
}

void CMonsterItemMng::gObjGiveItemSearchEx(int monsterlevel, int maxlevel)
{
	if (this->m_iMonsterInvenItemCount[monsterlevel] == MAX_ITEM_IN_MONSTER)
	{
		return;
	}

	this->NormalGiveItemSearchEx(monsterlevel, maxlevel);
	this->MagicBookGiveItemSearch(monsterlevel, maxlevel);
}

void CMonsterItemMng::MagicBookGiveItemSearch(int monsterlevel, int maxlevel)
{
	if (monsterlevel >= MAX_LEVEL_MONSTER)
	{
		g_Log.Add("[INFO] Monster Level greater than max drop Level (%d) excluded from general drop, line: %d", MAX_LEVEL_MONSTER - 1, __LINE__);
		return;
	}

	int BallTable[22]; // Orbs

	BallTable[0] = 7;
	BallTable[1] = 8;
	BallTable[2] = 9;
	BallTable[3] = 10;
	BallTable[4] = 11;
	BallTable[5] = 12;
	BallTable[6] = 13;
	BallTable[7] = 14;
	BallTable[8] = 16;
	BallTable[9] = 17;
	BallTable[10] = 18;
	BallTable[11] = 19;
	BallTable[12] = 21;
	BallTable[13] = 22;
	BallTable[14] = 23;
	BallTable[15] = 24;
	BallTable[16] = 35;
	BallTable[17] = 44;
	BallTable[18] = 45;
	BallTable[19] = 46;
	BallTable[20] = 47;
	BallTable[21] = 48;

	int LoopCount = 200;
	int incount = 0;
	int type, index;

	while (LoopCount != 0)
	{
		while (true)
		{
			if (rand() % 2 == 0)
			{
				type = 12;
				index = BallTable[GetLargeRand() % 0x16];
			}

			else
			{
				type = MAX_TYPE_ITEMS - 1;
				index = GetLargeRand() % (g_MaxItemIndexOfEachItemType[type] + 1);
			}

			if (type != 12 || index != 11)
				break;

			int result = GetLevelItem(12, 11, monsterlevel);

			if (result >= 0)
			{
				this->m_MagicBookItems[monsterlevel][incount].Convert(ITEMGET(12, 11), 0, 0, 0, 0, 0, 0, 0, -1, 0, 3);
				this->m_MagicBookItems[monsterlevel][incount].m_Level = result;
				this->m_MagicBookItems[monsterlevel][incount].m_Durability = this->m_MagicBookItems[monsterlevel][incount].m_BaseDurability;

				incount++;

				if (incount >= MAX_MAGIC_IN_MONSTER)
				{
					goto END_LOOP;
				}
			}
		}

		if (GetLevelItem(type, index, monsterlevel) == 0)
		{
			this->m_MagicBookItems[monsterlevel][incount].Convert(ITEMGET(type, index), 0, 0, 0, 0, 0, 0, 0, -1, 0, 3);
			this->m_MagicBookItems[monsterlevel][incount].m_Level = 0;
			this->m_MagicBookItems[monsterlevel][incount].m_Durability = this->m_MagicBookItems[monsterlevel][incount].m_BaseDurability;

			incount++;

			if (incount >= MAX_MAGIC_IN_MONSTER)
			{
				goto END_LOOP;
			}
		}

		LoopCount--;
	}

END_LOOP:
	this->m_iMagicBookItmeCount[monsterlevel] = incount;
}

void CMonsterItemMng::NormalGiveItemSearchEx(int monsterlevel, int maxlevel)
{
	BYTE DropTypes[14] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 13, 14 };
	int incount = 0;
	int result;
	int perc;
	int devilitemlevel;
	signed int bCheckDevil;

	if (monsterlevel >= MAX_LEVEL_MONSTER)
	{
		g_Log.Add("[INFO] Monster (%s) Level greater than max drop Level (%d) excluded from general drop, line: %d", this->MonsterName, MAX_LEVEL_MONSTER-1, __LINE__);
		return;
	}

	for (int type = 0; type < 14; type++)
	{
		for (int index = 0; index < g_MaxItemIndexOfEachItemType[DropTypes[type]]; index++)
		{
			if (!this->CheckMonsterDropItem(type, index))
				continue;
			if ((type != 13 || index >= 8)
				&& (type != 14 || index != 9 && index != 10 && index != 17 && index != 18)
				&& (type != 13 || index != 14)
				&& (type != 14 || index != 31))
			{
				result = GetLevelItem(type, index, monsterlevel);
				if (result >= 0 && result <= maxlevel)
				{
					if (type == 12 && index != 11)
						result = 0;
					if ((type != 12 || index != 11) && result > maxlevel)
						result = maxlevel;
					if (type == 4 && index == 7 || type == 4 && index == 15)
						result = 0;
					if (this->InsertItem(monsterlevel, type, index, result, 0, 0, 0) != 255)
					{
						++incount;

						if (incount > MAX_ITEM_IN_MONSTER-1)
							return;
					}
				}
			}
			else
			{
				perc = rand() % 8;
				bCheckDevil = 0;
				if (type == 12 && index == 15)
				{
					if (monsterlevel < 13 || monsterlevel > 66)
					{
						perc = 1;
					}
					else
					{
						perc = rand() % 7;
						if (perc < 3)
							perc = 0;
					}
				}

				if (!perc)
				{
					if (bCheckDevil == 1)
					{
						if (rand() % 5)
						{
							if (monsterlevel >= 3)
							{
								if (monsterlevel >= 36)
								{
									if (monsterlevel >= 47)
									{
										if (monsterlevel >= 60)
										{
											if (monsterlevel >= 70)
												devilitemlevel = monsterlevel >= 80 ? 6 : 5;
											else
												devilitemlevel = 4;
										}
										else
										{
											devilitemlevel = 3;
										}
									}
									else
									{
										devilitemlevel = 2;
									}
								}
								else
								{
									devilitemlevel = 1;
								}
							}
							else
							{
								devilitemlevel = 0;
							}
							if (devilitemlevel)
							{
								if (this->InsertItem(monsterlevel, type, index, devilitemlevel, 0, 0, 0) != 255)
								{
									++incount;

									if (incount > MAX_ITEM_IN_MONSTER-1)
										return;
								}
							}
						}
					}
					else
					{
						if (zzzItemLevel(type, index, monsterlevel) == 1)
						{
							if (this->InsertItem(monsterlevel, type, index, 0, 0, 0, 0) != 255)
							{
								++incount;
								if (incount > MAX_ITEM_IN_MONSTER-1)
									return;
							}
						}
					}
				}
			}
			if (this->m_iMonsterInvenItemCount[monsterlevel] >= MAX_ITEM_IN_MONSTER)
				return;

		}
	}

}
void CMonsterItemMng::NormalGiveItemSearch(int monsterlevel, int maxlevel)
{
	int devilitemlevel; 
	int perc; 
	signed int bCheckDevil; 
	int result; 
	int nDropType[14];
	int index; 
	int type; 
	int incount; 

	incount = 0;
	if (monsterlevel <= MAX_LEVEL_MONSTER-1)
	{
		nDropType[0] = 0;
		nDropType[1] = 1;
		nDropType[2] = 2;
		nDropType[3] = 3;
		nDropType[4] = 4;
		nDropType[5] = 5;
		nDropType[6] = 6;
		nDropType[7] = 7;
		nDropType[8] = 8;
		nDropType[9] = 9;
		nDropType[10] = 10;
		nDropType[11] = 11;
		nDropType[12] = 13;
		nDropType[13] = 14;
		while (1)
		{
			do
			{
				type = nDropType[rand() % 14];
				index = rand() % (g_MaxItemIndexOfEachItemType[type] + 1);
			} while (!this->CheckMonsterDropItem(type, index));
			if ((type != 13 || index >= 8)
				&& (type != 14 || index != 9 && index != 10 && index != 17 && index != 18)
				&& (type != 13 || index != 14)
				&& (type != 14 || index != 31))
			{
				result = GetLevelItem(type, index, monsterlevel);
				if (result >= 0 && result <= maxlevel)
				{
					if (type == 12 && index != 11)
						result = 0;
					if ((type != 12 || index != 11) && result > maxlevel)
						result = maxlevel;
					if (type == 4 && index == 7 || type == 4 && index == 15)
						result = 0;
					if (this->InsertItem(monsterlevel, type, index, result, 0, 0, 0) != 255)
					{
						++incount;
						if (incount > MAX_ITEM_IN_MONSTER-1)
							return;
					}
				}
			}
			else
			{
				perc = rand() % 8;
				bCheckDevil = 0;
				if (type == 12 && index == 15)
				{
					if (monsterlevel < 13 || monsterlevel > 66)
					{
						perc = 1;
					}
					else
					{
						perc = rand() % 7;
						if (perc < 3)
							perc = 0;
					}
				}

				if (!perc)
				{
					if (bCheckDevil == 1)
					{
						if (rand() % 5)
						{
							if (monsterlevel >= 3)
							{
								if (monsterlevel >= 36)
								{
									if (monsterlevel >= 47)
									{
										if (monsterlevel >= 60)
										{
											if (monsterlevel >= 70)
												devilitemlevel = monsterlevel >= 80 ? 6 : 5;
											else
												devilitemlevel = 4;
										}
										else
										{
											devilitemlevel = 3;
										}
									}
									else
									{
										devilitemlevel = 2;
									}
								}
								else
								{
									devilitemlevel = 1;
								}
							}
							else
							{
								devilitemlevel = 0;
							}
							if (devilitemlevel)
							{
								if (this->InsertItem(monsterlevel, type, index, devilitemlevel, 0, 0, 0) != 255)
								{
									++incount;
									if (incount > MAX_ITEM_IN_MONSTER-1)
										return;
								}
							}
						}
					}
					else
					{
						if (zzzItemLevel(type, index, monsterlevel) == 1)
						{
							if (this->InsertItem(monsterlevel, type, index, 0, 0, 0, 0) != 255)
							{
								++incount;
								if (incount > MAX_ITEM_IN_MONSTER-1)
									return;
							}
						}
					}
				}
			}
			if (this->m_iMonsterInvenItemCount[monsterlevel] >= MAX_ITEM_IN_MONSTER)
				return;
		}
	}
	g_Log.Add("error-L3 : Monster Level Overflow~ %s %d", __FILE__, __LINE__);

}

int CMonsterItemMng::CheckMonsterDropItem(int type, int index)
{
	int result;
	if ((type != 14 || index != 13)
		&& (type != 14 || index != 14)
		&& (type != 14 || index != 16)
		&& (type != 14 || index != 22)
		&& (type != 12 || index != 15))
	{
		if (type != 13 || index != 3)
		{
			if ((type != 13 || index != 32)
				&& (type != 13 || index != 33)
				&& (type != 13 || index != 34)
				&& (type != 13 || index != 35)
				&& (type != 13 || index != 36)
				&& (type != 13 || index != 37))
			{
				if ((type != 14 || index != 35)
					&& (type != 14 || index != 36)
					&& (type != 14 || index != 37)
					&& (type != 14 || index != 38)
					&& (type != 14 || index != 39)
					&& (type != 14 || index != 40))
				{
					if (IsCashItem(ITEMGET(type, index)) == 1)
					{
						result = 0;
					}
					else
					{
						if (IsPremiumItem(ITEMGET(type, index)) == 1)
						{
							result = 0;
						}
						else
						{
							if (g_LuckyItemManager.IsLuckyItemTicket(ITEMGET(type,index)) == 1)
							{
								result = 0;
							}
							else
							{
								if (g_LuckyItemManager.IsLuckyItemEquipment(ITEMGET(type, index)) == 1)
								{
									result = 0;
								}
								else
								{
									if ((type != 14 || index != 45)
										&& (type != 14 || index != 46)
										&& (type != 14 || index != 47)
										&& (type != 14 || index != 48)
										&& (type != 14 || index != 49)
										&& (type != 14 || index != 50))
									{
										if (type != 13 || index != 41)
										{
											if ((type != 13 || index != 106) && (type != 13 || index != 107))
												result = (type != 14 || index != 162)
												&& (type != 14 || index != 163)
												&& (type != 14 || index != 164)
												&& (type != 14 || index != 165)
												&& (type != 14 || index != 166);
											else
												result = 0;
										}
										else
										{
											result = 0;
										}
									}
									else
									{
										result = 0;
									}
								}
							}
						}
					}
				}
				else
				{
					result = 0;
				}
			}
			else
			{
				result = 0;
			}
		}
		else
		{
			result = 0;
		}
	}
	else
	{
		result = 0;
	}
	return result;
}

CItem * CMonsterItemMng::GetItemEx(int monsterlevel)
{
	if (this->m_bLoad == false)
	{
		return nullptr;
	}

	if (monsterlevel < 0 || monsterlevel >= MAX_LEVEL_MONSTER)
	{
		return nullptr;
	}
			
	DWORD nRandValue = GetLargeRand() % 10000000;	
	int nSum = this->m_MonsterItemDropRate[monsterlevel].m_MagicBook;
	
	if (nRandValue >= nSum)
	{
		nSum += this->m_MonsterItemDropRate[monsterlevel].m_BlessRate;
		
		if (nRandValue >= nSum)
		{
			nSum += this->m_MonsterItemDropRate[monsterlevel].m_SoulRate;
					
			if (nRandValue >= nSum)
			{
				nSum += this->m_MonsterItemDropRate[monsterlevel].m_ChaosItemRate;
						
				if (nRandValue >= nSum)
				{
					nSum += this->m_MonsterItemDropRate[monsterlevel].m_LifeRate;
							
					if (nRandValue >= nSum)
					{
						nSum += this->m_MonsterItemDropRate[monsterlevel].m_CreationRate;

						if (nRandValue >= nSum)
						{
							int itemcount = this->m_iMonsterInvenItemCount[monsterlevel];
									
							if (itemcount <= 0)
							{
								return nullptr;
							}

							else
							{
								return &this->m_MonsterInvenItems[monsterlevel][GetLargeRand() % itemcount];
							}
						}
								
						else
						{
							return this->m_JewelOfCreationItem;
						}
					}
							
					else
					{
						return this->m_JewelOfLifeItem;
					}
						
				}

				else
				{
					return this->m_JewelOfChaosItem;
				}
			}
					
			else
			{
				return this->m_JewelOfSoulItem;
			}
		}

		else
		{
			return this->m_JewelOfBlessItem;
		}
			
	}

	else
	{
		int itemcount = this->m_iMagicBookItmeCount[monsterlevel];

		if (itemcount <= 0)
		{
			return nullptr;
		}

		else
		{
			return &this->m_MagicBookItems[monsterlevel][GetLargeRand() % itemcount];
		}
	}

	return nullptr;
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


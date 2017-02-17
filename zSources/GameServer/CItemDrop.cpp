//////////////////////////////////////////////////////////////////////
// CItemDrop.cpp
#include "StdAfx.h"
#include "CItemDrop.h"
#include "user.h"
#include "DSProtocol.h"
#include "configread.h"
#include "ItemOptionTypeMng.h"
#include "SetItemOption.h"
#include "ItemSocketOptionSystem.h"
#include "PentagramSystem.h"
#include "MuunSystem.h"
#include "LargeRand.h"

CItemDrop ItemDrop;

CItemDrop::CItemDrop(void)
{
	this->m_dwDropUseRate = 0;
}

CItemDrop::~CItemDrop(void)
{
}

bool CItemDrop::LoadFile(const char *szFile)
{
	this->m_vtMonsterList.clear();

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(szFile);

	if (res.status != pugi::status_ok)
	{
		g_Log.MsgBox("Error loading %s file (%s)", szFile, res.description());
		return false;
	}

	pugi::xml_node main = file.child("DropManager");
	this->m_dwDropUseRate = main.attribute("DropUseRate").as_int();

	for (pugi::xml_node monster = main.child("Monster"); monster; monster = monster.next_sibling())
	{
		boost::shared_ptr<ITEMDROP_MONSTER> itemdrop_monster_ptr_(new ITEMDROP_MONSTER);
		itemdrop_monster_ptr_->wMonsterID = monster.attribute("Index").as_int();
		itemdrop_monster_ptr_->wMonsterMinLevel = monster.attribute("MonsterMinLevel").as_int();
		itemdrop_monster_ptr_->wMonsterMaxLevel = monster.attribute("MonsterMaxLevel").as_int();
		itemdrop_monster_ptr_->btMonsterMapNumber = monster.attribute("MapNumber").as_int();
		itemdrop_monster_ptr_->dwItemDropRate = monster.attribute("ItemRate").as_int();
		itemdrop_monster_ptr_->btItemCount = monster.attribute("ItemCount").as_int();
		itemdrop_monster_ptr_->bIsCoinReward = monster.attribute("CoinReward").as_bool();
		itemdrop_monster_ptr_->btCoinType = monster.attribute("CoinType").as_int();
		itemdrop_monster_ptr_->dwCoinValue = monster.attribute("CoinValue").as_int();
		itemdrop_monster_ptr_->wPlayerMinLevel = monster.attribute("PlayerMinLevel").as_int(1);

		if(strcmp(monster.attribute("PlayerMaxLevel").as_string(), "MAX") == 0)
		{
			itemdrop_monster_ptr_->wPlayerMaxLevel = g_ConfigRead.data.common.UserMaxLevel + g_ConfigRead.data.common.MLUserMaxLevel;
		}
		
		else
		{
			itemdrop_monster_ptr_->wPlayerMaxLevel = monster.attribute("PlayerMaxLevel").as_int(730);
		}

		for (pugi::xml_node item = monster.child("Item"); item; item = item.next_sibling())
		{
			boost::shared_ptr<ITEMDROP_ITEM> itemdrop_item_ptr_(new ITEMDROP_ITEM);
			itemdrop_item_ptr_->btItemType = item.attribute("Cat").as_int();
			itemdrop_item_ptr_->wItemIndex = item.attribute("Index").as_int();
			itemdrop_item_ptr_->btItemMinLevel = item.attribute("ItemMinLevel").as_int(0);
			itemdrop_item_ptr_->btItemMaxLevel = item.attribute("ItemMaxLevel").as_int(0);
			itemdrop_item_ptr_->btItemDurability = item.attribute("Durability").as_int(-1);
			itemdrop_item_ptr_->btIsSkill = item.attribute("Skill").as_int();
			itemdrop_item_ptr_->btIsLuck = item.attribute("Luck").as_int();
			itemdrop_item_ptr_->btIsOption = item.attribute("Option").as_int();
			itemdrop_item_ptr_->btIsExc = item.attribute("Exc").as_int();
			itemdrop_item_ptr_->btIsSetItem = item.attribute("SetItem").as_int();
			itemdrop_item_ptr_->btSocketCount = item.attribute("SocketCount").as_int();
			itemdrop_item_ptr_->btIsElementalItem = item.attribute("ElementalItem").as_int();
			itemdrop_item_ptr_->btMuunEvoItemType = item.attribute("MuunEvolutionItemCat").as_int(-1);
			itemdrop_item_ptr_->wMuunEvoItemIndex = item.attribute("MuunEvolutionItemIndex").as_int(-1);
			itemdrop_item_ptr_->dwItemDropRate = item.attribute("DropRate").as_int();
			itemdrop_item_ptr_->dwPeriodDuration = item.attribute("Duration").as_int();

			itemdrop_monster_ptr_->m_vtItems.push_back(itemdrop_item_ptr_);
		}

		this->m_vtMonsterList.push_back(itemdrop_monster_ptr_);
	} 
}

bool CItemDrop::DropItem(LPOBJ lpUser, LPOBJ lpMonster)
{
	if(!ObjectMaxRange(lpUser->m_Index))
		return false;

	if(!ObjectMaxRange(lpMonster->m_Index))
		return false;

	if (GetLargeRand() % 1000000 >= this->m_dwDropUseRate)
		return false;

	int iItemCount = 0;
	ITEMDROP_ITEM ** lpItems = this->GetItem(lpUser, lpMonster, iItemCount);

	if (lpItems == NULL)
	{
		return false;
	}

	if (iItemCount <= 0)
	{
		delete [] lpItems;
		return false;
	}

	for (int i = 0; i < iItemCount; i++)
	{
		int ItemNumber = ItemGetNumberMake(lpItems[i]->btItemType, lpItems[i]->wItemIndex);

		if (ItemNumber == -1)
		{
			delete [] lpItems;
			g_Log.AddC(TColor::Red, "ERROR - Bad Index (%d) CItemDrop::DropItem (%s)(%d)", ItemNumber, lpUser->Name, lpMonster->Class);
			return false;
		}

		LPITEM_ATTRIBUTE p = &ItemAttribute[ItemNumber];

		if (!p)
		{
			delete [] lpItems;
			g_Log.AddC(TColor::Red, "ERROR - Item not found (%d) CItemDrop::DropItem (%s)(%d)", ItemNumber, lpUser->Name, lpMonster->Class);
			return false;
		}

		BYTE btSkill = 0;
		BYTE btLuck = 0;
		BYTE btOption = 0;
		BYTE btExc = 0;
		BYTE btItemLevel = 0;
		BYTE btItemDurability = 0;
		BYTE btSetItem = 0;
		BYTE btSocketCount = 0;
		BYTE btMainAttribute;
		BYTE btSocketOption[5] = { -1, -1, -1, -1, -1 };
		DWORD dwDuration = lpItems[i]->dwPeriodDuration;

		if (lpItems[i]->btIsSkill == (BYTE)-1)
		{
			btSkill = rand() % 2;
		}

		else
		{
			btSkill = lpItems[i]->btIsSkill;
		}

		if (lpItems[i]->btIsLuck == (BYTE)-1)
		{
			btLuck = rand() % 2;
		}

		else
		{
			btLuck = lpItems[i]->btIsLuck;
		}

		if (lpItems[i]->btIsOption == (BYTE)-1)
		{
			btOption = rand() % (g_ConfigRead.data.common.Is28Opt == true ? 7 : 4);
		}

		else
		{
			btOption = lpItems[i]->btIsOption;
		}

		if (lpItems[i]->btIsExc == (BYTE)-1)
		{
			btExc = g_ItemOptionTypeMng.CommonExcOptionRand(p->ItemKindA);
		}

		else
		{
			btExc = lpItems[i]->btIsExc;
		}

		btItemLevel = this->GetItemLevel(lpItems[i]->btItemMinLevel, lpItems[i]->btItemMaxLevel);

		if (lpItems[i]->btItemDurability == (BYTE)-1)
		{
			btItemDurability = ItemGetDurability(ItemNumber, btItemLevel, btExc, btSetItem);
		}

		else
		{
			btItemDurability = lpItems[i]->btItemDurability;
		}

		if (lpItems[i]->btIsSetItem != FALSE && gSetItemOption.IsSetItem(ItemNumber) == TRUE)
		{
			btSetItem = gSetItemOption.GenSetOption(ItemNumber);
		}

		else
		{
			btSetItem = FALSE;
		}

		if (g_SocketOptionSystem.IsEnableSocketItem(ItemNumber) == TRUE)
		{
			if (lpItems[i]->btSocketCount != 0)
			{
				btSocketCount = (rand() % lpItems[i]->btSocketCount) + 1;

				for (int i = 0; i < btSocketCount; i++)
				{
					btSocketOption[i] = 0xFE;
				}
			}

			else
			{
				btSocketCount = g_SocketOptionSystem.GetMakeSocketSlotCount(ItemNumber);

				for (int i = 0; i < btSocketCount; i++)
				{
					btSocketOption[i] = 0xFE;
				}
			}
		}

		else
		{
			btSocketCount = 0;
		}

		if (g_PentagramSystem.IsPentagramItem(ItemNumber) || g_PentagramSystem.IsPentagramJewel(ItemNumber))
		{
			if (lpItems[i]->btIsElementalItem != 0 && lpItems[i]->btIsElementalItem != (BYTE)-1)
			{
				btMainAttribute = lpItems[i]->btIsElementalItem;
			}

			else
			{
				btMainAttribute = (rand() % 5) + 1;
			}

			if (g_PentagramSystem.IsPentagramItem(ItemNumber))
			{
				BYTE btEnableSlot[5];
				btSocketCount = g_PentagramSystem.GetMakePentagramSlotCountNKind(btEnableSlot, ItemNumber);

				for (int i = 0; i < 5; i++)
				{
					if (btEnableSlot[i] == TRUE)
					{
						btSocketOption[i] = 0xFE;
					}

					else
					{
						btSocketOption[i] = 0xFF;
					}
				}
			}
		}

		if (g_CMuunSystem.IsStoneofEvolution(ItemNumber))
		{
			int iMuunEvolutionItemID = ItemGetNumberMake(lpItems[i]->btMuunEvoItemType, lpItems[i]->wMuunEvoItemIndex);
			btSocketOption[0] = iMuunEvolutionItemID >> 8;
			btSocketOption[1] = ITEM_GET_INDEX(iMuunEvolutionItemID);
		}

		ItemSerialCreateSend(lpMonster->m_Index, lpMonster->MapNumber, lpMonster->X, lpMonster->Y, ItemNumber, btItemLevel, 0, btSkill, btLuck, btOption, lpUser->m_Index, btExc, btSetItem, dwDuration, btSocketOption, btMainAttribute);
	}

	delete [] lpItems;
	return true;
}

ITEMDROP_ITEM ** CItemDrop::GetItem(OBJECTSTRUCT *lpUser, OBJECTSTRUCT* lpMonster, int & iItemCount)
{
	boost::shared_ptr<ITEMDROP_MONSTER> itemdrop_monster_ = this->GetMonsterData(lpUser, lpMonster);

	if (itemdrop_monster_ == NULL)
	{
		return NULL;
	}

	this->AddCoinReward(itemdrop_monster_, lpUser);

	if (itemdrop_monster_->btItemCount <= 0)
	{
		return NULL;
	}

	iItemCount = 0;
	ITEMDROP_ITEM ** lpItems = new ITEMDROP_ITEM *[itemdrop_monster_->btItemCount];

	for (int i = 0; i < itemdrop_monster_->btItemCount; i++)
	{
		boost::shared_ptr<ITEMDROP_ITEM> itemdrop_item_ = this->GetItemFromMonster(itemdrop_monster_);

		if (itemdrop_item_ == NULL)
		{
			continue;
		}

		lpItems[iItemCount] = itemdrop_item_.get();
		iItemCount++;
	}

	return lpItems;
}

boost::shared_ptr<ITEMDROP_MONSTER> CItemDrop::GetMonsterData(OBJECTSTRUCT *lpUser, OBJECTSTRUCT* lpMonster)
{
	TRandomPoolMgr RandomPoolSection;

	RandomPoolSection.InitPool();
	int SectionID = 1;

	for (std::vector<boost::shared_ptr<ITEMDROP_MONSTER>>::iterator It = this->m_vtMonsterList.begin(); It != this->m_vtMonsterList.end(); It++, SectionID++)
	{
		ITEMDROP_MONSTER * itemdrop_monster_ = It->get();

		if (itemdrop_monster_->wMonsterID != lpMonster->Class && itemdrop_monster_->wMonsterID != (WORD)-1)
		{
			continue;
		}

		if (itemdrop_monster_->btMonsterMapNumber != lpMonster->MapNumber && itemdrop_monster_->btMonsterMapNumber != (BYTE)-1)
		{
			continue;
		}

		if (lpMonster->Level < itemdrop_monster_->wMonsterMinLevel && itemdrop_monster_->wMonsterMinLevel != (WORD)-1)
		{
			continue;
		}

		if (lpMonster->Level > itemdrop_monster_->wMonsterMaxLevel && itemdrop_monster_->wMonsterMaxLevel != (WORD)-1)
		{
			continue;
		}

		if (lpUser->Level < itemdrop_monster_->wPlayerMinLevel && itemdrop_monster_->wPlayerMinLevel != (WORD)-1)
		{
			continue;
		}

		if (lpUser->Level > itemdrop_monster_->wPlayerMaxLevel && itemdrop_monster_->wPlayerMaxLevel != (WORD)-1)
		{
			continue;
		}

		if (rand() % 10000 >= itemdrop_monster_->dwItemDropRate)
		{
			continue;
		}

		RandomPoolSection.AddValue(SectionID, 0);
	}

	int RetSection = RandomPoolSection.GetRandomValue(eRANDOMPOOL_BY_RANDOM);

	if (RetSection == 0)
	{
		return NULL;
	}

	return this->m_vtMonsterList[RetSection-1];
}

boost::shared_ptr<ITEMDROP_ITEM> CItemDrop::GetItemFromMonster(boost::shared_ptr<ITEMDROP_MONSTER> itemdrop_monster_ptr)
{
	int iSuccessRate = 0;
	int iSuccessMaxRate = 0;
	int itemDropRate = 0;

	for (std::vector<boost::shared_ptr<ITEMDROP_ITEM>>::iterator It = itemdrop_monster_ptr.get()->m_vtItems.begin(); It != itemdrop_monster_ptr.get()->m_vtItems.end(); It++)
	{
		iSuccessMaxRate += It->get()->dwItemDropRate;
	}

	if(iSuccessMaxRate < 1)
		return NULL;

	int randResult = rand()%iSuccessMaxRate;

	for (std::vector<boost::shared_ptr<ITEMDROP_ITEM>>::iterator It = itemdrop_monster_ptr.get()->m_vtItems.begin(); It != itemdrop_monster_ptr.get()->m_vtItems.end(); It++)
	{
		ITEMDROP_ITEM * itemdrop_item_ = It->get();
		iSuccessRate += itemdrop_item_->dwItemDropRate;
		
		itemDropRate = iSuccessRate;
		
		int iItemRateRangeLow = iSuccessRate-itemDropRate;
		int iItemRateRangeHigh = iSuccessRate;

		if (randResult <= iItemRateRangeHigh && randResult >= iItemRateRangeLow)
		{
			return *It;
		}
	}

	return NULL;
}

void CItemDrop::AddCoinReward(boost::shared_ptr<ITEMDROP_MONSTER> itemdrop_monster_ptr, OBJECTSTRUCT *lpUser)
{
	if (itemdrop_monster_ptr->bIsCoinReward == false)
	{
		return;
	}

	GDReqInGameShopPointAdd(lpUser->m_Index, itemdrop_monster_ptr->btCoinType, itemdrop_monster_ptr->dwCoinValue);
}

bool CItemDrop::LoadZenDropFile(const char *szFile)
{
	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(szFile);

	if (res.status != pugi::status_ok)
	{
		g_Log.MsgBox("Failed to load %s file (%s)", szFile, res.description());
		return false;
	}

	this->m_mapZenDrop.clear();

	pugi::xml_node main = file.child("ZenDropSystem");

	this->m_bZenDropEnable = main.attribute("Enable").as_bool();
	this->m_bMultiplyZenDropByMonLevel = main.attribute("MultiplyByMonsterLevel").as_bool();
	this->m_dwMultiplyZenChanceRate = main.attribute("MultiplyChanceRate").as_int();

	for (pugi::xml_node map = main.child("Map"); map; map = map.next_sibling())
	{
		ZEN_DROP ZenDrop;

		ZenDrop.btMapNumber = map.attribute("Number").as_int();
		ZenDrop.dwMinMoney = map.attribute("MinMoneyCount").as_int();
		ZenDrop.dwMaxMoney = map.attribute("MaxMoneyCount").as_int();

		this->m_mapZenDrop.insert(std::pair<int, ZEN_DROP>(ZenDrop.btMapNumber, ZenDrop));
	}
}

bool CItemDrop::IsZenDropActive(BYTE btMapNumber)
{
	if (this->m_bZenDropEnable == false)
	{
		return false;
	}

	if (this->m_mapZenDrop.find(btMapNumber) == this->m_mapZenDrop.end())
	{
		return false;
	}

	return true;
}

DWORD CItemDrop::GetZenAmount(BYTE btMapNumber, WORD wMonsterLevel)
{
	if (this->m_bZenDropEnable == false)
	{
		return 0;
	}

	std::map<int, ZEN_DROP>::iterator It = this->m_mapZenDrop.find(btMapNumber);

	if (It == this->m_mapZenDrop.end())
	{
		return 0;
	}

	DWORD dwMoney = 0;
	DWORD dwSub = It->second.dwMaxMoney - It->second.dwMinMoney;

	if (dwSub < 0)
	{
		return 0;
	}

	dwMoney = It->second.dwMinMoney + rand()%(dwSub+1);

	if (this->m_bMultiplyZenDropByMonLevel == true && (rand()%10000) < this->m_dwMultiplyZenChanceRate)
	{
		dwMoney *= wMonsterLevel;
	}

	return dwMoney * 2;
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


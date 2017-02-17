//////////////////////////////////////////////////////////////////////
// Bag.cpp
#include "stdafx.h"
#include "Bag.h"
#include "TLog.h"
#include "user.h"
#include "LuaBag.h"
#include "winutil.h"
#include "GameMain.h"
#include "DSProtocol.h"
#include "LargeRand.h"
#include "configread.h"

const char * szFileClassName [] = { "Wizard", "Knight", "FairyElf", "MagicGladiator", "DarkLord", "Summoner", "RageFighter", "GrowLancer" };

CBag::CBag()
{
	this->InitBag();
}

CBag::~CBag()
{
	this->m_BagData.m_vtSectionDrop.clear();
}

bool CBag::LoadBag(const char* lpFileName)
{
	this->InitBag();

	pugi::xml_document file;
	pugi::xml_parse_result result = file.load_file(lpFileName);

	if (result.status != pugi::status_ok)
	{
		g_Log.MsgBox("Failed to load %s file!", lpFileName);
		g_Log.AddC(TColor::Red, "[ERROR] ItemBag load failed: %s", lpFileName);

		return false;
	}

	pugi::xml_node main_section = file.child("ItemBag");
	pugi::xml_node config_section = main_section.child("BagConfig");
	pugi::xml_node coin_section = main_section.child("AddCoin");
	pugi::xml_node summon_section = main_section.child("SummonBook");

	strcpy(this->m_BagData.szBagName, config_section.attribute("Name").as_string());
	this->m_BagData.dwItemDropRate = config_section.attribute("ItemRate").as_int(0);
	this->m_BagData.dwRandomSetItemDropRate = config_section.attribute("SetItemRate").as_int(0);
	this->m_BagData.dwDropMoney = config_section.attribute("MoneyDrop").as_int(0);
	this->m_BagData.iBagUseEffect = config_section.attribute("BagUseEffect").as_int(-1);
	this->m_BagData.dwBagUseRate = config_section.attribute("BagUseRate").as_int(0);

	this->m_BagData.bAddCoinEnable = coin_section.attribute("Enable").as_bool(false);
	this->m_BagData.btAddCoinType = coin_section.attribute("CoinType").as_int(0);
	this->m_BagData.dwAddCoinValue = coin_section.attribute("CoinValue").as_int(0);

	this->m_BagData.bSummonBookEnable = summon_section.attribute("Enable").as_bool(false);
	this->m_BagData.dwSummonBookDropRate = summon_section.attribute("DropRate").as_int(0);
	this->m_BagData.btSummonBookItemType = summon_section.attribute("ItemCat").as_int(0);
	this->m_BagData.wSummonBookItemIndex = summon_section.attribute("ItemIndex").as_int(0);

	if(this->m_BagData.dwItemDropRate < 10000 && this->m_BagData.dwDropMoney == 0)
	{
		g_Log.MsgBox("Error - %s bag has ItemDropRate lower than maximum and DropMoney is zero", lpFileName);
	}

	int SectionNumber;
	BAG_SECTION_DROP m_DropSection;
	BAG_SECTION_ITEMS m_ItemSection;
	BAG_ITEM m_Item;

	for (pugi::xml_node section = main_section.child("DropAllow"); section; section = section.next_sibling())
	{
		SectionNumber = 1;

		for (int i = 0; i < MAX_TYPE_PLAYER; i++)
		{
			m_DropSection.btUseSectionAllowedClass[i] = section.attribute(szFileClassName[i]).as_int();
		}

		m_DropSection.wUseSectionMinLevel = section.attribute("PlayerMinLevel").as_int(1);

		if (strcmp("MAX", section.attribute("PlayerMaxLevel").as_string()) == 0)
		{
			m_DropSection.wUseSectionMaxLevel = g_ConfigRead.data.common.UserMaxLevel + g_ConfigRead.data.common.MLUserMaxLevel;
		}

		else
		{
			m_DropSection.wUseSectionMaxLevel = section.attribute("PlayerMaxLevel").as_int(730);
		}

		m_DropSection.btUseSectionAllowedMapNum = section.attribute("MapNumber").as_int(-1);

		if ( m_DropSection.btUseSectionAllowedMapNum != 0xFF && MAX_MAP_RANGE(m_DropSection.btUseSectionAllowedMapNum) == FALSE )
		{
			g_Log.MsgBox("Error: Bag (%s), DropAllow Section:%d, btUseSectionAllowedMapNum value is wrong", lpFileName, m_DropSection.btUseSectionAllowedMapNum);
			continue;
		}

		for (pugi::xml_node dropsec = section.child("Drop"); dropsec; dropsec = dropsec.next_sibling())
		{
			m_ItemSection.btSectionID = SectionNumber;
			m_ItemSection.dwSectionUseRate = dropsec.attribute("Rate").as_int();
			m_ItemSection.btItemDropCount = dropsec.attribute("Count").as_int();
			m_ItemSection.m_vtItems.clear();

			for (pugi::xml_node item = dropsec.child("Item"); item; item = item.next_sibling())
			{
				m_Item.btItemType = item.attribute("Cat").as_int(-1);
				m_Item.wItemIndex = item.attribute("Index").as_int(-1);
				m_Item.btItemMinLevel = item.attribute("ItemMinLevel").as_int(0);
				m_Item.btItemMaxLevel = item.attribute("ItemMaxLevel").as_int(0);
				m_Item.btItemDur = item.attribute("Durability").as_int(0);
				m_Item.btIsSkill = item.attribute("Skill").as_int(0);
				m_Item.btIsLuck = item.attribute("Luck").as_int(0);
				m_Item.btIsOption = item.attribute("Option").as_int(0);
				m_Item.btIsExItem = item.attribute("Exc").as_int(0);
				m_Item.btIsSetItem = item.attribute("SetItem").as_int(0);
				m_Item.btIsSocketItem = item.attribute("SocketCount").as_int(0);
				m_Item.btIsElementalItem = item.attribute("ElementalItem").as_int(0);
				m_Item.btMuunEvolutionItemType = item.attribute("MuunEvolutionItemCat").as_int(-1);
				m_Item.wMuunEvolutionItemIndex = item.attribute("MuunEvolutionItemIndex").as_int(-1);
				m_Item.dwDuration = item.attribute("Duration").as_int(0);

				if (ItemGetNumberMake(m_Item.btItemType, m_Item.wItemIndex) == -1)
				{
					g_Log.MsgBox("Error: Wrong Item in %s bag (Cat: %d Index: %d)", this->m_BagData.szBagName, m_Item.btItemType, m_Item.wItemIndex);
					continue;
				}

				m_ItemSection.m_vtItems.push_back(m_Item);
			}

			m_DropSection.m_mapSectionItems.insert(std::pair<int, BAG_SECTION_ITEMS>(SectionNumber, m_ItemSection));
			SectionNumber++;
		}

		this->m_BagData.m_vtSectionDrop.push_back(m_DropSection);
	}

	this->m_BagData.m_bLoadBag = true;
	return true;
}

void CBag::InitBag()
{
	memset(this->m_BagData.szBagName, 0x00, sizeof(this->m_BagData.szBagName));
	this->m_BagData.dwDropMoney = 0;
	this->m_BagData.dwItemDropRate = 0;
	this->m_BagData.iBagUseEffect = -1;
	this->m_BagData.m_vtSectionDrop.clear();
	this->m_BagData.m_bLoadBag = false;
}

int CBag::GetDropSection(int aIndex, BAG_SECTION_DROP &pRetDrop)
{
	if (this->m_BagData.m_bLoadBag == false)
	{
		return FALSE;
	}	
	
	if (ObjectMaxRange(aIndex) == false)
	{
		return FALSE;
	}

	if (gObj[aIndex].Type != OBJ_USER)
	{
		return FALSE;
	}

	if (this->m_BagData.m_vtSectionDrop.size() <= 0)
	{
		g_Log.AddC(TColor::Red, "[ERROR] Bag %s has no drop sections in XML file", this->m_BagData.szBagName);
		return FALSE;
	}

	LPOBJ lpObj = &gObj[aIndex];
	TRandomPoolMgr m_RandomPoolSection;

	m_RandomPoolSection.InitPool();
	int SectionID = 1;

	for (std::vector<BAG_SECTION_DROP>::iterator It = this->m_BagData.m_vtSectionDrop.begin(); It != this->m_BagData.m_vtSectionDrop.end(); It++, SectionID++)
	{
		if (lpObj->MapNumber != It->btUseSectionAllowedMapNum && It->btUseSectionAllowedMapNum != 0xFF)
		{
			continue;
		}

		if (It->btUseSectionAllowedClass[lpObj->Class] == 0)
		{
			continue;
		}

		if (It->btUseSectionAllowedClass[lpObj->Class] > 1)
		{
			if (It->btUseSectionAllowedClass[lpObj->Class] > (lpObj->m_PlayerData->ChangeUP + 1))
			{
				continue;
			}
		}

		if ((lpObj->Level + lpObj->m_PlayerData->MasterLevel) < It->wUseSectionMinLevel)
		{
			continue;
		}

		if ((lpObj->Level + lpObj->m_PlayerData->MasterLevel) > It->wUseSectionMaxLevel)
		{
			continue;
		}

		m_RandomPoolSection.AddValue(SectionID, 0);
	}

	int RetSection = m_RandomPoolSection.GetRandomValue(eRANDOMPOOL_BY_RANDOM);

	if (RetSection == 0)
	{
		return FALSE;
	}

	pRetDrop = this->m_BagData.m_vtSectionDrop[RetSection-1];

	return TRUE;
}

int CBag::GetItemsSection(BAG_SECTION_DROP &pDropSec, BAG_SECTION_ITEMS &pRetItems)
{
	TRandomPoolMgr m_RandomPoolSection;

	m_RandomPoolSection.InitPool();

	for (std::map<int, BAG_SECTION_ITEMS>::iterator It = pDropSec.m_mapSectionItems.begin(); It != pDropSec.m_mapSectionItems.end(); It++)
	{
		m_RandomPoolSection.AddValue(It->second.btSectionID, It->second.dwSectionUseRate);
	}

	int SectionID = m_RandomPoolSection.GetRandomValue(eRANDOMPOOL_BY_WEIGHT);

	if (SectionID == 0)
	{
		g_Log.AddC(TColor::Red, "[ERROR] Bag %s has no item sections in XML file", this->m_BagData.szBagName);
		return FALSE;
	}

	pRetItems = pDropSec.m_mapSectionItems[SectionID];

	return TRUE;
}

int CBag::GetItem(BAG_SECTION_ITEMS &pItemsSec, BAG_ITEM & pRetItem)
{
	if (pItemsSec.m_vtItems.size() == 0)
	{
		g_Log.AddC(TColor::Red, "[ERROR] CBag::GetItem() -> Item Container is EMPTY - check XML File (%s)", this->m_BagData.szBagName);
		return FALSE;
	}

	int BagSize = pItemsSec.m_vtItems.size();
	int ItemNum = rand() % BagSize;

	pRetItem = pItemsSec.m_vtItems[ItemNum];

	return TRUE;
}

int CBag::GetReadyItemToUse(int aIndex, CItem &pItem, time_t & DurationItem)
{
	LPOBJ lpObj = &gObj[aIndex];

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

	if (this->GetItem(m_ItemSection, m_Item) == FALSE)
	{
		g_Log.AddC(TColor::Red, "[ERROR] Bag %s -> GetItem() == FALSE -> probably no item in %d section", this->m_BagData.szBagName, m_ItemSection.btSectionID);
		return false;
	}

	bool bResult = gLuaBag.MakeItemFromBag(aIndex, &m_Item, pItem, DurationItem);

	if (bResult == false)
	{
		g_Log.Add("[%s] Create Item Fail [%s][%s]", this->m_BagData.szBagName, lpObj->AccountID, lpObj->Name);
		return false;
	}

	g_Log.Add("[%s] Create Item [%s][%s] : (%d)(%d/%d) Item:(%s)%d", this->m_BagData.szBagName,
		lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y, ItemAttribute[ITEMGET(m_Item.btItemType, m_Item.wItemIndex)].Name, ITEMGET(m_Item.btItemType, m_Item.wItemIndex));

	return true;
}

int CBag::GetItemLevel(int MinLevel, int MaxLevel)
{
	if (MinLevel == MaxLevel)	return MinLevel;

	int sub, level = 0;

	__try
	{
		sub = (MaxLevel - MinLevel) + 1;
		level = MinLevel + (rand() % sub);
	}

	__except (sub = 1, 1)
	{
		return 0;
	}

	return level;
}

void CBag::MakeBagEffectUse(int aIndex, int iMonsterIndex)
{
	PMSG_SERVERCMD ServerCmd;

	PHeadSubSetB((LPBYTE)&ServerCmd, 0xF3, 0x40, sizeof(ServerCmd));
	ServerCmd.CmdType = this->m_BagData.iBagUseEffect;

	switch (this->m_BagData.iBagUseEffect)
	{
		case 0:
			ServerCmd.X = gObj[iMonsterIndex].X;
			ServerCmd.Y = gObj[iMonsterIndex].Y;
			break;
		case 2:
			ServerCmd.X = gObj[iMonsterIndex].X;
			ServerCmd.Y = gObj[iMonsterIndex].Y;
			break;
		case 58:
			ServerCmd.X = SET_NUMBERH(aIndex);
			ServerCmd.Y = SET_NUMBERL(aIndex);
			break;
	}

	GSProtocol.MsgSendV2(&gObj[aIndex], (LPBYTE)&ServerCmd, ServerCmd.h.size);
	IOCP.DataSend(aIndex, (LPBYTE)&ServerCmd, ServerCmd.h.size);
}

void CBag::AddCoin(int aIndex)
{
	if (ObjectMaxRange(aIndex) == false)
	{
		return;
	}

	if (this->m_BagData.bAddCoinEnable == false)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	char *CoinTypes[] = { "C", "P", "G" };
	GDReqInGameShopPointAdd(aIndex, this->m_BagData.btAddCoinType, this->m_BagData.dwAddCoinValue);
	MsgOutput(aIndex, Lang.GetText(0,502), CoinTypes[this->m_BagData.btAddCoinType], this->m_BagData.dwAddCoinValue);
	g_Log.Add("[Bag Manager] (%s)(%s) Add Coin: (Type:%d) (Amount:%d) (Bag:%s)", lpObj->AccountID, lpObj->Name, this->m_BagData.btAddCoinType, this->m_BagData.dwAddCoinValue, this->m_BagData.szBagName);
}

void CBag::DropSummonItem(int aIndex)
{
	if (ObjectMaxRange(aIndex) == false)
	{
		return;
	}

	if (this->m_BagData.bSummonBookEnable == false)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->m_bIsHelpMon == true)
	{
		return;
	}

	if (GetLargeRand() % 1000000 <= this->m_BagData.dwSummonBookDropRate)
	{
		int iType = ItemGetNumberMake(this->m_BagData.btSummonBookItemType, this->m_BagData.wSummonBookItemIndex);

		if (iType == -1)
		{
			return;
		}

		ItemSerialCreateSend(aIndex, gObj[aIndex].MapNumber, gObj[aIndex].X, gObj[aIndex].Y, iType, 0, -1, 0, 0, 0, aIndex, 0, 0, 0, 0, 0);
	}
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


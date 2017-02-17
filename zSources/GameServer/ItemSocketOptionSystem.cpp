//////////////////////////////////////////////////////////////////////
// ItemSocketOptionSystem.cpp
#include "stdafx.h"
#include "ItemSocketOptionSystem.h"
#include "LargeRand.h"
#include "user.h"
#include "Protocol.h"
#include "TLog.h"
#include "configread.h"

CItemSocketOptionSystem	g_SocketOptionSystem;

CItemSocketOptionSystem::CItemSocketOptionSystem()
{

}

CItemSocketOptionSystem::~CItemSocketOptionSystem()
{

}

void CItemSocketOptionSystem::Initialize()
{
	int iClearIndex = 0;

	for (iClearIndex = 0; iClearIndex < 50; iClearIndex++)
	{
		m_SeedData[iClearIndex].Clear();
	}

	for (iClearIndex = 0; iClearIndex < 5; iClearIndex++)
	{
		m_SphereData[iClearIndex].Clear();
	}

	for (iClearIndex = 0; iClearIndex < 12; iClearIndex++)
	{
		m_BonusSocketOptionData[iClearIndex].Clear();
	}

	m_SeedExtractPool.InitPool();

	for (iClearIndex = 0; iClearIndex < 5; iClearIndex++)
	{
		m_SphereDropRate[iClearIndex] = 0;
		m_SphereDropLevel[iClearIndex] = 0;
	}
}

bool CItemSocketOptionSystem::LoadOptionScript(char *pchFileName)
{
	Initialize();

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(pchFileName);

	if (res.status != pugi::status_ok)
	{
		g_Log.MsgBox("[SocketOption] SocketOption Script file Load failed. (%s) (%s)",pchFileName, res.description());
		return false;
	}

	int iCategoryIndex = 0;
	int iSeedOptionDataCount = 0;
	int iSphereOptionDataCount = 0;
	int iBonusSocketOptionDataCount = 0;
	int iSetSocketOptionDataCount = 0;

	pugi::xml_node main = file.child("SocketSystem");
	pugi::xml_node item_option = main.child("SocketItemOptionSettings");

	for (pugi::xml_node option = item_option.child("Option"); option; option = option.next_sibling())
	{
		BYTE btSeedIndex = 0;
		BYTE btSeedType = 0;
		BYTE btItemLevel = 0;
		BYTE btOptionValueType = 0;
		WORD wOptionValue[5] = {0};

		int iExtractRate = 0;
		int iItemCode = 0;

		btSeedIndex			= option.attribute("Index").as_int();
		btSeedType			= option.attribute("ElementType").as_int();
		btItemLevel			= option.attribute("Level").as_int();
		btOptionValueType	= option.attribute("BonusType").as_int();
		wOptionValue[0]		= option.attribute("BonusValue1").as_int();
		wOptionValue[1]		= option.attribute("BonusValue2").as_int();
		wOptionValue[2]		= option.attribute("BonusValue3").as_int();
		wOptionValue[3]		= option.attribute("BonusValue4").as_int();
		wOptionValue[4]		= option.attribute("BonusValue5").as_int();
		iExtractRate		= option.attribute("PoolValue").as_int();

		m_SeedExtractPool.AddValue(btSeedIndex, iExtractRate);
		m_SeedData[iSeedOptionDataCount].SetOption(btSeedIndex, btSeedType, btItemLevel, btOptionValueType, wOptionValue, iExtractRate);
		iSeedOptionDataCount++;

		if (iSeedOptionDataCount > 50)
		{
			g_Log.MsgBox("[ERROR] MAX SeedSphere Option Data Overflow! [%d]", iSeedOptionDataCount);
			break;
		}
	}

	pugi::xml_node bonus_option = main.child("SocketBonusSettings");

	for (pugi::xml_node option = bonus_option.child("Option"); option; option = option.next_sibling())
	{
		BONUS_SOCKET_OPTION BonusOptionData;
		memset(&BonusOptionData, 0, sizeof(BONUS_SOCKET_OPTION));

		BonusOptionData.OptionIndex			= option.attribute("Index").as_int();	
		BonusOptionData.ItemTypeBegin		= option.attribute("MinItemType").as_int();
		BonusOptionData.ItemTypeEnd			= option.attribute("MaxItemType").as_int();

		BonusOptionData.OptionValueType		= option.attribute("BonusType").as_int();
		BonusOptionData.OptionValue			= option.attribute("BonusValue").as_int();
		BonusOptionData.ExtractRate			= option.attribute("BonusRate").as_int();
		BonusOptionData.SocketSlotType[0]	= option.attribute("ElementType1").as_int();
		BonusOptionData.SocketSlotType[1]	= option.attribute("ElementType2").as_int();
		BonusOptionData.SocketSlotType[2]	= option.attribute("ElementType3").as_int();
		BonusOptionData.SocketSlotType[3]	= option.attribute("ElementType4").as_int();
		BonusOptionData.SocketSlotType[4]	= option.attribute("ElementType5").as_int();

		for (int i = 0; i < 5; i++)
		{
			if (BonusOptionData.SocketSlotType[i] != NULL)
			{
				BonusOptionData.SocketSlotCount++;
			}
		}

		switch (BonusOptionData.OptionIndex)
		{
			case BONUS_OPTION_WEAPON_ATTACK_POWER:
			case BONUS_OPTION_WEAPON_ATTACK_POWER_4TH:			BonusOptionData.EffectType = SEED_EFFECT_TYPE_FIRE_BEGIN;			break;
			case BONUS_OPTION_WEAPON_ATTACK_SKILL_POWER:	
			case BONUS_OPTION_WEAPON_ATTACK_SKILL_POWER_4TH:	BonusOptionData.EffectType = SEED_EFFECT_TYPE_ATTACK_SKILL_POWER;	break;
			case BONUS_OPTION_STAFF_ATTACK_POWER:
			case BONUS_OPTION_STAFF_ATTACK_POWER_4TH:			BonusOptionData.EffectType = SEED_EFFECT_TYPE_FIRE_BEGIN;			break;
			case BONUS_OPTION_STAFF_ATTACK_SKILL_POWER:
			case BONUS_OPTION_STAFF_ATTACK_SKILL_POWER_4TH:		BonusOptionData.EffectType = SEED_EFFECT_TYPE_ATTACK_SKILL_POWER;	break;
			case BONUS_OPTION_PROTECT_BLOCK_POWER:
			case BONUS_OPTION_PROTECT_BLOCK_POWER_4TH:			BonusOptionData.EffectType = SEED_EFFECT_TYPE_BLOCK_POWER;			break;
			case BONUS_OPTION_PROTECT_MAX_HP_INC:
			case BONUS_OPTION_PROTECT_MAX_HP_INC_4TH:			BonusOptionData.EffectType = SEED_EFFECT_TYPE_MAX_HP_INC;			break;
		}

		memcpy(&m_BonusSocketOptionData[iBonusSocketOptionDataCount], &BonusOptionData, sizeof(BONUS_SOCKET_OPTION));
		iBonusSocketOptionDataCount++;
	}

	pugi::xml_node set_option = main.child("SpecialOptionSettings");

	for (pugi::xml_node option = set_option.child("Option"); option; option = option.next_sibling())
	{
		int iSetSocketOptionIndex = 0;

		BYTE btSetSocketType = 0;
		BYTE btSetSocketOptionValueType = 0;
		BYTE btSetSocketOptionValue = 0;
		int iExtractRate = 0;
		BYTE btSetSocketTypeIndex[6] = {0};

		iSetSocketOptionIndex		= option.attribute("Index").as_int();
		btSetSocketType				= option.attribute("Type").as_int();
		btSetSocketOptionValueType	= option.attribute("BonusType").as_int();
		btSetSocketOptionValue		= option.attribute("BonusValue").as_int();
		iExtractRate				= option.attribute("BonusRate").as_int();
		btSetSocketTypeIndex[0]		= option.attribute("SeedCountFire").as_int();
		btSetSocketTypeIndex[1]		= option.attribute("SeedCountWater").as_int();
		btSetSocketTypeIndex[2]		= option.attribute("SeedCountIce").as_int();
		btSetSocketTypeIndex[3]		= option.attribute("SeedCountWind").as_int();
		btSetSocketTypeIndex[4]		= option.attribute("SeedCountLight").as_int();
		btSetSocketTypeIndex[5]		= option.attribute("SeedCountEarth").as_int();

		m_SetSocketOptionData[iSetSocketOptionDataCount].OptionIndex = iSetSocketOptionIndex;
										
		m_SetSocketOptionData[iSetSocketOptionDataCount].OptionValueType = btSetSocketOptionValueType;
		m_SetSocketOptionData[iSetSocketOptionDataCount].OptionValue = btSetSocketOptionValue;
										
		m_SetSocketOptionData[iSetSocketOptionDataCount].ExtractRate = iExtractRate;
										
		m_SetSocketOptionData[iSetSocketOptionDataCount].SeedTypeTable[0] = btSetSocketTypeIndex[0];
		m_SetSocketOptionData[iSetSocketOptionDataCount].SeedTypeTable[1] = btSetSocketTypeIndex[1];
		m_SetSocketOptionData[iSetSocketOptionDataCount].SeedTypeTable[2] = btSetSocketTypeIndex[2];
		m_SetSocketOptionData[iSetSocketOptionDataCount].SeedTypeTable[3] = btSetSocketTypeIndex[3];
		m_SetSocketOptionData[iSetSocketOptionDataCount].SeedTypeTable[4] = btSetSocketTypeIndex[4];
		m_SetSocketOptionData[iSetSocketOptionDataCount].SeedTypeTable[5] = btSetSocketTypeIndex[5];

		switch (iSetSocketOptionIndex)
		{
			case SET_OPTION_DOUBLE_DAMAGE_RATE_INC:	m_SetSocketOptionData[iSetSocketOptionDataCount].EffectType = SEED_EFFECT_TYPE_SET_DOUBLE_DAMAGE_RATE_INC;	break;
			case SET_OPTION_IGNORE_ENEMY_DEFENSE:	m_SetSocketOptionData[iSetSocketOptionDataCount].EffectType = SEED_EFFECT_TYPE_SET_IGNORE_ENERMY_DEFENSE;	break;
		}

		iSetSocketOptionDataCount++;
	}

	LoadScript();

	return true;
}

void CItemSocketOptionSystem::LoadScript()
{
	m_mapSocketSlotCountPool.clear();
	m_TwoHandWeaponMakeSocketSlotCountPool.InitPool();

	LPSTR pchFileName = g_ConfigRead.GetPath("\\SocketSystem\\IGC_SocketSystem.xml");
	
	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(pchFileName);

	if (res.status != pugi::status_ok)
	{
		g_Log.AddC(TColor::Red, "[Socket System] Failed to load %s file (%s)", pchFileName, res.description());
		g_Log.MsgBox("%s load fail (%s)", pchFileName, res.description());
		return;
	}

	pugi::xml_node main = file.child("SocketSystem");

	m_SphereDropOn = main.attribute("SphereDrop").as_int();
	m_TetraDropOn = main.attribute("TetraDrop").as_int();

	pugi::xml_node sphere_drop = main.child("SphereDrop");
	pugi::xml_node sphere_rate = sphere_drop.child("Rate");
	pugi::xml_node sphere_level = sphere_drop.child("Level"); 

	int iCount = 0;
	for (pugi::xml_attribute rate = sphere_rate.first_attribute(); rate; rate = rate.next_attribute())
	{
		m_SphereDropRate[iCount++] = rate.as_int();
	}

	iCount = 0;
	for (pugi::xml_attribute level = sphere_level.first_attribute(); level; level = level.next_attribute())
	{
		m_SphereDropLevel[iCount++] = level.as_int();
	}

	pugi::xml_node tetra_drop = main.child("TetraDrop");
	pugi::xml_node tetra_bronze = tetra_drop.child("Bronze");
	pugi::xml_node tetra_silver = tetra_drop.child("Silver");
	pugi::xml_node tetra_gold = tetra_drop.child("Gold");

	m_TetraDropRate[0] = tetra_bronze.attribute("Rate").as_int();
	m_TetraDropRate[1] = tetra_silver.attribute("Rate").as_int();
	m_TetraDropRate[2] = tetra_gold.attribute("Rate").as_int();

	m_TetraDropMinLevel[0] = tetra_bronze.attribute("LevelMin").as_int();
	m_TetraDropMinLevel[1] = tetra_silver.attribute("LevelMin").as_int();
	m_TetraDropMinLevel[2] = tetra_gold.attribute("LevelMin").as_int();

	m_TetraDropMaxLevel[0] = tetra_bronze.attribute("LevelMax").as_int();
	m_TetraDropMaxLevel[1] = tetra_silver.attribute("LevelMax").as_int();
	m_TetraDropMaxLevel[2] = tetra_gold.attribute("LevelMax").as_int();

	pugi::xml_node seed_mixes = main.child("Seed");
	pugi::xml_node seedsphere_mixes = seed_mixes.child("Sphere");

	m_RequireMoneyForSeedExtract = seed_mixes.attribute("ExtractMoney").as_int();
	m_RequireMoneyForSeedSphereComposite = seedsphere_mixes.attribute("CompositeMoney").as_int();
	m_RequireMoneyForSeedSphereSet = seedsphere_mixes.attribute("SetMoney").as_int();
	m_RequireMoneyForSeedSphereRemove = seedsphere_mixes.attribute("RemoveMoney").as_int();

	pugi::xml_node socketrate = main.child("SocketRateSettings");

	int ItemCat;
	int SlotRate[5];
	TRandomPoolMgr Pool;
	
	for (pugi::xml_node socket = socketrate.child("Socket"); socket; socket = socket.next_sibling())
	{
		Pool.InitPool();

		ItemCat = socket.attribute("CatID").as_int();
		SlotRate[0] = socket.attribute("Count1").as_int();
		SlotRate[1] = socket.attribute("Count2").as_int();
		SlotRate[2] = socket.attribute("Count3").as_int();
		SlotRate[3] = socket.attribute("Count4").as_int();
		SlotRate[4] = socket.attribute("Count5").as_int();

		for (int i = 0; i < 5; i++)
		{
			Pool.AddValue(i + 1, SlotRate[i]);
		}

		m_mapSocketSlotCountPool.insert(std::pair<int, TRandomPoolMgr>(ItemCat, Pool));
	}

	pugi::xml_node twohand = socketrate.child("TwoHandWeapons");

	SlotRate[0] = twohand.attribute("Count1").as_int();
	SlotRate[1] = twohand.attribute("Count2").as_int();
	SlotRate[2] = twohand.attribute("Count3").as_int();
	SlotRate[3] = twohand.attribute("Count4").as_int();
	SlotRate[4] = twohand.attribute("Count5").as_int();

	for (int i = 0; i < 5; i++)
	{
		m_TwoHandWeaponMakeSocketSlotCountPool.AddValue(i + 1, SlotRate[i]);
	}
}

bool CItemSocketOptionSystem::LoadSocketSlotRateFile(char *pchFileName)
{
	this->m_mapSocketSlotCountPool.clear();
	this->m_TwoHandWeaponMakeSocketSlotCountPool.InitPool();
	
	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(pchFileName);

	if (res.status != pugi::status_ok)
	{
		g_Log.AddC(TColor::Red, "[Socket System] Failed to load %s file (%s)", pchFileName, res.description());
		g_Log.MsgBox("%s load fail (%s)", pchFileName, res.description());
		return false;
	}

	int ItemCat;
	int SlotRate[5];
	TRandomPoolMgr Pool;

	pugi::xml_node main = file.child("SocketRateSettings");
	
	for (pugi::xml_node socket = main.child("Socket"); socket; socket = socket.next_sibling())
	{
		Pool.InitPool();

		ItemCat = socket.attribute("CatID").as_int();
		SlotRate[0] = socket.attribute("Count1").as_int();
		SlotRate[1] = socket.attribute("Count2").as_int();
		SlotRate[2] = socket.attribute("Count3").as_int();
		SlotRate[3] = socket.attribute("Count4").as_int();
		SlotRate[4] = socket.attribute("Count5").as_int();

		for (int i = 0; i < 5; i++)
		{
			Pool.AddValue(i + 1, SlotRate[i]);
		}

		this->m_mapSocketSlotCountPool.insert(std::pair<int, TRandomPoolMgr>(ItemCat, Pool));
	}

	pugi::xml_node twohand = main.child("TwoHandWeapons");

	SlotRate[0] = twohand.attribute("Count1").as_int();
	SlotRate[1] = twohand.attribute("Count2").as_int();
	SlotRate[2] = twohand.attribute("Count3").as_int();
	SlotRate[3] = twohand.attribute("Count4").as_int();
	SlotRate[4] = twohand.attribute("Count5").as_int();

	for (int i = 0; i < 5; i++)
	{
		this->m_TwoHandWeaponMakeSocketSlotCountPool.AddValue(i + 1, SlotRate[i]);
	}
}

bool CItemSocketOptionSystem::IsEnableSocketItem(CItem *lpItemData)
{
	if (lpItemData == NULL)	return false;

	if (lpItemData->IsItem() == FALSE)	return false;

	return IsEnableSocketItem(lpItemData->m_Type);
}

bool CItemSocketOptionSystem::IsEnableSocketItem(int iItemCode)
{
	return GetItemGroup(iItemCode) == ITEMTYPE_SOCKET;
}

BYTE CItemSocketOptionSystem::GetEmptySlotCount(CItem *lpItemData)
{
	BYTE btEmptySlotCount = 0;

	for (int i = 0;i < 5; i++)
	{
		if (lpItemData->m_SocketOption[i] == 0xFE)
		{
			btEmptySlotCount++;
		}
	}

	return btEmptySlotCount;
}

bool CItemSocketOptionSystem::GetSeedSphereEffect(BYTE btSeedSphereIndex, BYTE& pbSeedType, BYTE& pbEffectValueType, WORD& pwEffectValue)
{
	BYTE SeedIndex = 0;
	BYTE SphereLevel = 0;

	CSeedItem* lpSeedData = NULL;
	CSphereItem *lpSphereData = NULL;

	CSeedSphereItem SeedSphere;

	SeedIndex = btSeedSphereIndex % 50;
	SphereLevel = (btSeedSphereIndex - SeedIndex) / 50 + 1;

	lpSeedData = GetSeedData(SeedIndex);
	lpSphereData = GetSphereData(SphereLevel);

	if (lpSeedData == NULL)	return false;

	SeedSphere.SetOption(lpSeedData, SphereLevel);

	pbSeedType = SeedSphere.m_SeedIndex;
	pbEffectValueType = SeedSphere.m_SeedOptionValueType;
	pwEffectValue = SeedSphere.m_SeedOptionValue;
	return true;
}

void CItemSocketOptionSystem::GetSocketOption(CItem *lpItemData, BYTE *pbSocketOption, BYTE& pbSocketBonusOption)
{
	if (lpItemData == NULL)	return;

	if (lpItemData->IsItem() == FALSE || 
		IsEnableSocketItem(lpItemData) == false) return;

	for (int i = 0; i < 5; i++)
	{
		pbSocketOption[i] = lpItemData->m_SocketOption[i];
	}
	pbSocketBonusOption = lpItemData->m_BonusSocketOption;
}

bool CItemSocketOptionSystem::IsSocketItem(CItem *lpItemData)
{
	if (lpItemData == NULL)	return false;

	if (lpItemData->IsItem() == FALSE)	return false;

	if (!IsEnableSocketItem(lpItemData))		return false;
		
	for (int i = 0; i < 5; i++ )
	{
		if (lpItemData->m_SocketOption[i] != 0xFF)	return true;
	}
	
	return false;
}

bool CItemSocketOptionSystem::IsSeedItem(int ItemCode)
{
	switch (ItemCode)
	{
	case ITEMGET(12,60):
	case ITEMGET(12,61):
	case ITEMGET(12,62):
	case ITEMGET(12,63):
	case ITEMGET(12,64):
	case ITEMGET(12,65):
		return true;
	}

	return false;
}

bool CItemSocketOptionSystem::IsSphereItem(int ItemCode)
{
	switch (ItemCode)
	{
	case ITEMGET(12,70):
	case ITEMGET(12,71):
	case ITEMGET(12,72):
	case ITEMGET(12,73):
	case ITEMGET(12,74):
		return true;
	}

	return false;
}

bool CItemSocketOptionSystem::IsSeedSphereItem(int ItemCode)
{
	BYTE btItemType = 0;
	BYTE btItemIndex = 0;

	btItemIndex = ITEM_GET_INDEX(ItemCode);
	btItemType = ITEM_GET_TYPE(ItemCode);

	if (btItemType == 12)
	{
		if  (btItemIndex >= 100 && 
			btItemIndex <= 129)
		{
			return true;
		}
	}

	return false;
}

BYTE CItemSocketOptionSystem::GetSeedIndex(int ItemCode, BYTE ItemLevel)
{
	for (int i = 0; i < 50; i++)
	{
		if (m_SeedData[i].m_ItemCode == ItemCode && 
			m_SeedData[i].m_ItemLevel == ItemLevel)
		{
			return m_SeedData[i].m_SeedIndex;
		}
	}

	return 0xFF;
}

BYTE CItemSocketOptionSystem::GetSphereLevel(int ItemCode)
{
	switch (ItemCode)
	{
	case ITEMGET(12,70):	return 1;	break;
	case ITEMGET(12,71):	return 2;	break;
	case ITEMGET(12,72):	return 3;   break;
	case ITEMGET(12,73):	return 4;	break;
	case ITEMGET(12,74):	return 5;   break;
	}

	return 0;
}

bool CItemSocketOptionSystem::CheckItemForSameSeedType(CItem *lpItemData, WORD SeedSphereItemCode, BYTE ItemLevel)
{
	BYTE btSeedIndex = 0;
	BYTE btSeedSphereIndex = 0;

	if (!IsEnableSocketItem(lpItemData))	return false;
	
	btSeedSphereIndex = GetSeedSphereIndexForItemCode(SeedSphereItemCode, ItemLevel);
	btSeedIndex = btSeedSphereIndex % 50;

	for (int i = 0; i < 5; i++ )
	{
		if (lpItemData->m_SocketOption[i] == 0xFF || 
			lpItemData->m_SocketOption[i] == 0xFE)	continue;

		if ((lpItemData->m_SocketOption[i] % 50) == btSeedIndex)	return false;
	}
	
	return true;
}

BYTE CItemSocketOptionSystem::GetSeedSphereIndexForItemCode(WORD ItemCode, BYTE ItemLevel)
{
	BYTE btSeedType = 0;
	BYTE btSphereLevel = 0;
	BYTE btSeedSphereIndex = 0;

	btSphereLevel = (ITEM_GET_INDEX(ItemCode) - 100) / 6;
	btSeedType = (ITEM_GET_INDEX(ItemCode) - 100) % 6 + 1;

	for (int i = 0; i < 50; i++)
	{
		if (m_SeedData[i].m_SeedType == btSeedType && 
			m_SeedData[i].m_ItemLevel == ItemLevel)
		{
			btSeedSphereIndex = btSphereLevel * 50 + m_SeedData[i].m_SeedIndex;
			return btSeedSphereIndex;
		}
	}

	return 0xFF;
}

DWORD CItemSocketOptionSystem::GetSeedSphereValue(BYTE btSeedSphereIndex)
{
	BYTE btSeedIndex = 0;
	BYTE btSphereLevel = 0;

	CSeedItem *lpSeedData = NULL;
	CSphereItem* lpSphereData = NULL;

	CSeedSphereItem SeedSphere;

	DWORD dwSeedSphereValue = 0;

	btSeedIndex = btSeedSphereIndex % 50;
	btSphereLevel = (btSeedSphereIndex - btSeedIndex) / 50 + 1;

	lpSeedData = GetSeedData(btSeedIndex);

	if (lpSeedData == NULL)	return 0;

	SeedSphere.SetOption(lpSeedData, btSphereLevel);

	dwSeedSphereValue = ItemAttribute[SeedSphere.GetItemCode()].BuyMoney;
	return dwSeedSphereValue;
}

bool CItemSocketOptionSystem::CheckSetSeedSphereItemType(CItem *lpItemData, BYTE btSeedSphereIndex)
{
	if (!IsEnableSocketItem(lpItemData))	return false;

	BYTE btItemType = 0;
	BYTE btItemIndex = 0;
	BYTE btSeedSphereType = 0;

	btItemIndex = ITEM_GET_INDEX(lpItemData->m_Type);
	btItemType = ITEM_GET_TYPE(lpItemData->m_Type);
	btSeedSphereType = GetSocketType(btSeedSphereIndex);

	if (btItemType >= 0 && btItemType <= 5)
	{
		switch (btSeedSphereType)
		{
		case SEED_TYPE_FIRE:
		case SEED_TYPE_ICE:
		case SEED_TYPE_LIGHTNING:
			return true;
		}
	}
	else if (btItemType >= 6 && btItemType <= 11)
	{
		switch (btSeedSphereType)
		{
		case SEED_TYPE_WATER:
		case SEED_TYPE_WIND:
		case SEED_TYPE_EARTH:
			return true;
		}
	}
	else
	{
		return false;
	}

	return false;
}

bool CItemSocketOptionSystem::IsEnableDropSocketItemMap(int iMapIndex)
{
	switch (iMapIndex)
	{
	case MAP_INDEX_RAKLION:
	case MAP_INDEX_HATCHERY:
		return true;
	}

	return false;
}

int CItemSocketOptionSystem::GetSphereDropInfo(int iMonsterLevel)
{
	int iItemCode = -1;
	int iSphereLevel = 0;

	if (m_SphereDropOn != TRUE)	return iItemCode;
	
	if (m_SphereDropLevel[4] != 0 && m_SphereDropLevel[4] <= iMonsterLevel)
	{
		iSphereLevel = 5;
	}
	else if (m_SphereDropLevel[3] != 0 && m_SphereDropLevel[3] <= iMonsterLevel)
	{
		iSphereLevel = 4;
	}
	else if (m_SphereDropLevel[2] != 0 && m_SphereDropLevel[2] <= iMonsterLevel)
	{
		iSphereLevel = 3;
	}
	else if (m_SphereDropLevel[1] != 0 && m_SphereDropLevel[1] <= iMonsterLevel)
	{
		iSphereLevel = 2;
	}
	else if (m_SphereDropLevel[0] != 0 && m_SphereDropLevel[0] <= iMonsterLevel)
	{
		iSphereLevel = 1;
	}
	else return iItemCode;

	for (int i = 0; i < iSphereLevel; i++)
	{
		if (m_SphereDropRate[i] >= GetLargeRand()%10000)
		{
			iItemCode = i + ITEMGET(12,70);
		}
	}

	return iItemCode;
}

int CItemSocketOptionSystem::GetTetraDropInfo(int iMonsterLevel)
{
	int iItemCode = -1;
	int iTetraLevel = 0;

	if (m_TetraDropOn != TRUE)	return iItemCode;

	if (iMonsterLevel >= m_TetraDropMinLevel[2] && iMonsterLevel <= m_TetraDropMaxLevel[2])
	{
		iTetraLevel = 3;
	}

	else if (iMonsterLevel >= m_TetraDropMinLevel[1] && iMonsterLevel <= m_TetraDropMaxLevel[1])
	{
		iTetraLevel = 2;
	}

	else if (iMonsterLevel >= m_TetraDropMinLevel[0] && iMonsterLevel <= m_TetraDropMaxLevel[0])
	{
		iTetraLevel = 1;
	}

	else return iItemCode;

	for (int i = 0; i < iTetraLevel; i++)
	{
		if (m_TetraDropRate[i] >= GetLargeRand()%10000)
		{
			iItemCode = ITEMGET(14,247) - i;
		}
	}

	return iItemCode;
}

BYTE CItemSocketOptionSystem::GetSeedIndexForSeedSpearItemCode(WORD ItemCode, BYTE ItemLevel)
{
	for (int i = 0; i < 50; i++)
	{
		if (this->m_SeedData[i].m_SeedType == ((ITEM_GET_INDEX(ItemCode) - 100) % 6 + 1) &&
			this->m_SeedData[i].m_ItemLevel == ItemLevel)
		{
			return this->m_SeedData[i].m_SeedIndex;
		}
	}

	return -1;
}

BYTE CItemSocketOptionSystem::GetSeedSphereIndex(BYTE btSeedIndex, BYTE btSphereLevel)
{
	return btSphereLevel * 50 + btSeedIndex;
}

bool CItemSocketOptionSystem::MakeSeedSphere(CSeedSphereItem *lpSeedSphere, BYTE btSeedIndex, BYTE btSphereLevel)
{
	CSeedItem* lpSeedData = NULL;
	CSphereItem* lpSphereData = NULL;

	lpSeedData = GetSeedData(btSeedIndex);

	if (lpSeedData == NULL)	return false;

	if (!lpSeedSphere->SetOption(lpSeedData, btSphereLevel))	return false;

	return true;
}

CSeedItem * CItemSocketOptionSystem::GetSeedData(BYTE btSeedIndex)
{
	for (int i = 0; i < 50; i++)
	{
		if (m_SeedData[i].m_SeedIndex == btSeedIndex )
		{
			return &m_SeedData[i];
		}
	}
	
	return NULL;
}

CSphereItem * CItemSocketOptionSystem::GetSphereData(BYTE btSphereLevel)
{
	for (int i = 0; i < 5; i++)
	{
		if (m_SphereData[i].m_SphereLevel == btSphereLevel)
		{
			return &m_SphereData[i];
		}
	}

	return NULL;
}

BYTE CItemSocketOptionSystem::GetExtractSeed()
{
	int iSeedDataIndex = 0;

	iSeedDataIndex = m_SeedExtractPool.GetRandomValue(eRANDOMPOOL_BY_WEIGHT);

	return iSeedDataIndex;
}

BYTE CItemSocketOptionSystem::GetSocketType(BYTE btSeedSphereIndex)
{
	BYTE btSeedIndex = 0;
	CSeedItem* lpSeedData = NULL;

	btSeedIndex = btSeedSphereIndex % 50;

	lpSeedData = GetSeedData(btSeedIndex);

	if (lpSeedData == NULL)	return 0;
	return lpSeedData->m_SeedType;
}

int CItemSocketOptionSystem::GetSeedSphereLevel(BYTE btSeedSphereIndex)
{
	BYTE SeedIndex = btSeedSphereIndex % 50;
	BYTE SphereLevel = (btSeedSphereIndex - SeedIndex) / 50;

	return SphereLevel;
}

BONUS_SOCKET_OPTION * CItemSocketOptionSystem::GetBonusSocketOption(BYTE btBonusSocketOptionIndex)
{
	for (int i = 0 ; i < 12; i++)
	{
		if (m_BonusSocketOptionData[i].OptionIndex == btBonusSocketOptionIndex)
		{
			return &m_BonusSocketOptionData[i];
		}
	}

	return NULL;
}

int CItemSocketOptionSystem::ApplySeedSphereEffect(LPOBJECTSTRUCT lpObj)
{
	int iTotalSeedSphereCount = 0;
	BYTE SeedSphereIndex = 0;
	BYTE SeedIndex = 0;
	BYTE SphereLevel = 0;

	CSeedSphereItem SeedSphere;

	for (int iEquip = 0; iEquip < INVETORY_WEAR_SIZE; iEquip++)
	{
		if (!IsEnableSocketItem(&lpObj->pInventory[iEquip]))	continue;
		
		for (int i = 0; i < 5; i++)
		{
			SeedSphereIndex = lpObj->pInventory[iEquip].m_SocketOption[i];

			if (SeedSphereIndex == 0xFF || 
				SeedSphereIndex == 0xFE)
			{
				continue;
			}

			SeedIndex = SeedSphereIndex % 50;
			SphereLevel = (SeedSphereIndex - SeedIndex ) / 50 + 1;

			if (!MakeSeedSphere(&SeedSphere,SeedIndex,SphereLevel))	continue;
			
			SetEffect(lpObj, SOCKET_OPTION_TYPE_SEED, SeedIndex, SeedSphere.m_SeedOptionValueType, SeedSphere.m_SeedOptionValue);
			iTotalSeedSphereCount++;
		}

	}
	return iTotalSeedSphereCount;
}

void CItemSocketOptionSystem::ApplyBonusOptionEffect(LPOBJECTSTRUCT lpObj)
{
	BYTE btBonusSocketIndex = 0;
	LPBONUS_SOCKET_OPTION lpBonusOptionData = NULL;

	for (int iEquip = 0; iEquip < INVETORY_WEAR_SIZE; iEquip++)
	{
		if (!IsEnableSocketItem(&lpObj->pInventory[iEquip]))	continue;

		btBonusSocketIndex = lpObj->pInventory[iEquip].m_BonusSocketOption;

		if (btBonusSocketIndex == 0xFF || 
			btBonusSocketIndex == 0xFE)
		{
			continue;
		}

		lpBonusOptionData = GetBonusSocketOption(btBonusSocketIndex);
		if (lpBonusOptionData == NULL)	continue;

		g_Log.Add("[SocketItem] [%s][%s] Set Bonus Socket Option - OptionIndex : %d, EffectType : %d, OptionType : %d, OptionValue : %d",lpObj->AccountID, lpObj->Name, lpBonusOptionData->OptionIndex, lpBonusOptionData->EffectType, lpBonusOptionData->OptionValueType, lpBonusOptionData->OptionValue);

		//							EffectType : %d, OptionType : %d, OptionValue : %d
		SetEffect(lpObj, SOCKET_OPTION_TYPE_BONUS, lpBonusOptionData->EffectType, lpBonusOptionData->OptionValueType, lpBonusOptionData->OptionValue);
	}
}

void CItemSocketOptionSystem::ApplySetOptionEffect(LPOBJECTSTRUCT lpObj)
{
	if (lpObj->Type != OBJ_USER)
	{
		return;
	}

	BYTE btTotalSeedTypeList[SEED_TYPE_MAX];
	BYTE btSeedType = SEED_TYPE_NONE;
	BYTE btMatchedCount = 0;

	memset(&btTotalSeedTypeList, 0x00, sizeof(btTotalSeedTypeList));

	for (int iUserSocketIndex = 0; iUserSocketIndex < 35; iUserSocketIndex++)
	{
		btSeedType = GetSocketType(lpObj->m_PlayerData->m_SeedOptionList[iUserSocketIndex].m_SocketOptionIndex);
		if (btSeedType == SEED_TYPE_NONE)	break;

		btTotalSeedTypeList[btSeedType - 1] += lpObj->m_PlayerData->m_SeedOptionList[iUserSocketIndex].m_SocketOptionCount;
	}

	for (int iSetOptionIndex = 0; iSetOptionIndex < 2; iSetOptionIndex++)
	{
		for (int i = 0; i < 6; i++)
		{
			if (m_SetSocketOptionData[iSetOptionIndex].SeedTypeTable[i] <= btTotalSeedTypeList[i])
			{
				btMatchedCount++;
			}
		}

		if (btMatchedCount == 6)
		{
			SetEffect(lpObj, SOCKET_OPTION_TYPE_SET, m_SetSocketOptionData[iSetOptionIndex].EffectType, m_SetSocketOptionData[iSetOptionIndex].OptionValueType, m_SetSocketOptionData[iSetOptionIndex].OptionValue);
		}
		btMatchedCount = 0;
	}
}

bool CItemSocketOptionSystem::SetApplySocketEffect(LPOBJECTSTRUCT lpObj)
{
	int iTotalSeedSphereCount = 0;

	if (lpObj == NULL)	return false;
	
	if (lpObj->Type != OBJ_USER || 
		lpObj->Connected < PLAYER_PLAYING)	return false;

	SetClearSocketEffect(lpObj);

	iTotalSeedSphereCount = ApplySeedSphereEffect(lpObj);
	if (iTotalSeedSphereCount <= 0)	return false;
	
	ApplyBonusOptionEffect(lpObj);
	ApplySetOptionEffect(lpObj);
	return true;
}

void CItemSocketOptionSystem::SetClearSocketEffect(LPOBJECTSTRUCT lpObj)
{
	if (lpObj == NULL)	return;
	
	if (lpObj->Type != OBJ_USER || 
		lpObj->Connected < PLAYER_PLAYING)	return;

	int iIndex = 0;

	for (iIndex = 0;iIndex < 35; iIndex++)
	{
		if (lpObj->m_PlayerData->m_SeedOptionList[iIndex].m_SocketOptionIndex == 0xFF)	break;
		lpObj->m_PlayerData->m_SeedOptionList[iIndex].Clear();
	}

	for (iIndex = 0;iIndex < 7; iIndex++)
	{
		if (lpObj->m_PlayerData->m_BonusOptionList[iIndex].m_SocketOptionIndex == 0xFF)	break;
		lpObj->m_PlayerData->m_BonusOptionList[iIndex].Clear();
	}

	for (iIndex = 0;iIndex < 2; iIndex++)
	{
		if (lpObj->m_PlayerData->m_SetOptionList[iIndex].m_SocketOptionIndex == 0xFF)	break;
		lpObj->m_PlayerData->m_SetOptionList[iIndex].Clear();
	}

	lpObj->m_PlayerData->m_btRefillHPSocketOption = 0;
	lpObj->m_PlayerData->m_btRefillMPSocketOption = 0;
	lpObj->m_PlayerData->m_wSocketOptionMonsterDieGetHp = 0;
	lpObj->m_PlayerData->m_wSocketOptionMonsterDieGetMana = 0;
	lpObj->m_PlayerData->m_btAGReduceRate = 0;
}

void CItemSocketOptionSystem::SetEffect(LPOBJECTSTRUCT lpObj, BYTE btOptionType, BYTE btEffectType, BYTE btEffectValueType, WORD wEffectValue)
{
	if (btEffectType >= 0 && btEffectType <= 9)
	{
		_SetFireEffect(lpObj, btEffectType, btEffectValueType, wEffectValue);
	}
	else if (btEffectType >= 10 && btEffectType <= 15)
	{
		_SetWaterEffect(lpObj, btEffectType, btEffectValueType, wEffectValue);
	}
	else if (btEffectType >= 16 && btEffectType <= 20)
	{
		_SetIceEffect(lpObj, btEffectType, btEffectValueType, wEffectValue);
	}
	else if (btEffectType >= 21 && btEffectType <= 28)
	{
		_SetWindEffect(lpObj, btEffectType, btEffectValueType, wEffectValue);
	}
	else if (btEffectType >= 29 && btEffectType <= 33)
	{
		_SetThunderEffect(lpObj, btEffectType, btEffectValueType, wEffectValue);
	}
	else if (btEffectType >= 34 && btEffectType <= 40)
	{
		_SetEarthEffect(lpObj, btEffectType, btEffectValueType, wEffectValue);
	}
	else if (btEffectType >= 41 && btEffectType <= 43)
	{
		_SetUniqueEffect(lpObj, btEffectType, btEffectValueType, wEffectValue);
	}
	else if (btEffectType >= 44 && btEffectType <= 45)
	{
		_SetSetEffect(lpObj, btEffectType, btEffectValueType, wEffectValue);
	}

	for (int i = 0; i < 43; i++) //{Nice WebZen =D}
	{
		if (lpObj->m_PlayerData->m_SeedOptionList[i].m_SocketOptionIndex == btEffectType &&
			lpObj->m_PlayerData->m_SeedOptionList[i].m_SocketOptionValueType == btEffectValueType)
		{
			lpObj->m_PlayerData->m_SeedOptionList[i].m_SocketOptionValue += wEffectValue;
			lpObj->m_PlayerData->m_SeedOptionList[i].m_SocketOptionCount++;
			break;
		}

		if (lpObj->m_PlayerData->m_SeedOptionList[i].m_SocketOptionIndex == 0xFF)
		{
			lpObj->m_PlayerData->m_SeedOptionList[i].m_SocketOptionIndex = btEffectType;
			lpObj->m_PlayerData->m_SeedOptionList[i].m_SocketOptionValueType = btEffectValueType;
			lpObj->m_PlayerData->m_SeedOptionList[i].m_SocketOptionValue = wEffectValue;
			lpObj->m_PlayerData->m_SeedOptionList[i].m_SocketOptionCount = 1;
			break;
		}
	}
}

void CItemSocketOptionSystem::ClearEffect(LPOBJECTSTRUCT lpObj, LPSOCKET_OPTION_LIST lpOptionData)
{
	//empty, possible due to "return;" in the beggining
}

void CItemSocketOptionSystem::_SetFireEffect(LPOBJECTSTRUCT lpObj, BYTE btEffectType, BYTE btEffectValueType, WORD wEffectValue)
{
	int iCalcEffectValue = 0;

	switch (btEffectType)
	{
	case SEED_EFFECT_TYPE_ATTACK_POWER:
		{
			iCalcEffectValue = CalcEffectValue(lpObj->m_AttackDamageMaxLeft, wEffectValue, btEffectValueType);
			lpObj->m_AttackDamageMaxLeft += iCalcEffectValue;

			iCalcEffectValue = CalcEffectValue(lpObj->m_AttackDamageMinLeft, wEffectValue, btEffectValueType);
			lpObj->m_AttackDamageMinLeft += iCalcEffectValue;

			iCalcEffectValue = CalcEffectValue(lpObj->m_AttackDamageMaxRight, wEffectValue, btEffectValueType);
			lpObj->m_AttackDamageMaxRight += iCalcEffectValue;

			iCalcEffectValue = CalcEffectValue(lpObj->m_AttackDamageMinRight, wEffectValue, btEffectValueType);
			lpObj->m_AttackDamageMinRight += iCalcEffectValue;
			
			iCalcEffectValue = CalcEffectValue(lpObj->m_MagicDamageMin, wEffectValue, btEffectValueType);
			lpObj->m_MagicDamageMin += iCalcEffectValue;

			iCalcEffectValue = CalcEffectValue(lpObj->m_MagicDamageMax, wEffectValue, btEffectValueType);
			lpObj->m_MagicDamageMax += iCalcEffectValue;
		}
		break;
	case SEED_EFFECT_TYPE_ATTACK_SPEED:
		{
			iCalcEffectValue = CalcEffectValue(lpObj->m_AttackSpeed, wEffectValue, btEffectValueType);
			lpObj->m_AttackSpeed += iCalcEffectValue;
			
			iCalcEffectValue = CalcEffectValue(lpObj->m_MagicSpeed, wEffectValue, btEffectValueType);
			lpObj->m_MagicSpeed += iCalcEffectValue;
		}
		break;
	case SEED_EFFECT_TYPE_MAX_ATTACK_POWER:
		{
			iCalcEffectValue = CalcEffectValue(lpObj->m_AttackDamageMaxLeft, wEffectValue, btEffectValueType);
			lpObj->m_AttackDamageMaxLeft += iCalcEffectValue;
			
			iCalcEffectValue = CalcEffectValue(lpObj->m_AttackDamageMaxRight, wEffectValue, btEffectValueType);
			lpObj->m_AttackDamageMaxRight += iCalcEffectValue;
			
			iCalcEffectValue = CalcEffectValue(lpObj->m_MagicDamageMax, wEffectValue, btEffectValueType);
			lpObj->m_MagicDamageMax += iCalcEffectValue;
		}
		break;
	case SEED_EFFECT_TYPE_MIN_ATTACK_POWER:
		{
			iCalcEffectValue = CalcEffectValue(lpObj->m_AttackDamageMinLeft, wEffectValue, btEffectValueType);
			lpObj->m_AttackDamageMinLeft += iCalcEffectValue;
			
			iCalcEffectValue = CalcEffectValue(lpObj->m_AttackDamageMinRight, wEffectValue, btEffectValueType);
			lpObj->m_AttackDamageMinRight += iCalcEffectValue;
			
			iCalcEffectValue = CalcEffectValue(lpObj->m_MagicDamageMin, wEffectValue, btEffectValueType);
			lpObj->m_MagicDamageMin += iCalcEffectValue;
		}
		break;
	case SEED_EFFECT_TYPE_MIN_MAX_ATTACK_POWER:
		{
			iCalcEffectValue = CalcEffectValue(lpObj->m_AttackDamageMaxLeft, wEffectValue, btEffectValueType);
			lpObj->m_AttackDamageMaxLeft += iCalcEffectValue;
			
			iCalcEffectValue = CalcEffectValue(lpObj->m_AttackDamageMinLeft, wEffectValue, btEffectValueType);
			lpObj->m_AttackDamageMinLeft += iCalcEffectValue;
			
			iCalcEffectValue = CalcEffectValue(lpObj->m_AttackDamageMaxRight, wEffectValue, btEffectValueType);
			lpObj->m_AttackDamageMaxRight += iCalcEffectValue;
			
			iCalcEffectValue = CalcEffectValue(lpObj->m_AttackDamageMinRight, wEffectValue, btEffectValueType);
			lpObj->m_AttackDamageMinRight += iCalcEffectValue;
			
			iCalcEffectValue = CalcEffectValue(lpObj->m_MagicDamageMin, wEffectValue, btEffectValueType);
			lpObj->m_MagicDamageMin += iCalcEffectValue;
			
			iCalcEffectValue = CalcEffectValue(lpObj->m_MagicDamageMax, wEffectValue, btEffectValueType);
			lpObj->m_MagicDamageMax += iCalcEffectValue;
		}
		break;
	case SEED_EFFECT_TYPE_REDUCE_AG_RATE:
		{
			lpObj->m_PlayerData->m_btAGReduceRate += wEffectValue;
			
			if (lpObj->m_PlayerData->m_btAGReduceRate > 100)
			{
				lpObj->m_PlayerData->m_btAGReduceRate = 100;
			}
		}
		break;
	}
}

void CItemSocketOptionSystem::_SetWaterEffect(LPOBJECTSTRUCT lpObj, BYTE btEffectType, BYTE btEffectValueType, WORD wEffectValue)
{
	int iCalcEffectValue = 0;

	switch (btEffectType)
	{
	case SEED_EFFECT_TYPE_SUCCESS_BLOCK:
		{
			iCalcEffectValue = CalcEffectValue(lpObj->m_SuccessfulBlocking, wEffectValue, btEffectValueType);
			lpObj->m_SuccessfulBlocking += iCalcEffectValue;
		}
		break;
	case SEED_EFFECT_TYPE_BLOCK_POWER:
		{
			iCalcEffectValue = CalcEffectValue(lpObj->m_Defense, wEffectValue, btEffectValueType);
			lpObj->m_Defense += iCalcEffectValue;

			iCalcEffectValue = CalcEffectValue(lpObj->m_MagicDefense, wEffectValue, btEffectValueType);
			lpObj->m_MagicDefense += iCalcEffectValue;
		}
		break;
	case SEED_EFFECT_TYPE_SHIELD_BLOCK_POWER:
		{
			lpObj->m_PlayerData->SetOpImproveSheldDefence += wEffectValue;
		}
		break;
	case SEED_EFFECT_TYPE_REDUCE_DAMAGE:
		{
			lpObj->DamageMinus += wEffectValue;
		}
		break;
	case SEED_EFFECT_TYPE_REFLECT_DAMAGE:
		{
			lpObj->DamageReflect += wEffectValue;
		}
		break;
	}
}

void CItemSocketOptionSystem::_SetIceEffect(LPOBJECTSTRUCT lpObj, BYTE btEffectType, BYTE btEffectValueType, WORD wEffectValue)
{
	int iAddValue = 0;

	switch (btEffectType)
	{
	case SEED_EFFECT_TYPE_MONSTER_KILL_REFILL_HP:
		{
			iAddValue += CalcEffectValue(lpObj->MaxLife, wEffectValue, btEffectValueType);
			lpObj->m_PlayerData->m_wSocketOptionMonsterDieGetHp += iAddValue;
		}
		break;
	case SEED_EFFECT_TYPE_MONSTER_KILL_REFILL_MP:
		{
			iAddValue += CalcEffectValue(lpObj->MaxMana, wEffectValue, btEffectValueType);
			lpObj->m_PlayerData->m_wSocketOptionMonsterDieGetMana += iAddValue;
		}
		break;
	case SEED_EFFECT_TYPE_ATTACK_SKILL_POWER:
		{
			iAddValue = CalcEffectValue(lpObj->m_PlayerData->SetOpAddSkillAttack, wEffectValue, btEffectValueType);
			lpObj->m_PlayerData->SetOpAddSkillAttack += iAddValue;
		}
		break;
	case SEED_EFFECT_TYPE_ATTACK_ACCURACY:
		{
			lpObj->m_AttackRating += wEffectValue;
		}
		break;

	}
}

void CItemSocketOptionSystem::_SetWindEffect(LPOBJECTSTRUCT lpObj, BYTE btEffectType, BYTE btEffectValueType, WORD wEffectValue)
{
	int iAddValue = 0;

	switch (btEffectType)
	{
	case SEED_EFFECT_TYPE_REFILL_HP_INC:
		{
			iAddValue = CalcEffectValue(lpObj->MaxLife, wEffectValue, btEffectValueType);
			lpObj->m_PlayerData->m_btRefillHPSocketOption += iAddValue;
		}
		break;
	case SEED_EFFECT_TYPE_MAX_HP_INC:
		{
			iAddValue = CalcEffectValue(lpObj->MaxLife, wEffectValue, btEffectValueType);
			lpObj->AddLife += iAddValue;
			GSProtocol.GCReFillSend(lpObj->m_Index,(lpObj->AddLife+lpObj->MaxLife),0xFE,0,(lpObj->iMaxShield+lpObj->iAddShield));
			GSProtocol.GCReFillSend(lpObj->m_Index,lpObj->Life,0xFF,0,lpObj->iShield);
		}
		break;
	case SEED_EFFECT_TYPE_MAX_MP_INC:
		{
			iAddValue = CalcEffectValue(lpObj->MaxMana, wEffectValue, btEffectValueType);
			lpObj->AddMana += iAddValue;
			GSProtocol.GCManaSend(lpObj->m_Index,(lpObj->AddMana+lpObj->MaxMana),0xFE,0,(lpObj->MaxBP+lpObj->AddBP));
			GSProtocol.GCManaSend(lpObj->m_Index,lpObj->Mana,0xFF,0,lpObj->BP);
		}
		break;
	case SEED_EFFECT_TYPE_REFILL_MP_INC:
		{
			iAddValue = CalcEffectValue(lpObj->MaxMana, wEffectValue, btEffectValueType);
			lpObj->m_PlayerData->m_btRefillMPSocketOption += iAddValue;
		}
		break;
	case SEED_EFFECT_TYPE_MAX_AG_INC:
		{
			iAddValue = CalcEffectValue(lpObj->MaxBP, wEffectValue, btEffectValueType);
			lpObj->AddBP += iAddValue;
			GSProtocol.GCManaSend(lpObj->m_Index,(lpObj->AddMana+lpObj->MaxMana),0xFE,0,(lpObj->MaxBP+lpObj->AddBP));
			GSProtocol.GCManaSend(lpObj->m_Index,lpObj->Mana,0xFF,0,lpObj->BP);
		}
		break;
	case SEED_EFFECT_TYPE_REFILL_AG_INC:
		{
			iAddValue = CalcEffectValue(lpObj->m_PlayerData->SetOpIncAGValue, wEffectValue, btEffectValueType);
			lpObj->m_PlayerData->SetOpIncAGValue += iAddValue;
		}
		break;
	case SEED_EFFECT_TYPE_MONSTER_KILL_ZEN_INC:
		{
			lpObj->MonsterDieGetMoney += wEffectValue;
		}
		break;
	}
}

void CItemSocketOptionSystem::_SetThunderEffect(LPOBJECTSTRUCT lpObj, BYTE btEffectType, BYTE btEffectValueType, WORD wEffectValue)
{
	int iAddValue = 0;

	switch (btEffectType)
	{
	case SEED_EFFECT_TYPE_EXCELLENT_DAMAGE_INC:
		{
			iAddValue = CalcEffectValue(lpObj->m_PlayerData->SetOpAddExDamage, wEffectValue, btEffectValueType);
			lpObj->m_PlayerData->SetOpAddExDamage += iAddValue;
		}
		break;
	case SEED_EFFECT_TYPE_EXCELLENT_DAMAGE_RATE_INC:
		{
			lpObj->m_ExcelentDamage += wEffectValue;
		}
		break;
	case SEED_EFFECT_TYPE_CRITICAL_DAMAGE_INC:
		{
			iAddValue = CalcEffectValue(lpObj->m_PlayerData->SetOpAddCriticalDamage, wEffectValue, btEffectValueType);
			lpObj->m_PlayerData->SetOpAddCriticalDamage += iAddValue;
		}
		break;
	case SEED_EFFECT_TYPE_CRITICAL_DAMAGE_RATE_INC:
		{
			lpObj->m_CriticalDamage += wEffectValue;
		}
		break;
	}
}

void CItemSocketOptionSystem::_SetEarthEffect(LPOBJECTSTRUCT lpObj, BYTE btEffectType, BYTE btEffectValueType, WORD wEffectValue)
{
	if (lpObj->Type != OBJ_USER)
	{
		return;
	}

	int iAddValue = 0;

	switch (btEffectType)
	{
	case SEED_EFFECT_TYPE_STRENGTH_INC:
		{
			iAddValue = CalcEffectValue(lpObj->m_PlayerData->Strength + lpObj->AddStrength, wEffectValue, btEffectValueType);
			lpObj->AddStrength += iAddValue;
		}
		break;
	case SEED_EFFECT_TYPE_DEXTERITY_INC:
		{
			iAddValue = CalcEffectValue(lpObj->m_PlayerData->Dexterity + lpObj->AddDexterity, wEffectValue, btEffectValueType);

			EnterCriticalSection(&lpObj->m_PlayerData->AgiCheckCriti);
			lpObj->m_PlayerData->AgilityCheckDelay = GetTickCount();
			lpObj->AddDexterity += iAddValue;
			LeaveCriticalSection(&lpObj->m_PlayerData->AgiCheckCriti);
		}
		break;
	case SEED_EFFECT_TYPE_VITALITY_INC:
		{
			iAddValue = CalcEffectValue(lpObj->m_PlayerData->Vitality + lpObj->AddVitality, wEffectValue, btEffectValueType);
			lpObj->AddVitality += iAddValue;
		}
		break;
	case SEED_EFFECT_TYPE_ENERGY_INC:
		{
			iAddValue = CalcEffectValue(lpObj->m_PlayerData->Energy + lpObj->AddEnergy, wEffectValue, btEffectValueType);
			lpObj->AddEnergy += iAddValue;
		}
		break;
	}
}

void CItemSocketOptionSystem::_SetUniqueEffect(LPOBJECTSTRUCT lpObj, BYTE btEffectType, BYTE btEffectValueType, WORD wEffectValue)
{
	//int iAddValue = btEffectType;

	switch (btEffectType)
	{
	case SEED_EFFECT_TYPE_UNIQUE_1:
		{
		}
		break;
	case SEED_EFFECT_TYPE_UNIQUE_2:
		{
		}
		break;
	}
}

void CItemSocketOptionSystem::_SetSetEffect(LPOBJECTSTRUCT lpObj, BYTE btEffectType, BYTE btEffectValueType, WORD wEffectValue)
{
	int iCalcEffectValue = 0;

	switch (btEffectType)
	{
	case SEED_EFFECT_TYPE_SET_DOUBLE_DAMAGE_RATE_INC:
		{
			lpObj->m_PlayerData->SetOpDoubleDamage += wEffectValue;
		}
		break;
	case SEED_EFFECT_TYPE_SET_IGNORE_ENERMY_DEFENSE:
		{
			lpObj->m_PlayerData->SetOpIgnoreDefense += wEffectValue;
		}
		break;
	}
}

int CItemSocketOptionSystem::CalcEffectValue(int iInputValue, WORD wEffectValue, BYTE btEffectValueType)
{
	int iReturnValue = 0;

	if (wEffectValue == 0)	return 0;

	switch (btEffectValueType)
	{
		case SEED_OPTION_TYPE_VALUE:		iReturnValue = wEffectValue;						break;
		case SEED_OPTION_TYPE_RATE:			iReturnValue = iInputValue * wEffectValue / 100;	break;
		case SEED_OPTION_TYPE_PER_LEVEL:	iReturnValue = iInputValue / wEffectValue;			break;
		case SEED_OPTION_TYPE_PER_HP:		iReturnValue = iInputValue / wEffectValue;			break;
		case SEED_OPTION_TYPE_PER_MP:		iReturnValue = iInputValue / wEffectValue;			break;
		default:	return -1;	break;
	}

	return iReturnValue;
}

void CItemSocketOptionSystem::SetRequireStatOption(CItem *lpItemdata)
{
	if (lpItemdata == NULL)	return;

	if (lpItemdata->IsItem() == FALSE || 
		!IsSocketItem(lpItemdata))	return;

	BYTE SeedIndex = 0;
	BYTE SphereLevel = 0;
	BYTE SeedSphereIndex = 0;

	CSeedSphereItem SeedSphere;

	int iReduceValue = 0;

	for (int i = 0; i < 5; i++)
	{
		if (lpItemdata->m_SocketOption[i] == 0xFF || 
			lpItemdata->m_SocketOption[i] == 0xFE)
		{
			continue;
		}

		SeedSphereIndex = lpItemdata->m_SocketOption[i];
		SeedIndex = SeedSphereIndex % 50;
		SphereLevel = (SeedSphereIndex - SeedIndex) / 50 + 1;

		if (!MakeSeedSphere(&SeedSphere,SeedIndex,SphereLevel))	continue;

		if (SeedIndex == SEED_EFFECT_TYPE_REQUIRE_STRENGTH_DEC)
		{
			iReduceValue = CalcEffectValue(lpItemdata->m_HJOpStrength, SeedSphere.m_SeedOptionValue, SeedSphere.m_SeedOptionValueType);
			lpItemdata->m_HJOpStrength += iReduceValue;
		}
		else if (SeedIndex == SEED_EFFECT_TYPE_REQUIRE_DEXTERITY_DEC)
		{
			iReduceValue = CalcEffectValue(lpItemdata->m_HJOpDexterity, SeedSphere.m_SeedOptionValue, SeedSphere.m_SeedOptionValueType);
			lpItemdata->m_HJOpDexterity += iReduceValue;
		}
		else if (SeedIndex == SEED_EFFECT_TYPE_ITEM_DURABILITY_IMPROVE)
		{
			lpItemdata->m_ImproveDurabilityRate += SeedSphere.m_SeedOptionValue;
		}
	}
}

void CItemSocketOptionSystem::MakeSocketSlot(CItem *lpItemData, BYTE btSocketSlotCount)
{
	lpItemData->MakeSocketSlot(btSocketSlotCount);
}

BYTE CItemSocketOptionSystem::GetMakeSocketSlotCount(int iItemCode)
{
	int iSocketSlotCount = 0;

	if (ItemAttribute[iItemCode].HaveItemInfo == 1)
	{
		if (ItemAttribute[iItemCode].TwoHand == TRUE)
		{
			iSocketSlotCount = this->m_TwoHandWeaponMakeSocketSlotCountPool.GetRandomValue(eRANDOMPOOL_BY_WEIGHT);
		}

		else
		{
			int ItemType = ITEM_GET_TYPE(iItemCode);

			std::map<int, TRandomPoolMgr>::iterator It = this->m_mapSocketSlotCountPool.find(ItemType);

			if (It == this->m_mapSocketSlotCountPool.end())
			{
				return iSocketSlotCount;
			}

			iSocketSlotCount = It->second.GetRandomValue(eRANDOMPOOL_BY_WEIGHT);
		}
	}

	return iSocketSlotCount;
}

void CItemSocketOptionSystem::ClearUserData(LPOBJECTSTRUCT lpObj)
{
	int iIndex = 0;

	if (lpObj == NULL)	return;
	if (lpObj->Type != OBJ_USER)	return;

	for (iIndex = 0; iIndex < 35; iIndex++)	lpObj->m_PlayerData->m_SeedOptionList[iIndex].Clear();
	for (iIndex = 0; iIndex < 7; iIndex++)	lpObj->m_PlayerData->m_BonusOptionList[iIndex].Clear();
	for (iIndex = 0; iIndex < 2; iIndex++)	lpObj->m_PlayerData->m_SetOptionList[iIndex].Clear();
}

bool CItemSocketOptionSystem::SetSocketSlot(CItem *lpTargetItem, CItem *lpSeedSphere, BYTE btSocketSlotIndex)
{
	if (lpTargetItem == NULL || lpSeedSphere == NULL)	return false;
	if (lpTargetItem->IsItem() == FALSE || lpSeedSphere->IsItem() == FALSE )	return false;
	if (!IsEnableSocketItem(lpTargetItem))	return false;
	if (btSocketSlotIndex > 5)	return false;
	if (lpTargetItem->m_SocketOption[btSocketSlotIndex] != 0xFE)	return false;

	BYTE btItemIndex = 0;
	BYTE btSeedIndex = 0;
	BYTE btSphereLevel = 0;
	BYTE btSeedSphereIndex = 0;

	btItemIndex = ITEM_GET_INDEX(lpSeedSphere->m_Type);
	btSeedIndex = btItemIndex - 99;
	btSphereLevel = (btSeedIndex - 1) / 6 + 1;
	btSeedSphereIndex = btSphereLevel * 50 + btSeedIndex;

	lpTargetItem->m_SocketOption[btSocketSlotIndex] = btSeedSphereIndex;
	SetBonusSocketOption(lpTargetItem);
	return true;
}

bool CItemSocketOptionSystem::SetSocketSlot(CItem *lpTargetItem, BYTE btSeedIndex, BYTE btSphereLevel, BYTE btSocketSlotIndex)
{
	if (lpTargetItem == NULL)	return false;
	if (lpTargetItem->IsItem() == FALSE)	return false;
	if (IsEnableSocketItem(lpTargetItem) == false)	return false;
	if (btSocketSlotIndex > 5)	return false;
	if (lpTargetItem->m_SocketOption[btSocketSlotIndex] != 0xFE)	return false;

	BYTE btSeedSphereIndex = 0;
	btSeedSphereIndex = 50 * btSphereLevel + btSeedIndex;
	lpTargetItem->m_SocketOption[btSocketSlotIndex] = btSeedSphereIndex;
	SetBonusSocketOption(lpTargetItem);
	return true;
}

bool CItemSocketOptionSystem::SetSocketSlot(CItem *lpTargetItem, BYTE btSeedSphereIndex, BYTE btSocketSlotIndex)
{
	if (lpTargetItem == NULL)	return false;
	if (lpTargetItem->IsItem() == FALSE)	return false;
	if (IsEnableSocketItem(lpTargetItem) == false)	return false;
	if (btSocketSlotIndex > 5)	return false;
	if (lpTargetItem->m_SocketOption[btSocketSlotIndex] != 0xFE)	return false;

	lpTargetItem->m_SocketOption[btSocketSlotIndex] = btSeedSphereIndex;
	SetBonusSocketOption(lpTargetItem);
	return true;
}

bool CItemSocketOptionSystem::SetBonusSocketOption(CItem *lpItemData)
{
	BYTE SeedSphereType[5];

	if (lpItemData == NULL)	return false;
	if (lpItemData->IsItem() == FALSE || IsEnableSocketItem(lpItemData) == false)	return false;

	int nSeedSphere4thCount = 0;

	for (int iSlotIndex = 0; iSlotIndex < 5; iSlotIndex++)
	{
		if (lpItemData->m_SocketOption[iSlotIndex] != 0xFF && 
			lpItemData->m_SocketOption[iSlotIndex] != 0xFE)
		{
			SeedSphereType[iSlotIndex] = GetSocketType(lpItemData->m_SocketOption[iSlotIndex]);

			if (this->GetSeedSphereLevel(lpItemData->m_SocketOption[iSlotIndex]) == 3 && iSlotIndex < 3)
			{
				nSeedSphere4thCount++;
			}
		}
	}

	BYTE btItemType = 0;
	WORD wItemIndex = 0;
	BYTE btSocketSlotCount = 0;

	wItemIndex = ITEM_GET_INDEX(lpItemData->m_Type);
	btItemType = ITEM_GET_TYPE(lpItemData->m_Type);

	for (int iBonusIndex = 0; iBonusIndex < 6; iBonusIndex++)
	{
		if (btItemType >= m_BonusSocketOptionData[iBonusIndex].ItemTypeBegin && 
			btItemType <= m_BonusSocketOptionData[iBonusIndex].ItemTypeEnd)
		{
			for (int iSlotIndex = 0;iSlotIndex < 5; iSlotIndex++)
			{
				if (m_BonusSocketOptionData[iBonusIndex].SocketSlotType[iSlotIndex] == SeedSphereType[iSlotIndex] && SeedSphereType[iSlotIndex] != NULL)
				{
					btSocketSlotCount++;
				}

				if (btSocketSlotCount >= m_BonusSocketOptionData[iBonusIndex].SocketSlotCount)
				{
					if (rand() % 100 < m_BonusSocketOptionData[iBonusIndex].ExtractRate)
					{
						lpItemData->m_BonusSocketOption = m_BonusSocketOptionData[iBonusIndex].OptionIndex;
						g_Log.Add("[SocketItem] Set Bonus Socket Option Success - OptionIndex : %d, ItemCode : %d, SocketOption[%d,%d,%d,%d,%d]", lpItemData->m_BonusSocketOption, lpItemData->m_Type, lpItemData->m_SocketOption[0], lpItemData->m_SocketOption[1], lpItemData->m_SocketOption[2], lpItemData->m_SocketOption[3], lpItemData->m_SocketOption[4]);
						return true;
					}
					
					lpItemData->m_BonusSocketOption = 0xFF;
					
					if (nSeedSphere4thCount != 3)
					{				
						return false;
					}
				}
			}

			btSocketSlotCount = 0;
		}
	}

	if ( nSeedSphere4thCount == 3 )
	{
		for (int iBonusIndex = 6; iBonusIndex < 12; iBonusIndex++)
		{
			if (btItemType >= m_BonusSocketOptionData[iBonusIndex].ItemTypeBegin && 
				btItemType <= m_BonusSocketOptionData[iBonusIndex].ItemTypeEnd)
			{
				for (int iSlotIndex = 0;iSlotIndex < 5; iSlotIndex++)
				{
					if (m_BonusSocketOptionData[iBonusIndex].SocketSlotType[iSlotIndex] == SeedSphereType[iSlotIndex] && SeedSphereType[iSlotIndex] != NULL)
					{
						btSocketSlotCount++;
					}

					if (btSocketSlotCount >= m_BonusSocketOptionData[iBonusIndex].SocketSlotCount)
					{
						if (rand() % 100 < m_BonusSocketOptionData[iBonusIndex].ExtractRate)
						{
							lpItemData->m_BonusSocketOption = m_BonusSocketOptionData[iBonusIndex].OptionIndex;
							g_Log.Add("[SocketItem] Set Bonus SocketEx Option Success - OptionIndex : %d, ItemCode : %d, SocketOption[%d,%d,%d,%d,%d]", lpItemData->m_BonusSocketOption, lpItemData->m_Type, lpItemData->m_SocketOption[0], lpItemData->m_SocketOption[1], lpItemData->m_SocketOption[2], lpItemData->m_SocketOption[3], lpItemData->m_SocketOption[4]);
							return true;
						}
					
						lpItemData->m_BonusSocketOption = 0xFF;		
						return false;
					}
				}

				btSocketSlotCount = 0;
			}
		}
	}

	lpItemData->m_BonusSocketOption = 0xFF;
	return false;
}

bool CItemSocketOptionSystem::ClearSocketSlot(CItem* lpTargetItem, BYTE btSocketSlotIndex)
{
	if (lpTargetItem == NULL)	return false;
	if (IsEnableSocketItem(lpTargetItem) == false)	return false;
	if (btSocketSlotIndex > 5)	return false;

	if (lpTargetItem->m_SocketOption[btSocketSlotIndex] == 0xFF || 
		lpTargetItem->m_SocketOption[btSocketSlotIndex] == 0xFE)
	{
		return false;
	}

	lpTargetItem->m_SocketOption[btSocketSlotIndex] = 0xFE;
	SetBonusSocketOption(lpTargetItem);
	return true;
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


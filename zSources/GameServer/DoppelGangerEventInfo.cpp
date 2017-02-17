//////////////////////////////////////////////////////////////////////
// DoppelGangerEventInfo.cpp
#include "stdafx.h"
#include "DoppelGangerEventInfo.h"
#include "TLog.h"

CDoppelGangerEventInfo::CDoppelGangerEventInfo(void)
{
}

CDoppelGangerEventInfo::~CDoppelGangerEventInfo(void)
{
}

void CDoppelGangerEventInfo::LoadDoppelgangerEventInfo(LPSTR lpFile)
{
	this->ClearDoppelgangerEventInfo();
	this->m_bLoad = FALSE;

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(lpFile);

	if ( res.status != pugi::status_ok )
	{
		g_Log.MsgBox("%s load fail (%s)", lpFile, res.description());
		return;
	}

	_tagDOPPELGANGER_EVENT_INFO m_DGInfo;

	pugi::xml_node main = file.child("DoppelGanger");

	for (pugi::xml_node range = main.child("Range"); range; range = range.next_sibling())
	{
		m_DGInfo.nUserMaxLevel = range.attribute("MaxPlayerLevel").as_int();

		pugi::xml_node monster_level = range.child("MonsterLevel");

		for (pugi::xml_node player = monster_level.child("Player"); player; player = player.next_sibling())
		{
			int iCount = player.attribute("Count").as_int() - 1;

			if (iCount < 0 || iCount >= 5)
			{
				continue;
			}

			m_DGInfo.fMonsterLevel[iCount] = player.attribute("Multiplier").as_float();
		}

		pugi::xml_node monster_hp = range.child("MonsterHP");

		for (pugi::xml_node player = monster_hp.child("Player"); player; player = player.next_sibling())
		{
			int iCount = player.attribute("Count").as_int() - 1;

			if (iCount < 0 || iCount >= 5)
			{
				continue;
			}

			m_DGInfo.fMonsterHp[iCount] = player.attribute("Multiplier").as_float();
		}

		pugi::xml_node monster_damage = range.child("MonsterDamage");

		for (pugi::xml_node player = monster_damage.child("Player"); player; player = player.next_sibling())
		{
			int iCount = player.attribute("Count").as_int() - 1;

			if (iCount < 0 || iCount >= 5)
			{
				continue;
			}

			m_DGInfo.fMonsterAttack[iCount] = player.attribute("Multiplier").as_float();
		}

		pugi::xml_node monster_defense = range.child("MonsterDefense");

		for (pugi::xml_node player = monster_damage.child("Player"); player; player = player.next_sibling())
		{
			int iCount = player.attribute("Count").as_int() - 1;

			if (iCount < 0 || iCount >= 5)
			{
				continue;
			}

			m_DGInfo.fMonsterDefense[iCount] = player.attribute("Multiplier").as_float();
		}

		pugi::xml_node interim_chest = range.child("InterimChest");

		m_DGInfo.nMiddleRewardItemDropCnt = interim_chest.attribute("ItemDropCount").as_int();

		for (pugi::xml_node chest = interim_chest.child("Chest"); chest; chest = chest.next_sibling())
		{
			int iNumber = chest.attribute("Number").as_int() - 1;

			if (iNumber < 0 || iNumber >= 3)
			{
				continue;
			}

			m_DGInfo.nMiddleItemBagIndex[iNumber] = chest.attribute("SectionID").as_int();
			m_DGInfo.nMiddleItemBagDropRate[iNumber] = chest.attribute("RewardRate").as_int();
		}

		pugi::xml_node final_chest = range.child("FinalChest");

		m_DGInfo.nLastRewardItemDropCnt = final_chest.attribute("ItemDropCount").as_int();

		for (pugi::xml_node chest = final_chest.child("Chest"); chest; chest = chest.next_sibling())
		{
			int iNumber = chest.attribute("Number").as_int() - 1;

			if (iNumber < 0 || iNumber >= 6)
			{
				continue;
			}

			m_DGInfo.nLastItemBagIndex[iNumber] = chest.attribute("SectionID").as_int();
			m_DGInfo.nLastItemBagDropRate[iNumber] = chest.attribute("RewardRate").as_int();
		}

		this->m_mapDoppelEvent_Info.insert(std::pair<int, _tagDOPPELGANGER_EVENT_INFO>(m_DGInfo.nUserMaxLevel, m_DGInfo));
	}

	g_Log.Add("Load Doppelganger EventInfo : %s", lpFile);
	this->m_bLoad = TRUE;
}

void CDoppelGangerEventInfo::ClearDoppelgangerEventInfo()
{
	this->m_mapDoppelEvent_Info.clear();
}

_tagDOPPELGANGER_EVENT_INFO * CDoppelGangerEventInfo::GetDoppelgangerEventInfo(int nUserMaxLevel)
{
	int nKey = (nUserMaxLevel-1) / 10 * 10 + 10;

	std::map<int, _tagDOPPELGANGER_EVENT_INFO>::iterator it = this->m_mapDoppelEvent_Info.find(nKey);

	if ( it == this->m_mapDoppelEvent_Info.end() )
	{
		return NULL;
	}

	return &it->second;
}

BOOL CDoppelGangerEventInfo::SetDoppelgangerEventInfo(int nUserCount, int nUserMaxLevel)
{
	this->m_nDoppelgangerUserCnt = nUserCount;

	_tagDOPPELGANGER_EVENT_INFO *lpDGInfo = this->GetDoppelgangerEventInfo(nUserMaxLevel);

	if ( lpDGInfo == NULL )
	{
		return FALSE;
	}

	memcpy(&this->m_DoppelgangerInfo, lpDGInfo, sizeof(this->m_DoppelgangerInfo));
	return TRUE;
}

float CDoppelGangerEventInfo::GetMonsterLevelWeight()
{
	return this->m_DoppelgangerInfo.fMonsterLevel[this->m_nDoppelgangerUserCnt-1];
}

float CDoppelGangerEventInfo::GetMonsterHpWeight()
{
	return this->m_DoppelgangerInfo.fMonsterHp[this->m_nDoppelgangerUserCnt-1];
}

float CDoppelGangerEventInfo::GetMonsterAttackWeight()
{
	return this->m_DoppelgangerInfo.fMonsterAttack[this->m_nDoppelgangerUserCnt-1];
}

float CDoppelGangerEventInfo::GetMonsterDefenseWeight()
{
	return this->m_DoppelgangerInfo.fMonsterDefense[this->m_nDoppelgangerUserCnt-1];
}

int CDoppelGangerEventInfo::GetMiddleItemDropCnt()
{
	return this->m_DoppelgangerInfo.nMiddleRewardItemDropCnt;
}

int CDoppelGangerEventInfo::GetMiddleItemBagIndex(int nRewardIndex)
{
	return this->m_DoppelgangerInfo.nMiddleItemBagIndex[nRewardIndex];
}

int CDoppelGangerEventInfo::GetMiddleItemDropRate(int nRewardIndex)
{
	return this->m_DoppelgangerInfo.nMiddleItemBagDropRate[nRewardIndex];
}

int CDoppelGangerEventInfo::GetLastItemDropCnt()
{
	return this->m_DoppelgangerInfo.nLastRewardItemDropCnt;
}

int CDoppelGangerEventInfo::GetLastItemBagIndex(int nRewardIndex)
{
	return this->m_DoppelgangerInfo.nLastItemBagIndex[nRewardIndex];
}

int CDoppelGangerEventInfo::GetLastItemDropRate(int nRewardIndex)
{
	return this->m_DoppelgangerInfo.nLastItemBagDropRate[nRewardIndex];
}




//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


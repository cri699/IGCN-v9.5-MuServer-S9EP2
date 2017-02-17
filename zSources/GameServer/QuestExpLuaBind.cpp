//////////////////////////////////////////////////////////////////////
// QuestExpLuaBind.cpp
#include "stdafx.h"
#include "QuestExpLuaBind.h"
#include "QuestExpUserMng.h"
#include "QuestExpProgMng.h"
#include "QuestExpUserInfo.h"
#include "QuestExpInfo.h"
#include "gObjMonster.h"
#include "user.h"
#include "NpcTalk.h"
#include "MasterLevelSkillTreeSystem.h"
#include "GensSystem.h"
#include "configread.h"
#include "ExpManager.h"

MULua g_MuLuaQuestExp;

const char QuestExpLuaBind::className[] = "QuestExpLuaBind";
const Luna<QuestExpLuaBind>::RegType QuestExpLuaBind::Register[]=
{
	{ "IsUsingExtendedWarehouse",   &QuestExpLuaBind::IsUsingExtendedWarehouse },
	{ "GetNpcIndex",                &QuestExpLuaBind::GetNpcIndex },
	{ "GetCharClass",               &QuestExpLuaBind::GetCharClass },
	{ "IsMasterLevel",              &QuestExpLuaBind::IsMasterLevel },
	{ "GetGensInfluence",           &QuestExpLuaBind::GetGensInfluence },
	{ "NpcShadowPhantomBuff",       &QuestExpLuaBind::NpcShadowPhantomBuff },
	{ "ChkProgQuest",               &QuestExpLuaBind::ChkProgQuest },
	{ "GetUserLv",                  &QuestExpLuaBind::GetUserLv },
	{ "GetInvenItemFind",           &QuestExpLuaBind::GetInvenItemFind },
	{ "SetQuestSwitch",             &QuestExpLuaBind::SetQuestSwitch },
	{ "SetQuestMonsterKill",        &QuestExpLuaBind::SetQuestMonsterKill },
	{ "DeleteInvenItem",            &QuestExpLuaBind::DeleteInvenItem },
	{ "SetQuestGetItem",            &QuestExpLuaBind::SetQuestGetItem },
	{ "SetQuestSkillLearn",         &QuestExpLuaBind::SetQuestSkillLearn },
	{ "SetQuestLevelUp",            &QuestExpLuaBind::SetQuestLevelUp },
	{ "SetTutorial",                &QuestExpLuaBind::SetTutorial },
	{ "SetQuestBuff",               &QuestExpLuaBind::SetQuestBuff },
	{ "SetQuestEventMapKillPoint",  &QuestExpLuaBind::SetQuestEventMapKillPoint },
	{ "SetQuestEventMapClear",      &QuestExpLuaBind::SetQuestEventMapClear },
	{ "SetQuestEventMapDevilPoint", &QuestExpLuaBind::SetQuestEventMapDevilPoint },
	{ "SetQuestProgress",           &QuestExpLuaBind::SetQuestProgress },
	{ "SetQuestRewardExp",          &QuestExpLuaBind::SetQuestRewardExp },
	{ "SetQuestRewardZen",          &QuestExpLuaBind::SetQuestRewardZen },
	{ "SetQuestRewardItem",         &QuestExpLuaBind::SetQuestRewardItem },
	{ "SetQuestRewardContribute",   &QuestExpLuaBind::SetQuestRewardContribute },
	{ "SetQuestRewardRandom",       &QuestExpLuaBind::SetQuestRewardRandom },
	{ "Make_QuestIndexIDToEP",      &QuestExpLuaBind::Make_QuestIndexIDToEP },
	{ "temp",                       &QuestExpLuaBind::temp },
	{ "GetQuestSwitch",             &QuestExpLuaBind::GetQuestSwitch },
	{ "SendQuestProgress",          &QuestExpLuaBind::SendQuestProgress },
	{ "AddQuestExpNpcTalk",         &QuestExpLuaBind::AddQuestExpNpcTalk },
	{ "AddItemAndEtcQuestList",     &QuestExpLuaBind::AddItemAndEtcQuestList },
	{ "SendQuestSwitchList",        &QuestExpLuaBind::SendQuestSwitchList },
	{ "SendQuestItemAndEventList",  &QuestExpLuaBind::SendQuestItemAndEventList },
	{ "IsQuestComplete",            &QuestExpLuaBind::IsQuestComplete },
	{ "SendQuestReward",            &QuestExpLuaBind::SendQuestReward },
	{ "SendQuestComplete",          &QuestExpLuaBind::SendQuestComplete },
	{ "SetQuestProg",               &QuestExpLuaBind::SetQuestProg },
	{ "SendQuestItemUseList",       &QuestExpLuaBind::SendQuestItemUseList },
	{ "SetQuestDropItem",           &QuestExpLuaBind::SetQuestDropItem },
	{ "SetQuestTimeLimit",          &QuestExpLuaBind::SetQuestTimeLimit },
	{ "GetQuestTimeLimitState",     &QuestExpLuaBind::GetQuestTimeLimitState },
	{ "SetQuestItemInfo",           &QuestExpLuaBind::SetQuestItemInfo },
	{ "SetQuestGetZen",             &QuestExpLuaBind::SetQuestGetZen },
	{ "SubQuestZen",                &QuestExpLuaBind::SubQuestZen },
	{ "SetQuestPVP_Point",          &QuestExpLuaBind::SetQuestPVP_Point },
	{ "SetQuestNPCTalk",            &QuestExpLuaBind::SetQuestNPCTalk },
	{ "SetQuestNPCTeleportPosInfo", &QuestExpLuaBind::SetQuestNPCTeleportPosInfo },
	{ "SendQuestConfirmBtn",        &QuestExpLuaBind::SendQuestConfirmBtn },
	{ "GetCheatChangeDay",			&QuestExpLuaBind::GetCheatChangeDay},

	{ 0 }
};

QuestExpLuaBind::QuestExpLuaBind(void)
{
}

QuestExpLuaBind::~QuestExpLuaBind(void)
{
}

int QuestExpLuaBind::IsUsingExtendedWarehouse(lua_State *L)
{
	int iObjIndex = luaL_checkinteger(L, 2);
	int iExtendedWarehouseCount = gObj[iObjIndex].m_PlayerData->m_WarehouseExpansion;
	lua_settop(L, -2);

	lua_pushnumber(L, iExtendedWarehouseCount > 0);
	return 1;
}

int QuestExpLuaBind::GetNpcIndex(lua_State *L)
{
	int iObjIndex = luaL_checkinteger(L, 2);

	if (gObj[iObjIndex].TargetNpcNumber == -1)
	{
		return 0;
	}

	int iNpcIndex = gObj[gObj[iObjIndex].TargetNpcNumber].Class;
	lua_settop(L, -2);

	lua_pushnumber(L, iNpcIndex);
	return 1;
}

int QuestExpLuaBind::GetCharClass(lua_State *L)
{
	int iObjIndex = luaL_checkinteger(L, 2);
	int iCharType = gObj[iObjIndex].Class;
	lua_settop(L, -2);

	lua_pushnumber(L, iCharType);
	return 1;
}

int QuestExpLuaBind::IsMasterLevel(lua_State *L)
{
	int iObjIndex = luaL_checkinteger(L, 2);
	lua_settop(L, -2);

	if (g_MasterLevelSkillTreeSystem.IsMasterLevelUser(&gObj[iObjIndex]))
	{
		lua_pushnumber(L, 1);
	}
	else
	{
		lua_pushnumber(L, 0);
	}

	return 1;
}

int QuestExpLuaBind::GetGensInfluence(lua_State *L)
{
	int iObjIndex = luaL_checkinteger(L, 2);
	int iGensInfluence = g_GensSystem.GetGensInfluence(&gObj[iObjIndex]);
	lua_settop(L, -2);

	lua_pushnumber(L, iGensInfluence);
	return 1;
}

int QuestExpLuaBind::NpcShadowPhantomBuff(lua_State *L)
{
	int iObjIndex = luaL_checkinteger(L, 2);
	NpcShadowPhantom(iObjIndex);
	lua_settop(L, -2);

	return 0;
}

int QuestExpLuaBind::ChkProgQuest(lua_State *L)
{
	int iEp = luaL_checkinteger(L, 2);
	int iObjIndex = luaL_checkinteger(L, 3);
	int iResult = g_QuestExpUserMng.IsProgQuestInfo(iEp, iObjIndex);
	lua_settop(L, -3);

	lua_pushnumber(L, iResult);
	return 1;
}

int QuestExpLuaBind::GetUserLv(lua_State *L)
{
	int iObjIndex = luaL_checkinteger(L, 2);
	int iUserLv = gObj[iObjIndex].Level + gObj[iObjIndex].m_PlayerData->MasterLevel;
	lua_settop(L, -2);

	lua_pushnumber(L, iUserLv);
	return 1;
}

int QuestExpLuaBind::GetInvenItemFind(lua_State *L)
{
	int iItemType = luaL_checkinteger(L, 2);
	int iItemIndex = luaL_checkinteger(L, 3);
	int iItemLevel = luaL_checkinteger(L, 4);
	int iItemSkill = luaL_checkinteger(L, 5);
	int iItemOpt = luaL_checkinteger(L, 6);
	int iItemExOpt = luaL_checkinteger(L, 7);
	int iObjIndex = luaL_checkinteger(L, 8);
	int iItemCnt = 0;
	int iDur = 0;

	for (int x = 0; x < INVENTORY_SIZE; x++)
	{
		if (!gObj[iObjIndex].pInventory[x].IsItem())
		{
			continue;
		}

		if (gObj[iObjIndex].pInventory[x].m_Type != ITEMGET(iItemType, iItemIndex))
		{
			continue;
		}

		if (gObj[iObjIndex].pInventory[x].m_Level != iItemLevel)
		{
			continue;
		}

		if ((gObj[iObjIndex].pInventory[x].m_Type < ITEMGET(14, 0)
			|| gObj[iObjIndex].pInventory[x].m_Type > ITEMGET(14, 8))
			&& (gObj[iObjIndex].pInventory[x].m_Type < ITEMGET(14, 35)
				|| gObj[iObjIndex].pInventory[x].m_Type > ITEMGET(14, 40))
			&& g_QuestExpManager.IsQuestItemAtt(gObj[iObjIndex].pInventory[x].m_Type, QUESTEXP_ITEM_OVERLAP) != true)
		{
			if (gObj[iObjIndex].pInventory[x].m_Option1 == iItemSkill && gObj[iObjIndex].pInventory[x].m_Option3 == iItemOpt && gObj[iObjIndex].pInventory[x].m_NewOption == iItemExOpt)
			{
				if (gObj[iObjIndex].pInventory[x].m_Option2 == 0 && gObj[iObjIndex].pInventory[x].m_SetOption == 0)
				{
					iItemCnt++;
				}
			}
		}
		else
		{
			iDur += gObj[iObjIndex].pInventory[x].m_Durability;
		}
	}

	if (iDur > 0)
		iItemCnt = iDur;

	lua_settop(L, -8);
	lua_pushnumber(L, iItemCnt);

	return 1;
}

int QuestExpLuaBind::SetQuestSwitch(lua_State *L)
{
	int iEp = luaL_checkinteger(L, 2);
	int iQS = luaL_checkinteger(L, 3);
	int iObjIndex = luaL_checkinteger(L, 4);

	g_QuestExpUserMng.SetQuestSwitch(iEp, iQS, iObjIndex);
	lua_settop(L, -4);

	return 0;
}

int QuestExpLuaBind::SetQuestMonsterKill(lua_State* L)
{
	int n = lua_gettop(L);
	int iAsk = luaL_checkinteger(L, 2);
	int iEpisode = luaL_checkinteger(L, 3);
	int iQuestSwitch = luaL_checkinteger(L, 4);
	int iMonsterIndex = luaL_checkinteger(L, 5);
	int iMonsterCnt = luaL_checkinteger(L, 6);
	int iPartyPlay = 0;

	QuestMonsterKill questMonsterKill;

	try
	{
		questMonsterKill.SetIndexID(iAsk);
		questMonsterKill.SetMonsterIndex(iMonsterIndex);
		questMonsterKill.SetMonsterKillCnt(iMonsterCnt);
		questMonsterKill.SetQuestType(QUESTEXP_ASK_MONSTER);

		if (n == 7)
		{
			iPartyPlay = luaL_checkinteger(L, 7);
			questMonsterKill.SetPartyPlay(iPartyPlay);
		}

		g_QuestExpManager.SetQuestExpInfo(&questMonsterKill, iEpisode, iQuestSwitch);
		lua_settop(L, -(n - 1) - 1);
	}
	catch (...)
	{
	}

	return 0;
}

int QuestExpLuaBind::DeleteInvenItem(lua_State *L)
{
	int iItemType = luaL_checkinteger(L, 2);
	int iItemIndex = luaL_checkinteger(L, 3);
	int iItemLevel = luaL_checkinteger(L, 4);
	int iItemSkill = luaL_checkinteger(L, 5);
	int iItemCnt = luaL_checkinteger(L, 6);
	int iItemOpt = luaL_checkinteger(L, 7);
	int iItemExOpt = luaL_checkinteger(L, 8);
	int iObjIndex = luaL_checkinteger(L, 9);

	QuestGetItem questGetItem;

	try
	{
		questGetItem.SetIndexID(0);
		questGetItem.SetQuestItemNum(iItemType, iItemIndex);
		questGetItem.SetQuestItemLevel(iItemLevel);
		questGetItem.SetQuestItemCnt(iItemCnt);
		questGetItem.SetQuestItemSkill(iItemSkill);
		questGetItem.SetQuestItemOpt(iItemOpt);
		questGetItem.SetQuestItemExOpt(iItemExOpt);
		questGetItem.SetQuestType(QUESTEXP_ASK_NONE);

		g_QuestExpProgMng.DeleteInventoryItem(&questGetItem, iObjIndex);
		lua_settop(L, -9);
	}
	catch (...)
	{
	}

	return 1;
}

int QuestExpLuaBind::SetQuestGetItem(lua_State *L)
{
	int iAsk = luaL_checkinteger(L, 2);
	int iEpisode = luaL_checkinteger(L, 3);
	int iQuestSwitch = luaL_checkinteger(L, 4);
	int iItemType = luaL_checkinteger(L, 5);
	int iItemIndex = luaL_checkinteger(L, 6);
	int iItemLevel = luaL_checkinteger(L, 7);
	int iItemSkill = luaL_checkinteger(L, 8);
	int iCnt = luaL_checkinteger(L, 9);
	int iItemOpt = luaL_checkinteger(L, 10);
	int iItemExOpt = luaL_checkinteger(L, 11);

	QuestGetItem quest_get_item;

	try
	{
		quest_get_item.SetIndexID(iAsk);
		quest_get_item.SetQuestItemNum(iItemType, iItemIndex);
		quest_get_item.SetQuestItemLevel(iItemLevel);
		quest_get_item.SetQuestItemCnt(iCnt);
		quest_get_item.SetQuestItemSkill(iItemSkill);
		quest_get_item.SetQuestItemOpt(iItemOpt);
		quest_get_item.SetQuestItemExOpt(iItemExOpt);
		quest_get_item.SetQuestType(QUESTEXP_ASK_GET_ITEM);

		g_QuestExpManager.SetQuestExpInfo(&quest_get_item, iEpisode, iQuestSwitch);
		lua_settop(L, -11);
	}
	catch (...) {}

	return 0;
}

int QuestExpLuaBind::SetQuestSkillLearn(lua_State *L)
{
	int iAsk = luaL_checkinteger(L, 2);
	int iEpisode = luaL_checkinteger(L, 3);
	int iQuestSwitch = luaL_checkinteger(L, 4);
	int iSkillIndex = luaL_checkinteger(L, 5);
	QuestSkillLearn questSkillLearn;

	try
	{
		questSkillLearn.SetIndexID(iAsk);
		questSkillLearn.SetQuestSkillIndex(iSkillIndex);
		questSkillLearn.SetQuestType(QUESTEXP_ASK_SKILL_LEARN);

		g_QuestExpManager.SetQuestExpInfo(&questSkillLearn, iEpisode, iQuestSwitch);
		lua_settop(L, -5);
	}
	catch (...) {}

	return 0;
}

int QuestExpLuaBind::SetQuestLevelUp(lua_State *L)
{
	int iAsk = luaL_checkinteger(L, 2);
	int iEpisode = luaL_checkinteger(L, 3);
	int iQuestSwitch = luaL_checkinteger(L, 4);
	int iLevelValue = luaL_checkinteger(L, 5);

	QuestLevelUp questLevelUp;

	try
	{
		questLevelUp.SetIndexID(iAsk);
		questLevelUp.SetQuestLevelVal(iLevelValue);
		questLevelUp.SetQuestType(QUESTEXP_ASK_LEVEL_UP);

		g_QuestExpManager.SetQuestExpInfo(&questLevelUp, iEpisode, iQuestSwitch);
		lua_settop(L, -5);
	}
	catch (...) {}

	return 0;
}

int QuestExpLuaBind::SetTutorial(lua_State *L)
{
	int iAsk = luaL_checkinteger(L, 2);
	int iEpisode = luaL_checkinteger(L, 3);
	int iQuestSwitch = luaL_checkinteger(L, 4);

	QuestTutorial questTutorial;

	try
	{
		questTutorial.SetIndexID(iAsk);
		questTutorial.SetQuestType(QUESTEXP_ASK_TUTORIAL_KEY);

		g_QuestExpManager.SetQuestExpInfo(&questTutorial, iEpisode, iQuestSwitch);
		lua_settop(L, -4);

	}
	catch (...) {}

	return 0;
}

int QuestExpLuaBind::SetQuestBuff(lua_State* L)
{
	int iAsk = luaL_checkinteger(L, 2);
	int iEpisode = luaL_checkinteger(L, 3);
	int iQuestSwitch = luaL_checkinteger(L, 4);
	int iBuffIndex = luaL_checkinteger(L, 5);

	QuestBuff questBuff;

	try
	{
		questBuff.SetIndexID(iAsk);
		questBuff.SetQuestBuffIndex(iBuffIndex);
		questBuff.SetQuestType(QUESTEXP_ASK_BUFF);

		g_QuestExpManager.SetQuestExpInfo(&questBuff, iEpisode, iQuestSwitch);
		lua_settop(L, -5);
	}
	catch (...) {}

	return 0;
}

int QuestExpLuaBind::SetQuestEventMapKillPoint(lua_State *L)
{
    int iAsk = luaL_checkinteger(L, 2);
    int iEpisode = luaL_checkinteger(L, 3);
    int iQuestSwitch = luaL_checkinteger(L, 4);
    int iQuestType = luaL_checkinteger(L, 5);
    int iEventMapLevel = luaL_checkinteger(L, 6);
    int iKillCnt = luaL_checkinteger(L, 7);

    QuestEventMapKillPoint questEventMapKillPoint;

    try
    {
        questEventMapKillPoint.SetIndexID(iAsk);
        questEventMapKillPoint.SetQuestEventMapKillCnt(iKillCnt);
        questEventMapKillPoint.SetQuestEventMapLevel(iEventMapLevel);
        questEventMapKillPoint.SetQuestType(iQuestType);

        g_QuestExpManager.SetQuestExpInfo(&questEventMapKillPoint, iEpisode, iQuestSwitch);
        lua_settop(L, -7);
    }
    catch(...){}

    return 0;
}

int QuestExpLuaBind::SetQuestEventMapClear(lua_State *L)
{
	int iAsk = luaL_checkinteger(L, 2);
	int iEpisode = luaL_checkinteger(L, 3);
	int iQuestSwitch = luaL_checkinteger(L, 4);
	int iQuestType = luaL_checkinteger(L, 5);
	int iEventMapLevel = luaL_checkinteger(L, 6);

	QuestEventMapClear questEventMapClear;

	try
	{
		questEventMapClear.SetIndexID(iAsk);
		questEventMapClear.SetQuestEventMapLevel(iEventMapLevel);
		questEventMapClear.SetQuestType(iQuestType);

		g_QuestExpManager.SetQuestExpInfo(&questEventMapClear, iEpisode, iQuestSwitch);
		lua_settop(L, -6);
	}
	catch (...) {}

	return 0;
}

int QuestExpLuaBind::SetQuestEventMapDevilPoint(lua_State *L)
{
    int iAsk = luaL_checkinteger(L, 2);
    int iEpisode = luaL_checkinteger(L, 3);
    int iQuestSwitch = luaL_checkinteger(L, 4);
    int iEventMapLevel = luaL_checkinteger(L, 5);
    int iEventMapDevilPoint = luaL_checkinteger(L, 6);

    QuestEventMapDevilPoint questEventMapDevilPoint;

    try
    {
        questEventMapDevilPoint.SetIndexID(iAsk);
        questEventMapDevilPoint.SetQuestEventMapLevel(iEventMapLevel);
        questEventMapDevilPoint.SetQuestEventMapDevilPoint(iEventMapDevilPoint);
        questEventMapDevilPoint.SetQuestType(QUESTEXP_ASK_DEVILSQUARE_POINT_GAIN);

        g_QuestExpManager.SetQuestExpInfo(&questEventMapDevilPoint, iEpisode, iQuestSwitch);
        lua_settop(L, -6);
    }
    catch(...){}

    return 0;
}

int QuestExpLuaBind::SetQuestProgress(lua_State *L)
{
	int iAsk = luaL_checkinteger(L, 2);
	int iEpisode = luaL_checkinteger(L, 3);
	int iQuestSwitch = luaL_checkinteger(L, 4);

	QuestProgress questProgress;

	try
	{
		questProgress.SetIndexID(iAsk);
		questProgress.SetQuestType(QUESTEXP_ASK_NONE);

		g_QuestExpManager.SetQuestExpInfo(&questProgress, iEpisode, iQuestSwitch);
		lua_settop(L, -4);
	}
	catch (...) {}

	return 0;
}

int QuestExpLuaBind::SetQuestRewardExp(lua_State *L)
{
	int iEpisode = luaL_checkinteger(L, 2);
	int iQuestSwitch = luaL_checkinteger(L, 3);
	int iExp = g_ExpManager.GetQuestExp() * luaL_checkinteger(L, 4);

	RewardExp rewardExp;

	try
	{
		rewardExp.SetRewardExp(iExp);
		rewardExp.SetRewardType(QUESTEXP_REWARD_EXP);

		g_QuestExpManager.SetQuestExpInfo(&rewardExp, iEpisode, iQuestSwitch);
		lua_settop(L, -4);
	}
	catch (...) {}

	return 0;
}

int QuestExpLuaBind::SetQuestRewardZen(lua_State *L)
{
    int iEpisode = luaL_checkinteger(L, 2);
    int iQuestSwitch = luaL_checkinteger(L, 3);
    int iZen = luaL_checkinteger(L, 4);

    RewardZen rewardZen;

    try
    {
        rewardZen.SetRewardZen(iZen);
        rewardZen.SetRewardType(QUESTEXP_REWARD_ZEN);

        g_QuestExpManager.SetQuestExpInfo(&rewardZen, iEpisode, iQuestSwitch);
        lua_settop(L, -4);
    }
    catch(...){}
    
    return 0;
}

int QuestExpLuaBind::SetQuestRewardItem(lua_State *L)
{
	int n = lua_gettop(L);
	int iEpisode = 0;
	int iQuestSwitch = 0;
	int iItemType = 0;
	int iItemIndex = 0;
	int iItemLevel = 0;
	int iItemDur = 0;
	int iItemSkill = 0;
	int iItemCnt = 0;
	int iItemOverlap = 0;
	int iItemOpt = 0;
	int iItemExOpt = 0;

	if (n == 9)
	{
		iEpisode = luaL_checkinteger(L, 2);
		iQuestSwitch = luaL_checkinteger(L, 3);
		iItemType = luaL_checkinteger(L, 4);
		iItemIndex = luaL_checkinteger(L, 5);
		iItemLevel = luaL_checkinteger(L, 6);
		iItemDur = luaL_checkinteger(L, 7);
		iItemSkill = luaL_checkinteger(L, 8);
		iItemCnt = luaL_checkinteger(L, 9);
		lua_settop(L, -9);
	}

	if (n == 10)
	{
		iEpisode = luaL_checkinteger(L, 2);
		iQuestSwitch = luaL_checkinteger(L, 3);
		iItemType = luaL_checkinteger(L, 4);
		iItemIndex = luaL_checkinteger(L, 5);
		iItemLevel = luaL_checkinteger(L, 6);
		iItemDur = luaL_checkinteger(L, 7);
		iItemSkill = luaL_checkinteger(L, 8);
		iItemCnt = luaL_checkinteger(L, 9);
		iItemOverlap = luaL_checkinteger(L, 10);
		lua_settop(L, -10);
	}

	if (n == 11)
	{
		iEpisode = luaL_checkinteger(L, 2);
		iQuestSwitch = luaL_checkinteger(L, 3);
		iItemType = luaL_checkinteger(L, 4);
		iItemIndex = luaL_checkinteger(L, 5);
		iItemLevel = luaL_checkinteger(L, 6);
		iItemDur = luaL_checkinteger(L, 7);
		iItemSkill = luaL_checkinteger(L, 8);
		iItemCnt = luaL_checkinteger(L, 9);
		iItemOverlap = luaL_checkinteger(L, 10);
		iItemOpt = luaL_checkinteger(L, 11);
		lua_settop(L, -11);
	}

	if (n == 12)
	{
		iEpisode = luaL_checkinteger(L, 2);
		iQuestSwitch = luaL_checkinteger(L, 3);
		iItemType = luaL_checkinteger(L, 4);
		iItemIndex = luaL_checkinteger(L, 5);
		iItemLevel = luaL_checkinteger(L, 6);
		iItemDur = luaL_checkinteger(L, 7);
		iItemSkill = luaL_checkinteger(L, 8);
		iItemCnt = luaL_checkinteger(L, 9);
		iItemOverlap = luaL_checkinteger(L, 10);
		iItemOpt = luaL_checkinteger(L, 11);
		iItemExOpt = luaL_checkinteger(L, 12);
		lua_settop(L, -12);
	}

	RewardItem rewardItem;

	try
	{
		rewardItem.SetRewardItemNum(iItemType, iItemIndex);
		rewardItem.SetRewardItemLevel(iItemLevel);
		rewardItem.SetRewardItemDur(iItemDur);
		rewardItem.SetRewardItemCnt(iItemCnt);
		rewardItem.SetRewardItemSkill(iItemSkill);
		rewardItem.SetRewardItemOverlap(iItemOverlap);
		rewardItem.SetRewardItemOpt(iItemOpt);
		rewardItem.SetRewardItemExOpt(iItemExOpt);
		rewardItem.SetRewardType(QUESTEXP_REWARD_ITEM);

		g_QuestExpManager.SetQuestExpInfo(&rewardItem, iEpisode, iQuestSwitch);
	}
	catch (...) {}

	return 0;
}

int QuestExpLuaBind::SetQuestRewardContribute(lua_State *L)
{
	int iEpisode = luaL_checkinteger(L, 2);
	int iQuestSwitch = luaL_checkinteger(L, 3);
	int iContributePT = luaL_checkinteger(L, 4);

	RewardContribute rewardContribute;

	try
	{
		rewardContribute.SetRewardContributePoint(iContributePT);
		rewardContribute.SetRewardType(QUESTEXP_REWARD_CONTRIBUTION_POINT);

		g_QuestExpManager.SetQuestExpInfo(&rewardContribute, iEpisode, iQuestSwitch);
		lua_settop(L, -4);
	}
	catch (...) {}

	return 0;
}

int QuestExpLuaBind::SetQuestRewardRandom(lua_State *L)
{
	QuestExpRewardKindRandom questExpRewardKindRandom;

	try
	{
		int iArrRewardIndex[MAX_QUESTEXP_REWARDKIND_RANDOM] = { 0, 0, 0, 0, 0 };
		int iArrRate[MAX_QUESTEXP_REWARDKIND_RANDOM] = { 0, 0, 0, 0 ,0 };
		int iItemCnt = 0;

		for (int i = 0; i < MAX_QUESTEXP_REWARDKIND_RANDOM; i++)
		{
			iArrRewardIndex[i] = -1;
			iArrRate[i] = -1;
		}

		int iEpisode = luaL_checkinteger(L, 2);
		int iQuestSwitch = luaL_checkinteger(L, 3);
		int iRandRewardCnt = luaL_checkinteger(L, 4);

		questExpRewardKindRandom.SetQuestEpisode(iEpisode);
		questExpRewardKindRandom.SetQuestSwitch(iQuestSwitch);
		questExpRewardKindRandom.SetRandRewardCnt(iRandRewardCnt);

		int n = lua_gettop(L);

		for (int j = 5; j <= n; j += 2)
		{
			int iRewardIndex = luaL_checkinteger(L, j);
			int iRewardRate = luaL_checkinteger(L, j + 1);

			iArrRewardIndex[iItemCnt] = iRewardIndex - 1;
			iArrRate[iItemCnt] = iRewardRate;

			iItemCnt++;
		}

		questExpRewardKindRandom.SetRewardItemTotCnt(iItemCnt);
		lua_settop(L, -(n - 1) - 1);

		questExpRewardKindRandom.SetRewardRate(iArrRate);
		questExpRewardKindRandom.SetRewardIndex(iArrRewardIndex);
		questExpRewardKindRandom.SetRewardKind(true);

		g_QuestExpManager.AddQuestExpRewardKind(&questExpRewardKindRandom);

	}
	catch (...) {}

	return 0;
}

int QuestExpLuaBind::Make_QuestIndexIDToEP(lua_State *L)
{
	DWORD dwQuestInfoIndexID = luaL_checknumber(L, 2);
	int iEpisode = GetQuestEpisodeFromInfoIndexId(dwQuestInfoIndexID);

	if (IsEpisode(iEpisode) == false)
	{
		g_Log.Add("[QuestExp] - Error - [%s] [%d]", __FILE__, __LINE__);
		return 0;
	}

	lua_settop(L, -2);
	lua_pushnumber(L, iEpisode);

	return 1;
}

int QuestExpLuaBind::temp(lua_State *L)
{
	return 0;
}

int QuestExpLuaBind::GetQuestSwitch(lua_State *L)
{
	int iEpisode = luaL_checkinteger(L, 2);
	int iObjIndex = luaL_checkinteger(L, 3);
	int iQuestSwitch = g_QuestExpUserMng.GetQuestSwitch(iEpisode, iObjIndex);

	if (iQuestSwitch == -1)
	{
		g_Log.Add("[QuestExp] - Error - LuaGetQuestSwitch [%s][%s] Episode[%d]", gObj[iObjIndex].AccountID, gObj[iObjIndex].Name, iEpisode);
		return 0;
	}

	lua_settop(L, -3);
	lua_pushnumber(L, iQuestSwitch);

	return 1;
}

int QuestExpLuaBind::SendQuestProgress(lua_State *L)
{
	int iEpisode = luaL_checkinteger(L, 2);
	int iQuestSwitch = luaL_checkinteger(L, 3);
	int iObjIndex = luaL_checkinteger(L, 4);

	g_QuestExpUserMng.SetQuestSwitch(iEpisode, iQuestSwitch, iObjIndex);
	g_QuestExpProgMng.SendQuestProgress(iEpisode, iQuestSwitch, iObjIndex);

	lua_settop(L, -4);

	return 0;
}

int QuestExpLuaBind::AddQuestExpNpcTalk(lua_State *L)
{
	int iEpisode = luaL_checkinteger(L, 2);
	int iQuestSwitch = luaL_checkinteger(L, 3);

	g_QuestExpProgMng.AddQuestSwitchList(iEpisode, iQuestSwitch);

	lua_settop(L, -3);

	return 0;
}

int QuestExpLuaBind::AddItemAndEtcQuestList(lua_State *L)
{
	int iEpisode = luaL_checkinteger(L, 2);
	int iQuestSwitch = luaL_checkinteger(L, 3);

	g_QuestExpProgMng.AddQuestSwitchList(iEpisode, iQuestSwitch);

	lua_settop(L, -3);

	return 0;
}

int QuestExpLuaBind::SendQuestSwitchList(lua_State *L)
{
	int iObjIndex = luaL_checkinteger(L, 2);

	g_QuestExpProgMng.SendQuestSwitchList(iObjIndex, QUESTEXP_SEND_NPC);

	lua_settop(L, -2);

	return 0;
}

int QuestExpLuaBind::SendQuestItemAndEventList(lua_State *L)
{
	int iObjIndex = luaL_checkinteger(L, 2);

	g_QuestExpProgMng.SendQuestSwitchList(iObjIndex, QUESTEXP_SEND_EVENT);

	lua_settop(L, -2);

	return 0;
}

int QuestExpLuaBind::IsQuestComplete(lua_State *L)
{
	int iEpisode = luaL_checkinteger(L, 2);
	int iObjIndex = luaL_checkinteger(L, 3);
	bool bComplete = g_QuestExpUserMng.IsQuestComplete(iEpisode, iObjIndex);

	lua_settop(L, -3);
	lua_pushboolean(L, bComplete);

	return 1;
}

int QuestExpLuaBind::SendQuestReward(lua_State *L)
{
	int iEpisode = luaL_checkinteger(L, 2);
	int iObjIndex = luaL_checkinteger(L, 3);
	int iResult = g_QuestExpUserMng.SendQuestReward(iEpisode, iObjIndex);

	if (iResult == 1)
	{
		g_QuestExpProgMng.SetQuestProg(iEpisode, iObjIndex, QUESTEXP_PROG_STATE_NONE);
	}

	lua_settop(L, -3);
	lua_pushnumber(L, iResult);

	return 1;
}

int QuestExpLuaBind::SendQuestComplete(lua_State *L)
{
	int iEpisode = luaL_checkinteger(L, 2);
	int iQuestSwitch = luaL_checkinteger(L, 3);
	int iObjIndex = luaL_checkinteger(L, 4);

	DWORD dwQuestInfoIndexID = GetQuestInfoIndexId(iEpisode, iQuestSwitch);

	g_QuestExpProgMng.GCANSQuestCompleteBtnClick(iObjIndex, dwQuestInfoIndexID, 1);
	g_QuestExpUserMng.InitUserQuestAskInfo(iEpisode, iObjIndex);
	g_QuestExpProgMng.SetQuestProg(iEpisode, iObjIndex, QUESTEXP_PROG_STATE_NONE);

	lua_settop(L, -4);

	return 0;
}

int QuestExpLuaBind::SetQuestProg(lua_State *L)
{
	int iEpisode = luaL_checkinteger(L, 2);
	int iObjIndex = luaL_checkinteger(L, 3);
	int iState = luaL_checkinteger(L, 4);

	g_QuestExpProgMng.SetQuestProg(iEpisode, iObjIndex, iState);

	lua_settop(L, -4);

	return 0;
}

int QuestExpLuaBind::SendQuestItemUseList(lua_State *L)
{
	int iObjIndex = luaL_checkinteger(L, 2);

	g_QuestExpProgMng.SendQuestSwitchList(iObjIndex, QUESTEXP_SEND_ITEM);

	lua_settop(L, -2);

	return 0;
}

int QuestExpLuaBind::SetQuestDropItem(lua_State *L)
{
	int iEpisode = luaL_checkinteger(L, 2);
	int iQuestSwitch = luaL_checkinteger(L, 3);
	int iAskIndex = luaL_checkinteger(L, 4);
	int iMonsterIndex = luaL_checkinteger(L, 5);
	int iDropRate = luaL_checkinteger(L, 6);
	int iItemType = luaL_checkinteger(L, 7);
	int iItemIndex = luaL_checkinteger(L, 8);
	int iItemLevel = luaL_checkinteger(L, 9);
	int iItemCnt = luaL_checkinteger(L, 10);

	DWORD dwQuestInfoIndexID = GetQuestInfoIndexId(iEpisode, iQuestSwitch);
	QuestDropItemInfo questDropItemInfo;

	try
	{
		questDropItemInfo.SetQuestEpisode(iEpisode);
		questDropItemInfo.SetQuestSwitch(iQuestSwitch);
		questDropItemInfo.SetAskIndex(iAskIndex);
		questDropItemInfo.SetMonsterIndex(iMonsterIndex);
		questDropItemInfo.SetDropRate(iDropRate);
		questDropItemInfo.SetItemNum(iItemType, iItemIndex);
		questDropItemInfo.SetItemLevel(iItemLevel);
		questDropItemInfo.SetItemCnt(iItemCnt);

		g_QuestExpManager.AddQuestDropItemInfo(&questDropItemInfo, dwQuestInfoIndexID);

		lua_settop(L, -10);
	}
	catch (...) {}

	return 0;
}

int QuestExpLuaBind::SetQuestTimeLimit(lua_State *L)
{
	int iEpisode = luaL_checkinteger(L, 2);
	int iQuestSwitch = luaL_checkinteger(L, 3);
	int iObjIndex = luaL_checkinteger(L, 4);
	int iTimeLimit = luaL_checkinteger(L, 5);

	DWORD dwQuestInfoIndexID = GetQuestInfoIndexId(iEpisode, iQuestSwitch);

	g_QuestExpProgMng.SetQuestTimeLimit(iObjIndex, dwQuestInfoIndexID, iTimeLimit * 60);

	lua_settop(L, -5);

	return 0;
}

int QuestExpLuaBind::GetQuestTimeLimitState(lua_State *L)
{
	int iState = 0;
	int iEpisode = luaL_checkinteger(L, 2);
	int iObjIndex = luaL_checkinteger(L, 3);

	if (g_QuestExpUserMng.GetQuestProgState(iEpisode, iObjIndex) == QUESTEXP_PROG_STATE_TIME_LIMIT)
	{
		iState = 1;
	}

	lua_settop(L, -3);
	lua_pushnumber(L, iState);

	return 1;
}

int QuestExpLuaBind::SetQuestItemInfo(lua_State *L)
{
	int n = lua_gettop(L);
	int iEp = 0;

	int iItemType = luaL_checkinteger(L, 2);
	int iItemIndex = luaL_checkinteger(L, 3);
	DWORD dwQuestItemAtt = luaL_checkinteger(L, 4);

	QUEST_ITEM_INFO stQuestItemInfo;
	stQuestItemInfo.iItemNum = ITEMGET(iItemType, iItemIndex);
	stQuestItemInfo.dwQuestItemAtt = dwQuestItemAtt;

	if (n == 5)
	{
		iEp = luaL_checkinteger(L, 5);
		stQuestItemInfo.iEp = iEp;
	}

	g_QuestExpManager.AddQuestItemInfo(stQuestItemInfo);

	lua_settop(L, -(n - 1) - 1);

	return 0;
}

int QuestExpLuaBind::SetQuestGetZen(lua_State *L)
{
	int iAskIndex = luaL_checkinteger(L, 2);
	int iEpisode = luaL_checkinteger(L, 3);
	int iQuestSwitch = luaL_checkinteger(L, 4);
	int iNeedZen = luaL_checkinteger(L, 5);

	QuestNeedZen questNeedZen;

	try
	{
		questNeedZen.SetIndexID(iAskIndex);
		questNeedZen.SetQuestType(QUESTEXP_ASK_NEED_ZEN);
		questNeedZen.SetQuestNeedZen(iNeedZen);

		g_QuestExpManager.SetQuestExpInfo(&questNeedZen, iEpisode, iQuestSwitch);

		lua_settop(L, -5);
	}
	catch (...) {}

	return 0;
}

int QuestExpLuaBind::SubQuestZen(lua_State *L)
{
	int iSubMoney = luaL_checkinteger(L, 2);
	int iObjIndex = luaL_checkinteger(L, 3);

	if (gObj[iObjIndex].m_PlayerData->Money >= iSubMoney)
	{
		gObj[iObjIndex].m_PlayerData->Money -= iSubMoney;
	}

	GSProtocol.GCMoneySend(iObjIndex, gObj[iObjIndex].m_PlayerData->Money);
	lua_settop(L, -3);

	return 0;
}

int QuestExpLuaBind::SetQuestPVP_Point(lua_State *L)
{
	int iAskIndex = luaL_checkinteger(L, 2);
	int iEpisode = luaL_checkinteger(L, 3);
	int iQuestSwitch = luaL_checkinteger(L, 4);
	int iMapNum = luaL_checkinteger(L, 5);
	int iPVP_Point = luaL_checkinteger(L, 6);

	QuestPVP_Point questPVP_Point;

	try
	{
		questPVP_Point.SetIndexID(iAskIndex);
		questPVP_Point.SetQuestType(QUESTEXP_ASK_CONTRIBUTION_POINT);
		questPVP_Point.SetQuestMapNum(iMapNum);
		questPVP_Point.SetQuestPVP_Point(iPVP_Point);

		g_QuestExpManager.SetQuestExpInfo(&questPVP_Point, iEpisode, iQuestSwitch);

		lua_settop(L, -6);
	}
	catch (...) {}

	return 0;
}

int QuestExpLuaBind::SetQuestNPCTalk(lua_State *L)
{
	int iAskIndex = luaL_checkinteger(L, 2);
	int iEpisode = luaL_checkinteger(L, 3);
	int iQuestSwitch = luaL_checkinteger(L, 4);
	int iNPCIndex = luaL_checkinteger(L, 5);

	QuestNpcTalk questNpcTalk;

	questNpcTalk.SetIndexID(iAskIndex);
	questNpcTalk.SetQuestType(QUESTEXP_ASK_NPC_TALK);
	questNpcTalk.SetQuestNPCTalk(iNPCIndex);

	g_QuestExpManager.SetQuestExpInfo(&questNpcTalk, iEpisode, iQuestSwitch);

	lua_settop(L, -5);

	return 0;
}

int QuestExpLuaBind::SetQuestNPCTeleportPosInfo(lua_State *L)
{
	int n = lua_gettop(L);
	int iIndex = luaL_checkinteger(L, 2);
	int iMapNum = luaL_checkinteger(L, 3);
	int iPosX = luaL_checkinteger(L, 4);
	int iPosY = luaL_checkinteger(L, 5);
	int iDir = luaL_checkinteger(L, 6);

	gQuestExpNpcTeleport.m_QuestNPCTeleportPos[iIndex].mapnum = iMapNum;
	gQuestExpNpcTeleport.m_QuestNPCTeleportPos[iIndex].x = iPosX;
	gQuestExpNpcTeleport.m_QuestNPCTeleportPos[iIndex].y = iPosY;
	gQuestExpNpcTeleport.m_QuestNPCTeleportPos[iIndex].dir = iDir;

	lua_settop(L, -(n - 1) - 1);

	return 0;
}

int QuestExpLuaBind::SendQuestConfirmBtn(lua_State *L)
{
    int iEpisode = luaL_checkinteger(L, 2);
    int iQS = luaL_checkinteger(L, 3);
    int iObjIndex = luaL_checkinteger(L, 4);

    DWORD dwQuestInfoIndexID = GetQuestInfoIndexId(iEpisode, iQS);
    g_QuestExpProgMng.GCANSQuestCompleteBtnClick(iObjIndex, dwQuestInfoIndexID, 1);

    if( gObj[iObjIndex].m_PlayerData->m_UserQuestInfo[iEpisode].GetAskCnt() > 0 )
    {
		gObj[iObjIndex].m_PlayerData->m_UserQuestInfo[iEpisode].SetAskCnt(0);

		for (int i = 0; i < MAX_QUESTEXP_USER_INFO; i++)
		{
			gObj[iObjIndex].m_PlayerData->m_UserQuestInfo[iEpisode].m_UserQuestAskInfo[i].Clear();
		}
    }

    lua_settop(L, -4);

    return 0;
}

int QuestExpLuaBind::GetCheatChangeDay(lua_State *L)
{
	int iChangeDay = g_QuestExpProgMng.GetCheatChangeDay();
	lua_pushnumber(L, iChangeDay);

	return 1;
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


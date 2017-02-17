//////////////////////////////////////////////////////////////////////
// QuestExpDefine.h
#ifndef QUESTEXPDEFINE_H
#define QUESTEXPDEFINE_H

#pragma once

#define MAX_QUESTEXP_ASK_INFO				5
#define MAX_QUESTEXP_REWARD_INFO			5

#define MAX_QUESTEXP_EPISODE				25
#define MAX_QUESTEXP_SWITCH					255

#define MAX_QUESTEXP_INFO					MAX_QUESTEXP_EPISODE
#define MAX_QUESTEXP_USER_INFO				MAX_QUESTEXP_EPISODE

#define MAX_QUESTEXP_REWARDKIND_RANDOM		5

#define MAX_QUESTEXP_ACTIVE_QUEST			10

inline static bool IsEpisode(int iEp)
{
	return (iEp >= 0 && iEp < MAX_QUESTEXP_EPISODE);
}

inline static bool IsQuestSwitch(int iQS)
{
	return (iQS >= 0 && iQS <= MAX_QUESTEXP_SWITCH); // RANGE: 0 - 255
}

inline static bool IsQuestAskInfo(int iAsk)
{
	return (iAsk >= 0 && iAsk < MAX_QUESTEXP_ASK_INFO);
}

inline static bool IsQuestRewardInfo(int iReward)
{
	return (iReward >= 0 && iReward < MAX_QUESTEXP_REWARD_INFO);
}

inline static bool IsQuestExpInfo(int iQuestExpInfo)
{
	return (iQuestExpInfo >= 0 && iQuestExpInfo < MAX_QUESTEXP_INFO);
}

inline static DWORD GetQuestInfoIndexId(int iEp, int iQS)
{
	return (iQS + (iEp << 0x10));
}

inline static int GetQuestEpisodeFromInfoIndexId(DWORD dwQuestInfoIndexID)
{
	return (dwQuestInfoIndexID >> 0x10);
}

inline static int GetQuestSwitchFromInfoIndexId(DWORD dwQuestInfoIndexID)
{
	return (dwQuestInfoIndexID % 0x10000);
}

enum QUESTEXP_ASK_TYPE
{
	QUESTEXP_ASK_NONE = 0x00,
	QUESTEXP_ASK_MONSTER = 0x01,
	QUESTEXP_ASK_SKILL_LEARN = 0x02,
	QUESTEXP_ASK_GET_ITEM = 0x04,
	QUESTEXP_ASK_LEVEL_UP = 0x08,
	QUESTEXP_ASK_TUTORIAL_KEY = 0x10,
	QUESTEXP_ASK_BUFF = 0x20,
	QUESTEXP_ASK_CHAOSCASTLE_USER_KILL = 0x40,
	QUESTEXP_ASK_CHAOSCASTLE_MON_KILL = 0x41,
	QUESTEXP_ASK_BLOODCASTLE_DOOR_KILL = 0x42,
	QUESTEXP_ASK_BLOODCASTLE_CLEAR = 0x100,
	QUESTEXP_ASK_CHAOSCASTLE_CLEAR = 0x101,
	QUESTEXP_ASK_DEVILSQUARE_CLEAR = 0x102,
	QUESTEXP_ASK_ILLUSIONTEMPLE_CLEAR = 0x103,
	QUESTEXP_ASK_DEVILSQUARE_POINT_GAIN = 0x104,
	QUESTEXP_ASK_NEED_ZEN = 0x105,
	QUESTEXP_ASK_CONTRIBUTION_POINT = 0x106,
	QUESTEXP_ASK_NPC_TALK = 0x107
};

enum QUESTEXP_REWARD_TYPE
{
	QUESTEXP_REWARD_NONE = 0x00,
	QUESTEXP_REWARD_EXP = 0x01,
	QUESTEXP_REWARD_ZEN = 0x02,
	QUESTEXP_REWARD_ITEM = 0x04,
	QUESTEXP_REWARD_CONTRIBUTION_POINT = 0x10
};

enum QUESTEXP_ITEM_ATTRIBUTE
{
	QUESTEXP_ITEM_OVERLAP = 0x01,
	QUESTEXP_ITEM_DROP = 0x02,
	QUESTEXP_ITEM_WAREHOUSE = 0x04,
	QUESTEXP_ITEM_SELL_TO_NPC = 0x08,
	QUESTEXP_ITEM_SELL_PSHOP = 0x10,
	QUESTEXP_ITEM_SELL_TRADE = 0x20,
	QUESTEXP_ITEM_QUEST_PROGR = 0x40
};

enum QUESTEXP_SEND_TYPE
{
	QUESTEXP_SEND_NPC = 0x00,
	QUESTEXP_SEND_EVENT = 0x01,
	QUESTEXP_SEND_ITEM = 0x02
};

enum QUESTEXP_PROG_STATE_TYPE
{
	QUESTEXP_PROG_STATE_NONE = 0x00,
	QUESTEXP_PROG_STATE_ONGOING = 0x01,
	QUESTEXP_PROG_STATE_TIME_LIMIT = 0x02
};

#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


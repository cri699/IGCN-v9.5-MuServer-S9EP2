//////////////////////////////////////////////////////////////////////
// QuestExpUserMng.h
#ifndef QUESTEXPUSERMNG_H
#define QUESTEXPUSERMNG_H

#pragma once

class QuestExpRewardKindRandom;
class QuestExpAsk;
class UserQuestInfo;
struct PMSG_ANS_QUESTEXP_INFO;

class QuestExpUserMng
{
public:
    QuestExpUserMng(void);
    ~QuestExpUserMng(void);

    int IsProgQuestInfo(int iEpisode, int iObjIndex);
	int IsProgQuestInfo(DWORD dwQuestInfoIndexID, int iObjIndex);

	bool IsQuestComplete(int iEpisode, int iObjIndex);

    bool QuestExpExtentCheck(int x, int y, int w, int h);
    bool QuestExpCheckInventoryEmptySpace(BYTE* btTempInventoryMap, int iItemHeight, int iItemWidth, int iObjIndex);
	BYTE QuestExpOnlyInventoryRectCheck(BYTE *btTempInventoryMap, int sx, int sy, int width, int height, int iObjIndex);
    void QuestExpTempInventoryItemSet(BYTE *btTempInventoryMap, int itempos, int xl, int yl, BYTE set_byte, int iObjIndex);

    bool GetRandomRewardItemIndex(int *iSetItem, QuestExpRewardKindRandom *pRewardKindRandom);
	bool IsRandRewardIndex(int iRewardIndexID, QuestExpRewardKindRandom *pRewardKindRandom);
	bool IsRandResultReward(int iRewardIndexID, QuestExpRewardKindRandom *pRewardKindRandom, int *iRandResultIndexID);
	 
    int InvenChk_EnableReward(int episode_number, int obj_index, int* rand_result_index_id);
    int SendQuestReward(int episode_number, int obj_index);
    void InitUserQuestAskInfo(int episode_number, int obj_index);
    void SetQuestSwitch(int episode_number, int quest_switch, int obj_index);
    bool AddUserQuestAskInfo(DWORD quest_info_index_id, int obj_index);
    void AddUserQuestAskMonsterKill(QuestExpAsk* quest_exp_ask, int obj_index, int episode_number, int ask_count);
    int GetQuestSwitch(int episode_number, int obj_index);
    void UserAllQuestInfoSave(int obj_index);
    void UserQuestInfoSave(DWORD quest_info_index_id, int obj_index);
    void DB_ReqUserQuestInfo(int obj_index);
    void UserQuestInfoLoad(PMSG_ANS_QUESTEXP_INFO* lpRecv);
    UserQuestInfo* GetUserQuestInfo(int episode_number, int obj_index);

    int GetQuestProgState(DWORD quest_info_index_id, int obj_index);
    int GetQuestProgState(int ep, int obj_index);

    bool IsQuestAccept(DWORD quest_info_index_id, int obj_index);
    bool InventoryExtentCheck(int x, int y, int w, int h);
    int CheckOutOfInventory(int aIndex, int sy, int height);
};

extern QuestExpUserMng g_QuestExpUserMng;
#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


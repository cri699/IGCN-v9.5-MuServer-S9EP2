//////////////////////////////////////////////////////////////////////
// QuestExpProgMng.h
#ifndef QUESTEXPPROGMNG_H
#define QUESTEXPPROGMNG_H

#pragma once

class QuestExpInfo;
class QuestBuff;
class UserQuestAskInfo;
class QuestNeedZen;
class QuestSkillLearn;
class QuestLevelUp;
class QuestGetItem;
class QuestMonsterKill;
class QuestDropItemInfo;

struct PMSG_QUEST_ASK_STATE;
struct OBJECTSTRUCT;

struct NPC_QUESTEXP_INFO;

class QuestExpProgMng
{
public:
	QuestExpProgMng(void);
	~QuestExpProgMng(void);

	void ReqQuestAskComplete(DWORD dwQuestInfoIndexID, int iObjIndex);

	void QuestExpGiveUpBtnClick(DWORD dwQuestInfoIndexID, int iObjIndex);

	void SendQuestProgress(DWORD dwQuestInfoIndexID, int iObjIndex);
	void SendQuestProgress(QuestExpInfo* pQuestExpInfo, DWORD dwQuestInfoIndexID, int iObjIndex);
	void SendQuestProgress(int iEpisode, int iQS, int iObjIndex);

	void SendQuestProgressInfo(DWORD dwQuestInfoIndexID, int iObjIndex);
	void SendQuestProgressInfo(QuestExpInfo* pQuestExpInfo, DWORD dwQuestInfoIndexID, int iObjIndex);

	bool ChkQuestAsk(int iEpisode, int iObjIndex);
	bool GetSendQuestInfo(QuestExpInfo* pQuestExpInfo, DWORD dwQuestInfoIndexID, int iObjIndex, NPC_QUESTEXP_INFO* pNpcQuestExpInfo);

	void ChkUserQuestTypeBuff(QuestBuff* pQuestBuff, UserQuestAskInfo* pUserQuestAskInfo, int iObjIndex);
	void ChkUserQuestTypeNeedZen(QuestNeedZen* pQuestNeedZen, UserQuestAskInfo* pUserQuestAskInfo, int iObjIndex);
	void ChkUserQuestTypeSkillLearn(QuestSkillLearn* pQuestSkillLearn, UserQuestAskInfo* pUserQuestAskInfo, int iObjIndex);
	void ChkUserQuestTypeLevelUp(QuestLevelUp* pQuestLevelUp, UserQuestAskInfo* pUserQuestAskInfo, int iObjIndex);

	void DeleteInventoryItem(QuestGetItem* pQuestGetItem, int iObjIndex);
	void ChkUserQuestTypeItem(QuestGetItem* pQuestGetItem, UserQuestAskInfo* pUserQuestAskInfo, int iObjIndex);

	void ChkMonsterKillPartyPlay(DWORD dwQuestInfoIndexID, int iAskIndex, QuestMonsterKill* pQuestMonsterKill, OBJECTSTRUCT* lpObj, OBJECTSTRUCT* lpMonsterObj);
	void ChkUserQuestTypeMonsterKill(OBJECTSTRUCT *lpObj, OBJECTSTRUCT *lpMonsterObj);

	void ChkUserQuestType(int iQuestType, OBJECTSTRUCT *lpObj, OBJECTSTRUCT *lpMonsterObj, int iKillerObjIndex);

	void ChkUserQuestTypeEventMap(int iQuestType, OBJECTSTRUCT *lpObj, int iMapLevel, int iValue);

	void AddQuestSwitchList(int iEpisode, int iQS);
	void SendQuestSwitchList(int iObjIndex, int iSendType);
	void SendQuestAskState(PMSG_QUEST_ASK_STATE* pMsg, int iObjIndex);
	void GCANSQuestCompleteBtnClick(int iObjIndex, DWORD dwQuestInfoIndexID, BYTE btResult);
	void SendProgressQuestList(int iObjIndex);

	void SetQuestProg(int iEpisode, int iObjIndex, int iState);

	bool ChkQuestIndexIDToEpLimit(DWORD dwQuestInfoIndexID, int iObjIndex);
	bool ChkQuestIndexIDToQSLimit(DWORD dwQuestInfoIndexID, int iObjIndex);
	bool ChkQuestIndexIDToEpAndQSLimit(DWORD dwQuestInfoIndexID, int iObjIndex);

	void QuestMonsterItemDrop(DWORD dwQuestInfoIndexID, OBJECTSTRUCT *lpObj, OBJECTSTRUCT *lpMonsterObj);
	bool ChkQuestMonsterItemDrop(QuestDropItemInfo* pQuestDropItemInfo);
	bool ChkQuestMonsterItemDrop(DWORD dwQuestInfoIndexID);
	bool IsQuestDropItem(int iIndex, WORD nType, WORD nLevel);

	void SetQuestTimeLimit(int iObjIndex, DWORD dwQuestIndexID, int iDuration);

	void SendQuestAskInfoUpdate(int iEpisode, int iQS, int iAskType, int iAskCount, int iObjIndex);

	int	GetCheatChangeDay() { return this->m_iChangeDay; };
	void SetCheatChangeDay(int iDay) { this->m_iChangeDay = iDay; }

	time_t GetCurrentDate();
	time_t GetExpireDate(int iDuration);
	bool CheckExpireDate(time_t lQuestExpireDate);
	time_t GetLeftDate(time_t lExpireDate);

private:
	std::vector<int>	m_vtQuestListNpcTalk;
	int					m_iChangeDay;
};

extern QuestExpProgMng g_QuestExpProgMng;
#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


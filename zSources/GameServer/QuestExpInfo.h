//////////////////////////////////////////////////////////////////////
// QuestExpInfo.h
#ifndef QUESTEXPINFO_H
#define QUESTEXPINFO_H

#pragma once
#include "QuestExpDefine.h"
#include "zzzitem.h"

class QuestExpAsk
{
public:
	QuestExpAsk()
	{
		this->m_iIndexID = 0;
		this->m_iQuestType = 0;
	}

	virtual ~QuestExpAsk()
	{

	}

	int GetQuestType() { return this->m_iQuestType; }
	void SetQuestType(int iQuestType) { this->m_iQuestType = iQuestType; }

	int GetIndexID() { return this->m_iIndexID; }
	void SetIndexID(int iIndexID) { this->m_iIndexID = iIndexID; }

protected:
	int m_iQuestType;
	int m_iIndexID;
};

class QuestGetItem : public QuestExpAsk
{
public:
	QuestGetItem()
	{
		this->m_iQuestItemNum = 0;
		this->m_iQuestItemCnt = 0;
		this->m_btItemLevel = 0;
		this->m_btItemDur = 0;
		this->m_btItemSkill = 0;
		this->m_btItemOpt = 0;
		this->m_btItemExOpt = 0;
	}

	virtual ~QuestGetItem()
	{

	}

	int GetQuestItemNum() { return this->m_iQuestItemNum; }
	void SetQuestItemNum(int iItemType, int iItemIndex) { this->m_iQuestItemNum = ITEMGET(iItemType, iItemIndex); }

	int GetQuestItemCnt() { return this->m_iQuestItemCnt; }
	void SetQuestItemCnt(int iQuestItemCnt) { this->m_iQuestItemCnt = iQuestItemCnt; }

	BYTE GetQuestItemLevel() { return this->m_btItemLevel; }
	void SetQuestItemLevel(BYTE btItemLevel) { this->m_btItemLevel = btItemLevel; }

	BYTE GetQuestItemSkill() { return this->m_btItemSkill; }
	void SetQuestItemSkill(BYTE btItemSkill) { this->m_btItemSkill = btItemSkill; }

	BYTE GetQuestItemDur() { return this->m_btItemDur; }
	void SetQuestItemDur(BYTE btItemDur) { this->m_btItemDur = btItemDur; }

	BYTE GetQuestItemExOpt() { return this->m_btItemExOpt; }
	void SetQuestItemExOpt(BYTE btItemExOpt) { this->m_btItemExOpt = btItemExOpt; }

	BYTE GetQuestItemOpt() { return this->m_btItemOpt; }
	void SetQuestItemOpt(BYTE btItemOpt) { this->m_btItemOpt = btItemOpt; }

private:
	int m_iQuestItemNum;
	int m_iQuestItemCnt;
	BYTE m_btItemLevel;
	BYTE m_btItemDur;
	BYTE m_btItemSkill;
	BYTE m_btItemExOpt;
	BYTE m_btItemOpt;
};

class QuestMonsterKill : public QuestExpAsk
{
public:
	QuestMonsterKill()
	{
		this->m_iMonsterKillCnt = 0;
		this->m_iMonsterIndex = 0;
		this->m_iPartyPlay = 0;
	}

	virtual ~QuestMonsterKill()
	{

	}

	int GetMonsterKillCnt() { return this->m_iMonsterKillCnt; }
	void SetMonsterKillCnt(int iMonsterKillCnt) { this->m_iMonsterKillCnt = iMonsterKillCnt; }

	int GetMonsterIndex() { return this->m_iMonsterIndex; }
	void SetMonsterIndex(int iMonsterIndex) { this->m_iMonsterIndex = iMonsterIndex; }

	bool IsPartyPlay() { return this->m_iPartyPlay; }
	void SetPartyPlay(int iPartyPlay) { this->m_iPartyPlay = iPartyPlay; }

private:
	int m_iMonsterKillCnt;
	int m_iMonsterIndex;
	int m_iPartyPlay;
};

class QuestTutorial : public QuestExpAsk
{
public:
	QuestTutorial()
	{

	}

	virtual ~QuestTutorial()
	{

	}
};

class QuestProgress : public QuestExpAsk
{
public:
	QuestProgress()
	{

	}

	virtual ~QuestProgress()
	{

	}
};

class QuestSkillLearn : public QuestExpAsk
{
public:
	QuestSkillLearn()
	{
		this->m_iSkillIndex = 0;
	}

	virtual ~QuestSkillLearn()
	{

	}

	int GetQuestSkillIndex() { return this->m_iSkillIndex; }
	void SetQuestSkillIndex(int iSkillIndex) { this->m_iSkillIndex = iSkillIndex; }

private:
	int m_iSkillIndex;
};

class QuestLevelUp : public QuestExpAsk
{
public:
	QuestLevelUp()
	{
		this->m_iLevelValue = 0;
	}

	virtual ~QuestLevelUp()
	{

	}

	int GetQuestLevelVal() { return this->m_iLevelValue; }
	void SetQuestLevelVal(int iLevelValue) { this->m_iLevelValue = iLevelValue; }

private:
	int m_iLevelValue;
};

class QuestBuff : public QuestExpAsk
{
public:
	QuestBuff()
	{
		this->m_iBuffIndex = 0;
	}

	virtual ~QuestBuff()
	{

	}

	int GetQuestBuffIndex() { return this->m_iBuffIndex; }
	void SetQuestBuffIndex(int iBuffIndex) { this->m_iBuffIndex = iBuffIndex; }

private:
	int m_iBuffIndex;
};

class QuestNeedZen : public QuestExpAsk
{
public:
	QuestNeedZen()
	{
		this->m_iNeedZen = 0;
	}

	virtual ~QuestNeedZen()
	{

	}

	int GetQuestNeedZen() { return this->m_iNeedZen; }
	void SetQuestNeedZen(int iNeedZen) { this->m_iNeedZen = iNeedZen; }

private:
	int m_iNeedZen;
};

class QuestPVP_Point : public QuestExpAsk
{
public:
	QuestPVP_Point()
	{
		this->m_iPVPPoint = 0;
		this->m_iMapNum = 0;
	}

	virtual ~QuestPVP_Point()
	{

	}

	int GetQuestPVP_Point() { return this->m_iPVPPoint; }
	void SetQuestPVP_Point(int iPVPPoint) { this->m_iPVPPoint = iPVPPoint; }

	int GetQuestMapNum() { return this->m_iMapNum; }
	void SetQuestMapNum(int iMapNum) { this->m_iMapNum = iMapNum; }

private:
	int m_iPVPPoint;
	int m_iMapNum;
};

class QuestNpcTalk : public QuestExpAsk
{
public:
	QuestNpcTalk()
	{
		this->m_iNPCIndex = 0;
	}

	virtual ~QuestNpcTalk()
	{

	}

	int GetQuestNPCTalk() { return this->m_iNPCIndex; }
	void SetQuestNPCTalk(int iNPCIndex) { this->m_iNPCIndex = iNPCIndex; }

private:
	int m_iNPCIndex;
};

class QuestEventMapKillPoint : public QuestExpAsk
{
public:
	QuestEventMapKillPoint()
	{
		this->m_iKillCnt = 0;
		this->m_iEventMapLevel = -1;
		this->m_iMonsterIndex = 0;
	}

	virtual ~QuestEventMapKillPoint()
	{

	}

	int GetQuestEventMapKillCnt() { return this->m_iKillCnt; }
	void SetQuestEventMapKillCnt(int iKillCnt) { this->m_iKillCnt = iKillCnt; }

	int GetQuestEventMapLevel() { return this->m_iEventMapLevel; }
	void SetQuestEventMapLevel(int iEventMapLevel) { this->m_iEventMapLevel = iEventMapLevel; }

private:
	int m_iKillCnt;
	int m_iEventMapLevel;
	int m_iMonsterIndex;
};

class QuestEventMapClear : public QuestExpAsk
{
public:
	QuestEventMapClear()
	{
		this->m_iEventMapLevel = 0;
	}

	virtual ~QuestEventMapClear()
	{

	}

	int GetQuestEventMapLevel() { return this->m_iEventMapLevel; }
	void SetQuestEventMapLevel(int iEventMapLevel) { this->m_iEventMapLevel = iEventMapLevel; }

private:
	int m_iEventMapLevel;
};

class QuestEventMapDevilPoint : public QuestExpAsk
{
public:
	QuestEventMapDevilPoint()
	{
		this->m_iDevilPoint = 0;
		this->m_iEventMapLevel = 0;
	}

	virtual ~QuestEventMapDevilPoint()
	{

	}

	int GetQuestEventMapDevilPoint() { return this->m_iDevilPoint; }
	void SetQuestEventMapDevilPoint(int iDevilPoint) { this->m_iDevilPoint = iDevilPoint; }

	int GetQuestEventMapLevel() { return this->m_iEventMapLevel; }
	void SetQuestEventMapLevel(int iEventMapLevel) { this->m_iEventMapLevel = iEventMapLevel; }

private:
	int m_iDevilPoint;
	int m_iEventMapLevel;
};

class QuestExpReward
{
public:
	QuestExpReward()
	{
		this->m_iRewardType = 0;
		this->m_iIndexID = 0;
	}

	virtual ~QuestExpReward()
	{

	}

	int GetRewardType() { return this->m_iRewardType; }
	void SetRewardType(int iRewardType) { this->m_iRewardType = iRewardType; }

	int GetIndexID() { return this->m_iIndexID; }
	void SetIndexID(int iIndexID) { this->m_iIndexID = iIndexID; }

protected:
	int m_iRewardType;
	int m_iIndexID;

};

class RewardExp : public QuestExpReward
{
public:

	RewardExp()
	{

	}

	virtual ~RewardExp()
	{

	}

	UINT64 GetRewardExp() { return this->m_iExp; }
	void SetRewardExp(UINT64 iExp) { this->m_iExp = iExp; }

private:
	UINT64 m_iExp;
};

class RewardItem : public QuestExpReward
{
public:

	RewardItem()
	{
		this->m_iItemNum = 0;
		this->m_btItemLevel = 0;
		this->m_btItemDur = 0;
		this->m_iItemCnt = 0;
		this->m_btItemSkill = 0;
		this->m_btItemOpt = 0;
		this->m_btItemExOpt = 0;
		this->m_btItemOverlap = 0;
	}

	virtual ~RewardItem()
	{

	}

	int GetRewardItemNum() { return this->m_iItemNum; }
	void SetRewardItemNum(int iItemType, int iItemIndex) { this->m_iItemNum = ITEMGET(iItemType, iItemIndex); }

	int GetRewardItemCnt() { return this->m_iItemCnt; }
	void SetRewardItemCnt(int iItemCnt) { this->m_iItemCnt = iItemCnt; }

	BYTE GetRewardItemLevel() { return this->m_btItemLevel; }
	void SetRewardItemLevel(BYTE btItemLevel) { this->m_btItemLevel = btItemLevel; }

	BYTE GetRewardItemSkill() { return this->m_btItemSkill; }
	void SetRewardItemSkill(BYTE btItemSkill) { this->m_btItemSkill = btItemSkill; }

	BYTE GetRewardItemDur() { return this->m_btItemDur; }
	void SetRewardItemDur(BYTE btItemDur) { this->m_btItemDur = btItemDur; }

	BYTE GetRewardItemExOpt() { return this->m_btItemExOpt; }
	void SetRewardItemExOpt(BYTE btItemExOpt) { this->m_btItemExOpt = btItemExOpt; }

	BYTE GetRewardItemOpt() { return this->m_btItemOpt; }
	void SetRewardItemOpt(BYTE btItemOpt) { this->m_btItemOpt = btItemOpt; }

	BYTE GetRewardItemOverlap() { return this->m_btItemOverlap; }
	void SetRewardItemOverlap(BYTE btItemOverlap) { this->m_btItemOverlap = btItemOverlap; }

private:
	int m_iItemNum;
	BYTE m_btItemLevel;
	BYTE m_btItemDur;
	int m_iItemCnt;
	BYTE m_btItemSkill;
	BYTE m_btItemOpt;
	BYTE m_btItemExOpt;
	BYTE m_btItemOverlap;
};

class RewardContribute : public QuestExpReward
{
public:
	RewardContribute()
	{
		this->m_iContributePoint = 0;
	}

	virtual ~RewardContribute()
	{

	}

	int GetRewardContributePoint() { return this->m_iContributePoint; }
	void SetRewardContributePoint(int iContributePoint) { this->m_iContributePoint = iContributePoint; }

private:
	int m_iContributePoint;
};

class RewardZen : public QuestExpReward
{
public:
	RewardZen()
	{
		this->m_iZen = 0;
	}

	virtual ~RewardZen()
	{

	}

	int GetRewardZen() { return this->m_iZen; }
	void SetRewardZen(int iZen) { this->m_iZen = iZen; }

private:
	int m_iZen;
};

class QuestExpRewardKind
{
public:
	QuestExpRewardKind()
	{
		this->m_iQuestEpisode = 0;
		this->m_iQuestSwitch = 0;
		this->m_iRewardKind = 0;
	}

	virtual ~QuestExpRewardKind()
	{

	}

	int GetQuestEpisode() { return this->m_iQuestEpisode; }
	void SetQuestEpisode(int iQuestEpisode) { this->m_iQuestEpisode = iQuestEpisode; }

	int GetQuestSwitch() { return this->m_iQuestSwitch; }
	void SetQuestSwitch(int iQuestSwitch) { this->m_iQuestSwitch = iQuestSwitch; }

	int GetRewardKind() { return this->m_iRewardKind; }
	void SetRewardKind(int iRewardKind) { this->m_iRewardKind = iRewardKind; }

protected:
	int m_iQuestEpisode;
	int m_iQuestSwitch;
	int m_iRewardKind;
};

class QuestExpRewardKindRandom : public QuestExpRewardKind
{
public:
	QuestExpRewardKindRandom()
	{
		memset(this->m_iRewardIndex, -1, sizeof(this->m_iRewardIndex));
		memset(this->m_iRewardRate, -1, sizeof(this->m_iRewardRate));
	}

	virtual ~QuestExpRewardKindRandom()
	{

	}

	int GetRandRewardCnt() { return this->m_iRandRewardCnt; }
	void SetRandRewardCnt(int iRandRewardCnt) { this->m_iRandRewardCnt = iRandRewardCnt; }

	int GetRewardItemTotCnt() { return this->m_iRewardItemTotCnt; }
	void SetRewardItemTotCnt(int iRewardItemTotCnt) { this->m_iRewardItemTotCnt = iRewardItemTotCnt; }

	int* GetRewardIndex() { return this->m_iRewardIndex; }
	void SetRewardIndex(int* iRewardIndex)
	{
		memcpy(this->m_iRewardIndex, iRewardIndex, sizeof(this->m_iRewardIndex));
	}

	int* GetRewardRate() { return this->m_iRewardRate; }

	void SetRewardRate(int* iRewardRate)
	{
		memcpy(this->m_iRewardRate, iRewardRate, sizeof(this->m_iRewardRate));
	}

private:
	int m_iRandRewardCnt;
	int m_iRewardItemTotCnt;
	int m_iRewardIndex[MAX_QUESTEXP_REWARDKIND_RANDOM];
	int m_iRewardRate[MAX_QUESTEXP_REWARDKIND_RANDOM];
};

class QuestDropItemInfo
{
public:
	QuestDropItemInfo()
	{
		this->m_iQuestEpisode = 0;
		this->m_iQuestSwitch = 0;
		this->m_iAskIndex = 0;
		this->m_iMonsterIndex = 0;
		this->m_iDropRate = 0;
		this->m_iItemNum = 0;
		this->m_btItemLevel = 0;
		this->m_iItemCnt = 0;
	}

	~QuestDropItemInfo()
	{

	}

	int GetQuestEpisode() { return this->m_iQuestEpisode; }
	void SetQuestEpisode(int iQuestEpisode) { this->m_iQuestEpisode = iQuestEpisode; }

	int GetQuestSwitch() { return this->m_iQuestSwitch; }
	void SetQuestSwitch(int iQuestSwitch) { this->m_iQuestSwitch = iQuestSwitch; }

	int GetAskIndex() { return this->m_iAskIndex; }
	void SetAskIndex(int iAskIndex) { this->m_iAskIndex = iAskIndex; }

	int GetMonsterIndex() { return this->m_iMonsterIndex; }
	void SetMonsterIndex(int iMonsterIndex) { this->m_iMonsterIndex = iMonsterIndex; }

	int GetDropRate() { return this->m_iDropRate; }
	void SetDropRate(int iDropRate) { this->m_iDropRate = iDropRate; }

	int GetItemNum() { return this->m_iItemNum; }
	void SetItemNum(int iItemType, int iItemIndex) { this->m_iItemNum = ITEMGET(iItemType, iItemIndex); }

	BYTE GetItemLevel() { return this->m_btItemLevel; }
	void SetItemLevel(BYTE btItemLevel) { this->m_btItemLevel = btItemLevel; }

	int GetItemCnt() { return this->m_iItemCnt; }
	void SetItemCnt(int iItemCnt) { this->m_iItemCnt = iItemCnt; }

private:
	int m_iQuestEpisode;
	int m_iQuestSwitch;
	int m_iAskIndex;
	int m_iMonsterIndex;
	int m_iDropRate;
	int m_iItemNum;
	BYTE m_btItemLevel;
	int m_iItemCnt;
};

class QuestExpInfo
{
public:

	QuestExpInfo();
	~QuestExpInfo();

	int GetQuestEpisode() { return this->m_iQuestEpisode; }
	void SetQuestEpisode(int iQuestEpisode) { this->m_iQuestEpisode = iQuestEpisode; }

	int GetQuestSwitch() { return this->m_iQuestSwitch; }
	void SetQuestSwitch(int iQuestSwitch) { this->m_iQuestSwitch = iQuestSwitch; }

	int GetAskCnt() { return this->m_iIndexID; }
	void SetAskCnt(int iAskCnt) { this->m_iIndexID = iAskCnt; }

	int GetRewardCnt() { return this->m_iRewardID; }
	void SetRewardCnt(int iRewardCnt) { this->m_iRewardID = iRewardCnt; }

	void SetQuestExpAsk(QuestExpAsk *pQuestExpAsk);
	void SetQuestExpAsk(QuestExpAsk *pQuestExpAsk, int iIndexID);
	std::map<int, QuestExpAsk*> GetQuestExpAsk();

	void SetQuestReward(QuestExpReward *pQuestExpReward);
	std::map<int, QuestExpReward*> GetQuestReward();

	void Release();

private:
	int m_iQuestEpisode;
	int m_iQuestSwitch;
	int m_iIndexID;
	int m_iRewardID;
	std::map<int, QuestExpAsk*> m_MapQuestExpAsk;
	std::map<int, QuestExpReward*> m_MapQuestReward;
};

struct QUEST_ITEM_INFO
{
	QUEST_ITEM_INFO()
	{
		this->iItemNum = 0;
		this->dwQuestItemAtt = 0;
		this->iEp = 0;
	}

	int iItemNum;
	DWORD dwQuestItemAtt;
	int iEp;
};

class QuestExpManager
{
public:

	QuestExpManager();
	~QuestExpManager();

	void QuestExpItemInit();
	void QuestInfoDelete();

	void SetQuestExpInfo(QuestExpAsk *pQuestExpAsk, int iEpisodeNum, int iQuestSwitch);
	void SetQuestExpInfo(QuestExpReward* pQuestExpReward, int iEpisodeNum, int iQuestSwitch);

	void AddQuestExpInfoList(QuestExpAsk* pQuestExpAsk, DWORD dwQuestInfoIndexID);
	void AddQuestExpInfoList(QuestExpReward *pQuestExpReward, DWORD dwQuestInfoIndexID);
	void AddQuestExpInfoList(QuestExpInfo* pQuestExpInfo, DWORD dwQuestInfoIndexID);

	void AddQuestExpRewardKind(QuestExpRewardKind* pQuestExpRewardKind, DWORD dwQuestInfoIndexID);
	void AddQuestExpRewardKind(QuestExpRewardKind* pQuestExpRewardKind);

	QuestExpRewardKind* GetQuestExpRewardKind(DWORD dwQuestInfoIndexID);

	QuestExpAsk* GetQuestExpAsk(int iEpisodeNum, int iQuestSwitch, int iIndexID);
	QuestExpAsk* GetQuestExpAsk(DWORD dwQuestInfoIndexID, int iIndexID);

	QuestExpReward* GetQuestExpReward(int iEpisodeNum, int iQuestSwitch, int iIndexID);

	QuestExpInfo* GetQuestExpInfo(DWORD dwQuestInfoIndexID);
	QuestExpInfo* GetQuestExpInfo(int iEpisodeNum, int iQuestSwitch);

	void AddQuestDropItemInfo(QuestDropItemInfo* pQuestDropItemInfo, DWORD dwQuestInfoIndexID);

	QuestDropItemInfo* GetQuestDropItemInfo(DWORD dwQuestInfoIndexID);
	void AddQuestItemInfo(QUEST_ITEM_INFO stQuestItemList);
	bool IsQuestItemInfo(int iItemNum);
	bool IsQuestItemAtt(int iItemNum, DWORD dwQuestItemAtt);
	int GetQuestItemEp(int iItemNum);

private:
	std::map<int, QuestExpInfo> m_Map_QuestExpInfoList;
	std::map<DWORD, QuestExpRewardKind*> m_MapQuestExpRewardKind;
	std::map<DWORD, QuestDropItemInfo> m_MapQuestDropItemInfo;
	std::vector<QUEST_ITEM_INFO> m_vtQuestItemInfo;
};

extern QuestExpManager g_QuestExpManager;

#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


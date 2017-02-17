//////////////////////////////////////////////////////////////////////
// DoppelGangerEventInfo.h
#pragma once

struct _tagDOPPELGANGER_EVENT_INFO
{
	int nUserMaxLevel;
	float fMonsterLevel[5];
	float fMonsterHp[5];
	float fMonsterAttack[5];
	float fMonsterDefense[5];
	int nMiddleRewardItemDropCnt;
	int nMiddleItemBagIndex[3];
	int nMiddleItemBagDropRate[3];
	int nLastRewardItemDropCnt;
	int nLastItemBagIndex[6];
	int nLastItemBagDropRate[6];
};

class CDoppelGangerEventInfo
{
public:
	CDoppelGangerEventInfo(void);
	virtual ~CDoppelGangerEventInfo(void);
	void LoadDoppelgangerEventInfo(LPSTR lpFile);
	void ClearDoppelgangerEventInfo();
	_tagDOPPELGANGER_EVENT_INFO * GetDoppelgangerEventInfo(int nUserMaxLevel);
	BOOL SetDoppelgangerEventInfo(int nUserCount, int nUserMaxLevel);

	float GetMonsterLevelWeight();
	float GetMonsterHpWeight();
	float GetMonsterAttackWeight();
	float GetMonsterDefenseWeight();
	int GetMiddleItemDropCnt();
	int GetMiddleItemBagIndex(int nRewardIndex);
	int GetMiddleItemDropRate(int nRewardIndex);
	int GetLastItemDropCnt();
	int GetLastItemBagIndex(int nRewardIndex);
	int GetLastItemDropRate(int nRewardIndex);

private:
	bool													m_bLoad;
	std::map<int, _tagDOPPELGANGER_EVENT_INFO>				m_mapDoppelEvent_Info;
	_tagDOPPELGANGER_EVENT_INFO								m_DoppelgangerInfo;
	int														m_nDoppelgangerUserCnt;

};



//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


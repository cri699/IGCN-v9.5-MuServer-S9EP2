//////////////////////////////////////////////////////////////////////
// MineSystem.h
#ifndef MINESYSTEM_H
#define MINESYSTEM_H

#include "user.h"

struct _ST_MINESYSTEM_REPAIR_VALUE
{
	WORD sItemType;
	WORD sItemIndex;
	WORD sLevel;
	WORD sRepairValue;
};

struct _ST_MINESYSTEM_MINE_MIRACLE_SUCCESS_INFO
{
	WORD wTwinkleType;
	int iStage;
	int iSuccessRate;
};

struct _ST_MINESYSTEM_DROP_ITEM
{
	int iItemType;
	int iItemIndex;
	int iLevel;
	int iNumber;
};

struct _ST_MINESYSTEM_MINE_SUCCESS_INFO
{
	WORD wTwinkleType;
	int iDurabilityDecrement;
	int iSuccessRate[5];
};

struct _ST_MINESYSTEM_REWARD_VALUE
{
	_ST_MINESYSTEM_REWARD_VALUE()
	{
		this->wTwinkleType = 0;
		this->iStage = 0;
		this->wType = 0;
		this->wIndex = 0;
		this->iTotalJewelNumber = 0;
		this->vecDropItem.clear();
	}

	WORD wTwinkleType;
	int iStage;
	WORD wType;
	WORD wIndex;
	int iTotalJewelNumber;
	std::vector<_ST_MINESYSTEM_DROP_ITEM> vecDropItem;
};

struct _ST_MINESYSTEM_TWINKLE
{
	_ST_MINESYSTEM_TWINKLE()
	{
		this->wTwinkleIndex = 0;
		this->wUserIndex = 0;
		this->iCurrentStage = 0;
		this->wTwinkleType = 0;
		this->bIsDominated = 0;
		this->byMapNumber = 0;
	}

	WORD wTwinkleIndex;
	WORD wTwinkleType;
	WORD wUserIndex;
	int iCurrentStage;
	int bIsDominated;
	BYTE byMapNumber;
};

struct _ST_MINESYSTEM_TWINKLE_SPAWN
{
	WORD wType;
	BYTE byMapNumber;
	BYTE byDistance;
	BYTE byX;
	BYTE byY;
	BYTE byDir;
};

// DS

struct SDHP_REQ_LOAD_MINESYSTEM_UPT_USERINFO
{
	PBMSG_HEAD2 h;
	WORD wUserIndex;
	char szCharName[MAX_ACCOUNT_LEN + 1];
};

struct SDHP_REQ_MINESYSTEM_UPT_USERINFO
{
	PBMSG_HEAD2 h;
	WORD wUserIndex;
	char szCharName[MAX_ACCOUNT_LEN + 1];
	WORD wTwinkleType;
	int iCurrentStage;
	BYTE byRequestType;
};

struct SDHP_ANS_MINESYSTEM_UPT_USERINFO
{
	PBMSG_HEAD2 h;
	WORD wUserIndex;
	BYTE byRequestType;
	BYTE Result;
};

struct SDHP_ANS_LOAD_MINESYSTEM_UPT_USERINFO
{
	PBMSG_HEAD2 h;
	WORD wUserIndex;
	BYTE byResult;
	char szCharName[MAX_ACCOUNT_LEN + 1];
	WORD wTwinkleType;
	int iCurrentStage;
};

#pragma once
class CMineSystem
{
public:
	CMineSystem();
	virtual ~CMineSystem();

	void LoadMineingDropScript();

	void CheckIsUPTUserWhenDisconnected(int aIndex);
	void CheckIsUPTUserWhenConnect(int aIndex);
	void GiveRewardItemToUPTUser(int aIndex, WORD wTwinkleType, int iStage);
	
	void MineTwinkle(PMSG_REQ_MINETWINKLE *lpMsg, int aIndex);
	void RewardMineTwinkle(PMSG_REQ_MINETWINKLE_REWARD *lpMsg, int aIndex);
	void FailMineTwinkle(PMSG_ANS_MINETWINKLE_END_ANIMATION *lpMsg, int aIndex, bool bError);

	void RequestDBToModifyUPTUserInfo(int aIndex, char *szCharName, WORD wTwinkleType, int iCurrentStage, BYTE byRequestType);
	void GDAnsModifyMineSystemUPTUserInfo(SDHP_ANS_MINESYSTEM_UPT_USERINFO *lpMsg);
	void GDAnsLoadMineSystemUPTUserInfo(SDHP_ANS_LOAD_MINESYSTEM_UPT_USERINFO *lpMsg);

	BOOL CheckMoveMapWhileMining(int aIndex);
	BOOL IsTwinkle(WORD wClass);
	BOOL IsEquipPickax(int aIndex);
	BOOL IsPickax(WORD wItemType);

	void SetTwinklesInfo();
	void SetTwinkleInfo(int aIndex);
	void ResetTwinkleInfo(int aIndex, WORD wTwinkleIndex, bool bFailMining);

	BOOL SuccessOrFailure(WORD wType, int iStage);
	BOOL MiracleSuccessOrFailure(WORD wType, int iStage);
	BOOL CheckValidationMineState(int aIndex, WORD wTwinkleIndex, WORD wTwinkleType, BYTE byMapNumber, WORD wUserIndex, int *iResult, bool bRewardCheck);
	BOOL GiveRewardItem(int aIndex, WORD wTwinkleType, int iStage, int bNotify);

	void ChangeUnconditionalSuccess(int iIsSuccess);

	short GetRepairValueOfJewel(WORD sItemType, WORD sLevel);

private:
	int m_iChanceStartValue;
	int m_iChanceStartValue_Copy;
	std::map<int, _ST_MINESYSTEM_MINE_SUCCESS_INFO> m_mapMineSuccessInfo;
	std::vector<_ST_MINESYSTEM_MINE_MIRACLE_SUCCESS_INFO> m_vecMiracleSuccessInfo;
	std::vector<_ST_MINESYSTEM_REWARD_VALUE> m_vecRewardValue;
	std::vector<_ST_MINESYSTEM_REWARD_VALUE> m_vecMiracleRewardValue;
	std::vector<_ST_MINESYSTEM_REPAIR_VALUE> m_vecRepairValue;
	std::map<int, _ST_MINESYSTEM_TWINKLE_SPAWN> m_mapTwinkleSpawn;
	std::map<int, std::string> m_mapMapName;
	std::map<int, _ST_MINESYSTEM_TWINKLE> m_mapTwinkle;
};

extern CMineSystem g_MineSystem;

#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


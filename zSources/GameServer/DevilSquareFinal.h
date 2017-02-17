//////////////////////////////////////////////////////////////////////
// DevilSquareFinal.h
#ifndef DEVILSQUAREFINAL_H
#define DEVILSQUAREFINAL_H

#include "user.h"

struct DSF_TIME_RULE_INFO
{
	void Clear()
	{
		this->BeforeEnterMinute = 0;
		this->AfterEnterMinute = 0;
		this->EachRoundMinute = 0;
		this->AfterRoundSecond = 0;
		this->AfterBossRoundSecond = 0;
		this->CommentTime = 0;
	}

	int BeforeEnterMinute;
	int AfterEnterMinute;
	int EachRoundMinute;
	int AfterRoundSecond;
	int AfterBossRoundSecond;
	int CommentTime;
};

struct DSF_UNIT_INFO
{
	void Clear()
	{
		this->MonsterUnitNumber = 0;
		this->PartyMinUser = 0;
		this->MinTeamCount = 0;
		this->MinUserCount = 0;
		this->DSFMinLevel = 0;
		this->MaxEnterCount = 0;
		memset(this->MonsterRewardRate, 0x00, sizeof(this->MonsterRewardRate));
	}

	int MonsterUnitNumber;
	int PartyMinUser;
	int MinTeamCount;
	int MinUserCount;
	int DSFMinLevel;
	int MaxEnterCount;
	int MonsterRewardRate[3];
};

struct DSF_START_DAY
{
	void Clear()
	{
		this->LeagueDay1 = 0;
		this->LeagueDay2 = 0;
		this->LeagueDay3 = 0;
		this->LeagueDay4 = 0;
		this->SemiFinalDay1 = 0;
		this->SemiFinalDay2 = 0;
		this->FinalDay = 0;
	}

	int LeagueDay1;
	int LeagueDay2;
	int LeagueDay3;
	int LeagueDay4;
	int SemiFinalDay1;
	int SemiFinalDay2;
	int FinalDay;
};

struct DSF_START_HOUR_TYPE
{
	void Clear()
	{
		this->DSFHour = -1;
		this->DSFMin = -1;
		this->DSFBattleType = 0;
	}

	int DSFHour;
	int DSFMin;
	int DSFBattleType;
};

struct DSF_REWARD_DAY
{
	void Clear()
	{
		this->RewardStartDay = 0;
		this->RewardEndDay = 0;
	}

	int RewardStartDay;
	int RewardEndDay;
};

struct DSF_EVENT_MONSTER_RATE
{
	void Clear()
	{
		this->RoundNum = 0;
		this->Rate = 0;
	}

	int RoundNum;
	int Rate;
};

struct DSF_AFTER_ROUND_ADD_POINT
{
	void Clear()
	{
		this->RoundNum = 0;
		this->Point = 0;
	}

	int RoundNum;
	int Point;
};

struct DSF_MONSTER_INFO
{
	void Clear()
	{
		this->RoundNum = -1;
		this->MonsterType = -1;
		this->MonsterMaxNumber = -1;
		this->MonsterKind = -1;
		this->MonsterPoint = -1;
	}

	int RoundNum;
	int MonsterType;
	int MonsterMaxNumber;
	int MonsterKind;
	int MonsterPoint;
};

struct DSF_MONSTER_REGEN_INFO
{
	void Clear()
	{
		this->MonsterNum = -1;
		this->MonsterMapNum = -1;
		this->MonsterDis = -1;
		this->RegenX = -1;
		this->RegenY = -1;
		this->MonsterDir = -1;
		this->MonsterKind = -1;
	}

	int MonsterNum;
	int MonsterMapNum;
	int MonsterDis;
	int RegenX;
	int RegenY;
	int MonsterDir;
	int MonsterKind;
};

struct DSF_EVENT_MONSTER_INFO
{
	void Clear()
	{
		this->RoundNum = 0;
		this->MonsterType = 0;
		this->MonsterMaxNumber = 0;
		this->MonsterKind = 0;
		this->MonsterPoint = 0;
	}

	int RoundNum;
	int MonsterType;
	int MonsterMaxNumber;
	int MonsterKind;
	int MonsterPoint;
};

struct DSF_PARTY
{
	int nPartyNo;
	int nRequestUserIndex;
	int n2ndPartyUserIndex;
	char szRequestUserID[MAX_ACCOUNT_LEN+1];
	char sz2ndPartyUserID[MAX_ACCOUNT_LEN+1];
	char szRequestUserName[MAX_ACCOUNT_LEN+1];
	char sz2ndPartyUserName[MAX_ACCOUNT_LEN+1];
	char szRequestUserRealName[MAX_ACCOUNT_LEN+1];
	char sz2ndPartyUserRealName[MAX_ACCOUNT_LEN+1];
	int nRequestUserServerCode;
	int n2ndPartyUserServerCode;
	int nRequestUserClass;
	int n2ndPartyUserClass;
	int nRequestUserLevel;
	int n2ndPartyUserLevel;
	bool bRequest;
	bool bEntered;
	BYTE btDSFType;
	BYTE btTeamIndex;
	int nPoint;
};

struct DSF_PARTY_RANK
{
	void Clear()
	{
		memset(this->szUserName1, 0x00, sizeof(this->szUserName1));
		memset(this->szUserName2, 0x00, sizeof(this->szUserName2));
		this->wServerCode1 = -1;
		this->wServerCode2 = -1;
		this->btRank = 0;
		this->btDSFType = 0;
		this->iPoint = 0;
	}

	char szUserName1[MAX_ACCOUNT_LEN+1];
	char szUserName2[MAX_ACCOUNT_LEN+1];
	WORD wServerCode1;
	WORD wServerCode2;
	BYTE btRank;
	BYTE btDSFType;
	int iPoint;
};

struct DSF_USER
{
	int m_nIndex;
	BYTE m_btTeam;
	BOOL m_bReward;
	bool m_bEnterOk;
	int m_nPartyIdx;
	int m_nPoint;
	char PlayerName[MAX_ACCOUNT_LEN+1];
};

struct DSF_ENTER_PLAYER_NAME
{
	int EnteredIndex;
	char PlayerName[MAX_ACCOUNT_LEN+1];
};

// DS <-> GS Protocol

struct PMSG_REQ_DSF_CAN_PARTY_ENTER
{
	PBMSG_HEAD2 h;
	int iUserIndex;
	char szAccountID1[MAX_ACCOUNT_LEN+1];
	char szUserName1[MAX_ACCOUNT_LEN+1];
	char szAccountID2[MAX_ACCOUNT_LEN+1];
	char szUserName2[MAX_ACCOUNT_LEN+1];
	BYTE btDSFType;
	int iEnterYear;
	BYTE btEnterMonth;
	BYTE btEnterDay;
};

struct PMSG_REQ_SAVE_DSF_PARTYPOINT
{
	PBMSG_HEAD2 h;
	char szAccountID1[MAX_ACCOUNT_LEN+1];
	char szUserName1[MAX_ACCOUNT_LEN+1];
	int nUserLevel1;
	char szAccountID2[MAX_ACCOUNT_LEN+1];
	char szUserName2[MAX_ACCOUNT_LEN+1];
	int nUserLevel2;
	int nDSFType;
	int nPoint;
	BYTE btEnterCnt;
	int nEnterYear;
	BYTE btEnterMonth;
	BYTE btEnterDay;
};

struct PMSG_REQ_DSF_PARTYRANKRENEW
{
	PBMSG_HEAD2 h;
	BYTE btDSFType;
	int nEnterYear;
	BYTE btEnterMonth;
	BYTE btEnterDay;
};

struct PMSG_REQ_DSF_GO_FINAL_PARTY
{
	PBMSG_HEAD2 h;
	BYTE btDSFType;
	int nEnterYear;
	BYTE btEnterMonth;
	BYTE btEnterDay;
};

struct PMSG_REQ_SAVE_DSF_REWARD_USER
{
	PBMSG_HEAD2 h;
	char szAccountID[MAX_ACCOUNT_LEN+1];
	char szUserName[MAX_ACCOUNT_LEN+1];
	int iClass;
	BYTE btDSFType;
	int nRewardYear;
	BYTE btRewardMonth;
	BYTE btRewardStartDay;
	BYTE btRewardEndDay;
};

struct PMSG_REQ_GET_DSF_REWARD
{
	PBMSG_HEAD2 h;
	char szAccountID[MAX_ACCOUNT_LEN+1];
	char szUserName[MAX_ACCOUNT_LEN+1];
	int nServerCode;
	int nUserIndex;
	int nRewardYear;
	BYTE btRewardMonth;
	BYTE btRewardDay;
};

struct PMSG_ANS_DSF_CAN_PARTY_ENTER
{
	PBMSG_HEAD2 h;
	int iUserIndex;
	BYTE btCount;
	BYTE btResult;
};

struct PMSG_ANS_DSF_PARTYRANKRENEW
{
	PBMSG_HEAD2 h;
	BYTE btResult;
};

struct PMSG_ANS_DSF_GO_FINAL_PARTY
{
	PWMSG_HEAD2 h;
	BYTE btPartyCnt;
	BYTE btDSFType;
};

struct PMSG_ANS_GET_DSF_REWARD
{
	PBMSG_HEAD2 h;
	int nUserIndex;
	BYTE btResult;
};

struct PMSG_REQ_SET_DSF_WINNER_INFO
{
	PBMSG_HEAD2 h;
	char UBFName[MAX_ACCOUNT_LEN + 1];
	BYTE btDSFType;
	BYTE btRewardType;
};

#pragma once
class CDevilSquareFinal
{
public:
	CDevilSquareFinal(void);
	~CDevilSquareFinal(void);

	void Initialize_DSFSchedule();
	void Load_DSFScheduleScript(char *pchFileName);

	void Initalize_DSFMonster();
	void Load_DSFMonsterScript(char *pchFileName);

	void ReqDSFSchedule(int aIndex);

	void DSFEventInit();

	BOOL CheckChangeRing(int nType);
	BOOL CheckWearingMOPH(int iUserIndex);
	
	void DSF_Run();

	BYTE WhatIsTodayTypeOfDSF();

	void Proc_DSF(int nCurTime);
	void Proc_DSFState_None(int nCurTime);
	void Proc_DSFState_Ready(int nCurTime);
	void Proc_DSFState_Playing(int nCurTime);
	void Proc_DSFState_RoundBreak(int nCurTime);
	void Proc_DSFState_End(int nCurTime);

	void SendAllUserAnyMsg(LPBYTE lpMsg, int iSize);

	void Set_DSFState(BYTE btState);
	void Set_DSFState_None();
	void Set_DSFState_Ready();
	void Set_DSFState_Playing();
	void Set_DSFState_RoundBreak();
	void Set_DSFState_End();

	BOOL IsAlreadyExistUserInDSF(int aIndex);

	char Find_PartySlot(int nPartyNumber, BYTE & btSlotNum);
	char Find_EmptyPartySlot(int nPartyNumber, BYTE & btSlotNum);
	char SetParty(BYTE btSlotOrder, BYTE btDSFType, int nPartyIdx, int nUserIndex);

	BOOL IsGoSemiFinalParty(char *szUserName1, char *szUserName2, WORD wServerCode1, WORD wServerCode2);
	BOOL IsGoFinalParty(char *szUserName1, char *szUserName2, WORD wServerCode1, WORD wServerCode2);

	void DeleteParty(int nPartyNumber);
	void DeletePartyByIndex(int nIndex);

	BOOL Enter_DSF(int aIndex, BYTE btSlotNum);
	BOOL Leave_DSF(int aIndex);

	void ClearUserData(int nIndex);

	void SendDSFState(BYTE btDSFState, int nIndex);
	void Send_DSF_PlayState(BYTE btTeam);

	void CheckUsersOnConnect();
	bool CanStartDSFBattle();

	void SendDSFResult(int aIndex);
	void SaveWinnerPartyPoint();
	
	void GDSaveDSFPartyPoint(char *szAccountID1, char *szUserName1, int nUser1Level, int nClass1, char *szAccountID2, char *szUserName2, int nUser2Level, int nClass2, int iDSFType, int iPoint, BYTE btEnterCnt);
	void GDReqDSFPartyRankRenew(BYTE btDSFType);
	void GDReqDSFGoFinalParty(int nDSFType);
	void GDInsertRewardUser(char *szAccountID, char *szUserName, int iClass, int iDSFType);
	void GDReqGetReward(int aIndex);

	void DSF_ProcessInit();

	int SetMonster();
	void DSFMonsterRegen(OBJECTSTRUCT * lpObj);
	void ClearMonster();

	BYTE GetUserTeam(int nIndex);

	int MonsterKillPoint(int nMonsterType);
	void MonsterKillPointCalc(int aIndex, int nPoint);

	void DSFUserDie(OBJECTSTRUCT * lpObj);
	void EndUserMove();
	
	void DSFDropReward(OBJECTSTRUCT * lpUser, OBJECTSTRUCT * lpMonster);
	void SetUBFGetReward(int iUserIndex, WORD wItemCode, UINT64 ItemSerial, BYTE btItemPos);
	void GDReqSetDSFReward_UBF(int iUserIndex, BYTE btDSFType, BYTE btRewardType);

	void GiveDSFWinnerItem(int iWinnerIndex, BYTE btDSFType, BYTE btRewardType);

public:
	bool m_bEVENT_ENABLE;
	int m_nDSF_TYPE;
	BYTE m_btDSFCurState;

	bool m_bGetGoFinalUser;

	int m_nTickCount;
	int m_nNextTimeMin;
	int m_nCurBattleStartTime;
	int m_nRemainTimeMin;
	int m_nRemainTimeMsec;

	bool m_bEntrance;
	bool m_bSendMsgBeforeReady;
	bool m_bRoundBreak;

	int m_nEventEndRemainTime;

	BYTE m_btDSFRound;

	bool m_bResultPointSend;
	bool m_bWinnerResultMsgSend;
	bool m_bGameStart;

	int m_nUserCount;
	int m_nTeamUserCount[10];
	int m_nTeamPoint[10];
	BYTE m_btWinTeam;

	int m_nDSFTestType;

	DSF_TIME_RULE_INFO m_DSFTimeRuleInfo;
	DSF_UNIT_INFO m_DSFUnitInfo;
	DSF_START_DAY m_DSFStartDay;
	DSF_START_HOUR_TYPE m_DSFStartHourType[25];
	DSF_REWARD_DAY m_DSFRewardDay;

	DSF_EVENT_MONSTER_RATE m_DSFEventMonsterRate[15];
	DSF_AFTER_ROUND_ADD_POINT m_DSFAfterRoundAddPoint[15];
	DSF_EVENT_MONSTER_INFO m_DSFEventMonsterInfo[15];
	DSF_MONSTER_INFO m_DSFMonsterInfo[15];
	DSF_MONSTER_REGEN_INFO m_DSFMonsterRegenInfo[200];

	CRITICAL_SECTION m_criPartyData;
	DSF_PARTY m_PartyDataSave[10];

	CRITICAL_SECTION m_critUserData;
	DSF_USER m_UserData[20];
	DSF_ENTER_PLAYER_NAME m_EnteredPlayerName[20];

	DSF_PARTY_RANK m_GoSemiFinalParty[40];
	DSF_PARTY_RANK m_GoFinalParty[10];
	DSF_PARTY_RANK m_WinnerParty;
};

extern CDevilSquareFinal g_DevilSquareFinal;

#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


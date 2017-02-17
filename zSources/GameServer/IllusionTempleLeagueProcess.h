//////////////////////////////////////////////////////////////////////
// IllusionTempleLeagueProcess.h
#ifndef ITL_PROCESS_H
#define ITL_PROCESS_H

#include "user.h"
#include <list>

struct __ITL_OBJECT_DATA
{
	int m_nIndex;
	WORD m_wType;
	WORD m_wPosNum;
	BYTE m_byStatus;
	BYTE byMapTagIndex;
};

struct __ITL_GUILD_DATA
{
	__ITL_GUILD_DATA()
	{
		this->nGuild = 0;
		this->bRequest = false;
		this->bEntered = false;
		this->byITLType = 0;
		this->nPartyNo = -1;
		this->bDBSave = false;
		this->byWin = -1;
		this->byLose = -1;
		this->byTeamIndex = -1;
		this->byOccupiedCount = 0;
		this->byEnterCount = 0;
		this->nRequestUserIndex = -1;
		memset(this->szGName, 0, MAX_GUILD_LEN+1);
	}

	int nGuild;
	int nRequestUserIndex;
	bool bRequest;
	bool bEntered;
	BYTE byITLType;
	int nPartyNo;
	char szGName[MAX_GUILD_LEN + 1];
	bool bDBSave;
	BYTE byWin;
	BYTE byLose;
	BYTE byTeamIndex;
	BYTE byOccupiedCount;
	BYTE byEnterCount;
};

struct __ITL_ENTER_PLAYER_NAME
{
	__ITL_ENTER_PLAYER_NAME()
	{
		this->EnteredIndex = -1;
		memset(this->PlayerName, 0, MAX_ACCOUNT_LEN + 1);
	}

	int EnteredIndex;
	char PlayerName[MAX_ACCOUNT_LEN + 1];
};

struct __ITL_TIME_TABLE
{
	BYTE nHour;
	BYTE nMin;
	BYTE nType;
};

struct __ITL_USER_DATA
{
	__ITL_USER_DATA()
	{
		this->m_nIndex = -1;
	}

	int m_nIndex;
	BYTE m_btTeam;
	int m_dwSkillProdectionTime;
	int m_dwSkillRestraintTime;
	BYTE m_btUserKillCount;
	BYTE m_byOccupiedCount;
	BYTE m_byInvalidationCount;
	bool m_bReward;
	bool m_bEnterOk;
	int m_nPartyIdx;
	int m_nGuildIdx;
	int m_nRelicsInvenPos;
};

struct ITL_NPC_POSITIONINFO
{
	WORD wNpcType;
	BYTE byMapNumber;
	BYTE byDistance;
	BYTE byX;
	BYTE byY;
	BYTE byDir;
	BYTE byMapTagIndex;
};

// DS

struct _tagPMSG_REQ_SAVE_ITL_GUILDPOINT
{
	PBMSG_HEAD2 h;
	char GuildName[MAX_GUILD_LEN + 1];
	int nType;
	BYTE byWin;
	BYTE byLose;
	BYTE byOccCnt;
	BYTE byEnterCnt;
};

struct _tagPMSG_REQ_SAVE_ITL_USERPOINT
{
	PBMSG_HEAD2 h;
	char UserName[MAX_ACCOUNT_LEN + 1];
	char GName[MAX_GUILD_LEN + 1];
	int nOccupiedCount;
	int nKillPoint;
	int nInvalidationCount;
	int nType;
	int nEnterCount;
	int nWin;
};

struct _tagPMSG_REQ_ITL_GUILDCOUNT_REQ
{
	PBMSG_HEAD2 h;
	char GuildName[MAX_GUILD_LEN + 1];
	int nUserIndex;
	WORD wMapSvrNum;
};

struct _tagPMSG_REQ_ITL_USERCOUNT_REQ
{
	PBMSG_HEAD2 h;
	char CharName[MAX_ACCOUNT_LEN + 1];
	int nUserIndex;
};

struct _tagPMSG_REQ_ITL_GUILDRANKRENEW
{
	PBMSG_HEAD2 h;
	BYTE byITLType;
};

struct _tagPMSG_REQ_ITL_GUILDRANK
{
	PBMSG_HEAD2 h;
};

struct _tagPMSG_REQ_ITL_GUILD_TOURNAMENTRANK
{
	PBMSG_HEAD2 h;
	BYTE byITLType;
};

struct _tagPMSG_ANS_ITL_TOURNAMENT
{
	PWMSG_HEAD2 h;
	BYTE byGuildCount;
	__ITL_GUILD_RANK m_GuildTournamentRank[6];
};

struct _stITLRewardList
{
	BYTE byRank;
	char szCharName[MAX_ACCOUNT_LEN + 1];
	char szGuildName[MAX_GUILD_LEN + 1];
	BYTE byEnterCount;
	BYTE byPoint;
	BYTE byGotReward;
};

struct _tagPMSG_ANS_ITL_REWARDLIST
{
	PWMSG_HEAD2 h;
	BYTE byCount;
	_stITLRewardList m_stRewardList[5];
};

struct _tagPMSG_ANS_ITL_USERCOUNTANS
{
	PBMSG_HEAD2 h;
	int nUserIndex;
	BYTE byCount;
};

struct _tagPMSG_ANS_ITL_GUILDCOUNTANS
{
	PBMSG_HEAD2 h;
	int nUserIndex;
	BYTE byCount;
};

struct _tagPMSG_ANS_ITL_GUILDLEAGUERANK
{
	PWMSG_HEAD2 h;
	BYTE byGuildCount;
	__ITL_GUILD_RANK m_GuildLeagueRank[56];
};

struct _tagPMSG_REQ_ITL_RENEW_REWARD
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;
};

struct _tagPMSG_REQ_ITL_GET_REWARDLIST
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;
};

struct _tagPMSG_REQ_ITL_REWARD_GOTRECEIVED
{
	PBMSG_HEAD2 h;
	char CharName[MAX_ACCOUNT_LEN + 1];
};

#pragma once
class CIllusionTempleLeagueProcess
{
public:
	CIllusionTempleLeagueProcess();
	virtual ~CIllusionTempleLeagueProcess();
	friend class CIllusionTempleLeagueEvent;

	void ITL_ProcessInit(int nTempleNum);
	void ClearUserData(int nIndex);
	void CheckSync();

	void Proc_ITL(int nCurTime);
	void Proc_ITLState_RoundBreak(int nCurTime);
	void Proc_ITLState_None(int nCurTime);
	void Proc_ITLState_Ready(int nCurTime);
	void Proc_ITLState_Playing(int nCurTime);
	void Proc_ITLState_End(int nCurTime);

	void Set_ITLState(BYTE btState);
	BYTE GetIllusionTempleLeagueState();

	void Set_ITLState_None();
	void Set_ITLState_Ready();
	void Set_ITLState_RoundBreak();
	void Set_ITLState_Playing();
	void Set_ITLState_End();

	BOOL EnterUserIllusionTempleLeague(int aIndex, BYTE btTempleIndex, int nGuildIdx, int nPartyIdx);
	BOOL LeaveUserIllusionTempleLeague(int aIndex);

	bool CanStartITLBattle(BYTE & ALLIED, BYTE & ILLUSION);
	void CheckUsersOnConnect();

	int GetRemainTimeMin();
	int GetEnterRemainTimeMin();

	int SearchRelicsItem(int iIndex);
	void DeleteAllRelicsItem();
	int FindITLUser(int index);
	void DropRelicsItem(int iIndex, BYTE byAct);
	char CheckHaveRelics(int aIndex); 
	void SetRelicsInventoryPos(int aIndex, BYTE btPos);

	void SendRelicsUserInfo(OBJECTSTRUCT *lpObj, BYTE byGet);

	void SetRemainTime();

	void AddITLNpcInfo(WORD wNpcType, BYTE byMapNumber, BYTE byDistance, BYTE byX, BYTE byY, BYTE byDir);
	void ResetITLPosNum();
	void RemoveAllObj();

	void SendRelicsError(OBJECTSTRUCT *lpObj, int index, BYTE byError, BYTE byAct);
	void Check_GetRelics(OBJECTSTRUCT *lpNpc, OBJECTSTRUCT *lpObj, WORD wNpcType);
	void GetRelics(OBJECTSTRUCT *lpNpc, OBJECTSTRUCT *lpObj, WORD wNpcType);
	void Check_RegisterRelics(OBJECTSTRUCT *lpNpc, OBJECTSTRUCT *lpObj);
	void RegisterRelics(OBJECTSTRUCT *lpNpc, OBJECTSTRUCT *lpObj);

	void FirstRegen_OccupiedStone();
	void FirstRegen_CursedStone();
	void RegenCursedStoneStatus(BYTE byRound);

	void SetStatusRegenTime();
	void AddITLOccupiedStonePos(WORD wNpcType, int index, int Count, BYTE byMapTagIndex);
	void AddITLCursedStonePos(WORD wNpcType, int index, BYTE byMapTagIndex);
	void SetNpc();
	void RegenOccupiedStoneStatus();

	void SendNoticeMessage(char *lpMsg);
	void Send_ITL_StoneInfo(int userindex, WORD wNpcType, BYTE byState, int NpcIndex, BYTE byMapTagIndex);
	void Send_ITL_PlayState(BYTE btTeam);
	void SendIllusionTempleLeagueState(BYTE btTempleState, int nIndex);

	BYTE GetUserTeam(int nIndex);
	void IncUserKillCount(int nIndex);
	void ResetKillCount();

	void ITL_UseSkill(int nIndex, WORD wSkillNumber, int nTargetIndex, BYTE btDis);
	void ITLSkillProc(OBJECTSTRUCT *lpObj);
	void Send_ITL_SkillEnd(OBJECTSTRUCT *lpObj, WORD wSkillNumber);
	int UseSkillProdection(OBJECTSTRUCT *lpObj);
	int UseSkillRestraint(OBJECTSTRUCT *lpObj, OBJECTSTRUCT* lpTargetObj);
	int UseSkillTeleport(OBJECTSTRUCT *lpObj);
	int UseSkillShieldBurn(OBJECTSTRUCT *lpObj, OBJECTSTRUCT* lpTargetObj);
	void SendUseSkillResult(int nIndex, int nTargetIndex, WORD wSkillNumber, BYTE btResult);

	void ITLUserDie(OBJECTSTRUCT* lpObj);
	void GetOccupiedStoneCount(BYTE &byAllied, BYTE &byIllusion);

	void CalCursetTempleResult();
	void DropChaosGem(int nIndex);

	void SendAllUserAnyMsg(char *lpMsg);

	int GetRealRemainTimeMin();

	BOOL CheckSkillProdection(int iIllusionTempleIndex);
	BOOL CheckSkillRestraint(int iIllusionTempleIndex);

	void SendITLResult();

	BYTE Find_SameGuild(int nGuildIdx, int &nUserIndex);
	BYTE Find_GuildSlot(int nGuildIdx, BYTE &bySlotNum);
	BYTE IsEmptySlot(BYTE bySlotNum);
	BYTE Find_EmptyGuildSlot(int nGuildNum, BYTE &byEmptySlot);
	
	BYTE DeleteGuild(int nGuildIdx);
	void ClearGuildDataSave();

	BYTE SetGuild(int nGuildIndex, BYTE bySlotOrder, BYTE byITLType, int nPartyIdx, int nUserIndex);

	void GDSaveITLGuildPoint(char* GuildName, int Type, BYTE btWin, BYTE btLos, BYTE byOccCnt, BYTE byEnterCnt);
	void GDSaveITLUserPoint(int index, char* UserName, char* GuildName, int nOccupiedCount, int KillPoint, int nInvalidationCount, int nType, int nEnterCount, int nWin);

	BYTE FindSameNameIndex(char *Name);
	void DeleteAllName();

	void MoveToStartPoint_WhenRoundBreak();

	BYTE FindGuildInfo(int nGuildIdx, BYTE &byTeam, int &nPartyIndex);

	int ReEnterUserIllusionTempleLeague(int aIndex, BYTE btTempleIndex, int nGuildIdx, int nPartyIdx, int nUserArrayNo, BYTE TeamIdx);

	void CalcKillCountByTeam(BYTE &byAlliedKillSum, BYTE &byIllusionKillSum);
	void ResetUserFlag_DoingRelicsThing(OBJECTSTRUCT *lpObj);
	void CancleStoneStateProcess(WORD wNpcType, BYTE byState);
	int GetRelicsUserIndex(OBJECTSTRUCT *lpObj);

	void ResetTimeVariables();
	bool GetEntrance() { return this->m_bEntrance; }
	void SetAllCheckSync() { this->m_bAllCheckSync = true; }
	bool GetAllCheckSync() { return this->m_bAllCheckSync; }

private:
	CRITICAL_SECTION			m_critUserData;
	__ITL_USER_DATA				m_UserData[10];
	__ITL_ENTER_PLAYER_NAME		m_EnteredPlayerName[10];
	DWORD						m_dwViewSkillState[10];
	CRITICAL_SECTION			m_criGuildData;
	__ITL_GUILD_DATA			m_GuildDataSave[2];
	int							m_nUserCount;
	int							m_nTempleNumber;
	BYTE						m_btIllusionTempleLeagueState;
	__ITL_OBJECT_DATA			m_Cursed_StoneStatus;
	__ITL_OBJECT_DATA			m_Occupied_StoneStatus[10];
	bool						m_bEntrance;
	BYTE						m_byITLRound;
	bool						m_bITLRoundStartFail;
	bool						m_bNobodyWinLose;
	int							m_StatusRegenTime;
	int							m_nRemainTimeMin;
	int							m_nRemainTimeMsec;
	int							m_nTickCount;
	int							m_nNextTimeMin;
	int							m_nRoundBreakCount;
	bool						m_bRoundBreak;
	int							m_nEventEndRemainTime;
	bool						m_bGameEndPosMove;
	bool						m_bSendMsgBeforeReady;
	bool						m_bSendMsgBeforePlay;
	int							m_nAlliedUserCount;
	int							m_nIllusionUserCount;
	BYTE						m_btWinTeam;
	BYTE						m_btLoseTeam;
	bool						m_bAllCheckSync;
	BYTE						m_byIllusionTeamPoint;
	BYTE						m_byAlliedTeamPoint;
	std::list<__ITL_TIME_TABLE> m_lst_ITL_Today_StartTime;
	ITL_NPC_POSITIONINFO		m_ITLNpcInfo[11];
	BYTE						m_byNpcCount;
	bool						m_bIsSetNpc;
	bool						m_bFirstRegen_CursedStone;
	bool						m_bFirstRegen_OccStone;
	bool						m_bSendResult;
};

#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


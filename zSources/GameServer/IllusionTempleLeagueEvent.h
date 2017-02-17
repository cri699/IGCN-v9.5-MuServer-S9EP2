//////////////////////////////////////////////////////////////////////
// IllusionTempleLeagueEvent.h
#ifndef ITL_EVENT_H
#define ITL_EVENT_H

#include "IllusionTempleLeagueProcess.h"

struct ITL_NPC_POSITION
{
	WORD wNpcType;
	BYTE byMapNumber;
	BYTE byDistance;
	BYTE byX;
	BYTE byY;
	BYTE byDir;
};

#pragma once
class CIllusionTempleLeagueEvent
{
public:
	CIllusionTempleLeagueEvent();
	virtual ~CIllusionTempleLeagueEvent();
	friend class CIllusionTempleLeagueProcess;

	int IllusionTempleLeague_EventInit();
	int Load_ITL_EventInfo();

	void Load_ITL_NPC_Position(char* filename);
	void Load_ITL_Script(char* filename);

	void ITL_Run();

	void SetRelicsInventoryPos(BYTE btMapNumber, int aIndex, BYTE btPos);
	void SendRelicsUserInfo(OBJECTSTRUCT* lpObj);

	int Enter_ITL(int aIndex, BYTE byTempleIndex, BYTE bySlotNum);
	int Leave_ITL(int aIndex, BYTE btMapNumber);

	void ActRelicsGetOrRegister(OBJECTSTRUCT *lpNpc, OBJECTSTRUCT *lpObj, BYTE byMapNumber, BYTE byPrePacket, BYTE byAct, WORD wNpcType);
	void CancleStoneState(WORD wNpcType, BYTE byState, BYTE mapnumber);
	void DropRelicsItem(BYTE btMapNumber, int nIndex);

	BYTE GetIllusionTempleLeagueState(BYTE btMapNumber);
	void AddNpcPosNum();
	void ResetPosNum();

	BYTE GetUserTeam(BYTE btMapNumber, int nIndex);
	void SetStatusRegenTime(BYTE btMapNumber);

	void ITL_UseSkill(int iIndex, WORD MagicNumber, int wTargetObjIndex, BYTE btDis);
	void EventSkillProc(OBJECTSTRUCT* lpObj);

	void IllusionTempleLeagueUserDie(OBJECTSTRUCT *lpObj);
	void IllusionTempleLeagueUserDieRegen(OBJECTSTRUCT *lpObj);

	void SetMirageIndex(int nIndex);

	BOOL CheckWearingMOPH(int nUserIndex);
	BOOL CheckChangeRing(int nType);
	BOOL CheckRelics(int nIndex);
	void ReqEventReward(int nIndex);

	int GetRemainTime();

	BOOL CheckSkillProdection(int iIllusionTempleIndex, BYTE btMapNumber);
	BOOL CheckSkillRestraint(int iIllusionTempleIndex, BYTE btMapNumber);
	bool IsCanEnterTiming();

	bool IsRewardDay();
	bool IsLeagueDays();
	bool IsSemiFinalDay();
	bool IsFinalDay();
	BYTE WhatIsTodayTypeOfITL();
	void ClearTodaySchedule();
	void SetTodaySchedule(BYTE ITLType);

	BYTE Update_GuildInfo(int nGuildIdx, BYTE byMapNumber, BYTE bySlotNumber, int nPartyIdx, BYTE byITLType, int nUserIndex);
	BYTE IsEmptyITLGuildInfo(BYTE byITLIndex, BYTE byEmptySlot);
	BYTE Find_EmptySlot(int nGuildIndex, BYTE &byEmptySlot);

	void Send_ITL_Schedule(int index);
	BYTE FindMyEnemy(char* GuildName);
	void Send_ITL_Tournament(int aIndex, BYTE byError);
	void Send_ITL_GuildRankInfo(int aIndex);

	bool IsRewardRenewDay();

	void GD_LoadGuildCount(char *GuildName, int UserIndex);
	void GD_Load_ITLUserEnterCount(char *CharName, int UserIndex);
	void GD_ITL_GuildRank_Renew(BYTE byITLType);
	void GD_ITL_GuildRank_Get();
	void GD_ITL_TournamentRank_Get(BYTE byITLType);
	void DG_ITL_Tournament_Get(_tagPMSG_ANS_ITL_TOURNAMENT *lpMsg);
	void DG_ITL_RewardList_Get(_tagPMSG_ANS_ITL_REWARDLIST *lpMsg);
	void DG_ITL_UserCount(_tagPMSG_ANS_ITL_USERCOUNTANS *lpMsg);
	void DG_ITL_GuildCount(_tagPMSG_ANS_ITL_GUILDCOUNTANS *lpMsg);
	void DG_ITL_GuildLeagueRank_Get(_tagPMSG_ANS_ITL_GUILDLEAGUERANK *lpMsg);

	void ResetITL_FlagAll();

	BYTE Find_SameGuild_AlreadyEntered(int nGuildIdx, int &ReqUserIndex);
	BYTE Find_ExistingGuildInfo(int nGuildIdx, BYTE &byMapNum, BYTE &bySlotNum);
	BYTE Delete_GuildInfo(int nGuildIdx);
	
	BYTE GetFinalTournament(int nGuildIdx, char *GuildName);
	BYTE GetMyGuildRankAtTournament(int nGuildIdx, char *GuildName);

	BYTE Find_MyPartyAndGuildInfo(BYTE byMapNumber, BYTE byArrayOrder, int GuildIndex, BYTE &byTeamIndex, int &nPartyIdx);
	BYTE Find_AlreadyEnteredUser(char *Name, BYTE &byMapNumber);

	BYTE CheckCanReEnterTiming(BYTE byMapNumber, BYTE &byNowState);
	BYTE ReEnterITL(int aIndex, BYTE btTempleIndex, int nGuildIdx, int nPartyIdx, int nUserArrayNo, int nITLState, BYTE byTeamIndex);

	void Insert_RewardList(char *Name, char *GuildName, BYTE byRank, BYTE byPoint, BYTE byEnter);
	void Send_RewardList(int aIndex, BYTE byError);
	void Renew_ITL_RewardList();
	void Get_ITL_RewardList();
	void Update_ITL_RewardFlag(char *Name);
	BYTE Check_ITLRewardList(int nUserIndex);
	void MakeITLRewardItem_CCFWing(int iUserIndex);

	void IncreaseKillCount(int nIndex, BYTE byMapNumber, BYTE byObjectType);

	void ReSetTimeVar();

	static bool m_bITL_Event_Enable;
	static bool m_bITL_INFO_LOAD_ALL;
	static bool m_bITL_DayFlagChange;
	static BYTE m_byITLFlagTimer;
	static bool m_bITL_SemiFinalEnd;
	static bool m_bITL_SemiFinalEnd_0;
	static bool m_bITL_SemiFinalEnd_1;
	static bool m_bITL_FinalEnd;
	static bool m_bSendOk_RewardRenew;
	static bool m_bSendGetITLRewardList;
	static bool m_bITL_SemiFinalEnd_Renew;
	static BYTE m_byITLRenewCount;
	static bool m_bITL_SemiFinalEnd_GetDone;
	static bool m_bITL_FinalEnd_Renew;
	static bool m_bITL_FinalEnd_GetDone;
	static BYTE m_byITLFinalStart;
	static int m_byITL_REWARDDAY;
	static bool m_bLoadOk_RewardList;

	static int m_nITLOpenTime;
	static int m_nITLReadyTime;
	static int m_nITLPlayTime;
	static int m_nITLEndTime;

	static std::list<__ITL_TIME_TABLE> m_lstALLITL_TIME;

private:
	CIllusionTempleLeagueProcess m_cIllusionTempleLeagueProc[2];
	int m_nMirageIndex;
	ITL_NPC_POSITION m_ITL_NPC[22];
	BYTE m_byTournamentRankPos1;
	BYTE m_byTournamentRankPos2;
	BYTE m_byTournamentRankPos3;
	BYTE m_byTournamentRankPos4;
	int m_nITL_TYPE;
	int m_nEnterLevelLimit;
	int m_LeagueDays[7];
	int m_SemiFinal;
	int m_Final;
	_tagITL_REWARDLIST m_ITL_Winner_RewardList[5];
	bool m_bLoadITLGuildEntryCountDone;
	__ITL_GUILD_RANK m_GuildLeagueRank[56];
	__ITL_GUILD_RANK m_GuildTournamentRank[6];
	bool m_bGuildRankRenew;
	bool m_bGuildRankGetDone;
};

extern CIllusionTempleLeagueEvent g_IllusionTempleLeagueEvent;

#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


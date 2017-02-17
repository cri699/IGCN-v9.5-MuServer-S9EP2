//////////////////////////////////////////////////////////////////////
// DevilSquareFinal.cpp
#include "stdafx.h"
#include "DevilSquareFinal.h"
#include "TLog.h"
#include "protocol.h"
#include "winutil.h"
#include "GameMain.h"
#include "DSProtocol.h"
#include "MapServerManager.h"
#include "TNotice.h"
#include "gObjMonster.h"
#include "configread.h"

CDevilSquareFinal g_DevilSquareFinal;

CDevilSquareFinal::CDevilSquareFinal(void)
{
	this->m_nDSF_TYPE = 0;
	this->m_btDSFCurState = 0;
	this->m_nTickCount = 0;
	this->m_nNextTimeMin = 0;
	this->m_nCurBattleStartTime = -1;
	this->m_nRemainTimeMin = -1;
	this->m_nRemainTimeMsec = -1;
	this->m_bEntrance = false;
	this->m_bSendMsgBeforeReady = false;
	this->m_nUserCount = 0;
	this->m_bRoundBreak = false;
	this->m_nEventEndRemainTime = 0;
	this->m_btDSFRound = 0;
	this->m_btWinTeam = -1;
	this->m_nDSFTestType = -1;
	this->m_bResultPointSend = false;
	this->m_bGetGoFinalUser = false;
	this->m_bWinnerResultMsgSend = false;
	this->m_bGameStart = false;

	for ( int i = 0; i < 10; i++ )
	{
		this->m_nTeamUserCount[i] = 0;
		this->m_nTeamPoint[i] = 0;
	}

	InitializeCriticalSection(&this->m_critUserData);
	InitializeCriticalSection(&this->m_criPartyData);
}

CDevilSquareFinal::~CDevilSquareFinal(void)
{
	DeleteCriticalSection(&this->m_critUserData);
	DeleteCriticalSection(&this->m_criPartyData);
}

void CDevilSquareFinal::Initialize_DSFSchedule()
{
	this->m_DSFTimeRuleInfo.Clear();
	this->m_DSFUnitInfo.Clear();
	this->m_DSFStartDay.Clear();

	for (int i = 0; i < 25; i++)
	{
		this->m_DSFStartHourType[i].Clear();
	}

	this->m_DSFRewardDay.Clear();

	for (int i = 0; i < 15; i++)
	{
		this->m_DSFEventMonsterRate[i].Clear();
	}

	for (int i = 0; i < 15; i++)
	{
		this->m_DSFAfterRoundAddPoint[i].Clear();
	}
}

void CDevilSquareFinal::Load_DSFScheduleScript(char *pchFileName)
{
	this->Initialize_DSFSchedule();

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(pchFileName);

	if (res.status != pugi::status_ok)
	{
		g_Log.MsgBox("Failed to load %s file (%s)", pchFileName, res.description());
		return;
	}

	pugi::xml_node main = file.child("TormentedSquare");

	if (g_ConfigRead.server.GetStateFromEventTable(g_ConfigRead.server.GetServerType(), EV_TABLE_DSF) == false)
	{
		this->m_bEVENT_ENABLE = false;
	}

	else
	{
		this->m_bEVENT_ENABLE = main.attribute("Enable").as_int();
	}

	this->m_DSFUnitInfo.PartyMinUser = main.attribute("MinUserInParty").as_int();
	this->m_DSFUnitInfo.DSFMinLevel = main.attribute("MinLevel").as_int();
	this->m_DSFUnitInfo.MinTeamCount = main.attribute("MinTeamsToStart").as_int();
	this->m_DSFUnitInfo.MinUserCount = main.attribute("MinUserToStart").as_int();
	this->m_DSFUnitInfo.MaxEnterCount = main.attribute("MaxEnterCount").as_int();
	this->m_DSFUnitInfo.MonsterUnitNumber = main.attribute("MonsterUnitNumber").as_int();

	pugi::xml_node time = main.child("Time");

	this->m_DSFTimeRuleInfo.BeforeEnterMinute = time.attribute("StartIn").as_int();
	this->m_DSFTimeRuleInfo.AfterEnterMinute = time.attribute("WaitingArea").as_int();
	this->m_DSFTimeRuleInfo.EachRoundMinute = time.attribute("Round").as_int();
	this->m_DSFTimeRuleInfo.AfterRoundSecond = time.attribute("AfterRound").as_int();
	this->m_DSFTimeRuleInfo.AfterBossRoundSecond = time.attribute("AfterBossRound").as_int();
	this->m_DSFTimeRuleInfo.CommentTime = time.attribute("TownSpawn").as_int();

	pugi::xml_node cycle = main.child("TournamentCycleDay");

	this->m_DSFStartDay.LeagueDay1 = cycle.attribute("EliminationOne").as_int();
	this->m_DSFStartDay.LeagueDay2 = cycle.attribute("EliminationTwo").as_int();
	this->m_DSFStartDay.LeagueDay3 = cycle.attribute("EliminationThree").as_int();
	this->m_DSFStartDay.LeagueDay4 = cycle.attribute("EliminationFour").as_int();
	this->m_DSFStartDay.SemiFinalDay1 = cycle.attribute("SemiFinalOne").as_int();
	this->m_DSFStartDay.SemiFinalDay2 = cycle.attribute("SemiFinalTwo").as_int();
	this->m_DSFStartDay.FinalDay = cycle.attribute("Final").as_int();

	pugi::xml_node reward = main.child("RewardDayRange");

	this->m_DSFRewardDay.RewardStartDay = reward.attribute("From").as_int();
	this->m_DSFRewardDay.RewardEndDay = reward.attribute("To").as_int();

	pugi::xml_node schedule = main.child("EventSchedule");
	int iValueCount = 0;

	for (pugi::xml_node start = schedule.child("Start"); start; start = start.next_sibling())
	{
		this->m_DSFStartHourType[iValueCount].DSFHour = start.attribute("Hour").as_int();
		this->m_DSFStartHourType[iValueCount].DSFMin = start.attribute("Minute").as_int();
		this->m_DSFStartHourType[iValueCount].DSFBattleType = start.attribute("StageType").as_int();
		iValueCount++;
	}

	pugi::xml_node eventmonster = main.child("EventMonsterAppearanceRate");
	iValueCount = 0;

	for (pugi::xml_node round = eventmonster.child("Round"); round; round = round.next_sibling())
	{
		this->m_DSFEventMonsterRate[iValueCount].RoundNum = round.attribute("Number").as_int();
		this->m_DSFEventMonsterRate[iValueCount].Rate = round.attribute("SpawnRate").as_int();
		iValueCount++;
	}

	pugi::xml_node extrapoint = main.child("RoundExtraPointReward");
	iValueCount = 0;

	for (pugi::xml_node round = extrapoint.child("Round"); round; round = round.next_sibling())
	{
		this->m_DSFAfterRoundAddPoint[iValueCount].RoundNum = round.attribute("Number").as_int();
		this->m_DSFAfterRoundAddPoint[iValueCount].Point = round.attribute("PointCount").as_int();
		iValueCount++;
	}

	g_Log.Add("[DSF] File Loaded (%s) (EnableEvent:%d)", pchFileName, this->m_bEVENT_ENABLE);
}

void CDevilSquareFinal::Initalize_DSFMonster()
{
	for (int i = 0; i < 15; i++)
	{
		this->m_DSFMonsterInfo[i].Clear();
	}

	for (int i = 0; i < 200; i++)
	{
		this->m_DSFMonsterRegenInfo[i].Clear();
	}

	for (int i = 0; i < 15; i++)
	{
		this->m_DSFEventMonsterInfo[i].Clear();
	}
}

void CDevilSquareFinal::Load_DSFMonsterScript(char *pchFileName)
{
	this->Initalize_DSFMonster();

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(pchFileName);

	if (res.status != pugi::status_ok)
	{
		g_Log.MsgBox("Failed to load %s file (%s)", pchFileName, res.description());
		return;
	}

	pugi::xml_node main = file.child("TormentedSquare");
	pugi::xml_node monster_round = main.child("MonsterRoundSettings");
	int iValueCount = 0;

	for (pugi::xml_node round = monster_round.child("Round"); round; round = round.next_sibling())
	{
		this->m_DSFMonsterInfo[iValueCount].RoundNum = round.attribute("Number").as_int();
		this->m_DSFMonsterInfo[iValueCount].MonsterType = round.attribute("MonsterIndex").as_int();
		this->m_DSFMonsterInfo[iValueCount].MonsterMaxNumber = round.attribute("MonsterCount").as_int();
		this->m_DSFMonsterInfo[iValueCount].MonsterKind = round.attribute("MonsterType").as_int();
		this->m_DSFMonsterInfo[iValueCount].MonsterPoint = round.attribute("PointCount").as_int();
		iValueCount++;
	}

	pugi::xml_node monster_regen = main.child("MonsterSpawnSettings");
	iValueCount = 0;

	for (pugi::xml_node monster = monster_regen.child("Spawn"); monster; monster = monster.next_sibling())
	{
		this->m_DSFMonsterRegenInfo[iValueCount].MonsterNum = monster.attribute("Index").as_int();
		this->m_DSFMonsterRegenInfo[iValueCount].MonsterMapNum = monster.attribute("MapNumber").as_int();
		this->m_DSFMonsterRegenInfo[iValueCount].MonsterDis = monster.attribute("Distance").as_int();
		this->m_DSFMonsterRegenInfo[iValueCount].RegenX = monster.attribute("PosX").as_int();
		this->m_DSFMonsterRegenInfo[iValueCount].RegenY = monster.attribute("PosY").as_int();
		this->m_DSFMonsterRegenInfo[iValueCount].MonsterDir = monster.attribute("Dir").as_int();
		this->m_DSFMonsterRegenInfo[iValueCount].MonsterKind = monster.attribute("Type").as_int();
		iValueCount++;
	}

	pugi::xml_node event_monster_round = main.child("EventMonsterSettings");
	iValueCount = 0;

	for (pugi::xml_node round = event_monster_round.child("Round"); round; round = round.next_sibling())
	{
		this->m_DSFEventMonsterInfo[iValueCount].RoundNum = round.attribute("Number").as_int();
		this->m_DSFEventMonsterInfo[iValueCount].MonsterType = round.attribute("MonsterIndex").as_int();
		this->m_DSFEventMonsterInfo[iValueCount].MonsterMaxNumber = round.attribute("MonsterCount").as_int();
		this->m_DSFEventMonsterInfo[iValueCount].MonsterKind = round.attribute("MonsterType").as_int();
		this->m_DSFEventMonsterInfo[iValueCount].MonsterPoint = round.attribute("PointCount").as_int();
		iValueCount++;
	}
}

void CDevilSquareFinal::ReqDSFSchedule(int aIndex)
{
	PMSG_ANS_DSF_SCHEDULE pResult;
	PHeadSubSetB((LPBYTE)&pResult, 0xDB, 0x00, sizeof(pResult));

	pResult.LeagueDay1 = this->m_DSFStartDay.LeagueDay1;
	pResult.LeagueDay2 = this->m_DSFStartDay.LeagueDay2;
	pResult.LeagueDay3 = this->m_DSFStartDay.LeagueDay3;
	pResult.LeagueDay4 = this->m_DSFStartDay.LeagueDay4;
	pResult.SemiFinalDay1 = this->m_DSFStartDay.SemiFinalDay1;
	pResult.SemiFinalDay2 = this->m_DSFStartDay.SemiFinalDay2;
	pResult.Final = this->m_DSFStartDay.FinalDay;
	pResult.CurDay = this->m_nDSF_TYPE;

	for (int i = 0; i < 10; i++)
	{
		pResult.LeagueHour[i] = -1;
		pResult.LeagueMin[i] = -1;
		pResult.SemiFinalHour[i] = -1;
		pResult.SemiFinalMin[i] = -1;
	}

	int nLeagueCount = 0;
	int nSemiFinalCount = 0;

	for (int i = 0; i < 25; i++)
	{
		switch ( this->m_DSFStartHourType[i].DSFBattleType )
		{
			case 1:
				pResult.LeagueHour[nLeagueCount] = this->m_DSFStartHourType[i].DSFHour;
				pResult.LeagueMin[nLeagueCount] = this->m_DSFStartHourType[i].DSFMin;
				nLeagueCount++;
				break;
			case 2:
				pResult.SemiFinalHour[nSemiFinalCount] = this->m_DSFStartHourType[i].DSFHour;
				pResult.SemiFinalMin[nSemiFinalCount] = this->m_DSFStartHourType[i].DSFMin;
				nSemiFinalCount++;
				break;
			case 3:
				pResult.FinalHour = this->m_DSFStartHourType[i].DSFHour;
				pResult.FinalMin = this->m_DSFStartHourType[i].DSFMin;
				break;
		}
	}
	
	pResult.RewardStartDay = this->m_DSFRewardDay.RewardStartDay;
	pResult.RewardEndDay = this->m_DSFRewardDay.RewardEndDay;

	IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
}

void CDevilSquareFinal::DSFEventInit()
{
	if (g_ConfigRead.server.GetStateFromEventTable(g_ConfigRead.server.GetServerType(), EV_TABLE_DSF) == false)
	{
		this->m_bEVENT_ENABLE = false;
		return;
	}

	g_Log.Add("[DSF] Event Initializing...");

	this->m_nDSF_TYPE = this->WhatIsTodayTypeOfDSF();
	EnterCriticalSection(&this->m_criPartyData);

	for (int i = 0; i < 10; i++)
	{
		this->m_PartyDataSave[i].bEntered = false;
		this->m_PartyDataSave[i].bRequest = false;
		this->m_PartyDataSave[i].btDSFType = 0;
		this->m_PartyDataSave[i].nRequestUserIndex = -1;
		this->m_PartyDataSave[i].n2ndPartyUserIndex = -1;
		memset(this->m_PartyDataSave[i].szRequestUserID, 0x00, MAX_ACCOUNT_LEN+1);
		memset(this->m_PartyDataSave[i].sz2ndPartyUserID, 0x00, MAX_ACCOUNT_LEN+1);
		memset(this->m_PartyDataSave[i].szRequestUserName, 0x00, MAX_ACCOUNT_LEN+1);
		memset(this->m_PartyDataSave[i].sz2ndPartyUserName, 0x00, MAX_ACCOUNT_LEN+1);
		memset(this->m_PartyDataSave[i].szRequestUserRealName, 0x00, MAX_ACCOUNT_LEN+1);
		memset(this->m_PartyDataSave[i].sz2ndPartyUserRealName, 0x00, MAX_ACCOUNT_LEN+1);
		this->m_PartyDataSave[i].nRequestUserServerCode = -1;
		this->m_PartyDataSave[i].n2ndPartyUserServerCode = -1;
		this->m_PartyDataSave[i].nRequestUserClass = -1;
		this->m_PartyDataSave[i].n2ndPartyUserClass = -1;
		this->m_PartyDataSave[i].nRequestUserLevel = -1;
		this->m_PartyDataSave[i].n2ndPartyUserLevel = -1;
		this->m_PartyDataSave[i].nPoint = 0;

		if (this->m_PartyDataSave[i].nPartyNo >= 0)
		{
			gParty.ClearPartyAuthDSF(this->m_PartyDataSave[i].nPartyNo);
			this->m_PartyDataSave[i].nPartyNo = -1;
		}

		this->m_PartyDataSave[i].btTeamIndex = -1;
	}

	LeaveCriticalSection(&this->m_criPartyData);
}

BOOL CDevilSquareFinal::CheckChangeRing(int nType)
{
	switch (nType)
	{
		case ITEMGET(13,10):
		case ITEMGET(13,39):
		case ITEMGET(13,40):
		case ITEMGET(13,41):
		case ITEMGET(13,68):
		case ITEMGET(13,76):
		case ITEMGET(13,77):
		case ITEMGET(13,78):
		case ITEMGET(13,122):
		case ITEMGET(13,163):
		case ITEMGET(13,164):
		case ITEMGET(13,165):
		case ITEMGET(13,166):
		case ITEMGET(13,169):
		case ITEMGET(13,170):
		case ITEMGET(13,268):
			return TRUE;
		default:
			return FALSE;
	}

	return FALSE;
}

BOOL CDevilSquareFinal::CheckWearingMOPH(int iUserIndex)
{
	if (gObjIsConnected(iUserIndex) == FALSE)
	{
		return FALSE;
	}

	if (gObj[iUserIndex].pInventory[10].IsItem() == TRUE)
	{
		if (gObj[iUserIndex].pInventory[10].m_Type == ITEMGET(13,10))
		{
			return TRUE;
		}

		if (gObj[iUserIndex].pInventory[10].m_Type == ITEMGET(13,39))
		{
			return TRUE;
		}

		if (gObj[iUserIndex].pInventory[10].m_Type == ITEMGET(13,40))
		{
			return TRUE;
		}

		if (gObj[iUserIndex].pInventory[10].m_Type == ITEMGET(13,41))
		{
			return TRUE;
		}

		if (gObj[iUserIndex].pInventory[10].m_Type == ITEMGET(13,68))
		{
			return TRUE;
		}

		if (gObj[iUserIndex].pInventory[10].m_Type == ITEMGET(13,76))
		{
			return TRUE;
		}

		if (gObj[iUserIndex].pInventory[10].m_Type == ITEMGET(13,77))
		{
			return TRUE;
		}

		if (gObj[iUserIndex].pInventory[10].m_Type == ITEMGET(13,78))
		{
			return TRUE;
		}

		if (gObj[iUserIndex].pInventory[10].m_Type == ITEMGET(13,122))
		{
			return TRUE;
		}

		if (gObj[iUserIndex].pInventory[10].m_Type == ITEMGET(13,163))
		{
			return TRUE;
		}

		if (gObj[iUserIndex].pInventory[10].m_Type == ITEMGET(13,164))
		{
			return TRUE;
		}

		if (gObj[iUserIndex].pInventory[10].m_Type == ITEMGET(13,165))
		{
			return TRUE;
		}

		if (gObj[iUserIndex].pInventory[10].m_Type == ITEMGET(13,166))
		{
			return TRUE;
		}

		if (gObj[iUserIndex].pInventory[10].m_Type == ITEMGET(13,122))
		{
			return TRUE;
		}

		if (gObj[iUserIndex].pInventory[10].m_Type == ITEMGET(13,166))
		{
			return TRUE;
		}

		if (gObj[iUserIndex].pInventory[10].m_Type == ITEMGET(13,169))
		{
			return TRUE;
		}

		if (gObj[iUserIndex].pInventory[10].m_Type == ITEMGET(13,170))
		{
			return TRUE;
		}
	}

	if (gObj[iUserIndex].pInventory[11].IsItem() == TRUE)
	{
		if (gObj[iUserIndex].pInventory[11].m_Type == ITEMGET(13,10))
		{
			return TRUE;
		}

		if (gObj[iUserIndex].pInventory[11].m_Type == ITEMGET(13,39))
		{
			return TRUE;
		}

		if (gObj[iUserIndex].pInventory[11].m_Type == ITEMGET(13,40))
		{
			return TRUE;
		}

		if (gObj[iUserIndex].pInventory[11].m_Type == ITEMGET(13,41))
		{
			return TRUE;
		}

		if (gObj[iUserIndex].pInventory[11].m_Type == ITEMGET(13,68))
		{
			return TRUE;
		}

		if (gObj[iUserIndex].pInventory[11].m_Type == ITEMGET(13,76))
		{
			return TRUE;
		}

		if (gObj[iUserIndex].pInventory[11].m_Type == ITEMGET(13,77))
		{
			return TRUE;
		}

		if (gObj[iUserIndex].pInventory[11].m_Type == ITEMGET(13,78))
		{
			return TRUE;
		}

		if (gObj[iUserIndex].pInventory[11].m_Type == ITEMGET(13,122))
		{
			return TRUE;
		}

		if (gObj[iUserIndex].pInventory[11].m_Type == ITEMGET(13,163))
		{
			return TRUE;
		}

		if (gObj[iUserIndex].pInventory[11].m_Type == ITEMGET(13,164))
		{
			return TRUE;
		}

		if (gObj[iUserIndex].pInventory[11].m_Type == ITEMGET(13,165))
		{
			return TRUE;
		}

		if (gObj[iUserIndex].pInventory[11].m_Type == ITEMGET(13,166))
		{
			return TRUE;
		}

		if (gObj[iUserIndex].pInventory[11].m_Type == ITEMGET(13,122))
		{
			return TRUE;
		}

		if (gObj[iUserIndex].pInventory[11].m_Type == ITEMGET(13,166))
		{
			return TRUE;
		}

		if (gObj[iUserIndex].pInventory[11].m_Type == ITEMGET(13,169))
		{
			return TRUE;
		}

		if (gObj[iUserIndex].pInventory[11].m_Type == ITEMGET(13,170))
		{
			return TRUE;
		}
	}

	return FALSE;
}

void CDevilSquareFinal::DSF_Run()
{
	this->m_nDSF_TYPE = this->WhatIsTodayTypeOfDSF();

	if (this->m_bGetGoFinalUser == false)
	{
		this->GDReqDSFPartyRankRenew(2);
		this->GDReqDSFPartyRankRenew(3);
		this->GDReqDSFPartyRankRenew(4);

		this->m_bGetGoFinalUser = true;
	}

	if (this->m_nDSF_TYPE != 0)
	{
		this->Proc_DSF(GetTickCount());
	}

	else
	{
		this->m_bEVENT_ENABLE = false;
	}
}

BYTE CDevilSquareFinal::WhatIsTodayTypeOfDSF()
{
	SYSTEMTIME time;
	GetLocalTime(&time);

	if (this->m_nDSFTestType > 0)
	{
		return this->m_nDSFTestType;
	}

	if (time.wDay == this->m_DSFStartDay.FinalDay)
	{
		return 3;
	}

	else if (time.wDay == this->m_DSFStartDay.SemiFinalDay1 || time.wDay == this->m_DSFStartDay.SemiFinalDay2)
	{
		return 2;
	}

	else if (time.wDay == this->m_DSFStartDay.LeagueDay1 || time.wDay == this->m_DSFStartDay.LeagueDay2 ||
		time.wDay == this->m_DSFStartDay.LeagueDay3 || time.wDay == this->m_DSFStartDay.LeagueDay4)
	{
		return 1;
	}

	else
	{
		return 0;
	}
}

void CDevilSquareFinal::Proc_DSF(int nCurTime)
{
	switch ( this->m_btDSFCurState )
	{
		case 0:
			this->Proc_DSFState_None(nCurTime);
			break;
		case 1:
			this->Proc_DSFState_Ready(nCurTime);
			break;
		case 2:
			this->Proc_DSFState_Playing(nCurTime);
			break;
		case 3:
			this->Proc_DSFState_End(nCurTime);
			break;
		case 4:
			this->Proc_DSFState_RoundBreak(nCurTime);
			break;
	}
}

void CDevilSquareFinal::Proc_DSFState_None(int nCurTime)
{
	int iTICK_MSEC = nCurTime - this->m_nTickCount;
	int nDSFType = this->WhatIsTodayTypeOfDSF();

	if (nDSFType != 0)
	{
		time_t ltime;
		tm * today;

		time(&ltime);
		today = localtime(&ltime);

		DWORD dwCurTime = 1000 * today->tm_sec + 60000 * today->tm_min + 3600000 * today->tm_hour;

		if (this->m_nCurBattleStartTime == -1)
		{
			for (int i = 0; i < 25; i++)
			{
				if (nDSFType == this->m_DSFStartHourType[i].DSFBattleType)
				{
					int nBattleStartTime = 0;
					int nBeforeBattleCommentTime = 0;

					if (this->m_nDSFTestType > 0)
					{
						if (!(today->tm_min % 3))
						{
							nBattleStartTime = 60000 * (today->tm_min + 6) + 3600000 * today->tm_hour;
						}

						nBeforeBattleCommentTime = 3600000;
					}

					else
					{
						nBattleStartTime = 60000 * this->m_DSFStartHourType[i].DSFMin + 3600000 * this->m_DSFStartHourType[i].DSFHour;
						nBeforeBattleCommentTime = 60000 * this->m_DSFTimeRuleInfo.CommentTime;
					}

					if (nBeforeBattleCommentTime + dwCurTime >= nBattleStartTime && dwCurTime < nBattleStartTime)
					{
						this->m_nCurBattleStartTime = nBattleStartTime;

						if (this->m_nDSFTestType > 0)
						{
							this->m_nRemainTimeMin = 6;
						}

						else
						{
							this->m_nRemainTimeMin = this->m_DSFTimeRuleInfo.CommentTime;
						}

						for (int k = 0; k < 20; k++)
						{
							this->ClearUserData(k);
						}

						EnterCriticalSection(&this->m_criPartyData);

						for (int l = 0; l < 10; l++)
						{
							this->m_PartyDataSave[l].bEntered = false;
							this->m_PartyDataSave[l].bRequest = false;
							this->m_PartyDataSave[l].btDSFType = 0;

							if (this->m_PartyDataSave[l].nPartyNo >= 0)
							{
								gParty.ClearPartyAuthDSF(this->m_PartyDataSave[l].nPartyNo);
								this->m_PartyDataSave[l].nPartyNo = -1;
							}

							this->m_PartyDataSave[l].btTeamIndex = -1;
							this->m_PartyDataSave[l].nRequestUserIndex = -1;
							this->m_PartyDataSave[l].n2ndPartyUserIndex = -1;
							memset(this->m_PartyDataSave[l].szRequestUserID, 0, MAX_ACCOUNT_LEN+1);
							memset(this->m_PartyDataSave[l].sz2ndPartyUserID, 0, MAX_ACCOUNT_LEN+1);
							memset(this->m_PartyDataSave[l].szRequestUserName, 0, MAX_ACCOUNT_LEN+1);
							memset(this->m_PartyDataSave[l].sz2ndPartyUserName, 0, MAX_ACCOUNT_LEN+1);
							memset(this->m_PartyDataSave[l].szRequestUserRealName, 0, MAX_ACCOUNT_LEN+1);
							memset(this->m_PartyDataSave[l].sz2ndPartyUserRealName, 0, MAX_ACCOUNT_LEN+1);
							this->m_PartyDataSave[l].nRequestUserServerCode = -1;
							this->m_PartyDataSave[l].n2ndPartyUserServerCode = -1;
							this->m_PartyDataSave[l].nRequestUserClass = -1;
							this->m_PartyDataSave[l].n2ndPartyUserClass = -1;
							this->m_PartyDataSave[l].nRequestUserLevel = -1;
							this->m_PartyDataSave[l].n2ndPartyUserLevel = -1;
							this->m_PartyDataSave[l].nPoint = 0;
						}

						LeaveCriticalSection(&this->m_criPartyData);

						char szTemp[128];
						sprintf(szTemp, "Tormented Square will be opened after %d minutes", this->m_nRemainTimeMin);
						GS_GDReqMapSvrMsgMultiCast(g_MapServerManager.GetMapSvrGroup(), szTemp);

						g_Log.Add("[DSF] %d minutes to open", this->m_nRemainTimeMin);
						this->m_nRemainTimeMin++;
						break;
					}
				}
			}
		}

		if (this->m_nCurBattleStartTime >= 0)
		{
			if (this->m_nNextTimeMin < nCurTime)
			{
				this->m_nRemainTimeMin--;
				this->m_nNextTimeMin = nCurTime + 60000;
				this->m_nRemainTimeMsec = 60000 * this->m_nRemainTimeMin;

				if (this->m_nRemainTimeMin <= this->m_DSFTimeRuleInfo.BeforeEnterMinute && this->m_nRemainTimeMin > 0)
				{
					PMSG_NOTICE pNotice;
					TNotice::MakeNoticeMsgEx(&pNotice, 0, "Tormented Square will close doors after %d minutes", this->m_nRemainTimeMin);
					this->SendAllUserAnyMsg((LPBYTE)&pNotice, pNotice.h.size);

					g_Log.Add("[DSF] %d minutes to close", this->m_nRemainTimeMin);
				}
			}

			if (iTICK_MSEC >= 1000)
			{
				if (this->m_nRemainTimeMsec > 0)
				{
					this->m_nRemainTimeMsec -= iTICK_MSEC;
				}

				this->m_nTickCount = nCurTime;

				if ( this->m_nRemainTimeMin <= this->m_DSFTimeRuleInfo.BeforeEnterMinute && this->m_nRemainTimeMsec > 5000 && this->m_bEntrance == false)
				{
					this->m_bEntrance = true;
					g_Log.Add("[DSF] Event has been opened.");
				}

				if (this->m_nRemainTimeMsec <= 5000 && this->m_bEntrance == true)
				{
					this->m_bEntrance = false;
					
					PMSG_NOTICE pNotice;
					TNotice::MakeNoticeMsgEx(&pNotice, 0, "Tormented Square is closed.");
					this->SendAllUserAnyMsg((LPBYTE)&pNotice, pNotice.h.size);

					g_Log.Add("[DSF] Event has been closed.");
				}
			}

			if (this->m_nRemainTimeMin <= 0 && this->m_nRemainTimeMsec <= 0)
			{
				this->m_bEntrance = false;
				this->CheckUsersOnConnect();

				for (int n = 0; n < 10; n++)
				{
					this->m_PartyDataSave[n].btTeamIndex = n;
				}

				BOOL bCanStart = this->CanStartDSFBattle();

				if (bCanStart == TRUE && this->m_nUserCount >= this->m_DSFUnitInfo.MinUserCount)
				{
					for (int j = 0; j < 20; j++)
					{
						if (this->m_UserData[j].m_nIndex != -1 &&
							gObj[this->m_UserData[j].m_nIndex].Connected > PLAYER_LOGGED &&
							gObj[this->m_UserData[j].m_nIndex].MapNumber == MAP_INDEX_DEVILSQUARE_FINAL &&
							gObj[this->m_UserData[j].m_nIndex].PartyNumber < 0)
						{
							gObj[this->m_UserData[j].m_nIndex].m_nDSFIndex = -1;
							gObjMoveGate(this->m_UserData[j].m_nIndex, 333);
							this->ClearUserData(j);

							g_Log.Add("[DSF][Proc_DSFState_None][%s][%s][%s][%d] Not PartyUser Kick Out",
								gObj[this->m_UserData[j].m_nIndex].AccountID, gObj[this->m_UserData[j].m_nIndex].m_PlayerData->m_RealNameOfUBF,
								gObj[this->m_UserData[j].m_nIndex].Name, gObj[this->m_UserData[j].m_nIndex].m_PlayerData->m_nServerCodeOfHomeWorld);
						}
					}

					for (int m = 0; m < 20; m++)
					{
						if ( this->m_UserData[m].m_nIndex != -1 && this->m_UserData[m].m_btTeam != (BYTE)-1 )
						{
							this->m_nTeamUserCount[this->m_UserData[m].m_btTeam]++;
						}
					}

					this->Set_DSFState(1);
				}
				
				else
				{
					if (this->m_nDSF_TYPE == 3)
					{
						for (int n = 0; n < 10; n++)
						{
							if (this->m_PartyDataSave[n].nPartyNo != -1)
							{
								this->m_PartyDataSave[n].nPoint = 1;
								this->m_nTeamPoint[this->m_PartyDataSave[n].btTeamIndex] = 1;
							}
						}
					}

					for (int j = 0; j < 20; j++)
					{
						int aIndex = this->m_UserData[j].m_nIndex;

						if (aIndex != -1 &&
							gObj[aIndex].Connected > PLAYER_LOGGED &&
							gObj[aIndex].MapNumber == MAP_INDEX_DEVILSQUARE_FINAL)
						{
							gObj[aIndex].m_nDSFIndex = -1;
							gObjMoveGate(aIndex, 333);
							this->ClearUserData(j);
						}
					}

					if (this->m_nDSF_TYPE == 3)
					{
						this->SendDSFResult(-1);
						this->SaveWinnerPartyPoint();
					}

					g_Log.Add("[DSF][Proc_DSFState_None] Failed to Start DSF (UserCount: %d) (m_nRemainTimeMin:%d)(m_nRemainTimeMsec:%d)",
						this->m_nUserCount, this->m_nRemainTimeMin, this->m_nRemainTimeMsec);
					g_Log.Add("[DSF][Proc_DSFState_None] StartFail : Not Enough User!");

					this->Set_DSFState(3);
				}
			}
		}
	}
}

void CDevilSquareFinal::Proc_DSFState_Ready(int nCurTime)
{
	int iTICK_MSEC = nCurTime - this->m_nTickCount;

	if (iTICK_MSEC >= 1000)
	{
		this->m_nRemainTimeMsec -= iTICK_MSEC;
		this->m_nTickCount = nCurTime;

		if (this->m_nRemainTimeMsec <= 20000 && this->m_nRemainTimeMsec > 0)
		{
			PMSG_SET_DEVILSQUARE pMsg;
			PHeadSetB((LPBYTE)&pMsg, 0x86, sizeof(pMsg));

			pMsg.Type = 26;
			
			for (int i = 0; i < 20; i++)
			{
				if (this->m_UserData[i].m_nIndex != -1 && 
					gObj[this->m_UserData[i].m_nIndex].Connected > PLAYER_LOGGED && 
					gObj[this->m_UserData[i].m_nIndex].MapNumber == MAP_INDEX_DEVILSQUARE_FINAL)
				{
					IOCP.DataSend(this->m_UserData[i].m_nIndex, (LPBYTE)&pMsg, pMsg.h.size);
				}
			}
		}
	}

	if (this->m_nUserCount == 0)
	{
		this->Set_DSFState(3);
		return;
	}

	if (this->m_nRemainTimeMsec <= 0)
	{
		this->m_btDSFRound++;
		this->Set_DSFState(2);

		for (int i = 0; i < 10; i++)
		{
			this->Send_DSF_PlayState(i);
		}
	}
}

void CDevilSquareFinal::Proc_DSFState_Playing(int nCurTime)
{
	int iTICK_MSEC = nCurTime - this->m_nTickCount;
	this->m_bRoundBreak = false;

	if (iTICK_MSEC >= 1000)
	{
		this->m_nRemainTimeMsec -= iTICK_MSEC;
		this->m_nTickCount = nCurTime;

		if (this->m_nRemainTimeMsec > 0)
		{
			for (int i = 0; i < 10; i++)
			{
				this->Send_DSF_PlayState(i);
			}
		}
	}

	if (this->m_nUserCount == 0)
	{
		this->Set_DSFState(3);
		return;
	}

	if (this->m_nRemainTimeMsec <= 0)
	{
		if (this->m_btDSFRound == 15)
		{
			int nAfterRoundPoint = 0;

			for (int i = 0; i < 15; i++)
			{
				if (this->m_DSFAfterRoundAddPoint[i].RoundNum == this->m_btDSFRound)
				{
					nAfterRoundPoint = this->m_DSFAfterRoundAddPoint[i].Point;
					break;
				}
			}

			for (int k = 0; k < 10; k++)
			{
				if (this->m_nTeamUserCount[k] > 0)
				{
					this->m_nTeamPoint[k] += nAfterRoundPoint * this->m_nTeamUserCount[k];
				}
			}

			for (int k = 0; k < 10; k++)
			{
				this->Send_DSF_PlayState(k);
			}

			this->Set_DSFState(3);
			this->SendDSFState(5, -1);
			this->m_bWinnerResultMsgSend = true;
		}

		else
		{
			this->Set_DSFState(4);
		}
	}

	BOOL bExistUser = FALSE;

	for (int k = 0; k < 10; k++)
	{
		if (this->m_nTeamUserCount[k] > 0)
		{
			bExistUser = TRUE;
			break;
		}
	}

	if (bExistUser == FALSE)
	{
		if (this->m_nRemainTimeMsec > 0)
		{
			this->m_nEventEndRemainTime = this->m_nRemainTimeMsec / 1000;
		}

		else
		{
			this->m_nEventEndRemainTime = 0;
		}

		for (int k = 0; k < 10; k++)
		{
			this->Send_DSF_PlayState(k);
		}

		this->Set_DSFState(3);
	}
}

void CDevilSquareFinal::Proc_DSFState_RoundBreak(int nCurTime)
{
	int iTICK_MSEC = nCurTime - this->m_nTickCount;

	if (iTICK_MSEC >= 1000)
	{
		this->m_nRemainTimeMsec -= iTICK_MSEC;
		this->m_nTickCount = nCurTime;

		if (this->m_nRemainTimeMsec <= 0)
		{
			this->m_btDSFRound++;
			this->Set_DSFState(2);
			this->m_bRoundBreak = false;
		}

		for (int i = 0; i < 10; i++)
		{
			this->Send_DSF_PlayState(i);
		}
	}

	if (this->m_nUserCount == 0)
	{
		this->Set_DSFState(3);
	}
}

void CDevilSquareFinal::Proc_DSFState_End(int nCurTime)
{
	int iTICK_MSEC = nCurTime - this->m_nTickCount;

	if (iTICK_MSEC >= 1000)
	{
		this->m_nRemainTimeMsec -= iTICK_MSEC;
		this->m_nTickCount = nCurTime;
	}

	if (this->m_nRemainTimeMsec <= 1000 * this->m_DSFTimeRuleInfo.AfterRoundSecond - 3000 && this->m_nRemainTimeMsec > 0 && this->m_bResultPointSend == false)
	{
		this->SendDSFResult(-1);
		this->SaveWinnerPartyPoint();
		this->m_bResultPointSend = true;
	}

	if (this->m_nRemainTimeMsec <= 0)
	{
		this->GDReqDSFPartyRankRenew(this->m_nDSF_TYPE + 1);
		this->Set_DSFState(0);
	}
}

void CDevilSquareFinal::SendAllUserAnyMsg(LPBYTE lpMsg, int iSize)
{
	for (int i = g_ConfigRead.server.GetObjectStartUserIndex(); i < g_ConfigRead.server.GetObjectMax(); i++)
	{
		if (gObj[i].Connected == PLAYER_PLAYING && gObj[i].Type == OBJ_USER)
		{
			if (!DG_MAP_RANGE(gObj[i].MapNumber) && gObj[i].MapNumber != MAP_INDEX_CHAOSCASTLE_SURVIVAL)
			{
				IOCP.DataSend(i, lpMsg, iSize);
			}
		}
	}
}

void CDevilSquareFinal::Set_DSFState(BYTE btState)
{
	this->m_btDSFCurState = btState;

	switch (btState)
	{
		case 0:
			this->Set_DSFState_None();
			break;
		case 1:
			this->Set_DSFState_Ready();
			break;
		case 2:
			this->Set_DSFState_Playing();
			break;
		case 3:
			this->Set_DSFState_End();
			break;
		case 4:
			this->Set_DSFState_RoundBreak();
			break;
	}
}

void CDevilSquareFinal::Set_DSFState_None()
{
	this->EndUserMove();
	this->SendDSFState(0, -1);
	this->DSF_ProcessInit();

	g_Log.Add("[DSF] SetState NONE");
}

void CDevilSquareFinal::Set_DSFState_Ready()
{
	this->m_nRemainTimeMsec = 60000 * this->m_DSFTimeRuleInfo.AfterEnterMinute;

	char sendbuf[4096];
	int lOfs = sizeof(PMSG_DSF_TEAM_USERNAME);

	int nPartyCount = 0;

	for (int i = 0; i < 10; i++)
	{
		if (this->m_PartyDataSave[i].nPartyNo != -1)
		{
			DSF_TEAM_PARTY_USER_NAME PartyUserName;

			PartyUserName.iTeamIndex = this->m_PartyDataSave[i].btTeamIndex;
			memcpy(PartyUserName.szUserName1, gObj[this->m_PartyDataSave[i].nRequestUserIndex].m_PlayerData->m_RealNameOfUBF, MAX_ACCOUNT_LEN+1);
			memcpy(PartyUserName.szUserName2, gObj[this->m_PartyDataSave[i].n2ndPartyUserIndex].m_PlayerData->m_RealNameOfUBF, MAX_ACCOUNT_LEN+1);
			PartyUserName.iTeamLevel = this->m_PartyDataSave[i].nRequestUserLevel + this->m_PartyDataSave[i].n2ndPartyUserLevel;
			
			memcpy(&sendbuf[lOfs], &PartyUserName, sizeof(DSF_TEAM_PARTY_USER_NAME));
			lOfs += sizeof(DSF_TEAM_PARTY_USER_NAME);
			nPartyCount++;
		}
	}

	PMSG_DSF_TEAM_USERNAME pMsg;
	PHeadSubSetW((LPBYTE)&pMsg, 0xDB, 0x07, lOfs);

	pMsg.nPartyCount = nPartyCount;

	if (nPartyCount <= 0)
	{
		pMsg.btResult = 100;
	}

	else
	{
		pMsg.btResult = 0;
	}

	memcpy(&sendbuf, &pMsg, sizeof(pMsg));

	for (int j = 0; j < 20; j++)
	{
		if (this->m_UserData[j].m_nIndex != -1 && gObj[this->m_UserData[j].m_nIndex].Connected > PLAYER_LOGGED)
		{
			IOCP.DataSend(this->m_UserData[j].m_nIndex, (LPBYTE)sendbuf, lOfs);
		}
	}

	if (this->m_nDSF_TYPE == 3)
	{
		for (int k = 0; k < 10; k++)
		{
			if ( this->m_PartyDataSave[k].nPartyNo != -1 )
			{
				this->m_PartyDataSave[k].nPoint = 1;
				this->m_nTeamPoint[this->m_PartyDataSave[k].btTeamIndex] = 1;
			}
		}

		for (int k = 0; k < 10; k++)
		{
			this->Send_DSF_PlayState(k);
		}
	}

	if (this->m_bGameStart == false)
	{
		this->m_bGameStart = true;
	}

	this->SendDSFState(1, -1);
	g_Log.Add("[DSF] SetState READY");
}

void CDevilSquareFinal::Set_DSFState_Playing()
{
	this->m_nRemainTimeMsec = 60000 * this->m_DSFTimeRuleInfo.EachRoundMinute;
	this->SetMonster();
	this->SendDSFState(2, -1);
	g_Log.Add("[DSF] SetState PLAYING");
}

void CDevilSquareFinal::Set_DSFState_RoundBreak()
{
	if (this->m_bRoundBreak == false)
	{
		this->m_bRoundBreak = true;

		if ( this->m_btDSFRound != 5 && this->m_btDSFRound != 10 )
		{
			this->m_nRemainTimeMsec = 1000 * this->m_DSFTimeRuleInfo.AfterRoundSecond;
		}

		else
		{
			this->m_nRemainTimeMsec = 1000 * this->m_DSFTimeRuleInfo.AfterBossRoundSecond;
		}
	}

	this->ClearMonster();
	this->SendDSFState(4, -1);

	int nAfterRoundPoint = 0;

	for (int i = 0; i < 15; i++)
	{
		if (this->m_DSFAfterRoundAddPoint[i].RoundNum == this->m_btDSFRound)
		{
			nAfterRoundPoint = this->m_DSFAfterRoundAddPoint[i].Point;
			break;
		}
	}

	for (int k = 0; k < 10; k++)
	{
		if (this->m_nTeamUserCount[k] > 0)
		{
			this->m_nTeamPoint[k] += nAfterRoundPoint * this->m_nTeamUserCount[k];

			g_Log.Add("[DSF] [Round %d Break] %d Team, UserCount:%d, TeamPoint:%d",
				this->m_btDSFRound, k, this->m_nTeamUserCount[k], this->m_nTeamPoint[k]);
		}
	}

	g_Log.Add("[DSF] SetState ROUND_BREAK");
}

void CDevilSquareFinal::Set_DSFState_End()
{
	this->m_nRemainTimeMsec = 1000 * this->m_DSFTimeRuleInfo.AfterRoundSecond;
	this->ClearMonster();
	this->SendDSFState(3, -1);
	g_Log.Add("[DSF] SetState END");
}

BOOL CDevilSquareFinal::IsAlreadyExistUserInDSF(int aIndex)
{
	for (int i = 0; i < 10; i++)
	{
		if (this->m_PartyDataSave[i].nPartyNo > 0)
		{
			if (!strcmp(this->m_PartyDataSave[i].szRequestUserName, gObj[aIndex].Name))
			{
				return TRUE;
			}

			if (!strcmp(this->m_PartyDataSave[i].sz2ndPartyUserName, gObj[aIndex].Name))
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}

char CDevilSquareFinal::Find_PartySlot(int nPartyNumber, BYTE & btSlotNum)
{
	if (nPartyNumber < 0)
	{
		return -1;
	}

	for (int i = 0; i < 10; i++)
	{
		if (this->m_PartyDataSave[i].nPartyNo == nPartyNumber)
		{
			btSlotNum = i;
			g_Log.Add("[DSF] Party Number %d find Party Slot:%d", nPartyNumber, btSlotNum);
			return 0;
		}
	}

	return -2;
}

char CDevilSquareFinal::Find_EmptyPartySlot(int nPartyNumber, BYTE & btSlotNum)
{
	if (nPartyNumber < 0)
	{
		return -1;
	}

	for (int i = 0; i < 10; i++)
	{
		if (this->m_PartyDataSave[i].nPartyNo < 0)
		{
			btSlotNum = i;
			g_Log.Add("[DSF] Party Number %d find Empty Party Slot:%d", nPartyNumber, btSlotNum);
			return 0;
		}
	}

	return -2;
}

char CDevilSquareFinal::SetParty(BYTE btSlotOrder, BYTE btDSFType, int nPartyIdx, int nUserIndex)
{
	if (nPartyIdx < 0)
	{
		return -1;
	}

	if (btSlotOrder >= 10)
	{
		return -3;
	}

	if (this->m_PartyDataSave[btSlotOrder].nPartyNo != -1)
	{
		return -4;
	}

	EnterCriticalSection(&this->m_criPartyData);

	this->m_PartyDataSave[btSlotOrder].bRequest = 1;
	this->m_PartyDataSave[btSlotOrder].btDSFType = btDSFType;
	this->m_PartyDataSave[btSlotOrder].nPartyNo = nPartyIdx;
	this->m_PartyDataSave[btSlotOrder].nRequestUserIndex = nUserIndex;
	memcpy(this->m_PartyDataSave[btSlotOrder].szRequestUserID, gObj[nUserIndex].AccountID, MAX_ACCOUNT_LEN+1);
	memcpy(this->m_PartyDataSave[btSlotOrder].szRequestUserName, gObj[nUserIndex].Name, MAX_ACCOUNT_LEN+1);
	memcpy(this->m_PartyDataSave[btSlotOrder].szRequestUserRealName, gObj[nUserIndex].m_PlayerData->m_RealNameOfUBF, MAX_ACCOUNT_LEN+1);
	this->m_PartyDataSave[btSlotOrder].nRequestUserServerCode = gObj[nUserIndex].m_PlayerData->m_nServerCodeOfHomeWorld;
	this->m_PartyDataSave[btSlotOrder].nRequestUserClass = gObj[nUserIndex].Class;
	this->m_PartyDataSave[btSlotOrder].nRequestUserLevel = gObj[nUserIndex].m_PlayerData->MasterLevel + gObj[nUserIndex].Level;

	int useridx;
	int dbnumber = 0;

	for (int i = 0; i < 2; i++)
	{
		if (gParty.GetIndexUser(gObj[nUserIndex].PartyNumber, i, useridx, dbnumber) == FALSE)
		{
			return -4;
		}

		if (useridx > 0 && nUserIndex != useridx)
		{
			this->m_PartyDataSave[btSlotOrder].n2ndPartyUserIndex = gObj[useridx].m_Index;
			memcpy(this->m_PartyDataSave[btSlotOrder].sz2ndPartyUserID, gObj[useridx].AccountID, MAX_ACCOUNT_LEN+1);
			memcpy(this->m_PartyDataSave[btSlotOrder].sz2ndPartyUserName, gObj[useridx].Name, MAX_ACCOUNT_LEN+1);
			memcpy(this->m_PartyDataSave[btSlotOrder].sz2ndPartyUserRealName, gObj[useridx].m_PlayerData->m_RealNameOfUBF, MAX_ACCOUNT_LEN+1);
			this->m_PartyDataSave[btSlotOrder].n2ndPartyUserServerCode = gObj[useridx].m_PlayerData->m_nServerCodeOfHomeWorld;
			this->m_PartyDataSave[btSlotOrder].n2ndPartyUserClass = gObj[useridx].Class;
			this->m_PartyDataSave[btSlotOrder].n2ndPartyUserLevel = gObj[useridx].m_PlayerData->MasterLevel + gObj[useridx].Level;
		}
	}

	LeaveCriticalSection(&this->m_criPartyData);
	return 0;
}

BOOL CDevilSquareFinal::IsGoSemiFinalParty(char *szUserName1, char *szUserName2, WORD wServerCode1, WORD wServerCode2)
{
	for (int i = 0; i < 40; i++)
	{
		if (!strcmp(this->m_GoSemiFinalParty[i].szUserName1, szUserName1) && 
			wServerCode1 == this->m_GoSemiFinalParty[i].wServerCode1 && 
			!strcmp(this->m_GoSemiFinalParty[i].szUserName2, szUserName2) && 
			wServerCode2 == this->m_GoSemiFinalParty[i].wServerCode2)
		{
			return TRUE;
		}

		if (!strcmp(this->m_GoSemiFinalParty[i].szUserName1, szUserName2) && 
			wServerCode2 == this->m_GoSemiFinalParty[i].wServerCode1 && 
			!strcmp(this->m_GoSemiFinalParty[i].szUserName2, szUserName1) && 
			wServerCode1 == this->m_GoSemiFinalParty[i].wServerCode2)
		{
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CDevilSquareFinal::IsGoFinalParty(char *szUserName1, char *szUserName2, WORD wServerCode1, WORD wServerCode2)
{
	for (int i = 0; i < 10; i++)
	{
		if (!strcmp(this->m_GoFinalParty[i].szUserName1, szUserName1) && 
			wServerCode1 == this->m_GoFinalParty[i].wServerCode1 && 
			!strcmp(this->m_GoFinalParty[i].szUserName2, szUserName2) && 
			wServerCode2 == this->m_GoFinalParty[i].wServerCode2)
		{
			return TRUE;
		}

		if (!strcmp(this->m_GoFinalParty[i].szUserName1, szUserName2) && 
			wServerCode2 == this->m_GoFinalParty[i].wServerCode1 && 
			!strcmp(this->m_GoFinalParty[i].szUserName2, szUserName1) && 
			wServerCode1 == this->m_GoFinalParty[i].wServerCode2)
		{
			return TRUE;
		}
	}

	return FALSE;
}

void CDevilSquareFinal::DeleteParty(int nPartyNumber)
{
	for (int i = 0; i < 10; i++)
	{
		if (this->m_PartyDataSave[i].nPartyNo == nPartyNumber)
		{
			EnterCriticalSection(&this->m_criPartyData);
			this->m_PartyDataSave[i].nRequestUserIndex = -1;
			this->m_PartyDataSave[i].n2ndPartyUserIndex = -1;
			memset(this->m_PartyDataSave[i].szRequestUserID, 0, MAX_ACCOUNT_LEN+1);
			memset(this->m_PartyDataSave[i].sz2ndPartyUserID, 0, MAX_ACCOUNT_LEN+1);
			memset(this->m_PartyDataSave[i].szRequestUserName, 0, MAX_ACCOUNT_LEN+1);
			memset(this->m_PartyDataSave[i].sz2ndPartyUserName, 0, MAX_ACCOUNT_LEN+1);
			memset(this->m_PartyDataSave[i].szRequestUserRealName, 0, MAX_ACCOUNT_LEN+1);
			memset(this->m_PartyDataSave[i].sz2ndPartyUserRealName, 0, MAX_ACCOUNT_LEN+1);
			this->m_PartyDataSave[i].nRequestUserServerCode = -1;
			this->m_PartyDataSave[i].n2ndPartyUserServerCode = -1;
			this->m_PartyDataSave[i].nRequestUserClass = -1;
			this->m_PartyDataSave[i].n2ndPartyUserClass = -1;
			this->m_PartyDataSave[i].nRequestUserLevel = -1;
			this->m_PartyDataSave[i].n2ndPartyUserLevel = -1;
			this->m_PartyDataSave[i].bEntered = false;
			this->m_PartyDataSave[i].bRequest = false;
			this->m_PartyDataSave[i].btDSFType = 0;
			this->m_PartyDataSave[i].nPartyNo = -1;
			this->m_PartyDataSave[i].nPoint = 0;
			this->m_PartyDataSave[i].btTeamIndex = -1;
			LeaveCriticalSection(&this->m_criPartyData);
			return;
		}
	}
}

void CDevilSquareFinal::DeletePartyByIndex(int nIndex)
{
	for (int i = 0; i < 10; i++)
	{
		int nFirst = strcmp(this->m_PartyDataSave[i].szRequestUserName, gObj[nIndex].Name);
		int nSecond = strcmp(this->m_PartyDataSave[i].sz2ndPartyUserName, gObj[nIndex].Name);

		if (!nFirst || !nSecond)
		{
			EnterCriticalSection(&this->m_criPartyData);
			this->m_PartyDataSave[i].nRequestUserIndex = -1;
			this->m_PartyDataSave[i].n2ndPartyUserIndex = -1;
			memset(this->m_PartyDataSave[i].szRequestUserID, 0, MAX_ACCOUNT_LEN+1);
			memset(this->m_PartyDataSave[i].sz2ndPartyUserID, 0, MAX_ACCOUNT_LEN+1);
			memset(this->m_PartyDataSave[i].szRequestUserName, 0, MAX_ACCOUNT_LEN+1);
			memset(this->m_PartyDataSave[i].sz2ndPartyUserName, 0, MAX_ACCOUNT_LEN+1);
			memset(this->m_PartyDataSave[i].szRequestUserRealName, 0, MAX_ACCOUNT_LEN+1);
			memset(this->m_PartyDataSave[i].sz2ndPartyUserRealName, 0, MAX_ACCOUNT_LEN+1);
			this->m_PartyDataSave[i].nRequestUserServerCode = -1;
			this->m_PartyDataSave[i].n2ndPartyUserServerCode = -1;
			this->m_PartyDataSave[i].nRequestUserClass = -1;
			this->m_PartyDataSave[i].n2ndPartyUserClass = -1;
			this->m_PartyDataSave[i].nRequestUserLevel = -1;
			this->m_PartyDataSave[i].n2ndPartyUserLevel = -1;
			this->m_PartyDataSave[i].bEntered = false;
			this->m_PartyDataSave[i].bRequest = false;
			this->m_PartyDataSave[i].btDSFType = 0;
			this->m_PartyDataSave[i].nPartyNo = -1;
			this->m_PartyDataSave[i].nPoint = 0;
			this->m_PartyDataSave[i].btTeamIndex = -1;
			LeaveCriticalSection(&this->m_criPartyData);
			return;
		}
	}
}

BOOL CDevilSquareFinal::Enter_DSF(int aIndex, BYTE btSlotNum)
{
	if (this->m_bEVENT_ENABLE == false)
	{
		g_Log.Add("[DSF] Enter Error: DSF is not active");
		return FALSE;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if (!lpObj)
	{
		g_Log.Add("[DSF] Enter Error: Critical Error (lpObj)");
		return FALSE;
	}

	if (this->m_btDSFCurState >= 1)
	{
		g_Log.Add("[DSF] Enter Error: DSF is ongoing!");
		return FALSE;
	}

	if (gObj[aIndex].m_nDSFIndex != -1)
	{
		g_Log.Add("[DSF][Enter_DSF] Enter Error: DSFIndex Fail %d", gObj[aIndex].m_nDSFIndex);
		return FALSE;
	}

	BOOL bResult = FALSE;
	int ArrayIndex = -1;

	EnterCriticalSection(&this->m_critUserData);

	for (int i = 0; i < 20; i++)
	{
		if (this->m_UserData[i].m_nIndex == -1)
		{
			if (lpObj->PartyNumber == this->m_PartyDataSave[btSlotNum].nPartyNo)
			{
				this->m_UserData[i].m_btTeam = btSlotNum;
				this->m_UserData[i].m_nIndex = aIndex;
				this->m_UserData[i].m_nPartyIdx = lpObj->PartyNumber;
				this->m_UserData[i].m_bEnterOk = true;
				this->m_nUserCount++;
				gObj[aIndex].m_nDSFIndex = i;
				memcpy(this->m_UserData[i].PlayerName, lpObj->Name, MAX_ACCOUNT_LEN+1);

				bResult = TRUE;
				ArrayIndex = i;

				g_Log.Add("[DSF][Enter_DSF][%s][%s][%s][%d] UserCnt:[%d], DSFIndex:[%d], Team:%d",
					gObj[aIndex].AccountID, gObj[aIndex].m_PlayerData->m_RealNameOfUBF, gObj[aIndex].Name,
					gObj[aIndex].m_PlayerData->m_nServerCodeOfHomeWorld, this->m_nUserCount, gObj[aIndex].m_nDSFIndex,
					this->m_UserData[i].m_btTeam);
			}
			
			else
			{
				bResult = FALSE;
			}

			break;
		}
	}

	LeaveCriticalSection(&this->m_critUserData);

	if (ArrayIndex != -1 && ArrayIndex < 20)
	{
		this->m_EnteredPlayerName[ArrayIndex].EnteredIndex = ArrayIndex;
		memcpy(this->m_EnteredPlayerName[ArrayIndex].PlayerName, gObj[aIndex].Name, MAX_ACCOUNT_LEN+1);
	}

	if (bResult == TRUE)
	{
		g_Log.Add("[DSF][Enter_DSF][%s][%s][%s][%d] Enter Success, class : %d",
			lpObj->AccountID, lpObj->m_PlayerData->m_RealNameOfUBF, lpObj->Name,
			lpObj->m_PlayerData->m_nServerCodeOfHomeWorld, lpObj->m_PlayerData->DbClass);

		this->SendDSFState(0, lpObj->m_Index);
		return TRUE;
	}

	else
	{
		g_Log.Add("[DSF][Enter_DSF][%s][%s][%s][%d] Enter Fail, class : %d",
			lpObj->AccountID, lpObj->m_PlayerData->m_RealNameOfUBF, lpObj->Name,
			lpObj->m_PlayerData->m_nServerCodeOfHomeWorld, lpObj->m_PlayerData->DbClass);

		return FALSE;
	}
}

BOOL CDevilSquareFinal::Leave_DSF(int aIndex)
{
	if (gObj[aIndex].m_nDSFIndex == -1)
	{
		return FALSE;
	}

	if (aIndex != this->m_UserData[gObj[aIndex].m_nDSFIndex].m_nIndex)
	{
		return FALSE;
	}

	if (!gObjIsConnected(aIndex))
	{
		return FALSE;
	}

	EnterCriticalSection(&this->m_critUserData);

	this->m_nTeamUserCount[this->m_UserData[gObj[aIndex].m_nDSFIndex].m_btTeam]--;
	int nUserCount = this->m_nTeamUserCount[this->m_UserData[gObj[aIndex].m_nDSFIndex].m_btTeam];

	if (this->m_bGameStart == false)
	{
		for (int i = 0; i < 10; i++)
		{
			if (this->m_UserData[gObj[aIndex].m_nDSFIndex].m_btTeam == this->m_PartyDataSave[i].btTeamIndex &&
				nUserCount == 0)
			{
				g_Log.Add("[DSF][Leave_DSF][%s][%s][%s][%d] All PartyUser Leave : Game is not start, TeamIndex:[%d], PartyUserName1:%s, PartyUserName2:%s",
					gObj[aIndex].AccountID, gObj[aIndex].m_PlayerData->m_RealNameOfUBF, gObj[aIndex].Name, gObj[aIndex].m_PlayerData->m_nServerCodeOfHomeWorld,
					this->m_PartyDataSave[i].btTeamIndex, this->m_PartyDataSave[i].szRequestUserRealName, this->m_PartyDataSave[i].sz2ndPartyUserRealName);

				this->ClearUserData(this->m_PartyDataSave[i].nRequestUserIndex);
				this->ClearUserData(this->m_PartyDataSave[i].n2ndPartyUserIndex);
				this->DeleteParty(this->m_PartyDataSave[i].nPartyNo);
			}
		}
	}

	g_Log.Add("[DSF][Leave_DSF][%s][%s][%s][%d] User Leave, UserCnt:[%d], DSFIndex:[%d]",
		gObj[aIndex].AccountID, gObj[aIndex].m_PlayerData->m_RealNameOfUBF, gObj[aIndex].Name,
		gObj[aIndex].m_PlayerData->m_nServerCodeOfHomeWorld, this->m_nUserCount, gObj[aIndex].m_nDSFIndex);

	gObj[aIndex].m_nDSFIndex = -1;
	this->m_nUserCount--;
	LeaveCriticalSection(&this->m_critUserData);

	gObjViewportListProtocolCreate(&gObj[aIndex]);
	return TRUE;
}

void CDevilSquareFinal::ClearUserData(int nIndex)
{
	if (!ObjectMaxRange(nIndex))
	{
		return;
	}

    this->m_UserData[nIndex].m_nIndex = -1;
    this->m_UserData[nIndex].m_btTeam = -1;
    this->m_UserData[nIndex].m_bReward = false;
    this->m_UserData[nIndex].m_bEnterOk = false;
    this->m_UserData[nIndex].m_nPartyIdx = -1;
    memset(this->m_UserData[nIndex].PlayerName, 0, MAX_ACCOUNT_LEN+1);
}

void CDevilSquareFinal::SendDSFState(BYTE btDSFState, int nIndex)
{
	PMSG_DSF_STATE pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0xDB, 0x04, sizeof(pMsg));

	pMsg.btDSFState = btDSFState;

	if (nIndex != -1)
	{
		IOCP.DataSend(nIndex, (LPBYTE)&pMsg, pMsg.h.size);
	}

	else
	{
		for (int i = 0; i < 20; i++)
		{
			if (this->m_UserData[i].m_nIndex != -1 && gObj[this->m_UserData[i].m_nIndex].Connected > PLAYER_LOGGED)
			{
				IOCP.DataSend(this->m_UserData[i].m_nIndex, (LPBYTE)&pMsg, pMsg.h.size);
			}
		}
	}
}

void CDevilSquareFinal::Send_DSF_PlayState(BYTE btTeam)
{
	if (this->m_nRemainTimeMsec < 0)
	{
		this->m_nRemainTimeMsec = 0;
	}

	PMSG_DSF_PLAY_STATE pMsg;
	
	pMsg.wRemainSec = this->m_nRemainTimeMsec / 1000;

	for (int j = 0; j < 10; j++)
	{
		pMsg.iTeamPoint[j] = this->m_nTeamPoint[j];
	}

	pMsg.btMyTeam = btTeam;
	PHeadSubSetB((LPBYTE)&pMsg, 0xDB, 0x05, sizeof(pMsg));

	for (int i = 0; i < 20; i++)
	{
		if ( this->m_UserData[i].m_nIndex != -1 && gObj[this->m_UserData[i].m_nIndex].Connected > PLAYER_LOGGED && this->m_UserData[i].m_btTeam == btTeam )
		{
			if (!strcmp(gObj[this->m_UserData[i].m_nIndex].Name, this->m_UserData[i].PlayerName))
			{
				IOCP.DataSend(this->m_UserData[i].m_nIndex, (LPBYTE)&pMsg, pMsg.h.size);
			}
		}
	}
}

void CDevilSquareFinal::CheckUsersOnConnect()
{
	EnterCriticalSection(&this->m_critUserData);
	this->m_nUserCount = 0;

	for (int i = 0; i < 20; i++)
	{
		if (this->m_UserData[i].m_nIndex != -1)
		{
			if (gObjIsConnected(this->m_UserData[i].m_nIndex))
			{
				if (gObj[this->m_UserData[i].m_nIndex].MapNumber == MAP_INDEX_DEVILSQUARE_FINAL)
				{
					this->m_nUserCount++;
				}

				else
				{
					gObj[this->m_UserData[i].m_nIndex].m_nDSFIndex = -1;
					this->ClearUserData(i);
				}
			}

			else
			{
				gObj[this->m_UserData[i].m_nIndex].m_nDSFIndex = -1;
				this->ClearUserData(i);
			}
		}
	}

	LeaveCriticalSection(&this->m_critUserData);
}

bool CDevilSquareFinal::CanStartDSFBattle()
{
	int nCanEnterPartyCount = 0;
	int nTeamCount[10];

	for (int j = 0; j < 10; j++)
	{
		nTeamCount[j] = 0;
	}

	for (int i = 0; i < 20; i++)
	{
		if (this->m_UserData[i].m_nIndex != -1 && this->m_UserData[i].m_bEnterOk == true)
		{
			if (gObjIsConnected(this->m_UserData[i].m_nIndex))
			{
				nTeamCount[this->m_UserData[i].m_btTeam]++;
			}
		}
	}

	for (int k = 0; k < 10; k++)
	{
		if (nTeamCount[k] >= 2)
		{
			nCanEnterPartyCount++;
		}
	}

	BOOL bCanStart = nCanEnterPartyCount >= this->m_DSFUnitInfo.MinTeamCount;

	g_Log.Add("[DSF][CanStartDSFBattle] Result %s nCanEnterPartyCount:%d", bCanStart == TRUE ? "TRUE" : "FALSE", nCanEnterPartyCount);
	return bCanStart;
}

void CDevilSquareFinal::SendDSFResult(int aIndex)
{
	for (int j = 0; j < 10; j++)
	{
		this->m_PartyDataSave[j].nPoint = this->m_nTeamPoint[this->m_PartyDataSave[j].btTeamIndex];
		gParty.ClearPartyAuthDSF(this->m_PartyDataSave[j].nPartyNo);
	}

	PMSG_DSF_RESULT pMsg;

	for (int k = 0; k < 10; k++)
	{
		pMsg.iTeamPoint[k] = this->m_nTeamPoint[k];
	}

	PHeadSubSetB((LPBYTE)&pMsg, 0xDB, 0x06, sizeof(pMsg));;
	
	if (aIndex <= 0)
	{
		for (int i = 0; i < 20; i++)
		{
			if (this->m_UserData[i].m_nIndex != -1 && gObj[this->m_UserData[i].m_nIndex].Connected > PLAYER_LOGGED)
			{
				if (!strcmp(gObj[this->m_UserData[i].m_nIndex].Name, this->m_UserData[i].PlayerName))
				{
					IOCP.DataSend(this->m_UserData[i].m_nIndex, (LPBYTE)&pMsg, pMsg.h.size);
				}
			}
		}
	}

	else
	{
		for (int i = 0; i < 20; i++)
		{
			if (this->m_UserData[i].m_nIndex != -1 && gObj[this->m_UserData[i].m_nIndex].Connected > PLAYER_LOGGED)
			{
				if (!strcmp(gObj[aIndex].Name, this->m_UserData[i].PlayerName))
				{
					IOCP.DataSend(this->m_UserData[i].m_nIndex, (LPBYTE)&pMsg, pMsg.h.size);
				}
			}
		}
	}

	for (int i = 0; i < 10; i++)
	{
		if (this->m_PartyDataSave[i].nPartyNo >= 0)
		{
			g_Log.Add("[DSF][SendDSFResult] - [%s][%s][%s][%d] / [%s][%s][%s][%d] DSF Result - Point : %d",
				this->m_PartyDataSave[i].szRequestUserID, this->m_PartyDataSave[i].szRequestUserRealName,
				this->m_PartyDataSave[i].szRequestUserName, this->m_PartyDataSave[i].nRequestUserServerCode,
				this->m_PartyDataSave[i].sz2ndPartyUserID, this->m_PartyDataSave[i].sz2ndPartyUserRealName,
				this->m_PartyDataSave[i].sz2ndPartyUserName, this->m_PartyDataSave[i].n2ndPartyUserServerCode,
				this->m_nTeamPoint[i]);
		}
	}
}

void CDevilSquareFinal::SaveWinnerPartyPoint()
{
	int n1stTeam = -1;
	int n2ndTeam = -1;
	int n3rdTeam = -1;
	int n4thTeam = -1;
	int n1stPoint = 0;
	int n2ndPoint = 0;
	int n3rdPoint = 0;
	int n4thPoint = 0;
	int n1stLevel = 0;
	int n2ndLevel = 0;
	int n3rdLevel = 0;
	int n4thLevel = 0;

	int nTempTeam, nTempPoint, nTempLevel, nNewPoint, nNewLevel;

	for (int i = 0; i < 10; i++)
	{
		if (this->m_PartyDataSave[i].nPartyNo != -1)
		{
			switch (i)
			{
				case 0:
					n1stPoint = this->m_PartyDataSave[0].nPoint;
					n1stTeam = this->m_PartyDataSave[0].btTeamIndex;
					n1stLevel = this->m_PartyDataSave[0].n2ndPartyUserLevel + this->m_PartyDataSave[0].nRequestUserLevel;
					break;
				case 1:
					n2ndPoint = this->m_PartyDataSave[1].nPoint;
					n2ndTeam = this->m_PartyDataSave[1].btTeamIndex;
					n2ndLevel = this->m_PartyDataSave[1].n2ndPartyUserLevel + this->m_PartyDataSave[1].nRequestUserLevel;

					if ( n1stPoint >= n2ndPoint )
					{
						if ( n1stPoint == n2ndPoint && n1stLevel > n2ndLevel )
						{
							nTempTeam = n1stTeam;
							n1stTeam = this->m_PartyDataSave[1].btTeamIndex;
							n2ndTeam = nTempTeam;
							nTempLevel = n1stLevel;
							n1stLevel = this->m_PartyDataSave[1].n2ndPartyUserLevel + this->m_PartyDataSave[1].nRequestUserLevel;
							n2ndLevel = nTempLevel;
						}
					}

					else
					{
						nTempPoint = n1stPoint;
						n1stPoint = this->m_PartyDataSave[1].nPoint;
						n2ndPoint = nTempPoint;
						nTempTeam = n1stTeam;
						n1stTeam = this->m_PartyDataSave[1].btTeamIndex;
						n2ndTeam = nTempTeam;
						nTempLevel = n1stLevel;
						n1stLevel = this->m_PartyDataSave[1].n2ndPartyUserLevel + this->m_PartyDataSave[1].nRequestUserLevel;
						n2ndLevel = nTempLevel;
					}
					break;
				case 2:
					n3rdPoint = this->m_PartyDataSave[2].nPoint;
					n3rdTeam = this->m_PartyDataSave[2].btTeamIndex;
					n3rdLevel = this->m_PartyDataSave[2].n2ndPartyUserLevel + this->m_PartyDataSave[2].nRequestUserLevel;

					if ( n1stPoint >= n3rdPoint )
					{
						if ( n1stPoint == n3rdPoint )
						{
							if ( n1stLevel <= n3rdLevel )
							{
								nTempPoint = n2ndPoint;
								n2ndPoint = this->m_PartyDataSave[2].nPoint;
								n3rdPoint = nTempPoint;
								nTempTeam = n2ndTeam;
								n2ndTeam = this->m_PartyDataSave[2].btTeamIndex;
								n3rdTeam = nTempTeam;
								nTempLevel = n2ndLevel;
								n2ndLevel = this->m_PartyDataSave[2].n2ndPartyUserLevel + this->m_PartyDataSave[2].nRequestUserLevel;
								n3rdLevel = nTempLevel;
							}

							else
							{
								nTempPoint = n1stPoint;
								n1stPoint = this->m_PartyDataSave[2].nPoint;
								n3rdPoint = n2ndPoint;
								n2ndPoint = nTempPoint;
								nTempTeam = n1stTeam;
								n1stTeam = this->m_PartyDataSave[2].btTeamIndex;
								n3rdTeam = n2ndTeam;
								n2ndTeam = nTempTeam;
								nTempLevel = n1stLevel;
								n1stLevel = this->m_PartyDataSave[2].n2ndPartyUserLevel + this->m_PartyDataSave[2].nRequestUserLevel;
								n3rdLevel = n2ndLevel;
								n2ndLevel = nTempLevel;
							}
						}

						else if ( n2ndPoint >= n3rdPoint )
						{
							if ( n2ndPoint == n3rdPoint && n2ndLevel > n3rdLevel )
							{
								nTempPoint = n2ndPoint;
								n2ndPoint = this->m_PartyDataSave[2].nPoint;
								n3rdPoint = nTempPoint;
								nTempTeam = n2ndTeam;
								n2ndTeam = this->m_PartyDataSave[2].btTeamIndex;
								n3rdTeam = nTempTeam;
								nTempLevel = n2ndLevel;
								n2ndLevel = this->m_PartyDataSave[2].n2ndPartyUserLevel + this->m_PartyDataSave[2].nRequestUserLevel;
								n3rdLevel = nTempLevel;
							}
						}

						else
						{
							nTempPoint = n2ndPoint;
							n2ndPoint = this->m_PartyDataSave[2].nPoint;
							n3rdPoint = nTempPoint;
							nTempTeam = n2ndTeam;
							n2ndTeam = this->m_PartyDataSave[2].btTeamIndex;
							n3rdTeam = nTempTeam;
							nTempLevel = n2ndLevel;
							n2ndLevel = this->m_PartyDataSave[2].n2ndPartyUserLevel + this->m_PartyDataSave[2].nRequestUserLevel;
							n3rdLevel = nTempLevel;
						}
					}

					else
					{
						nTempPoint = n1stPoint;
						n1stPoint = this->m_PartyDataSave[2].nPoint;
						n3rdPoint = n2ndPoint;
						n2ndPoint = nTempPoint;
						nTempTeam = n1stTeam;
						n1stTeam = this->m_PartyDataSave[2].btTeamIndex;
						n3rdTeam = n2ndTeam;
						n2ndTeam = nTempTeam;
						nTempLevel = n1stLevel;
						n1stLevel = this->m_PartyDataSave[2].n2ndPartyUserLevel + this->m_PartyDataSave[2].nRequestUserLevel;
						n3rdLevel = n2ndLevel;
						n2ndLevel = nTempLevel;
					}
					break;
				case 3:
					n4thPoint = this->m_PartyDataSave[3].nPoint;
					n4thTeam = this->m_PartyDataSave[3].btTeamIndex;
					n4thLevel = this->m_PartyDataSave[3].n2ndPartyUserLevel + this->m_PartyDataSave[3].nRequestUserLevel;

					if ( n1stPoint >= n4thPoint )
					{
						if ( n1stPoint == n4thPoint )
						{
							if ( n1stLevel <= n4thPoint )
							{
								nTempPoint = n2ndPoint;
								n2ndPoint = this->m_PartyDataSave[3].nPoint;
								n4thPoint = n3rdPoint;
								n3rdPoint = nTempPoint;
								nTempTeam = n2ndTeam;
								n2ndTeam = this->m_PartyDataSave[3].btTeamIndex;
								n4thTeam = n3rdTeam;
								n3rdTeam = nTempTeam;
								nTempLevel = n2ndLevel;
								n2ndLevel = this->m_PartyDataSave[3].n2ndPartyUserLevel + this->m_PartyDataSave[3].nRequestUserLevel;
								n4thLevel = n3rdLevel;
								n3rdLevel = nTempLevel;
							}

							else
							{
								nTempPoint = n1stPoint;
								n1stPoint = this->m_PartyDataSave[3].nPoint;
								n4thPoint = n3rdPoint;
								n3rdPoint = n2ndPoint;
								n2ndPoint = nTempPoint;
								nTempTeam = n1stTeam;
								n1stTeam = this->m_PartyDataSave[3].btTeamIndex;
								n4thTeam = n3rdTeam;
								n3rdTeam = n2ndTeam;
								n2ndTeam = nTempTeam;
								nTempLevel = n1stLevel;
								n1stLevel = this->m_PartyDataSave[3].n2ndPartyUserLevel + this->m_PartyDataSave[3].nRequestUserLevel;
								n4thLevel = n3rdLevel;
								n3rdLevel = n2ndLevel;
								n2ndLevel = nTempLevel;
							}
						}

						else if ( n2ndPoint >= n4thPoint )
						{
							if ( n2ndPoint == n4thPoint )
							{
								if ( n2ndLevel <= n4thPoint )
								{
									nTempPoint = n3rdPoint;
									n3rdPoint = this->m_PartyDataSave[3].nPoint;
									n4thPoint = nTempPoint;
									nTempTeam = n3rdTeam;
									n3rdTeam = this->m_PartyDataSave[3].btTeamIndex;
									n4thTeam = nTempTeam;
									nTempLevel = n3rdLevel;
									n3rdLevel = this->m_PartyDataSave[3].n2ndPartyUserLevel + this->m_PartyDataSave[3].nRequestUserLevel;
									n4thLevel = nTempLevel;
								}

								else
								{
									nTempPoint = n2ndPoint;
									n2ndPoint = this->m_PartyDataSave[3].nPoint;
									n4thPoint = n3rdPoint;
									n3rdPoint = nTempPoint;
									nTempTeam = n2ndTeam;
									n2ndTeam = this->m_PartyDataSave[3].btTeamIndex;
									n4thTeam = n3rdTeam;
									n3rdTeam = nTempTeam;
									nTempLevel = n2ndLevel;
									n2ndLevel = this->m_PartyDataSave[3].n2ndPartyUserLevel + this->m_PartyDataSave[3].nRequestUserLevel;
									n4thLevel = n3rdLevel;
									n3rdLevel = nTempLevel;
								}
							}

							else if ( n3rdPoint >= n4thPoint )
							{
								if ( n3rdPoint == n4thPoint && n3rdLevel > n4thPoint )
								{
									nTempPoint = n3rdPoint;
									n3rdPoint = this->m_PartyDataSave[3].nPoint;
									n4thPoint = nTempPoint;
									nTempTeam = n3rdTeam;
									n3rdTeam = this->m_PartyDataSave[3].btTeamIndex;
									n4thTeam = nTempTeam;
									nTempLevel = n3rdLevel;
									n3rdLevel = this->m_PartyDataSave[3].n2ndPartyUserLevel + this->m_PartyDataSave[3].nRequestUserLevel;
									n4thLevel = nTempLevel;
								}
							}

							else
							{
								nTempPoint = n3rdPoint;
								n3rdPoint = this->m_PartyDataSave[3].nPoint;
								n4thPoint = nTempPoint;
								nTempTeam = n3rdTeam;
								n3rdTeam = this->m_PartyDataSave[3].btTeamIndex;
								n4thTeam = nTempTeam;
								nTempLevel = n3rdLevel;
								n3rdLevel = this->m_PartyDataSave[3].n2ndPartyUserLevel + this->m_PartyDataSave[3].nRequestUserLevel;
								n4thLevel = nTempLevel;
							}
						}

						else
						{
							nTempPoint = n2ndPoint;
							n2ndPoint = this->m_PartyDataSave[3].nPoint;
							n4thPoint = n3rdPoint;
							n3rdPoint = nTempPoint;
							nTempTeam = n2ndTeam;
							n2ndTeam = this->m_PartyDataSave[3].btTeamIndex;
							n4thTeam = n3rdTeam;
							n3rdTeam = nTempTeam;
							nTempLevel = n2ndLevel;
							n2ndLevel = this->m_PartyDataSave[3].n2ndPartyUserLevel + this->m_PartyDataSave[3].nRequestUserLevel;
							n4thLevel = n3rdLevel;
							n3rdLevel = nTempLevel;
						}
					}

					else
					{
						nTempPoint = n1stPoint;
						n1stPoint = this->m_PartyDataSave[3].nPoint;
						n4thPoint = n3rdPoint;
						n3rdPoint = n2ndPoint;
						n2ndPoint = nTempPoint;
						nTempTeam = n1stTeam;
						n1stTeam = this->m_PartyDataSave[3].btTeamIndex;
						n4thTeam = n3rdTeam;
						n3rdTeam = n2ndTeam;
						n2ndTeam = nTempTeam;
						nTempLevel = n1stLevel;
						n1stLevel = this->m_PartyDataSave[3].n2ndPartyUserLevel + this->m_PartyDataSave[3].nRequestUserLevel;
						n4thLevel = n3rdLevel;
						n3rdLevel = n2ndLevel;
						n2ndLevel = nTempLevel;
					}
					break;
				default:
					nNewPoint = this->m_PartyDataSave[i].nPoint;
					nNewLevel = this->m_PartyDataSave[i].n2ndPartyUserLevel + this->m_PartyDataSave[i].nRequestUserLevel;

					if ( n1stPoint >= nNewPoint )
					{
						if ( n1stPoint == nNewPoint )
						{
							if ( n1stLevel <= nNewLevel )
							{
								nTempPoint = n2ndPoint;
								n2ndPoint = this->m_PartyDataSave[i].nPoint;
								n4thPoint = n3rdPoint;
								n3rdPoint = nTempPoint;
								nTempTeam = n2ndTeam;
								n2ndTeam = this->m_PartyDataSave[i].btTeamIndex;
								n4thTeam = n3rdTeam;
								n3rdTeam = nTempTeam;
								nTempLevel = n2ndLevel;
								n2ndLevel = this->m_PartyDataSave[i].n2ndPartyUserLevel + this->m_PartyDataSave[i].nRequestUserLevel;
								n4thLevel = n3rdLevel;
								n3rdLevel = nTempLevel;
							}

							else
							{
								nTempPoint = n1stPoint;
								n1stPoint = this->m_PartyDataSave[i].nPoint;
								n4thPoint = n3rdPoint;
								n3rdPoint = n2ndPoint;
								n2ndPoint = nTempPoint;
								nTempTeam = n1stTeam;
								n1stTeam = this->m_PartyDataSave[i].btTeamIndex;
								n4thTeam = n3rdTeam;
								n3rdTeam = n2ndTeam;
								n2ndTeam = nTempTeam;
								nTempLevel = n1stLevel;
								n1stLevel = this->m_PartyDataSave[i].n2ndPartyUserLevel + this->m_PartyDataSave[i].nRequestUserLevel;
								n4thLevel = n3rdLevel;
								n3rdLevel = n2ndLevel;
								n2ndLevel = nTempLevel;
							}
						}

						else if ( n2ndPoint >= nNewPoint )
						{
							if ( n2ndPoint == nNewPoint )
							{
								if ( n2ndLevel <= nNewLevel )
								{
									nTempPoint = n3rdPoint;
									n3rdPoint = this->m_PartyDataSave[i].nPoint;
									n4thPoint = nTempPoint;
									nTempTeam = n3rdTeam;
									n3rdTeam = this->m_PartyDataSave[i].btTeamIndex;
									n4thTeam = nTempTeam;
									nTempLevel = n3rdLevel;
									n3rdLevel = this->m_PartyDataSave[i].n2ndPartyUserLevel + this->m_PartyDataSave[i].nRequestUserLevel;
									n4thLevel = nTempLevel;
								}

								else
								{
									nTempPoint = n2ndPoint;
									n2ndPoint = this->m_PartyDataSave[i].nPoint;
									n4thPoint = n3rdPoint;
									n3rdPoint = nTempPoint;
									nTempTeam = n2ndTeam;
									n2ndTeam = this->m_PartyDataSave[i].btTeamIndex;
									n4thTeam = n3rdTeam;
									n3rdTeam = nTempTeam;
									nTempLevel = n2ndLevel;
									n2ndLevel = this->m_PartyDataSave[i].n2ndPartyUserLevel + this->m_PartyDataSave[i].nRequestUserLevel;
									n4thLevel = n3rdLevel;
									n3rdLevel = nTempLevel;
								}
							}

							else if ( n3rdPoint >= nNewPoint )
							{
								if ( n3rdPoint == nNewPoint )
								{
									if ( n3rdLevel <= nNewLevel )
									{
										n4thPoint = this->m_PartyDataSave[i].nPoint;
										n4thTeam = this->m_PartyDataSave[i].btTeamIndex;
										n4thLevel = this->m_PartyDataSave[i].n2ndPartyUserLevel + this->m_PartyDataSave[i].nRequestUserLevel;
									}

									else
									{
										nTempPoint = n3rdPoint;
										n3rdPoint = this->m_PartyDataSave[i].nPoint;
										n4thPoint = nTempPoint;
										nTempTeam = n3rdTeam;
										n3rdTeam = this->m_PartyDataSave[i].btTeamIndex;
										n4thTeam = nTempTeam;
										nTempLevel = n3rdLevel;
										n3rdLevel = this->m_PartyDataSave[i].n2ndPartyUserLevel + this->m_PartyDataSave[i].nRequestUserLevel;
										n4thLevel = nTempLevel;
									}
								}

								else if ( n4thPoint >= nNewPoint )
								{
									if ( n4thPoint == nNewPoint && n4thLevel > nNewLevel )
									{
										n4thPoint = this->m_PartyDataSave[i].nPoint;
										n4thTeam = this->m_PartyDataSave[i].btTeamIndex;
										n4thLevel = this->m_PartyDataSave[i].n2ndPartyUserLevel + this->m_PartyDataSave[i].nRequestUserLevel;
									}
								}

								else
								{
									n4thPoint = this->m_PartyDataSave[i].nPoint;
									n4thTeam = this->m_PartyDataSave[i].btTeamIndex;
									n4thLevel = this->m_PartyDataSave[i].n2ndPartyUserLevel + this->m_PartyDataSave[i].nRequestUserLevel;
								}
							}

							else
							{
								nTempPoint = n3rdPoint;
								n3rdPoint = this->m_PartyDataSave[i].nPoint;
								n4thPoint = nTempPoint;
								nTempTeam = n3rdTeam;
								n3rdTeam = this->m_PartyDataSave[i].btTeamIndex;
								n4thTeam = nTempTeam;
								nTempLevel = n3rdLevel;
								n3rdLevel = this->m_PartyDataSave[i].n2ndPartyUserLevel + this->m_PartyDataSave[i].nRequestUserLevel;
								n4thLevel = nTempLevel;
							}
						}

						else
						{
							nTempPoint = n2ndPoint;
							n2ndPoint = this->m_PartyDataSave[i].nPoint;
							n4thPoint = n3rdPoint;
							n3rdPoint = nTempPoint;
							nTempTeam = n2ndTeam;
							n2ndTeam = this->m_PartyDataSave[i].btTeamIndex;
							n4thTeam = n3rdTeam;
							n3rdTeam = nTempTeam;
							nTempLevel = n2ndLevel;
							n2ndLevel = this->m_PartyDataSave[i].n2ndPartyUserLevel + this->m_PartyDataSave[i].nRequestUserLevel;
							n4thLevel = n3rdLevel;
							n3rdLevel = nTempLevel;
						}
					}

					else
					{
						nTempPoint = n1stPoint;
						n1stPoint = this->m_PartyDataSave[i].nPoint;
						n4thPoint = n3rdPoint;
						n3rdPoint = n2ndPoint;
						n2ndPoint = nTempPoint;
						nTempTeam = n1stTeam;
						n1stTeam = this->m_PartyDataSave[i].btTeamIndex;
						n4thTeam = n3rdTeam;
						n3rdTeam = n2ndTeam;
						n2ndTeam = nTempTeam;
						nTempLevel = n1stLevel;
						n1stLevel = this->m_PartyDataSave[i].n2ndPartyUserLevel + this->m_PartyDataSave[i].nRequestUserLevel;
						n4thLevel = n3rdLevel;
						n3rdLevel = n2ndLevel;
						n2ndLevel = nTempLevel;
					}
					break;
			}
		}
	}

	g_Log.Add("[DSF][SaveWinnerPartyPoint] 1stTeam:%d, 2ndTeam:%d, 3rdTeam:%d, 4thTeam:%d",
		n1stTeam, n2ndTeam, n3rdTeam, n4thTeam);

	for (int j = 0; j < 10; j++)
	{
		if (this->m_nDSF_TYPE == 1)
		{
			if ((this->m_PartyDataSave[j].btTeamIndex == n1stTeam || this->m_PartyDataSave[j].btTeamIndex == n2ndTeam || 
				this->m_PartyDataSave[j].btTeamIndex == n3rdTeam || this->m_PartyDataSave[j].btTeamIndex == n4thTeam) &&
				this->m_PartyDataSave[j].nPoint > 0)
			{
				this->GDSaveDSFPartyPoint(this->m_PartyDataSave[j].szRequestUserID,
					this->m_PartyDataSave[j].szRequestUserName,
					this->m_PartyDataSave[j].nRequestUserLevel,
					this->m_PartyDataSave[j].nRequestUserClass,
					this->m_PartyDataSave[j].sz2ndPartyUserID,
					this->m_PartyDataSave[j].sz2ndPartyUserName,
					this->m_PartyDataSave[j].n2ndPartyUserLevel,
					this->m_PartyDataSave[j].n2ndPartyUserClass,
					this->m_PartyDataSave[j].btDSFType + 1,
					this->m_PartyDataSave[j].nPoint,
					1);

				g_Log.Add("[DSF][SaveWinnerPartyPoint] - [%s][%s][%s][%d][%d][%d] / [%s][%s][%s][%d][%d][%d] DSF League Result - DSFType:%d, Point:%d, TeamIndex:%d",
					this->m_PartyDataSave[j].szRequestUserID,
					this->m_PartyDataSave[j].szRequestUserRealName,
					this->m_PartyDataSave[j].szRequestUserName,
					this->m_PartyDataSave[j].nRequestUserServerCode,
					this->m_PartyDataSave[j].nRequestUserLevel,
					this->m_PartyDataSave[j].nRequestUserClass,
					this->m_PartyDataSave[j].sz2ndPartyUserID,
					this->m_PartyDataSave[j].sz2ndPartyUserRealName,
					this->m_PartyDataSave[j].sz2ndPartyUserName,
					this->m_PartyDataSave[j].n2ndPartyUserServerCode,
					this->m_PartyDataSave[j].n2ndPartyUserLevel,
					this->m_PartyDataSave[j].n2ndPartyUserClass,
					this->m_PartyDataSave[j].btDSFType + 1,
					this->m_PartyDataSave[j].nPoint,
					this->m_PartyDataSave[j].btTeamIndex);
			}
		}

		else if (this->m_nDSF_TYPE == 2)
		{
			if ((this->m_PartyDataSave[j].btTeamIndex == n1stTeam || this->m_PartyDataSave[j].btTeamIndex == n2ndTeam) &&
				this->m_PartyDataSave[j].nPoint > 0)
			{
				this->GDSaveDSFPartyPoint(this->m_PartyDataSave[j].szRequestUserID,
					this->m_PartyDataSave[j].szRequestUserName,
					this->m_PartyDataSave[j].nRequestUserLevel,
					this->m_PartyDataSave[j].nRequestUserClass,
					this->m_PartyDataSave[j].sz2ndPartyUserID,
					this->m_PartyDataSave[j].sz2ndPartyUserName,
					this->m_PartyDataSave[j].n2ndPartyUserLevel,
					this->m_PartyDataSave[j].n2ndPartyUserClass,
					this->m_PartyDataSave[j].btDSFType + 1,
					this->m_PartyDataSave[j].nPoint,
					1);

				g_Log.Add("[DSF][SaveWinnerPartyPoint] - [%s][%s][%s][%d][%d][%d] / [%s][%s][%s][%d][%d][%d] DSF SemiFinal Result - DSFType:%d, Point:%d, TeamIndex:%d",
					this->m_PartyDataSave[j].szRequestUserID,
					this->m_PartyDataSave[j].szRequestUserRealName,
					this->m_PartyDataSave[j].szRequestUserName,
					this->m_PartyDataSave[j].nRequestUserServerCode,
					this->m_PartyDataSave[j].nRequestUserLevel,
					this->m_PartyDataSave[j].nRequestUserClass,
					this->m_PartyDataSave[j].sz2ndPartyUserID,
					this->m_PartyDataSave[j].sz2ndPartyUserRealName,
					this->m_PartyDataSave[j].sz2ndPartyUserName,
					this->m_PartyDataSave[j].n2ndPartyUserServerCode,
					this->m_PartyDataSave[j].n2ndPartyUserLevel,
					this->m_PartyDataSave[j].n2ndPartyUserClass,
					this->m_PartyDataSave[j].btDSFType + 1,
					this->m_PartyDataSave[j].nPoint,
					this->m_PartyDataSave[j].btTeamIndex);
			}
		}

		else if (this->m_nDSF_TYPE == 3)
		{
			if (this->m_PartyDataSave[j].btTeamIndex == n1stTeam && this->m_PartyDataSave[j].nPoint > 0)
			{
				this->GDSaveDSFPartyPoint(this->m_PartyDataSave[j].szRequestUserID,
					this->m_PartyDataSave[j].szRequestUserName,
					this->m_PartyDataSave[j].nRequestUserLevel,
					this->m_PartyDataSave[j].nRequestUserClass,
					this->m_PartyDataSave[j].sz2ndPartyUserID,
					this->m_PartyDataSave[j].sz2ndPartyUserName,
					this->m_PartyDataSave[j].n2ndPartyUserLevel,
					this->m_PartyDataSave[j].n2ndPartyUserClass,
					this->m_PartyDataSave[j].btDSFType + 1,
					this->m_PartyDataSave[j].nPoint,
					1);

				g_Log.Add("[DSF][SaveWinnerPartyPoint] - [%s][%s][%s][%d][%d][%d] / [%s][%s][%s][%d][%d][%d] DSF Final Result - DSFType:%d, Point:%d, TeamIndex:%d",
					this->m_PartyDataSave[j].szRequestUserID,
					this->m_PartyDataSave[j].szRequestUserRealName,
					this->m_PartyDataSave[j].szRequestUserName,
					this->m_PartyDataSave[j].nRequestUserServerCode,
					this->m_PartyDataSave[j].nRequestUserLevel,
					this->m_PartyDataSave[j].nRequestUserClass,
					this->m_PartyDataSave[j].sz2ndPartyUserID,
					this->m_PartyDataSave[j].sz2ndPartyUserRealName,
					this->m_PartyDataSave[j].sz2ndPartyUserName,
					this->m_PartyDataSave[j].n2ndPartyUserServerCode,
					this->m_PartyDataSave[j].n2ndPartyUserLevel,
					this->m_PartyDataSave[j].n2ndPartyUserClass,
					this->m_PartyDataSave[j].btDSFType + 1,
					this->m_PartyDataSave[j].nPoint,
					this->m_PartyDataSave[j].btTeamIndex);
			}
		}
	}

	for (int j = 0; j < 10; j++)
	{
		if (this->m_PartyDataSave[j].btTeamIndex == n1stTeam)
		{
			this->GiveDSFWinnerItem(this->m_PartyDataSave[j].nRequestUserIndex, this->m_nDSF_TYPE, 1);
			this->GiveDSFWinnerItem(this->m_PartyDataSave[j].n2ndPartyUserIndex, this->m_nDSF_TYPE, 1);
		}
	}
}

void CDevilSquareFinal::GDSaveDSFPartyPoint(char *szAccountID1, char *szUserName1, int nUser1Level, int nClass1, char *szAccountID2, char *szUserName2, int nUser2Level, int nClass2, int iDSFType, int iPoint, BYTE btEnterCnt)
{
	PMSG_REQ_SAVE_DSF_PARTYPOINT pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.size = sizeof(pMsg);
	pMsg.h.headcode = 0xFD;
	pMsg.h.subcode = 0x02;

	memcpy(pMsg.szAccountID1, szAccountID1, MAX_ACCOUNT_LEN+1);
	memcpy(pMsg.szUserName1, szUserName1, MAX_ACCOUNT_LEN+1);
	pMsg.nUserLevel1 = nUser1Level;
	memcpy(pMsg.szAccountID2, szAccountID2, MAX_ACCOUNT_LEN+1);
	memcpy(pMsg.szUserName2, szUserName2, MAX_ACCOUNT_LEN+1);
	pMsg.nUserLevel2 = nUser2Level;

	pMsg.nDSFType = iDSFType;
	pMsg.nPoint = iPoint;
	pMsg.btEnterCnt = btEnterCnt;

	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);

	pMsg.nEnterYear = sysTime.wYear;
	pMsg.btEnterMonth = sysTime.wMonth;
	pMsg.btEnterDay = sysTime.wDay;

	wsDataCli.DataSend((char *)&pMsg, pMsg.h.size);

	g_Log.Add("[DSF][GDSaveDSFPartyPoint] AccountID1:[%s], UserName1:[%s], User1Level:[%d], AccountID2:[%s], UserName2:[%s], User2Level:[%d], DSFType:[%d], Point:[%d]",
		szAccountID1, szUserName1, nUser1Level, szAccountID2, szUserName2, nUser2Level, iDSFType, iPoint);

	if (iDSFType == 4)
	{
		this->GDInsertRewardUser(szAccountID1, szUserName1, nClass1, 4);
		this->GDInsertRewardUser(szAccountID2, szUserName2, nClass2, 4);
	}
}

void CDevilSquareFinal::GDReqDSFPartyRankRenew(BYTE btDSFType)
{
	PMSG_REQ_DSF_PARTYRANKRENEW pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.size = sizeof(pMsg);
	pMsg.h.headcode = 0xFD;
	pMsg.h.subcode = 0x03;
	pMsg.btDSFType = btDSFType;

	SYSTEMTIME time;
	GetLocalTime(&time);

	pMsg.nEnterYear = time.wYear;
	pMsg.btEnterMonth = time.wMonth;
	pMsg.btEnterDay = time.wDay;

	wsDataCli.DataSend((char *)&pMsg, pMsg.h.size);
}

void CDevilSquareFinal::GDReqDSFGoFinalParty(int nDSFType)
{
	PMSG_REQ_DSF_GO_FINAL_PARTY pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.size = sizeof(pMsg);
	pMsg.h.headcode = 0xFD;
	pMsg.h.subcode = 0x05;
	pMsg.btDSFType = nDSFType;

	SYSTEMTIME time;
	GetLocalTime(&time);

	pMsg.nEnterYear = time.wYear;
	pMsg.btEnterMonth = time.wMonth;
	pMsg.btEnterDay = time.wDay;

	wsDataCli.DataSend((char *)&pMsg, pMsg.h.size);
}

void CDevilSquareFinal::GDInsertRewardUser(char *szAccountID, char *szUserName, int iClass, int iDSFType)
{
	PMSG_REQ_SAVE_DSF_REWARD_USER pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.size = sizeof(pMsg);
	pMsg.h.headcode = 0xFD;
	pMsg.h.subcode = 0x06;

	memcpy(pMsg.szAccountID, szAccountID, MAX_ACCOUNT_LEN+1);
	memcpy(pMsg.szUserName, szUserName, MAX_ACCOUNT_LEN+1);
	pMsg.iClass = iClass;
	pMsg.btDSFType = iDSFType;

	SYSTEMTIME time;
	GetLocalTime(&time);

	pMsg.nRewardYear = time.wYear;
	pMsg.btRewardMonth = time.wMonth;
	pMsg.btRewardStartDay = this->m_DSFRewardDay.RewardStartDay;
	pMsg.btRewardEndDay = this->m_DSFRewardDay.RewardEndDay;

	wsDataCli.DataSend((char *)&pMsg, pMsg.h.size);
}

void CDevilSquareFinal::GDReqGetReward(int aIndex)
{
	PMSG_REQ_GET_DSF_REWARD pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.size = sizeof(pMsg);
	pMsg.h.headcode = 0xFD;
	pMsg.h.subcode = 0x07;

	memcpy(pMsg.szAccountID, gObj[aIndex].AccountID, MAX_ACCOUNT_LEN+1);
	memcpy(pMsg.szUserName, gObj[aIndex].Name, MAX_ACCOUNT_LEN+1);
	pMsg.nServerCode = g_ConfigRead.server.GetGameServerCode() / 20;
	pMsg.nUserIndex = aIndex;

	SYSTEMTIME time;
	GetLocalTime(&time);

	pMsg.nRewardYear = time.wYear;
	pMsg.btRewardMonth = time.wMonth;
	pMsg.btRewardDay = time.wDay;

	wsDataCli.DataSend((char *)&pMsg, pMsg.h.size);
	g_Log.Add("[DSF][GDReqGetReward][%s][%s][%s][%d] Request Get Reward",
		gObj[aIndex].AccountID, gObj[aIndex].m_PlayerData->m_RealNameOfUBF, 
		gObj[aIndex].Name, gObj[aIndex].m_PlayerData->m_nServerCodeOfHomeWorld);
}

void CDevilSquareFinal::DSF_ProcessInit()
{
	for (int nIndex = 0; nIndex < 20; nIndex++)
	{
		this->ClearUserData(nIndex);
	}

	this->m_nTickCount = GetTickCount();
	this->m_btWinTeam = -1;
	this->m_bEntrance = false;
	this->m_nCurBattleStartTime = -1;
	this->m_nRemainTimeMsec = -1;
	this->m_bSendMsgBeforeReady = false;
	this->m_nEventEndRemainTime = 0;
	this->m_bResultPointSend = false;
	this->m_bWinnerResultMsgSend = false;
	this->m_bGameStart = false;

	EnterCriticalSection(&this->m_criPartyData);

	for (int i = 0; i < 10; i++)
	{
		this->m_PartyDataSave[i].bEntered = false;
		this->m_PartyDataSave[i].bRequest = false;
		this->m_PartyDataSave[i].btDSFType = 0;

		if (this->m_PartyDataSave[i].nPartyNo >= 0)
		{
			gParty.ClearPartyAuthDSF(this->m_PartyDataSave[i].nPartyNo);
			this->m_PartyDataSave[i].nPartyNo = -1;
		}

		this->m_PartyDataSave[i].btTeamIndex = -1;
		this->m_PartyDataSave[i].nRequestUserIndex = -1;
		this->m_PartyDataSave[i].n2ndPartyUserIndex = -1;
		memset(this->m_PartyDataSave[i].szRequestUserID, 0, MAX_ACCOUNT_LEN+1);
		memset(this->m_PartyDataSave[i].sz2ndPartyUserID, 0, MAX_ACCOUNT_LEN+1);
		memset(this->m_PartyDataSave[i].szRequestUserName, 0, MAX_ACCOUNT_LEN+1);
		memset(this->m_PartyDataSave[i].sz2ndPartyUserName, 0, MAX_ACCOUNT_LEN+1);
		memset(this->m_PartyDataSave[i].szRequestUserRealName, 0, MAX_ACCOUNT_LEN+1);
		memset(this->m_PartyDataSave[i].sz2ndPartyUserRealName, 0, MAX_ACCOUNT_LEN+1);
		this->m_PartyDataSave[i].nRequestUserServerCode = -1;
		this->m_PartyDataSave[i].n2ndPartyUserServerCode = -1;
		this->m_PartyDataSave[i].nRequestUserClass = -1;
		this->m_PartyDataSave[i].n2ndPartyUserClass = -1;
		this->m_PartyDataSave[i].nRequestUserLevel = -1;
		this->m_PartyDataSave[i].n2ndPartyUserLevel = -1;
		this->m_PartyDataSave[i].nPoint = 0;
	}

	LeaveCriticalSection(&this->m_criPartyData);

	for (int k = 0; k < 20; k++)
	{
		this->m_EnteredPlayerName[k].EnteredIndex = -1;
		memset(this->m_EnteredPlayerName[k].PlayerName, 0x00, sizeof(this->m_EnteredPlayerName[k].PlayerName));
	}

	this->m_btDSFRound = 0;

	for (int i = 0; i < 10; i++)
	{
		this->m_nTeamUserCount[i] = 0;
		this->m_nTeamPoint[i] = 0;
	}
}

int CDevilSquareFinal::SetMonster()
{
	int iAddMonsterCount = 0;
	int nMonsterType = 0;
	int iMonsterMaxNumber = 0;
	int iMonsterKind = -1;

	for (int i = 0; i < 15; i++)
	{
		if (this->m_DSFMonsterInfo[i].RoundNum == this->m_btDSFRound)
		{
			nMonsterType = this->m_DSFMonsterInfo[i].MonsterType;
			iMonsterMaxNumber = this->m_DSFMonsterInfo[i].MonsterMaxNumber;
			iMonsterKind = this->m_DSFMonsterInfo[i].MonsterKind;
			break;
		}
	}

	for (int n = 0; n < 200; n++)
	{
		if (this->m_DSFMonsterRegenInfo[n].MonsterMapNum == MAP_INDEX_DEVILSQUARE_FINAL &&
			this->m_DSFMonsterRegenInfo[n].MonsterKind >= iMonsterKind)
		{
			if (iMonsterMaxNumber < iAddMonsterCount)
			{
				break;
			}

			int result = gObjAddMonster(this->m_DSFMonsterRegenInfo[n].MonsterMapNum);

			if (result >= 0)
			{
				gObj[result].m_PosNum = n;
				gObj[result].X = this->m_DSFMonsterRegenInfo[n].RegenX;
				gObj[result].Y = this->m_DSFMonsterRegenInfo[n].RegenY;
				gObj[result].MapNumber = this->m_DSFMonsterRegenInfo[n].MonsterMapNum;
				gObj[result].TX = gObj[result].X;
				gObj[result].TY = gObj[result].Y;
				gObj[result].m_OldX = gObj[result].X;
				gObj[result].m_OldY = gObj[result].Y;
				gObj[result].Dir = this->m_DSFMonsterRegenInfo[n].MonsterDir;
				gObj[result].StartX = gObj[result].X;
				gObj[result].StartY = gObj[result].Y;
				gObjSetMonster(result, nMonsterType);
				gObj[result].MaxRegenTime = 500;
				gObj[result].Dir = rand()%8;
				gObj[result].m_ViewRange = 8;
				iAddMonsterCount++;
			}
		}
	}

	int iEventMonsterCount = 0;
	int iSuccessRate = 0;
	int nEventMonsterType = 0;
	int iEventMonsterMaxNumber = -1;
	int iEventMonsterKind = -1;

	for (int i = 0; i < 15; i++)
	{
		if (this->m_DSFEventMonsterRate[i].RoundNum == this->m_btDSFRound)
		{
			iSuccessRate = this->m_DSFEventMonsterRate[i].Rate;
			break;
		}
	}

	if (rand()%100 < iSuccessRate)
	{
		for (int k = 0; k < 15; k++)
		{
			if (this->m_DSFEventMonsterInfo[k].RoundNum == this->m_btDSFRound)
			{
				nEventMonsterType = this->m_DSFEventMonsterInfo[k].MonsterType;
				iEventMonsterMaxNumber = this->m_DSFEventMonsterInfo[k].MonsterMaxNumber;
				iEventMonsterKind = this->m_DSFEventMonsterInfo[k].MonsterKind;
				break;
			}
		}
	}

	for (int l = 0; l < 200; l++)
	{
		if (this->m_DSFMonsterRegenInfo[l].MonsterMapNum == MAP_INDEX_DEVILSQUARE_FINAL &&
			this->m_DSFMonsterRegenInfo[l].MonsterKind >= iEventMonsterKind)
		{
			if (iEventMonsterMaxNumber < iEventMonsterCount)
			{
				break;
			}

			int result = gObjAddMonster(this->m_DSFMonsterRegenInfo[l].MonsterMapNum);

			if (result >= 0)
			{
				gObj[result].m_PosNum = l;
				gObj[result].X = this->m_DSFMonsterRegenInfo[l].RegenX;
				gObj[result].Y = this->m_DSFMonsterRegenInfo[l].RegenY;
				gObj[result].MapNumber = this->m_DSFMonsterRegenInfo[l].MonsterMapNum;
				gObj[result].TX = gObj[result].X;
				gObj[result].TY = gObj[result].Y;
				gObj[result].m_OldX = gObj[result].X;
				gObj[result].m_OldY = gObj[result].Y;
				gObj[result].Dir = this->m_DSFMonsterRegenInfo[l].MonsterDir;
				gObj[result].StartX = gObj[result].X;
				gObj[result].StartY = gObj[result].Y;
				gObj[result].m_Disappear_Monster = GetTickCount();
				gObj[result].MaxRegenTime = 5000;
				gObjSetMonster(result, nEventMonsterType);
				gObj[result].MaxRegenTime = 500;
				gObj[result].Dir = rand()%8;
				gObj[result].m_ViewRange = 8;
				iEventMonsterCount++;
			}
		}
	}

	return iAddMonsterCount;
}

void CDevilSquareFinal::DSFMonsterRegen(OBJECTSTRUCT * lpObj)
{
	if (lpObj->Class == -1)
	{
		g_Log.Add("[DSF][DSFMonsterRegen] [%d] Invalid MonterClass", lpObj->Class);
		return;
	}

	for (int i = 0; i < 15; i++)
	{
		if (this->m_DSFEventMonsterInfo[i].MonsterType == lpObj->Class)
		{
			return;
		}
	}

	lpObj->X = this->m_DSFMonsterRegenInfo[lpObj->m_PosNum].RegenX;
	lpObj->Y = this->m_DSFMonsterRegenInfo[lpObj->m_PosNum].RegenY;
	lpObj->MapNumber = this->m_DSFMonsterRegenInfo[lpObj->m_PosNum].MonsterMapNum;
	lpObj->TX = lpObj->X;
	lpObj->TY = lpObj->Y;
	lpObj->m_OldX = lpObj->X;
	lpObj->m_OldY = lpObj->Y;
	lpObj->Dir = this->m_DSFMonsterRegenInfo[lpObj->m_PosNum].MonsterDir;
	lpObj->StartX = lpObj->X;
	lpObj->StartY = lpObj->Y;
	gObjSetMonster(lpObj->m_Index, lpObj->Class);
	lpObj->DieRegen = 0;
	lpObj->MaxRegenTime = 500;
	lpObj->Dir = rand()%8;
	gObjMonsterHitDamageInit(lpObj);
	gObjMonsterRegen(lpObj);
	lpObj->MTX = lpObj->X;
	lpObj->MTY = lpObj->Y;
	CreateFrustrum(lpObj->X, lpObj->Y, lpObj->m_Index);

	g_Log.Add("[DSF][DSFMonsterRegen] Monster Regen [%d][%d,%d]", lpObj->Class, lpObj->X, lpObj->Y);
}

void CDevilSquareFinal::ClearMonster()
{
	for (int n = 0; n < g_ConfigRead.server.GetObjectMaxMonster(); n++)
	{
		if (gObj[n].MapNumber == MAP_INDEX_DEVILSQUARE_FINAL)
		{
			gObjDel(n);
		}
	}

	g_Log.Add("[DSF][ClearMonster] Clear Monster");
}

BYTE CDevilSquareFinal::GetUserTeam(int nIndex)
{
	if (!ObjectMaxRange(nIndex))
	{
		return -1;
	}

	if (gObj[nIndex].m_nDSFIndex == -1)
	{
		return -1;
	}

	if (this->m_UserData[gObj[nIndex].m_nDSFIndex].m_nIndex != nIndex)
	{
		return -1;
	}

	if (gObj[nIndex].Connected != PLAYER_PLAYING)
	{
		return -1;
	}

	return this->m_UserData[gObj[nIndex].m_nDSFIndex].m_btTeam;
}

int CDevilSquareFinal::MonsterKillPoint(int nMonsterType)
{
	int nPoint = 0;
	bool bEventMonster = false;

	for (int i = 0; i < 15; i++)
	{
		if (this->m_DSFEventMonsterInfo[i].MonsterType == nMonsterType)
		{
			nPoint = this->m_DSFEventMonsterInfo[i].MonsterPoint;
			bEventMonster = true;
			break;
		}
	}

	if (bEventMonster == false)
	{
		for (int i = 0; i < 15; i++)
		{
			if (this->m_DSFMonsterInfo[i].MonsterType == nMonsterType)
			{
				nPoint = this->m_DSFMonsterInfo[i].MonsterPoint;
				break;
			}
		}
	}

	return nPoint;
}

void CDevilSquareFinal::MonsterKillPointCalc(int aIndex, int nPoint)
{
	for (int i = 0; i < 20; i++)
	{
		if (this->m_UserData[i].m_nIndex != -1 && 
			gObj[this->m_UserData[i].m_nIndex].Connected > PLAYER_LOGGED && 
			this->m_UserData[i].m_nIndex == aIndex)
		{
			this->m_UserData[i].m_nPoint += nPoint;
			this->m_nTeamPoint[this->m_UserData[i].m_btTeam] += nPoint;
			return;
		}
	}
}

void CDevilSquareFinal::DSFUserDie(OBJECTSTRUCT * lpObj)
{
	if (lpObj->m_nDSFIndex == -1)
	{
		return;
	}

	if (this->m_UserData[lpObj->m_nDSFIndex].m_nIndex != lpObj->m_Index)
	{
		return;
	}

	if (!gObjIsConnected(lpObj->m_Index))
	{
		return;
	}

	EnterCriticalSection(&this->m_critUserData);

	this->m_nTeamUserCount[this->m_UserData[lpObj->m_nDSFIndex].m_btTeam]--;
	lpObj->m_nDSFIndex = -1;
	this->m_nUserCount--;

	LeaveCriticalSection(&this->m_critUserData);

	g_Log.Add("[DSF][DSFUserDie][%s][%s][%s][%d] UserDie",
		lpObj->AccountID, lpObj->m_PlayerData->m_RealNameOfUBF,
        lpObj->Name, lpObj->m_PlayerData->m_nServerCodeOfHomeWorld);
}

void CDevilSquareFinal::EndUserMove()
{
	for (int i = 0; i < 20; i++)
	{
		if (this->m_UserData[i].m_nIndex != -1 &&
			gObj[this->m_UserData[i].m_nIndex].Connected > PLAYER_LOGGED &&
			gObj[this->m_UserData[i].m_nIndex].MapNumber == MAP_INDEX_DEVILSQUARE_FINAL)
		{
			int aIndex = this->m_UserData[i].m_nIndex;
			gObj[aIndex].m_nDSFIndex = -1;
			gObjMoveGate(aIndex, 333);
			gObj[aIndex].m_Change = -1;
			gObjViewportListProtocolCreate(&gObj[aIndex]);
		}
	}
}

void CDevilSquareFinal::DSFDropReward(OBJECTSTRUCT * lpUser, OBJECTSTRUCT * lpMonster)
{
	if (this->m_btDSFRound != 5 && this->m_btDSFRound != 10 && this->m_btDSFRound != 15)
	{
		return;
	}

	WORD wItemCode = -1;

	if (this->m_nDSF_TYPE == 1)
	{
		wItemCode = ITEMGET(14,258);
	}

	else if (this->m_nDSF_TYPE == 2)
	{
		wItemCode = ITEMGET(14,259);
	}

	else if (this->m_nDSF_TYPE == 3)
	{
		wItemCode = ITEMGET(14,260);
	}

	if (rand()%10000 < this->m_DSFUnitInfo.MonsterRewardRate[this->m_nDSF_TYPE-1])
	{
		ItemSerialCreateSend(lpUser->m_Index, lpUser->MapNumber, lpUser->X, lpUser->Y, wItemCode, 0, 0, 0, 0, 0, lpUser->m_Index, 0, 0, 0, 0, 0);
	}
}

void CDevilSquareFinal::SetUBFGetReward(int iUserIndex, WORD wItemCode, UINT64 ItemSerial, BYTE btItemPos)
{
	if (!ObjectMaxRange(iUserIndex))
	{
		return;
	}

	LPOBJ lpObj = &gObj[iUserIndex];

	if (lpObj->Type != OBJ_USER)
	{
		return;
	}

	if (wItemCode != ITEMGET(14,258) && wItemCode != ITEMGET(14,259) && wItemCode != ITEMGET(14,260))
	{
		return;
	}

	gObjInventoryDeleteItem(iUserIndex, btItemPos);
	GSProtocol.GCInventoryItemDeleteSend(iUserIndex, btItemPos, 0);

	this->GDReqSetDSFReward_UBF(iUserIndex, this->m_nDSF_TYPE, 0);
}

void CDevilSquareFinal::GDReqSetDSFReward_UBF(int iUserIndex, BYTE btDSFType, BYTE btRewardType)
{
	if (!ObjectMaxRange(iUserIndex))
	{
		return;
	}

	LPOBJ lpObj = &gObj[iUserIndex];

	if (lpObj->Type != OBJ_USER)
	{
		return;
	}

	g_Log.Add("[UBF][DevilSquareFinal][GDReqSetDSFWinnerInformation]ACC:%s , NAME :%s,Real NAME :%s, DSFType:%d, RewardType:%d",
		lpObj->AccountID, lpObj->Name, lpObj->m_PlayerData->m_RealNameOfUBF, btDSFType, btRewardType);

	PMSG_REQ_SET_DSF_WINNER_INFO pMsg;
	
	memcpy(pMsg.UBFName, lpObj->Name, MAX_ACCOUNT_LEN+1);
	pMsg.btDSFType = btDSFType;
	pMsg.btRewardType = btRewardType;

	PHeadSubSetB((LPBYTE)&pMsg, 0xFD, 0x08, sizeof(pMsg));
	wsDataCli.DataSend((char *)&pMsg, pMsg.h.size);
}

void CDevilSquareFinal::GiveDSFWinnerItem(int iWinnerIndex, BYTE btDSFType, BYTE btRewardType)
{
	if (!gObjIsConnected(iWinnerIndex))
	{
		return;
	}

	if (g_ConfigRead.server.GetServerType() == SERVER_BATTLECORE)
	{
		this->GDReqSetDSFReward_UBF(iWinnerIndex, btDSFType, 1);
		return;
	}

	_stGremoryCaseItem stItem;
	WORD wItemType = -1;

	if (btDSFType == 1) // Ellimination
	{
		if (btRewardType == 0) // Monster Drop
		{
			wItemType = ITEMGET(14,258);
		}

		else if (btRewardType == 1) // Winner Prize
		{
			wItemType = ITEMGET(14,259);
		}
	}

	else if (btDSFType == 2) // Semi-Final
	{
		if (btRewardType == 0) // Monster Drop
		{
			wItemType = ITEMGET(14,259);
		}

		else if (btRewardType == 1) // Winner Prize
		{
			wItemType = ITEMGET(14,260);
		}
	}

	else if (btDSFType == 3) // Final
	{
		if (btRewardType == 0) // Monster Drop
		{
			wItemType = ITEMGET(14,260);
		}

		else if (btRewardType == 1) // Winner Prize
		{
			wItemType = ITEMGET(13,277);
		}
	}

	stItem.btStorageType = GC_STORAGE_CHARACTER;
	stItem.btRewardSource = GC_REWARD_TORMENTED_SQUARE;
	stItem.wItemID = wItemType;

	g_GremoryCase.GDReqAddItemToGremoryCase(iWinnerIndex, stItem, 30);
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


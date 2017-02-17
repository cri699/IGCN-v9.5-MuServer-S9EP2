//////////////////////////////////////////////////////////////////////
// IllusionTempleLeagueEvent.cpp
#include "stdafx.h"
#include "IllusionTempleLeagueEvent.h"
#include "configread.h"
#include "IniReader.h"
#include "ReadScript.h"
#include "GameMain.h"
#include "winutil.h"
#include "MapServerManager.h"
#include "DSProtocol.h"

CIllusionTempleLeagueEvent g_IllusionTempleLeagueEvent;

bool CIllusionTempleLeagueEvent::m_bITL_Event_Enable;
bool CIllusionTempleLeagueEvent::m_bITL_INFO_LOAD_ALL;
bool CIllusionTempleLeagueEvent::m_bITL_DayFlagChange;
BYTE CIllusionTempleLeagueEvent::m_byITLFlagTimer;
bool CIllusionTempleLeagueEvent::m_bITL_SemiFinalEnd;
bool CIllusionTempleLeagueEvent::m_bITL_SemiFinalEnd_0;
bool CIllusionTempleLeagueEvent::m_bITL_SemiFinalEnd_1;
bool CIllusionTempleLeagueEvent::m_bITL_FinalEnd;
bool CIllusionTempleLeagueEvent::m_bSendOk_RewardRenew;
bool CIllusionTempleLeagueEvent::m_bSendGetITLRewardList;
bool CIllusionTempleLeagueEvent::m_bITL_SemiFinalEnd_Renew;
BYTE CIllusionTempleLeagueEvent::m_byITLRenewCount;
bool CIllusionTempleLeagueEvent::m_bITL_SemiFinalEnd_GetDone;
bool CIllusionTempleLeagueEvent::m_bITL_FinalEnd_Renew;
bool CIllusionTempleLeagueEvent::m_bITL_FinalEnd_GetDone;
BYTE CIllusionTempleLeagueEvent::m_byITLFinalStart;
int CIllusionTempleLeagueEvent::m_byITL_REWARDDAY;
bool CIllusionTempleLeagueEvent::m_bLoadOk_RewardList;

int CIllusionTempleLeagueEvent::m_nITLOpenTime;
int CIllusionTempleLeagueEvent::m_nITLReadyTime;
int CIllusionTempleLeagueEvent::m_nITLPlayTime;
int CIllusionTempleLeagueEvent::m_nITLEndTime;

std::list<__ITL_TIME_TABLE> CIllusionTempleLeagueEvent::m_lstALLITL_TIME;

CIllusionTempleLeagueEvent::CIllusionTempleLeagueEvent()
{
}

CIllusionTempleLeagueEvent::~CIllusionTempleLeagueEvent()
{
}

int CIllusionTempleLeagueEvent::IllusionTempleLeague_EventInit()
{
	this->m_nITL_TYPE = this->WhatIsTodayTypeOfITL();

	if (this->m_nITL_TYPE == 1 || this->m_nITL_TYPE == 2 || this->m_nITL_TYPE == 3)
	{
		this->SetTodaySchedule(this->m_nITL_TYPE);

		for (int i = 0; i < 2; i++)
		{
			this->m_cIllusionTempleLeagueProc[i].ITL_ProcessInit(i);
		}
	}

	CIllusionTempleLeagueEvent::m_bITL_INFO_LOAD_ALL = TRUE;
	return TRUE;
}

int CIllusionTempleLeagueEvent::Load_ITL_EventInfo()
{
	CIniReader ITLIni(g_ConfigRead.GetPath("\\Events\\IllusionTemple_League\\IllusionTemple_League.ini"));
	CIllusionTempleLeagueEvent::m_bITL_Event_Enable = ITLIni.ReadInt("General", "EventEnabled", 0);

	this->Load_ITL_NPC_Position(g_ConfigRead.GetPath("\\Events\\IllusionTemple_League\\ITL_NPCPosition.dat"));
	this->Load_ITL_Script(g_ConfigRead.GetPath("\\Events\\IllusionTemple_League\\ITL_Script.dat"));

	return TRUE;
}

void CIllusionTempleLeagueEvent::Load_ITL_NPC_Position(char *filename)
{
	SMDFile = fopen(filename, "r");

	if (!SMDFile)
	{
		g_Log.MsgBox("[ ITL ] IllusionTempleLeagueNpc.txt File Load Fail %s", filename);
		return;
	}

	for (int nCount = 0;; nCount++)
	{
		GetToken();

		if (!strcmp("end", TokenString))
			break;

		this->m_ITL_NPC[nCount].wNpcType = TokenNumber;

		GetToken();
		this->m_ITL_NPC[nCount].byMapNumber = TokenNumber;

		GetToken();
		this->m_ITL_NPC[nCount].byDistance = TokenNumber;

		GetToken();
		this->m_ITL_NPC[nCount].byX = TokenNumber;

		GetToken();
		this->m_ITL_NPC[nCount].byY = TokenNumber;

		GetToken();
		this->m_ITL_NPC[nCount].byDir = TokenNumber;
	}

	fclose(SMDFile);
}

void CIllusionTempleLeagueEvent::Load_ITL_Script(char *filename)
{
	SMDFile = fopen(filename, "r");

	if (!SMDFile)
	{
		g_Log.MsgBox("[Illusion Temple League] Info file Load Fail [%s]", filename);
		return;
	}

	int Token;
	int type;
	__ITL_TIME_TABLE startTime;

	while (true)
	{
		Token = GetToken();

		if (Token == END)
			break;

		type = TokenNumber;

		if (type == 0)
		{
			while (true)
			{
				Token = GetToken();

				if (!strcmp("end", TokenString))
					break;

				CIllusionTempleLeagueEvent::m_nITLOpenTime = TokenNumber;

				Token = GetToken();
				CIllusionTempleLeagueEvent::m_nITLReadyTime = TokenNumber;

				Token = GetToken();
				CIllusionTempleLeagueEvent::m_nITLPlayTime = TokenNumber;

				Token = GetToken();
				CIllusionTempleLeagueEvent::m_nITLEndTime = TokenNumber;
			}
		}

		else if (type == 1)
		{
			while (true)
			{
				Token = GetToken();

				if (!strcmp("end", TokenString))
					break;

				startTime.nHour = TokenNumber;

				Token = GetToken();
				startTime.nMin = TokenNumber;

				Token = GetToken();
				startTime.nType = TokenNumber;

				CIllusionTempleLeagueEvent::m_lstALLITL_TIME.push_back(startTime);
			}
		}

		else if (type == 2)
		{
			while (true)
			{
				Token = GetToken();

				if (!strcmp("end", TokenString))
					break;

				this->m_nEnterLevelLimit = TokenNumber;
			}
		}

		else if (type == 3)
		{
			int iCount = 0;

			while (true)
			{
				Token = GetToken();

				if (!strcmp("end", TokenString))
					break;

				this->m_LeagueDays[iCount] = TokenNumber;
				iCount++;

				if (iCount > 7)
				{
					g_Log.MsgBox("ERROR CIllusionTempleLeagueEvent::LoadIllusionTempleScript Cannot over 7 leaguedays i:%d", iCount);
					return;
				}
			}
		}

		else if (type == 4)
		{
			while (true)
			{
				Token = GetToken();

				if (!strcmp("end", TokenString))
					break;

				this->m_SemiFinal = TokenNumber;

				Token = GetToken();
				this->m_Final = TokenNumber;

				Token = GetToken();
				CIllusionTempleLeagueEvent::m_byITL_REWARDDAY = TokenNumber;
			}
		}

		else if (type == 5)
		{
			while (true)
			{
				Token = GetToken();

				if (!strcmp("end", TokenString))
					break;

				this->m_byTournamentRankPos1 = TokenNumber;

				Token = GetToken();
				this->m_byTournamentRankPos2 = TokenNumber;

				Token = GetToken();
				this->m_byTournamentRankPos3 = TokenNumber;

				Token = GetToken();
				this->m_byTournamentRankPos4 = TokenNumber;
			}
		}
	}

	fclose(SMDFile);
}

void CIllusionTempleLeagueEvent::ITL_Run()
{
	if (CIllusionTempleLeagueEvent::m_bITL_Event_Enable == false)
	{
		return;
	}

	this->m_nITL_TYPE = this->WhatIsTodayTypeOfITL();

	if (this->m_nITL_TYPE != 0 && this->m_nITL_TYPE != 4)
	{
		int nCurTime = GetTickCount();
		bool bCheckSync = false;

		if (this->m_cIllusionTempleLeagueProc[0].GetRealRemainTimeMin() == 10 &&
			this->m_cIllusionTempleLeagueProc[0].GetAllCheckSync() == false)
		{
			bCheckSync = true;
			this->m_cIllusionTempleLeagueProc[0].SetAllCheckSync();
		}

		for (int i = 0; i < 2; i++)
		{
			if (bCheckSync == true)
			{
				this->m_cIllusionTempleLeagueProc[i].CheckSync();
			}

			this->m_cIllusionTempleLeagueProc[i].Proc_ITL(nCurTime);
		}
	}

	else
	{
		CIllusionTempleLeagueEvent::m_bITL_Event_Enable = false;
	}
}

void CIllusionTempleLeagueEvent::SetRelicsInventoryPos(BYTE btMapNumber, int aIndex, BYTE btPos)
{
	if (!ITL_MAP_RANGE(btMapNumber))
	{
		return;
	}

	this->m_cIllusionTempleLeagueProc[btMapNumber - 98].SetRelicsInventoryPos(aIndex, btPos);
}

void CIllusionTempleLeagueEvent::SendRelicsUserInfo(OBJECTSTRUCT* lpObj)
{
	if (!ITL_MAP_RANGE(lpObj->MapNumber))
	{
		return;
	}

	this->m_cIllusionTempleLeagueProc[lpObj->MapNumber - 98].SendRelicsUserInfo(lpObj, 0);
}

BOOL CIllusionTempleLeagueEvent::Enter_ITL(int aIndex, BYTE byTempleIndex, BYTE bySlotNum)
{
	if (CIllusionTempleLeagueEvent::m_bITL_Event_Enable == false)
	{
		return FALSE;
	}

	if (byTempleIndex < 0 || byTempleIndex > 1)
	{
		return FALSE;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if (!lpObj)
	{
		return FALSE;
	}

	if (this->m_cIllusionTempleLeagueProc[byTempleIndex].EnterUserIllusionTempleLeague(aIndex, byTempleIndex, lpObj->m_PlayerData->GuildNumber, lpObj->PartyNumber) == TRUE)
	{
		g_Log.Add("[ ITL ] ITL index:(%d) EnterUser: (%s)(%s) class:%d",
			byTempleIndex + 1, lpObj->AccountID, lpObj->Name, lpObj->m_PlayerData->DbClass);

		this->m_cIllusionTempleLeagueProc[byTempleIndex].SendIllusionTempleLeagueState(0, lpObj->m_Index);

		return TRUE;
	}

	else
	{
		g_Log.Add("[ ITL ] ITL index:(%d) EnterUser FAIL: (%s)(%s) class:%d",
			byTempleIndex + 1, lpObj->AccountID, lpObj->Name, lpObj->m_PlayerData->DbClass);

		return FALSE;
	}
}

int CIllusionTempleLeagueEvent::Leave_ITL(int aIndex, BYTE btMapNumber)
{
	if (!ITL_MAP_RANGE(btMapNumber))
	{
		return FALSE;
	}

	this->m_cIllusionTempleLeagueProc[btMapNumber - 98].LeaveUserIllusionTempleLeague(aIndex);
	return TRUE;
}

void CIllusionTempleLeagueEvent::ActRelicsGetOrRegister(OBJECTSTRUCT *lpNpc, OBJECTSTRUCT *lpObj, BYTE byMapNumber, BYTE byPrePacket, BYTE byAct, WORD wNpcType)
{
	if (!ITL_MAP_RANGE(byMapNumber))
	{
		return;
	}

	if (byPrePacket || byAct)
	{
		if (byPrePacket != 1 || byAct)
		{
			if (byPrePacket || byAct != 1)
			{
				if (byPrePacket == 1)
				{
					if (byAct == 1)
					{
						this->m_cIllusionTempleLeagueProc[byMapNumber - 98].RegisterRelics(lpNpc, lpObj);
					}
				}
			}

			else
			{
				this->m_cIllusionTempleLeagueProc[byMapNumber - 98].Check_RegisterRelics(lpNpc, lpObj);
			}
		}

		else
		{
			this->m_cIllusionTempleLeagueProc[byMapNumber - 98].GetRelics(lpNpc, lpObj, wNpcType);
		}
	}

	else
	{
		this->m_cIllusionTempleLeagueProc[byMapNumber - 98].Check_GetRelics(lpNpc, lpObj, wNpcType);
	}
}

void CIllusionTempleLeagueEvent::CancleStoneState(WORD wNpcType, BYTE byState, BYTE mapnumber)
{
	if (!ITL_MAP_RANGE(mapnumber))
	{
		return;
	}

	if (this->m_cIllusionTempleLeagueProc[mapnumber - 98].GetIllusionTempleLeagueState() == 2)
	{
		this->m_cIllusionTempleLeagueProc[mapnumber - 98].CancleStoneStateProcess(wNpcType, byState);
	}
}

void CIllusionTempleLeagueEvent::DropRelicsItem(BYTE btMapNumber, int nIndex)
{
	if (!ITL_MAP_RANGE(btMapNumber))
	{
		return;
	}

	this->m_cIllusionTempleLeagueProc[btMapNumber - 98].DropRelicsItem(nIndex, 1);
}

BYTE CIllusionTempleLeagueEvent::GetIllusionTempleLeagueState(BYTE btMapNumber)
{
	return this->m_cIllusionTempleLeagueProc[btMapNumber - 98].GetIllusionTempleLeagueState();
}

void CIllusionTempleLeagueEvent::AddNpcPosNum()
{
	for (int i = 0; i < 22; i++)
	{
		if (this->m_ITL_NPC[i].byMapNumber == MAP_INDEX_ILLUSIONTEMPLE7)
		{
			this->m_cIllusionTempleLeagueProc[0].AddITLNpcInfo(this->m_ITL_NPC[i].wNpcType, this->m_ITL_NPC[i].byMapNumber, 
				this->m_ITL_NPC[i].byDistance, this->m_ITL_NPC[i].byX, this->m_ITL_NPC[i].byY, this->m_ITL_NPC[i].byDir);
		}

		else if (this->m_ITL_NPC[i].byMapNumber == MAP_INDEX_ILLUSIONTEMPLE8)
		{
			this->m_cIllusionTempleLeagueProc[1].AddITLNpcInfo(this->m_ITL_NPC[i].wNpcType, this->m_ITL_NPC[i].byMapNumber,
				this->m_ITL_NPC[i].byDistance, this->m_ITL_NPC[i].byX, this->m_ITL_NPC[i].byY, this->m_ITL_NPC[i].byDir);
		}
	}
}

void CIllusionTempleLeagueEvent::ResetPosNum()
{
	for (int i = 0; i < 2; i++)
	{
		this->m_cIllusionTempleLeagueProc[i].ResetITLPosNum();
	}
}

BYTE CIllusionTempleLeagueEvent::GetUserTeam(BYTE btMapNumber, int nIndex)
{
	if (!ITL_MAP_RANGE(btMapNumber))
	{
		return -1;
	}

	return this->m_cIllusionTempleLeagueProc[btMapNumber - 98].GetUserTeam(nIndex);
}

void CIllusionTempleLeagueEvent::SetStatusRegenTime(BYTE btMapNumber)
{
	if (!ITL_MAP_RANGE(btMapNumber))
	{
		return;
	}

	this->m_cIllusionTempleLeagueProc[btMapNumber - 98].SetStatusRegenTime();
}

void CIllusionTempleLeagueEvent::ITL_UseSkill(int iIndex, WORD MagicNumber, int wTargetObjIndex, BYTE btDis)
{
	if (!ObjectMaxRange(iIndex))
	{
		g_Log.Add("ITL_UseSkill Error #1 iIndex :%d ", iIndex);
		return;
	}

	if (!ITL_MAP_RANGE(gObj[iIndex].MapNumber))
	{
		g_Log.Add("ITL_UseSkill Error #2 iIndex :%d Map:%d ", iIndex, gObj[iIndex].MapNumber);
		return;
	}

	this->m_cIllusionTempleLeagueProc[gObj[iIndex].MapNumber - 98].ITL_UseSkill(iIndex, MagicNumber, wTargetObjIndex, btDis);
}

void CIllusionTempleLeagueEvent::EventSkillProc(OBJECTSTRUCT* lpObj)
{
	return this->m_cIllusionTempleLeagueProc[lpObj->MapNumber - 98].ITLSkillProc(lpObj);
}

void CIllusionTempleLeagueEvent::IllusionTempleLeagueUserDie(OBJECTSTRUCT *lpObj)
{
	this->m_cIllusionTempleLeagueProc[lpObj->MapNumber - 98].ITLUserDie(lpObj);
}

void CIllusionTempleLeagueEvent::IllusionTempleLeagueUserDieRegen(LPOBJ lpObj)
{
	if (!ObjectMaxRange(lpObj->m_Index))
	{
		return;
	}

	if (!ITL_MAP_RANGE(lpObj->MapNumber))
	{
		return;
	}

	if (this->m_cIllusionTempleLeagueProc[lpObj->MapNumber - 98].GetIllusionTempleLeagueState() == 2)
	{
		this->m_cIllusionTempleLeagueProc[lpObj->MapNumber - 98].UseSkillProdection(lpObj);
	}
}

BOOL CIllusionTempleLeagueEvent::CheckWearingMOPH(int nUserIndex)
{
	if (!gObjIsConnected(nUserIndex))
		return 0;

	if (gObj[nUserIndex].pInventory[10].IsItem() == TRUE)
	{
		if (gObj[nUserIndex].pInventory[10].m_Type == ITEMGET(13,10))
			return TRUE;
		if (gObj[nUserIndex].pInventory[10].m_Type == ITEMGET(13,39))
			return TRUE;
		if (gObj[nUserIndex].pInventory[10].m_Type == ITEMGET(13,40))
			return TRUE;
		if (gObj[nUserIndex].pInventory[10].m_Type == ITEMGET(13,41))
			return TRUE;
		if (gObj[nUserIndex].pInventory[10].m_Type == ITEMGET(13,68))
			return TRUE;
		if (gObj[nUserIndex].pInventory[10].m_Type == ITEMGET(13,76))
			return TRUE;
		if (gObj[nUserIndex].pInventory[10].m_Type == ITEMGET(13,77))
			return TRUE;
		if (gObj[nUserIndex].pInventory[10].m_Type == ITEMGET(13,78))
			return TRUE;
		if (gObj[nUserIndex].pInventory[10].m_Type == ITEMGET(13,122))
			return TRUE;
		if (gObj[nUserIndex].pInventory[10].m_Type == ITEMGET(13,163))
			return TRUE;
		if (gObj[nUserIndex].pInventory[10].m_Type == ITEMGET(13,164))
			return TRUE;
		if (gObj[nUserIndex].pInventory[10].m_Type == ITEMGET(13,166))
			return TRUE;
		if (gObj[nUserIndex].pInventory[10].m_Type == ITEMGET(13,169))
			return TRUE;
		if (gObj[nUserIndex].pInventory[10].m_Type == ITEMGET(13,170))
			return TRUE;
		if (gObj[nUserIndex].pInventory[10].m_Type == ITEMGET(13,268))
			return TRUE;
	}

	if (gObj[nUserIndex].pInventory[11].IsItem() == TRUE)
	{
		if (gObj[nUserIndex].pInventory[11].m_Type == ITEMGET(13,10))
			return TRUE;
		if (gObj[nUserIndex].pInventory[11].m_Type == ITEMGET(13,39))
			return TRUE;
		if (gObj[nUserIndex].pInventory[11].m_Type == ITEMGET(13,40))
			return TRUE;
		if (gObj[nUserIndex].pInventory[11].m_Type == ITEMGET(13,41))
			return TRUE;
		if (gObj[nUserIndex].pInventory[11].m_Type == ITEMGET(13,68))
			return TRUE;
		if (gObj[nUserIndex].pInventory[11].m_Type == ITEMGET(13,76))
			return TRUE;
		if (gObj[nUserIndex].pInventory[11].m_Type == ITEMGET(13,77))
			return TRUE;
		if (gObj[nUserIndex].pInventory[11].m_Type == ITEMGET(13,78))
			return TRUE;
		if (gObj[nUserIndex].pInventory[11].m_Type == ITEMGET(13,122))
			return TRUE;
		if (gObj[nUserIndex].pInventory[11].m_Type == ITEMGET(13,163))
			return TRUE;
		if (gObj[nUserIndex].pInventory[11].m_Type == ITEMGET(13,164))
			return TRUE;
		if (gObj[nUserIndex].pInventory[11].m_Type == ITEMGET(13,166))
			return TRUE;
		if (gObj[nUserIndex].pInventory[11].m_Type == ITEMGET(13,169))
			return TRUE;
		if (gObj[nUserIndex].pInventory[11].m_Type == ITEMGET(13,170))
			return TRUE;
		if (gObj[nUserIndex].pInventory[11].m_Type == ITEMGET(13,268))
			return TRUE;
	}

	return FALSE;
}

BOOL CIllusionTempleLeagueEvent::CheckChangeRing(int nType)
{
	int bRet = 0;
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
			bRet = 1;
			break;
		default:
			return bRet;
	}
	return bRet;
}

BOOL CIllusionTempleLeagueEvent::CheckRelics(int nIndex)
{
	if (!gObjIsConnected(nIndex))
	{
		return FALSE;
	}

	if (!ObjectMaxRange(nIndex))
	{
		return FALSE;
	}

	if (!ITL_MAP_RANGE(gObj[nIndex].MapNumber))
	{
		return FALSE;
	}

	if (this->m_cIllusionTempleLeagueProc[gObj[nIndex].MapNumber - 98].GetIllusionTempleLeagueState() != 2)
	{
		return FALSE;
	}

	if (nIndex == this->m_cIllusionTempleLeagueProc[gObj[nIndex].MapNumber - 98].GetRelicsUserIndex(&gObj[nIndex]))
	{
		return TRUE;
	}

	return FALSE;
}

void CIllusionTempleLeagueEvent::ReqEventReward(int nIndex)
{
	if (!ITL_MAP_RANGE(gObj[nIndex].MapNumber))
	{
		return;
	}

	this->m_cIllusionTempleLeagueProc[gObj[nIndex].MapNumber - 98].DropChaosGem(nIndex);
}

int CIllusionTempleLeagueEvent::GetRemainTime()
{
	if (CIllusionTempleLeagueEvent::m_bITL_Event_Enable == false)
	{
		return 0;
	}

	return this->m_cIllusionTempleLeagueProc[0].GetEnterRemainTimeMin();
}

BOOL CIllusionTempleLeagueEvent::CheckSkillProdection(int iIllusionTempleIndex, BYTE btMapNumber)
{
	if (!ITL_MAP_RANGE(btMapNumber))
	{
		return FALSE;
	}

	return this->m_cIllusionTempleLeagueProc[btMapNumber - 98].CheckSkillProdection(iIllusionTempleIndex);
}

BOOL CIllusionTempleLeagueEvent::CheckSkillRestraint(int iIllusionTempleIndex, BYTE btMapNumber)
{
	if (!ITL_MAP_RANGE(btMapNumber))
	{
		return FALSE;
	}

	return this->m_cIllusionTempleLeagueProc[btMapNumber - 98].CheckSkillRestraint(iIllusionTempleIndex);
}

bool CIllusionTempleLeagueEvent::IsCanEnterTiming()
{
	return this->m_cIllusionTempleLeagueProc[0].GetIllusionTempleLeagueState() == 0 && this->m_cIllusionTempleLeagueProc[0].GetEntrance() == true;
}

bool CIllusionTempleLeagueEvent::IsRewardDay()
{
	SYSTEMTIME time;
	GetLocalTime(&time);

	return time.wDay == CIllusionTempleLeagueEvent::m_byITL_REWARDDAY;
}

bool CIllusionTempleLeagueEvent::IsLeagueDays()
{
	SYSTEMTIME time;
	GetLocalTime(&time);

	for (int i = 0; i < 7; i++)
	{
		if (time.wDay == this->m_LeagueDays[i])
		{
			return TRUE;
		}
	}
	
	return FALSE;
}

bool CIllusionTempleLeagueEvent::IsSemiFinalDay()
{
	SYSTEMTIME time;
	GetLocalTime(&time);

	return time.wDay == this->m_SemiFinal;
}

bool CIllusionTempleLeagueEvent::IsFinalDay()
{
	SYSTEMTIME time;
	GetLocalTime(&time);

	return time.wDay == this->m_Final;
}

BYTE CIllusionTempleLeagueEvent::WhatIsTodayTypeOfITL()
{
	SYSTEMTIME time;
	GetLocalTime(&time);

	if (time.wDay == this->m_SemiFinal)
	{
		return 2;
	}

	else if (time.wDay == this->m_Final)
	{
		return 3;
	}

	for (int i = 0; i < 7; i++)
	{
		if (time.wDay == this->m_LeagueDays[i])
		{
			return 1;
		}
	}

	if (time.wDay == CIllusionTempleLeagueEvent::m_byITL_REWARDDAY)
	{
		return 4;
	}

	return 0;
}

void CIllusionTempleLeagueEvent::ClearTodaySchedule()
{
	return; // empty function
}

void CIllusionTempleLeagueEvent::SetTodaySchedule(BYTE ITLType)
{
	std::list<__ITL_TIME_TABLE>::iterator it = CIllusionTempleLeagueEvent::m_lstALLITL_TIME.begin();

	for (int i = 0; i < 2; i++)
	{
		this->m_cIllusionTempleLeagueProc[i].m_lst_ITL_Today_StartTime.clear();
	}

	__ITL_TIME_TABLE itltimetable;

	while (true)
	{
		if (it == CIllusionTempleLeagueEvent::m_lstALLITL_TIME.end())
		{
			break;
		}

		if (it->nType > 0 && it->nType == ITLType)
		{
			itltimetable.nHour = it->nHour;
			itltimetable.nMin = it->nMin;
			itltimetable.nType = it->nType;

			for (int i = 0; i < 2; i++)
			{
				this->m_cIllusionTempleLeagueProc[i].m_lst_ITL_Today_StartTime.push_back(itltimetable);
			}
		}

		it++;
	}
}

BYTE CIllusionTempleLeagueEvent::Update_GuildInfo(int nGuildIdx, BYTE byMapNumber, BYTE bySlotNumber, int nPartyIdx, BYTE byITLType, int nUserIndex)
{
	return this->m_cIllusionTempleLeagueProc[byMapNumber].SetGuild(nGuildIdx, bySlotNumber, byITLType, nPartyIdx, nUserIndex);
}

BYTE CIllusionTempleLeagueEvent::IsEmptyITLGuildInfo(BYTE byITLIndex, BYTE byEmptySlot)
{
	return this->m_cIllusionTempleLeagueProc[byITLIndex].IsEmptySlot(byEmptySlot);
}

BYTE CIllusionTempleLeagueEvent::Find_EmptySlot(int nGuildIndex, BYTE &byEmptySlot)
{
	BYTE bReturn = 0xFF;

	for (int nMapNumber = 0; nMapNumber < 2; nMapNumber++)
	{
		bReturn = this->m_cIllusionTempleLeagueProc[nMapNumber].Find_EmptyGuildSlot(nGuildIndex, byEmptySlot);

		if ((byEmptySlot == 0 || byEmptySlot == 1) && bReturn == 0)
		{
			return nMapNumber;
		}
	}

	return bReturn;
}

void CIllusionTempleLeagueEvent::Send_ITL_Schedule(int index)
{
	PMSG_ANS_ITL_SCHEDULE pMsg;
	__ITL_TIME_TABLE pMsgTime;

	char sendbuf[1024];
	int lOfs;
	int Cnt;

	PHeadSubSetW((LPBYTE)&pMsg, 0xBF, 0x5C, sizeof(pMsg));
	Cnt = 0;
	memset(sendbuf, 0x00, sizeof(sendbuf));
	lOfs = sizeof(PMSG_ANS_ITL_SCHEDULE);

	pMsg.LeagueDay1 = this->m_LeagueDays[0];
	pMsg.LeagueDay2 = this->m_LeagueDays[1];
	pMsg.LeagueDay3 = this->m_LeagueDays[2];
	pMsg.LeagueDay4 = this->m_LeagueDays[3];
	pMsg.LeagueDay5 = this->m_LeagueDays[4];
	pMsg.LeagueDay6 = this->m_LeagueDays[5];
	pMsg.LeagueDay7 = this->m_LeagueDays[6];
	pMsg.SemiFinal = this->m_SemiFinal;
	pMsg.Final = this->m_Final;
	pMsg.ITL_STATE = this->m_cIllusionTempleLeagueProc[0].GetIllusionTempleLeagueState();

	std::list<__ITL_TIME_TABLE>::iterator it = CIllusionTempleLeagueEvent::m_lstALLITL_TIME.begin();

	while (true)
	{
		if (it == CIllusionTempleLeagueEvent::m_lstALLITL_TIME.end())
		{
			break;
		}

		pMsgTime.nHour = it->nHour;
		pMsgTime.nMin = it->nMin;
		pMsgTime.nType = it->nType;
		Cnt++;

		memcpy(&sendbuf[lOfs], &pMsgTime, sizeof(pMsgTime));
		lOfs += sizeof(__ITL_TIME_TABLE);

		it++;
	}

	pMsg.byCount = Cnt;
	pMsg.h.sizeH = HIBYTE(lOfs);
	pMsg.h.sizeL = LOBYTE(lOfs);

	memcpy(sendbuf, (LPBYTE)&pMsg, sizeof(pMsg));
	IOCP.DataSend(index, (LPBYTE)sendbuf, lOfs);
}

BYTE CIllusionTempleLeagueEvent::FindMyEnemy(char* GuildName)
{
	for (int i = 0; i < 56; i++)
	{
		if (!strcmp(this->m_GuildLeagueRank[i].szGuildName, GuildName))
		{
			return this->m_GuildLeagueRank[i].byRank;
		}
	}

	return 0;
}

void CIllusionTempleLeagueEvent::Send_ITL_Tournament(int aIndex, BYTE byError)
{
	PMSG_ITL_GUILD_RANKINFO pMsg;
	__ITL_GUILD_RANK pGuildInfo;
	PHeadSubSetW((LPBYTE)&pMsg, 0xBF, 0x5E, sizeof(pMsg));

	char sendbuf[1024];
	memset(sendbuf, 0x00, sizeof(sendbuf));
	int lOfs = sizeof(pMsg);
	BYTE byGuildRankCnt = 0;

	if (byError)
	{
		pMsg.byResult = byError;
	}

	else
	{
		if (this->WhatIsTodayTypeOfITL() != 2 || CIllusionTempleLeagueEvent::m_bITL_SemiFinalEnd != 0)
		{
			pMsg.byResult = 0;

			for (int j = 0; j < 6; j++)
			{
				pGuildInfo.byRank = this->FindMyEnemy(this->m_GuildTournamentRank[j].szGuildName);
				pGuildInfo.byWin = this->m_GuildTournamentRank[j].byWin;
				pGuildInfo.byLose = this->m_GuildTournamentRank[j].byLose;
				pGuildInfo.byITLType = this->m_GuildTournamentRank[j].byITLType;
				memcpy(pGuildInfo.szGuildName, this->m_GuildTournamentRank[j].szGuildName, MAX_GUILD_LEN + 1);

				memcpy(&sendbuf[lOfs], &pGuildInfo, sizeof(pGuildInfo));
				lOfs += sizeof(pGuildInfo);
				byGuildRankCnt++;
			}
		}

		else
		{
			for (int i = 0; i < 4; i++)
			{
				pGuildInfo.byRank = this->m_GuildLeagueRank[i].byRank;
				pGuildInfo.byWin = this->m_GuildLeagueRank[i].byWin;
				pGuildInfo.byLose = this->m_GuildLeagueRank[i].byLose;
				pGuildInfo.byITLType = 1;
				memcpy(pGuildInfo.szGuildName, this->m_GuildLeagueRank[i].szGuildName, MAX_GUILD_LEN + 1);

				memcpy(&sendbuf[lOfs], &pGuildInfo, sizeof(pGuildInfo));
				lOfs += sizeof(pGuildInfo);
				byGuildRankCnt++;
			}

			pMsg.byResult = 0;
		}
	}

	pMsg.byCnt = byGuildRankCnt;
	pMsg.h.sizeH = HIBYTE(lOfs);
	pMsg.h.sizeL = LOBYTE(lOfs);
	memcpy(sendbuf, &pMsg, sizeof(pMsg));

	IOCP.DataSend(aIndex, (LPBYTE)sendbuf, lOfs);
	gObj[aIndex].m_bITL_TournamentInfoSend = true;
}

void CIllusionTempleLeagueEvent::Send_ITL_GuildRankInfo(int aIndex)
{
	PMSG_ITL_GUILD_RANKINFO pMsg;
	__ITL_GUILD_RANK pGuildInfo;
	PHeadSubSetW((LPBYTE)&pMsg, 0xBF, 0x5E, sizeof(pMsg));

	char sendbuf[1024];
	memset(sendbuf, 0x00, sizeof(sendbuf));
	int lOfs = sizeof(pMsg);
	BYTE byGuildRankCnt = 0;

	for (int i = 0; i < 56; i++)
	{
		pGuildInfo.byRank = this->m_GuildLeagueRank[i].byRank;
		pGuildInfo.byWin = this->m_GuildLeagueRank[i].byWin;
		pGuildInfo.byLose = this->m_GuildLeagueRank[i].byLose;
		memcpy(pGuildInfo.szGuildName, this->m_GuildLeagueRank[i].szGuildName, MAX_GUILD_LEN + 1);

		memcpy(&sendbuf[lOfs], &pGuildInfo, sizeof(pGuildInfo));
		lOfs += sizeof(pGuildInfo);
		byGuildRankCnt++;
	}

	pMsg.byCnt = byGuildRankCnt;
	pMsg.byResult = 0;
	pMsg.h.sizeH = HIBYTE(lOfs);
	pMsg.h.sizeL = LOBYTE(lOfs);

	memcpy(sendbuf, &pMsg, sizeof(pMsg));
	IOCP.DataSend(aIndex, (LPBYTE)sendbuf, lOfs);
	gObj[aIndex].m_bITL_GuildRankInfoSend = true;
}

bool CIllusionTempleLeagueEvent::IsRewardRenewDay()
{
	SYSTEMTIME time;
	GetLocalTime(&time);

	return time.wDay == CIllusionTempleLeagueEvent::m_byITL_REWARDDAY;
}

void CIllusionTempleLeagueEvent::GD_LoadGuildCount(char *GuildName, int UserIndex)
{
	_tagPMSG_REQ_ITL_GUILDCOUNT_REQ pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.headcode = 0xF0;
	pMsg.h.subcode = 0xAB;
	pMsg.wMapSvrNum = g_MapServerManager.GetMapSvrGroup();
	pMsg.nUserIndex = UserIndex;
	memcpy(pMsg.GuildName, GuildName, MAX_GUILD_LEN + 1);

	wsDataCli.DataSend((char *)&pMsg, pMsg.h.size);
}

void CIllusionTempleLeagueEvent::GD_Load_ITLUserEnterCount(char *CharName, int UserIndex)
{
	_tagPMSG_REQ_ITL_USERCOUNT_REQ pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.headcode = 0xF0;
	pMsg.h.subcode = 0xAC;
	pMsg.nUserIndex = UserIndex;
	memcpy(pMsg.CharName, CharName, MAX_ACCOUNT_LEN + 1);

	wsDataCli.DataSend((char *)&pMsg, pMsg.h.size);
}

void CIllusionTempleLeagueEvent::GD_ITL_GuildRank_Renew(BYTE byITLType)
{
	_tagPMSG_REQ_ITL_GUILDRANKRENEW pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.size = sizeof(pMsg);
	pMsg.h.headcode = 0xF0;
	pMsg.h.subcode = 0xA5;
	pMsg.byITLType = byITLType;

	wsDataCli.DataSend((char *)&pMsg, pMsg.h.size);
}

void CIllusionTempleLeagueEvent::GD_ITL_GuildRank_Get()
{
	_tagPMSG_REQ_ITL_GUILDRANK pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.size = sizeof(pMsg);
	pMsg.h.headcode = 0xF0;
	pMsg.h.subcode = 0xA6;

	wsDataCli.DataSend((char *)&pMsg, pMsg.h.size);
}

void CIllusionTempleLeagueEvent::GD_ITL_TournamentRank_Get(BYTE byITLType)
{
	_tagPMSG_REQ_ITL_GUILD_TOURNAMENTRANK pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.size = sizeof(pMsg);
	pMsg.h.headcode = 0xF0;
	pMsg.h.subcode = 0xA7;
	pMsg.byITLType = byITLType;

	wsDataCli.DataSend((char *)&pMsg, pMsg.h.size);
}

void CIllusionTempleLeagueEvent::DG_ITL_Tournament_Get(_tagPMSG_ANS_ITL_TOURNAMENT *lpMsg)
{
	if (!lpMsg)
	{
		return;
	}

	if (g_ConfigRead.server.GetServerType() != SERVER_MARKET)
	{
		return;
	}

	for (int i = 0; i < lpMsg->byGuildCount; i++)
	{
		if (lpMsg->m_GuildTournamentRank[i].byRank <= 0)
		{
			memset(this->m_GuildTournamentRank[i].szGuildName, 0, MAX_GUILD_LEN + 1);
			this->m_GuildTournamentRank[i].byRank = 0;
			this->m_GuildTournamentRank[i].byWin = 0;
			this->m_GuildTournamentRank[i].byLose = 0;
			this->m_GuildTournamentRank[i].byITLType = 0;
		}

		else
		{
			memcpy(this->m_GuildTournamentRank[i].szGuildName, lpMsg->m_GuildTournamentRank[i].szGuildName, MAX_GUILD_LEN + 1);
			this->m_GuildTournamentRank[i].byRank = lpMsg->m_GuildTournamentRank[i].byRank;
			this->m_GuildTournamentRank[i].byWin = lpMsg->m_GuildTournamentRank[i].byWin;
			this->m_GuildTournamentRank[i].byLose = lpMsg->m_GuildTournamentRank[i].byLose;
			this->m_GuildTournamentRank[i].byITLType = lpMsg->m_GuildTournamentRank[i].byITLType;
		}
	}
}

void CIllusionTempleLeagueEvent::DG_ITL_RewardList_Get(_tagPMSG_ANS_ITL_REWARDLIST *lpMsg)
{
	if (!lpMsg)
	{
		return;
	}

	if (g_ConfigRead.server.GetServerType() != SERVER_MARKET)
	{
		return;
	}

	for (int i = 0; i < 5; i++)
	{
		if (lpMsg->m_stRewardList[i].byRank > 0 && lpMsg->m_stRewardList[i].byRank < 6)
		{
			this->m_ITL_Winner_RewardList[i].byEnteredCnt = lpMsg->m_stRewardList[i].byEnterCount;
			this->m_ITL_Winner_RewardList[i].byPoint = lpMsg->m_stRewardList[i].byPoint;
			this->m_ITL_Winner_RewardList[i].byRank = lpMsg->m_stRewardList[i].byRank;
			this->m_ITL_Winner_RewardList[i].bGotReward = lpMsg->m_stRewardList[i].byGotReward;
			memcpy(this->m_ITL_Winner_RewardList[i].szCharName, lpMsg->m_stRewardList[i].szCharName, MAX_ACCOUNT_LEN + 1);
			memcpy(this->m_ITL_Winner_RewardList[i].szGuildName, lpMsg->m_stRewardList[i].szGuildName, MAX_GUILD_LEN + 1);

			g_Log.Add("[ ITL ] REWARD RECEIVE NAME:%s, G.NAME:%s , EnterCnt:%d, Point:%d, Rank:%d, Get:%d",
				this->m_ITL_Winner_RewardList[i].szCharName,
				this->m_ITL_Winner_RewardList[i].szGuildName,
				this->m_ITL_Winner_RewardList[i].byEnteredCnt,
				this->m_ITL_Winner_RewardList[i].byPoint,
				this->m_ITL_Winner_RewardList[i].byRank,
				this->m_ITL_Winner_RewardList[i].bGotReward);

		}
	}

	CIllusionTempleLeagueEvent::m_bLoadOk_RewardList = true;
}

void CIllusionTempleLeagueEvent::DG_ITL_UserCount(_tagPMSG_ANS_ITL_USERCOUNTANS *lpMsg)
{
	if (!lpMsg)
	{
		return;
	}

	int aIndex = lpMsg->nUserIndex;

	if (!gObjIsConnected(aIndex))
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if (!lpObj)
	{
		return;
	}

	lpObj->m_byEnterITLUserCount = lpMsg->byCount;
	g_Log.Add("[ ITL ][ DG_ITL_UserCount ] Name :%s ,Cnt:%d",
		lpObj->Name, lpObj->m_byEnterITLUserCount);
}

void CIllusionTempleLeagueEvent::DG_ITL_GuildCount(_tagPMSG_ANS_ITL_GUILDCOUNTANS *lpMsg)
{
	int aIndex = lpMsg->nUserIndex;

	if (!gObjIsConnected(aIndex))
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if (!lpObj)
	{
		return;
	}

	lpObj->m_bEnterCountLoad = true;
	lpObj->m_byEnterITLCount = lpMsg->byCount;
	g_Log.Add("[ ITL ][ DG_ITL_GuildCount ] Name :%s ,Cnt:%d",
		lpObj->Name, lpMsg->byCount);
}

void CIllusionTempleLeagueEvent::DG_ITL_GuildLeagueRank_Get(_tagPMSG_ANS_ITL_GUILDLEAGUERANK *lpMsg)
{
	if (!lpMsg)
	{
		return;
	}

	if (g_ConfigRead.server.GetServerType() != SERVER_MARKET)
	{
		return;
	}

	for (int i = 0; i < lpMsg->byGuildCount; i++)
	{
		if (lpMsg->m_GuildLeagueRank[i].byRank <= 0)
		{
			memset(this->m_GuildLeagueRank[i].szGuildName, 0, MAX_GUILD_LEN + 1);
			this->m_GuildLeagueRank[i].byRank = 0;
			this->m_GuildLeagueRank[i].byWin = 0;
			this->m_GuildLeagueRank[i].byLose = 0;
		}

		else
		{
			memcpy(this->m_GuildLeagueRank[i].szGuildName, lpMsg->m_GuildLeagueRank[i].szGuildName, MAX_GUILD_LEN + 1);
			this->m_GuildLeagueRank[i].byRank = lpMsg->m_GuildLeagueRank[i].byRank;
			this->m_GuildLeagueRank[i].byWin = lpMsg->m_GuildLeagueRank[i].byWin;
			this->m_GuildLeagueRank[i].byLose = lpMsg->m_GuildLeagueRank[i].byLose;
		}
	}

	this->m_bGuildRankGetDone = true;
}

void CIllusionTempleLeagueEvent::ResetITL_FlagAll()
{
	for (int i = g_ConfigRead.server.GetObjectStartUserIndex(); i < g_ConfigRead.server.GetObjectMax(); i++)
	{
		LPOBJ lpObj = &gObj[i];

		if (gObjIsConnected(i) == TRUE && lpObj->Type == OBJ_USER)
		{
			if (g_ConfigRead.server.GetServerType() == SERVER_MARKET)
			{
				lpObj->m_bITL_GuildRankInfoSend = false;
				lpObj->m_bITL_ScheduleInfoSend = false;
				lpObj->m_bITL_TournamentInfoSend = false;
				lpObj->m_nITLRelicsTick = 0;
				lpObj->m_bRegisteringRelics = false;
				lpObj->m_bGettingRelics = false;
				lpObj->m_wITLNpcType = 0;
				lpObj->m_byStoneState = 99;
				lpObj->m_byEnterITLCount = -1;
				lpObj->m_bEnterCountLoad = false;
			}
		}
	}
}

BYTE CIllusionTempleLeagueEvent::Find_SameGuild_AlreadyEntered(int nGuildIdx, int &ReqUserIndex)
{
	BYTE bResult1 = this->m_cIllusionTempleLeagueProc[0].Find_SameGuild(nGuildIdx, ReqUserIndex);
	BYTE bResult2 = this->m_cIllusionTempleLeagueProc[1].Find_SameGuild(nGuildIdx, ReqUserIndex);

	if (bResult1 && bResult2)
	{
		if (bResult1 != 2 && bResult2 != 2)
		{
			return 0;
		}

		else
		{
			return 20;
		}
	}
	
	return -2;
}

BYTE CIllusionTempleLeagueEvent::Find_ExistingGuildInfo(int nGuildIdx, BYTE &byMapNum, BYTE &bySlotNum)
{
	BYTE bReturn1 = 0xFF;
	BYTE bReturn2 = 0xFF;

	for (int MapNum = 0; MapNum < 2; MapNum++)
	{
		int nTempNotUseIndex = -1;
		bReturn1 = this->m_cIllusionTempleLeagueProc[MapNum].Find_SameGuild(nGuildIdx, nTempNotUseIndex);

		if (bReturn1 == 0)
		{
			byMapNum = MapNum;
			bReturn2 = this->m_cIllusionTempleLeagueProc[MapNum].Find_GuildSlot(nGuildIdx, bySlotNum);

			if (bReturn2 == 0)
			{
				return 0;
			}
		}
	}

	return -1;
}

BYTE CIllusionTempleLeagueEvent::Delete_GuildInfo(int nGuildIdx)
{
	for (int i = 0; i < 2; i++)
	{
		if (this->m_cIllusionTempleLeagueProc[i].DeleteGuild(nGuildIdx) == 0)
		{
			return 0;
		}
	}

	return 1;
}

BYTE CIllusionTempleLeagueEvent::GetFinalTournament(int nGuildIdx, char *GuildName)
{
	if (nGuildIdx < 0)
	{
		return -2;
	}

	for (int i = 0; i < 6; i++)
	{
		if (this->m_GuildTournamentRank[i].byITLType == 2 &&
			!strcmp(this->m_GuildTournamentRank[i].szGuildName, GuildName))
		{
			return this->m_GuildTournamentRank[i].byRank;
		}
	}

	return -1;
}

BYTE CIllusionTempleLeagueEvent::GetMyGuildRankAtTournament(int nGuildIdx, char *GuildName)
{
	if (nGuildIdx < 0)
	{
		return -2;
	}

	for (int i = 0; i < 56; i++)
	{
		if (!strcmp(this->m_GuildLeagueRank[i].szGuildName, GuildName))
		{
			return this->m_GuildLeagueRank[i].byRank;
		}
	}

	return -1;
}

BYTE CIllusionTempleLeagueEvent::Find_MyPartyAndGuildInfo(BYTE byMapNumber, BYTE byArrayOrder, int GuildIndex, BYTE &byTeamIndex, int &nPartyIdx)
{
	return this->m_cIllusionTempleLeagueProc[byMapNumber].FindGuildInfo(GuildIndex, byTeamIndex, nPartyIdx);
}

BYTE CIllusionTempleLeagueEvent::Find_AlreadyEnteredUser(char *Name, BYTE &byMapNumber)
{
	for (int i = 0; i < 2; i++)
	{
		int nArrayIdx = this->m_cIllusionTempleLeagueProc[i].FindSameNameIndex(Name);

		if (nArrayIdx != 255 && nArrayIdx < 10)
		{
			byMapNumber = i;
			return nArrayIdx;
		}
	}

	return -1;
}

BYTE CIllusionTempleLeagueEvent::CheckCanReEnterTiming(BYTE byMapNumber, BYTE &byNowState)
{
	byNowState = this->m_cIllusionTempleLeagueProc[byMapNumber].GetIllusionTempleLeagueState();

	if (byNowState != 2 && byNowState != 1 && byNowState != 4)
	{
		return -1;
	}

	return 0;
}

BYTE CIllusionTempleLeagueEvent::ReEnterITL(int aIndex, BYTE btTempleIndex, int nGuildIdx, int nPartyIdx, int nUserArrayNo, int nITLState, BYTE byTeamIndex)
{
	return this->m_cIllusionTempleLeagueProc[btTempleIndex].ReEnterUserIllusionTempleLeague(aIndex, btTempleIndex, nGuildIdx, nPartyIdx, nUserArrayNo, byTeamIndex);
}

void CIllusionTempleLeagueEvent::Insert_RewardList(char *Name, char *GuildName, BYTE byRank, BYTE byPoint, BYTE byEnter)
{
	for (int i = 0; i < 5; i++)
	{
		if (this->m_ITL_Winner_RewardList[i].byRank == 0)
		{
			this->m_ITL_Winner_RewardList[i].byEnteredCnt = byEnter;
			this->m_ITL_Winner_RewardList[i].byPoint = byPoint;
			this->m_ITL_Winner_RewardList[i].byRank = byRank;
			memcpy(this->m_ITL_Winner_RewardList[i].szCharName, Name, MAX_ACCOUNT_LEN + 1);
			memcpy(this->m_ITL_Winner_RewardList[i].szGuildName, GuildName, MAX_GUILD_LEN + 1);
		}
	}
}

void CIllusionTempleLeagueEvent::Send_RewardList(int aIndex, BYTE byError)
{
	PMSG_ANS_REWARDLIST pMsg;
	_tagITL_REWARDLIST ITLRewardList;

	PHeadSubSetW((LPBYTE)&pMsg, 0xBF, 0x68, sizeof(pMsg));

	char sendbuff[1024];
	memset(sendbuff, 0x00, sizeof(sendbuff));
	int lOfs = sizeof(pMsg);

	if (this->IsRewardDay() == false)
	{
		pMsg.byResult = 1;
	}

	else
	{
		int nCount = 0;

		for (int i = 0; i < 5; i++)
		{
			ITLRewardList.byRank = this->m_ITL_Winner_RewardList[i].byRank;
			ITLRewardList.bGotReward = this->m_ITL_Winner_RewardList[i].bGotReward;
			ITLRewardList.byEnteredCnt = this->m_ITL_Winner_RewardList[i].byEnteredCnt;
			ITLRewardList.byPoint = this->m_ITL_Winner_RewardList[i].byPoint;
			memcpy(ITLRewardList.szCharName, this->m_ITL_Winner_RewardList[i].szCharName, MAX_ACCOUNT_LEN + 1);
			memcpy(ITLRewardList.szGuildName, this->m_ITL_Winner_RewardList[i].szGuildName, MAX_GUILD_LEN + 1);

			memcpy(&sendbuff[lOfs], &ITLRewardList, sizeof(ITLRewardList));
			lOfs += sizeof(ITLRewardList);
			nCount++;
		}

		pMsg.byCount = nCount;
		pMsg.byResult = 0;
	}

	pMsg.h.sizeH = HIBYTE(lOfs);
	pMsg.h.sizeL = LOBYTE(lOfs);
	memcpy(sendbuff, &pMsg, sizeof(pMsg));

	IOCP.DataSend(aIndex, (LPBYTE)sendbuff, lOfs);
}

void CIllusionTempleLeagueEvent::Renew_ITL_RewardList()
{
	_tagPMSG_REQ_ITL_RENEW_REWARD pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.size = sizeof(pMsg);
	pMsg.h.headcode = 0xF0;
	pMsg.h.subcode = 0xA8;
	pMsg.wMapSvrNum = g_MapServerManager.GetMapSvrGroup();

	wsDataCli.DataSend((char *)&pMsg, pMsg.h.size);
}

void CIllusionTempleLeagueEvent::Get_ITL_RewardList()
{
	_tagPMSG_REQ_ITL_GET_REWARDLIST pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.size = sizeof(pMsg);
	pMsg.h.headcode = 0xF0;
	pMsg.h.subcode = 0xA9;
	pMsg.wMapSvrNum = g_MapServerManager.GetMapSvrGroup();

	wsDataCli.DataSend((char *)&pMsg, pMsg.h.size);
}

void CIllusionTempleLeagueEvent::Update_ITL_RewardFlag(char *Name)
{
	_tagPMSG_REQ_ITL_REWARD_GOTRECEIVED pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.size = sizeof(pMsg);
	pMsg.h.headcode = 0xF0;
	pMsg.h.subcode = 0xAA;
	memcpy(pMsg.CharName, Name, MAX_ACCOUNT_LEN + 1);

	wsDataCli.DataSend((char *)&pMsg, sizeof(pMsg));
}

BYTE CIllusionTempleLeagueEvent::Check_ITLRewardList(int nUserIndex)
{
	LPOBJ lpObj = &gObj[nUserIndex];

	if (gObjIsConnected(nUserIndex) == TRUE && lpObj->Type == OBJ_USER)
	{
		for (int i = 0; i < 5; i++)
		{
			if (strcmp(this->m_ITL_Winner_RewardList[i].szGuildName, lpObj->m_PlayerData->GuildName))
			{
				return 2;
			}

			if (!strcmp(this->m_ITL_Winner_RewardList[i].szCharName, lpObj->Name))
			{
				if (this->m_ITL_Winner_RewardList[i].bGotReward)
				{
					return 1;
				}

				else
				{
					this->m_ITL_Winner_RewardList[i].bGotReward = true;
					return 0;
				}
			}
		}
	}

	return -1;
}

void CIllusionTempleLeagueEvent::MakeITLRewardItem_CCFWing(int iUserIndex)
{
	ItemSerialCreateSend(iUserIndex, gObj[iUserIndex].MapNumber, gObj[iUserIndex].X, gObj[iUserIndex].Y, ITEMGET(12, 266), 0, 0, 0, 1, 4, iUserIndex, 7, 0, 2419200, 0, 0);
	g_Log.Add("[ ITL ][MakeITLRewardItem_CCFWing] Index:%d, Name:%s", iUserIndex, gObj[iUserIndex].Name);
}

void CIllusionTempleLeagueEvent::IncreaseKillCount(int nIndex, BYTE byMapNumber, BYTE byObjectType)
{
	if (!ObjectMaxRange(nIndex))
	{
		return;
	}

	if (!ITL_MAP_RANGE(byMapNumber))
	{
		return;
	}

	if (byObjectType == OBJ_USER)
	{
		this->m_cIllusionTempleLeagueProc[byMapNumber - 98].IncUserKillCount(nIndex);
	}
}

void CIllusionTempleLeagueEvent::ReSetTimeVar()
{
	for (int i = 0; i < 2; i++)
	{
		this->m_cIllusionTempleLeagueProc[i].ResetTimeVariables();
	}
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


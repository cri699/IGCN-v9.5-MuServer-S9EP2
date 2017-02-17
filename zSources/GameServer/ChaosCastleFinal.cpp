//////////////////////////////////////////////////////////////////////
// ChaosCastleFinal.cpp
#include "stdafx.h"
#include "ChaosCastleFinal.h"
#include "configread.h"
#include "GameMain.h"
#include "TLog.h"
#include "TNotice.h"
#include "DSProtocol.h"
#include "user.h"
#include "BloodCastle.h"
#include "LargeRand.h"
#include "BuffEffectSlot.h"
#include "CrywolfSync.h"
#include "MasterLevelSkillTreeSystem.h"
#include "MapServerManager.h"
#include "GremoryCase.h"

ChaosCastleFinal g_ChaosCastleFinal;

BYTE ChaosCastleFinal::m_byCCFTimer;
bool ChaosCastleFinal::m_bCCFDayChange;

ChaosCastleFinal::ChaosCastleFinal()
{
	this->m_FirstRankingInfoLoad = false;
	this->m_bCCF_EVENT_ENABLE = false;
	this->m_bFinalDayEnd = 0;
	this->m_iCCF_TIME_MIN_ALL_NOTIFY = 15;
	this->m_iCCF_TIME_MIN_OPEN = 10;
	this->m_bIsCCF_Ranking_Renew = false;
	this->m_iCCF_BoxRatio1 = 30;
	this->m_iCCF_BoxRatio2 = 45;
	this->m_iCCF_BoxRatio3 = 60;
	this->m_iCCF_BoxRatio4 = 75;
	this->m_iCCF_BoxRatio5 = 90;
	this->m_iCCF_BoxRatio6 = 100;
	this->m_byBoxDropCnt = 0;
	this->m_CCF_MOB_CNT_SCRIPT = 0;
	this->m_stCCFData.m_byCCF_TYPE = 0;
	this->m_stCCFData.m_nCCF_STATE = 0;
	this->m_stCCFData.m_nMapNumber = MAP_INDEX_CHAOSCASTLE_SURVIVAL;
	this->m_stCCFData.m_nCCF_REMAIN_MSEC = -1;
	this->m_stCCFData.m_nCCF_TICK_COUNT = -1;
	this->m_stCCFData.m_fRewardExpRate = 1.0;
	this->m_byTryOut1 = 0;
	this->m_byTryOut2 = 0;
	this->m_byTryOut3 = 0;
	this->m_bySemiFinal1 = 0;
	this->m_bySemiFinal2 = 0;
	this->m_byFinal = 0;
	this->m_bCCF_CHEAT_BLOW = true;
	this->m_bCCF_CHEAT_TRAP = true;

	for (int i = 0; i < 50; i++)
	{
		this->m_CCFRanking[i].byRank = 0;
		this->m_CCFRanking[i].nPoint = 0;
		memset(this->m_CCFRanking[i].szCharName, 0, MAX_ACCOUNT_LEN + 1);
	}
	
	this->ClearCCFData();
	srand(time(NULL));

	this->m_nCCF_MinUserCount = 5;
	this->m_nCCF_Level = 280;
}

ChaosCastleFinal::~ChaosCastleFinal()
{
	
}

void ChaosCastleFinal::Run()
{
	if (this->m_bCCF_EVENT_ENABLE == false)
	{
		return;
	}

	switch (this->m_stCCFData.m_nCCF_STATE)
	{
		case 0:
			this->ProcState_None();
			break;
		case 1:
			this->ProcState_Closed();
			break;
		case 2:
			this->ProcState_Playing();
			break;
		case 3:
			this->ProcState_PlayEnd();
			break;
		default: 
			return;
	}
}

void ChaosCastleFinal::Init()
{
	if (g_ConfigRead.server.GetStateFromEventTable(g_ConfigRead.server.GetServerType(), EV_TABLE_CCF) == false)
	{
		this->m_bCCF_EVENT_ENABLE = false;
		return;
	}

	this->SetOpenTodaySchedule();
	this->SetState(1, 0);

	for (int iTRAP_STEP = 0; iTRAP_STEP < 4; iTRAP_STEP++)
	{
		g_rt_CCF_POINT_FRAME[0].left = (::g_iCCF_GroundAxis[2] + ::g_iCCF_GroundAxis[0]) / 2 + 1;
		g_rt_CCF_POINT_FRAME[0].top = g_iCCF_GroundAxis[1];
		g_rt_CCF_POINT_FRAME[0].right = ::g_iCCF_GroundAxis[2];
		g_rt_CCF_POINT_FRAME[0].bottom = (::g_iCCF_GroundAxis[3] + ::g_iCCF_GroundAxis[1]) / 2;

		g_rt_CCF_POINT_FRAME[1].left = (::g_iCCF_GroundAxis[2] + ::g_iCCF_GroundAxis[0]) / 2 + 1;
		g_rt_CCF_POINT_FRAME[1].top = (::g_iCCF_GroundAxis[3] + ::g_iCCF_GroundAxis[1]) / 2 + 1;
		g_rt_CCF_POINT_FRAME[1].right = g_iCCF_GroundAxis[2];
		g_rt_CCF_POINT_FRAME[1].bottom = ::g_iCCF_GroundAxis[3];

		g_rt_CCF_POINT_FRAME[2].left = ::g_iCCF_GroundAxis[0];
		g_rt_CCF_POINT_FRAME[2].top = (::g_iCCF_GroundAxis[3] + ::g_iCCF_GroundAxis[1]) / 2 + 1;
		g_rt_CCF_POINT_FRAME[2].right = (::g_iCCF_GroundAxis[2] + ::g_iCCF_GroundAxis[0]) / 2;
		g_rt_CCF_POINT_FRAME[2].bottom = ::g_iCCF_GroundAxis[3];

		g_rt_CCF_POINT_FRAME[3].left = ::g_iCCF_GroundAxis[0];
		g_rt_CCF_POINT_FRAME[3].top = ::g_iCCF_GroundAxis[1];
		g_rt_CCF_POINT_FRAME[3].right = (::g_iCCF_GroundAxis[2] + ::g_iCCF_GroundAxis[0]) / 2;
		g_rt_CCF_POINT_FRAME[3].bottom = (::g_iCCF_GroundAxis[3] + ::g_iCCF_GroundAxis[1]) / 2;

		g_rt_CCF_POINT_TRAP[0].left = (::g_iCCF_DamageAxis[iTRAP_STEP][2] + ::g_iCCF_DamageAxis[iTRAP_STEP][0]) / 2 + 1;
		g_rt_CCF_POINT_TRAP[0].top = ::g_iCCF_DamageAxis[iTRAP_STEP][1];
		g_rt_CCF_POINT_TRAP[0].right = ::g_iCCF_DamageAxis[iTRAP_STEP][2];
		g_rt_CCF_POINT_TRAP[0].bottom = (::g_iCCF_DamageAxis[iTRAP_STEP][3] + ::g_iCCF_DamageAxis[iTRAP_STEP][1]) / 2;

		g_rt_CCF_POINT_TRAP[1].left = (::g_iCCF_DamageAxis[iTRAP_STEP][2] + ::g_iCCF_DamageAxis[iTRAP_STEP][0]) / 2 + 1;
		g_rt_CCF_POINT_TRAP[1].top = (::g_iCCF_DamageAxis[iTRAP_STEP][3] + ::g_iCCF_DamageAxis[iTRAP_STEP][1]) / 2 + 1;
		g_rt_CCF_POINT_TRAP[1].right = g_iCCF_DamageAxis[iTRAP_STEP][2];
		g_rt_CCF_POINT_TRAP[1].bottom = ::g_iCCF_DamageAxis[iTRAP_STEP][3];

		g_rt_CCF_POINT_TRAP[2].left = ::g_iCCF_DamageAxis[iTRAP_STEP][0];
		g_rt_CCF_POINT_TRAP[2].top = (::g_iCCF_DamageAxis[iTRAP_STEP][3] + ::g_iCCF_DamageAxis[iTRAP_STEP][1]) / 2 + 1;
		g_rt_CCF_POINT_TRAP[2].right = (::g_iCCF_DamageAxis[iTRAP_STEP][2] + ::g_iCCF_DamageAxis[iTRAP_STEP][0]) / 2;
		g_rt_CCF_POINT_TRAP[2].bottom = ::g_iCCF_DamageAxis[iTRAP_STEP][3];

		g_rt_CCF_POINT_TRAP[3].left = ::g_iCCF_DamageAxis[iTRAP_STEP][0];
		g_rt_CCF_POINT_TRAP[3].top = ::g_iCCF_DamageAxis[iTRAP_STEP][1];
		g_rt_CCF_POINT_TRAP[3].right = (::g_iCCF_DamageAxis[iTRAP_STEP][2] + ::g_iCCF_DamageAxis[iTRAP_STEP][0]) / 2;
		g_rt_CCF_POINT_TRAP[3].bottom = (::g_iCCF_DamageAxis[iTRAP_STEP][3] + ::g_iCCF_DamageAxis[iTRAP_STEP][1]) / 2;
	}
}

void ChaosCastleFinal::Load_CCFMonster(char *Filename)
{
	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(Filename);

	if (res.status != pugi::status_ok)
	{
		g_Log.MsgBox("[ChaosCastleFinal] SetBase File Load Fail %s (%s)", Filename, res.description());
		return;
	}

	pugi::xml_node main_section = file.child("ChaosCastleSurvival");
	int nCount = 0;

	for (pugi::xml_node monster = main_section.child("Monster"); monster; monster = monster.next_sibling(), nCount++)
	{
		this->m_CCFMP[nCount].m_Type = monster.attribute("Index").as_int();
		this->m_CCFMP[nCount].m_MapNumber = monster.attribute("MapNumber").as_int();
		this->m_CCFMP[nCount].m_Dis = monster.attribute("Distance").as_int();
		this->m_CCFMP[nCount].m_X = monster.attribute("StartX").as_int();
		this->m_CCFMP[nCount].m_Y = monster.attribute("StartY").as_int();
		this->m_CCFMP[nCount].m_Dir = monster.attribute("Dir").as_int();
	}

	if (nCount == 100 || nCount < 0)
	{
		g_Log.Add("[ChaosCastleFinal][LoadMonster] Monster Count Invalid");
		nCount = 100;
	}

	this->m_CCF_MOB_CNT_SCRIPT = nCount;
}

void ChaosCastleFinal::Load_CCFInfo(char *Filename)
{
	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(Filename);

	if (res.status != pugi::status_ok)
	{
		g_Log.MsgBox("[ChaosCastleFinal] Info File Load Fail %s (%s)", Filename, res.description());
		return;
	}

	int nTimeTableCnt = 0;
	this->m_vtCCFTime.clear();

	pugi::xml_node main_section = file.child("ChaosCastleSurvival");
	pugi::xml_node time = main_section.child("Time");
	pugi::xml_node tournament = main_section.child("TournamentCycleDay");
	pugi::xml_node rewardrate = main_section.child("RewardRate");
	pugi::xml_node eventschedule = main_section.child("EventSchedule");

	this->m_bCCF_EVENT_ENABLE = g_ConfigRead.server.GetStateFromEventTable(g_ConfigRead.server.GetServerType(), EV_TABLE_CCF) == true ? main_section.attribute("Enable").as_int() : false;
	this->m_stCCFData.m_fRewardExpRate = main_section.attribute("ExpMultipler").as_float();
	this->m_nCCF_MinUserCount = main_section.attribute("MinPlayer").as_int();
	this->m_nCCF_Level = main_section.attribute("MinLevel").as_int();

	this->m_iCCF_TIME_MIN_OPEN = time.attribute("StartIn").as_int();
	this->m_iCCF_TIME_MIN_PLAY = time.attribute("Duration").as_int();
	this->m_iCCF_TIME_MIN_REST = time.attribute("TownSpawn").as_int();
	this->m_iCCF_TIME_MIN_ALL_NOTIFY = time.attribute("Notify").as_int();

	this->m_byTryOut1 = tournament.attribute("EliminationOne").as_int();
	this->m_byTryOut2 = tournament.attribute("EliminationTwo").as_int();
	this->m_byTryOut3 = tournament.attribute("EliminationThree").as_int();
	this->m_bySemiFinal1 = tournament.attribute("SemiFinalOne").as_int();
	this->m_bySemiFinal2 = tournament.attribute("SemiFinalTwo").as_int();
	this->m_byFinal = tournament.attribute("Final").as_int();

	pugi::xml_node gift = rewardrate.child("Gift");
	pugi::xml_node finalaward = rewardrate.child("FinalAward");

	this->m_iCCF_BoxRatio1 = gift.attribute("First").as_int();
	this->m_iCCF_BoxRatio2 = gift.attribute("Second").as_int();
	this->m_iCCF_BoxRatio3 = gift.attribute("Third").as_int();
	this->m_iCCF_BoxRatio4 = gift.attribute("Fourth").as_int();
	this->m_iCCF_BoxRatio5 = gift.attribute("Fifth").as_int();
	this->m_iCCF_BoxRatio6 = gift.attribute("Sixth").as_int();

	this->m_stCCFFinalReward.m_btCat = finalaward.attribute("Cat").as_int(13);
	this->m_stCCFFinalReward.m_wIndex = finalaward.attribute("Index").as_int(276);
	this->m_stCCFFinalReward.m_btLevel = finalaward.attribute("Level").as_int(0);
	this->m_stCCFFinalReward.m_btDurability = finalaward.attribute("Durability").as_int(0);
	this->m_stCCFFinalReward.m_btSkill = finalaward.attribute("Skill").as_int(0);
	this->m_stCCFFinalReward.m_btLuck = finalaward.attribute("Luck").as_int(0);
	this->m_stCCFFinalReward.m_btOption = finalaward.attribute("Option").as_int(0);
	this->m_stCCFFinalReward.m_btNewOption = finalaward.attribute("Exc").as_int(0);
	this->m_stCCFFinalReward.m_dwExpiryTime = finalaward.attribute("Duration").as_int(0);

	CCF_START_TIME pRET;

	for (pugi::xml_node start = eventschedule.child("Start"); start; start = start.next_sibling())
	{
		pRET.m_nHour = start.attribute("Hour").as_int();
		pRET.m_nMinute = start.attribute("Minute").as_int();
		pRET.m_nTryout = start.attribute("Stage1").as_int();
		pRET.m_nSemiFinal = start.attribute("Stage2").as_int();
		pRET.m_nFinal = start.attribute("Stage3").as_int();

		nTimeTableCnt++;

		if (nTimeTableCnt > 40)
		{
			g_Log.MsgBox("ChaosCastleFinal TimeTable MaxOver [%s] ", Filename);
			break;
		}

		this->m_vtCCFTime.push_back(pRET);
	}
}

void ChaosCastleFinal::ClearCCFData()
{
	this->m_stCCFData.Clear();

	for (int i = 0; i < 70; i++)
	{
		if (this->m_stCCFData.m_UserData[i].m_nIndex != -1)
		{
			gObj[this->m_stCCFData.m_UserData[i].m_nIndex].m_bCCF_UI_Using = false;
			gObj[this->m_stCCFData.m_UserData[i].m_nIndex].m_byCCF_CanEnter = 0;
			gObj[this->m_stCCFData.m_UserData[i].m_nIndex].m_nCCF_CertiTick = 0;
			gObj[this->m_stCCFData.m_UserData[i].m_nIndex].m_nCCF_UserIndex = -1;
			gObj[this->m_stCCFData.m_UserData[i].m_nIndex].m_nCCF_BlowTime = 0;
			gObj[this->m_stCCFData.m_UserData[i].m_nIndex].m_byKillUserCount = 0;
			gObj[this->m_stCCFData.m_UserData[i].m_nIndex].m_byKillMobCount = 0;
			gObj[this->m_stCCFData.m_UserData[i].m_nIndex].m_bCCF_Quit_Msg = false;
		}

		this->m_stCCFData.m_UserData[i].Clear();
	}
}

void ChaosCastleFinal::ProcState_None()
{
	return;
}

bool ChaosCastleFinal::CheckOpenOrNotOpenCCF_Final(BYTE bCCFType)
{
	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);

	CCF_START_TIME pRET;

	int iMIN_HOUR = 24;
	int iMIN_MINUTE = 60;
	int Old_Hour = 0;
	int Old_Min = 0;

	for (std::vector<CCF_START_TIME>::iterator it = this->m_vtCCFOpenTime.begin(); it != this->m_vtCCFOpenTime.end(); it++)
	{
		pRET = *it;

		if (pRET.m_nFinal == 3
			&& sysTime.wMinute + 60 * sysTime.wHour > Old_Min + 60 * Old_Hour
			&& sysTime.wMinute + 60 * sysTime.wHour < pRET.m_nMinute + 60 * pRET.m_nHour)
		{
			return FALSE;
		}

		Old_Hour = pRET.m_nHour;
		Old_Min = pRET.m_nMinute;
	}

	return true;
}

void ChaosCastleFinal::ProcState_Closed()
{
	DWORD iTICK_MSEC = GetTickCount() - this->m_stCCFData.m_nCCF_TICK_COUNT;

	if (iTICK_MSEC >= 1000)
	{
		this->m_stCCFData.m_nCCF_REMAIN_MSEC -= iTICK_MSEC;
		this->m_stCCFData.m_nCCF_TICK_COUNT = GetTickCount();

		if (this->m_bCCF_EVENT_ENABLE)
		{
			if (g_ConfigRead.server.GetServerType() == SERVER_BATTLECORE && this->Check_CCF_DayType())
			{
				BYTE BattleType = this->Check_CCF_DayType();
				bool IsNowFinal = false;

				if (BattleType == 3)
				{
					IsNowFinal = this->CheckOpenOrNotOpenCCF_Final(BattleType);
				}

				if (!IsNowFinal)
				{
					if (this->m_stCCFData.m_nCCF_REMAIN_MSEC <= 60000 * this->m_iCCF_TIME_MIN_ALL_NOTIFY && this->m_stCCFData.m_bCCF_CAN_ENTER == false && this->m_stCCFData.m_bCCF_MSG_ALL_NOTIFY == false)
					{
						g_Log.Add("[Chaos Castle Survival] Notify = 1");

						this->m_stCCFData.m_bCCF_MSG_ALL_NOTIFY = true;
						this->GD_SendCCFInfoAllServer(this->m_iCCF_TIME_MIN_ALL_NOTIFY, this->Check_CCF_DayType());
						this->m_stCCFData.m_byCCF_TYPE = this->Check_CCF_DayType();
					}

					if (this->m_stCCFData.m_nCCF_REMAIN_MSEC <= 60000 * this->m_iCCF_TIME_MIN_OPEN && !this->m_stCCFData.m_bCCF_CAN_ENTER)
					{
						this->m_stCCFData.m_bCCF_CAN_ENTER = 1;
						this->m_stCCFData.m_bCCF_CAN_PARTY = 1;
						this->m_stCCFData.m_byCCF_TYPE = this->Check_CCF_DayType();

						g_Log.Add("[Chaos Castle Survival] Opened.");
					}

					if (this->m_stCCFData.m_nCCF_REMAIN_MSEC <= 60000 * this->m_iCCF_TIME_MIN_OPEN && this->m_stCCFData.m_nCCF_REMAIN_MSEC > 0 && this->m_stCCFData.m_nCCF_REMAIN_MSEC / 60000 != this->m_stCCFData.m_nCCF_NOTIFY_COUNT)
					{
						this->m_stCCFData.m_nCCF_NOTIFY_COUNT = this->m_stCCFData.m_nCCF_REMAIN_MSEC / 60000;

						PMSG_NOTICE pNotice;
						TNotice::MakeNoticeMsgEx(&pNotice, 0, Lang.GetText(0,590), this->m_stCCFData.m_nCCF_NOTIFY_COUNT + 1);

						this->SendAllUserAnyMsg((LPBYTE)&pNotice, pNotice.h.size);
						g_Log.Add("[Chaos Castle Survival] Event starts in %d minutes", this->m_stCCFData.m_nCCF_NOTIFY_COUNT + 1);
					}

					if (this->m_stCCFData.m_nCCF_REMAIN_MSEC <= 30000 && this->m_stCCFData.m_nCCF_REMAIN_MSEC > 0 && !this->m_stCCFData.m_bCCF_MSG_BEFORE_ENTER)
					{
						this->m_stCCFData.m_bCCF_MSG_BEFORE_ENTER = TRUE;

						PMSG_SET_DEVILSQUARE pMsg;

						PHeadSetB((LPBYTE)&pMsg, 0x92, sizeof(pMsg));
						pMsg.Type = 20;

						for (int n = g_ConfigRead.server.GetObjectStartUserIndex(); n < g_ConfigRead.server.GetObjectMax(); n++)
						{
							if (gObj[n].Connected == PLAYER_PLAYING && gObj[n].Type == OBJ_USER && gObj[n].MapNumber != MAP_INDEX_CHAOSCASTLE_SURVIVAL)
							{
								IOCP.DataSend(n, (LPBYTE)&pMsg, pMsg.h.size);
							}
						}
					}
				}
			}
		}
	}

	if (this->m_stCCFData.m_nCCF_REMAIN_MSEC <= 0)
	{
		if (this->m_bCCF_EVENT_ENABLE)
		{
			this->SetState(2, 0);
		}

		else
		{
			this->SetState(1, 0);
		}
	}
}

void ChaosCastleFinal::SetState(int iCCF_STATE, int nFlag)
{
	char * temp1;
	char * temp2;

	if (iCCF_STATE >= 0 && iCCF_STATE <= 3)
	{
		switch (iCCF_STATE)
		{
			case 0:
				temp1 = "None";
				break;
			case 1:
				temp1 = "Closed";
				break;
			case 2:
				temp1 = "Playing";
				break;
			case 3:
				temp1 = "PlayEnd";
				break;
		}

		switch (this->m_stCCFData.m_nCCF_STATE)
		{
			case 0:
				temp2 = "None";
				break;
			case 1:
				temp2 = "Closed";
				break;
			case 2:
				temp2 = "Playing";
				break;
			case 3:
				temp2 = "PlayEnd";
				break;
		}

		g_Log.Add("[Chaos Castle Survival] Change State: %s -> %s", temp2, temp1);
		this->m_stCCFData.m_nCCF_STATE = iCCF_STATE;

		switch (this->m_stCCFData.m_nCCF_STATE)
		{
			case 0:
				this->SetState_None();
				break;
			case 1:
				this->SetState_Closed();
				break;
			case 2:
				this->SetState_Playing();
				break;
			case 3:
				this->SetState_PlayEnd(nFlag);
				break;
			default:
				return;
		}
	}
}

void ChaosCastleFinal::ProcState_Playing()
{
	DWORD iTICK_MSEC = GetTickCount() - this->m_stCCFData.m_nCCF_TICK_COUNT;

	if (iTICK_MSEC >= 1000)
	{
		this->m_stCCFData.m_nCCF_REMAIN_MSEC -= iTICK_MSEC;
		this->m_stCCFData.m_nCCF_TICK_COUNT = GetTickCount();

		if (this->m_stCCFData.m_nCCF_REMAIN_MSEC <= ((this->m_iCCF_TIME_MIN_PLAY * 60 - 30) * 1000) &&
			this->m_stCCFData.m_bCCF_MSG_BEFORE_PLAY == false)
		{
			this->m_stCCFData.m_bCCF_MSG_BEFORE_PLAY = true;

			PMSG_SET_DEVILSQUARE pMsg;
			PHeadSetB((LPBYTE)&pMsg, 0x92, sizeof(pMsg));
			pMsg.Type = 21;
			this->SendChaosCastleAnyMsg((BYTE*)&pMsg, sizeof(pMsg));
		}

		if (this->m_stCCFData.m_nCCF_REMAIN_MSEC <= 30000 &&
			this->m_stCCFData.m_nCCF_REMAIN_MSEC > 0 &&
			this->m_stCCFData.m_bCCF_MSG_BEFORE_END == false)
		{
			this->m_stCCFData.m_bCCF_MSG_BEFORE_END = true;
			PMSG_SET_DEVILSQUARE pMsg;
			PHeadSetB((LPBYTE)&pMsg, 0x92, sizeof(pMsg));
			pMsg.Type = 22;
			this->SendChaosCastleAnyMsg((BYTE*)&pMsg, sizeof(pMsg));
		}

		if (this->m_stCCFData.m_nCCF_REMAIN_MSEC <= ((this->m_iCCF_TIME_MIN_PLAY * 60 - 60) * 1000) &&
			this->m_stCCFData.m_bCCF_PLAY_START == false)
		{
			PMSG_NOTICE pNotice;

			this->m_stCCFData.m_nCCF_REMAIN_MSEC = (this->m_iCCF_TIME_MIN_PLAY * 60) * 1000;
			this->m_stCCFData.m_bCCF_CAN_PARTY = false;
			TNotice::MakeNoticeMsgEx((TNotice*)&pNotice, 0, Lang.GetText(0,591));
			this->SendChaosCastleAnyMsg((LPBYTE)&pNotice, pNotice.h.size);
			this->UnSafetyCastleZone();
			this->SendCastleZoneSafetyInfo(0);
			this->m_stCCFData.m_bCCF_PLAY_START = true;
			this->m_stCCFData.m_nCCF_FIRST_USER_COUNT = this->ReCalcUserCount();
			this->m_stCCFData.m_lCCF_CURRENT_MONSTER_COUNT = this->SetMonster();
			this->m_stCCFData.m_nCCF_FIRST_MONSTER_COUNT = this->m_stCCFData.m_lCCF_CURRENT_MONSTER_COUNT;
			this->SetItemsToMonster(this->Check_CCF_DayType());
			this->SendNoticeState(5);
			this->m_bIsCCF_Ranking_Renew = false;

			g_Log.Add("[Chaos Castle Survival] Started. (UserCount:%d)", this->GetCurPlayUser());
		}

		if (this->m_stCCFData.m_bCCF_PLAY_START == false &&
			this->m_stCCFData.m_bCCF_CAN_ENTER == false)
		{
			int iCurPlayUser = this->GetCurPlayUser();

			if (iCurPlayUser == 0)
			{
				this->SetState(1, 0);
				g_Log.Add("[Chaos Castle Survival] Closed - No User");
				return;
			}
		}

		if (this->m_stCCFData.m_bCCF_PLAY_START == true)
		{
			this->ProcessFallUser();
			this->CheckUserInDieTile();
			this->CheckMonsterInDieTile();
			this->ProcessTrapStatus();

			if (this->m_stCCFData.m_nCCF_TRAP_STATUS != this->m_stCCFData.m_nCCF_TRAP_OLD_STATUS)
			{
				this->m_stCCFData.m_nCCF_TRAP_OLD_STATUS = this->m_stCCFData.m_nCCF_TRAP_STATUS;

				this->SetMapAttrHollow(this->m_stCCFData.m_nCCF_TRAP_STATUS);
				this->PullObjInnerPlace(this->m_stCCFData.m_nCCF_TRAP_STATUS);
			}

			int iCUR_PLAY_STATE = this->CheckPlayEnded();

			if (iCUR_PLAY_STATE == 1)
			{
				int iCCF_WINNER_INDEX = -1;
				int iCCF_LEFT_MONSTER_COUNT = this->GetMonsterListCount();

				if (iCCF_LEFT_MONSTER_COUNT > 0)
				{
					g_Log.Add("[Chaos Castle Survival][CCS1] Has No Winner : Monster Left (%d)", this->m_stCCFData.m_lCCF_CURRENT_MONSTER_COUNT);
				}

				else
				{
					int nPoint = 0;
					int iCCF_WINNER_INDEX = this->GetCurrentCCFWinUser(&nPoint);

					if (iCCF_WINNER_INDEX == -1)
					{
						g_Log.Add("[Chaos Castle Survival][CCS1] Has No Winner.");
					}

					else
					{
						g_Log.Add("[Chaos Castle Survival][CCS1] [%s][%s][%s][ServerCode: %d] is Winner. [CharInfoSave : Class=%d, Level=%d, LvPoint=%d, Exp=%d, Str=%d, Dex=%d, Vit=%d, Energy=%d, LeaderShip=%d, Map=%d, Pk=%d]",
							gObj[iCCF_WINNER_INDEX].AccountID, gObj[iCCF_WINNER_INDEX].Name, gObj[iCCF_WINNER_INDEX].m_PlayerData->m_RealNameOfUBF,
							gObj[iCCF_WINNER_INDEX].m_PlayerData->m_nServerCodeOfHomeWorld, gObj[iCCF_WINNER_INDEX].m_PlayerData->DbClass,
							gObj[iCCF_WINNER_INDEX].Level, gObj[iCCF_WINNER_INDEX].m_PlayerData->LevelUpPoint, gObj[iCCF_WINNER_INDEX].m_PlayerData->Experience,
							gObj[iCCF_WINNER_INDEX].m_PlayerData->Strength, gObj[iCCF_WINNER_INDEX].m_PlayerData->Dexterity, gObj[iCCF_WINNER_INDEX].m_PlayerData->Vitality,
							gObj[iCCF_WINNER_INDEX].m_PlayerData->Energy, gObj[iCCF_WINNER_INDEX].Leadership, gObj[iCCF_WINNER_INDEX].MapNumber, gObj[iCCF_WINNER_INDEX].m_PK_Level);

						this->SendCCFWinMessage(iCCF_WINNER_INDEX, nPoint, 1);
						this->GiveCCFWinnerItem(iCCF_WINNER_INDEX, this->m_stCCFData.m_byCCF_TYPE, 1);
					}
				}

				this->SendAllLoserFailMessage(iCCF_WINNER_INDEX);
				this->SetState(3, 1);
				return;
			}

			if (iCUR_PLAY_STATE == 2)
			{
				int iCCF_WINNER_INDEX = -1;
				int iCCF_LEFT_MONSTER_COUNT = this->GetMonsterListCount();

				if (iCCF_LEFT_MONSTER_COUNT > 0)
				{
					g_Log.Add("[Chaos Castle Survival][CCS2] Has No Winner : Monster Left (%d)", this->m_stCCFData.m_lCCF_CURRENT_MONSTER_COUNT);
				}

				else
				{
					int nPoint = 0;
					int iCCF_WINNER_INDEX = this->GetCurrentCCFWinUser(&nPoint);

					if (iCCF_WINNER_INDEX == -1)
					{
						g_Log.Add("[Chaos Castle Survival][CCS2] Has No Winner.");
					}

					else
					{
						g_Log.Add("[Chaos Castle Survival][CCS2] [%s][%s][%s][ServerCode: %d] is Winner. [CharInfoSave : Class=%d, Level=%d, LvPoint=%d, Exp=%d, Str=%d, Dex=%d, Vit=%d, Energy=%d, LeaderShip=%d, Map=%d, Pk=%d]",
							gObj[iCCF_WINNER_INDEX].AccountID, gObj[iCCF_WINNER_INDEX].Name, gObj[iCCF_WINNER_INDEX].m_PlayerData->m_RealNameOfUBF,
							gObj[iCCF_WINNER_INDEX].m_PlayerData->m_nServerCodeOfHomeWorld, gObj[iCCF_WINNER_INDEX].m_PlayerData->DbClass,
							gObj[iCCF_WINNER_INDEX].Level, gObj[iCCF_WINNER_INDEX].m_PlayerData->LevelUpPoint, gObj[iCCF_WINNER_INDEX].m_PlayerData->Experience,
							gObj[iCCF_WINNER_INDEX].m_PlayerData->Strength, gObj[iCCF_WINNER_INDEX].m_PlayerData->Dexterity, gObj[iCCF_WINNER_INDEX].m_PlayerData->Vitality,
							gObj[iCCF_WINNER_INDEX].m_PlayerData->Energy, gObj[iCCF_WINNER_INDEX].Leadership, gObj[iCCF_WINNER_INDEX].MapNumber, gObj[iCCF_WINNER_INDEX].m_PK_Level);

						this->SendCCFWinMessage(iCCF_WINNER_INDEX, nPoint, 0);
						this->GiveCCFWinnerItem(iCCF_WINNER_INDEX, this->m_stCCFData.m_byCCF_TYPE, 1);
					}
				}

				this->SendAllLoserFailMessage(iCCF_WINNER_INDEX);
				this->SetState(3, 1);
				return;
			}

			if (iCUR_PLAY_STATE == 0)
				this->SendNoticeState(6);
		}
	}

	if (this->m_stCCFData.m_nCCF_REMAIN_MSEC <= 0)
	{
		g_Log.Add("[Chaos Castle Survival] is Over : TIME-OUT (Left User:%d, Monster:%d)",
			this->GetMonsterListCount(), this->GetCurPlayUser());

		int iCCF_WINNER_INDEX = -1;

		if (this->GetMonsterListCount() > 0)
		{
			if (this->GetCurEnteredUser() == 1)
			{
				for (int iMON = 0; iMON < g_ConfigRead.server.GetObjectMaxMonster(); iMON++)
				{
					if (this->m_stCCFData.m_lCCF_MONSTER_COUNT[iMON] != -1)
					{
						int iMonsterIndex = this->m_stCCFData.m_lCCF_MONSTER_COUNT[iMON];

						if (ObjectMaxRange(iMonsterIndex))
						{
							if (gObj[iMonsterIndex].Life > 0.0 &&
								gObj[iMonsterIndex].MapNumber == MAP_INDEX_CHAOSCASTLE_SURVIVAL &&
								gObj[iMonsterIndex].Connected > 0)
							{
								g_Log.Add("[Chaos Castle Survival][Bug Tracker] Left Monster AttrInfo %d/%d",
									gObj[iMonsterIndex].X, gObj[iMonsterIndex].Y);
							}
						}

						else
						{
							this->m_stCCFData.m_lCCF_MONSTER_COUNT[iMON] = -1;
						}
					}
				}
			}

			else
			{
				g_Log.Add("[Chaos Castle Survival][CCS4]  Has No Winner : Monster Left (%d)",
					this->m_stCCFData.m_lCCF_MONSTER_COUNT);
			}
		}

		else
		{
			int nPoint = 0;
			iCCF_WINNER_INDEX = this->GetCurrentCCFWinUser(&nPoint);

			if (iCCF_WINNER_INDEX == -1)
			{
				g_Log.Add("[Chaos Castle Survival][CCS3] Has No Winner.");
			}

			else
			{
				g_Log.Add("[Chaos Castle Survival][CCS3] [%s][%s][%s][ServerCode: %d] is Winner. [CharInfoSave : Class=%d, Level=%d, LvPoint=%d, Exp=%d, Str=%d, Dex=%d, Vit=%d, Energy=%d, LeaderShip=%d, Map=%d, Pk=%d]",
					gObj[iCCF_WINNER_INDEX].AccountID, gObj[iCCF_WINNER_INDEX].Name, gObj[iCCF_WINNER_INDEX].m_PlayerData->m_RealNameOfUBF,
					gObj[iCCF_WINNER_INDEX].m_PlayerData->m_nServerCodeOfHomeWorld, gObj[iCCF_WINNER_INDEX].m_PlayerData->DbClass,
					gObj[iCCF_WINNER_INDEX].Level, gObj[iCCF_WINNER_INDEX].m_PlayerData->LevelUpPoint, gObj[iCCF_WINNER_INDEX].m_PlayerData->Experience,
					gObj[iCCF_WINNER_INDEX].m_PlayerData->Strength, gObj[iCCF_WINNER_INDEX].m_PlayerData->Dexterity, gObj[iCCF_WINNER_INDEX].m_PlayerData->Vitality,
					gObj[iCCF_WINNER_INDEX].m_PlayerData->Energy, gObj[iCCF_WINNER_INDEX].Leadership, gObj[iCCF_WINNER_INDEX].MapNumber, gObj[iCCF_WINNER_INDEX].m_PK_Level);

				this->SendCCFWinMessage(iCCF_WINNER_INDEX, nPoint, 1);
				this->GiveCCFWinnerItem(iCCF_WINNER_INDEX, this->m_stCCFData.m_byCCF_TYPE, 1);
			}
		}

		this->SendAllLoserFailMessage(iCCF_WINNER_INDEX);
		this->SetState(3, 0);
	}
}

void ChaosCastleFinal::ProcState_PlayEnd()
{
	DWORD iTICK_MSEC = GetTickCount() - this->m_stCCFData.m_nCCF_TICK_COUNT;

	if (iTICK_MSEC >= 1000)
	{
		this->m_stCCFData.m_nCCF_REMAIN_MSEC -= iTICK_MSEC;
		this->m_stCCFData.m_nCCF_TICK_COUNT = GetTickCount();

		this->CheckUserInDieTile();

		if (this->m_stCCFData.m_nCCF_REMAIN_MSEC <= 30000)
		{
			if (this->m_stCCFData.m_nCCF_REMAIN_MSEC > 0 && !this->m_stCCFData.m_bCCF_MSG_BEFORE_QUIT)
			{
				this->m_stCCFData.m_bCCF_MSG_BEFORE_QUIT = TRUE;

				PMSG_SET_DEVILSQUARE pMsg;

				PHeadSetB((LPBYTE)&pMsg, 0x92, sizeof(pMsg));
				pMsg.Type = 23;

				this->SendChaosCastleAnyMsg((LPBYTE)&pMsg, sizeof(pMsg));
			}
		}
	}

	if (this->m_stCCFData.m_nCCF_REMAIN_MSEC <= 0)
	{
		this->SetState(1, 0);
	}
}

void ChaosCastleFinal::SendAllUserAnyMsg(LPBYTE lpMsg, int iSize)
{
	for (int i = g_ConfigRead.server.GetObjectStartUserIndex(); i < g_ConfigRead.server.GetObjectMax(); i++)
	{
		if (gObj[i].Connected == PLAYER_PLAYING)
		{
			if (gObj[i].Type == OBJ_USER)
			{
				if (DG_MAP_RANGE(gObj[i].MapNumber) == FALSE)
				{
					IOCP.DataSend(i, lpMsg, iSize);
				}
			}
		}
	}
}

void ChaosCastleFinal::SetState_PlayEnd(int nFlag)
{
	this->SendNoticeState(7);

	if (nFlag == FALSE)
	{
		this->ClearMonster();
	}

	this->m_stCCFData.m_bCCF_CAN_ENTER = false;
	this->m_stCCFData.m_bCCF_MSG_ALL_NOTIFY = false;

	this->m_stCCFData.m_byCCF_TYPE = this->Check_CCF_DayType();
	g_Log.Add("[Chaos Castle Survival] [ItemDropCount Reset] m_byBoxDropCnt: %d ",
		this->m_byBoxDropCnt);

	if (!this->m_bIsCCF_Ranking_Renew)
	{
		g_Log.Add("[Chaos Castle Survival] Ranking list START");

		for (int i = 0; i < 50; i++)
		{
			if (this->m_CCFRanking[i].byRank > 0)
				g_Log.Add("[Chaos Castle Survival] No.%d Name: %s Point: %d", this->m_CCFRanking[i].byRank, this->m_CCFRanking[i].szCharName, this->m_CCFRanking[i].nPoint);
		}

		g_Log.Add("[Chaos Castle Survival] Ranking list END");

		this->GD_Renew_CCF_Ranking(this->m_stCCFData.m_byCCF_TYPE);
		this->m_bIsCCF_Ranking_Renew = true;
		g_Log.Add("[Chaos Castle Survival][SetState_PlayEnd][REQ DataServer Renew Ranking] tick :%d", GetTickCount());
	}

	if (this->Check_CCF_DayType() == 3)
	{
		g_Log.Add("[Chaos Castle Survival][FINALDAY WAR END]");

		if (this->IsRealFinalDayEnd() == 1)
		{
			this->m_bFinalDayEnd = true;
		}
	}

	this->m_stCCFData.m_nCCF_REMAIN_MSEC = 60000 * this->m_iCCF_TIME_MIN_REST;

	g_Log.Add("[Chaos Castle Survival] SetState PLAYEND");
}

void ChaosCastleFinal::SetState_Playing()
{
	this->m_stCCFData.m_bCCF_CAN_ENTER = false;
	this->m_stCCFData.m_nCCF_REMAIN_MSEC = 60000 * this->m_iCCF_TIME_MIN_PLAY;
	this->SendCastleZoneSafetyInfo(1);

	if (this->CheckCanStart_CCF())
	{
		g_Log.Add("[Chaos Castle Survival] SetState PLAYING");
	}
	else
	{
		this->CCF_Start_Fail_So_RollBack();
		this->SetState_Closed();
		g_Log.Add("[Chaos Castle Survival] Failed to Start ChaosCastleFinal : Lack of User");
	}
}

void ChaosCastleFinal::SetState_Closed()
{
	this->SendNoticeState(7);
	this->ClearCCFData();
	this->ClearMonster();

	for (int n = g_ConfigRead.server.GetObjectStartUserIndex(); n < g_ConfigRead.server.GetObjectMax(); n++)
	{
		if (gObj[n].MapNumber == MAP_INDEX_CHAOSCASTLE_SURVIVAL)
		{
			gObjMoveGate(n, 333);
		}
	}

	this->SafetyCastleZone();
	this->SetMapAttrFill();
	this->CheckSync();
	this->GD_Load_CCF_RankingList(this->CCF_Ranking_DayType());

	g_Log.Add("[Chaos Castle Survival][SetState_Closed][GD_Load_CCF_RankingList] tick %d ", GetTickCount());
	g_Log.Add("[Chaos Castle Survival] SetState CLOSED");
}

void ChaosCastleFinal::SetState_None()
{
	return;
}

void ChaosCastleFinal::SendAllLoserFailMessage(int iWinnerIndex)
{
	if (iWinnerIndex != -1 && !ObjectMaxRange(iWinnerIndex))
	{
		g_Log.Add("[Chaos Castle Survival][SendAllLoserFailMessage] return ERROR iWinnerIndex : %d", iWinnerIndex);
		return;
	}

	for (int i = 0; i < 70; i++)
	{
		if (this->m_stCCFData.m_UserData[i].m_nIndex != -1
			&& this->m_stCCFData.m_UserData[i].m_nIndex != iWinnerIndex
			&& !gObj[this->m_stCCFData.m_UserData[i].m_nIndex].m_bCCF_Quit_Msg
			&& gObjIsConnected(this->m_stCCFData.m_UserData[i].m_nIndex))
		{
			if (gObj[this->m_stCCFData.m_UserData[i].m_nIndex].m_nCCF_UserIndex != -1)
			{
				PMSG_ANS_CCF_RESULT pMsg;
				int nPoint = gObj[this->m_stCCFData.m_UserData[i].m_nIndex].m_byKillMobCount + 2 * gObj[this->m_stCCFData.m_UserData[i].m_nIndex].m_byKillUserCount;

				this->GD_Req_Save_CCF_Result(this->m_stCCFData.m_UserData[i].m_nIndex, gObj[this->m_stCCFData.m_UserData[i].m_nIndex].Name,
					nPoint, this->Check_CCF_DayType());

				GSProtocol.GCServerMsgStringSend(Lang.GetText(0,557), this->m_stCCFData.m_UserData[i].m_nIndex, 1);

				PHeadSubSetB((LPBYTE)&pMsg, 0xAF, 0x09, sizeof(pMsg));
				pMsg.point = nPoint;
				pMsg.result = 2;
				pMsg.type = this->Check_CCF_DayType();
				pMsg.MobKillCnt = gObj[this->m_stCCFData.m_UserData[i].m_nIndex].m_byKillMobCount;
				pMsg.PCKillCnt = gObj[this->m_stCCFData.m_UserData[i].m_nIndex].m_byKillUserCount;
				IOCP.DataSend(this->m_stCCFData.m_UserData[i].m_nIndex, (LPBYTE)&pMsg, pMsg.h.size);

				gObj[this->m_stCCFData.m_UserData[i].m_nIndex].m_bCCF_Quit_Msg = true;
				g_Log.Add("[Chaos Castle Survival][GAME-RESULT][FAIL-CASE] NAME:[%s] UBFNAME:[%s] SERVERCODE:[%d],MOBKILL:[%d],USERKILL:[%d],POINT:[%d]",
					gObj[this->m_stCCFData.m_UserData[i].m_nIndex].Name,
					gObj[this->m_stCCFData.m_UserData[i].m_nIndex].m_PlayerData->m_RealNameOfUBF,
					gObj[this->m_stCCFData.m_UserData[i].m_nIndex].m_PlayerData->m_nServerCodeOfHomeWorld,
					gObj[this->m_stCCFData.m_UserData[i].m_nIndex].m_byKillMobCount,
					gObj[this->m_stCCFData.m_UserData[i].m_nIndex].m_byKillUserCount,
					nPoint);
			}
		}
	}
}

void ChaosCastleFinal::SendCCFWinMessage(int iWinnerIndex, int nPoint, bool bFinalWinner)
{
	if (ObjectMaxRange(iWinnerIndex) == false)
	{
		return;
	}

	if (gObjIsConnected(iWinnerIndex) == false)
	{
		return;
	}

	if (gObj[iWinnerIndex].m_bCCF_Quit_Msg == false)
	{
		if (bFinalWinner != 4 || this->Check_CCF_DayType() != 3)
		{
			this->GD_Req_Save_CCF_Result(iWinnerIndex, gObj[iWinnerIndex].Name, nPoint, this->Check_CCF_DayType());
		}

		else
		{
			this->GD_Req_Save_CCF_Result(iWinnerIndex, gObj[iWinnerIndex].Name, nPoint, 4);
		}

		PMSG_ANS_CCF_RESULT pMsg;
		int nPoint = gObj[iWinnerIndex].m_byKillMobCount + 2 * gObj[iWinnerIndex].m_byKillUserCount;

		GSProtocol.GCServerMsgStringSend(Lang.GetText(0,558), iWinnerIndex, 1);

		PHeadSubSetB((LPBYTE)&pMsg, 0xAF, 0x09, sizeof(pMsg));
		pMsg.point = nPoint;
		pMsg.result = 1;
		pMsg.type = this->Check_CCF_DayType();
		pMsg.MobKillCnt = gObj[iWinnerIndex].m_byKillMobCount;
		pMsg.PCKillCnt = gObj[iWinnerIndex].m_byKillUserCount;
		IOCP.DataSend(iWinnerIndex, (LPBYTE)&pMsg, pMsg.h.size);

		gObj[iWinnerIndex].m_bCCF_Quit_Msg = true;
		g_Log.Add("[Chaos Castle Survival][GAME-RESULT][WIN-CASE] NAME:[%s] UBFNAME:[%s] SERVERCODE:[%d],MOBKILL:[%d],USERKILL:[%d],POINT:[%d]",
			gObj[iWinnerIndex].Name,
			gObj[iWinnerIndex].m_PlayerData->m_RealNameOfUBF,
			gObj[iWinnerIndex].m_PlayerData->m_nServerCodeOfHomeWorld,
			gObj[iWinnerIndex].m_byKillMobCount,
			gObj[iWinnerIndex].m_byKillUserCount,
			nPoint);
	}
}

void ChaosCastleFinal::AddFallUser(int iUserIndex)
{
	for (int iFALL = 0; iFALL < 70; iFALL++)
	{
		if (this->m_stCCFData.m_lFallUser[iFALL] == -1)
		{
			InterlockedExchange(&this->m_stCCFData.m_lFallUser[iFALL], iUserIndex);
			return;
		}
	}
}

void ChaosCastleFinal::SearchNBlowObjs(int iMapNumber, int iX, int iY)
{
	if (!CHECK_LIMIT(iX, 256) || !CHECK_LIMIT(iY, 256))
		return;

	int iMIN_X = iX - 3;
	int iMAX_X = iX + 3;
	int iMIN_Y = iY - 3;
	int iMAX_Y = iY + 3;

	if (iMIN_X < 0) iMIN_X = 0;
	if (iMIN_Y < 0) iMIN_Y = 0;
	if (iMIN_X > 255) iMIN_X = 255;
	if (iMIN_Y > 255) iMIN_Y = 255;

	for (int i = 0; i<70; i++)
	{
		if (this->m_stCCFData.m_UserData[i].m_nIndex == -1)
		{
			continue;
		}

		int iIndex = this->m_stCCFData.m_UserData[i].m_nIndex;

		if (gObj[iIndex].MapNumber == iMapNumber && gObj[iIndex].Connected > PLAYER_LOGGED)
		{
			if (gObj[iIndex].X >= iMIN_X && gObj[iIndex].X <= iMAX_X &&
				gObj[iIndex].Y >= iMIN_Y && gObj[iIndex].Y <= iMAX_Y)
			{
				this->BlowObjsFromPoint(gObj[iIndex].m_Index, iMapNumber, iX, iY);

				BYTE btMapAttr = MapC[iMapNumber].m_attrbuf[iY * 256 + iX] & 0x08;

				if (btMapAttr == 8)
				{
					this->AddFallUser(iIndex);

					g_Log.Add("[Chaos Castle Survival] [%s][%s] User Dead In Chaos Castle : Fall from Castle (X:%d, Y:%d)",
						gObj[iIndex].AccountID, gObj[iIndex].Name, iX, iY);
				}
			}
		}
	}
}

int ChaosCastleFinal::BlowObjsFromPoint(int iIndex, int iMapNumber, int & iX, int & iY)
{
	if (this->m_bCCF_CHEAT_BLOW == false)
		return FALSE;

	if (!gObjIsConnected(iIndex))
		return FALSE;

	if (!CHECK_LIMIT(iX, 256) || !CHECK_LIMIT(iY, 256))
		return FALSE;

	LPOBJ lpObj = &gObj[iIndex];

	if (lpObj->DieRegen)
		return FALSE;

	if (lpObj->Teleport)
		return FALSE;

	if (lpObj->MapNumber != iMapNumber)
		return FALSE;

	int iOBJ_DIST = this->CalDistance(lpObj->X, lpObj->Y, iX, iY);

	if (!CHECK_LIMIT(iOBJ_DIST, 4))
		return FALSE;

	int iSIGN_X = 1;
	int iSIGN_Y = 1;
	int iUX = lpObj->X;
	int iUY = lpObj->Y;

	if (iUX > iX)
		iSIGN_X = 1;
	else if (iUX < iX)
		iSIGN_X = -1;
	else
	{
		int iRND = rand() % 2;
		(!iRND) ? iSIGN_X = 1 : iSIGN_X = -1;
	}

	if (iUY > iY)
		iSIGN_Y = 1;
	else if (iUY < iY)
		iSIGN_Y = -1;
	else
	{
		int iRND = rand() % 2;
		(!iRND) ? iSIGN_Y = 1 : iSIGN_Y = -1;
	}

	BOOL bSuccessBlowOut = FALSE;

	for (int iBLOWOUT_COUNT = 0; iBLOWOUT_COUNT < 5; iBLOWOUT_COUNT++)
	{
		int iBLOW_MIN = g_nCCF_BlowOutDistance[iOBJ_DIST][0];
		int iBLOW_MAX = g_nCCF_BlowOutDistance[iOBJ_DIST][1];
		int iBLOW_X = rand() % (iBLOW_MAX - iBLOW_MIN + 1) + iBLOW_MIN;
		int iBLOW_Y = rand() % (iBLOW_MAX - iBLOW_MIN + 1) + iBLOW_MIN;
		int iRND = rand() % 2;

		if (iRND)
		{
			if (iBLOW_X >= iBLOW_MAX)
			{
				iBLOW_X = iBLOW_MAX;
				iBLOW_Y = iBLOW_MIN + (rand() % 2 - 1);

				if (iBLOW_Y < 0)
					iBLOW_Y = 0;
			}
		}
		else if (iBLOW_Y >= iBLOW_MAX)
		{
			iBLOW_Y = iBLOW_MAX;
			iBLOW_X = iBLOW_MIN + (rand() % 2 - 1);

			if (iBLOW_X < 0)
				iBLOW_X = 0;
		}

		int iTX = lpObj->X + iBLOW_X * iSIGN_X;
		int iTY = lpObj->Y + iBLOW_Y * iSIGN_Y;

		if (iTX < 0) iTX = 0;
		if (iTY < 0) iTY = 0;
		if (iTX > 255) iTX = 255;
		if (iTY > 255) iTY = 255;

		bSuccessBlowOut = this->ObjSetPosition(iIndex, iTX, iTY);

		if (bSuccessBlowOut)
		{
			lpObj->m_iChaosCastleBlowTime = GetTickCount();
			iX = iTX;
			iY = iTY;

			this->GiveUserDamage(lpObj->m_Index, g_nCCF_BlowOutDamage[iOBJ_DIST % 4]);
			break;
		}
	}

	return TRUE;
}

int ChaosCastleFinal::CalDistance(int iX1, int iY1, int iX2, int iY2)
{
	if (iX1 == iX2 && iY1 == iY2)
	{
		return 0;
	}

	float fTX = (float)(iX1 - iX2);
	float fTY = (float)(iY1 - iY2);

	return sqrt((fTX * fTX) + (fTY * fTY));
}

int ChaosCastleFinal::GetCurrentCCFWinUser(int *nPoint)
{
	int iCCF_SCORE = -1;
	int iCCF_WINNER_INDEX = -1;
	int OldWinnerLevel = 0;
	UINT64 Oldi64MasterLevelExp = 0;

	for (int i = 0; i < 70; i++)
	{
		if (this->m_stCCFData.m_UserData[i].m_nIndex != -1
			&& gObjIsConnected(this->m_stCCFData.m_UserData[i].m_nIndex) == TRUE
			&& gObj[this->m_stCCFData.m_UserData[i].m_nIndex].MapNumber == MAP_INDEX_CHAOSCASTLE_SURVIVAL
			&& !this->m_stCCFData.m_UserData[i].m_nUserState)
		{
			LPOBJ lpObj = &gObj[this->m_stCCFData.m_UserData[i].m_nIndex];
			int iCCF_TEMP_SCORE = lpObj->m_byKillMobCount + 2 * lpObj->m_byKillUserCount;

			if (iCCF_SCORE >= iCCF_TEMP_SCORE)
			{
				if (iCCF_SCORE == iCCF_TEMP_SCORE)
				{
					int tempNewAllLevel = lpObj->m_PlayerData->MasterLevel + lpObj->Level;
					if (OldWinnerLevel <= tempNewAllLevel)
					{
						if (OldWinnerLevel == tempNewAllLevel)
						{
							if (Oldi64MasterLevelExp > lpObj->m_PlayerData->MasterExperience)
							{
								iCCF_SCORE = lpObj->m_byKillMobCount + 2 * lpObj->m_byKillUserCount;
								iCCF_WINNER_INDEX = lpObj->m_Index;
								OldWinnerLevel = lpObj->m_PlayerData->MasterLevel + lpObj->Level;
								*nPoint = iCCF_TEMP_SCORE;
								this->m_stCCFData.m_UserData[i].m_nScore = iCCF_TEMP_SCORE;
								Oldi64MasterLevelExp = lpObj->m_PlayerData->MasterExperience;
							}
						}
					}
					else
					{
						iCCF_SCORE = lpObj->m_byKillMobCount + 2 * lpObj->m_byKillUserCount;
						iCCF_WINNER_INDEX = lpObj->m_Index;
						OldWinnerLevel = lpObj->m_PlayerData->MasterLevel + lpObj->Level;
						*nPoint = iCCF_TEMP_SCORE;
						this->m_stCCFData.m_UserData[i].m_nScore = iCCF_TEMP_SCORE;
						Oldi64MasterLevelExp = lpObj->m_PlayerData->MasterExperience;
					}
				}
			}
			else
			{
				iCCF_SCORE = lpObj->m_byKillMobCount + 2 * lpObj->m_byKillUserCount;
				iCCF_WINNER_INDEX = lpObj->m_Index;
				OldWinnerLevel = lpObj->m_PlayerData->MasterLevel + lpObj->Level;
				*nPoint = iCCF_TEMP_SCORE;
				this->m_stCCFData.m_UserData[i].m_nScore = iCCF_TEMP_SCORE;
				Oldi64MasterLevelExp = lpObj->m_PlayerData->MasterExperience;
			}
		}
	}

	if (iCCF_WINNER_INDEX != -1)
	{
		if (!gObj[iCCF_WINNER_INDEX].m_byKillUserCount)
		{
			if (!gObj[iCCF_WINNER_INDEX].m_byKillMobCount)
			{
				if (this->GetCurPlayUser() != TRUE || this->GetMonsterListCount())
					iCCF_WINNER_INDEX = -1;
			}
		}
	}
	
	this->Check_CCF_DayType();
	*nPoint += 3;
	return iCCF_WINNER_INDEX;
}

int ChaosCastleFinal::GetMonsterListCount()
{
	int iAliveMonsterCount = 0;
	for (int iMON = 0; iMON < 100; iMON++)
	{
		if (this->m_stCCFData.m_lCCF_MONSTER_COUNT[iMON] != -1)
		{
			int iMonsterIndex = this->m_stCCFData.m_lCCF_MONSTER_COUNT[iMON];

			if (ObjectMaxRange(iMonsterIndex))
			{
				if (gObj[iMonsterIndex].Life > 0.0 && gObj[iMonsterIndex].MapNumber == MAP_INDEX_CHAOSCASTLE_SURVIVAL)
				{
					if (gObj[iMonsterIndex].Connected > 0)
						++iAliveMonsterCount;
				}
			}
			else
			{
				this->m_stCCFData.m_lCCF_MONSTER_COUNT[iMON] = -1;
			}
		}
	}
	return iAliveMonsterCount;
}

int ChaosCastleFinal::CheckPlayEnded()
{
	int iCUR_USER_COUNT = this->GetCurPlayUser();
	int iCUR_MONSTER_COUNT = this->GetMonsterListCount();

	if (iCUR_USER_COUNT > 0)
		return iCUR_USER_COUNT == 1 && iCUR_MONSTER_COUNT <= 0;
	else
		return 2;
}

void ChaosCastleFinal::PullObjInnerPlace(int iTRAP_STEP)
{
	if (CCF_TRAP_STEP_RANGE(iTRAP_STEP) == FALSE)
		return;

	if (iTRAP_STEP == 0)
		return;

	int iUSER_AXIS = 0;
	std::vector<POINT> vtMAP_UNTRAP[MAX_CCF_TRAP_STEP];

	for (int iAXIS = 0; iAXIS < MAX_CCF_TRAP_STEP; iAXIS++)
	{
		for (int iMAPX = g_rt_CCF_POINT_TRAP[iAXIS].left; iMAPX <= g_rt_CCF_POINT_TRAP[iAXIS].right; iMAPX++)
		{
			for (int iMAPY = g_rt_CCF_POINT_TRAP[iAXIS].top; iMAPY <= g_rt_CCF_POINT_TRAP[iAXIS].bottom; iMAPY++)
			{
				BYTE btMapAttr = MapC[MAP_INDEX_CHAOSCASTLE_SURVIVAL].GetAttr(iMAPX, iMAPY);

				if ((btMapAttr & 2) != 2 && (btMapAttr & 4) != 4 && (btMapAttr & 8) != 8)
				{
					POINT iPT = { iMAPX, iMAPY };
					vtMAP_UNTRAP[iAXIS].push_back(iPT);
				}
			}
		}
	}

	for (int i = 0; i<70; i++)
	{
		if (this->m_stCCFData.m_UserData[i].m_nIndex != -1)
		{
			if (gObjIsConnected(this->m_stCCFData.m_UserData[i].m_nIndex) == TRUE && gObj[this->m_stCCFData.m_UserData[i].m_nIndex].MapNumber == MAP_INDEX_CHAOSCASTLE_SURVIVAL)
			{
				int iSX = gObj[this->m_stCCFData.m_UserData[i].m_nIndex].X;
				int iSY = gObj[this->m_stCCFData.m_UserData[i].m_nIndex].Y;

				BYTE btMapAttr = MapC[MAP_INDEX_CHAOSCASTLE_SURVIVAL].m_attrbuf[iSY * 256 + iSX] & 0x08;

				if (btMapAttr == 8)
				{
					int n;
					for (n = 0; n<MAX_CCF_TRAP_STEP; n++)
					{
						if (iSX >= g_rt_CCF_POINT_FRAME[n].left && iSX <= g_rt_CCF_POINT_FRAME[n].right &&
							iSY >= g_rt_CCF_POINT_FRAME[n].top && iSY <= g_rt_CCF_POINT_FRAME[n].bottom)
						{
							iUSER_AXIS = n;
							break;
						}
					}

					if (n == MAX_CCF_TRAP_STEP)
						continue;

					if (vtMAP_UNTRAP[iUSER_AXIS].empty() == false)
					{
						POINT ptEMPTY = vtMAP_UNTRAP[iUSER_AXIS].back();
						vtMAP_UNTRAP[iUSER_AXIS].pop_back();

						if (this->ObjSetPosition(this->m_stCCFData.m_UserData[i].m_nIndex, ptEMPTY.x, ptEMPTY.y) == TRUE)
						{
							gObj[this->m_stCCFData.m_UserData[i].m_nIndex].m_nCCF_BlowTime = GetTickCount();
						}
					}
				}
			}
		}
	}

	for (int iMON = 0; iMON < 100; iMON++)
	{
		if (this->m_stCCFData.m_lCCF_MONSTER_COUNT[iMON] == -1)
			continue;

		int iMON_INDEX = this->m_stCCFData.m_lCCF_MONSTER_COUNT[iMON];

		if (!CHECK_LIMIT(iMON_INDEX, g_ConfigRead.server.GetObjectMaxMonster()))
		{
			this->m_stCCFData.m_lCCF_MONSTER_COUNT[iMON] = -1;
			continue;
		}

		if (gObj[iMON_INDEX].Life > 0.0f && gObj[iMON_INDEX].MapNumber == MAP_INDEX_CHAOSCASTLE_SURVIVAL != FALSE && gObj[iMON_INDEX].Connected > PLAYER_EMPTY)
		{
			int iSX = gObj[iMON_INDEX].X;
			int iSY = gObj[iMON_INDEX].Y;

			BYTE btMapAttr = MapC[MAP_INDEX_CHAOSCASTLE_SURVIVAL].m_attrbuf[iSY * 256 + iSX] & 0x08;

			if (btMapAttr == 8)
			{
				int n;
				for (n = 0; n<MAX_CCF_TRAP_STEP; n++)
				{
					if (iSX >= g_rt_CCF_POINT_FRAME[n].left && iSX <= g_rt_CCF_POINT_FRAME[n].right &&
						iSY >= g_rt_CCF_POINT_FRAME[n].top && iSY <= g_rt_CCF_POINT_FRAME[n].bottom)
					{
						iUSER_AXIS = n;
						break;
					}
				}

				if (n == MAX_CCF_TRAP_STEP)
					continue;

				if (vtMAP_UNTRAP[iUSER_AXIS].empty() == false)
				{
					POINT ptEMPTY = vtMAP_UNTRAP[iUSER_AXIS].back();
					vtMAP_UNTRAP[iUSER_AXIS].pop_back();

					if (this->ObjSetPosition(iMON_INDEX, ptEMPTY.x, ptEMPTY.y) == TRUE)
					{
						gObj[iMON_INDEX].m_iChaosCastleBlowTime = GetTickCount();
					}
				}
			}
		}
	}

	for (int iMAP_ITEM = 0; iMAP_ITEM < g_ConfigRead.server.GetObjectMaxItem(); iMAP_ITEM++)
	{
		if (MapC[MAP_INDEX_CHAOSCASTLE_SURVIVAL].m_cItem[iMAP_ITEM].IsItem())
		{
			int x = MapC[MAP_INDEX_CHAOSCASTLE_SURVIVAL].m_cItem[iMAP_ITEM].px;
			int y = MapC[MAP_INDEX_CHAOSCASTLE_SURVIVAL].m_cItem[iMAP_ITEM].py;

			if (x < ::g_iCCF_DamageAxis[iTRAP_STEP][0] || x > ::g_iCCF_DamageAxis[iTRAP_STEP][2] ||
				y < ::g_iCCF_DamageAxis[iTRAP_STEP][1] || y > ::g_iCCF_DamageAxis[iTRAP_STEP][3])
			{
				MapC[MAP_INDEX_CHAOSCASTLE_SURVIVAL].m_cItem[iMAP_ITEM].m_State = 8;
				MapC[MAP_INDEX_CHAOSCASTLE_SURVIVAL].m_cItem[iMAP_ITEM].Give = true;
				MapC[MAP_INDEX_CHAOSCASTLE_SURVIVAL].m_cItem[iMAP_ITEM].live = false;
			}
		}
	}
}

void ChaosCastleFinal::SetMapAttrHollow(int iTRAP_STEP)
{
	if (CCF_TRAP_STEP_RANGE(iTRAP_STEP) == FALSE)
	{
		return;
	}

	if (iTRAP_STEP == 0)
	{
		return;
	}

	for (int a = 0; a<MAX_CCF_HOLLOW_ZONE; a++)
	{
		for (int b = ::g_iCCF_MapHollowZone[iTRAP_STEP][a][0]; b <= ::g_iCCF_MapHollowZone[iTRAP_STEP][a][2]; b++)
		{
			for (int c = ::g_iCCF_MapHollowZone[iTRAP_STEP][a][1]; c <= ::g_iCCF_MapHollowZone[iTRAP_STEP][a][3]; c++)
			{
				MapC[MAP_INDEX_CHAOSCASTLE_SURVIVAL].m_attrbuf[c * 256 + b] |= 8;
			}
		}
	}
}

void ChaosCastleFinal::SetMapAttrFill()
{
	for (int iTRAP_STEP = 1; iTRAP_STEP<MAX_CCF_TRAP_STEP; iTRAP_STEP++)
	{
		for (int a = 0; a<MAX_CCF_HOLLOW_ZONE; a++)
		{
			for (int b = ::g_iCCF_MapHollowZone[iTRAP_STEP][a][0]; b <= ::g_iCCF_MapHollowZone[iTRAP_STEP][a][2]; b++)
			{
				for (int c = ::g_iCCF_MapHollowZone[iTRAP_STEP][a][1]; c <= ::g_iCCF_MapHollowZone[iTRAP_STEP][a][3]; c++)
				{
					MapC[MAP_INDEX_CHAOSCASTLE_SURVIVAL].m_attrbuf[c * 256 + b] &= ~8;
				}
			}
		}
	}
}

void ChaosCastleFinal::ProcessTrapStatus()
{
	int iCUR_LIVES = this->GetCurPlayUser() + this->GetMonsterListCount();

	if (iCUR_LIVES > 40)
	{
		this->m_stCCFData.m_nCCF_TRAP_STATUS = 0;
	}

	else if (iCUR_LIVES > 30)
	{
		this->m_stCCFData.m_nCCF_TRAP_STATUS = 1;
		this->SendNoticeState(8);
	}

	else if (iCUR_LIVES > 20)
	{
		this->m_stCCFData.m_nCCF_TRAP_STATUS = 2;
		this->SendNoticeState(9);
	}

	else
	{
		this->m_stCCFData.m_nCCF_TRAP_STATUS = 3;
		this->SendNoticeState(10);
	}
}

BOOL ChaosCastleFinal::CheckWearingMOPH(int iUserIndex)
{
	if (!gObjIsConnected(iUserIndex))
		return FALSE;

	if (gObj[iUserIndex].pInventory[10].IsItem() == TRUE)
	{
		if (gObj[iUserIndex].pInventory[10].m_Type == ITEMGET(13,10))
			return TRUE;
		if (gObj[iUserIndex].pInventory[10].m_Type == ITEMGET(13,39))
			return TRUE;
		if (gObj[iUserIndex].pInventory[10].m_Type == ITEMGET(13,40))
			return TRUE;
		if (gObj[iUserIndex].pInventory[10].m_Type == ITEMGET(13,41))
			return TRUE;
		if (gObj[iUserIndex].pInventory[10].m_Type == ITEMGET(13,68))
			return TRUE;
		if (gObj[iUserIndex].pInventory[10].m_Type == ITEMGET(13,76))
			return TRUE;
		if (gObj[iUserIndex].pInventory[10].m_Type == ITEMGET(13,77))
			return TRUE;
		if (gObj[iUserIndex].pInventory[10].m_Type == ITEMGET(13,78))
			return TRUE;
		if (gObj[iUserIndex].pInventory[10].m_Type == ITEMGET(13,122))
			return TRUE;
		if (gObj[iUserIndex].pInventory[10].m_Type == ITEMGET(13,163))
			return TRUE;
		if (gObj[iUserIndex].pInventory[10].m_Type == ITEMGET(13,164))
			return TRUE;
		if (gObj[iUserIndex].pInventory[10].m_Type == ITEMGET(13,166))
			return TRUE;
		if (gObj[iUserIndex].pInventory[10].m_Type == ITEMGET(13,169))
			return TRUE;
		if (gObj[iUserIndex].pInventory[10].m_Type == ITEMGET(13,170))
			return TRUE;
		if (gObj[iUserIndex].pInventory[10].m_Type == ITEMGET(13,268))
			return TRUE;
	}

	if (gObj[iUserIndex].pInventory[11].IsItem() == TRUE)
	{
		if (gObj[iUserIndex].pInventory[11].m_Type == ITEMGET(13,10))
			return TRUE;
		if (gObj[iUserIndex].pInventory[11].m_Type == ITEMGET(13,39))
			return TRUE;
		if (gObj[iUserIndex].pInventory[11].m_Type == ITEMGET(13,40))
			return TRUE;
		if (gObj[iUserIndex].pInventory[11].m_Type == ITEMGET(13,41))
			return TRUE;
		if (gObj[iUserIndex].pInventory[11].m_Type == ITEMGET(13,68))
			return TRUE;
		if (gObj[iUserIndex].pInventory[11].m_Type == ITEMGET(13,76))
			return TRUE;
		if (gObj[iUserIndex].pInventory[11].m_Type == ITEMGET(13,77))
			return TRUE;
		if (gObj[iUserIndex].pInventory[11].m_Type == ITEMGET(13,78))
			return TRUE;
		if (gObj[iUserIndex].pInventory[11].m_Type == ITEMGET(13,122))
			return TRUE;
		if (gObj[iUserIndex].pInventory[11].m_Type == ITEMGET(13,163))
			return TRUE;
		if (gObj[iUserIndex].pInventory[11].m_Type == ITEMGET(13,164))
			return TRUE;
		if (gObj[iUserIndex].pInventory[10].m_Type == ITEMGET(13,166))
			return TRUE;
		if (gObj[iUserIndex].pInventory[11].m_Type == ITEMGET(13,169))
			return TRUE;
		if (gObj[iUserIndex].pInventory[11].m_Type == ITEMGET(13,170))
			return TRUE;
		if (gObj[iUserIndex].pInventory[11].m_Type == ITEMGET(13,268))
			return TRUE;
	}

	return FALSE;
}

void ChaosCastleFinal::CheckMonsterInDieTile()
{
	if (this->m_bCCF_CHEAT_TRAP == false)
	{
		return;
	}

	int iUSER_AXIS = 0;
	std::vector<POINT> vtMAP_UNTRAP[4];

	for (int iAXIS = 0; iAXIS < 4; iAXIS++)
	{
		for (int iMAPX = g_rt_CCF_POINT_TRAP[iAXIS].left; iMAPX <= g_rt_CCF_POINT_TRAP[iAXIS].right; iMAPX++)
		{
			for (int iMAPY = g_rt_CCF_POINT_TRAP[iAXIS].top; iMAPY <= g_rt_CCF_POINT_TRAP[iAXIS].bottom; iMAPY++)
			{
				BYTE btMapAttr = MapC[MAP_INDEX_CHAOSCASTLE_SURVIVAL].GetAttr(iMAPX, iMAPY);

				if ((btMapAttr & 2) != 2 && (btMapAttr & 4) != 4 && (btMapAttr & 8) != 8)
				{
					POINT iPT = { iMAPX, iMAPY };
					vtMAP_UNTRAP[iAXIS].push_back(iPT);
				}
			}
		}
	}

	for (int iMON = 0; iMON < 100; iMON++)
	{
		if (this->m_stCCFData.m_lCCF_MONSTER_COUNT[iMON] == -1)
			continue;

		int iMON_INDEX = this->m_stCCFData.m_lCCF_MONSTER_COUNT[iMON];

		if (!CHECK_LIMIT(iMON_INDEX, g_ConfigRead.server.GetObjectMaxMonster()))
		{
			this->m_stCCFData.m_lCCF_MONSTER_COUNT[iMON] = -1;
			continue;
		}

		if (gObj[iMON_INDEX].Life > 0.0f && gObj[iMON_INDEX].MapNumber == MAP_INDEX_CHAOSCASTLE_SURVIVAL && gObj[iMON_INDEX].Connected > PLAYER_EMPTY)
		{
			int iSX = gObj[iMON_INDEX].X;
			int iSY = gObj[iMON_INDEX].Y;

			BYTE btMapAttr = MapC[MAP_INDEX_CHAOSCASTLE_SURVIVAL].m_attrbuf[iSY * 256 + iSX] & 0x08;

			if (btMapAttr == 8)
			{
				int n;
				for (n = 0; n<MAX_CCF_TRAP_STEP; n++)
				{
					if (iSX >= g_rt_CCF_POINT_FRAME[n].left && iSX <= g_rt_CCF_POINT_FRAME[n].right &&
						iSY >= g_rt_CCF_POINT_FRAME[n].top && iSY <= g_rt_CCF_POINT_FRAME[n].bottom)
					{
						iUSER_AXIS = n;
						break;
					}
				}

				if (n == MAX_CCF_TRAP_STEP)
					continue;

				if (vtMAP_UNTRAP[iUSER_AXIS].empty() == false)
				{
					POINT ptEMPTY = vtMAP_UNTRAP[iUSER_AXIS].back();
					vtMAP_UNTRAP[iUSER_AXIS].pop_back();

					if (this->ObjSetPosition(iMON_INDEX, ptEMPTY.x, ptEMPTY.y) == TRUE)
					{
						gObj[iMON_INDEX].m_nCCF_BlowTime = GetTickCount();
					}
				}
			}
		}
	}
}

void ChaosCastleFinal::CheckUserInDieTile()
{
	for (int i = 0; i<70; i++)
	{
		if (this->m_stCCFData.m_UserData[i].m_nIndex != -1)
		{
			if (gObjIsConnected(this->m_stCCFData.m_UserData[i].m_nIndex) == TRUE &&
				gObj[this->m_stCCFData.m_UserData[i].m_nIndex].MapNumber == MAP_INDEX_CHAOSCASTLE_SURVIVAL)
			{
				if (gObj[this->m_stCCFData.m_UserData[i].m_nIndex].Life > 0.0)
				{
					int iSX = gObj[this->m_stCCFData.m_UserData[i].m_nIndex].X;
					int iSY = gObj[this->m_stCCFData.m_UserData[i].m_nIndex].Y;

					BYTE btMapAttr = MapC[MAP_INDEX_CHAOSCASTLE_SURVIVAL].m_attrbuf[iSY * 256 + iSX] & 0x08;

					if (btMapAttr == 8)
					{
						gObj[this->m_stCCFData.m_UserData[i].m_nIndex].Life = 0;
						gObj[this->m_stCCFData.m_UserData[i].m_nIndex].m_State = 4;
						gObj[this->m_stCCFData.m_UserData[i].m_nIndex].KillerType = 1;
						gObj[this->m_stCCFData.m_UserData[i].m_nIndex].RegenTime = GetTickCount();
						gObj[this->m_stCCFData.m_UserData[i].m_nIndex].DieRegen = 1;
						gObj[this->m_stCCFData.m_UserData[i].m_nIndex].PathCount = 0;

						GSProtocol.GCDiePlayerSend(&gObj[this->m_stCCFData.m_UserData[i].m_nIndex],
							this->m_stCCFData.m_UserData[i].m_nIndex, 0, 0);

						g_Log.Add("[Chaos Castle Survival] [%s][%s] User Dead In Chaos Castle : Fall from Castle (X:%d, Y:%d)",
							gObj[this->m_stCCFData.m_UserData[i].m_nIndex].AccountID,
							gObj[this->m_stCCFData.m_UserData[i].m_nIndex].Name,
							iSX, iSY);

						this->SendFailMessage(this->m_stCCFData.m_UserData[i].m_nIndex);

					}
				}
			}
		}
	}
}

void ChaosCastleFinal::ProcessFallUser()
{
	for (int iFALL = 0; iFALL<70; iFALL++)
	{
		if (this->m_stCCFData.m_lFallUser[iFALL] != -1)
		{
			int iFALL_INDEX = this->m_stCCFData.m_lFallUser[iFALL];
			InterlockedExchange(&this->m_stCCFData.m_lFallUser[iFALL], -1);

			if (gObj[iFALL_INDEX].MapNumber == MAP_INDEX_CHAOSCASTLE_SURVIVAL && gObjIsConnected(iFALL_INDEX) && gObj[iFALL_INDEX].Life > 0)
			{
				gObj[iFALL_INDEX].Life = 0;
				gObj[iFALL_INDEX].m_State = 4;
				gObj[iFALL_INDEX].KillerType = 1;
				gObj[iFALL_INDEX].RegenTime = GetTickCount();
				gObj[iFALL_INDEX].DieRegen = 1;
				gObj[iFALL_INDEX].PathCount = 0;

				GSProtocol.GCDiePlayerSend(&gObj[iFALL_INDEX], gObj[iFALL_INDEX].m_Index, 0, 0);
			}
		}
	}
}

void ChaosCastleFinal::SetUserState(int iUserIndex, int iState)
{
	if (!ObjectMaxRange(iUserIndex))
		return;

	switch (iState)
	{
	case 0:
		this->m_stCCFData.m_UserData[gObj[iUserIndex].m_nCCF_UserIndex].m_nUserState = 0;
		break;
	case 1:
		this->m_stCCFData.m_UserData[gObj[iUserIndex].m_nCCF_UserIndex].m_nUserState = 1;
		break;
	}
}

int ChaosCastleFinal::GetCurPlayUser()
{
	int iPlayUser = 0;

	for (int i = 0; i<70; i++)
	{
		if (this->m_stCCFData.m_UserData[i].m_nIndex != -1)
		{
			if (gObjIsConnected(this->m_stCCFData.m_UserData[i].m_nIndex) == TRUE &&
				gObj[this->m_stCCFData.m_UserData[i].m_nIndex].MapNumber == MAP_INDEX_CHAOSCASTLE_SURVIVAL)
			{
				iPlayUser++;
			}
		}
	}

	return iPlayUser;
}

int ChaosCastleFinal::GetFinalRemainTime(int iUserIndex)
{
	CCF_START_TIME pRET;
	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);

	int iREMAIN_MINUTE = 0;

	for (std::vector<CCF_START_TIME>::iterator it = this->m_vtCCFOpenTime.begin(); it != this->m_vtCCFOpenTime.end(); it++)
	{
		pRET = *it;

		if (pRET.m_nFinal == 3)
		{
			iREMAIN_MINUTE = (60 * pRET.m_nMinute + 3600 * pRET.m_nHour - (sysTime.wSecond + 60 * sysTime.wMinute + 3600 * sysTime.wHour)) / 60 + 1;
		}
	}

	return iREMAIN_MINUTE;
}

int ChaosCastleFinal::GetRemainTime(int iUserIndex)
{
	int iREMAIN_MINUTE = 0;

	if (this->GetCurrentState() == 1)
	{
		iREMAIN_MINUTE = this->m_stCCFData.m_nCCF_REMAIN_MSEC / 60000 - this->m_iCCF_TIME_MIN_OPEN + 1;
	}
	else
	{
		SYSTEMTIME sysTime;
		GetLocalTime(&sysTime);
		int iMIN_HOUR = 24;
		int iMIN_MINUTE = 60;
		BOOL bTIME_CHANGED = FALSE;
		std::vector<CCF_START_TIME>::iterator it;

		for (it = this->m_vtCCFOpenTime.begin(); it != this->m_vtCCFOpenTime.end(); it++)
		{
			CCF_START_TIME & pRET = *it;

			if ((sysTime.wHour * 60 + sysTime.wMinute) < (pRET.m_nHour * 60 + pRET.m_nMinute))
			{
				if ((iMIN_HOUR * 60 + iMIN_MINUTE) > (pRET.m_nHour * 60 + pRET.m_nMinute))
				{
					bTIME_CHANGED = 1;
					iMIN_HOUR = pRET.m_nHour;
					iMIN_MINUTE = pRET.m_nMinute;
				}
			}
		}


		if (bTIME_CHANGED == 0)
		{
			it = this->m_vtCCFOpenTime.begin();
			iMIN_HOUR = 24;
			iMIN_MINUTE = 60;

			for (; it != this->m_vtCCFOpenTime.end(); it++)
			{
				CCF_START_TIME & pRET = *it;

				if ((iMIN_HOUR * 60 + iMIN_MINUTE) > (pRET.m_nHour * 60 + pRET.m_nMinute))
				{
					bTIME_CHANGED = 2;
					iMIN_HOUR = pRET.m_nHour;
					iMIN_MINUTE = pRET.m_nMinute;
				}
			}
		}

		switch (bTIME_CHANGED)
		{
		case 1:
			iREMAIN_MINUTE = (((iMIN_HOUR * 60 * 60 + iMIN_MINUTE * 60) - (sysTime.wHour * 60 * 60 + sysTime.wMinute * 60 + sysTime.wSecond))) / 60 + 1;
			break;
		case 2:
			iREMAIN_MINUTE = ((((iMIN_HOUR + 24) * 60 * 60 + iMIN_MINUTE * 60) - (sysTime.wHour * 60 * 60 + sysTime.wMinute * 60 + sysTime.wSecond))) / 60 + 1;
			break;
		default:
			iREMAIN_MINUTE = -1;
			break;
		}
	}

	if (iREMAIN_MINUTE < 0)
		iREMAIN_MINUTE = 0;

	return iREMAIN_MINUTE;
}

void ChaosCastleFinal::SendNoticeState(int iPlayState)
{
	PMSG_STATEBLOODCASTLE pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x9B, sizeof(PMSG_STATEBLOODCASTLE));
	pMsg.btPlayState = iPlayState;
	pMsg.wRemainSec = this->m_stCCFData.m_nCCF_REMAIN_MSEC / 1000;
	pMsg.wMaxKillMonster = this->m_stCCFData.m_nCCF_FIRST_USER_COUNT + this->m_stCCFData.m_nCCF_FIRST_MONSTER_COUNT;
	pMsg.wCurKillMonster = this->GetCurPlayUser() + this->GetMonsterListCount();
	pMsg.wUserHaveWeapon = -1;
	pMsg.btWeaponNum = -1;

	for (int i = 0; i < 70; i++)
	{
		if (this->m_stCCFData.m_UserData[i].m_nIndex != -1)
		{
			if (gObj[this->m_stCCFData.m_UserData[i].m_nIndex].Connected > PLAYER_LOGGED)
			{
				if (gObj[this->m_stCCFData.m_UserData[i].m_nIndex].m_nCCF_UserIndex != -1)
				{
					IOCP.DataSend(this->m_stCCFData.m_UserData[i].m_nIndex, (UCHAR*)&pMsg, pMsg.h.size);
				}
			}
		}
	}
}

void ChaosCastleFinal::SearchCCFNDropMonsterItem(int iMonsterIndex, int iMaxHitUserIndex)
{
	if (ObjectMaxRange(iMonsterIndex) == false)
	{
		return;
	}

	if (ObjectMaxRange(iMaxHitUserIndex) == false)
	{
		return;
	}

	if (this->m_stCCFData.m_nCCF_STATE != 2)
	{
		return;
	}

	EnterCriticalSection(&this->m_stCCFData.m_CCFDropCriti);

	std::map<int, _CCF_MONSTER_ITEM_DROP>::iterator it = this->m_stCCFData.m_mapCCFMonsterItemList.find(iMonsterIndex);

	if (it == this->m_stCCFData.m_mapCCFMonsterItemList.end())
	{
		LeaveCriticalSection(&this->m_stCCFData.m_CCFDropCriti);
		return;
	}

	int iItemType = it->second.m_nItemKind;
	int iLevel = 0;

	if (iItemType == ITEMGET(13, 15))
	{
		iLevel = rand() % 4;
	}

	BOOL bDrop = FALSE;
	int nRand = rand() % 100;

	if (nRand >= this->m_iCCF_BoxRatio1 || this->m_byBoxDropCnt)
	{
		if (this->m_iCCF_BoxRatio1 >= nRand || nRand >= this->m_iCCF_BoxRatio2 || this->m_byBoxDropCnt != 1)
		{
			if (this->m_iCCF_BoxRatio2 >= nRand || nRand >= this->m_iCCF_BoxRatio3 || this->m_byBoxDropCnt != 2)
			{
				if (this->m_iCCF_BoxRatio3 >= nRand || nRand >= this->m_iCCF_BoxRatio4 || this->m_byBoxDropCnt != 3)
				{
					if (this->m_iCCF_BoxRatio4 >= nRand || nRand >= this->m_iCCF_BoxRatio5 || this->m_byBoxDropCnt != 4)
					{
						if (this->m_iCCF_BoxRatio5 >= nRand || nRand >= this->m_iCCF_BoxRatio6 || this->m_byBoxDropCnt != 5)
						{
							if (this->m_byBoxDropCnt > 6)
							{
								return;
							}
						}
			
						else
						{
							bDrop = TRUE;
							this->m_byBoxDropCnt++;
						}
					}
		
					else
					{
						bDrop = TRUE;
						this->m_byBoxDropCnt++;
					}
				}

				else
				{
					bDrop = TRUE;
					this->m_byBoxDropCnt++;
				}
			}
			
			else
			{
				bDrop = TRUE;
				this->m_byBoxDropCnt++;
			}
		}

		else
		{
			bDrop = TRUE;
			this->m_byBoxDropCnt++;
		}
	}
	
	else
	{
		bDrop = TRUE;
		this->m_byBoxDropCnt++;
	}

	if (bDrop == TRUE)
	{
		ItemSerialCreateSend(iMaxHitUserIndex, gObj[iMaxHitUserIndex].MapNumber, gObj[iMaxHitUserIndex].X, gObj[iMaxHitUserIndex].Y, iItemType, iLevel, 0, 0, 0, 0, iMaxHitUserIndex, 0, 0, 0, 0, 0);

		g_Log.Add("[Chaos Castle Survival] Monster Dropped ChaosCastleFinal Item To [%s][%s] (%d)(Item:%s)",
			gObj[iMaxHitUserIndex].AccountID, gObj[iMaxHitUserIndex].Name, iMonsterIndex, ItemAttribute[iItemType].Name);
	}

	g_Log.Add("[Chaos Castle Survival][SearchCCFNDropMonsterItem] BoxCount :%d, Random %d ",
		this->m_byBoxDropCnt, nRand);

	LeaveCriticalSection(&this->m_stCCFData.m_CCFDropCriti);
}

void ChaosCastleFinal::SetItemsToMonster(int iCCFType)
{
	int iMAX_MONSTER = this->GetMonsterListCount();

	if (iMAX_MONSTER <= 0)
	{
		return;
	}

	EnterCriticalSection(&this->m_stCCFData.m_CCFDropCriti);

	for (int iB = 0; iB < ::g_iCCF_MonsterItems[iCCFType-1][1]; iB++)
	{
		for (int iC = 0; iC < 200; iC++)
		{
			int iMONSTER_INDEX = this->m_stCCFData.m_lCCF_MONSTER_COUNT[rand() % iMAX_MONSTER];
			std::map<int, _CCF_MONSTER_ITEM_DROP>::iterator it = this->m_stCCFData.m_mapCCFMonsterItemList.find(iMONSTER_INDEX);
			
			if (it == this->m_stCCFData.m_mapCCFMonsterItemList.end())
			{
				_CCF_MONSTER_ITEM_DROP MID;
				
				MID.m_nIndex = iMONSTER_INDEX;
				MID.m_nItemKind = g_iCCF_MonsterItems[iCCFType-1][0];
				this->m_stCCFData.m_mapCCFMonsterItemList.insert(std::pair<int, _CCF_MONSTER_ITEM_DROP>(iMONSTER_INDEX, MID));

				g_Log.Add("[Chaos Castle Survival] Set Monster Drop Item (%d)(Item:%s)",
					iMONSTER_INDEX, ItemAttribute[MID.m_nItemKind].Name);
				break;
			}
		}
	}

	LeaveCriticalSection(&this->m_stCCFData.m_CCFDropCriti);
	
	g_Log.Add("[Chaos Castle Survival] Set Monster Drop Item Count (%d)", this->m_stCCFData.m_mapCCFMonsterItemList.size());
}

int ChaosCastleFinal::SetMonster()
{
	int iNOW_ADDED_MONSTER_COUNT = 0;
	int iMAX_ADDABLE_MONSTER_COUNT = 100 - this->GetCurPlayUser();

	if (iMAX_ADDABLE_MONSTER_COUNT < 0)
		iMAX_ADDABLE_MONSTER_COUNT = 0;

	for (int n = 0; n < this->m_CCF_MOB_CNT_SCRIPT && iNOW_ADDED_MONSTER_COUNT < iMAX_ADDABLE_MONSTER_COUNT; n++)
	{
		if (this->m_CCFMP[n].m_MapNumber == MAP_INDEX_CHAOSCASTLE_SURVIVAL)
		{
			WORD btMonsterType = this->m_CCFMP[n].m_Type;
			int result = gObjAddMonster(this->m_CCFMP[n].m_MapNumber);

			if (result >= 0)
			{
				gObj[result].m_PosNum = n;
				gObj[result].X = this->m_CCFMP[n].m_X;
				gObj[result].Y = this->m_CCFMP[n].m_Y;
				gObj[result].MapNumber = this->m_CCFMP[n].m_MapNumber;
				gObj[result].TX = gObj[result].X;
				gObj[result].TY = gObj[result].Y;
				gObj[result].m_OldX = gObj[result].X;
				gObj[result].m_OldY = gObj[result].Y;
				gObj[result].Dir = this->m_CCFMP[n].m_Dir;
				gObj[result].StartX = gObj[result].X;
				gObj[result].StartY = gObj[result].Y;
				gObjSetMonster(result, btMonsterType);
				gObj[result].MaxRegenTime = 0;
				gObj[result].Dir = rand() % 8;

				iNOW_ADDED_MONSTER_COUNT++;
				this->AddMonsterList(result);
			}

			g_Log.Add("gObjAddMonster return %d", result);
		}
	}

	return iNOW_ADDED_MONSTER_COUNT;
}

void ChaosCastleFinal::AddMonsterList(int iMonsterIndex)
{
	for (int iMON = 0; iMON < 100; iMON++)
	{
		if (this->m_stCCFData.m_lCCF_MONSTER_COUNT[iMON] == -1)
		{
			InterlockedExchange(&this->m_stCCFData.m_lCCF_MONSTER_COUNT[iMON], iMonsterIndex);
			return;
		}
	}
}

void ChaosCastleFinal::DelMonsterList(int iMonsterIndex)
{
	g_Log.Add("[Chaos Castle Survival] [Bug Tracker] MONSTER DEL Count(%d) : ENTER - Index(%d)",
		this->m_stCCFData.m_lCCF_CURRENT_MONSTER_COUNT, iMonsterIndex);

	for (int iMON = 0; iMON < 100; iMON++)
	{
		if (this->m_stCCFData.m_lCCF_MONSTER_COUNT[iMON] == iMonsterIndex)
		{
			InterlockedExchange(&this->m_stCCFData.m_lCCF_MONSTER_COUNT[iMON], -1);
			break;
		}
	}

	InterlockedDecrement(&this->m_stCCFData.m_lCCF_CURRENT_MONSTER_COUNT);

	if (this->m_stCCFData.m_lCCF_CURRENT_MONSTER_COUNT < 0)
	{
		this->m_stCCFData.m_lCCF_CURRENT_MONSTER_COUNT = 0;
	}

	g_Log.Add("[Chaos Castle Survival] [Bug Tracker] MONSTER DEL Count(%d) : OUT - Index(%d)",
		this->m_stCCFData.m_lCCF_CURRENT_MONSTER_COUNT, iMonsterIndex);
}

int ChaosCastleFinal::ReCalcUserCount()
{
	int iPlayUser = this->GetCurPlayUser();
	int iRET_VAL;

	if (iPlayUser <= 0)
	{
		this->m_stCCFData.m_nCCF_ENTERED_USER = 0;
		iRET_VAL = 0;
	}
	else
	{
		this->m_stCCFData.m_nCCF_ENTERED_USER = iPlayUser;
		iRET_VAL = iPlayUser;
	}

	return iRET_VAL;
}

void ChaosCastleFinal::GetRandomDropPos(BYTE & btPosX, BYTE & btPosY)
{
	int nRandValue = 0;

	for (int n = 0; n < 5; n++)
	{
		int nDiffX = g_iCCF_Jewel_Random_Drop[n][4] - btPosX;
		int nDiffY = g_iCCF_Jewel_Random_Drop[n][5] - btPosY;

		int nDiffPos = (nDiffX * nDiffX) - (nDiffY - nDiffY);

		if (nDiffPos <= 0)
			nRandValue = n;
	}

	btPosX = GetLargeRand() % (g_iCCF_Jewel_Random_Drop[nRandValue][2] - g_iCCF_Jewel_Random_Drop[nRandValue][0] + g_iCCF_Jewel_Random_Drop[nRandValue][0]);
	btPosY = GetLargeRand() % (g_iCCF_Jewel_Random_Drop[nRandValue][3] - g_iCCF_Jewel_Random_Drop[nRandValue][1] + g_iCCF_Jewel_Random_Drop[nRandValue][1]);
}

static const BYTE g_byCCFSafetyZoneMapXY[4] = { 23, 75, 44, 108 };
static const BYTE g_byCCFCenterHollowZoneMapXY[2][4] = { 32, 84, 35, 87, 32, 96, 35, 99 };

void ChaosCastleFinal::SendCastleZoneSafetyInfo(bool bDoSet)
{
	char cTEMP_BUF[256];
	PMSG_SETMAPATTR_COUNT * lpMsg = (PMSG_SETMAPATTR_COUNT *)cTEMP_BUF;

	PHeadSetB((LPBYTE)lpMsg, 0x46, sizeof(PMSG_SETMAPATTR_COUNT) + sizeof(PMSG_SETMAPATTR) * 6);
	PMSG_SETMAPATTR * lpMsgBody = (PMSG_SETMAPATTR *)&cTEMP_BUF[7];
	lpMsg->btType = 0;
	lpMsg->btCount = 1;
	lpMsg->btMapAttr = 1;
	(bDoSet) ? (lpMsg->btMapSetType = 0) : (lpMsg->btMapSetType = 1);

	lpMsgBody[0].btX = ::g_byCCFSafetyZoneMapXY[0];
	lpMsgBody[0].btY = ::g_byCCFSafetyZoneMapXY[1];
	lpMsgBody[1].btX = ::g_byCCFSafetyZoneMapXY[2];
	lpMsgBody[1].btY = ::g_byCCFSafetyZoneMapXY[3];

	for (int i = g_ConfigRead.server.GetObjectStartUserIndex(); i < g_ConfigRead.server.GetObjectMax(); i++)
	{
		if (gObj[i].Connected > PLAYER_LOGGED)
		{
			IOCP.DataSend(i, (LPBYTE)lpMsg, lpMsg->h.size);
		}
	}
}

void ChaosCastleFinal::UnSafetyCastleZone()
{
	for (int i = g_byCCFSafetyZoneMapXY[0]; i <= g_byCCFSafetyZoneMapXY[2]; i++)
	{
		for (int j = g_byCCFSafetyZoneMapXY[1]; j <= g_byCCFSafetyZoneMapXY[3]; j++)
		{
			MapC[MAP_INDEX_CHAOSCASTLE_SURVIVAL].m_attrbuf[j * 256 + i] &= ~1;
		}
	}

	for (int iHOLE_NUM = 0; iHOLE_NUM <2; iHOLE_NUM++)
	{
		for (int i = g_byCCFCenterHollowZoneMapXY[iHOLE_NUM][0]; i <= g_byCCFCenterHollowZoneMapXY[iHOLE_NUM][2]; i++)
		{
			for (int j = g_byCCFCenterHollowZoneMapXY[iHOLE_NUM][1]; j <= g_byCCFCenterHollowZoneMapXY[iHOLE_NUM][3]; j++)
			{
				MapC[MAP_INDEX_CHAOSCASTLE_SURVIVAL].m_attrbuf[j * 256 + i] |= 8;
			}
		}
	}
}

void ChaosCastleFinal::SendChaosCastleAnyMsg(LPBYTE lpMsg, int iSize)
{
	for (int i = 0; i < 70; i++)
	{
		if (this->m_stCCFData.m_UserData[i].m_nIndex != -1)
		{
			if (gObjIsConnected(this->m_stCCFData.m_UserData[i].m_nIndex) != FALSE)
			{
				if (gObj[this->m_stCCFData.m_UserData[i].m_nIndex].m_nCCF_UserIndex != -1)
				{
					IOCP.DataSend(this->m_stCCFData.m_UserData[i].m_nIndex, lpMsg, iSize);
				}
			}
		}
	}
}

void ChaosCastleFinal::ClearMonster()
{
	for (int n = 0; n < g_ConfigRead.server.GetObjectMaxMonster(); n++)
	{
		if (gObj[n].MapNumber == MAP_INDEX_CHAOSCASTLE_SURVIVAL)
		{
			gObjDel(n);
		}
	}

	g_Log.Add("[Chaos Castle Survival]  Clear Monster");
}

void ChaosCastleFinal::CheckSync()
{
	if (this->m_vtCCFOpenTime.empty() != false)
	{
		g_Log.AddC(TColor::Red, "[Chaos Castle Survival] No Schedule Time Data - Chaos Castle Survival Terminated (m_vtCCFOpenTime.empty())");
		this->SetState(0, 0);
		return;
	}

	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);
	int iMIN_HOUR = 24;
	int iMIN_MINUTE = 60;
	BOOL bTIME_CHANGED = FALSE;
	std::vector<CCF_START_TIME>::iterator it;

	for (it = this->m_vtCCFOpenTime.begin(); it != this->m_vtCCFOpenTime.end(); it++)
	{
		CCF_START_TIME & stCCTime = *it;

		if ((sysTime.wHour * 60 + sysTime.wMinute) < (stCCTime.m_nHour * 60 + stCCTime.m_nMinute))
		{
			if ((iMIN_HOUR * 60 + iMIN_MINUTE) > (stCCTime.m_nHour * 60 + stCCTime.m_nMinute))
			{
				bTIME_CHANGED = TRUE;
				iMIN_HOUR = stCCTime.m_nHour;
				iMIN_MINUTE = stCCTime.m_nMinute;
			}
		}
	}

	if (bTIME_CHANGED == FALSE)
	{
		it = this->m_vtCCFOpenTime.begin();
		iMIN_HOUR = 24;
		iMIN_MINUTE = 60;

		for (; it != this->m_vtCCFOpenTime.end(); it++)
		{
			CCF_START_TIME & stCCTime = *it;

			if ((iMIN_HOUR * 60 + iMIN_MINUTE) > (stCCTime.m_nHour * 60 + stCCTime.m_nMinute))
			{
				bTIME_CHANGED = 2;
				iMIN_HOUR = stCCTime.m_nHour;
				iMIN_MINUTE = stCCTime.m_nMinute;
			}
		}
	}

	switch (bTIME_CHANGED)
	{
	case TRUE:
		this->m_stCCFData.m_nCCF_REMAIN_MSEC = (((iMIN_HOUR * 60 * 60) + (iMIN_MINUTE * 60)) - ((sysTime.wHour * 60 * 60) + (sysTime.wMinute * 60) + (sysTime.wSecond))) * 1000;
		break;

	case 2:
		this->m_stCCFData.m_nCCF_REMAIN_MSEC = ((((iMIN_HOUR + 24) * 60 * 60) + (iMIN_MINUTE * 60)) - ((sysTime.wHour * 60 * 60) + (sysTime.wMinute * 60) + (sysTime.wSecond))) * 1000;
		break;

	default:
		g_Log.AddC(TColor::Red, "[Chaos Castle Survival] No Schedule Time Data - Chaos Castle Terminated (bTIME_CHANGED=%d)", bTIME_CHANGED);
		this->SetState(0, 0);
		return;

	}

	this->m_stCCFData.m_nCCF_TICK_COUNT = GetTickCount();

	g_Log.Add("[Chaos Castle Survival] Sync Start Time. [%d] min remain (START HOUR:%d, MIN:%d)",
		this->m_stCCFData.m_nCCF_REMAIN_MSEC / 60000, iMIN_HOUR, iMIN_MINUTE);
}

void ChaosCastleFinal::SafetyCastleZone()
{
	for (int i = g_byCCFSafetyZoneMapXY[0]; i <= g_byCCFSafetyZoneMapXY[2]; i++)
	{
		for (int j = g_byCCFSafetyZoneMapXY[1]; j <= g_byCCFSafetyZoneMapXY[3]; j++)
		{
			MapC[MAP_INDEX_CHAOSCASTLE_SURVIVAL].m_attrbuf[j * 256 + i] |= 1;
		}
	}

	for (int iHOLE_NUM = 0; iHOLE_NUM <2; iHOLE_NUM++)
	{
		for (int i = g_byCCFCenterHollowZoneMapXY[iHOLE_NUM][0]; i <= g_byCCFCenterHollowZoneMapXY[iHOLE_NUM][2]; i++)
		{
			for (int j = g_byCCFCenterHollowZoneMapXY[iHOLE_NUM][1]; j <= g_byCCFCenterHollowZoneMapXY[iHOLE_NUM][3]; j++)
			{
				MapC[MAP_INDEX_CHAOSCASTLE_SURVIVAL].m_attrbuf[j * 256 + i] |= 8;
			}
		}
	}
}

static const int g_nCCF_ExpTable[2] = {4000, 5000};

void ChaosCastleFinal::RewardUserEXP(BOOL bWinner)
{
	for (int i = 0; i < 70; i++)
	{
		if (this->m_stCCFData.m_UserData[i].m_nIndex != -1
			&& gObjIsConnected(this->m_stCCFData.m_UserData[i].m_nIndex) == TRUE)
		{
			if (gObj[this->m_stCCFData.m_UserData[i].m_nIndex].m_nCCF_UserIndex == -1)
				return;

			int iKILLCOUNT_USER = gObj[this->m_stCCFData.m_UserData[i].m_nIndex].m_byKillUserCount;
			int iKILLCOUNT_MONSTER = gObj[this->m_stCCFData.m_UserData[i].m_nIndex].m_byKillMobCount;
			int iTOT_EXP = iKILLCOUNT_USER * g_nCCF_ExpTable[0] + iKILLCOUNT_MONSTER * g_nCCF_ExpTable[1];
			int iREWARD_EXP = this->CalcSendRewardEXP(this->m_stCCFData.m_UserData[i].m_nIndex, iTOT_EXP, iKILLCOUNT_USER, iKILLCOUNT_MONSTER);

			g_Log.Add("[Chaos Castle Survival] [%s][%s] Reward User EXP (USER_KILL:%d, MON_KILL:%d, TOT_EXP:%d, TOT_REWARD_EXP:%d)",
				gObj[this->m_stCCFData.m_UserData[i].m_nIndex].AccountID, gObj[this->m_stCCFData.m_UserData[i].m_nIndex].Name,
				iKILLCOUNT_USER, iKILLCOUNT_MONSTER, iTOT_EXP, iREWARD_EXP);
		}
	}
}

int ChaosCastleFinal::ObjSetPosition(int iIndex, int iX, int iY)
{
	if (!ObjectMaxRange(iIndex))
		return TRUE;

	LPOBJ lpObj = &gObj[iIndex];

	if (lpObj->MapNumber != MAP_INDEX_CHAOSCASTLE_SURVIVAL)
		return FALSE;

	if (lpObj->Teleport)
		return TRUE;

	BYTE btMapAttr = MapC[lpObj->MapNumber].GetAttr(iX, iY);

	if ((btMapAttr & 2) == 2 || (btMapAttr & 4) == 4)
		return FALSE;

	PMSG_POSISTION_SET pMove;

	pMove.h.c = 0xC1;
	pMove.h.headcode = 0x15;
	pMove.h.size = sizeof(pMove);
	pMove.X = iX;
	pMove.Y = iY;

	lpObj->m_Rest = 0;

	MapC[lpObj->MapNumber].ClearStandAttr(lpObj->m_OldX, lpObj->m_OldY);
	MapC[lpObj->MapNumber].SetStandAttr(iX, iY);

	lpObj->m_OldX = iX;
	lpObj->m_OldY = iY;

	lpObj->X = pMove.X;
	lpObj->Y = pMove.Y;

	PMSG_RECV_POSISTION_SET pMove2;

	PHeadSetB((LPBYTE)&pMove2, 0x15, sizeof(pMove2));
	pMove2.NumberH = SET_NUMBERH(iIndex);
	pMove2.NumberL = SET_NUMBERL(iIndex);
	pMove2.X = pMove.X;
	pMove2.Y = pMove.Y;
	lpObj->TX = pMove.X;
	lpObj->TY = pMove.Y;

	CreateFrustrum(lpObj->X, lpObj->Y, iIndex);

	if (lpObj->Type == OBJ_USER)
	{
		IOCP.DataSend(iIndex, (UCHAR *)&pMove2, pMove2.h.size);
	}

	GSProtocol.MsgSendV2(&gObj[iIndex], (UCHAR *)&pMove2, pMove2.h.size);

	MapC[lpObj->MapNumber].ClearStandAttr(lpObj->m_OldX, lpObj->m_OldY);
	MapC[lpObj->MapNumber].SetStandAttr(lpObj->TX, lpObj->TY);

	lpObj->m_OldX = lpObj->TX;
	lpObj->m_OldY = lpObj->TY;

	return TRUE;
}

void ChaosCastleFinal::GiveUserDamage(int iUserIndex, int iDamage)
{
	if (!gObjIsConnected(iUserIndex))
		return;

	if (gObj[iUserIndex].Life <= 0.0)
		return;

	PMSG_ATTACKRESULT pResult;

	PHeadSetB((LPBYTE)&pResult, 0x11, sizeof(pResult));
	pResult.NumberH = SET_NUMBERH(iUserIndex);
	pResult.NumberL = SET_NUMBERL(iUserIndex);
	pResult.DamageH = SET_NUMBERH(iDamage);
	pResult.DamageL = SET_NUMBERL(iDamage);
	pResult.btShieldDamageH = 0;
	pResult.btShieldDamageL = 0;
	pResult.IGCDamage = iDamage;
	
	if (gObj[iUserIndex].Type == OBJ_USER)
		IOCP.DataSend(iUserIndex, (LPBYTE)&pResult, pResult.h.size);

	gObj[iUserIndex].Life -= iDamage;

	if (gObj[iUserIndex].Life <= 0.0)
	{
		gObj[iUserIndex].Life = 0;
		gObj[iUserIndex].m_State = 4;
		gObj[iUserIndex].KillerType = 1;
		gObj[iUserIndex].RegenTime = GetTickCount();
		gObj[iUserIndex].DieRegen = 1;
		gObj[iUserIndex].PathCount = 0;

		GSProtocol.GCDiePlayerSend(&gObj[iUserIndex], gObj[iUserIndex].m_Index, 0, 0);
	}
}

void ChaosCastleFinal::SendFailMessage(int iLoserIndex)
{
	if (!ObjectMaxRange(iLoserIndex))
	{
		return;
	}

	if (gObj[iLoserIndex].m_bCCF_Quit_Msg == true)
	{
		return;
	}

	int nPoint = gObj[iLoserIndex].m_byKillMobCount + 2 * gObj[iLoserIndex].m_byKillUserCount;
	this->GD_Req_Save_CCF_Result(iLoserIndex, gObj[iLoserIndex].Name, nPoint, this->Check_CCF_DayType());
	GSProtocol.GCServerMsgStringSend(Lang.GetText(0,559), iLoserIndex, 1);

	PMSG_ANS_CCF_RESULT pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0xAF, 0x09, sizeof(pMsg));
	pMsg.point = nPoint;
	pMsg.result = 2;
	pMsg.type = this->Check_CCF_DayType();
	pMsg.MobKillCnt = gObj[iLoserIndex].m_byKillMobCount;
	pMsg.PCKillCnt = gObj[iLoserIndex].m_byKillUserCount;

	IOCP.DataSend(iLoserIndex, (LPBYTE)&pMsg, pMsg.h.size);
	g_Log.Add("[Chaos Castle Survival][GAME-RESULT] NAME:[%s],MOBKILL:[%d],USERKILL:[%d],POINT[%d]",
		gObj[iLoserIndex].Name, gObj[iLoserIndex].m_byKillMobCount, gObj[iLoserIndex].m_byKillUserCount, nPoint);

	gObj[iLoserIndex].m_bCCF_Quit_Msg = true;
}

int ChaosCastleFinal::CalcSendRewardEXP(int iUserIndex, int iEXP, int iKILLCOUNT_USER, int iKILLCOUNT_MONSTER)
{
	if (iEXP <= 0)
		return 0;

	if (g_CrywolfSync.GetOccupationState() == 1 && g_CrywolfSync.GetApplyPenalty() == TRUE)
	{
		iEXP = iEXP * g_CrywolfSync.GetGettingExpPenaltyRate() / 100;
	}

	UINT64 iRET_EXP = 0;
	UINT64 iCAL_EXP = iEXP;
	int iMAX_LEVCOUNT = 0;

	if (!gObjIsConnected(iUserIndex))
		return 0;

	if (g_MasterLevelSkillTreeSystem.IsMasterLevelUser(&gObj[iUserIndex]) == false)
	{
		if (iCAL_EXP > gObj[iUserIndex].m_PlayerData->Experience + gObj[iUserIndex].m_PlayerData->NextExp)
		{
			iCAL_EXP = gObj[iUserIndex].m_PlayerData->NextExp;
		}
	}
	else
	{
		if (iCAL_EXP > gObj[iUserIndex].m_PlayerData->MasterExperience + gObj[iUserIndex].m_PlayerData->MasterNextExp)
		{
			iCAL_EXP = gObj[iUserIndex].m_PlayerData->MasterNextExp;
		}
	}

	iRET_EXP = iCAL_EXP;

	if (gObj[iUserIndex].Type == OBJ_USER)
	{
		while (iCAL_EXP > 0)
		{
			if (iCAL_EXP > 0)
			{
				CheckItemOptForGetExpExRenewal(&gObj[iUserIndex], NULL, iCAL_EXP, 0, true);

				iCAL_EXP = this->LevelUp(iUserIndex, iCAL_EXP);
			}

			if (iCAL_EXP == 0)
			{
				return iRET_EXP;
			}

			iMAX_LEVCOUNT++;

			if (iMAX_LEVCOUNT > 1000)
				break;
		}

		PMSG_KILLPLAYER_EXT pkillMsg;

		PHeadSetBE((LPBYTE)&pkillMsg, 0x9C, sizeof(pkillMsg));
		pkillMsg.NumberH = -1;
		pkillMsg.NumberL = -1;
		pkillMsg.ExpH = SET_NUMBERHW(iEXP);
		pkillMsg.ExpL = SET_NUMBERLW(iEXP);
		pkillMsg.DamageH = 0;
		pkillMsg.DamageL = 0;

		IOCP.DataSend(iUserIndex, (LPBYTE)&pkillMsg, pkillMsg.h.size);
	}

	return iRET_EXP;
}

BOOL ChaosCastleFinal::LevelUp(int iUserIndex, int iAddExp)
{
	if (ObjectMaxRange(iUserIndex) == FALSE)
	{
		return 0;
	}

	if (g_MasterLevelSkillTreeSystem.IsMasterLevelUser(&gObj[iUserIndex]))
	{
		g_MasterLevelSkillTreeSystem.MasterLevelUp(&gObj[iUserIndex], iAddExp, 0, "Chaos Castle Survival");
		return 0;
	}

	int iLEFT_EXP = 0;

	g_Log.Add("[Chaos Castle Survival] Experience : [%s][%s](%d) Experience: %d + %d",
		gObj[iUserIndex].AccountID, gObj[iUserIndex].Name,
		gObj[iUserIndex].Level, gObj[iUserIndex].m_PlayerData->Experience,
		iAddExp);

	::gObjSetExpPetItem(iUserIndex, iAddExp);

	if (gObj[iUserIndex].Level >= g_ConfigRead.data.common.UserMaxLevel)
	{
		GSProtocol.GCServerMsgStringSend(Lang.GetText(0,45), gObj[iUserIndex].m_Index, 1);
		return 0;
	}

	if ((gObj[iUserIndex].m_PlayerData->Experience + iAddExp) < gObj[iUserIndex].m_PlayerData->NextExp)
	{
		gObj[iUserIndex].m_PlayerData->Experience += iAddExp;
	}
	else
	{
		iLEFT_EXP = gObj[iUserIndex].m_PlayerData->Experience + iAddExp - gObj[iUserIndex].m_PlayerData->NextExp;
		gObj[iUserIndex].m_PlayerData->Experience = gObj[iUserIndex].m_PlayerData->NextExp;
		gObj[iUserIndex].Level++;

		if ( g_ConfigRead.data.reset.iBlockLevelUpPointAfterResets == -1 || gObj[iUserIndex].m_PlayerData->m_iResets < g_ConfigRead.data.reset.iBlockLevelUpPointAfterResets )
		{
			if (gObj[iUserIndex].Class == CLASS_DARKLORD || gObj[iUserIndex].Class == CLASS_MAGUMSA || gObj[iUserIndex].Class == CLASS_RAGEFIGHTER || gObj[iUserIndex].Class == CLASS_GROWLANCER)
			{
				gObj[iUserIndex].m_PlayerData->LevelUpPoint += gLevelUpPointMGDL;
			}

			else
			{
				gObj[iUserIndex].m_PlayerData->LevelUpPoint += gLevelUpPointNormal;
			}

			if (gObj[iUserIndex].m_PlayerData->PlusStatQuestClear != false)
			{
				gObj[iUserIndex].m_PlayerData->LevelUpPoint++;

				g_Log.Add("[%s][%s] LevelUp PlusStatQuest Clear AddStat %d",
					gObj[iUserIndex].AccountID, gObj[iUserIndex].Name, gObj[iUserIndex].m_PlayerData->LevelUpPoint);
			}
		}

		gObj[iUserIndex].MaxLife += DCInfo.DefClass[gObj[iUserIndex].Class].LevelLife;
		gObj[iUserIndex].MaxMana += DCInfo.DefClass[gObj[iUserIndex].Class].LevelMana;
		gObj[iUserIndex].Life = gObj[iUserIndex].MaxLife;
		gObj[iUserIndex].Mana = gObj[iUserIndex].MaxMana;
		gObjNextExpCal(&gObj[iUserIndex]);
		gObjSetBP(iUserIndex);
		GSProtocol.GCLevelUpMsgSend(gObj[iUserIndex].m_Index, 1);
		gObjCalcMaxLifePower(gObj[iUserIndex].m_Index);
		g_Log.Add("Level Up [%s][%s][%d]", gObj[iUserIndex].AccountID, gObj[iUserIndex].Name, gObj[iUserIndex].Level);
		return 0;
	}

	return iLEFT_EXP;
}

int ChaosCastleFinal::CCF_Ranking_DayType()
{
	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);
	int nReturnDayType = 0;

	if (sysTime.wDay != this->m_byTryOut1 && sysTime.wDay != this->m_byTryOut2 && sysTime.wDay != this->m_byTryOut3)
	{
		if (sysTime.wDay == this->m_bySemiFinal1 || sysTime.wDay == this->m_bySemiFinal2 || sysTime.wDay == this->m_byFinal)
		{
			nReturnDayType = 2;
		}
	}

	else
	{
		nReturnDayType = 1;
	}

	return nReturnDayType;
}

int ChaosCastleFinal::Check_CCF_DayType()
{
	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);
	int nReturnDayType = 0;

	if (sysTime.wDay != this->m_byTryOut1 && sysTime.wDay != this->m_byTryOut2 && sysTime.wDay != this->m_byTryOut3)
	{
		if (sysTime.wDay != this->m_bySemiFinal1 && sysTime.wDay != this->m_bySemiFinal2)
		{
			if (sysTime.wDay == this->m_byFinal)
				nReturnDayType = 3;
			else
				nReturnDayType = 0;
		}

		else
		{
			nReturnDayType = 2;
		}
	}

	else
	{
		nReturnDayType = 1;
	}

	return nReturnDayType;
}

bool ChaosCastleFinal::CheckCanStart_CCF()
{
	int nEnteredUserCount;

	nEnteredUserCount = this->GetCurPlayUser();

	if (nEnteredUserCount >= this->m_nCCF_MinUserCount)
	{
		return true;
	}

	else
	{
		g_Log.Add("[Chaos Castle Survival][CheckCanStart_CCS] GetCurPlayUser() FAILED (UserCount:%d) (NeedMinCount:%d)", nEnteredUserCount, this->m_nCCF_MinUserCount);
		return false;
	}
}

static int g_nCCF_EnterFee[3] = { 0, 0, 0 };

void ChaosCastleFinal::CCF_Start_Fail_So_RollBack()
{
	int nCCF_Type = this->Check_CCF_DayType();

	for (int i = 0; i < 70; i++)
	{
		if (this->m_stCCFData.m_UserData[i].m_nIndex != -1 && gObjIsConnected(this->m_stCCFData.m_UserData[i].m_nIndex) == TRUE && gObj[this->m_stCCFData.m_UserData[i].m_nIndex].MapNumber == MAP_INDEX_CHAOSCASTLE_SURVIVAL)
		{
			if (this->m_stCCFData.m_UserData[i].m_nIndex != -1)
			{
				int iFIRST_MONEY = gObj[this->m_stCCFData.m_UserData[i].m_nIndex].m_PlayerData->Money;
				int iPAYBACK_MONEY = g_nCCF_EnterFee[nCCF_Type];
				gObj[this->m_stCCFData.m_UserData[i].m_nIndex].m_PlayerData->Money += iPAYBACK_MONEY;
				int iNOW_MONEY = gObj[this->m_stCCFData.m_UserData[i].m_nIndex].m_PlayerData->Money;

				GSProtocol.GCMoneySend(this->m_stCCFData.m_UserData[i].m_nIndex, gObj[this->m_stCCFData.m_UserData[i].m_nIndex].m_PlayerData->Money);
				gObjMoveGate(this->m_stCCFData.m_UserData[i].m_nIndex, 333);
				g_Log.Add("[Chaos Castle Survival][CCF_Start_Fail_So_RollBack] INDEX: %d ", this->m_stCCFData.m_UserData[i].m_nIndex);

				PMSG_NOTICE pNotice;
				TNotice::MakeNoticeMsgEx(&pNotice, 1, Lang.GetText(0,592));
				IOCP.DataSend(this->m_stCCFData.m_UserData[i].m_nIndex, (LPBYTE)&pNotice, pNotice.h.size);
			}
		}
	}
}

void ChaosCastleFinal::Send_CCF_Schedule(int aIndex)
{
	char sendbuf[4096];
	memset(sendbuf, 0x00, sizeof(sendbuf));

	PMSG_ANS_CCF_TIME pMsgTime;
	PMSG_ANS_CCF_SCHEDULE pMsg;
	int lOfs = sizeof(pMsg);

	PHeadSubSetW((LPBYTE)&pMsg, 0xAF, 0x03, sizeof(pMsg));
	pMsg.byCCFType = this->Check_CCF_DayType();
	pMsg.Tryout1 = this->m_byTryOut1;
	pMsg.Tryout2 = this->m_byTryOut2;
	pMsg.Tryout3 = this->m_byTryOut3;
	pMsg.SemiFinal1 = this->m_bySemiFinal1;
	pMsg.SemiFinal2 = this->m_bySemiFinal2;
	pMsg.Final = this->m_byFinal;

	int nCount = 0;

	for (std::vector<CCF_START_TIME>::iterator it = this->m_vtCCFOpenTime.begin(); it != this->m_vtCCFOpenTime.end(); it++)
	{
		pMsgTime.Hour = it->m_nHour;
		pMsgTime.Min = it->m_nMinute;
		pMsgTime.TryOut = it->m_nTryout;
		pMsgTime.SemiFinal = it->m_nSemiFinal;
		pMsgTime.Final = it->m_nFinal;
		nCount++;

		memcpy(&sendbuf[lOfs], &pMsgTime, sizeof(pMsgTime));
		lOfs += sizeof(pMsgTime);
	}

	pMsg.nCount = nCount;
	pMsg.h.sizeH = HIBYTE(lOfs);
	pMsg.h.sizeL = LOBYTE(lOfs);
	memcpy(&sendbuf, &pMsg, sizeof(pMsg));
	IOCP.DataSend(aIndex, (LPBYTE)sendbuf, lOfs);
}

int ChaosCastleFinal::EnterCheck_CCF_User(int iUserIndex)
{
	for (int i = 0; i < 70; i++)
	{
		if (this->m_stCCFData.m_UserData[i].m_nIndex > 0 && this->m_stCCFData.m_UserData[i].m_nIndex == iUserIndex)
		{
			return -2;
		}

		if (this->m_stCCFData.m_UserData[i].m_nIndex > 0 && this->m_stCCFData.m_nCCF_ENTERED_USER > 70)
		{
			this->m_stCCFData.m_nCCF_ENTERED_USER = 70;
			return -1;
		}
	}

	return 0;
}

BOOL ChaosCastleFinal::CheckUserEnterMoney(int iUserIndex, BYTE byBattleType)
{
	int nEnterFee = g_nCCF_EnterFee[byBattleType];

	if (gObj[iUserIndex].m_PlayerData->Money < nEnterFee)
	{
		g_Log.Add("[Chaos Castle Survival] %s don't have EnterCCF_Fee: %d , User Money: %d ",
			gObj[iUserIndex].Name, nEnterFee, gObj[iUserIndex].m_PlayerData->Money);

		return FALSE;
	}

	return TRUE;
}

void ChaosCastleFinal::GD_Req_Save_CCF_Result(int index, char *Name, int Point, int nCCFtype)
{
	if (ObjectMaxRange(index) == false)
	{
		g_Log.Add("[Chaos Castle Survival][GD_Req_Save_CCF_Result] CHECK_LIMIT index: %d", index);
		return;
	}

	if (gObjIsConnected(index) == false)
	{
		g_Log.Add("[Chaos Castle Survival][GD_Req_Save_CCF_Result] gObjIsConnected is NOT index: %d", index);
		return;
	}

	if (gObj[index].Type != OBJ_USER)
	{
		g_Log.Add("[Chaos Castle Survival][GD_Req_Save_CCF_Result] index is NOT USER: %d", index);
		return;
	}

	if (Point < 0)
	{
		g_Log.Add("[Chaos Castle Survival][GD_Req_Save_CCF_Result] index: %d, Under Zero Point: %d", index, Point);
		return;
	}

	if (Point > 200)
	{
		g_Log.Add("[Chaos Castle Survival][GD_Req_Save_CCF_Result] index: %d, Over 200 Point: %d", index, Point);
		return;
	}

	_tagPMSG_REQ_SAVE_CCF_RESULT pMsg;
	pMsg.szCharName[MAX_CHARNAME_LEN] = 0;
	memcpy(pMsg.szCharName, gObj[index].Name, MAX_ACCOUNT_LEN);
	PHeadSubSetB((LPBYTE)&pMsg, 0xF9, 0xA1, sizeof(pMsg));
	pMsg.nPoint = Point;
	pMsg.nCharClass = gObj[index].Class;
	pMsg.nCharLevel = gObj[index].Level;
	pMsg.nCharExp = gObj[index].m_PlayerData->Experience;

	if (nCCFtype)
	{
		pMsg.byCCFType = nCCFtype;
	}

	else
	{
		pMsg.byCCFType = this->Check_CCF_DayType();
	}

	g_Log.Add("[Chaos Castle Survival][GD_Req_Save_CCF_Result] pMsg.szCharName:%s, obj.Name :%s", pMsg.szCharName, gObj[index].Name);
	wsDataCli.DataSend((char *)&pMsg, pMsg.h.size);
}

void ChaosCastleFinal::GD_Renew_CCF_Ranking(BYTE byCCFType)
{
	SDHP_RENEW_RANKING pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.size = sizeof(pMsg);
	pMsg.h.headcode = 0xF9;
	pMsg.h.subcode = 0xA4;
	pMsg.byRankingType = byCCFType;

	wsDataCli.DataSend((char *)&pMsg, sizeof(pMsg));
}

void ChaosCastleFinal::GD_Load_CCF_RankingList(BYTE byCCFType)
{
	if (byCCFType != 2 && byCCFType != 1)
	{
		g_Log.Add("[Chaos Castle Survival][GD_Load_CCF_RankingList] byCCF_Type : %d ", byCCFType);
		return;
	}

	SDHP_REQ_CCF_RANKING pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.size = sizeof(pMsg);
	pMsg.h.headcode = 0xF9;
	pMsg.h.subcode = 0xA3;
	pMsg.byRankingType = byCCFType;
	pMsg.nServerCategory = g_ConfigRead.server.GetServerType() == SERVER_BATTLECORE ? TRUE : FALSE;

	wsDataCli.DataSend((char *)&pMsg, sizeof(pMsg));
}

void ChaosCastleFinal::GD_Req_Get_Permission(OBJECTSTRUCT *lpObj)
{
	if (ObjectMaxRange(lpObj->m_Index) == false)
	{
		return;
	}

	if (gObjIsConnected(lpObj->m_Index) == false)
	{
		return;
	}

	_tagPMSG_REQ_CCF_PERMISSION pMsg;

	pMsg.szCharName[MAX_ACCOUNT_LEN] = 0;
	memcpy(pMsg.szCharName, lpObj->Name, MAX_ACCOUNT_LEN);
	pMsg.h.c = 0xC1;
	pMsg.h.size = sizeof(pMsg);
	pMsg.h.headcode = 0xF9;
	pMsg.h.subcode = 0xA2;
	pMsg.byCCFType = this->Check_CCF_DayType() - 1;
	pMsg.nIndex = lpObj->m_Index;

	wsDataCli.DataSend((char *)&pMsg, pMsg.h.size);
	g_Log.Add("[Chaos Castle Survival][GD_Req_Get_Permission] NAME:%s,CCFTYPE:%d",
		lpObj->Name, pMsg.byCCFType);
}

BOOL ChaosCastleFinal::PayUserEnterMoney(int iUserIndex, int nCCFType)
{
	if (ObjectMaxRange(iUserIndex) == false)
	{
		return FALSE;
	}

	if (gObj[iUserIndex].m_PlayerData->Money < g_nCCF_EnterFee[nCCFType])
	{
		return FALSE;
	}

	gObj[iUserIndex].m_PlayerData->Money -= g_nCCF_EnterFee[nCCFType];
	GSProtocol.GCMoneySend(iUserIndex, gObj[iUserIndex].m_PlayerData->Money);

	return TRUE;
}

int ChaosCastleFinal::EnterCCF(int iUserIndex, int & nCCFUserIndex)
{
	for (int i = 0; i < 70; i++)
	{
		if (this->m_stCCFData.m_UserData[i].m_nIndex != -1 && this->m_stCCFData.m_UserData[i].m_nIndex == iUserIndex)
			return -2;

		if (this->m_stCCFData.m_UserData[i].m_nIndex == -1)
		{
			if (this->m_stCCFData.m_nCCF_ENTERED_USER <= 70)
			{
				this->m_stCCFData.m_UserData[i].m_nIndex = iUserIndex;
				this->m_stCCFData.m_UserData[i].m_nEXP = 0;
				this->m_stCCFData.m_UserData[i].m_nScore = 0;
				this->m_stCCFData.m_UserData[i].m_nUserState = 0;
				gObj[iUserIndex].m_bCCF_Quit_Msg = false;
				nCCFUserIndex = i;
				this->m_stCCFData.m_nCCF_ENTERED_USER++;

				return 0;
			}

			else
			{
				this->m_stCCFData.m_nCCF_ENTERED_USER = 70;
				return -1;
			}
		}
	}
}

int ChaosCastleFinal::CheckRankingReqValid(BYTE byCCFType)
{
	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);

	if (byCCFType != 1 || sysTime.wDay != this->m_byTryOut1 && sysTime.wDay != this->m_byTryOut2 && sysTime.wDay != this->m_byTryOut3)
	{
		if (byCCFType != 2 || sysTime.wDay != this->m_bySemiFinal1 && sysTime.wDay != this->m_bySemiFinal2 && sysTime.wDay != this->m_byFinal)
		{
			g_Log.Add("[Chaos Castle Survival][CheckRankingReqValid] USER req :%d Today :%d ", byCCFType, sysTime.wDay);
			return -1;
		}

		else
		{
			return 0;
		}
	}

	else
	{
		return 0;
	}
}

void ChaosCastleFinal::DG_Ans_CCF_Rank(_tagPMSG_ANS_CCF_RANK * lpMsg)
{
	if (g_ConfigRead.server.GetServerType() != SERVER_BATTLECORE)
	{
		return;
	}

	int nUserCnt = lpMsg->byUserCnt;
	g_Log.Add("[Chaos Castle Survival] Received ranking, count:%d", nUserCnt);

	for (int i = 0; i < nUserCnt; i++)
	{
		if (lpMsg->RankingInfo[i].byRank <= 0)
		{
			memset(this->m_CCFRanking[i].szCharName, 0x00, MAX_ACCOUNT_LEN + 1);
			this->m_CCFRanking[i].byRank = 0;
			this->m_CCFRanking[i].nPoint = 0;
		}

		else
		{
			memcpy(this->m_CCFRanking[i].szCharName, lpMsg->RankingInfo[i].szCharName, MAX_ACCOUNT_LEN + 1);
			this->m_CCFRanking[i].byRank = lpMsg->RankingInfo[i].byRank;
			this->m_CCFRanking[i].nPoint = lpMsg->RankingInfo[i].nPoint;
			g_Log.Add("[Chaos Castle Survival] (%d) rank: (Name:%s) (Point:%d)",
				this->m_CCFRanking[i].byRank, this->m_CCFRanking[i].szCharName, this->m_CCFRanking[i].nPoint);
		}
	}

	if (this->m_FirstRankingInfoLoad == false)
	{
		this->m_FirstRankingInfoLoad = true;
	}
}

void ChaosCastleFinal::Send_RankingInfo(BYTE byCCFType, int aIndex)
{
	_stPMSG_CCF_RANK_INFO pMsg;
	_CCFRankingInfo pRankInfo;
	char sendbuf[4096];

	PHeadSubSetW((LPBYTE)&pMsg, 0xAF, 0x07, sizeof(pMsg));
	memset(sendbuf, 0x00, sizeof(sendbuf));
	int lOfs = sizeof(pMsg);
	int nUserCnt = 0;

	int CanIsSeeRankingInfo = this->CheckRankingReqValid(byCCFType);

	if (CanIsSeeRankingInfo == FALSE)
	{
		for (int i = 0; i < 50; i++)
		{
			if (this->m_CCFRanking[i].byRank > 0)
			{
				memcpy(pRankInfo.szCharName, this->m_CCFRanking[i].szCharName, MAX_ACCOUNT_LEN + 1);
				pRankInfo.byRank = this->m_CCFRanking[i].byRank;
				pRankInfo.nPoint = this->m_CCFRanking[i].nPoint;
				memcpy(&sendbuf[lOfs], &pRankInfo, sizeof(pRankInfo));
				lOfs += sizeof(pRankInfo);
				nUserCnt++;
			}
		}
	}

	if (nUserCnt || CanIsSeeRankingInfo == -1)
	{
		if (CanIsSeeRankingInfo == -1)
		{
			pMsg.btResult = 2;
			g_Log.Add("[Chaos Castle Survival][Send_RangkingInfo][ERROR] CANNOT SEE RANKING==> CanIseeRankInfo:%d, nUserCnt:%d ", -1, nUserCnt);
		}

		else
		{
			pMsg.btResult = 0;
			g_Log.Add("[Chaos Castle Survival][Send_RangkingInfo] RankingInfo ==> CanIseeRankInfo:%d, nUserCnt:%d ", CanIsSeeRankingInfo, nUserCnt);
		}
	}

	else
	{
		pMsg.btResult = 1;
		g_Log.Add("[Chaos Castle Survival][Send_RangkingInfo][ERROR] No RankingInfo ==> CanIseeRankInfo:%d, nUserCnt:%d ", CanIsSeeRankingInfo, nUserCnt);
	}

	if (this->Check_CCF_DayType() != 3 || this->m_stCCFData.m_bCCF_PLAY_START != true)
	{
		if (this->m_bFinalDayEnd == true)
		{
			pMsg.btResult = 2;
			g_Log.Add("[Chaos Castle Survival][Send_RangkingInfo][ERROR] FINAL WAR END");
		}
	}
	else
	{
		pMsg.btResult = 2;
		g_Log.Add("[Chaos Castle Survival][Send_RangkingInfo][ERROR] NOW FINAL FIGHT");
	}

	pMsg.btCnt = nUserCnt;
	pMsg.h.sizeH = HIBYTE(lOfs);
	pMsg.h.sizeL = LOBYTE(lOfs);
	memcpy(&sendbuf, &pMsg, sizeof(pMsg));
	IOCP.DataSend(aIndex, (LPBYTE)sendbuf, lOfs);
}

void ChaosCastleFinal::GiveCCFWinnerItem(int iWinnerIndex, BYTE byCCFType, BYTE byRewardType)
{
	if (!gObjIsConnected(iWinnerIndex))
	{
		return;
	}

	if (g_ConfigRead.server.GetServerType() == SERVER_BATTLECORE)
	{
		this->GDReqSetCCFReward_UBF(iWinnerIndex, byCCFType, 1);
		return;
	}

	_stGremoryCaseItem stItem;
	WORD wItemType = -1;

	if (byCCFType == 1) // Tryout
	{
		if (byRewardType == 0) // Monster Drop
		{
			wItemType = ITEMGET(14,254);
		}

		else if (byRewardType == 1) // Winner Prize
		{
			wItemType = ITEMGET(14,256);
		}
	}

	else if (byCCFType == 2) // Semi-Final
	{
		if (byRewardType == 0) // Monster Drop
		{
			wItemType = ITEMGET(14,256);
		}

		else if (byRewardType == 1) // Winner Prize
		{
			wItemType = ITEMGET(14,257);
		}
	}

	else if (byCCFType == 3) // Final
	{
		if (byRewardType == 0) // Monster Drop
		{
			wItemType = ITEMGET(14,257);
		}

		else if (byRewardType == 1) // Winner Prize
		{
			wItemType = ITEMGET(13,276);
		}
	}

	stItem.btStorageType = GC_STORAGE_CHARACTER;
	stItem.btRewardSource = GC_REWARD_CHAOS_CASTLE_SURVIVAL;
	stItem.wItemID = wItemType;

	g_GremoryCase.GDReqAddItemToGremoryCase(iWinnerIndex, stItem, 30);
}

void ChaosCastleFinal::SendNoticeMessage(char* lpszMSG)
{
	PMSG_NOTICE pNotice;
	TNotice::MakeNoticeMsgEx(&pNotice, 0, lpszMSG);

	for (int i = 0; i < 70; i++)
	{
		if (this->m_stCCFData.m_UserData[i].m_nIndex != -1)
		{
			if (gObj[this->m_stCCFData.m_UserData[i].m_nIndex].Connected > PLAYER_LOGGED)
			{
				IOCP.DataSend(this->m_stCCFData.m_UserData[i].m_nIndex, (LPBYTE)&pNotice, pNotice.h.size);
			}
		}
	}
}

void ChaosCastleFinal::GD_SendCCFInfoAllServer(int nMinute, int nType)
{
	PMSG_SEND_CCF_INFO_ALL_SVR pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.size = sizeof(pMsg);
	pMsg.h.headcode = 0xF9;
	pMsg.h.subcode = 0xA5;
	pMsg.nMin = nMinute;
	pMsg.nType = nType;
	pMsg.wMapSvrNum = g_MapServerManager.GetMapSvrGroup();

	wsDataCli.DataSend((char*)&pMsg, sizeof(pMsg));
	g_Log.Add("[CCS][GD_SendCCFInfoAllServer] Send Notify To DATA svr TYPE:%d, SVR_GRP %d", nType, pMsg.wMapSvrNum);
}

void ChaosCastleFinal::SendAllMemberOfCCF(int index)
{
	if (ObjectMaxRange(index) == false)
	{
		return;
	}

	if (gObjIsConnected(index) == false)
	{
		return;
	}

	for (int i = 0; i < 70; i++)
	{
		if (this->m_stCCFData.m_UserData[i].m_nIndex != -1 && gObj[this->m_stCCFData.m_UserData[i].m_nIndex].Connected > PLAYER_LOGGED)
		{
			LPOBJ lpObj = &gObj[this->m_stCCFData.m_UserData[i].m_nIndex];

			if (lpObj)
			{
				char szMsg[128];
				wsprintf(szMsg, "[CCF Entry]-> No.%d, Name:%s, Level:%d, MasterLevel:%d, Class%d", i, lpObj->Name, lpObj->Level, lpObj->m_PlayerData->MasterLevel, lpObj->Class);
				GSProtocol.GCServerMsgStringSend(szMsg, index, 1);
			}
		}
	}
}

void ChaosCastleFinal::MakeChaosCastleRewardWing(int iUserIndex)
{
	ItemSerialCreateSend(iUserIndex, gObj[iUserIndex].MapNumber, gObj[iUserIndex].X, gObj[iUserIndex].Y, ITEMGET(this->m_stCCFFinalReward.m_btCat, this->m_stCCFFinalReward.m_wIndex), this->m_stCCFFinalReward.m_btLevel, this->m_stCCFFinalReward.m_btDurability, this->m_stCCFFinalReward.m_btSkill, this->m_stCCFFinalReward.m_btLuck, this->m_stCCFFinalReward.m_btOption, iUserIndex, this->m_stCCFFinalReward.m_btNewOption, 0, this->m_stCCFFinalReward.m_dwExpiryTime, 0, 0);
}

bool ChaosCastleFinal::IsRealFinalDayEnd()
{
	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);

	for (std::vector<CCF_START_TIME>::iterator it = this->m_vtCCFOpenTime.begin(); it != this->m_vtCCFOpenTime.end(); it++)
	{
		if (it->m_nFinal == 3 && sysTime.wMinute + 60 * sysTime.wHour > it->m_nMinute + 60 * it->m_nHour)
		{
			g_Log.Add("[Chaos Castle Survival][RealFinalDayEnd] SystemTime %dh:%dm,FinalTime %dh:%dm",
				sysTime.wHour, sysTime.wMinute, it->m_nHour, it->m_nMinute);

			return true;
		}
	}

	return false;
}

void ChaosCastleFinal::GDReqSetCCFReward_UBF(int iUserIndex, BYTE btCCFType, BYTE btRewardType)
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

	g_Log.Add("[UBF][ChaosCastleFinal][GDReqSetCCFWinerInformation]ACC:%s , NAME :%s,Real NAME :%s, CCFType:%d, RewardType:%d",
		lpObj->AccountID, lpObj->Name, lpObj->m_PlayerData->m_RealNameOfUBF, btCCFType, btRewardType);

	SDHP_REQ_SET_CCF_WINNER_INFO pMsg;
	
	memcpy(pMsg.UBFName, lpObj->Name, MAX_ACCOUNT_LEN+1);
	pMsg.btCCFType = btCCFType;
	pMsg.btRewardType = btRewardType;

	PHeadSubSetB((LPBYTE)&pMsg, 0xF9, 0xB0, sizeof(pMsg));
	wsDataCli.DataSend((char *)&pMsg, pMsg.h.size);
}

void ChaosCastleFinal::SetOpenTodaySchedule()
{
	BYTE byToday = this->Check_CCF_DayType();
	BYTE byCount = 0;

	this->m_vtCCFOpenTime.clear();

	if (byToday != 1 && byToday != 2 && byToday != 3)
	{
		g_Log.Add("[ChaosCastleFinal][SetOpenTodaySchedule] No related ChaosCastleFinal Day, Count:[%d]", byCount);
		return;
	}

	for (std::vector<CCF_START_TIME>::iterator It = this->m_vtCCFTime.begin(); It != this->m_vtCCFTime.end(); It++)
	{
		CCF_START_TIME & pRET = *It;

		if (pRET.m_nTryout != byToday && pRET.m_nSemiFinal != byToday && pRET.m_nFinal != byToday)
		{
			g_Log.Add("[ChaosCastleFinal][SetOpenTodaySchedule] NO DayType:[%d], Hour:[%d],minute[%d],Count:[%d] ",
				byToday, pRET.m_nHour, pRET.m_nMinute, byCount);
		}

		else
		{
			this->m_vtCCFOpenTime.push_back(pRET);

			g_Log.Add("[ChaosCastleFinal][SetOpenTodaySchedule] DayType:[%d], Hour:[%d],minute[%d],Count:[%d] ",
				byToday, pRET.m_nHour, pRET.m_nMinute, byCount);
			byCount++;
		}
	}
}

void ChaosCastleFinal::SetUBFGetReward(int iUserIndex, WORD wItemCode, UINT64 ItemSerial, BYTE btItemPos)
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

	if (wItemCode != ITEMGET(14,254) && wItemCode != ITEMGET(14,256) && wItemCode != ITEMGET(14,257))
	{
		return;
	}

	gObjInventoryDeleteItem(iUserIndex, btItemPos);
	GSProtocol.GCInventoryItemDeleteSend(iUserIndex, btItemPos, 0);

	this->GDReqSetCCFReward_UBF(iUserIndex, this->Check_CCF_DayType(), 0);
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


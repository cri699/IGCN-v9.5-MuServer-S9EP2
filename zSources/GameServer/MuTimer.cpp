//////////////////////////////////////////////////////////////////////
// MuTimer.cpp
#include "stdafx.h"
#include "MuTimer.h"
#include "GameMain.h"
#include "GameServer.h"
#include "EDSProtocol.h"
#include "NewPVP.h"
#include "DoppelGanger.h"
#include "BloodCastle.h"
#include "DevilSquare.h"
#include "ChaosCastle.h"
#include "IllusionTempleEvent_Renewal.h"
#include "Kanturu.h"
#include "MonsterRegenSystem.h"
#include "ChaosCastleFinal.h"
#include "DevilSquareFinal.h"
#include "ImperialGuardian.h"
#include "Weapon.h"
#include "Raklion.h"
#include "CashShop.h"
#include "NewsSystem.h"
#include "LastManStanding.h"
#include "PeriodItemEx.h"
#include "ArcaBattle.h"
#include "AcheronGuardianEvent.h"
#include "OfflineLevelling.h"
#include "TMonsterAI.h"
#include "EledoradoEvent.h"
#include "EventManagement.h"
#include "VipSys.h"
#include "ConMember.h"
#include "GensSystem.h"
#include "CastleSiegeSync.h"
#include "PersonalStore.h"
#include "EventMonSetBase.h"
#include "DSProtocol.h"
#include "configread.h"

CMuTimer g_MuTimer;

CMuTimer::CMuTimer()
{
	timeBeginPeriod(1);
	this->m_bTimerExpire = 0;

	this->m_MuTimer1.RegCallBack((WAITORTIMERCALLBACK)TimerProcQueue);
	this->m_MuTimer2.RegCallBack((WAITORTIMERCALLBACK)TimerProcQueue2);
}

CMuTimer::~CMuTimer()
{
	timeEndPeriod(1);
}

void CMuTimer::SetMuTimer()
{
	this->m_MuTimer1.CreateTimer(WM_SET_DATE, 60000, 0x00010000 | WT_EXECUTEINTIMERTHREAD);
	this->m_MuTimer1.CreateTimer(WM_LOG_DATE_CHANGE, 60000, 0x00010000 | WT_EXECUTEINTIMERTHREAD);
	this->m_MuTimer1.CreateTimer(WM_MATH_AUTHEN, 10000, 0x00010000 | WT_EXECUTEINTIMERTHREAD);
	this->m_MuTimer1.CreateTimer(WM_GS_CLOSE, 1000, 0x00010000 | WT_EXECUTEINTIMERTHREAD);
	this->m_MuTimer1.CreateTimer(WM_MONSTER_AND_MSG_PROC, 500, 0x00010000 | WT_EXECUTEINTIMERTHREAD);
	this->m_MuTimer1.CreateTimer(WM_MOVE_MONSTER_PROC, 300, 0x00010000 | WT_EXECUTEINTIMERTHREAD);
	this->m_MuTimer1.CreateTimer(WM_EVENT_RUN_PROC, 100, 0x00010000 | WT_EXECUTEINTIMERTHREAD);
	this->m_MuTimer1.CreateTimer(WM_AI_MONSTER_MOVE_PROC, 100, 0x00010000 | WT_EXECUTEINTIMERTHREAD);
	this->m_MuTimer1.CreateTimer(WM_AI_MONSTER_PROC, 500, 0x00010000 | WT_EXECUTEINTIMERTHREAD);
	this->m_MuTimer1.CreateTimer(WM_GUILD_LIST_REQUEST, 0, 0x00010000 | WT_EXECUTEDELETEWAIT);
	this->m_MuTimer1.CreateTimer(WM_SECOND_MSG_PROCESS, 1000, 0x00010000 | WT_EXECUTEINTIMERTHREAD);
	this->m_MuTimer1.CreateTimer(WM_BATTLECORE_PROCESS, 180000, 0x00010000 | WT_EXECUTEINTIMERTHREAD);
	this->m_MuTimer1.CreateTimer(WM_EVENTSETBASE_PROCESS, 60000, 0x00010000 | WT_EXECUTEINTIMERTHREAD);

	this->m_MuTimer2.CreateTimer(WM_FIRST_MSG_PROCESS, 1000, 0x00010000 | WT_EXECUTEINIOTHREAD);
	this->m_MuTimer2.CreateTimer(WM_VIEWPORT_PROCESS, 1000, 0x00010000 | WT_EXECUTEINIOTHREAD);
}

VOID WINAPI TimerProcQueue(LPVOID lpParameter, BOOL TimerOrWaitFired)
{
	if (g_MuTimer.GetTimerEnd() == TRUE)
	{
		return;
	}

	g_MuTimer.m_MuTimer1.SyncLock();

	int nEventID = (int)lpParameter;

	switch (nEventID)
	{
		case WM_GUILD_LIST_REQUEST:
			DGGuildListRequest();
			break;
		case WM_MONSTER_AND_MSG_PROC:
			MonsterAndMsgProc();
			g_NewPVP.Run();
			break;
		case WM_MOVE_MONSTER_PROC:
			MoveMonsterProc();
			break;
		case WM_EVENT_RUN_PROC:
			g_DoppelGanger.MoveDoppelgangerMonsterProc();
			g_BloodCastle.Run();
			g_DevilSquare.Run();
			g_RingAttackEvent.Run();
			g_XMasAttackEvent.Run();
			g_ChaosCastle.Run();

			if (g_ConfigRead.server.GetServerType() == SERVER_MARKET)
			{
				g_IT_Event.ITR_Run();
			}

			if (g_ConfigRead.server.GetServerType() == SERVER_CASTLE)
			{
				g_CastleSiege.Run();
				g_CastleSiege.CastleLordMixRun();
				g_CastleDeepEvent.Run();
				g_Crywolf.Run();
			}

			else
			{
				g_Kanturu.Run();
			}

			if (g_ConfigRead.server.GetServerType() == SERVER_NORMAL || g_ConfigRead.server.GetServerType() == SERVER_CASTLE)
			{
				g_MonsterRegenSystem.Run();
			}

			if (g_ConfigRead.server.GetServerType() == SERVER_BATTLECORE)
			{
				g_ChaosCastleFinal.Run();
				g_DevilSquareFinal.DSF_Run();
			}

			g_DoppelGanger.ProcDoppelganger(GetTickCount64());
			g_ImperialGuardian.Run();

			g_CsNPC_Weapon.WeaponAttackProc();

			g_Raklion.Run();
			g_CashShop.Run();
			g_NewsSystem.Run();
			g_LastManStanding.Run();
			g_PeriodItemEx.CheckPeriodData();
			g_GremoryCase.CheckStorageExpiredItems();
			g_ArcaBattle.Run();
			g_AcheronGuardianEvent.Run();
			g_OffLevel.Run();
			break;
		case WM_AI_MONSTER_MOVE_PROC:
			TMonsterAI::MonsterMoveProc();
			break;
		case WM_AI_MONSTER_PROC:
			TMonsterAI::MonsterAIProc();
			break;
		case WM_SECOND_MSG_PROCESS:
			for (int n = 0; n<MAX_NUMBER_MAP; n++)
			{
				MapC[n].WeatherVariationProcess();
			}
			wsGServer.m_SendSec = wsGServer.m_SendSecTmp;
			wsGServer.m_RecvSec = wsGServer.m_RecvSecTmp;
			wsGServer.m_SendSecTmp = 0;
			wsGServer.m_RecvSecTmp = 0;
			DragonEvent->Run();
			AttackEvent->Run();
			gEledoradoEvent.Run();
			g_EventManager.Run();
			g_VipSystem.Run();
			GDReqMapSrvGroupServerCount();
			break;
		case WM_GS_CLOSE:
			if (gCloseMsg != 0)
			{
				gCloseMsgTime--;

				if (gCloseMsgTime <= 1)
				{
					if (gCloseMsgTime == 1)
					{
						GSProtocol.AllSendServerMsg(Lang.GetText(0,1));
						g_Log.AddC(TColor::Red, "GameServer closed");
					}
				}
				else
				{
					if ((gCloseMsgTime % 10) == 0)
					{
						char szTemp[256];

						wsprintf(szTemp, Lang.GetText(0,2), gCloseMsgTime);
						GSProtocol.AllSendServerMsg(szTemp);
						g_Log.AddC(TColor::Red, "Server close automatically in %d second(s)", gCloseMsgTime);
					}
				}

				if (gCloseMsgTime < 0)
				{
					gObjAllLogOut();
					gCloseMsgTime = 0;
					gCloseMsg = 0;
				}
			}
			gObjSecondProc();
			ConMember.Run();

			if (g_GensSystem.GetAbusingResetTime() > 0)
			{
				for (int i = g_ConfigRead.server.GetObjectStartUserIndex(); i<g_ConfigRead.server.GetObjectMax(); i++)
				{
					if (gObjIsConnected(i))
					{
						g_GensSystem.AbusingInfoReset(&gObj[i]);
					}
				}
			}

			if (wsDataCli.GetConnect() == TRUE && g_ArcaBattle.IsDataLoadWinGuild() == FALSE)
			{
				g_ArcaBattle.GDReqWinGuildInfo();
			}

			if (wsDataCli.GetConnect() == TRUE && g_ArcaBattle.IsDataLoadProcState() == FALSE)
			{
				g_ArcaBattle.GDReqProcState();
			}

			if (wsDataCli.GetConnect() == TRUE && g_ArcaBattle.IsInitABProcMultiCast() == FALSE && g_ArcaBattle.IsArcaBattleServer() == TRUE)
			{
				g_ArcaBattle.SetArcaBattleProcMultiCast(1);
			}

			if (wsDataCli.GetConnect() == TRUE && g_AcheronGuardianEvent.IsInitAEProcMultiCast() == FALSE && g_ArcaBattle.IsArcaBattleServer() == TRUE)
			{
				g_AcheronGuardianEvent.SetAcheronEventProcMultiCast(1);
			}

			if (g_ConfigRead.server.GetServerType() == SERVER_CASTLE)
			{
				if (wsDataCli.GetConnect() == TRUE && g_CastleSiege.GetDataLoadState() == 2)
				{
					g_CastleSiege.DataRequest();
				}

				g_CastleSiege.SendCastleStateSync();
			}

			g_CastleSiegeSync.AdjustTributeMoney();

			if (g_ConfigRead.server.GetServerType() == SERVER_CASTLE)
			{
				g_Crywolf.CrywolfSecondAct();
			}

			break;
		case WM_SET_DATE:
			gSetDate();
			WhisperCash.TimeCheckCashDelete();
			break;
		case WM_MATH_AUTHEN:
			{
				if (g_ConfigRead.server.GetServerType() == SERVER_BATTLECORE)
				{
					if (wsDataCli.GetConnect() == TRUE)
					{
						BYTE CCFDay = g_ChaosCastleFinal.Check_CCF_DayType();

						if (ChaosCastleFinal::m_bCCFDayChange == true && g_ChaosCastleFinal.m_bCCF_EVENT_ENABLE == false && (CCFDay == 1 || CCFDay == 2 || CCFDay == 3))
						{
							ChaosCastleFinal::m_byCCFTimer++;

							if (ChaosCastleFinal::m_byCCFTimer == 10)
							{
								g_ChaosCastleFinal.m_bCCF_EVENT_ENABLE = true;
								ChaosCastleFinal::m_bCCFDayChange = false;
								ChaosCastleFinal::m_byCCFTimer = 0;

								g_Log.Add("[ChaosCastleFinal][End of Day] Resetting Event...");
								g_ChaosCastleFinal.Init();
							}
						}
					}
				}
			}
			break;
		case WM_BATTLECORE_PROCESS:
			if (g_ConfigRead.server.GetServerType() == SERVER_BATTLECORE)
			{
				g_UnityBattleField.GDObserverLogoutManager();
			}
			break;
		case WM_LOG_DATE_CHANGE:
			if (g_Log.LogDateChange() == TRUE)
			{
				if (g_ConfigRead.server.GetServerType() == SERVER_BATTLECORE)
				{
					g_ChaosCastleFinal.GD_Renew_CCF_Ranking(g_ChaosCastleFinal.CCF_Ranking_DayType());
					g_ChaosCastleFinal.m_FirstRankingInfoLoad = false;

					if (g_ChaosCastleFinal.m_bFinalDayEnd == true)
					{
						g_ChaosCastleFinal.m_bFinalDayEnd = false;
					}

					g_ChaosCastleFinal.m_bCCF_EVENT_ENABLE = false;
					ChaosCastleFinal::m_bCCFDayChange = true;

					g_Log.Add("[ChaosCastleFinal][End of Day] Preparing for reset.");

					if (g_DevilSquareFinal.WhatIsTodayTypeOfDSF() == 0)
					{
						g_Log.Add("[DSF] Not a DSF Day - disable Event");
						g_DevilSquareFinal.m_bEVENT_ENABLE = false;
					}

					else
					{
						g_Log.Add("[DSF] DSF Day Found - enable Event");
						g_DevilSquareFinal.m_bEVENT_ENABLE = true;
					}
				}
			}
			break;
		case WM_EVENTSETBASE_PROCESS:
			gEventMSetBase.Run();
			break;
		default:
			break;
	}

	g_MuTimer.m_MuTimer1.SyncUnlock();
}

VOID WINAPI TimerProcQueue2(LPVOID lpParameter, BOOL TimerOrWaitFired)
{
	if (g_MuTimer.GetTimerEnd() == TRUE)
	{
		return;
	}

	g_MuTimer.m_MuTimer2.SyncLock();

	int nEventID = (int)lpParameter;

	switch (nEventID)
	{
		case WM_FIRST_MSG_PROCESS:
			if (gDoPShopOpen != FALSE)
			{
				for (int n =/* ObjectStartUserIndex*/ 0; n < g_ConfigRead.server.GetObjectMax(); n++)
				{
					g_PersonalStore.PShop_ViewportListRegenarate(n);
				}

				for (int n = /*ObjectStartUserIndex*/ 0; n < g_ConfigRead.server.GetObjectMax(); n++)
				{
					if (gObjIsConnected(n) != PLAYER_EMPTY)
					{
						gObj[n].m_bPShopItemChange = false;
					}
				}
			}

			for (int n = g_ConfigRead.server.GetObjectStartUserIndex(); n < g_ConfigRead.server.GetObjectMax(); n++)
			{
				gObjUnionUpdateProc(n);
			}
			break;
		case WM_VIEWPORT_PROCESS:
			for (int n = 0; n < g_ConfigRead.server.GetObjectMax(); n++)
			{
				gObjStateSetCreate(n);
			}
			for (int n = 0; n < g_ConfigRead.server.GetObjectMax(); n++)
			{
				gObjViewportListDestroy(n);
			}
			for (int n = 0; n < g_ConfigRead.server.GetObjectMax(); n++)
			{
				gObjViewportListCreate(n);
			}
			for (int n = 0; n < g_ConfigRead.server.GetObjectMax(); n++)
			{
				gObjViewportListProtocol(n);
			}

			gObjSetState();
			break;

		default:
			break;
	}

	g_MuTimer.m_MuTimer2.SyncUnlock();
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


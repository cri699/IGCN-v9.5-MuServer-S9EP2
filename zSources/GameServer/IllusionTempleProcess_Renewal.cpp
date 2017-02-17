//////////////////////////////////////////////////////////////////////
// IllusionTempleProcess_Renewal.cpp
#include "stdafx.h"
#include "IllusionTempleProcess_Renewal.h"
#include "IllusionTempleEvent_Renewal.h"
#include "PartyClass.h"
#include "GameMain.h"
#include "TLog.h"
#include "winutil.h"
#include "SendNPCInfo.h"
#include "user.h"
#include "protocol.h"
#include "BuffEffectSlot.h"
#include "TNotice.h"
#include "DSProtocol.h"
#include "MapServerManager.h"
#include "QuestExpProgMng.h"
#include "CashShop.h"
#include "configread.h"

CIllusionTempleProcess_Renewal::CIllusionTempleProcess_Renewal()
{
	this->m_nTempleNumber = -1;
	this->m_btIllusionTempleState = 0;
	this->m_byITRRound = 1;
	this->m_bITRRoundStartFail = false;
	this->m_bNobodyWinLose = false;
	this->m_nNextTimeMin = 0;
	this->m_nRoundBreakCount = 0;
	this->m_bRoundBreak = false;
	this->m_byNpcCount = 0;
	this->m_bIsSetNpc = false;
	this->m_bFirstRegen_CursedStone = false;
	this->m_bFirstRegen_OccStone = false;

	InitializeCriticalSection(&this->m_critUserData);
	this->m_nRemainTimeMsec = -1;
	this->ResetITRPosNum();
}

CIllusionTempleProcess_Renewal::~CIllusionTempleProcess_Renewal()
{
	DeleteCriticalSection(&this->m_critUserData);
}

void CIllusionTempleProcess_Renewal::ITR_ProcessInit(int nTempleNum)
{
	if (this->m_nTempleNumber == -1)
	{
		this->m_nTempleNumber = nTempleNum;
	}

	for (int i = 0; i < 10; i++)
	{
		this->ClearUserData(i);
	}

	this->m_nTickCount = GetTickCount();
	this->m_nAlliedUserCount = 0;
	this->m_nIllusionUserCount = 0;
	this->m_StatusRegenTime = -1;
	this->m_btWinTeam = -1;
	this->m_btLoseTeam = -1;
	this->m_bEntrance = FALSE;
	this->m_nRemainTimeMsec = -1;
	this->m_bGameEndPosMove = false;
	this->m_bSendMsgBeforeReady = false;
	this->m_bSendMsgBeforePlay = false;
	this->m_nEventEndRemainTime = 0;
	this->CheckSync();
	this->m_bAllCheckSync = false;
	this->m_bSendResult = false;
	this->m_bNobodyWinLose = false;
	this->m_byITRRound = 1;
	this->m_byIllusionTeamPoint = 0;
	this->m_byAlliedTeamPoint = 0;

	for (int i = 0; i < 2; i++)
	{
		if (this->m_PartyTeam[i].nPartyIndex > -1)
		{
			gParty.ClearPartyAuthITR(this->m_PartyTeam[i].nPartyIndex);
		}
	}

	for (int i = 0; i < 2; i++)
	{
		this->m_PartyTeam[i].nPartyIndex = -1;
		this->m_PartyTeam[i].nTeamIndex = -1;
	}
}

void CIllusionTempleProcess_Renewal::ClearUserData(int nIndex)
{
	if (!ObjectMaxRange(nIndex))
	{
		g_Log.Add("return %s %d", __FILE__, __LINE__);
		return;
	}

	this->m_UserData[nIndex].m_dwSkillProdectionTime = 0;
	this->m_UserData[nIndex].m_dwSkillRestraintTime = 0;
	this->m_UserData[nIndex].m_nIndex = -1;
	this->m_UserData[nIndex].m_btTeam = -1;
	this->m_UserData[nIndex].m_btUserKillCount = 0;
	this->m_UserData[nIndex].m_bReward = 0;
	this->m_UserData[nIndex].m_bEnterOk = 0;
	this->m_UserData[nIndex].m_nPartyIdx = -1;
	this->m_UserData[nIndex].m_nRelicsInvenPos = -1;
	this->m_UserData[nIndex].m_byInvalidationCount = 0;
	this->m_UserData[nIndex].m_byOccupiedCount = 0;
}

void CIllusionTempleProcess_Renewal::CheckSync()
{
	std::list<_ITR_STARTTIME>::iterator iter;

	_ITR_STARTTIME prevStartTime;
	_ITR_STARTTIME nextStartTime;

	DWORD dwPrevTime = 0;
	DWORD dwNextTime = 0;
	DWORD dwCurTime = 0;

	if (g_IT_Event.m_lstALLITR_TIME.size() == 0)
	{
		g_Log.MsgBox("Error : Illusion Temple Renewal StartTime size is 0");
		return;
	}

	tm* today;
	time_t ltime;

	time(&ltime);
	today = localtime(&ltime);

	dwCurTime = (today->tm_hour * 60) + today->tm_min;
	prevStartTime = *g_IT_Event.m_lstALLITR_TIME.begin();

	for (iter = g_IT_Event.m_lstALLITR_TIME.begin(); iter != g_IT_Event.m_lstALLITR_TIME.end(); ++iter)
	{
		nextStartTime = *iter;

		dwPrevTime = (prevStartTime.nHour * 60) + prevStartTime.nMin;
		dwNextTime = (nextStartTime.nHour * 60) + nextStartTime.nMin;

		if (dwPrevTime == dwNextTime)
		{
			if (dwCurTime < dwNextTime)
			{
				nextStartTime = *iter;
				break;
			}
			continue;
		}

		if (dwCurTime >= dwPrevTime &&
			dwCurTime < dwNextTime)
		{
			break;
		}

		prevStartTime = *iter;
	}

	int Cnt = 2;

	while (Cnt-- != 0)
	{
		if (iter == g_IT_Event.m_lstALLITR_TIME.end())
		{
			iter = g_IT_Event.m_lstALLITR_TIME.begin();

			nextStartTime = (*iter);
		}

		dwNextTime = nextStartTime.nHour * 60 + nextStartTime.nMin;

		if (today->tm_hour <= nextStartTime.nHour)
		{
			m_nRemainTimeMin = dwNextTime - dwCurTime;
		}
		else
		{
			m_nRemainTimeMin = ((24 * 60) - dwCurTime) + dwNextTime;
		}

		if (m_nRemainTimeMin <= g_IT_Event.m_nITROpenTime)
		{
			iter++;

			if (iter != g_IT_Event.m_lstALLITR_TIME.end())
			{
				nextStartTime = (*iter);
			}
		}
		else
		{
			break;
		}
	}

	int tmpTime = GetTickCount();
	this->m_nNextTimeMin = tmpTime + ((1000 * 60) - (today->tm_sec * 1000));

	g_Log.Add("[ ITR ] (%d) Sync Open Time. [%d]min remain", m_nTempleNumber + 1, this->m_nRemainTimeMin);
}

void CIllusionTempleProcess_Renewal::Proc_ITR(int nCurTime)
{
	switch (this->m_btIllusionTempleState)
	{
		case 0:
			this->Proc_ITRState_None(nCurTime);
			break;
		case 1:
			this->Proc_ITRState_Ready(nCurTime);
			break;
		case 2:
			this->Proc_ITRState_Playing(nCurTime);
			break;
		case 3:
			this->Proc_ITRState_End(nCurTime);
			break;
		case 4:
			this->Proc_ITRState_RoundBreak(nCurTime);
			break;
	}
}

void CIllusionTempleProcess_Renewal::Proc_ITRState_RoundBreak(int nCurTime)
{
	int iTICK_MSEC = nCurTime - this->m_nTickCount;

	if (iTICK_MSEC >= 1000)
	{
		this->m_nRemainTimeMsec -= iTICK_MSEC;
		this->m_nTickCount = nCurTime;
		this->m_nRoundBreakCount++;

		if (this->m_nRoundBreakCount == 5)
		{
			this->m_byITRRound++;
			this->Set_ITRState(2);
			this->m_nRoundBreakCount = 0;
			this->m_bRoundBreak = false;
		}

		this->Send_ITR_PlayState(0);
		this->Send_ITR_PlayState(1);
	}

	if (!this->m_nUserCount)
	{
		this->Set_ITRState(3);
	}
}

void CIllusionTempleProcess_Renewal::Proc_ITRState_None(int nCurTime)
{
	int iTICK_MSEC = nCurTime - this->m_nTickCount;

	if (this->m_nNextTimeMin < nCurTime)
	{
		this->m_nRemainTimeMin--;
		this->m_nNextTimeMin = nCurTime + 60000;
		if (this->m_nRemainTimeMin <= g_IT_Event.m_nITROpenTime)
		{
			if (this->m_nRemainTimeMin > 0 && this->m_nTempleNumber == 0)
			{
				char szTemp[128];
				sprintf(szTemp, Lang.GetText(0,496), this->m_nRemainTimeMin);

				int MapServerGroup = g_MapServerManager.GetMapSvrGroup();
				GS_GDReqMapSvrMsgMultiCast(MapServerGroup, szTemp);
			}
		}
	}

	if (iTICK_MSEC >= 1000)
	{
		if (this->m_nRemainTimeMsec > 0)
			this->m_nRemainTimeMsec -= iTICK_MSEC;

		this->m_nTickCount = nCurTime;

		if (this->m_nRemainTimeMin <= g_IT_Event.m_nITROpenTime + 1 && this->m_nRemainTimeMsec == -1)
		{
			this->m_nRemainTimeMsec = 60000 * (g_IT_Event.m_nITROpenTime + 1);
		}

		if (this->m_nRemainTimeMin <= g_IT_Event.m_nITROpenTime && this->m_nRemainTimeMsec > 5000 && this->m_bEntrance == false)
		{
			this->m_bEntrance = 1;
		}

		if (this->m_nRemainTimeMsec <= 30000 && this->m_nRemainTimeMsec > 0 && this->m_nTempleNumber == 0 && this->m_bSendMsgBeforeReady == false)
		{
			this->m_bSendMsgBeforeReady = true;

			PMSG_SET_DEVILSQUARE pMsg;
			PHeadSetB((LPBYTE)&pMsg, 0x92, sizeof(pMsg));

			pMsg.Type = 24;

			for (int n = g_ConfigRead.server.GetObjectStartUserIndex(); n < g_ConfigRead.server.GetObjectMax(); n++)
			{
				if (gObj[n].Connected == PLAYER_PLAYING && gObj[n].Type == OBJ_USER)
				{
					if (gObj[n].MapNumber == MAP_INDEX_LORENMARKET)
					{
						IOCP.DataSend(n, (LPBYTE)&pMsg, pMsg.h.size);
					}
				}
			}
		}

		if (this->m_nRemainTimeMsec <= 5000)
		{
			if (this->m_bEntrance == true)
			{
				this->m_bEntrance = false;

				if (this->m_nTempleNumber == 0)
				{
					char szTemp[128];
					sprintf(szTemp, Lang.GetText(0,497), this->m_nRemainTimeMin);

					int MapServerGroup = g_MapServerManager.GetMapSvrGroup();
					GS_GDReqMapSvrMsgMultiCast(MapServerGroup, szTemp);
				}
			}
		}
	}

	if (this->m_nRemainTimeMin <= 0)
	{
		this->m_bEntrance = false;
		this->CheckUsersOnConnect();
		BYTE byAllied = 0;
		BYTE byIllusion = 0;
		bool bCanStart = this->CanStartITRBattle(byAllied, byIllusion);

		if (this->m_nUserCount >= 2 && bCanStart)
		{
			for (int nG = 0; nG < 2; nG++)
			{
				this->m_PartyTeam[nG].nTeamIndex = nG;
			}

			for (int i = 0; i < 10; i++)
			{
				if (this->m_UserData[i].m_nIndex != -1 && gObj[this->m_UserData[i].m_nIndex].m_nITR_Index == i)
				{
					if (this->m_UserData[i].m_btTeam == 1)
					{
						g_SendNPCInfo.SendITLNpcCoordinate(this->m_UserData[i].m_nIndex, this->m_nTempleNumber + 45);
						gObjMoveGate(this->m_UserData[i].m_nIndex, this->m_nTempleNumber + 154);
						gObj[this->m_UserData[i].m_nIndex].m_Change = 405;
						gObjViewportListProtocolCreate(&gObj[this->m_UserData[i].m_nIndex]);
						this->m_nIllusionUserCount++;
					}

					else if (this->m_UserData[i].m_btTeam == 0)
					{
						g_SendNPCInfo.SendITLNpcCoordinate(this->m_UserData[i].m_nIndex, this->m_nTempleNumber + 45);
						gObjMoveGate(this->m_UserData[i].m_nIndex, this->m_nTempleNumber + 148);
						gObj[this->m_UserData[i].m_nIndex].m_Change = 404;
						gObjViewportListProtocolCreate(&gObj[this->m_UserData[i].m_nIndex]);
						this->m_nAlliedUserCount++;
					}
				}
			}

			this->Set_ITRState(1);
		}

		else
		{
			BYTE byWinTeamIndex = -1;

			if (byAllied == 0 && byIllusion == 1)
			{
				byWinTeamIndex = 1;
			}

			else if (byAllied == 1 && byIllusion == 0)
			{
				byWinTeamIndex = 0;
			}

			g_Log.Add("[ ITR ] START FAIL ALLIED:ILLUSION == [%d VS %d],WINTEAM : [%d]",
				byAllied, byIllusion, byWinTeamIndex);

			for (int nIndex = 0; nIndex < 10; nIndex++)
			{
				if (this->m_UserData[nIndex].m_nIndex != -1 && gObj[this->m_UserData[nIndex].m_nIndex].Connected > PLAYER_LOGGED && gObj[this->m_UserData[nIndex].m_nIndex].MapNumber == this->m_nTempleNumber + 45)
				{
					gObjMoveGate(this->m_UserData[nIndex].m_nIndex, 333);
					this->ClearUserData(nIndex);
				}
			}

			g_Log.Add("[ ITR ] (%d) Failed to Start ITR (UserCount: %d) (m_nRemainTimeMin:%d)(m_nRemainTimeMsec:%d)",
				this->m_nTempleNumber + 1, this->m_nUserCount, this->m_nRemainTimeMin, this->m_nRemainTimeMsec);

			this->Set_ITRState(3);
			this->m_bITRRoundStartFail = true;
		}
	}
}

void CIllusionTempleProcess_Renewal::Proc_ITRState_Ready(int nCurTime)
{
	int iTICK_MSEC = nCurTime - this->m_nTickCount;

	if (iTICK_MSEC >= 1000)
	{
		this->m_nRemainTimeMsec -= iTICK_MSEC;
		this->m_nTickCount = nCurTime;

		if (this->m_nRemainTimeMsec <= 30000 && this->m_nRemainTimeMsec > 0 && this->m_bSendMsgBeforePlay == false)
		{
			this->m_bSendMsgBeforePlay = true;

			PMSG_SET_DEVILSQUARE pMsg;
			PHeadSetB((LPBYTE)&pMsg, 0x92, sizeof(pMsg));

			pMsg.Type = 25;

			for (int i = 0; i < 10; i++)
			{
				if (this->m_UserData[i].m_nIndex != -1 && gObj[this->m_UserData[i].m_nIndex].Connected > PLAYER_LOGGED)
				{
					if (gObj[this->m_UserData[i].m_nIndex].MapNumber == this->m_nTempleNumber + 45)
					{
						IOCP.DataSend(this->m_UserData[i].m_nIndex, (LPBYTE)&pMsg, pMsg.h.size);
					}
				}
			}
		}
	}

	if (this->m_nUserCount > 0)
	{
		if (this->m_nRemainTimeMsec <= 0)
		{
			this->Set_ITRState(2);
			this->Send_ITR_PlayState(0);
			this->Send_ITR_PlayState(1);
		}
	}

	else
	{
		this->Set_ITRState(3);
	}
}

void CIllusionTempleProcess_Renewal::Proc_ITRState_Playing(int nCurTime)
{
	int iTICK_MSEC = nCurTime - this->m_nTickCount;
	this->m_bRoundBreak = false;

	if (iTICK_MSEC >= 1000)
	{
		this->m_nRemainTimeMsec -= iTICK_MSEC;
		this->m_nTickCount = nCurTime;

		if (this->m_nRemainTimeMsec > 0)
		{
			this->Send_ITR_PlayState(0);
			this->Send_ITR_PlayState(1);
		}

		if (this->m_StatusRegenTime != -1)
		{
			if (this->m_StatusRegenTime < iTICK_MSEC)
			{
				this->RegenCursedStoneStatus(this->m_byITRRound);
			}
		}
	}

	if (this->m_nUserCount > 0)
	{
		if (this->m_nRemainTimeMsec <= 0 || this->m_nAlliedUserCount == 0 || this->m_nIllusionUserCount == 0)
		{
			if (this->m_nRemainTimeMsec > 0)
			{
				this->m_nEventEndRemainTime = this->m_nRemainTimeMsec / 1000;
			}

			else
			{
				this->m_nEventEndRemainTime = 0;
			}

			this->Send_ITR_PlayState(0);
			this->Send_ITR_PlayState(1);
			this->Set_ITRState(3);
		}
	}

	else
	{
		this->Set_ITRState(3);
	}
}

void CIllusionTempleProcess_Renewal::Proc_ITRState_End(int nCurTime)
{
	int iTICK_MSEC = nCurTime - this->m_nTickCount;

	if (iTICK_MSEC >= 1000)
	{
		this->m_nRemainTimeMsec -= iTICK_MSEC;
		this->m_nTickCount = nCurTime;

		if (this->m_nRemainTimeMsec <= 60000 * g_IT_Event.m_nITREndTime - 3000)
		{
			if (this->m_nRemainTimeMsec > 0 && this->m_bGameEndPosMove == false)
			{
				this->m_bGameEndPosMove = true;

				for (int i = 0; i < 10; i++)
				{
					if (this->m_UserData[i].m_nIndex != -1 && gObj[this->m_UserData[i].m_nIndex].Connected > PLAYER_LOGGED && gObj[this->m_UserData[i].m_nIndex].MapNumber == this->m_nTempleNumber + 45)
					{
						if (this->m_UserData[i].m_btTeam == 1)
						{
							gObjMoveGate(this->m_UserData[i].m_nIndex, this->m_nTempleNumber + 154);
						}

						else if (this->m_UserData[i].m_btTeam == 0)
						{
							gObjMoveGate(this->m_UserData[i].m_nIndex, this->m_nTempleNumber + 148);
						}

						gObjSetPosition(this->m_UserData[i].m_nIndex, gObj[this->m_UserData[i].m_nIndex].X, gObj[this->m_UserData[i].m_nIndex].Y);
					}

					if (this->m_UserData[i].m_nRelicsInvenPos > 0)
					{
						if (gObj[this->m_UserData[i].m_nIndex].pInventory[this->m_UserData[i].m_nRelicsInvenPos].m_Type == ITEMGET(14, 223))
						{
							gObjInventoryDeleteItem(this->m_UserData[i].m_nIndex, this->m_UserData[i].m_nRelicsInvenPos);
							GSProtocol.GCInventoryItemDeleteSend(this->m_UserData[i].m_nIndex, this->m_UserData[i].m_nRelicsInvenPos, 0);
							this->m_UserData[i].m_nRelicsInvenPos = -1;
						}
					}
				}

				if (this->m_bITRRoundStartFail == false)
				{
					this->CalCursetTempleResult();
				}
			}
		}
	}

	if (this->m_nRemainTimeMsec <= 60000 * g_IT_Event.m_nITREndTime - 6000 && this->m_nRemainTimeMsec > 0 && this->m_bGameEndPosMove == true && this->m_bSendResult == false)
	{
		if (this->m_bITRRoundStartFail == false)
		{
			this->SendITRResult();
		}

		this->m_bSendResult = true;
	}

	if (this->m_nRemainTimeMsec <= 0)
	{
		this->Set_ITRState(0);
	}
}

void CIllusionTempleProcess_Renewal::Set_ITRState(BYTE btState)
{
	this->m_btIllusionTempleState = btState;

	switch (btState)
	{
		case 0:
			this->Set_ITRState_None();
			break;
		case 1:
			this->Set_ITRState_Ready();
			break;
		case 2:
			this->Set_ITRState_Playing();
			break;
		case 3:
			this->Set_ITRState_End();
			break;
		case 4:
			this->Set_ITRState_RoundBreak();
			break;
	}
}

BYTE CIllusionTempleProcess_Renewal::GetIllusionTempleState()
{
	return this->m_btIllusionTempleState;
}

void CIllusionTempleProcess_Renewal::Set_ITRState_None()
{
	this->SendIllusionTempleState(0, -1);

	for (int i = 0; i < 10; i++)
	{
		if (this->m_UserData[i].m_nIndex != -1 && gObj[this->m_UserData[i].m_nIndex].Connected > PLAYER_LOGGED)
		{
			gObj[this->m_UserData[i].m_nIndex].m_nITR_Index = -1;
			gObj[this->m_UserData[i].m_nIndex].m_nITR_RelicsTick = 0;
			gObj[this->m_UserData[i].m_nIndex].m_bITR_GettingRelics = 0;
			gObj[this->m_UserData[i].m_nIndex].m_bITR_RegisteringRelics = 0;
			gObj[this->m_UserData[i].m_nIndex].m_wITR_NpcType = 0;
			gObj[this->m_UserData[i].m_nIndex].m_byITR_StoneState = 99;

			gObjMoveGate(this->m_UserData[i].m_nIndex, 333);
			gObj[this->m_UserData[i].m_nIndex].m_Change = -1;
			gObjViewportListProtocolCreate(&gObj[this->m_UserData[i].m_nIndex]);

		}
	}

	this->ITR_ProcessInit(this->m_nTempleNumber);
}

void CIllusionTempleProcess_Renewal::Set_ITRState_Ready()
{
	this->m_nRemainTimeMsec = 60000 * g_IT_Event.m_nITRReadyTime;
	this->m_bITRRoundStartFail = false;
	this->SendIllusionTempleState(1, -1);
}

void CIllusionTempleProcess_Renewal::Set_ITRState_RoundBreak()
{
	if (this->m_bRoundBreak == false)
	{
		this->MoveToStartPoint_WhenRoundBreak();
		this->DeleteAllRelicsItem();
		this->RegenCursedStoneStatus(this->m_byITRRound);
		this->RegenOccupiedStoneStatus();
		this->m_bRoundBreak = true;
	}

	this->SendIllusionTempleState(4, -1);
}

void CIllusionTempleProcess_Renewal::Set_ITRState_Playing()
{
	if (this->m_byITRRound == 1)
	{
		this->m_nRemainTimeMsec = 60000 * g_IT_Event.m_nITRPlayTime;

		for (int i = 0; i < 10; i++)
		{
			if (this->m_UserData[i].m_nIndex != -1 && gObj[this->m_UserData[i].m_nIndex].Connected > PLAYER_LOGGED)
			{
				if (gObj[this->m_UserData[i].m_nIndex].MapNumber == this->m_nTempleNumber + 45)
				{
					g_Log.Add("[ ITR ] MAP:[%d], ACC:[%s], NAME:[%s], TEAM:[%d], CLASS:[%d]",
						this->m_nTempleNumber, gObj[this->m_UserData[i].m_nIndex].AccountID, gObj[this->m_UserData[i].m_nIndex].Name,
						this->m_UserData[i].m_btTeam, gObj[this->m_UserData[i].m_nIndex].m_PlayerData->DbClass);
				}
			}
		}
	}

	if (this->m_bIsSetNpc == false)
	{
		this->SetNpc();
	}

	if (this->m_bFirstRegen_CursedStone == false)
	{
		this->FirstRegen_CursedStone();
		g_Log.Add("[ ITR ][FirstRegen_CursedStone]");
	}

	if (this->m_bFirstRegen_OccStone == false)
	{
		this->FirstRegen_OccupiedStone();
		g_Log.Add("[ ITR ][FirstRegen_OccupiedStone]");
	}

	this->SetStatusRegenTime();
	this->SendIllusionTempleState(2, -1);
}

void CIllusionTempleProcess_Renewal::Set_ITRState_End()
{
	this->m_nRemainTimeMsec = 60000 * g_IT_Event.m_nITREndTime;
	this->SendIllusionTempleState(3, -1);

	for (int i = 0; i < 10; i++)
	{
		if (this->m_UserData[i].m_nIndex != -1 && gObj[this->m_UserData[i].m_nIndex].Connected > PLAYER_LOGGED && gObj[this->m_UserData[i].m_nIndex].MapNumber == this->m_nTempleNumber + 45)
		{
			if (this->m_UserData[i].m_dwSkillProdectionTime != 0)
			{
				this->m_UserData[i].m_dwSkillProdectionTime = 0;
				gObjRemoveBuffEffect(&gObj[this->m_UserData[i].m_nIndex], BUFFTYPE_TEMPLE_PROTECTION);
				this->Send_ITR_SkillEnd(&gObj[this->m_UserData[i].m_nIndex], 210);
			}

			if (this->m_UserData[i].m_dwSkillRestraintTime != 0)
			{
				this->m_UserData[i].m_dwSkillRestraintTime = 0;
				gObjRemoveBuffEffect(&gObj[this->m_UserData[i].m_nIndex], BUFFTYPE_RESTRICTION);
				this->Send_ITR_SkillEnd(&gObj[this->m_UserData[i].m_nIndex], 211);
			}

			this->ResetUserFlag_DoingRelicsThing(&gObj[this->m_UserData[i].m_nIndex]);
		}
	}

	this->GetOccupiedStoneCount(this->m_byAlliedTeamPoint, this->m_byIllusionTeamPoint);
	g_Log.Add("[ ITR ][WarResult] Allied Team : %d , IllusionTeam :%d", this->m_byAlliedTeamPoint, this->m_byIllusionTeamPoint);

	this->RemoveAllObj();
	this->m_bIsSetNpc = false;
	this->m_bFirstRegen_CursedStone = false;
	this->m_bFirstRegen_OccStone = false;
}

BOOL CIllusionTempleProcess_Renewal::EnterUserIllusionTemple(int aIndex, BYTE btTempleIndex, int nPartyIdx)
{
	if (!ObjectMaxRange(aIndex))
	{
		g_Log.Add("return %s %d", __FILE__, __LINE__);
		return FALSE;
	}

	if (this->GetIllusionTempleState() >= 1)
	{
		return FALSE;
	}

	if (gObj[aIndex].m_nITR_Index != -1)
	{
		g_Log.Add("[ ITR ] Enter Error: IllusionTempleIndex Fail %d", gObj[aIndex].m_nITR_Index);
		return FALSE;
	}

	int nItemPos = this->SearchRelicsItem(aIndex);

	if (nItemPos >= 0 && gObj[aIndex].pInventory[nItemPos].m_Type == ITEMGET(14, 223))
	{
		gObjInventoryDeleteItem(aIndex, nItemPos);
		GSProtocol.GCInventoryItemDeleteSend(aIndex, nItemPos, 0);
	}

	BOOL bResult = FALSE;
	EnterCriticalSection(&this->m_critUserData);

	for (int i = 0; i < 10; i++)
	{
		if (this->m_UserData[i].m_nIndex == -1)
		{
			this->m_UserData[i].m_nIndex = aIndex;
			this->m_UserData[i].m_nPartyIdx = nPartyIdx;
			this->m_nUserCount++;
			gObj[aIndex].m_nITR_Index = i;
			this->m_UserData[i].m_bEnterOk = true;
			bResult = TRUE;

			if (this->m_PartyTeam[0].nPartyIndex == gObj[this->m_UserData[i].m_nIndex].PartyNumber)
			{
				this->m_UserData[i].m_btTeam = 0;
			}

			else if (this->m_PartyTeam[1].nPartyIndex == gObj[this->m_UserData[i].m_nIndex].PartyNumber)
			{
				this->m_UserData[i].m_btTeam = 1;
			}

			break;
		}
	}

	LeaveCriticalSection(&this->m_critUserData);
	return bResult;
}

BOOL CIllusionTempleProcess_Renewal::LeaveUserIllusionTemple(int aIndex)
{
	if (!ObjectMaxRange(aIndex))
	{
		g_Log.Add("return %s %d", __FILE__, __LINE__);
		return FALSE;
	}

	if (gObj[aIndex].m_nITR_Index != -1 && this->m_UserData[gObj[aIndex].m_nITR_Index].m_nIndex == aIndex && gObjIsConnected(aIndex))
	{
		if (this->m_UserData[gObj[aIndex].m_nITR_Index].m_dwSkillProdectionTime != 0)
		{
			gObjRemoveBuffEffect(&gObj[aIndex], BUFFTYPE_TEMPLE_PROTECTION);
			this->Send_ITR_SkillEnd(&gObj[aIndex], 210);
		}

		if (this->m_UserData[gObj[aIndex].m_nITR_Index].m_dwSkillRestraintTime != 0)
		{
			gObjRemoveBuffEffect(&gObj[aIndex], BUFFTYPE_RESTRICTION);
			this->Send_ITR_SkillEnd(&gObj[aIndex], 211);
		}

		EnterCriticalSection(&this->m_critUserData);

		if (this->m_UserData[gObj[aIndex].m_nITR_Index].m_btTeam == 0)
		{
			this->m_nAlliedUserCount--;
		}

		else if (this->m_UserData[gObj[aIndex].m_nITR_Index].m_btTeam == 1)
		{
			this->m_nIllusionUserCount--;
		}

		this->ClearUserData(gObj[aIndex].m_nITR_Index);
		gObj[aIndex].m_nITR_Index = -1;
		gObj[aIndex].m_wITR_NpcType = 0;
		gObj[aIndex].m_byITR_StoneState = 99;
		this->m_nUserCount--;

		LeaveCriticalSection(&this->m_critUserData);
		g_Log.Add("[ ITR ] (%d) LeaveUser: (%s)(%s)", this->m_nTempleNumber + 1, gObj[aIndex].AccountID, gObj[aIndex].Name);
	}

	gObj[aIndex].m_Change = -1;
	gObjViewportListProtocolCreate(&gObj[aIndex]);
	gObj[aIndex].m_nITR_RelicsTick = 0;
	gObj[aIndex].m_byITR_StoneState = 99;
	gObj[aIndex].m_wITR_NpcType = 0;
	gObj[aIndex].m_bITR_GettingRelics = 0;
	gObj[aIndex].m_bITR_RegisteringRelics = 0;
	gObj[aIndex].m_nITR_Index = -1;

	return TRUE;
}

bool CIllusionTempleProcess_Renewal::CanStartITRBattle(BYTE &ALLIED, BYTE &ILLUSION)
{
	int nAlliedCount = 0;
	int nIllusionCount = 0;

	for (int i = 0; i < 10; i++)
	{
		if (this->m_UserData[i].m_nIndex != -1 &&
			this->m_UserData[i].m_bEnterOk == true &&
			gObjIsConnected(this->m_UserData[i].m_nIndex))
		{
			if (this->m_UserData[i].m_btTeam == 0)
			{
				nAlliedCount++;
			}

			else if (this->m_UserData[i].m_btTeam == 1)
			{
				nIllusionCount++;
			}
		}
	}

	if (nAlliedCount <= 1 || nIllusionCount <= 1)
	{
		g_Log.Add("[ ITR ] MAP : %d[CanStartITRBattle] FALSE nAlliedCount:%d,nIllusionCount:%d ",
			this->m_nTempleNumber + 1, nAlliedCount, nIllusionCount);

		ALLIED = nAlliedCount;
		ILLUSION = nIllusionCount;

		return FALSE;
	}

	else
	{ 
		g_Log.Add("[ ITR ] MAP : %d[CanStartITRBattle] TRUE nAlliedCount:%d,nIllusionCount:%d ",
			this->m_nTempleNumber + 1, nAlliedCount, nIllusionCount);

		ALLIED = nAlliedCount;
		ILLUSION = nIllusionCount;

		return TRUE;
	}
}

void CIllusionTempleProcess_Renewal::CheckUsersOnConnect()
{
	EnterCriticalSection(&this->m_critUserData);

	this->m_nUserCount = 0;

	for (int i = 0; i < 10; i++)
	{
		if (this->m_UserData[i].m_nIndex != -1)
		{
			if (gObjIsConnected(this->m_UserData[i].m_nIndex))
			{
				if (gObj[this->m_UserData[i].m_nIndex].MapNumber == this->m_nTempleNumber + 45)
				{
					this->m_nUserCount++;
				}

				else
				{
					gObj[this->m_UserData[i].m_nIndex].m_nITR_Index = -1;
					this->ClearUserData(i);
				}
			}

			else
			{
				gObj[this->m_UserData[i].m_nIndex].m_nITR_Index = -1;
				this->ClearUserData(i);
			}
		}
	}

	LeaveCriticalSection(&this->m_critUserData);
}

int CIllusionTempleProcess_Renewal::GetRemainTimeMin()
{
	if (this->GetIllusionTempleState() || this->GetEntrance() != true)
	{
		return 0;
	}

	return this->m_nRemainTimeMin;
}

int CIllusionTempleProcess_Renewal::GetEnterRemainTimeMin()
{
	if (this->GetIllusionTempleState() == 0 && this->GetEntrance() == true)
	{
		return 0;
	}

	if (this->GetIllusionTempleState() == 0 && this->GetEntrance() == false)
	{
		return this->m_nRemainTimeMin - g_IT_Event.m_nITROpenTime;
	}

	std::list<_ITR_STARTTIME>::iterator iter;

	_ITR_STARTTIME prevStartTime;
	_ITR_STARTTIME nextStartTime;

	DWORD dwPrevTime = 0;
	DWORD dwNextTime = 0;
	DWORD dwCurTime = 0;
	int btRemainTime = 0;

	if (g_IT_Event.m_lstALLITR_TIME.size() == 0)
	{
		g_Log.MsgBox("Error : Illusion Temple StartTime size is 0");
		return 0;
	}

	tm* today;
	time_t ltime;

	time(&ltime);
	today = localtime(&ltime);

	dwCurTime = (today->tm_hour * 60) + today->tm_min;
	prevStartTime = *g_IT_Event.m_lstALLITR_TIME.begin();

	for (iter = g_IT_Event.m_lstALLITR_TIME.begin(); iter != g_IT_Event.m_lstALLITR_TIME.end(); ++iter)
	{
		nextStartTime = *iter;

		dwPrevTime = (prevStartTime.nHour * 60) + prevStartTime.nMin;
		dwNextTime = (nextStartTime.nHour * 60) + nextStartTime.nMin;

		if (dwPrevTime == dwNextTime)
		{
			if (dwCurTime < dwNextTime)
			{
				nextStartTime = *iter;
				break;
			}
			continue;
		}

		if (dwCurTime >= dwPrevTime &&
			dwCurTime < dwNextTime)
		{
			break;
		}

		prevStartTime = *iter;
	}

	int Cnt = 2;

	while (Cnt-- != 0)
	{
		if (iter == g_IT_Event.m_lstALLITR_TIME.end())
		{
			iter = g_IT_Event.m_lstALLITR_TIME.begin();

			nextStartTime = (*iter);
		}

		dwNextTime = nextStartTime.nHour * 60 + nextStartTime.nMin;

		if (today->tm_hour <= nextStartTime.nHour)
		{
			btRemainTime = dwNextTime - dwCurTime;
		}
		else
		{
			btRemainTime = ((24 * 60) - dwCurTime) + dwNextTime;
		}

		if (btRemainTime <= g_IT_Event.m_nITROpenTime)
		{
			iter++;

			if (iter != g_IT_Event.m_lstALLITR_TIME.end())
			{
				nextStartTime = (*iter);
			}
		}
		else
		{
			btRemainTime -= g_IT_Event.m_nITROpenTime;
			break;
		}
	}

	int tmpTime = GetTickCount();
	m_nNextTimeMin = tmpTime + ((1000 * 60) - (today->tm_sec * 1000));

	return btRemainTime;
}

int CIllusionTempleProcess_Renewal::SearchRelicsItem(int iIndex)
{
	if (!ObjectMaxRange(iIndex))
	{
		g_Log.Add("return %s %d", __FILE__, __LINE__);
		return -1;
	}

	for (int x = 0; x < INVENTORY_SIZE; x++)
	{
		if (gObj[iIndex].pInventory[x].IsItem() == TRUE &&
			gObj[iIndex].pInventory[x].m_Type == ITEMGET(14, 223))
		{
			return x;
		}
	}

	return -1;
}

void CIllusionTempleProcess_Renewal::DeleteAllRelicsItem()
{
	for (int i = 0; i < 10; i++)
	{
		if (this->m_UserData[i].m_nRelicsInvenPos > 0)
		{
			LPOBJ lpObj = &gObj[this->m_UserData[i].m_nIndex];

			if (lpObj)
			{
				if (lpObj->pInventory[this->m_UserData[i].m_nRelicsInvenPos].m_Type == ITEMGET(14, 223))
				{
					this->SendRelicsUserInfo(lpObj, 1);

					g_Log.Add("[ ITR ] (%d) (%s)(%s) to Delete All Relics Item [Serial:%I64d]",
						this->m_nTempleNumber + 1, lpObj->AccountID, lpObj->Name, lpObj->pInventory[this->m_UserData[i].m_nRelicsInvenPos].m_Number);

					gObjInventoryDeleteItem(this->m_UserData[i].m_nIndex, this->m_UserData[i].m_nRelicsInvenPos);
					GSProtocol.GCInventoryItemDeleteSend(this->m_UserData[i].m_nIndex, this->m_UserData[i].m_nRelicsInvenPos, 0);
					this->m_UserData[i].m_nRelicsInvenPos = -1;
				}
			}
		}
	}
}

int CIllusionTempleProcess_Renewal::FindITRUser(int index)
{
	for (int i = 0; i < 10; i++)
	{
		if (this->m_UserData[i].m_nIndex == index)
		{
			return i;
		}
	}

	return -1;
}

void CIllusionTempleProcess_Renewal::DropRelicsItem(int iIndex, BYTE byAct)
{
	if (!ObjectMaxRange(iIndex))
	{
		g_Log.Add("return %s %d", __FILE__, __LINE__);
		return;
	}

	LPOBJ lpObj = &gObj[iIndex];
	int nITR_USER_ARRAY = this->FindITRUser(iIndex);

	if (nITR_USER_ARRAY == -1)
	{
		return;
	}

	int iItemPos = this->m_UserData[nITR_USER_ARRAY].m_nRelicsInvenPos;

	if (iItemPos == -1)
	{
		return;
	}

	if (lpObj->pInventory[iItemPos].m_Type != ITEMGET(14, 223))
	{
		return;
	}

	this->SendRelicsUserInfo(lpObj, 1);

	PMSG_ITEMTHROW_RESULT pResult;

	PHeadSetB((LPBYTE)&pResult, 0x23, sizeof(pResult));
	pResult.Result = TRUE;
	pResult.Ipos = iItemPos;

	int map_num = gObj[iIndex].MapNumber;
	int type = gObj[iIndex].pInventory[iItemPos].m_Type;
	int level = gObj[iIndex].pInventory[iItemPos].m_Level;
	float dur = gObj[iIndex].pInventory[iItemPos].m_Durability;
	BOOL ret = gObj[iIndex].pInventory[iItemPos].IsItem();
	BYTE Option1 = gObj[iIndex].pInventory[iItemPos].m_Option1;
	BYTE Option2 = gObj[iIndex].pInventory[iItemPos].m_Option2;
	BYTE Option3 = gObj[iIndex].pInventory[iItemPos].m_Option3;
	BYTE NOption = gObj[iIndex].pInventory[iItemPos].m_NewOption;
	UINT64 s_num = gObj[iIndex].pInventory[iItemPos].m_Number;
	BYTE ItemExOption = g_kJewelOfHarmonySystem.GetItemStrengthenOption(&gObj[iIndex].pInventory[iItemPos]);
	BYTE ItemExLevel = g_kJewelOfHarmonySystem.GetItemOptionLevel(&gObj[iIndex].pInventory[iItemPos]);

	BYTE NewOption[8];

	::ItemIsBufExOption(NewOption, &gObj[iIndex].pInventory[iItemPos]);

	int PetLevel = gObj[iIndex].pInventory[iItemPos].m_PetItem_Level;

	UINT64 PetExp = gObj[iIndex].pInventory[iItemPos].m_PetItem_Exp;
	BYTE SOption = gObj[iIndex].pInventory[iItemPos].m_SetOption;

	BYTE ItemEffectEx = gObj[iIndex].pInventory[iItemPos].m_ItemOptionEx;
	int item_number = gObj[iIndex].pInventory[iItemPos].m_Number;

	char szItemName[50] = "Relics Item";

	int aAntiLootIndex = -1;
	short X = 0;
	short Y = 0;

	BYTE btMapAttr = MapC[map_num].GetAttr(gObj[iIndex].X, gObj[iIndex].Y);

	if ((btMapAttr & 4) == 4 || (btMapAttr & 8) == 8)
	{
		X = 168;
		Y = 85;
	}

	else
	{
		X = gObj[iIndex].X;
		Y = gObj[iIndex].Y;
	}

	if (MapC[map_num].ItemDrop(type, level, dur, X, Y, Option1, Option2, Option3, NOption, SOption, item_number, aAntiLootIndex, PetLevel, PetExp, ItemEffectEx, NULL, 0xFF, 0) == TRUE)
	{
		g_Log.Add("[ ITR ] (%d) (%s)(%s) to Drop Relics Item [Serial:%I64d]", this->m_nTempleNumber + 1, lpObj->AccountID, lpObj->Name, lpObj->pInventory[iItemPos].m_Number);

		gObjInventoryDeleteItem(iIndex, iItemPos);
		pResult.Result = TRUE;

		g_Log.Add("[%s][%s][%d]%d/%d Throw away Serial:%I64d [%s][%d][%d][%d][%d][%d] dur:[%d] Ex:[%d,%d,%d,%d,%d,%d,%d] Set:[%d] 380:[%d] HO:[%d,%d] E:[%d]", gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].MapNumber,
			gObj[iIndex].X, gObj[iIndex].Y, s_num, szItemName, type, level, Option1, Option2, Option3, (int)dur, NewOption[0],
			NewOption[1], NewOption[2], NewOption[3], NewOption[4], NewOption[5], NewOption[6], SOption,
			ItemEffectEx >> 7, ItemExOption, ItemExLevel);
	}

	else
	{
		pResult.Result = FALSE;
	}

	this->m_UserData[nITR_USER_ARRAY].m_nRelicsInvenPos = -1;
	IOCP.DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
}

char CIllusionTempleProcess_Renewal::CheckHaveRelics(int aIndex)
{
	return -1; // not used
}

void CIllusionTempleProcess_Renewal::SetRelicsInventoryPos(int aIndex, BYTE btPos)
{
	if (!ObjectMaxRange(aIndex))
	{
		g_Log.Add("return %s %d", __FILE__, __LINE__);
		return;
	}

	int nITR_USER_ARRAY = this->FindITRUser(aIndex);

	if (nITR_USER_ARRAY != -1)
	{
		this->m_UserData[nITR_USER_ARRAY].m_nRelicsInvenPos = btPos;
	}
}

void CIllusionTempleProcess_Renewal::SendRelicsUserInfo(LPOBJ lpObj, BYTE byGet)
{
	PMSG_ITL_RELICS_GET_USER pRelicsUser;
	PHeadSubSetB((LPBYTE)&pRelicsUser, 0xBF, 0x61, sizeof(pRelicsUser));

	pRelicsUser.wUserIndex = lpObj->m_Index;
	pRelicsUser.byGet = byGet;
	memcpy(pRelicsUser.Name, lpObj->Name, MAX_ACCOUNT_LEN);

	for (int i = 0; i < 10; i++)
	{
		if (this->m_UserData[i].m_nIndex != -1 && gObj[this->m_UserData[i].m_nIndex].Connected > PLAYER_LOGGED)
		{
			IOCP.DataSend(this->m_UserData[i].m_nIndex, (LPBYTE)&pRelicsUser, sizeof(pRelicsUser));
		}
	}

	g_Log.Add("[ ITR ][SendRelicsUserInfo] NAME:%s, ACT:%d", lpObj->Name, byGet);
}

void CIllusionTempleProcess_Renewal::SetRemainTime()
{
	this->m_nRemainTimeMin = 0;
	this->m_nRemainTimeMsec = 1000;
}

void CIllusionTempleProcess_Renewal::AddITRNpcInfo(WORD wNpcType, BYTE byMapNumber, BYTE byDistance, BYTE byX, BYTE byY, BYTE byDir)
{
	if (this->m_byNpcCount > 11)
	{
		g_Log.Add("[ ITR ][AddITRNpcInfo] Over NpcCount :%d ", this->m_byNpcCount);
		return;
	}

	BYTE byMapTagNpcIdentNo = -1;

	if (wNpcType == 658 || (wNpcType >= 659 && wNpcType <= 668))
	{
		this->m_ITRNpcInfo[this->m_byNpcCount].wNpcType = wNpcType;
		this->m_ITRNpcInfo[this->m_byNpcCount].byMapNumber = byMapNumber;
		this->m_ITRNpcInfo[this->m_byNpcCount].byDistance = byDistance;
		this->m_ITRNpcInfo[this->m_byNpcCount].byX = byX;
		this->m_ITRNpcInfo[this->m_byNpcCount].byY = byY;
		this->m_ITRNpcInfo[this->m_byNpcCount].byDir = byDir;

		g_SendNPCInfo.Find_ITL_Npc(byMapNumber, wNpcType, byMapTagNpcIdentNo);
		this->m_ITRNpcInfo[this->m_byNpcCount].byMapTagIndex = byMapTagNpcIdentNo;

		g_Log.Add("[ ITR ][AddITRNpcInfo] NpcID:%d, Map:%d, Dis:%d, X:%d, Y:%d, Dir:%d ,byMapTagNpcIdentNo:%d",
			wNpcType, byMapNumber, byDistance, byX, byY, byDir, byMapTagNpcIdentNo);

		this->m_byNpcCount++;
	}
}

void CIllusionTempleProcess_Renewal::ResetITRNpcInfo()
{
	for (int i = 0; i < 11; i++)
	{
		this->m_ITRNpcInfo[i].wNpcType = -1;
		this->m_ITRNpcInfo[i].byMapNumber = -1;
		this->m_ITRNpcInfo[i].byDistance = -1;
		this->m_ITRNpcInfo[i].byX = -1;
		this->m_ITRNpcInfo[i].byY = -1;
		this->m_ITRNpcInfo[i].byDir = -1;
		this->m_ITRNpcInfo[i].byMapTagIndex = -1;
	}

	this->m_byNpcCount = 0;
}

void CIllusionTempleProcess_Renewal::ResetITRPosNum()
{
	this->m_Cursed_StoneStatus.m_nIndex = -1;
	this->m_Cursed_StoneStatus.m_wPosNum = -1;
	this->m_Cursed_StoneStatus.m_wType = -1;
	this->m_Cursed_StoneStatus.byMapTagIndex = -1;

	for (int i = 0; i < 10; i++)
	{
		this->m_Occupied_StoneStatus[i].m_wType = -1;
		this->m_Occupied_StoneStatus[i].m_nIndex = -1;
		this->m_Occupied_StoneStatus[i].m_wPosNum = -1;
		this->m_Occupied_StoneStatus[i].byMapTagIndex = -1;
	}
}

void CIllusionTempleProcess_Renewal::RemoveAllObj()
{
	for (int i = 0; i < 10; i++)
	{
		if (this->m_Occupied_StoneStatus[i].m_nIndex != -1)
		{
			g_Log.Add("[ ITR ] (%d) Remove Occupied_StoneStatus Name: %s, (%d: %d/%d)",
				this->m_nTempleNumber + 1, gObj[this->m_Occupied_StoneStatus[i].m_nIndex].Name,
				gObj[this->m_Occupied_StoneStatus[i].m_nIndex].MapNumber, gObj[this->m_Occupied_StoneStatus[i].m_nIndex].X,
				gObj[this->m_Occupied_StoneStatus[i].m_nIndex].Y);

			gObjDel(this->m_Occupied_StoneStatus[i].m_nIndex);
			this->m_Occupied_StoneStatus[i].m_nIndex = -1;
			this->m_Occupied_StoneStatus[i].m_wPosNum = -1;
			this->m_Occupied_StoneStatus[i].m_wType = -1;
			this->m_Occupied_StoneStatus[i].m_byStatus = -1;
			this->m_Occupied_StoneStatus[i].byMapTagIndex = -1;
		}
	}

	if (this->m_Cursed_StoneStatus.m_nIndex != -1)
	{
		g_Log.Add("[ ITR ] (%d) Remove Cursed_StoneStatus Name: %s, (%d: %d/%d)",
			this->m_nTempleNumber + 1, gObj[this->m_Cursed_StoneStatus.m_nIndex].Name,
			gObj[this->m_Cursed_StoneStatus.m_nIndex].MapNumber, gObj[this->m_Cursed_StoneStatus.m_nIndex].X,
			gObj[this->m_Cursed_StoneStatus.m_nIndex].Y);

		gObjDel(this->m_Cursed_StoneStatus.m_nIndex);
		this->m_Cursed_StoneStatus.m_nIndex = -1;
		this->m_Cursed_StoneStatus.m_byStatus = -1;
		this->m_Cursed_StoneStatus.m_wPosNum = -1;
		this->m_Cursed_StoneStatus.m_wType = -1;
		this->m_Cursed_StoneStatus.byMapTagIndex = -1;
	}

	g_Log.Add("[ ITR ] (%d) Remove All Object", this->m_nTempleNumber + 1);
}

void CIllusionTempleProcess_Renewal::SendRelicsError(OBJECTSTRUCT *lpNpc, int index, BYTE byError, BYTE byAct)
{
	if (!lpNpc)
	{
		g_Log.Add("[ ITR ][SendRelicsError] lpNpc Is NULL,index:%d Error:%d,Act:%d", index, byError, byAct);
		return;
	}

	PMSG_ANS_ITL_RELATE_RELICS pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0xBF, 0x6A, sizeof(pMsg));

	pMsg.wNpcType = lpNpc->Class;
	pMsg.byNumberH = HIBYTE(lpNpc->m_Index);
	pMsg.byNumberL = LOBYTE(lpNpc->m_Index);
	pMsg.byError = byError;
	pMsg.byAct = byAct;

	IOCP.DataSend(index, (LPBYTE)&pMsg, pMsg.h.size);
	g_Log.Add("[ ITR ][SendRelicsError]index:%d, Error:%d,Act:%d,NPC_Class:%d,NpcNumber:%d",
		index, byError, byAct, lpNpc->Class, lpNpc->m_Index);
}

void CIllusionTempleProcess_Renewal::Check_GetRelics(OBJECTSTRUCT *lpNpc, OBJECTSTRUCT *lpObj, WORD wNpcType)
{
	if (!lpNpc)
	{
		return;
	}

	if (!lpObj)
	{
		return;
	}

	if (lpNpc->m_State == 0)
	{
		return;
	}

	if (lpNpc->m_PosNum == -1)
	{
		return;
	}

	if (abs(lpObj->Y - lpNpc->Y) > 3)
	{
		return;
	}

	if (abs(lpObj->X - lpNpc->X) > 3)
	{
		return;
	}

	BYTE byUserTeamIndex = this->m_UserData[this->FindITRUser(lpObj->m_Index)].m_btTeam;

	if (lpObj->m_bITR_GettingRelics == true)
	{
		g_Log.Add("[ ITR ][Check_GetRelics]NAME:%s,m_bITR_GettingRelics%d", lpObj->Name, lpObj->m_bITR_GettingRelics);
		return;
	}

	if (lpObj->m_byITR_StoneState != 99)
	{
		g_Log.Add("[ ITR ][Check_GetRelics]NAME:%s,byStoneState%d", lpObj->Name, lpObj->m_byITR_StoneState);
		return;
	}

	if (lpObj->m_nITR_RelicsTick != 0)
	{
		this->SendRelicsError(lpNpc, lpObj->m_Index, 1, -1);
		lpObj->m_nITR_RelicsTick = 0;
		return;
	}

	if (wNpcType >= 659 && wNpcType <= 668)
	{
		for (int i = 0; i < 10; i++)
		{
			if (lpNpc->m_Index == this->m_Occupied_StoneStatus[i].m_nIndex)
			{
				if (this->m_Occupied_StoneStatus[i].m_byStatus && this->m_Occupied_StoneStatus[i].m_byStatus != 1)
				{
					this->SendRelicsError(lpNpc, lpObj->m_Index, 6, -1);
				}

				else if (this->m_Occupied_StoneStatus[i].m_byStatus == byUserTeamIndex)
				{
					this->SendRelicsError(lpNpc, lpObj->m_Index, 5, -1);
				}

				else
				{
					this->m_Occupied_StoneStatus[i].m_byStatus = byUserTeamIndex + 10;
					lpObj->m_byITR_StoneState = byUserTeamIndex + 10;
					lpObj->m_nITR_RelicsTick = GetTickCount() + 10000;
					lpObj->m_bITR_GettingRelics = true;
					lpObj->m_wITR_NpcType = this->m_Occupied_StoneStatus[i].m_wType;

					this->Send_ITR_StoneInfo(lpObj->m_Index,
						this->m_Occupied_StoneStatus[i].m_wType,
						this->m_Occupied_StoneStatus[i].m_byStatus,
						this->m_Occupied_StoneStatus[i].m_nIndex,
						this->m_Occupied_StoneStatus[i].byMapTagIndex);

					this->SendRelicsError(lpNpc, lpObj->m_Index, 0, 1);
				}

				return;
			}
		}
	}

	else if (wNpcType == 658)
	{
		if (this->m_Cursed_StoneStatus.m_byStatus)
		{
			this->SendRelicsError(lpNpc, lpObj->m_Index, 3, -1);
			lpObj->m_nITR_RelicsTick = 0;
		}

		else
		{
			lpObj->m_nITR_RelicsTick = GetTickCount() + 10000;
			this->m_Cursed_StoneStatus.m_byStatus = byUserTeamIndex + 10;
			lpObj->m_byITR_StoneState = byUserTeamIndex + 10;
			lpObj->m_nITR_RelicsTick = GetTickCount() + 10000;
			lpObj->m_bITR_GettingRelics = 1;
			lpObj->m_wITR_NpcType = this->m_Cursed_StoneStatus.m_wType;

			this->Send_ITR_StoneInfo(lpObj->m_Index,
				this->m_Cursed_StoneStatus.m_wType,
				this->m_Cursed_StoneStatus.m_byStatus,
				this->m_Cursed_StoneStatus.m_nIndex,
				this->m_Cursed_StoneStatus.byMapTagIndex);

			this->SendRelicsError(lpNpc, lpObj->m_Index, 0, 1);
		}
	}

	else
	{
		this->SendRelicsError(lpNpc, lpObj->m_Index, 4, -1);
		lpObj->m_nITR_RelicsTick = 0;
	}
}

void CIllusionTempleProcess_Renewal::GetRelics(OBJECTSTRUCT *lpNpc, OBJECTSTRUCT *lpObj, WORD wNpcType)
{
	if (!lpNpc || !lpObj)
	{
		return;
	}

	BOOL bFail = FALSE;

	if (lpObj->m_wITR_NpcType < 0)
	{
		lpObj->m_wITR_NpcType = 0;
		g_Log.Add("[ITR ][GetRelics] ERROR CASE 1");
		bFail = TRUE;
	}

	if (lpObj->m_bITR_GettingRelics == false)
	{
		g_Log.Add("[ITR ][GetRelics] ERROR CASE 2");
		bFail = TRUE;
	}

	if (bFail == TRUE)
	{
		this->ResetUserFlag_DoingRelicsThing(lpObj);
		return;
	}

	if (lpNpc->m_State == 0 || lpNpc->m_PosNum == -1)
	{
		g_Log.Add("[ITR ][GetRelics] ERROR CASE 3");
		this->ResetUserFlag_DoingRelicsThing(lpObj);
		return;
	}

	if (abs(lpObj->Y - lpNpc->Y) > 3 || abs(lpObj->X - lpNpc->X) > 3)
	{
		g_Log.Add("[ITR ][GetRelics] ERROR CASE 4");
		this->ResetUserFlag_DoingRelicsThing(lpObj);
		return;
	}

	if (lpObj->m_nITR_RelicsTick > GetTickCount())
	{
		g_Log.Add("[ITR ][GetRelics] ERROR CASE 5");
		this->SendRelicsError(lpNpc, lpObj->m_Index, 7, -1);
		this->ResetUserFlag_DoingRelicsThing(lpObj);
		return;
	}

	BOOL bCanGetRelics = FALSE;
	int nUserIndex = this->FindITRUser(lpObj->m_Index);
	BYTE byUserTeamIndex = this->m_UserData[nUserIndex].m_btTeam;

	if (wNpcType == 658 && wNpcType == lpObj->m_wITR_NpcType)
	{
		if (this->m_Cursed_StoneStatus.m_byStatus - 10 != byUserTeamIndex ||
			lpObj->m_byITR_StoneState != this->m_Cursed_StoneStatus.m_byStatus)
		{
			g_Log.Add("[ITR ][GetRelics] ERROR CASE 6");
			this->SendRelicsError(lpNpc, lpObj->m_Index, 11, -1);
			this->ResetUserFlag_DoingRelicsThing(lpObj);
			return;
		}

		bCanGetRelics = TRUE;
		this->m_Cursed_StoneStatus.m_byStatus = -1;
		lpObj->m_bITR_GettingRelics = 0;

		this->SendRelicsError(lpNpc, lpObj->m_Index, 0, 2);
		this->Send_ITR_StoneInfo(lpObj->m_Index,
			this->m_Cursed_StoneStatus.m_wType,
			this->m_Cursed_StoneStatus.m_byStatus,
			this->m_Cursed_StoneStatus.m_nIndex,
			this->m_Cursed_StoneStatus.byMapTagIndex);

		lpNpc->m_PosNum = -1;
		lpNpc->Life = 0.0;

		gObjLifeCheck(lpNpc, lpObj, 0, 1, 0, 0, 0, 0, 0);
	}
	
	else if ((wNpcType >= 659 && wNpcType <= 668) && wNpcType == lpObj->m_wITR_NpcType)
	{
		for (int i = 0; i < 10; i++)
		{
			if (lpNpc->Class == this->m_Occupied_StoneStatus[i].m_wType)
			{
				if (this->m_Occupied_StoneStatus[i].m_byStatus != 10 &&
					this->m_Occupied_StoneStatus[i].m_byStatus != 11)
				{
					g_Log.Add("[ITR ][GetRelics] ERROR CASE 8");
					this->SendRelicsError(lpNpc, lpObj->m_Index, 10, -1);
					this->ResetUserFlag_DoingRelicsThing(lpObj);
					return;
				}

				if (this->m_Occupied_StoneStatus[i].m_byStatus - 10 == byUserTeamIndex)
				{
					bCanGetRelics = TRUE;

					this->m_Occupied_StoneStatus[i].m_byStatus = -2;
					lpObj->m_nITR_RelicsTick = 0;
					lpObj->m_bITR_GettingRelics = false;

					this->SendRelicsError(lpNpc, lpObj->m_Index, 0, 2);
					this->Send_ITR_StoneInfo(lpObj->m_Index,
						this->m_Occupied_StoneStatus[i].m_wType,
						this->m_Occupied_StoneStatus[i].m_byStatus,
						this->m_Occupied_StoneStatus[i].m_nIndex,
						this->m_Occupied_StoneStatus[i].byMapTagIndex);

					this->m_UserData[nUserIndex].m_byInvalidationCount++;
					break;
				}
			}
		}
	}

	else
	{
		g_Log.Add("[ITR ][GetRelics] ERROR CASE 9");
		this->SendRelicsError(lpNpc, lpObj->m_Index, 8, -1);
		this->ResetUserFlag_DoingRelicsThing(lpObj);
		return;
	}

	if (bCanGetRelics == TRUE)
	{
		ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, lpNpc->X, lpNpc->Y, ItemGetNumberMake(14, 223), 0, 0, 0, 0, 0, lpObj->m_Index, 0, 0, 0, 0, 0);
	}

	this->ResetUserFlag_DoingRelicsThing(lpObj);
}

void CIllusionTempleProcess_Renewal::Check_RegisterRelics(OBJECTSTRUCT *lpNpc, OBJECTSTRUCT *lpObj)
{
	if (!lpNpc || !lpObj)
	{
		g_Log.AddC(TColor::Red, "[ITR] Error (%s) - lpNpc or lpObj == NULL", __FUNCTION__);
		return;
	}

	if (abs(lpObj->Y - lpNpc->Y) > 3 || abs(lpObj->X - lpNpc->X) > 3)
	{
		g_Log.AddC(TColor::Red, "[ITR] Error (%s) - abs > 3 (%d/%d) (%d/%d)", __FUNCTION__, lpObj->Y, lpNpc->Y, lpObj->X, lpNpc->X);
		return;
	}

	BOOL bFail = FALSE;

	if (lpObj->m_bITR_RegisteringRelics == true)
	{
		g_Log.AddC(TColor::Red, "[ITR] Error (%s) lpObj->m_bITR_RegisteringRelics == true", __FUNCTION__);
		bFail = TRUE;
	}

	if (lpObj->m_wITR_NpcType)
		bFail = TRUE;
	if (lpObj->m_byITR_StoneState != 99)
		bFail = TRUE;

	if (bFail == TRUE)
	{
		return;
	}

	int nITR_USER_ARRAY = this->FindITRUser(lpObj->m_Index);

	if (this->m_UserData[nITR_USER_ARRAY].m_nRelicsInvenPos <= 0 ||
		lpObj->pInventory[this->m_UserData[nITR_USER_ARRAY].m_nRelicsInvenPos].m_Type != ITEMGET(14, 223))
	{
		this->SendRelicsError(lpNpc, lpObj->m_Index, 12, -1);
		return;
	}

	if (lpNpc->Class < 659 || lpNpc->Class > 668)
	{
		this->SendRelicsError(lpNpc, lpObj->m_Index, 13, -1);
		return;
	}

	for (int i = 0; i < 10; i++)
	{
		if (this->m_Occupied_StoneStatus[i].m_wType == lpNpc->Class)
		{
			if (this->m_Occupied_StoneStatus[i].m_byStatus == 0xFF)
			{
				if (lpObj->m_nITR_RelicsTick != 0)
				{
					this->SendRelicsError(lpNpc, lpObj->m_Index, 16, -1);
				}

				else
				{
					this->m_Occupied_StoneStatus[i].m_byStatus = this->m_UserData[nITR_USER_ARRAY].m_btTeam + 100;
					lpObj->m_byITR_StoneState = this->m_UserData[nITR_USER_ARRAY].m_btTeam + 100;
					lpObj->m_wITR_NpcType = this->m_Occupied_StoneStatus[i].m_wType;
					lpObj->m_nITR_RelicsTick = GetTickCount() + 10000;
					lpObj->m_bITR_RegisteringRelics = TRUE;

					this->SendRelicsError(lpNpc, lpObj->m_Index, 0, 3);
					this->Send_ITR_StoneInfo(lpObj->m_Index,
						this->m_Occupied_StoneStatus[i].m_wType,
						this->m_Occupied_StoneStatus[i].m_byStatus,
						this->m_Occupied_StoneStatus[i].m_nIndex,
						this->m_Occupied_StoneStatus[i].byMapTagIndex);

				}
			}
		}
	}
}

void CIllusionTempleProcess_Renewal::RegisterRelics(OBJECTSTRUCT *lpNpc, OBJECTSTRUCT *lpObj)
{
	if (this->GetIllusionTempleState() != 3)
	{
		this->ResetUserFlag_DoingRelicsThing(lpObj);
		return;
	}

	if (lpNpc->m_State == 0)
	{
		this->ResetUserFlag_DoingRelicsThing(lpObj);
		return;
	}

	if (lpNpc->m_PosNum == -1)
	{
		this->ResetUserFlag_DoingRelicsThing(lpObj);
		return;
	}

	if (abs(lpObj->Y - lpNpc->Y) > 3)
	{
		this->ResetUserFlag_DoingRelicsThing(lpObj);
		return;
	}

	if (abs(lpObj->X - lpNpc->X) > 3)
	{
		this->ResetUserFlag_DoingRelicsThing(lpObj);
		return;
	}

	int nRegisterRelicsUserArray = this->FindITRUser(lpObj->m_Index);

	if (nRegisterRelicsUserArray == -1)
	{
		this->ResetUserFlag_DoingRelicsThing(lpObj);
		return;
	}

	if (this->m_UserData[nRegisterRelicsUserArray].m_nRelicsInvenPos > 0 &&
		lpObj->pInventory[this->m_UserData[nRegisterRelicsUserArray].m_nRelicsInvenPos].m_Type == ITEMGET(14, 223))
	{
		bool bFail = false;

		if (lpObj->m_bITR_RegisteringRelics == false)
		{
			bFail = true;
		}

		if (lpObj->m_wITR_NpcType < 0)
		{
			bFail = true;
		}

		if (lpObj->m_byITR_StoneState == 99)
		{
			bFail = true;
		}

		if (bFail == true)
		{
			this->ResetUserFlag_DoingRelicsThing(lpObj);
		}

		else
		{
			if (lpObj->m_nITR_RelicsTick <= GetTickCount())
			{
				if (lpNpc->Class >= 659 && lpNpc->Class <= 668)
				{
					for (int i = 0; i < 10; i++)
					{
						if (this->m_Occupied_StoneStatus[i].m_wType == lpNpc->Class &&
							this->m_UserData[nRegisterRelicsUserArray].m_btTeam == this->m_Occupied_StoneStatus[i].m_byStatus - 100)
						{
							this->m_Occupied_StoneStatus[i].m_byStatus = this->m_UserData[nRegisterRelicsUserArray].m_btTeam;
							this->m_UserData[nRegisterRelicsUserArray].m_byOccupiedCount++;

							this->SendRelicsError(lpNpc, lpObj->m_Index, 0, 4);
							this->Send_ITR_StoneInfo(lpObj->m_Index,
								this->m_Occupied_StoneStatus[i].m_wType,
								this->m_Occupied_StoneStatus[i].m_byStatus,
								this->m_Occupied_StoneStatus[i].m_nIndex,
								this->m_Occupied_StoneStatus[i].byMapTagIndex);

							lpObj->m_bITR_RegisteringRelics = 0;

							if (this->m_UserData[nRegisterRelicsUserArray].m_nRelicsInvenPos > 0)
							{
								if (lpObj->pInventory[this->m_UserData[nRegisterRelicsUserArray].m_nRelicsInvenPos].m_Type == ITEMGET(14, 223))
								{
									this->SendRelicsUserInfo(lpObj, 1);
									g_Log.Add("[ ITR ][RegisterRelics] (%d) (%s)(%s) to Delete Relics Item [Serial:%I64d]",
										this->m_nTempleNumber + 1, lpObj->AccountID, lpObj->Name, lpObj->pInventory[this->m_UserData[nRegisterRelicsUserArray].m_nRelicsInvenPos].m_Number);

									gObjInventoryDeleteItem(lpObj->m_Index, this->m_UserData[nRegisterRelicsUserArray].m_nRelicsInvenPos);
									GSProtocol.GCInventoryItemDeleteSend(lpObj->m_Index, this->m_UserData[nRegisterRelicsUserArray].m_nRelicsInvenPos, 0);
									this->m_UserData[nRegisterRelicsUserArray].m_nRelicsInvenPos = -1;
								}
							}

							break;

						}
					}

					this->Set_ITRState(4);
					this->ResetUserFlag_DoingRelicsThing(lpObj);
				}

				else
				{
					this->SendRelicsError(lpNpc, lpObj->m_Index, 20, -1);
					this->ResetUserFlag_DoingRelicsThing(lpObj);
				}
			}

			else
			{
				this->SendRelicsError(lpNpc, lpObj->m_Index, 18, -1);
				this->ResetUserFlag_DoingRelicsThing(lpObj);
			}
		}
	}

	else
	{
		this->SendRelicsError(lpNpc, lpObj->m_Index, 17, -1);
		this->ResetUserFlag_DoingRelicsThing(lpObj);
	}
}

void CIllusionTempleProcess_Renewal::FirstRegen_OccupiedStone()
{
	for (int i = 0; i < 10; i++)
	{
		LPOBJ lpObj = &gObj[this->m_Occupied_StoneStatus[i].m_nIndex];

		if (lpObj)
		{
			lpObj->m_PosNum = this->m_Occupied_StoneStatus[i].m_wPosNum;
			lpObj->Live = TRUE;
			lpObj->DieRegen = 0;
			lpObj->m_State = 1;
			this->m_Occupied_StoneStatus[i].m_byStatus = -1;
		}
	}

	this->m_bFirstRegen_OccStone = true;
}

void CIllusionTempleProcess_Renewal::FirstRegen_CursedStone()
{
	LPOBJ lpObj = &gObj[this->m_Cursed_StoneStatus.m_nIndex];

	if (lpObj)
	{
		lpObj->m_PosNum = this->m_Cursed_StoneStatus.m_wPosNum;
		lpObj->Live = TRUE;
		lpObj->DieRegen = 0;
		lpObj->m_State = 1;
		this->m_Cursed_StoneStatus.m_byStatus = 0;
		this->m_bFirstRegen_CursedStone = true;
	}
}

void CIllusionTempleProcess_Renewal::RegenCursedStoneStatus(BYTE btRound)
{
	BYTE byOccupiedStoneCnt = 0;

	for (int i = 0; i < 10; i++)
	{
		if (this->m_Occupied_StoneStatus[i].m_byStatus == 0 ||
			this->m_Occupied_StoneStatus[i].m_byStatus == 1)
		{
			byOccupiedStoneCnt++;
		}
	}

	LPOBJ lpObj;

	if ((rand() % 100) > (100 - 10 * byOccupiedStoneCnt))
	{
		this->m_Cursed_StoneStatus.m_byStatus = -1;
		lpObj = &gObj[this->m_Cursed_StoneStatus.m_nIndex];
		lpObj->m_PosNum = this->m_Cursed_StoneStatus.m_wPosNum;
		lpObj->DieRegen = 2;
		lpObj->Live = 1;
		lpObj->m_State = 16;
	}
	else
	{
		this->m_Cursed_StoneStatus.m_byStatus = 0;
		lpObj = &gObj[this->m_Cursed_StoneStatus.m_nIndex];
		lpObj->m_PosNum = this->m_Cursed_StoneStatus.m_wPosNum;
		lpObj->Live = 1;
		lpObj->DieRegen = 0;
		lpObj->m_State = 1;
	}

	this->Send_ITR_StoneInfo(0, this->m_Cursed_StoneStatus.m_wType,
		this->m_Cursed_StoneStatus.m_byStatus,
		this->m_Cursed_StoneStatus.m_nIndex,
		this->m_Cursed_StoneStatus.byMapTagIndex);

	g_Log.Add("[ ITR ][RegenCursedStoneStatus] Create NPC Status Map:%d, Round:%d,RegenStatus:%d, X:%d,Y:%d",
		this->m_nTempleNumber + 1, btRound, this->m_Cursed_StoneStatus.m_byStatus, lpObj->X, lpObj->Y);
}

void CIllusionTempleProcess_Renewal::SetStatusRegenTime()
{
	this->m_StatusRegenTime = GetTickCount() + 5000;
}

void CIllusionTempleProcess_Renewal::AddITROccupiedStonePos(WORD wNpcType, int index, int Count, BYTE byMapTagIndex)
{
	if (wNpcType < 659 || wNpcType > 668)
	{
		return;
	}

	if (this->m_Occupied_StoneStatus[Count].m_nIndex == -1)
	{
		this->m_Occupied_StoneStatus[Count].m_nIndex = index;
		this->m_Occupied_StoneStatus[Count].m_wType = wNpcType;
		this->m_Occupied_StoneStatus[Count].m_wPosNum = Count;
		this->m_Occupied_StoneStatus[Count].m_byStatus = -1;
		this->m_Occupied_StoneStatus[Count].byMapTagIndex = byMapTagIndex;

		g_Log.Add("[ ITR ][AddITROccupiedStonePos] ArrayNUm :%d wNpcType:%d, index:%d ",
			Count, wNpcType, index);
	}
}

void CIllusionTempleProcess_Renewal::AddITRCursedStonePos(WORD wNpcType, int index, BYTE byMapTagIndex)
{
	if (wNpcType != 658)
	{
		return;
	}

	this->m_Cursed_StoneStatus.m_wType = wNpcType;
	this->m_Cursed_StoneStatus.m_nIndex = index;
	this->m_Cursed_StoneStatus.m_wPosNum = 0;
	this->m_Cursed_StoneStatus.m_byStatus = 0;
	this->m_Cursed_StoneStatus.byMapTagIndex = byMapTagIndex;
	g_Log.Add("[ ITR ][AddITRCursedStonePos] NpcType:%d, index :%d ", wNpcType, index);
}

void CIllusionTempleProcess_Renewal::SetNpc()
{
	int nOccCount = 0;

	for (int i = 0; i < 11; i++)
	{
		int result = gObjAddMonster(this->m_ITRNpcInfo[i].byMapNumber);

		if (result > 0)
		{
			gObj[result].m_PosNum = i;
			gObj[result].X = this->m_ITRNpcInfo[i].byX;
			gObj[result].Y = this->m_ITRNpcInfo[i].byY;
			gObj[result].MapNumber = this->m_ITRNpcInfo[i].byMapNumber;
			gObj[result].TX = gObj[result].X;
			gObj[result].TY = gObj[result].Y;
			gObj[result].m_OldX = gObj[result].X;
			gObj[result].m_OldY = gObj[result].Y;
			gObj[result].Dir = this->m_ITRNpcInfo[i].byDir;
			gObj[result].StartX = gObj[result].X;
			gObj[result].StartY = gObj[result].Y;
			gObjSetMonster(result, this->m_ITRNpcInfo[i].wNpcType);

			if (this->m_ITRNpcInfo[i].wNpcType == 658)
			{
				this->AddITRCursedStonePos(this->m_ITRNpcInfo[i].wNpcType, result, this->m_ITRNpcInfo[i].byMapTagIndex);
			}

			else if (this->m_ITRNpcInfo[i].wNpcType >= 659 && this->m_ITRNpcInfo[i].wNpcType <= 668)
			{
				this->AddITROccupiedStonePos(this->m_ITRNpcInfo[i].wNpcType, result, nOccCount, this->m_ITRNpcInfo[i].byMapTagIndex);
				nOccCount++;
			}
		}
	}

	this->m_bIsSetNpc = true;
}

void CIllusionTempleProcess_Renewal::RegenOccupiedStoneStatus()
{
	for (int i = 0; i < 10; i++)
	{
		if (this->m_Occupied_StoneStatus[i].m_nIndex != -1)
		{
			LPOBJ lpObj = &gObj[this->m_Occupied_StoneStatus[i].m_nIndex];

			lpObj->m_PosNum = this->m_Occupied_StoneStatus[i].m_wPosNum;
			lpObj->Live = TRUE;
			lpObj->DieRegen = FALSE;
			lpObj->m_State = 1;

			if (this->m_Occupied_StoneStatus[i].m_byStatus == 10 ||
				this->m_Occupied_StoneStatus[i].m_byStatus == 11 ||
				this->m_Occupied_StoneStatus[i].m_byStatus == 100 ||
				this->m_Occupied_StoneStatus[i].m_byStatus == 101 ||
				this->m_Occupied_StoneStatus[i].m_byStatus == 254)
			{
				this->m_Occupied_StoneStatus[i].m_byStatus = -1;
				this->Send_ITR_StoneInfo(0, this->m_Occupied_StoneStatus[i].m_wType,
					this->m_Occupied_StoneStatus[i].m_byStatus, this->m_Occupied_StoneStatus[i].m_nIndex,
					this->m_Occupied_StoneStatus[i].byMapTagIndex);

				g_Log.Add("[ ITR ][RegenOccupiedStoneStatus] Round [%d] NpcIndex:[%d],Type:[%d] MAKE NEUTRAL",
					this->m_byITRRound, this->m_Occupied_StoneStatus[i].m_nIndex, this->m_Occupied_StoneStatus[i].m_wType);
			}
		}
	}

	this->m_StatusRegenTime = -1;
}

void CIllusionTempleProcess_Renewal::SendNoticeMessage(char *lpMsg)
{
	PMSG_NOTICE pNotice;
	TNotice::MakeNoticeMsg(&pNotice, 0, lpMsg);

	for (int i = 0; i < 10; i++)
	{
		if (this->m_UserData[i].m_nIndex != -1)
		{
			if (gObj[this->m_UserData[i].m_nIndex].Connected > PLAYER_LOGGED)
			{
				IOCP.DataSend(this->m_UserData[i].m_nIndex, (LPBYTE)&pNotice, pNotice.h.size);
			}
		}
	}
}

void CIllusionTempleProcess_Renewal::Send_ITR_StoneInfo(int userindex, WORD wNpcType, BYTE byState, int NpcIndex, BYTE byMapTagIndex)
{
	PMSG_ITL_STONEINFO pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0xBF, 0x6C, sizeof(pMsg));

	pMsg.byMapTagIndex = byMapTagIndex;
	pMsg.wOccupiedStoneType = wNpcType;
	pMsg.byOccupiedStoneState = byState;
	pMsg.nNpcIndex = NpcIndex;

	for (int i = 0; i < 10; i++)
	{
		if (this->m_UserData[i].m_nIndex != -1)
		{
			if (IT_MAP_RANGE(gObj[this->m_UserData[i].m_nIndex].MapNumber))
			{
				IOCP.DataSend(this->m_UserData[i].m_nIndex, (LPBYTE)&pMsg, pMsg.h.size);
				g_Log.Add("[ ITR ][Send_ITR_StoneInfo] userindex:%d, npcType:%d, npcstate:%d ,npcindex:%d,MAPTAG:%d",
					this->m_UserData[i].m_nIndex, wNpcType, byState, NpcIndex, byMapTagIndex);
			}
		}
	}
}

void CIllusionTempleProcess_Renewal::Send_ITR_PlayState(BYTE btTeam)
{
	if (this->m_nRemainTimeMsec < 0)
	{
		this->m_nRemainTimeMsec = 0;
	}

	PMSG_ITL_PLAY_STATE pMsg;

	BYTE byAllied, byIllusion;
	this->GetOccupiedStoneCount(byAllied, byIllusion);

	pMsg.wRemainSec = this->m_nRemainTimeMsec / 1000;
	pMsg.byAlliedPoint = byAllied;
	pMsg.btIllusionPoint = byIllusion;
	pMsg.btMyTeam = btTeam;
	PHeadSubSetB((LPBYTE)&pMsg, 0xBF, 0x62, sizeof(pMsg));

	for (int i = 0; i < 10; i++)
	{
		if (this->m_UserData[i].m_nIndex != -1 && gObj[this->m_UserData[i].m_nIndex].Connected > PLAYER_LOGGED)
		{
			if (this->m_UserData[i].m_btTeam == btTeam)
			{
				IOCP.DataSend(this->m_UserData[i].m_nIndex, (LPBYTE)&pMsg, pMsg.h.size);
			}
		}
	}
}

void CIllusionTempleProcess_Renewal::SendIllusionTempleState(BYTE btTempleState, int nIndex)
{
	PMSG_ITL_STATE pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0xBF, 0x60, sizeof(pMsg));

	pMsg.btTempleNumber = this->m_nTempleNumber;
	pMsg.btITLState = btTempleState;

	if (nIndex == -1)
	{
		for (int i = 0; i < 10; i++)
		{
			if (this->m_UserData[i].m_nIndex != -1 && gObj[this->m_UserData[i].m_nIndex].Connected > PLAYER_LOGGED)
			{
				IOCP.DataSend(this->m_UserData[i].m_nIndex, (LPBYTE)&pMsg, pMsg.h.size);
			}
		}
	}

	else
	{
		IOCP.DataSend(nIndex, (LPBYTE)&pMsg, pMsg.h.size);
	}
}

BYTE CIllusionTempleProcess_Renewal::GetUserTeam(int nIndex)
{
	if (!ObjectMaxRange(nIndex))
	{
		g_Log.Add("return %s %d", __FILE__, __LINE__);
		return -1;
	}

	if (gObj[nIndex].m_nITR_Index == -1 || this->m_UserData[gObj[nIndex].m_nITR_Index].m_nIndex != nIndex)
	{
		return -1;
	}

	if (gObj[nIndex].Connected < PLAYER_PLAYING)
	{
		return -1;
	}

	return this->m_UserData[gObj[nIndex].m_nITR_Index].m_btTeam;
}

void CIllusionTempleProcess_Renewal::IncUserKillCount(int nIndex)
{
	if (!ObjectMaxRange(nIndex))
	{
		g_Log.Add("return %s %d", __FILE__, __LINE__);
		return;
	}

	if (gObj[nIndex].m_nITR_Index == -1 || this->m_UserData[gObj[nIndex].m_nITR_Index].m_nIndex != nIndex)
	{
		return;
	}

	if (gObj[nIndex].Connected < PLAYER_PLAYING)
	{
		return;
	}

	if (this->m_UserData[gObj[nIndex].m_nITR_Index].m_btUserKillCount >= 255)
	{
		return;
	}

	g_Log.Add("[ ITR ][UserKill] map:%d, ACC:%s, NAME:%s ,KillCnt:%d -> %d",
		this->m_nTempleNumber + 1, gObj[nIndex].AccountID, gObj[nIndex].Name,
		this->m_UserData[gObj[nIndex].m_nITR_Index].m_btUserKillCount, this->m_UserData[gObj[nIndex].m_nITR_Index].m_btUserKillCount + 1);

	this->m_UserData[gObj[nIndex].m_nITR_Index].m_btUserKillCount++;
}

void CIllusionTempleProcess_Renewal::ResetKillCount()
{
	for (int i = 0; i < 10; i++)
	{
		this->m_UserData[i].m_btUserKillCount = 0;
	}
}

void CIllusionTempleProcess_Renewal::ITR_UseSkill(int nIndex, WORD wSkillNumber, int nTargetIndex, BYTE btDis)
{
	if (!ObjectMaxRange(nIndex))
	{
		g_Log.Add("return %s %d", __FILE__, __LINE__);
		return;
	}

	if (!ObjectMaxRange(nTargetIndex))
	{
		g_Log.Add("return %s %d", __FILE__, __LINE__);
		return;
	}

	LPOBJ lpObj = &gObj[nIndex];
	LPOBJ lpTargetObj = &gObj[nTargetIndex];

	if (this->GetIllusionTempleState() != 2)
	{
		g_Log.Add("Process ITR_UseSkill Error #1 iIndex :%d Name:%s", nIndex, gObj[nIndex].Name);
		this->SendUseSkillResult(nIndex, nTargetIndex, wSkillNumber, 0);

		return;
	}

	if (lpObj->Connected < PLAYER_PLAYING || lpTargetObj->Connected < PLAYER_PLAYING)
	{
		g_Log.Add("Process ITR_UseSkill Error #2 iIndex :%d Name:%s", nIndex, gObj[nIndex].Name);
		this->SendUseSkillResult(nIndex, nTargetIndex, wSkillNumber, 0);

		return;
	}

	if (lpObj->MapNumber != lpTargetObj->MapNumber)
	{
		g_Log.Add("Process ITR_UseSkill Error #3 iIndex :%d Name:%s", nIndex, gObj[nIndex].Name);
		this->SendUseSkillResult(nIndex, nTargetIndex, wSkillNumber, 0);

		return;
	}

	if (lpObj->m_nITR_Index == -1 || lpTargetObj->m_nITR_Index == -1)
	{
		g_Log.Add("Process ITR_UseSkill Error #4 iIndex :%d Name:%s", nIndex, gObj[nIndex].Name);
		this->SendUseSkillResult(nIndex, nTargetIndex, wSkillNumber, 0);

		return;
	}

	if (nIndex == nTargetIndex && (wSkillNumber == 211 || wSkillNumber == 213))
	{
		g_Log.Add("Process ITR_UseSkill Error #5 iIndex :%d Name:%s", nIndex, gObj[nIndex].Name);
		this->SendUseSkillResult(nIndex, nTargetIndex, wSkillNumber, 0);

		return;
	}

	int ret;

	switch (wSkillNumber)
	{
	case 210:
		ret = this->UseSkillProdection(lpObj);
		break;
	case 211:
		ret = this->UseSkillRestraint(lpObj, lpTargetObj);
		break;
	case 212:
		ret = this->UseSkillTeleport(lpObj);
		break;
	case 213:
		ret = this->UseSkillShieldBurn(lpObj, lpTargetObj);
		break;
	default:
		ret = 0;
		break;
	}

	if (ret == TRUE)
	{
		g_Log.Add("[ ITR ] (%d) Use Skill (%d), (%s)(%s), (%d)",
			this->m_nTempleNumber + 1, wSkillNumber, lpObj->AccountID, lpObj->Name, -1);
	}

	if (wSkillNumber != 212)
	{
		if (ret == TRUE)
		{
			this->SendUseSkillResult(nIndex, nTargetIndex, wSkillNumber, TRUE);
		}

		else
		{
			g_Log.Add("Process ITR_UseSkill Error #6 iIndex :%d Name:%s", nIndex, gObj[nIndex].Name);
		}
	}

	if (ret == FALSE)
	{
		g_Log.Add("Process ITR_UseSkill Error #7 iIndex :%d Name:%s", nIndex, gObj[nIndex].Name);
		this->SendUseSkillResult(nIndex, nTargetIndex, wSkillNumber, FALSE);
	}
}

void CIllusionTempleProcess_Renewal::ITRSkillProc(LPOBJ lpObj)
{
	if (this->m_UserData[lpObj->m_nITR_Index].m_nIndex != lpObj->m_Index)
	{
		return;
	}

	if (this->m_UserData[lpObj->m_nITR_Index].m_dwSkillProdectionTime != 0)
	{
		this->m_UserData[lpObj->m_nITR_Index].m_dwSkillProdectionTime--;

		if (this->m_UserData[lpObj->m_nITR_Index].m_dwSkillProdectionTime == 0)
		{
			this->m_UserData[lpObj->m_nITR_Index].m_dwSkillProdectionTime = 0;
			gObjRemoveBuffEffect(lpObj, BUFFTYPE_TEMPLE_PROTECTION);
			this->Send_ITR_SkillEnd(lpObj, 210);
		}
	}

	if (this->m_UserData[lpObj->m_nITR_Index].m_dwSkillRestraintTime != 0)
	{
		this->m_UserData[lpObj->m_nITR_Index].m_dwSkillRestraintTime--;

		if (this->m_UserData[lpObj->m_nITR_Index].m_dwSkillRestraintTime == 0)
		{
			this->m_UserData[lpObj->m_nITR_Index].m_dwSkillRestraintTime = 0;
			gObjRemoveBuffEffect(lpObj, BUFFTYPE_RESTRICTION);
			this->Send_ITR_SkillEnd(lpObj, 211);
		}
	}
}

void CIllusionTempleProcess_Renewal::Send_ITR_SkillEnd(OBJECTSTRUCT *lpObj, WORD wSkillNumber)
{
	PMSG_ITL_SKILL_END pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0xBF, 0x63, sizeof(pMsg));

	pMsg.MagicNumberH = HIBYTE(wSkillNumber);
	pMsg.MagicNumberL = LOBYTE(wSkillNumber);
	pMsg.wObjIndex = lpObj->m_Index;

	IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
	GSProtocol.MsgSendV2(lpObj, (LPBYTE)&pMsg, pMsg.h.size);
}

int CIllusionTempleProcess_Renewal::UseSkillProdection(LPOBJ lpObj)
{
	this->m_UserData[lpObj->m_nITR_Index].m_dwSkillProdectionTime = 10;
	gObjAddBuffEffect(lpObj, BUFFTYPE_TEMPLE_PROTECTION, 0, 0, 0, 0, -10);
	return TRUE;
}

int CIllusionTempleProcess_Renewal::UseSkillRestraint(LPOBJ lpObj, LPOBJ lpTargetObj)
{
	if (gCheckSkillDistance(lpObj->m_Index, lpTargetObj->m_Index, 211) == FALSE)
	{
		return FALSE;
	}

	this->m_UserData[lpTargetObj->m_nITR_Index].m_dwSkillRestraintTime = 10;
	lpTargetObj->lpAttackObj = lpObj;
	gObjAddBuffEffect(lpTargetObj, BUFFTYPE_RESTRICTION, 0, 0, 0, 0, -10);
	lpTargetObj->PathCount = 0;
	lpTargetObj->PathStartEnd = 0;
	gObjSetPosition(lpTargetObj->m_Index, lpTargetObj->X, lpTargetObj->Y);
	return TRUE;
}

int CIllusionTempleProcess_Renewal::UseSkillTeleport(LPOBJ lpObj)
{
	if (gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_STUN) == TRUE)
	{
		g_Log.Add("UseSkillTeleport Error #1 iIndex :%d Name:%s", lpObj->m_Index, lpObj->Name);
		this->SendUseSkillResult(lpObj->m_Index, lpObj->m_Index, 212, 0);

		return FALSE;
	}

	if (gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_SLEEP) == TRUE)
	{
		g_Log.Add("UseSkillTeleport Error #2 iIndex :%d Name:%s", lpObj->m_Index, lpObj->Name);
		this->SendUseSkillResult(lpObj->m_Index, lpObj->m_Index, 212, 0);

		return FALSE;
	}

	int tmpRelicsUserIndex = -1;
	int RelicsHaveUserCount = 0;

	for (int i = 0; i < 10; i++)
	{
		if (this->m_UserData[i].m_nRelicsInvenPos > 0)
		{
			RelicsHaveUserCount++;
		}
	}

	if (RelicsHaveUserCount > 0)
	{
		int nRand = rand() % RelicsHaveUserCount;

		if (nRand < 10)
		{
			int nRealCount = 0;

			for (int j = 0; j < 10; j++)
			{
				if (this->m_UserData[j].m_nRelicsInvenPos > 0)
				{
					if (nRealCount == nRand)
					{
						tmpRelicsUserIndex = this->m_UserData[j].m_nIndex;
					}

					nRealCount++;
				}
			}
		}
	}

	if (tmpRelicsUserIndex == -1)
	{
		g_Log.Add("UseSkillTeleport Error #3 iIndex :%d Name:%s", lpObj->m_Index, lpObj->Name);
		this->SendUseSkillResult(lpObj->m_Index, lpObj->m_Index, 212, 0);

		return FALSE;
	}

	if (tmpRelicsUserIndex == lpObj->m_Index)
	{
		g_Log.Add("UseSkillTeleport Error #4 iIndex :%d Name:%s", lpObj->m_Index, lpObj->Name);
		this->SendUseSkillResult(lpObj->m_Index, lpObj->m_Index, 212, 0);

		return FALSE;
	}

	this->SendUseSkillResult(lpObj->m_Index, lpObj->m_Index, 212, TRUE);

	int addx = 0;
	int addy = 0;
	int count = 50; //ebp14
	BOOL Find = FALSE;

	while (count-- != 0)
	{
		addx = gObj[tmpRelicsUserIndex].X + rand() % 3 - 1;
		addy = gObj[tmpRelicsUserIndex].Y + rand() % 3 - 1;

		if (gObjCheckTeleportArea(lpObj->m_Index, addx, addy))
		{
			Find = TRUE;
			break;
		}
	}

	if (Find == FALSE)
	{
		addx = gObj[tmpRelicsUserIndex].X;
		addy = gObj[tmpRelicsUserIndex].Y;
	}

	gObjTeleportMagicUse(lpObj->m_Index, addx, addy);
	return TRUE;
}

int CIllusionTempleProcess_Renewal::UseSkillShieldBurn(LPOBJ lpObj, LPOBJ lpTargetObj)
{
	if (gCheckSkillDistance(lpObj->m_Index, lpTargetObj->m_Index, 213) == FALSE)
	{
		return FALSE;
	}

	lpTargetObj->lpAttackObj = lpObj;

	if (lpTargetObj->iShield <= 1)
	{
		GSProtocol.GCDamageSend(lpObj->m_Index, lpTargetObj->m_Index, 0, 0, 0, 0);
		return TRUE;
	}

	int nShieldDamage = lpTargetObj->iShield / 2;

	lpTargetObj->iShield = nShieldDamage;

	GSProtocol.GCDamageSend(lpObj->m_Index, lpTargetObj->m_Index, 0, 0, 0, nShieldDamage);
	return TRUE;
}

void CIllusionTempleProcess_Renewal::SendUseSkillResult(int nIndex, int nTargetIndex, WORD wSkillNumber, BYTE btResult)
{
	PMSG_ILLUSION_TEMPLE_USE_MAGIC_RESULT pResult;
	PHeadSubSetB((LPBYTE)&pResult, 0xBF, 0x02, sizeof(pResult));

	pResult.btResult = btResult;
	pResult.MagicNumberH = HIBYTE(wSkillNumber);
	pResult.MagicNumberL = LOBYTE(wSkillNumber);
	pResult.wSourceObjIndex = nIndex;
	pResult.wTargetObjIndex = nTargetIndex;

	IOCP.DataSend(nIndex, (LPBYTE)&pResult, pResult.h.size);

	if (btResult == TRUE)
	{
		GSProtocol.MsgSendV2(&gObj[nIndex], (LPBYTE)&pResult, pResult.h.size);
	}

	g_Log.Add("[ ITR ] SendUseSkillResult Success:[%d], Index:[%d], Target:[%d], Skill:[%d] ",
		btResult, nIndex, nTargetIndex, wSkillNumber);
}

void CIllusionTempleProcess_Renewal::ITRUserDie(LPOBJ lpObj)
{
	if (lpObj->m_nITR_Index == -1)
	{
		return;
	}

	if (this->m_UserData[lpObj->m_nITR_Index].m_nIndex != lpObj->m_Index)
	{
		return;
	}

	this->DropRelicsItem(lpObj->m_Index, 1);

	if (this->m_UserData[lpObj->m_nITR_Index].m_dwSkillProdectionTime != 0)
	{
		this->m_UserData[lpObj->m_nITR_Index].m_dwSkillProdectionTime = 0;
		gObjRemoveBuffEffect(lpObj, BUFFTYPE_TEMPLE_PROTECTION);
		this->Send_ITR_SkillEnd(lpObj, 210);
	}

	if (this->m_UserData[lpObj->m_nITR_Index].m_dwSkillRestraintTime != 0)
	{
		this->m_UserData[lpObj->m_nITR_Index].m_dwSkillRestraintTime = 0;
		gObjRemoveBuffEffect(lpObj, BUFFTYPE_RESTRICTION);
		this->Send_ITR_SkillEnd(lpObj, 211);
	}
}

void CIllusionTempleProcess_Renewal::GetOccupiedStoneCount(BYTE &byAllied, BYTE &byIllusion)
{
	byAllied = 0;
	byIllusion = 0;

	for (int i = 0; i < 10; i++)
	{
		if (this->m_Occupied_StoneStatus[i].m_byStatus == 0)
		{
			byAllied++;
		}

		else if (this->m_Occupied_StoneStatus[i].m_byStatus == 1)
		{
			byIllusion++;
		}
	}
}

void CIllusionTempleProcess_Renewal::CalCursetTempleResult()
{
	this->CheckUsersOnConnect();

	int nAlliedExp = 0;
	int nIllusionExp = 0;
	BYTE byWinnerGuildIndex = -1;
	BYTE byLoseGuildIndex = -1;
	BYTE byWinGuildOccupiedCount = 0;
	BYTE byLoseGuildOccupiedCount = 0;
	BYTE byAlliedCnt = this->m_byAlliedTeamPoint;
	BYTE byIllusionCnt = this->m_byIllusionTeamPoint;

	if (byAlliedCnt <= byIllusionCnt)
	{
		if (byAlliedCnt >= byIllusionCnt)
		{
			if (byAlliedCnt == byIllusionCnt)
			{
				BYTE byAlliedTeamKillSum = 0;
				BYTE byIllusionTeamKillSum = 0;

				this->CalcKillCountByTeam(byAlliedTeamKillSum, byIllusionTeamKillSum);

				if (byAlliedTeamKillSum <= byIllusionTeamKillSum)
				{
					if (byAlliedTeamKillSum >= byIllusionTeamKillSum)
					{
						int nAlliedLevelSum = 0;
						int nIllusionLevelSum = 0;

						for (int i = 0; i < 10; i++)
						{
							if (this->m_UserData[i].m_btTeam == 0)
							{
								nAlliedLevelSum += (gObj[this->m_UserData[i].m_nIndex].Level + gObj[this->m_UserData[i].m_nIndex].m_PlayerData->MasterLevel);
							}

							else if (this->m_UserData[i].m_btTeam == 1)
							{
								nIllusionLevelSum += (gObj[this->m_UserData[i].m_nIndex].Level + gObj[this->m_UserData[i].m_nIndex].m_PlayerData->MasterLevel);
							}
						}

						if (nAlliedLevelSum <= nIllusionLevelSum)
						{
							if (nAlliedLevelSum < nIllusionLevelSum)
							{
								this->m_btWinTeam = 0;
								this->m_btLoseTeam = 1;
								byWinGuildOccupiedCount = byAlliedCnt;
								byLoseGuildOccupiedCount = byIllusionCnt;
								byWinnerGuildIndex = 0;
								byLoseGuildIndex = 1;
							}
						}

						else
						{
							this->m_btWinTeam = 1;
							this->m_btLoseTeam = 0;
							byWinGuildOccupiedCount = byIllusionCnt;
							byLoseGuildOccupiedCount = byAlliedCnt;
							byWinnerGuildIndex = 1;
							byLoseGuildIndex = 0;
						}
					}

					else
					{
						this->m_btWinTeam = 1;
						this->m_btLoseTeam = 0;
						byWinGuildOccupiedCount = byIllusionCnt;
						byLoseGuildOccupiedCount = byAlliedCnt;
						byWinnerGuildIndex = 1;
						byLoseGuildIndex = 0;
					}
				}

				else
				{
					this->m_btWinTeam = 0;
					this->m_btLoseTeam = 1;
					byWinGuildOccupiedCount = byAlliedCnt;
					byLoseGuildOccupiedCount = byIllusionCnt;
					byWinnerGuildIndex = 0;
					byLoseGuildIndex = 1;
				}
			}
		}

		else
		{
			this->m_btWinTeam = 1;
			this->m_btLoseTeam = 0;
			byWinGuildOccupiedCount = byIllusionCnt;
			byLoseGuildOccupiedCount = byAlliedCnt;
			byWinnerGuildIndex = 1;
			byLoseGuildIndex = 0;
		}
	}

	else
	{
		this->m_btWinTeam = 0;
		this->m_btLoseTeam = 1;
		byWinGuildOccupiedCount = byAlliedCnt;
		byLoseGuildOccupiedCount = byIllusionCnt;
		byWinnerGuildIndex = 0;
		byLoseGuildIndex = 1;
	}

	if (byWinnerGuildIndex != 255 && byLoseGuildIndex != 255)
	{
		g_Log.Add("[ ITR ] (%d) Result (WinTeam: %d)(Score:[%d][%d])", this->m_nTempleNumber + 1, this->m_btWinTeam, byAlliedCnt, byIllusionCnt);
	}

	else
	{
		g_Log.Add("[ ITR ] (%d) Result (Team: %d)(Score:[%d][%d]) byWinnerGuildIndex :%d,byLoseGuildIndex : %d ", this->m_nTempleNumber + 1, this->m_btWinTeam, byAlliedCnt, byIllusionCnt, byWinnerGuildIndex, byLoseGuildIndex);
		this->m_bNobodyWinLose = true;
	}
}

void CIllusionTempleProcess_Renewal::DropITR_RewardBox(int nIndex)
{
	if (!ObjectMaxRange(nIndex))
	{
		return;
	}

	if (this->m_UserData[gObj[nIndex].m_nITR_Index].m_nIndex != nIndex)
	{
		return;
	}

	if (this->GetIllusionTempleState() != 3)
	{
		return;
	}

	if (this->m_UserData[gObj[nIndex].m_nITR_Index].m_btTeam == 0xFF)
	{
		return;
	}

	if (this->m_btWinTeam == 0xFF)
	{
		return;
	}

	if (this->m_UserData[gObj[nIndex].m_nITR_Index].m_btTeam != this->m_btWinTeam)
	{
		return;
	}

	if (this->m_UserData[gObj[nIndex].m_nITR_Index].m_bReward == true)
	{
		return;
	}

	this->m_UserData[gObj[nIndex].m_nITR_Index].m_bReward = true;

	_stGremoryCaseItem stItem;
	stItem.btStorageType = GC_STORAGE_CHARACTER;
	stItem.btRewardSource = GC_REWARD_ILLUSION_TEMPLE;
	stItem.wItemID = ItemGetNumberMake(14, this->m_nTempleNumber + 236);

	g_GremoryCase.GDReqAddItemToGremoryCase(nIndex, stItem, 3);
	g_Log.Add("[ ITR ][DropITR_RewardBox]Index:%d,Name:%s,Acc:%s", nIndex, gObj[nIndex].Name, gObj[nIndex].AccountID);

	g_QuestExpProgMng.ChkUserQuestTypeEventMap(QUESTEXP_ASK_ILLUSIONTEMPLE_CLEAR, &gObj[nIndex], this->m_nTempleNumber, 0);
	g_CashShop.AddCoin(&gObj[nIndex], EVENT_CW);
}

void CIllusionTempleProcess_Renewal::SendAllUserAnyMsg(char *lpMsg)
{
	PMSG_NOTICE pNotice;
	TNotice::MakeNoticeMsg(&pNotice, 0, lpMsg);

	for (int i = g_ConfigRead.server.GetObjectStartUserIndex(); i < g_ConfigRead.server.GetObjectMax(); i++)
	{
		if (gObj[i].Connected == PLAYER_PLAYING && gObj[i].Type == OBJ_USER)
		{
			if (!DG_MAP_RANGE(gObj[i].MapNumber))
			{
				IOCP.DataSend(i, (LPBYTE)&pNotice, pNotice.h.size);
			}
		}
	}
}

int CIllusionTempleProcess_Renewal::GetRealRemainTimeMin()
{
	if (this->GetIllusionTempleState() != 0)
	{
		return 0;
	}

	return this->m_nRemainTimeMin;
}

BOOL CIllusionTempleProcess_Renewal::CheckSkillProdection(int iIllusionTempleIndex)
{
	if (iIllusionTempleIndex < 0 || iIllusionTempleIndex > 9)
	{
		return FALSE;
	}

	if (this->m_UserData[iIllusionTempleIndex].m_nIndex != -1 && this->m_UserData[iIllusionTempleIndex].m_dwSkillProdectionTime != 0)
	{
		return TRUE;
	}

	else
	{
		return FALSE;
	}
}

BOOL CIllusionTempleProcess_Renewal::CheckSkillRestraint(int iIllusionTempleIndex)
{
	if (iIllusionTempleIndex < 0 || iIllusionTempleIndex > 9)
	{
		return FALSE;
	}

	if (this->m_UserData[iIllusionTempleIndex].m_nIndex != -1 && this->m_UserData[iIllusionTempleIndex].m_dwSkillRestraintTime != 0)
	{
		return TRUE;
	}

	else
	{
		return FALSE;
	}
}

void CIllusionTempleProcess_Renewal::SendITRResult()
{
	PMSG_ITR_RESULT pMsg;
	PMSG_ITR_USER_ADD_EXP pAddExpMsg;
	int nOffset = 0;
	char SendByte[256];

	pMsg.btUserCount = this->m_nUserCount;
	pMsg.byWinTeamIndex = this->m_btWinTeam;
	pMsg.btAlliedPoint = this->m_byAlliedTeamPoint;
	pMsg.btIllusionPoint = this->m_byIllusionTeamPoint;

	nOffset = sizeof(PMSG_ITR_RESULT);

	for (int j = 0; j < 10; j++)
	{
		if (this->m_UserData[j].m_nIndex != -1)
		{
			if (gObj[this->m_UserData[j].m_nIndex].Connected > PLAYER_LOGGED)
			{
				memcpy(pAddExpMsg.GameID, gObj[this->m_UserData[j].m_nIndex].Name, MAX_ACCOUNT_LEN);
				pAddExpMsg.btMapNumber = this->m_nTempleNumber + 45;
				pAddExpMsg.btTeam = this->m_UserData[j].m_btTeam;
				pAddExpMsg.btClass = gObj[this->m_UserData[j].m_nIndex].Class;
				pAddExpMsg.btUserKillCount = this->m_UserData[j].m_btUserKillCount;

				memcpy(&SendByte[nOffset], &pAddExpMsg, sizeof(pAddExpMsg));
				nOffset += sizeof(PMSG_ITR_USER_ADD_EXP);

				g_Log.Add("[ ITR ] MAP:(%d) ITR Result  (Account:%s, Name:%s, G.Name:%s Team:%d, Class:%d, UserKillCount:%d,InvalidationCount:%d,OccupiedCount:%d)",
					this->m_nTempleNumber + 1, gObj[this->m_UserData[j].m_nIndex].AccountID, gObj[this->m_UserData[j].m_nIndex].Name, gObj[this->m_UserData[j].m_nIndex].m_PlayerData->GuildName,
					this->m_UserData[j].m_btTeam, gObj[this->m_UserData[j].m_nIndex].m_PlayerData->DbClass, this->m_UserData[j].m_btUserKillCount, this->m_UserData[j].m_byInvalidationCount,
					this->m_UserData[j].m_byOccupiedCount);
			}
		}
	}

	PHeadSubSetB((LPBYTE)&pMsg, 0xBF, 0x04, nOffset);
	memcpy(SendByte, &pMsg, sizeof(PMSG_ITR_RESULT));

	for (int i = 0; i < 10; i++)
	{
		if (this->m_UserData[i].m_btTeam != 0xFF &&
			this->m_UserData[i].m_nIndex != -1 &&
			gObj[this->m_UserData[i].m_nIndex].Connected > PLAYER_LOGGED)
		{
			IOCP.DataSend(this->m_UserData[i].m_nIndex, (LPBYTE)SendByte, pMsg.h.size);
		}
	}

	for (int i = 0; i < 2; i++)
	{
		if (this->m_PartyTeam[i].nPartyIndex > -1)
		{
			gParty.ClearPartyAuthITR(this->m_PartyTeam[i].nPartyIndex);
		}
	}
}

void CIllusionTempleProcess_Renewal::MoveToStartPoint_WhenRoundBreak()
{
	for (int i = 0; i < 10; i++)
	{
		if (this->m_UserData[i].m_nIndex != -1 &&
			gObj[this->m_UserData[i].m_nIndex].Connected > PLAYER_LOGGED &&
			gObj[this->m_UserData[i].m_nIndex].MapNumber == this->m_nTempleNumber + 45)
		{
			if (this->m_UserData[i].m_btTeam == 0)
			{
				gObjMoveGate(this->m_UserData[i].m_nIndex, this->m_nTempleNumber + 148);
			}

			else if (this->m_UserData[i].m_btTeam == 1)
			{
				gObjMoveGate(this->m_UserData[i].m_nIndex, this->m_nTempleNumber + 154);
			}

			g_Log.Add("[ ITR ][ RoundBreak_Move2WaittingRoom] Round [%d] UserIndex :%d",
				this->m_byITRRound, this->m_UserData[i].m_nIndex);
		}
	}
}

void CIllusionTempleProcess_Renewal::CalcKillCountByTeam(BYTE &byAlliedKillSum, BYTE &byIllusionKillSum)
{
	byAlliedKillSum = 0;
	byIllusionKillSum = 0;

	for (int i = 0; i < 10; i++)
	{
		if (this->m_UserData[i].m_btTeam == 0)
		{
			byAlliedKillSum += this->m_UserData[i].m_btUserKillCount;
		}

		else if (this->m_UserData[i].m_btTeam == 1)
		{
			byIllusionKillSum += this->m_UserData[i].m_btUserKillCount;
		}
	}
}

void CIllusionTempleProcess_Renewal::ResetUserFlag_DoingRelicsThing(OBJECTSTRUCT *lpObj)
{
	if (!lpObj)
	{
		return;
	}

	g_Log.Add("[ ITR ][ResetUserFlag_DoingRelicsThing] NAME:%s, StoneState:%d, NPCTYPE:%d, GettingRelics:%d, RegisteringRelics:%d",
		lpObj->Name, lpObj->m_byITR_StoneState, lpObj->m_wITR_NpcType, lpObj->m_bITR_GettingRelics, lpObj->m_bITR_RegisteringRelics);

	lpObj->m_byITR_StoneState = 99;
	lpObj->m_wITR_NpcType = 0;
	lpObj->m_bITR_GettingRelics = 0;
	lpObj->m_bITR_RegisteringRelics = 0;
	lpObj->m_nITR_RelicsTick = 0;

}

void CIllusionTempleProcess_Renewal::CancleStoneStateProcess(WORD wNpcType, BYTE byState)
{
	int nNpcIndex = -1;
	BYTE SendState = 99;
	BYTE byMapTagIndex = -1;

	if (wNpcType == 658)
	{
		this->m_Cursed_StoneStatus.m_byStatus = 0;
		nNpcIndex = this->m_Cursed_StoneStatus.m_nIndex;
		SendState = this->m_Cursed_StoneStatus.m_byStatus;
		byMapTagIndex = this->m_Cursed_StoneStatus.byMapTagIndex;
	}

	else if (wNpcType >= 659 && wNpcType <= 668)
	{
		BYTE byGetRelicsCase = byState % 10;
		BYTE CASE = -1;

		if (byState == 10 || byState == 11)
		{
			CASE = 0;
		}

		for (int i = 0; i < 10; i++)
		{
			if (this->m_Occupied_StoneStatus[i].m_wType == wNpcType)
			{
				if (CASE != 0)
				{
					this->m_Occupied_StoneStatus[i].m_byStatus = -1;
					SendState = this->m_Occupied_StoneStatus[i].m_byStatus;
					nNpcIndex = this->m_Occupied_StoneStatus[i].m_nIndex;
					byMapTagIndex = this->m_Occupied_StoneStatus[i].byMapTagIndex;
					break;
				}

				if (byGetRelicsCase == 0 || byGetRelicsCase == 1)
				{
					if (byGetRelicsCase == 0)
					{
						byGetRelicsCase = 1;
					}

					else if (byGetRelicsCase == 1)
					{
						byGetRelicsCase = 0;
					}

					this->m_Occupied_StoneStatus[i].m_byStatus = byGetRelicsCase;
					SendState = this->m_Occupied_StoneStatus[i].m_byStatus;
					nNpcIndex = this->m_Occupied_StoneStatus[i].m_nIndex;
					byMapTagIndex = this->m_Occupied_StoneStatus[i].byMapTagIndex;
					break;
				}
			}
		}
	}

	this->Send_ITR_StoneInfo(0, wNpcType, SendState, nNpcIndex, byMapTagIndex);
}

int CIllusionTempleProcess_Renewal::GetRelicsUserIndex(OBJECTSTRUCT *lpObj)
{
	if (!lpObj)
	{
		return -1;
	}

	if (this->m_UserData[this->FindITRUser(lpObj->m_Index)].m_nRelicsInvenPos <= 0)
	{
		return -1;
	}

	return lpObj->m_Index;
}

BYTE CIllusionTempleProcess_Renewal::FindEmptyPartySlot(int partyindex, BYTE &byEmptySlot)
{
	if (partyindex < 0)
	{
		return -1;
	}

	for (int i = 0; i < 2; i++)
	{
		if (this->m_PartyTeam[i].nPartyIndex == partyindex)
		{
			return -3;
		}
	}

	for (int i = 0; i < 2; i++)
	{
		if (this->m_PartyTeam[i].nPartyIndex == -1)
		{
			byEmptySlot = i;
			return 0;
		}
	}

	return -2;
}

BYTE CIllusionTempleProcess_Renewal::SetPartyTeam(BYTE bySlotOrder, int nPartyIdx, int nUserIndex)
{
	if (bySlotOrder > 2)
	{
		return -3;
	}

	if (this->m_PartyTeam[bySlotOrder].nPartyIndex != -1)
	{
		return -4;
	}

	this->m_PartyTeam[bySlotOrder].nPartyIndex = nPartyIdx;
	return 0;
}

BYTE CIllusionTempleProcess_Renewal::DeletePartyInfo(int partyindex)
{
	for (int i = 0; i < 2; i++)
	{
		if (this->m_PartyTeam[i].nPartyIndex == partyindex)
		{
			this->m_PartyTeam[i].nPartyIndex = -1;
			this->m_PartyTeam[i].nTeamIndex = -1;
			return 0;
		}
	}

	return 1;
}

BYTE CIllusionTempleProcess_Renewal::Find_SameParty(int partyindex, int &nUserIndex)
{
	if (partyindex < 0)
	{
		return -1;
	}

	for (int i = 0; i < 2; i++)
	{
		if (this->m_PartyTeam[i].nPartyIndex == partyindex)
		{
			return 0;
		}
	}

	return 1;
}

BYTE CIllusionTempleProcess_Renewal::Find_PartySlot(int partyindex, BYTE &bySlotNum)
{
	if (partyindex < 0)
	{
		return -1;
	}

	for (int i = 0; i < 2; i++)
	{
		if (this->m_PartyTeam[i].nPartyIndex == partyindex)
		{
			bySlotNum = i;
			return 0;
		}
	}

	return 1;
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


// KanturuEntranceNPC.cpp: implementation of the CKanturuEntranceNPC class.
//	GS-N	1.00.18	JPN	0x00581670	-	Completed
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "KanturuEntranceNPC.h"
#include "Kanturu.h"
#include "KanturuBattleUserMng.h"
#include "TLog.h"
#include "KanturuUtil.h"
#include "winutil.h"
#include "configread.h"

static CKanturuUtil KANTURU_UTIL;

CKanturuEntranceNPC g_KanturuEntranceNPC;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKanturuEntranceNPC::CKanturuEntranceNPC()
{
	return;
}

CKanturuEntranceNPC::~CKanturuEntranceNPC()
{
	return;
}

struct PMSG_ANS_KANTURU_STATE_INFO
{
	PBMSG_HEAD2 h;	// C1:D1:00
	BYTE btState;	// 4
	BYTE btDetailState;	// 5
	BYTE btEnter;	// 6
	BYTE btUserCount;	// 7
	int iRemainTime;	// 8
};

void CKanturuEntranceNPC::NotifyEntranceInfo(int iIndex)
{
	PMSG_ANS_KANTURU_STATE_INFO pMsg = {0};

	PHeadSubSetB((LPBYTE)&pMsg, 0xD1, 0x00, sizeof(pMsg));
	int iCurrentState = g_Kanturu.GetKanturuState();
	int iCurrentDetailState = g_Kanturu.GetKanturuDetailState();
	pMsg.btState = iCurrentState;
	pMsg.btDetailState = iCurrentDetailState;
	pMsg.btEnter = g_Kanturu.CheckCanEnterKanturuBattle();
	pMsg.btUserCount = g_KanturuBattleUserMng.GetUserCount();
	pMsg.iRemainTime = g_Kanturu.GetRemainTime();

	KANTURU_UTIL.SendDataToUser(iIndex, (LPBYTE)&pMsg, sizeof(pMsg));
}


void CKanturuEntranceNPC::NotifyResultEnterKanturuBossMap(int iIndex)
{
	if ( !gObjIsConnectedGP(iIndex))
	{
		return;
	}

	LPOBJ lpObj = &gObj[iIndex];
	BOOL bMoveGateSuccess = FALSE;
	int iResult = g_Kanturu.CheckEnterKanturu(iIndex);

	if ( iResult == 0 )
	{
		int iCurrentState = g_Kanturu.GetKanturuState();

		if ( iCurrentState == KANTURU_STATE_BATTLE_OF_MAYA )
		{
			bMoveGateSuccess = gObjMoveGate(iIndex, 133);

			if ( bMoveGateSuccess ==TRUE )
			{
				g_KanturuBattleUserMng.AddUserData(iIndex);

				g_Log.Add("[Kanturu][User Mng] Add User [%s][%s] Current Battle User:%d State:[%d-%d] (CharInfoSave : Class=%d, Level=%d, LvPoint=%d, Exp=%u, Str=%d, Dex=%d, Vit=%d, Energy=%d, LeaderShip=%d, Map=%d, Pk=%d",
					gObj[iIndex].AccountID, gObj[iIndex].Name,
					g_KanturuBattleUserMng.GetUserCount(), g_Kanturu.GetKanturuState(),
					g_Kanturu.GetKanturuDetailState(),
					gObj[iIndex].Class, gObj[iIndex].Level, gObj[iIndex].m_PlayerData->LevelUpPoint,
					gObj[iIndex].m_PlayerData->Experience, gObj[iIndex].m_PlayerData->Strength, gObj[iIndex].m_PlayerData->Dexterity,
					gObj[iIndex].m_PlayerData->Vitality, gObj[iIndex].m_PlayerData->Energy, gObj[iIndex].Leadership, 
					gObj[iIndex].MapNumber, gObj[iIndex].m_PK_Level);

				KANTURU_UTIL.NotifyKanturuCurrentState(iIndex, 
					g_Kanturu.GetKanturuState(),g_Kanturu.GetKanturuDetailState());

				if (g_ConfigRead.server.GetServerType() != SERVER_CASTLE)
				{
					gObj[iIndex].m_PlayerData->m_bKanturuEntranceByNPC = TRUE;
				}
			}
		}
		else if ( iCurrentState == KANTURU_STATE_TOWER_OF_REFINEMENT )
		{
			bMoveGateSuccess = gObjMoveGate(iIndex, 135);

			KANTURU_UTIL.NotifyKanturuCurrentState(iIndex, 
				g_Kanturu.GetKanturuState(),g_Kanturu.GetKanturuDetailState());

			if (g_ConfigRead.server.GetServerType() != SERVER_CASTLE)
			{
				gObj[iIndex].m_PlayerData->m_bKanturuEntranceByNPC = TRUE;
			}
		}
		else
		{
			g_Log.Add("[Kanturu][Move to Tower] Error - State(%d) Character[%s][%s]",
				iCurrentState, lpObj->AccountID, lpObj->Name);
		}
	}
	else
	{
		KANTURU_UTIL.NotifyKanturuEntranceReqResult(iIndex, iResult);

		g_Log.Add("[Kanturu][Move to Tower] Fail - Result(%d) Character[%s][%s]",
			iResult, lpObj->AccountID, lpObj->Name);
		if (g_ConfigRead.server.GetServerType() != SERVER_CASTLE)
		{
			gObj[iIndex].m_PlayerData->m_bKanturuEntranceByNPC = FALSE;
		}
	}
}


//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


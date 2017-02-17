// CrywolfUtil.cpp: implementation of the CCrywolfUtil class.
//	GS-N	1.00.18	JPN	0x0056F8E0	-	Completed
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CrywolfUtil.h"
#include "DSProtocol.h"
#include "MapServerManager.h"
#include "TNotice.h"
#include "classdef.h"
#include "TLog.h"
#include "Gamemain.h"
#include "MasterLevelSkillTreeSystem.h"
#include "configread.h"

CCrywolfUtil UTIL;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCrywolfUtil::CCrywolfUtil()
{
	return;
}

CCrywolfUtil::~CCrywolfUtil()
{
	return;
}

void CCrywolfUtil::SendMapServerGroupMsg(LPSTR lpszMsg, ...)
{
	if ( !lpszMsg )
		return;

	char szBuffer[512] = "";
	va_list	pArguments;

	va_start(pArguments, lpszMsg);
	vsprintf(szBuffer, lpszMsg, pArguments);
	va_end(pArguments);

	GS_GDReqMapSvrMsgMultiCast(g_MapServerManager.GetMapSvrGroup(), szBuffer);
}

void CCrywolfUtil::SendAllUserAnyData(LPBYTE lpMsg, int iSize)
{
	for (int i = g_ConfigRead.server.GetObjectStartUserIndex(); i < g_ConfigRead.server.GetObjectMax(); i++)
	{
		if ( gObj[i].Connected == PLAYER_PLAYING )
		{
			if ( gObj[i].Type == OBJ_USER )
			{
				IOCP.DataSend(i, lpMsg, iSize);
			}
		}
	}
}


void CCrywolfUtil::SendAllUserAnyMsg(int iType, LPSTR lpszMsg, ...)
{
	if ( !lpszMsg )
		return;

	char szBuffer[512] = "";
	va_list	pArguments;

	va_start(pArguments, lpszMsg);
	vsprintf(szBuffer, lpszMsg, pArguments);
	va_end(pArguments);

	PMSG_NOTICE pNotice;

	switch ( iType )
	{
		case 1:
			TNotice::MakeNoticeMsg(&pNotice, 0, szBuffer);
			TNotice::SetNoticeProperty(&pNotice, 10, _ARGB(255, 255, 200, 80), 1, 0, 20);
			TNotice::SendNoticeToAllUser(&pNotice);
			break;

		case 2:
			TNotice::MakeNoticeMsg(&pNotice, 0, szBuffer);
			TNotice::SendNoticeToAllUser(&pNotice);
			break;
	}
}


void CCrywolfUtil::SendCrywolfUserAnyData(LPBYTE lpMsg, int iSize)
{
	for ( int i=g_ConfigRead.server.GetObjectStartUserIndex();i<g_ConfigRead.server.GetObjectMax();i++)
	{
		if ( gObj[i].Connected == PLAYER_PLAYING )
		{
			if ( gObj[i].Type == OBJ_USER )
			{
				if ( gObj[i].MapNumber == MAP_INDEX_CRYWOLF_FIRSTZONE )
				{
					IOCP.DataSend(i, lpMsg, iSize);
				}
			}
		}
	}
}


void CCrywolfUtil::SendCrywolfUserAnyMsg(int iType, LPSTR lpszMsg, ...)
{
	if ( !lpszMsg )
		return;

	char szBuffer[512] = "";
	va_list	pArguments;

	va_start(pArguments, lpszMsg);
	vsprintf(szBuffer, lpszMsg, pArguments);
	va_end(pArguments);

	PMSG_NOTICE pNotice;

	switch ( iType )
	{
		case 1:
			TNotice::MakeNoticeMsg(&pNotice, 0, szBuffer);
			TNotice::SetNoticeProperty(&pNotice, 10, _ARGB(255, 255, 200, 80), 1, 0, 20);
			break;

		case 2:
			TNotice::MakeNoticeMsg(&pNotice, 0, szBuffer);
			break;
	}

	for ( int i=g_ConfigRead.server.GetObjectStartUserIndex();i<g_ConfigRead.server.GetObjectMax();i++)
	{
		if ( gObj[i].Connected == PLAYER_PLAYING )
		{
			if ( gObj[i].Type == OBJ_USER )
			{
				if ( gObj[i].MapNumber == MAP_INDEX_CRYWOLF_FIRSTZONE )
				{
					IOCP.DataSend(i, (LPBYTE)&pNotice, pNotice.h.size);
				}
			}
		}
	}

}

#pragma warning ( disable : 4101 )
void CCrywolfUtil::SendCrywolfChattingMsg(int iObjIndex, LPSTR lpszMsg, ...)
{
	return;

	LPOBJ lpObj;
	char szBuffer[512];
	va_list pArguments;
	char szChat[60];

	for (int i;i<MaxViewportMonster;i++)
	{
		if ( lpObj->VpPlayer2[i].state != FALSE )
		{
			int iTargetNumber = lpObj->VpPlayer2[i].number;

			if ( ObjectMaxRange(iTargetNumber) != FALSE )
			{
				GSProtocol.ChatTargetSend(lpObj, szChat, iTargetNumber);
			}
		}
	}
}
#pragma warning ( default : 4101 )


int CCrywolfUtil::CrywolfMVPLevelUp(int iUserIndex, int iAddExp)
{
	if ( !ObjectMaxRange(iUserIndex ) )
		return 0;

	if(gObj[iUserIndex].Type != OBJ_USER)
	{
		return 0;
	}

	int iLEFT_EXP = 0;

	g_Log.Add("[ Crywolf ][MVP Exp.] : [%s][%s](%d) %u %d",
		gObj[iUserIndex].AccountID, gObj[iUserIndex].Name,
		gObj[iUserIndex].Level, gObj[iUserIndex].m_PlayerData->Experience,
		iAddExp);

	if ( g_MasterLevelSkillTreeSystem.IsMasterLevelUser(&gObj[iUserIndex]) == true )
	{
		return g_MasterLevelSkillTreeSystem.MasterLevelUp(&gObj[iUserIndex], iAddExp, 0, "Crywolf");
	}

	g_Log.Add("[Crywolf] Experience : [%s][%s](%d) Experience: %d + %d",
		gObj[iUserIndex].AccountID, gObj[iUserIndex].Name,
		gObj[iUserIndex].Level, gObj[iUserIndex].m_PlayerData->Experience, iAddExp);

	gObjSetExpPetItem(iUserIndex, iAddExp);

	if ( gObj[iUserIndex].Level >= g_ConfigRead.data.common.UserMaxLevel)
	{
		GSProtocol.GCServerMsgStringSend(Lang.GetText(0,45), gObj[iUserIndex].m_Index, 1);
		return 0;
	}

	if ( (gObj[iUserIndex].m_PlayerData->Experience + iAddExp) < gObj[iUserIndex].m_PlayerData->NextExp )
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
			if ( gObj[iUserIndex].Class == CLASS_DARKLORD || gObj[iUserIndex].Class == CLASS_MAGUMSA || gObj[iUserIndex].Class == CLASS_RAGEFIGHTER || gObj[iUserIndex].Class == CLASS_GROWLANCER)
			{
				gObj[iUserIndex].m_PlayerData->LevelUpPoint += gLevelUpPointMGDL;
			}

			else
			{
				gObj[iUserIndex].m_PlayerData->LevelUpPoint += gLevelUpPointNormal;
			}

			if ( gObj[iUserIndex].m_PlayerData->PlusStatQuestClear != false )
			{
				gObj[iUserIndex].m_PlayerData->LevelUpPoint++;

				g_Log.Add("[ Crywolf ][MVP Exp.] [%s][%s] LevelUp PlusStatQuest Clear AddStat %d",
					gObj[iUserIndex].AccountID, gObj[iUserIndex].Name,
					gObj[iUserIndex].m_PlayerData->LevelUpPoint);
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

		g_Log.Add("Level Up [%s][%s][%d]", gObj[iUserIndex].AccountID,
			gObj[iUserIndex].Name, gObj[iUserIndex].Level);
	}

	return iLEFT_EXP;
}
	





//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


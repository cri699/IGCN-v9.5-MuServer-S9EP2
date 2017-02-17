// RaklionUtil.cpp: implementation of the CRaklionUtil class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RaklionUtil.h"
#include "Winutil.h"
#include "Raklion.h"
#include "user.h"
#include "TNotice.h"
#include "RaklionBattleUserMng.h"
#include "GameMain.h"
#include "configread.h"

CRaklionUtil	g_RaklionUtil;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRaklionUtil::CRaklionUtil()
{

}

CRaklionUtil::~CRaklionUtil()
{

}

void CRaklionUtil::NotifyRaklionChangeState(int iState, int iDetailState)
{
	PMSG_ANS_RAKLION_STATE_CHANGE pMsg = {0};

	PHeadSubSetB((LPBYTE)&pMsg, 0xD1, 0x12, sizeof(pMsg));
	pMsg.btState = iState;
	pMsg.btDetailState = iDetailState;

	if(iState == RAKLION_STATE_NOTIFY_1)
	{
		SendDataRaklionMapUser((LPBYTE)&pMsg,sizeof(pMsg));	
		return;
	}

	SendDataRaklionBossMapUser((LPBYTE)&pMsg, sizeof(pMsg));
}

void CRaklionUtil::NotifyRaklionSuccessValue(int iSuccessValue)
{
	PMSG_ANS_RAKLION_BATTLE_RESULT pMsg = {0};

	PHeadSubSetB((LPBYTE)&pMsg, 0xD1, 0x13, sizeof(pMsg));
	pMsg.btResult = iSuccessValue;

	SendDataRaklionBossMapUser((LPBYTE)&pMsg, sizeof(pMsg));
}

void CRaklionUtil::NotifyRaklionCurrentState(int iIndex, int iState, int iDetailState)
{
	PMSG_ANS_RAKLION_CURRENT_STATE pMsg = {0};

	PHeadSubSetB((LPBYTE)&pMsg, 0xD1, 0x11, sizeof(pMsg));
	pMsg.btCurrentState = iState;
	pMsg.btCurrentDetailState = iDetailState;

	IOCP.DataSend(iIndex, (LPBYTE)&pMsg, sizeof(pMsg));
}

void CRaklionUtil::NotifyRaklionWideAreaAttack(int iIndex, int iTargetIndex, int iSkillType)
{
	PMSG_NOTIFY_RAKLION_WIDE_AREA_ATTACK pMsg;

	PHeadSubSetB((LPBYTE)&pMsg, 0xD1, 0x14, sizeof(pMsg));
	pMsg.btObjClassH = HIBYTE(gObj[iIndex].Class);
	pMsg.btObjClassL = LOBYTE(gObj[iIndex].Class);
	pMsg.btType = iSkillType;

	SendDataToUser(iTargetIndex, (LPBYTE)&pMsg, sizeof(pMsg));
}

void CRaklionUtil::NotifyRaklionUserMonsterCount(int iMonsterCount, int iUserCount)
{
	PMSG_NOTIFY_RAKLION_USER_MONSTER_COUNT pMsg;

	PHeadSubSetB((LPBYTE)&pMsg, 0xD1, 0x15, sizeof(pMsg));
	pMsg.btMonsterCount = iMonsterCount;
	pMsg.btUserCount = iUserCount;

	SendDataRaklionBossMapUser((LPBYTE)&pMsg, sizeof(pMsg));
}

void CRaklionUtil::NotifyLeftTopMsgRaklionBattleUser(char *lpszMsg, ...)
{
	if(lpszMsg == NULL)	return;

	char szBuffer[512] = "";
	va_list pArguments;

	va_start(pArguments, lpszMsg);
	vsprintf(szBuffer, lpszMsg, pArguments);
	va_end(pArguments);

	PMSG_NOTICE pNotice;
	TNotice::MakeNoticeMsg(&pNotice, 1, szBuffer);

	for(int iCount=0; iCount<g_RaklionBattleUserMng.m_BattleUser.size(); iCount++)
	{
		if( gObj[g_RaklionBattleUserMng.m_BattleUser[iCount]].Connected == PLAYER_PLAYING &&
			gObj[g_RaklionBattleUserMng.m_BattleUser[iCount]].Type == OBJ_USER)
		{
			TNotice::SendNoticeToUser(g_RaklionBattleUserMng.m_BattleUser[iCount], &pNotice);
		}
	}
}

void CRaklionUtil::SendMsgRaklionBossMapUser(char *lpszMsg, ...)
{
	if(lpszMsg == NULL)	return;

	char szBuffer[512] = "";
	va_list pArguments;

	va_start(pArguments, lpszMsg);
	vsprintf(szBuffer, lpszMsg, pArguments);
	va_end(pArguments);

	PMSG_NOTICE pNotice;
	TNotice::MakeNoticeMsg(&pNotice, 0, szBuffer);

	for(int iCount=g_ConfigRead.server.GetObjectStartUserIndex(); iCount<g_ConfigRead.server.GetObjectMax(); iCount++)
	{
		if( gObj[iCount].Connected == PLAYER_PLAYING &&
			gObj[iCount].Type == OBJ_USER &&
			gObj[iCount].MapNumber == MAP_INDEX_HATCHERY)
		{
			TNotice::SendNoticeToUser(iCount, &pNotice);
		}
	}
}

void CRaklionUtil::SendDataRaklionBossMapUser(BYTE *lpMsg, int iSize)
{
	for(int iCount=g_ConfigRead.server.GetObjectStartUserIndex(); iCount<g_ConfigRead.server.GetObjectMax(); iCount++)
	{
		if( gObj[iCount].Connected == PLAYER_PLAYING &&
			 gObj[iCount].Type == OBJ_USER &&
			 gObj[iCount].MapNumber == MAP_INDEX_HATCHERY)
		{
			IOCP.DataSend(iCount, lpMsg, iSize);
		}
	}
}

void CRaklionUtil::SendMsgRaklionMapUser(char *lpszMsg, ...)
{
	if(lpszMsg == NULL)	return;

	char szBuffer[512] = "";
	va_list pArguments;

	va_start(pArguments, lpszMsg);
	vsprintf(szBuffer, lpszMsg, pArguments);
	va_end(pArguments);

	PMSG_NOTICE pNotice;
	TNotice::MakeNoticeMsg(&pNotice, 0, szBuffer);

	for(int iCount=g_ConfigRead.server.GetObjectStartUserIndex(); iCount<g_ConfigRead.server.GetObjectMax(); iCount++)
	{
		if( gObj[iCount].Connected == PLAYER_PLAYING &&
			gObj[iCount].Type == OBJ_USER &&
			(gObj[iCount].MapNumber == MAP_INDEX_RAKLION || gObj[iCount].MapNumber == MAP_INDEX_HATCHERY) )
		{
			TNotice::SendNoticeToUser(iCount, &pNotice);
		}
	}
}

void CRaklionUtil::SendDataRaklionMapUser(BYTE *lpMsg, int iSize)
{
	for(int iCount=g_ConfigRead.server.GetObjectStartUserIndex(); iCount<g_ConfigRead.server.GetObjectMax(); iCount++)
	{
		if( gObj[iCount].Connected == PLAYER_PLAYING &&
			 gObj[iCount].Type == OBJ_USER &&
			 (gObj[iCount].MapNumber == MAP_INDEX_RAKLION || gObj[iCount].MapNumber == MAP_INDEX_HATCHERY) )
		{
			IOCP.DataSend(iCount, lpMsg, iSize);
		}
	}
}

void CRaklionUtil::SendMsgAllUser(char *lpszMsg, ...)
{
	if(lpszMsg == NULL)	return;

	char szBuffer[512] = "";
	va_list pArguments;

	va_start(pArguments, lpszMsg);
	vsprintf(szBuffer, lpszMsg, pArguments);
	va_end(pArguments);

	PMSG_NOTICE pNotice;
	TNotice::MakeNoticeMsg(&pNotice, 0, szBuffer);
	TNotice::SendNoticeToAllUser(&pNotice);
}

void CRaklionUtil::SendDataAllUser(BYTE *lpMsg, int iSize)
{
	for(int iCount=g_ConfigRead.server.GetObjectStartUserIndex(); iCount<g_ConfigRead.server.GetObjectMax(); iCount++)
	{
		if( gObj[iCount].Connected == PLAYER_PLAYING &&
			gObj[iCount].Type == OBJ_USER )
		{
			IOCP.DataSend(iCount, lpMsg, iSize);
		}
	}
}

void CRaklionUtil::SendMsgToUser(int iIndex, char *lpszMsg, ...)
{
	if(lpszMsg == NULL)	return;

	char szBuffer[512] = "";
	va_list pArguments;

	va_start(pArguments, lpszMsg);
	vsprintf(szBuffer, lpszMsg, pArguments);
	va_end(pArguments);

	PMSG_NOTICE pNotice;
	TNotice::MakeNoticeMsg(&pNotice, 0, szBuffer);
	TNotice::SendNoticeToUser(iIndex, &pNotice);
}

void CRaklionUtil::SendDataToUser(int iIndex, BYTE *lpMsg, int iSize)
{
	if ( gObj[iIndex].Connected == PLAYER_PLAYING &&
		 gObj[iIndex].Type == OBJ_USER )
	{
		IOCP.DataSend(iIndex, lpMsg, iSize);
	}
}

void CRaklionUtil::SendSystemMsgToUser(int iIndex, char *lpszMsg, ...)
{
	if(lpszMsg == NULL)	return;

	char szBuffer[512] = "";
	va_list pArguments;
	
	va_start( pArguments,  lpszMsg);
	vsprintf(szBuffer, lpszMsg, pArguments);
	va_end(pArguments);

	TNotice::GCServerMsgStringSend(lpszMsg, iIndex, 1);
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


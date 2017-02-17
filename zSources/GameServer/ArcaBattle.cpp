//////////////////////////////////////////////////////////////////////
// ArcaBattle.cpp
#include "StdAfx.h"
#include "ArcaBattle.h"
#include "user.h"
#include "LargeRand.h"
#include "TLog.h"
#include "GameMain.h"
#include "BuffEffectSlot.h"
#include "BuffEffect.h"
#include "GuildClass.h"
#include "MapServerManager.h"
#include "ObjCalCharacter.h"
#include "TNotice.h"
#include "Gate.h"
#include "DSProtocol.h"
#include "EDSProtocol.h"
#include "MasterLevelSkillTreeSystem.h"
#include "GensSystem.h"
#include "MapClass.h"
#include "ChaosBox.h"
#include "BagManager.h"
#include "winutil.h"
#include "configread.h"
#include "winutil.h"

CArcaBattle g_ArcaBattle;

CArcaBattle::CArcaBattle()
{
	this->m_bReqABWinGuildInfo = 0;
	this->m_iJoinGuildMemberCnt = 0;
	this->m_i64_AB_TICK_COUNT_SEC = 0;
	this->m_iCurUserCount = 0;
	this->m_iObeliskCnt = 0;
	this->m_iAuraChk = 1;
	this->m_iBootyMixCnt = 0;
	this->m_i64_DbDataLoadTickCount = 0;
	this->m_iAB_RegTimeMin = 0;
	this->m_iAB_PlayTimeMin = 0;
	this->m_iAB_WaitTimeMin = 0;
	this->m_iAB_ChannelTimeMin = 0;
	this->m_i64_AB_TICK_COUNT = 0;
	this->m_i64_AB_TICK_COUNT_2 = 0;
	this->m_iABJoinUserCnt = 0;
	this->m_iSkipObelisk = 0;
	this->m_bWindowClose = 0;
}

CArcaBattle::~CArcaBattle()
{

}

void CArcaBattle::Clear()
{
	this->m_iCurUserCount = 0;
	memset(&this->m_tmStartDate, 0, sizeof(this->m_tmStartDate));
	this->m_i64_AB_REMAIN_MSEC = 0;
	this->m_i64_AB_TICK_COUNT = 0;
	this->m_iAB_NOTIFY_COUNT = 0;
	this->m_i64_AB_TICK_COUNT_SEC = 0;
	this->SetRandomObeliskAttr();

	for(int i=0; i<300; i++)
	{
		this->m_UserData[i].Clear();
	}
        
	for(int i=0; i<10; i++)
	{
		this->m_stGuildMemberInfoLog[i].Clear();
	}

	for(int i=0; i<10; i++)
	{
		this->m_stGMasterGateNum[i].Clear();
	}

	this->m_bWindowClose = false;
}

BOOL CArcaBattle::GetBootyRewardItem(int iBootyCnt, _tagARCA_BATTLE_BOOTY_REWARD *pBootyRewardItem)
{
	_tagARCA_BATTLE_BOOTY_MIX pBootMix;
	bool bMixFound = false;

	for (int i = 0; i < this->m_iBootyMixCnt; i++ )
	{
		if ( this->m_stBootyMix[i].iMinCnt <= iBootyCnt && this->m_stBootyMix[i].iMaxCnt >= iBootyCnt )
		{
			pBootMix = this->m_stBootyMix[i];
			bMixFound = true;
			break;
		}
	}

	if ( bMixFound == false )
	{
		g_Log.Add("[ArcaBattle][BootyExchange] Error GetBootyRewardItem() NULL");
		return FALSE;
	}

	int successrate = pBootMix.iBaseRate + ((iBootyCnt  * pBootMix.iAddRate) - pBootMix.iBootyRewardCnt );
	int rand = GetLargeRand() % 100;

	if ( rand > successrate)
	{
		return 2;
	}

	DWORD iRewardRand = GetLargeRand() % 1000000;
	DWORD iSumRate = 0;
	int iRewardItem = 0;

	for (int RateCnt = 0; RateCnt < pBootMix.iMaxCnt; RateCnt++ )
	{
		iSumRate += pBootMix.stBootyReward[RateCnt].iRate;

		if ( iRewardRand <= iSumRate )
		{
			iRewardItem = RateCnt;
			break;
		}
	}

	if ( iRewardItem >= 20 )
	{
		g_Log.Add("[ArcaBattle][BootyExchange] Error Overflow iRewardItem");
		return FALSE;
	}

	pBootyRewardItem->iItemLink = pBootMix.stBootyReward[iRewardItem].iItemLink;
	pBootyRewardItem->iItemType = pBootMix.stBootyReward[iRewardItem].iItemType;
	pBootyRewardItem->iItemIndex = pBootMix.stBootyReward[iRewardItem].iItemIndex;
	pBootyRewardItem->iItemLevel = pBootMix.stBootyReward[iRewardItem].iItemLevel;
	pBootyRewardItem->iDurab = pBootMix.stBootyReward[iRewardItem].iDurab;
	pBootyRewardItem->iRate = pBootMix.stBootyReward[iRewardItem].iRate;

	return TRUE;
}

void  CArcaBattle::SendArcaBattlePlayInfo(OBJECTSTRUCT *lpObj, WORD wGuildGroupNum)
{
	_tagPMSG_ARCA_BATTLE_PLAY_INFO pMsg; 

	if ( lpObj->Connected < PLAYER_LOGGED )
	{
		return;
	}
   
	pMsg.h.c = 0xC1;
	pMsg.h.size = sizeof(pMsg);
	pMsg.h.headcode = 0xF8;
	pMsg.h.subcode = 0x39;
	pMsg.wGuildGroupNum = wGuildGroupNum;

	for ( int i = 0; i < 5; i++ )
	{
		if ( i != this->m_iSkipObelisk )
		{
			pMsg.m_stObeliskInfo[i].iGroupNumber = this->m_stObeliskInfo[i].m_iGroupNumber;
			pMsg.m_stObeliskInfo[i].iAttrKind = this->m_stObeliskInfo[i].m_iAttrKind;
			pMsg.m_stObeliskInfo[i].iMapIndex = this->m_stObeliskInfo[i].m_iMapIndex;
			pMsg.m_stObeliskInfo[i].iPosX = this->m_stObeliskInfo[i].m_iPosX;
			pMsg.m_stObeliskInfo[i].iPosY = this->m_stObeliskInfo[i].m_iPosY;

			g_Log.Add( "[ArcaBattle] Send ObeliskInfo iGroupNumber %d iAttr %d iMapIndex %d iPosX %d iPosY %d", 
				pMsg.m_stObeliskInfo[i].iGroupNumber, pMsg.m_stObeliskInfo[i].iAttrKind, pMsg.m_stObeliskInfo[i].iMapIndex,
				pMsg.m_stObeliskInfo[i].iPosX, pMsg.m_stObeliskInfo[i].iPosY);

			for ( int j = 0; j < 3; j++ )
			{
				pMsg.m_stObeliskInfo[i].m_StAuraInfo[j].m_iAuraPosX = this->m_stObeliskInfo[i].m_stAuraInfo[j].m_iAuraPosX;
				pMsg.m_stObeliskInfo[i].m_StAuraInfo[j].m_iAuraPosY = this->m_stObeliskInfo[i].m_stAuraInfo[j].m_iAuraPosY;

				g_Log.Add( "[ArcaBattle] Send ObeliskInfo iGroupNumber m_iAuraPosX %d m_iAuraPosY %d",
					pMsg.m_stObeliskInfo[i].m_StAuraInfo[j].m_iAuraPosX, pMsg.m_stObeliskInfo[i].m_StAuraInfo[j].m_iAuraPosY);
			}
		}
    }

	g_Log.Add("[ArcaBattle] Send Play Data");
	IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
}

void CArcaBattle::WinGuildMemberAddBuff(OBJECTSTRUCT *lpObj, unsigned __int16 wObeliskAttr)
{
	LPPERIOD_BUFF_EFFECT_INFO lpPeriBuff; 

	WORD wBuffIndex = this->GetObeliskAttrToRewardBuffIndex(wObeliskAttr);
	lpPeriBuff = g_BuffEffectSlot.GetPeriodBuffInfo(wBuffIndex);

	if ( lpPeriBuff != NULL )
		gObjAddPeriodBuffEffect(lpObj, lpPeriBuff, 604800); // 7 days
}

void CArcaBattle::GiveRewardBuffWinningGuild()
{
	for ( int i = 0; i < 5; i++ )
	{
		if ( this->m_stObeliskState[i].m_iObeliskState == 1 )
		{
			WORD wBuffIndex = this->GetObeliskAttrToRewardBuffIndex( this->m_stObeliskInfo[i].m_iAttrKind);

			if ( !wBuffIndex )
			{
				g_Log.Add("[ArcaBattle] Error Obelisk Attribute %d", this->m_stObeliskInfo[i].m_iAttrKind);
			}

			_GUILD_INFO_STRUCT * lpGuild = Guild.SearchGuild(this->m_stObeliskState[i].m_szOccupyGuildName);

			if ( lpGuild )
			{
				_tagPeriodBuffInfo BuffInfo;

				BuffInfo.btEffectType1 = 0;
				BuffInfo.btEffectType2 = 0;
				BuffInfo.wBuffIndex = wBuffIndex;
				BuffInfo.lDuration = 604800; // 7 Days

				g_BuffEffect.RequestGuildPeriodBuffInsert(this->m_stObeliskState[i].m_szOccupyGuildName, &BuffInfo);
				g_Log.Add("[ArcaBattle] Reward GuildBuff GuildName[%s] BuffIndex[%d]", this->m_stObeliskState[i].m_szOccupyGuildName, wBuffIndex);
			}

			else
			{
				g_Log.Add( "[ArcaBattle][Error] GiveRewardBuffWinningGuild() GuildInfo is NULL GuildName : %s ObeliskAttr : %d ObeliskGroup %d",
					this->m_stObeliskState[i].m_szOccupyGuildName, this->m_stObeliskInfo[i].m_iAttrKind, this->m_stObeliskInfo[i].m_iGroupNumber);
			}
		}
	}
}

void CArcaBattle::ReqRemoveRewardGuildBuff(char *szGuildName)
{
	WORD wBuffIndex = 0;

	for ( int i = 0; i < this->m_iWinGuildCnt; ++i )
	{
		if( strcmp(this->m_stABWinGuildInfo[i].szGuildName, szGuildName) == 0)
		{
			wBuffIndex = this->GetObeliskAttrToRewardBuffIndex(this->m_stABWinGuildInfo[i].wOccupyObelisk);
		}
	}

	g_BuffEffect.RequestGuildPeriodBuffDelete(&wBuffIndex, 1);
	g_Log.Add("[ArcaBattle] Request ExDB Delete Reward Guild Buff");
}

void CArcaBattle::ReqRemoveRewardBuffWinningGuild()
{
	_tagPMSG_REQ_REMOVE_ALL_GUILD_BUFF_DS pMsg;
	WORD wBuffIndex[5];

	wBuffIndex[0] = BUFFTYPE_ARCA_FIRETOWER;
	wBuffIndex[1] = BUFFTYPE_ARCA_WATERTOWER;
	wBuffIndex[2] = BUFFTYPE_ARCA_EARTHTOWER;
	wBuffIndex[3] = BUFFTYPE_ARCA_WINDTOWER;
	wBuffIndex[4] = BUFFTYPE_ARCA_DARKNESSTOWER;

	g_BuffEffect.RequestGuildPeriodBuffDelete(wBuffIndex, 5);
	g_Log.Add("[ArcaBAttle] Request DataServer Delete All Reward Guild Buff");

	pMsg.h.c = 0xC1;
	pMsg.h.size = sizeof(pMsg);
	pMsg.h.headcode = 0xF8;
	pMsg.h.subcode = 0x47;
	pMsg.wMapSvrNum = g_MapServerManager.GetMapSvrGroup();

	wsDataCli.DataSend((char*)&pMsg, pMsg.h.size);
	g_Log.Add( "[ArcaBattle] Request MultiCast Remove All Reward Guild Buff");
}

void CArcaBattle::DGAnsRemoveRewardBuff(_tagPMSG_ANS_REMOVE_GUILD_BUFF_DS *lpMsg)
{
	for (int i = 0; i < this->m_iWinGuildCnt; i++ )
	{
		if( strcmp(this->m_stABWinGuildInfo[i].szGuildName, lpMsg->szGuildName) == 0)
		{
			WORD wBuffIndex = this->GetObeliskAttrToRewardBuffIndex(this->m_stABWinGuildInfo[i].wOccupyObelisk);
			this->RemoveGuildBuff( this->m_stABWinGuildInfo[i].szGuildName, wBuffIndex);
			this->m_stABWinGuildInfo[i].Clear();
		}
	}
}

void CArcaBattle::DGAnsRemoveAllRewardBuff()
{
	for ( int i = 0; i < this->m_iWinGuildCnt; i++ )
	{
		WORD wBuffIndex = this->GetObeliskAttrToRewardBuffIndex(this->m_stABWinGuildInfo[i].wOccupyObelisk);
		this->RemoveGuildBuff(this->m_stABWinGuildInfo[i].szGuildName, wBuffIndex);

		g_Log.Add("[ArcaBattle] Remove All Reward Guild Buff GuildName[%s] BuffIndex[%d]", this->m_stABWinGuildInfo[i].szGuildName, wBuffIndex);
		this->m_stABWinGuildInfo[i].Clear();
	}
}

void CArcaBattle::RemoveRewardBuff(char *szGuildName)
{
	for ( int i = 0; i < this->m_iWinGuildCnt; i++ )
	{
		if( strcmp(this->m_stABWinGuildInfo[i].szGuildName, szGuildName) == 0)
		{
			WORD wBuffIndex = this->GetObeliskAttrToRewardBuffIndex(this->m_stABWinGuildInfo[i].wOccupyObelisk);
			this->RemoveGuildBuff(this->m_stABWinGuildInfo[i].szGuildName, wBuffIndex);
			this->m_stABWinGuildInfo[i].Clear();
		}
	}
}

void CArcaBattle::RemoveGuildBuff(char *szGuildName, WORD wBuffIndex)
{
	_GUILD_INFO_STRUCT *lpGuild = Guild.SearchGuild(szGuildName);

	if ( !lpGuild )
	{
		g_Log.Add("[ArcaBattle] Error Can Not Find Guild RemoveGuildBuff() lpGuild NULL GuildName : %s", szGuildName);
		return;
	}

	for ( int nGuildCnt = 0; nGuildCnt < lpGuild->Count; ++nGuildCnt )
	{
		int iIndex = lpGuild->Index[nGuildCnt];
		
		if ( gObjIsConnected(iIndex) == TRUE )
		{
			if ( gObjCheckUsedBuffEffect(&gObj[iIndex], wBuffIndex) == TRUE )
			{
				gObjRemoveBuffEffect(&gObj[iIndex], wBuffIndex);
				gObjCalCharacter.CalcCharacter(iIndex);

				g_Log.Add( "[ArcaBattle] Remove Buff [%s][%s] GuildName [%s] BuffIndex [%d]", 
					gObj[iIndex].AccountID, gObj[iIndex].Name, szGuildName, wBuffIndex);
			}
		}	
	}
}

void CArcaBattle::ReqGuildMemberDelBuffDel(OBJECTSTRUCT *lpObj, char *szGuildName)
{
	for ( int i = 0; i < 5; ++i )
	{
		if(strcmp(this->m_stABWinGuildInfo[i].szGuildName, szGuildName) == 0)
		{
			WORD wBuffIndex = this->GetObeliskAttrToRewardBuffIndex(this->m_stABWinGuildInfo[i].wOccupyObelisk);

			if ( gObjCheckUsedBuffEffect(lpObj, wBuffIndex) == TRUE )
			{
				g_BuffEffect.RequestPeriodBuffDelete(lpObj, wBuffIndex);
				gObjRemoveBuffEffect(lpObj, wBuffIndex);
				gObjCalCharacter.CalcCharacter(lpObj->m_Index);
			}
		}
	}
}

void CArcaBattle::ReqGuildMemberDelBuffDel(char *szName, char *szGuildName)
{
	for (int i = 0; i < 5; ++i)
	{
		if (strcmp(this->m_stABWinGuildInfo[i].szGuildName, szGuildName) == 0)
		{
			WORD wBuffIndex = this->GetObeliskAttrToRewardBuffIndex(this->m_stABWinGuildInfo[i].wOccupyObelisk);
			g_BuffEffect.RequestPeriodBuffDelete(szName, wBuffIndex);
		}
	}
}

WORD CArcaBattle::GetObeliskAttrToRewardBuffIndex( int iObeliskAttr)
{
	WORD wBuffIndex = 0;
	
	switch ( iObeliskAttr )
	{
		case 1:
			wBuffIndex = BUFFTYPE_ARCA_FIRETOWER;
			break;
		case 2:
			wBuffIndex = BUFFTYPE_ARCA_WATERTOWER;
			break;
		case 3:
			wBuffIndex = BUFFTYPE_ARCA_EARTHTOWER;
			break;
		case 4:
			wBuffIndex = BUFFTYPE_ARCA_WINDTOWER;
			break;
		case 5:
			wBuffIndex = BUFFTYPE_ARCA_DARKNESSTOWER;
			break;
		default:
			return wBuffIndex;
	}

	return wBuffIndex;
}

void CArcaBattle::AddArcaBattleWinGuildInfo(_stABWinGuildInfoDS *pABWinGuildInfoDS, int iGuildCnt)
{
	this->m_iWinGuildCnt = iGuildCnt;

	for (int i = 0; i < this->m_iWinGuildCnt; i++ )
	{
		memcpy(this->m_stABWinGuildInfo[i].szGuildName, pABWinGuildInfoDS[i].szGuildName, MAX_GUILD_LEN+1);
		this->m_stABWinGuildInfo[i].dwGuild = pABWinGuildInfoDS[i].dwGuild;
		this->m_stABWinGuildInfo[i].wOccupyObelisk = pABWinGuildInfoDS[i].wOccupyObelisk;
		this->m_stABWinGuildInfo[i].wObeliskGroup = pABWinGuildInfoDS[i].wObeliskGroup;

		_GUILD_INFO_STRUCT * lpGuild = Guild.SearchGuild(this->m_stABWinGuildInfo[i].szGuildName);

		if ( lpGuild )
		{
			for ( int nGuildCnt = 0; nGuildCnt < lpGuild->Count; nGuildCnt++ )
			{
				if ( gObjIsConnected(lpGuild->Index[nGuildCnt]) == TRUE )
				{
					this->WinGuildMemberAddBuff(&gObj[lpGuild->Index[nGuildCnt]], this->m_stABWinGuildInfo[i].wOccupyObelisk);
				}
			}	
		}

		g_Log.Add( "[ArcaBattle] Add Winning Guild Info GuildName[%s] ObeliskAttr[%d] ObeliskGroup[%d]",
			this->m_stABWinGuildInfo[i].szGuildName, this->m_stABWinGuildInfo[i].wOccupyObelisk, this->m_stABWinGuildInfo[i].wObeliskGroup);
	}

	this->SetDataLoadWinGuild(1);
}

int CArcaBattle::IsArcaBattleWinGuild(OBJECTSTRUCT *obj)
{
	if (obj->m_PlayerData->lpGuild )
	{
		for ( int i = 0; i < 5; ++i )
		{
			if(strcmp(obj->m_PlayerData->lpGuild->Name, this->m_stABWinGuildInfo[i].szGuildName) == 0)
			{
				if (obj->m_PlayerData->lpGuild->Number == this->m_stABWinGuildInfo[i].dwGuild )
					return TRUE;
			}
		}
	}

	return FALSE;
}

BOOL CArcaBattle::IsArcaBattleOccupyZone(OBJECTSTRUCT *lpObj, int gt)
{
	if ( !lpObj->m_PlayerData->lpGuild )
	{
		return FALSE;
	}

	if ( gt != 418 && gt != 422 && gt != 435 )
	{
		return FALSE;
	}

	for ( int i = 0; i < 5; i++ )
	{
		if(strcmp(lpObj->m_PlayerData->lpGuild->Name, this->m_stABWinGuildInfo[i].szGuildName))
		{
			continue;
		}

		if ( (gt == 418 && this->m_stABWinGuildInfo[i].wObeliskGroup == 1) || (gt == 422 && this->m_stABWinGuildInfo[i].wObeliskGroup == 2) || (gt == 435 && this->m_stABWinGuildInfo[i].wObeliskGroup == 3) )
		{
			return TRUE;
		}
	}

	return FALSE;
}

void CArcaBattle::SendArcaBattleOccupyZone(OBJECTSTRUCT *obj, int gt)
{
	_tagPMSG_SEND_AB_WIN_GUILD_INFO pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.size = sizeof(pMsg);
	pMsg.h.headcode = 0xF8;
	pMsg.h.subcode = 0x3D;

	if ( gt == 418 || gt == 422 || gt == 435 )
	{
		for (int i = 0; i < 5; i++ )
		{
			if ( ( gt == 418 && this->m_stABWinGuildInfo[i].wObeliskGroup == 1 ) || ( gt == 422 && this->m_stABWinGuildInfo[i].wObeliskGroup == 2 ) || (gt == 435 && this->m_stABWinGuildInfo[i].wObeliskGroup == 3 ))
			{
				memcpy(pMsg.szGuildName, this->m_stABWinGuildInfo[i].szGuildName, MAX_GUILD_LEN+1);
				IOCP.DataSend(obj->m_Index, (LPBYTE)&pMsg, sizeof(pMsg));
				return;
			}
		}
	}
}

void CArcaBattle::Init()
{
	if (g_ConfigRead.server.GetStateFromEventTable(g_ConfigRead.server.GetServerType(), EV_TABLE_ARCA) == false)
	{
		this->SetEventEnable(false);
	}

	if ( this->m_bDoEvent )
	{
		this->SetState(1);
		this->GDReqArcaBattleProcMultiCast(1);
	}

	else
	{
		this->SetState(0);
		this->GDReqArcaBattleProcMultiCast(0);
	}
}

int CArcaBattle::CheckSync()
{
	if ( !this->IsArcaBattleServer() )
	{
		return false;
	}

	SYSTEMTIME tmToDay;
	SYSTEMTIME systime;
	_tagARCA_BATTLE_START_TIME pRET;
	int bTIME_CHANGED = 0;
	int iMIN_HOUR;
	int iMIN_MINUTE;

	GetLocalTime(&tmToDay);

	int iTODAY_DATE_NUM = MACRO2(( MACRO1(tmToDay.wDay) | MACRO1(tmToDay.wMonth) << 8 ) & 0xFFFF ) | MACRO2(tmToDay.wYear) << 16;

	WORD wDayOfWeek = tmToDay.wDayOfWeek;

	if ( wDayOfWeek < 7 )
	{
		if ( this->m_iAB_DayOfWeekCycle[wDayOfWeek] == TRUE )
		{
			if( this->m_vtArcaBattleOpenTime.empty() == true )
			{
				g_Log.AddC(TColor::Red, "[ArcaBattle] No Schedule Time Data -  (m_vtArcaBattleOpenTime.empty())");
				this->SetState(0);
				this->GDReqArcaBattleProcMultiCast(0);
				return false;
			}

			GetLocalTime(&systime);

			for ( std::vector<_tagARCA_BATTLE_START_TIME>::iterator It = this->m_vtArcaBattleOpenTime.begin(); It != this->m_vtArcaBattleOpenTime.end(); It++ )
			{
				pRET = *It;

				if ( systime.wMinute + 60 * systime.wHour < pRET.m_iMinute + 60 * pRET.m_iHour )
				{
					bTIME_CHANGED = TRUE;
					iMIN_HOUR = pRET.m_iHour;
					iMIN_MINUTE = pRET.m_iMinute;
				}
			}

			if ( !bTIME_CHANGED )
			{
				iMIN_HOUR = 24;
				iMIN_MINUTE = 60;

				for ( std::vector<_tagARCA_BATTLE_START_TIME>::iterator It = this->m_vtArcaBattleOpenTime.begin(); It != this->m_vtArcaBattleOpenTime.end(); It++ )
				{
					if ( iMIN_MINUTE + 60 * iMIN_HOUR > It->m_iMinute + 60 * It->m_iHour )
					{
						bTIME_CHANGED = 2;
						iMIN_HOUR = It->m_iHour;
						iMIN_MINUTE = It->m_iMinute;
					}
				}
			}

			if ( bTIME_CHANGED == TRUE )
			{
				this->m_i64_AB_REMAIN_MSEC = 1000 * ( 60 * iMIN_MINUTE + 3600 * iMIN_HOUR - ( systime.wSecond + 60 * systime.wMinute + 3600 * systime.wHour) );
				this->SetState(2);
				this->GDReqArcaBattleProcMultiCast(2);
				this->m_i64_AB_TICK_COUNT = GetTickCount64();

				g_Log.Add("[ArcaBattle] Sync Start Time. [%I64d] min remain (START HOUR:%d, MIN:%d)", this->m_i64_AB_REMAIN_MSEC / 60000,
					iMIN_HOUR, iMIN_MINUTE);

				return true;
			}

			else if ( bTIME_CHANGED == 2 )
			{
				this->m_i64_AB_REMAIN_MSEC = -100;
				return false;
			}

			else
			{
				g_Log.AddC(TColor::Red, "[ArcaBattle] No Schedule Time Data - Arca War Terminated (bTIME_CHANGED=%d)", bTIME_CHANGED);
				this->SetState(0);
				this->GDReqArcaBattleProcMultiCast(0);
				return false;
			}
		}

		else
		{
			return false;
		}
	}

	else
	{
		g_Log.AddC(TColor::Red, "[ArcaBattle] Error DayOfWeek -  wDayOfWeek [%d]", wDayOfWeek);
		this->SetState(0);
		this->GDReqArcaBattleProcMultiCast(0);
		return false;
	}
}

void CArcaBattle::SetState(int iAB_State)
{
	this->m_iAB_State = iAB_State;
	if ( this->IsArcaBattleServer() )
	{
		this->SendArcaBattleStateAll(this->m_iAB_State);

		switch ( this->m_iAB_State )
		{
			case 2:
				this->SetStateClosed();
				break;
			case 3:
				this->SetStateMasterReg();
				break;
			case 5:
				this->SetStateReady();
				break;
			case 10:
				this->SetStatePlayEnd();
				break;
		}
	}
}
void CArcaBattle::SetDataLoadWinGuild(int bReqABWinGuildInfo)
{
	this->m_bReqABWinGuildInfo = bReqABWinGuildInfo;
}

int CArcaBattle::IsArcaBattleServer()
{
	return g_ConfigRead.server.GetServerType() == SERVER_ARCA ? TRUE : FALSE;
}

int CArcaBattle::IsArcaBattleDay()
{
	SYSTEMTIME tmToDay;
	GetLocalTime(&tmToDay);

	return this->m_iAB_DayOfWeekCycle[tmToDay.wDayOfWeek];
}

void CArcaBattle::SetStateReady()
{
	this->GDReqArcaBattleJoinMemberUnderReq();
}

void CArcaBattle::SetStateClosed()
{
  // empty
}

void CArcaBattle::SetStateMasterReg()
{

}

void CArcaBattle::SetStatePlayEnd()
{
	for (int i = 0; i < 50; ++i )
	{
		if ( this->m_stMonsterGroupInfo[i].m_nGroupNumber > -1 )
		{
			this->DeleteMonster(this->m_stMonsterGroupInfo[i].m_nGroupNumber);
		}
	}
}

int CArcaBattle::GetState()
{
	return this->m_iAB_State;
}

int CArcaBattle::DBDataRequest()
{
	if ( this->m_i64_DbDataLoadTickCount )
	{
		if ( GetTickCount64() - this->m_i64_DbDataLoadTickCount > 3000 )
		{
			this->m_i64_DbDataLoadTickCount = GetTickCount64();
			g_Log.AddC(TColor::Red, "[ArcaBattle] CArcaBattle::DBDataRequest() - REQUEST DATA AGAIN");
		}
	}

	else
	{
		this->m_i64_DbDataLoadTickCount = GetTickCount64();
		g_Log.AddC(TColor::Blue, "[ArcaBattle] CArcaBattle::DBDataRequest() - REQUEST DATA FIRST");
	}

	return 1;
}

void CArcaBattle::ProcStateClosed()
{
	if ( this->m_i64_AB_REMAIN_MSEC != -100 )
	{
		ULONGLONG i64TICK_MSEC = GetTickCount64() - this->m_i64_AB_TICK_COUNT;

		if ( i64TICK_MSEC >= 1000 )
		{
			this->m_i64_AB_REMAIN_MSEC -= i64TICK_MSEC;
			this->m_i64_AB_TICK_COUNT = GetTickCount64();

			if ( this->m_i64_AB_REMAIN_MSEC <= 0 )	
			{
				this->GDReqDeleteArcaBattleInfo(); // Dudi
				this->ReqRemoveRewardBuffWinningGuild();
				this->m_iAB_NOTIFY_COUNT = this->m_i64_AB_REMAIN_MSEC / 60000;
				this->SendMapServerGroupMsg(Lang.GetText(0,587));
				this->m_i64_AB_REMAIN_MSEC = 60000 * this->m_iAB_MasterRegMin;
				this->m_iAB_NOTIFY_COUNT = this->m_i64_AB_REMAIN_MSEC / 60000;

				g_Log.AddC(TColor::Blue, "[ArcaBattle] [State Change] -> STATE: MASTER_REG");

				this->GDReqAllGuildMarkCnt();
				this->SetState(3);
				this->GDReqArcaBattleProcMultiCast(3);
			}
		}
	}
}

void CArcaBattle::ProcStateMasterReg()
{
	char szMsg[256];
	ULONGLONG i64TICK_MSEC = GetTickCount64() - this->m_i64_AB_TICK_COUNT;

	if (i64TICK_MSEC >= 1000)
	{
		this->m_i64_AB_REMAIN_MSEC -= i64TICK_MSEC;
		this->m_i64_AB_TICK_COUNT = GetTickCount64();

		int iAB_REMAIN_MIN = this->m_i64_AB_REMAIN_MSEC / 60000;

		if (this->m_i64_AB_REMAIN_MSEC <= 60000 * this->m_iAB_MasterRegMin)
		{
			if (GetTickCount64() - this->m_i64_AB_TICK_COUNT_2 > 60000)
			{
				this->m_iAB_NOTIFY_COUNT = iAB_REMAIN_MIN;
				this->m_i64_AB_TICK_COUNT_2 = GetTickCount64();

				if (this->m_i64_AB_REMAIN_MSEC > 900000)
				{
					if (this->m_i64_AB_REMAIN_MSEC > 900000 && !((iAB_REMAIN_MIN + 1) % 5))
					{
						memset(szMsg, 0x00, sizeof(szMsg));
						sprintf(szMsg, Lang.GetText(0,326), this->m_iAB_NOTIFY_COUNT + 1);
						this->SendMapServerGroupMsg(szMsg);

						g_Log.AddC(TColor::Blue, "[ArcaBattle] [State Notify] STATE: MASTER_REG (Left Time:%d)", this->m_iAB_NOTIFY_COUNT + 1);
					}
				}
				else
				{
					memset(szMsg, 0x00, sizeof(szMsg));
					sprintf(szMsg, Lang.GetText(0,327), this->m_iAB_NOTIFY_COUNT + 1);
					this->SendMapServerGroupMsg(szMsg);

					g_Log.AddC(TColor::Blue, "[ArcaBattle] [State Notify] STATE: MASTER_REG (Left Time:%d)", this->m_iAB_NOTIFY_COUNT + 1);
				}
			}
		}
	}

	if (this->m_i64_AB_REMAIN_MSEC <= 0)
	{
		this->m_i64_AB_TICK_COUNT_2 = 0;
		this->m_i64_AB_REMAIN_MSEC = 60000 * this->m_iAB_RegTimeMin;
		this->m_iAB_NOTIFY_COUNT = this->m_i64_AB_REMAIN_MSEC / 60000;
		this->SetState(4);
		this->GDReqArcaBattleProcMultiCast(4);

		g_Log.AddC(TColor::Blue, "[ArcaBattle] [State Change] -> STATE: MEMBER_REG");
	}
}

void CArcaBattle::ProcStateReg()
{
	char szTemp[256];
	ULONGLONG i64TICK_MSEC = GetTickCount64() - this->m_i64_AB_TICK_COUNT;

	if (i64TICK_MSEC >= 1000)
	{
		this->m_i64_AB_REMAIN_MSEC -= i64TICK_MSEC;
		this->m_i64_AB_TICK_COUNT = GetTickCount64();

		if (this->m_i64_AB_REMAIN_MSEC <= 60000 * this->m_iAB_RegTimeMin)
		{
			if (GetTickCount64() - this->m_i64_AB_TICK_COUNT_2 > 60000)
			{
				this->m_i64_AB_TICK_COUNT_2 = GetTickCount64();
				this->m_iAB_NOTIFY_COUNT = this->m_i64_AB_REMAIN_MSEC / 60000;

				memset(szTemp, 0x00, sizeof(szTemp));
				sprintf(szTemp, Lang.GetText(0,328), this->m_iAB_NOTIFY_COUNT + 1);
				this->SendMapServerGroupMsg(szTemp);

				g_Log.Add("[ArcaBattle] Registration to Arca War of guild members is open for %d minutes", this->m_iAB_NOTIFY_COUNT + 1);

				if (this->m_iAB_NOTIFY_COUNT == this->m_iAB_RegTimeMin - 1 || this->m_iAB_NOTIFY_COUNT == (this->m_iAB_RegTimeMin / 2) - 1)
				{
					this->GDReqJoinMemberUnder();
				}
			}
		}
	}

	if (this->m_i64_AB_REMAIN_MSEC <= 0)
	{
		this->m_i64_AB_TICK_COUNT_2 = 0;
		this->m_i64_AB_REMAIN_MSEC = 60000 * this->m_iAB_WaitTimeMin;
		this->m_iAB_NOTIFY_COUNT = this->m_i64_AB_REMAIN_MSEC / 60000;
		this->SetState(5);
		this->GDReqArcaBattleProcMultiCast(5);

		g_Log.AddC(TColor::Blue, "[ArcaBattle] [State Change] -> STATE: READY");
	}
}

void CArcaBattle::ProcStateReady()
{
	char szTemp[256];
	ULONGLONG i64TICK_MSEC = GetTickCount64() - this->m_i64_AB_TICK_COUNT;

	if (i64TICK_MSEC >= 1000)
	{
		this->m_iAB_NOTIFY_COUNT = this->m_i64_AB_REMAIN_MSEC / 1000;

		if (this->m_iAB_NOTIFY_COUNT > 0)
		{
			memset(szTemp, 0x00, sizeof(szTemp));
			sprintf(szTemp, Lang.GetText(0,329), this->m_iAB_NOTIFY_COUNT);
			this->SendMapServerGroupMsg(szTemp);

			g_Log.Add("[ArcaBattle] %d remaining seconds to warp to the Arca War Please wait in a safe area.", this->m_iAB_NOTIFY_COUNT);
		}

		this->m_i64_AB_REMAIN_MSEC -= i64TICK_MSEC;
		this->m_i64_AB_TICK_COUNT = GetTickCount64();
	}

	if (this->m_i64_AB_REMAIN_MSEC <= 0)
	{
		this->m_i64_AB_TICK_COUNT_2 = 0;
		this->m_i64_AB_REMAIN_MSEC = 60000 * this->m_iAB_WaitPartyTimeMin;
		this->m_iAB_NOTIFY_COUNT = this->m_i64_AB_REMAIN_MSEC / 60000;
		this->SetState(6);
		this->GDReqArcaBattleProcMultiCast(6);

		g_Log.AddC(TColor::Blue, "[ArcaBattle] [State Change] -> STATE: PARTY");
	}
}

void CArcaBattle::ProcStatePaty()
{
	ULONGLONG i64TICK_MSEC = GetTickCount64() - this->m_i64_AB_TICK_COUNT;

	if ( i64TICK_MSEC >= 1000 )
	{
		this->m_i64_AB_REMAIN_MSEC -= i64TICK_MSEC;
		this->m_i64_AB_TICK_COUNT = GetTickCount64();

		if ( this->m_i64_AB_REMAIN_MSEC <= 60000 * this->m_iAB_WaitPartyTimeMin && GetTickCount64() - this->m_i64_AB_TICK_COUNT_2 > 60000 )
		{
			this->m_i64_AB_TICK_COUNT_2 = GetTickCount64();
			this->m_iAB_NOTIFY_COUNT = this->m_i64_AB_REMAIN_MSEC / 60000;
			this->SendMapServerGroupMsg(Lang.GetText(0,330));

			g_Log.Add("[ArcaBattle] Arca War is now accessible through the NPC named Lord Lesnar.");
			this->GCArcaBattleSendNotiMsg(11, this->m_iAB_NOTIFY_COUNT + 1, 0);
		}

		if ( this->m_i64_AB_REMAIN_MSEC <= 30000 && this->m_i64_AB_REMAIN_MSEC >= 0 )
		{
			this->m_i64_AB_TICK_COUNT = GetTickCount64();
			this->m_iAB_NOTIFY_COUNT = this->m_i64_AB_REMAIN_MSEC / 1000;
			this->GCArcaBattleSendNotiMsg(12, this->m_iAB_NOTIFY_COUNT + 1, 0);
		}
	}

	if (GetTickCount64() - this->m_i64_AB_10TICK_COUNT >= 10000)
	{
		for (int i = g_ConfigRead.server.GetObjectStartUserIndex(); i < g_ConfigRead.server.GetObjectMax(); i++)
		{
			LPOBJ lpObj = &gObj[i];

			if (gObjIsConnected(lpObj) && lpObj->Type == OBJ_USER)
			{
				this->GCArcaBattleUserInfo(i);
			}
		}

		this->m_i64_AB_10TICK_COUNT = GetTickCount64();
	}

	if ( this->m_i64_AB_REMAIN_MSEC <= 0 )
	{
		this->m_i64_AB_TICK_COUNT_2 = 0;
		this->m_iAB_PlayNotifyTimeMin = 3;
		this->m_i64_AB_REMAIN_MSEC = 60000 * this->m_iAB_PlayTimeMin;
		this->m_iAB_NOTIFY_COUNT = this->m_i64_AB_REMAIN_MSEC / 60000;
		this->SetState(7);
		this->GDReqArcaBattleProcMultiCast(7);
		this->SetObelisk();
		this->GDReqArcaBattleAllJoinUser();
		this->m_i64_AB_10TICK_COUNT = GetTickCount64();
		g_Log.AddC(TColor::Blue, "[ArcaBattle] [State Change] -> STATE: PLAYING");
	}
}

void CArcaBattle::ProcStatePlaying()
{
	ULONGLONG i64TICK_MSEC = GetTickCount64() - this->m_i64_AB_TICK_COUNT;
	ULONGLONG i64TICK_10MSEC = GetTickCount64() - this->m_i64_AB_10TICK_COUNT;

	if ( i64TICK_MSEC >= 1000 )
	{
		this->m_i64_AB_REMAIN_MSEC -= i64TICK_MSEC;
		this->m_i64_AB_TICK_COUNT = GetTickCount64();

		if ( this->m_i64_AB_REMAIN_MSEC >= 60000 * (this->m_iAB_PlayTimeMin - this->m_iAB_PlayNotifyTimeMin) && GetTickCount64() - this->m_i64_AB_TICK_COUNT_SEC >= 70000 )
		{
			this->m_i64_AB_TICK_COUNT_SEC = GetTickCount64();
			this->GCArcaBattleSendNotiMsg(0, 0, 0);
		}

		if ( this->m_i64_AB_REMAIN_MSEC <= 60000 * this->m_iAB_PlayTimeMin && GetTickCount64() - this->m_i64_AB_TICK_COUNT_2 >= 60000 )
		{
			this->m_i64_AB_TICK_COUNT_2 = GetTickCount64();
			this->m_iAB_NOTIFY_COUNT = this->m_i64_AB_REMAIN_MSEC / 60000;
			this->GCArcaBattleSendNotiMsg(6, this->m_iAB_NOTIFY_COUNT + 1, 0);
			g_Log.Add("[ArcaBattle] Event will end in %d minutes", this->m_iAB_NOTIFY_COUNT + 1);
		}

		if ( this->m_i64_AB_REMAIN_MSEC <= 60000 && this->m_i64_AB_REMAIN_MSEC >= 0 )
		{
			this->m_i64_AB_TICK_COUNT_2 = GetTickCount64();
			this->m_iAB_NOTIFY_COUNT = this->m_i64_AB_REMAIN_MSEC / 1000;
			this->GCArcaBattleSendNotiMsg(7, this->m_iAB_NOTIFY_COUNT + 1, 0);
			g_Log.Add("[ArcaBattle] Event will end in %d seconds", this->m_iAB_NOTIFY_COUNT + 1);
		}

		this->ChkAuraUserHover();
		this->RegenMonsterRun();
		this->SendObeliskLife();
		this->SendObeliskState();
		this->RegenObelisk();

		if (i64TICK_10MSEC >= 10000)
		{
			for (int i = g_ConfigRead.server.GetObjectStartUserIndex(); i < g_ConfigRead.server.GetObjectMax(); i++)
			{
				LPOBJ lpObj = &gObj[i];

				if (gObjIsConnected(lpObj) && lpObj->Type == OBJ_USER)
				{
					this->GCArcaBattleUserInfo(i);
				}
			}

			this->m_i64_AB_10TICK_COUNT = GetTickCount64();
		}

		if ( this->m_i64_AB_REMAIN_MSEC <= 0 )
		{
			this->m_i64_AB_TICK_COUNT_2 = 0;
			this->m_i64_AB_REMAIN_MSEC = 30000;
			this->m_iAB_NOTIFY_COUNT = this->m_i64_AB_REMAIN_MSEC / 60000;
			this->SetState(8);
			this->GDReqArcaBattleProcMultiCast(8);
			this->NotifyArcaBattle(4);
			this->DeleteArcaBattleAllMonster();
			g_Log.AddC(TColor::Blue, "[ArcaBattle] [State Change] -> STATE: RESULT");
		}
	}
}

void CArcaBattle::ProcStateResult()
{
	ULONGLONG i64TICK_MSEC = GetTickCount64() - this->m_i64_AB_TICK_COUNT;

	if ( i64TICK_MSEC >= 1000 )
	{
		this->m_i64_AB_REMAIN_MSEC -= i64TICK_MSEC;
		this->m_i64_AB_TICK_COUNT = GetTickCount64();

		if ( this->m_i64_AB_REMAIN_MSEC > 0 )
		{
			this->m_iAB_NOTIFY_COUNT = this->m_i64_AB_REMAIN_MSEC / 1000;
			this->GCArcaBattleSendNotiMsg(8, this->m_iAB_NOTIFY_COUNT + 1, 0);
			g_Log.Add("[ArcaBattle] Arca War has ended after %d seconds users will be warped to village.", this->m_iAB_NOTIFY_COUNT + 1);
		}

		if ( this->m_i64_AB_REMAIN_MSEC <= 0 )
		{
			this->m_i64_AB_TICK_COUNT_2 = 0;
			this->m_i64_AB_REMAIN_MSEC = 60000 * this->m_iAB_ChannelTimeMin;
			this->m_iAB_NOTIFY_COUNT = this->m_i64_AB_REMAIN_MSEC / 60000;

			int iGateNum = 426;
			BYTE btMAPNUM = 92;
			short sY; 
			short sLVL; 
			char btDIR; 
			short sX;

			int iMapNumber = gGateC.GetGate(426, (short&)sX, (short&)sY, (BYTE &)btMAPNUM, (BYTE &)btDIR, (short &)sLVL);

			if ( iMapNumber >= 0 )
			{
				for ( int n = g_ConfigRead.server.GetObjectStartUserIndex(); n < g_ConfigRead.server.GetObjectMax(); ++n )
				{
					if ( gObj[n].Connected == PLAYER_PLAYING )
					{
						if ( gObj[n].Type == OBJ_USER )
							gObjTeleport(n, btMAPNUM, sX, sY);
					}
				}
			}

			this->GDReqWinGuildInfoInsert();
			this->SendPlayResult();
			this->GiveRewardBuffWinningGuild();
			this->SetState(9);
			this->GDReqArcaBattleProcMultiCast(9);
			this->GDReqMarkRegAllDel();

			for (int iCnt = 0; iCnt < 5; ++iCnt)
			{
				if (this->m_stObeliskState[iCnt].m_iMonIndex >= 0)
				{
					g_Log.Add("[ArcaBattle][Obelisk] Delete Monster - %d ", this->m_stObeliskState[iCnt].m_iMonIndex);
					gObjDel(this->m_stObeliskState[iCnt].m_iMonIndex);
				}

				this->m_stObeliskState[iCnt].Clear();
			}

			g_Log.AddC(TColor::Blue, "[ArcaBattle] [State Change] -> STATE: CHANNEL_CLOSE");
		}
	}
}

void CArcaBattle::ProcStateChannelClose()
{
	ULONGLONG i64TICK_MSEC = GetTickCount64() - this->m_i64_AB_TICK_COUNT;

	if ( i64TICK_MSEC >= 1000 )
	{
		this->m_i64_AB_REMAIN_MSEC -= i64TICK_MSEC;
		this->m_i64_AB_TICK_COUNT = GetTickCount64();

		if ( this->m_i64_AB_REMAIN_MSEC <= 60000 * this->m_iAB_ChannelTimeMin && GetTickCount64() - this->m_i64_AB_TICK_COUNT_2 >= 60000 )
		{
			this->m_i64_AB_TICK_COUNT_2 = GetTickCount64();
			this->m_iAB_NOTIFY_COUNT = this->m_i64_AB_REMAIN_MSEC / 60000;
			this->GCArcaBattleSendNotiMsg(9, this->m_iAB_NOTIFY_COUNT + 1, 0);
			g_Log.Add("%d minutes remaining to close Arca War Channel", this->m_iAB_NOTIFY_COUNT + 1);
		}

		if ( this->m_i64_AB_REMAIN_MSEC <= 30000 && this->m_i64_AB_REMAIN_MSEC >= 0 )
		{
			this->m_iAB_NOTIFY_COUNT = this->m_i64_AB_REMAIN_MSEC / 1000;
			this->GCArcaBattleSendNotiMsg(10, this->m_iAB_NOTIFY_COUNT + 1, 0);
			g_Log.Add("%d seconds remaining to close Arca War Channel,", this->m_iAB_NOTIFY_COUNT + 1);
		}

		if ( this->m_bWindowClose == false && this->m_i64_AB_REMAIN_MSEC <= 10000 && this->m_i64_AB_REMAIN_MSEC > 0)
		{
			this->m_bWindowClose = true;
			_tagPMSG_SEND_WINDOW_CLOSE pMsg;

			pMsg.h.c = 0xC1;
			pMsg.h.size = sizeof(pMsg);
			pMsg.h.headcode = 0xF8;
			pMsg.h.subcode = 0x4D;

			for (int n = g_ConfigRead.server.GetObjectStartUserIndex(); n < g_ConfigRead.server.GetObjectMax(); n++)
			{
				if (gObj[n].Connected == PLAYER_PLAYING && gObj[n].Type == OBJ_USER)
				{
					IOCP.DataSend(n, (LPBYTE)&pMsg, pMsg.h.size);
				}
			}

			g_Log.Add("[ArcaBattle] Send Close Window Sec[%d]", this->m_iAB_NOTIFY_COUNT + 1);
		}

		if ( this->m_i64_AB_REMAIN_MSEC <= 0 )
		{
			this->Clear();

			for ( int n = g_ConfigRead.server.GetObjectStartUserIndex(); n < g_ConfigRead.server.GetObjectMax(); ++n )
			{
				if ( gObj[n].Connected == 3 )
				{
					if ( gObj[n].Type == OBJ_USER )
						gObjMoveGate(n, 27);
				}
			}

			this->SetState(1);
			this->GDReqArcaBattleProcMultiCast(1);
			g_Log.AddC(TColor::Blue, "[ArcaBattle] [State Change] -> STATE: CHECK_SYNC");
		}
	}
}

void CArcaBattle::ProcStatePlayEnd()
{
	ULONGLONG i64TICK_MSEC = GetTickCount64() - this->m_i64_AB_TICK_COUNT;

	if (i64TICK_MSEC >= 1000)
	{
		this->m_i64_AB_REMAIN_MSEC -= i64TICK_MSEC;
		this->m_i64_AB_TICK_COUNT = GetTickCount64();
	}

	if (this->m_i64_AB_REMAIN_MSEC <= 0)
	{
		for (int i = 0; i < 5; i++)
		{
			this->m_stArcaBattleResultInfo[i].iGuildNum = this->m_stObeliskState[i].m_iOccupyGuildNum;
			this->m_stArcaBattleResultInfo[i].iObeliskAttrKind = this->m_stObeliskInfo[i].m_iAttrKind;
		}
	}
}

void CArcaBattle::DeleteArcaBattleAllMonster()
{
	for (int nGroupNumber = 0; nGroupNumber < 50; nGroupNumber++)
	{
		if (this->m_stMonsterGroupInfo[nGroupNumber].m_nGroupNumber >= 0)
		{
			if (nGroupNumber < 0 || nGroupNumber > 50)
				return;

			for (int i = 0; i < 255; i++)
			{
				if (this->m_stMonsterGroupInfo[nGroupNumber].m_stMonsterAccountNumInfo[i].m_nIndex > -1)
				{
					int iMonsterIndex = this->m_stMonsterGroupInfo[nGroupNumber].m_stMonsterAccountNumInfo[i].m_nIndex;

					gObjDel(iMonsterIndex);
					g_Log.Add("[ArcaBattle][MonsterGroupRegen] Delete Monster - %d ", iMonsterIndex);

					this->m_stMonsterGroupInfo[nGroupNumber].m_stMonsterAccountNumInfo[i].m_nIndex = -1;
					this->m_stMonsterGroupInfo[nGroupNumber].m_stMonsterAccountNumInfo[i].bItemDrop = false;
				}
			}

			this->m_stMonsterGroupInfo[nGroupNumber].m_i64NextRegenTime = GetTickCount64();
		}
	}
}

void CArcaBattle::Run()
{
	if ( !this->IsArcaBattleServer() )
	{
		return;
	}

	switch ( this->m_iAB_State )
	{
		case 1:
			this->CheckSync();
			break;
		case 2:
			this->ProcStateClosed();
			break;
		case 3:
			this->ProcStateMasterReg();
			break;
		case 4:
			this->ProcStateReg();
			break;
		case 5:
			this->ProcStateReady();
			break;
		case 6:
			this->ProcStatePaty();
			break;
		case 7:
			this->ProcStatePlaying();
			break;
		case 8:
			this->ProcStateResult();
			break;
		case 9:
			this->ProcStateChannelClose();
			break;
		case 10:
			this->ProcStatePlayEnd();
			break;
		default:
			return;
	}
}

void CArcaBattle::SendMapServerGroupMsg(char *lpszMsg)
{
	int MapServerGroup = g_MapServerManager.GetMapSvrGroup();
	GS_GDReqMapSvrMsgMultiCast(MapServerGroup, lpszMsg);
}

int CArcaBattle::GetJoinMemberCnt()
{
	return this->m_iJoinGuildMemberCnt;
}

BOOL CArcaBattle::EnterArcaBattleEvent(int aIndex)
{
	if (this->m_iCurUserCount < 0 || this->m_iCurUserCount > 299)
	{
		g_Log.Add("[ArcaBattle] Entered User Over UserCount [%d]", this->m_iCurUserCount);
		return FALSE;
	}

	for (int i = 0; i < this->m_iCurUserCount; i++)
	{
		if (strcmp(this->m_UserData[i].szCharName, gObj[aIndex].Name) == 0)
		{
			this->m_UserData[this->m_iCurUserCount].iIndex = aIndex;
			g_Log.Add("[ArcaBattle] Entered ArcaBattle [%s] [%s] UserCnt [%d]", gObj[aIndex].AccountID, gObj[aIndex].Name, this->m_iCurUserCount);
			this->GCArcaBattleUserInfo(aIndex);

			if (this->GetState() == 7)
			{
				this->GCArcaBattleCurrentStatusForReConnectUser(aIndex);
			}

			return FALSE;
		}
	}

	memcpy(this->m_UserData[this->m_iCurUserCount].szCharName, gObj[aIndex].Name, MAX_ACCOUNT_LEN);
	this->m_UserData[this->m_iCurUserCount].iIndex = aIndex;
	this->m_iCurUserCount++;

	g_Log.Add("[ArcaBattle] Enter ArcaBattle [%s] [%s] UserCnt [%d]", gObj[aIndex].AccountID, gObj[aIndex].Name, this->m_iCurUserCount);
	this->GCArcaBattleUserInfo(aIndex);

	if (this->GetState() == 7)
	{
		this->GCArcaBattleCurrentStatusForReConnectUser(aIndex);
	}

	return TRUE;
}

_tagArcaBattleUserInfo * CArcaBattle::GetUserInfo(char *szName)
{
	for ( int i = 0; i < this->m_iCurUserCount; ++i )
	{
		if( strcmp(this->m_UserData[i].szCharName, szName) == 0)
		{
			return &this->m_UserData[i];
		}
	}

	return NULL;
}

void CArcaBattle::SendObeliskLife()
{
	_tagPMSG_SEND_OBELISK_LIFE pMsg;

	for (int i = 0; i < 5; i++)
	{
		int iObeliskIndex = this->m_stObeliskState[i].m_iMonIndex;

		if (this->m_stObeliskState[i].m_iMonIndex >= 0)
		{
			if (iObeliskIndex < 0 || iObeliskIndex > 9999)
				return;

			if (gObj[iObeliskIndex].Live)
			{
				float currentLife = (gObj[iObeliskIndex].Life / gObj[iObeliskIndex].MaxLife / 10.0f);
				pMsg.m_stObeliskLife[i].btLife = (int)ceil(currentLife) & 0xF;
				pMsg.m_stObeliskLife[i].btAttribute = this->m_stObeliskInfo[i].m_iAttrKind;
			}

			else
			{
				pMsg.m_stObeliskLife[i].btLife = 0;
				pMsg.m_stObeliskLife[i].btAttribute = this->m_stObeliskInfo[i].m_iAttrKind;
			}
		}
	}

	pMsg.h.c = 0xC1;
	pMsg.h.size = sizeof(pMsg);
	pMsg.h.headcode = 0xF8;
	pMsg.h.subcode = 0x3F;

	for (int n = g_ConfigRead.server.GetObjectStartUserIndex(); n < g_ConfigRead.server.GetObjectMax(); n++)
	{
		if (gObjIsConnected(n) == TRUE && gObj[n].Type == OBJ_USER)
		{
			IOCP.DataSend(n, (LPBYTE)&pMsg, pMsg.h.size);
		}
	}
}

void CArcaBattle::SendObeliskState()
{
	_tagPMSG_SEND_OBELISK_STATE pMsg; 

	pMsg.h.c = 0xC1;
	pMsg.h.size = sizeof(pMsg);
	pMsg.h.headcode = 0xF8;
	pMsg.h.subcode = 0x3A;

	for ( int i = 0; i < 5; ++i )
	{
		int iObeliskIndex = this->m_stObeliskState[i].m_iMonIndex;

		if (this->m_stObeliskState[i].m_iMonIndex >= 0)
		{
			LPOBJ lpObj = &gObj[iObeliskIndex];
			pMsg.btObeliskState = lpObj->Life > 0.0;
			pMsg.btObeliskAttribute = (BYTE)this->m_stObeliskInfo[i].m_iAttrKind;
			pMsg.btAuraState = 2;

			for ( int j = 0; j < 3; j++ )
			{
				if ( this->m_stObeliskState[i].m_stAuraState[j].m_btAuraState != 2 )
				{
					pMsg.btAuraState = 0;
					break;
				}
			}

			for ( int n = 0; n < MAX_VIEWPORT; n++ )
			{
				if (lpObj->VpPlayer2[n].type == OBJ_USER )
				{
					if ( lpObj->VpPlayer2[n].state )
					{
						IOCP.DataSend(lpObj->VpPlayer2[n].number, (LPBYTE)&pMsg, sizeof(pMsg));
					}
				}
			}
		}
	}
}
	
int CArcaBattle::RegenObelisk()
{
	for (int i = 0; i < this->m_iObeliskCnt; i++)
	{
		if(i == this->m_iSkipObelisk)
		{
			continue;
		}

		int iObeliskIndex = this->m_stObeliskState[i].m_iMonIndex;

		if (ObjectMaxRange(this->m_stObeliskState[i].m_iMonIndex) == false)
			return FALSE;

		if (gObj[iObeliskIndex].Live == 1)
		{
			this->m_stObeliskInfo[i].m_i64Tick = GetTickCount64();
		}

		else
		{
			if (GetTickCount64() - this->m_stObeliskInfo[i].m_i64Tick >= this->m_stObeliskInfo[i].m_i64RegenTime)
			{
				int result = gObjAddMonster(this->m_stObeliskInfo[i].m_iMapIndex);

				if (result >= 0)
				{
					gObjSetMonster(result, this->m_stObeliskInfo[i].m_iMonsterNum);
					gObj[result].Class = this->m_stObeliskInfo[i].m_iMonsterNum;
					gObj[result].MapNumber = this->m_stObeliskInfo[i].m_iMapIndex;
					gObj[result].X = this->m_stObeliskInfo[i].m_iPosX;
					gObj[result].Y = this->m_stObeliskInfo[i].m_iPosY;
					gObj[result].TX = this->m_stObeliskInfo[i].m_iPosX;
					gObj[result].TY = this->m_stObeliskInfo[i].m_iPosY;
					gObj[result].m_OldX = this->m_stObeliskInfo[i].m_iPosX;
					gObj[result].m_OldY = this->m_stObeliskInfo[i].m_iPosY;
					gObj[result].StartX = this->m_stObeliskInfo[i].m_iPosX;
					gObj[result].StartY = this->m_stObeliskInfo[i].m_iPosY;
					gObj[result].Dir = 1;
					gObj[result].Live = 1;
					gObj[result].DieRegen = FALSE;
					gObj[result].m_State = 1;
					gObj[result].MaxRegenTime = 0;
					gObj[result].MaxLife = this->m_stObeliskInfo[i].m_iOccupyHP;
					gObj[result].Life = this->m_stObeliskInfo[i].m_iOccupyHP;
					this->m_stObeliskState[i].m_iMonIndex = result;

					g_Log.Add("[ArcaBattle] (%d) Obelisk be Regen.", result);
				}
			}
		}
	}

	return TRUE;
}

void CArcaBattle::SetRandomObeliskAttr()
{
	int iArrMonsterIndex[5]; 
	int iArrObeliskAttr[5];
	int iObeliskAttrCnt = 0; 

	iArrObeliskAttr[0] = 0;
	iArrObeliskAttr[1] = 0;
	iArrObeliskAttr[2] = 0;
	iArrObeliskAttr[3] = 0;
	iArrObeliskAttr[4] = 0;
	iArrMonsterIndex[0] = 598;
	iArrMonsterIndex[1] = 599;
	iArrMonsterIndex[2] = 600;
	iArrMonsterIndex[3] = 601;
	iArrMonsterIndex[4] = 602;

	int iRand;
	bool bFlag;

	while ( iObeliskAttrCnt < 5 )
	{
		iRand = 1 + (rand()%5);
		bFlag = false;

		for ( int i=0;i<5;i++ )
		{
			if ( iArrObeliskAttr[i] == iRand )
			{
				bFlag = true;
			}
		}

		if ( !bFlag )
		{
			iArrObeliskAttr[iObeliskAttrCnt] = iRand;
			iObeliskAttrCnt++;
		}
	}

	this->m_iSkipObelisk = rand()%3;

	for (int i=0;i<5;i++)
	{
		this->m_stObeliskInfo[i].m_iAttrKind = iArrObeliskAttr[i];
		this->m_stObeliskInfo[i].m_iMonsterNum = iArrMonsterIndex[iArrObeliskAttr[i]-1];
	}
}

int CArcaBattle::SetObelisk()
{
	int nResult;

	for ( int iCnt = 0; iCnt < 5; iCnt++ )
	{
		if ( this->m_stObeliskState[iCnt].m_iMonIndex >= 0 )
		{ 
			g_Log.Add("[ArcaBattle][Obelisk] Delete Monster - %d ", this->m_stObeliskState[iCnt].m_iMonIndex);
			gObjDel(this->m_stObeliskState[iCnt].m_iMonIndex);
		}

		this->m_stObeliskState[iCnt].Clear();
	}

	for (int i = 0; i < this->m_iObeliskCnt; i++ )
	{
		if (i == this->m_iSkipObelisk)
		{
			continue;
		}

		nResult = gObjAddMonster(this->m_stObeliskInfo[i].m_iMapIndex);

		if ( nResult >= 0 )
		{
			gObjSetMonster(nResult, this->m_stObeliskInfo[i].m_iMonsterNum);
			gObj[nResult].Class = this->m_stObeliskInfo[i].m_iMonsterNum;
			gObj[nResult].MapNumber = this->m_stObeliskInfo[i].m_iMapIndex;
			gObj[nResult].X = this->m_stObeliskInfo[i].m_iPosX;
			gObj[nResult].Y = this->m_stObeliskInfo[i].m_iPosY;
			gObj[nResult].TX = this->m_stObeliskInfo[i].m_iPosX;
			gObj[nResult].TY = this->m_stObeliskInfo[i].m_iPosY;
			gObj[nResult].m_OldX = this->m_stObeliskInfo[i].m_iPosX;
			gObj[nResult].m_OldY = this->m_stObeliskInfo[i].m_iPosY;
			gObj[nResult].StartX = this->m_stObeliskInfo[i].m_iPosX;
			gObj[nResult].StartY = this->m_stObeliskInfo[i].m_iPosY;
			gObj[nResult].Dir = 1;
			gObj[nResult].Live = 1;
			gObj[nResult].DieRegen = 0;
			gObj[nResult].m_State = 1;
			gObj[nResult].MaxRegenTime = 0;
			gObj[nResult].MaxLife = this->m_stObeliskInfo[i].m_iHP;
			gObj[nResult].Life = this->m_stObeliskInfo[i].m_iHP;
			this->m_stObeliskState[i].m_iMonIndex = nResult;

			g_Log.Add("[ArcaBattle] (%d) Obelisk be created.", nResult);
		}

		for ( int nAura = 0; nAura < 3; nAura++ )
		{
			nResult = gObjAddMonster(this->m_stObeliskInfo[i].m_stAuraInfo[nAura].m_iMapIndex);

			if ( nResult >= 0 )
			{
				gObjSetMonster(nResult, 603);
				gObj[nResult].Class = 603;
				gObj[nResult].MapNumber = this->m_stObeliskInfo[i].m_stAuraInfo[nAura].m_iMapIndex;
				gObj[nResult].X = this->m_stObeliskInfo[i].m_stAuraInfo[nAura].m_iAuraPosX;
				gObj[nResult].Y = this->m_stObeliskInfo[i].m_stAuraInfo[nAura].m_iAuraPosY;
				gObj[nResult].TX = this->m_stObeliskInfo[i].m_stAuraInfo[nAura].m_iAuraPosX;
				gObj[nResult].TY = this->m_stObeliskInfo[i].m_stAuraInfo[nAura].m_iAuraPosX;
				gObj[nResult].m_OldX = this->m_stObeliskInfo[i].m_stAuraInfo[nAura].m_iAuraPosX;
				gObj[nResult].m_OldY = this->m_stObeliskInfo[i].m_stAuraInfo[nAura].m_iAuraPosX;
				gObj[nResult].StartX = this->m_stObeliskInfo[i].m_stAuraInfo[nAura].m_iAuraPosX;
				gObj[nResult].StartY = this->m_stObeliskInfo[i].m_stAuraInfo[nAura].m_iAuraPosX;
				gObj[nResult].Dir = 1;
				gObj[nResult].Live = TRUE;
				gObj[nResult].DieRegen = 0;
				gObj[nResult].m_State = 1;
				gObj[nResult].MaxRegenTime = 0;
				gObj[nResult].MaxLife = 1.0;
				gObj[nResult].Life = 1.0;
				this->m_stObeliskState[i].m_stAuraState[nAura].m_iIndex = nResult;

				g_Log.Add("[ArcaBattle] (%d) Aura be created.", nResult);
			}
		}
	}

	return TRUE;
}

void CArcaBattle::ChkAuraUserHover()
{
	for ( int i = 0; i < this->m_iObeliskCnt; i++ )
	{
		this->m_stObeliskState[i].m_iAuraState = 0;

		for ( int j = 0; j < 3; j++ )
		{
			this->m_stObeliskState[i].m_stAuraState[j].m_btAuraState = 0;
			this->m_stObeliskState[i].m_stAuraState[j].m_iHoverUserIndex = 0;
		}
	}

	for ( int n = g_ConfigRead.server.GetObjectStartUserIndex(); n < g_ConfigRead.server.GetObjectMax(); n++ )
	{
		LPOBJ lpObj = &gObj[n];

		if ( gObjIsConnected(n) == TRUE && lpObj->Type == OBJ_USER )
		{
			for (int i = 0; i < this->m_iObeliskCnt; i++ )
			{
				if (i == this->m_iSkipObelisk)
				{
					continue;
				}

				for ( int j = 0; j < 3; j++ )
				{
					if (this->m_stObeliskInfo[i].m_stAuraInfo[j].m_iAuraPosX == lpObj->X && this->m_stObeliskInfo[i].m_stAuraInfo[j].m_iAuraPosY == lpObj->Y)
					{
						this->m_stObeliskState[i].m_stAuraState[j].m_btAuraState = 1;
						this->m_stObeliskState[i].m_stAuraState[j].m_iHoverUserIndex = n;
						
						g_Log.Add( "[ArcaBattle] Obelisk foot hold [on][%s][%s][%s] : Pos X[%d] Y[%d]",
							lpObj->AccountID, lpObj->Name, lpObj->m_PlayerData->lpGuild->Name, lpObj->X, lpObj->Y);
					}
				}
			}
		}
	}

	for ( int i = 0; i < this->m_iObeliskCnt; ++i )
	{
		if ( i == this->m_iSkipObelisk )
		{
			continue;
		}

		int iAurReleaseCnt = 0;
		int iHoverUserIndex_1 = 0;
		int iHoverUserIndex_2 = 0;

		for ( int j = 0; j < 3; ++j )
		{
			if ( this->m_stObeliskState[i].m_stAuraState[j].m_btAuraState == 1 )
			iAurReleaseCnt++;
		}

		if ( iAurReleaseCnt == 3 )
		{
			this->m_stObeliskState[i].m_iAuraState = 2;

			for ( int j = 0; j < 2; ++j )
			{
				iHoverUserIndex_1 = this->m_stObeliskState[i].m_stAuraState[j].m_iHoverUserIndex;
				iHoverUserIndex_2 = this->m_stObeliskState[i].m_stAuraState[j+1].m_iHoverUserIndex;

				if (this->m_stObeliskState[i].m_stAuraState[j].m_iHoverUserIndex == -1)
				{
					g_Log.AddC(TColor::Red, "error %s %d", __FILE__, __LINE__);
					return;
				}

				if (gObj[iHoverUserIndex_1].m_PlayerData->GuildNumber != gObj[iHoverUserIndex_2].m_PlayerData->GuildNumber)
					this->m_stObeliskState[i].m_iAuraState = 1;
			}

			if (this->m_stObeliskState[i].m_iAuraState == 2)
			{
				int iUserIndex = this->m_stObeliskState[i].m_stAuraState[0].m_iHoverUserIndex;

				if(iUserIndex == -1)
				{
					g_Log.AddC(TColor::Red, "error %s %d", __FILE__, __LINE__);
					return;
				}

				if (this->m_stObeliskState[i].m_iAuraReleaseGuildNum != gObj[iUserIndex].m_PlayerData->GuildNumber)
				{
					int iObeliskIndex = this->m_stObeliskState[i].m_iMonIndex;

					if ( iObeliskIndex == -1 )
					{
						g_Log.AddC(TColor::Red, "error %s %d", __FILE__, __LINE__);
						return;
					}

					if (gObj[iObeliskIndex].Live == 1 )
					{
						if (this->m_stObeliskState[i].m_iObeliskState == 1)
						{
							gObj[iObeliskIndex].Life = this->m_stObeliskInfo[i].m_iOccupyHP;
						}

						else
						{
							gObj[iObeliskIndex].Life = this->m_stObeliskInfo[i].m_iHP;
						}
					}
				}

				if (this->m_stObeliskState[i].m_iAuraReleaseGuildNum != gObj[iUserIndex].m_PlayerData->GuildNumber &&
					this->m_stObeliskState[i].m_iOccupyGuildNum != gObj[iUserIndex].m_PlayerData->GuildNumber)
				{
					this->m_stObeliskState[i].m_iAuraReleaseGuildNum = gObj[iUserIndex].m_PlayerData->GuildNumber;
					_GUILD_INFO_STRUCT * lpGuild = Guild.SearchGuild_Number(this->m_stObeliskState[i].m_iAuraReleaseGuildNum);

					if (!lpGuild)
					{
						g_Log.Add("[ArcaBattle] Error Can Not Find Guild ChkAuraUserHover() lpGuild NULL GuildNumber : %d", this->m_stObeliskState[i].m_iAuraReleaseGuildNum);
						return;
					}
					this->GCArcaBattleCurrentStatus(this->m_stObeliskInfo[i].m_iGroupNumber, this->m_stObeliskInfo[i].m_iAttrKind, ARCA_BATTLE_OBELISK_CAPTURING, this->m_stObeliskState[i].m_iAuraReleaseGuildNum, lpGuild->Name, 0);
					this->GCArcaBattleSendNotiMsg(2, 0, lpGuild->Name);

					for (int j = 0; j < this->m_iABJoinUserCnt; j++)
					{
						DWORD dwTempGuildNum = this->m_stABJoinUserInfo[j].dwGuild;

						if (this->m_stABJoinUserInfo[j].dwGuild != this->m_stObeliskState[i].m_iAuraReleaseGuildNum &&
							this->m_stABJoinUserInfo[j].dwGuild != dwTempGuildNum)
						{
							this->GCArcaBattleSendNotiMsg(1, 0, this->m_stABJoinUserInfo[j].szGuildName);
						}
					}
				}

				for ( int j = 0; j < 3; ++j )
					this->m_stObeliskState[i].m_stAuraState[j].m_btAuraState = 2;
			}

			else
			{
				if (this->m_stObeliskState[i].m_iObeliskState == 1)
				{
					this->m_stObeliskState[i].m_iAuraReleaseGuildNum = this->m_stObeliskState[i].m_iOccupyGuildNum;
				}

				else
				{
					this->m_stObeliskState[i].m_iAuraReleaseGuildNum = -1;
				}
			}
		}

		else
		{
			if (iAurReleaseCnt > 0)
			{
				this->m_stObeliskState[i].m_iAuraState = 1;

				if (this->m_stObeliskState[i].m_iAuraReleaseGuildNum == -1 || this->m_stObeliskState[i].m_iObeliskState == 1)
				{
					if (this->m_stObeliskState[i].m_iAuraReleaseGuildNum != -1 && this->m_stObeliskState[i].m_iObeliskState == 1)
					{
						this->GCArcaBattleCurrentStatus(this->m_stObeliskInfo[i].m_iGroupNumber, this->m_stObeliskInfo[i].m_iAttrKind, ARCA_BATTLE_OBELISK_CAPTURED, this->m_stObeliskState[i].m_iOccupyGuildNum, this->m_stObeliskState[i].m_szOccupyGuildName, 0);
						this->m_stObeliskState[i].m_iAuraReleaseGuildNum = -1;
					}
				}

				else
				{
					this->GCArcaBattleCurrentStatus(this->m_stObeliskInfo[i].m_iGroupNumber, this->m_stObeliskInfo[i].m_iAttrKind, ARCA_BATTLE_OBELISK_UNCAPTURED, 0, NULL, 0);
					this->m_stObeliskState[i].m_iAuraReleaseGuildNum = -1;
				}
			}
		}
	}

	_tagPMSG_ARCA_BATTLE_OBELISK_INFO pMsg;
 
	pMsg.h.c = 0xC1;
	pMsg.h.size = sizeof(pMsg);
	pMsg.h.headcode = 0xF8;
	pMsg.h.subcode = 0x37;

	for (int i = 0; i < this->m_iObeliskCnt; i++ )
	{
		if ( i == this->m_iSkipObelisk )
		{
			continue;
		}

		for ( int nAura = 0; nAura < 3; nAura++ )
		{
			if (this->m_stObeliskState[i].m_stAuraState[nAura].m_iIndex == -1)
				continue;

			LPOBJ lpObj = &gObj[this->m_stObeliskState[i].m_stAuraState[nAura].m_iIndex];

			for ( int k = 0; k < MAX_VIEWPORT; ++k )
			{
				if ( lpObj->VpPlayer2[k].state == 1 && lpObj->VpPlayer2[k].type == OBJ_USER )
				{
					int iTarObjIndex = lpObj->VpPlayer2[k].number;
					pMsg.m_btObeliskAttribute = this->m_stObeliskInfo[i].m_iAttrKind;

					for ( int j = 0; j < 3; j++ )
					{
						if (this->GetChkAura() == false)
						{
							this->m_stObeliskState[i].m_stAuraState[j].m_btAuraState = 2;
						}

						pMsg.m_btAuraState[j] = this->m_stObeliskState[i].m_stAuraState[j].m_btAuraState;
						
						if(gObj[this->m_stObeliskState[i].m_stAuraState[nAura].m_iHoverUserIndex].Type == OBJ_USER && ObjectMaxRange(this->m_stObeliskState[i].m_stAuraState[nAura].m_iHoverUserIndex))
						{
							if ( this->m_stObeliskState[i].m_stAuraState[j].m_btAuraState != 0 && (this->m_stObeliskState[i].m_stAuraState[nAura].m_iHoverUserIndex != 0 ||
								gObj[iTarObjIndex].m_PlayerData->GuildNumber != gObj[this->m_stObeliskState[i].m_stAuraState[nAura].m_iHoverUserIndex].m_PlayerData->GuildNumber ))
							{
								pMsg.m_btAuraState[j] += 2;
							}
						}
					}

					IOCP.DataSend(gObj[iTarObjIndex].m_Index, (LPBYTE)&pMsg, sizeof(pMsg));
				}
			}
		}
	}
}

int CArcaBattle::GetChkAura()
{
	return this->m_iAuraChk;
}

int CArcaBattle::GetMaxObeliskCnt()
{
	return this->m_iObeliskCnt;
}

BOOL CArcaBattle::IsEnableAttackObelisk(OBJECTSTRUCT *lpObj, int iMonNumber)
{
	_tagOBELISK_STATE *pObeliskState = NULL;

	for (int i = 0; i < this->m_iObeliskCnt; i++)
	{
		if (i == this->m_iSkipObelisk)
		{
			continue;
		}

		if (this->m_stObeliskInfo[i].m_iMonsterNum == iMonNumber)
		{
			pObeliskState = &this->m_stObeliskState[i];
		}
	}

	if (pObeliskState == NULL)
	{
		return TRUE;
	}

	if (this->GetState() != 7)
	{
		return FALSE;
	}

	if (pObeliskState->m_iOccupyGuildNum == lpObj->m_PlayerData->GuildNumber)
	{
		return FALSE;
	}

	if (pObeliskState->m_iAuraState != 2)
	{
		return FALSE;
	}

	if (pObeliskState->m_iAuraReleaseGuildNum != lpObj->m_PlayerData->GuildNumber)
	{
		return FALSE;
	}

	return TRUE;
}

int CArcaBattle::IsPkEnable(LPOBJ lpObj, LPOBJ lpTargetObj)
{
	_GUILD_INFO_STRUCT * lpGuildInfo = lpObj->m_PlayerData->lpGuild;
	_GUILD_INFO_STRUCT *lpTarGuildInfo = lpTargetObj->m_PlayerData->lpGuild;

	if ( g_ConfigRead.server.GetServerType() != SERVER_ARCA )
	{
		return true;
	}

	if ( this->GetState() == 7 )
	{
		return true;
	}

	if ( !lpGuildInfo )
	{
		g_Log.AddC(TColor::Red, "[ArcaBattle] Error lpGuildInfo NULL");
		return false;
	}

	if ( !lpTarGuildInfo )
	{
		g_Log.AddC(TColor::Red, "[ArcaBattle] Error lpTarGuildInfo NULL");
		return false;
	}

	if ( lpGuildInfo->Number == lpTarGuildInfo->Number )
	{
		return false;
	}

	return true;
}

void CArcaBattle::SendArcaBattleStateAll(int iState)
{
	_tagPMSG_ARCA_BATTLE_STATE pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.size = sizeof(pMsg);
	pMsg.h.headcode = 0xF8;
	pMsg.h.subcode = 0x38;
	pMsg.m_btArcaBattleState = iState;

	for (int n = g_ConfigRead.server.GetObjectStartUserIndex(); n < g_ConfigRead.server.GetObjectMax(); n++)
	{
		LPOBJ lpObj = &gObj[n];

		if (gObjIsConnected(n) == TRUE && lpObj->Type == OBJ_USER)
		{
			IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		}
	}
}

void CArcaBattle::SendArcaBattleCurrentState(OBJECTSTRUCT *obj)
{
	_tagPMSG_ARCA_BATTLE_STATE pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.size = sizeof(pMsg);
	pMsg.h.headcode = 0xF8;
	pMsg.h.subcode = 0x38;
	pMsg.m_btArcaBattleState = this->GetState();
	IOCP.DataSend(obj->m_Index, &pMsg.h.c, pMsg.h.size);
}

int CArcaBattle::IsOccupyObelisk(LPOBJ lpObj, int iMonIndex)
{
	if (lpObj->Type != OBJ_USER)
	{
		return false;
	}

	if (this->IsArcaBattleServer() == FALSE)
	{
		return false;
	}

	if (lpObj->m_PlayerData->lpGuild == NULL)
	{
		g_Log.Add("[ArcaBattle] IsOccupyObelisk - Error lpGuild Is NULL  [%s][%s] MonIndex[%d]",
			lpObj->AccountID, lpObj->Name, iMonIndex);
		return false;
	}

	for ( int i = 0; i < this->m_iObeliskCnt; i++ )
	{
		if (i == this->m_iSkipObelisk)
		{
			continue;
		}

		if (iMonIndex == this->m_stObeliskInfo[i].m_iMonsterNum)
		{
			if (this->m_stObeliskState[i].m_iOccupyGuildNum == -1)
			{
				this->GCArcaBattleSendNotiMsg(4, 0, lpObj->m_PlayerData->lpGuild->Name);

				for (int i = 0; i < this->m_iABJoinUserCnt; i++)
				{
					if (this->m_stABJoinUserInfo[i].dwGuild != lpObj->m_PlayerData->GuildNumber)
					{
						this->GCArcaBattleSendNotiMsg(3, 0, this->m_stABJoinUserInfo[i].szGuildName);
					}
				}
			}

			else
			{
				this->GCArcaBattleSendNotiMsg(4, 0, lpObj->m_PlayerData->lpGuild->Name);

				for (int i = 0; i < this->m_iABJoinUserCnt; i++)
				{
					if (this->m_stABJoinUserInfo[i].dwGuild != lpObj->m_PlayerData->GuildNumber &&
						this->m_stABJoinUserInfo[i].dwGuild == this->m_stObeliskState[i].m_iOccupyGuildNum)
					{
						this->GCArcaBattleSendNotiMsg(5, 0, this->m_stABJoinUserInfo[i].szGuildName);
					}
				}

				for (int i = 0; i < this->m_iABJoinUserCnt; i++)
				{
					if (this->m_stABJoinUserInfo[i].dwGuild != lpObj->m_PlayerData->GuildNumber &&
						this->m_stABJoinUserInfo[i].dwGuild != this->m_stObeliskState[i].m_iOccupyGuildNum)
					{
						this->GCArcaBattleSendNotiMsg(3, 0, this->m_stABJoinUserInfo[i].szGuildName);
					}
				}
			}

			this->GCArcaBattleCurrentStatus(this->m_stObeliskInfo[i].m_iGroupNumber, this->m_stObeliskInfo[i].m_iAttrKind, ARCA_BATTLE_OBELISK_CAPTURED, lpObj->m_PlayerData->GuildNumber, lpObj->m_PlayerData->lpGuild->Name, 0);
			this->m_stObeliskState[i].m_iObeliskState = 1;
			this->m_stObeliskState[i].m_iOccupyGuildNum = lpObj->m_PlayerData->GuildNumber;
			this->m_stObeliskState[i].m_iGroupNumber = this->m_stObeliskInfo[i].m_iGroupNumber;
			memcpy(this->m_stObeliskState[i].m_szOccupyGuildName, lpObj->m_PlayerData->GuildName, MAX_GUILD_LEN + 1);

			g_Log.Add( "[ArcaBattle] Occupy Obelisk In ArcaBattle Progress  [%s][%s][%s] ObeliskGroup:[%d]",
				lpObj->AccountID, lpObj->Name, lpObj->m_PlayerData->GuildName, this->m_stObeliskState[i].m_iGroupNumber);

			return true;
		}
	}

	return false;
}

void CArcaBattle::NotifyArcaBattle(int iNotifyNum)
{
	PMSG_NOTICE pMsg; 

	if ( iNotifyNum == 3 )
	{
		TNotice::MakeNoticeMsgEx(&pMsg, 0, Lang.GetText(0,331));
		DWORD dwColor = _ARGB(-1, -1, -56, 80);
		TNotice::SetNoticeProperty(&pMsg, 0, dwColor, 1, 0, 20);
		TNotice::SendNoticeToAllUser(&pMsg);
		g_Log.Add("[ArcaBattle] Has started");
	}

	else if ( iNotifyNum == 4 )
    {
		TNotice::MakeNoticeMsgEx(&pMsg, 0, Lang.GetText(0,332));
		DWORD dwColor = _ARGB(-1, -1, -56, 80);
		TNotice::SetNoticeProperty(&pMsg, 0, dwColor, 1, 0, 20);
		TNotice::SendNoticeToAllUser(&pMsg);
		g_Log.Add("[ArcaBattle] Has finished");
	}
}

void CArcaBattle::GDReqWinGuildInfo()
{
	if (g_ConfigRead.server.GetServerType() == SERVER_BATTLECORE)
	{
		this->SetDataLoadWinGuild(TRUE);
		return;
	}

	_tagPMSG_REQ_AB_WIN_GUILD_INFO_DS pMsg; 

	pMsg.h.c = 0xC1;
	pMsg.h.size = sizeof(pMsg);
	pMsg.h.headcode = 0xF8;
	pMsg.h.subcode = 0x39;
	wsDataCli.DataSend((char*)&pMsg, pMsg.h.size);

	this->SetDataLoadWinGuild(TRUE);
}

void CArcaBattle::GDReqProcState()
{
	_tagPMSG_REQ_AB_PROC_STATE_DS pMsg;

	if ( g_ConfigRead.server.GetServerType() == SERVER_BATTLECORE )
	{
		this->SetDataLoadProcState(TRUE);
		return;
	}

	if ( this->IsArcaBattleServer() != 1 )
	{
		pMsg.h.c = 0xC1;
		pMsg.h.size = sizeof(pMsg);
		pMsg.h.headcode = 0xF8;
		pMsg.h.subcode = 0x3F;
		wsDataCli.DataSend((char*)&pMsg, sizeof(pMsg));

		this->SetDataLoadProcState(TRUE);
	}
}

void CArcaBattle::GDReqWinGuildInfoInsert()
{
	int iGuildCnt = 0; 
	_tagPMSG_REQ_AB_WIN_GUILD_INFO_INSERT_DS pMsg; 

	for ( int i = 0; i < 5; i++ )
	{
		if (this->m_stObeliskState[i].m_iObeliskState == 1)
		{
			pMsg.h.c = 0xC1;
			pMsg.h.size = sizeof(pMsg);
			pMsg.h.headcode = 0xF8;
			pMsg.h.subcode = 0x38;
			pMsg.m_stABWinGuildInfoDS[iGuildCnt].dwGuild = this->m_stObeliskState[i].m_iOccupyGuildNum;
			pMsg.m_stABWinGuildInfoDS[iGuildCnt].wOccupyObelisk = this->m_stObeliskInfo[i].m_iAttrKind;
			pMsg.m_stABWinGuildInfoDS[iGuildCnt].wObeliskGroup = this->m_stObeliskState[i].m_iGroupNumber;
			memcpy(pMsg.m_stABWinGuildInfoDS[iGuildCnt].szGuildName, this->m_stObeliskState[i].m_szOccupyGuildName, MAX_GUILD_LEN + 1);

			g_Log.Add( "[ArcaBattle] Request Win Guild Info Insert GuildName : %s(%d) OccupyObelisk : %d ObeliskGroup : %d",
				pMsg.m_stABWinGuildInfoDS[iGuildCnt].szGuildName, pMsg.m_stABWinGuildInfoDS[iGuildCnt].dwGuild, pMsg.m_stABWinGuildInfoDS[iGuildCnt].wOccupyObelisk, pMsg.m_stABWinGuildInfoDS[iGuildCnt].wObeliskGroup);
      
			++iGuildCnt;
		}
	}

	if ( iGuildCnt > 0 )
	{
		pMsg.btGuildCnt = iGuildCnt;
		pMsg.wMapSvrNum =	g_MapServerManager.GetMapSvrGroup();
		wsDataCli.DataSend((char*)&pMsg,pMsg.h.size);
	}
}

void CArcaBattle::SendPlayResult()
{
	int iCnt = 0;
	_tagPMSG_AB_RESULT pMsg; 

	for ( int i = 0; i < 5; i++ )
	{
		if (this->m_stObeliskState[i].m_iGroupNumber != -1)
		{
			pMsg.m_stABObeliskOccupy[iCnt].wObeliskKind = this->m_stObeliskInfo[i].m_iAttrKind;
			memcpy(pMsg.m_stABObeliskOccupy[iCnt].szGuildName, this->m_stObeliskState[i].m_szOccupyGuildName, MAX_GUILD_LEN+1);

			g_Log.Add("[ArcaBattle] SendPlayResult Occupy Guild Name : [%s] GroupNumber [%d] ObeliskAttribute [%d]", pMsg.m_stABObeliskOccupy[iCnt].szGuildName, this->m_stObeliskState[i].m_iGroupNumber, pMsg.m_stABObeliskOccupy[iCnt].wObeliskKind);
			iCnt++;
		}
	}

	pMsg.h.c = 0xC1;
	pMsg.h.size = sizeof(pMsg);
	pMsg.h.headcode = 0xF8;
	pMsg.h.subcode = 0x3B;

	LPOBJ lpObj;
  
	for ( int n = g_ConfigRead.server.GetObjectStartUserIndex(); n < g_ConfigRead.server.GetObjectMax(); n++ )
	{
		lpObj = &gObj[n];

		if ( gObjIsConnected(n) == TRUE && lpObj->Type == OBJ_USER )
		{
			if ( !lpObj->m_PlayerData->lpGuild  )
			{
				g_Log.Add("[ArcaBattle] Error SendPlayResult() lpObj->lpGuild NULL %s %s", lpObj->AccountID, lpObj->Name);
				continue;
			}
			
			pMsg.btABResult = 0;
			
			for ( int i = 0; i < 5; ++i )
			{
				if (lpObj->m_PlayerData->lpGuild->Number == this->m_stObeliskState[i].m_iOccupyGuildNum)
				{
					pMsg.btABResult = 1;
				}
			}

			_tagArcaBattleUserInfo * pUserInfo = this->GetUserInfo(lpObj->Name);

			if ( !pUserInfo )
			{
				g_Log.Add("[ArcaBattle] Error SendPlayResult() pUserInfo NULL %s %s GuildName %s", lpObj->AccountID, lpObj->Name, lpObj->m_PlayerData->GuildName);
				continue;
			}
					
			UINT64 iRewardExp = 0;
			this->CalcRewardExp(lpObj, iRewardExp);

			if ( gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_PCS_MARK3) || gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_PCBANG_POINT_MARK3) )
			{
				iRewardExp = 0;
			}

			if (g_MasterLevelSkillTreeSystem.IsMasterLevelUser(&gObj[n]))
			{
				gObj[n].m_PlayerData->MasterExperience += iRewardExp;
			}

			else
			{
				gObj[n].m_PlayerData->Experience += iRewardExp;
			}
				
			if (gObjLevelUp(&gObj[n], iRewardExp, 0, "Arca Battle") == true)
			{
				GSProtocol.GCKillPlayerMasterExpSend(n, -1, iRewardExp, 0, 0);
			}

			g_GensSystem.AddContributePoint(lpObj, pUserInfo->m_stAcquiredPoints.dwContributePoints);

			pMsg.wBootyCnt = LOWORD(pUserInfo->m_stAcquiredPoints.dwBootyCnt);
			pMsg.dwContributePoint = pUserInfo->m_stAcquiredPoints.dwContributePoints;
			pMsg.dwKillPoint = pUserInfo->m_stAcquiredPoints.dwKillPoints;
			pMsg.dwRewardExp = iRewardExp;
				
			g_Log.Add( "[ArcaBattle] SendPlayResult [%s][%s] BootyCnt[%d] ContributePoint[%d] KillPoint[%d] RewardExp[%d]", gObj[n].AccountID, gObj[n].Name, pMsg.wBootyCnt, pMsg.dwContributePoint, pMsg.dwKillPoint, iRewardExp);
			IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		}
	}
}

void CArcaBattle::RegenMonsterRun()
{
	UINT64 i64Tick = GetTickCount64();

	for ( int i = 0; i < 50 && this->m_stMonsterGroupInfo[i].m_nGroupNumber > -1; ++i )
	{
		if (i == this->m_iSkipObelisk)
		{
			continue;
		}

		if (i64Tick >= this->m_stMonsterGroupInfo[i].m_i64NextRegenTime)
		{
			this->DeleteMonster(i);
			this->RegenMonster(i);
			this->m_stMonsterGroupInfo[i].m_i64NextRegenTime = GetTickCount64() + 60000 * this->m_stMonsterGroupInfo[i].m_nRegenTimeMin;
		}
	}
}

void CArcaBattle::DeleteMonster(int nGroupNumber)
{
	if ( nGroupNumber < 0 || nGroupNumber >= 50 )
	{
		return;
	}

	for (int i = 0; i < 255; i++ )
	{
		if (this->m_stMonsterGroupInfo[nGroupNumber].m_stMonsterAccountNumInfo[i].m_nIndex > -1)
		{
			int nIndex = this->m_stMonsterGroupInfo[nGroupNumber].m_stMonsterAccountNumInfo[i].m_nIndex;

			if ( gObj[nIndex].Live == TRUE )
			{
				gObjDel(nIndex);
				g_Log.Add("[ArcaBattle][MonsterGroupRegen] Delete Monster - %d ", this->m_stMonsterGroupInfo[nGroupNumber].m_stMonsterAccountNumInfo[i].m_nIndex);
				this->m_stMonsterGroupInfo[nGroupNumber].m_stMonsterAccountNumInfo[i].m_nIndex = -1;
			}
		}
	}
}

void CArcaBattle::RegenMonster(int nGroupNumber)
{
	if ( nGroupNumber < 0 || nGroupNumber >= 50 )
	{
		g_Log.AddC(TColor::Red,"[ArcaBattle] Invalid nGroupNumber %d",nGroupNumber);
		return;
	}

	int nRegenArea = rand() % this->m_stMonsterPosition[nGroupNumber].m_nRegenAreaCnt;
    int nMonsterArrayIndex = 0;

	for (int n = 0; n < this->m_stMonsterGroupInfo[nGroupNumber].m_nCnt && this->m_stMonsterGroupInfo[nGroupNumber].m_nMapNumber != -1; n++)
	{
		int nIndex = this->m_stMonsterGroupInfo[nGroupNumber].m_stMonsterAccountNumInfo[n].m_nIndex;

		if ( nIndex <= -1 || gObj[nIndex].Live == FALSE )
		{
			int nResult = gObjAddMonster(this->m_stMonsterGroupInfo[nGroupNumber].m_nMapNumber);
			int nMonsterIndex = this->m_stMonsterPosition[nGroupNumber].m_stMonsterIndexInfo[rand() % this->m_stMonsterPosition[nGroupNumber].m_nMonsterKindCnt].m_nMonsterIndex;
			this->m_stMonsterGroupInfo[nGroupNumber].m_stMonsterAccountNumInfo[n].m_nIndex = nResult;

			if ( nResult < 0 )
			{
				g_Log.Add( "[ArcaBattle][MonsterGroupRegen] AddMonster fail!! [GROUP]:%d, [INDEX]:%d", nGroupNumber, nMonsterIndex);
				return;
			}

			if (this->SetPosMonster(nResult, this->m_stMonsterGroupInfo[nGroupNumber].m_nMapNumber,
				this->m_stMonsterPosition[nGroupNumber].m_stRegenArea[nRegenArea].m_nBeginX, this->m_stMonsterPosition[nGroupNumber].m_stRegenArea[nRegenArea].m_nBeginY,
				this->m_stMonsterPosition[nGroupNumber].m_stRegenArea[nRegenArea].m_nEndX, this->m_stMonsterPosition[nGroupNumber].m_stRegenArea[nRegenArea].m_nEndY) == FALSE)
			{
				gObjDel(nResult);
				g_Log.Add("[ArcaBattle] error : %s %d", __FILE__, __LINE__);
				return;
			}

			if ( gObjSetMonster(nResult, nMonsterIndex) == FALSE )
			{
				gObjDel(nResult);
				g_Log.Add("[ArcaBattle] error : %s %d", __FILE__, __LINE__);
				return;
			}

			nMonsterArrayIndex++;
			g_Log.Add( "[ArcaBattle][MonsterGroupRegen] Group: %d, MonsterIndex: %d, MapNumber: %d, X: %d, Y: %d ",
				nGroupNumber, nMonsterIndex, this->m_stMonsterGroupInfo[nGroupNumber].m_nMapNumber, gObj[nResult].X, gObj[nResult].Y);
		}
	}
}

int CArcaBattle::SetPosMonster(int aIndex, int nMapNumber, int nBeginX, int nBeginY, int nEndX, int nEndY)
{
	if ( !ObjectMaxRange(aIndex) )
	{
		g_Log.Add("error : %s %d", __FILE__, __LINE__);
		return false;
	}

	LPOBJ lpObj = &gObj[aIndex];

	lpObj->m_PosNum = -1;
	lpObj->MapNumber = nMapNumber;
	this->GetBoxPosition(nMapNumber, nBeginX, nBeginY, nEndX, nEndY, lpObj->X, lpObj->Y);
	lpObj->TX = lpObj->X;
	lpObj->TY = lpObj->Y;
	lpObj->m_OldX = lpObj->X;
	lpObj->m_OldY = lpObj->Y;
	lpObj->Dir = rand()%8;
	lpObj->StartX = lpObj->X;
	lpObj->StartY = lpObj->Y;
	lpObj->DieRegen = 0;
	lpObj->MaxRegenTime = 0;

	return true;
}

int CArcaBattle::GetBoxPosition(int mapnumber, int ax, int ay, int aw, int ah, short &mx, short &my)
{
	if (MAX_MAP_RANGE(mapnumber) == FALSE)
	{
		return false;
	}

	int count = 100 ; 
	int w;
	int h;
	int tx; 
	int ty;
	BYTE attr;

	while ( count-- != 0)
	{
		w = aw - ax;
		h = ah - ay;

		__try
		{
			tx = ax + (rand()%w);
			ty = ay + (rand()%h);
		}
		__except (w=1,h=1,1)
		{

		}

		attr = MapC[mapnumber].GetAttr(tx, ty);

		if ( ((attr&1) != 1) && ((attr &4) != 4) && ((attr&8) != 8) )
		{
			mx = tx;
			my = ty;
			return true;
		}
	}

	return false;
}

void CArcaBattle::AddContributePoint(OBJECTSTRUCT * lpObj, OBJECTSTRUCT * lpTargetObj)
{
	if (lpObj->Type != OBJ_USER)
	{
		return;
	}

	char szName[MAX_ACCOUNT_LEN+1];
	memcpy(szName, lpObj->Name, MAX_ACCOUNT_LEN+1);

	_tagArcaBattleUserInfo * pUserInfo = this->GetUserInfo(szName);

	if (pUserInfo == NULL)
	{
		return;
	}

	if (lpTargetObj->Type == OBJ_USER && (lpTargetObj->Class == 592 || lpTargetObj->Class == 593 || lpTargetObj->Class == 594 || lpTargetObj->Class == 595 || lpTargetObj->Class == 596 || lpTargetObj->Class == 611 || lpTargetObj->Class == 612 || lpTargetObj->Class == 613 || lpTargetObj->Class == 614 || lpTargetObj->Class == 615))
	{
		pUserInfo->m_stAcquiredPoints.dwContributePoints += 30;
		g_Log.Add("[ArcaBattle] Boss Monster Kill [%s] ContributePoints %d",
			pUserInfo->szCharName, pUserInfo->m_stAcquiredPoints.dwContributePoints);

		return;
	}
	
	int iAddValue = 0;

	if (lpObj->Level == lpTargetObj->Level)
	{
		iAddValue = 5;
	}

	else
	{
		if (lpObj->Level >= lpTargetObj->Level)
		{
			if (lpObj->Level > lpTargetObj->Level)
			{
				int levelDif = lpObj->Level - lpTargetObj->Level;

				if (levelDif < 51)
				{
					if (levelDif < 31 || levelDif > 50)
					{
						if (levelDif < 11 || levelDif > 30)
						{
							if (levelDif > 0 && levelDif <= 10)
							{
								iAddValue = 5;
							}
						}

						else
						{
							iAddValue = 3;
						}
					}

					else
					{
						iAddValue = 2;
					}
				}
							
				else
				{
					iAddValue = 1;
				}
			}			
		}
					
		else
		{
			int iCalcLv = lpTargetObj->Level - lpObj->Level;

			if (iCalcLv < 51)
			{
				if (iCalcLv < 31 || iCalcLv > 50)
				{
					if (iCalcLv < 11 || iCalcLv > 30)
					{
						if (iCalcLv > 0)
						{
							if (iCalcLv <= 10)
							{
								iAddValue = 5;
							}
						}
					}

					else
					{
						iAddValue = 6;
					}
				}
							
				else
				{
					iAddValue = 6;
				}
			}
						
			else
			{
				iAddValue = 7;
			}
		}
	}

	pUserInfo->m_stAcquiredPoints.dwContributePoints += 3 * iAddValue;		
	g_Log.Add("[ArcaBattle] [%s][%s] User Kill ContributePoint[%d] - Result : Lv [%d] / Lv [%d][%s][%s]",
		lpObj->AccountID, lpObj->Name, pUserInfo->m_stAcquiredPoints.dwContributePoints, lpObj->Level, lpTargetObj->Level, lpTargetObj->AccountID, lpTargetObj->Name);
}

void CArcaBattle::AddKillPoint(OBJECTSTRUCT * lpObj, OBJECTSTRUCT * lpTargetObj)
{
	if (lpObj->Type != OBJ_USER)
	{
		return;
	}

	char szName[MAX_ACCOUNT_LEN + 1];
	memcpy(szName, lpObj->Name, MAX_ACCOUNT_LEN + 1);

	_tagArcaBattleUserInfo * pUserInfo = this->GetUserInfo(szName);

	if (!pUserInfo)
	{
		return;
	}

	if (lpTargetObj->Type == OBJ_USER)
	{
		pUserInfo->m_stAcquiredPoints.dwKillPoints += 10;

		g_Log.Add("[ArcaBattle] User Kill Add KillPoints [%s][%s] Point[%d]",
			lpObj->AccountID, lpObj->Name, pUserInfo->m_stAcquiredPoints.dwKillPoints);

		char szTargetName[MAX_ACCOUNT_LEN + 1];
		memcpy(szTargetName, lpTargetObj->Name, MAX_ACCOUNT_LEN + 1);

		_tagArcaBattleUserInfo * pTargetUserInfo = this->GetUserInfo(szTargetName);

		if (pTargetUserInfo && pTargetUserInfo->bBootyDrop == false)
		{
			pTargetUserInfo->bBootyDrop = true;
			ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, lpObj->X, lpObj->Y, ITEMGET(13, 147), 0, 1, 0, 0, 0, -1, 0, 0, 0, 0, 0);

			g_Log.Add("[ArcaBattle] [%s][%s] User Dropped ArcaBattle Item Booty [%s][%s]",
				lpObj->AccountID, lpObj->Name, lpTargetObj->AccountID, lpTargetObj->Name);
		}
	}

	else if (lpTargetObj->Type == OBJ_MONSTER)
	{
		if (lpTargetObj->Class == 598 || lpTargetObj->Class == 599 || lpTargetObj->Class == 600 || lpTargetObj->Class == 601 || lpTargetObj->Class == 602)
		{
			pUserInfo->m_stAcquiredPoints.dwKillPoints += 50;

			g_Log.Add("[ArcaBattle] Obelisk Occupy Add KillPoints [%s][%s] Point[%d]",
				lpObj->AccountID, lpObj->Name, pUserInfo->m_stAcquiredPoints.dwKillPoints);
		}

		else if (lpTargetObj->Class == 592 || lpTargetObj->Class == 593 ||
			lpTargetObj->Class == 594 || lpTargetObj->Class == 595 ||
			lpTargetObj->Class == 596 || lpTargetObj->Class == 611 ||
			lpTargetObj->Class == 612 || lpTargetObj->Class == 613 ||
			lpTargetObj->Class == 614 || lpTargetObj->Class == 615)
		{

			for (int i = 0; i < 50; ++i)
			{
				int nMapNumber = this->m_stMonsterGroupInfo[i].m_nMapNumber;

				if (nMapNumber < 0)
					break;

				for (int j = 0; j < 255; ++j)
				{
					if (lpTargetObj->m_Index == this->m_stMonsterGroupInfo[i].m_stMonsterAccountNumInfo[j].m_nIndex)
					{
						if (this->m_stMonsterGroupInfo[i].m_stMonsterAccountNumInfo[j].bItemDrop == false)
						{
							this->m_stMonsterGroupInfo[i].m_stMonsterAccountNumInfo[j].bItemDrop = true;
							ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, lpObj->X, lpObj->Y, ITEMGET(13,147), 0, 1, 0, 0, 0, -1, 0, 0, 0, 0, 0);

							g_Log.Add("[ArcaBattle] Monster Dropped ArcaBattle Item Booty To [%s][%s]",
								lpObj->AccountID, lpObj->Name);
						}
					}
				}
			}

			pUserInfo->m_stAcquiredPoints.dwKillPoints += 5;

			g_Log.Add("[ArcaBattle] Monster Kill Add KillPoints [%s][%s] Point[%d]",
				lpObj->AccountID, lpObj->Name, pUserInfo->m_stAcquiredPoints.dwKillPoints);
		}
	}
}

void CArcaBattle::BootyItemGetCnt(OBJECTSTRUCT * lpObj)
{
	if ( lpObj->Type != OBJ_USER )
	{
		return;
	}

	char szName[MAX_ACCOUNT_LEN+1];
	memcpy(szName, lpObj->Name, MAX_ACCOUNT_LEN+1);

	_tagArcaBattleUserInfo * pUserInfo = this->GetUserInfo(szName);

	if ( pUserInfo )
	{
		pUserInfo->m_stAcquiredPoints.dwBootyCnt++;
	
		g_Log.Add( "[ArcaBattle] BootyItemGetCnt [%s][%s] Total BootyCnt[%d]", lpObj->AccountID, lpObj->Name, pUserInfo->m_stAcquiredPoints.dwBootyCnt);
	}
}

void CArcaBattle::CalcRewardExp(OBJECTSTRUCT * lpObj, UINT64 &iRewardExp)
{
	if (lpObj->Type != OBJ_USER)
	{
		return;
	}

	char szName[MAX_ACCOUNT_LEN+1];
	memcpy(szName, lpObj->Name, MAX_ACCOUNT_LEN+1);

	_tagArcaBattleUserInfo *pUserInfo = this->GetUserInfo(szName);

	if ( !pUserInfo )
    {
		return;
	}
	
	int dwKillPoints = pUserInfo->m_stAcquiredPoints.dwKillPoints;
	
	if ( dwKillPoints > 10 )
	{
		if ( dwKillPoints < 11 || dwKillPoints > 50 )
		{
			if ( dwKillPoints < 51 || dwKillPoints > 100 )
			{
				if ( dwKillPoints < 101 || dwKillPoints > 300 )
				{
					if ( dwKillPoints < 301 || dwKillPoints > 500 )
					{
						if ( dwKillPoints >= 501 )
						{
							iRewardExp = 3000000;
						}
					}

					else
					{
						iRewardExp = 2000000;
					}
				}

				else
				{
					iRewardExp = 1000000;
				}
			}

			else
			{
				iRewardExp = 700000;
			}
		}

		else
		{
			iRewardExp = 500000;
		}
	}

	else
	{
		iRewardExp = 300000;
	}

	g_Log.Add( "[ArcaBattle] CalcRewardExp [%s][%s] Total KillPoints[%d] Exp[%I64d]",
		lpObj->AccountID, lpObj->Name, dwKillPoints, iRewardExp);
}

int CArcaBattle::DropItem(OBJECTSTRUCT *lpObj, OBJECTSTRUCT *lpMonsterObj)
{
	if (lpObj->MapNumber != MAP_INDEX_ACHERON)
	{
		return false;
	}

	if (gObjCheckUsedBuffEffect(lpObj, 187) == false)
	{
		return false;
	}

	if (g_BagManager.SearchAndUseBag(lpObj->m_Index, BAG_EVENT, EVENTBAG_ARCA, lpMonsterObj->m_Index) == false)
	{
		return false;
	}

	return true;
}

void CArcaBattle::GDReqDeleteArcaBattleInfo()
{
	_tagPMSG_REQ_ARCA_BATTLE_INFO_DS pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.size = sizeof(pMsg);
	pMsg.h.headcode = 0xF8;
	pMsg.h.subcode = 0x3D;
	wsDataCli.DataSend((char*)&pMsg, sizeof(pMsg));
}

void CArcaBattle::GDReqArcaBattleProcMultiCast(int iState)
{
	_tagPMSG_REQ_AB_PROC_INSERT_DS pMsg; 

	pMsg.h.c = 0xC1;
	pMsg.h.headcode = 0xF8;
	pMsg.h.subcode = 0x3E;
	pMsg.h.size = sizeof(pMsg);
	pMsg.btProcState = iState;
	pMsg.wMapSvrNum = g_MapServerManager.GetMapSvrGroup();
	wsDataCli.DataSend((char*)&pMsg, sizeof(pMsg));
}

void CArcaBattle::GDReqJoinMemberUnder()
{
	_tagPMSG_REQ_AB_JOIN_MEMBER_UNDER_DS pMsg; 

	pMsg.h.c = 0xC1;
	pMsg.h.headcode = 0xF8;
	pMsg.h.subcode = 0x41;
	pMsg.h.size = sizeof(pMsg);
	pMsg.wMapSvrNum = g_MapServerManager.GetMapSvrGroup();

	wsDataCli.DataSend((char*)&pMsg, sizeof(pMsg));
}

void CArcaBattle::GDReqArcaBattleJoinMemberUnderReq()
{
	_tagPMSG_REQ_AB_JOIN_CANCEL_DS pMsg; 

	pMsg.h.c = 0xC1;
	pMsg.h.headcode = 0xF8;
	pMsg.h.subcode = 0x43;
	pMsg.h.size = sizeof(pMsg);
	pMsg.btMinGuildMemNum = this->GetJoinMemberCnt();
	pMsg.wMapSvrNum = g_MapServerManager.GetMapSvrGroup();

	wsDataCli.DataSend((char*)&pMsg, sizeof(pMsg));
}

void CArcaBattle::DGAnsArcaBattleJoinMemberUnderReq(_tagPMSG_ANS_AB_JOIN_CANCEL_DS *lpMsg)
{

	_GUILD_INFO_STRUCT *lpGuild;
	int iGuildCnt;
	_tagPMSG_SEND_AB_JOIN_CANCEL pMsg;

	iGuildCnt = lpMsg->btGuildCnt;
	pMsg.h.c = 0xC1;
	pMsg.h.size = sizeof(pMsg);
	pMsg.h.headcode = 0xF8;
	pMsg.h.subcode = 0x40;

	for (int i = 0; i < iGuildCnt; i++)
	{
		lpGuild = Guild.SearchGuild(lpMsg->CancelGuildNames[i].szGuildNames);
		if (lpGuild)
		{
			for (int n = 0; n < 80; n++)
			{
				if (gObjIsConnected(lpGuild->Index[n]) == TRUE)
				{
					IOCP.DataSend(lpGuild->Index[n], &pMsg.h.c, sizeof(pMsg));
					g_Log.Add("[ArcaBattle] Canceled With Less Than Guild Member - GuildName[%s]", lpMsg->CancelGuildNames[i].szGuildNames);
				}
			}
		}
	}
}

void CArcaBattle::DGAnsRegisteredMemberCnt(_tagPMSG_ANS_AB_REG_MEMBER_CNT_DS *lpMsg)
{
	_tagPMSG_ANS_AB_REG_MEMBER_CNT pMsg; 
	BYTE nResult = 0; 

	if ( !lpMsg->btRegMemCnt )
		nResult = 2;

	pMsg.h.c = 0xC1;
	pMsg.h.size = sizeof(pMsg);
	pMsg.h.headcode = 0xF8;
	pMsg.h.subcode = 0x42;
	pMsg.btRegMemCnt = lpMsg->btRegMemCnt;
	pMsg.btResult = nResult;

	IOCP.DataSend(lpMsg->wNumber, (LPBYTE)&pMsg, sizeof(pMsg));
}

int CArcaBattle::GetGuildMasterPos(LPOBJ lpObj, int nGateNum, short & x, short & y)
{
	if (lpObj->Type != OBJ_USER)
	{
		return FALSE;
	}

	if (!lpObj->m_PlayerData->lpGuild)
	{
		return FALSE;
	}

	if (MAX_MAP_RANGE(lpObj->MapNumber) == FALSE)
	{
		return FALSE;
	}

	BYTE attr = MapC[lpObj->MapNumber].GetAttr(lpObj->X, lpObj->Y);

	if (lpObj->m_PlayerData->GuildStatus == GUILD_MASTER)
	{
		this->SetGuildMasterGateNum(nGateNum, lpObj->m_PlayerData->GuildNumber);
		return FALSE;
	}

	if (gObjCheckUsedBuffEffect(lpObj, 184) == TRUE)
	{
		return FALSE;
	}

	if ((attr & 1) != 1)
	{
		return FALSE;
	}

	int iMasterIndex = -1;

	for (int n = 0; n < lpObj->m_PlayerData->lpGuild->Count;n++)
	{
		if (lpObj->m_PlayerData->lpGuild->GuildStatus[n] == GUILD_MASTER)
		{
			if (gObjIsConnected(lpObj->m_PlayerData->lpGuild->Index[n]) == TRUE)
			{
				iMasterIndex = lpObj->m_PlayerData->lpGuild->Index[n];
				break;
			}
		}
	}

	if (iMasterIndex == -1)
	{
		return FALSE;
	}

	if (MAX_MAP_RANGE(gObj[iMasterIndex].MapNumber) == FALSE)
	{
		return FALSE;
	}

	attr = MapC[gObj[iMasterIndex].MapNumber].GetAttr(gObj[iMasterIndex].X, gObj[iMasterIndex].Y);

	if (attr & 1)
	{
		this->SetGuildMasterGateNum(0, lpObj->m_PlayerData->GuildNumber);
		return FALSE;
	}

	if (this->GetGuildMasterGateNum(lpObj->m_PlayerData->GuildNumber) == nGateNum)
	{
		this->GetBoxPosition(gObj[iMasterIndex].MapNumber, gObj[iMasterIndex].X, gObj[iMasterIndex].Y, gObj[iMasterIndex].X + 3, gObj[iMasterIndex].Y + 3, x, y);
		return TRUE;
	}

	return FALSE;
}

void CArcaBattle::SetGuildMasterGateNum(int nGateNum, int nGuildNum)
{
	int nBlank = -1;

	for (int i = 0; i < 10; i++)
	{
		if (this->m_stGMasterGateNum[i].nGuildNum == nGuildNum)
		{
			this->m_stGMasterGateNum[i].nGateNum = nGateNum;
			return;
		}

		if (nBlank < 0)
			nBlank = i;

		if (this->m_stGMasterGateNum[i].nGuildNum == -1)
		{
			this->m_stGMasterGateNum[i].nGuildNum = nGuildNum;
			this->m_stGMasterGateNum[i].nGateNum = nGateNum;
			return;
		}
	}
}

int CArcaBattle::GetGuildMasterGateNum(int nGuildNum)
{
	for (int i = 0; i < 10; i++)
	{
		if (this->m_stGMasterGateNum[i].nGuildNum == nGuildNum)
		{
			return this->m_stGMasterGateNum[i].nGateNum;
		}
	}

	return -1;
}

int CArcaBattle::GetUserPos(int nGateNum, short & nOutX, short & nOutY)
{
	int nGroupNumber;

	if (nGateNum == ZONE_ALKMAR)
	{
		nGroupNumber = 0;
	}

	else if (nGateNum == ZONE_UBAID)
	{
		nGroupNumber = 1;
	}

	else if (nGateNum == ZONE_DEBENTER)
	{
		nGroupNumber = 2;
	}

	else
	{
		return FALSE;
	}

	int nArea = rand() % this->m_stAB_UserPos[nGroupNumber].area_cnt;
	int nMapNumber = this->m_stAB_UserPos[nGroupNumber].stUserPos[nArea].map_number;

	if (nMapNumber != -1 && this->GetBoxPosition(nMapNumber,
		this->m_stAB_UserPos[nGroupNumber].stUserPos[nArea].beginX,
		this->m_stAB_UserPos[nGroupNumber].stUserPos[nArea].beginY,
		this->m_stAB_UserPos[nGroupNumber].stUserPos[nArea].endX,
		this->m_stAB_UserPos[nGroupNumber].stUserPos[nArea].endY,
		nOutX, nOutY))
	{
		return TRUE;
	}

	else
	{
		return FALSE;
	}
}

void CArcaBattle::SetUserMapInfo(OBJECTSTRUCT *lpObj, int nGateNum)
{
	for (int i = 0 ; i < this->m_iABJoinUserCnt; i++)
	{
		if (!strcmp(lpObj->Name, this->m_stABJoinUserInfo[i].szUserName))
		{
			this->m_stABJoinUserInfo[i].wGuildMasterGateNum = nGateNum;
		}
	}
}

void CArcaBattle::SetArcaBattleProcMultiCast(int bABInitState)
{
	this->m_bABInitState = bABInitState;
	this->GDReqArcaBattleProcMultiCast(this->GetState());
}

void CArcaBattle::GDReqArcaBattleIsTopRank(int iIndex, DWORD dwGuildNum)
{
	_tagPMSG_REQ_ARCA_BATTLE_IS_TOP_RANK pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.size = sizeof(pMsg);
	pMsg.h.headcode = 0xF8;
	pMsg.h.subcode = 0x54;
	pMsg.wNumber = iIndex;
	pMsg.dwGuildNum = dwGuildNum;

	wsDataCli.DataSend((char *)&pMsg, sizeof(pMsg));
}

bool CArcaBattle::CGReqMarkReg(int iIndex)
{
	int iArcaBattleState = this->GetState();

	if (iArcaBattleState >= ARCA_STATE_MASTER_REG && iArcaBattleState <= ARCA_STATE_CHANNEL_CLOSE)
	{
		this->GCAnsMarkRegErrCode(5, iIndex);
		return false;
	}

	LPOBJ lpObj = &gObj[iIndex];
	_GUILD_INFO_STRUCT * lpGuildInfo = lpObj->m_PlayerData->lpGuild;

	if (!lpGuildInfo)
	{
		g_Log.Add("[ArcaBattle] [%s][%s] error-L3 : ArcaBattle Mark Reg GuildInfo is NULL", gObj[iIndex].AccountID, gObj[iIndex].Name);
		this->GCAnsMarkRegErrCode(15, iIndex);

		return false;
	}

	if (lpObj->m_PlayerData->GuildStatus != G_MASTER && lpObj->m_PlayerData->GuildStatus != G_SUB_MASTER && lpObj->m_PlayerData->GuildStatus != G_BATTLE_MASTER)
	{
		this->GCAnsMarkRegErrCode(15, iIndex);
		return false;
	}

	if (lpGuildInfo->Count < this->GetJoinMemberCnt())
	{
		this->GCAnsMarkRegErrCode(16, iIndex);
		return false;
	}

	this->GDReqMarkCnt(iIndex, lpObj->m_PlayerData->GuildNumber);
	return true;
}

void CArcaBattle::GCAnsMarkReg(int iIndex, DWORD dwMarkCnt)
{
	_tagPMSG_ANS_MARK_REG pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.size = sizeof(pMsg);
	pMsg.h.headcode = 0xF8;
	pMsg.h.subcode = 0x44;
	pMsg.btMarkCnt1 = SET_NUMBERH(SET_NUMBERHW(dwMarkCnt));
	pMsg.btMarkCnt2 = SET_NUMBERL(SET_NUMBERHW(dwMarkCnt));
	pMsg.btMarkCnt3 = SET_NUMBERH(SET_NUMBERLW(dwMarkCnt));
	pMsg.btMarkCnt4 = SET_NUMBERL(SET_NUMBERLW(dwMarkCnt));

	IOCP.DataSend(iIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

void CArcaBattle::GCAnsMarkRegErrCode(int iResult, int iIndex)
{
	switch (iResult)
	{
		case 5:
			g_Log.Add("[ArcaBattle] [%s][%s] ArcaBattle MarkReg Fail to Time Over", gObj[iIndex].AccountID, gObj[iIndex].Name);
			break;
		case 15:
			g_Log.Add("[ArcaBattle] [%s][%s] ArcaBattle MarkReg Fail to No Registration Authority", gObj[iIndex].AccountID, gObj[iIndex].Name);
			break;
		case 16:
			g_Log.Add("[ArcaBattle] [%s][%s] ArcaBattle MarkReg Fail to Below Guild Member", gObj[iIndex].AccountID, gObj[iIndex].Name);
			break;
		case 17:
			g_Log.Add("[ArcaBattle] [%s][%s] ArcaBattle MarkReg Fail to Guild Is More than 250 Registered", gObj[iIndex].AccountID, gObj[iIndex].Name);
			break;
	}

	_tagPMSG_ANS_MARK_REG_ERROR_CODE pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.size = sizeof(pMsg);
	pMsg.h.headcode = 0xF8;
	pMsg.h.subcode = 0x47;
	pMsg.btResult = iResult;

	IOCP.DataSend(iIndex, (LPBYTE)&pMsg, sizeof(pMsg));
}

void CArcaBattle::CGReqMarkRegButtonClick(OBJECTSTRUCT *lpObj)
{
	int iValidItemCount = 0;
	int iInvalidItemCount = 0;

	PMSG_CHAOSMIXRESULT pMsg;
	PHeadSetB((LPBYTE)&pMsg, 0x86, sizeof(pMsg));
	pMsg.Result = CB_ERROR;

	int iArcaBattleState = this->GetState();

	if (iArcaBattleState >= ARCA_STATE_MASTER_REG && iArcaBattleState <= ARCA_STATE_CHANNEL_CLOSE)
	{
		GSProtocol.GCServerMsgStringSend(Lang.GetText(0,532), lpObj->m_Index, 1);
		pMsg.Result = CB_ARCA_MARK_REG_ERROR;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

		lpObj->ChaosLock = FALSE;
		return;
	}
	
	lpObj->ChaosLock = TRUE;
	int iItemPos = -1;
	int iItemCnt = 0;

	for (int i = 0; i < CHAOS_BOX_SIZE; i++)
	{
		if (lpObj->pChaosBox[i].IsItem() == TRUE)
		{
			if (lpObj->pChaosBox[i].m_Type == ITEMGET(14,21) && lpObj->pChaosBox[i].m_Level == 3)
			{
				iItemCnt += lpObj->pChaosBox[i].m_Durability;
				iValidItemCount++;
				iItemPos = i;
			}

			else
			{
				iInvalidItemCount++;
				iItemPos = i;
			}
		}
	}

	if (iValidItemCount < 1 || iInvalidItemCount != 0 || iItemPos == -1)
	{
		g_Log.Add("[ArcaBattle][Mark] - Can Not be Reg Mark [%s][%s] CharClass[%d] ItemNum[%d] ItemName[%s]",
			lpObj->AccountID, lpObj->Name, lpObj->Class, lpObj->pChaosBox[iItemPos].m_Type, ItemAttribute[lpObj->pChaosBox[iItemPos].m_Type].Name);

		pMsg.Result = CB_ARCA_MARK_REG_ERROR;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

		lpObj->ChaosLock = FALSE;
		return;
	}

	if (iItemCnt > 255)
	{
		g_Log.Add("[ArcaBattle][Mark] - Reg Mark Item Cnt Over [%s][%s] ItemCnt[%d]", lpObj->AccountID, lpObj->Name, iItemCnt);

		pMsg.Result = CB_ARCA_MARK_REG_ERROR;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

		lpObj->ChaosLock = FALSE;
		return;
	}

	g_MixSystem.ChaosBoxInit(lpObj);
	GSProtocol.GCUserChaosBoxSend(lpObj, 0);
	gObjInventoryCommit(lpObj->m_Index);

	pMsg.Result = CB_ARCA_MARK_REG_SUCCESS;
	IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

	lpObj->ChaosLock = FALSE;

	g_Log.Add("[ArcaBattle] [%s][%s] ReqMarkReg Cnt [%d] - [%s][%d]",
		lpObj->AccountID, lpObj->Name, iItemCnt, lpObj->m_PlayerData->GuildName, lpObj->m_PlayerData->GuildStatus);

	this->GDReqMarkReg(lpObj->m_Index, iItemCnt);
}

void CArcaBattle::CGReqMarkRank(int iIndex)
{
	this->GDReqMarkRank(iIndex, gObj[iIndex].m_PlayerData->GuildNumber);
}

void CArcaBattle::GCAnsMarkRank(int iIndex, BYTE btRank, DWORD dwMarkCnt, BYTE btTopRankCnt, _stArcaBattleMarkTopRankDS *pArcaBattleMarkTopRank)
{
	if (!ObjectMaxRange(iIndex))
	{
		return;
	}

	LPOBJ lpObj = &gObj[iIndex];

	if (!gObjIsConnected(iIndex))
	{
		return;
	}

	_tagPMSG_ANS_MARK_RANK pMsg;

	if (btTopRankCnt == 0)
	{
		pMsg.h.c = 0xC1;
		pMsg.h.size = sizeof(pMsg);
		pMsg.h.headcode = 0xF8;
		pMsg.h.subcode = 0x46;
		
		IOCP.DataSend(iIndex, (LPBYTE)&pMsg, sizeof(pMsg));
		return;
	}

	int lOfs = sizeof(_tagPMSG_ANS_MARK_RANK);
	BYTE sendbuf[255];
	memset(sendbuf, 0x00, sizeof(sendbuf));

	_stArcaBattleMarkTopRank ArcaBattleMarkTopRank[6];

	for (int i = 0; i < btTopRankCnt; i++)
	{
		ArcaBattleMarkTopRank[i].btRank = pArcaBattleMarkTopRank[i].btRank;
		memcpy(ArcaBattleMarkTopRank[i].szGuildNames, pArcaBattleMarkTopRank[i].szGuildNames, MAX_GUILD_LEN+1);
		ArcaBattleMarkTopRank[i].btMarkCnt1 = SET_NUMBERH(SET_NUMBERHW(pArcaBattleMarkTopRank[i].dwMarkCnt));
		ArcaBattleMarkTopRank[i].btMarkCnt2 = SET_NUMBERL(SET_NUMBERHW(pArcaBattleMarkTopRank[i].dwMarkCnt));
		ArcaBattleMarkTopRank[i].btMarkCnt3 = SET_NUMBERH(SET_NUMBERLW(pArcaBattleMarkTopRank[i].dwMarkCnt));
		ArcaBattleMarkTopRank[i].btMarkCnt4 = SET_NUMBERL(SET_NUMBERLW(pArcaBattleMarkTopRank[i].dwMarkCnt));
	}

	memcpy(&sendbuf[lOfs], &ArcaBattleMarkTopRank, sizeof(_stArcaBattleMarkTopRank) * btTopRankCnt);
	lOfs += sizeof(_stArcaBattleMarkTopRank) * btTopRankCnt;

	pMsg.h.c = 0xC1;
	pMsg.h.size = lOfs;
	pMsg.btRank = btRank;
	pMsg.btMarkCnt1 = SET_NUMBERH(SET_NUMBERHW(dwMarkCnt));
	pMsg.btMarkCnt2 = SET_NUMBERL(SET_NUMBERHW(dwMarkCnt));
	pMsg.btMarkCnt3 = SET_NUMBERH(SET_NUMBERLW(dwMarkCnt));
	pMsg.btMarkCnt4 = SET_NUMBERL(SET_NUMBERLW(dwMarkCnt));
	pMsg.h.headcode = 0xF8;
	pMsg.h.subcode = 0x46;
	pMsg.btGuildCnt = btTopRankCnt;

	memcpy(&sendbuf, &pMsg, sizeof(pMsg));
	IOCP.DataSend(iIndex, sendbuf, lOfs);
}

void CArcaBattle::GDReqMarkCnt(int iIndex, DWORD dwGuildNum)
{
	_tagPMSG_REQ_ARCA_BATTLE_MARK_CNT_DS pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.size = sizeof(pMsg);
	pMsg.wNumber = iIndex;
	pMsg.dwGuildNum = dwGuildNum;
	pMsg.h.headcode = 0xF8;
	pMsg.h.subcode = 0x4D;

	wsDataCli.DataSend((char *)&pMsg, sizeof(pMsg));
}

void CArcaBattle::DGAnsMarkCnt(_tagPMSG_ANS_ARCA_BATTLE_MARK_CNT_DS *lpMsg)
{
	int iIndex = lpMsg->wNumber;

	if (lpMsg->btResult == TRUE)
	{
		this->GCAnsMarkRegErrCode(17, iIndex);
		return;
	}

	if (!ObjectMaxRange(iIndex))
	{
		return;
	}

	LPOBJ lpObj = &gObj[iIndex];

	if (!gObjIsConnected(iIndex))
	{
		return;
	}

	if (bCanChaosBox == TRUE)
	{
		if (lpObj->m_bPShopOpen == true)
		{
			g_Log.Add("[%s][%s] is Already Opening PShop, ChaosBox Failed",
				lpObj->AccountID, lpObj->Name);
			GSProtocol.GCServerMsgStringSend(Lang.GetText(0,112), lpObj->m_Index, 1);

			return;
		}

		lpObj->m_IfState.type = 7;
		lpObj->m_IfState.state = 0;
		gObjInventoryTrans(lpObj->m_Index);
		g_Log.Add("[ArcaBattle][MarkReg] [%s][%s]  Mark Item Reg Start", lpObj->AccountID, lpObj->Name);
		gObjItemTextSave(lpObj);
		gObjWarehouseTextSave(lpObj);
	}

	this->GCAnsMarkReg(lpMsg->wNumber, lpMsg->dwMarkCnt);
}

void CArcaBattle::GDReqMarkReg(int iIndex, DWORD dwMarkCnt)
{
	_GUILD_INFO_STRUCT *lpGuild = gObj[iIndex].m_PlayerData->lpGuild;

	if (!lpGuild)
	{
		g_Log.Add("[ArcaBattle][MarkReg] [%s][%s]  GuildInfo NULL GDReqMarkReg()", gObj[iIndex].AccountID, gObj[iIndex].Name);
		return;
	}

	_tagPMSG_REQ_ARCA_BATTLE_MARK_REG_DS pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.size = sizeof(pMsg);
	pMsg.wNumber = iIndex;
	pMsg.dwGuildNum = gObj[iIndex].m_PlayerData->GuildNumber;
	pMsg.dwMarkCnt = dwMarkCnt;
	pMsg.h.headcode = 0xF8;
	pMsg.h.subcode = 0x4F;

	for (int n = 0; n < MAX_USER_GUILD; n++)
	{
		if (lpGuild->GuildStatus[n] == G_MASTER)
		{
			memcpy(pMsg.szGuildMaster, lpGuild->Names[n], MAX_ACCOUNT_LEN+1);
			break;
		}
	}

	memcpy(pMsg.szGuildName, gObj[iIndex].m_PlayerData->GuildName, MAX_GUILD_LEN+1);
	wsDataCli.DataSend((char *)&pMsg, pMsg.h.size);
}

void CArcaBattle::GDReqMarkRank(int iIndex, DWORD dwGuildNum)
{
	_tagPMSG_REQ_ARCA_BATTLE_MARK_RANK_DS pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.size = sizeof(pMsg);
	pMsg.wNumber = iIndex;
	pMsg.dwGuildNum = dwGuildNum;
	pMsg.h.headcode = 0xF8;
	pMsg.h.subcode = 0x51;

	wsDataCli.DataSend((char *)&pMsg, pMsg.h.size);
}

void CArcaBattle::DGAnsMarkRank(_tagPMSG_ANS_ARCA_BATTLE_MARK_RANK_DS *lpMsg)
{
	this->GCAnsMarkRank(lpMsg->wNumber, lpMsg->btRank, lpMsg->dwMarkCnt, lpMsg->btGuildCnt, lpMsg->ArcaBattleMarkTopRank);
}

void CArcaBattle::GDReqMarkRegDel(int iIndex, DWORD dwGuildNum)
{
	_tagPMSG_REQ_ARCA_BATTLE_MARK_REG_DEL_DS pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.size = sizeof(pMsg);
	pMsg.wNumber = iIndex;
	pMsg.dwGuildNum = dwGuildNum;
	pMsg.h.headcode = 0xF8;
	pMsg.h.subcode = 0x53;

	wsDataCli.DataSend((char *)&pMsg, pMsg.h.size);
}

void CArcaBattle::GDReqMarkRegAllDel()
{
	_tagPMSG_REQ_AB_MARK_REG_ALL_DEL_DS pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.size = sizeof(pMsg);
	pMsg.h.headcode = 0xF8;
	pMsg.h.subcode = 0x55;

	wsDataCli.DataSend((char *)&pMsg, pMsg.h.size);
}

void CArcaBattle::DGAnsMarkReg(_tagPMSG_ANS_ARCA_BATTLE_MARK_REG_DS *lpMsg)
{
	int iIndex = lpMsg->wNumber;

	if (!ObjectMaxRange(iIndex))
	{
		return;
	}

	g_Log.Add("[ArcaBattle][MarkReg] Error Mark Reg Ans [%s][%s][%s] ",
		gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].m_PlayerData->GuildName);
}

void CArcaBattle::GDReqAllGuildMarkCnt()
{
	_tagPMSG_REQ_ALL_GUILD_MARK_CNT_DS pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.size = sizeof(pMsg);
	pMsg.h.headcode = 0xF8;
	pMsg.h.subcode = 0xFB;

	wsDataCli.DataSend((char *)&pMsg, pMsg.h.size);
}

void CArcaBattle::DGAnsAllGuildMarkCnt(_tagPMSG_ANS_ALL_GUILD_MARK_CNT_DS *lpMsg)
{
	WORD wGuildCnt = lpMsg->wGuildCnt;

	if (wGuildCnt <= 250)
	{
		for (int i = 0; i < wGuildCnt; i++)
		{
			g_Log.Add("[ArcaBattle] Guild Name [%s] - ReqMarkReg [%d]", lpMsg->ABAllGuildMark[i].szGuildName, lpMsg->ABAllGuildMark[i].dwMarkCnt);
		}
	}
}

void CArcaBattle::SetRegenObelisk(int iGroupNum, int iAttribute)
{
	int iArrMonsterIndex[5];

	iArrMonsterIndex[0] = 598;
	iArrMonsterIndex[1] = 599;
	iArrMonsterIndex[2] = 600;
	iArrMonsterIndex[3] = 601;
	iArrMonsterIndex[4] = 602;

	iAttribute -= 1;
	iGroupNum -= 1;

	int iObeliskIndex = this->m_stObeliskState[iGroupNum].m_iMonIndex;

	if (!ObjectMaxRange(iObeliskIndex))
	{
		return;
	}

	gObjDel(iObeliskIndex);
	this->m_stObeliskInfo[iGroupNum].m_i64Tick = GetTickCount64();
	int result = gObjAddMonster(this->m_stObeliskInfo[iGroupNum].m_iMapIndex);

	if ( result >= 0)
	{
		gObjSetMonster(result, iArrMonsterIndex[iAttribute]);
		gObj[result].Class = iArrMonsterIndex[iAttribute];
		gObj[result].MapNumber = this->m_stObeliskInfo[iGroupNum].m_iMapIndex;
		gObj[result].X = this->m_stObeliskInfo[iGroupNum].m_iPosX;
		gObj[result].Y = this->m_stObeliskInfo[iGroupNum].m_iPosY;
		gObj[result].TX = this->m_stObeliskInfo[iGroupNum].m_iPosX;
		gObj[result].TY = this->m_stObeliskInfo[iGroupNum].m_iPosY;
		gObj[result].m_OldX = this->m_stObeliskInfo[iGroupNum].m_iPosX;
		gObj[result].m_OldY = this->m_stObeliskInfo[iGroupNum].m_iPosY;
		gObj[result].StartX = this->m_stObeliskInfo[iGroupNum].m_iPosX;
		gObj[result].StartY = this->m_stObeliskInfo[iGroupNum].m_iPosY;
		gObj[result].Dir = 1;
		gObj[result].Live = TRUE;
		gObj[result].DieRegen = 0;
		gObj[result].MaxRegenTime = 0;
		gObj[result].MaxLife = this->m_stObeliskInfo[iGroupNum].m_iOccupyHP;
		gObj[result].Life = this->m_stObeliskInfo[iGroupNum].m_iOccupyHP;
		this->m_stObeliskState[iGroupNum].m_iMonIndex = result;

		g_Log.Add("[ArcaBattle] (%d) Cheat Obelisk be Regen.", result);
	}
}

void CArcaBattle::SetCheatABStart()
{
	this->DeleteArcaBattleAllMonster();
	this->Clear();
	this->SetState(2);
	this->m_i64_AB_REMAIN_MSEC = 0;
	this->m_i64_AB_TICK_COUNT = GetTickCount64();
}

void CArcaBattle::SetCheatABEnd()
{
	this->DeleteArcaBattleAllMonster();
	this->Clear();
	this->SetState(1);
}

void CArcaBattle::SetCheatABNextProc()
{
	this->m_i64_AB_REMAIN_MSEC = 0;
}

void CArcaBattle::CheatGDReqArcaBattleGuildJoin(OBJECTSTRUCT *lpObj)
{
	GDReqArcaBattleGuildJoin(lpObj);
}

void CArcaBattle::CheatGDReqArcaBattleGuildMemberJoin(OBJECTSTRUCT *lpObj, const char *lpszName)
{
	OBJECTSTRUCT TempObj;
	strcpy(TempObj.Name, lpszName);

	GDReqArcaBattleGuildMemberJoin(&TempObj);
}

void CArcaBattle::CheatABOccupyObelisk(OBJECTSTRUCT *lpObj, int iObeliskGroup)
{
	if (lpObj->Type != OBJ_USER)
	{
		return;
	}

	this->m_stObeliskState[iObeliskGroup - 1].m_iObeliskState = 1;
	this->m_stObeliskState[iObeliskGroup - 1].m_iOccupyGuildNum = lpObj->m_PlayerData->GuildNumber;
	this->m_stObeliskState[iObeliskGroup - 1].m_iGroupNumber = this->m_stObeliskInfo[iObeliskGroup - 1].m_iGroupNumber;
	memcpy(this->m_stObeliskState[iObeliskGroup - 1].m_szOccupyGuildName, lpObj->m_PlayerData->GuildName, MAX_GUILD_LEN + 1);
}

void CArcaBattle::SetJoinMemberCnt(int iJoinGuildMemberCnt)
{
	this->m_iJoinGuildMemberCnt = iJoinGuildMemberCnt;
}

void CArcaBattle::CheatGDReqMarkReg(int iIndex, char *szGuildName, DWORD dwGuildNum, DWORD dwMarkCnt)
{
	_GUILD_INFO_STRUCT *lpGuild = gObj[iIndex].m_PlayerData->lpGuild;

	if (!lpGuild)
	{
		g_Log.Add("[ArcaBattle][MarkReg] [%s][%s]  GuildInfo NULL GDReqMarkReg()", gObj[iIndex].AccountID, gObj[iIndex].Name);
		return;
	}

	_tagPMSG_REQ_ARCA_BATTLE_MARK_REG_DS pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.size = sizeof(pMsg);
	pMsg.wNumber = iIndex;
	pMsg.dwGuildNum = dwGuildNum;
	pMsg.dwMarkCnt = dwMarkCnt;
	pMsg.h.headcode = 0xF8;
	pMsg.h.subcode = 0x4F;

	for (int n = 0; n < MAX_USER_GUILD; n++)
	{
		if (lpGuild->GuildStatus[n] == G_MASTER)
		{
			memcpy(pMsg.szGuildMaster, lpGuild->Names[n], MAX_ACCOUNT_LEN+1);
			break;
		}
	}

	strcpy(pMsg.szGuildName, szGuildName);
	pMsg.szGuildName[8] = 0;

	wsDataCli.DataSend((char *)&pMsg, pMsg.h.size);
}

void CArcaBattle::CheatGDReqMarkRegSet(int iIndex, DWORD dwMarkCnt)
{
	_GUILD_INFO_STRUCT *lpGuild = gObj[iIndex].m_PlayerData->lpGuild;

	if (!lpGuild)
	{
		g_Log.Add("[ArcaBattle][MarkReg] [%s][%s]  GuildInfo NULL GDReqMarkReg()", gObj[iIndex].AccountID, gObj[iIndex].Name);
		return;
	}

	_tagPMSG_REQ_AB_MARK_REG_UPDATE_DS pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.size = sizeof(pMsg);
	pMsg.dwGuildNum = gObj[iIndex].m_PlayerData->GuildNumber;
	pMsg.dwMarkCnt = dwMarkCnt;
	pMsg.h.headcode = 0xF8;
	pMsg.h.subcode = 0xFD;

	wsDataCli.DataSend((char *)&pMsg, pMsg.h.size);
}

void CArcaBattle::GuildMemberAssignStatus(OBJECTSTRUCT *lpObj, int iGuildStatus)
{
	int aIndex = lpObj->m_Index;

	if (iGuildStatus == 1)
	{
		iGuildStatus = G_SUB_MASTER;
	}

	else if (iGuildStatus == 2)
	{
		iGuildStatus = G_BATTLE_MASTER;
	}

	else
	{
		iGuildStatus = G_PERSON;
	}

	if (!gObjIsConnectedGP(aIndex))
	{
		g_Log.Add("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	}

	if (!ObjectMaxRange(aIndex))
	{
		return;
	}

	PMSG_GUILD_ASSIGN_STATUS_RESULT pMsg;
	pMsg.h.set((LPBYTE)&pMsg, 0xE1, sizeof(pMsg));

	pMsg.btType = 1;
	memcpy(pMsg.szTagetName, lpObj->Name, MAX_ACCOUNT_LEN);

	if (lpObj->m_PlayerData->GuildNumber > 0 && lpObj->m_PlayerData->lpGuild)
	{
		if (lpObj->m_PlayerData->GuildStatus == G_MASTER)
		{
			pMsg.btResult = 17;
			IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);

			return;
		}

		int iSubMasterCount = 0;
		int iBattleMasterCount = 0;

		for (int n = 0; n < MAX_USER_GUILD; n++)
		{
			if (lpObj->m_PlayerData->lpGuild->Use[n] > 0)
			{
				if (lpObj->m_PlayerData->lpGuild->GuildStatus[n] == G_SUB_MASTER)
				{
					iSubMasterCount++;
				}

				else if (lpObj->m_PlayerData->lpGuild->GuildStatus[n] == G_BATTLE_MASTER)
				{
					iBattleMasterCount++;
				}
			}
		}

		if (iGuildStatus == G_SUB_MASTER)
		{
			if (iSubMasterCount != 0)
			{
				pMsg.btResult = 18;
				IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);

				return;
			}

			GDGuildReqAssignStatus(aIndex, pMsg.btType, pMsg.szTagetName, iGuildStatus);
			return;
		}

		else if (iGuildStatus == G_BATTLE_MASTER)
		{
			if (iBattleMasterCount >= (lpObj->m_PlayerData->MasterLevel + lpObj->Level) / 200 + 1)
			{
				pMsg.btResult = 18;
				IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);

				return;
			}

			GDGuildReqAssignStatus(aIndex, pMsg.btType, pMsg.szTagetName, iGuildStatus);
			return;
		}

		else if (iGuildStatus == G_PERSON)
		{
			GDGuildReqAssignStatus(aIndex, pMsg.btType, pMsg.szTagetName, iGuildStatus);
			return;
		}
	}

	else
	{
		pMsg.btResult = 16;
		IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);

		return;
	}
}

void CArcaBattle::GDReqGuildRegInit()
{
	_tagPMSG_REQ_GUILD_REG_INIT pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.size = sizeof(pMsg);
	pMsg.h.headcode = 0xF8;
	pMsg.h.subcode = 0xFE;

	wsDataCli.DataSend((char *)&pMsg, pMsg.h.size);
}

void CArcaBattle::PrintGuildMemberLog(_GUILD_INFO_STRUCT *lpGuild)
{
	if ( !lpGuild )
	{
		g_Log.Add("[ArcaBattle] Error : lpGuild Is NULL - PrintGuildMemberLog()");
		return;
	}

	int nBlank = -1;

	for (int i = 0; i < 10; ++i )
	{
		if ( this->m_stGuildMemberInfoLog[i].nGuildNum < 0 )
		{
			if ( nBlank < 0 )
			{
				nBlank = i;
			}
		}

		else
		{
			if ( this->m_stGuildMemberInfoLog[i].nGuildNum == lpGuild->Number )
			{
				return;
			}
		}
	}

	this->m_stGuildMemberInfoLog[nBlank].nGuildNum = lpGuild->Number;
	g_Log.Add("************************************************************************");
	g_Log.Add("[ArcaBattle] Guild Name [%s] GuildMemberCnt [%d]", lpGuild->Name, lpGuild->Count);

	for ( int n = 0; n < 80; ++n )
	{
		if ( lpGuild->Use[n] )
			g_Log.Add("[ArcaBattle] [%s] Guild Member (%d)[%s]", lpGuild->Name, n + 1, lpGuild->Names[n]);
	}

	g_Log.Add("************************************************************************");

}

void CArcaBattle::SetDataLoadProcState(int bReqABProcState)
{
	this->m_bReqABProcState = bReqABProcState;
}

void CArcaBattle::GDReqArcaBattleAllJoinUser()
{
	for (int i = 0; i < 200; i++)
	{
		this->m_stABJoinUserInfo[i].Clear();
	}

	_tagPMSG_REQ_AB_ALL_JOIN_USER_DS pMsg;
	PHeadSetB((LPBYTE)&pMsg, 0xE5, sizeof(pMsg));

	wsDataCli.DataSend((char *)&pMsg, sizeof(pMsg));
}

void CArcaBattle::DGAnsArcaBattleAllJoinUser(_tagPMSG_ANS_AB_ALL_JOIN_USER_DS *lpMsg)
{
	int iUserCnt = lpMsg->btUserCnt;
	this->m_iABJoinUserCnt = iUserCnt;

	for (int i = 0; i < iUserCnt; i++)
	{
		memcpy(this->m_stABJoinUserInfo[i].szGuildName, lpMsg->stABJoinUserInfo[i].szGuildName, MAX_GUILD_LEN+1);
		this->m_stABJoinUserInfo[i].dwGuild = lpMsg->stABJoinUserInfo[i].dwGuild;
		memcpy(this->m_stABJoinUserInfo[i].szUserName, lpMsg->stABJoinUserInfo[i].szUserName, MAX_ACCOUNT_LEN+1);
	}
}

void CArcaBattle::BootyExchange(OBJECTSTRUCT *lpObj)
{
	int iValidItemCount = 0;
	int iInvalidItemCount = 0;
	int iItemPos = -1;
	int iItemCnt = 0;
	CItem NewItem;

	PMSG_CHAOSMIXRESULT pMsg;
	PHeadSetB((LPBYTE)&pMsg, 0x86, sizeof(pMsg));
	pMsg.Result = CB_ERROR;

	lpObj->ChaosLock = TRUE;

	if (CheckInventoryEmptySpace(lpObj, 4, 4) == FALSE)
	{
		g_Log.Add("[ArcaBattle][BootyExchange] - Fail - Not Empty Inventory [%s][%s] CharClass[%d]", lpObj->AccountID, lpObj->Name, lpObj->Class);
		pMsg.Result = CB_NOT_ENOUGH_EMPTY_SPACE;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

		lpObj->ChaosLock = FALSE;
		return;
	}

	for (int i = 0; i < CHAOS_BOX_SIZE; i++)
	{
		if (lpObj->pChaosBox[i].IsItem() == TRUE)
		{
			if (lpObj->pChaosBox[i].m_Type == ITEMGET(13, 147))
			{
				iItemCnt += lpObj->pChaosBox[i].m_Durability;
				iValidItemCount++;
				iItemPos = i;
			}

			else
			{
				iInvalidItemCount++;
				iItemPos = i;
			}
		}
	}

	if (iValidItemCount < 1 || iInvalidItemCount != 0 || iItemPos == -1)
	{
		g_Log.Add("[ArcaBattle][BootyExchange] - Can Not be Exchanged [%s][%s] CharClass[%d] ItemNum[%d] ItemName[%s]",
			lpObj->AccountID, lpObj->Name, lpObj->Class, lpObj->pChaosBox[iItemPos].m_Type, ItemAttribute[lpObj->pChaosBox[iItemPos].m_Type].Name);
		pMsg.Result = CB_ERROR;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

		lpObj->ChaosLock = FALSE;
		return;
	}

	if (iItemCnt > 100)
	{
		pMsg.Result = CB_ERROR;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

		lpObj->ChaosLock = FALSE;
		return;
	}

	g_MixSystem.LogChaosItem(lpObj, "ArcaBattle Booty Item Exchange");
	g_Log.Add("[ArcaBattle][BootyExchange] - Exchange Start");
	g_Log.Add("[ArcaBattle][BootyExchange] Booty Item Cnt [%s][%s][%d]", lpObj->AccountID, lpObj->Name, iItemCnt);

	_tagARCA_BATTLE_BOOTY_REWARD stRewardItemInfo;
	BOOL bReward = g_ArcaBattle.GetBootyRewardItem(iItemCnt, &stRewardItemInfo);

	if (bReward == FALSE)
	{
		pMsg.Result = CB_ERROR;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

		lpObj->ChaosLock = FALSE;
		return;
	}

	if (bReward == 2)
	{
		g_MixSystem.ChaosBoxInit(lpObj);
		GSProtocol.GCUserChaosBoxSend(lpObj, 0);

		g_Log.Add("[ArcaBattle][Booty] Mix Fail [%s][%s] ", lpObj->AccountID, lpObj->Name);
		GSProtocol.GCServerMsgStringSend(Lang.GetText(0,333), lpObj->m_Index, 1);

		pMsg.Result = CB_ERROR;
		lpObj->ChaosLock = FALSE;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		return;
	}

	int itype = ITEMGET(stRewardItemInfo.iItemType, stRewardItemInfo.iItemIndex);
	ItemSerialCreateSend(lpObj->m_Index, -1, 0, 0, itype, stRewardItemInfo.iItemLevel, stRewardItemInfo.iDurab, 0, 0, 0, lpObj->m_Index, 0, 0, 0, 0, 0);

	g_Log.Add("[ArcaBattle][BootyExchange] - Exchange End [%s][%s] ItemName[%s] ItemNum[%d] Level[%d] Dur[%d] skill[%d] luck[%d] option[%d] ExOpt[%d] SetOption[%d] BootyCnt[%d]",
		lpObj->AccountID, lpObj->Name, ItemAttribute[itype].Name, itype, stRewardItemInfo.iItemLevel, stRewardItemInfo.iDurab, 0, 0, 0, 0, 0, iItemCnt);

	gObjInventoryCommit(lpObj->m_Index);
	lpObj->ChaosLock = FALSE;
}

void CArcaBattle::GCArcaBattleUserInfo(int iUserIndex)
{
	if (ObjectMaxRange(iUserIndex) == false)
	{
		return;
	}

	BYTE BUFFER[8192];
	LPOBJ lpObj = &gObj[iUserIndex];

	_stPMSG_AB_JOIN_USER_COUNT pMsg;
	_stABCurJoinGuildUserInfo stABCurJoinGuildUser;
	_GUILD_INFO_STRUCT * lpGuild;

	int lOfs = sizeof(pMsg);
	int count = 0;

	for (int i = 0; i < this->m_iABJoinUserCnt; i++)
	{
		if (!strcmp(lpObj->Name, this->m_stABJoinUserInfo[i].szUserName))
		{
			lpGuild = Guild.SearchGuild(this->m_stABJoinUserInfo[i].szGuildName);

			if (!lpGuild)
			{
				g_Log.Add("[ArcaBattle] Error Can Not Find Guild GCArcaBattleUserInfo() lpGuild NULL GuildName : %s", this->m_stABJoinUserInfo[i].szGuildName);
				return;
			}

			pMsg.iGuildNumber = lpGuild->Number;

			for (int j = 0; j < lpGuild->Count; j++)
			{
				int iGuildMembIndex = lpGuild->Index[j];

				LPOBJ lpGuildObj = &gObj[iGuildMembIndex];

				if (lpGuild->GuildStatus[j] == GUILD_MASTER)
				{
					if (ObjectMaxRange(iGuildMembIndex) == true && gObjIsConnected(lpGuildObj) == TRUE && MAX_MAP_RANGE(lpGuildObj->MapNumber) == TRUE)
					{
						BYTE btAttr = MapC[lpGuildObj->MapNumber].GetAttr(lpGuildObj->X, lpGuildObj->Y);

						if (lpGuildObj->MapNumber == MAP_INDEX_ARCA_WAR || lpGuildObj->MapNumber == MAP_INDEX_DEBENTER_ARCA_WAR)
						{
							if ((btAttr & 1) == 1)
							{
								memcpy(pMsg.szGuildMasterName, lpGuildObj->Name, MAX_ACCOUNT_LEN + 1);
								pMsg.dwGuildMasterGateNum = 0;
								pMsg.btGuildMasterPosX = lpGuildObj->X;
								pMsg.btGuildMasterPosY = lpGuildObj->Y;
							}

							else
							{
								memcpy(pMsg.szGuildMasterName, lpGuildObj->Name, MAX_ACCOUNT_LEN + 1);
								pMsg.dwGuildMasterGateNum = this->GetGuildMasterGateNum(this->m_stABJoinUserInfo[i].dwGuild);
								pMsg.btGuildMasterPosX = lpGuildObj->X;
								pMsg.btGuildMasterPosY = lpGuildObj->Y;
							}
						}
					}

					else
					{
						memcpy(pMsg.szGuildMasterName, lpGuild->Names[j], MAX_ACCOUNT_LEN + 1);
						pMsg.dwGuildMasterGateNum = 0;
						pMsg.btGuildMasterPosX = 0;
						pMsg.btGuildMasterPosY = 0;
					}
				}

				else
				{
					for (int k = 0; k < this->m_iABJoinUserCnt; k++)
					{
						if (strcmp("", this->m_stABJoinUserInfo[k].szUserName))
						{
							for (int l = 0; l < lpGuild->Count; l++)
							{
								int iIndex = lpGuild->Index[l];

								if (iIndex == -1)
								{
									if (!strcmp(lpGuild->Names[l], this->m_stABJoinUserInfo[k].szUserName))
									{
										memcpy(stABCurJoinGuildUser.szUserName, this->m_stABJoinUserInfo[k].szUserName, MAX_ACCOUNT_LEN + 1);
										stABCurJoinGuildUser.btStatus = lpGuild->GuildStatus[l];
										stABCurJoinGuildUser.btPosX = 0;
										stABCurJoinGuildUser.btPosY = 0;
										stABCurJoinGuildUser.wGuildMasterGateNum = 0;
										memcpy(&BUFFER[lOfs], &stABCurJoinGuildUser, sizeof(stABCurJoinGuildUser));
										lOfs += sizeof(stABCurJoinGuildUser);
										count++;
										break;
									}
								}

								else
								{
									if ((gObj[iIndex].MapNumber == MAP_INDEX_ARCA_WAR || gObj[iIndex].MapNumber == MAP_INDEX_DEBENTER_ARCA_WAR) &&
										!strcmp(gObj[iIndex].Name, this->m_stABJoinUserInfo[k].szUserName) &&
										gObjIsConnected(iIndex) == TRUE)
									{
										memcpy(stABCurJoinGuildUser.szUserName, gObj[iIndex].Name, MAX_ACCOUNT_LEN + 1);
										stABCurJoinGuildUser.btStatus = gObj[iIndex].m_PlayerData->GuildStatus;
										stABCurJoinGuildUser.btPosX = gObj[iIndex].X;
										stABCurJoinGuildUser.btPosY = gObj[iIndex].Y;

										BYTE btAttr = MapC[gObj[iIndex].MapNumber].GetAttr(gObj[iIndex].X, gObj[iIndex].Y);

										if ((btAttr & 1) == 1)
										{
											stABCurJoinGuildUser.wGuildMasterGateNum = 0;
										}

										else
										{
											stABCurJoinGuildUser.wGuildMasterGateNum = this->m_stABJoinUserInfo[k].wGuildMasterGateNum;
										}

										memcpy(&BUFFER[lOfs], &stABCurJoinGuildUser, sizeof(stABCurJoinGuildUser));
										lOfs += sizeof(stABCurJoinGuildUser);
										count++;
										break;
									}
								}
							}
						}
					}
					goto EndFunc;
				}
			}
		}
	}

EndFunc:
	pMsg.Result = count > 0;
	pMsg.btUserCnt = count;
	PHeadSubSetW((LPBYTE)&pMsg, 0xF8, 0x48, lOfs);
	memcpy(&BUFFER, &pMsg, sizeof(pMsg));

	IOCP.DataSend(iUserIndex, (LPBYTE)&BUFFER, lOfs);
}

void CArcaBattle::GCArcaBattleCurrentStatus(int iObeliskIndex, int iAttrKind, BYTE btObeliskState, int iGuildNum, char *szGuildName, int iUserIndex)
{
	_tagPMSG_ARCA_BATTLE_CURRENT_STATUS pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.size = sizeof(pMsg);
	pMsg.h.headcode = 0xF8;
	pMsg.h.subcode = 0x49;
	pMsg.iObelGroupNum = iObeliskIndex;
	pMsg.iAttrKind = iAttrKind;
	pMsg.btCurStatus = btObeliskState;
	pMsg.dwGuild = iGuildNum;

	if (szGuildName)
	{
		memcpy(pMsg.szGuildName, szGuildName, MAX_GUILD_LEN + 1);
	}

	if (iUserIndex < g_ConfigRead.server.GetObjectStartUserIndex())
	{
		for (int i = g_ConfigRead.server.GetObjectStartUserIndex(); i < g_ConfigRead.server.GetObjectMax(); i++)
		{
			LPOBJ lpObj = &gObj[i];

			if (gObjIsConnected(lpObj) && lpObj->Type == OBJ_USER)
			{
				IOCP.DataSend(i, (LPBYTE)&pMsg, pMsg.h.size);
			}
		}
	}

	else
	{
		IOCP.DataSend(iUserIndex, (LPBYTE)&pMsg, pMsg.h.size);
	}
}

void CArcaBattle::GCArcaBattleSendNotiMsg(BYTE btNoticeType, int iNoticeValue, char* szGuildName)
{
	_tagPMSG_ARCA_BATTLE_NOTI_MSG pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.size = sizeof(pMsg);
	pMsg.h.headcode = 0xF8;
	pMsg.h.subcode = 0x4A;
	pMsg.iNotiIndex = btNoticeType;
	pMsg.iValue = iNoticeValue;

	if (!szGuildName)
	{
		for (int i = g_ConfigRead.server.GetObjectStartUserIndex(); i < g_ConfigRead.server.GetObjectMax(); i++)
		{
			LPOBJ lpObj = &gObj[i];

			if (gObjIsConnected(lpObj) && lpObj->Type == OBJ_USER)
			{
				IOCP.DataSend(i, (LPBYTE)&pMsg, pMsg.h.size);
			}
		}
	}

	else
	{
		_GUILD_INFO_STRUCT * lpGuild = Guild.SearchGuild(szGuildName);

		if (!lpGuild)
		{
			g_Log.Add("[ArcaBattle] Error Can Not Find Guild GCArcaBattleSendNotiMsg() lpGuild NULL GuildName : %s", szGuildName);
			return;
		}

		for (int i = 0; i < lpGuild->Count; i++)
		{
			int iGuildUserIndex = lpGuild->Index[i];

			if (ObjectMaxRange(iGuildUserIndex) == false)
			{
				continue;
			}

			LPOBJ lpObj = &gObj[iGuildUserIndex];

			if (gObjIsConnected(lpObj) && lpObj->Type == OBJ_USER)
			{
				IOCP.DataSend(iGuildUserIndex, (LPBYTE)&pMsg, pMsg.h.size);
			}
		}
	}
}

void CArcaBattle::GCArcaBattleCurrentStatusForReConnectUser(int iUserIndex)
{
	for (int i = 0; i < this->m_iObeliskCnt; i++)
	{
		if (i == this->m_iSkipObelisk)
		{
			continue;
		}

		if (this->m_stObeliskState[i].m_iObeliskState != 0)
		{
			if (this->m_stObeliskState[i].m_iAuraState == 2)
			{
				_GUILD_INFO_STRUCT * lpGuild = Guild.SearchGuild_Number(this->m_stObeliskState[i].m_iAuraReleaseGuildNum);

				if (!lpGuild)
				{
					g_Log.Add("[ArcaBattle] Error Can Not Find Guild GCArcaBattleCurrentStatusForReConnectUser()");
					return;
				}

				if (this->m_stObeliskState[i].m_iAuraReleaseGuildNum == this->m_stObeliskState[i].m_iOccupyGuildNum)
				{
					this->GCArcaBattleCurrentStatus(this->m_stObeliskInfo[i].m_iGroupNumber, this->m_stObeliskInfo[i].m_iAttrKind, ARCA_BATTLE_OBELISK_CAPTURED, this->m_stObeliskState[i].m_iAuraReleaseGuildNum, lpGuild->Name, iUserIndex);
				}

				else
				{
					this->GCArcaBattleCurrentStatus(this->m_stObeliskInfo[i].m_iGroupNumber, this->m_stObeliskInfo[i].m_iAttrKind, ARCA_BATTLE_OBELISK_CAPTURING, this->m_stObeliskState[i].m_iAuraReleaseGuildNum, lpGuild->Name, iUserIndex);
				}
			}

			else
			{
				_GUILD_INFO_STRUCT * lpGuild = Guild.SearchGuild_Number(this->m_stObeliskState[i].m_iOccupyGuildNum);
				this->GCArcaBattleCurrentStatus(this->m_stObeliskInfo[i].m_iGroupNumber, this->m_stObeliskInfo[i].m_iAttrKind, ARCA_BATTLE_OBELISK_CAPTURED, this->m_stObeliskState[i].m_iOccupyGuildNum, lpGuild->Name, iUserIndex);
			}
		}

		else
		{
			if (this->m_stObeliskState[i].m_iAuraState == 2)
			{
				_GUILD_INFO_STRUCT * lpGuild = Guild.SearchGuild_Number(this->m_stObeliskState[i].m_iAuraReleaseGuildNum);

				if (!lpGuild)
				{
					g_Log.Add("[ArcaBattle] Error Can Not Find Guild GCArcaBattleCurrentStatusForReConnectUser()");
					return;
				}

				this->GCArcaBattleCurrentStatus(this->m_stObeliskInfo[i].m_iGroupNumber, this->m_stObeliskInfo[i].m_iAttrKind, ARCA_BATTLE_OBELISK_CAPTURING, this->m_stObeliskState[i].m_iAuraReleaseGuildNum, lpGuild->Name, iUserIndex);
			}
		}
	}
}

bool CArcaBattle::LoadScriptArcaBattle(char *lpszFileName)
{
	if ( lpszFileName == NULL )
	{
		g_Log.MsgBox("[ArcaBattle] file load error - File Name Error");
		return false;
	}

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(lpszFileName);

	if ( res.status != pugi::status_ok )
	{
		g_Log.MsgBox("[ArcaBattle] file load error - load_file() : %s (%s)", lpszFileName, res.description());
		return false;
	}

	pugi::xml_node main = file.child("ArcaBattleEvent");

	this->m_bDoEvent = g_ConfigRead.server.GetStateFromEventTable(g_ConfigRead.server.GetServerType(), EV_TABLE_ARCA) == true ? main.attribute("Enable").as_int() : false;
	this->m_iJoinGuildMemberCnt = main.attribute("MinGuildMember").as_int();
	
	this->m_iAB_MasterRegMin = main.attribute("MasterRegTime").as_int();
	this->m_iAB_RegTimeMin = main.attribute("MemberRegTime").as_int();
	this->m_iAB_WaitTimeMin = main.attribute("ProgressWait").as_int();
	this->m_iAB_WaitPartyTimeMin = main.attribute("PartyWait").as_int();
	this->m_iAB_PlayTimeMin = main.attribute("Duration").as_int();
	this->m_iAB_ChannelTimeMin = main.attribute("ChannelClose").as_int();

	pugi::xml_node activeday = main.child("ActiveDay");

	int iDay = 0;
	for (pugi::xml_attribute day = activeday.first_attribute(); day; day = day.next_attribute())
	{
		this->m_iAB_DayOfWeekCycle[iDay] = day.as_int();
		iDay++;
	}

	pugi::xml_node schedule = main.child("Schedule");
	this->m_vtArcaBattleOpenTime.clear();

	for (pugi::xml_node start = schedule.child("Start"); start; start = start.next_sibling())
	{
		_tagARCA_BATTLE_START_TIME pRET;

		pRET.m_iHour = start.attribute("Hour").as_int();
		pRET.m_iMinute = start.attribute("Minute").as_int();
		pRET.m_btEvent = start.attribute("Unk").as_int();

		this->m_vtArcaBattleOpenTime.push_back(pRET);
	}

	pugi::xml_node obeliskinfo = main.child("ObeliskInformation");
	this->m_iObeliskCnt = 0;

	for(pugi::xml_node obelisk = obeliskinfo.child("Obelisk"); obelisk; obelisk = obelisk.next_sibling())
	{
		int GroupNumber = obelisk.attribute("Index").as_int() - 1;

		if ( GroupNumber < 0 || GroupNumber > 5 )
		{
			g_Log.Add("[ArcaBattle][MonsterGroupRegen] Invalid Obelisk groupNumber");
			return false;
		}

		this->m_stObeliskInfo[GroupNumber].m_iGroupNumber = GroupNumber + 1;
		this->m_stObeliskInfo[GroupNumber].m_iAttrKind = 1;
		this->m_stObeliskInfo[GroupNumber].m_iMapIndex = obelisk.attribute("MapNumber").as_int();
		this->m_stObeliskInfo[GroupNumber].m_iMonsterNum = obelisk.attribute("Type").as_int();
		this->m_stObeliskInfo[GroupNumber].m_iPosX = obelisk.attribute("PosX").as_int();
		this->m_stObeliskInfo[GroupNumber].m_iPosY = obelisk.attribute("PosY").as_int();
		this->m_stObeliskInfo[GroupNumber].m_iHP = obelisk.attribute("HP").as_int();
		this->m_stObeliskInfo[GroupNumber].m_iOccupyHP = obelisk.attribute("OccupyHP").as_int();
		this->m_stObeliskInfo[GroupNumber].m_i64RegenTime = obelisk.attribute("RegenTime").as_int();
		this->m_iObeliskCnt++;
	}
					
	this->SetRandomObeliskAttr();
					
	int AuraCnt = 0;
	int TempGroupNumber = 0;
	pugi::xml_node aurapos = main.child("AuraPositionSettings");

	for (pugi::xml_node aura = aurapos.child("Aura"); aura; aura = aura.next_sibling())
	{
		int GroupNumber = aura.attribute("Index").as_int() - 1;

		if ( TempGroupNumber != GroupNumber )
		{
			TempGroupNumber = GroupNumber;
			AuraCnt = 0;
		}

		if ( GroupNumber < 0 || GroupNumber > 5 )
		{
			g_Log.Add("[ArcaBattle][MonsterGroupRegen] Invalid Aura groupNumber");
			return false;
		}

		this->m_stObeliskInfo[GroupNumber].m_stAuraInfo[AuraCnt].m_iGroupNumber = GroupNumber + 1;
		int iMapNumber = aura.attribute("MapNumber").as_int();
						
		if ( this->m_stObeliskInfo[GroupNumber].m_iMapIndex != iMapNumber )
		{
			g_Log.MsgBox("[ArcaBattle] file load error - Not Match MapIndex");
		}

		this->m_stObeliskInfo[GroupNumber].m_stAuraInfo[AuraCnt].m_iMapIndex = iMapNumber;
		this->m_stObeliskInfo[GroupNumber].m_stAuraInfo[AuraCnt].m_iAuraPosX = aura.attribute("PosX").as_int();
		this->m_stObeliskInfo[GroupNumber].m_stAuraInfo[AuraCnt].m_iAuraPosY = aura.attribute("PosY").as_int();
		AuraCnt++;
	}

	pugi::xml_node monstergroupinfo = main.child("MonsterGroupInformation");

	for (pugi::xml_node monster = monstergroupinfo.child("Monster"); monster; monster = monster.next_sibling())
	{
		int GroupNumber = monster.attribute("Group").as_int();

		if ( GroupNumber < 0 || GroupNumber > 50 )
		{
			g_Log.Add("[ArcaBattle][MonsterGroupRegen] Invalid Monster groupNumber");
			return false;
		}

		this->m_stMonsterGroupInfo[GroupNumber].m_nGroupNumber = GroupNumber;
		this->m_stMonsterGroupInfo[GroupNumber].m_nMapNumber = monster.attribute("MapNumber").as_int();
		this->m_stMonsterGroupInfo[GroupNumber].m_nCnt = monster.attribute("Count").as_int();
		this->m_stMonsterGroupInfo[GroupNumber].m_nRegenTimeMin = monster.attribute("RegenTime").as_int();
	}

	int AreaCnt = 0;
	TempGroupNumber = 0;
	pugi::xml_node monstergroupspot = main.child("MonsterGroupSpot");

	for (pugi::xml_node spot = monstergroupspot.child("Spot"); spot; spot = spot.next_sibling())
	{
		int GroupNumber = spot.attribute("MonsterGroup").as_int();

		if ( TempGroupNumber != GroupNumber )
		{
			TempGroupNumber = GroupNumber;
			AreaCnt = 0;
		}

		if ( GroupNumber < 0 || GroupNumber > 50 )
		{
			g_Log.Add("[ArcaBattle][MonsterGroupRegen] Invalid Monster groupNumber");
			return false;
		}

		this->m_stMonsterPosition[GroupNumber].m_nGroupNumber = GroupNumber;
		this->m_stMonsterPosition[GroupNumber].m_stRegenArea[AreaCnt].m_nBeginX = spot.attribute("StartX").as_int();
		this->m_stMonsterPosition[GroupNumber].m_stRegenArea[AreaCnt].m_nBeginY = spot.attribute("StartY").as_int();
		this->m_stMonsterPosition[GroupNumber].m_stRegenArea[AreaCnt].m_nEndX = spot.attribute("EndX").as_int();
		this->m_stMonsterPosition[GroupNumber].m_stRegenArea[AreaCnt].m_nEndY = spot.attribute("EndY").as_int();
		
		AreaCnt++;
		this->m_stMonsterPosition[GroupNumber].m_nRegenAreaCnt = AreaCnt;
	}
					
	int ArrayIndex = 0;			
	TempGroupNumber = 0;
	pugi::xml_node monstergroupmember = main.child("MonsterGroupMember");

	for (pugi::xml_node monster = monstergroupmember.child("Monster"); monster; monster = monster.next_sibling())
	{
		int GroupNumber = monster.attribute("Group").as_int();

		if( TempGroupNumber != GroupNumber )
		{
			TempGroupNumber = GroupNumber;
			ArrayIndex = 0;
		}

		if ( GroupNumber < 0 || GroupNumber > 50 )
		{
			g_Log.Add("[ArcaBattle][MonsterGroupRegen] Invalid Monster groupNumber");
			return false;
		}

		if ( this->m_stMonsterPosition[GroupNumber].m_nGroupNumber < 0 )
		{
			g_Log.Add("[ArcaBattle][MonsterGroupRegen] Invalid groupNumber(%d)", GroupNumber);
			return false;
		}

		this->m_stMonsterPosition[GroupNumber].m_stMonsterIndexInfo[ArrayIndex].m_nMonsterIndex = monster.attribute("Index").as_int();
		ArrayIndex++;

		if ( ArrayIndex >= 20 )
		{
			g_Log.Add("[ArcaBattle][MonsterGroupRegen] Out of Max MonsterKindIndex");
			return false;
		}

		this->m_stMonsterPosition[GroupNumber].m_nMonsterKindCnt = ArrayIndex;
	}

	int UserPosCnt = 0;
	TempGroupNumber = 0;
	pugi::xml_node playerspawn = main.child("PlayerSpawnSettings");

	for (pugi::xml_node spawn = playerspawn.child("Spawn"); spawn; spawn = spawn.next_sibling())
	{
		int GroupNumber = spawn.attribute("Group").as_int();

		if( TempGroupNumber != GroupNumber )
		{
			TempGroupNumber = GroupNumber;
			UserPosCnt = 0;
		}

		if ( GroupNumber < 0 || GroupNumber > 50 )
		{
			g_Log.Add("[ArcaBattle][MonsterGroupRegen] Invalid Monster groupNumber");
			return false;
		}

		this->m_stAB_UserPos[GroupNumber].group_number = GroupNumber;
		this->m_stAB_UserPos[GroupNumber].stUserPos[UserPosCnt].map_number = spawn.attribute("MapNumber").as_int();
		this->m_stAB_UserPos[GroupNumber].stUserPos[UserPosCnt].beginX = spawn.attribute("StartX").as_int();
		this->m_stAB_UserPos[GroupNumber].stUserPos[UserPosCnt].beginY = spawn.attribute("StartY").as_int();
		this->m_stAB_UserPos[GroupNumber].stUserPos[UserPosCnt].endX = spawn.attribute("EndX").as_int();
		this->m_stAB_UserPos[GroupNumber].stUserPos[UserPosCnt].endY = spawn.attribute("EndY").as_int();

		UserPosCnt++;
		this->m_stAB_UserPos[GroupNumber].area_cnt = UserPosCnt;
	}

	return true;
}

int CArcaBattle::LoadScriptBootyMix(char *lpszFileName)
{
	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(lpszFileName);

	if(res.status != pugi::status_ok)
	{
		g_Log.MsgBox("[ArcaBattle][BootyExchange] %s File Load Error (%s)", lpszFileName, res.description());
		return FALSE;
	}

	pugi::xml_node main = file.child("ArcaBattle");
	pugi::xml_node rewardsettings = main.child("RewardSettings");
	this->m_iBootyMixCnt = 0;

	for(pugi::xml_node reward = rewardsettings.child("Reward"); reward; reward = reward.next_sibling())
	{
		this->m_stBootyMix[this->m_iBootyMixCnt].iItemLink = reward.attribute("Index").as_int();
		this->m_stBootyMix[this->m_iBootyMixCnt].iMinCnt = reward.attribute("MinCount").as_int();
		this->m_stBootyMix[this->m_iBootyMixCnt].iMaxCnt = reward.attribute("MaxCount").as_int();
		this->m_stBootyMix[this->m_iBootyMixCnt].iBaseRate = reward.attribute("BaseRate").as_int();
		this->m_stBootyMix[this->m_iBootyMixCnt].iAddRate = reward.attribute("AddRate").as_int();
		this->m_iBootyMixCnt++;
	}

	pugi::xml_node trophymix = main.child("TrophyOfBattleMix");

	for(pugi::xml_node combination = trophymix.child("Combination"); combination; combination = combination.next_sibling())
	{
		int Level = combination.attribute("Level").as_int();
        
		if ( this->m_iBootyMixCnt < Level )
		{
			g_Log.MsgBox("[ArcaBattle] BootyMix %s File Load Error - BootyMixCnt Over", lpszFileName);
			break;
		}
		
		int iCount = 0;
		for(pugi::xml_node item = combination.child("Item"); item; item = item.next_sibling())
		{
			this->m_stBootyMix[Level-1].stBootyReward[iCount].iItemType = item.attribute("Cat").as_int();
			this->m_stBootyMix[Level-1].stBootyReward[iCount].iItemIndex = item.attribute("Index").as_int();
			this->m_stBootyMix[Level-1].stBootyReward[iCount].iItemLevel = item.attribute("Level").as_int();
			this->m_stBootyMix[Level-1].stBootyReward[iCount].iDurab = item.attribute("Durability").as_int();
			this->m_stBootyMix[Level-1].stBootyReward[iCount].iRate = item.attribute("Probability").as_int();
			this->m_stBootyMix[Level-1].stBootyReward[iCount].iItemLink = Level;
			this->m_stBootyMix[Level-1].iBootyRewardCnt++;
			iCount++;
		}
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


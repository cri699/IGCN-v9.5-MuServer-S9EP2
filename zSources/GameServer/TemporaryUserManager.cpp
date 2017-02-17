//////////////////////////////////////////////////////////////////////
// TemporaryUserManager.cpp
#include "stdafx.h"
#include "TemporaryUserManager.h"
#include "GameMain.h"
#include "PartyClass.h"
#include "TLog.h"
#include "protocol.h"
#include "DBSockMng.h"
#include "BloodCastle.h"
#include "DevilSquare.h"
#include "DoppelGanger.h"
#include "ImperialGuardian.h"
#include "IllusionTempleEvent.h"
#include "IllusionTempleProcess.h"
#include "winutil.h"

CTemporaryUserManager * CTemporaryUserManager::m_pInstance = NULL;

CTemporaryUserManager::CTemporaryUserManager(void)
{
	InitializeCriticalSection(&this->m_criti);
	this->m_UserData = new TEMPORARY_USER_MANAGER_DATA[ObjectMaxUser];
	this->InitializeUserData();
}

CTemporaryUserManager::~CTemporaryUserManager(void)
{
	delete [] this->m_UserData;
	DeleteCriticalSection(&this->m_criti);
}

CTemporaryUserManager * CTemporaryUserManager::GetInstance()
{
	if ( CTemporaryUserManager::m_pInstance != NULL )
	{
		return CTemporaryUserManager::m_pInstance;
	}

	CTemporaryUserManager::m_pInstance = new CTemporaryUserManager();

	return CTemporaryUserManager::m_pInstance;
}

void CTemporaryUserManager::InitializeUserData()
{
	for(int i=0;i<ObjectMaxUser;i++)
	{
		this->m_UserData[i].Clear();
	}
}

int CTemporaryUserManager::SearchEmptyIndex()
{
	for(int i=0;i<ObjectMaxUser;i++)
	{
		if ( this->m_UserData[i].m_Index == 0 )
		{
			return i;
		}
	}

	return -1;
}

void CTemporaryUserManager::ClearSpecificUserData(int Index)
{
	if ( Index < 0 || Index > ObjectMaxUser-1 )
	{
		return;
	}

	if ( ObjectMaxRange(this->m_UserData[Index].m_Index) == FALSE )
	{
		return;
	}

	gObj[this->m_UserData[Index].m_Index].Connected = PLAYER_EMPTY;
	this->m_UserData[Index].Clear();
}

BOOL CTemporaryUserManager::AddUserData(char * Name, int aIndex, int PartyNumber)
{
	EnterCriticalSection(&this->m_criti);
	int Index = this->SearchEmptyIndex();
	LeaveCriticalSection(&this->m_criti);

	if ( Index == -1 )
	{
		return FALSE;
	}

	this->m_UserData[Index].m_dwTick = GetTickCount();
	memcpy(this->m_UserData[Index].m_Name, Name, MAX_ACCOUNT_LEN);
	this->m_UserData[Index].m_Index = aIndex;
	this->m_UserData[Index].m_Change = gObj[aIndex].m_Change;
	this->m_UserData[Index].m_PartyNumber = gObj[aIndex].PartyNumber;
	this->m_UserData[Index].m_MapNumber = gObj[aIndex].MapNumber;
	this->m_UserData[Index].m_BloodCastleIndex = gObj[aIndex].m_cBloodCastleIndex;
	this->m_UserData[Index].m_BloodCastleSubIndex = gObj[aIndex].m_cBloodCastleSubIndex;
	this->m_UserData[Index].m_BloodCastleExp = gObj[aIndex].m_iBloodCastleEXP;
	this->m_UserData[Index].m_IllusionTempleIndex = gObj[aIndex].m_cIllusionTempleIndex;
	this->m_UserData[Index].m_ImperialEventIndex = g_ImperialGuardian.GetCurrentZoneIndex(aIndex);
	this->m_UserData[Index].m_DevilSquareScore = gObj[aIndex].m_nEventScore;
	this->m_UserData[Index].m_DevilSquareExp = gObj[aIndex].m_nEventExp;
	this->m_UserData[Index].m_DevilSquareMoney = gObj[aIndex].m_nEventMoney;
	this->m_UserData[Index].m_DevilSquareIndex = gObj[aIndex].m_bDevilSquareIndex;
	this->m_UserData[Index].m_DevilSquareAuth = gObj[aIndex].m_bDevilSquareAuth;

	return TRUE;
}

BOOL CTemporaryUserManager::AddToPriviousParty(int Index, int aIndex)
{
	if ( Index < 0 || Index > ObjectMaxUser-1 )
	{
		return FALSE;
	}

	if ( strcmp(this->m_UserData[Index].m_Name, gObj[aIndex].Name) )
	{
		return FALSE;
	}

	gParty.Delete(this->m_UserData[Index].m_PartyNumber, this->m_UserData[Index].m_Index, gObj[aIndex].DBNumber);

	if ( gParty.Add(this->m_UserData[Index].m_PartyNumber, aIndex, gObj[aIndex].DBNumber, gObj[aIndex].Level) >= 0 )
	{
		gObj[aIndex].PartyNumber = this->m_UserData[Index].m_PartyNumber;
		GSProtocol.CGPartyListAll(this->m_UserData[Index].m_PartyNumber);
		gParty.PartyLog(this->m_UserData[Index].m_PartyNumber, aIndex, 2);
	}

	else
	{
		GSProtocol.GCServerMsgStringSend(Lang.GetText(0,407), aIndex, 1);
	}

	this->ClearSpecificUserData(Index);

	return TRUE;
}

void CTemporaryUserManager::ClearOverTimeUserData(int OverTimeSeconds)
{
	DWORD Tick = GetTickCount();

	for ( int i=0; i<ObjectMaxUser; i++ )
	{
		if ( this->m_UserData[i].m_Index && Tick - this->m_UserData[i].m_dwTick > 1000 * OverTimeSeconds )
		{
 			if ( ObjectMaxRange(this->m_UserData[i].m_Index) == FALSE )
			{
				this->TidyUpTempUserDate(i);
				continue;
			}

			if ( this->CheckInEventMap(i) == 0 || this->CheckInEventMap(i) == 1 || this->CheckInEventMap(i) == 5 )
			{
				gParty.Delete(this->m_UserData[i].m_PartyNumber, this->m_UserData[i].m_Index, gObj[this->m_UserData[i].m_Index].DBNumber);
				gParty.UpdatePKPartyPanalty(this->m_UserData[i].m_PartyNumber);
				GSProtocol.CGPartyListAll(this->m_UserData[i].m_PartyNumber);

				if ( gParty.GetCount(this->m_UserData[i].m_PartyNumber) < 2 )
				{
					for ( int j=0; j<MAX_USER_IN_PARTY; j++ )
					{
						int aIndex = gParty.m_PartyS[this->m_UserData[i].m_PartyNumber].Number[j];
						if( aIndex >= 0 )
						{
							gParty.Delete(this->m_UserData[i].m_PartyNumber, j);
							gObj[aIndex].PartyNumber = -1;
							gObj[aIndex].PartyTargetUser = -1;
							GSProtocol.GCPartyDelUserSend(aIndex);
						}
					}

					gParty.Destroy(this->m_UserData[i].m_PartyNumber);
				}
			}

			this->TidyUpTempUserDate(i);
		}
	}
}

void CTemporaryUserManager::TidyUpTempUserDate(int Index)
{
	if ( Index < 0 || Index > ObjectMaxUser-1 )
	{
		return;
	}

	SDHP_REQ_SET_EXGAMESERVERCODE pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0xA6, sizeof(pMsg));

	memcpy(pMsg.szCharName, this->m_UserData[Index].m_Name, MAX_ACCOUNT_LEN+1);
	pMsg.sExGameServerCode = -1;

	wsDataCli.DataSend((char *)&pMsg, pMsg.h.size);

	this->LeaveEvent(Index);
	this->ClearSpecificUserData(Index);
}

void CTemporaryUserManager::DeleteUserInParty(int Index)
{
	if ( Index < 0 || Index > ObjectMaxUser-1 )
	{
		return;
	}

	if ( ObjectMaxRange(this->m_UserData[Index].m_Index) == FALSE )
	{
		return;
	}

	gParty.Delete(this->m_UserData[Index].m_PartyNumber, this->m_UserData[Index].m_Index, gObj[this->m_UserData[Index].m_Index].DBNumber);
	gParty.UpdatePKPartyPanalty(this->m_UserData[Index].m_PartyNumber);
	GSProtocol.CGPartyListAll(this->m_UserData[Index].m_PartyNumber);

	if ( gParty.GetCount(this->m_UserData[Index].m_PartyNumber) < 2 )
	{
		for ( int i=0; i<MAX_USER_IN_PARTY; i++ )
		{
			int aIndex = gParty.m_PartyS[this->m_UserData[Index].m_PartyNumber].Number[i];
			if( aIndex >= 0 )
			{
				gParty.Delete(this->m_UserData[Index].m_PartyNumber, i);
				gObj[aIndex].PartyNumber = -1;
				gObj[aIndex].PartyTargetUser = -1;	
				GSProtocol.GCPartyDelUserSend(aIndex);
			}
		}

		gParty.Destroy(this->m_UserData[Index].m_PartyNumber);
	}

	this->TidyUpTempUserDate(Index);
}

void CTemporaryUserManager::DeleteAllPartyUserData(int PartyNumber)
{
	if ( ObjectMaxRange(PartyNumber) == FALSE )
	{
		return;
	}

	for ( int i=0; i<ObjectMaxUser;i++ )
	{
		if ( this->m_UserData[i].m_PartyNumber == PartyNumber )
		{
			this->TidyUpTempUserDate(i);
		}
	}
}

int CTemporaryUserManager::CheckMatchCharacter(char * Name)
{
	for ( int i=0; i<ObjectMaxUser;i++ )
	{
		if ( this->m_UserData[i].m_Index != 0 )
		{
			if ( strcmp(this->m_UserData[i].m_Name, Name) == 0 )
			{
				return i;
			}
		}
	}
	
	return -1;
}

int CTemporaryUserManager::CheckMatchIndex(int aIndex)
{
	for ( int i=0; i<ObjectMaxUser;i++ )
	{
		if ( this->m_UserData[i].m_Index == aIndex )
		{
			return i;
		}
	}

	return -1;
}

int CTemporaryUserManager::CheckInEventMap(int Index)
{
	if ( Index < 0 || Index > ObjectMaxUser-1 )
	{
		return -1;
	}

	if ( BC_MAP_RANGE(this->m_UserData[Index].m_MapNumber) )
	{
		return 1;
	}

	if ( IT_MAP_RANGE(this->m_UserData[Index].m_MapNumber) )
	{
		return 2;
	}

	if ( IMPERIAL_MAP_RANGE(this->m_UserData[Index].m_MapNumber) )
	{
		return 3;
	}

	if ( DG_MAP_RANGE(this->m_UserData[Index].m_MapNumber) )
	{
		return 4;
	}

	if ( DS_MAP_RANGE(this->m_UserData[Index].m_MapNumber) )
	{
		return 5;
	}

	return 0;
}

void CTemporaryUserManager::LeaveEvent(int Index)
{
	int EventNumber = this->CheckInEventMap(Index);

	if ( EventNumber == 2 )
	{
		g_IllusionTempleEvent.LeaveUser(this->m_UserData[Index].m_Index, this->m_UserData[Index].m_MapNumber);
	}

	if ( EventNumber == 4 )
	{
		g_DoppelGanger.LeaveDoppelganger(this->m_UserData[Index].m_Index);
	}

	if ( EventNumber == 5 )
	{
		g_DevilSquare.LeaveDevilSquare(this->m_UserData[Index].m_DevilSquareIndex, this->m_UserData[Index].m_Index);
	}
}

BOOL CTemporaryUserManager::ResumeEvent(int Index, int aIndex)
{
	if ( Index < 0 || Index > ObjectMaxUser-1 )
	{
		return FALSE;
	}

	int EventMap = this->CheckInEventMap(Index);

	switch ( EventMap )
	{
		case 1:
			return this->ResumeBloodCastle(Index, aIndex);
		case 2:
			return this->ResumeIllusionTemple(Index, aIndex);
		case 3:
			return this->ResumeImperialGuardian(Index, aIndex);
		case 4:
			return this->ResumeDoppelganger(Index, aIndex);
		case 5:
			return this->ResumeDevilSquare(Index, aIndex);
		default:
			return FALSE;
	}

	return FALSE;
}

BOOL  CTemporaryUserManager::ResumeIllusionTemple(int Index, int aIndex)
{
	if ( Index < 0 || Index > ObjectMaxUser-1 )
	{
		return FALSE;
	}

	BYTE ITState = g_IllusionTempleEvent.GetCurrentState(this->m_UserData[Index].m_MapNumber);

	if ( ITState == ILLUSION_STATE_PLAYEND )
	{
		return FALSE;
	}

	BYTE Result = g_IllusionTempleEvent.ChangeTempleUserInformation(this->m_UserData[Index].m_Index, aIndex, this->m_UserData[Index].m_IllusionTempleIndex, this->m_UserData[Index].m_MapNumber);

	if ( Result == FALSE )
	{
		return FALSE;
	}

	gObj[aIndex].m_cIllusionTempleIndex = this->m_UserData[Index].m_IllusionTempleIndex;

	if ( g_IllusionTempleEvent.GetCurrentState(this->m_UserData[Index].m_MapNumber) > ILLUSION_STATE_CLOSED )
	{
		PMSG_ILLUSIONTEMPLE_CHANGESTATE pMsg;
		PHeadSubSetB((LPBYTE)&pMsg, 0xBF, 0x09, sizeof(pMsg));

		pMsg.IllusionTempleIndex = this->m_UserData[Index].m_IllusionTempleIndex;
		pMsg.IllusionTempleState = 1;

		IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);

		int SkillPoint = g_IllusionTempleEvent.GetSkillPoint(aIndex, this->m_UserData[Index].m_MapNumber);

		g_Log.Add(0, __FILE__, __FUNCTION__, "SkillPoint : %d, Name : %s", SkillPoint, this->m_UserData[Index].m_Name);

		GSProtocol.GCSendIllusionTempleKillPoint(aIndex, SkillPoint);

		return TRUE;
	}

	return FALSE;
}

BOOL CTemporaryUserManager::ResumeImperialGuardian(int Index, int aIndex)
{
	if ( Index < 0 || Index > ObjectMaxUser-1 )
	{
		return FALSE;
	}

	if ( g_ImperialGuardian.GetPlayUserCountRightNow(this->m_UserData[Index].m_ImperialEventIndex) >= 1 )
	{
		return g_ImperialGuardian.ChangeUserIndex(this->m_UserData[Index].m_Index, aIndex, this->m_UserData[Index].m_ImperialEventIndex);
	}

	return FALSE;
}

BOOL CTemporaryUserManager::ResumeBloodCastle(int Index, int aIndex)
{
	if ( Index < 0 || Index > ObjectMaxUser-1 )
	{
		return FALSE;
	}

	int BCCount = g_BloodCastle.GetPlayUserCountRightNow(this->m_UserData[Index].m_BloodCastleIndex);

	if ( BCCount >= 1 )
	{
		if ( g_BloodCastle.ChangeUserIndex(this->m_UserData[Index].m_Index, aIndex, this->m_UserData[Index].m_BloodCastleIndex) == TRUE )
		{
			gObj[aIndex].m_cBloodCastleIndex = this->m_UserData[Index].m_BloodCastleIndex;
			gObj[aIndex].m_cBloodCastleSubIndex = this->m_UserData[Index].m_BloodCastleSubIndex;
			gObj[aIndex].m_iBloodCastleEXP = this->m_UserData[Index].m_BloodCastleExp;

			return TRUE;
		}
	}

	return FALSE;
}

BOOL CTemporaryUserManager::ResumeDoppelganger(int Index, int aIndex)
{
	if ( Index < 0 || Index > ObjectMaxUser-1 )
	{
		return FALSE;
	}

	if ( g_DoppelGanger.GetDoppelgangerState() == 3 )
	{
		return FALSE;
	}

	if ( g_DoppelGanger.GetPlayUserCountRightNow() >= 1 )
	{
		return g_DoppelGanger.ChangeUserIndex(this->m_UserData[Index].m_Index, aIndex);
	}

	return FALSE;
}

BOOL CTemporaryUserManager::ResumeDevilSquare(int Index, int aIndex)
{
	if ( Index < 0 || Index > ObjectMaxUser-1 )
	{
		return FALSE;
	}

	if ( g_DevilSquare.GetPlayUserCountRightNow(this->m_UserData[Index].m_DevilSquareIndex) >= 1 )
	{
		if ( g_DevilSquare.GetState() > DevilSquare_CLOSE )
		{
			gObj[aIndex].m_bDevilSquareIndex = this->m_UserData[Index].m_DevilSquareIndex;
			gObj[aIndex].m_bDevilSquareAuth = this->m_UserData[Index].m_DevilSquareAuth;
			gObj[aIndex].m_nEventExp = this->m_UserData[Index].m_DevilSquareExp;
			gObj[aIndex].m_nEventMoney = this->m_UserData[Index].m_DevilSquareMoney;
			gObj[aIndex].m_nEventScore = this->m_UserData[Index].m_DevilSquareScore;

			return g_DevilSquare.ChangeUserIndex(this->m_UserData[Index].m_Index, aIndex, this->m_UserData[Index].m_DevilSquareIndex);
		}
	}

	return FALSE;
}

void CTemporaryUserManager::SendEventMapInfo(int Index, int aIndex)
{
	int EventMap = this->CheckInEventMap(Index);

	if ( EventMap == 2 )
	{
		PMSG_ILLUSIONTEMPLE_CHANGESTATE pMsg;
		PHeadSubSetB((LPBYTE)&pMsg, 0xBF, 0x09, sizeof(pMsg));

		pMsg.IllusionTempleIndex = this->m_UserData[Index].m_IllusionTempleIndex;
		pMsg.IllusionTempleState = g_IllusionTempleEvent.GetCurrentState(this->m_UserData[Index].m_MapNumber);

		IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
		return;
	}

	if ( EventMap == 1 )
	{
		if ( g_BloodCastle.GetPlayUserCountRightNow(this->m_UserData[Index].m_BloodCastleIndex) >= 1 )
		{
			int BlockInfo = g_BloodCastle.GetCastleBlockInfo(this->m_UserData[Index].m_BloodCastleIndex);

			if ( BlockInfo >= 1001 )
				g_BloodCastle.SendCastleEntranceBlockInfo(this->m_UserData[Index].m_BloodCastleIndex, 0);
			
			if ( BlockInfo >= 1002 )
				g_BloodCastle.SendCastleBridgeBlockInfo(this->m_UserData[Index].m_BloodCastleIndex, 0);

			if ( BlockInfo == 1003 )
				g_BloodCastle.SendCastleDoorBlockInfo(this->m_UserData[Index].m_BloodCastleIndex, 0);

			if ( g_BloodCastle.GetCurrentState(this->m_UserData[Index].m_BloodCastleIndex) == BC_STATE_PLAYING )
			{
				g_BloodCastle.SendNoticeMessageToSpecificUser(this->m_UserData[Index].m_BloodCastleIndex, aIndex, 0);
			}
		}

		return;
	}

	if ( EventMap == 4 )
	{
		if ( g_DoppelGanger.GetPlayUserCountRightNow() >= 1 )
		{
			int nState = g_DoppelGanger.GetDoppelgangerState();

			if ( nState >= 1 )
			{
				g_DoppelGanger.SendDoppelgangerStateToSpecificUser(aIndex, 0);
				g_DoppelGanger.SendDoppelgangerStateToSpecificUser(aIndex, 1);
			}

			if ( nState >= 2 )
			{
				g_DoppelGanger.SendMapTileInfo(&gObj[aIndex], 1);
				g_DoppelGanger.SendDoppelgangerStateToSpecificUser(aIndex, 2);
			}
		}

		else
		{
			this->LeaveEvent(Index);
		}
	}

	if ( EventMap == 3 )
	{
		if ( g_ImperialGuardian.GetPlayUserCountRightNow(this->m_UserData[Index].m_ImperialEventIndex) >= 1 )
		{
			g_ImperialGuardian.SendCurStateToUser(aIndex, this->m_UserData[Index].m_ImperialEventIndex);
			g_ImperialGuardian.SendCurGateInfoToUser(aIndex, this->m_UserData[Index].m_ImperialEventIndex);
		}
	}
}

void TidyupTempuserdate(int aIndex, int party_number, BYTE PartyPos)
{
	return;

	if ( PartyPos )
	{
		int Index = gTempUserManager->CheckMatchIndex(aIndex);

		if ( Index != -1 )
		{
			gTempUserManager->TidyUpTempUserDate(Index);
		}
	}

	else
	{
		for ( int i=0; i<5; i++ )
		{
			int Number = gParty.m_PartyS[party_number].Number[i];

			if ( Number != -1 )
			{
				int Index = gTempUserManager->CheckMatchIndex(aIndex);

				if ( Index != -1 )
				{
					int EventMap = gTempUserManager->CheckInEventMap(Index);

					if ( EventMap != 2 && EventMap != 3 && EventMap != 4 )
					{
						gTempUserManager->TidyUpTempUserDate(Index);
					}
				}
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


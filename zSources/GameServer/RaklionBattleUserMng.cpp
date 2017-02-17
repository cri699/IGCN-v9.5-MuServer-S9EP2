// RaklionBattleUserMng.cpp: implementation of the CRaklionBattleUserMng class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RaklionBattleUserMng.h"
#include "user.h"
#include "Raklion.h"
#include "RaklionUtil.h"
#include "TLog.h"
#include "configread.h"
CRaklionBattleUserMng	g_RaklionBattleUserMng;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRaklionBattleUserMng::CRaklionBattleUserMng()
{
	ResetAllData();
}

CRaklionBattleUserMng::~CRaklionBattleUserMng()
{

}

void CRaklionBattleUserMng::ResetAllData()
{
	while(!m_BattleUser.empty())
	{
		m_BattleUser.pop_back();
	}

	m_BattleUser.clear();
}

BOOL CRaklionBattleUserMng::AddUserData(int iIndex)
{
	if(gObjIsConnected(iIndex) == FALSE)
	{
		g_Log.Add("[ RAKLION ][ BattleUser ] Add User Fail - Disconnect User [%s][%s]", gObj[iIndex].AccountID, gObj[iIndex].Name);
		return FALSE;
	}

	m_BattleUser.push_back(iIndex);

	g_RaklionUtil.NotifyRaklionCurrentState(iIndex, g_Raklion.GetRaklionState(), g_Raklion.GetRaklionStateDetail());

	g_Log.Add("[ RAKLION ][ Battle User ] Add User [%s][%s] Current Battle User:%d", gObj[iIndex].AccountID, gObj[iIndex].Name, m_BattleUser.size());
	return TRUE;
}

BOOL CRaklionBattleUserMng::DeleteUserData(int iIndex)
{
	if( iIndex < 0 || iIndex > g_ConfigRead.server.GetObjectMax()-1 )
	{
		g_Log.AddC(TColor::Red, "[ RAKLION ][ BattleUser ] Delete User Fail - Unvalid Index:%d", iIndex);
		return FALSE;
	}

	for(int iCount=0; iCount < m_BattleUser.size(); iCount++)
	{
		if(m_BattleUser[iCount] == iIndex)
		{
			m_BattleUser.erase(m_BattleUser.begin()+iCount);
			g_Log.Add("[ RAKLION ][ BattleUser ] Delete User - [%s][%s] Current Battle User:%d", gObj[iIndex].AccountID, gObj[iIndex].Name, m_BattleUser.size());
			return TRUE;
		}
	}
	return FALSE;
}

void CRaklionBattleUserMng::CheckUserState()
{
	for(int iCount=0; iCount<m_BattleUser.size(); iCount++)
	{
		if( !gObjIsConnected(m_BattleUser[iCount]))
		{
			DeleteUserData(m_BattleUser[iCount]);
			g_Log.Add("[ RAKLION ][ BattleUser ] Delete User - Disconnect [%s][%s]", gObj[m_BattleUser[iCount]].AccountID, gObj[m_BattleUser[iCount]].Name);
		}

		if( gObj[m_BattleUser[iCount]].MapNumber != MAP_INDEX_HATCHERY &&
			 gObj[m_BattleUser[iCount]].m_State == 2 &&
			 gObj[m_BattleUser[iCount]].Live == 1 &&
			 gObj[m_BattleUser[iCount]].m_SkillInfo.LordSummonMapNumber != MAP_INDEX_HATCHERY )
		{
			DeleteUserData(m_BattleUser[iCount]);
			g_Log.Add("[ RAKLION ][ BattleUser ] Delete User - Map Move [%s][%s]", gObj[m_BattleUser[iCount]].AccountID, gObj[m_BattleUser[iCount]].Name);
		}
	}
}

BOOL CRaklionBattleUserMng::MoveAllUser(int iGateNumber)
{
	BOOL bMoveGateSuccess = FALSE;

	for(int iCount=0; iCount<m_BattleUser.size(); iCount++)
	{
		bMoveGateSuccess = gObjMoveGate(m_BattleUser[iCount], iGateNumber);
		if(bMoveGateSuccess == TRUE)
		{
			g_Log.Add("[ RAKLION ][ BattleUser ] [%s][%s] MoveGate(%d)", gObj[m_BattleUser[iCount]].AccountID, gObj[m_BattleUser[iCount]].Name, iGateNumber);
		}
		else
		{
			DeleteUserData(m_BattleUser[iCount]);
			g_Log.Add("[ RAKLION ][ BattleUser ] [%s][%s] MoveGate Fail (%d)", gObj[m_BattleUser[iCount]].AccountID, gObj[m_BattleUser[iCount]].Name, iGateNumber);
			gObjMoveGate(m_BattleUser[iCount], 286); //Raklion Entrance
		}
	}
	return TRUE;
}

int CRaklionBattleUserMng::GetUserCount()
{
	return m_BattleUser.size();
}

int CRaklionBattleUserMng::IsEmpty()
{
	return (BYTE)m_BattleUser.empty();
}

void CRaklionBattleUserMng::LogBattleWinnerUserInfo(BYTE btFlag, int iElapsedTime)
{
/*	int iIndex = -1;
	float fTime = iElapsedTime / 1000.0f;

	for(int iCount=0; iCount<GetUserCount(); iCount++)
	{
		iIndex = m_BattleUser[iCount];
		LogAddTD("[ RAKLION ][ BATTLE WINNER ] [%d/ElapsedTime:%0.3f] [%s][%s] MapNumber[%d]-(%d/%d)", btFlag, fTime, gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].MapNumber, gObj[iIndex].X, gObj[iIndex].Y);
		//g_RaklionUtil.SendDataRaklionTimeAttackEvent(iIndex, btFlag, fTime);
	}*/
}

int CRaklionBattleUserMng::GetUserObjIndex(int iBattleUserNumber)
{
	int iObjIndex = m_BattleUser[iBattleUserNumber];
	return iObjIndex;
}

BOOL CRaklionBattleUserMng::IsBattleUser(int iIndex)
{
	for(int iCount=0; iCount<GetUserCount(); iCount++)
	{
		if(iIndex == m_BattleUser[iCount])
		{
			return TRUE;
		}
	}
	return FALSE;
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


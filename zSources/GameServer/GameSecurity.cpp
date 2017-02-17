//////////////////////////////////////////////////////////////////////
// GameSecurity.cpp
#include "stdafx.h"
#include "GameSecurity.h"
#include "TLog.h"
#include "user.h"
CGameSecurity gGameSecurity;

CGameSecurity::CGameSecurity()
{
	this->m_ASData.clear();
	InitializeCriticalSection(&lpCritiASData);
}

CGameSecurity::~CGameSecurity()
{

}

void CGameSecurity::DebugInfo(int aIndex)
{
	ATTACK_SPEED sAttackSpeed;
	EnterCriticalSection(&lpCritiASData);
	std::map<int,ATTACK_SPEED>::iterator iter = this->m_ASData.find(aIndex);
	
	if(iter == m_ASData.end())
	{
		sAttackSpeed.iPenaltyCount = 0;
		sAttackSpeed.LastAttackTime = GetTickCount64();
		this->m_ASData.insert(std::pair<int,ATTACK_SPEED>(aIndex, sAttackSpeed));
		LeaveCriticalSection(&lpCritiASData);
		return;
	}
	else
	{
		sAttackSpeed = iter->second;

		DWORD64 dwTimeNow = GetTickCount64();

		double realAttackSpeed = (double)((double)dwTimeNow - (double)sAttackSpeed.LastAttackTime) / 1000.00;

		g_Log.AddC(TColor::Orange,"[Debug ANTI-HACK][%d][%s][%s] Attack Speed: %d Time Between last attack = %f",aIndex, gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].m_AttackSpeed, realAttackSpeed);

		iter->second.LastAttackTime = dwTimeNow;

	}

	LeaveCriticalSection(&lpCritiASData);

}



//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


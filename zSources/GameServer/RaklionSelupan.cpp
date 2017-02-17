// RaklionSelupan.cpp: implementation of the CRaklionSelupan class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RaklionSelupan.h"
#include "RaklionMonsterMng.h"
#include "user.h"
#include "gObjMonster.h"
#include "TMonsterSkillManager.h"
#include "TLog.h"
#include "configread.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRaklionSelupan::CRaklionSelupan()
{
	Init();
}

CRaklionSelupan::~CRaklionSelupan()
{

}

void CRaklionSelupan::Init()
{
	m_iSelupanObjIndex = -1;
	m_iNowBerserkLevel = 0;
	m_iSelupanMinDamage = 0;
	m_iSelupanMaxDamage = 0;
}

void CRaklionSelupan::RaklionSelupanAct_FirstSkill()
{
	int iSelupanIndex = GetSelupanObjIndex();
	int iTargetIndex = GetTargetOfSelupan();
	LPOBJ lpObj = &gObj[iSelupanIndex];

	if( iTargetIndex == -1 )
	{
		iTargetIndex = iSelupanIndex;
	}
	//else
	{
		CMagicInf cMagicInf;
		memset(&cMagicInf, 0, sizeof(cMagicInf));
		cMagicInf.m_Skill = 253;
		TMonsterSkillManager::UseMonsterSkill(iSelupanIndex, iTargetIndex, 31, 0, &cMagicInf);
	}

	g_Log.Add("[ RAKLION ][ FirstSkill ] Skill Using : Index(%d) %s", iSelupanIndex, lpObj->Name);
}

void CRaklionSelupan::RaklionSelupanAct_PoisonAttack()
{
	int iSelupanIndex = GetSelupanObjIndex();
	int iTargetIndex = GetTargetOfSelupan();
	LPOBJ lpObj = &gObj[iSelupanIndex];

	if( iTargetIndex == -1 )
	{
		iTargetIndex = iSelupanIndex;
		TMonsterSkillManager::UseMonsterSkill(iSelupanIndex, iTargetIndex, 1, 7, NULL);
	}
	else
	{
		CMagicInf cMagicInf;
		memset(&cMagicInf, 0, sizeof(cMagicInf));
		cMagicInf.m_Skill = 250;
		TMonsterSkillManager::UseMonsterSkill(iSelupanIndex, iTargetIndex, 1, 1, &cMagicInf);
	}

	g_Log.Add("[ RAKLION ][ PoisonAttack ] Skill Using : Index(%d) %s", iSelupanIndex, lpObj->Name);
}

void CRaklionSelupan::RaklionSelupanAct_IceStorm()
{
	int iSelupanIndex = GetSelupanObjIndex();
	int iTargetIndex = GetTargetOfSelupan();
	LPOBJ lpObj = &gObj[iSelupanIndex];

	if( iTargetIndex == -1 )
	{
		iTargetIndex = iSelupanIndex;
		TMonsterSkillManager::UseMonsterSkill(iSelupanIndex, iTargetIndex, 1, 7, NULL);
	}
	else
	{
		CMagicInf cMagicInf;
		memset(&cMagicInf, 0, sizeof(cMagicInf));
		cMagicInf.m_Skill = 251;
		TMonsterSkillManager::UseMonsterSkill(iSelupanIndex, iTargetIndex, 1, 2, &cMagicInf);
	}

	g_Log.Add("[ RAKLION ][ IceStorm ] Skill Using : Index(%d) %s", iSelupanIndex, lpObj->Name);
}

void CRaklionSelupan::RaklionSelupanAct_IceStrike()
{
	int iSelupanIndex = GetSelupanObjIndex();
	int iTargetIndex = GetTargetOfSelupan();
	LPOBJ lpObj = &gObj[iSelupanIndex];

	if( iTargetIndex == -1 )
	{
		iTargetIndex = iSelupanIndex;
		TMonsterSkillManager::UseMonsterSkill(iSelupanIndex, iTargetIndex, 1, 7, NULL);
	}
	else
	{
		CMagicInf cMagicInf;
		memset(&cMagicInf, 0, sizeof(cMagicInf));
		cMagicInf.m_Skill = 252;
		TMonsterSkillManager::UseMonsterSkill(iSelupanIndex, iTargetIndex, 1, 3, &cMagicInf);
	}

	g_Log.Add("[ RAKLION ][ IceStrike ] Skill Using : Index(%d) %s", iSelupanIndex, lpObj->Name);
}

void CRaklionSelupan::RaklionSelupanAct_SummonMonster()
{
	int iSelupanIndex = GetSelupanObjIndex();
	int iTargetIndex = iSelupanIndex;
	LPOBJ lpObj = &gObj[iSelupanIndex];

	TMonsterSkillManager::UseMonsterSkill(iSelupanIndex, iTargetIndex, 30, 4, NULL);
	
	RegenSummonMonster();
	
	g_Log.Add("[ RAKLION ][ SummonMonster ] Skill Using : Index(%d) %s", iSelupanIndex, lpObj->Name);
}

void CRaklionSelupan::RaklionSelupanAct_Heal()
{
	int iSelupanIndex = GetSelupanObjIndex();
	int iTargetIndex = iSelupanIndex;
	LPOBJ lpObj = &gObj[iSelupanIndex];

	TMonsterSkillManager::UseMonsterSkill(iSelupanIndex, iTargetIndex, 20, 5, NULL);

	g_Log.Add("[ RAKLION ][ Heal ] Skill Using : Index(%d) %s", iSelupanIndex, lpObj->Name);
}

void CRaklionSelupan::RaklionSelupanAct_Freeze()
{
	int iSelupanIndex = GetSelupanObjIndex();
	int iTargetIndex = GetTargetOfSelupan();
	LPOBJ lpObj = &gObj[iSelupanIndex];

	if( iTargetIndex == -1 )
	{
		iTargetIndex = iSelupanIndex;
		TMonsterSkillManager::UseMonsterSkill(iSelupanIndex, iTargetIndex, 1, 7, NULL);
	}
	else TMonsterSkillManager::UseMonsterSkill(iSelupanIndex, iTargetIndex, 1, 6, NULL);

	g_Log.Add("[ RAKLION ][ Freeze ] Skill Using : Index(%d) %s", iSelupanIndex, lpObj->Name);
}

void CRaklionSelupan::RaklionSelupanAct_Teleport()
{
	int iSelupanIndex = GetSelupanObjIndex();
	int iTargetIndex = GetTargetOfSelupan();
	LPOBJ lpObj = &gObj[iSelupanIndex];
	LPOBJ  lpObjTarget = &gObj[iTargetIndex];

	if( iTargetIndex < 0 )	iTargetIndex = iSelupanIndex;

	TMonsterSkillManager::UseMonsterSkill(iSelupanIndex, iTargetIndex, 1, 7, NULL);
	g_Log.Add("[ RAKLION ][ Teleport ] Skill Using : Index(%d) %s", iSelupanIndex, lpObj->Name);
}

void CRaklionSelupan::RaklionSelupanAct_Invincibility()
{
	int iSelupanIndex = GetSelupanObjIndex();
	int iTargetIndex = iSelupanIndex;
	LPOBJ lpObj = &gObj[iSelupanIndex];

	TMonsterSkillManager::UseMonsterSkill(iSelupanIndex, iTargetIndex, 10, 8, NULL);
	
	g_Log.Add("[ RAKLION ][ Invincibility ] Skill Using : Index(%d) %s", iSelupanIndex, lpObj->Name);
}

void CRaklionSelupan::RaklionSelupanAct_Berserk_Cancel()
{
	int iSelupanIndex = GetSelupanObjIndex();
	LPOBJ lpObj = &gObj[iSelupanIndex];

	lpObj->m_AttackDamageMin = m_iSelupanMinDamage;
	lpObj->m_AttackDamageMax = m_iSelupanMaxDamage;
	
	g_Log.Add("[ RAKLION ][ Berserk Cancel ] MinDamage(%d) Damage(%d~%d)", lpObj->m_AttackDamageMin, lpObj->m_AttackDamageMin, lpObj->m_AttackDamageMax);
}

void CRaklionSelupan::RaklionSelupanAct_Berserk1()
{
	int iSelupanIndex = GetSelupanObjIndex();
	int iTargetIndex = iSelupanIndex;
	LPOBJ lpObj = &gObj[iSelupanIndex];
	
	TMonsterSkillManager::UseMonsterSkill(iSelupanIndex, iTargetIndex, 1, 9, NULL);
	
	m_iNowBerserkLevel = 1;

	int iIncreaseAttackValue = 0;
	iIncreaseAttackValue = m_iNowBerserkLevel * lpObj->m_MonsterSkillElementInfo.m_iSkillElementAttack;

	lpObj->m_AttackDamageMin = m_iSelupanMinDamage + iIncreaseAttackValue;
	lpObj->m_AttackDamageMax = m_iSelupanMaxDamage + iIncreaseAttackValue;

	g_Log.Add("[ RAKLION ][ Berserk1 ] Skill Using : Index(%d) Damage(%d~%d)", iSelupanIndex, lpObj->m_AttackDamageMin, lpObj->m_AttackDamageMax);
}

void CRaklionSelupan::RaklionSelupanAct_Berserk2()
{
	int iSelupanIndex = GetSelupanObjIndex();
	int iTargetIndex = iSelupanIndex;
	LPOBJ lpObj = &gObj[iSelupanIndex];
	
	TMonsterSkillManager::UseMonsterSkill(iSelupanIndex, iTargetIndex, 1, 9, NULL);
	
	m_iNowBerserkLevel = 2;

	int iIncreaseAttackValue = 0;
	iIncreaseAttackValue = m_iNowBerserkLevel * lpObj->m_MonsterSkillElementInfo.m_iSkillElementAttack;

	lpObj->m_AttackDamageMin = m_iSelupanMinDamage + iIncreaseAttackValue;
	lpObj->m_AttackDamageMax = m_iSelupanMaxDamage + iIncreaseAttackValue;

	g_Log.Add("[ RAKLION ][ Berserk2 ] Skill Using : Index(%d) Damage(%d~%d)", iSelupanIndex, lpObj->m_AttackDamageMin, lpObj->m_AttackDamageMax);
}

void CRaklionSelupan::RaklionSelupanAct_Berserk3()
{
	int iSelupanIndex = GetSelupanObjIndex();
	int iTargetIndex = iSelupanIndex;
	LPOBJ lpObj = &gObj[iSelupanIndex];
	
	TMonsterSkillManager::UseMonsterSkill(iSelupanIndex, iTargetIndex, 1, 9, NULL);
	
	m_iNowBerserkLevel = 3;

	int iIncreaseAttackValue = 0;
	iIncreaseAttackValue = m_iNowBerserkLevel * lpObj->m_MonsterSkillElementInfo.m_iSkillElementAttack;

	lpObj->m_AttackDamageMin = m_iSelupanMinDamage + iIncreaseAttackValue;
	lpObj->m_AttackDamageMax = m_iSelupanMaxDamage + iIncreaseAttackValue;

	g_Log.Add("[ RAKLION ][ Berserk3 ] Skill Using : Index(%d) Damage(%d~%d)", iSelupanIndex, lpObj->m_AttackDamageMin, lpObj->m_AttackDamageMax);
}

void CRaklionSelupan::RaklionSelupanAct_Berserk4()
{
	int iSelupanIndex = GetSelupanObjIndex();
	int iTargetIndex = iSelupanIndex;
	LPOBJ lpObj = &gObj[iSelupanIndex];
	
	TMonsterSkillManager::UseMonsterSkill(iSelupanIndex, iTargetIndex, 1, 9, NULL);
	
	m_iNowBerserkLevel = 4;

	int iIncreaseAttackValue = 0;
	iIncreaseAttackValue = m_iNowBerserkLevel * lpObj->m_MonsterSkillElementInfo.m_iSkillElementAttack;

	lpObj->m_AttackDamageMin = m_iSelupanMinDamage + iIncreaseAttackValue;
	lpObj->m_AttackDamageMax = m_iSelupanMaxDamage + iIncreaseAttackValue;

	g_Log.Add("[ RAKLION ][ Berserk4 ] Skill Using : Index(%d) Damage(%d~%d)", iSelupanIndex, lpObj->m_AttackDamageMin, lpObj->m_AttackDamageMax);
}

DWORD CRaklionSelupan::GetSelupanHP()
{
	int iSelupanIndex = GetSelupanObjIndex();
	LPOBJ lpObj = &gObj[iSelupanIndex];
	return (DWORD)lpObj->Life;
}

void CRaklionSelupan::SetSelupanHP(int iSelupanHPPercent)
{
	int iSelupanIndex = GetSelupanObjIndex();
	LPOBJ lpObj = &gObj[iSelupanIndex];
	float iSelupanMAXLife = lpObj->AddLife + lpObj->MaxLife;
	lpObj->Life = iSelupanMAXLife * iSelupanHPPercent / 100;
}

void CRaklionSelupan::InitSelupanIndex()
{
	for (int iCount = 0; iCount < g_ConfigRead.server.GetObjectMaxMonster(); iCount++)
	{
		if (gObj[iCount].Connected == PLAYER_PLAYING && gObj[iCount].Class == 459)
		{
			this->m_iSelupanObjIndex = iCount;
		}
	}

	if (this->m_iSelupanObjIndex == -1)
	{
		g_Log.AddC(TColor::Red, "[Raklion][InitSelupanIndex] error : SelupanObjIndex(%d)", this->m_iSelupanObjIndex);
	}
}

BOOL CRaklionSelupan::RegenSelupan()
{
	int iSelupanPosNum = -1;

	for (int i = 0; i < g_RaklionMonsterMng.m_iMaxMonsterCount; i++)
	{
		if (g_RaklionMonsterMng.m_SetBaseInfo[i].wType == 459)
		{
			iSelupanPosNum = i;
		}
	}

	int result = gObjAddMonster(MAP_INDEX_HATCHERY);

	if (result < 0)
	{
		g_Log.Add("[RAKLION] CreateSelupan error");
		return FALSE;
	}

	g_RaklionMonsterMng.SetPosition(result, iSelupanPosNum);
	gObjSetMonster(result, 459);

	LPOBJ lpObj = &gObj[result];

	gObjMonsterHitDamageInit(lpObj);

	gObj[result].DieRegen = 0;
	gObj[result].m_State = 1;

	g_Log.Add("[RAKLION] Create Selupan : X (%d) / Y(%d)", lpObj->X, lpObj->Y);

	LPOBJ lpObjUser = NULL;

	for (int n = 0; n < g_ConfigRead.server.GetObjectMax(); n++)
	{
		lpObjUser = &gObj[n];
		if (lpObjUser->MapNumber == MAP_INDEX_HATCHERY)
		{
			gObjStateSetCreate(n);
		}
	}

	for (int n = 0; n < g_ConfigRead.server.GetObjectMax(); n++)
	{
		lpObjUser = &gObj[n];
		if (lpObjUser->MapNumber == MAP_INDEX_HATCHERY)
		{
			gObjViewportListDestroy(n);
		}
	}

	for (int n = 0; n < g_ConfigRead.server.GetObjectMax(); n++)
	{
		lpObjUser = &gObj[n];
		if (lpObjUser->MapNumber == MAP_INDEX_HATCHERY)
		{
			gObjViewportListCreate(n);
		}
	}

	for (int n = 0; n < g_ConfigRead.server.GetObjectMax(); n++)
	{
		lpObjUser = &gObj[n];
		if (lpObjUser->MapNumber == MAP_INDEX_HATCHERY)
		{
			gObjViewportListProtocol(n);
		}
	}

	gObjSetState();

	m_iSelupanMinDamage = lpObj->m_AttackDamageMin;
	m_iSelupanMaxDamage = lpObj->m_AttackDamageMax;
	return TRUE;
}

void CRaklionSelupan::HideSelupan()
{
	int iSelupanIndex = GetSelupanObjIndex();

	if (iSelupanIndex != -1)
	{
		gObjViewportListProtocolDestroy(&gObj[iSelupanIndex]);
		gObjViewportClose(&gObj[iSelupanIndex]);
		gObjDel(iSelupanIndex);
	}
}

int CRaklionSelupan::GetTargetOfSelupan()
{
	int iSelupanIndex = GetSelupanObjIndex();
	LPOBJ lpObj = &gObj[iSelupanIndex];

	if( lpObj->TargetNumber >= 0 && lpObj->TargetNumber < g_ConfigRead.server.GetObjectMax()) 
	{
		return lpObj->TargetNumber;
	}

	return gObjMonsterSearchEnemy(lpObj, lpObj->Type);
}

void CRaklionSelupan::SetSelupanSkillDelay(int iDelay)
{
	if( iDelay < 0 || iDelay > (1000*50) )
	{
		g_Log.AddC(TColor::Red, "[ RAKLION ][ SetSelupanSkillDelay ] SkillDelay error : Delay(%d)", m_iSkillDelay);
		return;
	}
	m_iSkillDelay = iDelay;
}

int CRaklionSelupan::GetSelupanSkillDelay()
{
	if( m_iSkillDelay < 0 || m_iSkillDelay > 50000 )
	{
		g_Log.AddC(TColor::Red, "[ RAKLION ][ GetSelupanSkillDelay ] SkillDelay error : Delay(%d)", m_iSkillDelay);
		return -1;
	}
	return m_iSkillDelay;
}

int CRaklionSelupan::GetBerserkLevel()
{
	return m_iNowBerserkLevel;
}

void CRaklionSelupan::InitSelupanDamage(int iMinDamage,int iMaxDamage)
{
	m_iSelupanMinDamage = iMinDamage;
	m_iSelupanMaxDamage = iMaxDamage;
}

void CRaklionSelupan::SetSelupanObjIndex(int iIndex)
{
	if( iIndex < 0 || iIndex > g_ConfigRead.server.GetObjectMaxMonster() ) 
	{
		g_Log.AddC(TColor::Red, "[ RAKLION ][ SetSelupanObjIndex ] SelupanObjIndex error : Index(%d)", iIndex);
		return;
	}
	m_iSelupanObjIndex = iIndex;
}

int CRaklionSelupan::GetSelupanObjIndex()
{
	InitSelupanIndex();
	return m_iSelupanObjIndex;
}

void CRaklionSelupan::RegenSummonMonster()
{
	LPOBJ lpObj = NULL;
	int iPosTableNum = -1;
	int iClass = -1;

	for (int iCount = 0; iCount < g_ConfigRead.server.GetObjectMaxMonster(); iCount++)
	{
		lpObj = &gObj[iCount];

		if (lpObj->MapNumber != MAP_INDEX_HATCHERY)	continue;

		if (lpObj->Class == 457 || lpObj->Class == 458)
		{
			if (lpObj->Connected == PLAYER_PLAYING &&
				lpObj->DieRegen == 0 &&
				lpObj->Live == 1)	continue;

			iClass = lpObj->Class;
			iPosTableNum = lpObj->m_PosNum;

			gObjCharZeroSet(iCount);
			gObjDel(lpObj->m_Index);

			lpObj->Class = 0;

			int result = gObjAddMonster(MAP_INDEX_HATCHERY);

			lpObj = &gObj[result];

			if (result < 0)
			{
				g_Log.Add("[RAKLION] CreateSelupan error");
				return;
			}

			g_RaklionMonsterMng.SetPosition(result, iPosTableNum);
			gObjSetMonster(result, iClass);
			lpObj->Live = 1;
			lpObj->DieRegen = 0;
			g_Log.Add("[RAKLION] Summoned Monster : %s => X-(%d) Y-(%d)", lpObj->Name, lpObj->X, lpObj->Y);
		}
	}
}

void CRaklionSelupan::DeleteSummonMonster()
{
	LPOBJ lpObj = NULL;

	for(int iCount=0; iCount<g_ConfigRead.server.GetObjectMaxMonster(); iCount++)
	{
		lpObj = &gObj[iCount];

		if( lpObj->MapNumber != MAP_INDEX_HATCHERY )	continue;	
		if( lpObj->Class == 457 || lpObj->Class == 458 )
		{
			gObjViewportListProtocolDestroy(lpObj);
			gObjViewportClose(lpObj);
			gObjDel(lpObj->m_Index);
			lpObj->Live = 0; 
			lpObj->DieRegen = 1;
		}
	}

	g_Log.AddC(TColor::Brown, "[ RAKLION ] [ DeleteSummonMonster ] Delete All SummonMonster");
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


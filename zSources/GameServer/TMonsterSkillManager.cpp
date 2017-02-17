// TMonsterSkillManager.cpp: implementation of the TMonsterSkillManager class.
//	GS-N	1.00.18	JPN	0x00558440	-	Completed
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TMonsterSkillManager.h"
#include "ObjUseSkill.h"
#include "SkillHitBox.h"
#include "TLog.h"
#include "configread.h"

TMonsterSkillInfo TMonsterSkillManager::s_MonsterSkillInfoArray[MAX_MONSTER_SKILL_INFO_ARRAY];
_ST_MONSTER_SKILL_DELAYTIME_INFO TMonsterSkillManager::s_MonsterSkillDelayInfoArray[MAX_MONSTER_SKILL_DELAY_INFO_ARRAY];
TSync TMonsterSkillManager::s_Sync;
BOOL TMonsterSkillManager::s_bDataLoad;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TMonsterSkillManager::TMonsterSkillManager()
{
	return;
}

TMonsterSkillManager::~TMonsterSkillManager()
{
	return;
}

BOOL TMonsterSkillManager::LoadData(LPSTR lpszFileName)
{
	TMonsterSkillManager::s_bDataLoad = FALSE;

	if ( !lpszFileName || !strcmp(lpszFileName, ""))
	{
		g_Log.MsgBox("[Monster Skill Manager] - File load error : File Name Error");
		return FALSE;
	}

	try
	{
		pugi::xml_document file;
		pugi::xml_parse_result res = file.load_file(lpszFileName);

		if ( res.status != pugi::status_ok )
		{
			g_Log.MsgBox("[Monster Skill Manager] - Can't Load %s (%s)", lpszFileName, res.description());
			return FALSE;
		}

		TMonsterSkillManager::DelAllSkillManagerInfo();

		pugi::xml_node main = file.child("MonsterSkill");

		for (pugi::xml_node skill = main.child("Skill"); skill; skill = skill.next_sibling())
		{
			BOOL bVerified = TRUE;
			int iMonsterIndex = skill.attribute("MonsterIndex").as_int();

			int iSkillUnitTypeArray[MAX_MONSTER_SKILL_UNIT_INFO];
			TMonsterSkillUnit * lpSkillUnitArray[MAX_MONSTER_SKILL_UNIT_INFO];
			int iSkillUnitCount = 0;

			char szTemp[255];
			memset(szTemp, 0x00, sizeof(szTemp));

			for ( int i = 0; i < MAX_MONSTER_SKILL_UNIT_INFO; i++ )
			{
				sprintf(szTemp, "Type%d", i);
				iSkillUnitTypeArray[i] = skill.attribute(szTemp).as_int();

				sprintf(szTemp, "Unit%d", i);
				int iSkillUnitNumber = skill.attribute(szTemp).as_int();

				if ( iSkillUnitNumber == -1 )
					continue;

				TMonsterSkillUnit * lpSkillUnit = TMonsterSkillUnit::FindSkillUnit(iSkillUnitNumber);
				
				if ( lpSkillUnit == NULL )
				{
					bVerified = FALSE;
					continue;
				}

				lpSkillUnitArray[i] = lpSkillUnit;
				iSkillUnitCount++;
			}

			if ( iMonsterIndex >= MAX_MONSTER_SKILL_INFO_ARRAY )
			{
				g_Log.MsgBox("[Monster Manager] - MonsterIndex(%d) Error (%s) File. ", iMonsterIndex, lpszFileName);
				continue;
			}

			if (bVerified == FALSE)
			{
				g_Log.AddC(TColor::Red, "[Monster Manager] - Invalid SkillInfo : MIndex(%d)", iMonsterIndex);
			}

			TMonsterSkillManager::s_MonsterSkillInfoArray[iMonsterIndex].m_iMonsterIndex = iMonsterIndex;

			for (int i = 0; i < MAX_MONSTER_SKILL_UNIT_INFO; i++)
			{
				TMonsterSkillManager::s_MonsterSkillInfoArray[iMonsterIndex].m_iSkillUnitTypeArray[i] = iSkillUnitTypeArray[i];
				TMonsterSkillManager::s_MonsterSkillInfoArray[iMonsterIndex].m_lpSkillUnitArray[i] = lpSkillUnitArray[i];
			}

			TMonsterSkillManager::s_MonsterSkillInfoArray[iMonsterIndex].m_iSkillUnitCount = iSkillUnitCount;
		}

		g_Log.AddC(TColor::Red,  "[Monster Skill Manager] - %s file is Loaded", lpszFileName);
		TMonsterSkillManager::s_bDataLoad = TRUE;
	}

	catch(DWORD)
	{
		g_Log.MsgBox("[Monster Skill Manager] - Loading Exception Error (%s) File. ", lpszFileName);
	}

	return FALSE;
}


void TMonsterSkillManager::DelAllSkillManagerInfo()
{
	for ( int i=0;i<MAX_MONSTER_SKILL_INFO_ARRAY;i++)
	{
		TMonsterSkillManager::s_MonsterSkillInfoArray[i].Reset();
	}

	for ( int j=0;j<MAX_MONSTER_SKILL_DELAY_INFO_ARRAY;j++)
	{
		TMonsterSkillManager::s_MonsterSkillDelayInfoArray[j].RESET();
	}
}

BOOL TMonsterSkillManager::CheckMonsterSkill(int iMonsterClass)
{
	if ( iMonsterClass < 0 || iMonsterClass > MAX_MONSTER_SKILL_INFO_ARRAY-1 )
		return FALSE;

	TMonsterSkillInfo * lpMonsterSkillInfo = &TMonsterSkillManager::s_MonsterSkillInfoArray[iMonsterClass];

	if ( lpMonsterSkillInfo->IsValid() == FALSE )
		return FALSE;

	return TRUE;
}

BOOL TMonsterSkillManager::AddMonsterSkillDelayInfo(int iIndex, int iTargetIndex, int iDelayTime, TMonsterSkillUnit * lpMonsterSkillUnit)
{
	for ( int i=0;i<MAX_MONSTER_SKILL_DELAY_INFO_ARRAY;i++)
	{
		if ( TMonsterSkillManager::s_MonsterSkillDelayInfoArray[i].bIsUsed == FALSE )
		{
			TMonsterSkillManager::s_MonsterSkillDelayInfoArray[i].iIndex = iIndex;
			TMonsterSkillManager::s_MonsterSkillDelayInfoArray[i].iTargetIndex = iTargetIndex;
			TMonsterSkillManager::s_MonsterSkillDelayInfoArray[i].dwDelayTime = GetTickCount() + iDelayTime;
			TMonsterSkillManager::s_MonsterSkillDelayInfoArray[i].lpMonsterSkillUnit = lpMonsterSkillUnit;
			TMonsterSkillManager::s_MonsterSkillDelayInfoArray[i].bIsUsed = TRUE;

			return TRUE;
		}
	}

	g_Log.AddC(TColor::Red,  "[Monster Skill Manager] - (MonsterSkillDelayInfo) Slot Full!! ");
	return FALSE;
}


void TMonsterSkillManager::MonsterSkillProc() 
{
	DWORD dwCurrentTick = GetTickCount();

	for ( int i=0;i<MAX_MONSTER_SKILL_DELAY_INFO_ARRAY;i++)
	{
		if ( TMonsterSkillManager::s_MonsterSkillDelayInfoArray[i].bIsUsed == TRUE )
		{
			if ( dwCurrentTick < TMonsterSkillManager::s_MonsterSkillDelayInfoArray[i].dwDelayTime )
			{
				_ST_MONSTER_SKILL_DELAYTIME_INFO & stInfo = TMonsterSkillManager::s_MonsterSkillDelayInfoArray[i];

				if ( !stInfo.IsValid() )
				{
					TMonsterSkillManager::s_MonsterSkillDelayInfoArray[i].RESET();
					continue;
				}

				LPOBJ lpObj = &gObj[stInfo.iIndex];
				LPOBJ lpTargetObj = &gObj[stInfo.iTargetIndex];

				if ( !lpObj->Live || !lpTargetObj->Live )
				{
					TMonsterSkillManager::s_MonsterSkillDelayInfoArray[i].RESET();
					continue;
				}

				if ( !gObjIsConnected(lpTargetObj))
				{
					TMonsterSkillManager::s_MonsterSkillDelayInfoArray[i].RESET();
					continue;
				}

				gObjAttack(lpObj, lpTargetObj, 0, 0, 0, 0, 0, 0, 0);
				stInfo.lpMonsterSkillUnit->RunSkill(stInfo.iIndex, stInfo.iTargetIndex);
				TMonsterSkillManager::s_MonsterSkillDelayInfoArray[i].RESET();
			}
		}
	}
}

TMonsterSkillUnit * TMonsterSkillManager::FindMonsterSkillUnit(int iIndex, int iMonsterSkillUnitType)
{
	LPOBJ lpObj = &gObj[iIndex];
	TMonsterSkillInfo * lpMonsterSkillInfo = &TMonsterSkillManager::s_MonsterSkillInfoArray[lpObj->Class];

	if ( lpMonsterSkillInfo->IsValid() == FALSE )
		return NULL;

	int iFoundSkillArray[10] = {-1};
	int iFoundSkillCount = 0;

	for(int i = 0; i < MAX_MONSTER_SKILL_UNIT_INFO; i++)
	{
		if ( iMonsterSkillUnitType == lpMonsterSkillInfo->m_iSkillUnitTypeArray[i] )
		{
			iFoundSkillArray[iFoundSkillCount] = i;
			iFoundSkillCount++;
		}
	}

	if ( iFoundSkillCount > 0 )
	{
		int iSlot = iFoundSkillArray[rand() % iFoundSkillCount];

		if ( iSlot == -1 )
			return NULL;

		TMonsterSkillUnit * lpMonsterSkillUnit = lpMonsterSkillInfo->m_lpSkillUnitArray[iSlot];
		
		return lpMonsterSkillUnit;
	}
	
	return NULL;
}


void TMonsterSkillManager::UseMonsterSkill(int iIndex, int iTargetIndex, int iMonsterSkillUnitType, int iMonsterSkillUnit, CMagicInf * lpMagic)
{
	if(!OBJMON_RANGE(iIndex))
	{
		g_Log.AddC(TColor::Red, "[TMonsterSkillManager][UserMonsterSkill] error: iIndex(%d)", iIndex);
		return;
	}

	if(!ObjectMaxRange(iTargetIndex))
	{
		g_Log.AddC(TColor::Red, "[TMonsterSkillManager][UserMonsterSkill] error: iTargetIndex(%d)", iTargetIndex);
		return;
	}

	/* g_Log.Add("[TMonsterSkillManager] [%s] Used skill at INDEX:%d (UNITTYPE:%d UNIT:%d)",
		gObj[iIndex].Name, iTargetIndex, iMonsterSkillUnitType, iMonsterSkillUnit); */

	LPOBJ lpObj = &gObj[iIndex];
	LPOBJ lpTargetObj = &gObj[iTargetIndex];
	TMonsterSkillInfo * lpMonsterSkillInfo = &TMonsterSkillManager::s_MonsterSkillInfoArray[lpObj->Class];

	if ( lpMonsterSkillInfo->IsValid() == FALSE )
		return;

	TMonsterSkillUnit * lpMonsterSkillUnit;

	if(iMonsterSkillUnit <= -1)
	{
		lpMonsterSkillUnit = TMonsterSkillManager::FindMonsterSkillUnit(iIndex, iMonsterSkillUnitType);
	}
	else
	{
		lpMonsterSkillUnit = lpMonsterSkillInfo->m_lpSkillUnitArray[iMonsterSkillUnit];
	}

	if ( lpMonsterSkillUnit == NULL )
	{
		g_Log.AddC(TColor::Red,  "[Monster Skill Manager] SkillUnit is NULL ");
		return;
	}

	if ( IMPERIAL_MAP_RANGE(lpObj->MapNumber) == TRUE )
	{
		CMagicInf cMagicInf;
		memset(&cMagicInf, 0x00, sizeof(cMagicInf));

		if ( lpObj->Class == 519 && lpMonsterSkillUnit->m_iUnitNumber == 46 )
		{
			int nTargetIndex = gGetLowHPMonster(lpObj->m_nZoneIndex, lpObj->m_Index, 6);

			if ( nTargetIndex == -1 )
			{
				GSProtocol.GCActionSend(lpObj, 120, lpObj->m_Index, lpObj->TargetNumber);
				gObjAttack(lpObj, lpTargetObj, 0, 0, 0, 0, 0, 0, 0);
				return;
			}

			lpTargetObj = &gObj[nTargetIndex];
			iTargetIndex = nTargetIndex;
		}

		if ( lpMonsterSkillUnit->m_iUnitTargetType == 5 )
		{
			lpTargetObj = &gObj[iIndex];
			iTargetIndex = iIndex;
		}

		if ( !lpMagic )
		{
			TMonsterSkillManager::FindMagicInf(lpMonsterSkillUnit, &cMagicInf);
			lpMagic = &cMagicInf;
		}
	}

	GSProtocol.GCUseMonsterSkillSend(lpObj, lpTargetObj, lpMonsterSkillUnit->m_iUnitNumber);
	
	if ( lpMonsterSkillUnit->m_iUnitScopeType == -1 )
	{
		if(lpObj->MapNumber == MAP_INDEX_RAKLION)
		{
			gObjAttack(lpObj, lpTargetObj, NULL, NULL, NULL, NULL, NULL, 0, 0);
		}
		else if(lpObj->MapNumber == MAP_INDEX_HATCHERY)
		{
			if(lpObj->Class == 459)
			{
				if(lpMonsterSkillUnit->m_iUnitNumber == 40)
				{
					gObjAttack(lpObj, lpTargetObj, 0,0,0,0,0,0,0);
				}
				else if ( lpMagic )
				{
					gObjAttack(lpObj, lpTargetObj, lpMagic, 0, 0, 0, 0, 0, 0);
				}
			}
			else
			{
				gObjAttack(lpObj, lpTargetObj, 0,0,0,0,0,0,0);
			}
		}
		lpMonsterSkillUnit->RunSkill(iIndex, iTargetIndex);
	}
	else
	{
		int iCount = 0;
		int iRangeTargetIndex = -1;
		int iAngle = gObjUseSkill.GetAngle(lpObj->X, lpObj->Y, lpTargetObj->X, lpTargetObj->Y);

		while ( true )
		{
			if ( lpObj->VpPlayer2[iCount].state )
			{
				iRangeTargetIndex = lpObj->VpPlayer2[iCount].number;

				if ( ObjectMaxRange(iRangeTargetIndex))
				{
					if ( gObj[iRangeTargetIndex].Type == OBJ_USER )
					{
						BOOL bTargetOK = FALSE;

						if ( lpMonsterSkillUnit->m_iUnitScopeType == 0 )
						{
							if ( gObjCalDistance(lpObj, &gObj[iRangeTargetIndex]) < lpMonsterSkillUnit->m_iUnitScopeValue )
							{
								bTargetOK = TRUE;
							}
						}
						else  if ( lpMonsterSkillUnit->m_iUnitScopeType == 1 )
						{
							if ( SkillElectricSparkHitBox.HitCheck(iAngle, lpObj->X, lpObj->Y,
								gObj[iRangeTargetIndex].X, gObj[iRangeTargetIndex].Y) )
							{
								bTargetOK = TRUE;
							}
						}
						else if ( lpMonsterSkillUnit->m_iUnitScopeType == 2 )
						{
							bTargetOK = TRUE;
						}
						else if ( lpMonsterSkillUnit->m_iUnitScopeType == 3 )
						{
							bTargetOK = TRUE;
						}

						if ( bTargetOK )
						{
							if(lpObj->MapNumber == MAP_INDEX_RAKLION)
							{
								gObjAttack(lpObj, lpTargetObj, NULL, NULL, NULL, NULL, NULL, 0, 0);
							}
							else if(lpObj->MapNumber == MAP_INDEX_HATCHERY)
							{
								if(lpObj->Class == 459)
								{
									if(lpMonsterSkillUnit->m_iUnitNumber == 40)
									{
										gObjAttack(lpObj, lpTargetObj, 0,0,0,0,0,0,0);
									}
									else if(lpMagic)
									{
										gObjAttack(lpObj, lpTargetObj, lpMagic, 0,0,0,0,0,0);
									}
								}
								else
								{
									gObjAttack(lpObj, lpTargetObj, 0,0,0,0,0,0,0);
								}
							}
							else if(lpObj->Class >= 504 && lpObj->Class <= 521)
							{
								if ( lpMagic )
								{
									gObjAttack(lpObj, lpTargetObj, lpMagic, 0,0,0,0,0,0);
								}
								else
								{
									gObjAttack(lpObj, lpTargetObj, 0,0,0,0,0,0,0);
								}
							}
							lpMonsterSkillUnit->RunSkill(iIndex, iRangeTargetIndex);
						}
					}
				}
			}

			iCount++;

			if ( iCount > MaxViewportMonster-1)
				break;
		}
	}
						
}

bool TMonsterSkillManager::FindMagicInf(TMonsterSkillUnit *lpMonsterSkillUnit, CMagicInf* lpOutMagic)
{
	if ( !lpMonsterSkillUnit )
	{
		return false;
	}

	if ( !lpOutMagic )
	{
		return false;
	}

	switch ( lpMonsterSkillUnit->m_iUnitNumber )
	{
		case 64:
			lpOutMagic->m_Skill = 55;
			return true;
		case 65:
			lpOutMagic->m_Skill = 237;
			return true;
		case 66:
			lpOutMagic->m_Skill = 236;
			return true;
		case 51:
			lpOutMagic->m_Skill = 5;
			return true;
		default:
			return false;
	}

	return false;
}





//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


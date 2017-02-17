//////////////////////////////////////////////////////////////////////
// Weapon.cpp
#include "stdafx.h"
#include "Weapon.h"
#include "user.h"
#include "ObjAttack.h"
#include "BuffEffect.h"
#include "BuffEffectSlot.h"
#include "GameMain.h"
#include "configread.h"

// GS-N 0.99.60T 0x0051A5F0
//	GS-N	1.00.18	JPN	0x0054BF60	-	Completed

CWeapon g_CsNPC_Weapon;

#define MAX_ST_CS_WEAPON	(4)

struct ST_CS_WEAPON
{
	BYTE btXBegin;	// 0
	BYTE btYBegin;	// 1
	BYTE btXEnd;	// 2
	BYTE btYEnd;	// 3
};

static const ST_CS_WEAPON WEAPON_CS_ATTACKER[MAX_ST_CS_WEAPON] =
{
	0x3E, 0x67, 0x48, 0x70, 0x58, 0x68, 0x7C, 0x6F,
	0x74, 0x69, 0x7C, 0x70, 0x49, 0x56, 0x69, 0x67
};

static const ST_CS_WEAPON WEAPON_CS_DEFENSER[MAX_ST_CS_WEAPON] =
{
	0x3D, 0x58, 0x5D, 0x6C, 0x5C, 0x59, 0x7F, 0x6F,
	0x54, 0x34, 0x66, 0x42, 0x00, 0x00, 0x00, 0x00
};



CWeapon::CWeapon()
{
	for ( int i=0;i<MAX_WEAPON_DAMAGED_TARGET_INFO;i++)	// #error Cahnge to MAX_WEAPON_DAMAGED_TARGET_INFO
	{
		this->m_WeaponDamagedTargetInfo[i].RESET();
	}

	for ( int i=0;i<MAX_WEAPON_CAL_DAMAGER_INFO;i++)	// #error Cahnge to MAX_WEAPON_CAL_DAMAGER_INFO
	{
		this->m_WeaponCalDamageInfo[i].RESET();
	}
}



CWeapon::~CWeapon()
{
	return;
}

BOOL CWeapon::SetWeaponCalDamageInfo(WORD wObjIndex, BYTE btTargetX, BYTE btTargetY, int iDelayTime)
{
	for ( int i=0;i<MAX_WEAPON_CAL_DAMAGER_INFO;i++)
	{
		if ( this->m_WeaponCalDamageInfo[i].m_IsUsed == FALSE )
		{
			this->m_WeaponCalDamageInfo[i].m_iWeaponObjIndex = wObjIndex;
			this->m_WeaponCalDamageInfo[i].m_iTargetX = btTargetX;
			this->m_WeaponCalDamageInfo[i].m_iTargetY = btTargetY;
			this->m_WeaponCalDamageInfo[i].m_iLimitTime = GetTickCount() + iDelayTime;
			this->m_WeaponCalDamageInfo[i].m_IsUsed = TRUE;

			return TRUE;
		}
	}

	return FALSE;
}

BOOL CWeapon::GetWeaponCalDamageInfo(WORD wObjIndex, BYTE & btTargetX, BYTE & btTargetY)
{
	for( int i=0;i<MAX_WEAPON_CAL_DAMAGER_INFO;i++)
	{
		if ( this->m_WeaponCalDamageInfo[i].m_IsUsed == TRUE && this->m_WeaponCalDamageInfo[i].m_iWeaponObjIndex == wObjIndex)
		{
			btTargetX = this->m_WeaponCalDamageInfo[i].m_iTargetX;
			btTargetY = this->m_WeaponCalDamageInfo[i].m_iTargetY;

			this->m_WeaponCalDamageInfo[i].RESET();

			return TRUE;
		}
	}

	return FALSE;
}


int  CWeapon::GetAttackDamage(int iObjClass)
{
	int iDamage = 0;

	if ( iObjClass == 221 )
	{
		iDamage = gWeaponAttackDamage;
	}
	else if ( iObjClass == 222 )
	{
		iDamage = gWeaponDefenseDamage;
	}

	return iDamage;
}



BOOL CWeapon::GetTargetPointXY(int iObjClass, int iTargetPointIndex, BYTE &btX, BYTE &btY, BOOL bRandomPos)
{
	if ( iObjClass == 221 )
	{
		if ( iTargetPointIndex >= 0 && iTargetPointIndex < MAX_ST_CS_WEAPON )
		{
			int tx = WEAPON_CS_ATTACKER[iTargetPointIndex].btXEnd - WEAPON_CS_ATTACKER[iTargetPointIndex].btXBegin;
			int ty = WEAPON_CS_ATTACKER[iTargetPointIndex].btYEnd - WEAPON_CS_ATTACKER[iTargetPointIndex].btYBegin;

			if ( bRandomPos != FALSE )
			{
				btX = WEAPON_CS_ATTACKER[iTargetPointIndex].btXBegin + rand() % tx;
				btY = WEAPON_CS_ATTACKER[iTargetPointIndex].btYBegin + rand() % ty;
			}
			else
			{
				btX = WEAPON_CS_ATTACKER[iTargetPointIndex].btXBegin + tx / 2;
				btY = WEAPON_CS_ATTACKER[iTargetPointIndex].btYBegin + ty / 2;
			}
		}
	}

	if ( iObjClass == 222 )
	{
		if ( iTargetPointIndex >= 0 && iTargetPointIndex < MAX_ST_CS_WEAPON )
		{
			int tx = WEAPON_CS_DEFENSER[iTargetPointIndex].btXEnd - WEAPON_CS_DEFENSER[iTargetPointIndex].btXBegin;
			int ty = WEAPON_CS_DEFENSER[iTargetPointIndex].btYEnd - WEAPON_CS_DEFENSER[iTargetPointIndex].btYBegin;

			if ( bRandomPos != FALSE )
			{
				btX = WEAPON_CS_DEFENSER[iTargetPointIndex].btXBegin + rand() % tx;
				btY = WEAPON_CS_DEFENSER[iTargetPointIndex].btYBegin + rand() % ty;
			}
			else
			{
				btX = WEAPON_CS_DEFENSER[iTargetPointIndex].btXBegin + tx / 2;
				btY = WEAPON_CS_DEFENSER[iTargetPointIndex].btYBegin + ty / 2;
			}
		}
	}

	return TRUE;
}


BOOL CWeapon::MissCheck(LPOBJ lpObj, LPOBJ lpTargetObj, int iSkill, int iSkillSuccess, BOOL& bAllMiss)
{
	return TRUE;
}



BOOL CWeapon::Attack(LPOBJ lpObj, LPOBJ lpTargetObj, CMagicInf * lpMagic, int iCriticalDamage, int iActionType)
{
	int iSkill = 0;
	int iSkillSuccess = TRUE;
	BYTE MsgDamage = 0;
	int ManaChange = 0;

	if ( lpMagic != NULL )
	{
		iSkill = lpMagic->m_Skill;
	}

	if ( (lpTargetObj->Authority & 2) == 2 || (lpTargetObj->Authority & 0x20) == 0x20 )
	{
		return FALSE;
	}

	if ( lpObj->MapNumber != lpTargetObj->MapNumber )
	{
		return FALSE;
	}

	if ( lpTargetObj->Type == OBJ_USER )
	{
		if ( gObjIsConnected(lpTargetObj) == FALSE )
		{
			return FALSE;
		}
	}

	if ( gObjAttackQ(lpTargetObj) == 0 )
	{
		return FALSE;
	}

	lpObj->m_TotalAttackCount++;

	int MSBFlag = 0;

	if ( iCriticalDamage != 0 )
	{
		MsgDamage = 3;
	}
	else
	{
		MsgDamage = 0;
	}

	int iAttackDamage = this->GetAttackDamage(lpObj->Class);
	int iTargetDefense = lpTargetObj->m_Defense;
	int iDecreaseDefense = gObjGetTotalValueOfEffect(lpObj, EFFECTTYPE_DECREASE_DEFENSE);

	iTargetDefense -= (iTargetDefense * iDecreaseDefense)/100;

	BOOL bAllMiss = FALSE;
	
	if ( this->MissCheck(lpObj, lpTargetObj, iSkill, iSkillSuccess, bAllMiss) == FALSE )
	{
		return FALSE;
	}

	if ( bAllMiss != FALSE )
	{
		iAttackDamage = (iAttackDamage * 30)/100;
	}

	if ( lpTargetObj->DamageMinus != 0 )
	{
		int lc11 = iAttackDamage;
		iAttackDamage -= (iAttackDamage * lpTargetObj->DamageMinus)/100;
	}

	int iTargetLevel = lpObj->Level / 10;

	if ( iAttackDamage < iTargetLevel )
	{
		if ( iTargetLevel < 1 )
		{
			iTargetLevel = 1;
		}

		iAttackDamage = iTargetLevel;
	}

	if ( lpTargetObj->m_SkillNumber == 18 )
	{
		if ( iAttackDamage > 1 )
		{
			iAttackDamage = iAttackDamage >> 1;
		}
	}

	if ( gObjAngelSprite(lpTargetObj) == TRUE )
	{
		if ( iAttackDamage > 1 )
		{
			float lc13 = (float)(iAttackDamage * 8 ) / 10.0f;
			iAttackDamage = (int)( lc13 );
		}
	}

	if ( gObjWingSprite(lpTargetObj) == TRUE )
	{
		CItem * Wing = &lpTargetObj->pInventory[7];

		if ( iAttackDamage > 1 && Wing->m_Type != ITEMGET(13, 30) )
		{
			double WingDamageBlock;

			if (lpTargetObj->m_PlayerData->m_MPSkillOpt.iMpsAddWingDamageBlock <= 0.0)
			{
				WingDamageBlock = 0.0;
			}

			else
			{
				WingDamageBlock = lpTargetObj->m_PlayerData->m_MPSkillOpt.iMpsAddWingDamageBlock;
			}

			g_ConfigRead.m_ItemCalcLua.Generic_Call("Wings_CalcAbsorb", "iiid>i", iAttackDamage, Wing->m_Type, Wing->m_Level, WingDamageBlock, &iAttackDamage);
		}
	}

	if ( gObjDenorantSprite(lpTargetObj) != FALSE )
	{
		CItem * Dino = &lpTargetObj->pInventory[8];

		int ReduceDamage = 90 - Dino->IsDinorantReduceAttackDamaege();

		lpObj->Life -= 1.0f;

		if ( lpObj->Life < 0.0f )
		{
			lpObj->Life = 0;
		}
		else
		{
			iAttackDamage = (iAttackDamage * ReduceDamage) / 100;
		}

		GSProtocol.GCReFillSend(lpObj->m_Index, lpObj->Life, 0xFF, 0, lpObj->iShield);
	}

	if ( gObjDarkHorse(lpTargetObj) != FALSE )
	{
		CItem * DarkHorse = &lpTargetObj->pInventory[8];

		int DHPercent = 100-(DarkHorse->m_PetItem_Level+30)/2;

		lpTargetObj->Life -= 1.0f;

		if ( lpTargetObj->Life < 0.0f )
		{
			lpTargetObj->Life = 0;
		}
		else
		{
			iAttackDamage = (iAttackDamage * DHPercent)/100;
		}

		GSProtocol.GCReFillSend(lpTargetObj->m_Index, lpTargetObj->Life, 0xFF, 0, lpTargetObj->iShield);
	}

	if ( lpTargetObj->Live != FALSE )
	{
		if(gObjCheckUsedBuffEffect(lpTargetObj, BUFFTYPE_MELEE_DEFENSE_INC) == true && iAttackDamage > 0)
		{
			int iWizardSkillDefense = 0;
			int iManaRate = 0;

			gObjGetValueOfBuffIndex(lpTargetObj, BUFFTYPE_MELEE_DEFENSE_INC, &iWizardSkillDefense, &iManaRate);

			int iReplacementMana = 0;

			if(iManaRate > 0)
			{
				iReplacementMana = lpTargetObj->Mana * iManaRate/1000;
			}
			else iReplacementMana = lpTargetObj->Mana * 2/100;

			if( iReplacementMana < lpTargetObj->Mana )
			{
				lpTargetObj->Mana -= iReplacementMana;

				int decattackdamage = 0;
				if(iWizardSkillDefense > 0)	decattackdamage = iAttackDamage * iWizardSkillDefense/100;

				iAttackDamage -= decattackdamage;
				ManaChange = 1;
			}
		}

		else if ( gObjCheckUsedBuffEffect(lpTargetObj, BUFFTYPE_MELEE_DEFENSE_INC_STR) && iAttackDamage > 0)
		{
			int iWizardSkillDefense = 0;
			int iManaRate = 0;

			gObjGetValueOfBuffIndex(lpTargetObj, BUFFTYPE_MELEE_DEFENSE_INC_STR, &iWizardSkillDefense, &iManaRate);

			int iReplacementMana = 0;

			if(iManaRate > 0){
				iReplacementMana = lpTargetObj->Mana * iManaRate/1000;
			}
			else iReplacementMana = lpTargetObj->Mana * 2/100;

			if( iReplacementMana < lpTargetObj->Mana )
			{
				lpTargetObj->Mana -= iReplacementMana;

				int decattackdamage = 0;
				if(iWizardSkillDefense > 0)	decattackdamage = iAttackDamage * iWizardSkillDefense/100;

				iAttackDamage -= decattackdamage;
				ManaChange = 1;
			}
		}

		lpTargetObj->Life -= iAttackDamage;

		if ( lpTargetObj->Life < 0.0f )
		{
			lpTargetObj->Life = 0;
		}
	}

	if ( iAttackDamage >= 1 )
	{
		if ( lpTargetObj->Type == OBJ_USER )
		{
			gObjArmorRandomDurDown(lpTargetObj, lpObj);
		}
	}

	if ( iAttackDamage >= 5 )
	{
		if ( lpTargetObj->Type == OBJ_MONSTER )
		{
			if ( (rand()%26) == 0 )
			{
				gObjAddMsgSendDelay(lpTargetObj, 4, lpObj->m_Index, 100, 0);
			}
		}
		else if ( (rand()%4) == 0 )
		{
			if ( gObjUniriaSprite(lpTargetObj) == FALSE )
			{
				MSBFlag = TRUE;
			}
		}
	}

	if (lpTargetObj->Type == OBJ_USER)
	{
		if (lpTargetObj->pInventory[lpTargetObj->m_btInvenPetPos].IsItem() == TRUE && lpTargetObj->pInventory[lpTargetObj->m_btInvenPetPos].m_Type == ITEMGET(13,37))
		{
			if (g_ConfigRead.data.common.DisableMSBEffect[MSB_DISABLE_CHARACTER_FENRIR] == true)
			{
				MSBFlag = FALSE;
			}
		}

		else
		{
			if (g_ConfigRead.data.common.DisableMSBEffect[MSB_DISABLE_CHARACTER] == true)
			{
				MSBFlag = FALSE;
			}
		}
	}

	else if (lpTargetObj->Type == OBJ_MONSTER)
	{
		if (g_ConfigRead.data.common.DisableMSBEffect[MSB_DISABLE_MONSTER] == true)
		{
			MSBFlag = FALSE;
		}
	}

	if ( ManaChange != FALSE )
	{
		GSProtocol.GCManaSend(lpTargetObj->m_Index, lpTargetObj->Mana, (BYTE)-1, 0, lpTargetObj->BP);
	}

	lpObj->m_Rest = 0;

	if ( iAttackDamage > 0 )
	{
		gObjLifeCheck(lpTargetObj, lpObj, iAttackDamage, 0, MSBFlag, MsgDamage, iSkill, 0, 0);
	}

	return TRUE;
}


BOOL CWeapon::AddWeaponDamagedTargetInfo(int iWeaponIndex, int iTargetIndex, int iDelayTime)
{
	for ( int i=0;i<MAX_WEAPON_DAMAGED_TARGET_INFO;i++)
	{
		if ( this->m_WeaponDamagedTargetInfo[i].m_IsUsed == FALSE )
		{
			this->m_WeaponDamagedTargetInfo[i].m_iWeaponObjIndex = iWeaponIndex;
			this->m_WeaponDamagedTargetInfo[i].m_iTargetObjIndex = iTargetIndex;
			this->m_WeaponDamagedTargetInfo[i].m_iDamageDealingTime = GetTickCount() + iDelayTime;
			this->m_WeaponDamagedTargetInfo[i].m_IsUsed = TRUE;

			return TRUE;
		}
	}

	return FALSE;
}

	

void CWeapon::WeaponAttackProc()
{
	for ( int i=0;i<MAX_WEAPON_DAMAGED_TARGET_INFO;i++)
	{
		if ( this->m_WeaponDamagedTargetInfo[i].m_IsUsed == TRUE )
		{
			if ( this->m_WeaponDamagedTargetInfo[i].m_iDamageDealingTime > GetTickCount() )
			{
				continue;
			}

			LPOBJ lpTargetObj = &gObj[this->m_WeaponDamagedTargetInfo[i].m_iTargetObjIndex];
			LPOBJ lpWeaponObj = & gObj[this->m_WeaponDamagedTargetInfo[i].m_iWeaponObjIndex];

			if ( gObjIsConnected(lpTargetObj) == FALSE )
			{
				this->m_WeaponDamagedTargetInfo[i].RESET();
				continue;
			}

			if (  ObjectMaxRange(lpWeaponObj->m_Index) == FALSE )
			{
				this->m_WeaponDamagedTargetInfo[i].RESET();
				continue;
			}

			this->Attack(lpWeaponObj, lpTargetObj, NULL, FALSE, 0);

			this->m_WeaponDamagedTargetInfo[i].RESET();
		}
	}

}


void CWeapon::WeaponAct(int iIndex)
{
	for ( int i=0;i<MAX_WEAPON_CAL_DAMAGER_INFO;i++)
	{
		if ( this->m_WeaponCalDamageInfo[i].m_IsUsed == TRUE )
		{
			if ( this->m_WeaponCalDamageInfo[i].m_iWeaponObjIndex == iIndex )
			{
				if ( this->m_WeaponCalDamageInfo[i].m_iLimitTime < GetTickCount() )
				{
					this->m_WeaponCalDamageInfo[i].RESET();
				}
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


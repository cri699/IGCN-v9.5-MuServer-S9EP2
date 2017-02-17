// TMonsterSkillElementInfo.cpp: implementation of the TMonsterSkillElementInfo class.
//	GS-N	1.00.18	JPN	0x00557A00	-	Completed
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "TMonsterSkillElementInfo.h"

#include "protocol.h"
#include "user.h"
#include "BuffEffectSlot.h"
//////////////////////////////////////////////////////////////////////
// Construction
//////////////////////////////////////////////////////////////////////

void TMonsterSkillElementInfo::CheckSkillElementInfoProc(LPOBJ lpObj)
{
	if ( lpObj->m_MonsterSkillElementInfo.CheckAttackTime() )
	{
		if ( lpObj->m_MonsterSkillElementInfo.DecAttackTime() )
		{
			lpObj->m_MonsterSkillElementInfo.ResetAttack();
		}
	}

	if ( lpObj->m_MonsterSkillElementInfo.CheckDefenseTime() )
	{
		if ( lpObj->m_MonsterSkillElementInfo.DecDefenseTime() )
		{
			lpObj->m_MonsterSkillElementInfo.ResetDefense();
		}
	}
	
	if ( lpObj->m_MonsterSkillElementInfo.CheckAutoAGTime() )
	{
		if ( lpObj->m_MonsterSkillElementInfo.DecAutoAGTime() )
		{
			lpObj->m_MonsterSkillElementInfo.ResetAutoAG();
		}
	}

	if ( lpObj->m_MonsterSkillElementInfo.CheckAutoHPTime() )
	{
		if ( lpObj->m_MonsterSkillElementInfo.DecAutoHPTime() )
		{
			lpObj->m_MonsterSkillElementInfo.ResetAutoHP();
		}
		else 
		{
			if ( lpObj->m_MonsterSkillElementInfo.m_iSkillElementAutoHPCycle == 110 )
				lpObj->Life += lpObj->m_MonsterSkillElementInfo.m_iSkillElementAutoHP;
			else
				lpObj->Life += lpObj->Life * lpObj->m_MonsterSkillElementInfo.m_iSkillElementAutoHP / 100.0f;

			if ( lpObj->Life <0 )
			{
				lpObj->Life = 0;
				GSProtocol.GCReFillSend(lpObj->m_Index, lpObj->Life, 0xFF, 0, lpObj->iShield);
			}
			else if ( (lpObj->MaxLife + lpObj->AddLife) < lpObj->Life )
			{
				lpObj->Life = lpObj->MaxLife + lpObj->AddLife;
				GSProtocol.GCReFillSend(lpObj->m_Index, lpObj->Life, 0xFF, 0, lpObj->iShield);
			}
			else
			{
				GSProtocol.GCReFillSend(lpObj->m_Index, lpObj->Life, 0xFF, 0, lpObj->iShield);
			}
		}
	}

	if ( lpObj->m_MonsterSkillElementInfo.CheckAutoMPTime() )
	{
		if ( lpObj->m_MonsterSkillElementInfo.DecAutoMPTime() )
		{
			lpObj->m_MonsterSkillElementInfo.ResetAutoMP();
		}
	}

	if ( lpObj->m_MonsterSkillElementInfo.CheckImmuneTime() )
	{
		if ( lpObj->m_MonsterSkillElementInfo.DecImmuneTime() )
		{
			lpObj->m_MonsterSkillElementInfo.ResetImmune();
			gObjRemoveBuffEffect(lpObj, BUFFTYPE_MONSTER_MAGIC_IMMUNE);
			gObjRemoveBuffEffect(lpObj, BUFFTYPE_MONSTER_MELEE_IMMUNE);
		}
	}

	if ( lpObj->m_MonsterSkillElementInfo.CheckResistTime() )
	{
		if ( lpObj->m_MonsterSkillElementInfo.DecResistTime() )
		{
			lpObj->m_MonsterSkillElementInfo.ResetResist();
		}
	}

	if ( lpObj->m_MonsterSkillElementInfo.CheckModifyStatTime() )
	{
		if ( lpObj->m_MonsterSkillElementInfo.DecModifyStatTime() )
		{
			lpObj->m_MonsterSkillElementInfo.ResetModifyStat();
		}
	}

	if (lpObj->m_MonsterSkillElementInfo.CheckImmuneAllTime())
	{
		if (lpObj->m_MonsterSkillElementInfo.DecImmuneAllTime())
		{
			lpObj->m_MonsterSkillElementInfo.ResetImmuneAll();
		}
	}
}

void TMonsterSkillElementInfo::Reset()
{
	this->ResetDefense();
	this->ResetAttack();
	this->ResetAutoHP();
	this->ResetAutoMP();
	this->ResetAutoAG();
	this->ResetImmune();
	this->ResetResist();
	this->ResetModifyStat();
	this->ResetImmuneAll();
}

void  TMonsterSkillElementInfo::ResetDefense()
{
	this->m_iSkillElementDefense = 0;
	this->m_iSkillElementDefenseTime = 0;
}

void  TMonsterSkillElementInfo::ResetAttack()
{
	this->m_iSkillElementAttack = 0;
	this->m_iSkillElementAttackTime = 0;
}

void  TMonsterSkillElementInfo::ResetAutoHP()
{
	this->m_iSkillElementAutoHP = 0;
	this->m_iSkillElementAutoHPCycle = 0;
	this->m_iSkillElementAutoHPTime = 0;
}

void  TMonsterSkillElementInfo::ResetAutoMP()
{
	this->m_iSkillElementAutoMP = 0;
	this->m_iSkillElementAutoMPCycle = 0;
	this->m_iSkillElementAutoMPTime = 0;
}

void  TMonsterSkillElementInfo::ResetAutoAG()
{
	this->m_iSkillElementAutoAG = 0;
	this->m_iSkillElementAutoAGCycle = 0;
	this->m_iSkillElementAutoAGTime = 0;
}

void  TMonsterSkillElementInfo::ResetImmune()
{
	this->m_iSkillElementImmuneNumber = 0;
	this->m_iSkillElementImmuneTime = 0;
}

void  TMonsterSkillElementInfo::ResetResist()
{
	this->m_iSkillElementResistNumber = 0;
	this->m_iSkillElementResistTime = 0;
}

void  TMonsterSkillElementInfo::ResetModifyStat()

{
	this->m_iSkillElementModifyStat = 0;
	this->m_iSkillElementModifyStatType = 0;
	this->m_iSkillElementModifyStatTime = 0;
}

void  TMonsterSkillElementInfo::ResetImmuneAll()
{
	this->m_iSkillElementImmuneAllTime = 0;
}

/////////////////////////////////////////////////////////

BOOL  TMonsterSkillElementInfo::CheckDefenseTime()
{
	return (this->m_iSkillElementDefense <= 0)?FALSE:TRUE;
}

BOOL  TMonsterSkillElementInfo::CheckAttackTime()
{
	return (this->m_iSkillElementAttack <= 0)?FALSE:TRUE;
}

BOOL  TMonsterSkillElementInfo::CheckAutoHPTime()
{
	return (this->m_iSkillElementAutoHPTime <= 0)?FALSE:TRUE;
}

BOOL  TMonsterSkillElementInfo::CheckAutoMPTime()
{
	return (this->m_iSkillElementAutoMPTime <= 0)?FALSE:TRUE;
}

BOOL  TMonsterSkillElementInfo::CheckAutoAGTime()
{
	return (this->m_iSkillElementAutoAGTime <= 0)?FALSE:TRUE;
}

BOOL  TMonsterSkillElementInfo::CheckImmuneTime()
{
	return (this->m_iSkillElementImmuneTime <= 0)?FALSE:TRUE;
}

BOOL  TMonsterSkillElementInfo::CheckResistTime()
{
	return (this->m_iSkillElementResistTime <= 0)?FALSE:TRUE;
}

BOOL  TMonsterSkillElementInfo::CheckModifyStatTime()
{
	return (this->m_iSkillElementModifyStatTime <= 0)?FALSE:TRUE;
}

BOOL  TMonsterSkillElementInfo::CheckImmuneAllTime()
{
	return (this->m_iSkillElementImmuneAllTime <= 0) ? FALSE : TRUE;
}
////////////////////////////////////////////////////////////////

BOOL  TMonsterSkillElementInfo::DecDefenseTime()
{
	this->m_iSkillElementDefense--;
	return (this->m_iSkillElementDefense > 0)?FALSE:TRUE;
}

BOOL  TMonsterSkillElementInfo::DecAttackTime()
{
	this->m_iSkillElementAttack--;
	return (this->m_iSkillElementAttack > 0)?FALSE:TRUE;
}

BOOL  TMonsterSkillElementInfo::DecAutoHPTime()
{
	this->m_iSkillElementAutoHPTime--;
	return (this->m_iSkillElementAutoHPTime > 0)?FALSE:TRUE;
}

BOOL  TMonsterSkillElementInfo::DecAutoMPTime()
{
	this->m_iSkillElementAutoMPTime--;
	return (this->m_iSkillElementAutoMPTime > 0)?FALSE:TRUE;
}

BOOL  TMonsterSkillElementInfo::DecAutoAGTime()
{
	this->m_iSkillElementAutoAGTime--;
	return (this->m_iSkillElementAutoAGTime > 0)?FALSE:TRUE;
}

BOOL  TMonsterSkillElementInfo::DecImmuneTime()
{
	this->m_iSkillElementImmuneTime--;
	return (this->m_iSkillElementImmuneTime > 0)?FALSE:TRUE;
}

BOOL  TMonsterSkillElementInfo::DecResistTime()
{
	this->m_iSkillElementResistTime--;
	return (this->m_iSkillElementResistTime > 0)?FALSE:TRUE;
}

BOOL  TMonsterSkillElementInfo::DecModifyStatTime()
{
	this->m_iSkillElementModifyStatTime--;
	return (this->m_iSkillElementModifyStatTime > 0)?FALSE:TRUE;
}

BOOL  TMonsterSkillElementInfo::DecImmuneAllTime()
{
	this->m_iSkillElementImmuneAllTime--;
	return (this->m_iSkillElementImmuneAllTime > 0) ? FALSE : TRUE;
}



//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


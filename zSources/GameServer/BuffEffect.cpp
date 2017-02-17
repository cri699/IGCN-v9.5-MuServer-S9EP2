// BuffEffect.cpp: implementation of the CBuffEffect class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "User.h"
#include "BuffEffect.h"
#include "BuffEffectSlot.h"
#include "Prodef.H"
#include "Winutil.h"
#include "Protocol.h"
#include "GameMain.h"
#include "PeriodItemEx.h"
#include "TLog.h"
#include "ArcaBattle.h"
#include "configread.h"
#include "ObjUseSkill.h"

CBuffEffect	g_BuffEffect;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBuffEffect::CBuffEffect()
{

}

CBuffEffect::~CBuffEffect()
{

}

void CBuffEffect::SetBuffEffect(LPOBJECTSTRUCT lpObj, BYTE EffectType, int EffectValue)
{
	if(lpObj == NULL || EffectType < EFFECTTYPE_NONE)	return;

	if(lpObj->Connected < PLAYER_PLAYING)	return;

	switch(EffectType)
	{
	case EFFECTTYPE_ATTACKSPEED:
		lpObj->m_AttackSpeed += EffectValue;
		lpObj->m_MagicSpeed += EffectValue;
		break;
	case EFFECTTYPE_IMPROVE_DEFENSE:
		lpObj->m_Defense += EffectValue;
		lpObj->m_MagicDefense += EffectValue;
		break;
	case EFFECTTYPE_HP:
		lpObj->AddLife += EffectValue;	
		GSProtocol.GCReFillSend(lpObj->m_Index, lpObj->AddLife+lpObj->MaxLife, 0xFE, 0, lpObj->iAddShield+lpObj->iMaxShield);
		GSProtocol.GCReFillSend(lpObj->m_Index, lpObj->Life, 0xFF, 0, lpObj->iShield);
		break;
	case EFFECTTYPE_MANA:
		lpObj->AddMana += EffectValue;	
		GSProtocol.GCManaSend(lpObj->m_Index, (lpObj->MaxMana + lpObj->AddMana), 0xFE, 0, (lpObj->MaxBP+lpObj->AddBP));
		GSProtocol.GCManaSend(lpObj->m_Index, lpObj->Mana, 0xFF, 0, lpObj->BP);
		break;
	case EFFECTTYPE_STRENGTH:
		lpObj->AddStrength += EffectValue;
		break;
	case EFFECTTYPE_DEXTERITY:
		EnterCriticalSection(&lpObj->m_PlayerData->AgiCheckCriti);
		lpObj->m_PlayerData->AgilityCheckDelay = GetTickCount();
		lpObj->AddDexterity += EffectValue;
		LeaveCriticalSection(&lpObj->m_PlayerData->AgiCheckCriti);
		break;
	case EFFECTTYPE_VITALITY:
		lpObj->AddVitality += EffectValue;
		break;
	case EFFECTTYPE_ENERGY:
		lpObj->AddEnergy += EffectValue;
		break;
	case EFFECTTYPE_LEADERSHIP:
		lpObj->AddLeadership += EffectValue;
		break;
	case EFFECTTYPE_MELEEDAMAGE:
		lpObj->m_AttackDamageMaxLeft += EffectValue;
		lpObj->m_AttackDamageMinLeft += EffectValue;
		lpObj->m_AttackDamageMaxRight += EffectValue;
		lpObj->m_AttackDamageMinRight += EffectValue;
		break;
	case EFFECTTYPE_MAGICDAMAGE:
		lpObj->m_MagicDamageMin += EffectValue;
		lpObj->m_MagicDamageMax += EffectValue;
		break;
	case EFFECTTYPE_IMPROVE_MELEE_DEFENSE:
		lpObj->m_SkillInfo.SoulBarrierDefence = EffectValue;
		break;
	case EFFECTTYPE_IMPROVE_MAGIC_DEFENSE:
		lpObj->m_MagicDefense += EffectValue;
		break;
	case EFFECTTYPE_DAMAGEREFLECT:
		lpObj->DamageReflect += EffectValue;
		break;
	case EFFECTTYPE_REDUCE_ATTACK_RATE:
		lpObj->m_AttackRating -= EffectValue;
		break;
	case EFFECTTYPE_MELEE_DEFENSE_DOWN_MANA:
		lpObj->m_SkillInfo.SoulBarrierManaRate = EffectValue;
		break;
	case EFFECTTYPE_BERSERKER_UP:
		lpObj->AddMana += (int)( (((float)(EffectValue) * (float)(lpObj->MaxMana))) / 100.0f);		
		GSProtocol.GCManaSend(lpObj->m_Index, (lpObj->MaxMana + lpObj->AddMana), 0xFE, 0, (lpObj->MaxBP+lpObj->AddBP));
		GSProtocol.GCManaSend(lpObj->m_Index, lpObj->Mana, 0xFF, 0, lpObj->BP);
		break;
	case EFFECTTYPE_BERSERKER_DOWN:
		{
			float fAddLife = 0;
			float fPer = (((float)(40.0f) - (float)(EffectValue))/100.0f);

			fPer = (fPer > 0.1f)?fPer:0.1f;
			fAddLife = fPer * lpObj->MaxLife;

			lpObj->AddLife -= (int)((float)(fAddLife));
			lpObj->Life = ( (float)(lpObj->Life) < ( (float)(lpObj->AddLife) + (float)(lpObj->MaxLife) ) )?( (float)(lpObj->Life) ):( ( (float)(lpObj->AddLife) + (float)(lpObj->MaxLife) ) );

			GSProtocol.GCReFillSend(lpObj->m_Index, lpObj->AddLife+lpObj->MaxLife, 0xFE, 0, lpObj->iAddShield+lpObj->iMaxShield);
			GSProtocol.GCReFillSend(lpObj->m_Index, lpObj->Life, 0xFF, 0, lpObj->iShield);
		}
		break;
	case EFFECTTYPE_MAGICPOWER_INC:
		lpObj->m_MagicDamageMin += EffectValue;
		break;
	case EFFECTTYPE_MAGICPOWERMAX_INC:
		lpObj->m_MagicDamageMax += EffectValue;
		break;
	case EFFECTTYPE_POWER_UP:
		lpObj->m_AttackDamageMaxLeft += EffectValue;
		lpObj->m_AttackDamageMinLeft += EffectValue;
		lpObj->m_AttackDamageMaxRight += EffectValue;
		lpObj->m_AttackDamageMinRight += EffectValue;

		lpObj->m_MagicDamageMin += EffectValue;
		lpObj->m_MagicDamageMax += EffectValue;

		lpObj->m_CurseDamageMin += EffectValue;
		lpObj->m_CurseDamageMax += EffectValue;
		break;
	case EFFECTTYPE_GUARD_UP:
		lpObj->m_Defense += EffectValue;
		break;
	case EFFECTTYPE_AG_UP:
		lpObj->AddBP += EffectValue * (lpObj->Level + lpObj->m_PlayerData->MasterLevel);
		GSProtocol.GCManaSend(lpObj->m_Index, (WORD)(lpObj->MaxMana + lpObj->AddMana), 0xFE, 0, (WORD)(lpObj->MaxBP+lpObj->AddBP));
		GSProtocol.GCManaSend(lpObj->m_Index, (WORD)lpObj->Mana, 0xFF, 0, (WORD)lpObj->BP);
		break;
	case EFFECTTYPE_SD_UP:
		lpObj->iAddShield += EffectValue * (lpObj->Level + lpObj->m_PlayerData->MasterLevel);
		GSProtocol.GCReFillSend(lpObj->m_Index, lpObj->AddLife+lpObj->MaxLife, 0xFE, 0, lpObj->iAddShield+lpObj->iMaxShield);
		GSProtocol.GCReFillSend(lpObj->m_Index, lpObj->Life, 0xFF, 0, lpObj->iShield);
		break;
	case EFFECTTYPE_SD_UP_VALUE:
		lpObj->iAddShield += EffectValue;
		GSProtocol.GCReFillSend(lpObj->m_Index, lpObj->AddLife+lpObj->MaxLife, 0xFE, 0, lpObj->iAddShield+lpObj->iMaxShield);
		GSProtocol.GCReFillSend(lpObj->m_Index, lpObj->Life, 0xFF, 0, lpObj->iShield);
		break;
	case EFFECTTYPE_AG_UP_VALUE:
		lpObj->AddBP += EffectValue;
		GSProtocol.GCManaSend(lpObj->m_Index, (WORD)(lpObj->MaxMana + lpObj->AddMana), 0xFE, 0, (WORD)(lpObj->MaxBP+lpObj->AddBP));
		GSProtocol.GCManaSend(lpObj->m_Index, (WORD)lpObj->Mana, 0xFF, 0, (WORD)lpObj->BP);
		break;
	case EFFECTTYPE_IMPROVE_DEFENSE_RATE:
		lpObj->m_SuccessfulBlocking += EffectValue;
		break;
	case EFFECTTYPE_DECREASE_DEFENSE_RATE:
		lpObj->m_SuccessfulBlocking -= EffectValue;
		if(lpObj->m_SuccessfulBlocking < 0)
			lpObj->m_SuccessfulBlocking = 0;
		break;
	case EFFECTTYPE_BLIND:
		lpObj->m_bBlind = true;
		break;
	case EFFECTTYPE_WRATH_INC_DAMAGE:
		lpObj->m_AttackDamageMinLeft += lpObj->m_AttackDamageMinLeft * EffectValue / 100;
		lpObj->m_AttackDamageMaxLeft += lpObj->m_AttackDamageMaxLeft * EffectValue / 100;
		lpObj->m_AttackDamageMinRight += lpObj->m_AttackDamageMinRight * EffectValue / 100;
		lpObj->m_AttackDamageMaxRight += lpObj->m_AttackDamageMaxRight * EffectValue / 100;
		break;
	}
}

void CBuffEffect::ClearBuffEffect(LPOBJECTSTRUCT lpObj, BYTE EffectType, int EffectValue)
{
	if(lpObj == NULL || EffectType < EFFECTTYPE_NONE)	return;

	if(lpObj->Connected < PLAYER_PLAYING)	return;

	switch(EffectType)
	{
	case EFFECTTYPE_ATTACKSPEED:
		lpObj->m_AttackSpeed -= EffectValue;
		lpObj->m_MagicSpeed -= EffectValue;
		break;
	case EFFECTTYPE_IMPROVE_DEFENSE:
		lpObj->m_Defense -= EffectValue;
		lpObj->m_MagicDefense -= EffectValue;
		break;
	case EFFECTTYPE_HP:
		lpObj->AddLife -= EffectValue;
		if(lpObj->AddLife <= 0.0)
			lpObj->AddLife = 0.0;
		GSProtocol.GCReFillSend(lpObj->m_Index, lpObj->AddLife+lpObj->MaxLife, 0xFE, 0, lpObj->iAddShield+lpObj->iMaxShield);
		GSProtocol.GCReFillSend(lpObj->m_Index, lpObj->Life, 0xFF, 0, lpObj->iShield);
		break;
	case EFFECTTYPE_MANA:
		lpObj->AddMana -= EffectValue;
		if(lpObj->AddMana <= 0.0)
			lpObj->AddMana = 0.0;
		GSProtocol.GCManaSend(lpObj->m_Index, (WORD)(lpObj->MaxMana + lpObj->AddMana), 0xFE, 0, (WORD)(lpObj->MaxBP+lpObj->AddBP));
		GSProtocol.GCManaSend(lpObj->m_Index, (WORD)lpObj->Mana, 0xFF, 0, (WORD)lpObj->BP);
		break;
	case EFFECTTYPE_STRENGTH:
		lpObj->AddStrength -= EffectValue;
		break;
	case EFFECTTYPE_DEXTERITY:
		EnterCriticalSection(&lpObj->m_PlayerData->AgiCheckCriti);
		lpObj->m_PlayerData->AgilityCheckDelay = GetTickCount();
		lpObj->AddDexterity -= EffectValue;
		LeaveCriticalSection(&lpObj->m_PlayerData->AgiCheckCriti);
		break;
	case EFFECTTYPE_VITALITY:
		lpObj->AddVitality -= EffectValue;
		break;
	case EFFECTTYPE_ENERGY:
		lpObj->AddEnergy -= EffectValue;
		break;
	case EFFECTTYPE_LEADERSHIP:
		lpObj->AddLeadership -= EffectValue;
		break;
	case EFFECTTYPE_MELEEDAMAGE:
		lpObj->m_AttackDamageMaxLeft -= EffectValue;
		lpObj->m_AttackDamageMinLeft -= EffectValue;
		lpObj->m_AttackDamageMaxRight -= EffectValue;
		lpObj->m_AttackDamageMinRight -= EffectValue;
		break;
	case EFFECTTYPE_MAGICDAMAGE:
		lpObj->m_MagicDamageMin -= EffectValue;
		lpObj->m_MagicDamageMax -= EffectValue;
		break;
	case EFFECTTYPE_IMPROVE_MELEE_DEFENSE:
		lpObj->m_SkillInfo.SoulBarrierDefence -= EffectValue;
		break;
	case EFFECTTYPE_IMPROVE_MAGIC_DEFENSE:
		lpObj->m_MagicDefense -= EffectValue;
		break;
	case EFFECTTYPE_DAMAGEREFLECT:
		lpObj->DamageReflect -= EffectValue;
		break;
	case EFFECTTYPE_REDUCE_ATTACK_RATE:
		lpObj->m_AttackRating += EffectValue;
		break;
	case EFFECTTYPE_MELEE_DEFENSE_DOWN_MANA:
		lpObj->m_SkillInfo.SoulBarrierManaRate = 0;
		break;
	case EFFECTTYPE_BERSERKER_UP:
		lpObj->AddMana -= (int)( (((float)(EffectValue) * (float)(lpObj->MaxMana))) / 100.0f);
		if(lpObj->AddMana <= 0.0)
			lpObj->AddMana = 0.0;
		lpObj->Mana = ( (float)(lpObj->Mana) < ( (float)(lpObj->AddMana) + (float)(lpObj->MaxMana) ) )?( (float)(lpObj->Mana) ):( ( (float)(lpObj->AddMana) + (float)(lpObj->MaxMana) ) );
		
		GSProtocol.GCManaSend(lpObj->m_Index, (WORD)(lpObj->MaxMana + lpObj->AddMana), 0xFE, 0, (WORD)(lpObj->MaxBP+lpObj->AddBP));
		GSProtocol.GCManaSend(lpObj->m_Index, (WORD)lpObj->Mana, 0xFF, 0, (WORD)lpObj->BP);
		break;
	case EFFECTTYPE_BERSERKER_DOWN:
		{
			float fAddLife = 0;
			float fPer = (((float)(40.0f) - (float)(EffectValue))/100.0f);

			fPer = (fPer > 0.1f)?fPer:0.1f;
			fAddLife = fPer * lpObj->MaxLife;

			lpObj->AddLife += (int)(fAddLife);

			GSProtocol.GCReFillSend(lpObj->m_Index, lpObj->AddLife+lpObj->MaxLife, 0xFE, 0, lpObj->iAddShield+lpObj->iMaxShield);
			GSProtocol.GCReFillSend(lpObj->m_Index, lpObj->Life, 0xFF, 0, lpObj->iShield);
		}
		break;
	case EFFECTTYPE_MAGICPOWER_INC:
		lpObj->m_MagicDamageMin -= EffectValue;
		break;
	case EFFECTTYPE_MAGICPOWERMAX_INC:
		lpObj->m_MagicDamageMax -= EffectValue;
		lpObj->m_PlayerData->m_MPSkillOpt.iMpsCriticalRateInc = 0.0;
		break;
	case EFFECTTYPE_POWER_UP:
		lpObj->m_AttackDamageMaxLeft -= EffectValue;
		lpObj->m_AttackDamageMinLeft -= EffectValue;
		lpObj->m_AttackDamageMaxRight -= EffectValue;
		lpObj->m_AttackDamageMinRight -= EffectValue;

		lpObj->m_MagicDamageMin -= EffectValue;
		lpObj->m_MagicDamageMax -= EffectValue;

		lpObj->m_CurseDamageMin -= EffectValue;
		lpObj->m_CurseDamageMax -= EffectValue;
		break;
	case EFFECTTYPE_GUARD_UP:
		lpObj->m_Defense -= EffectValue;
		break;
	case EFFECTTYPE_AG_UP:
		lpObj->AddBP -= EffectValue * (lpObj->Level + lpObj->m_PlayerData->MasterLevel);
		GSProtocol.GCManaSend(lpObj->m_Index, (WORD)(lpObj->MaxMana + lpObj->AddMana), 0xFE, 0, (WORD)(lpObj->MaxBP+lpObj->AddBP));
		GSProtocol.GCManaSend(lpObj->m_Index, (WORD)lpObj->Mana, 0xFF, 0, (WORD)lpObj->BP);
		break;
	case EFFECTTYPE_SD_UP:
		lpObj->iAddShield -= EffectValue * (lpObj->Level + lpObj->m_PlayerData->MasterLevel);
		GSProtocol.GCReFillSend(lpObj->m_Index, lpObj->AddLife+lpObj->MaxLife, 0xFE, 0, lpObj->iAddShield+lpObj->iMaxShield);
		GSProtocol.GCReFillSend(lpObj->m_Index, lpObj->Life, 0xFF, 0, lpObj->iShield);
		break;
	case EFFECTTYPE_SD_UP_VALUE:
		lpObj->iAddShield -= EffectValue;
		GSProtocol.GCReFillSend(lpObj->m_Index, lpObj->AddLife+lpObj->MaxLife, 0xFE, 0, lpObj->iAddShield+lpObj->iMaxShield);
		GSProtocol.GCReFillSend(lpObj->m_Index, lpObj->Life, 0xFF, 0, lpObj->iShield);
		break;
	case EFFECTTYPE_AG_UP_VALUE:
		lpObj->AddBP -= EffectValue;
		GSProtocol.GCManaSend(lpObj->m_Index, (WORD)(lpObj->MaxMana + lpObj->AddMana), 0xFE, 0, (WORD)(lpObj->MaxBP+lpObj->AddBP));
		GSProtocol.GCManaSend(lpObj->m_Index, (WORD)lpObj->Mana, 0xFF, 0, (WORD)lpObj->BP);
		break;
	case EFFECTTYPE_IMPROVE_DEFENSE_RATE:
		lpObj->m_SuccessfulBlocking -= EffectValue;
		break;
	case EFFECTTYPE_DECREASE_DEFENSE_RATE:
		lpObj->m_SuccessfulBlocking += EffectValue;
		break;
	case EFFECTTYPE_ELF_BLESS:
		{
			PMSG_USE_STAT_FRUIT pMsg;
			PHeadSetB((LPBYTE)&pMsg, 0x2C, sizeof(pMsg));
			pMsg.result = 0x12;
			pMsg.btStatValue = EffectValue;
			pMsg.btFruitType = 0x07;
			IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		}
		break;
	case EFFECTTYPE_BLIND:
		lpObj->m_bBlind = false;
		break;
	case EFFECTTYPE_WRATH_INC_DAMAGE:
		lpObj->m_AttackDamageMinLeft -= lpObj->m_AttackDamageMinLeft * EffectValue / 100;
		lpObj->m_AttackDamageMaxLeft -= lpObj->m_AttackDamageMaxLeft * EffectValue / 100;
		lpObj->m_AttackDamageMinRight -= lpObj->m_AttackDamageMinRight * EffectValue / 100;
		lpObj->m_AttackDamageMaxRight -= lpObj->m_AttackDamageMaxRight * EffectValue / 100;
		break;
	}
}

void CBuffEffect::SetActiveBuffEffect(class OBJECTSTRUCT* lpObj, BYTE EffectType, int EffectValue)
{
	if(EffectType <= EFFECTTYPE_NONE)	return;

	switch(EffectType)
	{
		case EFFECTTYPE_GIVE_DMG_TICK:		GiveDamageEffect(lpObj, EffectValue);			break;
		case EFFECTTYPE_POISON_DMG_TICK:	PoisonEffect(lpObj, EffectValue);				break;
		case EFFECTTYPE_FILLHP:				GiveDamageFillHPEffect(lpObj, EffectValue);		break;
	}
}

void CBuffEffect::GiveDamageEffect(class OBJECTSTRUCT* lpObj, int Damage)
{
	int DecreaseHealthPoint = 0;
	int DecreaseShiledPoint = 0;

	DecreaseHealthPoint = Damage;
	DecreaseShiledPoint = DecreaseHealthPoint;

	if(lpObj->Live == 0)	return;

	if( lpObj->lpAttackObj != NULL && 
		lpObj->Type == OBJ_USER && lpObj->m_bOffLevel == FALSE && lpObj->lpAttackObj->Type == OBJ_USER)
	{
		DecreaseShiledPoint = 90 * DecreaseHealthPoint / 100;
		DecreaseHealthPoint -= DecreaseShiledPoint;

		if(lpObj->iShield-DecreaseShiledPoint > 0)
		{
			lpObj->iShield -= DecreaseShiledPoint;
			lpObj->Life -= DecreaseHealthPoint;
		}
		else
		{
			DecreaseHealthPoint += DecreaseShiledPoint - lpObj->iShield;
			DecreaseShiledPoint = lpObj->iShield;
			lpObj->Life -= DecreaseHealthPoint;
			lpObj->iShield = 0;
		}
	}
	else
	{
		DecreaseShiledPoint = 0;
		lpObj->Life -= DecreaseHealthPoint;
	}

	if(lpObj->Life < 0.0f)
	{
		lpObj->Life = 0.0f;
	}

	if(lpObj->lpAttackObj != NULL)
	{
		gObjLifeCheck(lpObj, lpObj->lpAttackObj, DecreaseHealthPoint, 3, 0, 0, 0, DecreaseShiledPoint, 0);
	}
}

void CBuffEffect::PoisonEffect(LPOBJECTSTRUCT lpObj, BYTE PoisonRate)
{
	int DecreaseHealthPoint = 0;
	int DecreaseShiledPoint = 0;

	if (g_ConfigRead.data.common.IsJoinMu == 1)
	{
		gObjUseSkill.m_Lua.Generic_Call("PoisonEffectCalc", "d>i", lpObj->Life, &DecreaseHealthPoint);
	}
	else
	{
		DecreaseHealthPoint = (((int)(PoisonRate)* lpObj->Life) / 100);
	}

	DecreaseShiledPoint = DecreaseHealthPoint;

	if(lpObj->lpAttackObj != NULL && 
		lpObj->Type == OBJ_USER && lpObj->m_bOffLevel == FALSE && lpObj->lpAttackObj->Type == OBJ_USER)
	{
		if(lpObj->iShield-DecreaseShiledPoint > 0)
		{
			lpObj->iShield -= DecreaseShiledPoint;
			DecreaseHealthPoint = 0;
		}
		else
		{
			DecreaseHealthPoint = DecreaseShiledPoint - lpObj->iShield;
			DecreaseShiledPoint = lpObj->iShield;
			lpObj->Life -= DecreaseHealthPoint;
			lpObj->iShield = 0;
		}
	}
	else
	{
		lpObj->Life -= DecreaseHealthPoint;
		DecreaseShiledPoint = 0;
	}
	
	if(lpObj->Life < 0.0f)
	{
		lpObj->Life = 0.0f;
	}

	if(lpObj->lpAttackObj != NULL)
	{
		gObjLifeCheck(lpObj, lpObj->lpAttackObj, DecreaseHealthPoint, 2, 0, 0, 1, DecreaseShiledPoint, 0);
	}
}

void CBuffEffect::GiveDamageFillHPEffect(LPOBJECTSTRUCT lpObj, int Damage)
{
	int DecreaseHealthPoint = 0;
	int DecreaseShiledPoint = 0;

	DecreaseHealthPoint = Damage;
	DecreaseShiledPoint = DecreaseHealthPoint;

	if (lpObj->Live == 0)	return;

	if (lpObj->lpAttackObj != NULL &&
		lpObj->Type == OBJ_USER && lpObj->lpAttackObj->Type == OBJ_USER)
	{
		DecreaseShiledPoint = 90 * DecreaseHealthPoint / 100;
		DecreaseHealthPoint -= DecreaseShiledPoint;

		if (lpObj->iShield - DecreaseShiledPoint > 0)
		{
			lpObj->iShield -= DecreaseShiledPoint;
			lpObj->Life -= DecreaseHealthPoint;
		}
		else
		{
			DecreaseHealthPoint += DecreaseShiledPoint - lpObj->iShield;
			DecreaseShiledPoint = lpObj->iShield;
			lpObj->Life -= DecreaseHealthPoint;
			lpObj->iShield = 0;
		}
	}
	else
	{
		DecreaseShiledPoint = 0;
		lpObj->Life -= DecreaseHealthPoint;
	}

	if (lpObj->Life < 0.0f)
	{
		lpObj->Life = 0.0f;
	}

	if (lpObj->lpAttackObj != NULL)
	{
		gObjLifeCheck(lpObj, lpObj->lpAttackObj, DecreaseHealthPoint, 3, 0, 0, 0, DecreaseShiledPoint, 0);
	}

	if (gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_BLEEDING) == true)
	{
		int value1 = 0, value2 = 0;
		gObjGetValueOfBuffIndex(lpObj, BUFFTYPE_BLEEDING, &value1, &value2);

		if (ObjectMaxRange(value2) == false)	return;

		LPOBJ lpTarget = &gObj[value2];

		if ((lpObj->AddLife + lpObj->MaxLife) >= (lpObj->Life+DecreaseHealthPoint))
		{
			lpObj->Life += DecreaseHealthPoint;
		}

		else
		{
			lpObj->Life = lpObj->AddLife + lpObj->MaxLife;
		}

		GSProtocol.GCReFillSend(lpObj->m_Index, lpObj->Life, 0xFF, 0, lpObj->iShield);
	}
}

void CBuffEffect::SetPrevEffect(LPOBJECTSTRUCT lpObj)
{
	if(lpObj == NULL)	return;

	int BuffCount = 0;

	for(int i = 0; i < MAX_BUFFEFFECT; i++)
	{
		if(lpObj->m_BuffEffectList[i].BuffIndex == BUFFTYPE_NONE) continue;

		switch(lpObj->m_BuffEffectList[i].EffectType1)
		{
		case EFFECTTYPE_HP:
		case EFFECTTYPE_MANA:
		case EFFECTTYPE_STRENGTH:
		case EFFECTTYPE_DEXTERITY:
		case EFFECTTYPE_VITALITY:
		case EFFECTTYPE_ENERGY:
		case EFFECTTYPE_LEADERSHIP:
		case EFFECTTYPE_DAMAGEREFLECT:
		case EFFECTTYPE_SD_UP:
		case EFFECTTYPE_AG_UP:
		case EFFECTTYPE_SD_UP_VALUE:
		case EFFECTTYPE_AG_UP_VALUE:
			BuffCount++;
			SetBuffEffect(lpObj, lpObj->m_BuffEffectList[i].EffectType1, lpObj->m_BuffEffectList[i].EffectValue1);
			break;
		default:
			break;
		}

		switch(lpObj->m_BuffEffectList[i].EffectType2)
		{
		case EFFECTTYPE_HP:
		case EFFECTTYPE_MANA:
		case EFFECTTYPE_STRENGTH:
		case EFFECTTYPE_DEXTERITY:
		case EFFECTTYPE_VITALITY:
		case EFFECTTYPE_ENERGY:
		case EFFECTTYPE_LEADERSHIP:
		case EFFECTTYPE_DAMAGEREFLECT:
		case EFFECTTYPE_SD_UP:
		case EFFECTTYPE_AG_UP:
		case EFFECTTYPE_SD_UP_VALUE:
		case EFFECTTYPE_AG_UP_VALUE:
			BuffCount++;
			SetBuffEffect(lpObj, lpObj->m_BuffEffectList[i].EffectType2, lpObj->m_BuffEffectList[i].EffectValue2);
			break;
		default:
			break;
		}
	}
}

void CBuffEffect::SetNextEffect(LPOBJECTSTRUCT lpObj)
{
	if(lpObj == NULL)	return;

	int BuffCount = 0;

	for(int i = 0; i < MAX_BUFFEFFECT; i++)
	{
		if(lpObj->m_BuffEffectList[i].BuffIndex == BUFFTYPE_NONE) continue;

		switch(lpObj->m_BuffEffectList[i].EffectType1)
		{
		case EFFECTTYPE_HP:
		case EFFECTTYPE_MANA:
		case EFFECTTYPE_STRENGTH:
		case EFFECTTYPE_DEXTERITY:
		case EFFECTTYPE_VITALITY:
		case EFFECTTYPE_ENERGY:
		case EFFECTTYPE_LEADERSHIP:
		case EFFECTTYPE_DAMAGEREFLECT:
		case EFFECTTYPE_SD_UP:
		case EFFECTTYPE_AG_UP:
		case EFFECTTYPE_SD_UP_VALUE:
		case EFFECTTYPE_AG_UP_VALUE:
			break;
		default:
			BuffCount++;
			SetBuffEffect(lpObj, lpObj->m_BuffEffectList[i].EffectType1, lpObj->m_BuffEffectList[i].EffectValue1);
			break;
		}

		switch(lpObj->m_BuffEffectList[i].EffectType2)
		{
		case EFFECTTYPE_HP:
		case EFFECTTYPE_MANA:
		case EFFECTTYPE_STRENGTH:
		case EFFECTTYPE_DEXTERITY:
		case EFFECTTYPE_VITALITY:
		case EFFECTTYPE_ENERGY:
		case EFFECTTYPE_LEADERSHIP:
		case EFFECTTYPE_DAMAGEREFLECT:
		case EFFECTTYPE_SD_UP:
		case EFFECTTYPE_AG_UP:
		case EFFECTTYPE_SD_UP_VALUE:
		case EFFECTTYPE_AG_UP_VALUE:
			break;
		default:
			BuffCount++;
			SetBuffEffect(lpObj, lpObj->m_BuffEffectList[i].EffectType2, lpObj->m_BuffEffectList[i].EffectValue2);
			break;
		}
	}
}

void CBuffEffect::ClearPrevEffect(LPOBJECTSTRUCT lpObj)
{
	if(lpObj == NULL)	return;

	int BuffCount = 0;

	for(int i = 0; i < MAX_BUFFEFFECT; i++)
	{
		if(lpObj->m_BuffEffectList[i].BuffIndex == BUFFTYPE_NONE) continue;

		switch(lpObj->m_BuffEffectList[i].EffectType1)
		{
		case EFFECTTYPE_HP:
		case EFFECTTYPE_MANA:
		case EFFECTTYPE_STRENGTH:
		case EFFECTTYPE_DEXTERITY:
		case EFFECTTYPE_VITALITY:
		case EFFECTTYPE_ENERGY:
		case EFFECTTYPE_LEADERSHIP:
		case EFFECTTYPE_DAMAGEREFLECT:
		case EFFECTTYPE_SD_UP:
		case EFFECTTYPE_AG_UP:
		case EFFECTTYPE_SD_UP_VALUE:
		case EFFECTTYPE_AG_UP_VALUE:
			BuffCount++;
			ClearBuffEffect(lpObj, lpObj->m_BuffEffectList[i].EffectType1, lpObj->m_BuffEffectList[i].EffectValue1);
			break;
		default:
			break;
		}

		switch(lpObj->m_BuffEffectList[i].EffectType2)
		{
		case EFFECTTYPE_HP:
		case EFFECTTYPE_MANA:
		case EFFECTTYPE_STRENGTH:
		case EFFECTTYPE_DEXTERITY:
		case EFFECTTYPE_VITALITY:
		case EFFECTTYPE_ENERGY:
		case EFFECTTYPE_LEADERSHIP:
		case EFFECTTYPE_DAMAGEREFLECT:
		case EFFECTTYPE_SD_UP:
		case EFFECTTYPE_AG_UP:
		case EFFECTTYPE_SD_UP_VALUE:
		case EFFECTTYPE_AG_UP_VALUE:
			BuffCount++;
			ClearBuffEffect(lpObj, lpObj->m_BuffEffectList[i].EffectType2, lpObj->m_BuffEffectList[i].EffectValue2);
			break;
		default:
			break;
		}
	}
}

struct PMSG_REQ_GUILD_PERIODBUFF_INSERT
{
	PBMSG_HEAD2 head;
	char szGuildName[MAX_GUILD_LEN+1];
	WORD wBuffIndex;
	char btEffectType1;
	char btEffectType2;
	DWORD dwDuration;
	time_t lExpireDate;
};

void CBuffEffect::RequestGuildPeriodBuffInsert(char *szGuildName, _tagPeriodBuffInfo *lpBuffInfo)
{
	PMSG_REQ_GUILD_PERIODBUFF_INSERT pMsg;

	memcpy(pMsg.szGuildName, szGuildName, MAX_GUILD_LEN+1);
	pMsg.wBuffIndex = lpBuffInfo->wBuffIndex;
	pMsg.btEffectType1 = lpBuffInfo->btEffectType1;
	pMsg.btEffectType2 = lpBuffInfo->btEffectType2;
	pMsg.dwDuration = lpBuffInfo->lDuration;
	pMsg.lExpireDate = g_PeriodItemEx.GetExpireDate(lpBuffInfo->lDuration);
	PHeadSubSetB((LPBYTE)&pMsg, 0x53, 1, sizeof(pMsg));
 
	wsExDbCli.DataSend((char*)&pMsg, pMsg.head.size);
  
	g_Log.Add("[PeriodBuff][Insert] Request Insert Guild PeriodBuff. GuildName : %s, BuffIndex : %d, Duration : %d, lExpireDate : %d",
		szGuildName, lpBuffInfo->wBuffIndex, lpBuffInfo->lDuration, pMsg.lExpireDate);
}

struct PMSG_REQ_GUILD_PERIODBUFF_DELETE
{
	PBMSG_HEAD2 head;
	char btGuildCnt;
	WORD wBuffIndex[5];
};

void CBuffEffect::RequestGuildPeriodBuffDelete(WORD *wBuffIndex, char btGuildCnt)
{
	PMSG_REQ_GUILD_PERIODBUFF_DELETE pMsg;

	for ( int i = 0; i < btGuildCnt; ++i )
		pMsg.wBuffIndex[i] = wBuffIndex[i];

	pMsg.btGuildCnt = btGuildCnt;
	PHeadSubSetB((LPBYTE)&pMsg, 0x53, 2, sizeof(pMsg));

	wsExDbCli.DataSend((char*)&pMsg, pMsg.head.size);
	g_Log.Add("[PeriodBuff][Delete] Request All Delete Guild PeriodBuff");
}

struct PMSG_REQ_PERIODBUFF_DELETE
{
	PBMSG_HEAD2 head;
	WORD wUserIndex;
	WORD wBuffIndex;
	char szCharacterName[11];
};

void CBuffEffect::RequestPeriodBuffDelete(OBJECTSTRUCT *lpObj, WORD wBuffIndex)
{
	PMSG_REQ_PERIODBUFF_DELETE pMsg;

	pMsg.wUserIndex = lpObj->m_Index;
	pMsg.wBuffIndex = wBuffIndex;
	memcpy(pMsg.szCharacterName, lpObj->Name, MAX_ACCOUNT_LEN+1);
	PHeadSubSetB((LPBYTE)&pMsg, 0xE4, 2, sizeof(pMsg));
  
	wsDataCli.DataSend((char*)&pMsg, pMsg.head.size);
	g_Log.Add("[PeriodBuff][Delete] Request Delete PeriodBuff. User Id : %s(%d), Name : %s, BuffIndex : %d", lpObj->AccountID, lpObj->DBNumber, lpObj->Name, wBuffIndex);
}

void CBuffEffect::RequestPeriodBuffDelete(char *szName, WORD wBuffIndex)
{
	PMSG_REQ_PERIODBUFF_DELETE pMsg;

	pMsg.wUserIndex = 0;
	pMsg.wBuffIndex = wBuffIndex;
	memcpy(pMsg.szCharacterName, szName, MAX_ACCOUNT_LEN + 1);
	PHeadSubSetB((LPBYTE)&pMsg, 0xE4, 2, sizeof(pMsg));

	wsDataCli.DataSend((char*)&pMsg, pMsg.head.size);
	g_Log.Add("[PeriodBuff][Delete] Request Delete PeriodBuff.Name : %s, BuffIndex : %d", szName, wBuffIndex);
}

struct PMSG_REQ_PERIODBUFF_INSERT
{
	PBMSG_HEAD2 head;
	WORD wUserIndex;
	char szCharacterName[MAX_ACCOUNT_LEN+1];
	WORD wBuffIndex;
	char btEffectType1;
	char btEffectType2;
	DWORD dwDuration;
	time_t lExpireDate;
};

void CBuffEffect::RequestPeriodBuffInsert(OBJECTSTRUCT *lpObj, _tagPeriodBuffInfo *lpBuffInfo)
{
	PMSG_REQ_PERIODBUFF_INSERT pMsg; 

	pMsg.wUserIndex = lpObj->m_Index;
	memcpy(pMsg.szCharacterName, lpObj->Name, MAX_ACCOUNT_LEN+1);
	pMsg.wBuffIndex = lpBuffInfo->wBuffIndex;
	pMsg.btEffectType1 = lpBuffInfo->btEffectType1;
	pMsg.btEffectType2 = lpBuffInfo->btEffectType2;
	pMsg.dwDuration = lpBuffInfo->lDuration;
	pMsg.lExpireDate = g_PeriodItemEx.GetExpireDate(lpBuffInfo->lDuration);
	PHeadSubSetB((LPBYTE)&pMsg, 0xE4, 1, sizeof(pMsg));

	wsDataCli.DataSend((char*)&pMsg, pMsg.head.size);
	g_Log.Add("[PeriodBuff][Insert] Request Insert PeriodBuff. User Id : %s(%d), Name : %s, BuffIndex : %d, Duration %d, lExpireDate%d",
	lpObj->AccountID, lpObj->DBNumber, lpObj->Name, lpBuffInfo->wBuffIndex, lpBuffInfo->lDuration, pMsg.lExpireDate);
}

struct PMSG_REQ_PERIODBUFF_SELECT
{
	PBMSG_HEAD2 head;
	WORD wUserIndex;
	char szCharacterName[MAX_ACCOUNT_LEN+1];
};

void CBuffEffect::RequestPeriodBuffSelect(OBJECTSTRUCT *lpObj)
{
	PMSG_REQ_PERIODBUFF_SELECT pMsg;

	pMsg.wUserIndex = lpObj->m_Index;
	memcpy(pMsg.szCharacterName, lpObj->Name, MAX_ACCOUNT_LEN+1);
	PHeadSubSetB((LPBYTE)&pMsg, 0xE4, 3, sizeof(pMsg));

	wsDataCli.DataSend((char*)&pMsg, pMsg.head.size);
}

void CBuffEffect::DGPeriodItemExSelect(PMSG_ANS_PERIODBUFF_SELECT *lpMsg)
{
	if (!ObjectMaxRange(lpMsg->wUserIndex))
	{
		return;
	}

	LPOBJ lpObj = &gObj[lpMsg->wUserIndex];

	if ( lpObj->Connected < PLAYER_LOGGED )
	{
		return;
	}

	if ( lpObj->Type != OBJ_USER )
	{
		return;
	}

	if ( g_ArcaBattle.IsArcaBattleServer() == FALSE && lpMsg->wBuffIndex == 184 )
	{
		return;
	}

	LPPERIOD_BUFF_EFFECT_INFO lpPeriBuff = g_BuffEffectSlot.GetPeriodBuffInfo(lpMsg->wBuffIndex);

	if ( !lpPeriBuff )
	{
		return;
	}

	time_t lLeftDate = g_PeriodItemEx.GetLeftDate(lpMsg->lExpireDate);

	if ( gObjAddPeriodBuffEffect(lpObj, lpPeriBuff, lLeftDate) == FALSE )
	{
		g_Log.Add("[PeriodBuff][Error][Select] Answer Select PeriodBuff. User Id : %s(%d), Name : %s, BuffIndex : %d Type1 : %d Type2 : %d ExpireDate : %d ResultCode : %d",
			lpObj->AccountID, lpObj->DBNumber, lpObj->Name, lpMsg->wBuffIndex, lpMsg->btEffectType1, lpMsg->btEffectType2, lpMsg->lExpireDate, lpMsg->btResultCode);
	}

	else
	{
		g_Log.Add("[PeriodBuff][Select] Answer Select PeriodBuff. User Id : %s(%d), Name : %s, BuffIndex : %d Type1 : %d Type2 : %d ExpireDate : %d ResultCode : %d",
			lpObj->AccountID, lpObj->DBNumber, lpObj->Name, lpMsg->wBuffIndex, lpMsg->btEffectType1, lpMsg->btEffectType2, lpMsg->lExpireDate, lpMsg->btResultCode);
	}
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


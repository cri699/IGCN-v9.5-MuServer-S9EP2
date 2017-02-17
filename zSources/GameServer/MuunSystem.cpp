//////////////////////////////////////////////////////////////////////
// MuunSystem.cpp
#include "StdAfx.h"
#include "protocol.h"
#include "TLog.h"
#include "DSProtocol.h"
#include "GameMain.h"
#include "ItemSocketOptionSystem.h"
#include "PentagramSystem.h"
#include "winutil.h"
#include "NewPVP.h"
#include "ObjCalCharacter.h"
#include "MuunSystem.h"
#include "configread.h"
#include "gObjMonster.h"
#include "ItemOptionTypeMng.h"
#include "GensSystem.h"
#include "LargeRand.h"
#include "BuffEffectSlot.h"
#include "ImperialGuardian.h"
#include "BattleSoccerManager.h"
#include "AcheronGuardianEvent.h"
#include "ObjAttack.h"
#include "ChaosCastle.h"
#include "OfflineLevelling.h"
#include "LastManStanding.h"

CMuunAttack::CMuunAttack()
{

}

CMuunAttack::~CMuunAttack()
{

}

void CMuunAttack::SendAttackMsg(int aIndex, int aTargetIndex, int SubCode, int SubCode2)
{
	if (SubCode == MUUN_DEC_ENEMY_ATTACK_SKILL)
	{
		aTargetIndex = aIndex;
	}

	if (SubCode == MUUN_ATTACK_SKILL || SubCode == MUUN_ATTACK_SKILL_2 || SubCode == MUUN_ATTACK_SKILL_NONPVP)
	{
		if (gObjCalDistance(&gObj[aIndex], &gObj[aTargetIndex]) > 4)
		{
			g_CMuunSystem.ReSetTarget(&gObj[aIndex], aTargetIndex);
			return;
		}

		if (gObj[aTargetIndex].Life <= 0.0)
		{
			g_CMuunSystem.ReSetTarget(&gObj[aIndex], aTargetIndex);
			return;
		}

		if (SubCode == MUUN_ATTACK_SKILL_NONPVP)
		{
			if (gObj[aTargetIndex].Type == OBJ_USER)
			{
				g_CMuunSystem.ReSetTarget(&gObj[aIndex], aTargetIndex);
				return;
			}
		}
	}

	if (SubCode != MUUN_DEC_ENEMY_ATTACK_SKILL)
	{
		PMSG_MUUN_ATTACK_COMMAND pMsg;

		pMsg.Slot = SubCode2;
		pMsg.SkillType = SubCode;
		pMsg.NumberH = HIBYTE(aIndex);
		pMsg.NumberL = LOBYTE(aIndex);
		pMsg.TargetNumberH = HIBYTE(aTargetIndex);
		pMsg.TargetNumberL = LOBYTE(aTargetIndex);

		pMsg.h.set((LPBYTE)&pMsg, 0x4E, 0x12, sizeof(pMsg));

		IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
		GSProtocol.MsgSendV2(&gObj[aIndex], (LPBYTE)&pMsg, pMsg.h.size);
	}

	gObjAddAttackProcMsgSendDelay(&gObj[aIndex], 63, aTargetIndex, 600, SubCode, SubCode2);
}

void CMuunAttack::SkillProc(LPOBJ lpObj)
{
	for (int i = 0; i < MAX_MUUN_EFFECT_LIST; i++)
	{
		if (lpObj->pMuunInventory[i].m_Durability > 0.0 &&
			lpObj->m_MuunEffectList[i].bOptEnable == true &&
			(lpObj->m_MuunEffectList[i].nOptType == MUUN_ATTACK_SKILL ||
			lpObj->m_MuunEffectList[i].nOptType == MUUN_DEC_ENEMY_ATTACK_SKILL ||
			lpObj->m_MuunEffectList[i].nOptType == MUUN_ATTACK_SKILL_2 ||
			lpObj->m_MuunEffectList[i].nOptType == MUUN_ATTACK_SKILL_NONPVP))
		{
			DWORD dwSkillDelayTime = lpObj->m_MuunEffectList[i].nSkillDelayTime;
			int nTargetIndex = lpObj->m_MuunEffectList[i].nTargetIndex;

			if (nTargetIndex >= 0 || (lpObj->m_MuunEffectList[i].nOptType != MUUN_ATTACK_SKILL && lpObj->m_MuunEffectList[i].nOptType != MUUN_ATTACK_SKILL_2 && lpObj->m_MuunEffectList[i].nOptType != MUUN_ATTACK_SKILL_NONPVP))
			{
				if (g_GensSystem.IsPkEnable(lpObj, &gObj[nTargetIndex]) == FALSE)
				{
					return;
				}

				if ((MapC[gObj[nTargetIndex].MapNumber].GetAttr(gObj[nTargetIndex].X, gObj[nTargetIndex].Y) & 1) == 1)
				{
					g_CMuunSystem.ReSetTarget(lpObj, nTargetIndex);
					return;
				}

				if ((MapC[lpObj->MapNumber].GetAttr(lpObj->X, lpObj->Y) & 1) == 1)
				{
					g_CMuunSystem.ReSetTarget(lpObj, nTargetIndex);
					return;
				}

				if (lpObj->pMuunInventory[i].IsMuunItemPeriodExpire() == FALSE && lpObj->m_MuunEffectList[i].nAddOptType == 3)
				{
					dwSkillDelayTime /= lpObj->m_MuunEffectList[i].nAddOptValue;
				}

				if (GetTickCount() - lpObj->m_MuunEffectList[i].nTickTime > dwSkillDelayTime)
				{
					lpObj->m_MuunEffectList[i].nTickTime = GetTickCount();
					this->SendAttackMsg(lpObj->m_Index, nTargetIndex, lpObj->m_MuunEffectList[i].nOptType, i);
				}
			}
		}
	}
}

bool CMuunAttack::DamageAbsorb(LPOBJ lpObj, LPOBJ lpTargetObj, CMagicInf *lpMagic, int SubCode2)
{
	lpObj->m_MuunEffectList[SubCode2].bSkillUsed = false;
	return true;
}

bool CMuunAttack::Stun(LPOBJ lpObj, LPOBJ lpTargetObj, CMagicInf *lpMagic, int SubCode2)
{
	int iValue = lpObj->m_MuunEffectList[SubCode2].nOptValue;
	int iStunRate = iValue;

	if (lpTargetObj->Type == OBJ_USER)
	{
		iStunRate -= lpTargetObj->m_PlayerData->m_Resistance_Stun;
	}

	if (GetLargeRand() % 1000000 <= iStunRate)
	{
		gObjAddBuffEffect(lpTargetObj, BUFFTYPE_STUN, 0, 0, 0, 0, 2);
		gObjSetPosition(lpTargetObj->m_Index, lpTargetObj->X, lpTargetObj->Y);
	}

	return true;
}

bool CMuunAttack::Attack(LPOBJ lpObj, LPOBJ lpTargetObj, CMagicInf *lpMagic, int SubCode2)
{
	int skillSuccess = 0;
	LPOBJ lpCallObj;
	LPOBJ lpCallTargetObj;
	int MsgDamage = 0;
	int ManaChange = 0;
	int iTempShieldDamage = 0;
	int iTotalShieldDamage = 0;

	if ((lpObj->Authority & 2) == 2 || (lpTargetObj->Authority & 2) == 2) //s4 add-on
	{
		return FALSE;
	}

	if ((lpObj->Authority & 32) == 32 || (lpTargetObj->Authority & 32) == 32) //s4 add-on
	{
		if (gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_INVISABLE) == TRUE)
		{
			return FALSE;
		}

		if (gObjCheckUsedBuffEffect(lpTargetObj, BUFFTYPE_INVISABLE) == TRUE)
		{
			return FALSE;
		}
	}

	if ( lpObj->MapNumber != lpTargetObj->MapNumber )
	{
		return FALSE;
	}

	if( lpTargetObj->m_bOffLevel == true && g_OffLevel.m_General.Immortal == 1)
	{
		return FALSE;
	}

	if ( IMPERIAL_MAP_RANGE(lpObj->MapNumber) == TRUE && g_ImperialGuardian.IsAttackAbleMonster(lpTargetObj->m_Index) == false )
	{
		return FALSE;
	}

	if (g_ConfigRead.server.GetServerType() == SERVER_CASTLE)
	{
		if (g_Crywolf.GetCrywolfState() == 3 || g_Crywolf.GetCrywolfState() == 5)
		{
			if (CRYWOLF_MAP_RANGE(lpTargetObj->MapNumber))
			{
				if (lpTargetObj->Type == OBJ_MONSTER)
				{
					return FALSE;
				}
			}
		}
	}

	int skill = 0;
	
	if ( lpMagic )
		skill = lpMagic->m_Skill;

	skillSuccess = TRUE;

	if ( lpObj->m_PlayerData->GuildNumber > 0 )
	{
		if ( lpObj->m_PlayerData->lpGuild )
		{
			if ( lpObj->m_PlayerData->lpGuild->WarState )
			{
				if ( lpObj->m_PlayerData->lpGuild->WarType == 1 )
				{
					if ( !GetBattleSoccerGoalMove(0) )
					{
						return TRUE;
					}
				}
			}
		}
	}

	if ( lpTargetObj->Type == OBJ_MONSTER )
	{
		if ( lpTargetObj->Class == 200 )
		{
			if ( skill )
			{
				gObjMonsterStateProc(lpTargetObj, 7, lpObj->m_Index, 0);
			}
			else
			{
				gObjMonsterStateProc(lpTargetObj, 6, lpObj->m_Index, 0);
			}

			return TRUE;
		}
	}

	if ( lpObj->Type == OBJ_USER )
	{
		if ( !gObjIsConnected(lpObj))
		{
			return FALSE;
		}
	}

	if ( lpTargetObj->Type == OBJ_USER )
	{
		if ( !gObjIsConnected(lpTargetObj))
		{
			return FALSE;
		}
	}

	if ( lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_MONSTER )	// PLAYER VS MONSTER
	{
		if ( lpObj->m_RecallMon >= 0 )
		{
			if ( lpObj->m_RecallMon == lpTargetObj->m_Index )
			{
				return FALSE;
			}
		}
	}

	if (g_ConfigRead.antihack.EnableAttackBlockInSafeZone == TRUE)
	{
		BYTE btAttr = MapC[lpObj->MapNumber].GetAttr(lpObj->X, lpObj->Y);

		if ((btAttr & 1) == 1)
		{
			return FALSE;
		}
	}

	if ( !gObjAttackQ(lpTargetObj))
	{
		return FALSE;
	}

	if (g_ArcaBattle.IsArcaBattleServer() == TRUE && g_AcheronGuardianEvent.IsPlayStart() == false && g_ArcaBattle.IsEnableAttackObelisk(lpObj, lpTargetObj->Class) == false)
	{
		return FALSE;
	}

	if (lpTargetObj->Type == OBJ_USER && g_NewPVP.IsDuel(*lpTargetObj) && g_NewPVP.IsSafeState(*lpTargetObj))
	{
		return FALSE;
	}

	if (lpObj->Type == OBJ_USER && g_NewPVP.IsDuel(*lpObj) && g_NewPVP.IsSafeState(*lpObj))
	{
		return FALSE;
	}

	if (lpTargetObj->Type == OBJ_USER && g_NewPVP.IsObserver(*lpTargetObj))
	{
		return FALSE;
	}

	if (lpObj->Type == OBJ_USER && g_NewPVP.IsObserver(*lpObj))
	{
		return FALSE;
	}

	if (g_ConfigRead.server.GetServerType() == SERVER_CASTLE)
	{
		if (g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE)
		{
			if (lpObj->m_btCsJoinSide > 0)
			{
				if (lpObj->m_btCsJoinSide == lpTargetObj->m_btCsJoinSide)
				{
					return FALSE;
				}
			}
		}
	}

	lpObj->m_TotalAttackCount++;

	if ( this->CheckAttackArea(lpObj, lpTargetObj) == FALSE )
	{
		return FALSE;
	}

	lpCallObj = lpObj;
	lpCallTargetObj = lpTargetObj;

	if ( lpTargetObj->Type == OBJ_MONSTER )
	{
		if ( lpTargetObj->m_RecallMon >= 0 )
		{
			lpCallTargetObj = &gObj[lpTargetObj->m_RecallMon];
		}
	}

	if ( this->PkCheck(lpCallObj, lpTargetObj) == FALSE )
	{
		return FALSE;
	}

	BOOL bIsOnDuel = gObjDuelCheck(lpObj, lpTargetObj);

	if ( bIsOnDuel )
	{
		lpObj->m_iDuelTickCount = GetTickCount();
		lpTargetObj->m_iDuelTickCount = GetTickCount();
	}

	int iOriginTargetDefense = 0;
	int targetdefense = this->GetTargetDefense(lpObj, lpTargetObj, MsgDamage, iOriginTargetDefense);

	if (iOriginTargetDefense > 0)
	{
		targetdefense = iOriginTargetDefense;
	}

	int AttackDamage = this->GetAttackDamage(lpObj, targetdefense, SubCode2, lpTargetObj);

	if ( lpTargetObj->DamageMinus )
	{
		int beforeDamage = AttackDamage;
		AttackDamage -= ( ( AttackDamage * (int)lpTargetObj->DamageMinus) / 100 );
	}

	int tlevel = lpObj->Level / 10;

	if ( AttackDamage < tlevel )
	{
		if ( tlevel < 1 )
		{
			tlevel = 1;
		}

		AttackDamage = tlevel;
	}

	if ( lpTargetObj->m_SkillNumber == 18 )
	{
		if ( AttackDamage > 1 )
		{
			AttackDamage >>= 1;
		}
	}

	if ( gObjAngelSprite(lpTargetObj) == TRUE )
	{
		if ( AttackDamage > 1 )
		{
			float  damage = (AttackDamage * 8) / 10.0f;
			AttackDamage = damage;
		}
	}

	if ( gObjWingSprite(lpTargetObj) == TRUE )
	{
		CItem * Wing = &lpTargetObj->pInventory[7];

		if ( AttackDamage > 1 && Wing->m_Type != ITEMGET(13, 30) )
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

			g_ConfigRead.m_ItemCalcLua.Generic_Call("Wings_CalcAbsorb", "iiid>i", AttackDamage, Wing->m_Type, Wing->m_Level, WingDamageBlock, &AttackDamage);
		}
	}

	if ( gObjDenorantSprite(lpTargetObj ) )
	{
		CItem * Dinorant = &lpTargetObj->pInventory[8];
		int dinorantdecdamage = 90 - Dinorant->IsDinorantReduceAttackDamaege();
		lpObj->Life -= 1.0f;

		if ( lpObj->Life < 0.0f )
		{
			lpObj->Life = 0.0f;
		}
		else
		{
			AttackDamage = AttackDamage * dinorantdecdamage / 100;
		}

		GSProtocol.GCReFillSend(lpObj->m_Index, lpObj->Life, 0xFF, 0, lpObj->iShield);
	}

	if ( gObjDarkHorse(lpTargetObj ) )
	{
		CItem * Darkhorse = &lpTargetObj->pInventory[8];
		int decdamage = 100 - ((Darkhorse->m_PetItem_Level + 30) / 2 );

		lpTargetObj->Life -= 1.0f;

		if ( lpTargetObj->Life < 0.0f )
		{
			lpTargetObj->Life = 0.0f;
		}
		else
		{
			AttackDamage = AttackDamage * decdamage / 100;
		}

		GSProtocol.GCReFillSend(lpTargetObj->m_Index, lpTargetObj->Life, 0xFF, 0, lpTargetObj->iShield);
	}

	if (AttackDamage > 1)
	{
		int nMuunItemEffectValue = 0;

		if (g_CMuunSystem.GetMuunItemValueOfOptType(lpTargetObj, MUUN_DEC_ENEMY_ATTACK_SKILL, &nMuunItemEffectValue, 0) == TRUE)
		{
			AttackDamage = AttackDamage * (100 - nMuunItemEffectValue) / 100;
		}
	}

	if ( lpTargetObj->Live )
	{
		if ( lpTargetObj->m_SkillInfo.SoulBarrierDefence && AttackDamage > 0)
		{
			int replacemana = (WORD)lpTargetObj->Mana * 2 / 100;

			if ( replacemana < lpTargetObj->Mana )
			{
				lpTargetObj->Mana -= replacemana;
				int decattackdamage = AttackDamage * lpTargetObj->m_SkillInfo.SoulBarrierDefence / 100;
				AttackDamage -= decattackdamage;
				ManaChange = TRUE;

			}
		}

		if ( g_ShieldSystemOn == FALSE )
		{
			if ( lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER )
			{
				if ( CC_MAP_RANGE(lpObj->MapNumber ) && CC_MAP_RANGE(lpTargetObj->MapNumber) )
				{
					AttackDamage = AttackDamage * 50 / 100;
				}
			}

			if (lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER)
			{
				if (lpObj->MapNumber == MAP_INDEX_CHAOSCASTLE_SURVIVAL && lpTargetObj->MapNumber == MAP_INDEX_CHAOSCASTLE_SURVIVAL)
				{
					AttackDamage = AttackDamage * 50 / 100;
				}
			}
		}

		if (g_ConfigRead.server.GetServerType() == SERVER_CASTLE)
		{
			if (g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE)
			{
				if (lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER)
				{
					if (lpObj->MapNumber == MAP_INDEX_CASTLESIEGE && lpTargetObj->MapNumber == MAP_INDEX_CASTLESIEGE)
					{
						if (lpObj->m_btCsJoinSide == lpTargetObj->m_btCsJoinSide)
						{
							AttackDamage = AttackDamage * g_CastleSiege.CastleSiegeSelfDmgReductionPercent / 100;
						}
						else if (g_ShieldSystemOn == FALSE)
						{
							AttackDamage = AttackDamage * g_CastleSiege.CastleSiegeDmgReductionPercent / 100;
						}
					}
				}
			}
		}

		if ( lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_MONSTER )
		{
			if ( lpTargetObj->Class == 283 )
			{
				if ( gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_BLESS_POTION) == TRUE || gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_SOUL_POTION) == TRUE )
				{
					AttackDamage += (AttackDamage * 20) / 100;
				}
				else 
				{
					if ( lpObj->m_iAccumulatedDamage > 100 )
					{
						gObjWeaponDurDownInCastle(lpObj, lpTargetObj, 1);
						lpObj->m_iAccumulatedDamage = 0;
					}
					else
					{
						lpObj->m_iAccumulatedDamage += AttackDamage;
					}

					AttackDamage = AttackDamage * 20 / 100;
				}
			}

			if ( lpTargetObj->Class == 277 )
			{
				if ( gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_BLESS_POTION) == TRUE || gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_SOUL_POTION) == TRUE )
				{
					AttackDamage += (AttackDamage * 20) / 100;
				}
				else 
				{
					if ( lpObj->m_iAccumulatedDamage > 100 )
					{
						gObjWeaponDurDownInCastle(lpObj, lpTargetObj, 1);
						lpObj->m_iAccumulatedDamage = 0;
					}
					else
					{
						lpObj->m_iAccumulatedDamage += AttackDamage;
					}

					AttackDamage = AttackDamage * 20 / 100;
				}
			}
		}

		if (bIsOnDuel == TRUE)
		{
			AttackDamage = AttackDamage * g_NewPVP.m_iDuelDamageReduction / 100;
		}

		if (g_GensSystem.IsMapBattleZone(lpObj->MapNumber) && g_GensSystem.IsMapBattleZone(lpTargetObj->MapNumber))
		{
			if (lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER)
			{
				if (g_GensSystem.IsPkEnable(lpObj, lpTargetObj) == true)
				{
					AttackDamage = AttackDamage * g_GensSystem.GetDamageReduction() / 100;
				}
			}
		}

		if (lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER)
		{
			AttackDamage = AttackDamage * g_ConfigRead.calc.PvPDamageRate[lpObj->Class][lpTargetObj->Class] / 100;
		}

		else if (lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_MONSTER)
		{
			AttackDamage = AttackDamage * g_ConfigRead.calc.PvMDamageRate[lpObj->Class] / 100;
		}

		if ( lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER )
		{
			iTempShieldDamage = this->GetShieldDamage(lpObj, lpTargetObj, AttackDamage);
			lpTargetObj->iShield -= iTempShieldDamage;
			lpTargetObj->Life -= AttackDamage - iTempShieldDamage;
			iTotalShieldDamage += iTempShieldDamage;

			if ( lpTargetObj->Life < 0.0f )
			{
				lpTargetObj->Life = 0.0f;
			}
		}
		else
		{
			lpTargetObj->Life -= AttackDamage;

			if ( lpTargetObj->Life < 0.0f )
			{
				lpTargetObj->Life = 0.0f;
			}
		}
	}

	if ( lpTargetObj->Type == OBJ_MONSTER )
	{
		gObjAddMsgSendDelay(lpTargetObj, 0, lpObj->m_Index, 100, 0);
		lpTargetObj->LastAttackerID = lpObj->m_Index;

		if ( lpTargetObj->m_iCurrentAI )
		{
			lpTargetObj->m_Agro->IncAgro(lpObj->m_Index, AttackDamage / 100);
		}
	}

	BOOL selfdefense = 0;
	lpCallObj = lpTargetObj;
	
	if ( lpTargetObj->Type == OBJ_MONSTER )
	{
		if ( lpTargetObj->m_RecallMon >= 0 )
		{
			lpCallObj = &gObj[lpTargetObj->m_RecallMon];
		}
	}

	if ( AttackDamage >= 1 )
	{
		if ( lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER )
		{
			if ( gObjDuelCheck(lpObj, lpTargetObj) )
			{
				selfdefense = 0;
			}

			else if (lpObj->m_PlayerData->RegisterdLMS == 1 && lpTargetObj->m_PlayerData->RegisterdLMS == 1)
			{
				if (lpObj->MapNumber == g_LastManStanding.m_Cfg.iPVPMap)
				{
					if (g_LastManStanding.m_Rooms[lpObj->m_PlayerData->RegisteredLMSRoom].bState == 3)
					{
						selfdefense = 0;
					}
				}
			}

			else if ( CC_MAP_RANGE(lpObj->MapNumber) || CC_MAP_RANGE(lpTargetObj->MapNumber) )
			{
				selfdefense = 0;
			}

			else if (lpObj->MapNumber == MAP_INDEX_CHAOSCASTLE_SURVIVAL || lpTargetObj->MapNumber == MAP_INDEX_CHAOSCASTLE_SURVIVAL)
			{
				selfdefense = 0;
			}

			else if ( IT_MAP_RANGE(lpObj->MapNumber) || IT_MAP_RANGE(lpTargetObj->MapNumber) )
			{
				selfdefense = 0;
			}

			else if (g_GensSystem.IsMapBattleZone(lpObj->MapNumber) && g_GensSystem.IsMapBattleZone(lpTargetObj->MapNumber))
			{
				selfdefense = 0;
			}

			else if (g_ArcaBattle.IsArcaBattleServer() == TRUE)
			{
				selfdefense = 0;
			}

			else
			{
				selfdefense = 1;
			}

			if ( gObjGetRelationShip(lpObj, lpTargetObj) == 2 )
			{
				selfdefense = FALSE;
			}

			if (g_ConfigRead.server.GetServerType() == SERVER_CASTLE)
			{
				if (g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE)
				{
					if (lpObj->m_btCsJoinSide > 0)
					{
						selfdefense = FALSE;
					}
				}
			}
		}
		else if ( lpTargetObj->Type == OBJ_MONSTER && lpObj->Type == OBJ_USER )
		{
			if ( lpTargetObj->m_RecallMon >= 0 )
			{
				selfdefense = TRUE;
			}
		}

		if ( lpTargetObj->Type == OBJ_USER )
		{
			gObjArmorRandomDurDown(lpTargetObj, lpObj);
		}
	}

	if ( selfdefense == TRUE )
	{
		if ( !gObjTargetGuildWarCheck(lpObj, lpCallObj) )
		{
			if(lpCallObj->PartyNumber >= 0)
			{
				int number = 0;
				int partynum = lpCallObj->PartyNumber;
				
				if( (gParty.GetPKPartyPenalty(partynum)) < 5)
				{
					gObjCheckSelfDefense(lpObj, lpCallObj->m_Index);
				}	
			}
			else
			{
				gObjCheckSelfDefense(lpObj, lpCallObj->m_Index);
			}
		}
	}

	if ( lpObj->Type == OBJ_USER )
	{
		if ( lpObj->m_Change == 9 )
		{
			GSProtocol.GCMagicAttackNumberSend(lpObj, 3, lpTargetObj->m_Index, 1);
		}
	}

	lpObj->m_Rest = 0;

	if ( AttackDamage > 0 )
	{
		int atd_reflect = (int)((float)AttackDamage * lpTargetObj->DamageReflect / 100.0f);

		if(lpTargetObj->DamageReflect > g_ConfigRead.calc.ReflectDamage)
		{
			lpTargetObj->DamageReflect = g_ConfigRead.calc.ReflectDamage;
		}

		if ( atd_reflect )
		{
			gObjAddMsgSendDelay(lpTargetObj, 10, lpObj->m_Index, 10, atd_reflect);
		}

		if ( lpObj->Type == OBJ_USER && (rand()%100) < lpObj->m_PlayerData->SetOpReflectionDamage )
		{
			gObjAddMsgSendDelay(lpTargetObj, 10, lpObj->m_Index, 10, AttackDamage);
		}

		if ( g_ShieldSystemOn == TRUE )	// #error Remove the //
		{
			AttackDamage -= iTotalShieldDamage;
		}

		gObjLifeCheck(lpTargetObj, lpObj, AttackDamage, 0, 0, MsgDamage, skill, iTotalShieldDamage, 0);
	}
	else
	{
		GSProtocol.GCDamageSend(lpObj->m_Index, lpTargetObj->m_Index, 0, 0, MsgDamage, 0);
	}

	if ( lpObj->Life <= 0.0f && lpObj->Type == OBJ_USER )
	{
		if ( lpObj->m_CheckLifeTime <= 0 )
		{
			lpObj->lpAttackObj = lpTargetObj;

			if ( lpTargetObj->Type == OBJ_USER )
			{
				lpObj->m_bAttackerKilled = true;
			}
			else
			{
				lpObj->m_bAttackerKilled = false;
			}

			lpObj->m_CheckLifeTime = 3;
		}
	}

	return TRUE;
}

int CMuunAttack::GetAttackDamage(LPOBJ lpObj, int targetDefense, int SubCode2, LPOBJ lpTargetObj)
{
	int ad = 0;
	int AttackDamage = lpObj->m_MuunEffectList[SubCode2].nOptValue * (lpObj->m_PlayerData->MasterLevel + lpObj->Level) + 10;

	if (lpTargetObj->Type == OBJ_USER)
	{
		ad = AttackDamage / 2 - targetDefense;
	}

	else
	{
		ad = AttackDamage - targetDefense;
	}

	return ad;
}

int CMuunAttack::GetShieldDamage(LPOBJ lpObj, LPOBJ lpTargetObj, int iAttackDamage)
{
	if (g_ShieldSystemOn == FALSE)
		return 0;

	if (iAttackDamage <= 0)
		return 0;

	if (lpObj->Type != OBJ_USER)
		return 0;

	if (lpTargetObj->Type != OBJ_USER)
		return 0;

	int iReduceLife = 0;
	int iReduceShield = 0;
	int iReduceLifeForEffect = 0; 
	bool bReduceShieldGage = 0;
	int iDamageDevideToSDRate = g_iDamageDevideToSDRate;
	iDamageDevideToSDRate -= lpObj->m_PlayerData->m_JewelOfHarmonyEffect.HJOpDecreaseSDRate;
	iDamageDevideToSDRate += lpTargetObj->m_PlayerData->m_JewelOfHarmonyEffect.HJOpAddSDRate;

	if ( iDamageDevideToSDRate < 0 )
		iDamageDevideToSDRate = 0;

	if ( iDamageDevideToSDRate > 100 )
		iDamageDevideToSDRate = 100;

	if ( lpObj->m_PlayerData->m_JewelOfHarmonyEffect.HJOpAddIgnoreSDRate > 0 )
	{
		int iRand = rand() % 100;
		int iIgnoreSDRate = lpObj->m_PlayerData->m_JewelOfHarmonyEffect.HJOpAddIgnoreSDRate - lpTargetObj->m_PlayerData->m_Resistance_SD;

		if (iRand < iIgnoreSDRate)
		{
			iDamageDevideToSDRate = 0;
		}
	}

	if ( (lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER) && ( lpObj->m_PlayerData->m_JewelOfHarmonyEffect.HJOpDecreaseSDRate || lpTargetObj->m_PlayerData->m_JewelOfHarmonyEffect.HJOpAddSDRate || lpObj->m_PlayerData->m_JewelOfHarmonyEffect.HJOpAddIgnoreSDRate ) )
	{
		g_Log.Add("[JewelOfHarmony][PvP System] Attacker:[%s][%s]-SD Decrease[%d] SD Ignore[%d] Defender:[%s][%s] SD Increase Option[%d] - SD Rate[%d]",
			lpObj->AccountID, lpObj->Name,
			lpObj->m_PlayerData->m_JewelOfHarmonyEffect.HJOpDecreaseSDRate, lpObj->m_PlayerData->m_JewelOfHarmonyEffect.HJOpAddIgnoreSDRate, lpTargetObj->AccountID, lpTargetObj->Name, lpTargetObj->m_PlayerData->m_JewelOfHarmonyEffect.HJOpAddSDRate, iDamageDevideToSDRate);
	}

	iReduceShield = iAttackDamage * iDamageDevideToSDRate / 100;
	iReduceLife = iAttackDamage - iReduceShield;

	if ( (lpTargetObj->iShield-iReduceShield) <0 )
	{
		iReduceLife += iReduceShield  - lpTargetObj->iShield;
		iReduceShield = lpTargetObj->iShield;

		if ( lpTargetObj->iShield > 0 )
		{
			bReduceShieldGage = true;
		}
	}

	iReduceLifeForEffect = ( lpTargetObj->MaxLife + lpTargetObj->AddLife ) * 20.0f / 100.0f;

	if ( bReduceShieldGage == true && iReduceLife > iReduceLifeForEffect )
	{
		if ( !CC_MAP_RANGE(lpTargetObj->MapNumber) && lpTargetObj->MapNumber != MAP_INDEX_CHAOSCASTLE_SURVIVAL )
		{
			GSProtocol.GCSendEffectInfo(lpTargetObj->m_Index, 17);
		}
	}

	return iReduceShield;
}

CMuunSystem g_CMuunSystem;

CMuunSystem::CMuunSystem()
{

}

CMuunSystem::~CMuunSystem()
{
	if (this->m_MuunItemPeriodData != NULL)
	{
		delete[] this->m_MuunItemPeriodData;
	}
}

void CMuunSystem::Initialize()
{
	if (this->m_MuunItemPeriodData != NULL)
	{
		delete[] this->m_MuunItemPeriodData;
	}

	this->m_MuunItemPeriodData = new MUUN_ITEM_PERIOD_DATA[g_ConfigRead.server.GetObjectMaxUser()];

	for (int i = 0; i<g_ConfigRead.server.GetObjectMaxUser(); i++)
	{
		this->m_MuunItemPeriodData[i].Clear();
	}
}

bool CMuunSystem::LoadScriptMuunSystemInfo(char *lpszFileName)
{
	return this->m_MuunInfoMng.LoadScriptMuunSystemInfo(lpszFileName);
}

bool CMuunSystem::LoadScriptMuunSystemOption(char *lpszFileName)
{
	return this->m_MuunInfoMng.LoadScriptMuunSystemOption(lpszFileName);
}

void CMuunSystem::MuunItemDamage(OBJECTSTRUCT *lpObj, int damage)
{
	if (g_ConfigRead.pet.DamageDisableForPet[DAMAGE_OFF_MUUN] == true)
	{
		return;
	}

	if (lpObj->Type != OBJ_USER && (double)damage > 0.0)
	{
		return;
	}

	if (damage <= 0)
	{
		return;
	}

	for (int i = 0; i < 2; i++)
	{
		float fdamage = (float)damage;

		if (lpObj->pMuunInventory[i].IsItem() == TRUE && lpObj->pMuunInventory[i].m_Durability > 0.0)
		{
			fdamage = fdamage + fdamage;
			fdamage = fdamage / 400.0;
			fdamage = fdamage / 400.0;

			lpObj->pMuunInventory[i].m_Durability -= fdamage;

			if (lpObj->pMuunInventory[i].m_Durability != 0.0)
			{
				GSProtocol.GCMuunItemDurSend(lpObj->m_Index, i, lpObj->pMuunInventory[i].m_Durability);
			}

			if (lpObj->pMuunInventory[i].m_Durability < 1.0)
			{
				lpObj->pMuunInventory[i].m_Durability = 0.0;
				this->CalCharacterStat(lpObj->m_Index, lpObj->m_MuunEffectList[i].nOptType);
			}
		}
	}
}

BOOL CMuunSystem::MuunItemEquipment(int aIndex, int iPos, int iSource)
{
	LPOBJ lpObj = &gObj[aIndex];

	if (gObj[aIndex].Type != OBJ_USER)
	{
		return FALSE;
	}

	if (iPos >= 2)
	{
		if (iSource < 2 && iSource != -1 && lpObj->pMuunInventory[iSource].IsItem() == FALSE)
		{
			if (!iSource)
			{
				lpObj->m_wMuunItem = -1;
			}

			else if (iSource == 1)
			{
				lpObj->m_wMuunSubItem = -1;
			}

			this->RemoveUserMuunEffect(lpObj, iSource);
			this->GCSendConditionStatus(aIndex, iSource, 0);
			GSProtocol.GCMuunEquipmentChange(lpObj->m_Index, iSource);
		}
	}
	else
	{
		if (lpObj->pMuunInventory[iPos].IsItem() == TRUE)
		{
			WORD nItemNum = lpObj->pMuunInventory[iPos].m_Type;

			if (iPos == 0)
			{
				lpObj->m_wMuunItem = ITEM_GET_INDEX(lpObj->pMuunInventory[iPos].m_Type) + (((ITEM_GET_TYPE(lpObj->pMuunInventory[iPos].m_Type)) | (0x10 * lpObj->pMuunInventory[iPos].m_Level)) << 9);
			}

			else if (iPos == 1)
			{
				lpObj->m_wMuunSubItem = ITEM_GET_INDEX(lpObj->pMuunInventory[iPos].m_Type) + (((ITEM_GET_TYPE(lpObj->pMuunInventory[iPos].m_Type)) | (0x10 * lpObj->pMuunInventory[iPos].m_Level)) << 9);
			}

			this->SetUserMuunEffect(lpObj, nItemNum, lpObj->pMuunInventory[iPos].m_Level, iPos);
			this->CheckEquipMuunItemCondition(lpObj);
		}
	}

	return FALSE;
}

void CMuunSystem::SetMuunItemAddPeriodData(OBJECTSTRUCT *lpObj, int iMuunItemNum, UINT64 dwSerial)
{
	CMuunInfo *pCMuunInfo = this->m_MuunInfoMng.GetMuunItemNumToMuunInfo(iMuunItemNum);

	if (!pCMuunInfo)
	{
		g_Log.Add("[MuunSystem][Error] pCMuunInfo is NULL [%s][%s]", lpObj->AccountID, lpObj->Name);
		return;
	}

	pCMuunInfo->GetIndex();
	pCMuunInfo->GetAddOptStart();
	pCMuunInfo->GetAddOptEnd();
	pCMuunInfo->GetConditionType();
	pCMuunInfo->GetConditionVal1();
	pCMuunInfo->GetConditionVal2();
	this->AddMuunItmePeriodData(lpObj, iMuunItemNum, dwSerial, 0, pCMuunInfo);
}

bool CMuunSystem::SetUserMuunEffect(OBJECTSTRUCT *lpObj, int iMuunItemNum, int iMuunLv, int iEquipPos)
{
	CMuunInfo *pCMuunInfo = m_MuunInfoMng.GetMuunItemNumToMuunInfo(iMuunItemNum);

	if (!pCMuunInfo)
	{
		g_Log.Add("[MuunSystem][Error] pCMuunInfo is NULL [%s][%s]", lpObj->AccountID, lpObj->Name);
		return FALSE;
	}

	if (iEquipPos == 0)
	{
		lpObj->m_MuunEffectList[0].nOptType = pCMuunInfo->GetOptType();
		lpObj->m_MuunEffectList[0].nOptValue = pCMuunInfo->GetMuunLvVal(iMuunLv);
		lpObj->m_MuunEffectList[0].nAddOptType = pCMuunInfo->GetAddOptType();
		lpObj->m_MuunEffectList[0].nAddOptValue = pCMuunInfo->GetAddOptVal();
		lpObj->m_MuunEffectList[0].nIndex = pCMuunInfo->GetIndex();
		lpObj->m_MuunEffectList[0].nMuunItemNum = iMuunItemNum;
		lpObj->m_MuunEffectList[0].pCMuunInfo = pCMuunInfo;
		lpObj->m_MuunEffectList[0].bSkillUsed = false;
		lpObj->m_MuunEffectList[0].nSkillDelayTime = pCMuunInfo->GetDelayTime();
		lpObj->m_MuunEffectList[0].nTickTime = pCMuunInfo->GetDelayTime() + GetTickCount();

		this->SetAddOptTypeValue(&lpObj->m_MuunEffectList[0]);
	}

	else if (iEquipPos == 1)
	{
		lpObj->m_MuunEffectList[1].nOptType = pCMuunInfo->GetOptType();
		lpObj->m_MuunEffectList[1].nOptValue = pCMuunInfo->GetMuunLvVal(iMuunLv) / 2;
		lpObj->m_MuunEffectList[1].nAddOptType = pCMuunInfo->GetAddOptType();
		lpObj->m_MuunEffectList[1].nAddOptValue = pCMuunInfo->GetAddOptVal();
		lpObj->m_MuunEffectList[1].nIndex = pCMuunInfo->GetIndex();
		lpObj->m_MuunEffectList[1].nMuunItemNum = iMuunItemNum;
		lpObj->m_MuunEffectList[1].pCMuunInfo = pCMuunInfo;
		lpObj->m_MuunEffectList[1].bSkillUsed = false;
		lpObj->m_MuunEffectList[1].nSkillDelayTime = pCMuunInfo->GetDelayTime();
		lpObj->m_MuunEffectList[1].nTickTime = pCMuunInfo->GetDelayTime() + GetTickCount();

		this->SetAddOptTypeValue(&lpObj->m_MuunEffectList[1]);
	}

	return TRUE;
}

void CMuunSystem::SetAddOptTypeValue(_tagMUUN_EFFECT_LIST *pUserMuunEffect)
{
	if (pUserMuunEffect->nAddOptType == 1)
	{
		pUserMuunEffect->nTotalVal = pUserMuunEffect->nOptValue * pUserMuunEffect->nAddOptValue;
	}

	else if (pUserMuunEffect->nAddOptType == 2)
	{
		pUserMuunEffect->nTotalVal = pUserMuunEffect->nOptValue + pUserMuunEffect->nAddOptValue;
	}

	else if (pUserMuunEffect->nAddOptType == 3)
	{
		pUserMuunEffect->nTotalVal = pUserMuunEffect->nOptValue;
	}
}

bool CMuunSystem::RemoveUserMuunEffect(OBJECTSTRUCT *lpObj, int iEquipPos)
{
	if (iEquipPos >= 2)
	{
		return TRUE;
	}

	CMuunInfo *pCMuunInfo = lpObj->m_MuunEffectList[iEquipPos].pCMuunInfo;

	if (!pCMuunInfo)
	{
		g_Log.Add("[MuunSystem][Error] pCMuunInfo is NULL [%s][%s]", lpObj->AccountID, lpObj->Name);
		return FALSE;
	}

	int nOptType = pCMuunInfo->GetOptType();

	lpObj->m_MuunEffectList[iEquipPos].Clear();
	this->CalCharacterStat(lpObj->m_Index, nOptType);
}

bool CMuunSystem::GetMuunItemValueOfOptType(OBJECTSTRUCT *lpObj, int iMuunOptIndex, int *EffectValue1, int *EffectValue2)
{
	if (lpObj->Type != OBJ_USER)
	{
		return false;
	}

	*EffectValue1 = 0;

	for (int i = 0; i < MAX_MUUN_EFFECT_LIST; i++)
	{
		if (lpObj->pMuunInventory[i].IsItem() == FALSE)
		{
			continue;
		}

		if (iMuunOptIndex != lpObj->m_MuunEffectList[i].nOptType)
		{
			continue;
		}

		if (lpObj->m_MuunEffectList[i].bOptEnable == FALSE)
		{
			continue;
		}

		if (lpObj->pMuunInventory[i].m_Durability <= 0.0)
		{
			continue;
		}

		if (lpObj->pMuunInventory[i].IsMuunItemPeriodExpire())
		{
			*EffectValue1 += lpObj->m_MuunEffectList[i].nOptValue;
		}

		else
		{
			*EffectValue1 += lpObj->m_MuunEffectList[i].nTotalVal;
		}

		if (iMuunOptIndex == 51)
		{
			if (lpObj->m_MuunEffectList[i].bSkillUsed == true)
			{
				return false;
			}

			lpObj->m_MuunEffectList[i].bSkillUsed = true;

			PMSG_MUUN_ATTACK_COMMAND pMsg;

			pMsg.Slot = i;
			pMsg.SkillType = 51;
			pMsg.NumberH = HIBYTE(lpObj->m_Index);
			pMsg.NumberL = LOBYTE(lpObj->m_Index);
			pMsg.TargetNumberH = HIBYTE(lpObj->m_Index);
			pMsg.TargetNumberL = LOBYTE(lpObj->m_Index);

			PHeadSubSetB((LPBYTE)&pMsg, 0x4E, 0x12, sizeof(pMsg));

			IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
			GSProtocol.MsgSendV2(lpObj, (LPBYTE)&pMsg, pMsg.h.size);
		}

		return true;
	}

	return false;
}

void CMuunSystem::GDReqLoadMuunInvenItem(OBJECTSTRUCT *obj)
{
	SDHP_REQ_DBMUUN_INVEN_LOAD pMsg;

	if (!ObjectMaxRange(obj->m_Index))
	{
		return;
	}

	if (!gObjIsConnected(obj->m_Index))
	{
		return;
	}
			
	memcpy(pMsg.AccountID, obj->AccountID, MAX_ACCOUNT_LEN + 1);
	memcpy(pMsg.Name, obj->Name, MAX_ACCOUNT_LEN + 1);
	pMsg.aIndex = obj->m_Index;

	pMsg.h.c = 0xC1;
	pMsg.h.size = sizeof(pMsg);
	pMsg.h.headcode = 0xF1;
	
	wsDataCli.DataSend((char*)&pMsg, pMsg.h.size);
}

void CMuunSystem::DGLoadMuunInvenItem(_tagSDHP_ANS_DBMUUN_INVEN_LOAD *lpMsg)
{
	int aIndex = lpMsg->aIndex;
	char szId[11];
	szId[MAX_ACCOUNT_LEN] = 0;
	memcpy(szId, gObj[aIndex].AccountID, MAX_ACCOUNT_LEN);

	if (ObjectMaxRange(aIndex) == false)
	{
		g_Log.AddC(TColor::Yellow, "[DEBUG MUUN] Wrong aIndex (RECV)");
		return;
	}

	if (gObj[aIndex].m_bMapSvrMoveReq == true)
	{
		g_Log.Add("[DGLoadMuunInvenItem] MapServerMove User.Can't Open Event Inven. return!! [%s], IP [%s] ", gObj[aIndex].AccountID, gObj[aIndex].m_PlayerData->Ip_addr);
		return;
	}

	if (gObj[aIndex].m_State == 32)
	{
		g_Log.Add("[DGLoadMuunInvenItem] MapServerMove User.Can't Open Event Inven. return!! [%s], IP [%s] ", gObj[aIndex].AccountID, gObj[aIndex].m_PlayerData->Ip_addr);
		return;
	}

	if (gObj[aIndex].m_bMapSvrMoveQuit == 1)
	{
		g_Log.Add("[DGLoadMuunInvenItem] MapServerMove User.Can't Open Event Inven. return!! [%s], IP [%s] ", gObj[aIndex].AccountID, gObj[aIndex].m_PlayerData->Ip_addr);
		return;
	}

	if (!gObjIsAccontConnect(aIndex, szId))
	{
		g_Log.Add("error-L1 : Request to receive Warehouse information doesn't match the user. [%s][%d]", szId, aIndex);
		return;
	}

	gObj[aIndex].bMuunInventoryLoad = true;
	LPOBJ lpObj = &gObj[aIndex];

	int itype;
	int _type;
	BYTE OptionData;
	CItem item;
	int nMuunInvenItemCount = 0;

	for (int n = 0; n < MUUN_INVENTORY_SIZE; n++)
	{
		itype = lpMsg->dbItems[MAX_DBITEM_INFO * n];
		itype |= (lpMsg->dbItems[MAX_DBITEM_INFO * n + 9] & 0xF0) << 5;
		itype |= (lpMsg->dbItems[MAX_DBITEM_INFO * n + 7] & 0x80) << 1;
		_type = itype;

		lpObj->pMuunInventory[n].Clear();

		if (lpMsg->dbItems[MAX_DBITEM_INFO * n] == (BYTE)-1 && (lpMsg->dbItems[MAX_DBITEM_INFO * n + 9] & 0xF0) == 0xF0 && lpMsg->dbItems[MAX_DBITEM_INFO * n + 7] & 0x80)
		{
			itype = -1;
		}

		if (IsItem(_type) == FALSE)
		{
			itype = -1;
		}

		if (itype != -1)
		{
			item.m_Level = DBI_GET_LEVEL(lpMsg->dbItems[MAX_DBITEM_INFO * n + 1]);
			OptionData = lpMsg->dbItems[n*MAX_DBITEM_INFO + 1];
			item.m_Option1 = DBI_GET_SKILL(OptionData);
			item.m_Option2 = DBI_GET_LUCK(OptionData);
			item.m_Option3 = DBI_GET_OPTION(OptionData);

			if (_type == ITEMGET(13, 3))
			{
				item.m_Option3 |= (lpMsg->dbItems[MAX_DBITEM_INFO * n + DBI_NOPTION_DATA] & 0x40) >> 4;
			}

			item.m_Durability = lpMsg->dbItems[MAX_DBITEM_INFO * n + DBI_DUR];
			item.m_JewelOfHarmonyOption = lpMsg->dbItems[MAX_DBITEM_INFO * n + DBI_JOH_DATA];
			item.m_ItemOptionEx = DBI_GET_380OPTION(lpMsg->dbItems[n*MAX_DBITEM_INFO + DBI_OPTION380_DATA]);

			if (item.m_ItemOptionEx != 0)
			{
				item.m_Type = itype;
				if (!g_kItemSystemFor380.Is380Item(&item))
				{
					item.m_ItemOptionEx = 0;
					g_Log.Add("[380Item][%s][%s] Invalid 380 Item Option in Muun Inventory pos[%d]", lpObj->AccountID, lpObj->Name, n);
				}
			}

			BYTE SocketOption[5];
			BYTE SocketOptionIndex = 0xFF;
			memset(&SocketOption, 0xFF, 5);

			if (g_SocketOptionSystem.IsEnableSocketItem(itype) == 1)
			{
				SocketOptionIndex = lpMsg->dbItems[MAX_DBITEM_INFO * n + DBI_JOH_DATA];

				for (int iSocketSlotIndex = 0; iSocketSlotIndex < 5; iSocketSlotIndex++)
				{
					SocketOption[iSocketSlotIndex] = lpMsg->dbItems[((n*MAX_DBITEM_INFO) + DBI_SOCKET_1) + iSocketSlotIndex];
				}
			}

			else if (g_PentagramSystem.IsPentagramItem(itype) == TRUE ||
				g_PentagramSystem.IsPentagramJewel(itype) == TRUE)
			{
				SocketOptionIndex = lpMsg->dbItems[MAX_DBITEM_INFO * n + DBI_JOH_DATA];

				for (int iSocketSlotIndex = 0; iSocketSlotIndex < 5; iSocketSlotIndex++)
				{
					SocketOption[iSocketSlotIndex] = lpMsg->dbItems[((n*MAX_DBITEM_INFO) + DBI_SOCKET_1) + iSocketSlotIndex];
				}
			}

			else if (this->IsMuunItem(itype) == TRUE)
			{
				SocketOptionIndex = lpMsg->dbItems[MAX_DBITEM_INFO * n + DBI_JOH_DATA];

				for (int iSocketSlotIndex = 0; iSocketSlotIndex < 5; iSocketSlotIndex++)
				{
					SocketOption[iSocketSlotIndex] = lpMsg->dbItems[((n*MAX_DBITEM_INFO) + DBI_SOCKET_1) + iSocketSlotIndex];
				}
			}

			else if (this->IsStoneofEvolution(itype) == TRUE)
			{
				SocketOptionIndex = 0;

				for (int iSocketSlotIndex = 0; iSocketSlotIndex < 5; iSocketSlotIndex++)
				{
					SocketOption[iSocketSlotIndex] = lpMsg->dbItems[((n*MAX_DBITEM_INFO) + DBI_SOCKET_1) + iSocketSlotIndex];
				}
			}

			else
			{
				SocketOptionIndex = 0;

				for (int iSocketSlotIndex = 0; iSocketSlotIndex < 5; ++iSocketSlotIndex)
				{
					SocketOption[iSocketSlotIndex] = lpMsg->dbItems[((n*MAX_DBITEM_INFO) + DBI_SOCKET_1) + iSocketSlotIndex];

					if (SocketOption[iSocketSlotIndex] == 0)
					{
						SocketOption[iSocketSlotIndex] = 0xFF;
					}
				}
			}

			item.m_PeriodItemOption = (lpMsg->dbItems[MAX_DBITEM_INFO * n + 9] & 6) >> 1;
			item.Convert(itype, item.m_Option1, item.m_Option2, item.m_Option3, DBI_GET_NOPTION(lpMsg->dbItems[n*MAX_DBITEM_INFO + DBI_NOPTION_DATA]), lpMsg->dbItems[n*MAX_DBITEM_INFO + DBI_SOPTION_DATA], item.m_ItemOptionEx, SocketOption, SocketOptionIndex, item.m_PeriodItemOption, 3);

			lpObj->pMuunInventory[n].m_Option1 = item.m_Option1;
			lpObj->pMuunInventory[n].m_Option2 = item.m_Option2;
			lpObj->pMuunInventory[n].m_Option3 = item.m_Option3;
			lpObj->pMuunInventory[n].m_JewelOfHarmonyOption = item.m_JewelOfHarmonyOption;
			lpObj->pMuunInventory[n].m_ItemOptionEx = item.m_ItemOptionEx;

			DWORD hidword = MAKE_NUMBERDW(MAKE_NUMBERW(lpMsg->dbItems[n*MAX_DBITEM_INFO + DBI_SERIAL1], lpMsg->dbItems[n*MAX_DBITEM_INFO + DBI_SERIAL2]), MAKE_NUMBERW(lpMsg->dbItems[n*MAX_DBITEM_INFO + DBI_SERIAL3], lpMsg->dbItems[n*MAX_DBITEM_INFO + DBI_SERIAL4]));
			DWORD lodword = MAKE_NUMBERDW(MAKE_NUMBERW(lpMsg->dbItems[n*MAX_DBITEM_INFO + DBI_SERIAL5], lpMsg->dbItems[n*MAX_DBITEM_INFO + DBI_SERIAL6]), MAKE_NUMBERW(lpMsg->dbItems[n*MAX_DBITEM_INFO + DBI_SERIAL7], lpMsg->dbItems[n*MAX_DBITEM_INFO + DBI_SERIAL8]));
			item.m_Number = MAKEQWORD(hidword, lodword);

			if (this->IsMuunItem(&item) == TRUE)
			{
				if (item.IsMuunItemPeriodExpire() == FALSE)
				{
					CMuunInfo *	pCMuunInfo = this->m_MuunInfoMng.GetMuunItemNumToMuunInfo(itype);

					if (pCMuunInfo == NULL)
					{
						g_Log.Add("[MuunSystem][Error] [%s][%s] DGLoadMuunInvenItem() pCMuunInfo is NULL %d", lpObj->AccountID, lpObj->Name, __LINE__);
					}

					else
					{
						if (this->CheckAddOptionExpireDate(pCMuunInfo->GetAddOptStart(), pCMuunInfo->GetAddOptEnd()) == TRUE)
						{
							item.SetMuunItemPeriodExpire();
						}

						else
						{
							this->SetMuunItemAddPeriodData(lpObj, itype, item.m_Number);
						}
					}
				}

				else
				{
					CMuunInfo *	pCMuunInfo = this->m_MuunInfoMng.GetMuunItemNumToMuunInfo(itype);

					if (pCMuunInfo == NULL)
					{
						g_Log.Add("[MuunSystem][Error] [%s][%s] DGLoadMuunInvenItem() pCMuunInfo is NULL %d", lpObj->AccountID, lpObj->Name, __LINE__);
					}

					else
					{
						if (this->CheckAddOptionExpireDate(pCMuunInfo->GetAddOptStart(), pCMuunInfo->GetAddOptEnd()) == FALSE)
						{
							item.SetMuunItemPeriodReset();
							this->SetMuunItemAddPeriodData(lpObj, itype, item.m_Number);

							g_Log.Add("[MuunSystem][MuunItemPeriodReset] [%s][%s] MuunItemPeriodReset() Type:[%d] serial:[%I64d]",
								lpObj->AccountID, lpObj->Name, itype, item.m_Number);
						}
					}
				}
			}

			gObjMuunInventoryInsertItemPos(lpObj->m_Index, item, n);

			if (n < 2)
			{
				this->MuunItemEquipment(lpObj->m_Index, n, -1);

				if (!n)
				{
					GSProtocol.GCMuunEquipmentChange(lpObj->m_Index, 0);
				}
			}

			nMuunInvenItemCount++;
		}
	}

	for (int i = 0; i < MUUN_INVENTORY_SIZE; i++)
	{
		if (lpObj->pMuunInventory[i].IsItem() == TRUE)
		{
			if (this->IsStoneofEvolution(lpObj->pMuunInventory[i].m_Type) == TRUE)
			{
				WORD wPetItemNumber = (lpObj->pMuunInventory[i].m_SocketOption[0] << 8) | lpObj->pMuunInventory[i].m_SocketOption[1];
				LPITEM_ATTRIBUTE lpItemAttribute = GetItemAttr(wPetItemNumber);

				if (lpItemAttribute != NULL)
				{
					BYTE NewOption[MAX_EXOPTION_SIZE];
					ItemIsBufExOption(NewOption, &lpObj->pMuunInventory[i]);

					g_Log.Add("[%s][%s]MI[%d,%s(%s),%d,%d,%d,%d] Rank:[%d] Expire:[%d] serial:[%I64d] dur:[%d] Ex:[%d,%d,%d,%d,%d,%d,%d] Set[%d]",
						lpObj->AccountID, lpObj->Name, i, lpObj->pMuunInventory[i].GetName(), lpItemAttribute->Name, lpObj->pMuunInventory[i].m_Level, lpObj->pMuunInventory[i].m_Option1,
						lpObj->pMuunInventory[i].m_Option2, lpObj->pMuunInventory[i].m_Option3, lpObj->pMuunInventory[i].GetMuunItemRank(), lpObj->pMuunInventory[i].IsMuunItemPeriodExpire(),
						lpObj->pMuunInventory[i].m_Number, (int)lpObj->pMuunInventory[i].m_Durability, NewOption[0], NewOption[1], NewOption[2], NewOption[3], NewOption[4],
						NewOption[5], NewOption[6], lpObj->pMuunInventory[i].m_SetOption);
				}

				else
				{
					g_Log.Add("[MuunSystem][Error] [%s][%s] DGLoadMuunInvenItem() pCMuunInfo is NULL %d", lpObj->AccountID, lpObj->Name, __LINE__);
				}
			}

			else
			{
				BYTE NewOption[MAX_EXOPTION_SIZE];
				ItemIsBufExOption(NewOption, &lpObj->pMuunInventory[i]);
				g_Log.Add("[%s][%s]MI[%d,%s,%d,%d,%d,%d] Rank:[%d] Expire:[%d] serial:[%I64d] dur:[%d] Ex:[%d,%d,%d,%d,%d,%d,%d] Set[%d]",
					lpObj->AccountID, lpObj->Name, i, lpObj->pMuunInventory[i].GetName(), lpObj->pMuunInventory[i].m_Level, lpObj->pMuunInventory[i].m_Option1,
					lpObj->pMuunInventory[i].m_Option2, lpObj->pMuunInventory[i].m_Option3, lpObj->pMuunInventory[i].GetMuunItemRank(), lpObj->pMuunInventory[i].IsMuunItemPeriodExpire(),
					lpObj->pMuunInventory[i].m_Number, (int)lpObj->pMuunInventory[i].m_Durability, NewOption[0], NewOption[1], NewOption[2], NewOption[3], NewOption[4],
					NewOption[5], NewOption[6], lpObj->pMuunInventory[i].m_SetOption);

			}
		}
	}

	g_Log.Add("[MuunInvenItem] [%s][%s] ItemCount[%d] Muun Item Load Complete.", lpObj->AccountID, lpObj->Name, nMuunInvenItemCount);
	GCMuunInventoryItemListSend(aIndex);

	lpObj->dwCheckMuunItemTime = GetTickCount();
}

void CMuunSystem::GDReqSaveMuunInvenItem(LPOBJ lpObj)
{
	if (lpObj->bMuunInventoryLoad == false)
	{
		return;
	}

	if (!ObjectMaxRange(lpObj->m_Index))
	{
		return;
	}

	if (!gObjIsConnected(lpObj->m_Index))
	{
		return;
	}

	for (int n = 0; n < MUUN_INVENTORY_SIZE; n++)
	{
		if (lpObj->pMuunInventory[n].IsItem() == FALSE)
		{
			continue;
		}

		if (this->IsStoneofEvolution(lpObj->pMuunInventory[n].m_Type) == TRUE)
		{
			WORD wPetItemNumber = (lpObj->pMuunInventory[n].m_SocketOption[0] << 8) | lpObj->pMuunInventory[n].m_SocketOption[1];

			LPITEM_ATTRIBUTE pItemAttribute = GetItemAttr(wPetItemNumber);

			if (!pItemAttribute)
			{
				g_Log.Add("[MuunSystem][Error][%s][%s] GDReqSaveMuunInvenItem() pItemAttribute is NULL", lpObj->AccountID, lpObj->Name);
			}

			else
			{
				BYTE NewOption[MAX_EXOPTION_SIZE];
				ItemIsBufExOption(NewOption, &lpObj->pMuunInventory[n]);

				g_Log.Add("[%s][%s]MI[%d,%s(%s),%d,%d,%d,%d] Rank:[%d] Expire:[%d] serial:[%I64d] dur:[%d] Ex:[%d,%d,%d,%d,%d,%d,%d] Set[%d] 380:[%d] HO:[%d,%d] SC[%d,%d,%d,%d,%d] BonusOption[%d]",
					lpObj->AccountID, lpObj->Name, n, lpObj->pMuunInventory[n].GetName(), pItemAttribute->Name,
					lpObj->pMuunInventory[n].m_Level, lpObj->pMuunInventory[n].m_Option1, lpObj->pMuunInventory[n].m_Option2, lpObj->pMuunInventory[n].m_Option3,
					lpObj->pMuunInventory[n].GetMuunItemRank(), lpObj->pMuunInventory[n].IsMuunItemPeriodExpire(), lpObj->pMuunInventory[n].m_Number,
					(int)lpObj->pMuunInventory[n].m_Durability, NewOption[0], NewOption[1], NewOption[2], NewOption[3], NewOption[4], NewOption[5], NewOption[6],
					lpObj->pMuunInventory[n].m_SetOption, lpObj->pMuunInventory[n].m_ItemOptionEx >> 7,
					g_kJewelOfHarmonySystem.GetItemStrengthenOption(&lpObj->pMuunInventory[n]), g_kJewelOfHarmonySystem.GetItemOptionLevel(&lpObj->pMuunInventory[n]),
					lpObj->pMuunInventory[n].m_SocketOption[0], lpObj->pMuunInventory[n].m_SocketOption[1], lpObj->pMuunInventory[n].m_SocketOption[2], lpObj->pMuunInventory[n].m_SocketOption[3], lpObj->pMuunInventory[n].m_SocketOption[4],
					lpObj->pMuunInventory[n].m_BonusSocketOption);
			}
		}

		else
		{
			BYTE NewOption[MAX_EXOPTION_SIZE];
			ItemIsBufExOption(NewOption, &lpObj->pMuunInventory[n]);

			g_Log.Add("[%s][%s]MI[%d,%s,%d,%d,%d,%d] Rank:[%d] Expire:[%d] serial:[%I64d] dur:[%d] Ex:[%d,%d,%d,%d,%d,%d,%d] Set[%d] 380:[%d] HO:[%d,%d] SC[%d,%d,%d,%d,%d] BonusOption[%d]",
				lpObj->AccountID, lpObj->Name, n, lpObj->pMuunInventory[n].GetName(),
				lpObj->pMuunInventory[n].m_Level, lpObj->pMuunInventory[n].m_Option1, lpObj->pMuunInventory[n].m_Option2, lpObj->pMuunInventory[n].m_Option3,
				lpObj->pMuunInventory[n].GetMuunItemRank(), lpObj->pMuunInventory[n].IsMuunItemPeriodExpire(), lpObj->pMuunInventory[n].m_Number,
				(int)lpObj->pMuunInventory[n].m_Durability, NewOption[0], NewOption[1], NewOption[2], NewOption[3], NewOption[4], NewOption[5], NewOption[6],
				lpObj->pMuunInventory[n].m_SetOption, lpObj->pMuunInventory[n].m_ItemOptionEx >> 7,
				g_kJewelOfHarmonySystem.GetItemStrengthenOption(&lpObj->pMuunInventory[n]), g_kJewelOfHarmonySystem.GetItemOptionLevel(&lpObj->pMuunInventory[n]),
				lpObj->pMuunInventory[n].m_SocketOption[0], lpObj->pMuunInventory[n].m_SocketOption[1], lpObj->pMuunInventory[n].m_SocketOption[2], lpObj->pMuunInventory[n].m_SocketOption[3], lpObj->pMuunInventory[n].m_SocketOption[4],
				lpObj->pMuunInventory[n].m_BonusSocketOption);
		}
	}

	g_Log.Add("[MuunInvenItem] [%s][%s] Muun Item Save Complete.", lpObj->AccountID, lpObj->Name);

	_tagSDHP_REQ_DBMUUN_INVEN_SAVE pMsg;
	memcpy(pMsg.AccountID, lpObj->AccountID, MAX_ACCOUNT_LEN+1);
	memcpy(pMsg.Name, lpObj->Name, MAX_ACCOUNT_LEN+1);

	pMsg.h.c = 0xC2;
	pMsg.h.sizeH = SET_NUMBERH(sizeof(pMsg));
	pMsg.h.sizeL = SET_NUMBERL(sizeof(pMsg));
	pMsg.h.headcode = 0xF2;
	ItemByteConvert32((LPBYTE)pMsg.dbInventory, lpObj->pMuunInventory, MUUN_INVENTORY_SIZE);
	
	wsDataCli.DataSend((char*)&pMsg, sizeof(pMsg));
}

void CMuunSystem::GCSendConditionStatus(int aIndex, int iPos, int iStatus)
{
	PMSG_MUUNITEM_CONDITION_STATUS pMsg;

	pMsg.btStatus = iStatus;
	pMsg.btIPos = iPos;
	PHeadSubSetB((LPBYTE)&pMsg, 0x4E, 0x07, sizeof(pMsg));

	this->MsgIsMuunItemActive(&gObj[aIndex], iPos);
	IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

bool CMuunSystem::IsMuunItem(CItem *pCitem)
{
	if (!pCitem)
	{
		return false;
	}

	LPITEM_ATTRIBUTE pItemAttribute = GetItemAttr(pCitem->m_Type);

	if (!pItemAttribute)
	{
		return false;
	}
	
	return pItemAttribute->ItemKindA == 12 && (pItemAttribute->ItemKindB >= 63 && pItemAttribute->ItemKindB <= 66);
}

bool CMuunSystem::IsMuunItem(int iItemNum)
{
	LPITEM_ATTRIBUTE pItemAttribute = GetItemAttr(iItemNum);

	if (!pItemAttribute)
	{
		return false;
	}

	return pItemAttribute->ItemKindA == 12 && (pItemAttribute->ItemKindB >= 63 && pItemAttribute->ItemKindB <= 66);
}

bool CMuunSystem::IsStoneofEvolution(CItem *pCitem)
{
	if (!pCitem)
	{
		return false;
	}

	LPITEM_ATTRIBUTE pItemAttribute = GetItemAttr(pCitem->m_Type);

	if (!pItemAttribute)
	{
		return false;
	}

	return pItemAttribute->ItemKindA == 12 && pItemAttribute->ItemKindB == 0;
}

bool CMuunSystem::IsStoneofEvolution(int iItemNum)
{
	LPITEM_ATTRIBUTE pItemAttribute = GetItemAttr(iItemNum);

	if (!pItemAttribute)
	{
		return false;
	}

	return pItemAttribute->ItemKindA == 12 && pItemAttribute->ItemKindB == 0;
}

int CMuunSystem::GetEvolutionMuunItemIndex(int iItemNum)
{
	CMuunInfo *pCMuunInfo = this->m_MuunInfoMng.GetMuunItemNumToMuunInfo(iItemNum);

	if (!pCMuunInfo)
	{
		g_Log.Add("[MuunSystem][Error]GetEvolutionMuunItemIndex() pItemAttribute is NULL %d", __LINE__);
		return 0;
	}

	return pCMuunInfo->GetEvolutionMuunNum();
}

int CMuunSystem::GetBeforeEvolutionMuunItemIndex(int iItemNum)
{
	return this->m_MuunInfoMng.GetBeforeEvolutionMuunItemIndex(iItemNum);
}

int CMuunSystem::GetMuunRankOfMuunInfo(int iItemNum)
{
	CMuunInfo *pCMuunInfo = this->m_MuunInfoMng.GetMuunItemNumToMuunInfo(iItemNum);

	if (!pCMuunInfo)
	{
		g_Log.Add("[MuunSystem][Error] GetMuunRankOfMuunInfo() pCMuunInfo is NULL %d", __LINE__);
		return 0;
	}

	return pCMuunInfo->GetMuunRank();
}

void CMuunSystem::CGMuunInventoryUseItemRecv(PMSG_USEITEM_MUUN_INVEN *lpMsg, int aIndex)
{
	int iItemUseType = lpMsg->btItemUseType;

	if (gObj[aIndex].m_IfState.use == 1)
	{
		g_Log.Add("[%s][%s]_If return  %d", gObj[aIndex].AccountID, gObj[aIndex].Name, __LINE__);
		this->GCMuunInventoryUseItemResult(aIndex, iItemUseType, 1);

		return;
	}

	if (gObj[aIndex].CloseType != -1)
	{
		g_Log.Add("[%s][%s] CGMuunInventoryUseItemRecv()_CloseType return %d", gObj[aIndex].AccountID, gObj[aIndex].Name, __LINE__);
		this->GCMuunInventoryUseItemResult(aIndex, iItemUseType, 1);

		return;
	}

	if (!::gObjFixMuunInventoryPointer(aIndex))
	{
		g_Log.Add("[Fix Inv.Ptr] False Location - %d", __LINE__);
	}

	if (gObj[aIndex].pTransaction == 1)
	{
		g_Log.Add("[%s][%s] CGUseItemRecv() Failed : Transaction == 1, IF_TYPE : %d", gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].m_IfState.type);
		this->GCMuunInventoryUseItemResult(aIndex, iItemUseType, 1);

		return;
	}

	if (g_NewPVP.IsObserver(gObj[aIndex]))
	{
		GSProtocol.GCServerMsgStringSend(Lang.GetText(0,562), aIndex, 1);
		this->GCMuunInventoryUseItemResult(aIndex, iItemUseType, 1);

		return;
	}

	if (lpMsg->inventoryPos == lpMsg->invenrotyTarget)
	{
		g_Log.Add("error-L1 : [%s][%s] CGMuunInventoryUseItemRecv()_Pos return %d", gObj[aIndex].AccountID, gObj[aIndex].Name, __LINE__);
		this->GCMuunInventoryUseItemResult(aIndex, iItemUseType, 1);

		return;
	}

	switch (iItemUseType)
	{
		case 1:
			if (this->MuunItemLevelUp(&gObj[aIndex], lpMsg->inventoryPos, lpMsg->invenrotyTarget) == FALSE)
			{
				g_Log.Add("[MuunSystem][LevelUp] [%s][%s] [Fail]", gObj[aIndex].AccountID, gObj[aIndex].Name);
				this->GCMuunInventoryUseItemResult(aIndex, iItemUseType, 1);

				return;
			}

			this->ClearPeriodMuunItemData(&gObj[aIndex], gObj[aIndex].pMuunInventory[lpMsg->inventoryPos].m_Type, gObj[aIndex].pMuunInventory[lpMsg->inventoryPos].m_Number);
			gObj[aIndex].pMuunInventory[lpMsg->inventoryPos].Clear();
			GSProtocol.GCMuunInventoryItemDeleteSend(aIndex, lpMsg->inventoryPos, 1);
			GSProtocol.GCMuunInventoryItemOneSend(aIndex, lpMsg->invenrotyTarget);
			break;

		case 2:
			if (this->MuunItemEvolution(&gObj[aIndex], lpMsg->inventoryPos, lpMsg->invenrotyTarget) == FALSE)
			{
				g_Log.Add("[MuunSystem] [%s][%s]Item Use Muun Item Evolution Fail", gObj[aIndex].AccountID, gObj[aIndex].Name);
				this->GCMuunInventoryUseItemResult(aIndex, iItemUseType, 1);

				return;
			}

			gObj[aIndex].pMuunInventory[lpMsg->inventoryPos].Clear();
			GSProtocol.GCMuunInventoryItemDeleteSend(aIndex, lpMsg->inventoryPos, 1);
			break;

		case 3:
			if (this->MuunItemLifeGem(&gObj[aIndex], lpMsg->inventoryPos, lpMsg->invenrotyTarget) == FALSE)
			{
				g_Log.Add("[MuunSystem] [%s][%s]Item Use Muun Item LifeGem Fail", gObj[aIndex].AccountID, gObj[aIndex].Name);
				this->GCMuunInventoryUseItemResult(aIndex, iItemUseType, 1);

				return;
			}

			gObjInventoryItemSet(aIndex, lpMsg->inventoryPos, -1);
			gObj[aIndex].pInventory[lpMsg->inventoryPos].Clear();
			GSProtocol.GCInventoryItemDeleteSend(aIndex, lpMsg->inventoryPos, 1);
			GSProtocol.GCMuunItemDurSend(aIndex, lpMsg->invenrotyTarget, -1);
			break;

		case 4:
			if (this->MuunItemEnergyGenerator(&gObj[aIndex], lpMsg->inventoryPos, lpMsg->invenrotyTarget) == FALSE)
			{
				g_Log.Add("[MuunSystem] [%s][%s]Item Use Muun Item Energy Generator Fail", gObj[aIndex].AccountID, gObj[aIndex].Name);
				this->GCMuunInventoryUseItemResult(aIndex, iItemUseType, 1);

				return;
			}

			this->ClearPeriodMuunItemData(&gObj[aIndex], gObj[aIndex].pMuunInventory[lpMsg->inventoryPos].m_Type, gObj[aIndex].pMuunInventory[lpMsg->inventoryPos].m_Number);
			gObj[aIndex].pMuunInventory[lpMsg->inventoryPos].Clear();
			GSProtocol.GCMuunInventoryItemDeleteSend(aIndex, lpMsg->inventoryPos, 1);
			GSProtocol.GCMuunInventoryItemOneSend(aIndex, lpMsg->invenrotyTarget);
			break;
	}

	this->GCMuunInventoryUseItemResult(aIndex, iItemUseType, 0);
}
			

bool CMuunSystem::MuunItemEvolution(OBJECTSTRUCT *lpObj, int source, int target)
{
	if (source < 2 || source > MUUN_INVENTORY_SIZE - 1)
	{
		return false;
	}

	if (target < 2 || target > MUUN_INVENTORY_SIZE - 1)
	{
		return false;
	}

	if (lpObj->pMuunInventory[source].IsItem() == FALSE)
	{
		return false;
	}

	if (lpObj->pMuunInventory[target].IsItem() == FALSE)
	{
		return false;
	}

	if (this->IsStoneofEvolution(lpObj->pMuunInventory[source].m_Type) == FALSE)
	{
		return false;
	}

	if (this->IsMuunItem(lpObj->pMuunInventory[target].m_Type) == FALSE)
	{
		return false;
	}

	if (lpObj->pMuunInventory[target].m_Level == 0)
	{
		return false;
	}

	WORD nEvoMuunItemNum = (lpObj->pMuunInventory[source].m_SocketOption[0] << 8) | lpObj->pMuunInventory[source].m_SocketOption[1];

	if (nEvoMuunItemNum != lpObj->pMuunInventory[target].m_Type)
	{
		return false;
	}

	if (lpObj->pMuunInventory[target].GetMuunItemRank() + 1 != lpObj->pMuunInventory[target].m_Level)
	{
		return false;
	}

	if (lpObj->pMuunInventory[target].GetMuunItemRank() + 1 < lpObj->pMuunInventory[target].m_Level)
	{
		g_Log.Add("[MuunSystem][Error][%s][%s] Over Level(%d) %d", lpObj->AccountID, lpObj->Name, lpObj->pMuunInventory[target].m_Level, __LINE__);
		return false;
	}

	WORD nEvolutionMuunNum = this->GetEvolutionMuunItemIndex(lpObj->pMuunInventory[target].m_Type);

	if (nEvolutionMuunNum == 0)
	{
		g_Log.Add("[MuunSystem][Error][%s][%s] Not Evolution MuunItem Index - target Item Index(%d) %d", lpObj->AccountID, lpObj->Name, lpObj->pMuunInventory[target].m_Type, __LINE__);
		return false;
	}

	LPITEM_ATTRIBUTE pSItemAttribute = GetItemAttr(lpObj->pMuunInventory[source].m_Type);
	LPITEM_ATTRIBUTE pTItemAttribute = GetItemAttr(lpObj->pMuunInventory[target].m_Type);

	if (pTItemAttribute && pSItemAttribute)
	{
		UINT64 serial = lpObj->pMuunInventory[target].m_Number;
		WORD Level = lpObj->pMuunInventory[target].m_Level;
		int iMuunRank = lpObj->pMuunInventory[target].GetMuunItemRank();

		g_Log.Add("[MuunSystem][Evolution] [%s][%s] [Success] Source: [%s] Pos[%d] Serial[%I64d] - Target: [%s] Pos[%d] Rank[%d] Level[%d] Serial[%I64d]",
			lpObj->AccountID, lpObj->Name, pSItemAttribute->Name, source, lpObj->pMuunInventory[source].m_Number, pTItemAttribute->Name, target, iMuunRank, Level, serial);
	}

	float fDur = lpObj->pMuunInventory[target].m_Durability;

	this->ClearPeriodMuunItemData(lpObj, lpObj->pMuunInventory[target].m_Type, lpObj->pMuunInventory[target].m_Number);
	GSProtocol.GCMuunInventoryItemDeleteSend(lpObj->m_Index, target, 1);

	lpObj->pMuunInventory[target].Clear();

	BYTE SocketOption[5];
	memset(SocketOption, -1, sizeof(SocketOption));

	ItemSerialCreateSend(lpObj->m_Index, 0xE0, 0, 0, nEvolutionMuunNum, 0, fDur, 0, 0, 0, lpObj->m_Index, 0, 0, 0, SocketOption, 0);

	return true;
}

bool CMuunSystem::MuunItemLevelUp(OBJECTSTRUCT *lpObj, int source, int target)
{
	if (source < 2 || source > MUUN_INVENTORY_SIZE - 1)
	{
		return false;
	}

	if (target < 2 || target > MUUN_INVENTORY_SIZE - 1)
	{
		return false;
	}

	if (lpObj->pMuunInventory[source].IsItem() == FALSE)
	{
		return false;
	}

	if (lpObj->pMuunInventory[target].IsItem() == FALSE)
	{
		return false;
	}

	if (this->IsMuunItem(lpObj->pMuunInventory[target].m_Type) == FALSE)
	{
		return false;
	}
	
	if (this->IsMuunItem(lpObj->pMuunInventory[source].m_Type) == FALSE)
	{
		return false;
	}

	if (lpObj->pMuunInventory[source].m_Level == 0 || lpObj->pMuunInventory[target].m_Level == 0)
	{
		return false;
	}
							
	if (lpObj->pMuunInventory[source].m_Type != lpObj->pMuunInventory[target].m_Type)
	{
		return false;
	}
	
	if (lpObj->pMuunInventory[source].m_Level != 1)
	{
		return false;
	}

	if (lpObj->pMuunInventory[target].GetMuunItemRank() + 1 == lpObj->pMuunInventory[target].m_Level)
	{
		return false;
	}
										
	if (lpObj->pMuunInventory[target].m_Level >= 5)
	{
		return false;
	}

	if (lpObj->pMuunInventory[target].GetMuunItemRank() + 1 < lpObj->pMuunInventory[target].m_Level)
	{
		g_Log.Add("[MuunSystem][Error][%s][%s] Over Level(%d) %d", lpObj->AccountID, lpObj->Name, lpObj->pMuunInventory[target].m_Level, __LINE__);
		return false;
	}

	int nBeforeLv = lpObj->pMuunInventory[target].m_Level;
	lpObj->pMuunInventory[target].m_Level++;

	LPITEM_ATTRIBUTE pSItemAttribute = GetItemAttr(lpObj->pMuunInventory[source].m_Type);
	LPITEM_ATTRIBUTE pTItemAttribute = GetItemAttr(lpObj->pMuunInventory[target].m_Type);
												
	if (pTItemAttribute && pSItemAttribute)
	{
		g_Log.Add("[MuunSystem][LevelUp] [%s][%s] [Success] Source: [%s] Pos[%d] Rank[%d] Level[%d] Serial[%I64d] - Target: [%s] Pos[%d] Rank[%d] Level[%d]/[%d] Serial[%I64d]",
			lpObj->AccountID, lpObj->Name, pSItemAttribute->Name, source, lpObj->pMuunInventory[source].GetMuunItemRank(), lpObj->pMuunInventory[source].m_Level, lpObj->pMuunInventory[source].m_Number,
			pTItemAttribute->Name, target, lpObj->pMuunInventory[target].GetMuunItemRank(), nBeforeLv, lpObj->pMuunInventory[target].m_Level, lpObj->pMuunInventory[target].m_Number);
	}

	return true;
}

bool CMuunSystem::MuunItemLifeGem(OBJECTSTRUCT *lpObj, int source, int target)
{
	if (source < INVETORY_WEAR_SIZE || source > MAIN_INVENTORY_SIZE - 1)
	{
		return false;
	}

	if (target < 2 || target > MUUN_INVENTORY_SIZE - 1)
	{
		return false;
	}

	if (lpObj->pInventory[source].IsItem() == FALSE)
	{
		return false;
	}

	if (lpObj->pMuunInventory[target].IsItem() == FALSE)
	{
		return false;
	}

	if (this->IsMuunItem(lpObj->pMuunInventory[target].m_Type) == FALSE)
	{
		return false;
	}
	
	if (lpObj->pInventory[source].m_Type != ITEMGET(14, 16))
	{
		return false;
	}

	if (lpObj->pMuunInventory[target].m_Durability >= 255.0)
	{
		return false;
	}

	int nBeforeDur = lpObj->pMuunInventory[target].m_Durability;
	lpObj->pMuunInventory[target].m_Durability = 255.0;

	LPITEM_ATTRIBUTE pItemAttribute = GetItemAttr(lpObj->pInventory[source].m_Type);
							
	if (pItemAttribute)
	{
		g_Log.Add("[MuunSystem] [%s][%s] Item Use Muun Item LifeGem Success target[%d] [%s] Dur:[%d]/[%d] Serial:%I64d",
			lpObj->AccountID, lpObj->Name, target, pItemAttribute->Name, nBeforeDur, (int)lpObj->pMuunInventory[target].m_Durability, lpObj->pMuunInventory[target].m_Number);
	}

	return true;
}

bool CMuunSystem::MuunItemEnergyGenerator(LPOBJ lpObj, int source, int target)
{
	if (source < 2 || source > MUUN_INVENTORY_SIZE - 1)
	{
		return false;
	}

	if (target < 2 || target > MUUN_INVENTORY_SIZE - 1)
	{
		return false;
	}

	if (lpObj->pMuunInventory[source].IsItem() == FALSE)
	{
		return false;
	}

	if (lpObj->pMuunInventory[target].IsItem() == FALSE)
	{
		return false;
	}

	int nEnergy = 0;
	int nRankEnergy = 0;
	int nLvEnergy = 0;

	if (this->IsStoneofEvolution(lpObj->pMuunInventory[source].m_Type) == FALSE &&
		this->IsMuunItem(lpObj->pMuunInventory[source].m_Type) == FALSE &&
		lpObj->pMuunInventory[source].m_Type != ITEMGET(13,239))
	{
		return false;
	}

	if (lpObj->pMuunInventory[target].m_Type != ITEMGET(13,239))
	{
		return false;
	}

	int nMuunRank = lpObj->pMuunInventory[source].GetMuunItemRank();
	int nMuunLv = lpObj->pMuunInventory[source].m_Level;

	if (this->IsMuunItem(lpObj->pMuunInventory[source].m_Type) == TRUE)
	{
		for (int i = 0; i < 8; i++)
		{
			if ( nMuunRank == this->m_stEnergyGeneratorPoint[i].iRank )
			{
				nRankEnergy = this->m_stEnergyGeneratorPoint[i].iRankPt;
			}

			if ( nMuunLv == this->m_stEnergyGeneratorPoint[i].iMuunLv )
			{
				nLvEnergy = this->m_stEnergyGeneratorPoint[i].iMuunLvPt;
			}
		}

		if (nMuunLv != 0)
		{
			nEnergy = nLvEnergy * nRankEnergy;
		}

		else
		{
			nEnergy = nLvEnergy * (nRankEnergy + nRankEnergy * nRankEnergy);
		}
	}

	else if (this->IsStoneofEvolution(lpObj->pMuunInventory[source].m_Type) == TRUE)
	{
		nEnergy = this->m_iStoneOfEvolutionPt;
	}

	else if (lpObj->pMuunInventory[source].m_Type == ITEMGET(13,239))
	{
		nEnergy = lpObj->pMuunInventory[source].m_Durability;
	}

	int nMuunDurability = nEnergy + lpObj->pMuunInventory[target].m_Durability;

	if (nMuunDurability < 120)
	{
		LPITEM_ATTRIBUTE pSItemAttribute = GetItemAttr(lpObj->pMuunInventory[source].m_Type);
		LPITEM_ATTRIBUTE pTItemAttribute = GetItemAttr(lpObj->pMuunInventory[target].m_Type);
												
		if (pTItemAttribute && pSItemAttribute)
		{
			g_Log.Add("[MuunSystem][EnergyGenerator] [%s][%s] [Mix Success] Source: [%s] Pos[%d] Rank[%d] Level[%d] Serial[%I64d] - Target: [%s] Pos[%d] Serial[%I64d] Dur[%d/%d/%d]",
				lpObj->AccountID, lpObj->Name, pSItemAttribute->Name, source, lpObj->pMuunInventory[source].m_Durability, lpObj->pMuunInventory[source].m_Level, lpObj->pMuunInventory[source].m_Number,
				pTItemAttribute->Name, target, lpObj->pInventory[target].m_Number, lpObj->pMuunInventory[target].m_Durability, nEnergy, nMuunDurability);
		}

		lpObj->pMuunInventory[target].m_Durability = nMuunDurability;
		GSProtocol.GCMuunItemDurSend(lpObj->m_Index, target, nMuunDurability);
	}

	else
	{
		LPITEM_ATTRIBUTE pSItemAttribute = GetItemAttr(lpObj->pMuunInventory[source].m_Type);
		LPITEM_ATTRIBUTE pTItemAttribute = GetItemAttr(lpObj->pMuunInventory[target].m_Type);
												
		if (pTItemAttribute && pSItemAttribute)
		{
			g_Log.Add("[MuunSystem][EnergyGenerator] [%s][%s] [Create Success] Source: [%s] Pos[%d] Rank[%d] Level[%d] Serial[%I64d] - Target: [%s] Pos[%d] Serial[%I64d] Dur[%d/%d/%d]",
				lpObj->AccountID, lpObj->Name, pSItemAttribute->Name, source, lpObj->pMuunInventory[source].m_Durability, lpObj->pMuunInventory[source].m_Level, lpObj->pMuunInventory[source].m_Number,
				pTItemAttribute->Name, target, lpObj->pInventory[target].m_Number, lpObj->pMuunInventory[target].m_Durability, nEnergy, nMuunDurability);
		}

		int iType = ITEMGET(13,240);
		int iLevel = 0;
		float fDur = 255.0;

		BYTE SocketOption[5];
		memset(SocketOption, -1, sizeof(SocketOption));

		ItemSerialCreateSend(lpObj->m_Index, 0xE0, 0, 0, iType, iLevel, fDur, 0, 0, 0, lpObj->m_Index, 0, 0, 0, SocketOption, 0);

		lpObj->pMuunInventory[target].Clear();
		GSProtocol.GCMuunInventoryItemDeleteSend(lpObj->m_Index, target, 1);
	}

	return true;
}

void CMuunSystem::GCMuunInventoryUseItemResult(int aIndex, int iUseType, int iResult)
{
	PMSG_USEITEM_MUUN_INVEN_RESULT pResult; 

	pResult.btItemUseType = iUseType;
	pResult.btResult = iResult;
	PHeadSubSetB((LPBYTE)&pResult, 0x4E, 0x08, sizeof(pResult));
	IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
}

int CMuunSystem::AddMuunItemPeriodInfo(OBJECTSTRUCT *lpObj)
{
	for (int i = 0; i < g_ConfigRead.server.GetObjectMaxUser(); i++)
	{
		if (this->m_MuunItemPeriodData[i].bIsUsed == false)
		{
			this->m_MuunItemPeriodData[i].Clear();
			this->m_MuunItemPeriodData[i].bIsUsed = true;
			this->m_MuunItemPeriodData[i].lpUserObj = lpObj;
			this->m_MuunItemPeriodData[i].dwUserGUID = lpObj->DBNumber;
			this->m_MuunItemPeriodData[i].wUserIndex = lpObj->m_Index;
			this->m_MuunItemPeriodData[i].btUsedDataCount = 0;
			memcpy(this->m_MuunItemPeriodData[i].chAccountID, lpObj->Name, MAX_ACCOUNT_LEN + 1);
			memcpy(this->m_MuunItemPeriodData[i].chCharacterName, lpObj->AccountID, MAX_ACCOUNT_LEN + 1);

			lpObj->m_iMuunItmePeriodDataIndex = i;
			return i;
		}
	}

	return -1;
}

bool CMuunSystem::RemoveMuunItemPeriodInfo(OBJECTSTRUCT *lpObj)
{
	if (this->IsCorrectUser(lpObj) == false)
	{
		return false;
	}

	int iPeriodItemSlotIndex = lpObj->m_iMuunItmePeriodDataIndex;

	if (iPeriodItemSlotIndex < 0 || iPeriodItemSlotIndex >= g_ConfigRead.server.GetObjectMaxUser())
	{
		return false;
	}

	if (this->m_MuunItemPeriodData[iPeriodItemSlotIndex].bIsUsed == false)
	{
		return false;
	}

	this->m_MuunItemPeriodData[iPeriodItemSlotIndex].Clear();

	return true;
}

bool CMuunSystem::IsCorrectUser(OBJECTSTRUCT *lpObj)
{
	int iPeriodItemSlotIndex = lpObj->m_iMuunItmePeriodDataIndex;

	if (iPeriodItemSlotIndex < 0 || iPeriodItemSlotIndex >= g_ConfigRead.server.GetObjectMax())
	{
		return false;
	}

	if (this->m_MuunItemPeriodData[iPeriodItemSlotIndex].bIsUsed == FALSE)
	{
		return false;
	}

	if (this->m_MuunItemPeriodData[iPeriodItemSlotIndex].dwUserGUID != lpObj->DBNumber)
	{
		return false;
	}

	return true;
}

int CMuunSystem::AddMuunItmePeriodData(OBJECTSTRUCT *lpObj, WORD wItemCode, UINT64 dwSerial, int iDuration, CMuunInfo *pCMuunInfo)
{
	int iMuunItemPeriodDataIndex = lpObj->m_iMuunItmePeriodDataIndex;

	if (CMuunSystem::IsCorrectUser(lpObj) == false)
	{
		return -1;
	}

	for (int i = 0; i < MUUN_INVENTORY_SIZE; i++)
	{
		if (!this->m_MuunItemPeriodData[iMuunItemPeriodDataIndex].ItemData[i].bIsUsed)
		{
			this->GetExpireDate(iDuration);
			this->m_MuunItemPeriodData[iMuunItemPeriodDataIndex].ItemData[i].bIsUsed = TRUE;
			this->m_MuunItemPeriodData[iMuunItemPeriodDataIndex].ItemData[i].wItemCode = wItemCode;
			this->m_MuunItemPeriodData[iMuunItemPeriodDataIndex].ItemData[i].dwSerial = dwSerial;
			this->m_MuunItemPeriodData[iMuunItemPeriodDataIndex].ItemData[i].pCMuunInfo = pCMuunInfo;
			return i;
		}
	}

	return -1;
}

void CMuunSystem::CheckMuunItemPeriodData(OBJECTSTRUCT *lpObj)
{
	if (lpObj == NULL)
	{
		g_Log.Add("[MuunSystem][Error] lpObj is NULL [%d]", __LINE__);
		return;
	}

	this->SkillProc(lpObj);

	if (GetTickCount() - lpObj->dwCheckMuunItemTime >= 60000)
	{
		lpObj->dwCheckMuunItemTime = GetTickCount();
		int i = lpObj->m_iMuunItmePeriodDataIndex;

		if (i < 0 || i >= g_ConfigRead.server.GetObjectMaxUser())
		{
			return;
		}
			
		if (this->m_MuunItemPeriodData[i].bIsUsed == TRUE)
		{
			if (this->m_MuunItemPeriodData[i].lpUserObj != NULL)
			{
				LPOBJ lpUserObj = this->m_MuunItemPeriodData[i].lpUserObj;
					
				if (lpUserObj->Type == OBJ_USER)
				{
					if (lpUserObj->Connected == PLAYER_PLAYING)
					{
						this->CheckEquipMuunItemConditionProc(lpUserObj);

						for (int iItemSlotIndex = 0; iItemSlotIndex < MUUN_INVENTORY_SIZE; iItemSlotIndex++)
						{
							if (this->m_MuunItemPeriodData[i].ItemData[iItemSlotIndex].bIsUsed == TRUE)
							{
								CMuunInfo *pCMuunInfo = this->m_MuunItemPeriodData[i].ItemData[iItemSlotIndex].pCMuunInfo;
									
								if (!pCMuunInfo)
								{
									g_Log.Add("[MuunSystem][Error] pCMuunInfo is NULL [%s][%s] [%d]", lpUserObj->AccountID, lpUserObj->Name, __LINE__);								
									return;
								}
								
								if (this->CheckAddOptionExpireDate(pCMuunInfo->GetAddOptStart(), pCMuunInfo->GetAddOptEnd()) == 1)
								{
									this->RemovePeriodMunnItemData(lpUserObj, this->m_MuunItemPeriodData[i].ItemData[iItemSlotIndex].wItemCode, this->m_MuunItemPeriodData[i].ItemData[iItemSlotIndex].dwSerial);							
									this->m_MuunItemPeriodData[i].ItemData[iItemSlotIndex].Clear();
								}
							}
						}
					}
				}
			}
		}
	}
}

bool CMuunSystem::ClearPeriodMuunItemData(OBJECTSTRUCT *lpObj, WORD wItemCode, UINT64 dwSerial)
{
	bool result;
	signed int iItemSlotIndex; 

	if (lpObj)
	{
		if (lpObj->Type == OBJ_USER && lpObj->Connected >= PLAYER_LOGGED)
		{
			if (this->IsMuunItem(wItemCode))
			{
				int i = lpObj->m_iMuunItmePeriodDataIndex;
				if (this->m_MuunItemPeriodData[i].bIsUsed == TRUE)
				{
					for (iItemSlotIndex = 0; iItemSlotIndex < MUUN_INVENTORY_SIZE; ++iItemSlotIndex)
					{
						if (this->m_MuunItemPeriodData[i].ItemData[iItemSlotIndex].bIsUsed == 1
							&& this->m_MuunItemPeriodData[i].ItemData[iItemSlotIndex].wItemCode == wItemCode
							&& this->m_MuunItemPeriodData[i].ItemData[iItemSlotIndex].dwSerial == dwSerial)
						{
							this->m_MuunItemPeriodData[i].ItemData[iItemSlotIndex].Clear();
							return 1;
						}
					}
				}
				result = 0;
			}
			else
			{
				result = 0;
			}
		}
		else
		{
			result = 0;
		}
	}
	else
	{
		result = 0;
	}
	return result;
}

bool CMuunSystem::RemovePeriodMunnItemData(OBJECTSTRUCT *lpObj, WORD wItemCode, UINT64 dwSerial)
{
	bool result; 
	int iInventoryPosition; 

	if (lpObj)
	{
		if (lpObj->Type == 1 && lpObj->Connected >= 2)
		{
			iInventoryPosition = this->GetItemFromMuunInventory(lpObj, wItemCode, dwSerial);
			if (iInventoryPosition == -1)
			{
				result = 0;
			}
			else
			{
				this->SetDisableMuunItemToExpire(lpObj, iInventoryPosition);
				result = 1;
			}
		}
		else
		{
			result = 0;
		}
	}
	else
	{
		result = 0;
	}
	return result;
}

bool CMuunSystem::SetDisableMuunItemToExpire(OBJECTSTRUCT *lpObj, int iInventoryPosition)
{
	bool result; 

	if (lpObj)
	{
		if (lpObj->Connected >= 2 || lpObj->Type == 1)
		{
			if (iInventoryPosition == -1)
			{
				result = 0;
			}
			else
			{
				lpObj->pMuunInventory[iInventoryPosition].SetMuunItemPeriodExpire();
				if (iInventoryPosition < 2)
				{
					if (lpObj->pMuunInventory[iInventoryPosition].IsItem() == 1)
						this->CalCharacterStat(lpObj->m_Index,lpObj->m_MuunEffectList[iInventoryPosition].nOptType);
				}
				GCMuunInventoryItemListSend(lpObj->m_Index);
				result = 1;
			}
		}
		else
		{
			result = 0;
		}
	}
	else
	{
		result = 0;
	}
	return result;
}

void CMuunSystem::CheckMuunItemConditionLevelUp(OBJECTSTRUCT *lpObj)
{
	int nMaxLv; 
	CMuunOpt *pCMuunInfo; 
	int iRet = -1;
	for (int i = 0; i < 2; ++i)
	{
		if (lpObj->pMuunInventory[i].IsItem())
		{
			pCMuunInfo = lpObj->m_MuunEffectList[i].pCMuunInfo;
			if (!pCMuunInfo)
				return;
			if (pCMuunInfo->GetConditionType() == 2)
			{
				int nMinLv = pCMuunInfo->GetConditionVal1();
				nMaxLv = pCMuunInfo->GetConditionVal2();
				if (nMinLv > lpObj->Level || nMaxLv < lpObj->Level)
				{
					if (lpObj->m_MuunEffectList[i].bOptEnable == 1)
					{
						lpObj->m_MuunEffectList[i].bOptEnable = 0;
						iRet = 0;
					}
				}
				else
				{
					if (!lpObj->m_MuunEffectList[i].bOptEnable)
					{
						lpObj->m_MuunEffectList[i].bOptEnable = 1;
						iRet = 1;
					}
				}
				if (iRet > -1)
				{
					this->GCSendConditionStatus(lpObj->m_Index, i, iRet);
					this->CalCharacterStat(lpObj->m_Index,lpObj->m_MuunEffectList[i].pCMuunInfo);
				}
			}
		}
	}
}

void CMuunSystem::CheckMuunItemMoveMapConditionMap(OBJECTSTRUCT *lpObj, int iMapNumber)
{
	CMuunOpt *pCMuunInfo; 

	for (int i = 0; i < 2; ++i)
	{
		if (lpObj->pMuunInventory[i].IsItem() == 1)
		{
			pCMuunInfo = lpObj->m_MuunEffectList[i].pCMuunInfo;
			if (!pCMuunInfo)
				return;
			if (pCMuunInfo->GetConditionType() == 1)
			{
				if (pCMuunInfo->GetConditionVal1() == iMapNumber)
				{
					if (!lpObj->m_MuunEffectList[i].bOptEnable)
					{
						lpObj->m_MuunEffectList[i].bOptEnable = 1;
						this->GCSendConditionStatus(lpObj->m_Index, i, 1);
						this->CalCharacterStat(lpObj->m_Index,lpObj->m_MuunEffectList[i].pCMuunInfo);
					}
				}
				else
				{
					if (lpObj->m_MuunEffectList[i].bOptEnable == 1)
					{
						lpObj->m_MuunEffectList[i].bOptEnable = 0;
						this->GCSendConditionStatus(lpObj->m_Index, i, 0);
						this->CalCharacterStat(lpObj->m_Index,lpObj->m_MuunEffectList[i].pCMuunInfo);
					}
				}
			}
		}
	}
}

void CMuunSystem::CheckEquipMuunItemCondition(OBJECTSTRUCT *lpObj)
{
	 int iRet; 

	for (int i = 0; i < 2; ++i)
	{
		if (lpObj->pMuunInventory[i].IsItem() == 1)
		{
			iRet = this->CheckMuunItemCondition(lpObj,&lpObj->m_MuunEffectList[i],lpObj->m_MuunEffectList[i].pCMuunInfo);
			if (iRet > -1)
				this->CalCharacterStat(lpObj->m_Index,lpObj->m_MuunEffectList[i].pCMuunInfo);
			if (iRet < 0)
				iRet = 0;
			this->GCSendConditionStatus(lpObj->m_Index, i, iRet);
		}
	}
}

void CMuunSystem::CheckEquipMuunItemConditionProc(OBJECTSTRUCT *lpObj)
{

	signed int iRet; 

	for (int i = 0; i < 2; ++i)
	{
		if (lpObj->pMuunInventory[i].IsItem() == 1)
		{
			iRet = this->CheckMuunItemConditionProc(&lpObj->m_MuunEffectList[i],lpObj->m_MuunEffectList[i].pCMuunInfo);
			if (iRet > -1)
			{
				this->GCSendConditionStatus( lpObj->m_Index, i, iRet);
				this->CalCharacterStat(lpObj->m_Index,lpObj->m_MuunEffectList[i].pCMuunInfo);
			}
		}
	}
}

void CMuunSystem::CalCharacterStat(int aIndex, CMuunInfo *pCMuunInfo)
{
	if (!pCMuunInfo)
	{
		g_Log.Add("[MuunSystem][Error] CalCharacterStat() pCMuunInfo is NULL [%d]", __LINE__);
		return;
	}

	if (pCMuunInfo->GetOptType() == 7 || pCMuunInfo->GetOptType() == 1)
	{
		gObjCalCharacter.CalcCharacter(aIndex);
	}
}

void CMuunSystem::CalCharacterStat( int aIndex, int iOptType)
{
	if (iOptType == 7 || iOptType == 1)
	{
		gObjCalCharacter.CalcCharacter(aIndex);
	}
}

int CMuunSystem::CheckMuunItemCondition(OBJECTSTRUCT * lpObj, _tagMUUN_EFFECT_LIST *pUserMuunEffect, CMuunInfo *pCMuunInfo)
{
	if (pCMuunInfo == NULL)
	{
		g_Log.Add("[MuunSystem][Error] CheckMuunItemCondition() pCMuunInfo is NULL [%s][%s] [%d]", lpObj->AccountID, lpObj->Name, __LINE__);
		return -1;
	}

	switch (pCMuunInfo->GetConditionType())
	{
		case 3:
			return this->ChkMuunOptConditionTime(pUserMuunEffect, pCMuunInfo);
		case 4:
			return this->ChkMuunOptConditionDay(pUserMuunEffect, pCMuunInfo);
		case 2:
			return this->ChkMuunOptConditionLevel(lpObj, pUserMuunEffect, pCMuunInfo);
		case 1:
			return this->ChkMuunOptConditionMap(lpObj, pUserMuunEffect, pCMuunInfo);
		default:
			return -1;
	}
}

int CMuunSystem::CheckMuunItemConditionProc( _tagMUUN_EFFECT_LIST *pUserMuunEffect, CMuunInfo *pCMuunInfo)
{
	int result; 
	int iRet; 

	if (pCMuunInfo)
	{
		iRet = -1;
		int iConditionType = pCMuunInfo->GetConditionType();
		if (iConditionType == 3)
		{
			iRet = this->ChkMuunOptConditionTime(pUserMuunEffect, pCMuunInfo);
		}
		else
		{
			if (iConditionType == 4)
				iRet = this->ChkMuunOptConditionDay(pUserMuunEffect, pCMuunInfo);
		}
		result = iRet;
	}
	else
	{
		g_Log.Add(
			"[MuunSystem][Error] CheckMuunItemConditionProc() pCMuunInfo is NULL [%d]",
			__LINE__);
		result = -1;
	}
	return result;
}

int CMuunSystem::ChkMuunOptConditionTime(_tagMUUN_EFFECT_LIST *pUserMuunEffect, CMuunInfo *pCMuunInfo)
{
	if (pCMuunInfo == NULL)
	{
		g_Log.Add("[MuunSystem][Error] ChkMuunOptConditionTime() pCMuunInfo is NULL [%d]",__LINE__);
		return -1;
	}

	SYSTEMTIME tmToDay;
	int nStartTime = pCMuunInfo->GetConditionVal1();
	int nEndTime = pCMuunInfo->GetConditionVal2();
	GetLocalTime(&tmToDay);

	if (tmToDay.wHour < nStartTime || tmToDay.wHour >= nEndTime)
	{
		if (pUserMuunEffect->bOptEnable == TRUE)
		{
			pUserMuunEffect->bOptEnable = FALSE;
			return FALSE;
		}
	}

	else
	{
		if (pUserMuunEffect->bOptEnable == FALSE)
		{
			pUserMuunEffect->bOptEnable = TRUE;
			return TRUE;
		}
	}

	return -1;
}

int CMuunSystem::ChkMuunOptConditionDay( _tagMUUN_EFFECT_LIST *pUserMuunEffect, CMuunInfo *pCMuunInfo)
{
	SYSTEMTIME tmToDay; 
	char DayOfWeek[7] = { 64, 32, 16, 8, 4, 2, 1 };

	if (!pCMuunInfo)
	{
		g_Log.Add("[MuunSystem][Error] ChkMuunOptConditionDay() pCMuunInfo is NULL [%d]", __LINE__);
		return -1;
	}

	GetLocalTime(&tmToDay);

	if ((pCMuunInfo->GetConditionVal1() & DayOfWeek[tmToDay.wDayOfWeek]) != DayOfWeek[tmToDay.wDayOfWeek])
	{
		if (pUserMuunEffect->bOptEnable == TRUE)
		{
			pUserMuunEffect->bOptEnable = FALSE;
			return FALSE;
		}
	}

	else
	{
		if (pUserMuunEffect->bOptEnable == FALSE)
		{
			pUserMuunEffect->bOptEnable = TRUE;
			return TRUE;
		}
	}

	return -1;
}

int CMuunSystem::ChkMuunOptConditionLevel(OBJECTSTRUCT *lpObj, _tagMUUN_EFFECT_LIST *pUserMuunEffect, CMuunInfo *pCMuunInfo)
{
	if (!pCMuunInfo)
	{
		g_Log.Add("[MuunSystem][Error] ChkMuunOptConditionLevel() pCMuunInfo is NULL [%d]", __LINE__);
		return -1;
	}

	if (lpObj->Type != OBJ_USER)
	{
		g_Log.Add("[MuunSystem][Error] ChkMuunOptConditionLevel() lpObj->Type != OBJ_USER", __LINE__);
		return -1;
	}

	int nMinLv = pCMuunInfo->GetConditionVal1();
	int nMaxLv = pCMuunInfo->GetConditionVal2();

	if (nMinLv > (lpObj->Level+lpObj->m_PlayerData->MasterLevel) || nMaxLv < (lpObj->Level+lpObj->m_PlayerData->MasterLevel))
	{
		pUserMuunEffect->bOptEnable = FALSE;
		return FALSE;
	}

	else
	{
		pUserMuunEffect->bOptEnable = TRUE;
		return TRUE;
	}
}

int CMuunSystem::ChkMuunOptConditionMap(OBJECTSTRUCT *lpObj, _tagMUUN_EFFECT_LIST *pUserMuunEffect, CMuunInfo *pCMuunInfo)
{
	if (!pCMuunInfo)
	{
		g_Log.Add("[MuunSystem][Error] ChkMuunOptConditionMap() pCMuunInfo is NULL [%d]", __LINE__);
		return -1;
	}
		
	if (pCMuunInfo->GetConditionVal1() == lpObj->MapNumber)
	{
		pUserMuunEffect->bOptEnable = TRUE;
		return TRUE;
	}

	else
	{
		pUserMuunEffect->bOptEnable = FALSE;
		return FALSE;
	}
}

int CMuunSystem::GetItemFromMuunInventory(OBJECTSTRUCT *lpObj, WORD wItemCode, UINT64 dwSerial)
{
	if(!lpObj)
	{
		return -1;
	}

	if(lpObj->Type != OBJ_USER)
	{
		return -1;
	}

	if(lpObj->Connected < PLAYER_LOGGED)
	{
		return -1;
	}

	for (int i = 0; i < MUUN_INVENTORY_SIZE; i++)
	{
		if (lpObj->pMuunInventory[i].IsItem() == TRUE && lpObj->pMuunInventory[i].m_Type == wItemCode && lpObj->pMuunInventory[i].GetNumber() == dwSerial)
		{
			return i;
		}
	}

	return -1;
}

time_t CMuunSystem::GetCurrentDate()
{
	return time(NULL);
}

time_t CMuunSystem::GetExpireDate(int iDuration)
{
	time_t expiretime = time(NULL);

	expiretime += iDuration;

	return expiretime;
}

bool CMuunSystem::CheckExpireDate(time_t dwItemExpireDate)
{
	return this->GetCurrentDate() > dwItemExpireDate;
}

time_t CMuunSystem::GetLeftDate(time_t ExpireDate) // unused
{
	time_t currtime = time(NULL);
	time_t difftime = ExpireDate - currtime;

	return difftime;
}

bool CMuunSystem::CheckAddOptionExpireDate(time_t dwStartDate, time_t dwEndDate)
{
	bool result; 
	int dwCurrentDate; 

	dwCurrentDate = this->GetCurrentDate();
	if (dwCurrentDate >= dwStartDate)
		result = dwCurrentDate < dwStartDate || dwCurrentDate >dwEndDate;
	else
		result = 0;
	return result;
}

void CMuunSystem::MsgIsMuunItemActive(OBJECTSTRUCT *lpObj, int iPos)
{
	//trash

}

bool CMuunSystem::LoadScriptMuunExchange(char *lpszFileName)
{
	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(lpszFileName);

	if (res.status != pugi::status_ok)
	{
		g_Log.MsgBox("[MuunSystem] Cannot load %s file! (%s)", lpszFileName, res.description());
		return false;
	}

	pugi::xml_node main = file.child("MuunSystem");
	pugi::xml_node energy_converter = main.child("EnergyConvertSettings");

	this->m_iStoneOfEvolutionPt = energy_converter.attribute("StoneOfEvolutionPoint").as_int();

	int iCount = 0;

	for (pugi::xml_node muun = energy_converter.child("Muun"); muun; muun = muun.next_sibling())
	{
		if (iCount >= 8)
		{
			g_Log.MsgBox("Exceed max entries (StoneOfEvolutionPoint)");
			break;
		}

		this->m_stEnergyGeneratorPoint[iCount].iRank = muun.attribute("Rank").as_int();
		this->m_stEnergyGeneratorPoint[iCount].iRankPt = muun.attribute("RankPoint").as_int();
		this->m_stEnergyGeneratorPoint[iCount].iMuunLv = muun.attribute("Level").as_int();
		this->m_stEnergyGeneratorPoint[iCount].iMuunLvPt = muun.attribute("LevelPoint").as_int();
		iCount++;
	}

	pugi::xml_node required_items = main.child("RequiredItemsExchange");
	iCount = 0;

	for (pugi::xml_node item = required_items.child("Item"); item; item = item.next_sibling())
	{
		if (iCount >= 10)
		{
			g_Log.MsgBox("Exceed max entries (RequiredItemsExchange)");
			break;
		}

		this->m_stMuunExchangeInfo[iCount].iItemType = item.attribute("Cat").as_int();
		this->m_stMuunExchangeInfo[iCount].iItemIndex = item.attribute("Index").as_int();
		this->m_stMuunExchangeInfo[iCount].iInvenChk = item.attribute("InventoryCheck").as_int();
		this->m_stMuunExchangeInfo[iCount].iItemCnt = item.attribute("ItemCount").as_int();
		this->m_stMuunExchangeInfo[iCount].iItemBagIndex = item.attribute("ItemBagIndex").as_int();
		iCount++;
	}

	pugi::xml_node bag_list = main.child("BagListSettings");
	iCount = 0;

	for (pugi::xml_node bag = bag_list.child("Bag"); bag; bag = bag.next_sibling())
	{
		if (iCount >= 20)
		{
			g_Log.MsgBox("Exceed max entries (BagListSettings)");
			break;
		}

		this->m_stMuunExchangeItembag[iCount].iIndex = bag.attribute("Index").as_int();
		this->m_stMuunExchangeItembag[iCount].iItemType = bag.attribute("ItemCat").as_int();
		this->m_stMuunExchangeItembag[iCount].iItemIndex = bag.attribute("ItemIndex").as_int();
		this->m_stMuunExchangeItembag[iCount].iMinLv = bag.attribute("ItemMinLevel").as_int();
		this->m_stMuunExchangeItembag[iCount].iMaxLv = bag.attribute("ItemMaxLevel").as_int();
		this->m_stMuunExchangeItembag[iCount].iSkill = bag.attribute("Skill").as_int();
		this->m_stMuunExchangeItembag[iCount].iLuck = bag.attribute("Luck").as_int();
		this->m_stMuunExchangeItembag[iCount].iAddOpt = bag.attribute("Option").as_int();
		this->m_stMuunExchangeItembag[iCount].iExItem = bag.attribute("Exc").as_int();
		this->m_stMuunExchangeItembag[iCount].iStoneMuunItemType = bag.attribute("MuunEvoItemCat").as_int();
		this->m_stMuunExchangeItembag[iCount].iStoneMuunItemIndex = bag.attribute("MuunEvoItemIndex").as_int();
		this->m_stMuunExchangeItembag[iCount].iInvenChk = bag.attribute("InventoryCheck").as_int();
		iCount++;
	}

	return true;
}

void CMuunSystem::CGMuunExchangeItem(PMSG_REQ_MUUN_EXCHANGE *lpMsg, int aIndex)
{
	if (!ObjectMaxRange(aIndex))
	{
		return;
	}
    
	if (gObjIsConnected(aIndex) == FALSE)
	{
		return;
	}

	BYTE nSelect = lpMsg->btSelect;

	if (nSelect >= 10)
	{
		g_Log.Add("[MuunSystem][MuunExchange][Error][%s][%s] Select Over %d [%s, %d]", gObj[aIndex].AccountID, gObj[aIndex].Name, nSelect, __FILE__, __LINE__);
		return;
	}

	if (this->ChkMuunExchangeInvenNeedItem(&gObj[aIndex], nSelect, 0) == FALSE)
	{
		g_Log.Add("[MuunSystem][MuunExchange][FAIL][%s][%s] Lack of Materials", gObj[aIndex].AccountID, gObj[aIndex].Name);

		this->SendMsgMuunExchange(aIndex, 1);
		return;
	}
          
	if (this->ChkMuunExchangeInvenEmpty(&gObj[aIndex], nSelect) == FALSE)
	{
		g_Log.Add("[MuunSystem][MuunExchange][FAIL][%s][%s] Not Empty Space", gObj[aIndex].AccountID, gObj[aIndex].Name);

		this->SendMsgMuunExchange(aIndex, 2);
		return;
	}
            
	this->GDMuunExchangeInsertInven(&gObj[aIndex], nSelect);
}

bool CMuunSystem::GDMuunExchangeInsertInven(LPOBJ lpObj, int iSelect)
{
	if (!ObjectMaxRange(lpObj->m_Index))
	{
		return false;
	}

	if (!gObjIsConnected(lpObj->m_Index))
	{
		return false;
	}
	
	float fDur = 0.0;
	int iType = 0;
	int iLevel = 0;
	BYTE Y = 0;
	int iOption1 = 0;
	int iOption2 = 0;
	int iOption3 = 0;
	int iExOption = 0;
	int nSelect = iSelect;

	int nItemBagIndex = this->m_stMuunExchangeInfo[iSelect].iItemBagIndex;
	int level = 0;

	if (this->m_stMuunExchangeItembag[nItemBagIndex].iMinLv == this->m_stMuunExchangeItembag[nItemBagIndex].iMaxLv)
	{
		level = this->m_stMuunExchangeItembag[nItemBagIndex].iMinLv;
	}

	else
	{
		int sub = this->m_stMuunExchangeItembag[nItemBagIndex].iMaxLv- this->m_stMuunExchangeItembag[nItemBagIndex].iMinLv + 1;
		level = this->m_stMuunExchangeItembag[nItemBagIndex].iMinLv + rand() % sub;
	}

	iLevel = level;
	iType = ItemGetNumberMake(this->m_stMuunExchangeItembag[nItemBagIndex].iItemType,this->m_stMuunExchangeItembag[nItemBagIndex].iItemIndex);

	if ( iType == -1 )
	{
		return false;
	}

	BYTE SocketOption[5];
	memset(SocketOption, -1, sizeof(SocketOption));

	if (this->IsStoneofEvolution(iType) == true)
	{
		int nItemIndex = this->m_stMuunExchangeItembag[nItemBagIndex].iStoneMuunItemIndex;
		SocketOption[0] = (nItemIndex + (this->m_stMuunExchangeItembag[nItemBagIndex].iStoneMuunItemType << 9)) >> 8;
		SocketOption[1] = nItemIndex;
	}
		
	if (this->m_stMuunExchangeItembag[nItemBagIndex].iSkill)
	{
		iOption1 = TRUE;
	}

	if (this->m_stMuunExchangeItembag[nItemBagIndex].iLuck)
	{
		iOption2 = FALSE;
		
		if ((rand() % 2) == 0)
		{
			iOption2 = TRUE;
		}
	}
		
	LPITEM_ATTRIBUTE ItemAttribute = GetItemAttr(iType);

	if (ItemAttribute == NULL)
	{
		return false;
	}
			
	if (this->m_stMuunExchangeItembag[nItemBagIndex].iExItem)
	{
		iExOption = g_ItemOptionTypeMng.CommonExcOptionRand(ItemAttribute->ItemKindA);
		iOption2 = FALSE;
		iOption1 = TRUE;
		iLevel = 0;
	}

	if (this->m_stMuunExchangeItembag[nItemBagIndex].iAddOpt)
	{
		if ((rand() % 5) == 0)
		{
			iOption3 = 3;
		}
		
		else
		{
			iOption3 = rand() % 3;
		}
	}

	if ( iType == ITEMGET(12,15) || iType == ITEMGET(14,13) || iType == ITEMGET(14,14) )
	{
		iOption1 = FALSE;
		iOption2 = FALSE;
		iOption3 = FALSE;
		iLevel = 0;
	}

	if ( iType == ITEMGET(14, 70) || iType == ITEMGET(14,71) )
		fDur = 50.0;

	if ( iType == ITEMGET(14,85) || iType == ITEMGET(14,86) || iType == ITEMGET(14,87) )
		fDur = 10.0;

	if ( iType == ITEMGET(14,53) )
		fDur = 10.0;

	ItemSerialCreateSend(lpObj->m_Index, 218, nSelect, Y, iType, iLevel, fDur, iOption1, iOption2, iOption3, lpObj->m_Index, iExOption, 0, 0, SocketOption, 0);

	g_Log.Add("[MuunSystem][MuunExchange][ItemSerialCreateSend] [%s][%s] : Item:(%s)%d Level:%d op1:%d op2:%d op3:%d ExOp:%d",
		lpObj->AccountID, lpObj->Name, ItemAttribute->Name, iType, iLevel, iOption1, iOption2, iOption3, iExOption);
	return true;
}

BYTE CMuunSystem::DGMuunExchangeInsertInven(LPOBJ lpObj, CItem CreateItem, int iSelect)
{
	if (!ObjectMaxRange(lpObj->m_Index))
	{
		return false;
	}

	if (!gObjIsConnected(lpObj->m_Index))
	{
		return false;
	}

	BYTE btRet = -1;
	int nItemBagIndex = this->m_stMuunExchangeInfo[iSelect].iItemBagIndex;
	int nChkInven = this->m_stMuunExchangeItembag[nItemBagIndex].iInvenChk;

	if (this->ChkAndDelItemMuunExchange(lpObj, iSelect) == false)
	{
		g_Log.Add("[MuunSystem][MuunExchange][ItemSerialCreateRecv][ItemDelFail][%s][%s][%d]",
			lpObj->AccountID, lpObj->Name, iSelect);
		return btRet;
	}

	if (nChkInven == 22)
	{
		btRet = gObjMuunInventoryInsertItem(lpObj->m_Index, CreateItem);

		if (btRet != 0xFF)
		{
			GSProtocol.GCMuunInventoryItemOneSend(lpObj->m_Index, btRet);
			this->SendMsgMuunExchange(lpObj->m_Index, 0);
			return btRet;
		}
	}

	else if (nChkInven == 0)
	{
		btRet = gObjInventoryInsertItem(lpObj->m_Index, CreateItem);

		if (btRet != 0xFF)
		{
			GSProtocol.GCInventoryItemOneSend(lpObj->m_Index, btRet);
			this->SendMsgMuunExchange(lpObj->m_Index, 0);
			return btRet;
		}
	}

	else
	{
		g_Log.Add("[MuunSystem][MuunExchange][Error][%s][%s] DGMuunExchangeInsertInven() ChkInven %d [%s, %d]",
			lpObj->AccountID, lpObj->Name, nChkInven, __FILE__, __LINE__);

		return btRet;
	}

	this->SendMsgMuunExchange(lpObj->m_Index, 2);
	g_Log.Add("[MuunSystem][MuunExchange][FAIL][%s][%s] Not Empty Space", lpObj->AccountID, lpObj->Name);
	return btRet;
}

bool CMuunSystem::ChkAndDelItemMuunExchange(LPOBJ lpObj, int iSelect)
{
	int ItemPos[10];
	memset(ItemPos, -1, sizeof(ItemPos));

	int nNeedItemCnt = this->m_stMuunExchangeInfo[iSelect].iItemCnt;
	int nNeedItemNum = ITEMGET(this->m_stMuunExchangeInfo[iSelect].iItemType, this->m_stMuunExchangeInfo[iSelect].iItemIndex);
	int nChkInven = this->m_stMuunExchangeInfo[iSelect].iInvenChk;

	if (this->ChkMuunExchangeInvenNeedItem(lpObj, iSelect, ItemPos) == false)
	{
		return false;
	}

	if (nChkInven == 22)
	{
		for (int nMuunInven = 0; nMuunInven < nNeedItemCnt; nMuunInven++)
		{
			int nItemPos =  ItemPos[nMuunInven];

			if (nItemPos == -1)
			{
				break;
			}

			if (lpObj->pMuunInventory[nItemPos].IsItem() == TRUE &&
				lpObj->pMuunInventory[nItemPos].m_Type == nNeedItemNum)
			{
				BYTE NewOption[MAX_EXOPTION_SIZE];
				ItemIsBufExOption(NewOption, &lpObj->pMuunInventory[nItemPos]);

				g_Log.Add("[MuunSystem][MuunExchange] Delete MuunInven Item [%s][%s] Delete Item Info - Item:[%s,%d,%d,%d,%d] serial:[%I64d][%d] Ex:[%d,%d,%d,%d,%d,%d,%d] Set[%d] 380:[%d] HO:[%d,%d] SC[%d,%d,%d,%d,%d] BonusOption[%d]",
					lpObj->AccountID, lpObj->Name, lpObj->pMuunInventory[nItemPos].GetName(), lpObj->pMuunInventory[nItemPos].m_Level, lpObj->pMuunInventory[nItemPos].m_Option1, lpObj->pMuunInventory[nItemPos].m_Option2, lpObj->pMuunInventory[nItemPos].m_Option3,
					lpObj->pMuunInventory[nItemPos].m_Number, (int)lpObj->pMuunInventory[nItemPos].m_Durability, NewOption[0], NewOption[1], NewOption[2], NewOption[3], NewOption[4], NewOption[5], NewOption[6], lpObj->pMuunInventory[nItemPos].m_SetOption,
					lpObj->pMuunInventory[nItemPos].m_ItemOptionEx >> 7, g_kJewelOfHarmonySystem.GetItemStrengthenOption(&lpObj->pMuunInventory[nItemPos]), g_kJewelOfHarmonySystem.GetItemOptionLevel(&lpObj->pMuunInventory[nItemPos]),
					lpObj->pMuunInventory[nItemPos].m_SocketOption[0], lpObj->pMuunInventory[nItemPos].m_SocketOption[1], lpObj->pMuunInventory[nItemPos].m_SocketOption[2], lpObj->pMuunInventory[nItemPos].m_SocketOption[3], lpObj->pMuunInventory[nItemPos].m_SocketOption[4],
					lpObj->pMuunInventory[nItemPos].m_BonusSocketOption);

				this->ClearPeriodMuunItemData(lpObj, lpObj->pMuunInventory[nItemPos].m_Type, lpObj->pMuunInventory[nItemPos].m_Number);
				lpObj->pMuunInventory[nItemPos].Clear();
				GSProtocol.GCMuunInventoryItemDeleteSend(lpObj->m_Index, nItemPos, 1);
			}
		}
	}

	else if (nChkInven == 0)
	{
		for (int nInven = 0; nInven < nNeedItemCnt; nInven++)
		{
			int nItemPos =  ItemPos[nInven];

			if (nItemPos == -1)
			{
				break;
			}

			if (lpObj->pInventory[nItemPos].IsItem() == TRUE &&
				lpObj->pInventory[nItemPos].m_Type == nNeedItemNum)
			{
				BYTE NewOption[MAX_EXOPTION_SIZE];
				ItemIsBufExOption(NewOption, &lpObj->pMuunInventory[nItemPos]);

				g_Log.Add("[MuunSystem][MuunExchange] Delete Inven Item [%s][%s] Delete Item Info - Item:[%s,%d,%d,%d,%d] serial:[%I64d][%d] Ex:[%d,%d,%d,%d,%d,%d,%d] Set[%d] 380:[%d] HO:[%d,%d] SC[%d,%d,%d,%d,%d] BonusOption[%d]",
					lpObj->AccountID, lpObj->Name, lpObj->pInventory[nItemPos].GetName(), lpObj->pInventory[nItemPos].m_Level, lpObj->pInventory[nItemPos].m_Option1, lpObj->pInventory[nItemPos].m_Option2, lpObj->pInventory[nItemPos].m_Option3,
					lpObj->pInventory[nItemPos].m_Number, (int)lpObj->pInventory[nItemPos].m_Durability, NewOption[0], NewOption[1], NewOption[2], NewOption[3], NewOption[4], NewOption[5], NewOption[6], lpObj->pInventory[nItemPos].m_SetOption,
					lpObj->pInventory[nItemPos].m_ItemOptionEx >> 7, g_kJewelOfHarmonySystem.GetItemStrengthenOption(&lpObj->pInventory[nItemPos]), g_kJewelOfHarmonySystem.GetItemOptionLevel(&lpObj->pInventory[nItemPos]),
					lpObj->pInventory[nItemPos].m_SocketOption[0], lpObj->pInventory[nItemPos].m_SocketOption[1], lpObj->pInventory[nItemPos].m_SocketOption[2], lpObj->pInventory[nItemPos].m_SocketOption[3], lpObj->pInventory[nItemPos].m_SocketOption[4],
					lpObj->pInventory[nItemPos].m_BonusSocketOption);

				gObjInventoryItemSet(lpObj->m_Index, nItemPos, -1);
				lpObj->pInventory[nItemPos].Clear();
				GSProtocol.GCInventoryItemDeleteSend(lpObj->m_Index, nItemPos, 1);
			}
		}
	}

	else
	{
		g_Log.Add("[MuunSystem][MuunExchange][Error][%s][%s] ChkAndDelItemMuunExchange() ChkInven %d [%s, %d]",
			lpObj->AccountID, lpObj->Name, nChkInven, __FILE__, __LINE__);

		return false;
	}

	return true;
}

bool CMuunSystem::ChkMuunExchangeInvenNeedItem(int & iItemCnt, int iInvenPos, int iNeedItemCnt, int iInvenItemNum, int iNeedItemNum, int * ItemPos)
{
	if (iInvenItemNum != iNeedItemNum)
	{
		return false;
	}

	if (ItemPos != 0)
	{
		if (iItemCnt < 10)
		{
			ItemPos[iItemCnt] = iInvenPos;
		}
	}

	iItemCnt++;

	if (iItemCnt < iNeedItemCnt)
	{
		return false;
	}

	return true;
}

bool CMuunSystem::ChkMuunExchangeInvenNeedItem(LPOBJ lpObj, int iSelect, int *ItemPos)
{
	int nItemCnt = 0;
	int nNeedItemCnt = this->m_stMuunExchangeInfo[iSelect].iItemCnt;
	int nNeedItemNum = ITEMGET(this->m_stMuunExchangeInfo[iSelect].iItemType, this->m_stMuunExchangeInfo[iSelect].iItemIndex);
	int nChkInven = this->m_stMuunExchangeInfo[iSelect].iInvenChk;

	if ( nChkInven == 22 )
	{
		for (int nMuunInven = 2; nMuunInven < MUUN_INVENTORY_SIZE; nMuunInven++ )
		{
			if (lpObj->pMuunInventory[nMuunInven].IsItem() == TRUE && this->ChkMuunExchangeInvenNeedItem(nItemCnt,nMuunInven, nNeedItemCnt, lpObj->pMuunInventory[nMuunInven].m_Type, nNeedItemNum,ItemPos) == TRUE)
			{
				return TRUE;
			}
		}
	}

	else if (nChkInven == 0)
	{
		for (int nInven = INVETORY_WEAR_SIZE; nInven < MAIN_INVENTORY_SIZE; nInven++ )
		{
			if (lpObj->pInventory[nInven].IsItem() == TRUE && this->ChkMuunExchangeInvenNeedItem(nItemCnt, nInven, nNeedItemCnt, lpObj->pInventory[nInven].m_Type, nNeedItemNum, ItemPos) == TRUE)
			{
				return TRUE;
			}
		}
    }

	else
	{
		g_Log.Add("[MuunSystem][MuunExchange][Error][%s][%s] ChkMuunExchangeInvenNeedItem() ChkInven %d [%s, %d]",lpObj->AccountID,lpObj->Name, nChkInven, __FILE__, __LINE__);
		return FALSE;
	}

	g_Log.Add("[MuunSystem][MuunExchange][FAIL][%s][%s] Lack of Materials", lpObj->AccountID, lpObj->Name);
	
	this->SendMsgMuunExchange(lpObj->m_Index, 1);
	return false;
}

bool CMuunSystem::ChkMuunExchangeInvenEmpty(LPOBJ lpObj, int iSelect)
{
	int nChkInven = this->m_stMuunExchangeItembag[this->m_stMuunExchangeInfo[iSelect].iItemBagIndex].iInvenChk;

	if (nChkInven == 22)
	{
		if ( gObjChkMuunInventoryEmpty(lpObj) == 0xFF )
		{
			this->SendMsgMuunExchange(lpObj->m_Index, 2);
			return false;
		}
	}

	else if (nChkInven == 0)
	{
		if (CheckInventoryEmptySpace(lpObj, 4, 4) == FALSE)
		{
			this->SendMsgMuunExchange(lpObj->m_Index, 2);
			return false;
		}
	}

	else
	{
		g_Log.Add("[MuunSystem][MuunExchange][Error][%s][%s] ChkMuunExchangeInvenEmpty() ChkInven %d [%s, %d]",  lpObj->AccountID, lpObj->Name, nChkInven, __FILE__,__LINE__);
		return false;
	}

	return true;
}

void CMuunSystem::SendMsgMuunExchange(int aIndex, int iResult)
{
	PMSG_ANS_MUUN_EXCHANGE pMsg; 

	pMsg.btResult = iResult;
	PHeadSubSetB((LPBYTE)&pMsg, 0x4E, 0x13, sizeof(pMsg));
	IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

void CMuunSystem::SetTarget(LPOBJ lpObj, int aTargetIndex)
{
	for (int i = 0; i < 2; i++)
	{
		if (lpObj->m_MuunEffectList[i].nOptType == 50 || lpObj->m_MuunEffectList[i].nOptType == 52)
		{
			lpObj->m_MuunEffectList[i].nTargetIndex = aTargetIndex;
		}

		if (lpObj->m_MuunEffectList[i].nOptType == 53)
		{
			if (gObj[aTargetIndex].Type != OBJ_USER)
			{
				lpObj->m_MuunEffectList[i].nTargetIndex = aTargetIndex;
			}
		}
	}
}

void CMuunSystem::ReSetTarget(LPOBJ lpObj, int aTargetIndex)
{
	for (int i = 0; i < 2; i++)
	{
		if (lpObj->m_MuunEffectList[i].nOptType == 50 || lpObj->m_MuunEffectList[i].nOptType == 52 || lpObj->m_MuunEffectList[i].nOptType == 53)
		{
			lpObj->m_MuunEffectList[i].nTargetIndex = -1;
		}
	}
}

void CMuunSystem::CGReqRideSelect(PMSG_MUUN_RIDE_SELECT *lpMsg, int aIndex)
{
	if(!ObjectMaxRange(aIndex))
		return;

	if(!gObjIsConnected(aIndex))
		return;

	LPOBJ lpObj = &gObj[aIndex];

	lpObj->m_wMuunRideItem = -1;

	for (int i = 0; i < 2; i++)
	{
		int nItemNum = lpObj->pMuunInventory[i].m_Type;

		if ( nItemNum == lpMsg->wItemNum )
		{
			lpObj->m_wMuunRideItem = nItemNum;
			break;
		}
	}

	_tagMuunRideViewPortInfo MuunViewPortInfo; 
	BYTE btMuunInfosendBuf[2048]; 

	MuunViewPortInfo.NumberH = SET_NUMBERH(lpObj->m_Index);
	MuunViewPortInfo.NumberL = SET_NUMBERL(lpObj->m_Index);
	MuunViewPortInfo.MuunRideItemH = SET_NUMBERH(lpObj->m_wMuunRideItem);
	MuunViewPortInfo.MuunRideItemL = SET_NUMBERL(lpObj->m_wMuunRideItem);

	if ( lpObj->m_wMuunRideItem == 0xFFFF )
	{
		MuunViewPortInfo.MuunRideItemH = SET_NUMBERH(lpObj->m_wInvenPet);
		MuunViewPortInfo.MuunRideItemL = SET_NUMBERL(lpObj->m_wInvenPet);
	}

	PMSG_SEND_MUUN_RIDE_VIEWPORT_INFO pMsgMuun;

	int lOfs = sizeof(pMsgMuun);

	memcpy(&btMuunInfosendBuf[lOfs], &MuunViewPortInfo, 4);
	lOfs += sizeof(_tagMuunRideViewPortInfo);

	pMsgMuun.Count = 1;
	pMsgMuun.h.set((LPBYTE)&pMsgMuun,0x4E,0x14,lOfs);
	memcpy(btMuunInfosendBuf, &pMsgMuun, sizeof(pMsgMuun));

	IOCP.DataSend(lpObj->m_Index, btMuunInfosendBuf, lOfs);

	for (int n = 0; n < MAX_VIEWPORT; n++)
	{
		if (lpObj->VpPlayer2[n].state != 0)
		{
			int tObjNum = lpObj->VpPlayer2[n].number;

			if (tObjNum >= 0 && lpObj->VpPlayer2[n].type == OBJ_USER)
			{
				IOCP.DataSend(tObjNum, btMuunInfosendBuf, lOfs);
			}
		}
	}
}

void CMuunSystem::SkillProc(LPOBJ lpObj)
{
	this->m_MuunAttack.SkillProc(lpObj);
}

bool CMuunSystem::IsMuunExpireDate(int iType)
{
	if (this->IsMuunItem(iType) == false)
	{
		return false;
	}

	CMuunInfo *pCMuunInfo = this->m_MuunInfoMng.GetMuunItemNumToMuunInfo(iType);

	if (!pCMuunInfo)
	{
		return false;
	}

	if (this->CheckAddOptionExpireDate(pCMuunInfo->GetAddOptStart(), pCMuunInfo->GetAddOptEnd()) == true)
	{
		return true;
	}

	return false;
}

void CMuunSystem::Attack(LPOBJ lpObj, LPOBJ lpTargetObj, CMagicInf *lpMagic, int SubCode, int SubCode2)
{
	switch (SubCode)
	{
		case 50:
			this->m_MuunAttack.Attack(lpObj, lpTargetObj, lpMagic, SubCode2);
			break;
		case 51:
			this->m_MuunAttack.DamageAbsorb(lpObj, lpTargetObj, lpMagic, SubCode2);
			break;
		case 52:
			this->m_MuunAttack.Stun(lpObj, lpTargetObj, lpMagic, SubCode2);
			break;
		case 53:
			this->m_MuunAttack.Attack(lpObj, lpTargetObj, lpMagic, SubCode2);
			break;
	}
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


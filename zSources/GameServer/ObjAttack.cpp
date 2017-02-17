//////////////////////////////////////////////////////////////////////
// ObjAttack.cpp
#include "stdafx.h"
#include "ObjAttack.h"
#include "BattleSoccerManager.h"
#include "DarkSpirit.h"
#include "ObjUseSkill.h"
#include "gObjMonster.h"
#include "BloodCastle.h"
#include "ChaosCastle.h"
#include "Gamemain.h"
#include "BuffEffect.h"
#include "BuffEffectSlot.h"
#include "PentagramSystem.h"
#include "configread.h"
#include "GensSystem.h"
#include "NewPVP.h"
#include "ImperialGuardian.h"
#include "MasterLevelSkillTreeSystem.h"
#include "LuckyItemManager.h"
#include "Crywolf.h"
#include "LastManStanding.h"
#include "MuunSystem.h"
#include "AcheronGuardianEvent.h"
#include "ObjCalCharacter.h"
#include "OfflineLevelling.h"
#include "SkillSpecialize.h"
#include "GameSecurity.h"
#include "MineSystem.h"
#include "SkillSafeZoneUse.h"

CObjAttack gclassObjAttack;

CObjAttack::CObjAttack() : m_Lua(true)
{
	return;
}

CObjAttack::~CObjAttack()
{
	return;
}

void CObjAttack::Init()
{
	this->m_Lua.DoFile(g_ConfigRead.GetPath("\\Scripts\\Character\\CalcCharacter.lua"));
}

BOOL CObjAttack::Attack(LPOBJ lpObj, LPOBJ lpTargetObj, CMagicInf* lpMagic,  int magicsend, unsigned char MSBFlag, int AttackDamage, BOOL bCombo, BYTE RFAttack, BYTE byReflect, BYTE byPentagramAttack)
{
	/*if(lpObj->Type == OBJ_USER)
	{
		gGameSecurity.DebugInfo(lpObj->m_Index);
	}*/
	int skillSuccess = 0;
	LPOBJ lpCallObj;
	LPOBJ lpCallTargetObj;
	int MsgDamage = 0;
	int ManaChange = 0;
	int iTempShieldDamage = 0;
	int iTotalShieldDamage = 0;
	int LifeChange;
	int iOption = 0;
	BOOL bDragonKickSDAttackSuccess = FALSE;
	BOOL bCommonAttackMiss = FALSE;
	BOOL bHaveWingOption_FullMana = FALSE;
	BOOL bHaveWingOption_FullHP = FALSE;

	if ( (lpObj->Authority&2) == 2 || (lpTargetObj->Authority&2) == 2 ) //s4 add-on
	{
		return FALSE;
	}

	if ( (lpObj->Authority&32) == 32 || (lpTargetObj->Authority&32) == 32) //s4 add-on
	{
		if(gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_INVISABLE) == TRUE)
		{
			return FALSE;
		}

		if(gObjCheckUsedBuffEffect(lpTargetObj, BUFFTYPE_INVISABLE) == TRUE)
		{
			return FALSE;
		}
	}

	if( lpTargetObj->m_bOffLevel == true && g_OffLevel.m_General.Immortal == 1)
		return FALSE;

	if( lpTargetObj->Type == OBJ_USER && lpTargetObj->m_PlayerData->ISBOT == true)
		return FALSE;

	if ( lpObj->MapNumber != lpTargetObj->MapNumber )
		return FALSE;

	if(g_MineSystem.IsTwinkle(lpTargetObj->Class))
	{
		g_Log.AddC(TColor::Red,"[Anti-Hack] [%d][%s][%s] Attempt to attack Pandora Twinkle", lpObj->m_Index, lpObj->AccountID, lpObj->Name);
		AntiHackLog->Output("[Anti-Hack] [%d][%s][%s] Attempt to attack Pandora Twinkle", lpObj->m_Index, lpObj->AccountID, lpObj->Name);
		return FALSE;
	}

	if (lpTargetObj->Class == 104 || lpTargetObj->Class == 105 || lpTargetObj->Class == 106 || lpTargetObj->Class == 523 || lpTargetObj->Class == 689)
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
					return FALSE;
			}
		}
	}

	if( lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER )
	{
		if( lpObj->MapNumber == g_LastManStanding.m_Cfg.iPVPMap)
		{
			if(g_LastManStanding.m_Rooms[0].bState == 2) // state same for all rooms
			{
				if(lpObj->m_PlayerData->RegisterdLMS == 1 && lpTargetObj->m_PlayerData->RegisterdLMS == 1)
				{
					return FALSE;
				}
			}
		}
	}

	if ( lpObj->Type == OBJ_USER && !byReflect && g_ConfigRead.antihack.EnabledAntiRefTimeCheck )
	{
		if ( GetTickCount() - lpObj->m_PlayerData->m_dwMSBFlagAttackDisable < g_ConfigRead.antihack.AntiRefCheckTime )
		{
			return FALSE;
		}
	}

	if ( lpTargetObj->Type == OBJ_USER && lpObj->Type == OBJ_USER )
	{
		if ( g_GensSystem.IsPkEnable(lpObj, lpTargetObj) == FALSE )
		{
			return FALSE;
		}

		if(lpObj->PartyNumber != -1 && lpTargetObj->PartyNumber != -1)
		{
			if(lpTargetObj->PartyNumber == lpObj->PartyNumber)
			{
				return FALSE;
			}
		}
	}

	if (g_ArcaBattle.IsArcaBattleServer() == TRUE && g_AcheronGuardianEvent.IsPlayStart() == false)
	{
		if (lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER && g_ArcaBattle.IsPkEnable(lpObj, lpTargetObj) == FALSE)
		{
			return FALSE;
		}

		if (lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_MONSTER && g_ArcaBattle.IsEnableAttackObelisk(lpObj, lpTargetObj->Class) == FALSE)
		{
			return FALSE;
		}
	}

	if ( lpTargetObj->Type == OBJ_USER && g_NewPVP.IsDuel(*lpTargetObj) && g_NewPVP.IsSafeState(*lpTargetObj) )
	{
		return FALSE;
	}

	if ( lpObj->Type == OBJ_USER && g_NewPVP.IsDuel(*lpObj) && g_NewPVP.IsSafeState(*lpObj) )
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

	if ( g_iUseCharacterAutoRecuperationSystem && lpObj->Level <= g_iCharacterRecuperationMaxLevel )
	{
		if ( lpObj->Type == OBJ_USER )
		{
			lpObj->m_iAutoRecuperationTime = GetTickCount();
		}

		if ( lpTargetObj->Type == OBJ_USER )
		{
			lpTargetObj->m_iAutoRecuperationTime = GetTickCount();
		}
	}

	if ( lpObj->Type == OBJ_USER )
		lpObj->dwShieldAutoRefillTimer = GetTickCount();

	if ( lpTargetObj->Type == OBJ_USER )
		lpTargetObj->dwShieldAutoRefillTimer = GetTickCount();

	int skill = 0;
	
	if ( lpMagic )
		skill = lpMagic->m_Skill;

	skillSuccess = TRUE;

	if ( lpObj->Type == OBJ_USER && lpObj->m_PlayerData->GuildNumber > 0 )
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

			if ( lpObj->m_PlayerData->lpGuild->WarState )
			{
				if ( lpObj->m_PlayerData->lpGuild->WarType == 0 )
				{
					if ( lpTargetObj->Type == OBJ_MONSTER )
					{
						return TRUE;
					}
				}
			}
		}
	}

	if ( lpTargetObj->Type == OBJ_MONSTER )
	{
		if ( lpTargetObj->m_iMonsterBattleDelay > 0 )
			return TRUE;
		
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

			if ( magicsend )
			{
				GSProtocol.GCMagicAttackNumberSend(lpObj, skill, lpTargetObj->m_Index, skillSuccess);
			}

			return TRUE;
		}

		if ( gObjCheckUsedBuffEffect(lpTargetObj, BUFFTYPE_MONSTER_MAGIC_IMMUNE) == TRUE )
		{
			BOOL bCheckAttackIsMagicType = gObjCheckAttackTypeMagic(lpObj->Class, skill);

			if ( bCheckAttackIsMagicType == TRUE )
			{
				GSProtocol.GCMagicAttackNumberSend(lpObj, skill, lpTargetObj->m_Index, 0);
				return TRUE;
			}

			if ( lpTargetObj->Class == 673 )
			{
				GSProtocol.GCDamageSend(lpObj->m_Index, lpTargetObj->m_Index, 0, 0, 0, 0);
				return TRUE;
			}
		}

		if ( gObjCheckUsedBuffEffect(lpTargetObj, BUFFTYPE_MONSTER_MELEE_IMMUNE) == TRUE )
		{
			BOOL bCheckAttackIsMagicType = gObjCheckAttackTypeMagic(lpObj->Class, skill);

			if ( bCheckAttackIsMagicType == FALSE )
			{
				GSProtocol.GCDamageSend(lpObj->m_Index, lpTargetObj->m_Index, 0, 0, 0, 0);
				return TRUE;
			}

			if ( lpTargetObj->Class == 673 )
			{
				GSProtocol.GCDamageSend(lpObj->m_Index, lpTargetObj->m_Index, 0, 0, 0, 0);
				return TRUE;
			}
		}

		if ( lpTargetObj->m_MonsterSkillElementInfo.m_iSkillElementImmuneTime > 0 )
		{
			if ( lpTargetObj->m_MonsterSkillElementInfo.m_iSkillElementImmuneNumber == skill )
			{
				return TRUE;
			}
		}

		if ( lpTargetObj->m_MonsterSkillElementInfo.m_iSkillElementImmuneAllTime > 0 ) //season4
		{
			GSProtocol.GCDamageSend(lpObj->m_Index, lpTargetObj->m_Index, 0, 0, 32, 0);
			return TRUE;
		}

		if ( lpTargetObj->Class == 523 )
		{
			return TRUE;
		}

		if ( lpTargetObj->Class >= 524 && lpTargetObj->Class <= 528 && lpTargetObj->Class != 526 )
		{
			if ( g_ImperialGuardian.IsAttackAbleMonster(lpTargetObj->m_Index) == false )
			{
				return TRUE;
			}
		}
	}

	if ( lpObj->Type == OBJ_USER && lpObj->m_PlayerData->ISBOT == false )
	{
		if ( !gObjIsConnected(lpObj) )
		{
			return FALSE;
		}

		if ( lpObj->m_Change == 8 )
		{
			skill = 1;
			lpMagic = &DefMagicInf[1];
			magicsend = 1; 
		}

		gDarkSpirit[lpObj->m_Index- g_ConfigRead.server.GetObjectStartUserIndex()].SetTarget(lpTargetObj->m_Index);
		g_CMuunSystem.SetTarget(lpObj, lpTargetObj->m_Index);
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

	if ( !gObjAttackQ(lpTargetObj) && lpObj->m_bOffLevel == false)
		return FALSE;

	if ( g_ConfigRead.antihack.EnableAttackBlockInSafeZone == TRUE )
	{
		BYTE btAttr = MapC[lpObj->MapNumber].GetAttr(lpObj->X, lpObj->Y);

		if((btAttr & 1) == 1)
		{
			if (lpMagic)
			{
				if (g_SkillSafeZone.CanUseSkill(lpObj, lpMagic) == false)
				{
					return FALSE;
				}
			}

			else
			{
				return FALSE;
			}
		}
	}

	if ( lpObj->m_RecallMon >= 0 )
		gObjCallMonsterSetEnemy(lpObj, lpTargetObj->m_Index);
	
	lpObj->m_TotalAttackCount++;

	if ( AttackDamage == 0 )
	{
		if ( skill != 76 )
		{
			if ( gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_INFINITY_ARROW) == FALSE && gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_INFINITY_ARROW_STR) == FALSE )
			{
				if ( !this->DecreaseArrow(lpObj) )
				{
					return FALSE;
				}
			}
		}
	}

	if ( this->CheckAttackArea(lpObj, lpTargetObj) == FALSE )
		return FALSE;

	lpCallObj = lpObj;

	if ( lpObj->Type == OBJ_MONSTER )
	{
		if ( lpObj->m_RecallMon >= 0 )
		{
			lpCallObj = &gObj[lpObj->m_RecallMon];
		}
	}

	lpCallTargetObj = lpTargetObj;

	if ( lpTargetObj->Type == OBJ_MONSTER )
	{
		if ( lpTargetObj->m_RecallMon >= 0 )
		{
			lpCallTargetObj = &gObj[lpTargetObj->m_RecallMon];
		}
	}

	if ( g_GensSystem.IsMapBattleZone(lpObj->MapNumber) == FALSE && this->PkCheck(lpCallObj, lpTargetObj) == FALSE )
		return FALSE;

	int Strength = 0; 
	int Dexterity = 0;
	int Vitality = 0;
	int Energy = 0;

	if(lpObj->Type == OBJ_USER)
	{ 
		Strength = lpObj->m_PlayerData->Strength + lpObj->AddStrength;
		Dexterity = lpObj->m_PlayerData->Dexterity + lpObj->AddDexterity;
		Vitality = lpObj->m_PlayerData->Vitality + lpObj->AddVitality;
		Energy = lpObj->m_PlayerData->Energy + lpObj->AddEnergy;
	}

	BOOL bIsOnDuel = gObjDuelCheck(lpObj, lpTargetObj);

	if ( bIsOnDuel )
	{
		lpObj->m_iDuelTickCount = GetTickCount();
		lpTargetObj->m_iDuelTickCount = GetTickCount();
	}

	if ( lpObj->pInventory[0].m_Type == ITEMGET(2,5) && lpObj->pInventory[0].m_IsValidItem != false )	// Crystal Sword
	{
		if ( (rand()%20) == 0 )
		{
			skill = 7;
			lpMagic = &DefMagicInf[7];
			magicsend = 1;
		}
	}
	else if ( lpObj->pInventory[1].m_Type == ITEMGET(2,5) && lpObj->pInventory[1].m_IsValidItem != false )	// Crystal Sword
	{
		if ( (rand()%20) == 0 )
		{
			skill = 7;
			lpMagic = &DefMagicInf[7];
			magicsend = 1;
		}
	}

	MSBFlag = 0;
	MsgDamage = 0;
	skillSuccess = this->ResistanceCheck(lpObj, lpTargetObj, skill);
	BOOL skillIceArrowSuccess = skillSuccess;

	if ( skill == 51 || skill == 424 )
		skillSuccess = 0;

	BOOL bAllMiss = FALSE;
	
	if ( gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_INVISABLE) == TRUE && ( lpObj->Authority & 0x20 ) != 0x20 )
		gObjUseSkill.RemoveCloakingEffect(lpObj->m_Index);

	BOOL bDamageReflect = FALSE;
	int iTargetDefense = 0;
	BOOL bPentagramBossMonster = FALSE;

	if (lpTargetObj->m_iPentagramAttributePattern == 2)
		bPentagramBossMonster = TRUE;

	if ( AttackDamage > 0 || bPentagramBossMonster )
	{
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
							if (g_CastleSiege.m_bCastleSiegeFriendlyFire == false)
							{
								AttackDamage = 0;
							}
							else
							{
								AttackDamage = AttackDamage * g_CastleSiege.CastleSiegeSelfDmgReductionPercent / 100;
							}
						}
						else if (g_ShieldSystemOn == 0)
						{
							AttackDamage = AttackDamage * g_CastleSiege.CastleSiegeDmgReductionPercent / 100;
						}
					}
				}
			}
			if (g_CastleSiege.GetCastleState() != CASTLESIEGE_STATE_STARTSIEGE)
			{
				if (lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_MONSTER)
				{
					if (lpObj->MapNumber == MAP_INDEX_CASTLESIEGE && lpTargetObj->MapNumber == MAP_INDEX_CASTLESIEGE)
					{
						if (lpTargetObj->Class == 277 || lpTargetObj->Class == 283)
						{
							return 0;
						}
					}
				}
			}
		}

		if ( skill == 216 )
		{
			if ( g_ShieldSystemOn == TRUE )
			{
				if ( lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER )
				{
					if ( !this->MissCheckPvP(lpObj, lpTargetObj, skill, skillSuccess, magicsend, bAllMiss, 0) )
					{
						return FALSE;
					}
				}

				else
				{
					if ( !this->MissCheck(lpObj, lpTargetObj, skill, skillSuccess, magicsend, bAllMiss, 0) )
					{
						return FALSE;
					}
				}
			}

			else
			{
				if ( !this->MissCheck(lpObj, lpTargetObj, skill, skillSuccess, magicsend, bAllMiss, 0) )
				{
					return FALSE;
				}
			}
		}
		if ( skill != 79 && skill != 216 && !bPentagramBossMonster )
		{
			bDamageReflect = TRUE;
			MsgDamage = 4;
		}

		if (skill == 215 || skill == 455)
		{
			bCombo = 0;
		}

		if ( bPentagramBossMonster )
		{
			AttackDamage = 0;
		}

		if ( lpObj->Type != OBJ_USER || lpTargetObj->Type != OBJ_USER )
		{
			lpTargetObj->Life -= AttackDamage;

			if(lpTargetObj->Life <= 0.0)
				lpTargetObj->Life = 0.0;
		}

		else
		{
			if ( skill == 265 || skill == 564 || skill == 566 )
			{
				int nSuccessRate = 0, nDecreaseSDRate = 0;
				gObjUseSkill.m_Lua.Generic_Call("DragonSlasher_DecreaseVictimSD", "i>ii", Energy, &nSuccessRate, &nDecreaseSDRate);

				if (rand()%100 < nSuccessRate)
				{
					bDragonKickSDAttackSuccess = TRUE;
					int iDragonKickSDDamage = nDecreaseSDRate * lpTargetObj->iShield / 100;
					lpTargetObj->iShield -= iDragonKickSDDamage;
					lpTargetObj->Life -= AttackDamage;
					iTotalShieldDamage += iDragonKickSDDamage;
				}

				else
				{
					iTempShieldDamage = this->GetShieldDamage(lpObj, lpTargetObj, AttackDamage);
					lpTargetObj->iShield -= iTempShieldDamage;
					lpTargetObj->Life -= AttackDamage - iTempShieldDamage;
					iTotalShieldDamage += iTempShieldDamage;
				}
			}

			else
			{
				iTempShieldDamage = this->GetShieldDamage(lpObj, lpTargetObj, AttackDamage);
				lpTargetObj->iShield -= iTempShieldDamage;
				lpTargetObj->Life -= AttackDamage - iTempShieldDamage;
				iTotalShieldDamage += iTempShieldDamage;
			}

			if(lpTargetObj->Life <= 0.0)
				lpTargetObj->Life = 0.0;
		}
	}
	
	else if ( AttackDamage == 0 )
	{
		if ( g_ShieldSystemOn == TRUE )
		{
			if ( lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER )
			{
				if(!this->MissCheckPvP(lpObj, lpTargetObj, skill, skillSuccess, magicsend, bAllMiss, RFAttack))
				{
					if ( lpObj->m_iPentagramMainAttribute > 0 && byPentagramAttack == TRUE )
					{
						this->PentagramAttack(lpObj, lpTargetObj, lpMagic, 0, MsgDamage, 0, 0);
					}

					return FALSE;
				}
			}
			else
			{
				if ( ( lpObj->Type != OBJ_MONSTER || !lpObj->m_iPentagramMainAttribute) && !this->MissCheck(lpObj, lpTargetObj, skill, skillSuccess, magicsend, bAllMiss, RFAttack) ) // ?
				{
					if (lpObj->m_iPentagramMainAttribute > 0 && byPentagramAttack == TRUE)
					{
						this->PentagramAttack(lpObj, lpTargetObj, lpMagic, 0, MsgDamage, 0, 0);
					}

					return FALSE;
				}
			}
		}
		else
		{
			if(!this->MissCheck(lpObj, lpTargetObj, skill, skillSuccess, magicsend, bAllMiss, RFAttack))
			{
				if (lpObj->m_iPentagramMainAttribute > 0 && byPentagramAttack == TRUE)
				{
					this->PentagramAttack(lpObj, lpTargetObj, lpMagic, 0, MsgDamage, 0, 0);
				}

				return FALSE;
			}
		}
		
		if ( (skill == 51 || skill == 424) && skillIceArrowSuccess == TRUE )
		{
			gObjAddBuffEffect(lpTargetObj, BUFFTYPE_STONE, 0, 0, 0, 0, 7);
			lpTargetObj->lpAttackObj = lpObj;
			lpTargetObj->PathCount = 0;
			lpTargetObj->PathStartEnd = 0;
			skillSuccess = TRUE;

			gObjSetPosition(lpTargetObj->m_Index, lpTargetObj->X, lpTargetObj->Y);
		}

		int iOriginTargetDefense = 0;
		int targetdefense = this->GetTargetDefense(lpObj, lpTargetObj, MsgDamage, iOriginTargetDefense);

		if ( lpTargetObj->Type == OBJ_USER && gObjCheckUsedBuffEffect(lpTargetObj, BUFFTYPE_BERSERKER) )
		{
			float fDefense = Dexterity / 3;
			float fValue = (float)gObjGetTotalValueOfEffect(lpTargetObj, 32);
			float fPercent = (40 - fValue) / 100;
			fPercent = (0.1f > fPercent) ? 0.1f : fPercent;

			targetdefense -= fDefense * fPercent;
			iOriginTargetDefense -= fDefense * fPercent;
		}

		if (lpTargetObj->Type == OBJ_USER && (gObjCheckUsedBuffEffect(lpTargetObj, BUFFTYPE_BERSERKER_STR) || gObjCheckUsedBuffEffect(lpTargetObj, BUFFTYPE_BERSERKER_MAS) || gObjCheckUsedBuffEffect(lpTargetObj, BUFFTYPE_BERSERKER_PRO)))
		{
			float fDefense = Dexterity / 3;
			float fValue = (float)gObjGetTotalValueOfEffect(lpTargetObj, 32);
			float fPercent = (40 - (fValue+lpTargetObj->m_PlayerData->m_MPSkillOpt.iMpsIncPercentBerserker1)) / 100;
			fPercent = (0.1f > fPercent) ? 0.1f : fPercent;

			targetdefense -= fDefense * fPercent;
			iOriginTargetDefense -= fDefense * fPercent;
		}

		int iCurseValue = 0;
		gObjGetValueOfBuffIndex(lpTargetObj, BUFFTYPE_CURSED_DEFENSE_DOWN, &iCurseValue, 0);
		targetdefense -= targetdefense * iCurseValue / 100;
		iOriginTargetDefense -= iOriginTargetDefense * iCurseValue / 100;

		if (gObjCheckUsedBuffEffect(lpTargetObj, BUFFTYPE_INNOVATE) == true)
		{
			iCurseValue = 0;
			gObjGetValueOfBuffIndex(lpTargetObj, BUFFTYPE_INNOVATE, &iCurseValue, NULL);
			targetdefense -= targetdefense * iCurseValue / 100;
			iOriginTargetDefense -= iOriginTargetDefense * iCurseValue / 100;
		}

		if (lpTargetObj->Type == OBJ_USER && lpTargetObj->pInventory[7].IsItem() == TRUE && lpTargetObj->pInventory[7].m_IsValidItem == true)
		{
			targetdefense += lpTargetObj->m_PlayerData->m_MPSkillOpt.iMpsAddWingDefense;
		}

		iTargetDefense = iOriginTargetDefense;

		if ( skill == 19
			|| skill == 20
			|| skill == 21
			|| skill == 23
			|| skill == 56
			|| skill == 41
			|| skill == 47
			|| skill == 42
			|| skill == 49
			|| skill == 43
			|| skill == 22
			|| skill == 55
			|| skill == 44
			|| skill == 57
			|| skill == 74
			|| g_MasterLevelSkillTreeSystem.GetBaseMasterLevelSkill(skill) == 330
			|| g_MasterLevelSkillTreeSystem.GetBaseMasterLevelSkill(skill) == 481
			|| g_MasterLevelSkillTreeSystem.GetBaseMasterLevelSkill(skill) == 336
			|| g_MasterLevelSkillTreeSystem.GetBaseMasterLevelSkill(skill) == 331
			|| g_MasterLevelSkillTreeSystem.GetBaseMasterLevelSkill(skill) == 490
			|| g_MasterLevelSkillTreeSystem.GetBaseMasterLevelSkill(skill) == 482
			|| skill == 235
			|| skill == 232
			|| skill == 236
			|| skill == 238
			|| skill == 326
			|| skill == 327
			|| skill == 328
			|| skill == 329
			|| skill == 479
			|| skill == 337
			|| skill == 332
			|| skill == 333
			|| skill == 344
			|| skill == 346
			|| skill == 411
			|| skill == 431
			|| skill == 492
			|| skill == 494
			|| skill == 523
			|| skill == 493)
		{
			AttackDamage = this->GetAttackDamage(lpObj, lpTargetObj, targetdefense, MsgDamage, lpMagic);

			if ( AttackDamage > 0 )
			{
				gObjWeaponDurDown(lpObj, lpTargetObj, 0);
			}
		}
		else if ( skill == 76 && lpObj->Type == OBJ_USER )	// pluzzmanton
		{
			int iFenrirAttackDmg = 0;

			if ( lpObj->Class == CLASS_KNIGHT || lpObj->Class == CLASS_MAGUMSA || lpObj->Class == CLASS_RAGEFIGHTER )
			{
				iFenrirAttackDmg = lpObj->m_PlayerData->Strength / 3 + lpObj->m_PlayerData->Dexterity / 5 + lpObj->m_PlayerData->Vitality / 5 + lpObj->m_PlayerData->Energy / 7;	// #formula
			}
			else if ( lpObj->Class == CLASS_WIZARD || lpObj->Class == CLASS_SUMMONER )
			{
				iFenrirAttackDmg = lpObj->m_PlayerData->Strength / 5 + lpObj->m_PlayerData->Dexterity / 5 + lpObj->m_PlayerData->Vitality / 7 + lpObj->m_PlayerData->Energy / 3;	// #formula
			}
			else if ( lpObj->Class == CLASS_ELF )
			{
				iFenrirAttackDmg = lpObj->m_PlayerData->Strength / 5 + lpObj->m_PlayerData->Dexterity / 3 + lpObj->m_PlayerData->Vitality / 7 + lpObj->m_PlayerData->Energy / 5;	// #formula
			}
			else	// Dark Lord
			{
				iFenrirAttackDmg = lpObj->m_PlayerData->Strength / 5 + lpObj->m_PlayerData->Dexterity / 5 + lpObj->m_PlayerData->Vitality / 7 + lpObj->m_PlayerData->Energy / 3 + lpObj->Leadership / 3;	// #formula
			}

			if ( iFenrirAttackDmg < 0 )
				iFenrirAttackDmg = 0;

			int CritDamage = lpObj->m_CriticalDamage;

			if (lpTargetObj->Type == OBJ_USER)
			{
				CritDamage -= lpTargetObj->m_PlayerData->m_Resistance_Critical;
			}

			if ( CritDamage > 0 )
			{
				if ( (rand()%100) < CritDamage )
				{
					MsgDamage = 3;
				}
			}

			int ExtDamage = lpObj->m_ExcelentDamage;

			if (lpTargetObj->Type == OBJ_USER)
			{
				ExtDamage -= lpTargetObj->m_PlayerData->m_Resistance_Excellent;
			}

			if ( ExtDamage > 0 )
			{
				if ( (rand()%100) < ExtDamage )
				{
					MsgDamage = 2;
				}
			}

			if ( MsgDamage == 3 )	// Critical Damage
			{
				AttackDamage =  iFenrirAttackDmg + lpMagic->m_DamageMax;
				AttackDamage += lpObj->m_PlayerData->SetOpAddCriticalDamage;
				iOption = 0;
				gObjGetValueOfBuffIndex(lpObj, BUFFTYPE_CRITICAL_DMG_INC, &iOption, 0);
				AttackDamage += iOption;
				iOption = 0;
				gObjGetValueOfBuffIndex(lpObj, BUFFTYPE_CRITICAL_DMG_INC_STR, &iOption, 0);
				AttackDamage += iOption;
				iOption = 0;
				gObjGetValueOfBuffIndex(lpObj, BUFFTYPE_CRITICAL_DMG_INC_MAS, &iOption, 0);
				AttackDamage += iOption;
				AttackDamage += lpObj->m_PlayerData->m_JewelOfHarmonyEffect.HJOpAddCriticalDamage;
				AttackDamage -= targetdefense;
			}
			else if ( MsgDamage == 2 )	// Excellent
			{
				AttackDamage = iFenrirAttackDmg + lpMagic->m_DamageMax;
				AttackDamage += AttackDamage * 20 / 100;
				AttackDamage += lpObj->m_PlayerData->SetOpAddExDamage;
				AttackDamage -= targetdefense;
			}
			else
			{
				AttackDamage = (iFenrirAttackDmg + lpMagic->m_DamageMin) + (rand()%(lpMagic->m_DamageMax - lpMagic->m_DamageMin + 1));
				AttackDamage -= targetdefense;
			}
		}
		else
		{
			if ( ( lpObj->Class == CLASS_WIZARD || lpObj->Class == CLASS_MAGUMSA ) && skill )
			{
				AttackDamage = this->GetAttackDamageWizard(lpObj, lpTargetObj, targetdefense, lpMagic, MsgDamage);

				if ( AttackDamage > 0 )
				{
					gObjWeaponDurDown(lpObj, lpTargetObj, 1);
				}
			}

			else if ( lpObj->Class == CLASS_SUMMONER && skill )
			{
				AttackDamage = this->GetAttackDamageSummoner(lpObj, lpTargetObj, targetdefense, lpMagic, MsgDamage);

				if ( AttackDamage > 0 )
				{
					gObjWeaponDurDown(lpObj, lpTargetObj, 1);
				}
			}

			else
			{
				AttackDamage = this->GetAttackDamage(lpObj, lpTargetObj, targetdefense, MsgDamage, lpMagic);

				if ( AttackDamage > 0 )
				{
					gObjWeaponDurDown(lpObj, lpTargetObj, 0);
				}
			}
		}

		if ( MsgDamage == 3 )
		{
			int iOption = 0;

			if ( gObjGetValueOfBuffIndex(lpObj, BUFFTYPE_PCS_SCROLL_CRITICAL, &iOption, 0) )
				AttackDamage += iOption * AttackDamage / 100;

			iOption = 0;
			if ( gObjGetValueOfBuffIndex(lpObj, 122, &iOption, 0) )
				AttackDamage += iOption * AttackDamage / 100;

			iOption = 0;
			if (gObjGetValueOfBuffIndex(lpObj, 202, &iOption, 0))
				AttackDamage += iOption * AttackDamage / 100;

			int iMuunEffectValue = 0;

			if (g_CMuunSystem.GetMuunItemValueOfOptType(lpObj, MUUN_INC_CRITICAL_DAMAGE, &iMuunEffectValue, 0) == TRUE)
			{
				AttackDamage += iMuunEffectValue;
			}

			if (lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER && g_CMuunSystem.GetMuunItemValueOfOptType(lpObj, MUUN_INC_CRITICAL_DAMAGE_NONPVP, &iMuunEffectValue, 0) == TRUE)
			{
				AttackDamage += iMuunEffectValue;
			}
		}

		else if ( MsgDamage == 2 )
		{
			int iOption = 0;

			if ( gObjGetValueOfBuffIndex(lpObj, BUFFTYPE_PCS_SCROLL_EXCELLENT, &iOption, 0) )
				AttackDamage += iOption * AttackDamage / 100;

			iOption = 0;
			if ( gObjGetValueOfBuffIndex(lpObj, 123, &iOption, 0) )
				AttackDamage += iOption * AttackDamage / 100;

			iOption = 0;
			if (gObjGetValueOfBuffIndex(lpObj, 203, &iOption, 0))
				AttackDamage += iOption * AttackDamage / 100;

			int iMuunEffectValue = 0;

			if (g_CMuunSystem.GetMuunItemValueOfOptType(lpObj, MUUN_INC_EXCELLENT_DAMAGE, &iMuunEffectValue, 0) == TRUE)
			{
				AttackDamage += iMuunEffectValue;
			}

			if (lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER && g_CMuunSystem.GetMuunItemValueOfOptType(lpObj, MUUN_INC_EXCELLENT_DAMAGE_NONPVP, &iMuunEffectValue, 0) == TRUE)
			{
				AttackDamage += iMuunEffectValue;
			}
		}

		if ( bAllMiss )
		{
			AttackDamage = ( AttackDamage * 30 ) / 100;
		}

		int tlevel;

		if ( lpObj->Type == OBJ_USER )
		{
			tlevel = (lpObj->Level+lpObj->m_PlayerData->MasterLevel) / 10;
		}

		else
		{
			tlevel = lpObj->Level / 10;
		}

		if ( AttackDamage < tlevel )
		{
			if ( tlevel < 1 )
			{
				tlevel = 1;
			}

			AttackDamage = tlevel;
		}

		if ( lpTargetObj->DamageMinus )
		{
			if(lpTargetObj->DamageMinus > g_ConfigRead.calc.MaximumDamageDec)
			{
				lpTargetObj->DamageMinus = g_ConfigRead.calc.MaximumDamageDec;
			}
			int beforeDamage = AttackDamage;
			AttackDamage -= ( ( AttackDamage * (int)lpTargetObj->DamageMinus) / 100 );
		}

		if ( lpTargetObj->m_SkillNumber == 18 )
		{
			if ( AttackDamage > 1 )
			{
				gObjUseSkill.m_Lua.Generic_Call("ShieldSkill_DecreaseVictimDamage", "i>i", AttackDamage, &AttackDamage);
			}
		}

		gObjSpriteDamage(lpTargetObj, AttackDamage);

		if (lpTargetObj->m_btInvenPetPos != 0 && lpTargetObj->m_wInvenPet != (WORD)-1)
		{
			gObjInvenPetDamage(lpTargetObj, AttackDamage);
		}

		g_CMuunSystem.MuunItemDamage(lpTargetObj, AttackDamage);

		if ( gObjSatanSprite(lpObj) == TRUE )
		{
			if ( lpObj->Class != CLASS_RAGEFIGHTER)
			{
				lpObj->Life -= 3.0f;
			}

			else
			{
				if ( skill == 263 ||
					skill == 559 ||
					skill == 563 ||
					skill == 269 ||
					skill == 262 ||
					skill == 558 ||
					skill == 562 )
				{
					lpObj->Life -= 4.0f;
				}

				else if ( skill == 265 || skill == 564 || skill == 566 )
				{
					lpObj->Life -= 100.0f;
				}

				else if ( skill == 260 || skill == 261 || skill == 264 || skill == 551 || skill == 554 || skill == 552 || skill == 555 || skill == 560 || skill == 561 || skill == 270 )
				{
					lpObj->Life -= 2.0f;
				}

				else
				{
					lpObj->Life -= 3.0f;
				}
			}

			if ( lpObj->Life < 0.0f )
			{
				lpObj->Life = 0.0f;
			}
			else
			{
				AttackDamage = AttackDamage + ((AttackDamage * g_ConfigRead.pet.SatanAddDamage) / 100); // 
			}

			GSProtocol.GCReFillSend(lpObj->m_Index, lpObj->Life, 0xFF, 0, lpObj->iShield);
		}

		if ( gObjAngelSprite(lpTargetObj) == TRUE )
		{
			if ( AttackDamage > 1 )
			{
				AttackDamage = AttackDamage * g_ConfigRead.pet.AngelAddDefense / 100;
			}
		}

		if (AttackDamage > 1)
		{
			int iEffectValue = 0;

			if (g_CMuunSystem.GetMuunItemValueOfOptType(lpTargetObj, MUUN_DEC_ENEMY_ATTACK_SKILL, &iEffectValue, NULL) == TRUE)
			{
				AttackDamage = AttackDamage * (100 - iEffectValue) / 100;
			}
		}

		if (gObjSafeGuardSprite(lpTargetObj) == TRUE)
		{
			if (AttackDamage > 1)
			{
				AttackDamage = AttackDamage * (100 - g_ConfigRead.pet.SafeGuardAddDefense) / 100;
			}
		}

		if ( gObjWingSprite(lpObj) == TRUE )
		{
			CItem * Wing = &lpObj->pInventory[7];

			int LifeDec = 0;

			g_ConfigRead.m_ItemCalcLua.Generic_Call("Wings_DecreaseHP", "iiii>i", Wing->m_Type, (int)Wing->m_Durability, lpObj->Class, skill, &LifeDec);
			lpObj->Life -= LifeDec;

			if ( lpObj->Life < 0.0f )
			{
				lpObj->Life = 0.0f;
			}

			else
			{
				g_ConfigRead.m_ItemCalcLua.Generic_Call("Wings_CalcIncAttack", "iii>i", AttackDamage, Wing->m_Type, Wing->m_Level, &AttackDamage);
			}

			GSProtocol.GCReFillSend(lpObj->m_Index, lpObj->Life, 0xFF, 0, lpObj->iShield);
		}

		if ( gObjWingSprite(lpTargetObj) == TRUE )
		{
			CItem * Wing = &lpTargetObj->pInventory[7];

			if (AttackDamage > 1)
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

			if (lpTargetObj->Type == OBJ_USER)
			{
				float fHPRecoveryRate = 0.0;

				if (lpTargetObj->m_PlayerData->m_WingExcOption.iWingOpRecoveryHP != 0)
				{
					bHaveWingOption_FullHP = TRUE;
					fHPRecoveryRate = lpTargetObj->m_PlayerData->m_WingExcOption.iWingOpRecoveryHP;
				}

				if (lpTargetObj->Class == CLASS_RAGEFIGHTER)
				{
					if (lpTargetObj->m_PlayerData->m_MPSkillOpt.iMpsRecoverHPRate_Monk > 0.0)
					{
						fHPRecoveryRate += lpTargetObj->m_PlayerData->m_MPSkillOpt.iMpsRecoverHPRate_Monk;
					}
				}

				else if (lpTargetObj->m_PlayerData->m_MPSkillOpt.iMpsRecoverHPRate > 0.0)
				{
					fHPRecoveryRate += lpTargetObj->m_PlayerData->m_MPSkillOpt.iMpsRecoverHPRate;
				}

				if (rand() % 100 < fHPRecoveryRate)
				{
					gObjAddMsgSendDelay(lpTargetObj, 13, lpTargetObj->m_Index, 100, 0);
				}

				float fManaRecoveryRate = 0.0;

				if (lpTargetObj->m_PlayerData->m_WingExcOption.iWingOpRecoveryMana != 0)
				{
					bHaveWingOption_FullMana = TRUE;
					fManaRecoveryRate = lpTargetObj->m_PlayerData->m_WingExcOption.iWingOpRecoveryMana;
				}

				if (lpTargetObj->Class == CLASS_RAGEFIGHTER)
				{
					if (lpTargetObj->m_PlayerData->m_MPSkillOpt.iMpsRecoverManaRate_Monk > 0.0)
					{
						fManaRecoveryRate += lpTargetObj->m_PlayerData->m_MPSkillOpt.iMpsRecoverManaRate_Monk;
					}
				}

				else if (lpTargetObj->m_PlayerData->m_MPSkillOpt.iMpsRecoverManaRate > 0.0)
				{
					fManaRecoveryRate += lpTargetObj->m_PlayerData->m_MPSkillOpt.iMpsRecoverManaRate;
				}

				if (rand() % 100 < fManaRecoveryRate)
				{
					gObjAddMsgSendDelay(lpTargetObj, 14, lpTargetObj->m_Index, 100, 0);
				}
			}
		}

		if (lpTargetObj->Type == OBJ_USER)
		{
			if (bHaveWingOption_FullHP == FALSE)
			{
				float fHPRecoveryRate = 0.0;

				if (lpTargetObj->Class == CLASS_RAGEFIGHTER)
				{
					if (lpTargetObj->m_PlayerData->m_MPSkillOpt.iMpsRecoverHPRate_Monk > 0.0)
					{
						fHPRecoveryRate += lpTargetObj->m_PlayerData->m_MPSkillOpt.iMpsRecoverHPRate_Monk;
					}
				}

				else if (lpTargetObj->m_PlayerData->m_MPSkillOpt.iMpsRecoverHPRate > 0.0)
				{
					fHPRecoveryRate += lpTargetObj->m_PlayerData->m_MPSkillOpt.iMpsRecoverHPRate;
				}

				if (rand() % 100 < fHPRecoveryRate)
				{
					gObjAddMsgSendDelay(lpTargetObj, 13, lpTargetObj->m_Index, 100, 0);
				}
			}

			if (bHaveWingOption_FullMana == FALSE)
			{
				float fManaRecoveryRate = 0.0;

				if (lpTargetObj->Class == CLASS_RAGEFIGHTER)
				{
					if (lpTargetObj->m_PlayerData->m_MPSkillOpt.iMpsRecoverManaRate_Monk > 0.0)
					{
						fManaRecoveryRate += lpTargetObj->m_PlayerData->m_MPSkillOpt.iMpsRecoverManaRate_Monk;
					}
				}

				else if (lpTargetObj->m_PlayerData->m_MPSkillOpt.iMpsRecoverManaRate > 0.0)
				{
					fManaRecoveryRate += lpTargetObj->m_PlayerData->m_MPSkillOpt.iMpsRecoverManaRate;
				}

				if (rand() % 100 < fManaRecoveryRate)
				{
					gObjAddMsgSendDelay(lpTargetObj, 14, lpTargetObj->m_Index, 100, 0);
				}
			}

			if (lpObj->Type == OBJ_USER && lpObj->Class == CLASS_RAGEFIGHTER)
			{
				if (lpObj->m_PlayerData->m_MPSkillOpt.iMpsRecoverSDRate_Monk > 0.0)
				{
					if (rand() % 100 < lpObj->m_PlayerData->m_MPSkillOpt.iMpsRecoverSDRate_Monk)
					{
						gObjAddMsgSendDelay(lpObj, 15, lpTargetObj->m_Index, 100, 0);
					}
				}
			}

			else if (lpObj->Type == OBJ_USER && lpObj->m_PlayerData->m_MPSkillOpt.iMpsRecoverSDRate > 0.0)
			{
				if (rand() % 100 < lpObj->m_PlayerData->m_MPSkillOpt.iMpsRecoverSDRate)
				{
					gObjAddMsgSendDelay(lpObj, 15, lpTargetObj->m_Index, 100, 0);
				}
			}
		}

		if ( gObjDenorantSprite(lpObj ) )
		{
			lpObj->Life -= 1.0f;

			if ( lpObj->Life < 0.0f )
			{
				lpObj->Life = 0.0f;
			}
			else
			{
				AttackDamage = AttackDamage * 115 / 100;
			}

			GSProtocol.GCReFillSend(lpObj->m_Index, lpObj->Life, 0xFF, 0, lpObj->iShield);
		}

		if ( gObjDenorantSprite(lpTargetObj ) )
		{
			CItem * Dinorant = &lpTargetObj->pInventory[lpTargetObj->m_btInvenPetPos];
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
			CItem * Darkhorse = &lpTargetObj->pInventory[lpTargetObj->m_btInvenPetPos];
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

		if ( lpTargetObj->Live )
		{
			switch ( skill )
			{
				case 19:
				case 20:
				case 21:
				case 22:
				case 23:
				case 41:
				case 42:
				case 43:
				case 44:
				case 49:
				case 55:
				case 57:
					gObjUseSkill.m_Lua.Generic_Call("Knight_Gladiator_CalcSkillBonus", "iii>i", lpObj->Class, AttackDamage, Energy, &AttackDamage);
					break;

				case 47:
					if ( lpObj->pInventory[lpObj->m_btInvenPetPos].m_Type == ITEMGET(13,3) ||
						 lpObj->pInventory[lpObj->m_btInvenPetPos].m_Type == ITEMGET(13,2) ||
						 lpObj->pInventory[lpObj->m_btInvenPetPos].m_Type == ITEMGET(13,37) )
					{
						gObjUseSkill.m_Lua.Generic_Call("ImpaleSkillCalc", "iii>i", lpObj->Class, AttackDamage, Energy, &AttackDamage);
					}
					break;

				case 56:
					gObjUseSkill.m_Lua.Generic_Call("GladiatorPowerSlash", "ii>i", AttackDamage, Energy, &AttackDamage);
					break;

				case 46:
				case 51:
				case 52:
					gObjUseSkill.m_Lua.Generic_Call("Elf_CalcSkillBonus", "ii>i", AttackDamage, Energy, &AttackDamage);
					break;

				case 24:
					if (gObjUseSkill.m_SkillData.UseCalcForTripleShot == true)
					{
						gObjUseSkill.m_Lua.Generic_Call("Elf_CalcTripleShot", "iiiii>i", AttackDamage, Strength, Dexterity, Vitality, Energy, &AttackDamage);
					}
					break;

				case 60:
				case 61:
				case 62:
				case 65:
				case 74:
				case 78:
					gObjUseSkill.m_Lua.Generic_Call("Lord_CalcSkillBonus", "ii>i", AttackDamage, Energy, &AttackDamage);
					break;

				case 76:
					if (lpObj->Type != OBJ_USER) break;
					gObjUseSkill.m_Lua.Generic_Call("FenrirSkillCalc", "iii>i", AttackDamage, lpObj->Level, lpObj->m_PlayerData->MasterLevel, &AttackDamage);
					break;
				case 214:
					{
						int LifeChange = 0;

						if (lpTargetObj->Type == OBJ_MONSTER)
						{
							gObjUseSkill.m_Lua.Generic_Call("SummonerDrainLife_Monster", "ii>i", Energy, lpTargetObj->Level, &LifeChange);
						}

						else if (lpTargetObj->Type == OBJ_USER)
						{
							gObjUseSkill.m_Lua.Generic_Call("SummonerDrainLife_PvP", "ii>i", Energy, AttackDamage, &LifeChange);
						}

						lpObj->Life += LifeChange;

						if ((lpObj->MaxLife + lpObj->AddLife) < lpObj->Life)
						{
							lpObj->Life = lpObj->MaxLife + lpObj->AddLife;
						}

						GSProtocol.GCReFillSend(lpObj->m_Index, lpObj->Life, 0xFF, 0, lpObj->iShield);
					}
					break;
				case 216:
					gObjUseSkill.SplashDamage(lpObj, lpTargetObj, 216, AttackDamage, 1, 50);
					break;
				case 219:
					{
						int SuccessRate = 0;
						int Time = 0;

						if (lpTargetObj->Type == OBJ_MONSTER)
						{
							gObjUseSkill.m_Lua.Generic_Call("Sleep_Monster", "iii>ii", Energy, lpObj->m_CurseSpell, lpTargetObj->Level, &SuccessRate, &Time);
						}

						else if (lpTargetObj->Type == OBJ_USER)
						{
							gObjUseSkill.m_Lua.Generic_Call("Sleep_PvP", "iiii>ii", Energy, lpObj->m_CurseSpell, lpObj->Level, lpTargetObj->Level, &SuccessRate, &Time);
						}

						if(SuccessRate >= rand()% 100)
						{
							if(Time >= 0)
							{
								if (gObjCheckUsedBuffEffect(lpTargetObj, BUFFTYPE_IRON_DEFENSE) == false && gObjCheckUsedBuffEffect(lpTargetObj, BUFFTYPE_IRON_DEFENSE_STR) == false)
								{
									if (gObjCheckPowerfulEffect(lpTargetObj, BUFFTYPE_SLEEP, Time, 0) == TRUE)
									{
										GSProtocol.GCMagicAttackNumberSend(lpObj, AT_SKILL_SLEEP, lpTargetObj->m_Index, 0);
									}
									else
									{
										gObjAddBuffEffect(lpTargetObj, BUFFTYPE_SLEEP, EFFECTTYPE_SLEEP, Time, 0, 0, Time);
									}
								}
							}
							else
							{
								GSProtocol.GCDamageSend(lpObj->m_Index, lpTargetObj->m_Index, 0, 0, 0, 0);
							}
						}
						else
						{
							GSProtocol.GCDamageSend(lpObj->m_Index, lpTargetObj->m_Index, 0, 0, 0, 0);
						}
					}
					return 0;
				case 221:
					{
						int SuccessRate = 0;
						int Value = 0;
						int Time = 0;

						if (lpTargetObj->Type == OBJ_MONSTER)
						{
							gObjUseSkill.m_Lua.Generic_Call("SummonerWeakness_Monster", "iii>iii", Energy, lpObj->m_CurseSpell, lpTargetObj->Level, &SuccessRate, &Value, &Time);
						}

						else if (lpTargetObj->Type == OBJ_USER)
						{
							gObjUseSkill.m_Lua.Generic_Call("SummonerWeakness_PvP", "iiii>iii", Energy, lpObj->m_CurseSpell, lpObj->Level, lpTargetObj->Level, &SuccessRate, &Value, &Time);
						}

						if(SuccessRate < rand()%100)
						{
							GSProtocol.GCDamageSend(lpObj->m_Index, lpTargetObj->m_Index, 0, 0, 0, 0);
							return 0;
						}

						else
						{
							if(Time >= 1)
							{
								if(gObjCheckPowerfulEffect(lpTargetObj, BUFFTYPE_CURSED_ATTACK_DOWN, Value, 0) == TRUE)
								{
									GSProtocol.GCMagicAttackNumberSend(lpObj, skill, lpTargetObj->m_Index, 0);
									return 0;
								}
								else
								{
									gObjAddBuffEffect(lpTargetObj, BUFFTYPE_CURSED_ATTACK_DOWN, EFFECTTYPE_CURSED_ATTACKDOWN, Value, 0, 0, Time);
									return 1;
								}
							}
							else
							{
								GSProtocol.GCDamageSend(lpObj->m_Index, lpTargetObj->m_Index, 0, 0, 0, 0);
								return 0;
							}
						}
						return 0;
					}
				case 222:
					{
						int SuccessRate = 0;
						int Time = 0;
						int Value = 0;

						if(lpTargetObj->Type == OBJ_MONSTER)
						{
							gObjUseSkill.m_Lua.Generic_Call("SummonerInnovation_Monster", "iii>iii", Energy, lpObj->m_CurseSpell, lpTargetObj->Level, &SuccessRate, &Value, &Time);
						}

						else if(lpTargetObj->Type == OBJ_USER)
						{
							gObjUseSkill.m_Lua.Generic_Call("SummonerInnovation_PvP", "iiii>iii", Energy, lpObj->m_CurseSpell, lpObj->Level, lpTargetObj->Level, &SuccessRate, &Value, &Time);
						}

						if(SuccessRate < rand()%100)
						{
							GSProtocol.GCDamageSend(lpObj->m_Index, lpTargetObj->m_Index, 0, 0, 0, 0);
							return 0;
						}

						else
						{
							if(Time >= 1)
							{
								if(gObjCheckPowerfulEffect(lpTargetObj, BUFFTYPE_CURSED_DEFENSE_DOWN, Value, 0) == TRUE)
								{
									GSProtocol.GCMagicAttackNumberSend(lpObj, skill, lpTargetObj->m_Index, 0);
									return 0;
								}
								else
								{
									gObjAddBuffEffect(lpTargetObj, BUFFTYPE_CURSED_DEFENSE_DOWN, EFFECTTYPE_CURSED_DEFENSEDOWN, Value, 0, 0, Time);
									return 1;
								}
							}
							else
							{
								GSProtocol.GCDamageSend(lpObj->m_Index, lpTargetObj->m_Index, 0, 0, 0, 0);
								return 0;
							}
						}
						return 0;
					}
					break;
				case AT_SKILL_CHAIN_LIGHTING:
					gObjUseSkill.m_Lua.Generic_Call("ChainLightningCalc", "ii>i", AttackDamage, bCombo, &AttackDamage);
					bCombo = 0; 
					break;
				case 223:
					{
						lpTargetObj->lpAttackObj = lpObj;
						int DOT = 0, Time = 0;
						gObjUseSkill.m_Lua.Generic_Call("ExplosionDotDamage", "ii>ii", AttackDamage, lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncDotDamage, &DOT, &Time);
						gObjAddBuffEffect(lpTargetObj, BUFFTYPE_FIREDOT, EFFECTTYPE_GIVE_DMG_TICK, DOT, 0, 0, Time);
					}
					break;
				case 224:
					{
						if (gObjCheckUsedBuffEffect(lpTargetObj, BUFFTYPE_IRON_DEFENSE) == false && gObjCheckUsedBuffEffect(lpTargetObj, BUFFTYPE_IRON_DEFENSE_STR) == false)
						{
							lpTargetObj->lpAttackObj = lpObj;
							int DOT = 0, Time = 0;
							gObjUseSkill.m_Lua.Generic_Call("RequiemDotDamage", "i>ii", AttackDamage, &DOT, &Time);
							gObjAddBuffEffect(lpTargetObj, BUFFTYPE_BLOODDOT, EFFECTTYPE_GIVE_DMG_TICK, DOT, 0, 0, Time);
						}
					}
					break;
				case 225:
					if(gObjCheckUsedBuffEffect(lpTargetObj, BUFFTYPE_FREEZE) == FALSE)
					{
						lpTargetObj->lpAttackObj = lpObj;
						lpTargetObj->DelayLevel = 1;
						lpTargetObj->DelayActionTime = 1000;
						gObjAddBuffEffect(lpTargetObj, BUFFTYPE_FREEZE, EFFECTTYPE_REDUCE_MOVE_SPEED, 0, 0, 0, 2);
					}
					break;
				case 230:
					gObjAddBuffEffect(lpTargetObj, BUFFTYPE_LIGHTNINGSHOCK, 0, 0, 0, 0, 1);
					break;
				case 232:
					lpTargetObj->lpAttackObj = lpObj;
					lpTargetObj->DelayLevel = 1;
					lpTargetObj->DelayActionTime = 1000;
					gObjAddBuffEffect(lpTargetObj, BUFFTYPE_COLD, EFFECTTYPE_REDUCE_MOVE_SPEED, 0, 0, 0, 10); // 20 -> 10, previous was wrong
					gObjUseSkill.m_Lua.Generic_Call("StrikeOfDestructionCalc", "ii>i", AttackDamage, Energy, &AttackDamage);
					break;
				case 236:
					gObjUseSkill.m_Lua.Generic_Call("FlameStrikeCalc", "i>i", AttackDamage, &AttackDamage);
					gObjAddBuffEffect(lpTargetObj, BUFFTYPE_FLAMESTRKE, 0, 0, 0, 0, 1);
					break;
				case 237:
					gObjAddBuffEffect(lpTargetObj, BUFFTYPE_GIGANTICSTORM, 0, 0, 0, 0, 1);
					break;
				case 238:
					{
						gObjUseSkill.m_Lua.Generic_Call("ChaoticDiseierCalc", "ii>i", AttackDamage, Energy, &AttackDamage);

						if (lpMagic->m_Skill == 238 && lpObj->Class == 561 && gObjCheckUsedBuffEffect(lpTargetObj, BUFFTYPE_POISON) == FALSE)
						{
							lpTargetObj->DelayActionTime = 800;
							lpTargetObj->DelayLevel = 1;
							lpTargetObj->lpAttackObj = lpObj;

							int iAccessoryEffect = 0;

							if (lpTargetObj->Type == OBJ_USER)
							{
								iAccessoryEffect = lpTargetObj->m_PlayerData->m_Resistance_Stun;
							}

							int iStunRate = 50 - iAccessoryEffect;

							if (rand() % 100 < (100 - iStunRate))
							{
								gObjAddBuffEffect(lpTargetObj, BUFFTYPE_STUN, 0, 0, 0, 0, 5);
							}

							AttackDamage = 1000;
						}
					}
					break;
				case 239:
					gObjBackSpring(lpTargetObj, lpObj);
					break;
				case 250: // selupan
					AttackDamage *= 2;
					break;
				case 251: // selupan
					AttackDamage *= 2.2;
					break;
				case 252: // selupan
					AttackDamage *= 2.3;
					break;
				case 253: // selupan
					AttackDamage *= 2.5;
					break;
				case 260:
					gObjUseSkill.m_Lua.Generic_Call("RageFighterKillingBlow", "ii>i", AttackDamage, Vitality, &AttackDamage);
					break;
				case 261:
					gObjUseSkill.m_Lua.Generic_Call("RageFighterBeastUppercut", "ii>i", AttackDamage, Vitality, &AttackDamage);
					break;
				case 262:
					gObjUseSkill.m_Lua.Generic_Call("RageFighterChainDrive", "ii>i", AttackDamage, Vitality, &AttackDamage);
					break;
				case AT_SKILL_DARK_SIDE:
					gObjUseSkill.m_Lua.Generic_Call("RageFighterDarkSideIncDamage", "iii>i", AttackDamage, Dexterity, Energy, &AttackDamage);
					break;
				case AT_SKILL_DRAGON_ROAR:
					gObjUseSkill.m_Lua.Generic_Call("RageFighterDragonRoar", "ii>i", AttackDamage, Energy, &AttackDamage);
					break;
				case AT_SKILL_DRAGON_SLASH:
					gObjUseSkill.m_Lua.Generic_Call("RageFighterDragonSlasher", "iii>i", AttackDamage, Energy, lpTargetObj->Type, &AttackDamage);
					break;
				case AT_SKILL_PHOENIX_SHOT:
					gObjUseSkill.m_Lua.Generic_Call("RageFighterPhoenixShot", "ii>i", AttackDamage, Vitality, &AttackDamage);
					break;
				case AT_SKILL_CHARGE:
					gObjUseSkill.m_Lua.Generic_Call("RageFighterCharge", "ii>i", AttackDamage, Vitality, &AttackDamage);
					break;
				case AT_SKILL_SPIN_STEP:
					gObjUseSkill.m_Lua.Generic_Call("GrowLancerSpinStep", "ii>i", AttackDamage, Strength, &AttackDamage);
					break;
				case AT_SKILL_HARSH_STRIKE:
					gObjUseSkill.m_Lua.Generic_Call("GrowLancerHarshStrike", "ii>i", AttackDamage, Dexterity, &AttackDamage);
					break;
				case AT_SKILL_MAGIC_PIN:
				case AT_SKILL_MAGIC_PIN_EXPLOSION:
					gObjUseSkill.m_Lua.Generic_Call("GrowLancerMagicPin", "ii>i", AttackDamage, Strength, &AttackDamage);
					break;
				case AT_SKILL_BRECHE:
					gObjUseSkill.m_Lua.Generic_Call("GrowLancerBreche", "ii>i", AttackDamage, Dexterity, &AttackDamage);
					break;
				case AT_SKILL_SHINING_PEAK:
					gObjUseSkill.m_Lua.Generic_Call("GrowLancerShiningPeak", "iii>i", AttackDamage, Strength, Dexterity, &AttackDamage);
					break;
				case AT_SKILL_CLASH:
					{
						int iAccessoryEffect = 0;

						if (lpTargetObj->Type == OBJ_USER)
						{
							iAccessoryEffect = lpTargetObj->m_PlayerData->m_Resistance_Stun;
						}

						int iStunRate = 100 - iAccessoryEffect;

						if (rand() % 100 < (100 - iStunRate))
						{
							gObjAddBuffEffect(lpTargetObj, BUFFTYPE_STUN, 0, 0, 0, 0, 2);
						}
					}
					break;

				default:
				{
					if (g_MasterLevelSkillTreeSystem.CheckMasterLevelSkill(skill) == true)
					{
						double fSkillValue = g_MasterLevelSkillTreeSystem.GetMasterSkillValue(MagicDamageC.SkillGet(lpMagic->m_Skill), lpMagic->m_Level);
						double fPrevBase = 0.0;
						int iMLSBase = g_MasterLevelSkillTreeSystem.GetBaseMasterLevelSkill(skill);

						switch (iMLSBase)
						{
						case 330:
						case 332:
							g_MasterLevelSkillTreeSystem.m_Lua.Generic_Call("TwistingSlashCalc_Master_Knight", "ii>i", AttackDamage, Energy, &AttackDamage);
							break;
						case 481:
							g_MasterLevelSkillTreeSystem.m_Lua.Generic_Call("TwistingSlashCalc_Master_Gladiator", "ii>i", AttackDamage, Energy, &AttackDamage);
							break;
						case 326:
							g_MasterLevelSkillTreeSystem.m_Lua.Generic_Call("CycloneCalc_Master_Knight", "ii>i", AttackDamage, Energy, &AttackDamage);
							break;
						case 479:
							g_MasterLevelSkillTreeSystem.m_Lua.Generic_Call("CycloneCalc_Master_Gladiator", "ii>i", AttackDamage, Energy, &AttackDamage);
							break;
						case 327:
							g_MasterLevelSkillTreeSystem.m_Lua.Generic_Call("SlashCalc_Master_Knight", "ii>i", AttackDamage, Energy, &AttackDamage);
							break;
						case 328:
							g_MasterLevelSkillTreeSystem.m_Lua.Generic_Call("FallingSlashCalc_Master_Knight", "ii>i", AttackDamage, Energy, &AttackDamage);
							break;
						case 329:
							g_MasterLevelSkillTreeSystem.m_Lua.Generic_Call("LungeCalc_Master_Knight", "ii>i", AttackDamage, Energy, &AttackDamage);
							break;
						case 337:
							lpTargetObj->lpAttackObj = lpObj;
							lpTargetObj->DelayActionTime = 1000;
							lpTargetObj->DelayLevel = 1;
							gObjAddBuffEffect(lpTargetObj, BUFFTYPE_COLD, EFFECTTYPE_REDUCE_MOVE_SPEED, 0, 0, 0, 10);
							g_MasterLevelSkillTreeSystem.m_Lua.Generic_Call("StrikeOfDestructionCalc_MasterLevel1_Knight", "ii>i", AttackDamage, Energy, &AttackDamage);
							break;
						case 344:
							g_MasterLevelSkillTreeSystem.m_Lua.Generic_Call("BloodStormCalc_MasterLevel1_Knight", "ii>i", AttackDamage, Energy, &AttackDamage);
							break;
						case 346:
							g_MasterLevelSkillTreeSystem.m_Lua.Generic_Call("BloodStormCalc_MasterLevel2_Knight", "ii>i", AttackDamage, Energy, &AttackDamage);
							break;
						case 495:
						{
							int DebuffRate = 0, DebuffTime = 0;
							g_MasterLevelSkillTreeSystem.m_Lua.Generic_Call("EarthPrison_Level1", ">ii", &DebuffRate, &DebuffTime);

							if (rand() % 100 < DebuffRate)
							{
								gObjAddBuffEffect(lpTargetObj, BUFFTYPE_EARTH_BINDS, 0, 0, 0, 0, DebuffTime);
								lpTargetObj->PathCount = 0;
								lpTargetObj->PathStartEnd = 0;
								gObjSetPosition(lpTargetObj->m_Index, lpTargetObj->X, lpTargetObj->Y);
							}
						}
							break;
						case 497:
						{
							int DebuffRate = 0, DebuffTime = 0;
							g_MasterLevelSkillTreeSystem.m_Lua.Generic_Call("EarthPrison_Level2", ">ii", &DebuffRate, &DebuffTime);

							if (rand() % 100 < DebuffRate)
							{
								gObjAddBuffEffect(lpTargetObj, BUFFTYPE_EARTH_BINDS, 0, 0, 0, 0, DebuffTime);
								lpTargetObj->PathCount = 0;
								lpTargetObj->PathStartEnd = 0;
								gObjSetPosition(lpTargetObj->m_Index, lpTargetObj->X, lpTargetObj->Y);
							}
						}
							break;
						case 427:
							if (skillSuccess == TRUE)
							{
								lpTargetObj->lpAttackObj = lpObj;
								double fDmgValue = 0.0, fTime = 0.0;
								g_MasterLevelSkillTreeSystem.m_Lua.Generic_Call("PoisonArrow_MasterLevel1_DotDamage", "i>dd", lpObj->m_AttackDamageMinRight, &fDmgValue, &fTime);
								gObjAddBuffEffect(lpTargetObj, BUFFTYPE_DEBUFF_POISON1, EFFECTTYPE_GIVE_DMG_TICK, fDmgValue, 0, 0, fTime);
							}
							break;
						case 434:
							if (skillSuccess == TRUE)
							{
								lpTargetObj->lpAttackObj = lpObj;
								double fDmgValue = 0.0, fTime = 0.0;
								g_MasterLevelSkillTreeSystem.m_Lua.Generic_Call("PoisonArrow_MasterLevel2_DotDamage", "id>dd", lpObj->m_AttackDamageMinRight, fSkillValue, &fDmgValue, &fTime);
								gObjAddBuffEffect(lpTargetObj, BUFFTYPE_DEBUFF_POISON2, EFFECTTYPE_GIVE_DMG_TICK, fDmgValue, 0, 0, fTime);
							}
							break;
						case 414:
							if (gObjUseSkill.m_SkillData.UseCalcForTripleShot == true)
							{
								g_MasterLevelSkillTreeSystem.m_Lua.Generic_Call("Elf_CalcTripleShot_Level1", "iiiii>i", AttackDamage, Strength, Dexterity, Vitality, Energy, &AttackDamage);
							}
							break;
						case 418:
							if (gObjUseSkill.m_SkillData.UseCalcForTripleShot == true)
							{
								g_MasterLevelSkillTreeSystem.m_Lua.Generic_Call("Elf_CalcTripleShot_Level2", "iiiii>i", AttackDamage, Strength, Dexterity, Vitality, Energy, &AttackDamage);
							}
							break;
						case 416:
							g_MasterLevelSkillTreeSystem.m_Lua.Generic_Call("PenetrationCalc_Master_Elf", "ii>i", AttackDamage, Energy, &AttackDamage);
							break;
						case 424:
							g_MasterLevelSkillTreeSystem.m_Lua.Generic_Call("IceArrowCalc_Master_Elf", "ii>i", AttackDamage, Energy, &AttackDamage);
							break;
						case 516:
							g_MasterLevelSkillTreeSystem.m_Lua.Generic_Call("EarthShake_MasterLevel2_Lord", "ii>i", AttackDamage, Energy, &AttackDamage);
							if (fSkillValue > 0)
							{
								int iAccessoryEffect = 0;

								if (lpTargetObj->Type == OBJ_USER)
								{
									iAccessoryEffect = lpTargetObj->m_PlayerData->m_Resistance_Stun;
								}

								if (rand() % 100 < fSkillValue - iAccessoryEffect)
								{
									gObjAddBuffEffect(lpTargetObj, BUFFTYPE_STUN, 0, 0, 0, 0, 2);
									gObjSetPosition(lpTargetObj->m_Index, lpTargetObj->X, lpTargetObj->Y);
								}
							}
							break;
						case 514:
							g_MasterLevelSkillTreeSystem.m_Lua.Generic_Call("FireBurst_MasterLevel2_Lord", "ii>i", AttackDamage, Energy, &AttackDamage);
							if (fSkillValue > 0)
							{
								int iAccessoryEffect = 0;

								if (lpTargetObj->Type == OBJ_USER)
								{
									iAccessoryEffect = lpTargetObj->m_PlayerData->m_Resistance_Stun;
								}

								if (rand() % 100 < fSkillValue - iAccessoryEffect)
								{
									gObjAddBuffEffect(lpTargetObj, BUFFTYPE_STUN, 0, 0, 0, 0, 2);
									gObjSetPosition(lpTargetObj->m_Index, lpTargetObj->X, lpTargetObj->Y);
								}
							}
							break;
						case 509:
							g_MasterLevelSkillTreeSystem.m_Lua.Generic_Call("ForceWave_Master_Lord", "ii>i", AttackDamage, Energy, &AttackDamage);
							break;
						case 512:
							g_MasterLevelSkillTreeSystem.m_Lua.Generic_Call("EarthShake_MasterLevel1_Lord", "ii>i", AttackDamage, Energy, &AttackDamage);
							break;
						case 508:
							g_MasterLevelSkillTreeSystem.m_Lua.Generic_Call("FireBurst_MasterLevel1_Lord", "ii>i", AttackDamage, Energy, &AttackDamage);
							break;
						case 336:
							g_MasterLevelSkillTreeSystem.m_Lua.Generic_Call("DeathStab_MasterLevel1_Knight", "ii>i", AttackDamage, Energy, &AttackDamage);
							break;
						case 331:
						case 333:
							g_MasterLevelSkillTreeSystem.m_Lua.Generic_Call("RagefulBlow_Master_Knight", "ii>i", AttackDamage, Energy, &AttackDamage);
							break;
						case 490:
							g_MasterLevelSkillTreeSystem.m_Lua.Generic_Call("FireSlash_MasterLevel1_Gladiator", "ii>i", AttackDamage, Energy, &AttackDamage);
							break;
						case 482:
							g_MasterLevelSkillTreeSystem.m_Lua.Generic_Call("PowerSlash_Master_Gladiator", "ii>i", AttackDamage, Energy, &AttackDamage);
							break;
						case 518:
							g_MasterLevelSkillTreeSystem.m_Lua.Generic_Call("FireScream_MasterLevel1_Lord", "ii>i", AttackDamage, Energy, &AttackDamage);
							break;
						case 551:
						case 554:
							g_MasterLevelSkillTreeSystem.m_Lua.Generic_Call("KillingBlow_Master", "ii>i", AttackDamage, Vitality, &AttackDamage);
							break;
						case 552:
						case 555:
							g_MasterLevelSkillTreeSystem.m_Lua.Generic_Call("BeastUppercut_Master", "ii>i", AttackDamage, Vitality, &AttackDamage);
							break;
						case 558:
						case 562:
							g_MasterLevelSkillTreeSystem.m_Lua.Generic_Call("ChainDrive_Master", "ii>i", AttackDamage, Vitality, &AttackDamage);
							break;
						case 559:
						case 563:
							g_MasterLevelSkillTreeSystem.m_Lua.Generic_Call("RageFighterDarkSideIncDamage", "iii>i", AttackDamage, Dexterity, Energy, &AttackDamage);
							break;
						case 560:
						case 561:
							g_MasterLevelSkillTreeSystem.m_Lua.Generic_Call("DragonRoar_Master", "ii>i", AttackDamage, Energy, &AttackDamage);
							break;
						case 564:
							g_MasterLevelSkillTreeSystem.m_Lua.Generic_Call("DragonSlasher_Level1", "idii>i", AttackDamage, fSkillValue, Energy, lpTargetObj->Type, &AttackDamage);
							break;
						case 566:
						{
							fPrevBase = g_MasterLevelSkillTreeSystem.GetBrandOfMasterSkillValue(lpObj, skill, 1);
							g_MasterLevelSkillTreeSystem.m_Lua.Generic_Call("DragonSlasher_Level2", "idii>i", AttackDamage, fPrevBase, Energy, lpTargetObj->Type, &AttackDamage);

							if (gObjCheckUsedBuffEffect(lpTargetObj, BUFFTYPE_IRON_DEFENSE) == false && gObjCheckUsedBuffEffect(lpTargetObj, BUFFTYPE_IRON_DEFENSE_STR) == false)
							{
								if (fSkillValue > 0.0)
								{
									int iAccessoryEffect = 0;

									if (lpTargetObj->Type == OBJ_USER)
									{
										iAccessoryEffect = lpTargetObj->m_PlayerData->m_Resistance_Stun;
									}

									if (rand() % 100 < fSkillValue - iAccessoryEffect)
									{
										gObjAddBuffEffect(lpTargetObj, BUFFTYPE_STUN, 0, 0, 0, 0, 2);
										gObjSetPosition(lpTargetObj->m_Index, lpTargetObj->X, lpTargetObj->Y);
									}
								}
							}
						}
							break;
						case 394:
							if (gObjCheckUsedBuffEffect(lpTargetObj, BUFFTYPE_IRON_DEFENSE) == false && gObjCheckUsedBuffEffect(lpTargetObj, BUFFTYPE_IRON_DEFENSE_STR) == false)
							{
								if (fSkillValue > 0.0)
								{
									int iAccessoryEffect = 0;

									if (lpTargetObj->Type == OBJ_USER)
									{
										iAccessoryEffect = lpTargetObj->m_PlayerData->m_Resistance_Stun;
									}

									if (rand() % 100 < fSkillValue - iAccessoryEffect)
									{
										gObjAddBuffEffect(lpTargetObj, BUFFTYPE_STUN, 0, 0, 0, 0, 2);
										gObjSetPosition(lpTargetObj->m_Index, lpTargetObj->X, lpTargetObj->Y);
									}
								}
							}
							break;
						case 339:
							g_MasterLevelSkillTreeSystem.m_Lua.Generic_Call("DeathStab_MasterLevel2_Knight", "ii>i", AttackDamage, Energy, &AttackDamage);
							break;
						case 342:
							g_MasterLevelSkillTreeSystem.m_Lua.Generic_Call("DeathStab_MasterLevel3_Knight", "ii>i", AttackDamage, Energy, &AttackDamage);
							break;
						case 340:
							lpTargetObj->lpAttackObj = lpObj;
							lpTargetObj->DelayActionTime = 1000;
							lpTargetObj->DelayLevel = 1;
							gObjAddBuffEffect(lpTargetObj, BUFFTYPE_COLD, EFFECTTYPE_REDUCE_MOVE_SPEED, 0, 0, 0, 10);
							g_MasterLevelSkillTreeSystem.m_Lua.Generic_Call("StrikeOfDestructionCalc_MasterLevel2_Knight", "ii>i", AttackDamage, Energy, &AttackDamage);
							break;
						case 343:
							lpTargetObj->lpAttackObj = lpObj;
							lpTargetObj->DelayActionTime = 1000;
							lpTargetObj->DelayLevel = 1;
							gObjAddBuffEffect(lpTargetObj, BUFFTYPE_COLD, EFFECTTYPE_REDUCE_MOVE_SPEED, 0, 0, 0, 10);
							g_MasterLevelSkillTreeSystem.m_Lua.Generic_Call("StrikeOfDestructionCalc_MasterLevel3_Knight", "ii>i", AttackDamage, Energy, &AttackDamage);
							break;
						case 431:
							if (gObjCheckUsedBuffEffect(lpTargetObj, BUFFTYPE_IRON_DEFENSE) == false && gObjCheckUsedBuffEffect(lpTargetObj, BUFFTYPE_IRON_DEFENSE_STR) == false)
							{
								if (fSkillValue > 0.0)
								{
									int iAccessoryEffect = 0;

									if (lpTargetObj->Type == OBJ_USER)
									{
										iAccessoryEffect = lpTargetObj->m_PlayerData->m_Resistance_Stun;
									}

									if (rand() % 100 < fSkillValue - iAccessoryEffect)
									{
										gObjAddBuffEffect(lpTargetObj, BUFFTYPE_STUN, 0, 0, 0, 0, 2);
										gObjSetPosition(lpTargetObj->m_Index, lpTargetObj->X, lpTargetObj->Y);
									}
								}
							}
							break;
						case 459:
						{
							int iSuccessRate = 0;
							int iEffectTime = 0;
							int iEffectValue = 0;

							if (lpTargetObj->Type == OBJ_MONSTER)
							{
								g_MasterLevelSkillTreeSystem.m_Lua.Generic_Call("SummonerWeakness_Monster", "iii>iii", Energy, lpObj->m_CurseSpell, lpTargetObj->Level, &iSuccessRate, &iEffectValue, &iEffectTime);
							}

							else if (lpTargetObj->Type == OBJ_USER)
							{
								g_MasterLevelSkillTreeSystem.m_Lua.Generic_Call("SummonerWeakness_PvP", "iiii>iii", Energy, lpObj->m_CurseSpell, lpObj->Level, lpTargetObj->Level, &iSuccessRate, &iEffectValue, &iEffectTime);
							}

							if (rand() % 100 > iSuccessRate)
							{
								GSProtocol.GCDamageSend(lpObj->m_Index, lpTargetObj->m_Index, 0, 0, 0, 0);
								return FALSE;
							}

							iEffectTime += fSkillValue;
							iEffectValue += fSkillValue;

							if (iEffectTime < 1)
							{
								GSProtocol.GCDamageSend(lpObj->m_Index, lpTargetObj->m_Index, 0, 0, 0, 0);
								return FALSE;
							}

							if (gObjCheckPowerfulEffect(lpTargetObj, BUFFTYPE_WEAKEN, iEffectValue, 0) == TRUE)
							{
								GSProtocol.GCMagicAttackNumberSend(lpObj, 459, lpTargetObj->m_Index, FALSE);
								return FALSE;
							}

							else
							{
								gObjAddBuffEffect(lpTargetObj, BUFFTYPE_WEAKEN, EFFECTTYPE_CURSED_ATTACKDOWN, iEffectValue, 0, 0, iEffectTime);
								return TRUE;
							}
						}
							break;
						case 460:
						{
							int iSuccessRate = 0;
							int iEffectTime = 0;
							int iEffectValue = 0;

							if (lpTargetObj->Type == OBJ_MONSTER)
							{
								g_MasterLevelSkillTreeSystem.m_Lua.Generic_Call("SummonerInnovation_Monster", "iii>iii", Energy, lpObj->m_CurseSpell, lpTargetObj->Level, &iSuccessRate, &iEffectValue, &iEffectTime);
							}

							else if (lpTargetObj->Type == OBJ_USER)
							{
								g_MasterLevelSkillTreeSystem.m_Lua.Generic_Call("SummonerInnovation_PvP", "iiii>iii", Energy, lpObj->m_CurseSpell, lpObj->Level, lpTargetObj->Level, &iSuccessRate, &iEffectValue, &iEffectTime);
							}

							if (rand() % 100 > iSuccessRate)
							{
								GSProtocol.GCDamageSend(lpObj->m_Index, lpTargetObj->m_Index, 0, 0, 0, 0);
								return FALSE;
							}

							iEffectTime += fSkillValue;
							iEffectValue += fSkillValue;

							if (iEffectTime < 1)
							{
								GSProtocol.GCDamageSend(lpObj->m_Index, lpTargetObj->m_Index, 0, 0, 0, 0);
								return FALSE;
							}

							if (gObjCheckPowerfulEffect(lpTargetObj, BUFFTYPE_INNOVATE, iEffectValue, 0) == TRUE)
							{
								GSProtocol.GCMagicAttackNumberSend(lpObj, 460, lpTargetObj->m_Index, FALSE);
								return FALSE;
							}

							else
							{
								gObjAddBuffEffect(lpTargetObj, BUFFTYPE_INNOVATE, EFFECTTYPE_CURSED_DEFENSEDOWN, iEffectValue, 0, 0, iEffectTime);
								return TRUE;
							}
						}
							break;
						case 461:
							return 0;
						case 463:
							if (gObjCheckUsedBuffEffect(lpTargetObj, BUFFTYPE_IRON_DEFENSE) == false && gObjCheckUsedBuffEffect(lpTargetObj, BUFFTYPE_IRON_DEFENSE_STR) == false)
							{
								if (fSkillValue > 0.0)
								{
									int iAccessoryEffect = 0;

									if (lpTargetObj->Type == OBJ_USER)
									{
										iAccessoryEffect = lpTargetObj->m_PlayerData->m_Resistance_Stun;
									}

									if (rand() % 100 < fSkillValue - iAccessoryEffect)
									{
										gObjAddBuffEffect(lpTargetObj, BUFFTYPE_STUN, 0, 0, 0, 0, 2);
										gObjSetPosition(lpTargetObj->m_Index, lpTargetObj->X, lpTargetObj->Y);
									}
								}
							}
							return 0;
						case 492:
							g_MasterLevelSkillTreeSystem.m_Lua.Generic_Call("FlameStrike_MasterLevel1_Gladiator", "ii>i", AttackDamage, Energy, &AttackDamage);
							gObjAddBuffEffect(lpTargetObj, BUFFTYPE_FLAMESTRKE, 0, 0, 0, 0, 1);
							break;
						case 494:
							g_MasterLevelSkillTreeSystem.m_Lua.Generic_Call("FlameStrike_MasterLevel2_Gladiator", "ii>i", AttackDamage, Energy, &AttackDamage);
							gObjAddBuffEffect(lpTargetObj, BUFFTYPE_FLAMESTRKE, 0, 0, 0, 0, 1);
							break;
						case 493:
							g_MasterLevelSkillTreeSystem.m_Lua.Generic_Call("FireSlash_MasterLevel2_Gladiator", "ii>i", AttackDamage, Energy, &AttackDamage);
							break;
						case 496:
							gObjAddBuffEffect(lpTargetObj, BUFFTYPE_GIGANTICSTORM, 0, 0, 0, 0, 1);
							break;
						case 519:
							g_MasterLevelSkillTreeSystem.m_Lua.Generic_Call("ElectricSpark_Master_Lord", "ii>i", AttackDamage, Energy, &AttackDamage);
							break;
						case 520:
							g_MasterLevelSkillTreeSystem.m_Lua.Generic_Call("FireScream_MasterLevel2_Lord", "ii>i", AttackDamage, Energy, &AttackDamage);
							break;
						case 523:
							g_MasterLevelSkillTreeSystem.m_Lua.Generic_Call("ChaoticDiseier_Master_Lord", "ii>i", AttackDamage, Energy, &AttackDamage);
							break;
						case 462:
						{
							fPrevBase = g_MasterLevelSkillTreeSystem.GetBrandOfMasterSkillValue(lpObj, skill, 1);

							int nAddHP2 = 0;

							if (lpTargetObj->Type == OBJ_MONSTER)
							{
								g_MasterLevelSkillTreeSystem.m_Lua.Generic_Call("SummonerDrainLife_Monster_Level2", "ii>i", Energy, lpTargetObj->Level, &nAddHP2);
							}

							else if (lpTargetObj->Type == OBJ_USER)
							{
								g_MasterLevelSkillTreeSystem.m_Lua.Generic_Call("SummonerDrainLife_PvP_Level2", "ii>i", Energy, AttackDamage, &nAddHP2);
							}

							int tmpLife2 = lpObj->Life + nAddHP2;

							if ((lpObj->AddLife + lpObj->MaxLife) >= tmpLife2)
							{
								lpObj->Life = tmpLife2;
							}

							else
							{
								lpObj->Life = lpObj->AddLife + lpObj->MaxLife;
							}

							GSProtocol.GCReFillSend(lpObj->m_Index, lpObj->Life, 0xFF, 0, lpObj->iShield);

							if (rand() % 100 <= 30)
							{
								gObjAddBuffEffect(lpTargetObj, BUFFTYPE_VAMPIRIC, EFFECTTYPE_FILLHP, fSkillValue, 0, lpObj->m_Index, 5);
								lpTargetObj->lpAttackObj = lpObj;
							}
						}
							break;
						case 454:
						{
							int iSuccessRate = 0;
							int iEffectTime = 0;

							if (lpTargetObj->Type == OBJ_MONSTER)
							{
								g_MasterLevelSkillTreeSystem.m_Lua.Generic_Call("Sleep_Monster", "iii>ii", Energy, lpObj->m_CurseSpell, lpTargetObj->Level, &iSuccessRate, &iEffectTime);
							}

							else if (lpTargetObj->Type == OBJ_USER)
							{
								g_MasterLevelSkillTreeSystem.m_Lua.Generic_Call("Sleep_PvP", "iiii>ii", Energy, lpObj->m_CurseSpell, lpObj->Level, lpTargetObj->Level, &iSuccessRate, &iEffectTime);
							}

							if (rand() % 100 > (iSuccessRate + fSkillValue))
							{
								GSProtocol.GCDamageSend(lpObj->m_Index, lpTargetObj->m_Index, 0, 0, 0, 0);
								return FALSE;
							}

							if (iEffectTime < 1)
							{
								GSProtocol.GCDamageSend(lpObj->m_Index, lpTargetObj->m_Index, 0, 0, 0, 0);
								return FALSE;
							}

							if (gObjCheckUsedBuffEffect(lpTargetObj, BUFFTYPE_IRON_DEFENSE) == true || gObjCheckUsedBuffEffect(lpTargetObj, BUFFTYPE_IRON_DEFENSE_STR) == true)
							{
								GSProtocol.GCDamageSend(lpObj->m_Index, lpTargetObj->m_Index, 0, 0, 0, 0);
								return FALSE;
							}

							if (gObjCheckPowerfulEffect(lpTargetObj, BUFFTYPE_SLEEP, iEffectTime, 0) == TRUE)
							{
								GSProtocol.GCMagicAttackNumberSend(lpObj, 219, lpTargetObj->m_Index, FALSE);
								return FALSE;
							}

							else
							{
								gObjAddBuffEffect(lpTargetObj, BUFFTYPE_SLEEP, EFFECTTYPE_SLEEP, iEffectTime, 0, 0, iEffectTime);
								return TRUE;
							}
						}
							break;
						case 455:
							g_MasterLevelSkillTreeSystem.m_Lua.Generic_Call("ChainLightning_Master_Summoner", "ii>i", AttackDamage, bCombo, &AttackDamage);
							bCombo = 0;
							break;
						case 456:
							gObjAddBuffEffect(lpTargetObj, BUFFTYPE_LIGHTNINGSHOCK, 0, 0, 0, 0, 1);
							break;
						case 458:
						{
							int nAddHP = 0;

							if (lpTargetObj->Type == OBJ_MONSTER)
							{
								g_MasterLevelSkillTreeSystem.m_Lua.Generic_Call("SummonerDrainLife_Monster_Level1", "ii>i", Energy, lpTargetObj->Level, &nAddHP);
							}

							else if (lpTargetObj->Type == OBJ_USER)
							{
								g_MasterLevelSkillTreeSystem.m_Lua.Generic_Call("SummonerDrainLife_PvP_Level1", "ii>i", Energy, AttackDamage, &nAddHP);
							}

							int tmpLife = lpObj->Life + nAddHP;

							if ((lpObj->AddLife + lpObj->MaxLife) >= tmpLife)
							{
								lpObj->Life = tmpLife;
							}

							else
							{
								lpObj->Life = lpObj->AddLife + lpObj->MaxLife;
							}

							GSProtocol.GCReFillSend(lpObj->m_Index, lpObj->Life, 0xFF, 0, lpObj->iShield);
						}
							break;
						default:
							break;
						}
					}
				}
			}

			if (lpObj->pInventory[1].GetDetailItemType() == ITEM_SUMMONER_BOOK &&
				lpObj->pInventory[1].m_Special[0] == skill)
			{
				if (MagicDamageC.GetSkillAttr(skill) == R_FIRE)
				{
					if (lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncDamageFireProper > 0.0f)
					{
						AttackDamage += lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncDamageFireProper;
					}
				}
				else if (MagicDamageC.GetSkillAttr(skill) == R_WIND)
				{
					if (lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncDamageWindProper > 0.0f)
					{
						AttackDamage += lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncDamageWindProper;
					}

					if (lpObj->m_PlayerData->m_MPSkillOpt.iMpsAddSturn2 > 0.0f)
					{
						float fPercent = rand() % 100;
						int iAccessoryEffect = 0;

						if (lpTargetObj->Type == OBJ_USER)
						{
							iAccessoryEffect = lpTargetObj->m_PlayerData->m_Resistance_Stun;
						}

						if (fPercent < lpObj->m_PlayerData->m_MPSkillOpt.iMpsAddSturn2 - iAccessoryEffect)
						{
							gObjAddBuffEffect(lpTargetObj, BUFFTYPE_STUN, 0, 0, 0, 0, 3);
							gObjSetPosition(lpTargetObj->m_Index, lpTargetObj->X, lpTargetObj->Y);
						}
					}
				}
				else if (MagicDamageC.GetSkillAttr(skill) == R_LIGHTNING)
				{
					if (lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncDamageThunderProper > 0.0f)
					{
						AttackDamage += lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncDamageThunderProper;
					}

					if (lpObj->m_PlayerData->m_MPSkillOpt.iMpsAddSpring > 0.0f)
					{
						float fPercent = rand() % 100;

						if (fPercent < lpObj->m_PlayerData->m_MPSkillOpt.iMpsAddSpring)
						{
							gObjBackSpring2(lpTargetObj, lpObj, 2);
						}
					}
				}
			}

			if ( gObjCheckUsedBuffEffect(lpTargetObj, BUFFTYPE_TEMPLE_PROTECTION) == TRUE )
			{
				AttackDamage = 0;
				GSProtocol.GCDamageSend(lpObj->m_Index, lpTargetObj->m_Index, 0, 0 ,0 ,0);
				return 0;
			}

			if ( skill == 0 )
			{
				if ( lpObj->pInventory[lpObj->m_btInvenPetPos].m_Type == ITEMGET(13, 3) )
				{
					AttackDamage = AttackDamage * 130 / 100;
				}
			}
			
			if ( gObjCheckUsedBuffEffect(lpTargetObj, BUFFTYPE_MELEE_DEFENSE_INC) == TRUE && AttackDamage > 0)
			{
				int iWizardSkillDefense = 0;
				int iManaRate = 0;

				gObjGetValueOfBuffIndex(lpTargetObj, BUFFTYPE_MELEE_DEFENSE_INC, &iWizardSkillDefense, &iManaRate);

				int replacemana = 0;

				if(iManaRate > 0)
				{
					replacemana = lpTargetObj->Mana * iManaRate / 1000;
				}
				else replacemana = lpTargetObj->Mana * 2 / 100;

				if( replacemana < lpTargetObj->Mana )
				{
					lpTargetObj->Mana -= replacemana;

					int decattackdamage = 0;

					if(iWizardSkillDefense > 0)
					{
						decattackdamage = AttackDamage * iWizardSkillDefense / 100;
					}
					AttackDamage -= decattackdamage;
					ManaChange = 1;
				}
			}

			else if ( gObjCheckUsedBuffEffect(lpTargetObj, BUFFTYPE_MELEE_DEFENSE_INC_STR)  && AttackDamage > 0)
			{
				int iWizardSkillDefense = 0;
				int iManaRate = 0;

				gObjGetValueOfBuffIndex(lpTargetObj, BUFFTYPE_MELEE_DEFENSE_INC_STR, &iWizardSkillDefense, &iManaRate);

				int replacemana = 0;

				if(iManaRate > 0)
				{
					replacemana = lpTargetObj->Mana * iManaRate / 1000;
				}
				else replacemana = lpTargetObj->Mana * 2 / 100;

				if( replacemana < lpTargetObj->Mana )
				{
					lpTargetObj->Mana -= replacemana;

					int decattackdamage = 0;

					if(iWizardSkillDefense > 0)
					{
						decattackdamage = AttackDamage * iWizardSkillDefense / 100;
					}
					AttackDamage -= decattackdamage;
					ManaChange = 1;
				}
			}

			if(lpObj->Type == OBJ_USER)
			{
				AttackDamage += lpObj->m_PlayerData->SetOpAddDamage;
			}
			if ( lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER )
			{
				AttackDamage += lpObj->m_PlayerData->m_ItemOptionExFor380.OpAddDamage;
			}

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
								if (g_CastleSiege.m_bCastleSiegeFriendlyFire == false)
								{
									AttackDamage = 0;
								}
								else
								{
									AttackDamage = AttackDamage * g_CastleSiege.CastleSiegeSelfDmgReductionPercent / 100;
								}
							}
							else
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
					if ( gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_BLESS_POTION) != NULL )
					{
						AttackDamage += (AttackDamage * 20) / 100;
					}
					else if ( gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_SOUL_POTION)  != NULL )
					{
						AttackDamage = AttackDamage;
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

						AttackDamage = AttackDamage * 5 / 100;
					}
				}

				if ( lpTargetObj->Class == 277 )
				{
					if ( gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_BLESS_POTION) != NULL )
					{
						AttackDamage += (AttackDamage * 20) / 100;
					}
					else if ( gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_SOUL_POTION)  != NULL )
					{
						AttackDamage = AttackDamage;
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

						AttackDamage = AttackDamage * 5 / 100;
					}
				}
			}

			AttackDamage += AttackDamage * lpObj->m_SkillInfo.fInfinityArrowIncRate / 100;

			if ( gObjFenrir( lpObj ) )
			{
				int iIncPercent = 0;

				iIncPercent += lpObj->pInventory[lpObj->m_btInvenPetPos].IsFenrirIncLastAttackDamage();
				iIncPercent += lpObj->pInventory[lpObj->m_btInvenPetPos].IsFenrirIllusion();

				if ( iIncPercent > 0 )
				{
					AttackDamage += AttackDamage * iIncPercent / 100;
				}
			}

			if ( gObjFenrir( lpTargetObj ) )
			{
				int iDecPercent = 0;

				iDecPercent += lpTargetObj->pInventory[lpTargetObj->m_btInvenPetPos].IsFenrirDecLastAttackDamage();
				iDecPercent += lpTargetObj->pInventory[lpTargetObj->m_btInvenPetPos].IsFenrirIllusionDecDmg();
				
				if ( iDecPercent > 0 )
				{
					AttackDamage -= AttackDamage * iDecPercent / 100;
				}
			}

			if (gObjCheckUsedBuffEffect(lpTargetObj, BUFFTYPE_ARCA_DARKNESSTOWER) == TRUE)
			{
				int iArcaEffect = 0;
				gObjGetValueOfBuffIndex(lpTargetObj, BUFFTYPE_ARCA_DARKNESSTOWER, &iArcaEffect, NULL);

				AttackDamage = AttackDamage * (100 - iArcaEffect) / 100;
			}

			int nMuunEffectValue = 0;

			if (g_CMuunSystem.GetMuunItemValueOfOptType(lpTargetObj, MUUN_DEC_ENEMY_DAMAGE, &nMuunEffectValue, 0) == TRUE)
			{
				AttackDamage -= nMuunEffectValue;
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

			if ( AttackDamage < 0 )
				AttackDamage = 0;

			if ( skill == 76 )
			{
				if ( lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER )
				{
					if ( AttackDamage > 0 )
					{
						if (g_ConfigRead.server.GetServerType() != SERVER_CASTLE || (lpObj->m_btCsJoinSide == FALSE || (lpObj->m_btCsJoinSide != lpTargetObj->m_btCsJoinSide)))
						{
							int iEquipmentPos = rand()%5 + 2;	// Select and Armor
							CItem * lpEquipment = &lpTargetObj->pInventory[iEquipmentPos];

							if ( lpEquipment && lpEquipment->IsItem() )
							{
								int iDurDecValue = 50;

								if (lpTargetObj->Class == CLASS_RAGEFIGHTER)
								{
									if (lpTargetObj->m_PlayerData->m_MPSkillOpt.iMpsDownDur1_Monk > 0.0)
									{
										iDurDecValue = lpTargetObj->m_PlayerData->m_MPSkillOpt.btMpsDownDur1Level_Monk + 50;
									}
								}

								else if (lpTargetObj->m_PlayerData->m_MPSkillOpt.iMpsDownDur1 > 0.0)
								{
									iDurDecValue = lpTargetObj->m_PlayerData->m_MPSkillOpt.btMpsDownDur1Level + 50;
								}

								if (iDurDecValue < 0)
									iDurDecValue = 1;

								int iDurEquipment = lpEquipment->m_Durability * iDurDecValue / 100.0f;

								for(int iBuffIndex = 0; iBuffIndex < lpTargetObj->m_BuffEffectCount; iBuffIndex++)
								{
									if( lpTargetObj->m_BuffEffectList[iBuffIndex].BuffIndex == BUFFTYPE_CHARM_PROTECTITEM )
									{
										iDurEquipment = lpEquipment->m_Durability;
										break;
									}
								}

								if (g_LuckyItemManager.IsLuckyItemEquipment(lpEquipment->m_Type))
								{
									iDurEquipment = lpEquipment->m_Durability;
								}

								lpEquipment->m_Durability = iDurEquipment;

								if ( lpEquipment->m_Durability < 0.0f )
								{
									lpEquipment->m_Durability = 0.0f;
								}

								GSProtocol.GCItemDurSend(lpTargetObj->m_Index, iEquipmentPos, lpEquipment->m_Durability, 0);
							}
						}
					}
				}
			}

			if (g_ConfigRead.server.GetServerType() == SERVER_CASTLE)
			{
				if (g_CastleSiege.GetCastleState() != CASTLESIEGE_STATE_STARTSIEGE)
				{
					if (lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_MONSTER)
					{
						if (lpObj->MapNumber == MAP_INDEX_CASTLESIEGE && lpTargetObj->MapNumber == MAP_INDEX_CASTLESIEGE)
						{
							if (lpTargetObj->Class == 277 || lpTargetObj->Class == 283)
							{
								return 0;
							}
						}
					}
				}
			}

			if (!byReflect && lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER)
			{
				if (lpObj->m_PlayerData->m_MPSkillOpt.iMpsTwoHandsPvPAttack > 0.0)
					AttackDamage += lpObj->m_PlayerData->m_MPSkillOpt.iMpsTwoHandsPvPAttack;
				if (lpObj->m_PlayerData->m_MPSkillOpt.iMpsTwoHandsStaffManAttack > 0.0)
					AttackDamage += lpObj->m_PlayerData->m_MPSkillOpt.iMpsTwoHandsStaffManAttack;
				if (lpObj->m_PlayerData->m_MPSkillOpt.iMpsTwoHandsBowManAttack > 0.0)
					AttackDamage += lpObj->m_PlayerData->m_MPSkillOpt.iMpsTwoHandsBowManAttack;
				if (lpObj->m_PlayerData->m_MPSkillOpt.iMpsAddManAttackStick > 0.0)
					AttackDamage += lpObj->m_PlayerData->m_MPSkillOpt.iMpsAddManAttackStick;
				if (lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncCeptorManAttack > 0.0)
					AttackDamage += lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncCeptorManAttack;
			}

			if ( lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER )
			{
				if ( ( skill == 265 || skill == 564 || skill == 566 ) && ( (rand()%100) < ( Energy / 100 +10 ) ) )
				{
					int nSuccessRate = 0, nDecreaseSDRate = 0;
					gObjUseSkill.m_Lua.Generic_Call("DragonSlasher_DecreaseVictimSD", "i>ii", Energy, &nSuccessRate, &nDecreaseSDRate);

					if (rand()%100 < nSuccessRate)
					{
						bDragonKickSDAttackSuccess = TRUE;
						int iDragonKickSDDamage = nDecreaseSDRate * lpTargetObj->iShield / 100;
						lpTargetObj->iShield -= iDragonKickSDDamage;
						lpTargetObj->Life -= AttackDamage;
						iTotalShieldDamage += iDragonKickSDDamage;
					}

					else
					{
						iTempShieldDamage = this->GetShieldDamage(lpObj, lpTargetObj, AttackDamage);
						lpTargetObj->iShield -= iTempShieldDamage;
						lpTargetObj->Life -= AttackDamage - iTempShieldDamage;
						iTotalShieldDamage += iTempShieldDamage;
					}
				}

				else
				{
					iTempShieldDamage = this->GetShieldDamage(lpObj, lpTargetObj, AttackDamage);
					lpTargetObj->iShield -= iTempShieldDamage;
					lpTargetObj->Life -= AttackDamage - iTempShieldDamage;
					iTotalShieldDamage += iTempShieldDamage;
				}

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
	}

	if ( lpTargetObj->Type == OBJ_MONSTER )
	{
		gObjAddMsgSendDelay(lpTargetObj, 0, lpObj->m_Index, 100, 0);
		lpTargetObj->LastAttackerID = lpObj->m_Index;

		if ( lpTargetObj->m_iCurrentAI )
		{
			lpTargetObj->m_Agro->IncAgro(lpObj->m_Index, AttackDamage / 50);
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
			if( gObjDuelCheck(lpObj, lpTargetObj) )
			{
				selfdefense = FALSE;
			}
			else if ( lpObj->m_PlayerData->RegisterdLMS == 1 && lpTargetObj->m_PlayerData->RegisterdLMS == 1)
			{
				if(lpObj->MapNumber == g_LastManStanding.m_Cfg.iPVPMap)
				{
					if(g_LastManStanding.m_Rooms[lpObj->m_PlayerData->RegisteredLMSRoom].bState == 3)
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
			else if ( g_GensSystem.IsMapBattleZone(lpObj->MapNumber) && g_GensSystem.IsMapBattleZone(lpTargetObj->MapNumber) )
			{
				selfdefense = 0;

				if (byReflect == false)
				{
					lpObj->m_PlayerData->m_dwGensHitTeleportDelay = GetTickCount();
				}
			}
			else if ( g_ArcaBattle.IsArcaBattleServer() == TRUE )
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
				if ( !IT_MAP_RANGE(lpObj->MapNumber) )
				{
					selfdefense = TRUE;
				}
			}

			if ( lpTargetObj->Class >= 678 && lpTargetObj->Class == 680 )
			{
				selfdefense = FALSE;
			}
		}

		if ( lpTargetObj->Type == OBJ_USER )
		{
			gObjArmorRandomDurDown(lpTargetObj, lpObj);

			if (skill == 333)
			{
				float fDurDownRate = g_MasterLevelSkillTreeSystem.GetMasterSkillValue(MagicDamageC.SkillGet(lpMagic->m_Skill), lpMagic->m_Level);

				if (rand() % 100 < fDurDownRate)
				{
					if (g_ConfigRead.server.GetServerType() != SERVER_CASTLE || (lpObj->m_btCsJoinSide == 0 || lpObj->m_btCsJoinSide != lpTargetObj->m_btCsJoinSide))
					{
						int item_num[5];
						item_num[0] = 2;
						item_num[1] = 3;
						item_num[2] = 4;
						item_num[3] = 5;
						item_num[4] = 6;
						int iEquipmentPos = rand() % 5;	// Select and Armor
						CItem * lpEquipment = &lpTargetObj->pInventory[item_num[iEquipmentPos]];

						if (lpEquipment && lpEquipment->IsItem())
						{
							float iDurDecValue = 0;

							if (lpTargetObj->Class == CLASS_RAGEFIGHTER && lpTargetObj->m_PlayerData->m_MPSkillOpt.iMpsDownDur1_Monk > 0.0)
							{
								iDurDecValue += lpTargetObj->m_PlayerData->m_MPSkillOpt.btMpsDownDur1Level_Monk / 5.0;
							}

							else if (lpTargetObj->m_PlayerData->m_MPSkillOpt.iMpsDownDur1 > 0.0)
							{
								iDurDecValue += lpTargetObj->m_PlayerData->m_MPSkillOpt.btMpsDownDur1Level / 5.0;
							}

							if (iDurDecValue <= 0.0)
								iDurDecValue = 1.0;

							int damagemin = lpEquipment->m_BaseDurability / 100.0*(10 - iDurDecValue);

							int iDurEquipment = lpEquipment->m_Durability - damagemin;

							for (int iBuffIndex = 0; iBuffIndex < lpTargetObj->m_BuffEffectCount; iBuffIndex++)
							{
								if (lpTargetObj->m_BuffEffectList[iBuffIndex].BuffIndex == BUFFTYPE_CHARM_PROTECTITEM)
								{
									iDurEquipment = lpEquipment->m_Durability;
									break;
								}
							}							
							
							if (g_LuckyItemManager.IsLuckyItemEquipment(lpEquipment->m_Type))
							{
								iDurEquipment = lpEquipment->m_Durability;
							}

							lpEquipment->m_Durability = (float)iDurEquipment;

							if (lpEquipment->m_Durability < 0.0f)
							{
								lpEquipment->m_Durability = 0.0f;
							}

							GSProtocol.GCItemDurSend(lpTargetObj->m_Index, item_num[iEquipmentPos], (BYTE)lpEquipment->m_Durability, 0);
						}
					}
				}
			}
		}

		if ( gObjCheckUsedBuffEffect(lpTargetObj, BUFFTYPE_STONE) == TRUE )
		{
			gObjChangeBuffValidTime(lpTargetObj, BUFFTYPE_STONE, -1);

			if(gObjCheckUsedBuffEffect(lpTargetObj, BUFFTYPE_STONE) == FALSE)
			{
				gObjRemoveBuffEffect(lpTargetObj, BUFFTYPE_STONE);
				GSProtocol.GCMagicCancelSend(lpTargetObj, 51);
				GSProtocol.GCMagicCancelSend(lpTargetObj, 424);
			}
		}

		if ( gObjCheckUsedBuffEffect(lpTargetObj, BUFFTYPE_SLEEP) != 0 )
		{
			gObjRemoveBuffEffect(lpTargetObj, BUFFTYPE_SLEEP);
		}
	}

	if ( selfdefense == TRUE && bDamageReflect == FALSE )
	{
		if ( !gObjTargetGuildWarCheck(lpObj, lpCallObj) )
		{
			if(lpCallObj->PartyNumber >= 0) //Season 2.5 add-on
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

			gObjCheckSelfDefense(lpObj, lpCallObj->m_Index);
		}

	}

	if ( lpTargetObj->Class == 275 )	// KUNDUN
	{
		if ( lpTargetObj->m_iMonsterBattleDelay <= 0 )
		{
			if ( (rand()%15) < 1 )
			{
				gObjAddMsgSendDelay(lpTargetObj, 4, lpObj->m_Index, 100, 0);
				lpTargetObj->m_iMonsterBattleDelay = 10;
				GSProtocol.GCActionSend(lpTargetObj, 126, lpTargetObj->m_Index, lpObj->m_Index);
			}
		}
	}

	if ( lpTargetObj->Class == 131 ||BC_STATUE_RANGE(lpTargetObj->Class-132) )
	{
		gObjAddMsgSendDelay(lpTargetObj, 4, lpObj->m_Index, 100, 0);
		gObjSetPosition(lpTargetObj->m_Index, lpTargetObj->X, lpTargetObj->Y);
	}
	else if ( AttackDamage >= 5 )	// To make strong hit
	{
		if ( lpTargetObj->Type == OBJ_MONSTER )
		{
			if ( (rand()%26) == 0 )
			{
				gObjAddMsgSendDelay(lpTargetObj,4, lpObj->m_Index, 100, 0);
			}
		}
		else if ( (rand()%4) == 0 )
		{
			if ( !gObjUniriaSprite(lpTargetObj) )
			{
				MSBFlag = 1;
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

	if ( ManaChange )
	{
		GSProtocol.GCManaSend(lpTargetObj->m_Index, lpTargetObj->Mana, 0xFF, 0, lpTargetObj->BP);
	}

	if ( magicsend )
	{
		GSProtocol.GCMagicAttackNumberSend(lpObj, skill, lpTargetObj->m_Index, skillSuccess);
	}

	if (bCombo)
	{
		GSProtocol.GCMagicAttackNumberSend(lpObj, AT_SKILL_COMBO, lpTargetObj->m_Index, skillSuccess);
	}

	if ( lpObj->Type == OBJ_USER )
	{
		if ( lpObj->m_Change == 9 )
		{
			GSProtocol.GCMagicAttackNumberSend(lpObj, 3, lpTargetObj->m_Index, 1);
		}
	}

	if ( lpObj->Class == CLASS_ELF && lpObj->Level == 1 && AttackDamage > 10 )
	{
		g_Log.Add("error-Level1 : [%s][%s] Str:%d %d %d %d %d %d %d",
			lpObj->AccountID, lpObj->Name, Strength,
			lpObj->m_AttackDamageMinRight, lpObj->m_AttackDamageMaxRight,
			lpObj->m_AttackDamageMinLeft, lpObj->m_AttackDamageMaxLeft, 
			lpObj->m_AttackDamageMax, lpObj->m_AttackDamageMin);
	}

	lpObj->m_Rest = 0;

	if ( lpObj->Class >= 504 && lpObj->Class <= 511 && gObjCheckUsedBuffEffect(lpObj, 81) == TRUE )
		AttackDamage *= 2;
	if ( lpTargetObj->Class >= 504 && lpTargetObj->Class <= 511 && gObjCheckUsedBuffEffect(lpTargetObj, 81) == TRUE )
		AttackDamage /= 2;

	if ( AttackDamage > 0 )
	{
		int refdmg = lpTargetObj->DamageReflect;

		if (refdmg > g_ConfigRead.calc.ReflectDamage)
		{
			refdmg = g_ConfigRead.calc.ReflectDamage;
		}

		int atd_reflect = AttackDamage * refdmg / 100;

		if ( atd_reflect > 0 )
		{
			gObjAddMsgSendDelay(lpTargetObj, 10, lpObj->m_Index, 10, atd_reflect);
		}

		if ( lpObj->Type == OBJ_USER && (rand()%100) < lpObj->m_PlayerData->SetOpReflectionDamage )
		{
			gObjAddMsgSendDelay(lpTargetObj, 10, lpObj->m_Index, 10, AttackDamage);
		}

		if(lpTargetObj->Type == OBJ_USER && lpTargetObj->m_PlayerData->m_WingExcOption.iWingOpReturnEnemyDamage != 0)
		{
			if(rand()%100 < lpTargetObj->m_PlayerData->m_WingExcOption.iWingOpReturnEnemyDamage )
			{
				if (lpObj->Type == OBJ_MONSTER)
				{
					gObjAddMsgSendDelay(lpTargetObj, 12, lpObj->m_Index, 10, lpObj->m_AttackDamageMax);
				}
				else if (lpObj->Type == OBJ_USER)
				{
					gObjAddMsgSendDelay(lpTargetObj, 12, lpObj->m_Index, 10, AttackDamage);
				}
			}
		}

		if (lpTargetObj->Type == OBJ_USER)
		{
			float fRevengeRate = 0.0;

			if (lpTargetObj->Class == CLASS_RAGEFIGHTER)
			{
				fRevengeRate = lpTargetObj->m_PlayerData->m_MPSkillOpt.iMpsReturnEnemyAttack_Monk;
			}

			else
			{
				fRevengeRate = lpTargetObj->m_PlayerData->m_MPSkillOpt.iMpsReturnEnemyAttack;
			}

			if (fRevengeRate > 0.0 && rand() % 100 <= fRevengeRate)
			{
				if (lpObj->Type == OBJ_MONSTER)
				{
					gObjAddMsgSendDelay(lpTargetObj, 12, lpObj->m_Index, 10, lpObj->m_AttackDamageMax);
				}

				else if (lpObj->Type == OBJ_USER)
				{
					gObjAddMsgSendDelay(lpTargetObj, 12, lpObj->m_Index, 10, AttackDamage);
				}

				return TRUE;
			}
		}

		if ( bCombo )
		{
			int iComboDamage = 0;
			gObjUseSkill.m_Lua.Generic_Call("ComboSkillDamage", "iiii>i", Strength, Dexterity, Vitality, Energy, &iComboDamage);

			if (lpObj->m_PlayerData->m_MPSkillOpt.iMpsComboDamage > 0.0)
				iComboDamage += iComboDamage * lpObj->m_PlayerData->m_MPSkillOpt.iMpsComboDamage / 100.0;

			AttackDamage += iComboDamage;

			if ( lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER )
			{
				iTempShieldDamage = this->GetShieldDamage(lpObj, lpTargetObj, iComboDamage);
				lpTargetObj->iShield -= iTempShieldDamage;
				lpTargetObj->Life -= iComboDamage - iTempShieldDamage;
				iTotalShieldDamage += iTempShieldDamage;

				if ( lpTargetObj->Life < 0.0f )
				{
					lpTargetObj->Life = 0.0f;
				}
			}
			else
			{
				lpTargetObj->Life -= iComboDamage;

				if ( lpTargetObj->Life < 0.0f )
				{
					lpTargetObj->Life = 0.0f;
				}
			}

			MsgDamage |= 0x80;
			skill = 59;
		}

		if (gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_BLOOD_HOWL) == true)
		{
			int SkillEffect = 0;
			int SkillSuccessRate = 0;
			int SkillTime = 0;

			g_MasterLevelSkillTreeSystem.m_Lua.Generic_Call("BloodHowl_Level1", "d>iii", lpTargetObj->Life, &SkillEffect, &SkillSuccessRate, &SkillTime);

			if (rand() % 100 < SkillSuccessRate)
			{
				gObjAddBuffEffect(lpTargetObj, BUFFTYPE_BLEEDING, EFFECTTYPE_GIVE_DMG_TICK, SkillEffect, 0, 0, SkillTime);
			}
		}

		if (gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_BLOOD_HOWL_STR) == true)
		{
			int SkillEffect = 0;
			int SkillSuccessRate = 0;
			int SkillTime = 0;

			g_MasterLevelSkillTreeSystem.m_Lua.Generic_Call("BloodHowl_Level2", "d>iii", lpTargetObj->Life, &SkillEffect, &SkillSuccessRate, &SkillTime);

			SkillSuccessRate += lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncDamageBloodHowling;
			SkillEffect += lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncDamageBloodHowling;

			if (rand() % 100 < SkillSuccessRate)
			{
				gObjAddBuffEffect(lpTargetObj, BUFFTYPE_BLEEDING, EFFECTTYPE_GIVE_DMG_TICK, SkillEffect, 0, 0, SkillTime);
			}
		}

		if (gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_CIRCLE_SHIELD) || gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_CIRCLE_SHIELD_STR) || gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_CIRCLE_SHIELD_MAS))
		{
			if (rand() % 100 < lpObj->m_SkillInfo.fCircleShieldRate)
			{
				int iDecreaseBP = gObjGetTotalValueOfEffect(lpObj, EFFECTTYPE_AG_OPPONENT_DECREASE);
				lpTargetObj->BP -= iDecreaseBP;

				if (lpTargetObj->BP < 0)
				{
					lpTargetObj->BP = 0;
				}
			}
		}

		if (lpObj->Type == OBJ_USER)
		{
			float fRate = 0.0;

			if (lpObj->Class == CLASS_RAGEFIGHTER)
			{
				fRate = lpObj->m_PlayerData->SetOpDoubleDamage + lpObj->m_PlayerData->m_MPSkillOpt.iMpsAddDoubleDamage
					+ lpObj->m_PlayerData->m_MPSkillOpt.iMpsMonkDoubleDamage + lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncDoubleDamageRate_Monk;
			}

			else
			{
				fRate = lpObj->m_PlayerData->SetOpDoubleDamage + lpObj->m_PlayerData->m_MPSkillOpt.iMpsAddDoubleDamage + lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncDoubleDamageRate;
			}

			if (gObjWingSprite(lpObj) == TRUE)
			{
				if (lpObj->pInventory[7].m_Type == ITEMGET(12, 267))
				{
					fRate += 4.0;
				}
			}

			if (lpTargetObj->Type == OBJ_USER)
			{
				fRate -= lpTargetObj->m_PlayerData->m_Resistance_Double;
			}

			if (rand() % 100 < fRate)
			{
				if (skill == 78 || g_MasterLevelSkillTreeSystem.GetBaseMasterLevelSkill(skill) == 518 || skill == 520)
				{
					int iAddDamage = 0;

					if (lpMagic->m_Skill == 520)
					{
						iAddDamage = g_MasterLevelSkillTreeSystem.GetMasterSkillValue(MagicDamageC.SkillGet(lpMagic->m_Skill), lpMagic->m_Level);
					}

					gObjUseSkill.FireScreamExplosionAttack(lpObj, lpTargetObj, AttackDamage, iAddDamage);
				}

				if (lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER)
				{
					if ( skill == 265 || skill == 564 || skill == 566 )
					{
						int nSuccessRate = 0, nDecreaseSDRate = 0;
						gObjUseSkill.m_Lua.Generic_Call("DragonSlasher_DecreaseVictimSD", "i>ii", Energy, &nSuccessRate, &nDecreaseSDRate);

						if (rand()%100 < nSuccessRate)
						{
							bDragonKickSDAttackSuccess = TRUE;
							int iDragonKickSDDamage = nDecreaseSDRate * lpTargetObj->iShield / 100;
							lpTargetObj->iShield -= iDragonKickSDDamage;
							lpTargetObj->Life -= AttackDamage;
							iTotalShieldDamage += iDragonKickSDDamage;
						}

						else
						{
							iTempShieldDamage = this->GetShieldDamage(lpObj, lpTargetObj, AttackDamage);
							lpTargetObj->iShield -= iTempShieldDamage;
							lpTargetObj->Life -= AttackDamage - iTempShieldDamage;
							iTotalShieldDamage += iTempShieldDamage;
						}
					}

					else
					{
						iTempShieldDamage = this->GetShieldDamage(lpObj, lpTargetObj, AttackDamage);
						lpTargetObj->iShield -= iTempShieldDamage;
						lpTargetObj->Life -= AttackDamage - iTempShieldDamage;
						iTotalShieldDamage += iTempShieldDamage;
					}

					AttackDamage *= 2;

					if (lpTargetObj->Life < 0.0f)
					{
						lpTargetObj->Life = 0.0f;
					}
				}
				else
				{
					lpTargetObj->Life -= AttackDamage;
					AttackDamage *= 2;

					if (lpTargetObj->Life < 0.0f)
					{
						lpTargetObj->Life = 0.0f;
					}
				}

				MsgDamage |= 0x40;
			}

			float fTripleRate = 0.0;

			if (lpObj->Class == CLASS_RAGEFIGHTER)
				fTripleRate = lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncTripleDamageRate_Monk;
			else
				fTripleRate = lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncTripleDamageRate;

			if (fTripleRate > 0.0 && !(MsgDamage & 0x40) && !(MsgDamage & 1) && (rand() % 100) < fTripleRate)
			{
				int nDoubleDamage = AttackDamage * 2;

				if (skill == 78 || g_MasterLevelSkillTreeSystem.GetBaseMasterLevelSkill(skill) == 518 || skill == 520)
				{
					int iAddDamage = 0;

					if (lpMagic->m_Skill == 520)
					{
						iAddDamage = g_MasterLevelSkillTreeSystem.GetMasterSkillValue(MagicDamageC.SkillGet(lpMagic->m_Skill), lpMagic->m_Level);
					}

					gObjUseSkill.FireScreamExplosionAttack(lpObj, lpTargetObj, AttackDamage, iAddDamage);

					if ( skill == 265 || skill == 564 || skill == 566 )
					{
						int nSuccessRate = 0, nDecreaseSDRate = 0;
						gObjUseSkill.m_Lua.Generic_Call("DragonSlasher_DecreaseVictimSD", "i>ii", Energy, &nSuccessRate, &nDecreaseSDRate);

						if (rand()%100 < nSuccessRate)
						{
							bDragonKickSDAttackSuccess = TRUE;
							int iDragonKickSDDamage = nDecreaseSDRate * lpTargetObj->iShield / 100;
							lpTargetObj->iShield -= iDragonKickSDDamage;
							lpTargetObj->Life -= nDoubleDamage;
							iTotalShieldDamage += iDragonKickSDDamage;
						}

						else
						{
							iTempShieldDamage = this->GetShieldDamage(lpObj, lpTargetObj, nDoubleDamage);
							lpTargetObj->iShield -= iTempShieldDamage;
							lpTargetObj->Life -= nDoubleDamage - iTempShieldDamage;
							iTotalShieldDamage += iTempShieldDamage;
						}
					}

					else
					{
						iTempShieldDamage = this->GetShieldDamage(lpObj, lpTargetObj, nDoubleDamage);
						lpTargetObj->iShield -= iTempShieldDamage;
						lpTargetObj->Life -= nDoubleDamage - iTempShieldDamage;
						iTotalShieldDamage += iTempShieldDamage;
					}

					AttackDamage *= 3;

					if (lpTargetObj->Life < 0.0f)
					{
						lpTargetObj->Life = 0.0f;
					}
				}
				else
				{
					lpTargetObj->Life -= nDoubleDamage;
					AttackDamage *= 3;

					if (lpTargetObj->Life < 0.0f)
					{
						lpTargetObj->Life = 0.0f;
					}
				}

				MsgDamage |= 0x100;
			}
		}

		if ( g_ShieldSystemOn == TRUE && !bDragonKickSDAttackSuccess )
		{
			AttackDamage -= iTotalShieldDamage;
		}

		if ( g_ShieldSystemOn == FALSE )
		{
			if ( lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER )
			{
				if ( CC_MAP_RANGE(lpObj->MapNumber) && CC_MAP_RANGE(lpTargetObj->MapNumber) )
				{
					AttackDamage = AttackDamage * 50 / 100;
				}
			}
		}

		if (RFAttack)
		{
			if (lpObj->Class == CLASS_RAGEFIGHTER)
			{
				if (skill == 261 || skill == 263 || skill == 552 || skill == 555 || skill == 559 || skill == 563)
				{
					if (RFAttack % 2)
					{
						MsgDamage |= 0x10;
					}

					else
					{
						MsgDamage |= 0x20;
					}
				}

				else
				{
					if (RFAttack % 4)
					{
						MsgDamage |= 0x10;
					}

					else
					{
						MsgDamage |= 0x20;
					}
				}
			}

			else if (lpObj->Class == CLASS_GROWLANCER)
			{
				if (skill == 276 || skill == 274 || skill == 277)
				{
					if (RFAttack % 2)
					{
						MsgDamage |= 0x10;
					}

					else
					{
						MsgDamage |= 0x20;
					}
				}
			}
		}

		gObjLifeCheck(lpTargetObj, lpObj, AttackDamage, 0, MSBFlag, MsgDamage, skill, iTotalShieldDamage, 0);

		if ( iTotalShieldDamage > 0 )
		{
			g_Log.Add("[PvP System] Victim:[%s][%s], Attacker:[%s][%s] - SD[%d] HP[%f] -> SD[%d] HP[%f]", lpTargetObj->AccountID, lpTargetObj->Name, lpObj->AccountID, lpObj->Name, lpTargetObj->iShield + iTotalShieldDamage, lpTargetObj->Life + AttackDamage, lpTargetObj->iShield, lpTargetObj->Life);
		}
	}
	else
	{
		if ( lpObj->Type != OBJ_MONSTER || lpObj->m_iPentagramMainAttribute <= 0 )
		{
			GSProtocol.GCDamageSend(lpObj->m_Index, lpTargetObj->m_Index, 0, 0, MsgDamage, 0);
		}
	}

	if (byPentagramAttack == TRUE)
	{
		this->PentagramAttack(lpObj, lpTargetObj, lpMagic, MSBFlag, MsgDamage, AttackDamage, iTargetDefense);
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

	CItem* Right = &lpObj->pInventory[0];
	CItem* Left = &lpObj->pInventory[1];

	if (Right->GetDetailItemType() == ITEM_MACE || Left->GetDetailItemType() == ITEM_MACE)
	{
		if (lpObj->m_PlayerData->m_MPSkillOpt.iMpsAddSturn > 0.0)
		{
			if (gObjCheckUsedBuffEffect(lpTargetObj, BUFFTYPE_IRON_DEFENSE) == false && gObjCheckUsedBuffEffect(lpTargetObj, BUFFTYPE_IRON_DEFENSE_STR) == false)
			{
				float fPercent = rand() % 100;
				int iAccessoryEffect = 0;

				if (lpTargetObj->Type == OBJ_USER)
				{
					iAccessoryEffect = lpTargetObj->m_PlayerData->m_Resistance_Stun;
				}

				if (fPercent < lpObj->m_PlayerData->m_MPSkillOpt.iMpsAddSturn - iAccessoryEffect)
				{
					gObjAddBuffEffect(lpTargetObj, BUFFTYPE_STUN, 0, 0, 0, 0, 2);
					gObjSetPosition(lpTargetObj->m_Index, lpTargetObj->X, lpTargetObj->Y);
				}
			}
		}
	}

	if (lpMagic)
	{
		if (lpMagic->m_Skill == 332)
		{
			float fSpringValue = g_MasterLevelSkillTreeSystem.GetMasterSkillValue(MagicDamageC.SkillGet(lpMagic->m_Skill), lpMagic->m_Level);

			if (fSpringValue > 0.0f)
			{
				float fPercent = rand() % 100;

				if (fPercent < fSpringValue)
				{
					gObjBackSpring2(lpTargetObj, lpObj, 2);
				}
			}
		}

		else if (lpMagic->m_Skill == 494)
		{
			float fSpringValue = g_MasterLevelSkillTreeSystem.GetMasterSkillValue(MagicDamageC.SkillGet(lpMagic->m_Skill), lpMagic->m_Level);

			if (fSpringValue > 0.0f)
			{
				float fPercent = rand() % 100;

				if (fPercent < fSpringValue)
				{
					gObjBackSpring2(lpTargetObj, lpObj, 2);
				}
			}
		}
	}

	if ( lpMagic )
	{
		gObjUseSkill.SpecificSkillAdditionTreat(lpObj, lpTargetObj, lpMagic, AttackDamage);
	}

	if ( g_ConfigRead.antihack.EnabledAntiRefTimeCheck && MSBFlag )
	{
		if ( lpTargetObj->Type == OBJ_USER )
		{
			lpTargetObj->m_PlayerData->m_dwMSBFlagAttackDisable = GetTickCount();
		}
	}

	return TRUE;
}

BOOL CObjAttack::PentagramAttack(LPOBJ lpObj, LPOBJ lpTargetObj, CMagicInf* lpMagic, BYTE MSBFlag, int MsgDamage, int AttackDamage, int iTargetDefense)
{
	bool bPentagramEquip = false;
	int PentagramAttackDamage = 0;
	char PentagramDamageType1;
	char PentagramDamageType2;

	if ((lpObj->Type == OBJ_USER && lpMagic) || lpObj->Type == OBJ_MONSTER)
	{
		if (lpObj->Type == OBJ_USER && g_PentagramSystem.IsPentagramItem(lpObj->pInventory[236].m_Type) && lpObj->pInventory[236].m_IsValidItem)
		{
			bPentagramEquip = true;
			PentagramAttackDamage = this->GetElementalDamage(lpObj, lpTargetObj, &PentagramDamageType1, &PentagramDamageType2, AttackDamage, iTargetDefense);
		}

		else if (lpObj->Type == OBJ_MONSTER && lpObj->m_iPentagramMainAttribute > 0)
		{
			bPentagramEquip = TRUE;
			PentagramAttackDamage = this->GetElementalDamage(lpObj, lpTargetObj, &PentagramDamageType1, &PentagramDamageType2, AttackDamage, iTargetDefense);
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
						if (g_CastleSiege.m_bCastleSiegeFriendlyFire == false)
						{
							PentagramAttackDamage = 0;
						}
					}
				}
			}
		}
	}

	if (PentagramAttackDamage > 0)
	{
		if (lpObj->Type == OBJ_USER)
		{
			if (lpObj->m_PlayerData->m_PentagramOptions.m_iOnyx_1RankSlowMoveSkillRate > 0)
			{
				if (rand() % 100 < lpObj->m_PlayerData->m_PentagramOptions.m_iOnyx_1RankSlowMoveSkillRate)
				{
					gObjAddBuffEffect(lpTargetObj, BUFFTYPE_CONFUSION, EFFECTTYPE_REDUCE_MOVE_SPEED, 0, 0, 0, 20);
				}
			}

			if (lpObj->m_PlayerData->m_PentagramOptions.m_iOnyx_5RankHalfValueSkillRate > 0)
			{
				if (rand() % 100 < lpObj->m_PlayerData->m_PentagramOptions.m_iOnyx_5RankHalfValueSkillRate)
				{
					int iDuration = 0;

					switch (PentagramDamageType1)
					{
					case EL_FIRE:
						gObjUseSkill.m_Lua.Generic_Call("ElementalDebuffGetDuration", "ii>i", BUFFTYPE_ACHERON_FIRE, (lpObj->Level + lpObj->m_PlayerData->MasterLevel), &iDuration);
						gObjAddBuffEffect(lpTargetObj, BUFFTYPE_ACHERON_FIRE, 83, 0, 0, 0, iDuration);
						gObjCalCharacter.CalcCharacter(lpTargetObj->m_Index);
						break;
					case EL_WATER:
						gObjUseSkill.m_Lua.Generic_Call("ElementalDebuffGetDuration", "ii>i", BUFFTYPE_ACHERON_FROST, (lpObj->Level + lpObj->m_PlayerData->MasterLevel), &iDuration);
						gObjAddBuffEffect(lpTargetObj, BUFFTYPE_ACHERON_FROST, 82, 0, 0, 0, iDuration);
						gObjCalCharacter.CalcCharacter(lpTargetObj->m_Index);
						break;
					case EL_EARTH:
						gObjUseSkill.m_Lua.Generic_Call("ElementalDebuffGetDuration", "ii>i", BUFFTYPE_ACHERON_TORNADO, (lpObj->Level + lpObj->m_PlayerData->MasterLevel), &iDuration);
						gObjAddBuffEffect(lpTargetObj, BUFFTYPE_ACHERON_TORNADO, 84, 0, 0, 0, iDuration);
						gObjCalCharacter.CalcCharacter(lpTargetObj->m_Index);
						break;
					case EL_WIND:
						gObjUseSkill.m_Lua.Generic_Call("ElementalDebuffGetDuration", "ii>i", BUFFTYPE_ACHERON_BIND, (lpObj->Level + lpObj->m_PlayerData->MasterLevel), &iDuration);
						gObjAddBuffEffect(lpTargetObj, BUFFTYPE_ACHERON_BIND, 81, 0, 0, 0, iDuration);
						gObjCalCharacter.CalcCharacter(lpTargetObj->m_Index);
						break;
					case EL_DARKNESS:
						gObjUseSkill.m_Lua.Generic_Call("ElementalDebuffGetDuration", "ii>i", BUFFTYPE_ACHERON_DARKNESS, (lpObj->Level + lpObj->m_PlayerData->MasterLevel), &iDuration);
						gObjAddBuffEffect(lpTargetObj, BUFFTYPE_ACHERON_DARKNESS, 0, 0, 0, 0, iDuration);
						gObjCalCharacter.CalcCharacter(lpTargetObj->m_Index);
						break;
					}
				}
			}
		}
	}

	if (bPentagramEquip)
	{
		if (PentagramAttackDamage <= 0)
		{
			if (PentagramDamageType2)
			{
				GSProtocol.GCElementalDamageSend(lpObj->m_Index, lpTargetObj->m_Index, 0, PentagramDamageType2);
			}

			else
			{
				GSProtocol.GCElementalDamageSend(lpObj->m_Index, lpTargetObj->m_Index, 0, PentagramDamageType1);
			}
		}

		else
		{
			int PentagramSDDamage = 0;

			if (lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER)
			{
				PentagramSDDamage = 90 * PentagramAttackDamage / 100;
				PentagramAttackDamage -= PentagramSDDamage;
			}

			if (PentagramSDDamage <= 0)
			{
				lpTargetObj->Life -= PentagramAttackDamage;
			}

			else
			{
				if (lpTargetObj->iShield > 0)
				{
					if (lpTargetObj->iShield < PentagramSDDamage)
					{
						PentagramSDDamage -= lpTargetObj->iShield;
						lpTargetObj->iShield = 0;
						lpTargetObj->Life -= PentagramSDDamage - PentagramAttackDamage;
					}

					else
					{
						lpTargetObj->iShield -= PentagramSDDamage;
					}
				}

				else if (g_ConfigRead.calc.ElementalDamageAffectSDOnly == false)
				{
					lpTargetObj->Life -= (PentagramAttackDamage + PentagramSDDamage);
				}
			}

			if (PentagramSDDamage > 0)
			{
				PentagramAttackDamage += PentagramSDDamage;
			}

			if (lpTargetObj->Life < 0.0)
			{
				lpTargetObj->Life = 0.0;
			}

			if (lpTargetObj->Type == OBJ_USER)
			{
				GSProtocol.GCReFillSend(lpTargetObj->m_Index, lpTargetObj->Life, 0xFF, 0, lpTargetObj->iShield);
			}

			if (PentagramDamageType2)
			{
				gObjLifeCheck(lpTargetObj, lpObj, PentagramAttackDamage, 0, MSBFlag, PentagramDamageType2, 0, 0, TRUE);
			}

			else
			{
				gObjLifeCheck(lpTargetObj, lpObj, PentagramAttackDamage, 0, MSBFlag, PentagramDamageType1, 0, 0, TRUE);
			}
		}
	}

	return TRUE;
}

BOOL CObjAttack::GetBuffTypePhysicalIncrease(LPOBJ lpObj, int *iAttackBerserkerMin, int *iAttackBerserkerMax, int nBuffType)
{
	gObjUseSkill.m_Lua.Generic_Call("SummonerBerserkerAttackDamage", "ii>ii", (lpObj->m_PlayerData->Strength + lpObj->AddStrength), (lpObj->m_PlayerData->Dexterity + lpObj->AddDexterity), iAttackBerserkerMin, iAttackBerserkerMax);

	int iValue = 0;
	gObjGetValueOfBuffIndex(lpObj, nBuffType, &iValue, 0);

	*iAttackBerserkerMin = iValue * *iAttackBerserkerMin / 100;
	*iAttackBerserkerMax = iValue * *iAttackBerserkerMax / 100;

	if (lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncValueBerserker3 > 0.0)
	{
		*iAttackBerserkerMin += *iAttackBerserkerMin * lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncValueBerserker3 / 100.0;
		*iAttackBerserkerMax  += *iAttackBerserkerMax * lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncValueBerserker3 / 100.0;
	}

	return TRUE;
}


int  CObjAttack::GetAttackDamage(LPOBJ lpObj, LPOBJ lpTargetObj, int targetDefense, int& effect, CMagicInf* lpMagic)
{
	int ad;
	int sub;
	int SkillRightMaxDamage = 0;
	int SkillRightMinDamage = 0;
	int SkillLeftMaxDamage = 0;
	int SkillLeftMinDamage = 0;
	CItem * Right = &lpObj->pInventory[0];
	CItem * Left = &lpObj->pInventory[1];
	BOOL bTwoHandWeapon = FALSE;

	if ( lpObj->Type == OBJ_MONSTER || lpObj->Type == OBJ_NPC )
	{
		int AttackMin = 0;
		sub = lpObj->m_AttackDamageMax - lpObj->m_AttackDamageMin;
		
		if (sub < 0)
		{
			g_Log.Add("Monster Class [%d] Damage is 0, check files configuration", lpObj->Class);
			return 0;
		}

		AttackMin = lpObj->m_AttackDamageMin + (rand()%(sub+1));
		lpObj->m_AttackDamageRight = AttackMin;
		lpObj->m_AttackDamageLeft = AttackMin;
	}
	else
	{
		int cDamage=0;

		if ( lpObj->Class == CLASS_DARKLORD ||
			 lpObj->Class == CLASS_KNIGHT   ||
			 lpObj->Class == CLASS_MAGUMSA  ||
			 lpObj->Class == CLASS_RAGEFIGHTER )
		{
			if ( Right->m_Type >= ITEMGET(0,0) && Right->m_Type < ITEMGET(4,0) &&
				 Left->m_Type >= ITEMGET(0,0) && Left->m_Type < ITEMGET(4,0) )
			{
				if ( Right->m_IsValidItem && Left->m_IsValidItem )
				{
					bTwoHandWeapon = TRUE;
				}
			}
		}

		if ( lpMagic )
		{
			if ( (lpMagic ->m_Skill == 60 || lpMagic->m_Skill == 509) && lpObj->SkillLongSpearChange )	// #error 60 and skil from GEt is 66???
			{
				SkillRightMaxDamage = DefMagicInf[66].m_DamageMax;// #error ???
				SkillRightMinDamage = DefMagicInf[66].m_DamageMin;// #error ???
			}
			else
			{
				SkillRightMaxDamage = lpMagic->m_DamageMax;
				SkillRightMinDamage = lpMagic->m_DamageMin;
			}

			SkillRightMinDamage += g_SkillSpec.CalcStatBonus(lpObj, lpMagic->m_Skill);
			SkillRightMaxDamage += g_SkillSpec.CalcStatBonus(lpObj, lpMagic->m_Skill);

			if (g_MasterLevelSkillTreeSystem.CheckMasterLevelSkill(lpMagic->m_Skill) == true)
			{
				float fDamage = g_MasterLevelSkillTreeSystem.GetSkillAttackDamage(lpObj, lpMagic->m_Skill);
				SkillRightMaxDamage += fDamage;
				SkillRightMinDamage += fDamage;
			}

			if ( bTwoHandWeapon == FALSE )
			{
				SkillLeftMaxDamage = SkillRightMaxDamage;
				SkillLeftMinDamage = SkillRightMinDamage;
			}			
			
			SkillRightMaxDamage += lpObj->m_PlayerData->SetOpAddSkillAttack;
			SkillRightMinDamage += lpObj->m_PlayerData->SetOpAddSkillAttack;
			SkillLeftMaxDamage += lpObj->m_PlayerData->SetOpAddSkillAttack;
			SkillLeftMinDamage += lpObj->m_PlayerData->SetOpAddSkillAttack;
			
			if ( gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_ARCA_FIRETOWER) == TRUE )
			{
				int nEffectValue = 0;
				gObjGetValueOfBuffIndex(lpObj, BUFFTYPE_ARCA_FIRETOWER, &nEffectValue, NULL);

				SkillRightMaxDamage += nEffectValue;
				SkillRightMinDamage += nEffectValue;
				SkillLeftMaxDamage += nEffectValue;
				SkillLeftMinDamage += nEffectValue;
			}

			if (gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_OBSIDIAN) == TRUE)
			{
				int nEffectValue = 0;
				gObjGetValueOfBuffIndex(lpObj, BUFFTYPE_OBSIDIAN, &nEffectValue, NULL);

				SkillRightMaxDamage += nEffectValue;
				SkillRightMinDamage += nEffectValue;
				SkillLeftMaxDamage += nEffectValue;
				SkillLeftMinDamage += nEffectValue;
			}

			if (gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_OBSIDIAN_STR) == TRUE)
			{
				int nEffectValue = 0;
				gObjGetValueOfBuffIndex(lpObj, BUFFTYPE_OBSIDIAN_STR, &nEffectValue, NULL);

				SkillRightMaxDamage += nEffectValue;
				SkillRightMinDamage += nEffectValue;
				SkillLeftMaxDamage += nEffectValue;
				SkillLeftMinDamage += nEffectValue;
			}

			int nMuunItemEffectValue = 0;

			if (g_CMuunSystem.GetMuunItemValueOfOptType(lpObj, MUUN_INC_SKILL_POWER, &nMuunItemEffectValue, 0) == TRUE)
			{
				SkillRightMaxDamage += nMuunItemEffectValue;
				SkillRightMinDamage += nMuunItemEffectValue;
				SkillLeftMaxDamage += nMuunItemEffectValue;
				SkillLeftMinDamage += nMuunItemEffectValue;
			}

			if (lpObj->Type == OBJ_USER && lpTargetObj->Type != OBJ_USER)
			{
				nMuunItemEffectValue = 0;
				g_CMuunSystem.GetMuunItemValueOfOptType(lpObj, MUUN_INC_ATTACK_POWER_NONPVP, &nMuunItemEffectValue, 0);

				SkillRightMaxDamage += nMuunItemEffectValue;
				SkillRightMinDamage += nMuunItemEffectValue;
				SkillLeftMaxDamage += nMuunItemEffectValue;
				SkillLeftMinDamage += nMuunItemEffectValue;

				nMuunItemEffectValue = 0;
				g_CMuunSystem.GetMuunItemValueOfOptType(lpObj, MUUN_INC_MAX_ATTACK_POWER_NONPVP, &nMuunItemEffectValue, 0);

				SkillRightMaxDamage += nMuunItemEffectValue;
				SkillLeftMaxDamage += nMuunItemEffectValue;

				nMuunItemEffectValue = 0;
				g_CMuunSystem.GetMuunItemValueOfOptType(lpObj, MUUN_INC_SKILL_POWER_NONPVP, &nMuunItemEffectValue, 0);

				SkillRightMaxDamage += nMuunItemEffectValue;
				SkillRightMinDamage += nMuunItemEffectValue;
				SkillLeftMaxDamage += nMuunItemEffectValue;
				SkillLeftMinDamage += nMuunItemEffectValue;
			}

			int SkillAttr = MagicDamageC.GetSkillAttr(lpMagic->m_Skill);

			if ( SkillAttr != -1 )
			{
				SkillRightMaxDamage += (BYTE)lpObj->m_AddResistance[SkillAttr];
				SkillRightMinDamage += (BYTE)lpObj->m_AddResistance[SkillAttr];
				SkillLeftMaxDamage  += (BYTE)lpObj->m_AddResistance[SkillAttr];
				SkillLeftMinDamage  += (BYTE)lpObj->m_AddResistance[SkillAttr];
			}

			SkillRightMaxDamage += lpObj->m_PlayerData->m_JewelOfHarmonyEffect.HJOpAddSkillAttack;
			SkillRightMinDamage += lpObj->m_PlayerData->m_JewelOfHarmonyEffect.HJOpAddSkillAttack;
			SkillLeftMaxDamage += lpObj->m_PlayerData->m_JewelOfHarmonyEffect.HJOpAddSkillAttack;
			SkillLeftMinDamage += lpObj->m_PlayerData->m_JewelOfHarmonyEffect.HJOpAddSkillAttack;

			if ( lpMagic->m_Skill == 65 || lpMagic->m_Skill == 519 )
			{
				int iPartyCount = this->GetPartyMemberCount(lpObj);
				int addskilldamage = ( lpObj->Leadership + lpObj->AddLeadership ) / 10 + ( iPartyCount * 50 );	// #formula

				SkillRightMaxDamage += addskilldamage;
				SkillRightMinDamage += addskilldamage;
				SkillLeftMaxDamage += addskilldamage;
				SkillLeftMinDamage += addskilldamage;
			}
			else if( lpMagic->m_Skill == 62 || g_MasterLevelSkillTreeSystem.GetBaseMasterLevelSkill(lpMagic->m_Skill) == 512 || lpMagic->m_Skill == 516 )
			{
				if ( lpObj->pInventory[lpObj->m_btInvenPetPos].m_Type == ITEMGET(13,4) )	// Dark Horse
				{
					int addskilldamage = ( lpObj->m_PlayerData->Strength + lpObj->AddStrength ) / 10 + ( lpObj->Leadership + lpObj->AddLeadership ) / 5 + lpObj->pInventory[lpObj->m_btInvenPetPos].m_PetItem_Level * 10;	// #formula

					addskilldamage = addskilldamage * g_ConfigRead.calc.DarkHorseDamageMultiplier / 100.0f;

					SkillRightMaxDamage += addskilldamage;
					SkillRightMinDamage += addskilldamage;
					SkillLeftMaxDamage += addskilldamage;
					SkillLeftMinDamage += addskilldamage;

				}
			}

			else if (lpObj->Class == CLASS_DARKLORD && (lpMagic->m_Skill == 238 || lpMagic->m_Skill == 523))
			{
				int iadddamage = (lpObj->m_PlayerData->Strength + lpObj->AddStrength) / 30 + (lpObj->m_PlayerData->Energy + lpObj->AddEnergy) / 55;	// #formula

				SkillRightMaxDamage += iadddamage;
				SkillRightMinDamage += iadddamage;
				SkillLeftMaxDamage += iadddamage;
				SkillLeftMinDamage += iadddamage;
			}

			else if ( lpObj->Class == CLASS_DARKLORD )
			{
				int iadddamage = ( lpObj->m_PlayerData->Strength + lpObj->AddStrength ) / 25 + ( lpObj->m_PlayerData->Energy + lpObj->AddEnergy ) / 50;	// #formula

				SkillRightMaxDamage += iadddamage;
				SkillRightMinDamage += iadddamage;
				SkillLeftMaxDamage += iadddamage;
				SkillLeftMinDamage += iadddamage;
			}

			if (lpObj->Class == CLASS_ELF && (lpMagic->m_Skill == 235 || lpMagic->m_Skill == 411 || lpMagic->m_Skill == 431)) //season4 add-on
			{
				SkillLeftMaxDamage = SkillLeftMaxDamage * 80 / 100;
				SkillLeftMinDamage = SkillLeftMinDamage * 80 / 100;
			}
		}		

		int nCritical = lpObj->m_CriticalDamage;
		int nExcellent = lpObj->m_ExcelentDamage;

		if (lpObj->Class == CLASS_RAGEFIGHTER)
		{
			if (lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncCriticalDamageRate_Monk > 0.0)
				nCritical = (nCritical + lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncCriticalDamageRate_Monk);
			if (lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncExcellentDamageRate_Monk > 0.0)
				nExcellent = (nExcellent + lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncExcellentDamageRate_Monk);
		}
		else
		{
			if (lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncCriticalDamageRate > 0.0)
				nCritical = (nCritical + lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncCriticalDamageRate);
			if (lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncExcellentDamageRate > 0.0)
				nExcellent = (nExcellent + lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncExcellentDamageRate);
		}

		if (gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_CRITICAL_DMG_INC_MAS) == true && lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncExcellentDamageRate_Darklord > 0.0)
		{
			nExcellent += lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncExcellentDamageRate_Darklord;
		}

		if (lpTargetObj->Type == OBJ_USER)
		{
			nCritical -= lpTargetObj->m_PlayerData->m_Resistance_Critical;
			nExcellent -= lpTargetObj->m_PlayerData->m_Resistance_Excellent;
		}
		
		if ( nCritical > 0 )
		{
			if ((rand() % 100) < nCritical)
			{
				cDamage = TRUE;
				effect = 3;
			}
		}

		if ( nExcellent > 0 )
		{
			if ((rand() % 100) < nExcellent)
			{
				cDamage = TRUE;
				effect = 2;
			}
		}

		int iAttackBerserkerMin = 0;
		int iAttackBerserkerMax = 0;
		int iMuunOptDamage = 0;
		int iMuunOptDamageMax = 0;

		if (lpObj->Type == OBJ_USER && gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_BERSERKER))
			this->GetBuffTypePhysicalIncrease(lpObj, &iAttackBerserkerMin, &iAttackBerserkerMax, BUFFTYPE_BERSERKER);
		else if(lpObj->Type == OBJ_USER && gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_BERSERKER_STR))
			this->GetBuffTypePhysicalIncrease(lpObj, &iAttackBerserkerMin, &iAttackBerserkerMax, BUFFTYPE_BERSERKER_STR);
		else if(lpObj->Type == OBJ_USER && gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_BERSERKER_MAS))
			this->GetBuffTypePhysicalIncrease(lpObj, &iAttackBerserkerMin, &iAttackBerserkerMax, BUFFTYPE_BERSERKER_MAS);
		else if(lpObj->Type == OBJ_USER && gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_BERSERKER_PRO))
			this->GetBuffTypePhysicalIncrease(lpObj, &iAttackBerserkerMin, &iAttackBerserkerMax, BUFFTYPE_BERSERKER_PRO);

		if (lpObj->Type == OBJ_USER && lpTargetObj->Type != OBJ_USER) // Non PvP
		{
			g_CMuunSystem.GetMuunItemValueOfOptType(lpObj, MUUN_INC_ATTACK_POWER_NONPVP, &iMuunOptDamage, 0);
			g_CMuunSystem.GetMuunItemValueOfOptType(lpObj, MUUN_INC_MAX_ATTACK_POWER_NONPVP, &iMuunOptDamageMax, 0);
		}

		__try
		{
			sub = ( lpObj->m_AttackDamageMaxRight + SkillRightMaxDamage + iAttackBerserkerMax + iMuunOptDamage + iMuunOptDamageMax ) - ( lpObj->m_AttackDamageMinRight + SkillRightMinDamage + iAttackBerserkerMin + iMuunOptDamage );	// #formula
			lpObj->m_AttackDamageRight = ( lpObj->m_AttackDamageMinRight + SkillRightMinDamage + iAttackBerserkerMin + iMuunOptDamage ) + (rand()%(sub+1));
		}
		__except ( sub=1, 1 )
		{

		}

		__try
		{
			sub = ( lpObj->m_AttackDamageMaxLeft + SkillLeftMaxDamage + iAttackBerserkerMax + iMuunOptDamage + iMuunOptDamageMax ) - ( lpObj->m_AttackDamageMinLeft + SkillLeftMinDamage + iAttackBerserkerMin + iMuunOptDamage );	// #formula
			lpObj->m_AttackDamageLeft = ( lpObj->m_AttackDamageMinLeft + SkillLeftMinDamage + iAttackBerserkerMin + iMuunOptDamage ) + (rand()%(sub+1));
		}
		__except ( sub=1, 1 )
		{

		}

		if ( cDamage )
		{
			lpObj->m_AttackDamageRight = lpObj->m_AttackDamageMaxRight + SkillRightMaxDamage + iAttackBerserkerMax + iMuunOptDamage + iMuunOptDamageMax;
			lpObj->m_AttackDamageLeft = lpObj->m_AttackDamageMaxLeft + SkillLeftMaxDamage + iAttackBerserkerMax + iMuunOptDamage + iMuunOptDamageMax;

			lpObj->m_AttackDamageRight += lpObj->m_PlayerData->SetOpAddCriticalDamage;
			lpObj->m_AttackDamageLeft += lpObj->m_PlayerData->SetOpAddCriticalDamage;

			lpObj->m_AttackDamageRight += lpObj->m_PlayerData->m_JewelOfHarmonyEffect.HJOpAddCriticalDamage;
			lpObj->m_AttackDamageLeft += lpObj->m_PlayerData->m_JewelOfHarmonyEffect.HJOpAddCriticalDamage;

			int iOption = 0;
			gObjGetValueOfBuffIndex(lpObj, BUFFTYPE_CRITICAL_DMG_INC, &iOption, 0);

			lpObj->m_AttackDamageRight += iOption;
			lpObj->m_AttackDamageLeft += iOption;

			iOption = 0;
			gObjGetValueOfBuffIndex(lpObj, BUFFTYPE_CRITICAL_DMG_INC_STR, &iOption, 0);

			lpObj->m_AttackDamageRight += iOption;
			lpObj->m_AttackDamageLeft += iOption;

			iOption = 0;
			gObjGetValueOfBuffIndex(lpObj, BUFFTYPE_CRITICAL_DMG_INC_MAS, &iOption, 0);

			lpObj->m_AttackDamageRight += iOption;
			lpObj->m_AttackDamageLeft += iOption;

			if ( effect == 2 )
			{
				lpObj->m_AttackDamageRight += ( lpObj->m_AttackDamageMaxRight + SkillRightMaxDamage + iAttackBerserkerMax + iMuunOptDamage + iMuunOptDamageMax) * 20 / 100;
				lpObj->m_AttackDamageLeft += ( lpObj->m_AttackDamageMaxLeft + SkillLeftMaxDamage + iAttackBerserkerMax + iMuunOptDamage + iMuunOptDamageMax) * 20 / 100;

				lpObj->m_AttackDamageRight += lpObj->m_PlayerData->SetOpAddExDamage;
				lpObj->m_AttackDamageLeft += lpObj->m_PlayerData->SetOpAddExDamage;
			}
		}
	}

	if ( bTwoHandWeapon )
	{
		ad = ( lpObj->m_AttackDamageRight + lpObj->m_AttackDamageLeft ) - targetDefense;
		ad -= lpObj->m_PlayerData->SetOpAddCriticalDamage; //season4 add-on
	}

	else if ((Left->m_Type >= ITEMGET(4, 0) && Left->m_Type < ITEMGET(4, 7)) ||
		Left->m_Type == ITEMGET(4, 20) || Left->m_Type == ITEMGET(4, 21) ||
		Left->m_Type == ITEMGET(4, 22) ||
		Left->m_Type == ITEMGET(4, 23) ||//season4 add-on
		Left->m_Type == ITEMGET(4, 24)) //season4.6 add-on
	{
		ad = lpObj->m_AttackDamageLeft - targetDefense;

		if (lpObj->m_PlayerData->m_MPSkillOpt.iMpsBowAddDamage > 0.0)
		{
			ad += lpObj->m_PlayerData->m_MPSkillOpt.iMpsBowAddDamage;
		}
	}

	else if ((Right->m_Type >= ITEMGET(4, 8) && Right->m_Type < ITEMGET(4, 15)) ||
		(Right->m_Type >= ITEMGET(4, 16) && Right->m_Type < ITEMGET(5, 0)))
	{
		ad = lpObj->m_AttackDamageRight - targetDefense;

		if (lpObj->m_PlayerData->m_MPSkillOpt.iMpsTwoHandsBowAddDamage > 0.0)
		{
			ad += lpObj->m_PlayerData->m_MPSkillOpt.iMpsTwoHandsBowAddDamage;
		}
	}

	else if (Right->GetDetailItemType() == ITEM_TWOHAND_SWORD && Right->GetDetailItemType() == ITEM_TWOHAND_SWORD)
	{
		ad = lpObj->m_AttackDamageRight - targetDefense;

		if (lpObj->m_PlayerData->m_MPSkillOpt.iMpsTwoHandSwordAddDamage > 0.0)
		{
			ad += lpObj->m_PlayerData->m_MPSkillOpt.iMpsTwoHandSwordAddDamage;
		}
	}

	else if ( Right->m_Type >= ITEMGET(0,0) && Right->m_Type < ITEMGET(4,0) )
	{
		ad = lpObj->m_AttackDamageRight - targetDefense;
	}

	else if ( Right->m_Type >= ITEMGET(5,0) && Right->m_Type < ITEMGET(6,0) )
	{
		ad = lpObj->m_AttackDamageRight - targetDefense;

	}

	else
	{
		ad = lpObj->m_AttackDamageLeft - targetDefense;
	}

	if ( lpObj->Type == OBJ_USER && lpObj->m_PlayerData->SetOpTwoHandSwordImproveDamage )
	{
		ad += ad * lpObj->m_PlayerData->SetOpTwoHandSwordImproveDamage  / 100;
	}
	
	if ( lpObj->m_SkillInfo.RemedyOfLoveEffect )
	{
		ad += 15;
	}

	if ( lpObj->m_MonsterSkillElementInfo.m_iSkillElementAttackTime > 0 )
	{
		ad += lpObj->m_MonsterSkillElementInfo.m_iSkillElementAttack;

		if ( ad < 0 )
		{
			ad = 0;
		}
	}

	int BuffAtk = gObjGetTotalValueOfEffect(lpObj, EFFECTTYPE_IMPROVE_DAMAGE);
	int BuffForce = gObjGetTotalValueOfEffect(lpObj, EFFECTTYPE_MELEEDAMAGE);
	
	ad += BuffAtk;
	ad += BuffForce;

	if (gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_ARCA_WINDTOWER))
	{
		int iArcaEffect = 0;
		gObjGetValueOfBuffIndex(lpObj, BUFFTYPE_ARCA_WINDTOWER, &iArcaEffect, NULL);

		ad += iArcaEffect;
	}

	int iValue = 0;
	gObjGetValueOfBuffIndex(lpObj, BUFFTYPE_CURSED_ATTACK_DOWN, &iValue, 0);
	ad -= iValue * ad / 100;

	iValue = 0;

	if (gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_WEAKEN) == true)
	{
		gObjGetValueOfBuffIndex(lpObj, BUFFTYPE_WEAKEN, &iValue, 0);
		ad -= iValue * ad / 100;
	}

	float nAddAttackDamageLeft = 0.0;
	float nAddAttackDamageRight = 0.0;

	if (lpObj->Type == OBJ_USER)
	{
		switch (Left->GetDetailItemType())
		{
		case ITEM_ONEHAND_SWORD:
		{
			if (lpObj->m_PlayerData->m_MPSkillOpt.iMpsSwordAddDamage > 0.0)
			{
				nAddAttackDamageLeft += lpObj->m_PlayerData->m_MPSkillOpt.iMpsSwordAddDamage;
			}
		}
			break;
		case ITEM_MACE:
		{
			if (lpObj->m_PlayerData->m_MPSkillOpt.iMpsScultAddDamage > 0.0)
			{
				nAddAttackDamageLeft += lpObj->m_PlayerData->m_MPSkillOpt.iMpsScultAddDamage;
			}
		}
			break;
		case ITEM_SPEAR:
		{
			if (lpObj->m_PlayerData->m_MPSkillOpt.iMpsSpearAddDamage > 0.0)
			{
				nAddAttackDamageLeft += lpObj->m_PlayerData->m_MPSkillOpt.iMpsSpearAddDamage;
			}
		}
			break;
		case ITEM_DARKLORD_SCEPTER:
		{
			if (lpObj->m_PlayerData->m_MPSkillOpt.iMpsAddCeptorDamage > 0.0)
			{
				nAddAttackDamageLeft += lpObj->m_PlayerData->m_MPSkillOpt.iMpsAddCeptorDamage;
			}
		}
			break;
		case ITEM_RAGEFIGHTER_WEAPON:
		{
			if (lpObj->m_PlayerData->m_MPSkillOpt.iMpsMonkWeaponDamage > 0.0)
			{
				nAddAttackDamageLeft += lpObj->m_PlayerData->m_MPSkillOpt.iMpsMonkWeaponDamage;
			}
		}
			break;
		}

		switch (Right->GetDetailItemType())
		{
		case ITEM_ONEHAND_SWORD:
		{
			if (lpObj->m_PlayerData->m_MPSkillOpt.iMpsSwordAddDamage > 0.0)
			{
				nAddAttackDamageRight += lpObj->m_PlayerData->m_MPSkillOpt.iMpsSwordAddDamage;
			}
		}
			break;
		case ITEM_MACE:
		{
			if (lpObj->m_PlayerData->m_MPSkillOpt.iMpsScultAddDamage > 0.0)
			{
				nAddAttackDamageRight += lpObj->m_PlayerData->m_MPSkillOpt.iMpsScultAddDamage;
			}
		}
			break;
		case ITEM_SPEAR:
		{
			if (lpObj->m_PlayerData->m_MPSkillOpt.iMpsSpearAddDamage > 0.0)
			{
				nAddAttackDamageRight += lpObj->m_PlayerData->m_MPSkillOpt.iMpsSpearAddDamage;
			}
		}
			break;
		case ITEM_DARKLORD_SCEPTER:
		{
			if (lpObj->m_PlayerData->m_MPSkillOpt.iMpsAddCeptorDamage > 0.0)
			{
				nAddAttackDamageRight += lpObj->m_PlayerData->m_MPSkillOpt.iMpsAddCeptorDamage;
			}
		}
			break;
		case ITEM_RAGEFIGHTER_WEAPON:
		{
			if (lpObj->m_PlayerData->m_MPSkillOpt.iMpsMonkWeaponDamage > 0.0)
			{
				nAddAttackDamageRight += lpObj->m_PlayerData->m_MPSkillOpt.iMpsMonkWeaponDamage;
			}
		}
			break;
		}
	}

	if (bTwoHandWeapon)
	{
		ad += nAddAttackDamageRight * 0.5 + nAddAttackDamageLeft * 0.5;
	}
	else
	{
		ad += nAddAttackDamageRight + nAddAttackDamageLeft;
	}

	if (lpObj->Type == OBJ_USER)
	{
		if (lpObj->Class == CLASS_KNIGHT
			|| lpObj->Class == CLASS_MAGUMSA
			|| lpObj->Class == CLASS_DARKLORD
			|| lpObj->Class == CLASS_RAGEFIGHTER)
		{
			if (lpObj->m_PlayerData->m_MPSkillOpt.iMpsAddPhysicDamage > 0.0)
			{
				ad += lpObj->m_PlayerData->m_MPSkillOpt.iMpsAddPhysicDamage;
			}
		}

		if (lpObj->Class == CLASS_ELF)
		{
			if (lpObj->m_PlayerData->m_MPSkillOpt.iMpsElfAddPhysicDamage > 0.0)
			{
				ad += lpObj->m_PlayerData->m_MPSkillOpt.iMpsElfAddPhysicDamage;
			}
		}

		if (lpObj->m_PlayerData->m_MPSkillOpt.iMpsAddControlAttack > 0.0)
		{
			if (lpObj->pInventory[0].GetDetailItemType() == ITEM_DARKLORD_SCEPTER || lpObj->pInventory[1].GetDetailItemType() == ITEM_DARKLORD_SCEPTER)
			{
				int nAddAttack = (lpObj->AddLeadership + lpObj->Leadership) / lpObj->m_PlayerData->m_MPSkillOpt.iMpsAddControlAttack;
				ad += nAddAttack;
			}
		}

		if (lpObj->pInventory[7].IsItem() == TRUE && lpObj->pInventory[7].m_IsValidItem == true)
		{
			if (lpObj->Class == CLASS_KNIGHT || lpObj->Class == CLASS_ELF || lpObj->Class == CLASS_MAGUMSA || lpObj->Class == CLASS_DARKLORD || lpObj->Class == CLASS_RAGEFIGHTER)
			{
				ad += lpObj->m_PlayerData->m_MPSkillOpt.iMpsAddWingDamage;
			}
		}
	}

	if (ad < 0)
	{
		ad = 0;
	}

	return ad;
}

int CObjAttack::GetAttackDamageSummoner(LPOBJ lpObj, LPOBJ lpTargetObj, int targetDefense, CMagicInf* lpMagic, int& effect)
{
	double fCurseDamageMin = 0;
	double fCurseDamageMax = 0;
	double fMagicDamageMin = 0;
	double fMagicDamageMax = 0;
	int damagemin;
	int damagemax;
	int ad;
	int iIncreaseDamage = 0;

	ad = lpMagic->GetDamage();

	switch (lpMagic->m_Skill)
	{
	case 219:
	case 220:
	case 221:
	case 222:
	case 223:
	case 224:
	case 225:
	case 459:
	case 460:
	{
		if (gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_BERSERKER) == true)
		{
			int iValue = gObjGetTotalValueOfEffect(lpObj, EFFECTTYPE_BERSERKER_UP);
			gObjUseSkill.m_Lua.Generic_Call("SummonerBerserkerCurseDamage", "ii>dd", (lpObj->m_PlayerData->Energy + lpObj->AddEnergy), iValue, &fCurseDamageMin, &fCurseDamageMax);
		}

		if (gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_BERSERKER_STR) == true
			|| gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_BERSERKER_MAS) == true
			|| gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_BERSERKER_PRO) == true)
		{
			int iValue = gObjGetTotalValueOfEffect(lpObj, 31);

			if (lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncPercentBerserker1 > 0.0)
			{
				iValue += lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncPercentBerserker1;
			}

			if (lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncValueBerserker3 > 0.0)
			{
				iValue += lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncValueBerserker3;
			}

			gObjUseSkill.m_Lua.Generic_Call("SummonerBerserkerCurseDamage", "ii>dd", (lpObj->m_PlayerData->Energy + lpObj->AddEnergy), iValue, &fCurseDamageMin, &fCurseDamageMax);
		}

		damagemin = lpObj->m_CurseDamageMin + fCurseDamageMin + lpMagic->m_DamageMin + g_SkillSpec.CalcStatBonus(lpObj, lpMagic->m_Skill);
		damagemax = lpObj->m_CurseDamageMax + fCurseDamageMax + lpMagic->m_DamageMax + g_SkillSpec.CalcStatBonus(lpObj, lpMagic->m_Skill);

		if (g_MasterLevelSkillTreeSystem.CheckMasterLevelSkill(lpMagic->m_Skill) == true)
		{
			float fDamage = g_MasterLevelSkillTreeSystem.GetSkillAttackDamage(lpObj, lpMagic->m_Skill);
			damagemin += fDamage;
			damagemax += fDamage;
		}

		if (lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncMagicPower > 0.0)
		{
			damagemin += lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncMagicPower;
			damagemax += lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncMagicPower;
		}

		if (lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncMinMagicDamage > 0.0)
		{
			damagemin += lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncMinMagicDamage;
		}

		if (lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncMaxMagicDamage > 0.0)
		{
			damagemax += lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncMaxMagicDamage;
		}

		if (lpObj->pInventory[1].IsItem() && (lpObj->pInventory[1].m_Type >= ITEMGET(5, 0) && lpObj->pInventory[1].m_Type < ITEMGET(6, 0)) && lpObj->pInventory[1].m_IsValidItem)
		{
			int damage = lpObj->pInventory[1].m_CurseSpell / 2 + lpObj->pInventory[1].m_Level * 2;	// #formula
			damage -= (WORD)(lpObj->pInventory[1].m_CurrentDurabilityState * damage);	// #formula

			damagemin += damagemin * damage / 100;	// #formula
			damagemax += damagemax * damage / 100;	// #formula
		}

		iIncreaseDamage += gObjGetTotalValueOfEffect(lpObj, EFFECTTYPE_CURSED_DAMAGE);
		break;
	}
	default:
	{
		if (gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_BERSERKER) == true)
		{
			int iValue = gObjGetTotalValueOfEffect(lpObj, 31);
			gObjUseSkill.m_Lua.Generic_Call("SummonerBerserkerMagicDamage", "ii>dd", (lpObj->m_PlayerData->Energy + lpObj->AddEnergy), iValue, &fMagicDamageMin, &fMagicDamageMax);
		}

		if (gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_BERSERKER_STR) == true
			|| gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_BERSERKER_MAS) == true
			|| gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_BERSERKER_PRO) == true)
		{
			int iValue = gObjGetTotalValueOfEffect(lpObj, 31);

			if (lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncPercentBerserker2 > 0.0)
			{
				iValue += lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncPercentBerserker2;
			}

			gObjUseSkill.m_Lua.Generic_Call("SummonerBerserkerMagicDamage", "ii>dd", (lpObj->m_PlayerData->Energy + lpObj->AddEnergy), iValue, &fMagicDamageMin, &fMagicDamageMax);
		}

		if (lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncMagicPower > 0.0)
		{
			fMagicDamageMin += lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncMagicPower;
			fMagicDamageMax += lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncMagicPower;
		}

		if (lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncMinMagicDamage > 0.0)
		{
			fMagicDamageMin += lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncMinMagicDamage;
		}

		if (lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncMaxMagicDamage > 0.0)
		{
			fMagicDamageMax += lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncMaxMagicDamage;
		}

		damagemin = (int)((float)(lpMagic->m_DamageMin + lpObj->m_MagicDamageMin) + fMagicDamageMin + g_SkillSpec.CalcStatBonus(lpObj, lpMagic->m_Skill));
		damagemax = (int)((float)(lpMagic->m_DamageMax + lpObj->m_MagicDamageMax) + fMagicDamageMax + g_SkillSpec.CalcStatBonus(lpObj, lpMagic->m_Skill));

		if (g_MasterLevelSkillTreeSystem.CheckMasterLevelSkill(lpMagic->m_Skill) == true)
		{
			float fDamage = g_MasterLevelSkillTreeSystem.GetSkillAttackDamage(lpObj, lpMagic->m_Skill);
			damagemin += fDamage;
			damagemax += fDamage;
		}

		if (lpObj->pInventory[0].IsItem() && (lpObj->pInventory[0].m_Type >= ITEMGET(5, 0) && lpObj->pInventory[0].m_Type < ITEMGET(6, 0)) && lpObj->pInventory[0].m_IsValidItem)
		{
			int damage = lpObj->pInventory[0].m_Magic / 2 + lpObj->pInventory[0].m_Level * 2;	// #formula
			damage -= (WORD)(lpObj->pInventory[0].m_CurrentDurabilityState * damage);	// #formula

			damagemin += damagemin * damage / 100;	// #formula
			damagemax += damagemax * damage / 100;	// #formula
		}

		iIncreaseDamage += gObjGetTotalValueOfEffect(lpObj, EFFECTTYPE_MAGICDAMAGE);
	}
		break;
	}

	damagemin += lpObj->m_PlayerData->SetOpAddSkillAttack;
	damagemax += lpObj->m_PlayerData->SetOpAddSkillAttack;

	int SkillAttr = MagicDamageC.GetSkillAttr(lpMagic->m_Skill);

	if (SkillAttr != -1)
	{
		if ((lpObj->Authority & 0x20) != 0x20 || (lpObj->pInventory[10].m_Type != ITEMGET(13, 42) && lpObj->pInventory[11].m_Type != ITEMGET(13, 42)))
		{
			damagemin += (BYTE)lpObj->m_AddResistance[SkillAttr];
			damagemax += (BYTE)lpObj->m_AddResistance[SkillAttr];
		}
		else
		{
			damagemin += 255;
			damagemax += 255;
		}
	}

	damagemin += lpObj->m_PlayerData->m_JewelOfHarmonyEffect.HJOpAddSkillAttack;
	damagemax += lpObj->m_PlayerData->m_JewelOfHarmonyEffect.HJOpAddSkillAttack;

	if (gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_ARCA_FIRETOWER))
	{
		int nEffectValue = 0;
		gObjGetValueOfBuffIndex(lpObj, BUFFTYPE_ARCA_FIRETOWER, &nEffectValue, 0);

		damagemin += nEffectValue;
		damagemax += nEffectValue;
	}

	if (gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_OBSIDIAN) == TRUE)
	{
		int nEffectValue = 0;
		gObjGetValueOfBuffIndex(lpObj, BUFFTYPE_OBSIDIAN, &nEffectValue, NULL);

		damagemin += nEffectValue;
		damagemax += nEffectValue;
	}

	if (gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_OBSIDIAN_STR) == TRUE)
	{
		int nEffectValue = 0;
		gObjGetValueOfBuffIndex(lpObj, BUFFTYPE_OBSIDIAN_STR, &nEffectValue, NULL);

		damagemin += nEffectValue;
		damagemax += nEffectValue;
	}

	int nMuunItemEffectValue = 0;

	if (g_CMuunSystem.GetMuunItemValueOfOptType(lpObj, MUUN_INC_SKILL_POWER, &nMuunItemEffectValue, 0) == TRUE)
	{
		damagemin += nMuunItemEffectValue;
		damagemax += nMuunItemEffectValue;
	}

	if (lpObj->Type == OBJ_USER && lpTargetObj->Type != OBJ_USER)
	{
		nMuunItemEffectValue = 0;
		g_CMuunSystem.GetMuunItemValueOfOptType(lpObj, MUUN_INC_ATTACK_POWER_NONPVP, &nMuunItemEffectValue, 0);

		damagemin += nMuunItemEffectValue;
		damagemax += nMuunItemEffectValue;

		g_CMuunSystem.GetMuunItemValueOfOptType(lpObj, MUUN_INC_MAX_ATTACK_POWER_NONPVP, &nMuunItemEffectValue, 0);
		
		damagemax += nMuunItemEffectValue;

		nMuunItemEffectValue = 0;
		g_CMuunSystem.GetMuunItemValueOfOptType(lpObj, MUUN_INC_SKILL_POWER_NONPVP, &nMuunItemEffectValue, 0);

		damagemin += nMuunItemEffectValue;
		damagemax += nMuunItemEffectValue;
	}

	int subd = damagemax - damagemin;

	__try
	{
		ad = (damagemin + (rand() % (subd + 1))) - targetDefense;
	}

	__except (subd = 1, 1)
	{

	}

	int nCritical = lpObj->m_CriticalDamage;
	int nExcellent = lpObj->m_ExcelentDamage;

	if (lpObj->Type == OBJ_USER)
	{
		if (lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncCriticalDamageRate > 0.0)
		{
			nCritical += lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncCriticalDamageRate;
		}

		if (lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncExcellentDamageRate > 0.0)
		{
			nExcellent += lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncExcellentDamageRate;
		}

		if (lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncExcellentDamageRate_Darklord > 0.0)
		{
			nExcellent += lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncExcellentDamageRate_Darklord;
		}
	}

	if (lpTargetObj->Type == OBJ_USER)
	{
		nCritical -= lpTargetObj->m_PlayerData->m_Resistance_Critical;
		nExcellent -= lpTargetObj->m_PlayerData->m_Resistance_Excellent;
	}

	int cDamage = FALSE;

	if (nCritical > 0)
	{
		if ((rand() % 100) < nCritical)
		{
			effect = 3;
			cDamage = TRUE;
		}
	}

	if (nExcellent > 0)
	{
		if ((rand() % 100) < nExcellent)
		{
			effect = 2;
			cDamage = TRUE;
		}
	}

	if (cDamage)
	{
		ad = damagemax - targetDefense;
		ad += lpObj->m_PlayerData->SetOpAddCriticalDamage;
		ad += lpObj->m_PlayerData->m_JewelOfHarmonyEffect.HJOpAddCriticalDamage;
		int iOption = 0;
		gObjGetValueOfBuffIndex(lpObj, BUFFTYPE_CRITICAL_DMG_INC, &iOption, 0);
		ad += iOption;
		iOption = 0;
		gObjGetValueOfBuffIndex(lpObj, BUFFTYPE_CRITICAL_DMG_INC_STR, &iOption, 0);
		ad += iOption;
		iOption = 0;
		gObjGetValueOfBuffIndex(lpObj, BUFFTYPE_CRITICAL_DMG_INC_MAS, &iOption, 0);
		ad += iOption;

		if (effect == 2)
		{
			ad += damagemax * 20 / 100;
			ad += lpObj->m_PlayerData->SetOpAddExDamage;
		}
	}

	if ( lpObj->m_SkillInfo.RemedyOfLoveEffect )
	{
		ad += 10;
	}

	iIncreaseDamage += gObjGetTotalValueOfEffect(lpObj, EFFECTTYPE_IMPROVE_DAMAGE);

	if (gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_ARCA_WINDTOWER))
	{
		int iArcaEffect = 0;
		gObjGetValueOfBuffIndex(lpObj, BUFFTYPE_ARCA_WINDTOWER, &iArcaEffect, NULL);

		iIncreaseDamage += iArcaEffect;
	}

	ad += iIncreaseDamage;

	return ad;
}

int  CObjAttack::GetAttackDamageWizard(LPOBJ lpObj, LPOBJ lpTargetObj, int targetDefense, CMagicInf* lpMagic, int& effect)
{
	if (lpObj->Type != OBJ_USER)
	{
		return 0;
	}

	int damagemin;
	int damagemax;
	int ad;

	if (lpMagic->m_Skill == 40)
	{
		if (lpObj->m_PlayerData->SkillHellFire2Count >= 0)
		{
			int SkillHellFire2CountDamageTable[13] =
			{
				0, 20, 50, 99, 160,
				225, 325, 425, 550, 700,
				880, 1090, 1320
			};
			int CountDamage;

			if (lpObj->m_PlayerData->SkillHellFire2Count > 12)
			{
				CountDamage = 0;
			}
			else
			{
				CountDamage = SkillHellFire2CountDamageTable[lpObj->m_PlayerData->SkillHellFire2Count];
			}

			ad = (lpObj->m_PlayerData->Strength + lpObj->AddStrength) / 2 + CountDamage;

			damagemin = ad + lpObj->m_MagicDamageMin;
			damagemax = ad + lpObj->m_MagicDamageMax;

			damagemin += lpObj->m_PlayerData->SetOpAddSkillAttack;
			damagemax += lpObj->m_PlayerData->SetOpAddSkillAttack;

			if (gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_ARCA_FIRETOWER))
			{
				int nEffectValue = 0;
				gObjGetValueOfBuffIndex(lpObj, BUFFTYPE_ARCA_FIRETOWER, &nEffectValue, 0);

				damagemin += nEffectValue;
				damagemax += nEffectValue;
			}

			if (gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_OBSIDIAN) == TRUE)
			{
				int nEffectValue = 0;
				gObjGetValueOfBuffIndex(lpObj, BUFFTYPE_OBSIDIAN, &nEffectValue, NULL);

				damagemin += nEffectValue;
				damagemax += nEffectValue;
			}

			if (gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_OBSIDIAN_STR) == TRUE)
			{
				int nEffectValue = 0;
				gObjGetValueOfBuffIndex(lpObj, BUFFTYPE_OBSIDIAN_STR, &nEffectValue, NULL);

				damagemin += nEffectValue;
				damagemax += nEffectValue;
			}

			int nMuunItemValue = 0;

			if (g_CMuunSystem.GetMuunItemValueOfOptType(lpObj, MUUN_INC_SKILL_POWER, &nMuunItemValue, 0) == 1)
			{
				damagemin += nMuunItemValue;
				damagemax += nMuunItemValue;
			}

			if (lpObj->Type == OBJ_USER && lpTargetObj->Type != OBJ_USER)
			{
				nMuunItemValue = 0;
				g_CMuunSystem.GetMuunItemValueOfOptType(lpObj, MUUN_INC_ATTACK_POWER_NONPVP, &nMuunItemValue, 0);

				damagemin += nMuunItemValue;
				damagemax += nMuunItemValue;

				nMuunItemValue = 0;
				g_CMuunSystem.GetMuunItemValueOfOptType(lpObj, MUUN_INC_MAX_ATTACK_POWER_NONPVP, &nMuunItemValue, 0);
				
				damagemax += nMuunItemValue;

				nMuunItemValue = 0;
				g_CMuunSystem.GetMuunItemValueOfOptType(lpObj, MUUN_INC_SKILL_POWER_NONPVP, &nMuunItemValue, 0);

				damagemin += nMuunItemValue;
				damagemax += nMuunItemValue;
			}

			int SkillAttr = MagicDamageC.GetSkillAttr(lpMagic->m_Skill);

			if (SkillAttr != -1)
			{
				if ((lpObj->Authority & 0x20) != 0x20 || (lpObj->pInventory[10].m_Type != ITEMGET(13, 42) && lpObj->pInventory[11].m_Type != ITEMGET(13, 42)))
				{
					damagemin += (BYTE)lpObj->m_AddResistance[SkillAttr];
					damagemax += (BYTE)lpObj->m_AddResistance[SkillAttr];
				}
				else
				{
					damagemin += 255;
					damagemax += 255;
				}
			}

			damagemin += lpObj->m_PlayerData->m_JewelOfHarmonyEffect.HJOpAddSkillAttack;
			damagemax += lpObj->m_PlayerData->m_JewelOfHarmonyEffect.HJOpAddSkillAttack;
		}
	}

	else if (lpMagic->m_Skill == 392)
	{
		if (lpObj->m_PlayerData->SkillStrengthenHellFire2Count >= 0)
		{
			int SkillHellFire2CountDamageTable[13] =
			{
				0, 20, 50, 99, 160,
				225, 325, 425, 550, 700,
				880, 1090, 1320
			};

			int CountDamage;

			if (lpObj->m_PlayerData->SkillStrengthenHellFire2Count > 12)
			{
				CountDamage = 0;
			}
			else
			{
				CountDamage = SkillHellFire2CountDamageTable[lpObj->m_PlayerData->SkillStrengthenHellFire2Count];
			}

			ad = (lpObj->m_PlayerData->Strength + lpObj->AddStrength) / 2 + CountDamage;

			damagemin = ad + lpObj->m_MagicDamageMin;
			damagemax = ad + lpObj->m_MagicDamageMax;

			damagemin += lpObj->m_PlayerData->SetOpAddSkillAttack;
			damagemax += lpObj->m_PlayerData->SetOpAddSkillAttack;

			int SkillAttr = MagicDamageC.GetSkillAttr(lpMagic->m_Skill);

			if (SkillAttr != -1)
			{
				if ((lpObj->Authority & 0x20) != 0x20 || (lpObj->pInventory[10].m_Type != ITEMGET(13, 42) && lpObj->pInventory[11].m_Type != ITEMGET(13, 42)))
				{
					damagemin += (BYTE)lpObj->m_AddResistance[SkillAttr];
					damagemax += (BYTE)lpObj->m_AddResistance[SkillAttr];
				}
				else
				{
					damagemin += 255;
					damagemax += 255;
				}
			}

			if (gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_ARCA_FIRETOWER))
			{
				int nEffectValue = 0;
				gObjGetValueOfBuffIndex(lpObj, BUFFTYPE_ARCA_FIRETOWER, &nEffectValue, 0);

				damagemin += nEffectValue;
				damagemax += nEffectValue;
			}

			if (gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_OBSIDIAN) == TRUE)
			{
				int nEffectValue = 0;
				gObjGetValueOfBuffIndex(lpObj, BUFFTYPE_OBSIDIAN, &nEffectValue, NULL);

				damagemin += nEffectValue;
				damagemax += nEffectValue;
			}

			if (gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_OBSIDIAN_STR) == TRUE)
			{
				int nEffectValue = 0;
				gObjGetValueOfBuffIndex(lpObj, BUFFTYPE_OBSIDIAN_STR, &nEffectValue, NULL);

				damagemin += nEffectValue;
				damagemax += nEffectValue;
			}

			int nMuunItemValue = 0;

			if (g_CMuunSystem.GetMuunItemValueOfOptType(lpObj, MUUN_INC_SKILL_POWER, &nMuunItemValue, 0) == 1)
			{
				damagemin += nMuunItemValue;
				damagemax += nMuunItemValue;
			}

			if (lpObj->Type == OBJ_USER && lpTargetObj->Type != OBJ_USER)
			{
				nMuunItemValue = 0;
				g_CMuunSystem.GetMuunItemValueOfOptType(lpObj, MUUN_INC_ATTACK_POWER_NONPVP, &nMuunItemValue, 0);

				damagemin += nMuunItemValue;
				damagemax += nMuunItemValue;

				nMuunItemValue = 0;
				g_CMuunSystem.GetMuunItemValueOfOptType(lpObj, MUUN_INC_MAX_ATTACK_POWER_NONPVP, &nMuunItemValue, 0);
				
				damagemax += nMuunItemValue;

				nMuunItemValue = 0;
				g_CMuunSystem.GetMuunItemValueOfOptType(lpObj, MUUN_INC_SKILL_POWER_NONPVP, &nMuunItemValue, 0);

				damagemin += nMuunItemValue;
				damagemax += nMuunItemValue;
			}

			damagemin += lpObj->m_PlayerData->m_JewelOfHarmonyEffect.HJOpAddSkillAttack;
			damagemax += lpObj->m_PlayerData->m_JewelOfHarmonyEffect.HJOpAddSkillAttack;
		}
	}

	else
	{
		ad = lpMagic->GetDamage();

		damagemin = lpMagic->m_DamageMin + lpObj->m_MagicDamageMin + g_SkillSpec.CalcStatBonus(lpObj, lpMagic->m_Skill);
		damagemax = lpMagic->m_DamageMax + lpObj->m_MagicDamageMax + g_SkillSpec.CalcStatBonus(lpObj, lpMagic->m_Skill);

		if (g_MasterLevelSkillTreeSystem.CheckMasterLevelSkill(lpMagic->m_Skill) == true)
		{
			float fDamage = g_MasterLevelSkillTreeSystem.GetSkillAttackDamage(lpObj, lpMagic->m_Skill);
			damagemin += fDamage;
			damagemax += fDamage;
		}

		if (gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_ARCA_FIRETOWER))
		{
			int nEffectValue = 0;
			gObjGetValueOfBuffIndex(lpObj, BUFFTYPE_ARCA_FIRETOWER, &nEffectValue, 0);

			damagemin += nEffectValue;
			damagemax += nEffectValue;
		}

		if (gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_OBSIDIAN) == TRUE)
		{
			int nEffectValue = 0;
			gObjGetValueOfBuffIndex(lpObj, BUFFTYPE_OBSIDIAN, &nEffectValue, NULL);

			damagemin += nEffectValue;
			damagemax += nEffectValue;
		}

		if (gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_OBSIDIAN_STR) == TRUE)
		{
			int nEffectValue = 0;
			gObjGetValueOfBuffIndex(lpObj, BUFFTYPE_OBSIDIAN_STR, &nEffectValue, NULL);

			damagemin += nEffectValue;
			damagemax += nEffectValue;
		}

		int nMuunOptValue = 0;

		if (g_CMuunSystem.GetMuunItemValueOfOptType(lpObj, MUUN_INC_SKILL_POWER, &nMuunOptValue, 0) == 1)
		{
			damagemin += nMuunOptValue;
			damagemax += nMuunOptValue;
		}

		if (lpObj->Type == OBJ_USER && lpTargetObj->Type != OBJ_USER)
		{
			nMuunOptValue = 0;
			g_CMuunSystem.GetMuunItemValueOfOptType(lpObj, MUUN_INC_ATTACK_POWER_NONPVP, &nMuunOptValue, 0);

			damagemin += nMuunOptValue;
			damagemax += nMuunOptValue;

			nMuunOptValue = 0;
			g_CMuunSystem.GetMuunItemValueOfOptType(lpObj, MUUN_INC_MAX_ATTACK_POWER_NONPVP, &nMuunOptValue, 0);

			damagemax += nMuunOptValue;

			nMuunOptValue = 0;
			g_CMuunSystem.GetMuunItemValueOfOptType(lpObj, MUUN_INC_SKILL_POWER_NONPVP, &nMuunOptValue, 0);

			damagemin += nMuunOptValue;
			damagemax += nMuunOptValue;
		}

		damagemin += lpObj->m_PlayerData->SetOpAddSkillAttack;
		damagemax += lpObj->m_PlayerData->SetOpAddSkillAttack;

		int SkillAttr = MagicDamageC.GetSkillAttr(lpMagic->m_Skill);

		if (SkillAttr != -1)
		{
			if ((lpObj->Authority & 0x20) != 0x20 || (lpObj->pInventory[10].m_Type != ITEMGET(13, 42) && lpObj->pInventory[11].m_Type != ITEMGET(13, 42)))
			{
				damagemin += (BYTE)lpObj->m_AddResistance[SkillAttr];
				damagemax += (BYTE)lpObj->m_AddResistance[SkillAttr];
			}
			else
			{
				damagemin += 255;
				damagemax += 255;
			}
		}

		damagemin += lpObj->m_PlayerData->m_JewelOfHarmonyEffect.HJOpAddSkillAttack;
		damagemax += lpObj->m_PlayerData->m_JewelOfHarmonyEffect.HJOpAddSkillAttack;
	}

	CItem * Right = &lpObj->pInventory[0];

	if (Right->IsItem())
	{
		if ((Right->m_Type >= ITEMGET(5, 0) && Right->m_Type < ITEMGET(6, 0)) ||
			Right->m_Type == ITEMGET(0, 31) ||
			Right->m_Type == ITEMGET(0, 21) ||
			Right->m_Type == ITEMGET(0, 23) ||
			Right->m_Type == ITEMGET(0, 25) ||
			Right->m_Type == ITEMGET(0, 28) ||
			Right->m_Type == ITEMGET(0, 30))
		{
			if (Right->m_IsValidItem)
			{
				int damage = Right->m_Magic / 2 + Right->m_Level * 2;	// #formula
				damage -= (WORD)(Right->m_CurrentDurabilityState * damage);	// #formula

				damagemin += damagemin * damage / 100;	// #formula
				damagemax += damagemax * damage / 100;	// #formula
			}
		}
	}

	int subd = damagemax - damagemin;

	__try
	{
		ad = (damagemin + (rand() % (subd + 1))) - targetDefense;
	}

	__except (subd = 1, 1)
	{

	}

	int nCritical = lpObj->m_CriticalDamage;
	int nExcellent = lpObj->m_ExcelentDamage;

	if (lpObj->Type == OBJ_USER)
	{
		if (gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_MAGIC_POWER_INC_MAS) == true)
		{
			nCritical += lpObj->m_PlayerData->m_MPSkillOpt.iMpsCriticalRateInc;
		}

		if (lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncCriticalDamageRate > 0.0)
		{
			nCritical += lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncCriticalDamageRate;
		}

		if (lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncExcellentDamageRate > 0.0)
		{
			nExcellent += lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncExcellentDamageRate;
		}
	}

	if (lpTargetObj->Type == OBJ_USER)
	{
		nCritical -= lpTargetObj->m_PlayerData->m_Resistance_Critical;
		nExcellent -= lpTargetObj->m_PlayerData->m_Resistance_Excellent;
	}

	int cDamage = FALSE;

	if (nCritical > 0)
	{
		if ((rand() % 100) < nCritical)
		{
			effect = 3;
			cDamage = TRUE;
		}
	}

	if (nExcellent > 0)
	{
		if ((rand() % 100) < nExcellent)
		{
			effect = 2;
			cDamage = TRUE;
		}
	}

	if (cDamage)
	{
		ad = damagemax - targetDefense;
		ad += lpObj->m_PlayerData->SetOpAddCriticalDamage;
		ad += lpObj->m_PlayerData->m_JewelOfHarmonyEffect.HJOpAddCriticalDamage;
		int iOption = 0;
		gObjGetValueOfBuffIndex(lpObj, BUFFTYPE_CRITICAL_DMG_INC, &iOption, 0);
		ad += iOption;
		iOption = 0;
		gObjGetValueOfBuffIndex(lpObj, BUFFTYPE_CRITICAL_DMG_INC_STR, &iOption, 0);
		ad += iOption;
		iOption = 0;
		gObjGetValueOfBuffIndex(lpObj, BUFFTYPE_CRITICAL_DMG_INC_MAS, &iOption, 0);
		ad += iOption;

		if (effect == 2)
		{
			ad += damagemax * 20 / 100;
			ad += lpObj->m_PlayerData->SetOpAddExDamage;
		}
	}

	int BuffAtk = gObjGetTotalValueOfEffect(lpObj, EFFECTTYPE_IMPROVE_DAMAGE);
	int BuffForce = gObjGetTotalValueOfEffect(lpObj, EFFECTTYPE_MELEEDAMAGE);
	
	ad += BuffAtk;
	ad += BuffForce;

	if (gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_ARCA_WINDTOWER))
	{
		int iArcaEffect = 0;
		gObjGetValueOfBuffIndex(lpObj, BUFFTYPE_ARCA_WINDTOWER, &iArcaEffect, NULL);

		ad += iArcaEffect;
	}

	if ( lpObj->m_SkillInfo.RemedyOfLoveEffect )
	{
		ad += 10;
	}

	return ad;
}

BOOL gObjDenorantSprite(LPOBJ lpObj)
{
	if ( lpObj->Type != OBJ_USER )
	{
		return FALSE;
	}

	if ( lpObj->m_btInvenPetPos == 0 || lpObj->m_wInvenPet == (WORD)-1 )
	{
		return FALSE;
	}

	CItem * Denorant = & lpObj->pInventory[lpObj->m_btInvenPetPos];

	if ( Denorant->m_Type == ITEMGET(13,3) )
	{
		if ( Denorant->m_Durability > 0.0f )
		{
			return TRUE;
		}
	}

	return FALSE;
}



BOOL gObjDarkHorse(LPOBJ lpObj)
{
	if ( lpObj->Type != OBJ_USER )
	{
		return FALSE;
	}

	if ( lpObj->m_btInvenPetPos == 0 || lpObj->m_wInvenPet == (WORD)-1 )
	{
		return FALSE;
	}

	CItem * Denorant = & lpObj->pInventory[lpObj->m_btInvenPetPos];

	if ( Denorant->m_Type == ITEMGET(13,4) )
	{
		if ( Denorant->m_Durability > 0.0f )
		{
			return TRUE;
		}
	}

	return FALSE;
}


BOOL gObjFenrir(LPOBJ lpObj)
{
	if ( lpObj->Type != OBJ_USER )
	{
		return FALSE;
	}

	if ( lpObj->m_btInvenPetPos == 0 || lpObj->m_wInvenPet == (WORD)-1 )
	{
		return FALSE;
	}

	CItem * lpFenrir = & lpObj->pInventory[lpObj->m_btInvenPetPos];

	if ( lpFenrir->m_Type == ITEMGET(13,37) )	// Fenrir
	{
		if ( lpFenrir->m_Durability > 0.0f )
		{
			return TRUE;
		}
	}

	return FALSE;
}

int CObjAttack::GetShieldDamage(LPOBJ lpObj, LPOBJ lpTargetObj, int iAttackDamage)
{
	int iShieldDamage = 0;

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
		int iRand = rand()%100;
		int iIgnoreSDRate = lpObj->m_PlayerData->m_JewelOfHarmonyEffect.HJOpAddIgnoreSDRate - lpTargetObj->m_PlayerData->m_Resistance_SD;

		if (iRand < iIgnoreSDRate)
		{
			iDamageDevideToSDRate = 0;
		}
	}

	if ( (lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER) && ( lpObj->m_PlayerData->m_JewelOfHarmonyEffect.HJOpDecreaseSDRate ||
			lpTargetObj->m_PlayerData->m_JewelOfHarmonyEffect.HJOpAddSDRate ||
			lpObj->m_PlayerData->m_JewelOfHarmonyEffect.HJOpAddIgnoreSDRate ) )
	{
		g_Log.Add("[JewelOfHarmony][PvP System] Attacker:[%s][%s]-SD Decrease[%d] SD Ignore[%d] Defender:[%s][%s] SD Increase Option[%d] - SD Rate[%d]",
			lpObj->AccountID, lpObj->Name,
			lpObj->m_PlayerData->m_JewelOfHarmonyEffect.HJOpDecreaseSDRate,
			lpObj->m_PlayerData->m_JewelOfHarmonyEffect.HJOpAddIgnoreSDRate,
			lpTargetObj->AccountID, lpTargetObj->Name,
			lpTargetObj->m_PlayerData->m_JewelOfHarmonyEffect.HJOpAddSDRate,
			iDamageDevideToSDRate);
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

int CObjAttack::GetElementalDamage(LPOBJ lpObj, LPOBJ lpTargetObj, char* DamageType1, char* DamageType2, int iAttackDamage, int iTargetDefense) // 1.1.0.4 decompilation
{
	int Strength = 0;
	int Dexterity = 0;
	int Vitality = 0;
	int Energy = 0;
	int PentagramMinDamage = 0;
	int PentagramMaxDamage = 0;
	int PentagramDefense = 0;
	int BaseDamage = 0;
	int BaseDefense = 0;
	int MinDamage = 0;
	int MaxDamage = 0;
	int AddDamage = 0;
	int AddDefense = 0;
	int TotalDamage = 0;
	int TotalDefense = 0;
	int TotalDamageWithRelationShip = 0;
	int TotalDefenseWithRelationShip = 0;
	int AttackRelationShip = 0;
	int DefenseRelationShip = 0;
	int AttackSuccessRate = 0;
	int DefenseSuccessRate = 0;
	int AttackerAttribute = lpObj->m_iPentagramMainAttribute;
	int DefenserAttribute = lpTargetObj->m_iPentagramMainAttribute;
	bool bExcellentDamageOption = false;
	bool bCriticalDamageOption = false;

	if(lpObj->Type == OBJ_USER)
	{
		Strength = lpObj->m_PlayerData->Strength + lpObj->m_PlayerData->m_PentagramOptions.m_iOnyx_2RankAddStrength;
		Dexterity = lpObj->m_PlayerData->Dexterity + lpObj->m_PlayerData->m_PentagramOptions.m_iOnyx_2RankAddDexterity;
		Vitality = lpObj->m_PlayerData->Vitality + lpObj->m_PlayerData->m_PentagramOptions.m_iOnyx_2RankAddVitality;
		Energy = lpObj->m_PlayerData->Energy + lpObj->m_PlayerData->m_PentagramOptions.m_iOnyx_2RankAddEnergy;
	}

	switch ( AttackerAttribute )
	{
		case EL_FIRE:
			*DamageType1 = 1;
			break;
		case EL_WATER:
			*DamageType1 = 2;
			break;
		case EL_EARTH:
			*DamageType1 = 3;
			break;
		case EL_WIND:
			*DamageType1 = 4;
			break;
		case EL_DARKNESS:
			*DamageType1 = 5;
			break;
		default:
			*DamageType1 = 0;
			break;
	}

	if ( lpObj->Type == 1 )
	{
		if ( g_PentagramSystem.IsPentagramItem(&lpObj->pInventory[236]) == true )
		{
			PentagramMinDamage = lpObj->pInventory[236].m_DamageMin;
			PentagramMaxDamage = lpObj->pInventory[236].m_DamageMax;
		}

		int iExcelDamageRate = 0;

		if (lpObj->m_PlayerData->m_PentagramOptions.m_iOnyx_4RankOptionNum == 1)
		{
			if (lpTargetObj->Type == OBJ_USER)
			{
				iExcelDamageRate = lpObj->m_PlayerData->m_PentagramOptions.m_iOnyx_4RankAddExllentDamageRate;
			}
		}

		else if (lpObj->m_PlayerData->m_PentagramOptions.m_iOnyx_4RankOptionNum == 2)
		{
			if (lpTargetObj->Type == OBJ_MONSTER)
			{
				iExcelDamageRate = lpObj->m_PlayerData->m_PentagramOptions.m_iOnyx_4RankAddExllentDamageRate;
			}
		}

		if (rand() % 10000 < iExcelDamageRate)
		{
			bExcellentDamageOption = TRUE;
		}

		if (bExcellentDamageOption == FALSE)
		{
			int iCriDamageRate = 0;

			if (lpObj->m_PlayerData->m_PentagramOptions.m_iRuby_5RankOptionNum == 1)
			{
				if (lpTargetObj->Type == OBJ_USER)
				{
					iCriDamageRate = lpObj->m_PlayerData->m_PentagramOptions.m_iRuby_5RankCriticalDamageRate;
				}
			}

			else if (lpObj->m_PlayerData->m_PentagramOptions.m_iRuby_5RankOptionNum == 2)
			{
				if (lpTargetObj->Type == OBJ_MONSTER)
				{
					iCriDamageRate = lpObj->m_PlayerData->m_PentagramOptions.m_iRuby_5RankCriticalDamageRate;
				}
			}

			if (lpObj->m_PlayerData->m_PentagramOptions.m_isAddCriPentaDamage == true)
			{
				iCriDamageRate += 1000;
			}

			if (rand() % 10000 < iCriDamageRate)
			{
				bCriticalDamageOption = TRUE;
			}
		}

		if (lpObj->m_PlayerData->m_PentagramOptions.m_isAddPentaAttack == true)
		{
			PentagramMinDamage += PentagramMinDamage * 0.1;
			PentagramMaxDamage += PentagramMaxDamage * 0.1;
		}
	}

	else if ( lpObj->Type == 2 )
	{
		MinDamage = lpObj->m_iPentagramAttackMin;
		MaxDamage = lpObj->m_iPentagramAttackMax;
	}

	if ( lpObj->Type == OBJ_USER )
	{
		switch ( lpObj->Class )
		{
			case CLASS_WIZARD:
				this->m_Lua.Generic_Call("WizardElementalDamageCalc", "iiiiii>ii", Strength, Dexterity, Vitality, Energy, PentagramMinDamage, PentagramMaxDamage, &MinDamage, &MaxDamage);
				break;
			case CLASS_KNIGHT:
				this->m_Lua.Generic_Call("KnightElementalDamageCalc", "iiiiii>ii", Strength, Dexterity, Vitality, Energy, PentagramMinDamage, PentagramMaxDamage, &MinDamage, &MaxDamage);
				break;
			case CLASS_ELF:
				this->m_Lua.Generic_Call("ElfElementalDamageCalc", "iiiiii>ii", Strength, Dexterity, Vitality, Energy, PentagramMinDamage, PentagramMaxDamage, &MinDamage, &MaxDamage);
				break;
			case CLASS_MAGUMSA:
				this->m_Lua.Generic_Call("GladiatorElementalDamageCalc", "iiiiii>ii", Strength, Dexterity, Vitality, Energy, PentagramMinDamage, PentagramMaxDamage, &MinDamage, &MaxDamage);
				break;
			case CLASS_DARKLORD:
				this->m_Lua.Generic_Call("LordElementalDamageCalc", "iiiiii>ii", Strength, Dexterity, Vitality, Energy, PentagramMinDamage, PentagramMaxDamage, &MinDamage, &MaxDamage);
				break;
			case CLASS_SUMMONER:
				this->m_Lua.Generic_Call("SummonerElementalDamageCalc", "iiiiii>ii", Strength, Dexterity, Vitality, Energy, PentagramMinDamage, PentagramMaxDamage, &MinDamage, &MaxDamage);
				break;
			case CLASS_RAGEFIGHTER:
				this->m_Lua.Generic_Call("RageFighterElementalDamageCalc", "iiiiii>ii", Strength, Dexterity, Vitality, Energy, PentagramMinDamage, PentagramMaxDamage, &MinDamage, &MaxDamage);
				break;
			case CLASS_GROWLANCER:
				this->m_Lua.Generic_Call("GrowLancerElementalDamageCalc", "iiiiii>ii", Strength, Dexterity, Vitality, Energy, PentagramMinDamage, PentagramMaxDamage, &MinDamage, &MaxDamage);
				break;
		}
	}

	if (bCriticalDamageOption == TRUE)
	{
		MinDamage = MaxDamage;
	}

	int RandDamage = 0;

	if (lpObj->m_iPentagramDamageOrigin == 1)
	{
		if (MinDamage <= MaxDamage)
			RandDamage = MaxDamage - MinDamage;
		else
			RandDamage = MinDamage - MaxDamage;

		BaseDamage = MinDamage + (rand() % (RandDamage + 1));
	}
	else
	{
		if (lpObj->m_iPentagramDamageMax == 1)
		{
			BaseDamage = MaxDamage;
		}
		else if (lpObj->m_iPentagramDamageMin == 1)
		{
			BaseDamage = MinDamage;
		}
	}

	if ( lpObj->Type == OBJ_USER )
	{
		AddDamage = lpObj->m_PlayerData->m_PentagramOptions.m_iRuby_1RankAddDamage;

		if (lpObj->m_PlayerData->m_PentagramOptions.m_iRuby_3RankOptionNum == 1 && lpTargetObj->Type == OBJ_USER)
		{
			AddDamage += lpObj->m_PlayerData->m_PentagramOptions.m_iRuby_3RankAddDamage;
		}

		else if (lpObj->m_PlayerData->m_PentagramOptions.m_iRuby_3RankOptionNum == 2 && lpTargetObj->Type == OBJ_MONSTER)
		{
			AddDamage += lpObj->m_PlayerData->m_PentagramOptions.m_iRuby_3RankAddDamage;
		}

		if (lpObj->m_PlayerData->m_PentagramOptions.m_iRuby_4RankOptionNum == 1)
		{
			if (lpTargetObj->Type == OBJ_MONSTER)
			{
				AddDamage += lpObj->m_PlayerData->m_PentagramOptions.m_iRuby_4RankAddDamage;
			}

			else if (lpTargetObj->Class == CLASS_WIZARD || lpTargetObj->Class == CLASS_ELF || lpTargetObj->Class == CLASS_MAGUMSA || lpTargetObj->Class == CLASS_SUMMONER)
			{
				AddDamage += lpObj->m_PlayerData->m_PentagramOptions.m_iRuby_4RankAddDamage;
			}
		}

		else if (lpObj->m_PlayerData->m_PentagramOptions.m_iRuby_4RankOptionNum == 2)
		{
			if (lpTargetObj->Type == OBJ_MONSTER)
			{
				AddDamage += lpObj->m_PlayerData->m_PentagramOptions.m_iRuby_4RankAddDamage;
			}

			else if (lpTargetObj->Class == CLASS_KNIGHT || lpTargetObj->Class == CLASS_DARKLORD || lpTargetObj->Class == CLASS_RAGEFIGHTER)
			{
				AddDamage += lpObj->m_PlayerData->m_PentagramOptions.m_iRuby_4RankAddDamage;
			}
		}

		if (lpObj->m_PlayerData->m_PentagramOptions.m_iEme_3RankOptionNum == 1 && lpTargetObj->Type == OBJ_USER)
		{
			AddDamage += lpObj->m_PlayerData->m_PentagramOptions.m_iEme_3RankAddDamage;
		}

		else if (lpObj->m_PlayerData->m_PentagramOptions.m_iEme_3RankOptionNum == 2 && lpTargetObj->Type == OBJ_MONSTER)
		{
			AddDamage += lpObj->m_PlayerData->m_PentagramOptions.m_iEme_3RankAddDamage;
		}

		if (lpObj->m_PlayerData->m_PentagramOptions.m_iEme_4RankOptionNum == 1)
		{
			if (lpTargetObj->Type == OBJ_MONSTER)
			{
				AddDamage += lpObj->m_PlayerData->m_PentagramOptions.m_iEme_4RankAddDamage;
			}

			else if (lpTargetObj->Class == CLASS_WIZARD || lpTargetObj->Class == CLASS_ELF || lpTargetObj->Class == CLASS_MAGUMSA || lpTargetObj->Class == CLASS_SUMMONER)
			{
				AddDamage += lpObj->m_PlayerData->m_PentagramOptions.m_iEme_4RankAddDamage;
			}
		}

		else if (lpObj->m_PlayerData->m_PentagramOptions.m_iEme_4RankOptionNum == 2)
		{
			if (lpTargetObj->Type == OBJ_MONSTER)
			{
				AddDamage += lpObj->m_PlayerData->m_PentagramOptions.m_iEme_4RankAddDamage;
			}

			else if (lpTargetObj->Class == CLASS_KNIGHT || lpTargetObj->Class == CLASS_DARKLORD || lpTargetObj->Class == CLASS_RAGEFIGHTER)
			{
				AddDamage += lpObj->m_PlayerData->m_PentagramOptions.m_iEme_4RankAddDamage;
			}
		}
	}

	if (lpObj->Type == OBJ_USER && lpObj->m_PlayerData->m_PentagramOptions.m_isChangePentaAttack == true)
	{
		AddDamage += iAttackDamage * 0.01;
	}

	TotalDamage = BaseDamage + AddDamage;

	if ( lpObj->Type == OBJ_USER )
	{
		if ( lpObj->m_PlayerData->m_PentagramOptions.m_iEme_5RankOptionNum == 1 && lpTargetObj->Type == OBJ_USER )
		{
			TotalDamage += TotalDamage * lpObj->m_PlayerData->m_PentagramOptions.m_iEme_5RankAddDamageRate / 100;
		}

		else if ( lpObj->m_PlayerData->m_PentagramOptions.m_iEme_5RankOptionNum == 2 && lpTargetObj->Type == OBJ_MONSTER )
		{
			TotalDamage += TotalDamage * lpObj->m_PlayerData->m_PentagramOptions.m_iEme_5RankAddDamageRate / 100;
		}
	}

	if (TotalDamage > 0 && lpObj->Type == OBJ_USER)
	{
		if (gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_ARCA_FIRETOWER) == TRUE && AttackerAttribute == EL_FIRE)
		{
			TotalDamage += 5 * TotalDamage / 100;
		}

		else if (gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_ARCA_WATERTOWER) == TRUE && AttackerAttribute == EL_WATER)
		{
			TotalDamage += 5 * TotalDamage / 100;
		}

		else if (gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_ARCA_EARTHTOWER) == TRUE && AttackerAttribute == EL_EARTH)
		{
			TotalDamage += 5 * TotalDamage / 100;
		}

		else if (gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_ARCA_WINDTOWER) == TRUE && AttackerAttribute == EL_WIND)
		{
			TotalDamage += 5 * TotalDamage / 100;
		}

		else if (gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_ARCA_DARKNESSTOWER) == TRUE && AttackerAttribute == EL_DARKNESS)
		{
			TotalDamage += 5 * TotalDamage / 100;
		}
	}

	if ( lpTargetObj->Type == OBJ_USER )
	{
		if ( g_PentagramSystem.IsPentagramItem(&lpTargetObj->pInventory[236]) == true )
		{
			PentagramDefense = lpTargetObj->pInventory[236].m_Defense;
		}

		if (lpTargetObj->m_PlayerData->m_PentagramOptions.m_isAddPentaDefense == true)
		{
			PentagramDefense += PentagramDefense * 0.1;
		}
	}

	else if ( lpTargetObj->Type == OBJ_MONSTER )
	{
		PentagramDefense = lpTargetObj->m_iPentagramDefense;
	}

	if ( lpTargetObj->Type == OBJ_USER )
	{
		this->m_Lua.Generic_Call("ElementalDefenseCalc", "ii>i", lpTargetObj->Class, lpTargetObj->m_PlayerData->Dexterity, &BaseDefense);
		BaseDefense += PentagramDefense;
	}

	else
	{
		BaseDefense = PentagramDefense;
	}

	if ( lpTargetObj->Type == OBJ_USER && g_PentagramSystem.IsPentagramItem(&lpTargetObj->pInventory[236]) == true)
	{
		AddDefense = lpTargetObj->m_PlayerData->m_PentagramOptions.m_iSapph_1RankAddDefense;

		if (lpTargetObj->m_PlayerData->m_PentagramOptions.m_iSapph_3RankOptionNum == 1 && lpObj->Type == OBJ_USER)
		{
			AddDefense += lpTargetObj->m_PlayerData->m_PentagramOptions.m_iSapph_3RankAddDefense;
		}

		else if (lpTargetObj->m_PlayerData->m_PentagramOptions.m_iSapph_3RankOptionNum == 2 && lpObj->Type == OBJ_MONSTER)
		{
			AddDefense += lpTargetObj->m_PlayerData->m_PentagramOptions.m_iSapph_3RankAddDefense;
		}

		if (lpTargetObj->m_PlayerData->m_PentagramOptions.m_iSapph_4RankOptionNum == 1)
		{
			if (lpObj->Type == OBJ_MONSTER)
			{
				AddDefense += lpTargetObj->m_PlayerData->m_PentagramOptions.m_iSapph_4RankAddDefense;
			}

			else if (lpObj->Class == CLASS_WIZARD || lpObj->Class == CLASS_ELF || lpObj->Class == CLASS_MAGUMSA || lpObj->Class == CLASS_SUMMONER)
			{
				AddDefense += lpTargetObj->m_PlayerData->m_PentagramOptions.m_iSapph_4RankAddDefense;
			}
		}

		else if (lpTargetObj->m_PlayerData->m_PentagramOptions.m_iSapph_4RankOptionNum == 2)
		{
			if (lpObj->Type == OBJ_MONSTER)
			{
				AddDefense += lpTargetObj->m_PlayerData->m_PentagramOptions.m_iSapph_4RankAddDefense;
			}

			else if (lpObj->Class == CLASS_KNIGHT || lpObj->Class == CLASS_DARKLORD || lpObj->Class == CLASS_RAGEFIGHTER)
			{
				AddDefense += lpTargetObj->m_PlayerData->m_PentagramOptions.m_iSapph_4RankAddDefense;
			}
		}

		if (lpTargetObj->m_PlayerData->m_PentagramOptions.m_iTopa_3RankOptionNum == 1 && lpObj->Type == OBJ_USER)
		{
			AddDefense += lpTargetObj->m_PlayerData->m_PentagramOptions.m_iTopa_3RankAddDefense;
		}

		else if (lpTargetObj->m_PlayerData->m_PentagramOptions.m_iTopa_3RankOptionNum == 2 && lpObj->Type == OBJ_MONSTER)
		{
			AddDefense += lpTargetObj->m_PlayerData->m_PentagramOptions.m_iTopa_3RankAddDefense;
		}

		if (lpTargetObj->m_PlayerData->m_PentagramOptions.m_iTopa_4RankOptionNum == 1)
		{
			if (lpObj->Type == OBJ_MONSTER)
			{
				AddDefense += lpTargetObj->m_PlayerData->m_PentagramOptions.m_iTopa_4RankAddDefense;
			}

			else if (lpObj->Class == CLASS_WIZARD || lpObj->Class == CLASS_ELF || lpTargetObj->Class == CLASS_MAGUMSA || lpObj->Class == CLASS_SUMMONER)
			{
				AddDefense += lpTargetObj->m_PlayerData->m_PentagramOptions.m_iTopa_4RankAddDefense;
			}
		}

		else if (lpTargetObj->m_PlayerData->m_PentagramOptions.m_iTopa_4RankOptionNum == 2)
		{
			if (lpObj->Type == OBJ_MONSTER)
			{
				AddDefense += lpTargetObj->m_PlayerData->m_PentagramOptions.m_iTopa_4RankAddDefense;
			}

			else if (lpObj->Class == CLASS_KNIGHT || lpObj->Class == CLASS_DARKLORD || lpObj->Class == CLASS_RAGEFIGHTER)
			{
				AddDefense += lpTargetObj->m_PlayerData->m_PentagramOptions.m_iTopa_4RankAddDefense;
			}
		}

		if (lpTargetObj->m_PlayerData->m_PentagramOptions.m_isChangePentaDefense == true)
		{
			AddDefense += (2 * iTargetDefense) * 0.05;
		}
	}

	TotalDefense = BaseDefense + AddDefense;

	if (TotalDefense > 0 && lpTargetObj->Type == OBJ_USER)
	{
		if (gObjCheckUsedBuffEffect(lpTargetObj, BUFFTYPE_ARCA_FIRETOWER) == TRUE && DefenserAttribute == EL_FIRE)
		{
			TotalDefense += 5 * TotalDefense / 100;
		}

		else if (gObjCheckUsedBuffEffect(lpTargetObj, BUFFTYPE_ARCA_WATERTOWER) == TRUE && DefenserAttribute == EL_WATER)
		{
			TotalDefense += 5 * TotalDefense / 100;
		}

		else if (gObjCheckUsedBuffEffect(lpTargetObj, BUFFTYPE_ARCA_EARTHTOWER) == TRUE && DefenserAttribute == EL_EARTH)
		{
			TotalDefense += 5 * TotalDefense / 100;
		}

		else if (gObjCheckUsedBuffEffect(lpTargetObj, BUFFTYPE_ARCA_WINDTOWER) == TRUE && DefenserAttribute == EL_WIND)
		{
			TotalDefense += 5 * TotalDefense / 100;
		}

		else if (gObjCheckUsedBuffEffect(lpTargetObj, BUFFTYPE_ARCA_DARKNESSTOWER) == TRUE && DefenserAttribute == EL_DARKNESS)
		{
			TotalDefense += 5 * TotalDefense / 100;
		}
	}

	if ( lpTargetObj->Type == OBJ_USER )
	{
		if ( lpTargetObj->m_PlayerData->m_PentagramOptions.m_iTopa_5RankOptionNum == 1 && lpObj->Type == OBJ_USER )
		{
			TotalDamage -= lpTargetObj->m_PlayerData->m_PentagramOptions.m_iTopa_5RankDamageAbsorbRate * TotalDamage / 100;
		}

		else if (lpTargetObj->m_PlayerData->m_PentagramOptions.m_iTopa_5RankOptionNum == 2 && lpObj->Type == OBJ_MONSTER)
		{
			TotalDamage -= lpTargetObj->m_PlayerData->m_PentagramOptions.m_iTopa_5RankDamageAbsorbRate * TotalDamage / 100;
		}
	}

	switch ( AttackerAttribute )
	{
		case EL_FIRE:
			switch ( DefenserAttribute )
			{
				case EL_FIRE:
					if ( lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER )
					{
						AttackRelationShip = 130;
					}
					else
					{
						AttackRelationShip = 100;
					}
					break;
				case EL_WATER:
					if ( lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER )
					{
						AttackRelationShip = 116;
					}
					else
					{
						AttackRelationShip = 90;
					}
					break;
				case EL_EARTH:
					if ( lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER )
					{
						AttackRelationShip = 123;
					}
					else
					{
						AttackRelationShip = 95;
					}
					break;
				case EL_WIND:
					if ( lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER )
					{
						AttackRelationShip = 136;
					}
					else
					{
						AttackRelationShip = 105;
					}
					break;
				case EL_DARKNESS:
					if ( lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER )
					{
						AttackRelationShip = 143;
					}
					else
					{
						AttackRelationShip = 110;
					}
					break;
				default:
					if ( lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER )
					{
						AttackRelationShip = 143;
					}
					else
					{
						if ( lpTargetObj->Type == OBJ_USER )
						{
							AttackRelationShip = 110;
						}

						else if ( lpTargetObj->Type == OBJ_MONSTER )
						{
							AttackRelationShip = 100;
						}
					}
					break;
			}
			break;

		case EL_WATER:
			switch ( DefenserAttribute )
			{
				case EL_FIRE:
					if ( lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER )
					{
						AttackRelationShip = 143;
					}
					else
					{
						AttackRelationShip = 110;
					}
					break;
				case EL_WATER:
					if ( lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER )
					{
						AttackRelationShip = 130;
					}
					else
					{
						AttackRelationShip = 100;
					}
					break;
				case EL_EARTH:
					if ( lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER )
					{
						AttackRelationShip = 116;
					}
					else
					{
						AttackRelationShip = 90;
					}
					break;
				case EL_WIND:
					if ( lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER )
					{
						AttackRelationShip = 123;
					}
					else
					{
						AttackRelationShip = 95;
					}
					break;
				case EL_DARKNESS:
					if ( lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER )
					{
						AttackRelationShip = 136;
					}
					else
					{
						AttackRelationShip = 105;
					}
					break;
				default:
					if ( lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER )
					{
						AttackRelationShip = 143;
					}
					else
					{
						if ( lpTargetObj->Type == OBJ_USER )
						{
							AttackRelationShip = 110;
						}

						else if ( lpTargetObj->Type == OBJ_MONSTER )
						{
							AttackRelationShip = 100;
						}
					}
					break;
			}
			break;
		case EL_EARTH:
			switch ( DefenserAttribute )
			{
				case EL_FIRE:
					if ( lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER )
					{
						AttackRelationShip = 136;
					}
					else
					{
						AttackRelationShip = 105;
					}
					break;
				case EL_WATER:
					if ( lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER )
					{
						AttackRelationShip = 143;
					}
					else
					{
						AttackRelationShip = 110;
					}
					break;
				case EL_EARTH:
					if ( lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER )
					{
						AttackRelationShip = 130;
					}
					else
					{
						AttackRelationShip = 100;
					}
					break;
				case EL_WIND:
					if ( lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER )
					{
						AttackRelationShip = 116;
					}
					else
					{
						AttackRelationShip = 90;
					}
					break;
				case EL_DARKNESS:
					if ( lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER )
					{
						AttackRelationShip = 123;
					}
					else
					{
						AttackRelationShip = 95;
					}
					break;
				default:
					if ( lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER )
					{
						AttackRelationShip = 143;
					}
					else
					{
						if ( lpTargetObj->Type == OBJ_USER )
						{
							AttackRelationShip = 110;
						}

						else if ( lpTargetObj->Type == OBJ_MONSTER )
						{
							AttackRelationShip = 100;
						}
					}
					break;
			}
			break;
		case EL_WIND:
			switch ( DefenserAttribute )
			{
				case EL_FIRE:
					if ( lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER )
					{
						AttackRelationShip = 123;
					}
					else
					{
						AttackRelationShip = 95;
					}
					break;
				case EL_WATER:
					if ( lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER )
					{
						AttackRelationShip = 136;
					}
					else
					{
						AttackRelationShip = 105;
					}
					break;
				case EL_EARTH:
					if ( lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER )
					{
						AttackRelationShip = 143;
					}
					else
					{
						AttackRelationShip = 110;
					}
					break;
				case EL_WIND:
					if ( lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER )
					{
						AttackRelationShip = 130;
					}
					else
					{
						AttackRelationShip = 100;
					}
					break;
				case EL_DARKNESS:
					if ( lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER )
					{
						AttackRelationShip = 116;
					}
					else
					{
						AttackRelationShip = 90;
					}
					break;
				default:
					if ( lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER )
					{
						AttackRelationShip = 143;
					}
					else
					{
						if ( lpTargetObj->Type == OBJ_USER )
						{
							AttackRelationShip = 110;
						}

						else if ( lpTargetObj->Type == OBJ_MONSTER )
						{
							AttackRelationShip = 100;
						}
					}
					break;
			}
			break;
		case EL_DARKNESS:
			switch ( DefenserAttribute )
			{
				case EL_FIRE:
					if ( lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER )
					{
						AttackRelationShip = 116;
					}
					else
					{
						AttackRelationShip = 90;
					}
					break;
				case EL_WATER:
					if ( lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER )
					{
						AttackRelationShip = 123;
					}
					else
					{
						AttackRelationShip = 95;
					}
					break;
				case EL_EARTH:
					if ( lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER )
					{
						AttackRelationShip = 136;
					}
					else
					{
						AttackRelationShip = 105;
					}
					break;
				case EL_WIND:
					if ( lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER )
					{
						AttackRelationShip = 143;
					}
					else
					{
						AttackRelationShip = 110;
					}
					break;
				case EL_DARKNESS:
					if ( lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER )
					{
						AttackRelationShip = 130;
					}
					else
					{
						AttackRelationShip = 100;
					}
					break;
				default:
					if ( lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER )
					{
						AttackRelationShip = 143;
					}
					else
					{
						if ( lpTargetObj->Type == OBJ_USER )
						{
							AttackRelationShip = 110;
						}

						else if ( lpTargetObj->Type == OBJ_MONSTER )
						{
							AttackRelationShip = 100;
						}
					}
					break;
			}
			break;
		default:
			return 0;
	}

	if ( lpObj->Type == OBJ_USER )
	{
		switch ( lpObj->m_PlayerData->m_PentagramOptions.m_iRuby_2RankOptionNum )
		{
			case 1:
				if (DefenserAttribute == EL_FIRE)
					AttackRelationShip += lpObj->m_PlayerData->m_PentagramOptions.m_iRuby_2RankAddAttackRelationshipRate;
				break;
			case 2:
				if (DefenserAttribute == EL_WATER)
					AttackRelationShip += lpObj->m_PlayerData->m_PentagramOptions.m_iRuby_2RankAddAttackRelationshipRate;
				break;
			case 3:
				if (DefenserAttribute == EL_EARTH)
					AttackRelationShip += lpObj->m_PlayerData->m_PentagramOptions.m_iRuby_2RankAddAttackRelationshipRate;
				break;
			case 4:
				if (DefenserAttribute == EL_WIND)
					AttackRelationShip += lpObj->m_PlayerData->m_PentagramOptions.m_iRuby_2RankAddAttackRelationshipRate;
				break;
			case 5:
				if (DefenserAttribute == EL_DARKNESS)
					AttackRelationShip += lpObj->m_PlayerData->m_PentagramOptions.m_iRuby_2RankAddAttackRelationshipRate;
				break;
		}

		switch (lpObj->m_PlayerData->m_PentagramOptions.m_iEme_2RankOptionNum)
		{
			case 1:
				if (DefenserAttribute == EL_FIRE)
					AttackRelationShip += lpObj->m_PlayerData->m_PentagramOptions.m_iEme_2RankAddAttackRelationshipRate;
				break;
			case 2:
				if (DefenserAttribute == EL_WATER)
					AttackRelationShip += lpObj->m_PlayerData->m_PentagramOptions.m_iEme_2RankAddAttackRelationshipRate;
				break;
			case 3:
				if (DefenserAttribute == EL_EARTH)
					AttackRelationShip += lpObj->m_PlayerData->m_PentagramOptions.m_iEme_2RankAddAttackRelationshipRate;
				break;
			case 4:
				if (DefenserAttribute == EL_WIND)
					AttackRelationShip += lpObj->m_PlayerData->m_PentagramOptions.m_iEme_2RankAddAttackRelationshipRate;
				break;
			case 5:
				if (DefenserAttribute == EL_DARKNESS)
					AttackRelationShip += lpObj->m_PlayerData->m_PentagramOptions.m_iEme_2RankAddAttackRelationshipRate;
				break;
		}
	}

	if ( lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER )
	{
		AttackRelationShip *= 2;
	}

	TotalDamageWithRelationShip = AttackRelationShip * TotalDamage / 100;

	BOOL StrongerElement = FALSE;

	switch ( DefenserAttribute )
	{
		case EL_FIRE:
			{
				switch ( AttackerAttribute )
				{
					case EL_FIRE:
						DefenseRelationShip = 100;
						break;
					case EL_WATER:
						StrongerElement = TRUE;
						DefenseRelationShip = 90;
						break;
					case EL_EARTH:
						StrongerElement = TRUE;
						DefenseRelationShip = 95;
						break;
					case EL_WIND:
						DefenseRelationShip = 105;
						break;
					case EL_DARKNESS:
						DefenseRelationShip = 110;
						break;
					default:
						return 0;
				}
			}
			break;
		case EL_WATER:
			{
				switch ( AttackerAttribute )
				{
					case EL_FIRE:
						DefenseRelationShip = 110;
						break;
					case EL_WATER:
						DefenseRelationShip = 100;
						break;
					case EL_EARTH:
						StrongerElement = TRUE;
						DefenseRelationShip = 90;
						break;
					case EL_WIND:
						StrongerElement = TRUE;
						DefenseRelationShip = 95;
						break;
					case EL_DARKNESS:
						DefenseRelationShip = 105;
						break;
					default:
						return 0;
				}
			}
			break;
		case EL_EARTH:
			{
				switch ( AttackerAttribute )
				{
					case EL_FIRE:
						DefenseRelationShip = 105;
						break;
					case EL_WATER:
						DefenseRelationShip = 110;
						break;
					case EL_EARTH:
						StrongerElement = TRUE;
						DefenseRelationShip = 90;
						break;
					case EL_WIND:
						DefenseRelationShip = 100;
						break;
					case EL_DARKNESS:
						StrongerElement = TRUE;
						DefenseRelationShip = 95;
						break;
					default:
						return 0;
				}
			}
			break;
		case EL_WIND:
			{
				switch ( AttackerAttribute )
				{
					case EL_FIRE:
						StrongerElement = TRUE;
						DefenseRelationShip = 95;
						break;
					case EL_WATER:
						DefenseRelationShip = 105;
						break;
					case EL_EARTH:
						DefenseRelationShip = 110;
						break;
					case EL_WIND:
						DefenseRelationShip = 100;
						break;
					case EL_DARKNESS:
						StrongerElement = TRUE;
						DefenseRelationShip = 90;
						break;
					default:
						return 0;
				}
			}
			break;
		case EL_DARKNESS:
			{
				switch ( AttackerAttribute )
				{
					case EL_FIRE:
						StrongerElement = TRUE;
						DefenseRelationShip = 90;
						break;
					case EL_WATER:
						StrongerElement = TRUE;
						DefenseRelationShip = 95;
						break;
					case EL_EARTH:
						DefenseRelationShip = 105;
						break;
					case EL_WIND:
						DefenseRelationShip = 110;
						break;
					case EL_DARKNESS:
						DefenseRelationShip = 100;
						break;
					default:
						return 0;
				}
			}
			break;
		default:
			{
				if ( AttackerAttribute < 0 || AttackerAttribute > 5 )
					return 0;

				if ( lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER )
					DefenseRelationShip = 80;

				if ( lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_MONSTER )
					DefenseRelationShip = 100;
				else
					DefenseRelationShip = 90;
			}
			break;
	}

	if ( lpTargetObj->Type == OBJ_USER && lpObj->Type == OBJ_USER )
	{
		switch (lpTargetObj->m_PlayerData->m_PentagramOptions.m_iSapph_2RankOptionNum)
		{
			case 1:
				if (AttackerAttribute == EL_FIRE)
					DefenseRelationShip += lpTargetObj->m_PlayerData->m_PentagramOptions.m_iSapph_2RankAddDefenseRelationshipRate;
				break;
			case 2:
				if (AttackerAttribute == EL_WATER)
					DefenseRelationShip += lpTargetObj->m_PlayerData->m_PentagramOptions.m_iSapph_2RankAddDefenseRelationshipRate;
				break;
			case 3:
				if (AttackerAttribute == EL_EARTH)
					DefenseRelationShip += lpTargetObj->m_PlayerData->m_PentagramOptions.m_iSapph_2RankAddDefenseRelationshipRate;
				break;
			case 4:
				if (AttackerAttribute == EL_WIND)
					DefenseRelationShip += lpTargetObj->m_PlayerData->m_PentagramOptions.m_iSapph_2RankAddDefenseRelationshipRate;
				break;
			case 5:
				if (AttackerAttribute == EL_DARKNESS)
					DefenseRelationShip += lpTargetObj->m_PlayerData->m_PentagramOptions.m_iSapph_2RankAddDefenseRelationshipRate;
				break;
		}

		switch (lpTargetObj->m_PlayerData->m_PentagramOptions.m_iTopa_2RankOptionNum)
		{
			case 1:
				if (AttackerAttribute == EL_FIRE)
					DefenseRelationShip += lpTargetObj->m_PlayerData->m_PentagramOptions.m_iTopa_2RankAddDefenseRelationshipRate;
				break;
			case 2:
				if (AttackerAttribute == EL_WATER)
					DefenseRelationShip += lpTargetObj->m_PlayerData->m_PentagramOptions.m_iTopa_2RankAddDefenseRelationshipRate;
				break;
			case 3:
				if (AttackerAttribute == EL_EARTH)
					DefenseRelationShip += lpTargetObj->m_PlayerData->m_PentagramOptions.m_iTopa_2RankAddDefenseRelationshipRate;
				break;
			case 4:
				if (AttackerAttribute == EL_WIND)
					DefenseRelationShip += lpTargetObj->m_PlayerData->m_PentagramOptions.m_iTopa_2RankAddDefenseRelationshipRate;
				break;
			case 5:
				if (AttackerAttribute == EL_DARKNESS)
					DefenseRelationShip += lpTargetObj->m_PlayerData->m_PentagramOptions.m_iTopa_2RankAddDefenseRelationshipRate;
				break;
		}
	}

	TotalDefenseWithRelationShip = DefenseRelationShip * TotalDefense / 100;

	*DamageType2 = 0;

	if ( TotalDamageWithRelationShip - TotalDefenseWithRelationShip > 0 && TotalDamageWithRelationShip > 10 )
	{
		if (lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER)
		{
			TotalDamageWithRelationShip = TotalDamageWithRelationShip * g_ConfigRead.calc.ElementalPvPDamageRate[lpObj->Class][lpTargetObj->Class] / 100;
		}

		else if (lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_MONSTER)
		{
			TotalDamageWithRelationShip = TotalDamageWithRelationShip * g_ConfigRead.calc.ElementalPvMDamageRate[lpObj->Class] / 100;
		}

		TotalDamageWithRelationShip -= TotalDefenseWithRelationShip;

		if (bExcellentDamageOption == TRUE)
		{
			TotalDamageWithRelationShip += TotalDamageWithRelationShip * 0.5;
			*DamageType2 = 6;
		}

		if (bExcellentDamageOption == FALSE && bCriticalDamageOption == TRUE)
		{
			*DamageType2 = 7;
		}

		if ( lpTargetObj->Type == OBJ_USER )
		{
			if (lpTargetObj->m_PlayerData->m_PentagramOptions.m_iSapph_5RankOptionNum == 1 && lpObj->Type == OBJ_USER)
			{
				TotalDamageWithRelationShip -= lpTargetObj->m_PlayerData->m_PentagramOptions.m_iSapph_5RankMinusTargetDamageRate * TotalDamageWithRelationShip / 100;
			}

			else if (lpTargetObj->m_PlayerData->m_PentagramOptions.m_iSapph_5RankOptionNum == 2 && lpObj->Type == OBJ_MONSTER)
			{
				TotalDamageWithRelationShip -= lpTargetObj->m_PlayerData->m_PentagramOptions.m_iSapph_5RankMinusTargetDamageRate * TotalDamageWithRelationShip / 100;
			}
		}
	}

	else
	{
		if ( lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER )
		{
			TotalDamageWithRelationShip = (lpObj->Level+lpObj->m_PlayerData->MasterLevel) / ((rand()%31) + 70);
		}

		else if ( lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_MONSTER )
		{
			if ( lpObj->Level - lpTargetObj->Level > 10 )
			{
				TotalDamageWithRelationShip = (lpObj->Level+lpObj->m_PlayerData->MasterLevel-lpTargetObj->Level) / ((rand()%31) + 70);
			}

			else
			{
				TotalDamageWithRelationShip = 0;
			}
		}

		else if ( lpObj->Type == OBJ_MONSTER && lpTargetObj->Type == OBJ_USER )
		{
			TotalDamageWithRelationShip = 0;
		}
	}

	if (lpTargetObj->Type == OBJ_USER && lpTargetObj->m_PlayerData->m_PentagramOptions.m_isAddResistByStrongRelationShip == true && (rand()%100) < 5)
	{
		switch (AttackerAttribute)
		{
		case 1:
			if (DefenserAttribute == EL_WIND || DefenserAttribute == EL_DARKNESS)
				return 0;
			break;
		case 2:
			if (DefenserAttribute == EL_DARKNESS || DefenserAttribute == EL_FIRE)
				return 0;
			break;
		case 3:
			if (DefenserAttribute == EL_FIRE || DefenserAttribute == EL_WATER)
				return 0;
			break;
		case 4:
			if (DefenserAttribute == EL_WATER || DefenserAttribute == EL_EARTH)
				return 0;
			break;
		default:
			if (AttackerAttribute == EL_DARKNESS && (DefenserAttribute == EL_EARTH || DefenserAttribute == EL_WIND))
				return 0;
			break;
		}
	}

	if (lpTargetObj->Type == OBJ_USER && lpTargetObj->m_PlayerData->m_PentagramOptions.m_isAddResistByPentaAttack == true && (rand()%100) < 5)
		return 0;

	if ( lpObj->Type == OBJ_USER )
	{
		AttackSuccessRate = ( 3 * ( lpObj->AddDexterity + lpObj->m_PlayerData->Dexterity ) / 2 ) + ( 5 * ( lpObj->Level + lpObj->m_PlayerData->MasterLevel ) ) + ( ( lpObj->AddStrength+lpObj->m_PlayerData->Strength) / 4 );
		AttackSuccessRate += AttackSuccessRate * lpObj->m_PlayerData->m_PentagramOptions.m_iEme_1RankAddAttackRate;
	}

	else
	{
		AttackSuccessRate = lpObj->m_iPentagramAttackRating;
	}

	if ( lpTargetObj->Type == OBJ_USER )
	{
		switch ( lpTargetObj->Class )
		{
			case CLASS_ELF:
				DefenseSuccessRate = lpTargetObj->m_PlayerData->Dexterity / 4;
				break;
			case CLASS_DARKLORD:
				DefenseSuccessRate = lpTargetObj->m_PlayerData->Dexterity / 7;
				break;
			case CLASS_SUMMONER:
				DefenseSuccessRate = lpTargetObj->m_PlayerData->Dexterity / 4;
				break;
			case CLASS_RAGEFIGHTER:
				DefenseSuccessRate = lpTargetObj->m_PlayerData->Dexterity / 10;
				break;
			default:
				DefenseSuccessRate = lpTargetObj->m_PlayerData->Dexterity / 3;
				break;
		}

		DefenseSuccessRate += DefenseSuccessRate * lpTargetObj->m_PlayerData->m_PentagramOptions.m_iTopa_1RankAddDefenseSuccessRate / 100;
	}

	else
	{
		DefenseSuccessRate = lpTargetObj->m_iPentagramDefenseRating;
	}

	BOOL bAllMiss = FALSE;

	if ( lpObj->PartyNumber < 0 || lpTargetObj->PartyNumber < 0 || lpObj->PartyNumber != lpTargetObj->PartyNumber )
	{
		if ( AttackSuccessRate < DefenseSuccessRate || AttackSuccessRate == 0)
			bAllMiss = TRUE;

		if ( bAllMiss )
		{
			if (rand() % 100 >= 5)
			{
				return 0;
			}
		}

		else if ( rand()% AttackSuccessRate >= DefenseSuccessRate )
		{
			return TotalDamageWithRelationShip;
		}

		else
		{
			return 0;
		}
	}

	return 0;
}



//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// MasterLevelSkillTreeSystem.cpp
#include "stdafx.h"
#include "MasterLevelSkillTreeSystem.h"
#include "MasterSkillSystem.h"
#include "TLog.h"
#include "GameMain.h"
#include "configread.h"
#include "ObjCalCharacter.h"
#include "BuffEffectSlot.h"
#include "BuffEffect.h"
#include "SkillAdditionInfo.h"
#include "ChaosCastle.h"
#include "ObjUseSkill.h"
#include "SkillHitBox.h"
#include "zzzmathlib.h"
#include "GensSystem.h"
#include "ObjAttack.h"
#include "ArcaBattle.h"
#include "winutil.h"

CMasterLevelSkillTreeSystem g_MasterLevelSkillTreeSystem;

MLST_Value_Table::MLST_Value_Table()
{
	this->fValueTable = NULL;
}

MLST_Value_Table::~MLST_Value_Table()
{
	if (this->fValueTable != NULL)
	{
		delete[] this->fValueTable;
		this->fValueTable = NULL;
	}
}

CMasterLevelSkillTreeSystem::CMasterLevelSkillTreeSystem(void) : m_Lua(true)
{
	this->m_MLSValueTable = NULL;
}

CMasterLevelSkillTreeSystem::~CMasterLevelSkillTreeSystem(void)
{
	if ( this->m_MLSValueTable != NULL )
	{
		delete [] this->m_MLSValueTable;
		this->m_MLSValueTable = NULL;
	}
}

void CMasterLevelSkillTreeSystem::Init()
{
	memset(&this->m_MLSTable, 0x00, sizeof(this->m_MLSTable));

	if (this->m_MLSValueTable != NULL)
	{
		delete [] this->m_MLSValueTable;
		this->m_MLSValueTable = NULL;
	}

	this->m_MLSValueTable = new MLST_Value_Table[MAX_MASTER_SKILL_TYPE];

	for (int i = 0; i < MAX_MASTER_SKILL_TYPE; i++ )
	{
		this->m_MLSValueTable[i].Init(this->GetMaxMasterSkillLevel());
	}

	lua_State* pLuaState = this->m_Lua.GetLua();

	if (pLuaState == NULL)
	{
		g_Log.Add("[MasterLevelSkillTreeSystem] - Error - [%s] [%d]", __FILE__, __LINE__);
		return;
	}

	this->m_Lua.DoFile(g_ConfigRead.GetPath("\\Scripts\\Skills\\MasterSkillPoint.lua"));
	this->m_Lua.DoFile(g_ConfigRead.GetPath("\\Scripts\\Skills\\MasterSkillCalc.lua"));

	this->m_Lua.Generic_Call("MLS_ValueInit", ">");
}

void CMasterLevelSkillTreeSystem::AddToValueTable(int iSkillValueID, const char * szFunctionName, int iSkillValueType)
{
	if (iSkillValueID < 0 || iSkillValueID > MAX_MASTER_SKILL_TYPE)
	{
		g_Log.Add("[MasterLevelSkillTreeSystem] - Error - Wrong iSkillValueID [%d] - [%s] [%d]", iSkillValueID, __FILE__, __LINE__);
		return;
	}

	this->m_MLSValueTable[iSkillValueID].iValueType = iSkillValueType;
	this->m_MLSValueTable[iSkillValueID].fValueTable[0] = 0.0;

	double fValue = 0.0;

	for (int i = 1; i <= this->GetMaxMasterSkillLevel(); i++)
	{
		this->m_Lua.Generic_Call(szFunctionName, "i>d", i, &fValue);
		this->m_MLSValueTable[iSkillValueID].fValueTable[i] = this->RoundValue(fValue, 2);
	}
}

void CMasterLevelSkillTreeSystem::Load(const char *lpszFileName)
{
	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(lpszFileName);

	if ( res.status != pugi::status_ok )
	{
		g_Log.MsgBox("[MasterLevelSkillTreeSystem] - Can't Load %s (%s)", lpszFileName, res.description());
		return;
	}

	int iIndex = -1;
	int iTempIndex, iRank, iPos, iPoint, iMaxPoint, iParentSkill1, iParentSkill2, iSkillID;

	pugi::xml_node main = file.child("MasterSkillTree");

	this->m_wMaxMasterSkillLevel = main.attribute("MaxSkillLevel").as_int(20); // private custom !!!
	this->Init();

	for (pugi::xml_node class_node = main.child("Class"); class_node; class_node = class_node.next_sibling())
	{
		int iClass = class_node.attribute("ID").as_int();

		for (pugi::xml_node tree = class_node.child("Tree"); tree; tree = tree.next_sibling())
		{
			int iTreeType = tree.attribute("Type").as_int();

			for (pugi::xml_node skill = tree.child("Skill"); skill; skill = skill.next_sibling())
			{
				iIndex = skill.attribute("Index").as_int();
				iTempIndex = iIndex % 36 - 1;
				iRank = iTempIndex / 4;
				iPos = iTempIndex % 4;

				iPoint = skill.attribute("ReqMinPoint").as_int();
				iMaxPoint = skill.attribute("MaxPoint").as_int();
				iParentSkill1 = skill.attribute("ParentSkill1").as_int();
				iParentSkill2 = skill.attribute("ParentSkill2").as_int();
				iSkillID = skill.attribute("MagicNumber").as_int();

				for (int i = 0; i < MAX_TYPE_PLAYER; i++)
				{
					if (CHARACTER_CLASS_TABLE[i][0] & iClass)
					{
						this->m_MLSTable[CHARACTER_CLASS_TABLE[i][1]].iAbility[iTreeType].iElement[iRank][iPos].m_dwIndex = iIndex;
						this->m_MLSTable[CHARACTER_CLASS_TABLE[i][1]].iAbility[iTreeType].iElement[iRank][iPos].m_byConsumePoint = iPoint;
						this->m_MLSTable[CHARACTER_CLASS_TABLE[i][1]].iAbility[iTreeType].iElement[iRank][iPos].m_byMaxPoint = iMaxPoint;
						this->m_MLSTable[CHARACTER_CLASS_TABLE[i][1]].iAbility[iTreeType].iElement[iRank][iPos].m_dwParentSkill1 = iParentSkill1;
						this->m_MLSTable[CHARACTER_CLASS_TABLE[i][1]].iAbility[iTreeType].iElement[iRank][iPos].m_dwParentSkill2 = iParentSkill2;
						this->m_MLSTable[CHARACTER_CLASS_TABLE[i][1]].iAbility[iTreeType].iElement[iRank][iPos].m_dwSkillID = iSkillID;
					}
				}
			}
		}
	}

	g_Log.Add("[MasterLevelSkillTreeSystem] - %s file is Loaded", lpszFileName);
}

void CMasterLevelSkillTreeSystem::InitPassiveSkillData(LPOBJ lpObj)
{
	if ( lpObj->Type != OBJ_USER )
	{
		return;
	}

	lpObj->m_PlayerData->m_MPSkillOpt.Clear();
}

void CMasterLevelSkillTreeSystem::CalcPassiveSkillData(LPOBJ lpObj, int iMLSkill, int iSkillLevel)
{
	if ( lpObj->Type != OBJ_USER )
	{
		return;
	}

	if ( lpObj->m_PlayerData->Strength < MagicDamageC.GetRequireStrength(iMLSkill) )
	{
		return;
	}

	if ( lpObj->m_PlayerData->Dexterity < MagicDamageC.GetRequireDexterity(iMLSkill) )
	{
		return;
	}

	float iSkillValue = this->GetMasterSkillValue(MagicDamageC.SkillGet(iMLSkill), iSkillLevel);

	switch ( this->GetBaseMasterLevelSkill(iMLSkill) )
	{
		case 325:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsAttackSuccessRate = iSkillValue;
			break;
		case 347:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsPVPAttackDmgRate = iSkillValue;
			break;
		case 301:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsPVPBlockingRate = iSkillValue;
			break;
		case 300:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsDownDur1 = iSkillValue;
			lpObj->m_PlayerData->m_MPSkillOpt.btMpsDownDur1Level = iSkillLevel;
			break;
		case 305:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsDownDur2 = iSkillValue;
			break;
		case 312:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsDownDur3 = iSkillValue;
			break;
		case 304:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsResistancePoison = iSkillValue;
			break;
		case 308:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsResistanceThunder = iSkillValue;
			break;
		case 311:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsResistanceIce = iSkillValue;
			break;
		case 307:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsAutoRecoverLife = iSkillValue;
			break;
		case 313:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsBlockingSuccessRate = iSkillValue;
			break;
		case 309:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsDefence = iSkillValue;
			break;
		case 334:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsMaxHP = iSkillValue;
			break;
		case 338:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsMaxMana = iSkillValue;
			break;
		case 341:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsMaxBP = iSkillValue;
			break;
		case 362:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsMonsterDieGetMana = iSkillValue;
			break;
		case 359:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsMonsterDieGetLife = iSkillValue;
			break;
		case 358:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsMonsterDieGetSD = iSkillValue;
			break;
		case 303:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsAutoRecoverMana = iSkillValue;
			break;
		case 310:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsAutoRecoverAG = iSkillValue;
			break;
		case 302:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsMaxSD = iSkillValue;
			break;
		case 306:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsSDSpeed = iSkillValue;
			break;
		case 364:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsMaxAttackDamage = iSkillValue;
			break;
		case 361:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsMinAttackDamage = iSkillValue;
			break;
		case 357:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsDecreaseMana = iSkillValue;
			break;
		case 407:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsMaxMagicDamage = iSkillValue;
			break;
		case 405:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsMinMagicDamage = iSkillValue;
			break;
		case 535:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsPetDurDownSpeed = iSkillValue;
			break;
		case 335:
		case 485:
		case 513:
		case 557:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsAddPhysicDamage = iSkillValue;
			break;
		case 348:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsTwoHandSwordAddDamage = iSkillValue;
			break;
		case 349:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsSwordAddDamage = iSkillValue;
			break;
		case 350:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsScultAddDamage = iSkillValue;
			break;
		case 351:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsSpearAddDamage = iSkillValue;
			break;
		case 352:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsTwoHandsPvPAttack = iSkillValue;
			break;
		case 353:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsAddAttackSpeed = iSkillValue;
			break;
		case 354:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsAddSturn = iSkillValue;
			break;
		case 355:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsAddDoubleDamage = iSkillValue;
			break;
		case 386:
		case 488:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsAddMagicDamage = iSkillValue;
			break;
		case 397:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsStaffMagicPower = iSkillValue;
			break;
		case 398:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsTwoHandsMagicPower = iSkillValue;
			break;
		case 399:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncShield = iSkillValue;
			break;
		case 400:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsStaffAttackSpeed = iSkillValue;
			break;
		case 401:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsTwoHandsStaffManAttack = iSkillValue;
			break;
		case 402:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsShieldBlockRate = iSkillValue;
			break;
		case 435:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsBowAddDamage = iSkillValue;
			break;
		case 438:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsBowAddAttackSpeed = iSkillValue;
			break;
		case 436:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsTwoHandsBowAddDamage = iSkillValue;
			break;
		case 439:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsTwoHandsBowManAttack = iSkillValue;
			break;
		case 421:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsElfAddPhysicDamage = iSkillValue;
			break;
		case 415:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsCallMonHp = iSkillValue;
			break;
		case 419:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsCallMonDefense = iSkillValue;
			break;
		case 428:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsCallMonAttack = iSkillValue;
			break;
		case 442:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsAddMinAttack = iSkillValue;
			break;
		case 443:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsAddMaxAttack = iSkillValue;
			break;
		case 448:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncDamageFireProper = iSkillValue;
			break;
		case 449:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncDamageWindProper = iSkillValue;
			break;
		case 450:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncDamageThunderProper = iSkillValue;
			break;
		case 451:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncDotDamage = iSkillValue;
			break;
		case 452:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsAddSturn2 = iSkillValue;
			break;
		case 453:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsAddSpring = iSkillValue;
			break;
		case 457:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncMagicPower = iSkillValue;
			break;
		case 465:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsAddMagicPowerStick = iSkillValue;
			break;
		case 466:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsAddCurseMagicBook = iSkillValue;
			break;
		case 467:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsAddManAttackStick = iSkillValue;
			break;
		case 468:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsAddAttackSpeedMagicBook = iSkillValue;
			break;
		case 471:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncMinMagicDamage = iSkillValue;
			break;
		case 526:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsAddDarkSpiritDamage = iSkillValue;
			break;
		case 530:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsAddCriticalDamageRate = iSkillValue;
			break;
		case 534:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsAddExcellentDamageRate = iSkillValue;
			break;
		case 527:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsAddCeptorDamage = iSkillValue;
			break;
		case 531:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncCeptorManAttack = iSkillValue;
			break;
		case 528:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsAddShieldDefense = iSkillValue;
			break;
		case 532:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsAddShieldBlockingRage = iSkillValue;
			break;
		case 529:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsAddPetAttack = iSkillValue;
			break;
		case 533:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsAddControlAttack = iSkillValue;
			break;
		case 556:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsAddBlockingRate_Monk = iSkillValue;
			break;
		case 568:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsMonkWeaponDamage = iSkillValue;
			break;
		case 571:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsMonkDoubleDamage = iSkillValue;
			break;
		case 574:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsMonkAddVitalToDefense = iSkillValue;
			break;
		case 599:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsAttackSuccessRate_Monk = iSkillValue;
			break;
		case 603:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsPVPAttackDmgRate_Monk = iSkillValue;
			break;
		case 579:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsPVPBlockingRate_Monk = iSkillValue;
			break;
		case 578:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsDownDur1_Monk = iSkillValue;
			lpObj->m_PlayerData->m_MPSkillOpt.btMpsDownDur1Level_Monk = iSkillLevel;
			break;
		case 583:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsDownDur2_Monk = iSkillValue;
			break;
		case 590:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsDownDur3_Monk = iSkillValue;
			break;
		case 582:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsResistancePoison_Monk = iSkillValue;
			break;
		case 586:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsResistanceThunder_Monk = iSkillValue;
			break;
		case 589:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsResistanceIce_Monk = iSkillValue;
			break;
		case 585:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsAutoRecoverLife_Monk = iSkillValue;
			break;
		case 591:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsBlockingSuccessRate_Monk = iSkillValue;
			break;
		case 587:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsDefence_Monk = iSkillValue;
			break;
		case 600:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsMaxHP_Monk = iSkillValue;
			break;
		case 601:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsMaxMana_Monk = iSkillValue;
			break;
		case 602:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsMaxBP_Monk = iSkillValue;
			break;
		case 608:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsMonsterDieGetMana_Monk = iSkillValue;
			break;
		case 606:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsMonsterDieGetLife_Monk = iSkillValue;
			break;
		case 605:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsMonsterDieGetSD_Monk = iSkillValue;
			break;
		case 581:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsAutoRecoverMana_Monk = iSkillValue;
			break;
		case 588:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsAutoRecoverAG_Monk = iSkillValue;
			break;
		case 580:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsMaxSD_Monk = iSkillValue;
			break;
		case 584:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsSDSpeed_Monk = iSkillValue;
			break;
		case 609:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsAddMaxAttack_Monk = iSkillValue;
			break;
		case 607:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsAddMinAttack_Monk = iSkillValue;
			break;
		case 604:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsDecreaseMana_Monk = iSkillValue;
			break;
		case 314:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsImmuneRate = iSkillValue;
			break;
		case 315:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncSetItemDefense = iSkillValue;
			break;
		case 316:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsReturnEnemyAttack = iSkillValue;
			break;
		case 317:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncEnergyStat = iSkillValue;
			break;
		case 318:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncVitalStat = iSkillValue;
			break;
		case 319:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncDexStat = iSkillValue;
			break;
		case 320:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncPowerStat = iSkillValue;
			break;
		case 506:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncLeadershipStat = iSkillValue;
			break;
		case 321:
		case 374:
		case 409:
		case 445:
		case 475:
		case 504:
		case 548:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsAddWingDamageBlock = iSkillValue;
			break;
		case 322:
		case 375:
		case 410:
		case 446:
		case 476:
		case 505:
		case 549:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsAddWingDefense = iSkillValue;
			break;
		case 324:
		case 377:
		case 412:
		case 447:
		case 478:
		case 507:
		case 550:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsAddWingDamage = iSkillValue;
			break;
		case 366:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncCriticalDamageRate = iSkillValue;
			break;
		case 367:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsRecoverManaRate = iSkillValue;
			break;
		case 368:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsRecoverHPRate = iSkillValue;
			break;
		case 369:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncExcellentDamageRate = iSkillValue;
			break;
		case 370:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncDoubleDamageRate = iSkillValue;
			break;
		case 371:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncIgnoreEnemyBlock = iSkillValue;
			break;
		case 372:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsRecoverSDRate = iSkillValue;
			break;
		case 373:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncTripleDamageRate = iSkillValue;
			break;
		case 345:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsComboDamage = iSkillValue;
			break;
		case 473:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncMaxMagicDamage = iSkillValue;
			break;
		case 536:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncDarkSpiritAttackSpeed = iSkillValue;
			break;
		case 538:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncDarkSpiritDoubleDamageRate = iSkillValue;
			break;
		case 539:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsDarkSpiritIgnoreEnemyDefense = iSkillValue;
			break;
		case 592:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsImmuneRate_Monk = iSkillValue;
			break;
		case 593:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncSetItemDefense_Monk = iSkillValue;
			break;
		case 594:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsReturnEnemyAttack_Monk = iSkillValue;
			break;
		case 595:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncEnergyStat_Monk = iSkillValue;
			break;
		case 596:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncVitalStat_Monk = iSkillValue;
			break;
		case 597:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncDexStat_Monk = iSkillValue;
			break;
		case 598:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncPowerStat_Monk = iSkillValue;
			break;
		case 610:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncCriticalDamageRate_Monk = iSkillValue;
			break;
		case 611:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsRecoverManaRate_Monk = iSkillValue;
			break;
		case 612:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsRecoverHPRate_Monk = iSkillValue;
			break;
		case 613:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncExcellentDamageRate_Monk = iSkillValue;
			break;
		case 614:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncDoubleDamageRate_Monk = iSkillValue;
			break;
		case 615:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncIgnoreEnemyBlock_Monk = iSkillValue;
			break;
		case 616:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsRecoverSDRate_Monk = iSkillValue;
			break;
		case 617:
			lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncTripleDamageRate_Monk = iSkillValue;
			break;
		default:
			break;
	}

	gObjCalCharacter.CalcCharacter(lpObj->m_Index);
}

int CMasterLevelSkillTreeSystem::CheckSkillCondition(LPOBJ lpObj, int iMLSkill, int iSkillLevel)
{
	if ( gObjIsConnected(lpObj) == FALSE )
	{
		GSProtocol.GCResultSend(lpObj->m_Index, 0x51, 0x03);
		return 2;
	}

	if ( iMLSkill < 0 || iMLSkill > MAX_SKILL-1 )
	{
		g_Log.Add("error: %s %d", __FILE__, __LINE__);
		return 3;
	}

	if ( this->IsMasterLevelUser(lpObj) == false )
	{
		return 2;
	}

	if ( this->CheckMasterLevelSkill(iMLSkill) == false )
	{
		return 4;
	}

	if ( this->GetMaxPointOfMasterSkill(lpObj->Class, iMLSkill) < iSkillLevel )
	{
		return 4;
	}

	int iReqMLPoint = this->GetRequireMLPoint(lpObj->Class, iMLSkill);

	if ( iReqMLPoint < 0 )
	{
		return 4;
	}

	if ( lpObj->m_PlayerData->MasterPoint < iReqMLPoint )
	{
		return 4;
	}

	if ( iSkillLevel != 1 )
	{
		return 1;
	}

	if ( this->CheckCompleteBrandOfSkill(lpObj, iMLSkill) == false )
	{
		return 4;
	}

	if ( this->CheckPreviousRankSkill(lpObj, iMLSkill, 1) == false )
	{
		return 4;
	}
	
	return 1;
}

BOOL CMasterLevelSkillTreeSystem::CheckCompleteBrandOfSkill(LPOBJ lpObj, int iMLSkill)
{
	if ( MagicDamageC.GetSkillUseType(iMLSkill) == 4 )
	{
		int iBrandOfSkill = MagicDamageC.GetBrandOfSkill(iMLSkill);
		BOOL iCompleteSkill = FALSE;

		if ( iBrandOfSkill > 0 )
		{
			for ( int iCount = 0; iCount < MAX_MAGIC; iCount++ )
			{
				if ( lpObj->Magic[iCount].IsMagic() == TRUE && lpObj->Magic[iCount].m_Skill == iBrandOfSkill )
				{
					iCompleteSkill = TRUE;
					break;
				}
			}
		}

		if ( iCompleteSkill == FALSE )
		{
			return FALSE;
		}
	}

	std::pair<int,int>iBrandOfSkill = std::pair<int, int>(this->GetParentSkill1(lpObj->Class, iMLSkill), this->GetParentSkill2(lpObj->Class, iMLSkill));

	if ( iBrandOfSkill.first <= 0 )
	{
		return TRUE;
	}

	BOOL iCompleteSkill1 = FALSE;
	BOOL iCompleteSkill2 = FALSE;

	if ( iBrandOfSkill.first > 0 )
	{
		for (  int i=0;i<MAX_MAGIC;i++ )
		{
			if ( lpObj->Magic[i].IsMagic() == TRUE &&
				lpObj->Magic[i].m_Skill == iBrandOfSkill.first &&
				lpObj->Magic[i].m_Level >= 10 )
			{
				iCompleteSkill1 = TRUE;
				break;
			}
		}
	}

	if ( iBrandOfSkill.second > 0 )
	{
		for (  int i=0;i<MAX_MAGIC;i++ )
		{
			if ( lpObj->Magic[i].IsMagic() == TRUE &&
				lpObj->Magic[i].m_Skill == iBrandOfSkill.second &&
				lpObj->Magic[i].m_Level >= 10 )
			{
				iCompleteSkill2 = TRUE;
				break;
			}
		}
	}

	if ( (iBrandOfSkill.first + iBrandOfSkill.second) == iBrandOfSkill.first )
	{
		if ( iCompleteSkill1 == TRUE )
		{
			return TRUE;
		}
	}

	else if ( (iBrandOfSkill.first + iBrandOfSkill.second) == iBrandOfSkill.second )
	{
		if ( iCompleteSkill2 == TRUE )
		{
			return TRUE;
		}
	}

	else if ( iCompleteSkill1 == TRUE && iCompleteSkill2 == TRUE )
	{
		return TRUE;
	}

	return FALSE;
}

BOOL CMasterLevelSkillTreeSystem::CheckMasterLevelSkill(int iMLSkill)
{
	int iSkillUseType = MagicDamageC.GetSkillUseType(iMLSkill);

	return iSkillUseType == 3 || iSkillUseType == 4;
}

BOOL CMasterLevelSkillTreeSystem::IsExistMLSBasedSkill(LPOBJ lpObj, int iSkill)
{
	for ( int iCount=0; iCount<MAX_MAGIC;iCount++ )
	{
		if ( lpObj->Magic[iCount].IsMagic() == TRUE &&
			this->CheckMasterLevelSkill(lpObj->Magic[iCount].m_Skill) == true &&
			iSkill == MagicDamageC.GetBrandOfSkill(lpObj->Magic[iCount].m_Skill) )
		{
			return TRUE;
		}
	}

	return FALSE;
}

int CMasterLevelSkillTreeSystem::GetBaseMasterLevelSkill(int iMLSkill)
{
	if (iMLSkill < 0 || iMLSkill > MAX_SKILL - 1)
	{
		return -1;
	}

	if ( this->CheckMasterLevelSkill(iMLSkill) == true )
	{
		return iMLSkill;
	}

	return -1;
}

int CMasterLevelSkillTreeSystem::gObjMagicAdd_MLS(OBJECTSTRUCT *lpObj, int iMLSkill, int iSkillLevel)
{
	if ( g_EnergyCheckOff == TRUE )
	{
		int reqeng = MagicDamageC.SkillGetRequireEnergy(iMLSkill);

		if(reqeng < 0)
		{
			g_Log.Add("error-L2: Skill energy not enough");
			return -1;
		}

		if ( lpObj->Class == CLASS_KNIGHT)
			reqeng -= 10;
		if ( lpObj->Class == CLASS_ELF )
			reqeng -= 5;
		if ( lpObj->Class == CLASS_DARKLORD )
			reqeng -= 5;

		if ( (lpObj->m_PlayerData->Energy + lpObj->AddEnergy) < reqeng )
		{
			g_Log.Add("error-L2: Skill energy not enough");
			return -1;
		}
	}

	if(MagicDamageC.SkillGetRequireClass(lpObj->Class,lpObj->m_PlayerData->ChangeUP, iMLSkill) < 1 && lpObj->Type == OBJ_USER && lpObj->m_PlayerData->ISBOT == false)
	{
		g_Log.Add("error-L2: His class is not allowed to use the skill [ID: %d]", iMLSkill);
		return -1;
	}

	for ( int n=0;n<MAX_MAGIC;n++ )
	{
		if ( lpObj->Magic[n].IsMagic() == TRUE && lpObj->Magic[n].m_Skill == iMLSkill && lpObj->Magic[n].m_Level == iSkillLevel )
		{
			g_Log.Add("Same skill already exists %d %d", lpObj->Magic[n].m_Skill,iMLSkill);
			return -1;
		}
	}

	if ( MagicDamageC.GetSkillUseType(iMLSkill) == 3 )
	{
		if ( iSkillLevel > 1 )
		{
			for ( int n=0;n<MAX_MAGIC;n++ )
			{
				if ( lpObj->Magic[n].IsMagic() == TRUE && lpObj->Magic[n].m_Skill == iMLSkill && lpObj->Magic[n].m_Level == iSkillLevel-1 )
				{
					if ( lpObj->Magic[n].UpdateMasterSkill(iMLSkill, iSkillLevel) < 0 )
					{
						return -1;
					}

					else
					{
						return n;
					}
				}
			}
		}
	}

	else if ( MagicDamageC.GetSkillUseType(iMLSkill) == 4 ) 
	{
		int iBrand = MagicDamageC.GetBrandOfSkill(iMLSkill);

		for ( int n=0;n<MAX_MAGIC;n++ )
		{
			if ( lpObj->Magic[n].IsMagic() == TRUE )
			{
				if ( lpObj->Magic[n].m_Skill == iBrand )
					lpObj->Magic[n].m_Pass = 1;
				if ( lpObj->Magic[n].m_Skill == iMLSkill )
				{
					if ( lpObj->Magic[n].UpdateMasterSkill(iMLSkill, iSkillLevel) < 0 )
					{
						return -1;
					}

					else
					{
						return n;
					}
				}
			}
		}
	}

	else
	{
		return -1;
	}

	for ( int n=0;n<MAX_MAGIC;n++ )
	{
		if ( lpObj->Magic[n].IsMagic() == FALSE )
		{
			if ( lpObj->Magic[n].Set(iMLSkill, iSkillLevel) >= 0 )
			{
				lpObj->MagicCount++;

				if ( MagicDamageC.GetSkillUseType(iMLSkill) == 4 && !MagicDamageC.GetBrandOfSkill(iMLSkill) )
				{
					GSProtocol.GCMagicListOneSend(lpObj->m_Index, n, lpObj->Magic[n].m_Skill, lpObj->Magic[n].m_Level, 0, 0);
				}

				return n;
			}

			else
			{
				g_Log.Add("Same magic already exists %s %d",__FILE__,__LINE__);
				return -1;
			}
		}
	}

	g_Log.Add("No space to add magic %s %d",__FILE__,__LINE__);
	return -1;
}

void CMasterLevelSkillTreeSystem::CGReqGetMasterLevelSkillTree(int aIndex)
{
	if ( gObjIsConnected(aIndex) == FALSE )
	{
		GSProtocol.GCSendDisableReconnect(aIndex);
		//IOCP.CloseClient(aIndex); // lol
		return;
	}

	BYTE sendbuf[3072];
	PMSG_MASTERLEVELSKILLTREE_COUNT pCountMsg;
	PMSG_MASTERLEVELSKILLTREE pElementMsg;
	LPOBJ lpObj = &gObj[aIndex];

	PHeadSubSetW((LPBYTE)&pCountMsg, 0xF3, 0x53, sizeof(pCountMsg));

	int nCount = 0;
	memset(sendbuf, 0x00, sizeof(sendbuf));
	int lOfs = 12;

	for ( int i=0;i<MAX_MAGIC;i++ )
	{
		if ( lpObj->Magic[i].IsMagic() == TRUE && this->CheckMasterLevelSkill(lpObj->Magic[i].m_Skill) == true )
		{
			pElementMsg.btMasterSkillUIIndex = this->GetMasterSkillUIIndex(lpObj->Class, lpObj->Magic[i].m_Skill);
			pElementMsg.btMasterSkillLevel = lpObj->Magic[i].m_Level;

			if ( this->GetMasterSkillValue(MagicDamageC.SkillGet(lpObj->Magic[i].m_Skill), lpObj->Magic[i].m_Level) <= 0.0 )
			{
				pElementMsg.fMasterSkillCurValue = -1.0;
			}

			else
			{
				pElementMsg.fMasterSkillCurValue = this->GetMasterSkillValue(MagicDamageC.SkillGet(lpObj->Magic[i].m_Skill), lpObj->Magic[i].m_Level);
			}

			if ( this->GetMasterSkillValue(MagicDamageC.SkillGet(lpObj->Magic[i].m_Skill), lpObj->Magic[i].m_Level + 1) <= 0.0 )
			{
				pElementMsg.fMasterSkillNextValue = -1.0;
			}

			else
			{
				pElementMsg.fMasterSkillNextValue = this->GetMasterSkillValue(MagicDamageC.SkillGet(lpObj->Magic[i].m_Skill), lpObj->Magic[i].m_Level + 1);
			}

			nCount++;
			memcpy(&sendbuf[lOfs], &pElementMsg, sizeof(pElementMsg));
			lOfs += sizeof(pElementMsg);
		}
	}
	
	pCountMsg.count = nCount;
	pCountMsg.h.sizeH = HIBYTE(lOfs);
	pCountMsg.h.sizeL = LOBYTE(lOfs);

	memcpy(sendbuf, &pCountMsg, sizeof(pCountMsg));

	IOCP.DataSend(aIndex, sendbuf, lOfs);
}

void CMasterLevelSkillTreeSystem::CGReqGetMasterLevelSkill(PMSG_REQ_MASTERLEVEL_SKILL *lpMsg, int aIndex)
{
	if ( !gObjIsConnected(aIndex) )
	{
		GSProtocol.GCSendDisableReconnect(aIndex);
		//IOCP.CloseClient(aIndex);
		return;
	}

	if ( gObj[aIndex].Type != OBJ_USER )
	{
		return;
	}

	PMSG_ANS_MASTERLEVELSKILLTREE pMsg;

	LPOBJ lpObj = &gObj[aIndex];
	PHeadSubSetB((LPBYTE)&pMsg, 0xF3, 0x52, sizeof(pMsg));

	pMsg.btResult = 0;
	pMsg.wMasterLevelPoint = lpObj->m_PlayerData->MasterPoint;
	pMsg.dwMasterSkillIndex = -1;
	pMsg.dwMasterSkillLevel = 0;

	if ( lpObj->m_IfState.use && lpObj->m_IfState.type == 1 )
	{
		g_Log.Add("[MasterSkill] [%s][%s] Fail(Use Trade Interface) - Add Magic List", lpObj->AccountID, lpObj->Name);

		pMsg.btResult = 6;
		IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
		return;
	}

	int iCurSkillLevel = this->CheckSkillLevel(lpObj, lpMsg->iMasterLevelSkill);
	int iSkillLevel = this->GetRequireMLPoint(lpObj->Class, lpMsg->iMasterLevelSkill) + iCurSkillLevel;
	int iResult = this->CheckSkillCondition(lpObj, lpMsg->iMasterLevelSkill, iSkillLevel);

	if ( iResult != TRUE )
	{
		g_Log.Add("[MasterSkill] [%s][%s] Fail - Result:%d, Skill:%d Level:%d MLPoint:%d",
			lpObj->AccountID, lpObj->Name, iResult, lpMsg->iMasterLevelSkill, iSkillLevel, lpObj->m_PlayerData->MasterPoint);
		pMsg.btResult = iResult;
	}

	else
	{
		if ( this->gObjMagicAdd_MLS(lpObj, lpMsg->iMasterLevelSkill, iSkillLevel) < 0 )
		{
			g_Log.Add("[MasterSkill] [%s][%s] Fail - Add Magic List, Skill:%d Level:%d", lpObj->AccountID, lpObj->Name, lpMsg->iMasterLevelSkill, iSkillLevel);
		}

		else
		{
			int iReqMLPoint = this->GetRequireMLPoint(lpObj->Class, lpMsg->iMasterLevelSkill);

			if ( iReqMLPoint < 0 ) 
			{
				return;
			}

			lpObj->m_PlayerData->MasterPoint -= iReqMLPoint;
			lpObj->m_PlayerData->MasterPointUsed += iReqMLPoint;

			pMsg.btResult = TRUE;
			pMsg.wMasterLevelPoint = lpObj->m_PlayerData->MasterPoint;
			pMsg.btMasterSkillUIIndex = this->GetMasterSkillUIIndex(lpObj->Class, lpMsg->iMasterLevelSkill);
			pMsg.dwMasterSkillIndex = lpMsg->iMasterLevelSkill;
			pMsg.dwMasterSkillLevel = iSkillLevel;

			if ( this->GetMasterSkillValue(MagicDamageC.SkillGet(lpMsg->iMasterLevelSkill), iSkillLevel) <= 0.0 )
			{
				pMsg.fMasterSkillCurValue = -1.0;
			}

			else
			{
				pMsg.fMasterSkillCurValue = this->GetMasterSkillValue(MagicDamageC.SkillGet(lpMsg->iMasterLevelSkill), iSkillLevel);
			}

			if ( this->GetMasterSkillValue(MagicDamageC.SkillGet(lpMsg->iMasterLevelSkill), iSkillLevel + 1) <= 0.0 )
			{
				pMsg.fMasterSkillNextValue = -1.0;
			}

			else
			{
				pMsg.fMasterSkillNextValue = this->GetMasterSkillValue(MagicDamageC.SkillGet(lpMsg->iMasterLevelSkill), iSkillLevel + 1);
			}

			this->CalcPassiveSkillData(lpObj, lpMsg->iMasterLevelSkill, iSkillLevel);

			g_Log.Add("[MasterSkill] [%s][%s] Success - Add Magic List, Skill:%d Level:%d MLPoint:%d",
				lpObj->AccountID, lpObj->Name, lpMsg->iMasterLevelSkill, iSkillLevel, lpObj->m_PlayerData->MasterPoint);
		}
	}

	IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

BOOL CMasterLevelSkillTreeSystem::RunningSkill_MLS(OBJECTSTRUCT *lpObj, int aTargetIndex, CMagicInf *lpMagic, BOOL bCombo, BYTE x, BYTE y, BYTE aTargetPos)
{
	if ( !lpObj || !lpMagic )
	{
		return FALSE;
	}

	if (this->m_Lua.GetLua() == NULL)
	{
		return FALSE;
	}

	LPOBJ lpTargetObj = &gObj[aTargetIndex];

	switch ( lpMagic->m_Skill )
	{
		case 403:
			this->MLS_WizardMagicDefense(lpObj, aTargetIndex, lpMagic);
			return 1;
		case 413:
			this->MLS_Healing(lpObj, aTargetIndex, lpMagic);
			return 1;
		case 330:
		case 332:
		case 481:
			this->MLS_Wheel(lpObj, lpMagic, aTargetIndex, bCombo);
			return 1;
		case 360:
			this->MLS_SkillFulKnightSkillAddLife(lpObj, lpMagic);
			return 1;
		case 356:
			this->MLS_KnightSkillAddLife(lpObj, lpMagic);
			return 1;
		case 512:
		case 516:
			this->MLS_DarkHorseAttack(lpObj, aTargetIndex, lpMagic);
			return 1;
		case 508:
		case 514:
			this->MLS_FireBurst(lpObj, aTargetIndex, lpMagic);
			return 1;
		case 391:
			this->MLS_SuddenIce(lpObj->m_Index, lpMagic, x, y, aTargetIndex);
			return 1;
		case 336:
			this->MLS_KnightBlow(lpObj->m_Index, aTargetIndex, lpMagic, bCombo);
			return 1;
		case 331:
		case 333:
			this->MLS_SkillBlowOfFury(lpObj->m_Index, lpMagic, x, y, aTargetIndex, bCombo);
			return 1;
		case 417:
			this->MLS_SkillDefense(lpObj->m_Index, aTargetIndex, lpMagic);
			return 1;
		case 420:
			this->MLS_SkillAttack(lpObj->m_Index, aTargetIndex, lpMagic);
			return 1;
		case 482:
			this->MLS_PowerSlash(lpObj->m_Index, lpMagic, x, y, aTargetPos, aTargetIndex);
			return 1;
		case 454:
			this->MLS_SkillSleep(lpObj->m_Index, aTargetIndex, lpMagic);
			return 1;
		case 455:
			this->MLS_SkillChainLightning(lpObj->m_Index, aTargetIndex, lpMagic);
			return 1;
		case 456:
			this->MLS_SkillLightningShock(lpObj->m_Index, aTargetIndex, lpMagic);
			return 1;
		case 458:
			this->MLS_SkillDrainLife(lpObj->m_Index, aTargetIndex, lpMagic);
			return 1;
		case 326:
			this->MLS_SkillSword4(lpObj, lpMagic, aTargetIndex, bCombo);
			return 1;
		case 479:
			this->MLS_SkillSword4_Magumsa(lpObj, lpMagic, aTargetIndex, 0);
			return 1;
		case 327:
			this->MLS_SkillSword5(lpObj, lpMagic, aTargetIndex, bCombo);
			return 1;
		case 328:
			this->MLS_SkillSword1(lpObj, lpMagic, aTargetIndex, bCombo);
			return 1;
		case 329:
			this->MLS_SkillSword2(lpObj, lpMagic, aTargetIndex, bCombo);
			return 1;
		case 337:
			this->MLS_SkillStrengthenDesruction(lpObj, lpMagic, x, y, aTargetIndex, bCombo);
			return 1;
		case 380:
			this->MLS_SkillStrengthenMagicPower(lpObj, lpMagic, aTargetIndex);
			return 1;
		case 383:
			this->MLS_SkillSkillfulMagicPower(lpObj, lpMagic, aTargetIndex);
			return 1;
		case 379:
		case 480:
			this->MLS_SkillStrengthenThunder(lpObj, lpMagic, aTargetIndex);
			return 1;
		case 384:
			this->MLS_SkillStrengthenPoison(lpObj, lpMagic, aTargetIndex);
			return 1;
		case 387:
			this->MLS_SkillStrengthenExPoison(lpObj->m_Index, lpMagic, x, y, aTargetIndex);
			return 1;
		case 389:
		case 489:
			this->MLS_SkillStrengthenIce(lpObj, lpMagic, aTargetIndex);
			return 1;
		case 404:
			this->MLS_SkillWizardMagicDefense(lpObj, lpMagic, aTargetIndex);
			return 1;
		case 422:
			this->MLS_SkillStrenghtenAttack(lpObj->m_Index, aTargetIndex, lpMagic);
			return 1;
		case 423:
			this->MLS_SkillStrengthenDefense(lpObj->m_Index, aTargetIndex, lpMagic);
			return 1;
		case 441:
			this->MLS_SkillInfinityArrow(lpObj->m_Index, aTargetIndex, lpMagic);
			return 1;
		case 469:
			this->MLS_SkillStrengthenBerserker(lpObj->m_Index, lpMagic);
			return 1;
		case 470:
			this->MLS_SkillSkillfulBerserker(lpObj->m_Index, lpMagic);
			return 1;
		case 472:
			this->MLS_SkillMasteryBerserker(lpObj->m_Index, lpMagic);
			return 1;
		case 511:
			this->MLS_SkillStrendthenAddCriticalDamage(lpObj->m_Index, lpMagic);
			return 1;
		case 515:
			this->MLS_SkillSkillFulAddCriticalDamage(lpObj->m_Index, lpMagic);
			return 1;
		case 517:
			this->MLS_SkillMasteryAddCriticalDamage(lpObj->m_Index, lpMagic);
			return 1;
		case 509:
			this->MLS_SkillStrengthenSpear(lpObj->m_Index, aTargetIndex, lpMagic);
			return 1;
		case 424:
			gObjAttack(lpObj, lpTargetObj, lpMagic, 1, 0, 0, 0, 0, 0);
			return 1;
		case 551:
		case 552:
		case 554:
		case 555:
		case 558:		
		case 562:
			this->MLS_SkillMonkBarrageJustOneTarget(lpObj->m_Index, lpMagic, aTargetIndex);
			return 1;
		case 559:
		case 563:
			gObjUseSkill.SkillDarkSide(lpObj->m_Index, aTargetIndex, lpMagic);
			return 1;
		case 560:
		case 561:
			this->MLS_SkillStrengthenDragonRoar(lpObj->m_Index, lpMagic, gObj[aTargetIndex].X, gObj[aTargetIndex].Y, aTargetIndex, 3, TRUE, 0);
			return 1;
		case 569:
			this->MLS_SkillStrengthenBlockRate(lpObj->m_Index, lpMagic);
			return 1;
		case 572:
			this->MLS_SkillMasteryBlockRate(lpObj->m_Index, lpMagic);
			return 1;
		case 573:
			this->MLS_SkillStrengthenIncVital(lpObj->m_Index, lpMagic);
			return 1;
		case 575:
			this->MLS_SkillStrengthenIgnoreEnemyDefense(lpObj->m_Index, lpMagic);
			return 1;
		case 344:
		case 346:
			this->MLS_SkillBloodStorm(lpObj, lpMagic, x, y, aTargetIndex, bCombo);
			return 1;
		case 495:
		case 497:
			this->MLS_SkillEarthPrison(lpObj, aTargetIndex, lpMagic);
			return 1;
		case 427:
		case 434:
			this->MLS_SkillPoisonArrow(lpObj, lpMagic, aTargetIndex, bCombo);
			return 1;
		case 425:
			this->MLS_SkillCure(lpObj, lpMagic, aTargetIndex);
			return 1;
		case 432:
			this->MLS_SkillMonsterCall(lpObj->m_Index, 597, lpObj->X-1, lpObj->Y+1);
			return 1;
		case 426:
		case 429:
			this->MLS_SkillPartyHealing(lpObj->m_Index, aTargetIndex, lpMagic);
			return 1;
		case 430:
			this->MLS_SkillBless(lpObj, aTargetIndex, lpMagic);
			return 1;
		case 433:
			this->MLS_SkillStrengthenBless(lpObj, aTargetIndex, lpMagic);
			return 1;
		case 461:
		case 463:
			this->MLS_SkillBlind(lpObj, aTargetIndex, lpMagic);
			return 1;
		case 521:
			this->MLS_SkillIronDefense(lpObj, aTargetIndex, lpMagic);
			return 1;
		case 565:
			this->MLS_SkillBloodHowling(lpObj, aTargetIndex, lpMagic);
			return 1;
		case 339:
			this->MLS_SkillfulKnightBlow(lpObj->m_Index, aTargetIndex, lpMagic, bCombo);
			return 1;
		case 342:
			this->MLS_MasteryKnightBlow(lpObj->m_Index, aTargetIndex, lpMagic, bCombo);
			return 1;
		case 340:
			this->MLS_SkillSkillfulDesruction(lpObj, lpMagic, x, y, aTargetIndex, bCombo);
			return 1;
		case 343:
			this->MLS_SkillMasteryDesruction(lpObj, lpMagic, x, y, aTargetIndex, bCombo);
			return 1;
		case 393:
			this->MLS_MasterySuddenIce(lpObj->m_Index, lpMagic, x, y, aTargetIndex);
			return 1;
		case 392:
			this->MLS_SkillHellBust(lpObj->m_Index, aTargetIndex, lpMagic);
			return 1;
		case 390:
		case 394:
			gObjAttack(lpObj, lpTargetObj, lpMagic, 1, 0, 0, 0, 0, 0);
			return 1;
		case 411:
		case 431:
			this->MLS_SkillStrengthenMultiShot(lpObj->m_Index, lpMagic, aTargetPos, aTargetIndex);
			return 1;
		case 363:
			this->MLS_MasteryKnightSkillAddLife(lpObj, lpMagic);
			break;
		case 406:
			this->MLS_SkillMasteryWizardMagicDefense(lpObj, lpMagic, aTargetIndex);
			break;
		case 459:
			this->MLS_SkillStrengthenWeakness(lpObj->m_Index, lpMagic, x, y, aTargetIndex);
			return 1;
		case 460:
			this->MLS_SkillStrengthenEnervation(lpObj->m_Index, lpMagic, x, y, aTargetIndex);
			return 1;
		case 462:
			this->MLS_SkillDrainLife(lpObj->m_Index, aTargetIndex, lpMagic);
			return 1;
		case 491:
			this->MLS_SkillMasteryIce(lpObj, lpMagic, aTargetIndex);
			return 1;
		case 492:
		case 494:
			this->MLS_SkillStrengthenFlameStrike(lpObj->m_Index, lpMagic, aTargetPos, aTargetIndex);
			return 1;
		case 496:
			this->MLS_SkillGiganticStorm(lpObj->m_Index, lpMagic, x, y, aTargetIndex);
			return 1;
		case 519:
			this->MLS_SkillStrengthenElectricSpark(lpObj->m_Index, lpMagic, x, y, aTargetPos, aTargetIndex);
			return 1;
		case 522:
			this->MLS_SkillMasteryAddCriticalDamage2(lpObj->m_Index, lpMagic);
			return 1;
		case 523:
			this->MLS_SkillStrengthenchaoticDesair(lpObj->m_Index, lpMagic, aTargetPos, aTargetIndex);
			return 1;
		case 524:
			this->MLS_SkillStrengthenIronDefense(lpObj, aTargetIndex, lpMagic);
			return 1;
		case 564:
		case 566:
			gObjAttack(lpObj, lpTargetObj, lpMagic, 1, 0, 0, 0, 0, 0);
			return 1;
		case 567:
			this->MLS_SkillStrengthenBloodHowling(lpObj, aTargetIndex, lpMagic);
			return 1;
		default:
			return 1;
	}

	return TRUE;
}

void CMasterLevelSkillTreeSystem::MLS_WizardMagicDefense(LPOBJ lpObj, int aTargetIndex, CMagicInf* lpMagic)
{
	LPOBJ lpTargetObj = &gObj[aTargetIndex];

	if ( lpObj->Type != OBJ_USER )
	{
		return;
	}

	if ( lpObj->m_RecallMon != -1 )
	{
		return;
	}

	if ( lpObj->Class != CLASS_WIZARD && lpObj->Class != CLASS_MAGUMSA )
	{
		return;
	}

	if ( lpObj->PartyNumber != lpTargetObj->PartyNumber )
	{
		return;
	}

	double fEffectValue = 0.0;
	int iSkillTime = 0;

	this->m_Lua.Generic_Call("WizardMagicDefense_Level1", "iiii>di", lpObj->m_Index, aTargetIndex, lpObj->m_PlayerData->Dexterity + lpObj->AddDexterity, lpObj->AddEnergy + lpObj->m_PlayerData->Energy, &fEffectValue, &iSkillTime);
	
	float fSkillValue = this->GetMasterSkillValue(MagicDamageC.SkillGet(lpMagic->m_Skill), lpMagic->m_Level);

	gObjAddBuffEffect(lpTargetObj, BUFFTYPE_MELEE_DEFENSE_INC, EFFECTTYPE_IMPROVE_MELEE_DEFENSE, fEffectValue + fSkillValue, EFFECTTYPE_MELEE_DEFENSE_DOWN_MANA, g_SkillAdditionInfo.GetSoulBarrierManaRate_Level(lpMagic->m_Level), iSkillTime);
	GSProtocol.GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, aTargetIndex, TRUE);
}

void CMasterLevelSkillTreeSystem::MLS_Healing(LPOBJ lpObj, int aTargetIndex, CMagicInf* lpMagic)
{
	LPOBJ lpTargetObj = &gObj[aTargetIndex];

	if ( lpTargetObj->Type != OBJ_USER && lpTargetObj->m_RecallMon == -1 )
	{
		return;
	}

	if ( lpObj->Class != CLASS_ELF )
	{
		return;
	}

	if ((CC_MAP_RANGE(lpObj->MapNumber) || lpObj->MapNumber == MAP_INDEX_CHAOSCASTLE_SURVIVAL) && lpObj->m_Index != lpTargetObj->m_Index)
	{
		return;
	}

	double fAddLife = 0.0;

	this->m_Lua.Generic_Call("ElfHeal", "iiii>d", lpTargetObj->Class, lpObj->m_Index, aTargetIndex, lpObj->m_PlayerData->Energy + lpObj->AddEnergy, &fAddLife);

	float fSkillValue = this->GetMasterSkillValue(MagicDamageC.SkillGet(lpMagic->m_Skill), lpMagic->m_Level);

	if ( fSkillValue > 0.0 )
	{
		fAddLife = ((fAddLife * fSkillValue) / 100.0) + fAddLife;
	}

	lpTargetObj->Life += fAddLife;

	if ( ( lpTargetObj->MaxLife + lpTargetObj->AddLife ) < lpTargetObj->Life )
	{
		lpTargetObj->Life = lpTargetObj->MaxLife + lpTargetObj->AddLife;
	}

	if ( lpTargetObj->Type == OBJ_USER )
	{
		GSProtocol.GCReFillSend(lpTargetObj->m_Index, lpTargetObj->Life, 0xFF, 0, lpTargetObj->iShield);
		GSProtocol.GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, aTargetIndex, TRUE);

		for (int n = 0; n < MAX_VIEWPORT; n++)
		{
			if ( lpObj->VpPlayer2[n].type == OBJ_MONSTER && lpObj->VpPlayer2[n].state != 0 )
			{
				LPOBJ lpMonster = &gObj[lpObj->VpPlayer2[n].number];

				if ( lpMonster->m_iCurrentAI != 0 )
				{
					lpMonster->m_Agro->IncAgro(lpObj->m_Index,(lpObj->m_PlayerData->Energy / 5)/40);
				}
			}
		}
	}

	else if ( lpTargetObj->m_RecallMon >= 0 )
	{
		GSProtocol.GCRecallMonLife(lpTargetObj->m_RecallMon,lpTargetObj->MaxLife,lpTargetObj->Life);
	}
}

void CMasterLevelSkillTreeSystem::MLS_Wheel(LPOBJ lpObj, CMagicInf *lpMagic, int aTargetIndex, BOOL isCombo)
{
	int count = 0;
	int DuelIndex = lpObj->m_iDuelUser;
	int tObjNum;
	BOOL EnableAttack;

	while ( true )
	{
		if ( lpObj->VpPlayer2[count].state != 0 )
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if ( tObjNum >= 0 )
			{
				EnableAttack = FALSE;

				if ( lpObj->VpPlayer2[count].type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon < 0 )
				{
					EnableAttack = TRUE;
				}

				else if ( tObjNum == aTargetIndex || DuelIndex == tObjNum )
				{
					EnableAttack = TRUE;
				}

				else if ( g_ConfigRead.server.GetServerType() == SERVER_CASTLE && lpObj->VpPlayer2[count].type != OBJ_NPC && gObj[tObjNum].MapNumber == MAP_INDEX_CASTLESIEGE && g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE )
				{
					EnableAttack = TRUE;
				}

				else
				{
					int CallMonIndex = gObj[tObjNum].m_Index;

					if ( gObj[tObjNum].Type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon >= 0 )
					{
						CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
					}

					if ( gObjTargetGuildWarCheck(lpObj,&gObj[CallMonIndex]) == TRUE )
					{
						EnableAttack = TRUE;
					}

					if ( gObj[tObjNum].Class >= 678 && gObj[tObjNum].Class <= 680 )
					{
						EnableAttack = TRUE;
					}
				}

				if ( EnableAttack == TRUE )
				{
					if ( gObjCalDistance(lpObj, &gObj[tObjNum]) <= MagicDamageC.GetSkillDistance(lpMagic->m_Skill) )
					{
						gObjAttack(lpObj, &gObj[tObjNum], lpMagic, FALSE, TRUE, FALSE, isCombo, FALSE, FALSE);
					}
				}
			}
		}

		count++;

		if ( count > MAX_VIEWPORT-1 )
			break;
	}
}

void CMasterLevelSkillTreeSystem::MLS_SkillSword4(LPOBJ lpObj, CMagicInf *lpMagic, int aTargetIndex, BOOL isCombo)
{
	LPOBJ lpTargetObj = &gObj[aTargetIndex];

	if ( gObjIsConnected(lpObj->m_Index) == FALSE && gObjIsConnected(lpTargetObj->m_Index) == FALSE )
	{
		return;
	}

	if ( lpObj->Type != OBJ_USER || lpObj->Class != CLASS_KNIGHT )
	{
		return;
	}

	gObjAttack(lpObj, lpTargetObj, lpMagic, 0, 1, 0, isCombo, 0, 0);
}

void CMasterLevelSkillTreeSystem::MLS_SkillSword4_Magumsa(LPOBJ lpObj, CMagicInf *lpMagic, int aTargetIndex, BOOL isCombo)
{
	LPOBJ lpTargetObj = &gObj[aTargetIndex];

	if ( gObjIsConnected(lpObj->m_Index) == FALSE && gObjIsConnected(lpTargetObj->m_Index) == FALSE )
	{
		return;
	}

	if ( lpObj->Type != OBJ_USER || lpObj->Class != CLASS_MAGUMSA )
	{
		return;
	}

	gObjAttack(lpObj, lpTargetObj, lpMagic, 0, 1, 0, isCombo, 0, 0);
}

void CMasterLevelSkillTreeSystem::MLS_SkillSword5(LPOBJ lpObj, CMagicInf *lpMagic, int aTargetIndex, BOOL isCombo)
{
	LPOBJ lpTargetObj = &gObj[aTargetIndex];

	if ( gObjIsConnected(lpObj->m_Index) == FALSE && gObjIsConnected(lpTargetObj->m_Index) == FALSE )
	{
		return;
	}

	if ( lpObj->Type != OBJ_USER || lpObj->Class != CLASS_KNIGHT )
	{
		return;
	}

	gObjAttack(lpObj, lpTargetObj, lpMagic, 0, 1, 0, isCombo, 0, 0);
}

void CMasterLevelSkillTreeSystem::MLS_SkillSword1(LPOBJ lpObj, CMagicInf *lpMagic, int aTargetIndex, BOOL isCombo)
{
	LPOBJ lpTargetObj = &gObj[aTargetIndex];

	if ( gObjIsConnected(lpObj->m_Index) == FALSE && gObjIsConnected(lpTargetObj->m_Index) == FALSE )
	{
		return;
	}

	if ( lpObj->Type != OBJ_USER || lpObj->Class != CLASS_KNIGHT )
	{
		return;
	}

	gObjAttack(lpObj, lpTargetObj, lpMagic, 0, 1, 0, isCombo, 0, 0);
}

void CMasterLevelSkillTreeSystem::MLS_SkillSword2(LPOBJ lpObj, CMagicInf *lpMagic, int aTargetIndex, BOOL isCombo)
{
	LPOBJ lpTargetObj = &gObj[aTargetIndex];

	if ( gObjIsConnected(lpObj->m_Index) == FALSE && gObjIsConnected(lpTargetObj->m_Index) == FALSE )
	{
		return;
	}

	if ( lpObj->Type != OBJ_USER || lpObj->Class != CLASS_KNIGHT )
	{
		return;
	}

	gObjAttack(lpObj, lpTargetObj, lpMagic, 0, 1, 0, isCombo, 0, 0);
}

void CMasterLevelSkillTreeSystem::MLS_SkillStrengthenDesruction(LPOBJ lpObj, CMagicInf *lpMagic, BYTE x, BYTE y, int aTargetIndex, int isCombo)
{
	int count = 0;
	int DuelIndex = lpObj->m_iDuelUser;
	int nHitCount = 0;
	BOOL bAttack = FALSE;
	BOOL EnableAttack;
	int tObjNum;

	if ( this->MLS_CalDistance(lpObj->X, lpObj->Y, x, y) <= MagicDamageC.GetSkillDistance(lpMagic->m_Skill) )
	{
		while ( true )
		{
			if ( lpObj->VpPlayer2[count].state != 0 )
			{
				tObjNum = lpObj->VpPlayer2[count].number;

				if ( tObjNum >= 0 )
				{
					EnableAttack = FALSE;

					if ( lpObj->VpPlayer2[count].type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon < 0 )
					{
						EnableAttack = TRUE;
					}

					else if ( tObjNum == aTargetIndex || DuelIndex == tObjNum )
					{
						EnableAttack = TRUE;
					}

					else if ( g_ConfigRead.server.GetServerType() == SERVER_CASTLE && lpObj->VpPlayer2[count].type != OBJ_NPC && gObj[tObjNum].MapNumber == MAP_INDEX_CASTLESIEGE && g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE )
					{
						EnableAttack = TRUE;
					}

					else
					{
						int CallMonIndex = gObj[tObjNum].m_Index;

						if ( gObj[tObjNum].Type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon >= 0 )
						{
							CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
						}

						if ( gObjTargetGuildWarCheck(lpObj,&gObj[CallMonIndex]) == TRUE )
						{
							EnableAttack = TRUE;
						}

						if ( gObj[tObjNum].Class >= 678 && gObj[tObjNum].Class <= 680 )
						{
							EnableAttack = TRUE;
						}
					}

					if ( EnableAttack == TRUE )
					{
						if ( gObj[tObjNum].X >= x - 3 
							&& gObj[tObjNum].X <= x + 3
							&& gObj[tObjNum].Y >= y - 3
							&& gObj[tObjNum].Y <= y + 3 )
						{
							gObj[tObjNum].lpAttackObj = lpObj;
							nHitCount++;

							if ( nHitCount <= 4 )
							{
								bAttack = TRUE;
							}

							else if ( rand()%2 )
							{
								bAttack = TRUE;
							}

							if ( nHitCount > 8 )
							{
								bAttack = FALSE;
							}

							if ( bAttack == TRUE )
							{
								gObjAddAttackProcMsgSendDelay(lpObj, 50, tObjNum, 500, lpMagic->m_Skill, 0);
							}
						}
					}
				}
			}

			count++;

			if ( count > MAX_VIEWPORT-1 ) 
				break;
		}
	}
}

void CMasterLevelSkillTreeSystem::MLS_KnightSkillAddLife(LPOBJ lpObj, CMagicInf * lpMagic)
{
	if(lpObj->Type != OBJ_USER && lpObj->m_RecallMon == -1)
	{
		return;
	}

	if(lpObj->Class != CLASS_KNIGHT && lpObj->Class != CLASS_MAGUMSA)
	{
		return;
	}

	int number;
	int partynum = 0;
	int totallevel = 0;
	int partycount;
	int dis;

	int ApplyPartyIndex[MAX_USER_IN_PARTY];

	int viewplayer = 0;
	int viewpercent = 0;

	LPOBJ lpPartyObj;

	memset(ApplyPartyIndex,-1,sizeof(ApplyPartyIndex));

	partynum = lpObj->PartyNumber;

	if(partynum != -1)
	{
		partycount = gParty.m_PartyS[partynum].Count;

		for(int n = 0; n < MAX_USER_IN_PARTY; n++)
		{
			number = gParty.m_PartyS[partynum].Number[n];

			if(number >= 0)
			{
				lpPartyObj = &gObj[number];

				if(lpObj->MapNumber == lpPartyObj->MapNumber)
				{
					dis = gObjCalDistance(lpObj,&gObj[number]);

					if(dis < 10)
					{
						ApplyPartyIndex[n] = lpPartyObj->m_Index;
						viewplayer++;
					}
				}
			}
		}
	}

	if(viewplayer > 1)
	{
		if(viewplayer == 2)
		{
			viewpercent = 1;
		}
		else if(viewplayer == 3)
		{
			viewpercent = 2;
		}
		else if(viewplayer == 4)
		{
			viewpercent = 3;
		}
		else
		{
			viewpercent = 4;
		}
	}

	double fAddValue = this->GetMasterSkillValue(MagicDamageC.SkillGet(lpMagic->m_Skill), lpMagic->m_Level);
	double faddLifepower = 0.0;
	int iLifeTime = 0;

	this->m_Lua.Generic_Call("KnightSkillAddLife_Level1", "iii>di", lpObj->m_PlayerData->Vitality + lpObj->AddVitality, lpObj->m_PlayerData->Energy + lpObj->AddEnergy, viewpercent, &faddLifepower, &iLifeTime);

	faddLifepower += fAddValue;

	if ( partynum == -1 )
	{
		gObjAddBuffEffect(lpObj, BUFFTYPE_HP_INC, EFFECTTYPE_HP, (lpObj->MaxLife * faddLifepower / 100.0), 0, 0, iLifeTime);
		GSProtocol.GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, lpObj->m_Index, TRUE);
	}

	else
	{
		for ( int i=0;i<MAX_USER_IN_PARTY;i++ )
		{
			if ( ApplyPartyIndex[i] != -1 )
			{
				gObjAddBuffEffect(&gObj[ApplyPartyIndex[i]], BUFFTYPE_HP_INC, EFFECTTYPE_HP, (gObj[ApplyPartyIndex[i]].MaxLife * faddLifepower / 100.0), 0, 0, iLifeTime);
				GSProtocol.GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, ApplyPartyIndex[i], TRUE);
			}
		}
	}
}

void CMasterLevelSkillTreeSystem::MLS_SkillFulKnightSkillAddLife(LPOBJ lpObj, CMagicInf * lpMagic)
{
	if(lpObj->Type != OBJ_USER && lpObj->m_RecallMon == -1)
	{
		return;
	}

	if(lpObj->Class != CLASS_KNIGHT && lpObj->Class != CLASS_MAGUMSA)
	{
		return;
	}

	int number;
	int partynum = 0;
	int totallevel = 0;
	int partycount;
	int dis;

	int ApplyPartyIndex[MAX_USER_IN_PARTY];

	int viewplayer = 0;
	int viewpercent = 0;

	LPOBJ lpPartyObj;

	memset(ApplyPartyIndex,-1,sizeof(ApplyPartyIndex));

	partynum = lpObj->PartyNumber;

	if(partynum != -1)
	{
		partycount = gParty.m_PartyS[partynum].Count;

		for(int n = 0; n < MAX_USER_IN_PARTY; n++)
		{
			number = gParty.m_PartyS[partynum].Number[n];

			if(number >= 0)
			{
				lpPartyObj = &gObj[number];

				if(lpObj->MapNumber == lpPartyObj->MapNumber)
				{
					dis = gObjCalDistance(lpObj,&gObj[number]);

					if(dis < 10)
					{
						ApplyPartyIndex[n] = lpPartyObj->m_Index;
						viewplayer++;
					}
				}
			}
		}
	}

	if(viewplayer > 1)
	{
		if(viewplayer == 2)
		{
			viewpercent = 1;
		}
		else if(viewplayer == 3)
		{
			viewpercent = 2;
		}
		else if(viewplayer == 4)
		{
			viewpercent = 3;
		}
		else
		{
			viewpercent = 4;
		}
	}

	double faddLifepower = 0.0;
	int iLifeTime = 0;
	int iPreSkill = this->GetParentSkill1(lpObj->Class, lpMagic->m_Skill);

	if ( iPreSkill <= 0 )
	{
		this->m_Lua.Generic_Call("KnightSkillAddLife_Level2", "iii>di", lpObj->m_PlayerData->Vitality + lpObj->AddVitality, lpObj->m_PlayerData->Energy + lpObj->AddEnergy, viewpercent, &faddLifepower, &iLifeTime);
	}

	else
	{
		int iPreLevel = this->GetCurSkillLevel(lpObj, iPreSkill);

		if ( iPreLevel > 0 )
		{
			this->m_Lua.Generic_Call("KnightSkillAddLife_Level2", "iii>di", lpObj->m_PlayerData->Vitality + lpObj->AddVitality, lpObj->m_PlayerData->Energy + lpObj->AddEnergy, viewpercent, &faddLifepower, &iLifeTime);
			faddLifepower += this->GetMasterSkillValue(MagicDamageC.SkillGet(iPreSkill), iPreLevel);
		}
	}

	double faddManapower = 0.0;

	if  ( lpMagic->m_Skill == 360 )
	{
		faddManapower = viewpercent + this->GetMasterSkillValue(MagicDamageC.SkillGet(lpMagic->m_Skill), lpMagic->m_Level);
	}

	if ( partynum == -1 )
	{
		gObjAddBuffEffect(lpObj, BUFFTYPE_HP_INC_STR, EFFECTTYPE_HP, (lpObj->MaxLife * faddLifepower / 100.0), EFFECTTYPE_MANA, (lpObj->MaxMana * faddManapower / 100.0), iLifeTime);
		GSProtocol.GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, lpObj->m_Index, TRUE);
	}

	else
	{
		for ( int i=0;i<MAX_USER_IN_PARTY;i++ )
		{
			if ( ApplyPartyIndex[i] != -1 )
			{
				gObjAddBuffEffect(&gObj[ApplyPartyIndex[i]], BUFFTYPE_HP_INC_STR, EFFECTTYPE_HP, (gObj[ApplyPartyIndex[i]].MaxLife * faddLifepower / 100.0), EFFECTTYPE_MANA, (gObj[ApplyPartyIndex[i]].MaxMana * faddManapower / 100.0), iLifeTime);
				GSProtocol.GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, ApplyPartyIndex[i], TRUE);
			}
		}
	}
}

void CMasterLevelSkillTreeSystem::MLS_MasteryKnightSkillAddLife(LPOBJ lpObj, CMagicInf * lpMagic)
{
	if(lpObj->Type != OBJ_USER && lpObj->m_RecallMon == -1)
	{
		return;
	}

	if(lpObj->Class != CLASS_KNIGHT && lpObj->Class != CLASS_MAGUMSA)
	{
		return;
	}

	int number;
	int partynum = 0;
	int totallevel = 0;
	int partycount;
	int dis;

	int ApplyPartyIndex[MAX_USER_IN_PARTY];

	int viewplayer = 0;
	int viewpercent = 0;

	LPOBJ lpPartyObj;

	memset(ApplyPartyIndex,-1,sizeof(ApplyPartyIndex));

	partynum = lpObj->PartyNumber;

	if(partynum != -1)
	{
		partycount = gParty.m_PartyS[partynum].Count;

		for(int n = 0; n < MAX_USER_IN_PARTY; n++)
		{
			number = gParty.m_PartyS[partynum].Number[n];

			if(number >= 0)
			{
				lpPartyObj = &gObj[number];

				if(lpObj->MapNumber == lpPartyObj->MapNumber)
				{
					dis = gObjCalDistance(lpObj,&gObj[number]);

					if(dis < 10)
					{
						ApplyPartyIndex[n] = lpPartyObj->m_Index;
						viewplayer++;
					}
				}
			}
		}
	}

	if(viewplayer > 1)
	{
		if(viewplayer == 2)
		{
			viewpercent = 1;
		}
		else if(viewplayer == 3)
		{
			viewpercent = 2;
		}
		else if(viewplayer == 4)
		{
			viewpercent = 3;
		}
		else
		{
			viewpercent = 4;
		}
	}

	double faddLifepower = 0.0;
	double faddManapower = 0.0;
	int iLifeTime = 0;

	int iPreSkill = this->GetParentSkill1(lpObj->Class, lpMagic->m_Skill);

	if ( iPreSkill <= 0 )
	{
		faddManapower = viewpercent;
	}

	else
	{
		int iPrePreSkill = this->GetParentSkill1(lpObj->Class, iPreSkill);

		if ( iPrePreSkill <= 0 )
		{
			this->m_Lua.Generic_Call("KnightSkillAddLife_Level3", "iii>di", lpObj->m_PlayerData->Vitality + lpObj->AddVitality, lpObj->m_PlayerData->Energy + lpObj->AddEnergy, viewpercent, &faddLifepower, &iLifeTime);
		}

		else
		{
			int iPrePreLevel = this->GetCurSkillLevel(lpObj, iPrePreSkill);

			if ( iPrePreLevel > 0 )
			{
				this->m_Lua.Generic_Call("KnightSkillAddLife_Level3", "iii>di", lpObj->m_PlayerData->Vitality + lpObj->AddVitality, lpObj->m_PlayerData->Energy + lpObj->AddEnergy, viewpercent, &faddLifepower, &iLifeTime);
				faddLifepower += this->GetMasterSkillValue(MagicDamageC.SkillGet(iPrePreSkill), iPrePreLevel);
			}

			else
			{
				this->m_Lua.Generic_Call("KnightSkillAddLife_Level3", "iii>di", lpObj->m_PlayerData->Vitality + lpObj->AddVitality, lpObj->m_PlayerData->Energy + lpObj->AddEnergy, viewpercent, &faddLifepower, &iLifeTime);
			}
		}

		int iPreLevel = this->GetCurSkillLevel(lpObj, iPreSkill);

		if ( iPreLevel > 0 )
		{
			faddManapower = viewpercent + this->GetMasterSkillValue(MagicDamageC.SkillGet(iPreSkill), iPreLevel);
		}
	}

	if ( partynum == -1 )
	{
		lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncMaxBP = this->GetMasterSkillValue(MagicDamageC.SkillGet(lpMagic->m_Skill), lpMagic->m_Level);
		gObjAddBuffEffect(lpObj, BUFFTYPE_HP_INC_STR, EFFECTTYPE_HP, (lpObj->MaxLife * faddLifepower / 100.0), EFFECTTYPE_MANA, (lpObj->MaxMana * faddManapower / 100.0), iLifeTime);
		GSProtocol.GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, lpObj->m_Index, TRUE);
	}

	else
	{
		for ( int i=0;i<MAX_USER_IN_PARTY;i++ )
		{
			if ( ApplyPartyIndex[i] != -1 )
			{
				gObj[ApplyPartyIndex[i]].m_PlayerData->m_MPSkillOpt.iMpsIncMaxBP = this->GetMasterSkillValue(MagicDamageC.SkillGet(lpMagic->m_Skill), lpMagic->m_Level);
				gObjAddBuffEffect(&gObj[ApplyPartyIndex[i]], BUFFTYPE_HP_INC_STR, EFFECTTYPE_HP, (gObj[ApplyPartyIndex[i]].MaxLife * faddLifepower / 100.0), EFFECTTYPE_MANA, (gObj[ApplyPartyIndex[i]].MaxMana * faddManapower / 100.0), iLifeTime);
				GSProtocol.GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, ApplyPartyIndex[i], TRUE);
			}
		}
	}
}

void CMasterLevelSkillTreeSystem::MLS_SkillStrengthenMagicPower(LPOBJ lpObj, CMagicInf* lpMagic, int aTargetIndex)
{
	if ( lpObj->Type != OBJ_USER )
	{
		return;
	}

	if ( lpObj->Class != CLASS_WIZARD )
	{
		return;
	}

	if ( gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_MAGIC_POWER_INC_STR) == TRUE )
	{
		return;
	}

	double SkillEffect = 0.0;
	int Time = 0;

	gObjAddBuffEffect(lpObj, BUFFTYPE_MAGIC_POWER_INC_STR, EFFECTTYPE_MAGICPOWER_INC, ( (lpObj->m_PlayerData->Energy+lpObj->AddEnergy) / 9.0 * 0.20 ),
		EFFECTTYPE_MAGICPOWERMAX_INC, (lpObj->m_MagicDamageMax / 100.0 * this->GetMasterSkillValue(MagicDamageC.SkillGet(lpMagic->m_Skill), lpMagic->m_Level)), 1800);

	GSProtocol.GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, lpObj->m_Index, TRUE);
}

void CMasterLevelSkillTreeSystem::MLS_SkillSkillfulMagicPower(LPOBJ lpObj, CMagicInf* lpMagic, int aTargetIndex)
{
	if ( lpObj->Type != OBJ_USER )
	{
		return;
	}

	if ( lpObj->Class != CLASS_WIZARD )
	{
		return;
	}

	if ( gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_MAGIC_POWER_INC_MAS) == TRUE )
	{
		return;
	}

	lpObj->m_PlayerData->m_MPSkillOpt.iMpsCriticalRateInc = this->GetMasterSkillValue(MagicDamageC.SkillGet(lpMagic->m_Skill), lpMagic->m_Level);
	gObjAddBuffEffect(lpObj, BUFFTYPE_MAGIC_POWER_INC_MAS, EFFECTTYPE_MAGICPOWER_INC, ( (lpObj->m_PlayerData->Energy+lpObj->AddEnergy) / 9.0 * 0.20 ),
		EFFECTTYPE_MAGICPOWERMAX_INC, (lpObj->m_MagicDamageMax / 100.0 * this->GetBrandOfMasterSkillValue(lpObj, lpMagic->m_Skill, 1)), 1800);

	GSProtocol.GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, lpObj->m_Index, TRUE);
}

void CMasterLevelSkillTreeSystem::MLS_SkillStrengthenFlame(OBJECTSTRUCT *lpObj, CMagicInf *lpMagic, int aTargetIndex)
{
	if ( lpObj->Type != OBJ_USER )
	{
		return;
	}

	gObjAttack(lpObj, &gObj[aTargetIndex], lpMagic, TRUE, 0, 0, 0, 0, 0);
}

void CMasterLevelSkillTreeSystem::MLS_SkillStrengthenThunder(OBJECTSTRUCT *lpObj, CMagicInf *lpMagic, int aTargetIndex)
{
	if ( lpObj->Type != OBJ_USER )
	{
		return;
	}

	gObjAttack(lpObj, &gObj[aTargetIndex], lpMagic, TRUE, 0, 0, 0, 0, 0);
}

void CMasterLevelSkillTreeSystem::MLS_SkillStrengthenInferno(LPOBJ lpObj, CMagicInf *lpMagic)
{
	return;
}

void CMasterLevelSkillTreeSystem::MLS_SkillStrengthenHellFire(OBJECTSTRUCT *lpObj, CMagicInf *lpMagic)
{
	int count = 0;
	GSProtocol.GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, lpObj->m_Index, TRUE);

	while ( true )
	{
		if ( lpObj->VpPlayer2[count].state ) 
		{
			if ( lpObj->VpPlayer2[count].type == OBJ_MONSTER )
			{
				int tObjNum = lpObj->VpPlayer2[count].number;

				if ( tObjNum >= 0 )
				{
					if ( gObjCalDistance(lpObj, &gObj[tObjNum]) < 4 )
					{
						gObjAddAttackProcMsgSendDelay(lpObj, 50, tObjNum, 200, lpMagic->m_Skill, 0);
					}
				}
			}
		}

		count++;

		if ( count > MAX_VIEWPORT-1 )
		{
			break;
		}
	}
}

void CMasterLevelSkillTreeSystem::MLS_SkillStrengthenPoison(OBJECTSTRUCT *lpObj, CMagicInf *lpMagic, int aTargetIndex)
{
	if ( lpObj->Type != OBJ_USER )
	{
		return;
	}

	gObjAttack(lpObj, &gObj[aTargetIndex], lpMagic, TRUE, 0, 0, 0, 0, 0);
}

void CMasterLevelSkillTreeSystem::MLS_SkillStrengthenExPoison(int aIndex, CMagicInf *lpMagic, BYTE x, BYTE y, int aTargetIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	if ( lpObj->Type != OBJ_USER )
	{
		return;
	}
	
	int count = 0;
	int DuelIndex = lpObj->m_iDuelUser;
	int tObjNum;
	BOOL EnableAttack;

	while ( true )
	{
		if ( lpObj->VpPlayer2[count].state != 0 )
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if ( tObjNum >= 0 )
			{
				EnableAttack = FALSE;

				if ( lpObj->VpPlayer2[count].type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon < 0 )
				{
					EnableAttack = TRUE;
				}

				else if ( tObjNum == aTargetIndex || DuelIndex == tObjNum )
				{
					EnableAttack = TRUE;
				}

				else if ( g_ConfigRead.server.GetServerType() == SERVER_CASTLE && lpObj->VpPlayer2[count].type != OBJ_NPC && gObj[tObjNum].MapNumber == MAP_INDEX_CASTLESIEGE && g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE )
				{
					EnableAttack = TRUE;
				}

				else
				{
					int CallMonIndex = gObj[tObjNum].m_Index;

					if ( gObj[tObjNum].Type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon >= 0 )
					{
						CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
					}

					if ( gObjTargetGuildWarCheck(lpObj,&gObj[CallMonIndex]) == TRUE )
					{
						EnableAttack = TRUE;
					}

					if ( gObj[tObjNum].Class >= 678 && gObj[tObjNum].Class <= 680 )
					{
						EnableAttack = TRUE;
					}
				}

				if ( EnableAttack == TRUE )
				{
					if ( this->MLS_CalDistance(x, y, gObj[tObjNum].X, gObj[tObjNum].Y) < 4 )
					{
						gObjAddAttackProcMsgSendDelay(lpObj, 50, tObjNum, rand()%500, lpMagic->m_Skill, 0);
					}
				}
			}
		}

		count++;

		if ( count > MAX_VIEWPORT-1 )
			break;
	}
}

void CMasterLevelSkillTreeSystem::MLS_SkillStrengthenIce(OBJECTSTRUCT *lpObj, CMagicInf *lpMagic, int aTargetIndex)
{
	if ( lpObj->Type != OBJ_USER )
	{
		return;
	}

	gObjAttack(lpObj, &gObj[aTargetIndex], lpMagic, TRUE, 0, 0, 0, 0, 0);
}

void CMasterLevelSkillTreeSystem::MLS_SkillWizardMagicDefense(LPOBJ lpObj, CMagicInf *lpMagic, int aTargetIndex)
{
	LPOBJ lpTargetObj = &gObj[aTargetIndex];

	if ( lpObj->Type != OBJ_USER )
	{
		return;
	}

	if ( lpObj->m_RecallMon != -1 )
	{
		return;
	}

	if ( lpObj->Class != CLASS_WIZARD && lpObj->Class != CLASS_MAGUMSA )
	{
		return;
	}

	if ( lpObj->PartyNumber != lpTargetObj->PartyNumber )
	{
		return;
	}

	double fDefenseValue = 0.0;
	int iSkillTime = 0;

	int iPreSkill = this->GetParentSkill1(lpObj->Class, lpMagic->m_Skill);

	if ( iPreSkill <= 0 )
	{
		this->m_Lua.Generic_Call("WizardMagicDefense_Level2", "iiii>di", lpObj->m_Index, aTargetIndex, lpObj->AddDexterity + lpObj->m_PlayerData->Dexterity, lpObj->AddEnergy + lpObj->m_PlayerData->Energy, &fDefenseValue, &iSkillTime);
	}

	else
	{
		int iPreLevel = this->GetCurSkillLevel(lpObj, iPreSkill);

		if ( iPreLevel > 0 )
		{
			this->m_Lua.Generic_Call("WizardMagicDefense_Level2", "iiii>di", lpObj->m_Index, aTargetIndex, lpObj->AddDexterity + lpObj->m_PlayerData->Dexterity, lpObj->AddEnergy + lpObj->m_PlayerData->Energy, &fDefenseValue, &iSkillTime);
			fDefenseValue += this->GetMasterSkillValue(MagicDamageC.SkillGet(iPreSkill), iPreLevel);
		}
	}

	gObjAddBuffEffect(lpTargetObj, BUFFTYPE_MELEE_DEFENSE_INC, EFFECTTYPE_IMPROVE_MELEE_DEFENSE, fDefenseValue,
		EFFECTTYPE_MELEE_DEFENSE_DOWN_MANA,g_SkillAdditionInfo.GetSoulBarrierManaRate_Level(lpMagic->m_Level),
		iSkillTime + this->GetMasterSkillValue(MagicDamageC.SkillGet(lpMagic->m_Skill), lpMagic->m_Level));

	GSProtocol.GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, lpTargetObj->m_Index, TRUE);
}

void CMasterLevelSkillTreeSystem::MLS_SkillStrengthenPenetration(OBJECTSTRUCT *lpObj, CMagicInf *lpMagic, int aTargetIndex)
{
	if ( lpObj->Type != OBJ_USER )
	{
		return;
	}

	gObjAttack(lpObj, &gObj[aTargetIndex], lpMagic, TRUE, 0, 0, 0, 0, 0);
}

void CMasterLevelSkillTreeSystem::MLS_SkillInfinityArrow(int aIndex, int aTargetIndex, CMagicInf* lpMagic)
{
	LPOBJ lpObj = &gObj[aIndex];

	if ( lpObj->Type != OBJ_USER )
	{
		return;
	}

	if ( lpObj->Class != CLASS_ELF )
	{
		return;
	}

	if ( lpObj->m_PlayerData->ChangeUP == FALSE )
	{
		return;
	}

	if ( lpObj->m_Index != aTargetIndex )
	{
		return;
	}

	if ( gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_INFINITY_ARROW_STR) == TRUE )
	{
		return;
	}

	lpObj->m_SkillInfo.fInfinityArrowIncRate = this->GetMasterSkillValue(MagicDamageC.SkillGet(lpMagic->m_Skill), lpMagic->m_Level);
	gObjAddBuffEffect(lpObj, BUFFTYPE_INFINITY_ARROW_STR, 0, 0, 0, 0, -10);
	GSProtocol.GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, lpObj->m_Index, TRUE);

	g_Log.Add("[%s][%s] Use Infinity Arrow Skill (Time:%d)(Character Level : %d)(ChangeUp: %d)",
		lpObj->AccountID, lpObj->Name, g_SkillAdditionInfo.GetInfinityArrowSkillTime(), lpObj->Level, lpObj->m_PlayerData->ChangeUP);
}

void CMasterLevelSkillTreeSystem::MLS_SkillStrengthenBerserker(int aIndex, CMagicInf *lpMagic)
{
	if ( !ObjectMaxRange(aIndex) )
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	int iDuration = 0;
	int iEffectUPValue = 0;
	int iEffectDownValue = 0;

	if ( lpObj->Type == OBJ_USER )
	{
		this->m_Lua.Generic_Call("SummonerBerserker_Level1", "i>iii", lpObj->m_PlayerData->Energy + lpObj->AddEnergy, &iEffectUPValue, &iEffectDownValue, &iDuration);
		lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncPercentBerserker1 = this->GetMasterSkillValue(MagicDamageC.SkillGet(lpMagic->m_Skill), lpMagic->m_Level);
	}

	else
	{
		iDuration = 600000;
		iEffectUPValue = 0;
		iEffectDownValue = 0;
	}

	gObjAddBuffEffect(lpObj, BUFFTYPE_BERSERKER_STR, EFFECTTYPE_BERSERKER_UP, iEffectUPValue, EFFECTTYPE_BERSERKER_DOWN, iEffectDownValue, iDuration);
	GSProtocol.GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, aIndex, TRUE);
}

void CMasterLevelSkillTreeSystem::MLS_SkillSkillfulBerserker(int aIndex, CMagicInf *lpMagic)
{
	if ( !ObjectMaxRange(aIndex) )
	{
		return;
	}

	lua_Reader();
	LPOBJ lpObj = &gObj[aIndex];

	int iDuration = 0;
	int iEffectUPValue = 0;
	int iEffectDownValue = 0;

	if ( lpObj->Type == OBJ_USER )
	{
		this->m_Lua.Generic_Call("SummonerBerserker_Level2", "i>iii", lpObj->m_PlayerData->Energy + lpObj->AddEnergy, &iEffectUPValue, &iEffectDownValue, &iDuration);
		lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncPercentBerserker1 = this->GetBrandOfMasterSkillValue(lpObj, lpMagic->m_Skill, 1);
		lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncPercentBerserker2 = this->GetMasterSkillValue(MagicDamageC.SkillGet(lpMagic->m_Skill), lpMagic->m_Level);
	}

	else
	{
		iDuration = 600000;
		iEffectUPValue = 0;
		iEffectDownValue = 0;
	}

	gObjAddBuffEffect(lpObj, BUFFTYPE_BERSERKER_MAS, EFFECTTYPE_BERSERKER_UP, iEffectUPValue, EFFECTTYPE_BERSERKER_DOWN, iEffectDownValue, iDuration);
	GSProtocol.GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, aIndex, TRUE);
}

void CMasterLevelSkillTreeSystem::MLS_SkillMasteryBerserker(int aIndex, CMagicInf *lpMagic)
{
	if ( !ObjectMaxRange(aIndex) )
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	int iDuration = 0;
	int iEffectUPValue = 0;
	int iEffectDownValue = 0;

	if ( lpObj->Type == OBJ_USER )
	{
		this->m_Lua.Generic_Call("SummonerBerserker_Level3", "i>iii", lpObj->m_PlayerData->Energy + lpObj->AddEnergy, &iEffectUPValue, &iEffectDownValue, &iDuration);
		lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncPercentBerserker1 = this->GetBrandOfMasterSkillValue(lpObj, lpMagic->m_Skill, 2);
		lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncPercentBerserker2 = this->GetBrandOfMasterSkillValue(lpObj, lpMagic->m_Skill, 1);
		lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncValueBerserker3 = this->GetMasterSkillValue(MagicDamageC.SkillGet(lpMagic->m_Skill), lpMagic->m_Level);
	}

	else
	{
		iDuration = 600000;
		iEffectUPValue = 0;
		iEffectDownValue = 0;
	}

	gObjAddBuffEffect(lpObj, BUFFTYPE_BERSERKER_PRO, EFFECTTYPE_BERSERKER_UP, iEffectUPValue, EFFECTTYPE_BERSERKER_DOWN, iEffectDownValue, iDuration);
	GSProtocol.GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, aIndex, TRUE);
}

void CMasterLevelSkillTreeSystem::MLS_SkillStrengthenBlast(LPOBJ lpObj, CMagicInf* lpMagic, int aTargetIndex)
{
	if ( lpObj->Type = OBJ_USER )
	{
		return;
	}

	gObjAttack(lpObj, &gObj[aTargetIndex], lpMagic, TRUE, 0, 0, 0, 0, 0);
}

void CMasterLevelSkillTreeSystem::MLS_SkillStrendthenAddCriticalDamage(int aIndex, CMagicInf* lpMagic)
{
	LPOBJ lpObj = &gObj[aIndex];

	int number;
	int ApplyPartyIndex[MAX_USER_IN_PARTY];
	LPOBJ lpPartyObj;
	memset(ApplyPartyIndex,-1,sizeof(ApplyPartyIndex));
	
	int partynum = lpObj->PartyNumber;

	if(partynum != -1)
	{
		for(int n = 0; n < MAX_USER_IN_PARTY; n++)
		{
			number = gParty.m_PartyS[partynum].Number[n];

			if(number >= 0)
			{
				lpPartyObj = &gObj[number];

				if(lpObj->MapNumber == lpPartyObj->MapNumber)
				{
					if(gObjCalDistance(lpObj,&gObj[number]) < 10)
					{
						ApplyPartyIndex[n] = lpPartyObj->m_Index;
					}
				}
			}
		}
	}

	int addcriticaldamagevalue = 0;
	int SkillTime = 0;

	this->m_Lua.Generic_Call("DarkLordCriticalDamage_Level1", "ii>ii", lpObj->Leadership + lpObj->AddLeadership, lpObj->m_PlayerData->Energy + lpObj->AddEnergy, &addcriticaldamagevalue, &SkillTime);

	addcriticaldamagevalue += this->GetMasterSkillValue(MagicDamageC.SkillGet(lpMagic->m_Skill), lpMagic->m_Level);

	if ( partynum == -1 )
	{
		gObjAddBuffEffect(lpObj, BUFFTYPE_CRITICAL_DMG_INC, EFFECTTYPE_CRITICAL_DMG, addcriticaldamagevalue, 0, 0, SkillTime);
		MsgOutput(aIndex, Lang.GetText(0,134), SkillTime);
		GSProtocol.GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, lpObj->m_Index, TRUE);
	}

	else
	{
		for(int n = 0;n < MAX_USER_IN_PARTY;n++)
		{
			if(ApplyPartyIndex[n] != -1)
			{
				lpPartyObj = &gObj[ApplyPartyIndex[n]];

				gObjAddBuffEffect(lpPartyObj, BUFFTYPE_CRITICAL_DMG_INC, EFFECTTYPE_CRITICAL_DMG, addcriticaldamagevalue, 0, 0, SkillTime);
				MsgOutput(ApplyPartyIndex[n], Lang.GetText(0,134), SkillTime);
				GSProtocol.GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, lpPartyObj->m_Index, TRUE);
			}
		}
	}
}

void CMasterLevelSkillTreeSystem::MLS_SkillSkillFulAddCriticalDamage(int aIndex, CMagicInf* lpMagic)
{
	LPOBJ lpObj = &gObj[aIndex];

	int number;
	int ApplyPartyIndex[MAX_USER_IN_PARTY];
	LPOBJ lpPartyObj;
	memset(ApplyPartyIndex,-1,sizeof(ApplyPartyIndex));
	
	int partynum = lpObj->PartyNumber;

	if(partynum != -1)
	{
		for(int n = 0; n < MAX_USER_IN_PARTY; n++)
		{
			number = gParty.m_PartyS[partynum].Number[n];

			if(number >= 0)
			{
				lpPartyObj = &gObj[number];

				if(lpObj->MapNumber == lpPartyObj->MapNumber)
				{
					if(gObjCalDistance(lpObj,&gObj[number]) < 10)
					{
						ApplyPartyIndex[n] = lpPartyObj->m_Index;
					}
				}
			}
		}
	}

	int addcriticaldamagevalue = 0;
	int SkillTime = 0;

	this->m_Lua.Generic_Call("DarkLordCriticalDamage_Level2", "ii>ii", lpObj->Leadership + lpObj->AddLeadership, lpObj->m_PlayerData->Energy + lpObj->AddEnergy, &addcriticaldamagevalue, &SkillTime);

	addcriticaldamagevalue += this->GetBrandOfMasterSkillValue(lpObj, lpMagic->m_Skill, 1);
	SkillTime += this->GetMasterSkillValue(MagicDamageC.SkillGet(lpMagic->m_Skill), lpMagic->m_Level);

	if ( partynum == -1 )
	{
		gObjAddBuffEffect(lpObj, BUFFTYPE_CRITICAL_DMG_INC, EFFECTTYPE_CRITICAL_DMG, addcriticaldamagevalue, 0, 0, SkillTime);
		MsgOutput(aIndex, Lang.GetText(0,134), SkillTime);
		GSProtocol.GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, lpObj->m_Index, TRUE);
	}

	else
	{
		for(int n = 0;n < MAX_USER_IN_PARTY;n++)
		{
			if(ApplyPartyIndex[n] != -1)
			{
				lpPartyObj = &gObj[ApplyPartyIndex[n]];

				gObjAddBuffEffect(lpPartyObj, BUFFTYPE_CRITICAL_DMG_INC, EFFECTTYPE_CRITICAL_DMG, addcriticaldamagevalue, 0, 0, SkillTime);
				MsgOutput(ApplyPartyIndex[n], Lang.GetText(0,134), SkillTime);
				GSProtocol.GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, lpPartyObj->m_Index, TRUE);
			}
		}
	}
}

void CMasterLevelSkillTreeSystem::MLS_SkillMasteryAddCriticalDamage(int aIndex, CMagicInf* lpMagic)
{
	LPOBJ lpObj = &gObj[aIndex];

	int number;
	int ApplyPartyIndex[MAX_USER_IN_PARTY];
	LPOBJ lpPartyObj;
	memset(ApplyPartyIndex,-1,sizeof(ApplyPartyIndex));
	
	int partynum = lpObj->PartyNumber;

	if(partynum != -1)
	{
		for(int n = 0; n < MAX_USER_IN_PARTY; n++)
		{
			number = gParty.m_PartyS[partynum].Number[n];

			if(number >= 0)
			{
				lpPartyObj = &gObj[number];

				if(lpObj->MapNumber == lpPartyObj->MapNumber)
				{
					if(gObjCalDistance(lpObj,&gObj[number]) < 10)
					{
						ApplyPartyIndex[n] = lpPartyObj->m_Index;
					}
				}
			}
		}
	}

	int addcriticaldamagevalue = 0;
	int SkillTime = 0;
	
	this->m_Lua.Generic_Call("DarkLordCriticalDamage_Level3", "ii>ii", lpObj->Leadership + lpObj->AddLeadership, lpObj->m_PlayerData->Energy + lpObj->AddEnergy, &addcriticaldamagevalue, &SkillTime);

	addcriticaldamagevalue += this->GetBrandOfMasterSkillValue(lpObj, lpMagic->m_Skill, 2);
	SkillTime += this->GetBrandOfMasterSkillValue(lpObj, lpMagic->m_Skill, 1);
	
	float fRate = this->GetMasterSkillValue(MagicDamageC.SkillGet(lpMagic->m_Skill), lpMagic->m_Level);

	if ( partynum == -1 )
	{
		gObjAddBuffEffect(lpObj, BUFFTYPE_CRITICAL_DMG_INC_STR, EFFECTTYPE_CRITICAL_DMG, addcriticaldamagevalue, EFFECTTYPE_CRITICALDAMAGE, fRate, SkillTime);
		MsgOutput(aIndex, Lang.GetText(0,134), SkillTime);
		GSProtocol.GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, lpObj->m_Index, TRUE);
	}

	else
	{
		for(int n = 0;n < MAX_USER_IN_PARTY;n++)
		{
			if(ApplyPartyIndex[n] != -1)
			{
				lpPartyObj = &gObj[ApplyPartyIndex[n]];

				gObjAddBuffEffect(lpPartyObj, BUFFTYPE_CRITICAL_DMG_INC_STR, EFFECTTYPE_CRITICAL_DMG, addcriticaldamagevalue, EFFECTTYPE_CRITICALDAMAGE, fRate, SkillTime);
				MsgOutput(ApplyPartyIndex[n], Lang.GetText(0,134), SkillTime);
				GSProtocol.GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, lpPartyObj->m_Index, TRUE);
			}
		}
	}
}

void CMasterLevelSkillTreeSystem::MLS_DarkHorseAttack(LPOBJ lpObj, int aTargetIndex, CMagicInf* lpMagic)
{
	if ( lpObj->Type != OBJ_USER )
	{
		return;
	}

	if (gObjDarkHorse(lpObj) == FALSE)
	{
		return;
	}
	
	int count = 0;
	int DuelIndex = lpObj->m_iDuelUser;
	int tObjNum;
	BOOL EnableAttack;

	GSProtocol.GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, lpObj->m_Index, TRUE);

	while ( true )
	{
		if ( lpObj->VpPlayer2[count].state != 0 )
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if ( tObjNum >= 0 )
			{
				EnableAttack = FALSE;

				if ( lpObj->VpPlayer2[count].type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon < 0 )
				{
					EnableAttack = TRUE;
				}

				else if ( tObjNum == aTargetIndex || DuelIndex == tObjNum )
				{
					EnableAttack = TRUE;
				}

				else if ( g_ConfigRead.server.GetServerType() == SERVER_CASTLE && lpObj->VpPlayer2[count].type != OBJ_NPC && gObj[tObjNum].MapNumber == MAP_INDEX_CASTLESIEGE && g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE )
				{
					EnableAttack = TRUE;
				}

				else
				{
					int CallMonIndex = gObj[tObjNum].m_Index;

					if ( gObj[tObjNum].Type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon >= 0 )
					{
						CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
					}

					if ( gObjTargetGuildWarCheck(lpObj,&gObj[CallMonIndex]) == TRUE )
					{
						EnableAttack = TRUE;
					}

					if ( gObj[tObjNum].Class >= 678 && gObj[tObjNum].Class <= 680 )
					{
						EnableAttack = TRUE;
					}
				}

				if ( EnableAttack == TRUE )
				{
					if ( gObjCalDistance(lpObj, &gObj[tObjNum]) < 5 )
					{
						gObjAddAttackProcMsgSendDelay(lpObj, 50, tObjNum, 500, lpMagic->m_Skill, 0);
					}
				}
			}
		}

		count++;

		if ( count > MAX_VIEWPORT-1 )
			break;
	}
}

void CMasterLevelSkillTreeSystem::MLS_FireBurst(LPOBJ lpObj, int aTargetIndex, CMagicInf* lpMagic)
{
	LPOBJ lpTargetObj = &gObj[aTargetIndex];
	int count = 0;
	int DuelIndex = lpObj->m_iDuelUser;
	int tObjNum;
	BOOL EnableAttack;

	GSProtocol.GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, aTargetIndex, TRUE);
	gObjAttack(lpObj, lpTargetObj, lpMagic, 0, TRUE, 0, 0, 0, 0);

	while ( true )
	{
		if ( lpObj->VpPlayer2[count].state != 0 )
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if ( tObjNum >= 0 )
			{
				EnableAttack = FALSE;

				if ( lpObj->VpPlayer2[count].type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon < 0 )
				{
					EnableAttack = TRUE;
				}

				else if ( tObjNum == aTargetIndex || DuelIndex == tObjNum )
				{
					EnableAttack = TRUE;
				}

				else if ( g_ConfigRead.server.GetServerType() == SERVER_CASTLE && lpObj->VpPlayer2[count].type != OBJ_NPC && gObj[tObjNum].MapNumber == MAP_INDEX_CASTLESIEGE && g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE )
				{
					EnableAttack = TRUE;
				}

				else
				{
					int CallMonIndex = gObj[tObjNum].m_Index;

					if ( gObj[tObjNum].Type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon >= 0 )
					{
						CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
					}

					if ( gObjTargetGuildWarCheck(lpObj,&gObj[CallMonIndex]) == TRUE )
					{
						EnableAttack = TRUE;
					}

					if ( gObj[tObjNum].Class >= 678 && gObj[tObjNum].Class <= 680 )
					{
						EnableAttack = TRUE;
					}
				}

				if ( EnableAttack == TRUE )
				{
					if ( gObjCalDistance(lpObj, &gObj[tObjNum]) < 3 )
					{
						gObjAddAttackProcMsgSendDelay(lpObj, 50, tObjNum, (rand()*17)%300 + 500, lpMagic->m_Skill, 0);
					}
				}
			}
		}

		count++;

		if ( count > MAX_VIEWPORT-1 )
			break;
	}
}

void CMasterLevelSkillTreeSystem::MLS_SkillStrengthenSpear(int aIndex, int aTargetIndex, CMagicInf *lpMagic)
{
	LPOBJ lpObj = &gObj[aIndex];
	int count = 0;

	GSProtocol.GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, aTargetIndex, TRUE);

	if( lpObj->SkillLongSpearChange == false )
	{
		gObjAttack(lpObj,&gObj[aTargetIndex],lpMagic,0,1,0,0,0,0);
		return;
	}

	int DuelIndex = lpObj->m_iDuelUser;
	int iangle = this->MLS_GetAngle(gObj[aIndex].X, gObj[aIndex].Y, gObj[aTargetIndex].X, gObj[aTargetIndex].Y);
	int tObjNum;
	BOOL EnableAttack;

	while ( true )
	{
		if ( lpObj->VpPlayer2[count].state != 0 )
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if ( tObjNum >= 0 )
			{
				EnableAttack = FALSE;

				if ( lpObj->VpPlayer2[count].type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon < 0 )
				{
					EnableAttack = TRUE;
				}

				else if ( tObjNum == aTargetIndex || DuelIndex == tObjNum )
				{
					EnableAttack = TRUE;
				}

				else if ( g_ConfigRead.server.GetServerType() == SERVER_CASTLE && lpObj->VpPlayer2[count].type != OBJ_NPC && gObj[tObjNum].MapNumber == MAP_INDEX_CASTLESIEGE && g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE )
				{
					EnableAttack = TRUE;
				}

				else
				{
					int CallMonIndex = gObj[tObjNum].m_Index;

					if ( gObj[tObjNum].Type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon >= 0 )
					{
						CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
					}

					if ( gObjTargetGuildWarCheck(lpObj,&gObj[CallMonIndex]) == TRUE )
					{
						EnableAttack = TRUE;
					}

					if ( gObj[tObjNum].Class >= 678 && gObj[tObjNum].Class <= 680 )
					{
						EnableAttack = TRUE;
					}
				}

				if ( EnableAttack == TRUE )
				{
					if ( SkillSpearHitBox.HitCheck(iangle, gObj[aIndex].X, gObj[aIndex].Y, gObj[tObjNum].X, gObj[tObjNum].Y) == TRUE )
					{
						gObjAttack(lpObj, &gObj[tObjNum], lpMagic, 0, 1, 0, 0, 0, 0);
					}
				}
			}
		}

		count++;

		if ( count > MAX_VIEWPORT-1 )
			break;
	}
}

void CMasterLevelSkillTreeSystem::MLS_SkillAreaMonsterAttack(int aIndex, CMagicInf* lpMagic, BYTE x, BYTE y, int aTargetIndex, int aDistance, BOOL bPVP, int nSuccessRate, int nDelay) // function not used
{
	return;
}

void CMasterLevelSkillTreeSystem::MLS_SuddenIce(int aIndex, CMagicInf *lpMagic, BYTE x, BYTE y, int aTargetIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	int count = 0;
	int DuelIndex = lpObj->m_iDuelUser;
	int tObjNum;
	BOOL EnableAttack;

	while ( true )
	{
		if ( lpObj->VpPlayer2[count].state != 0 )
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if ( tObjNum >= 0 )
			{
				EnableAttack = FALSE;

				if ( lpObj->VpPlayer2[count].type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon < 0 )
				{
					EnableAttack = TRUE;
				}

				else if ( tObjNum == aTargetIndex || DuelIndex == tObjNum )
				{
					EnableAttack = TRUE;
				}

				else if ( g_ConfigRead.server.GetServerType() == SERVER_CASTLE && lpObj->VpPlayer2[count].type != OBJ_NPC && gObj[tObjNum].MapNumber == MAP_INDEX_CASTLESIEGE && g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE )
				{
					EnableAttack = TRUE;
				}

				else
				{
					int CallMonIndex = gObj[tObjNum].m_Index;

					if ( gObj[tObjNum].Type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon >= 0 )
					{
						CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
					}

					if ( gObjTargetGuildWarCheck(lpObj,&gObj[CallMonIndex]) == TRUE )
					{
						EnableAttack = TRUE;
					}

					if ( gObj[tObjNum].Class >= 678 && gObj[tObjNum].Class <= 680 )
					{
						EnableAttack = TRUE;
					}
				}

				if ( EnableAttack == TRUE )
				{
					if ( this->MLS_CalDistance(x, y, gObj[tObjNum].X, gObj[tObjNum].Y) < 4 )
					{
						gObjAttack(lpObj, &gObj[tObjNum], lpMagic, 1, 1, 0, 0, 0, 0);
					}
				}
			}
		}

		count++;

		if ( count > MAX_VIEWPORT-1 )
			break;
	}
}

int CMasterLevelSkillTreeSystem::MLS_CalDistance(int x, int y, int x1, int y1)
{
	if(x == x1 && y == y1)
	{
		return 0;
	}

	float tx = x - x1;
	float ty = y - y1;

	return sqrt(tx*tx + ty*ty);
}

int CMasterLevelSkillTreeSystem::MLS_GetAngle(int x, int y, int tx, int ty)
{
	double diffX = x - tx;
	double diffY = y - ty;

	double rad = atan2(diffY,diffX);

	int angle = rad * 180 / 3.141592741012573 + 90;

	if(angle < 0)
	{
		angle += 360;
	}

	return angle;
}

void CMasterLevelSkillTreeSystem::MLS_SkillFrustrum(int aIndex, float fangle, float tx, float ty)
{
	LPOBJ lpObj = &gObj[aIndex];

	vec3_t p[4];

	Vector(-tx ,ty , 0.f, p[0]);
	Vector( tx ,ty , 0.f, p[1]);
	Vector( 1.0f, 0.f, 0.f, p[2]);
	Vector(-1.0f, 0.f, 0.f, p[3]);

	vec3_t Angle;

	Vector(0.f, 0.f, fangle, Angle);

	float Matrix[3][4];

	vec3_t vFrustrum[4];

	AngleMatrix(Angle,Matrix);

	for(int i = 0; i < 4; i++)
	{
		VectorRotate(p[i],Matrix,vFrustrum[i]);

		lpObj->fSkillFrustrumX[i] = (int)vFrustrum[i][0] + lpObj->X;
		lpObj->fSkillFrustrumY[i] = (int)vFrustrum[i][1] + lpObj->Y;
	}
}

void CMasterLevelSkillTreeSystem::MLS_KnightBlow(int aIndex, int aTargetIndex, CMagicInf* lpMagic, BOOL isCombo)
{
	LPOBJ lpObj = &gObj[aIndex];

	float fangle = this->MLS_GetAngle(gObj[aIndex].X, gObj[aIndex].Y, gObj[aTargetIndex].X, gObj[aTargetIndex].Y);

	float fDistance = MagicDamageC.GetSkillDistance(lpMagic->m_Skill) + 1.0;
	this->MLS_SkillFrustrum(aIndex, fangle, 1.5, fDistance);

	int count = 0;
	int loopcount = 0;
	int DuelIndex = lpObj->m_iDuelUser;
	int tObjNum;
	BOOL EnableAttack;

	GSProtocol.GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, aTargetIndex, TRUE);

	while ( true )
	{
		if ( lpObj->VpPlayer2[count].state != 0 )
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if ( tObjNum >= 0 )
			{
				EnableAttack = FALSE;

				if ( lpObj->VpPlayer2[count].type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon < 0 )
				{
					EnableAttack = TRUE;
				}

				else if ( tObjNum == aTargetIndex || DuelIndex == tObjNum )
				{
					EnableAttack = TRUE;
				}

				else if ( g_ConfigRead.server.GetServerType() == SERVER_CASTLE && lpObj->VpPlayer2[count].type != OBJ_NPC && gObj[tObjNum].MapNumber == MAP_INDEX_CASTLESIEGE && g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE )
				{
					EnableAttack = TRUE;
				}

				else
				{
					int CallMonIndex = gObj[tObjNum].m_Index;

					if ( gObj[tObjNum].Type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon >= 0 )
					{
						CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
					}

					if ( gObjTargetGuildWarCheck(lpObj,&gObj[CallMonIndex]) == TRUE )
					{
						EnableAttack = TRUE;
					}

					if ( gObj[tObjNum].Class >= 678 && gObj[tObjNum].Class <= 680 )
					{
						EnableAttack = TRUE;
					}
				}

				if ( EnableAttack == TRUE )
				{
					BOOL attackcheck = FALSE;

					if ( loopcount )
					{
						if ( (rand()%3) == 0 )
						{
							attackcheck = TRUE;
						}
					}

					else
					{
						attackcheck = TRUE;
					}

					if ( attackcheck == TRUE )
					{
						if ( SkillTestFrustrum(gObj[tObjNum].X, gObj[tObjNum].Y, aIndex) )
						{
							gObjAttack(lpObj, &gObj[tObjNum], lpMagic, 0, TRUE, 0, isCombo, 0, 0);
						}
					}
				}
			}
		}

		count++;

		if ( count > MAX_VIEWPORT-1 )
		{
			loopcount++;
			count = 0;

			if ( loopcount >= 3 )
				break;
		}
	}
}

void CMasterLevelSkillTreeSystem::MLS_SkillBlowOfFury(int aIndex, CMagicInf *lpMagic, BYTE x, BYTE y, int aTargetIndex, BOOL isCombo)
{
	LPOBJ lpObj = &gObj[aIndex];
	int count = 0;
	int DuelIndex = lpObj->m_iDuelUser;
	int tObjNum;
	BOOL EnableAttack;

	while ( true )
	{
		if ( lpObj->VpPlayer2[count].state != 0 )
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if ( tObjNum >= 0 )
			{
				EnableAttack = FALSE;

				if ( lpObj->VpPlayer2[count].type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon < 0 )
				{
					EnableAttack = TRUE;
				}

				else if ( tObjNum == aTargetIndex || DuelIndex == tObjNum )
				{
					EnableAttack = TRUE;
				}

				else if ( g_ConfigRead.server.GetServerType() == SERVER_CASTLE && lpObj->VpPlayer2[count].type != OBJ_NPC && gObj[tObjNum].MapNumber == MAP_INDEX_CASTLESIEGE && g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE )
				{
					EnableAttack = TRUE;
				}

				else
				{
					int CallMonIndex = gObj[tObjNum].m_Index;

					if ( gObj[tObjNum].Type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon >= 0 )
					{
						CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
					}

					if ( gObjTargetGuildWarCheck(lpObj,&gObj[CallMonIndex]) == TRUE )
					{
						EnableAttack = TRUE;
					}

					if ( gObj[tObjNum].Class >= 678 && gObj[tObjNum].Class <= 680 )
					{
						EnableAttack = TRUE;
					}
				}

				if ( EnableAttack == TRUE )
				{
					if ( gObjCalDistance(lpObj, &gObj[tObjNum]) <= MagicDamageC.GetSkillDistance(lpMagic->m_Skill) )
					{
						gObjAddAttackProcMsgSendDelay(lpObj, 50, tObjNum, 500, lpMagic->m_Skill, isCombo);
					}
				}
			}
		}

		count++;

		if ( count > MAX_VIEWPORT-1 )
			break;
	}
}

void CMasterLevelSkillTreeSystem::MLS_SkillDefense(int aIndex, int aTargetIndex, CMagicInf* lpMagic)
{
	LPOBJ lpObj = &gObj[aIndex];
	LPOBJ lpTargetObj = &gObj[aTargetIndex];

	if(lpTargetObj->Type != OBJ_USER && lpTargetObj->m_RecallMon == -1)
	{
		return;
	}

	if(lpObj->Class != CLASS_ELF)
	{
		return;
	}

	if ((CC_MAP_RANGE(lpObj->MapNumber) || lpObj->MapNumber == MAP_INDEX_CHAOSCASTLE_SURVIVAL) && lpObj->m_Index != lpTargetObj->m_Index)
	{
		return;
	}

	double skill_defense = 0.0;
	double skill_time = 0.0;

	this->m_Lua.Generic_Call("ElfDefense_Level1", "iiii>dd", lpTargetObj->Class, aIndex, aTargetIndex, lpObj->m_PlayerData->Energy + lpObj->AddEnergy, &skill_defense, &skill_time);

	skill_defense += skill_defense * this->GetMasterSkillValue(MagicDamageC.SkillGet(lpMagic->m_Skill), lpMagic->m_Level) / 100.0;

	if ( gObjCheckPowerfulEffect(lpTargetObj, BUFFTYPE_DEFENSE_POWER_INC, skill_defense, 0) == TRUE )
	{
		GSProtocol.GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, aTargetIndex, FALSE);
		return;
	}

	gObjAddBuffEffect(lpTargetObj, BUFFTYPE_DEFENSE_POWER_INC, EFFECTTYPE_IMPROVE_DEFENSE, skill_defense, 0, 0, skill_time);

	for(int n = 0; n < MAX_VIEWPORT; n++)
	{
		if(lpObj->VpPlayer2[n].type == OBJ_MONSTER && lpObj->VpPlayer2[n].state != 0)
		{
			LPOBJ lpMonster = &gObj[lpObj->VpPlayer2[n].number];
			if(lpMonster->m_iCurrentAI != 0)
			{
				lpMonster->m_Agro->IncAgro(lpObj->m_Index,int(skill_defense)/10);
			}
		}
	}

	GSProtocol.GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, aTargetIndex, TRUE);
}

void CMasterLevelSkillTreeSystem::MLS_SkillAttack(int aIndex, int aTargetIndex, CMagicInf* lpMagic)
{
	LPOBJ lpObj = &gObj[aIndex];
	LPOBJ lpTargetObj = &gObj[aTargetIndex];

	if(lpTargetObj->Type != OBJ_USER && lpTargetObj->m_RecallMon == -1)
	{
		return;
	}

	if(lpObj->Class != CLASS_ELF)
	{
		return;
	}

	if ((CC_MAP_RANGE(lpObj->MapNumber) || lpObj->MapNumber == MAP_INDEX_CHAOSCASTLE_SURVIVAL) && lpObj->m_Index != lpTargetObj->m_Index)
	{
		return;
	}

	double skill_attack = 0.0;
	double skill_time = 0.0;

	this->m_Lua.Generic_Call("ElfAttack_Level1", "iiii>dd", lpTargetObj->Class, aIndex, aTargetIndex, lpObj->m_PlayerData->Energy + lpObj->AddEnergy, &skill_attack, &skill_time);

	skill_attack += skill_attack * this->GetMasterSkillValue(MagicDamageC.SkillGet(lpMagic->m_Skill), lpMagic->m_Level) / 100.0;

	if ( gObjCheckPowerfulEffect(lpTargetObj, BUFFTYPE_ATTACK_POWER_INC, skill_attack, 0) == TRUE )
	{
		GSProtocol.GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, aTargetIndex, FALSE);
		return;
	}

	else if ( gObjCheckUsedBuffEffect(lpTargetObj, BUFFTYPE_BERSERKER) == TRUE )
	{
		GSProtocol.GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, aTargetIndex, FALSE);
		return;
	}

	else if ( gObjCheckUsedBuffEffect(lpTargetObj, BUFFTYPE_BERSERKER_STR) == TRUE )
	{
		GSProtocol.GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, aTargetIndex, FALSE);
		return;
	}

	else if ( gObjCheckUsedBuffEffect(lpTargetObj, BUFFTYPE_BERSERKER_MAS) == TRUE )
	{
		GSProtocol.GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, aTargetIndex, FALSE);
		return;
	}

	else if ( gObjCheckUsedBuffEffect(lpTargetObj, BUFFTYPE_BERSERKER_PRO) == TRUE )
	{
		GSProtocol.GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, aTargetIndex, FALSE);
		return;
	}

	gObjAddBuffEffect(lpTargetObj, BUFFTYPE_ATTACK_POWER_INC, EFFECTTYPE_IMPROVE_DAMAGE, skill_attack, 0, 0, skill_time);

	for(int n = 0; n < MAX_VIEWPORT; n++)
	{
		if(lpObj->VpPlayer2[n].type == OBJ_MONSTER && lpObj->VpPlayer2[n].state != 0)
		{
			LPOBJ lpMonster = &gObj[lpObj->VpPlayer2[n].number];
			if(lpMonster->m_iCurrentAI != 0)
			{
				lpMonster->m_Agro->IncAgro(lpObj->m_Index,int(skill_attack)/10);
			}
		}
	}

	GSProtocol.GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, aTargetIndex, TRUE);
}

void CMasterLevelSkillTreeSystem::MLS_SkillStrenghtenAttack(int aIndex, int aTargetIndex, CMagicInf* lpMagic)
{
	LPOBJ lpObj = &gObj[aIndex];
	LPOBJ lpTargetObj = &gObj[aTargetIndex];

	if(lpTargetObj->Type != OBJ_USER && lpTargetObj->m_RecallMon == -1)
	{
		return;
	}

	if(lpObj->Class != CLASS_ELF)
	{
		return;
	}

	if ((CC_MAP_RANGE(lpObj->MapNumber) || lpObj->MapNumber == MAP_INDEX_CHAOSCASTLE_SURVIVAL) && lpObj->m_Index != lpTargetObj->m_Index)
	{
		return;
	}

	float aMLSkillValue = 0.0;

	int iPreSkill = this->GetParentSkill1(lpObj->Class, lpMagic->m_Skill);

	if ( iPreSkill > 0 )
	{
		int iPreLevel = this->GetCurSkillLevel(lpObj, iPreSkill);

		if ( iPreLevel > 0 )
		{
			aMLSkillValue = this->GetMasterSkillValue(MagicDamageC.SkillGet(iPreSkill), iPreLevel);
		}
	}

	float fValue = this->GetMasterSkillValue(MagicDamageC.SkillGet(lpMagic->m_Skill), lpMagic->m_Level);

	double skill_attack = 0.0;
	double skill_time = 0.0;

	this->m_Lua.Generic_Call("ElfAttack_Level2", "iiii>dd", lpTargetObj->Class, aIndex, aTargetIndex, lpObj->m_PlayerData->Energy + lpObj->AddEnergy, &skill_attack, &skill_time);

	skill_attack += skill_attack * (aMLSkillValue + fValue) / 100.0;

	if ( gObjCheckPowerfulEffect(lpTargetObj, BUFFTYPE_ATTACK_POWER_INC, skill_attack, 0) == TRUE )
	{
		GSProtocol.GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, aTargetIndex, FALSE);
		return;
	}

	else if ( gObjCheckUsedBuffEffect(lpTargetObj, BUFFTYPE_BERSERKER) == TRUE )
	{
		GSProtocol.GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, aTargetIndex, FALSE);
		return;
	}

	else if ( gObjCheckUsedBuffEffect(lpTargetObj, BUFFTYPE_BERSERKER_STR) == TRUE )
	{
		GSProtocol.GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, aTargetIndex, FALSE);
		return;
	}

	else if ( gObjCheckUsedBuffEffect(lpTargetObj, BUFFTYPE_BERSERKER_MAS) == TRUE )
	{
		GSProtocol.GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, aTargetIndex, FALSE);
		return;
	}

	else if ( gObjCheckUsedBuffEffect(lpTargetObj, BUFFTYPE_BERSERKER_PRO) == TRUE )
	{
		GSProtocol.GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, aTargetIndex, FALSE);
		return;
	}

	gObjAddBuffEffect(lpTargetObj, BUFFTYPE_ATTACK_POWER_INC, EFFECTTYPE_IMPROVE_DAMAGE, skill_attack, 0, 0, skill_time + fValue);

	for(int n = 0; n < MAX_VIEWPORT; n++)
	{
		if(lpObj->VpPlayer2[n].type == OBJ_MONSTER && lpObj->VpPlayer2[n].state != 0)
		{
			LPOBJ lpMonster = &gObj[lpObj->VpPlayer2[n].number];
			if(lpMonster->m_iCurrentAI != 0)
			{
				lpMonster->m_Agro->IncAgro(lpObj->m_Index,int(skill_attack)/10);
			}
		}
	}

	GSProtocol.GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, aTargetIndex, TRUE);
}

void CMasterLevelSkillTreeSystem::MLS_SkillStrengthenDefense(int aIndex, int aTargetIndex, CMagicInf* lpMagic)
{
	LPOBJ lpObj = &gObj[aIndex];
	LPOBJ lpTargetObj = &gObj[aTargetIndex];

	if(lpTargetObj->Type != OBJ_USER && lpTargetObj->m_RecallMon == -1)
	{
		return;
	}

	if(lpObj->Class != CLASS_ELF)
	{
		return;
	}

	if ((CC_MAP_RANGE(lpObj->MapNumber) || lpObj->MapNumber == MAP_INDEX_CHAOSCASTLE_SURVIVAL) && lpObj->m_Index != lpTargetObj->m_Index)
	{
		return;
	}

	float aMLSkillValue = 0.0;

	int iPreSkill = this->GetParentSkill1(lpObj->Class, lpMagic->m_Skill);

	if ( iPreSkill > 0 )
	{
		int iPreLevel = this->GetCurSkillLevel(lpObj, iPreSkill);

		if ( iPreLevel > 0 )
		{
			aMLSkillValue = this->GetMasterSkillValue(MagicDamageC.SkillGet(iPreSkill), iPreLevel);
		}
	}

	float fValue = this->GetMasterSkillValue(MagicDamageC.SkillGet(lpMagic->m_Skill), lpMagic->m_Level);

	double skill_defense = 0.0;
	double skill_time = 0.0;

	this->m_Lua.Generic_Call("ElfDefense_Level2", "iiii>dd", lpTargetObj->Class, aIndex, aTargetIndex, lpObj->m_PlayerData->Energy + lpObj->AddEnergy, &skill_defense, &skill_time);

	skill_defense += skill_defense * (aMLSkillValue + fValue) / 100.0;
	skill_time += fValue;

	if ( gObjCheckPowerfulEffect(lpTargetObj, BUFFTYPE_DEFENSE_POWER_INC, skill_defense, 0) == TRUE )
	{
		GSProtocol.GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, aTargetIndex, FALSE);
		return;
	}

	gObjAddBuffEffect(lpTargetObj, BUFFTYPE_DEFENSE_POWER_INC, EFFECTTYPE_IMPROVE_DEFENSE, skill_defense, 0, 0, skill_time);

	for(int n = 0; n < MAX_VIEWPORT; n++)
	{
		if(lpObj->VpPlayer2[n].type == OBJ_MONSTER && lpObj->VpPlayer2[n].state != 0)
		{
			LPOBJ lpMonster = &gObj[lpObj->VpPlayer2[n].number];
			if(lpMonster->m_iCurrentAI != 0)
			{
				lpMonster->m_Agro->IncAgro(lpObj->m_Index,int(skill_defense)/10);
			}
		}
	}

	GSProtocol.GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, aTargetIndex, TRUE);
}

void CMasterLevelSkillTreeSystem::MLS_MaGumSkillDefenseDown(int aIndex, int aTargetIndex)
{
	LPOBJ lpObj = &gObj[aIndex];
	LPOBJ lpTargetObj = &gObj[aTargetIndex];

	if ( lpObj->Type == OBJ_USER && lpObj->Class != CLASS_MAGUMSA )
	{
		return;
	}

	if ( !ObjectMaxRange(aTargetIndex) )
	{
		return;
	}

	if ( retCalcSkillResistance(lpTargetObj, 1) )
	{
		gObjAddBuffEffect(lpTargetObj, BUFFTYPE_DEFENSE_POWER_DEC, EFFECTTYPE_DECREASE_DEFENSE, 10, 0, 0, 10);
		GSProtocol.GCMagicAttackNumberSend(lpObj, 490, aTargetIndex, TRUE);
	}
}

void CMasterLevelSkillTreeSystem::MLS_PowerSlash(int aIndex, CMagicInf* lpMagic, BYTE x, BYTE y, BYTE Targetangle, int aTargetIndex)
{
	LPOBJ lpObj = &gObj[aIndex];
	int iSkillDis = MagicDamageC.GetSkillDistance(lpMagic->m_Skill);

	this->MLS_SkillFrustrum(aIndex, (360 * Targetangle / 255), iSkillDis + 1.0, iSkillDis + 1.0);

	int count = 0;
	int HitCount = 0;
	int DuelIndex = lpObj->m_iDuelUser;
	int tObjNum;
	BOOL EnableAttack;
	BOOL bAttack;

	while ( true )
	{
		if ( lpObj->VpPlayer2[count].state != 0 )
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if ( tObjNum >= 0 )
			{
				EnableAttack = FALSE;

				if ( lpObj->VpPlayer2[count].type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon < 0 )
				{
					EnableAttack = TRUE;
				}

				else if ( tObjNum == aTargetIndex || DuelIndex == tObjNum )
				{
					EnableAttack = TRUE;
				}

				else if ( g_ConfigRead.server.GetServerType() == SERVER_CASTLE && lpObj->VpPlayer2[count].type != OBJ_NPC && gObj[tObjNum].MapNumber == MAP_INDEX_CASTLESIEGE && g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE )
				{
					EnableAttack = TRUE;
				}

				else
				{
					int CallMonIndex = gObj[tObjNum].m_Index;

					if ( gObj[tObjNum].Type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon >= 0 )
					{
						CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
					}

					if ( gObjTargetGuildWarCheck(lpObj,&gObj[CallMonIndex]) == TRUE )
					{
						EnableAttack = TRUE;
					}

					if ( gObj[tObjNum].Class >= 678 && gObj[tObjNum].Class <= 680 )
					{
						EnableAttack = TRUE;
					}
				}

				if ( EnableAttack == TRUE )
				{
					if ( SkillTestFrustrum(gObj[tObjNum].X, gObj[tObjNum].Y, aIndex) == TRUE )
					{
						if ( gObjCalDistance(lpObj, &gObj[tObjNum]) <= iSkillDis )
						{
							bAttack = FALSE;

							if ( HitCount > 10 )
								bAttack = FALSE;
							if ( HitCount < 5)
								bAttack = TRUE;
							else
								bAttack = rand()%2;

							if ( bAttack == TRUE )
							{
								gObjAttack(lpObj, &gObj[tObjNum], lpMagic, TRUE, TRUE, 0, 0, 0, 0);
							}

							HitCount++;
						}
					}
				}
			}
		}

		count++;
		
		if ( count > MAX_VIEWPORT-1 )
			break;
	}						
}

void CMasterLevelSkillTreeSystem::MLS_FireScream(int aIndex, int aTargetIndex, CMagicInf *lpMagic)
{
	if(g_SkillAdditionInfo.GetFireScreamSkill() == false)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];
	LPOBJ lpTargetObj = &gObj[aTargetIndex];

	if ( gObjIsConnected(lpObj->m_Index) == false && gObjIsConnected(lpTargetObj->m_Index) == false )
	{
		return;
	}

	if ( lpObj->Type != OBJ_USER )
	{
		return;
	}

	if ( lpObj->Class != CLASS_DARKLORD )
	{
		return;
	}

	gObjAttack(lpObj,lpTargetObj,lpMagic,0,0,0,0,0,0);
}

void CMasterLevelSkillTreeSystem::MLS_SkillMonkBarrageJustOneTarget(int aIndex, CMagicInf* lpMagic, int aTargetIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	if ( ObjectMaxRange(aTargetIndex) == FALSE )
	{
		g_Log.Add("[InvalidTargetIndex][MLS_SkillMonkBarrageJustOneTarget] Index :%d , AccountID : %s", aIndex, gObj[aIndex].AccountID);
		return;
	}

	LPOBJ lpTargetObj = &gObj[aTargetIndex];

	BOOL EnableAttack;
	int nViewPortObjectCount = 0;
	int nTargetObjNum;
	int nMaxBarrageCount;

	int nDistance = MagicDamageC.GetSkillDistance(lpMagic->m_Skill);

	if ( lpMagic->m_Skill == 563 )
	{
		int iPoint = this->GetMasterSkillValue(MagicDamageC.SkillGet(lpMagic->m_Skill), lpMagic->m_Level);
		nDistance += iPoint / 10;
	}

	while ( true )
	{
		if ( lpObj->VpPlayer2[nViewPortObjectCount].state == TRUE )
		{
			nTargetObjNum = lpObj->VpPlayer2[nViewPortObjectCount].number;
			if ( nTargetObjNum >= 0 )
			{
				if ( nTargetObjNum == aTargetIndex )
				{
					break;
				}
			}
		}

		nViewPortObjectCount++;

		if ( nViewPortObjectCount > MAX_VIEWPORT-1 )
			return;
	}

	EnableAttack = FALSE;

	if ( lpObj->VpPlayer2[nViewPortObjectCount].type == OBJ_USER )
	{
		if ( g_GensSystem.IsMapBattleZone(lpObj->MapNumber) == TRUE )
		{
			EnableAttack = TRUE;
		}

		else if ( g_ConfigRead.server.GetServerType() == SERVER_CASTLE && lpTargetObj->MapNumber == MAP_INDEX_CASTLESIEGE && g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE )
		{
			EnableAttack = TRUE;
		}

		else if ( gclassObjAttack.PkCheck(lpObj, lpTargetObj) == TRUE )
		{
			EnableAttack = TRUE;
		}
	}

	else if ( gObj[nTargetObjNum].Type == OBJ_MONSTER && gObj[nTargetObjNum].m_RecallMon >= 0 )
	{
		int CallMonIndex = gObj[gObj[nTargetObjNum].m_RecallMon].m_Index;
	
		if ( gObjTargetGuildWarCheck(lpObj,&gObj[CallMonIndex])==1 )
		{
			EnableAttack = TRUE;
		}

		if ( gObj[nTargetObjNum].Class >= 678 && gObj[nTargetObjNum].Class <= 680 )
		{
			EnableAttack = TRUE;
		}
	}

	else if ( lpTargetObj->Type == OBJ_MONSTER || lpTargetObj->m_RecallMon < 0 )
	{
		EnableAttack = TRUE;
	}

	if ( EnableAttack == TRUE && this->MLS_CalDistance(lpObj->X, lpObj->Y, lpTargetObj->X, lpTargetObj->Y) < nDistance )
	{
		switch ( lpMagic->m_Skill )
		{
			case 551:
			case 554:
				nMaxBarrageCount = 5;
				break;
			case 552:
			case 555:
				nMaxBarrageCount = 3;
				break;
			case 558:
			case 562:
				nMaxBarrageCount = 9;
				break;
			case 559:
			case 563:
				nMaxBarrageCount = 3;
				break;
			default:
				nMaxBarrageCount = 0;
				break;
		}

		switch ( lpMagic->m_Skill )
		{
			case 558:
			case 562:
				if ( rand()%100 <= 40 )
				{
					BYTE btAttr = MapC[lpTargetObj->MapNumber].GetAttr(lpTargetObj->X, lpTargetObj->Y);

					if ((btAttr & 1) != 1)
					{
						lpTargetObj->lpAttackObj = lpObj;
						lpTargetObj->DelayActionTime = 1000;
						lpTargetObj->DelayLevel = 1;

						gObjAddBuffEffect(lpTargetObj, BUFFTYPE_COLD, EFFECTTYPE_REDUCE_MOVE_SPEED, 0, 0, 0, 10);
					}
				}
				break;
			case 551:
				if ( rand()%100 <= 10 )
					gObjAddBuffEffect(lpTargetObj, BUFFTYPE_CURSED_ATTACK_DOWN, EFFECTTYPE_CURSED_ATTACKDOWN, 5, 0, 0, 10);
				break;
			case 554:
				if ( rand()%100 <= (this->GetMasterSkillValue(MagicDamageC.SkillGet(lpMagic->m_Skill), lpMagic->m_Level) + 10) )
					gObjAddBuffEffect(lpTargetObj, BUFFTYPE_CURSED_ATTACK_DOWN, EFFECTTYPE_CURSED_ATTACKDOWN, 5, 0, 0, 10);
				break;
			case 552:
				if ( rand()%100 <= 10 )
					gObjAddBuffEffect(lpTargetObj, BUFFTYPE_DEFENSE_POWER_DEC, EFFECTTYPE_DECREASE_DEFENSE, 10, 0, 0, 10);
				break;
			case 555:
				if ( rand()%100 <= (this->GetMasterSkillValue(MagicDamageC.SkillGet(lpMagic->m_Skill), lpMagic->m_Level) + 10) )
					gObjAddBuffEffect(lpTargetObj, BUFFTYPE_DEFENSE_POWER_DEC, EFFECTTYPE_DECREASE_DEFENSE, 10, 0, 0, 10);
				break;
		}

		if ( lpMagic->m_Skill == 562 )
		{
			if ( rand()%100 <= this->GetMasterSkillValue(MagicDamageC.SkillGet(lpMagic->m_Skill), lpMagic->m_Level) )
			{
				gObjAddBuffEffect(lpTargetObj, BUFFTYPE_DEBUFF_FROST, EFFECTTYPE_GIVE_DMG_TICK, 100, 0, 0, 10);
			}
		}

		for ( int i = 1; i < nMaxBarrageCount; i++ )
		{
			if ( i % (nMaxBarrageCount - 1) == 1 )
				gObjAttack(lpObj, lpTargetObj, lpMagic, 1, 1, 0, 0, i, 0);
			else
				gObjAttack(lpObj, lpTargetObj, lpMagic, 0, 1, 0, 0, i, 0);
		}
	}
}

void CMasterLevelSkillTreeSystem::MLS_SkillStrengthenDragonRoar(int aIndex, CMagicInf *lpMagic, BYTE x, BYTE y, int aTargetIndex, int aDistance, BOOL bPVP, int nDelay)
{
	LPOBJ lpObj = &gObj[aIndex];

	int DuelIndex = lpObj->m_iDuelUser;
	int nHitCount = 0;
	BOOL bAttack = FALSE;
	int nMinCount = 4;
	int nMaxCount = 8;

	BOOL EnableAttack = FALSE;
	int tObjNum;
	int count = 0;

	if ( this->MLS_CalDistance(lpObj->X, lpObj->Y, x, y) <= MagicDamageC.GetSkillDistance(lpMagic->m_Skill) )
	{
		while ( true )
		{
			if ( lpObj->VpPlayer2[count].state != 0 )
			{
				tObjNum = lpObj->VpPlayer2[count].number;

				if ( tObjNum >= 0 )
				{
					EnableAttack = FALSE;

					if ( lpObj->VpPlayer2[count].type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon < 0 )
					{
						EnableAttack = TRUE;
					}

					else if ( (tObjNum == aTargetIndex && bPVP == TRUE) || DuelIndex == tObjNum )
					{
						EnableAttack = TRUE;
					}

					else if ( g_ConfigRead.server.GetServerType() == SERVER_CASTLE && lpObj->VpPlayer2[count].type != OBJ_NPC && gObj[tObjNum].MapNumber == MAP_INDEX_CASTLESIEGE && g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE )
					{
						EnableAttack = TRUE;
					}

					else if ( lpObj->VpPlayer2[count].type == OBJ_USER && tObjNum == aTargetIndex && bPVP == TRUE )
					{
						if ( g_GensSystem.IsMapBattleZone(lpObj->MapNumber) == TRUE )
						{
							EnableAttack = TRUE;
						}

						else if ( gclassObjAttack.PkCheck(lpObj, &gObj[aTargetIndex]) == TRUE )
						{
							EnableAttack = TRUE;
						}
					}

					else
					{
						int CallMonIndex = gObj[tObjNum].m_Index;

						if ( gObj[tObjNum].Type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon >= 0 )
						{
							CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
						}

						if ( gObjTargetGuildWarCheck(lpObj,&gObj[CallMonIndex]) == TRUE )
						{
							EnableAttack = TRUE;
						}

						if ( gObj[tObjNum].Class >= 678 && gObj[tObjNum].Class <= 680 )
						{
							EnableAttack = TRUE;
						}
					}

					if ( EnableAttack == TRUE )
					{
						if ( gObj[tObjNum].X >= x - aDistance 
							&& gObj[tObjNum].X <= x + aDistance
							&& gObj[tObjNum].Y >= y - aDistance
							&& gObj[tObjNum].Y <= y + aDistance )
						{
							gObj[tObjNum].lpAttackObj = lpObj;
							nHitCount++;

							if ( nHitCount <= nMinCount )
							{
								bAttack = TRUE;
							}

							else if ( nHitCount > nMaxCount )
							{
								bAttack = FALSE;
							}

							else
							{
								bAttack = rand()%2;
							}

							if ( bAttack == TRUE )
							{
								if ( nDelay )
								{
									gObjAddAttackProcMsgSendDelay(lpObj, 50, tObjNum, nDelay, lpMagic->m_Skill, 0);
								}

								else if ( lpMagic->m_Skill != 560 && lpMagic->m_Skill != 561 )
								{
									gObjAttack(lpObj, &gObj[tObjNum], lpMagic, 1, 0, 0, 0, 0, 0);
								}

								else
								{
									for ( int i = 1; i <= 4; i++ )
									{
										if ( i == 1 )
											gObjAttack(lpObj, &gObj[tObjNum], lpMagic, 1, 1, 0, 0, i, 0);
										else
											gObjAttack(lpObj, &gObj[tObjNum], lpMagic, 0, 1, 0, 0, i, 0);
									}

									if ( lpMagic->m_Skill == 561 )
									{
										if ( this->GetMasterSkillValue(MagicDamageC.SkillGet(lpMagic->m_Skill), lpMagic->m_Level) >= (rand()%100) )
										{
											gObjAddBuffEffect(&gObj[tObjNum], BUFFTYPE_DEBUFF_FIRE, EFFECTTYPE_GIVE_DMG_TICK, 100, 0, 0, 10);
										}
									}
								}
							}
						}
					}
				}
			}

			count++;
			
			if ( count > MAX_VIEWPORT-1 )
				break;
		}
	}
}

void CMasterLevelSkillTreeSystem::MLS_SkillStrengthenBlockRate(int aIndex, CMagicInf* lpMagic)
{
	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->Type != OBJ_USER && lpObj->m_RecallMon == -1)
	{
		return;
	}

	if(lpObj->Class != CLASS_RAGEFIGHTER)
	{
		return;
	}

	int number;
	int ApplyPartyIndex[MAX_USER_IN_PARTY];
	LPOBJ lpPartyObj;
	memset(ApplyPartyIndex,-1,sizeof(ApplyPartyIndex));
	
	int partynum = lpObj->PartyNumber;

	if(partynum != -1)
	{
		for(int n = 0; n < MAX_USER_IN_PARTY; n++)
		{
			number = gParty.m_PartyS[partynum].Number[n];

			if(number >= 0)
			{
				lpPartyObj = &gObj[number];

				if(lpObj->MapNumber == lpPartyObj->MapNumber)
				{
					if(gObjCalDistance(lpObj,&gObj[number]) < 10)
					{
						ApplyPartyIndex[n] = lpPartyObj->m_Index;
					}
				}
			}
		}
	}

	int nEffectValue = (lpObj->m_PlayerData->Energy+lpObj->AddEnergy) / 50.0 + 10.0;

	if ( nEffectValue > 30 )
		nEffectValue = 30;

	nEffectValue += this->GetMasterSkillValue(MagicDamageC.SkillGet(lpMagic->m_Skill), lpMagic->m_Level);

	if ( partynum == -1 )
	{
		gObjAddBuffEffect(lpObj, BUFFTYPE_MONK_INCREASE_DEFENSE_RATE_STR, EFFECTTYPE_IMPROVE_DEFENSE_RATE, nEffectValue, 0, 0, lpObj->m_PlayerData->Energy / 5 + 60);
		GSProtocol.GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, aIndex, TRUE);
	}

	else
	{
		for ( int i=0;i<MAX_USER_IN_PARTY;i++ )
		{
			if ( ApplyPartyIndex[i] != -1 )
			{
				gObjAddBuffEffect(&gObj[ApplyPartyIndex[i]], BUFFTYPE_MONK_INCREASE_DEFENSE_RATE_STR, EFFECTTYPE_IMPROVE_DEFENSE_RATE, nEffectValue, 0, 0, lpObj->m_PlayerData->Energy / 5 + 60);
				GSProtocol.GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, ApplyPartyIndex[i], TRUE);
			}
		}
	}
}

void CMasterLevelSkillTreeSystem::MLS_SkillMasteryBlockRate(int aIndex, CMagicInf* lpMagic)
{
	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->Type != OBJ_USER && lpObj->m_RecallMon == -1)
	{
		return;
	}

	if(lpObj->Class != CLASS_RAGEFIGHTER)
	{
		return;
	}

	int number;
	int ApplyPartyIndex[MAX_USER_IN_PARTY];
	LPOBJ lpPartyObj;
	memset(ApplyPartyIndex,-1,sizeof(ApplyPartyIndex));
	
	int partynum = lpObj->PartyNumber;

	if(partynum != -1)
	{
		for(int n = 0; n < MAX_USER_IN_PARTY; n++)
		{
			number = gParty.m_PartyS[partynum].Number[n];

			if(number >= 0)
			{
				lpPartyObj = &gObj[number];

				if(lpObj->MapNumber == lpPartyObj->MapNumber)
				{
					if(gObjCalDistance(lpObj,&gObj[number]) < 10)
					{
						ApplyPartyIndex[n] = lpPartyObj->m_Index;
					}
				}
			}
		}
	}

	int nEffectValue = (lpObj->m_PlayerData->Energy+lpObj->AddEnergy) / 50.0 + 10.0;

	if ( nEffectValue > 30 )
		nEffectValue = 30;

	nEffectValue += this->GetBrandOfMasterSkillValue(lpObj, lpMagic->m_Skill, 1);
	float fDefenseValue = this->GetMasterSkillValue(MagicDamageC.SkillGet(lpMagic->m_Skill), lpMagic->m_Level);

	if ( partynum == -1 )
	{
		gObjAddBuffEffect(lpObj, BUFFTYPE_MONK_INCREASE_DEFENSE_RATE_STR, EFFECTTYPE_IMPROVE_DEFENSE_RATE, nEffectValue, EFFECTTYPE_IMPROVE_DEFENSE, fDefenseValue, lpObj->m_PlayerData->Energy / 5 + 60);
		GSProtocol.GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, aIndex, TRUE);
	}

	else
	{
		for ( int i=0;i<MAX_USER_IN_PARTY;i++ )
		{
			if ( ApplyPartyIndex[i] != -1 )
			{
				gObjAddBuffEffect(&gObj[ApplyPartyIndex[i]], BUFFTYPE_MONK_INCREASE_DEFENSE_RATE_STR, EFFECTTYPE_IMPROVE_DEFENSE_RATE, nEffectValue, EFFECTTYPE_IMPROVE_DEFENSE, fDefenseValue, lpObj->m_PlayerData->Energy / 5 + 60);
				GSProtocol.GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, ApplyPartyIndex[i], TRUE);
			}
		}
	}
}

void CMasterLevelSkillTreeSystem::MLS_SkillStrengthenIncVital(int aIndex, CMagicInf* lpMagic)
{
	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->Type != OBJ_USER && lpObj->m_RecallMon == -1)
	{
		return;
	}

	if(lpObj->Class != CLASS_RAGEFIGHTER)
	{
		return;
	}

	int number;
	int ApplyPartyIndex[MAX_USER_IN_PARTY];
	LPOBJ lpPartyObj;
	memset(ApplyPartyIndex,-1,sizeof(ApplyPartyIndex));
	
	int partynum = lpObj->PartyNumber;

	if(partynum != -1)
	{
		for(int n = 0; n < MAX_USER_IN_PARTY; n++)
		{
			number = gParty.m_PartyS[partynum].Number[n];

			if(number >= 0)
			{
				lpPartyObj = &gObj[number];

				if(lpObj->MapNumber == lpPartyObj->MapNumber)
				{
					if(gObjCalDistance(lpObj,&gObj[number]) < 10)
					{
						ApplyPartyIndex[n] = lpPartyObj->m_Index;
					}
				}
			}
		}
	}

	int nEffectValue = ((lpObj->m_PlayerData->Energy+lpObj->AddEnergy)-132) / 10.0 + 30.0;

	nEffectValue += this->GetMasterSkillValue(MagicDamageC.SkillGet(lpMagic->m_Skill), lpMagic->m_Level);

	if ( partynum == -1 )
	{
		gObjAddBuffEffect(lpObj, BUFFTYPE_MONK_INCREASE_HEALTH_STR, EFFECTTYPE_MONK_VITALITY, nEffectValue, 0, 0, lpObj->m_PlayerData->Energy / 5 + 60);
		GSProtocol.GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, aIndex, TRUE);
		gObjCalCharacter.CalcCharacter(lpObj->m_Index);
	}

	else
	{
		for ( int i=0;i<MAX_USER_IN_PARTY;i++ )
		{
			if ( ApplyPartyIndex[i] != -1 )
			{
				gObjAddBuffEffect(&gObj[ApplyPartyIndex[i]], BUFFTYPE_MONK_INCREASE_HEALTH_STR, EFFECTTYPE_MONK_VITALITY, nEffectValue, 0, 0, lpObj->m_PlayerData->Energy / 5 + 60);
				GSProtocol.GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, ApplyPartyIndex[i], TRUE);
				gObjCalCharacter.CalcCharacter(ApplyPartyIndex[i]);
			}
		}
	}
}

void CMasterLevelSkillTreeSystem::MLS_SkillStrengthenIgnoreEnemyDefense(int aIndex, CMagicInf *lpMagic)
{
	LPOBJ lpObj = &gObj[aIndex];

	if ( lpObj->Type != OBJ_USER )
	{
		return;
	}

	if ( lpObj->Class != CLASS_RAGEFIGHTER )
	{
		return;
	}

	if ( lpMagic == NULL )
	{
		return;
	}

	int nEffectValue = ((lpObj->m_PlayerData->Energy+lpObj->AddEnergy)-404) / 100.0 + 3.0;

	if ( nEffectValue > 10 )
		nEffectValue = 10;

	gObjAddBuffEffect(lpObj, BUFFTYPE_MONK_IGNORE_ENEMY_DEFENSE, EFFECTTYPE_DECREASE_DEFENSE_RATE, nEffectValue, EFFECTTYPE_MONK_DECREASE_DEFENSE, this->GetMasterSkillValue(MagicDamageC.SkillGet(lpMagic->m_Skill), lpMagic->m_Level), lpObj->m_PlayerData->Energy / 5 + 60);
	GSProtocol.GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, aIndex, TRUE);
}

// eX700 skills

void CMasterLevelSkillTreeSystem::MLS_SkillBloodStorm(LPOBJ lpObj, CMagicInf* lpMagic, BYTE x, BYTE y, int aTargetIndex, BOOL isCombo)
{
	int DuelIndex = lpObj->m_iDuelUser;
	int nHitCount = 0;
	BOOL bAttack = FALSE;
	int tObjNum;
	int count = 0;
	BOOL EnableAttack = FALSE;

	if ( this->MLS_CalDistance(lpObj->X, lpObj->Y, x, y) <= MagicDamageC.GetSkillDistance(lpMagic->m_Skill) )
	{
		while ( true )
		{
			if ( lpObj->VpPlayer2[count].state != 0 )
			{
				tObjNum = lpObj->VpPlayer2[count].number;

				if ( tObjNum >= 0 )
				{
					EnableAttack = FALSE;

					if ( lpObj->VpPlayer2[count].type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon < 0 )
					{
						EnableAttack = TRUE;
					}

					else if ( tObjNum == aTargetIndex || DuelIndex == tObjNum )
					{
						EnableAttack = TRUE;
					}

					else if ( g_ConfigRead.server.GetServerType() == SERVER_CASTLE && lpObj->VpPlayer2[count].type != OBJ_NPC && gObj[tObjNum].MapNumber == MAP_INDEX_CASTLESIEGE && g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE )
					{
						EnableAttack = TRUE;
					}

					else
					{
						int CallMonIndex = gObj[tObjNum].m_Index;

						if ( gObj[tObjNum].Type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon >= 0 )
						{
							CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
						}

						if ( gObjTargetGuildWarCheck(lpObj,&gObj[CallMonIndex]) == TRUE )
						{
							EnableAttack = TRUE;
						}

						if ( gObj[tObjNum].Class >= 678 && gObj[tObjNum].Class <= 680 )
						{
							EnableAttack = TRUE;
						}
					}

					if ( EnableAttack == TRUE )
					{
						if ( gObj[tObjNum].X >= x - 2 
							&& gObj[tObjNum].X <= x + 2
							&& gObj[tObjNum].Y >= y - 2
							&& gObj[tObjNum].Y <= y + 2 )
						{
							gObj[tObjNum].lpAttackObj = lpObj;
							nHitCount++;

							if ( nHitCount <= 4 )
							{
								bAttack = TRUE;
							}

							else if ( rand()%2 )
							{
								bAttack = TRUE;
							}

							if ( nHitCount > 8 )
							{
								bAttack = FALSE;
							}

							if ( bAttack == TRUE )
							{
								gObjAddAttackProcMsgSendDelay(lpObj, 60, tObjNum, 1000, lpMagic->m_Skill, isCombo);
								gObjAddAttackProcMsgSendDelay(lpObj, 50, tObjNum, 1500, lpMagic->m_Skill, 0);
							}
						}
					}
				}
			}

			count++;

			if ( count > MAX_VIEWPORT-1 ) 
				break;
		}
	}
}

void CMasterLevelSkillTreeSystem::MLS_SkillEarthPrison(LPOBJ lpObj, int aTargetIndex, CMagicInf* lpMagic)
{
	int nHitCount = 0;
	BOOL bAttack = FALSE;
	int DuelIndex = lpObj->m_iDuelUser;
	int count = 0;
	int tObjNum;
	BOOL EnableAttack = FALSE;

	while ( true )
	{
		if ( lpObj->VpPlayer2[count].state != 0 )
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if ( tObjNum >= 0 )
			{
				EnableAttack = FALSE;

				if ( gObj[tObjNum].Class >= 100 && gObj[tObjNum].Class <= 110 )
				{
					EnableAttack = FALSE;
				}

				if ( gObj[tObjNum].Class == 689 )
				{
					EnableAttack = FALSE;
				}

				else if ( lpObj->VpPlayer2[count].type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon < 0 )
				{
					EnableAttack = TRUE;
				}

				else if ( tObjNum == aTargetIndex || DuelIndex == tObjNum )
				{
					EnableAttack = TRUE;
				}

				else if ( g_ConfigRead.server.GetServerType() == SERVER_CASTLE && lpObj->VpPlayer2[count].type != OBJ_NPC && gObj[tObjNum].MapNumber == MAP_INDEX_CASTLESIEGE && g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE )
				{
					EnableAttack = TRUE;
				}

				else
				{
					int CallMonIndex = gObj[tObjNum].m_Index;

					if ( gObj[tObjNum].Type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon >= 0 )
					{
						CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
					}

					if ( gObjTargetGuildWarCheck(lpObj,&gObj[CallMonIndex]) == TRUE )
					{
						EnableAttack = TRUE;
					}

					if ( gObj[tObjNum].Class >= 678 && gObj[tObjNum].Class <= 680 )
					{
						EnableAttack = TRUE;
					}
				}

				if ( EnableAttack == TRUE )
				{
					if ( gObjCalDistance(lpObj, &gObj[tObjNum]) <= 3 )
					{
						bAttack = TRUE;
						nHitCount++;

						if ( nHitCount > 10 )
							bAttack = FALSE;

						if ( bAttack == TRUE )
						{
							gObjAddAttackProcMsgSendDelay(lpObj, 60, tObjNum, 1200, lpMagic->m_Skill, 0);
							gObjAddAttackProcMsgSendDelay(lpObj, 50, tObjNum, 1500, lpMagic->m_Skill, 0);
						}
					}
				}
			}
		}

		count++;

		if ( count > MAX_VIEWPORT-1 )
			break;
	}
}

void CMasterLevelSkillTreeSystem::MLS_SkillPoisonArrow(LPOBJ lpObj, CMagicInf *lpMagic, int aTargetIndex, BOOL isCombo)
{
	LPOBJ lpTargetObj = &gObj[aTargetIndex];

	if ( gObjIsConnected(lpObj->m_Index) == FALSE && gObjIsConnected(lpTargetObj->m_Index) == FALSE )
	{
		return;
	}

	if ( lpObj->Type != OBJ_USER )
	{
		return;
	}

	gObjAttack(lpObj, lpTargetObj, lpMagic, 1, 0, 0, isCombo, 0, 0);
	GSProtocol.GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, aTargetIndex, TRUE);
}

void CMasterLevelSkillTreeSystem::MLS_SkillCure(LPOBJ lpObj, CMagicInf *lpMagic, int aTargetIndex)
{
	LPOBJ lpTargetObj = &gObj[aTargetIndex];

	if ( gObjIsConnected(lpObj->m_Index) == FALSE && gObjIsConnected(lpTargetObj->m_Index) == FALSE )
	{
		return;
	}

	if ( lpObj->Type != OBJ_USER )
	{
		return;
	}

	gObjRemoveOneDebuffEffect(lpTargetObj);
	GSProtocol.GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, aTargetIndex, TRUE);
}

void CMasterLevelSkillTreeSystem::MLS_SkillMonsterCall(int aIndex, int MonsterType, int x, int y)
{
	if(gObj[aIndex].Type != OBJ_USER)
	{
		return;
	}

	if(gObj[aIndex].MapNumber == MAP_INDEX_ICARUS)
	{
		return;
	}

	if(CC_MAP_RANGE(gObj[aIndex].MapNumber) || gObj[aIndex].MapNumber == MAP_INDEX_CHAOSCASTLE_SURVIVAL)
	{
		return;
	}

	if(gObj[aIndex].m_RecallMon >= 0)
	{
		GSProtocol.GCRecallMonLife(aIndex,60,0);
		gObjMonsterCallKill(aIndex);
		return;
	}

	int result = gObjAddCallMon();

	if(result >= 0)
	{
		gObj[result].X = x;
		gObj[result].Y = y;
		gObj[result].MTX = x;
		gObj[result].MTY = y;
		gObj[result].Dir = 2;
		gObj[result].MapNumber = gObj[aIndex].MapNumber;
		gObjSetMonster(result, MonsterType);
		gObj[result].m_RecallMon = aIndex;
		gObj[result].m_Attribute = 100;
		gObj[result].TargetNumber = -1;
		gObj[result].m_ActState.Emotion = 0;
		gObj[result].m_ActState.Attack = 0;
		gObj[result].m_ActState.EmotionCount = 0;
		gObj[result].PathCount = 0;

		gObj[aIndex].m_RecallMon = result;
		gObj[result].m_MoveRange = 15;

		if ( gObj[aIndex].m_PlayerData->m_MPSkillOpt.iMpsCallMonHp > 0.0 )
		{
			gObj[result].Life += gObj[result].Life * gObj[aIndex].m_PlayerData->m_MPSkillOpt.iMpsCallMonHp / 100.0;
			gObj[result].MaxLife += gObj[result].MaxLife * gObj[aIndex].m_PlayerData->m_MPSkillOpt.iMpsCallMonHp / 100.0;
		}

		if ( gObj[aIndex].m_PlayerData->m_MPSkillOpt.iMpsCallMonDefense > 0.0 )
		{
			gObj[result].m_Defense += gObj[result].m_Defense * gObj[aIndex].m_PlayerData->m_MPSkillOpt.iMpsCallMonDefense / 100.0;
		}

		if ( gObj[aIndex].m_PlayerData->m_MPSkillOpt.iMpsCallMonAttack > 0.0 )
		{
			gObj[result].m_AttackDamageMin += gObj[result].m_AttackDamageMin * gObj[aIndex].m_PlayerData->m_MPSkillOpt.iMpsCallMonAttack / 100.0;
			gObj[result].m_AttackDamageMax += gObj[result].m_AttackDamageMax * gObj[aIndex].m_PlayerData->m_MPSkillOpt.iMpsCallMonAttack / 100.0;
		}

		GSProtocol.GCRecallMonLife(gObj[result].m_RecallMon,gObj[result].MaxLife,gObj[result].Life);
	}
}

void CMasterLevelSkillTreeSystem::MLS_SkillPartyHealing(int aIndex, int aTargetIndex, CMagicInf* lpMagic)
{
	if ( ObjectMaxRange(aIndex) == false )
	{
		return;
	}

	if ( ObjectMaxRange(aTargetIndex) == false )
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];
	LPOBJ lpTargetObj = &gObj[aTargetIndex];

	if ( lpObj->Type != OBJ_USER) 
	{
		return;
	}

	if ( lpTargetObj->Type != OBJ_USER )
	{
		return;
	}

	int count = 0;
	int nChainTarget[3] = { -1, -1, -1 };
	int partynum = lpObj->PartyNumber;

	if ( gCheckSkillDistance(aIndex, aTargetIndex, lpMagic->m_Skill) == FALSE )
	{
		return;
	}

	if ( partynum > -1 && partynum != lpTargetObj->PartyNumber )
	{
		g_Log.Add("[party healing] mismatch party number. target : %d, (%d - %d)", aTargetIndex, lpObj->PartyNumber, lpTargetObj->PartyNumber);
		return;
	}

	GSProtocol.GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, aTargetIndex, TRUE);

	nChainTarget[0] = aTargetIndex;
	count++;

	if ( partynum > -1 )
	{
		int partycount = gParty.m_PartyS[partynum].Count;
		BOOL EnableHealing = FALSE;

		for ( int nIndex = 0; nIndex < MAX_USER_IN_PARTY; nIndex++ )
		{
			int number = gParty.m_PartyS[partynum].Number[nIndex];

			if ( number >= 0 )
			{
				if ( aIndex == number )
					continue;

				if ( aTargetIndex == number )
					continue;

				LPOBJ lpPartyObj = &gObj[number];

				if ( lpPartyObj )
				{
					if ( lpPartyObj->X >= lpTargetObj->X - 3 &&
						 lpPartyObj->X <= lpTargetObj->X + 3 &&
						 lpPartyObj->Y >= lpTargetObj->Y - 3 &&
						 lpPartyObj->Y <= lpTargetObj->Y + 3 )
					{
						EnableHealing = TRUE;
						nChainTarget[1] = number;
						count++;
						break;
					}
				}
			}
		}

		if ( EnableHealing == TRUE )
		{
			for ( int nIndex = 0; nIndex < MAX_USER_IN_PARTY; nIndex++ )
			{
				int number = gParty.m_PartyS[partynum].Number[nIndex];

				if ( number >= 0 )
				{
					if ( nChainTarget[1] == number )
						continue;

					if ( aIndex == number )
						continue;

					if ( aTargetIndex == number )
						continue;

					LPOBJ lpPartyObj = &gObj[number];

					if ( lpPartyObj )
					{
						if ( lpPartyObj->X >= lpTargetObj->X - 3 &&
							 lpPartyObj->X <= lpTargetObj->X + 3 &&
							 lpPartyObj->Y >= lpTargetObj->Y - 3 &&
							 lpPartyObj->Y <= lpTargetObj->Y + 3 )
						{
							EnableHealing = TRUE;
							nChainTarget[2] = number;
							count++;
							break;
						}
					}
				}
			}
		}
	}

	if ( count >= 1 )
	{
		PMSG_CHAIN_MAGIC pMsg;
		PMSG_CHAIN_MAGIC_OBJECT pMagicObj;
		char SendByte[256];

		pMsg.MagicNumberH = HIBYTE(lpMagic->m_Skill);
		pMsg.MagicNumberL = LOBYTE(lpMagic->m_Skill);
		pMsg.wUserIndex = aIndex;
		pMsg.nCount = count;
		int nOffset = 10;

		for ( int i=0; i<count; i++ )
		{
			pMagicObj.nTargetIndex = nChainTarget[i];
			memcpy(&SendByte[nOffset], &pMagicObj, 2);
			nOffset += 2;
		}

		PHeadSubSetB((LPBYTE)&pMsg, 0xBF, 0x18, nOffset);
		memcpy(&SendByte, &pMsg, sizeof(pMsg));

		if ( lpObj->Type == OBJ_USER )
		{
			IOCP.DataSend(aIndex, (LPBYTE)&SendByte, nOffset);
		}

		GSProtocol.MsgSendV2(lpObj, (LPBYTE)&SendByte, nOffset);
	}

	int nHP = 0;
	int nApplyHP = 0;

	this->m_Lua.Generic_Call("ElfPartyHealing", "i>i", lpObj->m_PlayerData->Energy + lpObj->AddEnergy, &nHP);

	for(int n=0;n<count;n++)
	{
		if ( ObjectMaxRange(nChainTarget[n]) == false )
			continue;

		nApplyHP = nHP - (nHP * n * 0.2);

		if ( lpMagic->m_Skill == 429 )
		{
			nApplyHP += nApplyHP * this->GetMasterSkillValue(MagicDamageC.SkillGet(lpMagic->m_Skill), lpMagic->m_Level) / 100.0;
		}

		gObjAddMsgSendDelay(&gObj[nChainTarget[n]], 16, nChainTarget[n], 200 * n + 200, nApplyHP);
	}
}

void CMasterLevelSkillTreeSystem::MLS_SkillBless(LPOBJ lpObj, int aTargetIndex, CMagicInf* lpMagic)
{
	if ( ObjectMaxRange(aTargetIndex) == false )
	{
		return;
	}

	LPOBJ lpTargetObj = &gObj[aTargetIndex];

	if (lpTargetObj->Type != OBJ_USER)
	{
		GSProtocol.GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, aTargetIndex, FALSE);
		return;
	}

	if ( gObjCheckUsedBuffEffect(lpTargetObj, BUFFTYPE_BLESS) )
		gObjRemoveBuffEffect(lpTargetObj, BUFFTYPE_BLESS);

	int iEffectValue = 0;
	this->m_Lua.Generic_Call("ElfBless_Level1", "i>i", lpObj->m_PlayerData->Energy + lpObj->AddEnergy, &iEffectValue);

	// protection for exceeding stats over maximum allowed
	int Strength = lpTargetObj->m_PlayerData->Strength + lpTargetObj->AddStrength;
	int Dexterity = lpTargetObj->m_PlayerData->Dexterity + lpTargetObj->AddDexterity;
	int Vitality = lpTargetObj->m_PlayerData->Vitality + lpTargetObj->AddVitality;
	int Energy = lpTargetObj->m_PlayerData->Energy + lpTargetObj->AddEnergy;
	int Leadership = lpTargetObj->Leadership + lpTargetObj->AddLeadership;

	if ((Strength + iEffectValue) > g_MaxStrength)
	{
		GSProtocol.GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, aTargetIndex, FALSE);
		return;
	}

	if ((Dexterity + iEffectValue) > g_MaxAgility)
	{
		GSProtocol.GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, aTargetIndex, FALSE);
		return;
	}

	if ((Vitality + iEffectValue) > g_MaxVitality)
	{
		GSProtocol.GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, aTargetIndex, FALSE);
		return;
	}

	if ((Energy + iEffectValue) > g_MaxEnergy)
	{
		GSProtocol.GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, aTargetIndex, FALSE);
		return;
	}

	if ((Leadership + iEffectValue) > g_MaxCommand)
	{
		GSProtocol.GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, aTargetIndex, FALSE);
		return;
	}

	EnterCriticalSection(&lpObj->m_PlayerData->AgiCheckCriti);
	lpTargetObj->m_PlayerData->AgilityCheckDelay = GetTickCount();

	gObjAddBuffEffect(lpTargetObj, BUFFTYPE_BLESS, EFFECTTYPE_ELF_BLESS, iEffectValue, 0, 0, MagicDamageC.GetSkillKeepTime(lpMagic->m_Skill));

	PMSG_USE_STAT_FRUIT pMsg;
	PHeadSetB((LPBYTE)&pMsg, 0x2C, sizeof(pMsg));

	pMsg.result = 17;
	pMsg.btStatValue = iEffectValue;
	pMsg.btFruitType = 7;
	
	IOCP.DataSend(aTargetIndex, (LPBYTE)&pMsg, pMsg.h.size);
	LeaveCriticalSection(&lpObj->m_PlayerData->AgiCheckCriti);

	gObjCalCharacter.CalcCharacter(aTargetIndex);
	GSProtocol.GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, aTargetIndex, TRUE);
}

void CMasterLevelSkillTreeSystem::MLS_SkillStrengthenBless(LPOBJ lpObj, int aTargetIndex, CMagicInf* lpMagic)
{
	if (ObjectMaxRange(aTargetIndex) == false)
	{
		return;
	}

	LPOBJ lpTargetObj = &gObj[aTargetIndex];

	if (lpTargetObj->Type != OBJ_USER)
	{
		GSProtocol.GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, aTargetIndex, FALSE);
		return;
	}

	if ( gObjCheckUsedBuffEffect(lpTargetObj, BUFFTYPE_BLESS) )
		gObjRemoveBuffEffect(lpTargetObj, BUFFTYPE_BLESS);

	double fValue = 0.0;
	this->m_Lua.Generic_Call("ElfBless_Level2", "i>d", lpObj->m_PlayerData->Energy + lpObj->AddEnergy, &fValue);

	fValue += this->GetMasterSkillValue(MagicDamageC.SkillGet(lpMagic->m_Skill), lpMagic->m_Level);

	// protection for exceeding stats over maximum allowed
	int Strength = lpTargetObj->m_PlayerData->Strength + lpTargetObj->AddStrength;
	int Dexterity = lpTargetObj->m_PlayerData->Dexterity + lpTargetObj->AddDexterity;
	int Vitality = lpTargetObj->m_PlayerData->Vitality + lpTargetObj->AddVitality;
	int Energy = lpTargetObj->m_PlayerData->Energy + lpTargetObj->AddEnergy;
	int Leadership = lpTargetObj->Leadership + lpTargetObj->AddLeadership;

	if ((Strength + fValue) > g_MaxStrength)
	{
		GSProtocol.GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, aTargetIndex, FALSE);
		return;
	}

	if ((Dexterity + fValue) > g_MaxAgility)
	{
		GSProtocol.GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, aTargetIndex, FALSE);
		return;
	}

	if ((Vitality + fValue) > g_MaxVitality)
	{
		GSProtocol.GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, aTargetIndex, FALSE);
		return;
	}

	if ((Energy + fValue) > g_MaxEnergy)
	{
		GSProtocol.GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, aTargetIndex, FALSE);
		return;
	}

	if ((Leadership + fValue) > g_MaxCommand)
	{
		GSProtocol.GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, aTargetIndex, FALSE);
		return;
	}

	EnterCriticalSection(&lpObj->m_PlayerData->AgiCheckCriti);

	lpTargetObj->m_PlayerData->AgilityCheckDelay = GetTickCount();

	gObjAddBuffEffect(lpTargetObj, BUFFTYPE_BLESS, EFFECTTYPE_ELF_BLESS, fValue, 0, 0, MagicDamageC.GetSkillKeepTime(lpMagic->m_Skill));

	PMSG_USE_STAT_FRUIT pMsg;
	PHeadSetB((LPBYTE)&pMsg, 0x2C, sizeof(pMsg));

	pMsg.result = 17;
	pMsg.btStatValue = fValue;
	pMsg.btFruitType = 7;
	
	IOCP.DataSend(aTargetIndex, (LPBYTE)&pMsg, pMsg.h.size);
	LeaveCriticalSection(&lpObj->m_PlayerData->AgiCheckCriti);

	gObjCalCharacter.CalcCharacter(aTargetIndex);
	GSProtocol.GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, aTargetIndex, TRUE);
}

void CMasterLevelSkillTreeSystem::MLS_SkillBlind(LPOBJ lpObj, int aTargetIndex, CMagicInf* lpMagic)
{
	if ( ObjectMaxRange(aTargetIndex) == false )
	{
		return;
	}

	LPOBJ lpTargetObj = &gObj[aTargetIndex];
	
	if ( rand()%100 <= ((lpObj->m_PlayerData->Energy+lpObj->AddEnergy) / 100 + 5) )
	{
		gObjAddBuffEffect(lpTargetObj, BUFFTYPE_BLIND_2, EFFECTTYPE_BLIND, 50, 0, 0, MagicDamageC.GetSkillKeepTime(lpMagic->m_Skill));
		gObjViewportListCreate(lpTargetObj->m_Index);
		gObjAttack(lpObj, lpTargetObj, lpMagic, 1, 1, 0, 0, 0, 0);
	}

	GSProtocol.GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, aTargetIndex, TRUE);
}

void CMasterLevelSkillTreeSystem::MLS_SkillIronDefense(LPOBJ lpObj, int aTargetIndex, CMagicInf* lpMagic)
{
	if ( ObjectMaxRange(aTargetIndex) == false )
	{
		return;
	}

	LPOBJ lpTargetObj = &gObj[aTargetIndex];
	float fValue = this->GetMasterSkillValue(MagicDamageC.SkillGet(lpMagic->m_Skill), lpMagic->m_Level);

	gObjAddBuffEffect(lpTargetObj, BUFFTYPE_IRON_DEFENSE, EFFECTTYPE_HP, fValue, EFFECTTYPE_IMPROVE_DEFENSE, fValue, MagicDamageC.GetSkillKeepTime(lpMagic->m_Skill));
	GSProtocol.GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, aTargetIndex, TRUE);
}

void CMasterLevelSkillTreeSystem::MLS_SkillStrengthenIronDefense(LPOBJ lpObj, int aTargetIndex, CMagicInf* lpMagic)
{
	if ( ObjectMaxRange(aTargetIndex) == false )
	{
		return;
	}

	LPOBJ lpTargetObj = &gObj[aTargetIndex];
	float fPrevValue = 0.0;

	int iPreSkill = this->GetParentSkill1(lpObj->Class, lpMagic->m_Skill);

	if ( iPreSkill > 0 )
	{
		int iPreLevel = this->GetCurSkillLevel(lpObj, iPreSkill);

		if ( iPreLevel > 0 )
		{
			fPrevValue = this->GetMasterSkillValue(MagicDamageC.SkillGet(iPreSkill), iPreLevel);
		}
	}

	fPrevValue += this->GetMasterSkillValue(MagicDamageC.SkillGet(lpMagic->m_Skill), lpMagic->m_Level);

	gObjAddBuffEffect(lpTargetObj, BUFFTYPE_IRON_DEFENSE_STR, EFFECTTYPE_HP, fPrevValue, EFFECTTYPE_IMPROVE_DEFENSE, fPrevValue, MagicDamageC.GetSkillKeepTime(lpMagic->m_Skill));
	GSProtocol.GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, aTargetIndex, TRUE);
}

void CMasterLevelSkillTreeSystem::MLS_SkillBloodHowling(LPOBJ lpObj, int aTargetIndex, CMagicInf* lpMagic)
{
	if ( ObjectMaxRange(aTargetIndex) == false )
	{
		return;
	}

	LPOBJ lpTargetObj = &gObj[aTargetIndex];

	gObjAddBuffEffect(lpTargetObj, BUFFTYPE_BLOOD_HOWL, 0, 0, 0, 0, MagicDamageC.GetSkillKeepTime(lpMagic->m_Skill));
	GSProtocol.GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, aTargetIndex, TRUE);
}

void CMasterLevelSkillTreeSystem::MLS_SkillStrengthenBloodHowling(LPOBJ lpObj, int aTargetIndex, CMagicInf* lpMagic)
{
	if ( ObjectMaxRange(aTargetIndex) == false )
	{
		return;
	}

	LPOBJ lpTargetObj = &gObj[aTargetIndex];

	if (lpTargetObj->Type != OBJ_USER)
	{
		return;
	}

	gObjAddBuffEffect(lpTargetObj, BUFFTYPE_BLOOD_HOWL_STR, 0, 0, 0, 0, MagicDamageC.GetSkillKeepTime(lpMagic->m_Skill));
	lpTargetObj->m_PlayerData->m_MPSkillOpt.iMpsIncDamageBloodHowling = this->GetMasterSkillValue(MagicDamageC.SkillGet(lpMagic->m_Skill), lpMagic->m_Level);
	GSProtocol.GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, aTargetIndex, TRUE);
}

void CMasterLevelSkillTreeSystem::MLS_SkillfulKnightBlow(int aIndex, int aTargetIndex, CMagicInf* lpMagic, BOOL isCombo)
{
	LPOBJ lpObj = &gObj[aIndex];
	LPOBJ lpTargetObj = &gObj[aTargetIndex];

	float fangle = this->MLS_GetAngle(gObj[aIndex].X, gObj[aIndex].Y, gObj[aTargetIndex].X, gObj[aTargetIndex].Y);

	float fDistance = MagicDamageC.GetSkillDistance(lpMagic->m_Skill) + 1.0;
	this->MLS_SkillFrustrum(aIndex, fangle, 1.5, fDistance);

	int count = 0;
	int loopcount = 0;
	int DuelIndex = lpObj->m_iDuelUser;
	int tObjNum;
	BOOL EnableAttack;

	GSProtocol.GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, aTargetIndex, TRUE);
	float fValue = this->GetMasterSkillValue(MagicDamageC.SkillGet(lpMagic->m_Skill), lpMagic->m_Level);

	while ( true )
	{
		if ( lpObj->VpPlayer2[count].state != 0 )
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if ( tObjNum >= 0 )
			{
				EnableAttack = FALSE;

				if ( lpObj->VpPlayer2[count].type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon < 0 )
				{
					EnableAttack = TRUE;
				}

				else if ( tObjNum == aTargetIndex || DuelIndex == tObjNum )
				{
					EnableAttack = TRUE;
				}

				else if ( g_ConfigRead.server.GetServerType() == SERVER_CASTLE && lpObj->VpPlayer2[count].type != OBJ_NPC && gObj[tObjNum].MapNumber == MAP_INDEX_CASTLESIEGE && g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE )
				{
					EnableAttack = TRUE;
				}

				else
				{
					int CallMonIndex = gObj[tObjNum].m_Index;

					if ( gObj[tObjNum].Type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon >= 0 )
					{
						CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
					}

					if ( gObjTargetGuildWarCheck(lpObj,&gObj[CallMonIndex]) == TRUE )
					{
						EnableAttack = TRUE;
					}

					if ( gObj[tObjNum].Class >= 678 && gObj[tObjNum].Class <= 680 )
					{
						EnableAttack = TRUE;
					}
				}

				if (g_GensSystem.IsMapBattleZone(lpObj->MapNumber) == TRUE)
				{
					EnableAttack = TRUE;
				}

				if (gclassObjAttack.PkCheck(lpObj, lpTargetObj) == FALSE)
				{
					EnableAttack = FALSE;
				}

				if ( EnableAttack == TRUE )
				{
					BOOL attackcheck = FALSE;

					if ( loopcount )
					{
						if ( (rand()%3) == 0 )
						{
							attackcheck = TRUE;
						}
					}

					else
					{
						attackcheck = TRUE;
					}

					if ( attackcheck == TRUE )
					{
						if ( SkillTestFrustrum(gObj[tObjNum].X, gObj[tObjNum].Y, aIndex) )
						{
							gObj[tObjNum].lpAttackObj = lpObj;
							gObjAttack(lpObj, &gObj[tObjNum], lpMagic, 0, 1, 0, isCombo, 0, 0);

							if ( fValue > (rand()%100) )
							{
								if ( gObjCheckUsedBuffEffect(&gObj[tObjNum], BUFFTYPE_BLEEDING) == TRUE )
								{
									gObjRemoveBuffEffect(&gObj[tObjNum], BUFFTYPE_BLEEDING);
								}

								gObjAddBuffEffect(&gObj[tObjNum], BUFFTYPE_BLEEDING, EFFECTTYPE_GIVE_DMG_TICK, (lpObj->m_PlayerData->Strength+lpObj->AddStrength)/10, 0, 0, 10);
							}
						}
					}
				}
			}
		}

		count++;

		if ( count > MAX_VIEWPORT-1 )
		{
			loopcount++;
			count = 0;

			if ( loopcount >= 3 )
				break;
		}
	}
}

void CMasterLevelSkillTreeSystem::MLS_MasteryKnightBlow(int aIndex, int aTargetIndex, CMagicInf* lpMagic, BOOL isCombo)
{
	LPOBJ lpObj = &gObj[aIndex];
	LPOBJ lpTargetObj = &gObj[aTargetIndex];

	float fangle = this->MLS_GetAngle(gObj[aIndex].X, gObj[aIndex].Y, gObj[aTargetIndex].X, gObj[aTargetIndex].Y);

	float fDistance = MagicDamageC.GetSkillDistance(lpMagic->m_Skill) + 1.0;
	this->MLS_SkillFrustrum(aIndex, fangle, 1.5, fDistance);

	int count = 0;
	int loopcount = 0;
	int DuelIndex = lpObj->m_iDuelUser;
	int tObjNum;
	BOOL EnableAttack;

	GSProtocol.GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, aTargetIndex, TRUE);

	float fPrevValue = 0.0;
	int iPreSkill = this->GetParentSkill1(lpObj->Class, lpMagic->m_Skill);

	if ( iPreSkill > 0 )
	{
		int iPreLevel = this->GetCurSkillLevel(lpObj, iPreSkill);

		if ( iPreLevel > 0 )
		{
			fPrevValue = this->GetMasterSkillValue(MagicDamageC.SkillGet(iPreSkill), iPreLevel);
		}
	}

	float fValue = this->GetMasterSkillValue(MagicDamageC.SkillGet(lpMagic->m_Skill), lpMagic->m_Level);

	while ( true )
	{
		if ( lpObj->VpPlayer2[count].state != 0 )
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if ( tObjNum >= 0 )
			{
				EnableAttack = FALSE;

				if ( lpObj->VpPlayer2[count].type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon < 0 )
				{
					EnableAttack = TRUE;
				}

				else if ( tObjNum == aTargetIndex || DuelIndex == tObjNum )
				{
					EnableAttack = TRUE;
				}

				else if ( g_ConfigRead.server.GetServerType() == SERVER_CASTLE && lpObj->VpPlayer2[count].type != OBJ_NPC && gObj[tObjNum].MapNumber == MAP_INDEX_CASTLESIEGE && g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE )
				{
					EnableAttack = TRUE;
				}

				else
				{
					int CallMonIndex = gObj[tObjNum].m_Index;

					if ( gObj[tObjNum].Type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon >= 0 )
					{
						CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
					}

					if ( gObjTargetGuildWarCheck(lpObj,&gObj[CallMonIndex]) == TRUE )
					{
						EnableAttack = TRUE;
					}

					if ( gObj[tObjNum].Class >= 678 && gObj[tObjNum].Class <= 680 )
					{
						EnableAttack = TRUE;
					}
				}

				if (g_GensSystem.IsMapBattleZone(lpObj->MapNumber) == TRUE)
				{
					EnableAttack = TRUE;
				}

				if (gclassObjAttack.PkCheck(lpObj, lpTargetObj) == FALSE)
				{
					EnableAttack = FALSE;
				}

				if ( EnableAttack == TRUE )
				{
					BOOL attackcheck = FALSE;

					if ( loopcount )
					{
						if ( (rand()%3) == 0 )
						{
							attackcheck = TRUE;
						}
					}

					else
					{
						attackcheck = TRUE;
					}

					if ( attackcheck == TRUE )
					{
						if ( SkillTestFrustrum(gObj[tObjNum].X, gObj[tObjNum].Y, aIndex) )
						{
							gObj[tObjNum].lpAttackObj = lpObj;
							gObjAttack(lpObj, &gObj[tObjNum], lpMagic, 0, 1, 0, isCombo, 0, 0);

							if ( fPrevValue >= (rand()%100) )
							{
								if ( gObjCheckUsedBuffEffect(&gObj[tObjNum], BUFFTYPE_BLEEDING) == TRUE )
								{
									gObjRemoveBuffEffect(&gObj[tObjNum], BUFFTYPE_BLEEDING);
								}

								gObjAddBuffEffect(&gObj[tObjNum], BUFFTYPE_BLEEDING, EFFECTTYPE_GIVE_DMG_TICK, (lpObj->m_PlayerData->Strength+lpObj->AddStrength)/10, 0, 0, 10);
							}

							int iStunEffect = fValue;
							
							if (gObj[tObjNum].Type == OBJ_USER)
							{
								iStunEffect -= gObj[tObjNum].m_PlayerData->m_Resistance_Stun;
							}

							if ( iStunEffect >= (rand()%100) )
							{
								gObjAddBuffEffect(&gObj[tObjNum], BUFFTYPE_STUN, 0, 0, 0, 0, 2);
								gObjSetPosition(tObjNum, gObj[tObjNum].X, gObj[tObjNum].Y);
							}
						}
					}
				}
			}
		}

		count++;

		if ( count > MAX_VIEWPORT-1 )
		{
			loopcount++;
			count = 0;

			if ( loopcount >= 3 )
				break;
		}
	}
}

void CMasterLevelSkillTreeSystem::MLS_SkillSkillfulDesruction(OBJECTSTRUCT *lpObj, CMagicInf *lpMagic, BYTE x, BYTE y, int aTargetIndex, BOOL isCombo)
{
	int count = 0;
	int DuelIndex = lpObj->m_iDuelUser;
	int nHitCount = 0;
	BOOL bAttack = FALSE;
	BOOL EnableAttack;
	int tObjNum;
	float fValue = this->GetMasterSkillValue(MagicDamageC.SkillGet(lpMagic->m_Skill), lpMagic->m_Level);

	if ( this->MLS_CalDistance(lpObj->X, lpObj->Y, x, y) <= MagicDamageC.GetSkillDistance(lpMagic->m_Skill) )
	{
		while ( true )
		{
			if ( lpObj->VpPlayer2[count].state != 0 )
			{
				tObjNum = lpObj->VpPlayer2[count].number;

				if ( tObjNum >= 0 )
				{
					EnableAttack = FALSE;

					if ( lpObj->VpPlayer2[count].type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon < 0 )
					{
						EnableAttack = TRUE;
					}

					else if ( tObjNum == aTargetIndex || DuelIndex == tObjNum )
					{
						EnableAttack = TRUE;
					}

					else if ( g_ConfigRead.server.GetServerType() == SERVER_CASTLE && lpObj->VpPlayer2[count].type != OBJ_NPC && gObj[tObjNum].MapNumber == MAP_INDEX_CASTLESIEGE && g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE )
					{
						EnableAttack = TRUE;
					}

					else
					{
						int CallMonIndex = gObj[tObjNum].m_Index;

						if ( gObj[tObjNum].Type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon >= 0 )
						{
							CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
						}

						if ( gObjTargetGuildWarCheck(lpObj,&gObj[CallMonIndex]) == TRUE )
						{
							EnableAttack = TRUE;
						}

						if ( gObj[tObjNum].Class >= 678 && gObj[tObjNum].Class <= 680 )
						{
							EnableAttack = TRUE;
						}
					}

					if ( EnableAttack == TRUE )
					{
						if ( gObj[tObjNum].X >= x - 3 
							&& gObj[tObjNum].X <= x + 3
							&& gObj[tObjNum].Y >= y - 3
							&& gObj[tObjNum].Y <= y + 3 )
						{
							gObj[tObjNum].lpAttackObj = lpObj;
							nHitCount++;

							if ( nHitCount <= 4 )
							{
								bAttack = TRUE;
							}

							else if ( rand()%2 )
							{
								bAttack = TRUE;
							}

							if ( nHitCount > 8 )
							{
								bAttack = FALSE;
							}

							if ( bAttack == TRUE )
							{
								gObjAddAttackProcMsgSendDelay(lpObj, 50, tObjNum, 500, lpMagic->m_Skill, 0);

								if ( fValue >= (rand()%100) )
								{
									gObjAddBuffEffect(&gObj[tObjNum], BUFFTYPE_FREEZE_2, 0, 0, 0, 0, 3);

									gObj[tObjNum].PathCount = 0;
									gObj[tObjNum].PathStartEnd = 0;

									gObjSetPosition(tObjNum, gObj[tObjNum].X, gObj[tObjNum].Y);
								}
							}
						}
					}
				}
			}

			count++;

			if ( count > MAX_VIEWPORT-1 ) 
				break;
		}
	}
}

void CMasterLevelSkillTreeSystem::MLS_SkillMasteryDesruction(OBJECTSTRUCT *lpObj, CMagicInf *lpMagic, BYTE x, BYTE y, int aTargetIndex, BOOL isCombo)
{
	int count = 0;
	int DuelIndex = lpObj->m_iDuelUser;
	int nHitCount = 0;
	BOOL bAttack = FALSE;
	BOOL EnableAttack;
	int tObjNum;

	float fPrevValue = this->GetBrandOfMasterSkillValue(lpObj, lpMagic->m_Skill, 1);
	float fValue = this->GetMasterSkillValue(MagicDamageC.SkillGet(lpMagic->m_Skill), lpMagic->m_Level);

	if ( this->MLS_CalDistance(lpObj->X, lpObj->Y, x, y) <= MagicDamageC.GetSkillDistance(lpMagic->m_Skill) )
	{
		while ( true )
		{
			if ( lpObj->VpPlayer2[count].state != 0 )
			{
				tObjNum = lpObj->VpPlayer2[count].number;

				if ( tObjNum >= 0 )
				{
					EnableAttack = FALSE;

					if ( lpObj->VpPlayer2[count].type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon < 0 )
					{
						EnableAttack = TRUE;
					}

					else if ( tObjNum == aTargetIndex || DuelIndex == tObjNum )
					{
						EnableAttack = TRUE;
					}

					else if ( g_ConfigRead.server.GetServerType() == SERVER_CASTLE && lpObj->VpPlayer2[count].type != OBJ_NPC && gObj[tObjNum].MapNumber == MAP_INDEX_CASTLESIEGE && g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE )
					{
						EnableAttack = TRUE;
					}

					else
					{
						int CallMonIndex = gObj[tObjNum].m_Index;

						if ( gObj[tObjNum].Type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon >= 0 )
						{
							CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
						}

						if ( gObjTargetGuildWarCheck(lpObj,&gObj[CallMonIndex]) == TRUE )
						{
							EnableAttack = TRUE;
						}

						if ( gObj[tObjNum].Class >= 678 && gObj[tObjNum].Class <= 680 )
						{
							EnableAttack = TRUE;
						}
					}

					if ( EnableAttack == TRUE )
					{
						if ( gObj[tObjNum].X >= x - 3 
							&& gObj[tObjNum].X <= x + 3
							&& gObj[tObjNum].Y >= y - 3
							&& gObj[tObjNum].Y <= y + 3 )
						{
							gObj[tObjNum].lpAttackObj = lpObj;
							nHitCount++;

							if ( nHitCount <= 4 )
							{
								bAttack = TRUE;
							}

							else if ( rand()%2 )
							{
								bAttack = TRUE;
							}

							if ( nHitCount > 8 )
							{
								bAttack = FALSE;
							}

							if ( bAttack == TRUE )
							{
								gObjAddAttackProcMsgSendDelay(lpObj, 50, tObjNum, 500, lpMagic->m_Skill, 0);

								if ( fPrevValue >= (rand()%100) )
								{
									gObjAddBuffEffect(&gObj[tObjNum], BUFFTYPE_FREEZE_2, 0, 0, 0, 0, 3);

									gObj[tObjNum].PathCount = 0;
									gObj[tObjNum].PathStartEnd = 0;

									gObjSetPosition(tObjNum, gObj[tObjNum].X, gObj[tObjNum].Y);
								}

								if ( fValue >= (rand()%100) )
								{
									if ( !gObjCheckUsedBuffEffect(&gObj[tObjNum], BUFFTYPE_ATTACK_SPEED_DEC) )
									{
										gObjAddBuffEffect(&gObj[tObjNum], BUFFTYPE_ATTACK_SPEED_DEC, EFFECTTYPE_REDUCE_ATTACK_SPEED, 0, 0, 0, 5);
									}
								}
							}
						}
					}
				}
			}

			count++;

			if ( count > MAX_VIEWPORT-1 ) 
				break;
		}
	}
}

void CMasterLevelSkillTreeSystem::MLS_MasterySuddenIce(int aIndex, CMagicInf *lpMagic, BYTE x, BYTE y, int aTargetIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	int count = 0;
	int DuelIndex = lpObj->m_iDuelUser;
	int tObjNum;
	BOOL EnableAttack;
	float fRate = this->GetMasterSkillValue(MagicDamageC.SkillGet(lpMagic->m_Skill), lpMagic->m_Level);

	while ( true )
	{
		if ( lpObj->VpPlayer2[count].state != 0 )
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if ( tObjNum >= 0 )
			{
				EnableAttack = FALSE;

				if ( lpObj->VpPlayer2[count].type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon < 0 )
				{
					EnableAttack = TRUE;
				}

				else if ( tObjNum == aTargetIndex || DuelIndex == tObjNum )
				{
					EnableAttack = TRUE;
				}

				else if ( g_ConfigRead.server.GetServerType() == SERVER_CASTLE && lpObj->VpPlayer2[count].type != OBJ_NPC && gObj[tObjNum].MapNumber == MAP_INDEX_CASTLESIEGE && g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE )
				{
					EnableAttack = TRUE;
				}

				else
				{
					int CallMonIndex = gObj[tObjNum].m_Index;

					if ( gObj[tObjNum].Type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon >= 0 )
					{
						CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
					}

					if ( gObjTargetGuildWarCheck(lpObj,&gObj[CallMonIndex]) == TRUE )
					{
						EnableAttack = TRUE;
					}

					if ( gObj[tObjNum].Class >= 678 && gObj[tObjNum].Class <= 680 )
					{
						EnableAttack = TRUE;
					}
				}

				if ( EnableAttack == TRUE )
				{
					if ( this->MLS_CalDistance(x, y, gObj[tObjNum].X, gObj[tObjNum].Y) < 4 )
					{
						gObjAttack(lpObj, &gObj[tObjNum], lpMagic, 1, 1, 0, 0, 0, 0);

						if ( fRate >= (rand()%100) )
						{
							gObjAddBuffEffect(&gObj[tObjNum], BUFFTYPE_FREEZE_2, 0, 0, 0, 0, 3);

							gObj[tObjNum].lpAttackObj = lpObj;
							gObj[tObjNum].PathCount = 0;
							gObj[tObjNum].PathStartEnd = 0;

							gObjSetPosition(tObjNum, gObj[tObjNum].X, gObj[tObjNum].Y);
						}
					}
				}
			}
		}

		count++;

		if ( count > MAX_VIEWPORT-1 )
			break;
	}
}

void CMasterLevelSkillTreeSystem::MLS_SkillStrengthenMultiShot(int aIndex, CMagicInf *lpMagic, BYTE bAngle, int aTargetIndex)
{
	LPOBJECTSTRUCT lpObj = (LPOBJECTSTRUCT)&gObj[aIndex];

	this->MLS_SkillFrustrum3(aIndex, bAngle, 6.0f, 7.0f, 1.0f, 0);

	vec3_t p;
	Vector(0.f, 6.0f, 0.f, p);
	
	vec3_t Angle[4];
	float Matrix[3][4];
	vec3_t vLine[5];

	Vector(0.f, 0.f, 40.0f, Angle[0]);
	Vector(0.f, 0.f, 20.0f, Angle[1]);
	Vector(0.f, 0.f, 340.0f, Angle[2]);
	Vector(0.f, 0.f, 320.0f, Angle[3]);

	AngleMatrix(Angle[0], Matrix);
	VectorRotate(p, Matrix, vLine[0]);
	AngleMatrix(Angle[1], Matrix);
	VectorRotate(p, Matrix, vLine[1]);
	Vector(0.f, 6.0f, 0.f, vLine[2]);

	AngleMatrix(Angle[2], Matrix);
	VectorRotate(p, Matrix, vLine[3]);
	AngleMatrix(Angle[3], Matrix);
	VectorRotate(p, Matrix, vLine[4]);
	Vector(0.f, 0.f, (bAngle*360/(BYTE)255), Angle[0]);

	AngleMatrix((float*)Angle,Matrix);

	int vSkillLineX[5];
	int vSkillLineY[5];

	vec3_t vFrustrum[5];

	for(int i = 0; i < 5; i++)
	{
		VectorRotate(vLine[i], Matrix, vFrustrum[i]);
		vSkillLineX[i] = (int)vFrustrum[i][0]+lpObj->X;
		vSkillLineY[i] = (int)vFrustrum[i][1]+lpObj->Y;
	}

	int tObjNum;
	int count = 0;
	int HitCount = 0;
	int DuelIndex = lpObj->m_iDuelUser;
	BOOL EnableAttack;

	while( true )
	{
		if(lpObj->VpPlayer2[count].state != 0)
		{
			tObjNum = lpObj->VpPlayer2[count].number;
			if(tObjNum >= 0)
			{
				EnableAttack = FALSE;

				if( lpObj->VpPlayer2[count].type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon < 0 )
				{
					EnableAttack = TRUE;
				}
				else if( tObjNum == aTargetIndex || DuelIndex == tObjNum )
				{
					EnableAttack = TRUE;
				}

				else if ( g_ConfigRead.server.GetServerType() == SERVER_CASTLE && lpObj->VpPlayer2[count].type != OBJ_NPC && gObj[tObjNum].MapNumber == MAP_INDEX_CASTLESIEGE && g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE )
				{
					EnableAttack = TRUE;
				}

				else
				{
					int CallMonIndex = gObj[tObjNum].m_Index;

					if( gObj[tObjNum].Type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon >= 0 )
					{
						CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
					}

					if( gObjTargetGuildWarCheck(lpObj,&gObj[CallMonIndex]) == TRUE )
					{
						EnableAttack = TRUE;
					}

					if ( gObj[tObjNum].Class >= 678 && gObj[tObjNum].Class <= 680 )
					{
						EnableAttack = TRUE;
					}
				}

				if( EnableAttack )
				{
					int nResult[5];
					for(int i = 0; i < 5; i++)
					{
						nResult[i] = (((int)vSkillLineX[i]-lpObj->X) * (gObj[tObjNum].Y-lpObj->Y)) - (((int)vSkillLineY[i]-lpObj->Y) * (gObj[tObjNum].X-lpObj->X));
						if( SkillTestFrustrum(gObj[tObjNum].X,gObj[tObjNum].Y,aIndex) )
						{
							if(nResult[i] > -5 && nResult[i] < 5)
							{
								gObjAttack(lpObj, &gObj[tObjNum], lpMagic, 1, 0, 0, FALSE, 0, 0);
							}
						}
					}
				}
			}
		}
		count++;
		if(count > MAX_VIEWPORT-1)	
			break;
	}
}

void CMasterLevelSkillTreeSystem::MLS_SkillHellBust(int aIndex, int aTargetIndex, CMagicInf *lpMagic)
{
	if ( aTargetIndex == 395 )
	{
		this->MLS_SkillHellBustStart(aIndex, lpMagic);
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->m_PlayerData->SkillStrengthenHellFire2State == 0)
	{
		g_Log.Add("[%s][%s] [MLS] Nova skill didn't cast",lpObj->AccountID,lpObj->Name); //HermeX Fix
		return;
	}

	lpObj->m_PlayerData->SkillStrengthenHellFire2State = 0;
	int count = 0;
	lpObj->m_PlayerData->SkillStrengthenHellFire2Time = 0;

	LPOBJ lpTargetObj = &gObj[aTargetIndex];
	int DuelIndex = lpObj->m_iDuelUser;
	BOOL EnableAttack = FALSE;
	int tObjNum;

	while ( true )
	{
		if(lpObj->VpPlayer2[count].state != 0)
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if(tObjNum >= 0)
			{
				EnableAttack = 0;

				if(lpObj->VpPlayer2[count].type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon < 0)
				{
					EnableAttack = 1;
				}
				else if(aTargetIndex == tObjNum || DuelIndex == tObjNum)
				{
					EnableAttack = 1;
				}

				else if ( g_ConfigRead.server.GetServerType() == SERVER_CASTLE && lpObj->VpPlayer2[count].type != OBJ_NPC && gObj[tObjNum].MapNumber == MAP_INDEX_CASTLESIEGE && g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE )
				{
					EnableAttack = TRUE;
				}

				else
				{
					int CallMonIndex = gObj[tObjNum].m_Index;

					if(gObj[tObjNum].Type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon >= 0)
					{
						CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
					}

					if(gObjTargetGuildWarCheck(lpObj,&gObj[CallMonIndex])==1)
					{
						EnableAttack = 1;
					}

					if ( gObj[tObjNum].Class >= 678 && gObj[tObjNum].Class <= 680 )
					{
						EnableAttack = TRUE;
					}
				}

				if(EnableAttack != 0)
				{
					if(gObjCalDistance(lpObj,&gObj[tObjNum]) <= MagicDamageC.GetSkillDistance(lpMagic->m_Skill))
					{
						gObjAddAttackProcMsgSendDelay(lpObj,50,tObjNum,600,lpMagic->m_Skill,0);
					}
				}
			}
		}

		count++;
		if(count > MAX_VIEWPORT -1)
		{
			break;
		}
	}

	GSProtocol.GCMagicAttackNumberSend(lpObj,lpMagic->m_Skill,lpObj->m_Index,1);
}

void CMasterLevelSkillTreeSystem::MLS_SkillHellBustStart(int aIndex, CMagicInf * lpMagic)
{
	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->Type != OBJ_USER)
	{
		return;
	}

	if(lpObj->m_PlayerData->SkillStrengthenHellFire2State != 0)
	{
		g_Log.Add("[%s][%s] [MLS] Nova skill already casted",lpObj->AccountID,lpObj->Name);
		return;
	}

	lpObj->m_PlayerData->SkillStrengthenHellFire2Time = GetTickCount();
	lpObj->m_PlayerData->SkillStrengthenHellFire2State = 1;
	lpObj->m_PlayerData->SkillStrengthenHellFire2Count = 0;

	GSProtocol.GCMagicAttackNumberSend(lpObj,395,aIndex,1);
}

void CMasterLevelSkillTreeSystem::MLS_SkillMasteryWizardMagicDefense(LPOBJ lpObj, CMagicInf *lpMagic, int aTargetIndex)
{
	LPOBJ lpTargetObj = &gObj[aTargetIndex];

	if ( lpObj->Type != OBJ_USER )
	{
		return;
	}

	if (lpTargetObj->Type != OBJ_USER)
	{
		return;
	}

	if ( lpObj->m_RecallMon != -1 )
	{
		return;
	}

	if ( lpObj->Class != CLASS_WIZARD && lpObj->Class != CLASS_MAGUMSA )
	{
		return;
	}

	if ( lpObj->PartyNumber != lpTargetObj->PartyNumber )
	{
		return;
	}

	double fDefenseValue = 0.0;
	double fSkillValidTime = 0.0;
	
	this->m_Lua.Generic_Call("WizardMagicDefense_Level3", "iiii>dd", lpObj->m_Index, aTargetIndex, lpObj->AddDexterity + lpObj->m_PlayerData->Dexterity, lpObj->AddEnergy + lpObj->m_PlayerData->Energy, &fDefenseValue, &fSkillValidTime);
	
	fDefenseValue += this->GetBrandOfMasterSkillValue(lpObj, lpMagic->m_Skill, 2);
	fSkillValidTime += this->GetBrandOfMasterSkillValue(lpObj, lpMagic->m_Skill, 1);

	lpTargetObj->m_PlayerData->m_MPSkillOpt.iMpsMasManaRate_Wizard = this->GetMasterSkillValue(MagicDamageC.SkillGet(lpMagic->m_Skill), lpMagic->m_Level);

	gObjAddBuffEffect(lpTargetObj, BUFFTYPE_MELEE_DEFENSE_INC_STR, EFFECTTYPE_IMPROVE_MELEE_DEFENSE, fDefenseValue, EFFECTTYPE_MELEE_DEFENSE_DOWN_MANA, g_SkillAdditionInfo.GetSoulBarrierManaRate_Level(lpMagic->m_Skill), fSkillValidTime);
	GSProtocol.GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, aTargetIndex, TRUE);
}

void CMasterLevelSkillTreeSystem::MLS_SkillStrengthenWeakness(int aIndex, CMagicInf *lpMagic, BYTE x, BYTE y, int aTargetIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	int count = 0;
	int HitCount = 0;
	int DuelIndex = lpObj->m_iDuelUser;
	int tObjNum;
	BOOL EnableAttack;

	GSProtocol.GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, aTargetIndex, TRUE);

	while ( true )
	{
		if(lpObj->VpPlayer2[count].state != 0)
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if(tObjNum >= 0)
			{
				EnableAttack = FALSE;

				if(lpObj->VpPlayer2[count].type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon < 0)
				{
					EnableAttack = TRUE;
				}
				else if(DuelIndex == tObjNum)
				{
					EnableAttack = TRUE;
				}

				else if ( g_ConfigRead.server.GetServerType() == SERVER_CASTLE && lpObj->VpPlayer2[count].type != OBJ_NPC && gObj[tObjNum].MapNumber == MAP_INDEX_CASTLESIEGE && g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE )
				{
					EnableAttack = TRUE;
				}

				else if ( CC_MAP_RANGE(lpObj->MapNumber) == TRUE || lpObj->MapNumber == MAP_INDEX_CHAOSCASTLE_SURVIVAL )
				{
					EnableAttack = TRUE;
				}

				else if ( IT_MAP_RANGE(lpObj->MapNumber) == TRUE )
				{
					EnableAttack = TRUE;
				}

				else
				{
					int CallMonIndex = gObj[tObjNum].m_Index;

					if( gObj[tObjNum].Type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon >= 0 )
					{
						CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
					}

					if( gObjTargetGuildWarCheck(lpObj,&gObj[CallMonIndex]) == TRUE )
					{
						EnableAttack = TRUE;
					}

					if ( gObj[tObjNum].Class >= 678 && gObj[tObjNum].Class <= 680 )
					{
						EnableAttack = TRUE;
					}
				}

				if( EnableAttack )
				{
					if ( gObjCalDistance(lpObj, &gObj[tObjNum]) < 5 )
					{
						gObjAddAttackProcMsgSendDelay(lpObj, 50, tObjNum, 700, lpMagic->m_Skill, 0);
						HitCount++;

						if ( HitCount >= 5 )
							break;
					}
				}
			}
		}

		count++;

		if ( count > MAX_VIEWPORT-1 )
			break;
	}
}

void CMasterLevelSkillTreeSystem::MLS_SkillStrengthenEnervation(int aIndex, CMagicInf *lpMagic, BYTE x, BYTE y, int aTargetIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	int count = 0;
	int HitCount = 0;
	int DuelIndex = lpObj->m_iDuelUser;
	int tObjNum;
	BOOL EnableAttack;

	GSProtocol.GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, aTargetIndex, TRUE);

	while ( true )
	{
		if(lpObj->VpPlayer2[count].state != 0)
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if(tObjNum >= 0)
			{
				EnableAttack = FALSE;

				if(lpObj->VpPlayer2[count].type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon < 0)
				{
					EnableAttack = TRUE;
				}
				else if(DuelIndex == tObjNum)
				{
					EnableAttack = TRUE;
				}

				else if ( g_ConfigRead.server.GetServerType() == SERVER_CASTLE && lpObj->VpPlayer2[count].type != OBJ_NPC && gObj[tObjNum].MapNumber == MAP_INDEX_CASTLESIEGE && g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE )
				{
					EnableAttack = TRUE;
				}

				else if (CC_MAP_RANGE(lpObj->MapNumber) == TRUE || lpObj->MapNumber == MAP_INDEX_CHAOSCASTLE_SURVIVAL)
				{
					EnableAttack = TRUE;
				}

				else if ( IT_MAP_RANGE(lpObj->MapNumber) == TRUE )
				{
					EnableAttack = TRUE;
				}

				else
				{
					int CallMonIndex = gObj[tObjNum].m_Index;

					if( gObj[tObjNum].Type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon >= 0 )
					{
						CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
					}

					if( gObjTargetGuildWarCheck(lpObj,&gObj[CallMonIndex]) == TRUE )
					{
						EnableAttack = TRUE;
					}

					if ( gObj[tObjNum].Class >= 678 && gObj[tObjNum].Class <= 680 )
					{
						EnableAttack = TRUE;
					}
				}

				if( EnableAttack )
				{
					if ( gObjCalDistance(lpObj, &gObj[tObjNum]) < 5 )
					{
						gObjAddAttackProcMsgSendDelay(lpObj, 50, tObjNum, 700, lpMagic->m_Skill, 0);
						HitCount++;

						if ( HitCount >= 5 )
							break;
					}
				}
			}
		}

		count++;

		if ( count > MAX_VIEWPORT-1 )
			break;
	}
}

void CMasterLevelSkillTreeSystem::MLS_SkillMasteryIce(LPOBJ lpObj, CMagicInf* lpMagic, int aTargetIndex)
{
	if ( lpObj->Type != OBJ_USER )
	{
		return;
	}

	LPOBJ lpTargetObj = &gObj[aTargetIndex];
	float fValue = this->GetMasterSkillValue(MagicDamageC.SkillGet(lpMagic->m_Skill), lpMagic->m_Level);

	gObjAttack(lpObj, lpTargetObj, lpMagic, 1, 0, 0, 0, 0, 0);

	if ( fValue >= (rand()%100) )
	{
		BYTE btAttr = MapC[lpObj->MapNumber].GetAttr(lpObj->X, lpObj->Y);

		if ((btAttr & 1) != 1)
		{
			gObjAddBuffEffect(lpTargetObj, BUFFTYPE_FREEZE_2, 0, 0, 0, 0, 2);
			lpTargetObj->lpAttackObj = lpObj;
			lpTargetObj->PathCount = 0;
			lpTargetObj->PathStartEnd = 0;
			gObjSetPosition(aTargetIndex, lpTargetObj->X, lpTargetObj->Y);
		}
	}
}

void CMasterLevelSkillTreeSystem::MLS_SkillStrengthenFlameStrike(int aIndex, CMagicInf *lpMagic, BYTE bAngle, int aTargetIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	this->MLS_SkillFrustrum3(aIndex, bAngle, 2.0, 4.0, 5.0, 0.0);

	int count = 0;
	int HitCount = 0;
	int DuelIndex = lpObj->m_iDuelUser;
	int tObjNum;
	BOOL EnableAttack = FALSE;
	BOOL bAttack = FALSE;

	while ( true )
	{
		if ( lpObj->VpPlayer2[count].state != 0 )
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if ( tObjNum >= 0 )
			{
				EnableAttack = FALSE;

				if ( lpObj->VpPlayer2[count].type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon < 0 )
				{
					EnableAttack = TRUE;
				}

				else if ( tObjNum == aTargetIndex || DuelIndex == tObjNum )
				{
					EnableAttack = TRUE;
				}

				else if ( g_ConfigRead.server.GetServerType() == SERVER_CASTLE && lpObj->VpPlayer2[count].type != OBJ_NPC && gObj[tObjNum].MapNumber == MAP_INDEX_CASTLESIEGE && g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE )
				{
					EnableAttack = TRUE;
				}

				else
				{
					int CallMonIndex = gObj[tObjNum].m_Index;

					if ( gObj[tObjNum].Type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon >= 0 )
					{
						CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
					}

					if ( gObjTargetGuildWarCheck(lpObj,&gObj[CallMonIndex]) == TRUE )
					{
						EnableAttack = TRUE;
					}

					if ( gObj[tObjNum].Class >= 678 && gObj[tObjNum].Class <= 680 )
					{
						EnableAttack = TRUE;
					}
				}

				if ( EnableAttack == TRUE )
				{
					if ( SkillTestFrustrum(gObj[tObjNum].X, gObj[tObjNum].Y, aIndex) &&
						gObjCalDistance(lpObj, &gObj[tObjNum]) <= MagicDamageC.GetSkillDistance(lpMagic->m_Skill) )
					{
						bAttack = FALSE;

						if ( HitCount < 8 )
							bAttack = TRUE;

						else if ( rand()%2 )
							bAttack = TRUE;

						if ( bAttack )
						{
							gObjAttack(lpObj, &gObj[tObjNum], lpMagic, 1, 0, 0, 0, 0, 0);
							gObjAttack(lpObj, &gObj[tObjNum], lpMagic, 1, 0, 0, 0, 0, 0);
						}

						HitCount++;
					}
				}
			}
		}

		count++;
		if ( count > MAX_VIEWPORT-1 )
			break;
	}
}

void CMasterLevelSkillTreeSystem::MLS_SkillGiganticStorm(int aIndex, CMagicInf *lpMagic, BYTE x, BYTE y, int aTargetIndex)
{
	LPOBJ lpObj = &gObj[aIndex];
	int count = 0;
	int DuelIndex = lpObj->m_iDuelUser;
	int nHitCount = 0;
	BOOL bAttack = FALSE;
	int tObjNum;
	BOOL EnableAttack = FALSE;

	int nMinCount = 8;
	int nMaxCount = 12;

	if ( this->MLS_CalDistance(lpObj->X, lpObj->Y, x, y) <= MagicDamageC.GetSkillDistance(lpMagic->m_Skill) )
	{
		while ( true )
		{
			if ( lpObj->VpPlayer2[count].state != 0 )
			{
				tObjNum = lpObj->VpPlayer2[count].number;

				if ( tObjNum >= 0 )
				{
					EnableAttack = FALSE;

					if ( lpObj->VpPlayer2[count].type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon < 0 )
					{
						EnableAttack = TRUE;
					}

					else if ( tObjNum == aTargetIndex || DuelIndex == tObjNum )
					{
						EnableAttack = TRUE;
					}

					else if ( g_ConfigRead.server.GetServerType() == SERVER_CASTLE && lpObj->VpPlayer2[count].type != OBJ_NPC && gObj[tObjNum].MapNumber == MAP_INDEX_CASTLESIEGE && g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE )
					{
						EnableAttack = TRUE;
					}

					else
					{
						int CallMonIndex = gObj[tObjNum].m_Index;

						if ( gObj[tObjNum].Type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon >= 0 )
						{
							CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
						}

						if ( gObjTargetGuildWarCheck(lpObj,&gObj[CallMonIndex]) == TRUE )
						{
							EnableAttack = TRUE;
						}

						if ( gObj[tObjNum].Class >= 678 && gObj[tObjNum].Class <= 680 )
						{
							EnableAttack = TRUE;
						}
					}

					if ( EnableAttack == TRUE )
					{
						if ( gObj[tObjNum].X >= x - 6 
							&& gObj[tObjNum].X <= x + 6
							&& gObj[tObjNum].Y >= y - 6
							&& gObj[tObjNum].Y <= y + 6 )
						{
							gObj[tObjNum].lpAttackObj = lpObj;
							nHitCount++;

							if ( nHitCount <= nMinCount )
							{
								bAttack = TRUE;
							}

							else if ( rand()%2 )
							{
								bAttack = TRUE;
							}

							if ( nHitCount > nMaxCount )
							{
								bAttack = FALSE;
							}

							if ( bAttack == TRUE )
							{
								gObjAddAttackProcMsgSendDelay(lpObj, 50, tObjNum, 400, lpMagic->m_Skill, 0);
							}
						}
					}
				}
			}

			count++;

			if ( count > MAX_VIEWPORT-1 )
				break;
		}
	}
}

void CMasterLevelSkillTreeSystem::MLS_SkillMasteryDefenseDown(int aIndex, int aTargetIndex, CMagicInf *lpMagic)
{
	LPOBJ lpObj = &gObj[aIndex];
	LPOBJ lpTargetObj = &gObj[aTargetIndex];

	if ( lpObj->Type != OBJ_USER )
	{
		return;
	}

	if ( lpObj->Class != CLASS_MAGUMSA )
	{
		return;
	}

	if ( ObjectMaxRange(aTargetIndex) == false )
	{
		return;
	}

	float fDownValue = this->GetMasterSkillValue(MagicDamageC.SkillGet(lpMagic->m_Skill), lpMagic->m_Level);

	if ( retCalcSkillResistance(lpTargetObj, 1) )
	{
		gObjAddBuffEffect(lpTargetObj, BUFFTYPE_DECREASE_DEFENSE_RATE, EFFECTTYPE_DECREASE_DEFENSE, fDownValue, 0, 0, 10);
		GSProtocol.GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, aTargetIndex, TRUE);
	}
}

void CMasterLevelSkillTreeSystem::MLS_SkillMasteryAddCriticalDamage2(int aIndex, CMagicInf* lpMagic)
{
	LPOBJ lpObj = &gObj[aIndex];

	int number;
	int ApplyPartyIndex[MAX_USER_IN_PARTY];
	LPOBJ lpPartyObj;
	memset(ApplyPartyIndex,-1,sizeof(ApplyPartyIndex));
	
	int partynum = lpObj->PartyNumber;

	if(partynum != -1)
	{
		for(int n = 0; n < MAX_USER_IN_PARTY; n++)
		{
			number = gParty.m_PartyS[partynum].Number[n];

			if(number >= 0)
			{
				lpPartyObj = &gObj[number];

				if(lpObj->MapNumber == lpPartyObj->MapNumber)
				{
					if(gObjCalDistance(lpObj,&gObj[number]) < 10)
					{
						ApplyPartyIndex[n] = lpPartyObj->m_Index;
					}
				}
			}
		}
	}

	int addcriticaldamagevalue = 0;
	int SkillTime = 0;

	this->m_Lua.Generic_Call("DarkLordCriticalDamage_Level4", "ii>ii", lpObj->Leadership + lpObj->AddLeadership, lpObj->m_PlayerData->Energy + lpObj->AddEnergy, &addcriticaldamagevalue, &SkillTime);

	addcriticaldamagevalue += this->GetBrandOfMasterSkillValue(lpObj, lpMagic->m_Skill, 3);
	SkillTime += this->GetBrandOfMasterSkillValue(lpObj, lpMagic->m_Skill, 2);

	float fRate = this->GetBrandOfMasterSkillValue(lpObj, lpMagic->m_Skill, 1);

	if ( partynum == -1 )
	{
		lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncExcellentDamageRate_Darklord = this->GetMasterSkillValue(MagicDamageC.SkillGet(lpMagic->m_Skill), lpMagic->m_Level);
		gObjAddBuffEffect(lpObj, BUFFTYPE_CRITICAL_DMG_INC_MAS, EFFECTTYPE_CRITICAL_DMG, addcriticaldamagevalue, EFFECTTYPE_CRITICALDAMAGE, fRate, SkillTime);
		MsgOutput(aIndex, Lang.GetText(0,134), SkillTime);
		GSProtocol.GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, lpObj->m_Index, TRUE);
	}

	else
	{
		for(int n = 0;n < MAX_USER_IN_PARTY;n++)
		{
			if(ApplyPartyIndex[n] != -1)
			{
				lpPartyObj = &gObj[ApplyPartyIndex[n]];
				lpPartyObj->m_PlayerData->m_MPSkillOpt.iMpsIncExcellentDamageRate_Darklord = this->GetMasterSkillValue(MagicDamageC.SkillGet(lpMagic->m_Skill), lpMagic->m_Level);
				gObjAddBuffEffect(lpPartyObj, BUFFTYPE_CRITICAL_DMG_INC_MAS, EFFECTTYPE_CRITICAL_DMG, addcriticaldamagevalue, EFFECTTYPE_CRITICALDAMAGE, fRate, SkillTime);
				MsgOutput(ApplyPartyIndex[n], Lang.GetText(0,134), SkillTime);
				GSProtocol.GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, lpPartyObj->m_Index, TRUE);
			}
		}
	}
}

void CMasterLevelSkillTreeSystem::MLS_SkillFrustrum3(int aIndex, BYTE bAngle, float tx1, float ty1, float tx2, float ty2)
{
	LPOBJECTSTRUCT lpObj = (LPOBJECTSTRUCT)&gObj[aIndex];

	vec3_t p[4];
	Vector(-tx1 , ty1, 0.f,p[0]);
	Vector( tx1 , ty1, 0.f,p[1]);
	Vector( tx2, ty2, 0.f,p[2]);
	Vector(-tx2, ty2, 0.f,p[3]);

	vec3_t Angle;
	Vector(0.f,0.f,bAngle*360/(BYTE)255,Angle);

	float Matrix[3][4];
	vec3_t vFrustrum[4];
	
	AngleMatrix(Angle,Matrix);

	for(int i = 0; i < 4; i++)
	{
		VectorRotate(p[i],Matrix,vFrustrum[i]);

		lpObj->fSkillFrustrumX[i] = (int)vFrustrum[i][0] + lpObj->X;
		lpObj->fSkillFrustrumY[i] = (int)vFrustrum[i][1] + lpObj->Y;
	}
}

void CMasterLevelSkillTreeSystem::MLS_SkillStrengthenElectricSpark(int aIndex, CMagicInf* lpMagic, BYTE x, BYTE y, BYTE TargetPos, int aTargetIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	int TargePosx = lpObj->X - (8 - (TargetPos & 15));
	int TargePosy = lpObj->Y - (8 - ((TargetPos & 240) >> 4));

	int tObjNum;
	int count = 0;
	int DuelIndex = lpObj->m_iDuelUser;
	int iangle = this->MLS_GetAngle(lpObj->X,lpObj->Y,TargePosx,TargePosy);
	int delaytime;
	int Hit = 0;
	int EnableAttack;

	while ( true )
	{
		if(lpObj->VpPlayer2[count].state != 0)
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if(tObjNum >= 0)
			{
				EnableAttack = 0;

				if(lpObj->VpPlayer2[count].type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon < 0)
				{
					EnableAttack = 1;
				}
				else if(tObjNum == aTargetIndex || DuelIndex == tObjNum)
				{
					EnableAttack = 1;
				}

				else if ( g_ConfigRead.server.GetServerType() == SERVER_CASTLE && lpObj->VpPlayer2[count].type != OBJ_NPC && gObj[tObjNum].MapNumber == MAP_INDEX_CASTLESIEGE && g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE )
				{
					EnableAttack = TRUE;
				}

				else
				{
					int CallMonIndex = gObj[tObjNum].m_Index;

					if(gObj[tObjNum].Type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon >= 0)
					{
						CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
					}

					if(gObjTargetGuildWarCheck(lpObj,&gObj[CallMonIndex])==1)
					{
						EnableAttack = 1;
					}

					if ( gObj[tObjNum].Class >= 678 && gObj[tObjNum].Class <= 680 )
					{
						EnableAttack = TRUE;
					}
				}
				if(EnableAttack != 0)
				{
					if(SkillElectricSparkHitBox.HitCheck(iangle,gObj[aIndex].X,gObj[aIndex].Y,gObj[tObjNum].X,gObj[tObjNum].Y))
					{
						delaytime = 500;
						Hit = 1;

						gObjAddAttackProcMsgSendDelay(lpObj,50,tObjNum,delaytime,lpMagic->m_Skill,0);
					}
				}
			}
		}

		count++;
		if(count > MAX_VIEWPORT -1)
		{
			break;
		}
	}

	if(Hit != 0)
	{
		DecPartyMemberHPandMP(lpObj);
	}
}

void CMasterLevelSkillTreeSystem::MLS_SkillStrengthenchaoticDesair(int aIndex, CMagicInf* lpMagic, BYTE bAngle, int aTargetIndex)
{
	LPOBJECTSTRUCT lpObj = (LPOBJECTSTRUCT)&gObj[aIndex];

	int skillSuccess = 1;
	
	this->MLS_SkillFrustrum3(aIndex, bAngle, 1.5f, 6.0f, 1.5f, 0);

	int tObjNum;
	int count = 0;
	int HitCount = 0;
	BOOL bAttack;
	int DuelIndex = lpObj->m_iDuelUser;
	BOOL EnableAttack;

	while( true )
	{
		if(lpObj->VpPlayer2[count].state != 0)
		{
			tObjNum = lpObj->VpPlayer2[count].number;
			if(tObjNum >= 0)
			{
				EnableAttack = FALSE;

				if(lpObj->VpPlayer2[count].type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon < 0)
				{
					EnableAttack = TRUE;
				}
				else if(tObjNum == aTargetIndex || DuelIndex == tObjNum )
				{
					EnableAttack = TRUE;
				}

				else if ( g_ConfigRead.server.GetServerType() == SERVER_CASTLE && lpObj->VpPlayer2[count].type != OBJ_NPC && gObj[tObjNum].MapNumber == MAP_INDEX_CASTLESIEGE && g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE )
				{
					EnableAttack = TRUE;
				}

				else
				{
					int CallMonIndex = gObj[tObjNum].m_Index;

					if( gObj[tObjNum].Type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon >= 0 )
					{
						CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
					}

					if( gObjTargetGuildWarCheck(lpObj,&gObj[CallMonIndex]) == TRUE )
					{
						EnableAttack = TRUE;
					}

					if ( gObj[tObjNum].Class >= 678 && gObj[tObjNum].Class <= 680 )
					{
						EnableAttack = TRUE;
					}
				}

				if( EnableAttack )
				{
					if( SkillTestFrustrum(gObj[tObjNum].X,gObj[tObjNum].Y,aIndex) )
					{
						bAttack = FALSE;

						if(HitCount >= 8)
						{
							if(rand()%2)
							{
								bAttack = TRUE;
							}
						}
						else bAttack = TRUE;

						if( bAttack )
						{
							gObjAddAttackProcMsgSendDelay(lpObj, 50, tObjNum, 200, lpMagic->m_Skill, 0);
						}

						if(ObjectMaxRange(aTargetIndex) == true && tObjNum == aTargetIndex)
						{
							gObjAddAttackProcMsgSendDelay(lpObj, 50, tObjNum, 300, lpMagic->m_Skill, 0);
						}

						HitCount++;						
					}					
				}
			}
		}

		count++;
		if(count > MAX_VIEWPORT -1)	break;
	}
}

bool CMasterLevelSkillTreeSystem::MLS_WeaponSkillDel(LPOBJ lpObj, WORD aSkill, BYTE Level)
{
	for ( int iCount=0; iCount<MAX_MAGIC; iCount++ )
	{
		if ( lpObj->Magic[iCount].IsMagic() == TRUE )
		{
			if ( this->CheckMasterLevelSkill(lpObj->Magic[iCount].m_Skill) )
			{
				int iBaseMLS = this->GetBaseMasterLevelSkill(lpObj->Magic[iCount].m_Skill);

				if ( iBaseMLS > 0 )
				{
					int BrandSkill = MagicDamageC.GetNormalBrandOfSkill(iBaseMLS);

					if ( aSkill == BrandSkill )
					{
						if ( MagicDamageC.IsItemAttachedSkill(BrandSkill) && this->IsExistMLSBasedSkill(lpObj, iBaseMLS) == false )
						{
							GSProtocol.GCMagicListOneDelSend(lpObj->m_Index, iCount, lpObj->Magic[iCount].m_Skill, lpObj->Magic[iCount].m_Level, 0, 0);
							return true;
						}
					}
				}
			}
		}
	}

	return false;
}

bool CMasterLevelSkillTreeSystem::MLS_WeaponSkillAdd(LPOBJ lpObj, WORD aSkill, BYTE Level)
{
	for ( int iCount=0; iCount<MAX_MAGIC; iCount++ )
	{
		if ( lpObj->Magic[iCount].IsMagic() == TRUE )
		{
			if ( this->CheckMasterLevelSkill(lpObj->Magic[iCount].m_Skill) )
			{
				int iBaseMLS = this->GetBaseMasterLevelSkill(lpObj->Magic[iCount].m_Skill);

				if ( iBaseMLS > 0 )
				{
					if ( aSkill == MagicDamageC.GetNormalBrandOfSkill(iBaseMLS) &&
						!this->IsExistMLSBasedSkill(lpObj, iBaseMLS) )
					{
						GSProtocol.GCMagicListOneSend(lpObj->m_Index, iCount, lpObj->Magic[iCount].m_Skill, lpObj->Magic[iCount].m_Level, 0, 0);
						return true;
					}
				}
			}
		}
	}

	return false;
}

BOOL CMasterLevelSkillTreeSystem::CheckUsableWeaponSkill(LPOBJ lpObj, int nCount)
{
	if ( !this->CheckMasterLevelSkill(lpObj->Magic[nCount].m_Skill) )
	{
		return TRUE;
	}

	int iBaseMLS = this->GetBaseMasterLevelSkill(lpObj->Magic[nCount].m_Skill);

	if ( iBaseMLS <= 0 )
	{
		return TRUE;
	}

	if ( MagicDamageC.GetSkillUseType(iBaseMLS) != 4 )
	{
		return TRUE;
	}

	int BrandOfSkill = MagicDamageC.GetNormalBrandOfSkill(iBaseMLS);

	if ( MagicDamageC.IsItemAttachedSkill(lpObj->Magic[nCount].m_Skill) == false )
	{
		return TRUE;
	}

	if ( lpObj->pInventory[1].IsItem() == TRUE && lpObj->pInventory[1].m_Special[0] == BrandOfSkill )
	{
		return TRUE;
	}

	if ( lpObj->pInventory[0].IsItem() == TRUE && lpObj->pInventory[0].m_Special[0] == BrandOfSkill )
	{
		return TRUE;
	}

	return FALSE;

}

void CMasterLevelSkillTreeSystem::MLS_SkillSleep(int aIndex, int aTargetIndex, CMagicInf* lpMagic)
{
	LPOBJ lpObj = &gObj[aIndex];
	int result = 0;
	int tObjNum;
	int count = 0;
	int DuelIndex;
	BOOL bEnableAttack;

	GSProtocol.GCMagicAttackNumberSend(lpObj,lpMagic->m_Skill,aTargetIndex,1);

	DuelIndex = lpObj->m_iDuelUser;

	while( true )
	{
		if(lpObj->VpPlayer2[count].state != 0)
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if(tObjNum >= 0)
			{
				bEnableAttack = FALSE;

				if(lpObj->VpPlayer2[count].type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon < 0)
				{
					bEnableAttack = TRUE;
				}
				else if(tObjNum == aTargetIndex && DuelIndex == tObjNum)
				{
					bEnableAttack = TRUE;
				}
				else if (CC_MAP_RANGE(lpObj->MapNumber) != FALSE || IT_MAP_RANGE(lpObj->MapNumber) != FALSE || lpObj->MapNumber == MAP_INDEX_CHAOSCASTLE_SURVIVAL)
				{
					bEnableAttack = TRUE;
				}

				else if ( g_ConfigRead.server.GetServerType() == SERVER_CASTLE && lpObj->VpPlayer2[count].type != OBJ_NPC && gObj[tObjNum].MapNumber == MAP_INDEX_CASTLESIEGE && g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE )
				{
					bEnableAttack = TRUE;
				}

				else
				{
					int CallMonIndex = gObj[tObjNum].m_Index;

					if(gObj[tObjNum].Type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon >= 0)
					{
						CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
					}

					if(gObjTargetGuildWarCheck(lpObj,&gObj[CallMonIndex])==TRUE)
					{
						bEnableAttack = TRUE;
					}

					if ( gObj[tObjNum].Class >= 678 && gObj[tObjNum].Class <= 680 )
					{
						bEnableAttack = TRUE;
					}
				}

				if(bEnableAttack != FALSE)
				{
					if(tObjNum == aTargetIndex)
					{
						if(gCheckSkillDistance(aIndex, aTargetIndex, lpMagic->m_Skill) != FALSE)
						{
							gObjAttack(lpObj,&gObj[aTargetIndex],lpMagic,1,0,0,0,0,0);
							break;
						}
					}
				}
			}
		}

		count++;
			
		if(count > MAX_VIEWPORT -1)
		{
			break;
		}
	}
}

void CMasterLevelSkillTreeSystem::MLS_SkillChainLightning(int aIndex, int aTargetIndex, CMagicInf *lpMagic)
{
	if( ObjectMaxRange(aIndex) == false )	return;
	if( ObjectMaxRange(aTargetIndex) == false )	return;

	LPOBJECTSTRUCT lpObj = (LPOBJECTSTRUCT)&gObj[aIndex];
	LPOBJECTSTRUCT lpTargetObj = (LPOBJECTSTRUCT)&gObj[aTargetIndex];

	int tObjNum;
	int count = 0;
	int nChainTarget[3] = {-1, -1, -1};
	int DuelIndex;
	BOOL EnableAttack;

	GSProtocol.GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, aTargetIndex, 1);

	DuelIndex = lpObj->m_iDuelUser;

	if(gCheckSkillDistance(aIndex, aTargetIndex, lpMagic->m_Skill) == FALSE)
	{
		return;
	}

	nChainTarget[0] = aTargetIndex;

	while ( true )
	{
		if(lpObj->VpPlayer2[count].state != 0)
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if(tObjNum >= 0)
			{
				EnableAttack = FALSE;

				if(lpObj->VpPlayer2[count].type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon < 0	&& ((gObj[tObjNum].Class < 100 || gObj[tObjNum].Class > 110) || gObj[tObjNum].Class != 689) )
				{
					EnableAttack = TRUE;
				}

				else if(tObjNum == aTargetIndex || DuelIndex == tObjNum)
				{
					EnableAttack = TRUE;
				}

				else
				{
					int CallMonIndex = gObj[tObjNum].m_Index;

					if(gObj[tObjNum].Type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon >= 0)
					{
						CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
					}

					if(gObjTargetGuildWarCheck(lpObj,&gObj[CallMonIndex]) == TRUE)
					{
						EnableAttack = TRUE;
					}

					if ( gObj[tObjNum].Class >= 678 && gObj[tObjNum].Class <= 680 )
					{
						EnableAttack = TRUE;
					}
				}

				if( EnableAttack )
				{
					if( gObj[tObjNum].X >= lpTargetObj->X-1 && gObj[tObjNum].X <= lpTargetObj->X+1 && 
						gObj[tObjNum].Y >= lpTargetObj->Y-1 && gObj[tObjNum].Y <= lpTargetObj->Y+1 && 
						gObj[tObjNum].Type == OBJ_MONSTER)
                    {
                        nChainTarget[1] = tObjNum;
                        if(nChainTarget[2] != -1)	break;
                    }
					else if( gObj[tObjNum].X >= lpTargetObj->X-2 && gObj[tObjNum].X <= lpTargetObj->X+2 && 
							 gObj[tObjNum].Y >= lpTargetObj->Y-2 && gObj[tObjNum].Y <= lpTargetObj->Y+2 && 
							 gObj[tObjNum].Type == OBJ_MONSTER)
                    {
                        nChainTarget[2] = tObjNum;
                        if(nChainTarget[1] != -1)	break;
                    }
				}
			}
		}
		count++;
		if(count > MAX_VIEWPORT-1)	break;
	}

	if(nChainTarget[2] == -1)
	{
		nChainTarget[2] = nChainTarget[1];
	}

	if(nChainTarget[1] == -1)
	{
		nChainTarget[1] = nChainTarget[0];
		nChainTarget[2] = nChainTarget[1];
	}

	int nOffSet = 0;

	PMSG_CHAIN_MAGIC pMsg;
	pMsg.MagicNumberH = HIBYTE(lpMagic->m_Skill);
	pMsg.MagicNumberL = LOBYTE(lpMagic->m_Skill);
	pMsg.wUserIndex = aIndex;
	pMsg.nCount = 3;
	nOffSet += sizeof(pMsg);

	BYTE SendByte[256];

	PMSG_CHAIN_MAGIC_OBJECT	pMagicObj;

	for(int n = 0; n<3; n++)
	{
		pMagicObj.nTargetIndex = nChainTarget[n];
		memcpy(&SendByte[nOffSet], &pMagicObj, sizeof(PMSG_CHAIN_MAGIC_OBJECT));
		nOffSet += sizeof(PMSG_CHAIN_MAGIC_OBJECT);
	}

	PHeadSubSetB((LPBYTE)&pMsg, 0xBF, 0x0A, nOffSet);

	memcpy(SendByte, &pMsg, sizeof(pMsg));

    if(lpObj->Type == OBJ_USER)
        IOCP.DataSend(lpObj->m_Index, SendByte, pMsg.h.size);

    GSProtocol.MsgSendV2(lpObj, SendByte, pMsg.h.size);

    for(int n = 0; n<3; n++)
    {
        gObjAddAttackProcMsgSendDelay(lpObj, 50, nChainTarget[n], (n*200)+200, lpMagic->m_Skill, n+1);
    }
}

void CMasterLevelSkillTreeSystem::MLS_SkillLightningShock(int aIndex, int aTargetIndex, CMagicInf* lpMagic)
{
	LPOBJ lpObj = &gObj[aIndex];

	BOOL EnableAttack;

	for (int n = 0; n < MAX_VIEWPORT; n++)
	{
		EnableAttack = FALSE;

		if (lpObj->VpPlayer2[n].state <= 0)
		{
			continue;
		}

		int index = lpObj->VpPlayer2[n].number;

		if (gObj[index].Type == OBJ_MONSTER && gObj[index].m_RecallMon < 0)
		{
			EnableAttack = TRUE;
		}

		if (gObj[aIndex].Type == OBJ_MONSTER && gObj[index].Type == OBJ_USER) // DoppelGanger Fix
		{
			EnableAttack = TRUE;
		}

		if (aTargetIndex == index || index == lpObj->m_iDuelUser)
		{
			EnableAttack = TRUE;
		}

		if (CC_MAP_RANGE(lpObj->MapNumber) || IT_MAP_RANGE(lpObj->MapNumber) || lpObj->MapNumber == MAP_INDEX_CHAOSCASTLE_SURVIVAL)
		{
			EnableAttack = TRUE;
		}

		else if (g_ArcaBattle.IsArcaBattleServer() == TRUE)
		{
			EnableAttack = TRUE;
		}

		else if (g_ConfigRead.server.GetServerType() == SERVER_CASTLE)
		{
			if (gObj[index].Type != OBJ_NPC &&
				gObj[index].MapNumber == MAP_INDEX_CASTLESIEGE &&
				g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE)
			{
				EnableAttack = TRUE;
			}
		}

		int summon = aIndex;

		if (gObj[aIndex].Type == OBJ_MONSTER && gObj[aIndex].m_RecallMon >= 0)
		{
			summon = gObj[aIndex].m_RecallMon;
		}

		if (gObjTargetGuildWarCheck(&gObj[summon], lpObj) != 0)
		{
			EnableAttack = TRUE;
		}

		if ( gObj[index].Class >= 678 && gObj[index].Class <= 680 )
		{
			EnableAttack = TRUE;
		}

		if (gObjCalDistance(lpObj, &gObj[index]) <= 6 && EnableAttack == TRUE)
		{
			gObjAddAttackProcMsgSendDelay(lpObj, 50, index, 300, lpMagic->m_Skill, 0);
		}
	}
}

void CMasterLevelSkillTreeSystem::MLS_SkillDrainLife(int aIndex, int aTargetIndex, CMagicInf* lpMagic)
{
	if(ObjectMaxRange(aTargetIndex) == false)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];
	int tObjNum;
	int count = 0;

	GSProtocol.GCMagicAttackNumberSend(lpObj,lpMagic->m_Skill,aTargetIndex,1);

	int DuelIndex = lpObj->m_iDuelUser;
	BOOL EnableAttack;

	while ( true )
	{
		if(lpObj->VpPlayer2[count].state != 0)
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if(tObjNum >= 0)
			{
				EnableAttack = 0;

				if(lpObj->VpPlayer2[count].type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon < 0)
				{
					EnableAttack = 1;
				}
				else if(tObjNum == aTargetIndex || DuelIndex == tObjNum)
				{
					EnableAttack = 1;
				}
				else
				{
					int CallMonIndex = gObj[tObjNum].m_Index;

					if(gObj[tObjNum].Type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon >= 0)
					{
						CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
					}

					if(gObjTargetGuildWarCheck(lpObj,&gObj[CallMonIndex])==1)
					{
						EnableAttack = 1;
					}

					if ( gObj[tObjNum].Class >= 678 && gObj[tObjNum].Class <= 680 )
					{
						EnableAttack = TRUE;
					}
				}

				if(EnableAttack != 0)
				{
					if(tObjNum == aTargetIndex)
					{
						if(gCheckSkillDistance(aIndex, aTargetIndex, lpMagic->m_Skill) != FALSE)
						{
							gObjAddAttackProcMsgSendDelay(lpObj,50,tObjNum,700,lpMagic->m_Skill,0);
							break;
						}
					}
				}
			}
		}

		count++;
		if(count > MAX_VIEWPORT -1)
		{
			break;
		}
	}
}

// End of MLS functions ( Skills )

int CMasterLevelSkillTreeSystem::GetRequireMLPoint(int iClass, int iSkill)
{
	if ( iClass < 0 || iClass >= MAX_TYPE_PLAYER )
	{
		g_Log.Add("[MasterLevelSkillTreeSystem] GetRequireMLPoint - Invalid Class Code  : %d", iClass);
		return -1;
	}

	if ( iSkill < 0 || iSkill > 700 )
	{
		g_Log.Add("[MasterLevelSkillTreeSystem] GetRequireMLPoint - Invalid Skill Code  : %d", iSkill);
		return -1;
	}
	
	for ( int iTreeKind = 0; iTreeKind < MAX_TREE_TYPE; iTreeKind++ )
	{
		for ( int iRank = 0; iRank < MAX_MASTER_SKILL_RANK; iRank++ )
		{
			for ( int iPos = 0; iPos < MAX_SKILL_POS; iPos++ )
			{
				if ( this->m_MLSTable[iClass].iAbility[iTreeKind].iElement[iRank][iPos].m_dwSkillID == iSkill )
				{
					return this->m_MLSTable[iClass].iAbility[iTreeKind].iElement[iRank][iPos].m_byConsumePoint;
				}
			}
		}
	}

	return -1;
}

int CMasterLevelSkillTreeSystem::GetMaxPointOfMasterSkill(int iClass, int iSkill)
{
	if ( iClass < 0 || iClass >= MAX_TYPE_PLAYER )
	{
		g_Log.Add("[MasterLevelSkillTreeSystem] GetMaxPointOfMasterSkill - Invalid Class Code  : %d", iClass);
		return -1;
	}

	if ( iSkill < 0 || iSkill > 700 )
	{
		g_Log.Add("[MasterLevelSkillTreeSystem] GetMaxPointOfMasterSkill - Invalid Skill Code  : %d", iSkill);
		return -1;
	}
	
	for (int iTreeKind = 0; iTreeKind < MAX_TREE_TYPE; iTreeKind++)
	{
		for (int iRank = 0; iRank < MAX_MASTER_SKILL_RANK; iRank++)
		{
			for (int iPos = 0; iPos < MAX_SKILL_POS; iPos++)
			{
				if ( this->m_MLSTable[iClass].iAbility[iTreeKind].iElement[iRank][iPos].m_dwSkillID == iSkill )
				{
					return this->m_MLSTable[iClass].iAbility[iTreeKind].iElement[iRank][iPos].m_byMaxPoint;
				}
			}
		}
	}

	return -1;
}

float CMasterLevelSkillTreeSystem::GetMasterSkillValue(int iType, int iPoint)
{
	if ( iPoint < 0 || iPoint > this->GetMaxMasterSkillLevel() +1 )
	{
		g_Log.Add("[MasterLevelSkillTreeSystem] GetMasterSkillValue - Invalid Point : %d", iPoint);
		return 0.0;
	}

	if ( iType < 0 || iType > MAX_MASTER_SKILL_TYPE )
	{
		g_Log.Add("[MasterLevelSkillTreeSystem] GetMasterSkillValue - Invalid Type : %d", iType);
		return 0.0;
	}

	if (iPoint == this->GetMaxMasterSkillLevel() + 1 || iPoint == 0)
	{
		return 0.0;
	}

	return this->m_MLSValueTable[iType].fValueTable[iPoint];
}

float CMasterLevelSkillTreeSystem::GetMasterSkillManaIncRate(int iPoint)
{
	if ( iPoint < 1 || iPoint > this->GetMaxMasterSkillLevel())
	{
		g_Log.Add("[MasterLevelSkillTreeSystem] GetMasterSkillManaIncRate - Invalid Point : %d", iPoint);
		return -1.0;
	}

	for (int i = 0; i < MAX_MASTER_SKILL_TYPE; i++)
	{
		if (this->m_MLSValueTable[i].iValueType == MLST_VALUE_MANA_INC)
		{
			return this->m_MLSValueTable[i].fValueTable[iPoint];
		}
	}

	return -1.0;
}

float CMasterLevelSkillTreeSystem::GetBrandOfMasterSkillValue(LPOBJ lpObj, int iSkill, int iStep)
{
	int nStep = iStep;
	int nPrevSkill = iSkill;
	int nSkill = iSkill;

	while ( nStep && nPrevSkill )
	{
		WORD nBrandOfSkill = MagicDamageC.GetBrandOfSkill(nSkill);
		nPrevSkill = nBrandOfSkill;
		nSkill = nBrandOfSkill;
		nStep--;
	}

	if ( this->CheckMasterLevelSkill(nSkill) )
	{
		for ( int iCount=0; iCount<MAX_MAGIC; iCount++ )
		{
			if ( lpObj->Magic[iCount].IsMagic() == TRUE && lpObj->Magic[iCount].m_Skill == nSkill )
			{
				return this->GetMasterSkillValue(MagicDamageC.SkillGet(nSkill), lpObj->Magic[iCount].m_Level);
			}
		}
	}

	return 0.0;
}

int CMasterLevelSkillTreeSystem::CheckPreviousRankSkill(LPOBJ lpObj, int iSkill, int iLevel)
{
	if ( iSkill < 0 || iSkill > 700 )
	{
		g_Log.Add("[MasterLevelSkillTreeSystem] CheckPreviousRankSkill - Invalid Skill Code  : %d", iSkill);
		return -1;
	}

	int nTreeKind = -1;
	int nRank = MagicDamageC.GetSkillRank(iSkill) -1;

	if ( nRank < 1 )
	{
		return 1;
	}
	
	int iClass = lpObj->Class;

	for (int iTreeKind = 0; iTreeKind < MAX_TREE_TYPE; iTreeKind++)
	{
		for (int iRank = 0; iRank < MAX_MASTER_SKILL_RANK; iRank++)
		{
			for (int iPos = 0; iPos < MAX_SKILL_POS; iPos++)
			{
				if ( this->m_MLSTable[iClass].iAbility[iTreeKind].iElement[iRank][iPos].m_dwSkillID == iSkill )
				{
					nTreeKind = iTreeKind;
					break;
				}
			}
		}
	}

	int nPoint = 0;
	nRank--;
	
	for (int iPos = 0; iPos < MAX_SKILL_POS; iPos++)
	{
		for ( int i=0; i<MAX_MAGIC; i++ )
		{
			if ( lpObj->Magic[i].IsMagic() == TRUE &&
				this->CheckMasterLevelSkill(lpObj->Magic[i].m_Skill) == true )
			{
				if ( lpObj->Magic[i].m_Skill == this->m_MLSTable[iClass].iAbility[nTreeKind].iElement[nRank][iPos].m_dwSkillID )
				{
					nPoint |= lpObj->Magic[i].m_Level;
				}
			}
		}
	}

	return nPoint >= 10;
}

int CMasterLevelSkillTreeSystem::CheckSkillLevel(LPOBJ lpObj, int iSkill)
{
	for ( int i=0; i<MAX_MAGIC; i++ )
	{
		if ( lpObj->Magic[i].IsMagic() == TRUE && lpObj->Magic[i].m_Skill == iSkill )
		{
			return lpObj->Magic[i].m_Level;
		}
	}

	return 0;
}

int CMasterLevelSkillTreeSystem::GetMasterSkillUIIndex(int iClass, int iSkill)
{
	if ( iClass < 0 || iClass >= MAX_TYPE_PLAYER )
	{
		g_Log.Add("[MasterLevelSkillTreeSystem] GetMasterSkillUIIndex - Invalid Class Code  : %d", iClass);
		return -1;
	}

	if ( iSkill < 0 || iSkill > 700 )
	{
		g_Log.Add("[MasterLevelSkillTreeSystem] GetMasterSkillUIIndex - Invalid Skill Code  : %d", iSkill);
		return -1;
	}
	
	for (int iTreeKind = 0; iTreeKind < MAX_TREE_TYPE; iTreeKind++)
	{
		for (int iRank = 0; iRank < MAX_MASTER_SKILL_RANK; iRank++)
		{
			for (int iPos = 0; iPos < MAX_SKILL_POS; iPos++)
			{
				if ( this->m_MLSTable[iClass].iAbility[iTreeKind].iElement[iRank][iPos].m_dwSkillID == iSkill )
				{
					return this->m_MLSTable[iClass].iAbility[iTreeKind].iElement[iRank][iPos].m_dwIndex;
				}
			}
		}
	}

	return -1;
}

int CMasterLevelSkillTreeSystem::GetParentSkill1(int iClass, int iSkill)
{
	for (int iTreeKind = 0; iTreeKind < MAX_TREE_TYPE; iTreeKind++)
	{
		for (int iRank = 0; iRank < MAX_MASTER_SKILL_RANK; iRank++)
		{
			for (int iPos = 0; iPos < MAX_SKILL_POS; iPos++)
			{
				if ( this->m_MLSTable[iClass].iAbility[iTreeKind].iElement[iRank][iPos].m_dwSkillID == iSkill )
				{
					return this->m_MLSTable[iClass].iAbility[iTreeKind].iElement[iRank][iPos].m_dwParentSkill1;
				}
			}
		}
	}

	return 0;
}

int CMasterLevelSkillTreeSystem::GetParentSkill2(int iClass, int iSkill)
{
	for (int iTreeKind = 0; iTreeKind < MAX_TREE_TYPE; iTreeKind++)
	{
		for (int iRank = 0; iRank < MAX_MASTER_SKILL_RANK; iRank++)
		{
			for (int iPos = 0; iPos < MAX_SKILL_POS; iPos++)
			{
				if ( this->m_MLSTable[iClass].iAbility[iTreeKind].iElement[iRank][iPos].m_dwSkillID == iSkill )
				{
					return this->m_MLSTable[iClass].iAbility[iTreeKind].iElement[iRank][iPos].m_dwParentSkill2;
				}
			}
		}
	}

	return 0;
}

void CMasterLevelSkillTreeSystem::SetItemMLPassiveSkill(LPOBJ lpObj, int iType)
{
	switch ( iType )
	{
		case ITEM_TWOHAND_SWORD:
			{
				for ( int iCount=0; iCount<MAX_MAGIC; iCount++ )
				{
					if ( lpObj->Magic[iCount].IsMagic() == TRUE && lpObj->Magic[iCount].m_Skill == 348 )
					{
						lpObj->m_PlayerData->m_MPSkillOpt.iMpsTwoHandSwordAddDamage = this->GetMasterSkillValue(MagicDamageC.SkillGet(lpObj->Magic[iCount].m_Skill), lpObj->Magic[iCount].m_Level);
						break;
					}
				}

				for ( int iCount=0; iCount<MAX_MAGIC; iCount++ )
				{
					if ( lpObj->Magic[iCount].IsMagic() == TRUE && lpObj->Magic[iCount].m_Skill == 352 )
					{
						lpObj->m_PlayerData->m_MPSkillOpt.iMpsTwoHandsPvPAttack = this->GetMasterSkillValue(MagicDamageC.SkillGet(lpObj->Magic[iCount].m_Skill), lpObj->Magic[iCount].m_Level);
						return;
					}
				}
			}
			break;
		case ITEM_ONEHAND_SWORD:
			{
				for ( int iCount=0; iCount<MAX_MAGIC; iCount++ )
				{
					if ( lpObj->Magic[iCount].IsMagic() == TRUE && lpObj->Magic[iCount].m_Skill == 349 )
					{
						lpObj->m_PlayerData->m_MPSkillOpt.iMpsSwordAddDamage = this->GetMasterSkillValue(MagicDamageC.SkillGet(lpObj->Magic[iCount].m_Skill), lpObj->Magic[iCount].m_Level);
						break;
					}
				}

				for ( int iCount=0; iCount<MAX_MAGIC; iCount++ )
				{
					if ( lpObj->Magic[iCount].IsMagic() == TRUE && lpObj->Magic[iCount].m_Skill == 353 )
					{
						lpObj->m_PlayerData->m_MPSkillOpt.iMpsAddAttackSpeed = this->GetMasterSkillValue(MagicDamageC.SkillGet(lpObj->Magic[iCount].m_Skill), lpObj->Magic[iCount].m_Level);
						return;
					}
				}
			}
			break;
		case ITEM_MACE:
			{
				for ( int iCount=0; iCount<MAX_MAGIC; iCount++ )
				{
					if ( lpObj->Magic[iCount].IsMagic() == TRUE && lpObj->Magic[iCount].m_Skill == 350 )
					{
						lpObj->m_PlayerData->m_MPSkillOpt.iMpsScultAddDamage = this->GetMasterSkillValue(MagicDamageC.SkillGet(lpObj->Magic[iCount].m_Skill), lpObj->Magic[iCount].m_Level);
						break;
					}
				}

				for ( int iCount=0; iCount<MAX_MAGIC; iCount++ )
				{
					if ( lpObj->Magic[iCount].IsMagic() == TRUE && lpObj->Magic[iCount].m_Skill == 354 )
					{
						lpObj->m_PlayerData->m_MPSkillOpt.iMpsAddSturn = this->GetMasterSkillValue(MagicDamageC.SkillGet(lpObj->Magic[iCount].m_Skill), lpObj->Magic[iCount].m_Level);
						return;
					}
				}
			}
			break;
		case ITEM_SPEAR:
			{
				for ( int iCount=0; iCount<MAX_MAGIC; iCount++ )
				{
					if ( lpObj->Magic[iCount].IsMagic() == TRUE && lpObj->Magic[iCount].m_Skill == 351 )
					{
						lpObj->m_PlayerData->m_MPSkillOpt.iMpsSpearAddDamage = this->GetMasterSkillValue(MagicDamageC.SkillGet(lpObj->Magic[iCount].m_Skill), lpObj->Magic[iCount].m_Level);
						break;
					}
				}

				for ( int iCount=0; iCount<MAX_MAGIC; iCount++ )
				{
					if ( lpObj->Magic[iCount].IsMagic() == TRUE && lpObj->Magic[iCount].m_Skill == 355 )
					{
						lpObj->m_PlayerData->m_MPSkillOpt.iMpsAddDoubleDamage = this->GetMasterSkillValue(MagicDamageC.SkillGet(lpObj->Magic[iCount].m_Skill), lpObj->Magic[iCount].m_Level);
						return;
					}
				}
			}
			break;
		case ITEM_ONEHAND_STAFF:
			{
				for ( int iCount=0; iCount<MAX_MAGIC; iCount++ )
				{
					if ( lpObj->Magic[iCount].IsMagic() == TRUE && lpObj->Magic[iCount].m_Skill == 397 )
					{
						lpObj->m_PlayerData->m_MPSkillOpt.iMpsStaffMagicPower = this->GetMasterSkillValue(MagicDamageC.SkillGet(lpObj->Magic[iCount].m_Skill), lpObj->Magic[iCount].m_Level);
						break;
					}
				}

				for ( int iCount=0; iCount<MAX_MAGIC; iCount++ )
				{
					if ( lpObj->Magic[iCount].IsMagic() == TRUE && lpObj->Magic[iCount].m_Skill == 400 )
					{
						lpObj->m_PlayerData->m_MPSkillOpt.iMpsStaffAttackSpeed = this->GetMasterSkillValue(MagicDamageC.SkillGet(lpObj->Magic[iCount].m_Skill), lpObj->Magic[iCount].m_Level);
						return;
					}
				}
			}
			break;
		case ITEM_TWOHAND_STAFF:
			{
				for ( int iCount=0; iCount<MAX_MAGIC; iCount++ )
				{
					if ( lpObj->Magic[iCount].IsMagic() == TRUE && lpObj->Magic[iCount].m_Skill == 398 )
					{
						lpObj->m_PlayerData->m_MPSkillOpt.iMpsTwoHandsMagicPower = this->GetMasterSkillValue(MagicDamageC.SkillGet(lpObj->Magic[iCount].m_Skill), lpObj->Magic[iCount].m_Level);
						break;
					}
				}

				for ( int iCount=0; iCount<MAX_MAGIC; iCount++ )
				{
					if ( lpObj->Magic[iCount].IsMagic() == TRUE && lpObj->Magic[iCount].m_Skill == 401 )
					{
						lpObj->m_PlayerData->m_MPSkillOpt.iMpsTwoHandsStaffManAttack = this->GetMasterSkillValue(MagicDamageC.SkillGet(lpObj->Magic[iCount].m_Skill), lpObj->Magic[iCount].m_Level);
						return;
					}
				}
			}
			break;
		case ITEM_SHIELD:
			{
				for ( int iCount=0; iCount<MAX_MAGIC; iCount++ )
				{
					if ( lpObj->Magic[iCount].IsMagic() == TRUE )
					{
						if ( lpObj->Magic[iCount].m_Skill == 399 )
						{
							lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncShield = this->GetMasterSkillValue(MagicDamageC.SkillGet(lpObj->Magic[iCount].m_Skill), lpObj->Magic[iCount].m_Level);
							break;
						}

						if ( lpObj->Magic[iCount].m_Skill == 437 )
						{
							lpObj->m_PlayerData->m_MPSkillOpt.iMpsElfAddShield = this->GetMasterSkillValue(MagicDamageC.SkillGet(lpObj->Magic[iCount].m_Skill), lpObj->Magic[iCount].m_Level);
							break;
						}

						if ( lpObj->Magic[iCount].m_Skill == 528 )
						{
							lpObj->m_PlayerData->m_MPSkillOpt.iMpsAddShieldDefense = this->GetMasterSkillValue(MagicDamageC.SkillGet(lpObj->Magic[iCount].m_Skill), lpObj->Magic[iCount].m_Level);
							break;
						}
					}
				}

				for ( int iCount=0; iCount<MAX_MAGIC; iCount++ )
				{
					if ( lpObj->Magic[iCount].IsMagic() == TRUE )
					{
						if ( lpObj->Magic[iCount].m_Skill == 402 )
						{
							lpObj->m_PlayerData->m_MPSkillOpt.iMpsShieldBlockRate = this->GetMasterSkillValue(MagicDamageC.SkillGet(lpObj->Magic[iCount].m_Skill), lpObj->Magic[iCount].m_Level);
							return;
						}

						if ( lpObj->Magic[iCount].m_Skill == 440 )
						{
							lpObj->m_PlayerData->m_MPSkillOpt.iMpsElfShieldBlockRate = this->GetMasterSkillValue(MagicDamageC.SkillGet(lpObj->Magic[iCount].m_Skill), lpObj->Magic[iCount].m_Level);
							return;
						}

						if ( lpObj->Magic[iCount].m_Skill == 532 )
						{
							lpObj->m_PlayerData->m_MPSkillOpt.iMpsAddShieldBlockingRage = this->GetMasterSkillValue(MagicDamageC.SkillGet(lpObj->Magic[iCount].m_Skill), lpObj->Magic[iCount].m_Level);
							return;
						}
					}
				}
			}
			break;
		case ITEM_BOW:
			{
				for ( int iCount=0; iCount<MAX_MAGIC; iCount++ )
				{
					if ( lpObj->Magic[iCount].IsMagic() == TRUE && lpObj->Magic[iCount].m_Skill == 435 )
					{
						lpObj->m_PlayerData->m_MPSkillOpt.iMpsBowAddDamage = this->GetMasterSkillValue(MagicDamageC.SkillGet(lpObj->Magic[iCount].m_Skill), lpObj->Magic[iCount].m_Level);
						break;
					}
				}

				for ( int iCount=0; iCount<MAX_MAGIC; iCount++ )
				{
					if ( lpObj->Magic[iCount].IsMagic() == TRUE && lpObj->Magic[iCount].m_Skill == 438 )
					{
						lpObj->m_PlayerData->m_MPSkillOpt.iMpsBowAddAttackSpeed = this->GetMasterSkillValue(MagicDamageC.SkillGet(lpObj->Magic[iCount].m_Skill), lpObj->Magic[iCount].m_Level);
						return;
					}
				}
			}
			break;
		case ITEM_CROSSBOW:
			{
				for ( int iCount=0; iCount<MAX_MAGIC; iCount++ )
				{
					if ( lpObj->Magic[iCount].IsMagic() == TRUE && lpObj->Magic[iCount].m_Skill == 436 )
					{
						lpObj->m_PlayerData->m_MPSkillOpt.iMpsTwoHandsBowAddDamage = this->GetMasterSkillValue(MagicDamageC.SkillGet(lpObj->Magic[iCount].m_Skill), lpObj->Magic[iCount].m_Level);
						break;
					}
				}

				for ( int iCount=0; iCount<MAX_MAGIC; iCount++ )
				{
					if ( lpObj->Magic[iCount].IsMagic() == TRUE && lpObj->Magic[iCount].m_Skill == 439 )
					{
						lpObj->m_PlayerData->m_MPSkillOpt.iMpsTwoHandsBowManAttack = this->GetMasterSkillValue(MagicDamageC.SkillGet(lpObj->Magic[iCount].m_Skill), lpObj->Magic[iCount].m_Level);
						return;
					}
				}
			}
			break;
		case ITEM_SUMMONER_STAFF:
			{
				for ( int iCount=0; iCount<MAX_MAGIC; iCount++ )
				{
					if ( lpObj->Magic[iCount].IsMagic() == TRUE && lpObj->Magic[iCount].m_Skill == 465 )
					{
						lpObj->m_PlayerData->m_MPSkillOpt.iMpsAddMagicPowerStick = this->GetMasterSkillValue(MagicDamageC.SkillGet(lpObj->Magic[iCount].m_Skill), lpObj->Magic[iCount].m_Level);
						break;
					}
				}

				for ( int iCount=0; iCount<MAX_MAGIC; iCount++ )
				{
					if ( lpObj->Magic[iCount].IsMagic() == TRUE && lpObj->Magic[iCount].m_Skill == 467 )
					{
						lpObj->m_PlayerData->m_MPSkillOpt.iMpsAddManAttackStick = this->GetMasterSkillValue(MagicDamageC.SkillGet(lpObj->Magic[iCount].m_Skill), lpObj->Magic[iCount].m_Level);
						return;
					}
				}
			}
			break;
		case ITEM_SUMMONER_BOOK:
			{
				for ( int iCount=0; iCount<MAX_MAGIC; iCount++ )
				{
					if ( lpObj->Magic[iCount].IsMagic() == TRUE && lpObj->Magic[iCount].m_Skill == 466 )
					{
						lpObj->m_PlayerData->m_MPSkillOpt.iMpsAddCurseMagicBook = this->GetMasterSkillValue(MagicDamageC.SkillGet(lpObj->Magic[iCount].m_Skill), lpObj->Magic[iCount].m_Level);
						break;
					}
				}

				for ( int iCount=0; iCount<MAX_MAGIC; iCount++ )
				{
					if ( lpObj->Magic[iCount].IsMagic() == TRUE && lpObj->Magic[iCount].m_Skill == 468 )
					{
						lpObj->m_PlayerData->m_MPSkillOpt.iMpsAddAttackSpeedMagicBook = this->GetMasterSkillValue(MagicDamageC.SkillGet(lpObj->Magic[iCount].m_Skill), lpObj->Magic[iCount].m_Level);
						return;
					}
				}
			}
			break;
		case ITEM_DARKLORD_SCEPTER:
			{
				for ( int iCount=0; iCount<MAX_MAGIC; iCount++ )
				{
					if ( lpObj->Magic[iCount].IsMagic() == TRUE && lpObj->Magic[iCount].m_Skill == 527 )
					{
						lpObj->m_PlayerData->m_MPSkillOpt.iMpsAddCeptorDamage = this->GetMasterSkillValue(MagicDamageC.SkillGet(lpObj->Magic[iCount].m_Skill), lpObj->Magic[iCount].m_Level);
						break;
					}
				}

				for ( int iCount=0; iCount<MAX_MAGIC; iCount++ )
				{
					if ( lpObj->Magic[iCount].IsMagic() == TRUE && lpObj->Magic[iCount].m_Skill == 529 )
					{
						lpObj->m_PlayerData->m_MPSkillOpt.iMpsAddPetAttack = this->GetMasterSkillValue(MagicDamageC.SkillGet(lpObj->Magic[iCount].m_Skill), lpObj->Magic[iCount].m_Level);
						break;
					}
				}

				for ( int iCount=0; iCount<MAX_MAGIC; iCount++ )
				{
					if ( lpObj->Magic[iCount].IsMagic() == TRUE && lpObj->Magic[iCount].m_Skill == 531 )
					{
						lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncCeptorManAttack = this->GetMasterSkillValue(MagicDamageC.SkillGet(lpObj->Magic[iCount].m_Skill), lpObj->Magic[iCount].m_Level);
						break;
					}
				}

				for ( int iCount=0; iCount<MAX_MAGIC; iCount++ )
				{
					if ( lpObj->Magic[iCount].IsMagic() == TRUE && lpObj->Magic[iCount].m_Skill == 533 )
					{
						lpObj->m_PlayerData->m_MPSkillOpt.iMpsAddControlAttack = this->GetMasterSkillValue(MagicDamageC.SkillGet(lpObj->Magic[iCount].m_Skill), lpObj->Magic[iCount].m_Level);
						return;
					}
				}
			}
			break;
		case ITEM_RAGEFIGHTER_WEAPON:
			{
				for ( int iCount=0; iCount<MAX_MAGIC; iCount++ )
				{
					if ( lpObj->Magic[iCount].IsMagic() == TRUE && lpObj->Magic[iCount].m_Skill == 568 )
					{
						lpObj->m_PlayerData->m_MPSkillOpt.iMpsMonkWeaponDamage = this->GetMasterSkillValue(MagicDamageC.SkillGet(lpObj->Magic[iCount].m_Skill), lpObj->Magic[iCount].m_Level);
						break;
					}
				}

				for ( int iCount=0; iCount<MAX_MAGIC; iCount++ )
				{
					if ( lpObj->Magic[iCount].IsMagic() == TRUE && lpObj->Magic[iCount].m_Skill == 571 )
					{
						lpObj->m_PlayerData->m_MPSkillOpt.iMpsMonkDoubleDamage = this->GetMasterSkillValue(MagicDamageC.SkillGet(lpObj->Magic[iCount].m_Skill), lpObj->Magic[iCount].m_Level);
						return;
					}
				}
			}
			break;
		default:
			return;
	}
}

void CMasterLevelSkillTreeSystem::SetPetItemMLPassiveSkill(LPOBJ lpObj, int iType)
{
	if ( iType == ITEMGET(13,4) )
	{
		for ( int iCount=0; iCount<MAX_MAGIC; iCount++ )
		{
			if ( lpObj->Magic[iCount].IsMagic() == TRUE && lpObj->Magic[iCount].m_Skill == 510 )
			{
				lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncDarkHorseDefense = this->GetMasterSkillValue(MagicDamageC.SkillGet(lpObj->Magic[iCount].m_Skill), lpObj->Magic[iCount].m_Level);
				return;
			}
		}
	}
}

void CMasterLevelSkillTreeSystem::SetWingMLPassiveSkill(LPOBJ lpObj, int iType)
{
	switch ( iType )
	{
		case ITEMGET(12,36):
			{
				for ( int iCount=0; iCount<MAX_MAGIC; iCount++ )
				{
					if ( lpObj->Magic[iCount].IsMagic() == TRUE && lpObj->Magic[iCount].m_Skill == 324 )
					{
						lpObj->m_PlayerData->m_MPSkillOpt.iMpsAddWingDamage = this->GetMasterSkillValue(MagicDamageC.SkillGet(lpObj->Magic[iCount].m_Skill), lpObj->Magic[iCount].m_Level);
						return;
					}
				}
			}
			break;
		case ITEMGET(12,37):
			{
				for ( int iCount=0; iCount<MAX_MAGIC; iCount++ )
				{
					if ( lpObj->Magic[iCount].IsMagic() == TRUE && lpObj->Magic[iCount].m_Skill == 377 )
					{
						lpObj->m_PlayerData->m_MPSkillOpt.iMpsAddWingDamage = this->GetMasterSkillValue(MagicDamageC.SkillGet(lpObj->Magic[iCount].m_Skill), lpObj->Magic[iCount].m_Level);
						return;
					}
				}
			}
			break;
		case ITEMGET(12,38):
			{
				for ( int iCount=0; iCount<MAX_MAGIC; iCount++ )
				{
					if ( lpObj->Magic[iCount].IsMagic() == TRUE && lpObj->Magic[iCount].m_Skill == 412 )
					{
						lpObj->m_PlayerData->m_MPSkillOpt.iMpsAddWingDamage = this->GetMasterSkillValue(MagicDamageC.SkillGet(lpObj->Magic[iCount].m_Skill), lpObj->Magic[iCount].m_Level);
						return;
					}
				}
			}
			break;
		case ITEMGET(12,39):
			{
				for ( int iCount=0; iCount<MAX_MAGIC; iCount++ )
				{
					if ( lpObj->Magic[iCount].IsMagic() == TRUE && lpObj->Magic[iCount].m_Skill == 478 )
					{
						lpObj->m_PlayerData->m_MPSkillOpt.iMpsAddWingDamage = this->GetMasterSkillValue(MagicDamageC.SkillGet(lpObj->Magic[iCount].m_Skill), lpObj->Magic[iCount].m_Level);
						return;
					}
				}
			}
			break;
		case ITEMGET(12,40):
			{
				for ( int iCount=0; iCount<MAX_MAGIC; iCount++ )
				{
					if ( lpObj->Magic[iCount].IsMagic() == TRUE && lpObj->Magic[iCount].m_Skill == 507 )
					{
						lpObj->m_PlayerData->m_MPSkillOpt.iMpsAddWingDamage = this->GetMasterSkillValue(MagicDamageC.SkillGet(lpObj->Magic[iCount].m_Skill), lpObj->Magic[iCount].m_Level);
						return;
					}
				}
			}
			break;
		case ITEMGET(12,43):
			{
				for ( int iCount=0; iCount<MAX_MAGIC; iCount++ )
				{
					if ( lpObj->Magic[iCount].IsMagic() == TRUE && lpObj->Magic[iCount].m_Skill == 447 )
					{
						lpObj->m_PlayerData->m_MPSkillOpt.iMpsAddWingDamage = this->GetMasterSkillValue(MagicDamageC.SkillGet(lpObj->Magic[iCount].m_Skill), lpObj->Magic[iCount].m_Level);
						return;
					}
				}
			}
			break;
		case ITEMGET(12,50):
			{
				for ( int iCount=0; iCount<MAX_MAGIC; iCount++ )
				{
					if ( lpObj->Magic[iCount].IsMagic() == TRUE && lpObj->Magic[iCount].m_Skill == 550 )
					{
						lpObj->m_PlayerData->m_MPSkillOpt.iMpsAddWingDamage = this->GetMasterSkillValue(MagicDamageC.SkillGet(lpObj->Magic[iCount].m_Skill), lpObj->Magic[iCount].m_Level);
						return;
					}
				}
			}
			break;
		default:
			return;
	}
}

int CMasterLevelSkillTreeSystem::GetCurSkillLevel(LPOBJ lpObj, int iSkill)
{
	for ( int iCount=0; iCount<MAX_MAGIC; iCount++ )
	{
		if ( lpObj->Magic[iCount].IsMagic() == TRUE && lpObj->Magic[iCount].m_Skill == iSkill )
		{
			return lpObj->Magic[iCount].m_Level;
		}
	}

	return -1;
}

int CMasterLevelSkillTreeSystem::GetScriptClassCode(int iClass)
{
	if ( iClass < 0 || iClass >= MAX_TYPE_PLAYER )
	{
		g_Log.Add("[MasterLevelSkillTreeSystem] GetScriptClassCode - Invalid Class Code  : %d", iClass);
		return -1;
	}

	for ( int i=0;i<MAX_TYPE_PLAYER;i++ )
	{
		if ( CHARACTER_CLASS_TABLE[i][1] == iClass )
		{
			return CHARACTER_CLASS_TABLE[i][0];
		}
	}

	return -1;
}

float CMasterLevelSkillTreeSystem::RoundValue(long double dbValue, int iPos)
{
	char szPos[16];
	char szTemp[256];

	sprintf(szPos, "%%.%d", iPos);
	strcat(szPos, "f");
	sprintf(szTemp, szPos, dbValue);

	return atof(szTemp);
}

BYTE CMasterLevelSkillTreeSystem::ResetMasterSkill(int aIndex, int nTreeType)
{
	LPOBJ lpObj = &gObj[aIndex];

	if ( !lpObj )
	{
		return -1;
	}

	if ( nTreeType < 0 || nTreeType > 3 )
	{
		return -1;
	}

	int iClass = lpObj->Class;

	for ( int iCount = 0; iCount < MAX_MAGIC; iCount++ )
	{
		if ( lpObj->Magic[iCount].IsMagic() == TRUE && this->CheckMasterLevelSkill(lpObj->Magic[iCount].m_Skill) && 
			( nTreeType == 0 || this->CheckSkillInTree(iClass, lpObj->Magic[iCount].m_Skill, nTreeType -1)) )
		{
			if ( MagicDamageC.GetSkillUseType(lpObj->Magic[iCount].m_Skill) == 3 )
			{
				lpObj->m_PlayerData->MasterPoint += lpObj->Magic[iCount].m_Level;
				lpObj->Magic[iCount].Clear();
			}

			else
			{
				int iBrandOfSkill = MagicDamageC.GetBrandOfSkill(lpObj->Magic[iCount].m_Skill);

				if ( iBrandOfSkill > 0 )
				{
					if ( this->CheckMasterLevelSkill(iBrandOfSkill) )
					{
						lpObj->m_PlayerData->MasterPoint += lpObj->Magic[iCount].m_Level;
						lpObj->Magic[iCount].Clear();
					}

					else
					{
						CItem * lpLeftItem = &lpObj->pInventory[1];
						CItem * lpRightItem = &lpObj->pInventory[0];

						if ( lpLeftItem->m_Special[0] == iBrandOfSkill )
						{
							lpObj->m_PlayerData->MasterPoint += lpObj->Magic[iCount].m_Level;
							lpObj->Magic[iCount].UpdateMasterSkill(iBrandOfSkill, lpLeftItem->m_Level);
						}

						else if ( lpRightItem->m_Special[0] == iBrandOfSkill )
						{
							lpObj->m_PlayerData->MasterPoint += lpObj->Magic[iCount].m_Level;
							lpObj->Magic[iCount].UpdateMasterSkill(iBrandOfSkill, lpRightItem->m_Level);
						}

						else
						{
							lpObj->m_PlayerData->MasterPoint += lpObj->Magic[iCount].m_Level;
							lpObj->Magic[iCount].UpdateMasterSkill(iBrandOfSkill, 0);
						}
					}
				}

				else
				{
					lpObj->m_PlayerData->MasterPoint += lpObj->Magic[iCount].m_Level;
					lpObj->Magic[iCount].Clear();
				}
			}
		}
	}

	g_Log.Add("[%s][%s] Master Skill Reset (TreeType: %d)", lpObj->AccountID, lpObj->Name, nTreeType);
	gObjCloseSet(lpObj->m_Index, 1);
	
	return 0;
}

BYTE CMasterLevelSkillTreeSystem::CheckMasterSkillPoint(int aIndex, int nTreeType)
{
	LPOBJ lpObj = &gObj[aIndex];
	int nSkillCnt = 0;

	if ( !lpObj )
	{
		return -1;
	}

	if ( nTreeType < 0 || nTreeType > MAX_TREE_TYPE )
	{
		return -1;
	}

	int iClass = lpObj->Class;

	if ( nTreeType == 0 )
	{
		return 1;
	}

	for ( int n=0; n<MAX_MAGIC; n++ )
	{
		if ( lpObj->Magic[n].IsMagic() == TRUE &&
			this->CheckMasterLevelSkill(lpObj->Magic[n].m_Skill) == true && 
			nTreeType > 0 )
		{
			if ( this->CheckSkillInTree(iClass, lpObj->Magic[n].m_Skill, nTreeType - 1) ) 
			{
				nSkillCnt++;
			}
		}
	}

	return nSkillCnt > 0;
}

BYTE CMasterLevelSkillTreeSystem::CheckSkillInTree(int iClass, int iSkill, int iTreeKind)
{
	for (int iRank = 0; iRank < MAX_MASTER_SKILL_RANK; iRank++)
	{
		for (int iPos = 0; iPos < MAX_SKILL_POS; iPos++)
		{
			if (this->m_MLSTable[iClass].iAbility[iTreeKind].iElement[iRank][iPos].m_dwSkillID == iSkill)
			{
				return true;
			}
		}
	}

	return false;
}

float CMasterLevelSkillTreeSystem::GetSkillAttackDamage(LPOBJ lpObj, int iSkill)
{
	if ( !lpObj )
	{
		return 0.0;
	}

	if ( !this->CheckMasterLevelSkill(iSkill) )
	{
		return 0.0;
	}

	int nGroup = MagicDamageC.GetSkillGroup(iSkill);
	WORD nBrandOfSkill = iSkill;
	WORD iBrandOfSkill = 0;
	int nCnt = 0;
	BOOL bReturnAddDamage = FALSE;
	int iValueType;

	while ( this->CheckMasterLevelSkill(nBrandOfSkill) )
	{
		iBrandOfSkill = nBrandOfSkill;

		if ( MagicDamageC.GetSkillGroup(nBrandOfSkill) != nGroup )
		{
			iBrandOfSkill = 0;
			break;
		}

		iValueType = MagicDamageC.SkillGet(nBrandOfSkill);

		if ( this->m_MLSValueTable[iValueType].iValueType == MLST_VALUE_DAMAGE && nBrandOfSkill != 520 )
		{
			bReturnAddDamage = TRUE;
			break;
		}

		nBrandOfSkill = MagicDamageC.GetBrandOfSkill(nBrandOfSkill);
		nCnt++;

		if ( nCnt > 2 )
		{
			g_Log.Add("[GetSkillAttackDamage] fail!!! - %d", iSkill);
			iBrandOfSkill = 0;
			break;
		}
	}

	if ( iBrandOfSkill > 0 )
	{
		if ( bReturnAddDamage == TRUE )
		{
			for ( int n=0; n<MAX_MAGIC; n++)
			{
				if ( lpObj->Magic[n].IsMagic() == TRUE && lpObj->Magic[n].m_Skill == iBrandOfSkill )
				{
					return this->GetMasterSkillValue(iValueType, lpObj->Magic[n].m_Level);
				}
			}
		}
	}

	return 0.0;
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


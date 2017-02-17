//////////////////////////////////////////////////////////////////////
// MonsterStatCalc.cpp
#include "stdafx.h"
#include "MonsterStatCalc.h"
#include "TLog.h"
#include "MonsterAttr.h"
#include "GameMain.h"

CMonsterStatCalc g_MonsterStatCalc;

CMonsterStatCalc::CMonsterStatCalc(void)
{
}

CMonsterStatCalc::~CMonsterStatCalc(void)
{
}

void CMonsterStatCalc::LoadScript(LPSTR Filename)
{
	this->m_bLoad = false;

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(Filename);

	if ( res.status != pugi::status_ok )
	{
		g_Log.AddC(TColor::Red, "File load error: %s (%s)", Filename, res.description());
		return;
	}

	pugi::xml_node main = file.child("ImperialGuardian");
	_stMonsterStatRef monsterStatRef;

	for (pugi::xml_node monster = main.child("Monster"); monster; monster = monster.next_sibling())
	{
		monsterStatRef.m_nMinLevel = monster.attribute("PlayerMinLevel").as_int();
		monsterStatRef.m_nMaxLevel = monster.attribute("PlayerMaxLevel").as_int();
		monsterStatRef.m_nLevel = monster.attribute("Level").as_int();
		monsterStatRef.m_nHp = monster.attribute("HealthPoint").as_int();
		monsterStatRef.m_nMp = monster.attribute("ManaPoint").as_int();
		monsterStatRef.m_nDamageMin = monster.attribute("DamageMin").as_int();
		monsterStatRef.m_nDamageMax = monster.attribute("DamageMax").as_int();
		monsterStatRef.m_nDefense = monster.attribute("Defense").as_int();
		monsterStatRef.m_nMagicDefense = monster.attribute("MagicDefense").as_int();
		monsterStatRef.m_nAttackRating = monster.attribute("AttackRate").as_int();
		monsterStatRef.m_nSuccessfulblocking = monster.attribute("SuccessfulBlocking").as_int();

		this->m_vtMonsterStatRefAttr.push_back(monsterStatRef);
	}

	this->m_bLoad = true;

	g_Log.Add("%s file load", Filename);
}

BOOL CMonsterStatCalc::ConvertMonsterStat(LPOBJ lpObj, int nUserMaxLevel)
{
	if ( this->m_bLoad == FALSE )
	{
		return FALSE;
	}

	if ( lpObj == NULL )
	{
		return FALSE;
	}

	LPMONSTER_ATTRIBUTE lpAttr = gMAttr.GetAttr(lpObj->Class);

	if ( lpAttr == NULL )
	{
		return FALSE;
	}

	if ( lpAttr->m_Level == 0 )
	{
		return FALSE;
	}

	int nSize = this->m_vtMonsterStatRefAttr.size();
	_stMonsterStatRef monsterStatRef;

	for ( int i=0; i<nSize; i++ )
	{
		monsterStatRef = this->m_vtMonsterStatRefAttr.at(i);

		if ( nUserMaxLevel >= monsterStatRef.m_nMinLevel && nUserMaxLevel <= monsterStatRef.m_nMaxLevel )
		{
			strncpy(lpObj->Name, lpAttr->m_Name, MAX_ACCOUNT_LEN);
			lpObj->Level = lpAttr->m_Level * monsterStatRef.m_nLevel;
			lpObj->Life = lpAttr->m_Hp * monsterStatRef.m_nHp;
			lpObj->MaxLife = lpAttr->m_Hp * monsterStatRef.m_nHp;
			lpObj->Mana = lpAttr->m_Mp * monsterStatRef.m_nMp;
			lpObj->MaxMana = lpAttr->m_Mp * monsterStatRef.m_nMp;
			lpObj->m_AttackDamageMin = lpAttr->m_DamageMin * monsterStatRef.m_nDamageMin;
			lpObj->m_AttackDamageMax = lpAttr->m_DamageMax * monsterStatRef.m_nDamageMax;
			lpObj->m_Defense = lpAttr->m_Defense * monsterStatRef.m_nDefense;
			lpObj->m_MagicDefense = lpAttr->m_MagicDefense * monsterStatRef.m_nMagicDefense;
			lpObj->m_AttackRating = lpAttr->m_AttackRating * monsterStatRef.m_nAttackRating;
			lpObj->m_SuccessfulBlocking = lpAttr->m_Successfulblocking * monsterStatRef.m_nSuccessfulblocking;
			lpObj->m_MoveRange = lpAttr->m_MoveRange;
			lpObj->m_AttackSpeed = lpAttr->m_AttackSpeed;
			lpObj->m_MoveSpeed = lpAttr->m_MoveSpeed;
			lpObj->MaxRegenTime = 1000 * lpAttr->m_RegenTime;
			lpObj->m_AttackRange = lpAttr->m_AttackRange;
			lpObj->m_ViewRange = lpAttr->m_ViewRange;
			lpObj->m_Attribute = lpAttr->m_Attribute;
			lpObj->m_AttackType = lpAttr->m_AttackType;
			lpObj->m_ItemRate = lpAttr->m_ItemRate;
			lpObj->m_MoneyRate = lpAttr->m_MoneyRate;
			lpObj->m_Resistance[R_ICE] = lpAttr->m_Resistance[R_ICE];
			lpObj->m_Resistance[R_POISON] = lpAttr->m_Resistance[R_POISON];
			lpObj->m_Resistance[R_LIGHTNING] = lpAttr->m_Resistance[R_LIGHTNING];
			lpObj->m_Resistance[R_FIRE] = lpAttr->m_Resistance[R_FIRE];
			lpObj->m_iScriptMaxLife = lpAttr->m_iScriptHP;
			lpObj->m_iPentagramMainAttribute = lpAttr->m_PentagramMainAttribute;
			lpObj->m_iPentagramAttributePattern = lpAttr->m_PentagramAttributePattern;
			lpObj->m_iPentagramDefense = lpAttr->m_PentagramDefense;
			lpObj->m_iPentagramAttackMin = lpAttr->m_PentagramAttackMin;
			lpObj->m_iPentagramAttackMax = lpAttr->m_PentagramAttackMax;
			lpObj->m_iPentagramAttackRating = lpAttr->m_PentagramAttackRating;
			lpObj->m_iPentagramDefenseRating = lpAttr->m_PentagramDefense;
			lpObj->m_iBasicAI = lpAttr->m_iAINumber;
			lpObj->m_iCurrentAI = lpAttr->m_iAINumber;
			lpObj->m_iCurrentAIState = 0;
			lpObj->m_iLastAIRunTime = 0;
			lpObj->m_iGroupNumber = 0;
			lpObj->m_iSubGroupNumber = 0;
			lpObj->m_iGroupMemberGuid = -1;
			lpObj->m_iRegenType = 0;

			lpObj->m_Agro->ResetAll();

			lpObj->m_iLastAutomataRuntime = 0;
			lpObj->m_iLastAutomataDelay = 0;

			memcpy(lpObj->m_Resistance, lpAttr->m_Resistance, MAX_RESISTENCE_TYPE );

			return TRUE;
		}
	}
	
	return FALSE;
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


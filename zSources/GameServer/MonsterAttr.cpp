//////////////////////////////////////////////////////////////////////
// MonsterAttr.cpp
#include "stdafx.h"
#include "MonsterAttr.h"
#include "TLog.h"
#include "user.h"

// GS-N 0.99.60T 0x00418C10 - Completed - Just few disorders with same result int LoadAttr
//	GS-N	1.00.18	JPN	0x0041F300	-	Completed

CMonsterAttr::CMonsterAttr()
{
	return;
}

CMonsterAttr::~CMonsterAttr()
{
	return;
}

LPMONSTER_ATTRIBUTE CMonsterAttr::GetAttr(int aClass)
{
	for ( int n = 0; n<MAX_MONSTER_TYPE;n++)
	{
		if ( this->m_MonsterAttr[n].m_Index == aClass )
		{
			return &this->m_MonsterAttr[n];
		}
	}

	return NULL;
}

void CMonsterAttr::LoadAttr(char* filename)
{
	pugi::xml_document file;
	pugi::xml_parse_result result = file.load_file(filename);

	if (result.status != pugi::status_ok)
	{
		g_Log.MsgBox("load error %s (%s)", filename, result.description());
		return;
	}

	pugi::xml_node main_section = file.child("MonsterList");
	int nCount = 0;

	for (pugi::xml_node monster = main_section.child("Monster"); monster; monster = monster.next_sibling())
	{
		this->m_MonsterAttr[nCount].m_Index = monster.attribute("Index").as_int(-1);
		this->m_MonsterAttr[nCount].m_btExpType = monster.attribute("ExpType").as_int(0);
		strcpy(this->m_MonsterAttr[nCount].m_Name, monster.attribute("Name").as_string(""));
		this->m_MonsterAttr[nCount].m_Level = monster.attribute("Level").as_int(0);
		this->m_MonsterAttr[nCount].m_Hp = monster.attribute("HP").as_int(0);
		this->m_MonsterAttr[nCount].m_Mp = monster.attribute("MP").as_int(0);
		this->m_MonsterAttr[nCount].m_DamageMin = monster.attribute("DamageMin").as_int(0);
		this->m_MonsterAttr[nCount].m_DamageMax = monster.attribute("DamageMax").as_int(0);
		this->m_MonsterAttr[nCount].m_Defense = monster.attribute("Defense").as_int(0);
		this->m_MonsterAttr[nCount].m_MagicDefense = monster.attribute("MagicDefense").as_int(0);
		this->m_MonsterAttr[nCount].m_AttackRating = monster.attribute("AttackRate").as_int(0);
		this->m_MonsterAttr[nCount].m_Successfulblocking = monster.attribute("BlockRate").as_int(0);
		this->m_MonsterAttr[nCount].m_MoveRange = monster.attribute("MoveRange").as_int(0);
		this->m_MonsterAttr[nCount].m_AttackType = monster.attribute("AttackType").as_int(0);
		this->m_MonsterAttr[nCount].m_AttackRange = monster.attribute("AttackRange").as_int(0);
		this->m_MonsterAttr[nCount].m_ViewRange = monster.attribute("ViewRange").as_int(0);
		this->m_MonsterAttr[nCount].m_MoveSpeed = monster.attribute("MoveSpeed").as_int(0);
		this->m_MonsterAttr[nCount].m_AttackSpeed = monster.attribute("AttackSpeed").as_int(0);
		this->m_MonsterAttr[nCount].m_RegenTime = monster.attribute("RegenTime").as_int(0);
		this->m_MonsterAttr[nCount].m_Attribute = monster.attribute("Attribute").as_int(0);
		this->m_MonsterAttr[nCount].m_ItemRate = monster.attribute("ItemDropRate").as_int(0);
		this->m_MonsterAttr[nCount].m_MoneyRate = monster.attribute("MoneyDropRate").as_int(0);
		this->m_MonsterAttr[nCount].m_MaxItemLevel = monster.attribute("MaxItemLevel").as_int(0);
		this->m_MonsterAttr[nCount].m_MonsterSkill = monster.attribute("MonsterSkill").as_int(0);
		this->m_MonsterAttr[nCount].m_Resistance[R_ICE] = monster.attribute("IceRes").as_int(0);
		this->m_MonsterAttr[nCount].m_Resistance[R_POISON] = monster.attribute("PoisonRes").as_int(0);
		this->m_MonsterAttr[nCount].m_Resistance[R_LIGHTNING] = monster.attribute("LightRes").as_int(0);
		this->m_MonsterAttr[nCount].m_Resistance[R_FIRE] = monster.attribute("FireRes").as_int(0);
		this->m_MonsterAttr[nCount].m_PentagramMainAttribute = monster.attribute("PentagramMainAttrib").as_int(0);
		this->m_MonsterAttr[nCount].m_PentagramAttributePattern = monster.attribute("PentagramAttribPattern").as_int(0);
		this->m_MonsterAttr[nCount].m_PentagramAttackMin = monster.attribute("PentagramDamageMin").as_int(0);
		this->m_MonsterAttr[nCount].m_PentagramAttackMax = monster.attribute("PentagramDamageMax").as_int(0);
		this->m_MonsterAttr[nCount].m_PentagramAttackRating = monster.attribute("PentagramAttackRate").as_int(0);
		this->m_MonsterAttr[nCount].m_PentagramDefenseRating = monster.attribute("PentagramDefenseRate").as_int(0);
		this->m_MonsterAttr[nCount].m_PentagramDefense = monster.attribute("PentagramDefense").as_int(0);

		if (this->m_MonsterAttr[nCount].m_PentagramMainAttribute > 0)
		{
			this->m_MonsterAttr[nCount].m_Resistance[R_ICE] = 0;
			this->m_MonsterAttr[nCount].m_Resistance[R_POISON] = 0;
			this->m_MonsterAttr[nCount].m_Resistance[R_LIGHTNING] = 0;
			this->m_MonsterAttr[nCount].m_Resistance[R_FIRE] = 0;
		}

		this->m_MonsterAttr[nCount].m_iScriptHP = this->m_MonsterAttr[nCount].m_Hp;

		nCount++;
	}
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


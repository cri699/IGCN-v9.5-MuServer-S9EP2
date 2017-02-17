//////////////////////////////////////////////////////////////////////
// SkillSpecialize.h
#ifndef SKILLSPECIALIZE_H
#define SKILLSPECIALIZE_H

#include "user.h"
#include "MuLua.h"

#pragma once
class CSkillSpecialize
{
public:
	CSkillSpecialize(void);
	virtual ~CSkillSpecialize(void);

	void LoadScript();
	int CalcStatBonus(OBJECTSTRUCT *lpObj, int iSkill);

private:
	MULua m_Lua;
};

extern CSkillSpecialize g_SkillSpec;

#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


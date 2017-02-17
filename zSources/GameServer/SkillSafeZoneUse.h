//////////////////////////////////////////////////////////////////////
// SkillSafeZoneUse.h
#ifndef SKILL_SAFEZONE_USE_H
#define SKILL_SAFEZONE_USE_H

#include "MagicDamage.h"
#include "MapClass.h"

struct SKILL_SAFEZONE_USE_AREA
{
	BYTE btMapNumber;
	BYTE StartX;
	BYTE StartY;
	BYTE EndX;
	BYTE EndY;
};

struct SKILL_SAFEZONE_USE
{
	WORD wSkill;
	std::vector<SKILL_SAFEZONE_USE_AREA> m_vtArea;
};

#pragma once
class CSkillSafeZoneUse
{
public:
	CSkillSafeZoneUse();
	virtual ~CSkillSafeZoneUse();

	void LoadFile(char *szFileName);
	bool CanUseSkill(LPOBJ lpObj, CMagicInf * lpMagic);

private:
	bool m_bFileLoad;
	std::vector<SKILL_SAFEZONE_USE> m_vtSkillData;
};

extern CSkillSafeZoneUse g_SkillSafeZone;

#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


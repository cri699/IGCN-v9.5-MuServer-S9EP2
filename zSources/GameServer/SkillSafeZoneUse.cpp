//////////////////////////////////////////////////////////////////////
// SkillSafeZoneUse.cpp
#include "stdafx.h"
#include "SkillSafeZoneUse.h"
#include "user.h"
#include "MagicInf.h"

CSkillSafeZoneUse g_SkillSafeZone;

CSkillSafeZoneUse::CSkillSafeZoneUse()
{
	this->m_bFileLoad = false;
}

CSkillSafeZoneUse::~CSkillSafeZoneUse()
{
}

void CSkillSafeZoneUse::LoadFile(char *szFileName)
{
	this->m_bFileLoad = false;

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(szFileName);

	if (res.status != pugi::status_ok)
	{
		g_Log.MsgBox("Error loading %s file (%s)", szFileName, res.description());
		return;
	}

	pugi::xml_node main = file.child("SkillUseArea");
	this->m_vtSkillData.clear();

	for (pugi::xml_node skill = main.child("Skill"); skill; skill = skill.next_sibling())
	{
		SKILL_SAFEZONE_USE m_Skill;

		m_Skill.wSkill = skill.attribute("ID").as_int();

		for (pugi::xml_node area = skill.child("Area"); area; area = area.next_sibling())
		{
			SKILL_SAFEZONE_USE_AREA m_Area;

			m_Area.btMapNumber = area.attribute("MapNumber").as_int();
			m_Area.StartX = area.attribute("StartX").as_int();
			m_Area.EndX = area.attribute("EndX").as_int();
			m_Area.StartY = area.attribute("StartY").as_int();
			m_Area.EndY = area.attribute("EndY").as_int();

			m_Skill.m_vtArea.push_back(m_Area);
		}

		this->m_vtSkillData.push_back(m_Skill);
	}

	this->m_bFileLoad = true;
}

bool CSkillSafeZoneUse::CanUseSkill(LPOBJ lpObj, CMagicInf * lpMagic)
{
	if (this->m_bFileLoad == false)
	{
		return true;
	}

	for (std::vector<SKILL_SAFEZONE_USE>::iterator It = this->m_vtSkillData.begin(); It != this->m_vtSkillData.end(); It++)
	{
		if (It->wSkill == lpMagic->m_Skill)
		{
			for (std::vector<SKILL_SAFEZONE_USE_AREA>::iterator It2 = It->m_vtArea.begin(); It2 != It->m_vtArea.end(); It2++)
			{
				if (It2->btMapNumber == lpObj->MapNumber && lpObj->X >= It2->StartX && lpObj->X <= It2->EndX && lpObj->Y >= It2->StartY && lpObj->Y <= It2->EndY)
				{
					return true;
				}
			}
		}
	}

	return false;
}


//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


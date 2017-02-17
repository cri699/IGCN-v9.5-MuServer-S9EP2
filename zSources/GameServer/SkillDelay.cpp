// SkillDelay.cpp: implementation of the CSkillDelay class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SkillDelay.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSkillDelay::CSkillDelay()
{
	Init();
}

CSkillDelay::~CSkillDelay()
{

}

void CSkillDelay::Init()
{
	memset(LastSkillUseTime, 0, sizeof(LastSkillUseTime));
}

BOOL CSkillDelay::Check(WORD skill)
{
	int skilldelaytime = MagicDamageC.GetDelayTime(skill);

	if (skilldelaytime == 0)
	{
		return TRUE;
	}

	ULONGLONG dwtime = GetTickCount64();

	if (skilldelaytime + this->LastSkillUseTime[skill] >= dwtime)
	{
		return FALSE;
	}

	else
	{
		this->LastSkillUseTime[skill] = dwtime;
		return TRUE;
	}
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


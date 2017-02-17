// SkillDelay.h: interface for the CSkillDelay class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SKILLDELAY_H__B49397E0_3335_4B10_A115_29A2ACE442D5__INCLUDED_)
#define AFX_SKILLDELAY_H__B49397E0_3335_4B10_A115_29A2ACE442D5__INCLUDED_

#include "MagicDamage.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSkillDelay  
{
	ULONGLONG LastSkillUseTime[MAX_SKILL];
public:
	CSkillDelay();
	virtual ~CSkillDelay();

	void Init();
	int Check(WORD skill);
};

#endif // !defined(AFX_SKILLDELAY_H__B49397E0_3335_4B10_A115_29A2ACE442D5__INCLUDED_)


//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


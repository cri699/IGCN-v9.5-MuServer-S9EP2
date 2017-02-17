//////////////////////////////////////////////////////////////////////
// MagicInf.h
#ifndef MAGICINF_H
#define MAGICINF_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MagicDamage.h"

#define MAX_MAGIC_INFO	3

	#define	DBM_TYPE	0
	#define DBM_LEVEL	1
	#define DBM_EXTRA	2

#define MAX_MAGIC_LEVEL 20


class CMagicInf
{

public:

	CMagicInf();
	virtual ~CMagicInf();
	
	int IsMagic();
	//void __thiscall Init();
	void Clear();
	int Set(int aSkill, BYTE aLevel);
	int Set(BYTE aType, WORD aIndex, BYTE aLevel);
	int GetDamage();
	int UpdateMasterSkill(int iSkill, BYTE btLevel);
	
	CMagicInf & operator = (CMagicInf const & __that)
	{
		this->m_Level = __that.m_Level;
		this->m_Skill = __that.m_Skill;
		this->m_DamageMin = __that.m_DamageMin;
		this->m_DamageMax = __that.m_DamageMax;
		this->m_Pass = __that.m_Pass;

		return *this;
	};

public:

	BYTE m_Level;	// 4
	int m_Skill;	// 5
	int m_DamageMin;	// 8
	int m_DamageMax;	// C
	BYTE m_Pass;

};

extern CMagicInf DefMagicInf[MAX_SKILL-1];

int GetSkillNumberInex(int type, int Index, int level);
void MagicByteConvert(unsigned char* buf, CMagicInf* const Magici, int maxmagic);
void MagicByteConvert2(unsigned char* buf, CMagicInf * const Magici, int maxmagic);

#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


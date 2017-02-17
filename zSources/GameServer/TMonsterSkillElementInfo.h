// TMonsterSkillElementInfo.h: interface for the TMonsterSkillElementInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TMONSTERSKILLELEMENTINFO_H__8EC5FD6E_2B9F_409A_B2AA_70024491BEE5__INCLUDED_)
#define AFX_TMONSTERSKILLELEMENTINFO_H__8EC5FD6E_2B9F_409A_B2AA_70024491BEE5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class TMonsterSkillElementInfo
{

public:

	TMonsterSkillElementInfo()
	{
		// Reset Data
		this->Reset();
	}

	void  Reset();
	void  ResetDefense();
	void  ResetAttack();
	void  ResetAutoHP();
	void  ResetAutoMP();
	void  ResetAutoAG();
	void  ResetImmune();
	void  ResetResist();
	void  ResetModifyStat();
	void  ResetImmuneAll();//NEW
	int  CheckDefenseTime();
	int  CheckAttackTime();
	int  CheckAutoHPTime();
	int  CheckAutoMPTime();
	int  CheckAutoAGTime();
	int  CheckImmuneTime();
	int  CheckResistTime();
	int  CheckModifyStatTime();
	int	 CheckImmuneAllTime();//NEW
	int  DecDefenseTime();
	int  DecAttackTime();
	int  DecAutoHPTime();
	int  DecAutoMPTime();
	int  DecAutoAGTime();
	int  DecImmuneTime();
	int  DecResistTime();
	int  DecModifyStatTime();
	int	 DecImmuneAllTime();//NEW

	static void CheckSkillElementInfoProc(struct OBJECTSTRUCT * lpObj);

public:

	int m_iSkillElementDefense;	// 0
	int m_iSkillElementDefenseTime;	// 4
	int m_iSkillElementAttack;	// 8
	int m_iSkillElementAttackTime;	// C
	int m_iSkillElementAutoHP;	// 10
	int m_iSkillElementAutoHPCycle;	// 14
	int m_iSkillElementAutoHPTime;	// 18
	int m_iSkillElementAutoMP;	// 1C
	int m_iSkillElementAutoMPCycle;	// 20
	int m_iSkillElementAutoMPTime;	// 24
	int m_iSkillElementAutoAG;	// 28
	int m_iSkillElementAutoAGCycle;	// 2C
	int m_iSkillElementAutoAGTime;	// 30
	int m_iSkillElementImmuneNumber;	// 34
	int m_iSkillElementImmuneTime;	// 38
	int m_iSkillElementResistNumber;	// 3C
	int m_iSkillElementResistTime;	// 40
	int m_iSkillElementModifyStat;	// 44
	int m_iSkillElementModifyStatType;	// 48
	int m_iSkillElementModifyStatTime;	// 4C
	int m_iSkillElementImmuneAllTime; //50

};


#endif // !defined(AFX_TMONSTERSKILLELEMENTINFO_H__8EC5FD6E_2B9F_409A_B2AA_70024491BEE5__INCLUDED_)


//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


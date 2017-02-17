// SkillAdditionInfo.h: interface for the CSkillAdditionInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SKILLADDITIONINFO_H__6698BD5D_64C0_4EB5_8FE2_93B4993661F5__INCLUDED_)
#define AFX_SKILLADDITIONINFO_H__6698BD5D_64C0_4EB5_8FE2_93B4993661F5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MagicInf.h"

class CSkillAdditionInfo
{

public:

	CSkillAdditionInfo();
	virtual ~CSkillAdditionInfo();

	void Init();
	int Load();
	void SetInfinityArrowSkillTime(int iValue){this->m_iInfinityArrowSkillTime = iValue;}
	int GetInfinityArrowSkillTime(){return this->m_iInfinityArrowSkillTime;}
	void SetInfinityArrowUseLevel(int iValue){this->m_iInfinityArrowUseLevel = iValue;}
	int GetInfinityArrowUseLevel(){return this->m_iInfinityArrowUseLevel;}
	void SetInfinityArrowMPConsumptionPlus0(int iValue){this->m_iInfinityArrowMPConsumptionPlus0 = iValue;}
	int GetInfinityArrowMPConsumptionPlus0(){return this->m_iInfinityArrowMPConsumptionPlus0;}
	void SetInfinityArrowMPConsumptionPlus1(int iValue){this->m_iInfinityArrowMPConsumptionPlus1 = iValue;}
	int GetInfinityArrowMPConsumptionPlus1(){return this->m_iInfinityArrowMPConsumptionPlus1;}
	void SetInfinityArrowMPConsumptionPlus2(int iValue){this->m_iInfinityArrowMPConsumptionPlus2 = iValue;}
	int GetInfinityArrowMPConsumptionPlus2(){return this->m_iInfinityArrowMPConsumptionPlus2;}
	void SetInfinityArrowMPConsumptionPlus3(int iValue){ this->m_iInfinityArrowMPConsumptionPlus3 = iValue; }
	int GetInfinityArrowMPConsumptionPlus3(){ return this->m_iInfinityArrowMPConsumptionPlus3; }
	void SetFireScreamSkill(int iValue){this->m_bFireScreamSkill = iValue;}
	BOOL GetFireScreamSkill(){return this->m_bFireScreamSkill;}
	void SetFireScreamExplosionAttackDistance(int iValue){this->m_iFireScreamExplosionAttackDistance = iValue;}
	int GetFireScreamExplosionAttackDistance(){return this->m_iFireScreamExplosionAttackDistance;}
	void SetFireScreamExplosionRate(int iValue){this->m_iFireScreamExplosionRate = iValue;}
	int GetFireScreamExplosionRate(){return this->m_iFireScreamExplosionRate;}
	void SetFireScreamMaxAttackCountSameSerial(int iValue){this->m_iFireScreamMaxAttackCountSameSerial = iValue;}
	int GetFireScreamMaxAttackCountSameSerial(){return this->m_iFireScreamMaxAttackCountSameSerial;}
	void SetFireScreamSerialInitCount(int iValue){this->m_iFireScreamSerialInitCount = iValue;}
	int GetFireScreamSerialInitCount(){return this->m_iFireScreamSerialInitCount;}
	void SetCheckFireScreamSkill(int iValue){this->m_bCheckFireScreamSerial = iValue;}
	BOOL GetCheckFireScreamSkill(){return this->m_bCheckFireScreamSerial;}
	short GetSoulBarrierManaRate_Level(int iLevel);

private:

	int m_iInfinityArrowSkillTime;	// 4
	int m_iInfinityArrowUseLevel;	// 8
	int m_iInfinityArrowMPConsumptionPlus0;	// C
	int m_iInfinityArrowMPConsumptionPlus1;	// 10
	int m_iInfinityArrowMPConsumptionPlus2;	// 14
	int m_iInfinityArrowMPConsumptionPlus3;
	BOOL m_bFireScreamSkill;	// 18
	int m_iFireScreamExplosionAttackDistance;	// 1C
	int m_iFireScreamExplosionRate;	// 20
	int m_iFireScreamMaxAttackCountSameSerial;	// 24
	int m_iFireScreamSerialInitCount;	// 28
	BOOL m_bCheckFireScreamSerial;	// 2C
	short m_nSoulBarrierManaRate_Level[MAX_MAGIC_LEVEL + 1];
};

extern CSkillAdditionInfo g_SkillAdditionInfo;

#endif // !defined(AFX_SKILLADDITIONINFO_H__6698BD5D_64C0_4EB5_8FE2_93B4993661F5__INCLUDED_)


//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


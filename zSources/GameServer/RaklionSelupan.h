// RaklionSelupan.h: interface for the CRaklionSelupan class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RAKLIONSELUPAN_H__AF79D58C_C27B_4512_82EF_AC0648775FB2__INCLUDED_)
#define AFX_RAKLIONSELUPAN_H__AF79D58C_C27B_4512_82EF_AC0648775FB2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CRaklionSelupan  
{
	int m_iSelupanObjIndex;
	int m_iSkillDelay;
	int m_iNowBerserkLevel;
	int m_iSelupanMinDamage;
	int m_iSelupanMaxDamage;

public:
	CRaklionSelupan();
	virtual ~CRaklionSelupan();
	void Init();
	
	void RaklionSelupanAct_FirstSkill();
	void RaklionSelupanAct_PoisonAttack();
	void RaklionSelupanAct_IceStorm();
	void RaklionSelupanAct_IceStrike();
	void RaklionSelupanAct_SummonMonster();
	void RaklionSelupanAct_Heal();
	void RaklionSelupanAct_Freeze();
	void RaklionSelupanAct_Teleport();
	void RaklionSelupanAct_Invincibility();
	void RaklionSelupanAct_Berserk1();
	void RaklionSelupanAct_Berserk2();
	void RaklionSelupanAct_Berserk3();
	void RaklionSelupanAct_Berserk4();
	void RaklionSelupanAct_Berserk_Cancel();
	
	void SetSelupanObjIndex(int iIndex);
	int GetSelupanObjIndex();
	void InitSelupanIndex();
	
	DWORD GetSelupanHP();
	void SetSelupanHP(int iSelupanHPPercent);
	
	int RegenSelupan();
	void HideSelupan();
	
	int GetTargetOfSelupan();
	void SetSelupanSkillDelay(int iDelay);
	
	int GetSelupanSkillDelay();
	int GetBerserkLevel();
	
	void InitSelupanDamage(int iMinDamage, int iMaxDamage);
	
	void RegenSummonMonster();
	void DeleteSummonMonster();

};

#endif // !defined(AFX_RAKLIONSELUPAN_H__AF79D58C_C27B_4512_82EF_AC0648775FB2__INCLUDED_)


//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


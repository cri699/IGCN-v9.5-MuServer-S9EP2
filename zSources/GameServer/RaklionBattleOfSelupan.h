// RaklionBattleOfSelupan.h: interface for the CRaklionBattleOfSelupan class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RAKLIONBATTLEOFSELUPAN_H__6374C4E1_3EC9_4AE6_87E1_789B8FF24030__INCLUDED_)
#define AFX_RAKLIONBATTLEOFSELUPAN_H__6374C4E1_3EC9_4AE6_87E1_789B8FF24030__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RaklionSelupan.h"

class CRaklionBattleOfSelupan  
{
	BOOL m_bFileDataLoad;
	int m_iBattleOfSelupanState;
	int m_iSelupanPatternCondition[7];
	LONGLONG m_iSelupanSkillDelay;
	LONGLONG m_iSelupanSkillStartTime;

	BOOL m_bIsRunFirstSkill;
	BOOL m_bIsSucccess;
	CRaklionSelupan m_RaklionSelupan;
	int m_SelupanPattern[7][9];
public:
	CRaklionBattleOfSelupan();
	virtual ~CRaklionBattleOfSelupan();

	void ResetAllData();

	BOOL LoadData(char* lpszFileName);

	void Run();

	void ProcState_NONE();
	void ProcState_STANDBY();
	void ProcState_PATTERN_1();
	void ProcState_PATTERN_2();
	void ProcState_PATTERN_3();
	void ProcState_PATTERN_4();
	void ProcState_PATTERN_5();
	void ProcState_PATTERN_6();
	void ProcState_PATTERN_7();
	void ProcState_DIE();

	void SetState(int iBattleOfSelupanState);

	void SetState_NONE();
	void SetState_STANDBY();
	void SetState_PATTERN_1();
	void SetState_PATTERN_2();
	void SetState_PATTERN_3();
	void SetState_PATTERN_4();
	void SetState_PATTERN_5();
	void SetState_PATTERN_6();
	void SetState_PATTERN_7();
	void SetState_DIE();

	void ActionOfSelupan(int iAction);

	void SetBattleOfSelupanStateAuto();
	void SetBattleOfSelupanState(int iState){ m_iBattleOfSelupanState = iState; }
	int GetBattleOfSelupanState(){ return m_iBattleOfSelupanState; }

	void SetSuccessValue(BOOL bIsSuccess){ m_bIsSucccess = bIsSuccess; }
	BOOL GetSuccessValue(){ return m_bIsSucccess; };

	void SetPatternCondition(int iPatternNum, int iCondition);
	
	void RegenSelupan();
	void HideSelupan();
	
	int GetSelupanObjIndex();
	void DeleteSummonMonster();
	
	void SetSelupanSkillDelay(int iDelay);
	
	void GmCommandSelupanPattern(int iPatternNumber);
	
	int GetSelupanBattleUserCount();
};

#endif // !defined(AFX_RAKLIONBATTLEOFSELUPAN_H__6374C4E1_3EC9_4AE6_87E1_789B8FF24030__INCLUDED_)


//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


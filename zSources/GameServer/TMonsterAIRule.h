// TMonsterAIRule.h: interface for the TMonsterAIRule class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TMONSTERAIRULE_H__1DF16E76_073C_41E7_AA23_77ACAB49E5D1__INCLUDED_)
#define AFX_TMONSTERAIRULE_H__1DF16E76_073C_41E7_AA23_77ACAB49E5D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TMonsterAIRuleInfo.h"

#define MAX_MONSTER_AI_RULE_INFO	(200)
#define MAX_MONSTER_AI_RULE_TABLE	(1000)

class TMonsterAIRule  
{

public:

	TMonsterAIRule();
	virtual ~TMonsterAIRule();

	static bool LoadData(char* lpszFileName);
	static bool DelAllAIRule();
	static int GetCurrentAIUnit(int iMonsterClass);
	static void MonsterAIRuleProc();

public:

	static bool s_bDataLoad;
	static int s_iMonsterCurrentAIUnitTable[MAX_MONSTER_AI_RULE_TABLE];
	static TMonsterAIRuleInfo s_MonsterAIRuleInfoArray[MAX_MONSTER_AI_RULE_INFO];
	static int s_iMonsterAIRuleInfoCount;

};

#endif // !defined(AFX_TMONSTERAIRULE_H__1DF16E76_073C_41E7_AA23_77ACAB49E5D1__INCLUDED_)


//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


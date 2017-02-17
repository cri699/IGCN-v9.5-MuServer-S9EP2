// TMonsterAIRule.cpp: implementation of the TMonsterAIRule class.
//	GS-N	1.00.18	JPN	0x00562270	-	Completed
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TMonsterAIRule.h"
#include "TLog.h"


bool TMonsterAIRule::s_bDataLoad = FALSE;
int TMonsterAIRule::s_iMonsterCurrentAIUnitTable[MAX_MONSTER_AI_RULE_TABLE];
TMonsterAIRuleInfo TMonsterAIRule::s_MonsterAIRuleInfoArray[MAX_MONSTER_AI_RULE_INFO];
int TMonsterAIRule::s_iMonsterAIRuleInfoCount;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TMonsterAIRule::TMonsterAIRule()
{
	this->DelAllAIRule();
}

TMonsterAIRule::~TMonsterAIRule()
{
	return;
}



bool TMonsterAIRule::LoadData(LPSTR lpszFileName)
{
	TMonsterAIRule::s_bDataLoad = FALSE;

	if ( !lpszFileName || !strcmp(lpszFileName, ""))
	{
		g_Log.MsgBox("[Monster AI Rule] - File load error : File Name Error");
		return FALSE;
	}

	try
	{
		pugi::xml_document file;
		pugi::xml_parse_result res = file.load_file(lpszFileName);

		if ( res.status != pugi::status_ok )
		{
			g_Log.MsgBox("[Monster AI Rule] - Can't Load %s (%s)", lpszFileName, res.description());
			return FALSE;
		}

		TMonsterAIRule::DelAllAIRule();

		pugi::xml_node main = file.child("MonsterAI");

		for (pugi::xml_node rule = main.child("Rule"); rule; rule = rule.next_sibling())
		{
			int iAIRuleNumber = rule.attribute("Number").as_int();
			int iMonsterClass = rule.attribute("MonsterIndex").as_int();
			TMonsterAIRuleInfo MonsterAIRuleInfo;

			memcpy(MonsterAIRuleInfo.m_szRuleDesc, rule.attribute("Name").as_string(), sizeof(MonsterAIRuleInfo.m_szRuleDesc));

			MonsterAIRuleInfo.m_iMonsterAIUnit = rule.attribute("MonsterAIUnit").as_int();
			MonsterAIRuleInfo.m_iRuleCondition = rule.attribute("Condition").as_int();
			MonsterAIRuleInfo.m_iWaitTime = rule.attribute("WaitTime").as_int();
			MonsterAIRuleInfo.m_iContinuanceTime = rule.attribute("ContinuanceTime").as_int();
			MonsterAIRuleInfo.m_iMonth = rule.attribute("Month").as_int();
			MonsterAIRuleInfo.m_iDay = rule.attribute("Day").as_int();
			MonsterAIRuleInfo.m_iWeekDay = rule.attribute("WeekDay").as_int();
			MonsterAIRuleInfo.m_iHour = rule.attribute("Hour").as_int();
			MonsterAIRuleInfo.m_iMinute = rule.attribute("Minute").as_int();

			if ( iAIRuleNumber  < 0 || iAIRuleNumber >= MAX_MONSTER_AI_RULE_INFO )
			{
				g_Log.MsgBox("[Monster AI Rule] - AIRuleNumber(%d) Error (%s) File. ", iAIRuleNumber, lpszFileName);
				continue;
			}

			if ( iMonsterClass  < 0 || iMonsterClass >= MAX_MONSTER_AI_RULE_TABLE )
			{
				g_Log.MsgBox("[Monster AI Rule] - MonsterClass(%d) Error (%s) File. ", iMonsterClass, lpszFileName);
				continue;
			}

			MonsterAIRuleInfo.m_iRuleNumber = iAIRuleNumber;
			MonsterAIRuleInfo.m_iMonsterClass = iMonsterClass;
			memcpy(&TMonsterAIRule::s_MonsterAIRuleInfoArray[TMonsterAIRule::s_iMonsterAIRuleInfoCount], &MonsterAIRuleInfo, sizeof(MonsterAIRuleInfo));
			TMonsterAIRule::s_iMonsterAIRuleInfoCount++;
		}

		g_Log.AddC(TColor::Red,  "[Monster AI Rule ] - %s file is Loaded", lpszFileName);
		TMonsterAIRule::s_bDataLoad = TRUE;
	}

	catch(DWORD)
	{
		g_Log.MsgBox("[Monster AI Rule] - Loading Exception Error (%s) File. ", lpszFileName);
	}

	return FALSE;
}


bool TMonsterAIRule::DelAllAIRule()
{
	for ( int i=0;i<MAX_MONSTER_AI_RULE_INFO;i++)
	{
		TMonsterAIRule::s_MonsterAIRuleInfoArray[i].Reset();
	}

	for ( int j=0;j<MAX_MONSTER_AI_RULE_TABLE;j++)
	{
		TMonsterAIRule::s_iMonsterCurrentAIUnitTable[j] = 0;
	}

	TMonsterAIRule::s_iMonsterAIRuleInfoCount = 0;

	return FALSE;
}

int TMonsterAIRule::GetCurrentAIUnit(int iMonsterClass)
{
	if ( iMonsterClass < 0 || iMonsterClass >= MAX_MONSTER_AI_RULE_TABLE )
	{
		g_Log.Add("[Monster AI Rule] GetCurrentAIUnit() Error - (MonsterClass=%d) ", iMonsterClass);
		return 0;
	}

	return TMonsterAIRule::s_iMonsterCurrentAIUnitTable[iMonsterClass];
}



void TMonsterAIRule::MonsterAIRuleProc()
{
	int iChangedMonsterClassList[MAX_MONSTER_AI_RULE_INFO] = {-1};
	int iChangedAIUnitList[MAX_MONSTER_AI_RULE_INFO] = {0};
	int iChangedListCount = 0;
	
	for (int i=0;i<TMonsterAIRule::s_iMonsterAIRuleInfoCount;i++)
	{
		TMonsterAIRuleInfo & RuleInfo = TMonsterAIRule::s_MonsterAIRuleInfoArray[i];

		if ( RuleInfo.m_iMonsterClass < 0 || RuleInfo.m_iMonsterClass >= MAX_MONSTER_AI_RULE_TABLE )
			return;

		if ( RuleInfo.IsValid() )
		{
			iChangedMonsterClassList[iChangedListCount] = RuleInfo.m_iMonsterClass;
			iChangedAIUnitList[iChangedListCount] = RuleInfo.m_iMonsterAIUnit;
			iChangedListCount++;
		}
		else
		{
			TMonsterAIRule::s_iMonsterCurrentAIUnitTable[RuleInfo.m_iMonsterClass] = 0;
		}
	}

	for(int j=0;j<iChangedListCount;j++)
	{
		TMonsterAIRule::s_iMonsterCurrentAIUnitTable[iChangedMonsterClassList[j]] = iChangedAIUnitList[j];
	}
}


//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


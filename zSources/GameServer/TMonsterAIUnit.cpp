// TMonsterAIUnit.cpp: implementation of the TMonsterAIUnit class.
//	GS-N	1.00.18	JPN	0x00563910	-	Completed
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TMonsterAIUnit.h"
#include "TLog.h"

bool TMonsterAIUnit::s_bDataLoad = FALSE;
TMonsterAIUnit TMonsterAIUnit::s_MonsterAIUnitArray[MAX_MONSTER_AI_UNIT];

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TMonsterAIUnit::TMonsterAIUnit()
{
	this->Reset();
}

TMonsterAIUnit::~TMonsterAIUnit()
{
	return;
}

void TMonsterAIUnit::Reset()
{
	this->m_iUnitNumber = -1;
	this->m_iDelayTime = NULL;
	this->m_lpAutomata = NULL;
	this->m_lpAIClassNormal = NULL;
	this->m_lpAIClassMove = NULL;
	this->m_lpAIClassAttack = NULL;
	this->m_lpAIClassHeal = NULL;
	this->m_lpAIClassAvoid = NULL;
	this->m_lpAIClassHelp = NULL;
	this->m_lpAIClassSpecial = NULL;
	this->m_lpAIClassEvent = NULL;
	memset(this->m_szUnitName, 0, sizeof(this->m_szUnitName));
	this->m_lppAIClassMap[0] = &this->m_lpAIClassNormal;
	this->m_lppAIClassMap[1] = &this->m_lpAIClassMove;
	this->m_lppAIClassMap[2] = &this->m_lpAIClassAttack;
	this->m_lppAIClassMap[3] = &this->m_lpAIClassHeal;
	this->m_lppAIClassMap[4] = &this->m_lpAIClassAvoid;
	this->m_lppAIClassMap[5] = &this->m_lpAIClassHelp;
	this->m_lppAIClassMap[6] = &this->m_lpAIClassSpecial;
	this->m_lppAIClassMap[7] = &this->m_lpAIClassEvent;
}

BOOL TMonsterAIUnit::LoadData(LPSTR lpszFileName)
{
	TMonsterAIUnit::s_bDataLoad = FALSE;

	if ( !lpszFileName || !strcmp(lpszFileName, ""))
	{
		g_Log.MsgBox("[Monster AI Unit] - File load error : File Name Error");
		return FALSE;
	}

	try
	{
		pugi::xml_document file;
		pugi::xml_parse_result res = file.load_file(lpszFileName);

		if ( res.status != pugi::status_ok )
		{
			g_Log.MsgBox("[Monster AI Unit] - Can't Load %s (%s)", lpszFileName, res.description());
			return FALSE;
		}

		TMonsterAIUnit::DelAllAIUnit();

		pugi::xml_node main = file.child("MonsterAI");

		for (pugi::xml_node unit = main.child("Unit"); unit; unit = unit.next_sibling())
		{
			char szUnitName[50]={0};
			memcpy(szUnitName, unit.attribute("Name").as_string(), sizeof(szUnitName));

			int iUnitNumber = unit.attribute("Number").as_int();
			int iDelayTime = unit.attribute("DelayTime").as_int();
			int iAutomata = unit.attribute("Automata").as_int();
			int iAIClassNormal = unit.attribute("AIClassNormal").as_int();
			int iAIClassMove = unit.attribute("AIClassMove").as_int();
			int iAIClassAttack = unit.attribute("AIClassAttack").as_int();
			int iAIClassHeal = unit.attribute("AIClassHeal").as_int();
			int iAIClassAvoid = unit.attribute("AIClassAvoid").as_int();
			int iAIClassHelp = unit.attribute("AIClassHelp").as_int();
			int iAIClassSpecial = unit.attribute("AIClassSpecial").as_int();
			int iAIClassEvent = unit.attribute("AIClassEvent").as_int();

			if ( iUnitNumber < 0 || iUnitNumber >= MAX_MONSTER_AI_UNIT )
			{
				g_Log.MsgBox("[Monster AI Unit] - UnitNumber(%d) Error (%s) File. ", iUnitNumber, lpszFileName);
				continue;
			}

			if ( iAutomata < 0 || iAutomata >= MAX_MONSTER_AI_AUTOMATA )
			{
				g_Log.MsgBox("[Monster AI Unit] - AutomataNumber(%d) Error (%s) File. ", iAutomata, lpszFileName);
				continue;
			}

			TMonsterAIUnit::s_MonsterAIUnitArray[iUnitNumber].m_iUnitNumber = iUnitNumber;
			TMonsterAIUnit::s_MonsterAIUnitArray[iUnitNumber].m_iDelayTime = iDelayTime;
			TMonsterAIUnit::s_MonsterAIUnitArray[iUnitNumber].m_lpAutomata = TMonsterAIAutomata::FindAutomata(iAutomata);
			TMonsterAIUnit::s_MonsterAIUnitArray[iUnitNumber].m_lpAIClassNormal = TMonsterAIElement::FindAIElement(iAIClassNormal);
			TMonsterAIUnit::s_MonsterAIUnitArray[iUnitNumber].m_lpAIClassMove = TMonsterAIElement::FindAIElement(iAIClassMove);
			TMonsterAIUnit::s_MonsterAIUnitArray[iUnitNumber].m_lpAIClassAttack = TMonsterAIElement::FindAIElement(iAIClassAttack);
			TMonsterAIUnit::s_MonsterAIUnitArray[iUnitNumber].m_lpAIClassHeal = TMonsterAIElement::FindAIElement(iAIClassHeal);
			TMonsterAIUnit::s_MonsterAIUnitArray[iUnitNumber].m_lpAIClassAvoid = TMonsterAIElement::FindAIElement(iAIClassAvoid);
			TMonsterAIUnit::s_MonsterAIUnitArray[iUnitNumber].m_lpAIClassHelp = TMonsterAIElement::FindAIElement(iAIClassHelp);
			TMonsterAIUnit::s_MonsterAIUnitArray[iUnitNumber].m_lpAIClassSpecial = TMonsterAIElement::FindAIElement(iAIClassSpecial);
			TMonsterAIUnit::s_MonsterAIUnitArray[iUnitNumber].m_lpAIClassEvent = TMonsterAIElement::FindAIElement(iAIClassEvent);
			memcpy(TMonsterAIUnit::s_MonsterAIUnitArray[iUnitNumber].m_szUnitName, szUnitName, sizeof(TMonsterAIUnit::s_MonsterAIUnitArray[iUnitNumber].m_szUnitName));
		}

		g_Log.AddC(TColor::Red,  "[Monster AI Unit] - %s file is Loaded", lpszFileName);
		TMonsterAIUnit::s_bDataLoad = TRUE;
	}

	catch(DWORD)
	{
		g_Log.MsgBox("[Monster AI Unit] - Loading Exception Error (%s) File. ", lpszFileName);
	}

	return FALSE;
}

BOOL TMonsterAIUnit::DelAllAIUnit()
{
	for (int i=0;i<MAX_MONSTER_AI_UNIT;i++)
	{
		TMonsterAIUnit::s_MonsterAIUnitArray[i].Reset();
	}

	return FALSE;
}

TMonsterAIUnit * TMonsterAIUnit::FindAIUnit(int iUnitNumber)
{
	if ( iUnitNumber < 0 || iUnitNumber >= MAX_MONSTER_AI_UNIT )
	{
		g_Log.Add("[Monster AI Unit] FindAIUnit() Error - (UnitNumber=%d) ",
			iUnitNumber);

		return NULL;
	}

	if ( TMonsterAIUnit::s_MonsterAIUnitArray[iUnitNumber].m_iUnitNumber == iUnitNumber )
	{
		return &TMonsterAIUnit::s_MonsterAIUnitArray[iUnitNumber];
	}

	g_Log.Add("[Monster AI Unit] FindAIUnit() Error - (UnitNumber=%d) ",
		iUnitNumber);

	return NULL;
}


BOOL TMonsterAIUnit::RunAIUnit(int iIndex)
{
	LPOBJ lpObj = &gObj[iIndex];

	if ( this->m_lpAutomata == NULL )
		return FALSE;

	if ( (GetTickCount() - lpObj->m_iLastAutomataRuntime) < lpObj->m_iLastAutomataDelay )
		return FALSE;

	TMonsterAIState * pAIState = this->m_lpAutomata->RunAutomata(iIndex);

	if ( pAIState == NULL )
		return FALSE;

	lpObj->m_iLastAutomataRuntime = GetTickCount();
	TMonsterAIElement * pAIElement = *this->m_lppAIClassMap[pAIState->m_iNextState];

	if ( pAIElement == NULL )
		return FALSE;

	int iRetExec = pAIElement->ForceAIElement(iIndex, 0, pAIState);

	if ( iRetExec == 0 )
		return FALSE;

	lpObj->m_iCurrentAIState = pAIState->m_iNextState;
	
	return TRUE;
}


//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


// KanturuBattleOfNightmare.cpp: implementation of the CKanturuBattleOfNightmare class.
//	GS-N	1.00.18	JPN	0x0057D3F0	-	Completed
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "KanturuBattleOfNightmare.h"
#include "Kanturu.h"
#include "KanturuMonsterMng.h"
#include "KanturuBattleUserMng.h"
#include "TMonsterAIGroup.h"
#include "TLog.h"
#include "KanturuUtil.h"

static CKanturuUtil KANTURU_UTIL;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKanturuBattleOfNightmare::CKanturuBattleOfNightmare()
{
	this->SetBattleOfNightmareState(KANTURU_NIGHTMARE_NONE);
	this->ResetAllData();
	this->m_iNightmareAIGroupNumber = 0;
	this->m_iNightmareObjIndex = -1;
	this->SetSuccessValue(FALSE);
}

CKanturuBattleOfNightmare::~CKanturuBattleOfNightmare()
{
	return;
}


void CKanturuBattleOfNightmare::ResetAllData()
{
	this->m_StateInfoCount = 0;

	for ( int iCount=0;iCount<KANTURU_NIGHTMARE_STATE_INFO;iCount++)
	{
		this->m_StateInfo[iCount].ResetTimeInfo();
	}
}


BOOL CKanturuBattleOfNightmare::LoadData(LPSTR lpszFileName)
{
	this->m_bFileDataLoad = FALSE;

	if ( !lpszFileName || !strcmp(lpszFileName, ""))
	{
		g_Log.MsgBox("[Kanturu][Battle of Nightmare] - File load error : File Name Error");
		return FALSE;
	}

	try
	{	
		pugi::xml_document file;
		pugi::xml_parse_result res = file.load_file(lpszFileName);

		if ( res.status != pugi::status_ok )
		{
			g_Log.MsgBox("[Kanturu][Battle of Nightmare] - Can't Load %s (%s)", lpszFileName, res.description());
			return FALSE;
		}

		this->ResetAllData();

		pugi::xml_node main = file.child("KanturuEvent");
		pugi::xml_node battle_nightmare = main.child("BattleOfNightMare");

		this->m_iNightmareAIGroupNumber = battle_nightmare.attribute("AIGroup").as_int();

		pugi::xml_node state = battle_nightmare.child("State");

		for (pugi::xml_node cycle = state.child("Cycle"); cycle; cycle = cycle.next_sibling())
		{
			int iState = cycle.attribute("State").as_int();
			int iCondition = cycle.attribute("SwitchType").as_int();
			int iValue = cycle.attribute("Time").as_int();

			if ( this->m_StateInfoCount < 0 || this->m_StateInfoCount >= KANTURU_NIGHTMARE_STATE_INFO )
			{
				g_Log.MsgBox("[Kanturu][Battle of Nightmare] - Exceed Max State Time (%d)", this->m_StateInfoCount);
				break;
			}

			this->m_StateInfo[this->m_StateInfoCount].SetStateInfo(iState);
			this->m_StateInfo[this->m_StateInfoCount].SetCondition(iCondition);

			if ( iCondition == 1 )
				iValue *= 1000;

			this->m_StateInfo[this->m_StateInfoCount].SetValue(iValue);
			this->m_StateInfoCount++;
		}

		g_Log.AddC(TColor::Red,  "[Kanturu][Battle of Nightmare]  %s file is Loaded", lpszFileName);
		this->m_bFileDataLoad = TRUE;
	}

	catch ( DWORD )
	{
		g_Log.MsgBox("[Kanturu][Battle of Nightmare] - Loading Exception Error (%s) File. ", lpszFileName);
	}

	return this->m_bFileDataLoad;
}


void CKanturuBattleOfNightmare::Run()
{
	this->CheckStateTime();
	this->CheckNightmareBattleUser();
	g_KanturuBattleUserMng.CheckUserState();

	switch ( this->m_iBattleOfNightmareState )
	{
		case KANTURU_NIGHTMARE_NONE:
			this->ProcState_NONE();
			break;
		case KANTURU_NIGHTMARE_IDLE:
			this->ProcState_IDLE();
			break;
		case KANTURU_NIGHTMARE_NOTIFY:
			this->ProcState_NOTIFY();
			break;
		case KANTURU_NIGHTMARE_START:
			this->ProcState_START();
			break;
		case KANTURU_NIGHTMARE_END:
			this->ProcState_END();
			break;
		case KANTURU_NIGHTMARE_ENDCYCLE:
			this->ProcState_ENDCYCLE();
			break;
	}
}


void CKanturuBattleOfNightmare::SetState(int iBattleOfNightmareState)
{
	if ( this->m_StateInfo[iBattleOfNightmareState].GetCondition() > 0 )
	{
		this->m_StateInfo[iBattleOfNightmareState].SetAppliedTime();
	}

	g_KanturuMonsterMng.ResetRegenMonsterObjData();
	this->m_iNightmareObjIndex = -1;

	g_Log.Add("[Kanturu][Battle of Nightmare] CKanturuBattleOfNightmare::SetState(...)  m_iNightmareObjIndex %d",
		this->m_iNightmareObjIndex);

	switch ( iBattleOfNightmareState )
	{
		case KANTURU_NIGHTMARE_NONE:
			this->SetState_NONE();
			break;
		case KANTURU_NIGHTMARE_IDLE:
			this->SetState_IDLE();
			break;
		case KANTURU_NIGHTMARE_NOTIFY:
			this->SetState_NOTIFY();
			break;
		case KANTURU_NIGHTMARE_START:
			this->SetState_START();
			break;
		case KANTURU_NIGHTMARE_END:
			this->SetState_END();
			break;
		case KANTURU_NIGHTMARE_ENDCYCLE:
			this->SetState_ENDCYCLE();
			break;
	}

	KANTURU_UTIL.NotifyKanturuChangeState(KANTURU_STATE_BATTLE_OF_NIGHTMARE, this->m_iBattleOfNightmareState);
}


void CKanturuBattleOfNightmare::SetNextState(int iCurrentState)
{
	int iNextState = iCurrentState + 1;

	if ( iNextState >= KANTURU_NIGHTMARE_STATE_INFO )
	{
		iNextState = KANTURU_NIGHTMARE_NONE;
	}

	this->SetState(iNextState);
}


void CKanturuBattleOfNightmare::SetState_NONE()
{
	g_Log.AddC(TColor::Brown,  "[Kanturu][Battle of Nightmare] State(%d) -> NONE",
		this->m_iBattleOfNightmareState);

	this->SetBattleOfNightmareState(KANTURU_NIGHTMARE_NONE);
	TMonsterAIGroup::DelGroupInstance(this->m_iNightmareAIGroupNumber);

	g_Log.Add("[Kanturu][Battle of Nightmare] DelGroupInstance SetState_NONE");
}

void CKanturuBattleOfNightmare::SetState_IDLE()
{
	g_Log.AddC(TColor::Brown,  "[Kanturu][Battle of Nightmare] State(%d) -> IDLE",
		this->m_iBattleOfNightmareState);

	g_Log.Add("[Kanturu][Battle of Nightmare] State(%d) -> IDLE",
		this->m_iBattleOfNightmareState);

	this->SetBattleOfNightmareState(KANTURU_NIGHTMARE_IDLE);
	this->SetSuccessValue(FALSE);
	TMonsterAIGroup::DelGroupInstance(this->m_iNightmareAIGroupNumber);

	g_Log.Add("[Kanturu][Battle of Nightmare] DelGroupInstance SetState_IDLE");
	g_KanturuBattleUserMng.MoveAllUser(134);
}

void CKanturuBattleOfNightmare::SetState_NOTIFY()
{
	g_Log.AddC(TColor::Brown,  "[Kanturu][Battle of Nightmare] State(%d) -> NOTIFY",
		this->m_iBattleOfNightmareState);

	this->SetBattleOfNightmareState(KANTURU_NIGHTMARE_NOTIFY);
}

void CKanturuBattleOfNightmare::SetState_START()
{
	g_Log.AddC(TColor::Brown,  "[Kanturu][Battle of Nightmare] State(%d) -> START",
		this->m_iBattleOfNightmareState);

	this->SetBattleOfNightmareState(KANTURU_NIGHTMARE_START);

	if ( this->m_StateInfo[this->m_iBattleOfNightmareState].GetCondition() == 1 )
	{
		KANTURU_UTIL.NotifyKanturuBattleTime(this->m_StateInfo[this->m_iBattleOfNightmareState].GetValue());
	}

	g_KanturuMonsterMng.SetKanturuMonster(2);

	TMonsterAIGroup::Init(this->m_iNightmareAIGroupNumber);
	TMonsterAIGroup::ChangeAIOrder(this->m_iNightmareAIGroupNumber, 0);

	int iIndex = TMonsterAIGroup::FindGroupLeader(this->m_iNightmareAIGroupNumber);
	this->m_iNightmareObjIndex = iIndex;

	g_Log.Add("[Kanturu][Battle of Nightmare] CKanturuNightmare Set m_iNightmareObjIndex %d",
		this->m_iNightmareObjIndex);
}


void CKanturuBattleOfNightmare::SetState_END()
{
	g_Log.AddC(TColor::Brown,  "[Kanturu][Battle of Nightmare] State(%d) -> END",
		this->m_iBattleOfNightmareState);

	this->SetBattleOfNightmareState(KANTURU_NIGHTMARE_END);
	KANTURU_UTIL.NotifyKanturuSuccessValue(this->GetSuccessValue());

	if ( this->GetSuccessValue() == FALSE )
	{
		KANTURU_UTIL.SendMsgKanturuBattleUser(Lang.GetText(0,266));
		g_Log.Add("[Kanturu][Battle of Nightmare] Fail!! TimeOut");
	}
}

void CKanturuBattleOfNightmare::SetState_ENDCYCLE()
{
	g_Log.AddC(TColor::Brown,  "[Kanturu][Battle of Nightmare] State(%d) -> ENDCYCLE",
		this->m_iBattleOfNightmareState);

	g_Log.Add("[Kanturu][Battle of Nightmare] State(%d) -> ENDCYCLE",
		this->m_iBattleOfNightmareState);

	this->SetBattleOfNightmareState(KANTURU_NIGHTMARE_ENDCYCLE);

	if ( this->GetSuccessValue() == TRUE )
	{
		KANTURU_UTIL.SendMsgKanturuBattleUser(Lang.GetText(0,265));
	}
	else
	{
		g_KanturuBattleUserMng.MoveAllUser(137);
		g_KanturuBattleUserMng.ResetAllData();
	}
}


void CKanturuBattleOfNightmare::ProcState_NONE()
{
	return;
}

void CKanturuBattleOfNightmare::ProcState_IDLE()
{
	return;
}

void CKanturuBattleOfNightmare::ProcState_NOTIFY()
{
	return;
}

void CKanturuBattleOfNightmare::ProcState_START()
{
	KANTURU_UTIL.NotifyKanturuUserMonsterCount(g_KanturuMonsterMng.GetAliveMonsterCount(),
		g_KanturuBattleUserMng.GetUserCount());

	int iIndex = this->m_iNightmareObjIndex;

	if (ObjectMaxRange(iIndex))
	{
		if ( gObj[iIndex].Live == FALSE )
		{
			this->SetSuccessValue(TRUE);

			g_Log.Add("[Kanturu][Battle of Nightmare] Success!! Nightmare Die");
			g_KanturuBattleUserMng.LogBattleWinnerUserInfo(3, this->m_StateInfo[this->m_iBattleOfNightmareState].GetElapsedTime());

			this->SetNextState(this->m_iBattleOfNightmareState);
		}
	}
}


void CKanturuBattleOfNightmare::ProcState_END()
{
	return;
}


void CKanturuBattleOfNightmare::ProcState_ENDCYCLE()
{
	TMonsterAIGroup::DelGroupInstance(this->m_iNightmareAIGroupNumber);

	g_Log.Add("[Kanturu][Battle of Nightmare] DelGroupInstance ProcState_ENDCYCLE");
}

void CKanturuBattleOfNightmare::CheckStateTime()
{
	int iCurrentState = this->GetBattleOfNightmareState();

	if ( iCurrentState )
	{
		if ( this->m_StateInfo[iCurrentState].GetCondition() == 1 )
		{
			if ( this->m_StateInfo[iCurrentState].IsTimeOut() == TRUE )
			{
				this->SetNextState(iCurrentState);
			}
		}
	}
}


int CKanturuBattleOfNightmare::GetRemainTime()
{
	int iCurrentState = this->GetBattleOfNightmareState();
	return this->m_StateInfo[iCurrentState].GetRemainTime();
}


void CKanturuBattleOfNightmare::CheckNightmareBattleUser()
{
	if ( this->GetBattleOfNightmareState() == KANTURU_NIGHTMARE_NOTIFY ||
		 this->GetBattleOfNightmareState() == KANTURU_NIGHTMARE_START )
	{
		if ( g_KanturuBattleUserMng.IsEmpty() == TRUE )
		{
			g_KanturuBattleUserMng.ResetAllData();
			this->SetSuccessValue(FALSE);

			g_Log.Add("[Kanturu][Battle of Nightmare] Fail!! All Battle User Die");

			this->SetState(KANTURU_NIGHTMARE_END);
		}
	}
}


//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


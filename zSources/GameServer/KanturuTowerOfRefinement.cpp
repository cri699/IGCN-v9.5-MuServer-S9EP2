// KanturuTowerOfRefinement.cpp: implementation of the CKanturuTowerOfRefinement class.
//	GS-N	1.00.18	JPN	0x0057F810	-	Completed
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "KanturuTowerOfRefinement.h"
#include "Kanturu.h"
#include "KanturuBattleUserMng.h"
#include "TLog.h"
#include "KanturuUtil.h"
#include "configread.h"

static CKanturuUtil KANTURU_UTIL;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKanturuTowerOfRefinement::CKanturuTowerOfRefinement()
{
	this->SetTowerOfRefinementState(KANTURU_TOWER_OF_REFINEMENT_NONE);
	this->SetEntrancePermit(FALSE);
	this->SetUseTowerOfRefinement(FALSE);

	this->m_StateInfoCount = 0;
}

CKanturuTowerOfRefinement::~CKanturuTowerOfRefinement()
{
	return;
}


void CKanturuTowerOfRefinement::Init()
{
	return;
}


void CKanturuTowerOfRefinement::ResetAllData()
{
	this->m_StateInfoCount = 0;

	for ( int iCount=0;iCount < KANTURU_TOWER_OF_REFINEMENT_STATE_INFO; iCount++)
	{
		this->m_StateInfo[iCount].ResetTimeInfo();
	}
}



BOOL CKanturuTowerOfRefinement::LoadData(LPSTR lpszFileName)
{
	this->m_bFileDataLoad = FALSE;

	if ( !lpszFileName || !strcmp(lpszFileName, ""))
	{
		g_Log.MsgBox("[Kanturu][Tower of Refinement] - File load error : File Name Error");
		return FALSE;
	}

	try
	{
		pugi::xml_document file;
		pugi::xml_parse_result res = file.load_file(lpszFileName);

		if ( res.status != pugi::status_ok )
		{
			g_Log.MsgBox("[Kanturu][Tower of Refinement] - Can't Load %s file (%s)", lpszFileName, res.description());
			return FALSE;
		}

		pugi::xml_node main = file.child("KanturuEvent");
		pugi::xml_node tower_refinement = main.child("TowerOfRefinement");

		this->ResetAllData();

		pugi::xml_node state = tower_refinement.child("State");

		for (pugi::xml_node cycle = state.child("Cycle"); cycle; cycle = cycle.next_sibling())
		{
			int iState = cycle.attribute("State").as_int();
			int iCondition = cycle.attribute("SwitchType").as_int();
			int iValue = cycle.attribute("Time").as_int();

			if ( this->m_StateInfoCount < 0 || this->m_StateInfoCount >= KANTURU_TOWER_OF_REFINEMENT_STATE_INFO )
			{
				g_Log.MsgBox("[Kanturu][Tower of Refinement] - Exceed Max State Time (%d)", this->m_StateInfoCount);
				break;
			}

			this->m_StateInfo[this->m_StateInfoCount].SetStateInfo(iState);
			this->m_StateInfo[this->m_StateInfoCount].SetCondition(iCondition);

			if ( iCondition == 1 )
				iValue *= 1000;

			this->m_StateInfo[this->m_StateInfoCount].SetValue(iValue);
			this->m_StateInfoCount++;
		}

		g_Log.AddC(TColor::Red,  "[Kanturu][Tower of Refinement] %s file is Loaded", lpszFileName);
		this->m_bFileDataLoad = TRUE;
	}

	catch ( DWORD )
	{
		g_Log.MsgBox("[Kanturu][Tower of Refinement] - Loading Exception Error (%s) File. ", lpszFileName);
	}

	return this->m_bFileDataLoad;
}


void CKanturuTowerOfRefinement::Run()
{
	this->CheckStateTime();

	switch ( this->m_iTowerOfRefinementState )
	{
		case KANTURU_TOWER_OF_REFINEMENT_NONE:
			this->ProcState_NONE();
			break;
		case KANTURU_TOWER_OF_REFINEMENT_REVITALIZATION:
			this->ProcState_REVITALIZATION();
			break;
		case KANTURU_TOWER_OF_REFINEMENT_NOTIFY1:
			this->ProcState_NOTIFY1();
			break;
		case KANTURU_TOWER_OF_REFINEMENT_CLOSE:
			this->ProcState_CLOSE();
			break;
		case KANTURU_TOWER_OF_REFINEMENT_NOTIFY2:
			this->ProcState_NOTIFY2();
			break;
		case KANTURU_TOWER_OF_REFINEMENT_END:
			this->ProcState_END();
			break;
	}
}


void CKanturuTowerOfRefinement::SetState(int iTowerOfRefinement)
{
	if ( this->m_StateInfo[iTowerOfRefinement].GetCondition() > 0 )
	{
		this->m_StateInfo[iTowerOfRefinement].SetAppliedTime();
	}

	switch ( iTowerOfRefinement )
	{
		case KANTURU_TOWER_OF_REFINEMENT_NONE:
			this->SetState_NONE();
			break;
		case KANTURU_TOWER_OF_REFINEMENT_REVITALIZATION:
			this->SetState_REVITALIZATION();
			break;
		case KANTURU_TOWER_OF_REFINEMENT_NOTIFY1:
			this->SetState_NOTIFY1();
			break;
		case KANTURU_TOWER_OF_REFINEMENT_CLOSE:
			this->SetState_CLOSE();
			break;
		case KANTURU_TOWER_OF_REFINEMENT_NOTIFY2:
			this->SetState_NOTIFY2();
			break;
		case KANTURU_TOWER_OF_REFINEMENT_END:
			this->SetState_END();
			break;
	}

	KANTURU_UTIL.NotifyKanturuChangeState(KANTURU_STATE_TOWER_OF_REFINEMENT, this->m_iTowerOfRefinementState);
	g_KanturuBattleUserMng.ResetAllData();
}


void CKanturuTowerOfRefinement::SetNextState(int iCurrentState)
{
	int iNextState = iCurrentState + 1;

	if ( iNextState >= KANTURU_TOWER_OF_REFINEMENT_STATE_INFO )
	{
		iNextState = KANTURU_TOWER_OF_REFINEMENT_NONE;
	}

	this->SetState(iNextState);
}

void CKanturuTowerOfRefinement::SetState_NONE()
{
	g_Log.AddC(TColor::Brown,  "[Kanturu][Tower of Refinement] State(%d) -> NONE",
		this->m_iTowerOfRefinementState);

	this->SetTowerOfRefinementState(KANTURU_TOWER_OF_REFINEMENT_NONE);
	this->SetEntrancePermit(FALSE);
	this->SetUseTowerOfRefinement(FALSE);
	g_KanturuBattleUserMng.ResetAllData();
}

void CKanturuTowerOfRefinement::SetState_REVITALIZATION()
{
	g_Log.AddC(TColor::Brown,  "[Kanturu][Tower of Refinement] State(%d) -> REVITALIZATION",
		this->m_iTowerOfRefinementState);

	this->SetTowerOfRefinementState(KANTURU_TOWER_OF_REFINEMENT_REVITALIZATION);
	this->SetEntrancePermit(TRUE);
	this->SetUseTowerOfRefinement(TRUE);
}


void CKanturuTowerOfRefinement::SetState_NOTIFY1()
{
	g_Log.AddC(TColor::Brown,  "[Kanturu][Tower of Refinement] State(%d) -> NOTIFY1",
		this->m_iTowerOfRefinementState);

	this->SetTowerOfRefinementState(KANTURU_TOWER_OF_REFINEMENT_NOTIFY1);
	this->SetEntrancePermit(TRUE);
	this->SetUseTowerOfRefinement(TRUE);
	KANTURU_UTIL.SendMsgKauturuMapUser(Lang.GetText(0,260));
}


void CKanturuTowerOfRefinement::SetState_CLOSE()
{
	g_Log.AddC(TColor::Brown,  "[Kanturu][Tower of Refinement] State(%d) -> CLOSE",
		this->m_iTowerOfRefinementState);

	this->SetTowerOfRefinementState(KANTURU_TOWER_OF_REFINEMENT_CLOSE);
	this->SetEntrancePermit(FALSE);
	this->SetUseTowerOfRefinement(TRUE);
	KANTURU_UTIL.SendMsgKauturuMapUser(Lang.GetText(0,261));
}


void CKanturuTowerOfRefinement::SetState_NOTIFY2()
{
	g_Log.AddC(TColor::Brown,  "[Kanturu][Tower of Refinement] State(%d) -> NOTIFY2",
		this->m_iTowerOfRefinementState);

	this->SetTowerOfRefinementState(KANTURU_TOWER_OF_REFINEMENT_NOTIFY2);
	this->SetEntrancePermit(FALSE);
	this->SetUseTowerOfRefinement(TRUE);
}


void CKanturuTowerOfRefinement::SetState_END()
{
	g_Log.AddC(TColor::Brown,  "[Kanturu][Tower of Refinement] State(%d) -> END",
		this->m_iTowerOfRefinementState);

	this->SetTowerOfRefinementState(KANTURU_TOWER_OF_REFINEMENT_END);
	this->SetEntrancePermit(FALSE);
	this->SetUseTowerOfRefinement(FALSE);

	for ( int iCount=0;iCount<g_ConfigRead.server.GetObjectMax();iCount++)
	{
		if ( gObj[iCount].Type == OBJ_USER &&
			 gObj[iCount].MapNumber == MAP_INDEX_KANTURU_BOSS )
		{
			gObjMoveGate(iCount, 136);

			g_Log.Add("[Kanturu][Tower of Refinement] State End:Kick Out [%s][%s]",
				gObj[iCount].AccountID, gObj[iCount].Name);
		}
	}
}


void CKanturuTowerOfRefinement::ProcState_NONE()
{
	return;
}

void CKanturuTowerOfRefinement::ProcState_REVITALIZATION()
{
	return;
}

void CKanturuTowerOfRefinement::ProcState_NOTIFY1()
{
	return;
}

void CKanturuTowerOfRefinement::ProcState_CLOSE()
{
	return;
}

void CKanturuTowerOfRefinement::ProcState_NOTIFY2()
{
	return;
}

void CKanturuTowerOfRefinement::ProcState_END()
{
	return;
}


void CKanturuTowerOfRefinement::CheckStateTime()
{
	int iCurrentState = this->GetTowerOfRefinementState();

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


int CKanturuTowerOfRefinement::GetRemainTime()
{
	int iCurrentState = this->GetTowerOfRefinementState();
	return this->m_StateInfo[iCurrentState].GetRemainTime();
}


//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


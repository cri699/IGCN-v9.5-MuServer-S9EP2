// RaklionBattleOfSelupan.cpp: implementation of the CRaklionBattleOfSelupan class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RaklionBattleOfSelupan.h"
#include "RaklionBattleUserMng.h"
#include "RaklionUtil.h"
#include "Raklion.h"
#include "User.h"
#include "TLog.h"
#include "gObjMonster.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRaklionBattleOfSelupan::CRaklionBattleOfSelupan()
{

}

CRaklionBattleOfSelupan::~CRaklionBattleOfSelupan()
{

}

void CRaklionBattleOfSelupan::ResetAllData()
{
	m_bFileDataLoad = FALSE;
	m_iSelupanSkillDelay = 0;
	m_iSelupanSkillStartTime = 0;
	m_bIsRunFirstSkill = FALSE;

	memset(m_SelupanPattern, 0, sizeof(m_SelupanPattern));

	for (int iCount=0; iCount<7; iCount++)
	{
		m_SelupanPattern[iCount][1] = 1;
		m_SelupanPattern[iCount][2] = 1;
		m_SelupanPattern[iCount][3] = 1;
		m_SelupanPattern[iCount][7] = 1;
	}

	m_SelupanPattern[1][6] = 1;
	m_SelupanPattern[2][6] = 1;
	m_SelupanPattern[3][6] = 1;
	m_SelupanPattern[3][5] = 1;
	m_SelupanPattern[4][4] = 1;
	m_SelupanPattern[4][6] = 1;
	m_SelupanPattern[4][5] = 1;
	m_SelupanPattern[5][4] = 1;
	m_SelupanPattern[5][6] = 1;
	m_SelupanPattern[5][5] = 1;
	m_SelupanPattern[6][4] = 1;
	m_SelupanPattern[6][6] = 1;
	m_SelupanPattern[6][5] = 1;
	m_SelupanPattern[6][8] = 1;

	SetSuccessValue(FALSE);
	SetBattleOfSelupanState(0);
}

BOOL CRaklionBattleOfSelupan::LoadData(char *lpszFileName)
{
	return TRUE;
}

void CRaklionBattleOfSelupan::Run()
{
	if( m_RaklionSelupan.GetSelupanHP() <= 0 && GetBattleOfSelupanState() != 9 )
	{
		SetState(9);
		return;
	}

	if( m_iSelupanSkillDelay > GetTickCount64() - m_iSelupanSkillStartTime )	 return;

	switch ( m_iBattleOfSelupanState )
	{
	case 0:		ProcState_NONE();		break;
	case 1:		ProcState_STANDBY();	break;
	case 2:		ProcState_PATTERN_1();	break;
	case 3:		ProcState_PATTERN_2();	break;
	case 4:		ProcState_PATTERN_3();	break;
	case 5:		ProcState_PATTERN_4();	break;
	case 6:		ProcState_PATTERN_5();	break;
	case 7:		ProcState_PATTERN_6();	break;
	case 8:		ProcState_PATTERN_7();	break;
	case 9:		ProcState_DIE();		break;
	}
}

void CRaklionBattleOfSelupan::SetState(int iBattleOfSelupanState)
{
	if( m_iSelupanSkillDelay > GetTickCount64() - m_iSelupanSkillStartTime ) 	return;
		
	switch ( iBattleOfSelupanState )
	{
	case 0:		SetState_NONE();		break;
	case 1:		SetState_STANDBY();	break;
	case 2:		SetState_PATTERN_1();	break;
	case 3:		SetState_PATTERN_2();	break;
	case 4:		SetState_PATTERN_3();	break;
	case 5:		SetState_PATTERN_4();	break;
	case 6:		SetState_PATTERN_5();	break;
	case 7:		SetState_PATTERN_6();	break;
	case 8:		SetState_PATTERN_7();	break;
	case 9:		SetState_DIE();		break;
	}

	g_RaklionUtil.NotifyRaklionChangeState(RAKLION_STATE_DETAIL_STATE, GetBattleOfSelupanState());
}

void CRaklionBattleOfSelupan::SetState_NONE()
{
	g_Log.AddC(TColor::Brown, "[ RAKLION ][ BattleOfSelupan ] State(%d) -> NONE", m_iBattleOfSelupanState);
	
	m_RaklionSelupan.RaklionSelupanAct_Berserk_Cancel();
	SetBattleOfSelupanState(0);
}

void CRaklionBattleOfSelupan::SetState_STANDBY()
{
	g_Log.AddC(TColor::Brown, "[ RAKLION ][ BattleOfSelupan ] State(%d) -> STANDBY", m_iBattleOfSelupanState);
	
	m_RaklionSelupan.InitSelupanIndex();
	m_RaklionSelupan.RegenSelupan();
	m_RaklionSelupan.RaklionSelupanAct_Berserk_Cancel();
	SetBattleOfSelupanState(1);
}

void CRaklionBattleOfSelupan::SetState_PATTERN_1()
{
	g_Log.AddC(TColor::Brown, "[ RAKLION ][ BattleOfSelupan ] State(%d) -> PATTERN_1 // Life(%d/4800000)", m_iBattleOfSelupanState, m_RaklionSelupan.GetSelupanHP());
	
	m_RaklionSelupan.RaklionSelupanAct_Berserk_Cancel();
	SetBattleOfSelupanState(2);
}

void CRaklionBattleOfSelupan::SetState_PATTERN_2()
{
	g_Log.AddC(TColor::Brown, "[ RAKLION ][ BattleOfSelupan ] State(%d) -> PATTERN_2 // Life(%d/4800000)", m_iBattleOfSelupanState, m_RaklionSelupan.GetSelupanHP());

	m_RaklionSelupan.RaklionSelupanAct_Berserk1();  
	SetBattleOfSelupanState(3);
}

void CRaklionBattleOfSelupan::SetState_PATTERN_3()
{
	g_Log.AddC(TColor::Brown, "[ RAKLION ][ BattleOfSelupan ] State(%d) -> PATTERN_3 // Life(%d/4800000)", m_iBattleOfSelupanState, m_RaklionSelupan.GetSelupanHP());
	
	m_RaklionSelupan.RaklionSelupanAct_Berserk2();
	SetBattleOfSelupanState(4);
}

void CRaklionBattleOfSelupan::SetState_PATTERN_4()
{
	g_Log.AddC(TColor::Brown, "[ RAKLION ][ BattleOfSelupan ] State(%d) -> PATTERN_4 // Life(%d/4800000)", m_iBattleOfSelupanState, m_RaklionSelupan.GetSelupanHP());
	
	if( m_RaklionSelupan.GetBerserkLevel() != 2 )	m_RaklionSelupan.RaklionSelupanAct_Berserk2();
	SetBattleOfSelupanState(5);
}

void CRaklionBattleOfSelupan::SetState_PATTERN_5()
{
	g_Log.AddC(TColor::Brown, "[ RAKLION ][ BattleOfSelupan ] State(%d) -> PATTERN_5 // Life(%d/4800000)", m_iBattleOfSelupanState, m_RaklionSelupan.GetSelupanHP());
	
	m_RaklionSelupan.RaklionSelupanAct_Berserk3();
	SetBattleOfSelupanState(6);
}

void CRaklionBattleOfSelupan::SetState_PATTERN_6()
{
	g_Log.AddC(TColor::Brown, "[ RAKLION ][ BattleOfSelupan ] State(%d) -> PATTERN_6 // Life(%d/4800000)", m_iBattleOfSelupanState, m_RaklionSelupan.GetSelupanHP());
	
	m_RaklionSelupan.RaklionSelupanAct_Berserk4();
	SetBattleOfSelupanState(7);
}

void CRaklionBattleOfSelupan::SetState_PATTERN_7()
{
	g_Log.AddC(TColor::Brown, "[ RAKLION ][ BattleOfSelupan ] State(%d) -> PATTERN_7 // Life(%d/4800000)", m_iBattleOfSelupanState, m_RaklionSelupan.GetSelupanHP());
	
	if( m_RaklionSelupan.GetBerserkLevel() != 4 )	m_RaklionSelupan.RaklionSelupanAct_Berserk4();
	SetBattleOfSelupanState(8);
}

void CRaklionBattleOfSelupan::SetState_DIE()
{
	g_Log.AddC(TColor::Brown, "[ RAKLION ][ BattleOfSelupan ] State(%d) -> DIE", m_iBattleOfSelupanState);
	
	m_RaklionSelupan.RaklionSelupanAct_Berserk_Cancel();
	DeleteSummonMonster();

	int iSelupanIndex = GetSelupanObjIndex();
	int iWinner = gObjMonsterTopHitDamageUser(&gObj[iSelupanIndex]);

	if (iWinner == -1)
	{
		iWinner = m_RaklionSelupan.GetTargetOfSelupan();
	}

	if (ObjectMaxRange(iWinner) == true)
	{
		LPOBJ lpObj = &gObj[iWinner];
		g_RaklionUtil.SendMsgAllUser(Lang.GetText(0,307), lpObj->Name);
	}

	time_t t;
	time(&t);
	tm * pCurrentTime=localtime(&t);

	for(int iCount=0; iCount<g_RaklionBattleUserMng.GetUserCount(); iCount++)
	{
		g_Log.Add("[Raklion][SelupanDie] %d/%d/%d %d:%d:%d %dth Winner : %s (%s)",	
			pCurrentTime->tm_year+1900, pCurrentTime->tm_mon+1, pCurrentTime->tm_mday, 
			pCurrentTime->tm_hour, pCurrentTime->tm_min, pCurrentTime->tm_sec, iCount+1, 
			gObj[g_RaklionBattleUserMng.GetUserObjIndex(iCount)].Name, 
			gObj[g_RaklionBattleUserMng.GetUserObjIndex(iCount)].AccountID);
	}

	SetBattleOfSelupanState(9);
}

void CRaklionBattleOfSelupan::ProcState_NONE()
{
}

void CRaklionBattleOfSelupan::ProcState_STANDBY()
{
}

void CRaklionBattleOfSelupan::ProcState_PATTERN_1()
{
	int iAction = 0;
	while(m_SelupanPattern[0][iAction] == 0)
	{
		iAction = rand()%9;
	}
	ActionOfSelupan(iAction);
}

void CRaklionBattleOfSelupan::ProcState_PATTERN_2()
{
	int iAction = 0;
	while(m_SelupanPattern[1][iAction] == 0)
	{
		iAction = rand()%9;
	}
	ActionOfSelupan(iAction);
}

void CRaklionBattleOfSelupan::ProcState_PATTERN_3()
{
	int iAction = 0;
	while(m_SelupanPattern[2][iAction] == 0)
	{
		iAction = rand()%9;
	}
	ActionOfSelupan(iAction);
}

void CRaklionBattleOfSelupan::ProcState_PATTERN_4()
{
	int iAction = 0;
	while(m_SelupanPattern[3][iAction] == 0)
	{
		iAction = rand()%9;
	}
	ActionOfSelupan(iAction);
}

void CRaklionBattleOfSelupan::ProcState_PATTERN_5()
{
	int iAction = 0;
	while(m_SelupanPattern[4][iAction] == 0)
	{
		iAction = rand()%9;
	}
	ActionOfSelupan(iAction);
}

void CRaklionBattleOfSelupan::ProcState_PATTERN_6()
{
	int iAction = 0;
	while(m_SelupanPattern[5][iAction] == 0)
	{
		iAction = rand()%9;
	}
	ActionOfSelupan(iAction);
}

void CRaklionBattleOfSelupan::ProcState_PATTERN_7()
{
	int iAction = 0;
	while(m_SelupanPattern[6][iAction] == 0)
	{
		iAction = rand()%9;
	}
	ActionOfSelupan(iAction);
}

void CRaklionBattleOfSelupan::ProcState_DIE()
{
}

void CRaklionBattleOfSelupan::ActionOfSelupan(int iAction)
{
	int iSelupanIndex = m_RaklionSelupan.GetSelupanObjIndex();

	LPOBJ lpObj = &gObj[iSelupanIndex];

	if( lpObj->Teleport || lpObj->m_ActState.Move )	return;
	if( iAction < 0 || iAction >= 9 )	return;

	switch ( iAction )
    {
	case 0:	m_RaklionSelupan.RaklionSelupanAct_FirstSkill();	break;
	case 1:	m_RaklionSelupan.RaklionSelupanAct_PoisonAttack();	break;
	case 2:	m_RaklionSelupan.RaklionSelupanAct_IceStorm();		break;
	case 3:	m_RaklionSelupan.RaklionSelupanAct_IceStrike();		break;
	case 4:	m_RaklionSelupan.RaklionSelupanAct_SummonMonster();	break;
	case 5:	m_RaklionSelupan.RaklionSelupanAct_Heal();			break;
	case 6:	m_RaklionSelupan.RaklionSelupanAct_Freeze();		break;
	case 7:	m_RaklionSelupan.RaklionSelupanAct_Teleport();		break;
	case 8:	m_RaklionSelupan.RaklionSelupanAct_Invincibility();	break;
	}

	m_iSelupanSkillStartTime = GetTickCount64();
    m_iSelupanSkillDelay = m_RaklionSelupan.GetSelupanSkillDelay();
}

void CRaklionBattleOfSelupan::SetPatternCondition(int iPatternNum, int iCondition)
{
	if( iPatternNum < 0 || iCondition < 0 )
	{
		g_Log.AddC(TColor::Red, "[RAKLION][SetPatternCondition] error : PatternNum(%d), Condition(%d)", iPatternNum, iCondition);
	}

	m_iSelupanPatternCondition[iPatternNum] = iCondition;
}

void CRaklionBattleOfSelupan::SetBattleOfSelupanStateAuto()
{
	int iSelupanObjIndex = 0;
	int iSelupanMAXLife = 0;
	int iSelupanLifePercent = 0;
	LPOBJ lpObj = NULL;

	iSelupanObjIndex = m_RaklionSelupan.GetSelupanObjIndex();

	if (iSelupanObjIndex == -1)
	{
		return;
	}
	
	lpObj = &gObj[iSelupanObjIndex];
	
	if( lpObj->DieRegen )	return;

	iSelupanMAXLife = lpObj->AddLife + lpObj->MaxLife;
	if(iSelupanMAXLife <= 0)
	{
		g_Log.AddC(TColor::Red, "[RAKLION] SelupanMAXLife error : Index(%d), MaxLife(%d)", lpObj->m_Index, lpObj->MaxLife);
		return;
	}	

	iSelupanLifePercent = m_RaklionSelupan.GetSelupanHP() * 100 / iSelupanMAXLife;
	if( iSelupanLifePercent < 0 )
	{
		g_Log.AddC(TColor::Red, "[RAKLION] SelupanLifePercent error : Index(%d), MaxLife(%d)", lpObj->m_Index, iSelupanLifePercent);
		return;
	}
	
	if( m_bIsRunFirstSkill == FALSE )
    {
		ActionOfSelupan(0);
		m_bIsRunFirstSkill = TRUE;
		return;
	}

	if (iSelupanLifePercent > this->m_iSelupanPatternCondition[0])
	{
		if (this->GetBattleOfSelupanState() != 2)
		{
			this->SetState(2);
		}
	}
	else if (iSelupanLifePercent > this->m_iSelupanPatternCondition[1])
	{
		if (this->GetBattleOfSelupanState() != 3)
		{
			this->SetState(3);
		}
	}
	else if (iSelupanLifePercent > this->m_iSelupanPatternCondition[2])
	{
		if (this->GetBattleOfSelupanState() != 4)
		{
			this->SetState(4);
		}
	}

	else if (iSelupanLifePercent > this->m_iSelupanPatternCondition[3])
	{
		if (this->GetBattleOfSelupanState() != 5)
		{
			this->SetState(5);
		}
	}
	else if (iSelupanLifePercent > this->m_iSelupanPatternCondition[4])
	{
		if (this->GetBattleOfSelupanState() != 6)
		{
			this->SetState(6);
		}
	}
	else if (iSelupanLifePercent > this->m_iSelupanPatternCondition[5])
	{
		if (this->GetBattleOfSelupanState() != 7)
		{
			this->SetState(7);
		}
	}
	else if (iSelupanLifePercent > this->m_iSelupanPatternCondition[6])
	{
		if (this->GetBattleOfSelupanState() != 8)
		{
			this->SetState(8);
		}
	}
}

int CRaklionBattleOfSelupan::GetSelupanBattleUserCount()
{
	return g_RaklionBattleUserMng.GetUserCount();
}

void CRaklionBattleOfSelupan::RegenSelupan()
{
	m_RaklionSelupan.RegenSelupan();
}

void CRaklionBattleOfSelupan::HideSelupan()
{
	m_RaklionSelupan.HideSelupan();
}

int CRaklionBattleOfSelupan::GetSelupanObjIndex()
{
	return m_RaklionSelupan.GetSelupanObjIndex();
}

void CRaklionBattleOfSelupan::DeleteSummonMonster()
{
	m_RaklionSelupan.DeleteSummonMonster();
}

void CRaklionBattleOfSelupan::SetSelupanSkillDelay(int iDelay)
{
	m_RaklionSelupan.SetSelupanSkillDelay(iDelay);
}

void CRaklionBattleOfSelupan::GmCommandSelupanPattern(int iPatternNumber)
{
	if( iPatternNumber < 2 || iPatternNumber > 8 )
	{
		g_Log.AddC(TColor::Red, "[RAKLION] [GmCommandSelupanPattern] Invalid pattern number : %d", iPatternNumber);
		return;
	}
	
	switch( iPatternNumber )
	{
	case 2: m_RaklionSelupan.SetSelupanHP(100);	break;
	case 3: m_RaklionSelupan.SetSelupanHP(80);	break;
	case 4:	m_RaklionSelupan.SetSelupanHP(60);	break;
	case 5:	m_RaklionSelupan.SetSelupanHP(50);	break;
	case 6:	m_RaklionSelupan.SetSelupanHP(40);	break;
	case 7:	m_RaklionSelupan.SetSelupanHP(20);	break;
	case 8:	m_RaklionSelupan.SetSelupanHP(10);	break;
	}
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


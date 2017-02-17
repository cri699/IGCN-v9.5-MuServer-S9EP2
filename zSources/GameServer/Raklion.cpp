// Raklion.cpp: implementation of the CRaklion class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Raklion.h"
#include "user.h"
#include "RaklionUtil.h"
#include "RaklionBattleUserMng.h"
#include "RaklionMonsterMng.h"
#include "gObjMonster.h"
#include "GameMain.h"
#include "configread.h"

CRaklion	g_Raklion;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRaklion::CRaklion()
{
	tm *pCurrentTime;
	time_t t;

	time(&t);
	pCurrentTime = localtime(&t);

	m_iTodayYear = pCurrentTime->tm_year;
	m_iTodayMon = pCurrentTime->tm_mon;
	m_iTodayDay = pCurrentTime->tm_mday;
	m_bEvent = FALSE;

	ResetAllData();
}

CRaklion::~CRaklion()
{

}

void CRaklion::Init()
{
	if (g_ConfigRead.server.GetStateFromEventTable(g_ConfigRead.server.GetServerType(), EV_TABLE_RAKLION) == false)
	{
		this->SetRaklionBossEnable(false);
	}

	if (this->IsRaklionBossEnable() == true)
	{
		g_RaklionMonsterMng.SetAllMonsters();
		this->SetState(RAKLION_STATE_END);
	}
}

void CRaklion::ResetAllData()
{
	m_bIsBossKillSuccess = FALSE;
	m_bFileDataLoad = FALSE;
	m_iBossAppearanceStartTime = 0;
	m_iBosszoneCloseStartTime = 0;
	m_iBosszoneOpenStartTime = 0;
	m_bIsNotify1 = FALSE;
	m_iBossEggDieCount = 0;
}

BOOL CRaklion::LoadData(char *lpszFileName)
{
	m_bFileDataLoad = FALSE;
	BOOL iRaklionEnable = FALSE;

	if( lpszFileName == NULL || strcmp(lpszFileName,"") == 0 )
	{
		g_Log.MsgBox("[ RAKLION ] - File load error : File Name Error");
		return FALSE;
	}

	try
	{
		pugi::xml_document file;
		pugi::xml_parse_result res = file.load_file(lpszFileName);

		if (res.status != pugi::status_ok)
		{
			g_Log.MsgBox("[ RAKLION ] - Can't Load %s (%s)", lpszFileName, res.description());
			return FALSE;
		}

		pugi::xml_node main = file.child("RaklionEvent");

		iRaklionEnable = main.attribute("Enable").as_int();

		if (g_ConfigRead.server.GetStateFromEventTable(g_ConfigRead.server.GetServerType(), EV_TABLE_RAKLION) == false)
		{
			iRaklionEnable = FALSE;
		}

		pugi::xml_node time = main.child("Time");

		m_iBossAppearanceDelay	= time.attribute("BossAppearance").as_int() * 1000;
		m_iBosszoneCloseDelay	= time.attribute("BossZoneClose").as_int() * 1000;
		m_iBosszoneOpenDelay	= time.attribute("EventStart").as_int() * 1000;

		int iBossSkillDelay = time.attribute("SelupanSkillHitDelay").as_int();

		if( iBossSkillDelay < 0)
		{
			g_Log.MsgBox("[ RAKLION ] - BossSkillDelay count error : (%d)", iBossSkillDelay);
			return FALSE;
		}

		m_BattleOfSelupan.SetSelupanSkillDelay(iBossSkillDelay);

		pugi::xml_node eggcount = main.child("EggCount");

		m_iBossEggHalf = eggcount.attribute("Half").as_int();
		m_iBossEggMax = eggcount.attribute("Max").as_int();

		if(m_iBossEggHalf < 0 )
		{
			g_Log.MsgBox("[ RAKLION ] - BossEggHalf count error : (%d)", m_iBossEggHalf);
			return FALSE;
		}

		if(m_iBossEggMax < 0 )
		{
			g_Log.MsgBox("[ RAKLION ] - BossEggMax count error : (%d)", m_iBossEggMax);
			return FALSE;
		}

		pugi::xml_node skill_patterns = main.child("SkillPatterns");
		int iBossPatternCondition[6] = { 0 };
		int iCount = 0;

		for (pugi::xml_attribute pattern = skill_patterns.first_attribute(); pattern; pattern = pattern.next_attribute())
		{
			iBossPatternCondition[iCount] = pattern.as_int();
			iCount++;

			if (iCount >= 6)
			{
				break;
			}
		}

		for(int iCount = 0; iCount < 6; iCount++)
		{
			if(iBossPatternCondition[iCount] < 0)
			{
				g_Log.MsgBox("[ RAKLION ] - BossPatternCondition[%d] error : (%d)", iCount, iBossPatternCondition[iCount]);
				return FALSE;
			}

			m_BattleOfSelupan.SetPatternCondition(iCount,iBossPatternCondition[iCount]);
		}

		g_Log.AddC(TColor::Red, "[ RAKLION ] - %s file is Loaded", lpszFileName);
		m_bFileDataLoad = TRUE;
	}

	catch(...)
	{
		g_Log.MsgBox("[ RAKLION ] - Loading Exception Error (%s) File. ", lpszFileName);
	}

	if (m_bFileDataLoad == TRUE)
	{
		this->SetRaklionBossEnable(iRaklionEnable);
	}

	else
	{
		this->SetRaklionBossEnable(FALSE);
	}

	return m_bFileDataLoad;
}

void CRaklion::LoadRaklionMapAttr(char *lpszFileName, BYTE btLevel)
{
	return;
}

void CRaklion::Run()
{
	int iRaklionUserCount = 0;

	time_t t;
	tm *pCurrentTime;

	time(&t);
	pCurrentTime = localtime(&t);

	if( m_iTodayDay != pCurrentTime->tm_mday || m_iTodayMon != pCurrentTime->tm_mon || m_iTodayYear != pCurrentTime->tm_year )
	{
		m_iTodayDay = pCurrentTime->tm_mday;
		m_iTodayMon = pCurrentTime->tm_mon;
		m_iTodayYear = pCurrentTime->tm_year;
		m_bEvent = FALSE;
	}

	if( !m_bEvent && pCurrentTime->tm_hour == 0 && pCurrentTime->tm_min == 0 && pCurrentTime->tm_sec == 0 )
	{
		for(int iCount = g_ConfigRead.server.GetObjectStartUserIndex(); iCount < g_ConfigRead.server.GetObjectMax(); iCount++)
		{
			if( gObj[iCount].MapNumber == MAP_INDEX_RAKLION || 
				gObj[iCount].MapNumber == MAP_INDEX_HATCHERY )
			{
				if( gObj[iCount].Connected == PLAYER_PLAYING && 
					gObj[iCount].Type == OBJ_USER )
				{
					iRaklionUserCount++;
					g_Log.Add("[RAKLION][IsInRaklionEvent] %d/%d/%d %dth User : %s (%s)", pCurrentTime->tm_year+1900, pCurrentTime->tm_mon+1, pCurrentTime->tm_mday, iRaklionUserCount, gObj[iCount].Name, gObj[iCount].AccountID);
				}
			}
		}
		m_bEvent = TRUE;
	}

	if( !IsRaklionBossEnable() )	return;

	CheckUserOnRaklionBossMap();

	g_RaklionBattleUserMng.CheckUserState();

	switch( m_iRaklionState )
	{
	case RAKLION_STATE_IDLE:			ProcState_IDLE();			break;
	case RAKLION_STATE_NOTIFY_1:		ProcState_NOTIFY_1();		break;
	case RAKLION_STATE_STANDBY:			ProcState_STANDBY();		break;
	case RAKLION_STATE_NOTIFY_2:		ProcState_NOTIFY_2();		break;
	case RAKLION_STATE_READY:			ProcState_READY();			break;
	case RAKLION_STATE_START_BATTLE:	ProcState_START_BATTLE();	break;
	case RAKLION_STATE_NOTIFY_3:		ProcState_NOTIFY_3();		break;
	case RAKLION_STATE_CLOSE_DOOR:		ProcState_CLOSE_DOOR();		break;
	case RAKLION_STATE_ALL_USER_DIE:	ProcState_ALL_USER_DIE();	break;
	case RAKLION_STATE_NOTIFY_4:		ProcState_NOTIFY_4();		break;
	case RAKLION_STATE_END:				ProcState_END();			break;
	}
}

void CRaklion::SetState(int iRaklionState)
{
	switch ( iRaklionState )
	{
	case RAKLION_STATE_IDLE:			SetState_IDLE();			break;
	case RAKLION_STATE_NOTIFY_1:		SetState_NOTIFY_1();		break;
	case RAKLION_STATE_STANDBY:			SetState_STANDBY();			break;
	case RAKLION_STATE_NOTIFY_2:		SetState_NOTIFY_2();		break;
	case RAKLION_STATE_READY:			SetState_READY();			break;
	case RAKLION_STATE_START_BATTLE:	SetState_START_BATTLE();	break;
	case RAKLION_STATE_NOTIFY_3:		SetState_NOTIFY_3();		break;
	case RAKLION_STATE_CLOSE_DOOR:		SetState_CLOSE_DOOR();		break;
	case RAKLION_STATE_ALL_USER_DIE:	SetState_ALL_USER_DIE();	break;
	case RAKLION_STATE_NOTIFY_4:		SetState_NOTIFY_4();		break;
	case RAKLION_STATE_END:				SetState_END();				break;
	}

	g_RaklionUtil.NotifyRaklionChangeState(GetRaklionState(), m_BattleOfSelupan.GetBattleOfSelupanState());
}

void CRaklion::SetNextState(RAKLION_STATE iCurrentState)
{
	int iNextState = iCurrentState + 1;
	if( iNextState > RAKLION_STATE_MAX )	iNextState = RAKLION_STATE_IDLE;

	SetState(iNextState);
}

void CRaklion::ChangeState(int iState, int DetailState)
{
}

void CRaklion::SetState_IDLE()
{
	g_Log.AddC(TColor::Pink, "[ RAKLION ] State(%d) -> IDLE", m_iRaklionState);
	SetRaklionState(RAKLION_STATE_IDLE);
}

void CRaklion::SetState_NOTIFY_1()
{
	g_Log.AddC(TColor::Pink, "[ RAKLION ] State(%d) -> NOTIFY_1", m_iRaklionState);
	SetRaklionState(RAKLION_STATE_NOTIFY_1);
}

void CRaklion::SetState_STANDBY()
{
	g_Log.AddC(TColor::Pink, "[ RAKLION ] State(%d) -> STANDBY", m_iRaklionState);
	m_iBossAppearanceStartTime = GetTickCount64();
	SetRaklionState(RAKLION_STATE_STANDBY);
}

void CRaklion::SetState_NOTIFY_2()
{
	g_Log.AddC(TColor::Pink, "[ RAKLION ] State(%d) -> NOTIFY_2", m_iRaklionState);
	int iDelayMinute = m_iBosszoneCloseDelay / 60 / 1000;
	if(iDelayMinute == 0)	iDelayMinute = 1;
	g_RaklionUtil.SendMsgAllUser(Lang.GetText(0,293), iDelayMinute);
	SetRaklionState(RAKLION_STATE_NOTIFY_2);
}

void CRaklion::SetState_READY()
{
	g_Log.AddC(TColor::Pink, "[ RAKLION ] State(%d) -> READY", m_iRaklionState);
	m_BattleOfSelupan.SetState(1);
	SetRaklionState(RAKLION_STATE_READY);
}

void CRaklion::SetState_START_BATTLE()
{
	g_Log.AddC(TColor::Pink, "[ RAKLION ] State(%d) -> START_BATTLE", m_iRaklionState);
	m_iBosszoneCloseStartTime = GetTickCount64();
	SetRaklionState(RAKLION_STATE_START_BATTLE);
}

void CRaklion::SetState_NOTIFY_3()
{
	g_Log.AddC(TColor::Pink, "[ RAKLION ] State(%d) -> NOTIFY_3", m_iRaklionState);
	g_RaklionUtil.SendMsgAllUser(Lang.GetText(0,296));
	SetRaklionState(RAKLION_STATE_NOTIFY_3);
}

void CRaklion::SetState_CLOSE_DOOR()
{
	g_Log.AddC(TColor::Pink, "[ RAKLION ] State(%d) -> CLOSE_DOOR", m_iRaklionState);
	SetRaklionState(RAKLION_STATE_CLOSE_DOOR);
}

void CRaklion::SetState_ALL_USER_DIE()
{
	g_Log.AddC(TColor::Pink, "[ RAKLION ] State(%d) -> ALL_USER_DIE", m_iRaklionState);
	m_BattleOfSelupan.HideSelupan();
	m_BattleOfSelupan.DeleteSummonMonster();
	SetRaklionState(RAKLION_STATE_ALL_USER_DIE);
}

void CRaklion::SetState_NOTIFY_4()
{
	g_Log.AddC(TColor::Pink, "[ RAKLION ] State(%d) -> NOTIFY_4", m_iRaklionState);
	m_iBosszoneOpenStartTime = GetTickCount64();
	BOOL bBattleSuccess = m_BattleOfSelupan.GetSuccessValue();
	int iDelayMinute = m_iBosszoneOpenDelay / 60 / 1000;
	if(iDelayMinute == 0)	iDelayMinute = 1;
	g_RaklionUtil.SendMsgAllUser(Lang.GetText(0,295), iDelayMinute);
	g_RaklionUtil.NotifyRaklionSuccessValue(bBattleSuccess);
	SetRaklionState(RAKLION_STATE_NOTIFY_4);
}

void CRaklion::SetState_END()
{
	g_Log.AddC(TColor::Pink, "[ RAKLION ] State(%d) -> END", m_iRaklionState);
	m_BattleOfSelupan.HideSelupan();
	m_BattleOfSelupan.DeleteSummonMonster();
	m_BattleOfSelupan.ResetAllData();
	ResetAllData();
	g_RaklionUtil.SendMsgAllUser(Lang.GetText(0,294));
	SetRaklionState(RAKLION_STATE_END);
	RegenBossEgg();
}

void CRaklion::ProcState_IDLE()
{
	if( GetBossEggDieCount() == 0 )
	{
		SetState(RAKLION_STATE_STANDBY);
		return;
	}
	if( !m_bIsNotify1 )
	{
		if( GetBossEggDieCount() == m_iBossEggHalf )
		{
			SetState(RAKLION_STATE_NOTIFY_1);
			m_bIsNotify1 = TRUE;
		}
	}
}

void CRaklion::ProcState_NOTIFY_1()
{
	SetState(RAKLION_STATE_IDLE);
}

void CRaklion::ProcState_STANDBY()
{
	if( m_iBossAppearanceDelay <= GetTickCount64() - m_iBossAppearanceStartTime )
	{
		SetState(RAKLION_STATE_NOTIFY_2);
	}
}

void CRaklion::ProcState_NOTIFY_2()
{
	SetState(RAKLION_STATE_READY);
}

void CRaklion::ProcState_READY()
{
	SetState(RAKLION_STATE_START_BATTLE);
}

void CRaklion::ProcState_START_BATTLE()
{
	if( m_iBosszoneCloseDelay < GetTickCount64() - m_iBosszoneCloseStartTime )
	{
		SetState(RAKLION_STATE_NOTIFY_3);
	}

	m_BattleOfSelupan.SetBattleOfSelupanStateAuto();
	m_BattleOfSelupan.Run();
}

void CRaklion::ProcState_NOTIFY_3()
{
	m_BattleOfSelupan.SetBattleOfSelupanStateAuto();
	m_BattleOfSelupan.Run();
	SetState(RAKLION_STATE_CLOSE_DOOR);
}

void CRaklion::ProcState_CLOSE_DOOR()
{
	if (m_BattleOfSelupan.GetBattleOfSelupanState() == 9)
	{
		m_BattleOfSelupan.SetSuccessValue(TRUE);
		SetState(RAKLION_STATE_NOTIFY_4);
		return;
	}

	if( g_RaklionBattleUserMng.IsEmpty() == 1)
	{
		m_BattleOfSelupan.SetSuccessValue(FALSE);
		SetState(RAKLION_STATE_ALL_USER_DIE);
		return;
	}		

	m_BattleOfSelupan.SetBattleOfSelupanStateAuto();
	m_BattleOfSelupan.Run();
}

void CRaklion::ProcState_ALL_USER_DIE()
{
	SetState(RAKLION_STATE_NOTIFY_4);
}

void CRaklion::ProcState_NOTIFY_4()
{
	if( m_iBosszoneOpenDelay < GetTickCount64() - m_iBosszoneOpenStartTime )
	{
		SetState(RAKLION_STATE_END);
	}
}

void CRaklion::ProcState_END()
{
	SetState(RAKLION_STATE_IDLE);
}

void CRaklion::SetRaklionBossEnable(BOOL bEnable)
{
	m_bRaklionBossEnable = bEnable;
}

BOOL CRaklion::IsRaklionBossEnable()
{
	return m_bRaklionBossEnable;
}

void CRaklion::SetRaklionState(RAKLION_STATE iRaklionState)
{
	m_iRaklionState = iRaklionState;
}

int CRaklion::GetRaklionState()
{
	return m_iRaklionState;
}

void CRaklion::SetRaklionStateDetail(int iSelupanState)
{
	m_BattleOfSelupan.SetBattleOfSelupanState(iSelupanState);
}

int CRaklion::GetRaklionStateDetail()
{
	return m_BattleOfSelupan.GetBattleOfSelupanState();
}

void CRaklion::BossEggDieIncrease()
{
	if( m_iBossEggDieCount >= m_iBossEggMax )
	{
		g_Log.AddC(TColor::Red, "[RAKLION] BossEggDieCount Error : (%d)", m_iBossEggDieCount);
		return;
	}
	m_iBossEggDieCount++;
}

void CRaklion::BossEggDieDecrease()
{
	if( m_iBossEggDieCount <= 0 )
	{
		g_Log.AddC(TColor::Red, "[RAKLION] BossEggDieCount Error : (%d)", m_iBossEggDieCount);
		return;
	}
	m_iBossEggDieCount--;
}

DWORD CRaklion::GetBossEggDieCount()
{
	return m_iBossEggDieCount;
}

void CRaklion::RegenBossEgg()
{
	for (int iCount = 0; iCount < g_ConfigRead.server.GetObjectMaxMonster(); iCount++)
	{
		if (gObj[iCount].Class == 460 || gObj[iCount].Class == 461 || gObj[iCount].Class == 462)
		{
			gObjSetMonster(gObj[iCount].m_Index, gObj[iCount].Class);

			if (gObjMonsterRegen(&gObj[iCount]) == TRUE)
			{
				gObj[iCount].Live = TRUE;
				gObj[iCount].DieRegen = FALSE;
				BossEggDieIncrease();
			}
		}
	}

	g_Log.AddC(TColor::Brown, "[ RAKLION ] [ BossEggRegen ] : BossEgg(%d ea)", GetBossEggDieCount());
}

void CRaklion::DeleteBossEgg()
{
	for(int iCount=0; iCount<g_ConfigRead.server.GetObjectMaxMonster(); iCount++)
	{
		if (gObj[iCount].Class == 460 || gObj[iCount].Class == 461 || gObj[iCount].Class == 462 )
		{
			if (gObj[iCount].Connected == PLAYER_PLAYING)
			{
				gObjDel(iCount);
				gObj[iCount].Live = 0;
				gObj[iCount].m_State = 0;
				gObjViewportListProtocolDestroy(&gObj[iCount]);
				gObjViewportClose(&gObj[iCount]);
				BossEggDieDecrease();
			}
		}
	}

	g_Log.AddC(TColor::Brown, "[ RAKLION ] [ BossEggDelete ] : BossEgg(%d ea)", GetBossEggDieCount());
}

void CRaklion::CheckUserOnRaklionBossMap()
{
	for (int iCount = g_ConfigRead.server.GetObjectStartUserIndex(); iCount < g_ConfigRead.server.GetObjectMax(); iCount++)
	{
		if (gObj[iCount].Connected == PLAYER_PLAYING &&
			gObj[iCount].Type == OBJ_USER &&
			gObj[iCount].MapNumber == MAP_INDEX_HATCHERY)
		{
			if (g_RaklionBattleUserMng.IsBattleUser(iCount))	continue;

			if (gObj[iCount].RegenOk == 0 && gObj[iCount].m_State == 2 && gObj[iCount].Live == 1)
			{
				gObjMoveGate(iCount, 286);
				g_Log.AddC(TColor::Red, "[ RAKLION ][ Invalid User ] Invalid Raklion Boss Map User[%s][%s]", gObj[iCount].AccountID, gObj[iCount].Name);
			}
		}
	}
}

int CRaklion::CheckEnterRaklion(int iUserIndex)
{
	if( !gObjIsConnected(iUserIndex) )	return -1;

	if( GetRaklionState() == RAKLION_STATE_CLOSE_DOOR )
	{
		g_Log.Add("[ RAKLION ][ Entrance Fail ] [%s][%s] State(%d)", gObj[iUserIndex].AccountID, gObj[iUserIndex].Name, GetRaklionState());
		return 3;
	}
	if( gObj[iUserIndex].MapNumber != MAP_INDEX_HATCHERY ) 
	{
		g_Log.Add("[ RAKLION ][ Entrance Fail ] Invalid Map Number(%d) [%s][%s] State(%d)", gObj[iUserIndex].MapNumber, gObj[iUserIndex].AccountID, gObj[iUserIndex].Name, GetRaklionState());
		return 4;
	}
	return 0;
}

void CRaklion::OperateGmCommand(int iUserIndex, int iCommand)
{
	return;
}

void CRaklion::UserMonsterCountCheck()
{
	return;
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


// Kanturu.cpp: implementation of the CKanturu class.
//	GS-N	1.00.18	JPN	0x00581CD0	-	Completed
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Kanturu.h"
#include "KanturuMonsterMng.h"
#include "KanturuBattleUserMng.h"
#include "KanturuUtil.h"
#include "Gamemain.h"
#include "TLog.h"
#include "CashShop.h"
#include "configread.h"

static CKanturuUtil KANTURU_UTIL;
CKanturu g_Kanturu;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKanturu::CKanturu()
{
	this->m_bKanturuEnable = FALSE;
	this->m_iKanturuState = 0;
	this->m_StateInfoCount = 0;
	this->m_bFileDataLoad = FALSE;
	this->m_bEnableCheckMoonStone = FALSE;
	this->m_iKanturuBattleCounter = 0;
	this->m_iKanturuBattleDate = 0;
}

CKanturu::~CKanturu()
{
	return;
}


void CKanturu::ResetAllData()
{
	this->m_StateInfoCount = 0;

	for ( int iCount=0;iCount<MAX_KANTURU_STATE_INFO;iCount++)
	{
		this->m_StateInfo[iCount].ResetTimeInfo();
	}
}


BOOL CKanturu::LoadData(LPSTR lpszFileName)
{
	this->m_bFileDataLoad = FALSE;

	if ( !lpszFileName || !strcmp(lpszFileName, ""))
	{
		g_Log.MsgBox("[Kanturu] - File load error : File Name Error");
		return FALSE;
	}

	try
	{
		pugi::xml_document file;
		pugi::xml_parse_result res = file.load_file(lpszFileName);

		if ( res.status != pugi::status_ok )
		{
			g_Log.MsgBox("[Kanturu] - Can't Load %s (%s)", lpszFileName, res.description());
			return FALSE;
		}

		this->ResetAllData();

		pugi::xml_node main = file.child("KanturuEvent");

		this->SetKanturuEnable(g_ConfigRead.server.GetStateFromEventTable(g_ConfigRead.server.GetServerType(), EV_TABLE_KANTURU) == true ? main.attribute("Enable").as_bool() : false);
		this->SetEnableCheckMoonStone(main.attribute("MoonStoneCheck").as_bool());

		pugi::xml_node item_drop = main.child("Item");

		this->m_iKanturuMoonStoneDropRate = item_drop.attribute("MoonStoneDropRate").as_int();
		this->m_iKanturuJewelOfHarmonyDropRate = item_drop.attribute("JewelOfHarmonyDropRate").as_int();

		pugi::xml_node states = main.child("GeneralStates");

		for (pugi::xml_node state = states.child("Event"); state; state = state.next_sibling())
		{
			int iState = state.attribute("State").as_int();
			int iCondition = state.attribute("SwitchType").as_int();
			int iValue = state.attribute("Time").as_int();

			if ( this->m_StateInfoCount < 0 || this->m_StateInfoCount >= MAX_KANTURU_STATE_INFO )
			{
				g_Log.MsgBox("[Kanturu] - Exceed Max State Time (%d)", this->m_StateInfoCount);
				break;
			}

			this->m_StateInfo[this->m_StateInfoCount].SetStateInfo(iState);
			this->m_StateInfo[this->m_StateInfoCount].SetCondition(iCondition);

			if ( iCondition == 1 )
				iValue *= 1000;

			this->m_StateInfo[this->m_StateInfoCount].SetValue(iValue);

			this->m_StateInfoCount++;
		}

		g_Log.AddC(TColor::Red,  "[Kanturu] - %s file is Loaded", lpszFileName);
		
		this->m_bFileDataLoad = TRUE;
		this->m_BattleStanby.LoadData(lpszFileName);
		this->m_BattleOfMaya.LoadData(lpszFileName);
		this->m_BattleOfNightmare.LoadData(lpszFileName);
		this->m_TowerOfRefinement.LoadData(lpszFileName);
	}

	catch ( DWORD )
	{
		g_Log.MsgBox("[Kanturu] - Loading Exception Error (%s) File. ", lpszFileName);
	}

	return this->m_bFileDataLoad;
}



void CKanturu::LoadKanturuMapAttr(LPSTR lpszFileName, BYTE btLevel)
{
	if ( !lpszFileName || btLevel > MAX_KANTURU_MAP_LEVEL )
		return;

	switch ( btLevel )
	{
		case KANTURU_MAP_CLOSE:
			this->m_KanturuMap[btLevel].LoadMapAttr(lpszFileName, MAP_INDEX_KANTURU_BOSS);
			break;
		case KANTURU_MAP_OPEN:
			this->m_KanturuMap[btLevel].LoadMapAttr(lpszFileName, MAP_INDEX_KANTURU_BOSS);
			break;
	}
}


void CKanturu::SetKanturuMapAttr(BYTE btLevel)
{
	MapClass & KanturuMap = this->m_KanturuMap[btLevel];
	memcpy(MapC[MAP_INDEX_KANTURU_BOSS].m_attrbuf, KanturuMap.m_attrbuf, 256*256);

	g_Log.AddC(TColor::Red,  "[Kanturu][MapAttrChange] Map(%d) State(%d) DetailState(%d)",
		btLevel, this->GetKanturuState(), this->GetKanturuDetailState());

}

void CKanturu::Init()
{
	if (g_ConfigRead.server.GetStateFromEventTable(g_ConfigRead.server.GetServerType(), EV_TABLE_KANTURU) == false)
	{
		this->SetKanturuEnable(false);
	}
}

void CKanturu::Run()
{
	if ( !this->IsKanturuEnable() )
		return;

	this->CheckStateTime();
	this->CheckUserOnKanturuBossMap();

	switch ( this->m_iKanturuState )
	{
		case KANTURU_STATE_NONE:
			this->ProcState_NONE();
			break;
		case KANTURU_STATE_BATTLE_STANTBY:
			this->ProcState_BATTLE_STANDBY();
			break;
		case KANTURU_STATE_BATTLE_OF_MAYA:
			this->ProcState_BATTLE_OF_MAYA();
			break;
		case KANTURU_STATE_BATTLE_OF_NIGHTMARE:
			this->ProcState_BATTLE_OF_NIGHTMARE();
			break;
		case KANTURU_STATE_TOWER_OF_REFINEMENT:
			this->ProcState_TOWER_OF_REFINEMENT();
			break;
		case KANTURU_STATE_END:
			this->ProcState_END();
			break;
	}
}

void CKanturu::SetState(int iKanturuState)
{
	if ( this->m_StateInfo[iKanturuState].GetCondition() > 0 )
	{
		this->m_StateInfo[iKanturuState].SetAppliedTime();
	}

	g_KanturuMonsterMng.ResetRegenMonsterObjData();
	this->SetKanturuMapAttr(0);

	switch ( iKanturuState )
	{
		case KANTURU_STATE_NONE:
			this->SetState_NONE();
			break;
		case KANTURU_STATE_BATTLE_STANTBY:
			this->SetState_BATTLE_STANDBY();
			break;
		case KANTURU_STATE_BATTLE_OF_MAYA:
			this->SetState_BATTLE_OF_MAYA();
			break;
		case KANTURU_STATE_BATTLE_OF_NIGHTMARE:
			this->SetState_BATTLE_OF_NIGHTMARE();
			break;
		case KANTURU_STATE_TOWER_OF_REFINEMENT:
			this->SetState_TOWER_OF_REFINEMENT();
			break;
		case KANTURU_STATE_END:
			this->SetState_END();
			break;
	}
}

void CKanturu::SetNextState(int iCurrentState)
{
	int iNextState = iCurrentState + 1;

	if ( iNextState >= MAX_KANTURU_STATE_INFO )
	{
		iNextState = KANTURU_STATE_NONE;
	}

	this->SetState(iNextState);
}

#pragma warning ( disable : 4060 )
void CKanturu::ChangeState(int iState, int DetailState)
{
	switch ( iState )
	{
	}
}
#pragma warning ( default : 4060 )

void CKanturu::SetState_NONE()
{
	g_Log.AddC(TColor::Purple,  "[Kanturu] State(%d) -> NONE", this->m_iKanturuState);
	this->SetKanturuState(KANTURU_STATE_NONE);
}

void CKanturu::SetState_BATTLE_STANDBY()
{
	g_Log.AddC(TColor::Purple,  "[Kanturu] State(%d) -> STANDBY", this->m_iKanturuState);
	g_KanturuBattleUserMng.ResetAllData();
	this->SetKanturuState(KANTURU_STATE_BATTLE_STANTBY);
	this->m_BattleStanby.SetState(1);
	this->SetKanturuTimeAttackEventInfo();
}

void CKanturu::SetState_BATTLE_OF_MAYA()
{
	g_Log.AddC(TColor::Purple,  "[Kanturu] State(%d) -> BATTLE_OF_MAYA", this->m_iKanturuState);
	this->SetKanturuState(KANTURU_STATE_BATTLE_OF_MAYA);
	this->m_BattleOfMaya.SetState(1);
}

void CKanturu::SetState_BATTLE_OF_NIGHTMARE()
{
	g_Log.AddC(TColor::Purple,  "[Kanturu] State(%d) -> BATTLE_OF_NIGHTMARE", this->m_iKanturuState);
	this->SetKanturuState(KANTURU_STATE_BATTLE_OF_NIGHTMARE);
	this->m_BattleOfNightmare.SetState(1);
}

void CKanturu::SetState_TOWER_OF_REFINEMENT()
{
	g_Log.AddC(TColor::Purple,  "[Kanturu] State(%d) -> TOWER_OF_REFINEMENT", this->m_iKanturuState);
	this->SetKanturuState(KANTURU_STATE_TOWER_OF_REFINEMENT);
	this->m_TowerOfRefinement.SetState(1);
	this->SetKanturuMapAttr(1);
}

void CKanturu::SetState_END()
{
	g_Log.AddC(TColor::Purple,  "[Kanturu] State(%d) -> END", this->m_iKanturuState);
	this->SetKanturuState(KANTURU_STATE_END);
	g_KanturuBattleUserMng.ResetAllData();
}

void CKanturu::ProcState_NONE()
{
	this->SetState(KANTURU_STATE_BATTLE_STANTBY);
}

void CKanturu::ProcState_BATTLE_STANDBY()
{
	if ( this->m_BattleStanby.GetBattleStanbyState() == 3 )
	{
		this->m_BattleStanby.SetState_NONE();
		this->SetState(KANTURU_STATE_BATTLE_OF_MAYA);
	}
	else
	{
		this->m_BattleStanby.Run();
	}
}

void CKanturu::ProcState_BATTLE_OF_MAYA()
{
	if ( this->m_BattleOfMaya.GetBattleOfMayaState() == 18 )
	{
		this->m_BattleOfMaya.SetState_NONE();

		if ( this->m_BattleOfMaya.GetSuccessValue() == 1 )
		{
			this->SetState(KANTURU_STATE_BATTLE_OF_NIGHTMARE);
		}
		else
		{
			this->SetState(KANTURU_STATE_END);
		}
	}
	else
	{
		this->m_BattleOfMaya.Run();
	}
}

void CKanturu::ProcState_BATTLE_OF_NIGHTMARE()
{
	if ( this->m_BattleOfNightmare.GetBattleOfNightmareState() == 5 )
	{
		this->m_BattleOfNightmare.SetState(0);

		if ( this->m_BattleOfNightmare.GetSuccessValue() == 1 )
		{
			for(int i=0;i<g_KanturuBattleUserMng.GetUserCount(); ++i)
			{
				int iIndex = g_KanturuBattleUserMng.m_BattleUser[i].GetIndex();

				if(ObjectMaxRange(iIndex) && gObjIsConnected(iIndex) && gObj[iIndex].Type == OBJ_USER)
				{
					g_CashShop.AddCoin(&gObj[iIndex], EVENT_DEEP);
				}
			}
				
			this->SetState(KANTURU_STATE_TOWER_OF_REFINEMENT);
		}
		else
		{
			this->SetState(KANTURU_STATE_END);
		}
	}
	else
	{
		this->m_BattleOfNightmare.Run();
	}
}

void CKanturu::ProcState_TOWER_OF_REFINEMENT()
{
	if ( this->m_TowerOfRefinement.GetTowerOfRefinementState() == 5 )
	{
		this->m_TowerOfRefinement.SetState(0);
		this->SetState(KANTURU_STATE_END);
	}
	else
	{
		this->m_TowerOfRefinement.Run();
	}
}

void CKanturu::ProcState_END()
{
	this->SetState(KANTURU_STATE_NONE);
}

int CKanturu::GetKanturuDetailState()
{
	int iCurrentDetailState = KANTURU_STATE_NONE;

	switch ( this->GetKanturuState() )
	{
		case KANTURU_STATE_BATTLE_STANTBY:
			iCurrentDetailState = this->m_BattleStanby.GetBattleStanbyState();
			break;
		case KANTURU_STATE_BATTLE_OF_MAYA:
			iCurrentDetailState = this->m_BattleOfMaya.GetBattleOfMayaState();
			break;
		case KANTURU_STATE_BATTLE_OF_NIGHTMARE:
			iCurrentDetailState = this->m_BattleOfNightmare.GetBattleOfNightmareState();
			break;
		case KANTURU_STATE_TOWER_OF_REFINEMENT:
			iCurrentDetailState = this->m_TowerOfRefinement.GetTowerOfRefinementState();
			break;
	}

	return iCurrentDetailState;
}

void CKanturu::CheckStateTime()
{
	if ( this->GetKanturuState() )
	{
		int iState = this->GetKanturuState();

		if ( this->m_StateInfo[iState].GetCondition() == 1 )
		{
			if ( this->m_StateInfo[iState].IsTimeOut() == TRUE )
			{
				this->SetNextState(iState);
			}
		}
	}
}


int CKanturu::GetRemainTime()
{
	int iRemainTime = 0;

	switch ( this->GetKanturuState() )
	{
		case KANTURU_STATE_BATTLE_STANTBY:
			iRemainTime = this->m_BattleStanby.GetRemainTime();
			break;
		case KANTURU_STATE_BATTLE_OF_MAYA:
			iRemainTime = this->m_BattleOfMaya.GetRemainTime();
			break;
		case KANTURU_STATE_BATTLE_OF_NIGHTMARE:
			iRemainTime = this->m_BattleOfNightmare.GetRemainTime();
			break;
		case KANTURU_STATE_TOWER_OF_REFINEMENT:
			iRemainTime = this->m_TowerOfRefinement.GetRemainTime();
			break;
	}

	return iRemainTime;
}


void CKanturu::KanturuMonsterDieProc(int iMonIndex, int iKillerIndex)
{
	g_KanturuMonsterMng.MonsterDie(iMonIndex);	
}

void CKanturu::CheckUserOnKanturuBossMap()
{
	for (int iCount = g_ConfigRead.server.GetObjectStartUserIndex(); iCount < g_ConfigRead.server.GetObjectMax(); iCount++)
	{
		if (gObj[iCount].Connected == PLAYER_PLAYING &&
			gObj[iCount].Type == OBJ_USER &&
			gObj[iCount].MapNumber == MAP_INDEX_KANTURU_BOSS)
		{
			if (g_ConfigRead.server.GetServerType() != SERVER_CASTLE && gObj[iCount].m_PlayerData->m_bKanturuEntranceByNPC == FALSE)
			{
				if ((gObj[iCount].Authority & 2) != 2 && (gObj[iCount].Authority & 0x20) != 0x20)
				{
					if (gObj[iCount].RegenOk == 0 &&
						gObj[iCount].m_State == 2 &&
						gObj[iCount].Live == TRUE)
					{
						gObjMoveGate(iCount, 17);

						g_Log.AddC(TColor::Red, "[Kanturu] Error: Invalid Kanturu Boss Map User[%s][%s]",
							gObj[iCount].AccountID, gObj[iCount].Name);
					}
				}
			}
		}
	}
}

int CKanturu::CheckEnterKanturu(int iUserIndex)
{
	if ( !gObjIsConnected(iUserIndex) )
		return -1;

	if ( gObj[iUserIndex].MapNumber != MAP_INDEX_KANTURU2 ) 
	{
		g_Log.Add("[Kanturu][Core Gate] Fail: Invalid Map Number(%d) [%s][%s] State(%d)",
			gObj[iUserIndex].MapNumber, gObj[iUserIndex].AccountID, gObj[iUserIndex].Name,
			this->GetKanturuState());

		return 4;
	}

	if (this->GetKanturuState() == KANTURU_STATE_BATTLE_OF_MAYA &&
		this->m_BattleOfMaya.GetEntrancePermit() == TRUE)
	{
		if (g_KanturuBattleUserMng.IsOverMaxUser() == TRUE)
		{
			g_Log.Add("[Kanturu][Core Gate] Fail: Over Max User [%s][%s] State(%d)-(%d)",
				gObj[iUserIndex].AccountID, gObj[iUserIndex].Name,
				this->GetKanturuState(), this->m_BattleOfMaya.GetBattleOfMayaState());

			return 1;
		}

		if (this->CheckEqipmentMoonStone(iUserIndex) == FALSE)
		{
			g_Log.Add("[Kanturu][Core Gate] Fail: Moon Stone is not exist [%s][%s] State(%d)-(%d)",
				gObj[iUserIndex].AccountID, gObj[iUserIndex].Name,
				this->GetKanturuState(), this->m_BattleOfMaya.GetBattleOfMayaState());

			return 2;
		}

		if (g_ConfigRead.server.GetServerType() != SERVER_CASTLE)
		{
			if ((gObj[iUserIndex].pInventory[7].m_Type < ITEMGET(12, 0) || gObj[iUserIndex].pInventory[7].m_Type > ITEMGET(12, 6) && gObj[iUserIndex].pInventory[7].m_Type < ITEMGET(12, 36)) &&
				gObj[iUserIndex].pInventory[7].m_Type != ITEMGET(13, 30) &&
				gObj[iUserIndex].pInventory[gObj[iUserIndex].m_btInvenPetPos].m_Type != ITEMGET(13, 3) &&
				gObj[iUserIndex].pInventory[gObj[iUserIndex].m_btInvenPetPos].m_Type != ITEMGET(13, 37) &&
				gObj[iUserIndex].pInventory[7].m_Type != ITEMGET(12, 49) &&
				gObj[iUserIndex].pInventory[7].m_Type != ITEMGET(12, 50) &&
				!(gObj[iUserIndex].pInventory[7].m_Type >= ITEMGET(12, 36) && gObj[iUserIndex].pInventory[7].m_Type <= ITEMGET(12, 43)) &&
				!(gObj[iUserIndex].pInventory[7].m_Type >= ITEMGET(12, 262) && gObj[iUserIndex].pInventory[7].m_Type <= ITEMGET(12, 265)) &&
				gObj[iUserIndex].pInventory[7].m_Type != ITEMGET(12, 266) &&
				gObj[iUserIndex].pInventory[7].m_Type != ITEMGET(12, 267) &&
				gObj[iUserIndex].pInventory[7].m_Type != ITEMGET(12, 268))
			{
				g_Log.Add("[Kanturu][Core Gate] Fail: Wing Item is not exist [%s][%s] State(%d)-(%d)",
					gObj[iUserIndex].AccountID, gObj[iUserIndex].Name,
					this->GetKanturuState(), this->m_BattleOfMaya.GetBattleOfMayaState());

				return 7;
			}

			if (gObj[iUserIndex].pInventory[gObj[iUserIndex].m_btInvenPetPos].m_Type == ITEMGET(13, 2))
			{
				g_Log.Add("[Kanturu][Core Gate] Fail: Uniria Item is exist [%s][%s] State(%d)-(%d)",
					gObj[iUserIndex].AccountID, gObj[iUserIndex].Name,
					this->GetKanturuState(), this->m_BattleOfMaya.GetBattleOfMayaState());

				return 5;
			}

			if (gObj[iUserIndex].pInventory[11].m_Type == ITEMGET(13, 10) ||
				gObj[iUserIndex].pInventory[10].m_Type == ITEMGET(13, 10))
			{
				g_Log.Add("[Kanturu][Core Gate] Fail: Trasportation Ring is exist [%s][%s] State(%d)-(%d)",
					gObj[iUserIndex].AccountID, gObj[iUserIndex].Name,
					this->GetKanturuState(), this->m_BattleOfMaya.GetBattleOfMayaState());

				return 6;
			}
		}

		if ( gObj[iUserIndex].pInventory[11].m_Type == ITEMGET(13,39) ||
			gObj[iUserIndex].pInventory[10].m_Type == ITEMGET(13,39) )
		{
			g_Log.Add("[Kanturu][Core Gate] Fail: EliteSkelletoneChangeRing is exist [%s][%s] State(%d)-(%d)",
				gObj[iUserIndex].AccountID, gObj[iUserIndex].Name,
				this->GetKanturuState(), this->m_BattleOfMaya.GetBattleOfMayaState());

			return 6;
		}

		if ( gObj[iUserIndex].pInventory[11].m_Type == ITEMGET(13,40) ||
			gObj[iUserIndex].pInventory[10].m_Type == ITEMGET(13,40) )
		{
			g_Log.Add("[Kanturu][Core Gate] Fail: Jack'O Lantern Polymorph Ring is exist [%s][%s] State(%d)-(%d)",
				gObj[iUserIndex].AccountID, gObj[iUserIndex].Name,
				this->GetKanturuState(), this->m_BattleOfMaya.GetBattleOfMayaState());

			return 6;
		}

		if ( gObj[iUserIndex].pInventory[11].m_Type == ITEMGET(13,41) ||
			gObj[iUserIndex].pInventory[10].m_Type == ITEMGET(13,41) )
		{
			g_Log.Add("[Kanturu][Core Gate] Fail: Santa Polymorph Ring is exist [%s][%s] State(%d)-(%d)",
				gObj[iUserIndex].AccountID, gObj[iUserIndex].Name,
				this->GetKanturuState(), this->m_BattleOfMaya.GetBattleOfMayaState());

			return 6;
		}

		g_Log.Add("[Kanturu][Core Gate] Entered to Kanturu Tower: [%s][%s] State(%d)-(%d)",
			gObj[iUserIndex].AccountID, gObj[iUserIndex].Name,
			this->GetKanturuState(), this->m_BattleOfMaya.GetBattleOfMayaState());

		return 0;
	}

	if ( this->GetKanturuState() == KANTURU_STATE_TOWER_OF_REFINEMENT &&
		 this->m_TowerOfRefinement.GetEntrancePermit() == TRUE )
	{
		g_Log.Add("[Kanturu][Core Gate] Entered to Kanturu Tower: [%s][%s] State(%d)-(%d)",
			gObj[iUserIndex].AccountID, gObj[iUserIndex].Name,
			this->GetKanturuState(), this->m_TowerOfRefinement.GetTowerOfRefinementState());

		return 0;
	}

	g_Log.Add("[Kanturu][Core Gate] Fail to Enter: [%s][%s] State(%d)",
		gObj[iUserIndex].AccountID, gObj[iUserIndex].Name,
		this->GetKanturuState());

	return 3;
}


BOOL CKanturu::CheckCanEnterKanturuBattle()
{
	if ( this->GetKanturuState() == KANTURU_STATE_BATTLE_OF_MAYA &&
		 this->m_BattleOfMaya.GetEntrancePermit() == TRUE )
	{
		return TRUE;
	}

	if ( this->GetKanturuState() == KANTURU_STATE_TOWER_OF_REFINEMENT &&
		 this->m_TowerOfRefinement.GetEntrancePermit() == TRUE &&
		 this->m_TowerOfRefinement.IsUseTowerOfRefinement() == TRUE)
	{
		return TRUE;
	}

	return FALSE;
}

BOOL CKanturu::CheckEqipmentMoonStone(int iUserIndex)
{
	if ( !this->GetEnableCheckMoonStone() )
		return TRUE;

	if ( !gObjIsConnected(iUserIndex) )
		return FALSE;

	if ( gObj[iUserIndex].pInventory[10].IsItem() ||
		 gObj[iUserIndex].pInventory[11].IsItem() )	
	{
		if ( gObj[iUserIndex].pInventory[10].m_Type == ITEMGET(13,38) &&
			 gObj[iUserIndex].pInventory[10].m_Durability != 0.0f )
		{
			return TRUE;
		}

		if ( gObj[iUserIndex].pInventory[11].m_Type == ITEMGET(13,38) &&
			 gObj[iUserIndex].pInventory[11].m_Durability != 0.0f )
		{
			return TRUE;
		}
	}

	return FALSE;
}

void CKanturu::OperateGmCommand(int iUserIndex, int iCommand)
{
	if(!gObjIsConnected(iUserIndex))
	{
		return;
	}

	if( (gObj[iUserIndex].Authority & 0x02) != 0x02 && (gObj[iUserIndex].Authority & 0x20) != 0x20 )
	{
		return;
	}

	if( (gObj[iUserIndex].GameMaster & GM_EVENT_MODIFY) != GM_EVENT_MODIFY )
	{
		MsgOutput(iUserIndex, Lang.GetText(0,882));
		return;
	}

	switch ( iCommand )
	{
		case 0:
			this->SetState(KANTURU_STATE_BATTLE_STANTBY);
			break;
		case 1:
			this->SetState(KANTURU_STATE_BATTLE_OF_MAYA);
			break;
		case 2:
			this->SetState(KANTURU_STATE_TOWER_OF_REFINEMENT);
			break;
	}

	g_Log.AddC(TColor::Red,  "[ADMIN COMMAND] [%s][%s][%s] Used KANTURU Command ID: %d",
		gObj[iUserIndex].AccountID, gObj[iUserIndex].Name,
		gObj[iUserIndex].m_PlayerData->Ip_addr, iCommand);
}

void CKanturu::SetKanturuTimeAttackEventInfo()
{
	tm * today;
	time_t ltime;

	time(&ltime);
	today = localtime(&ltime);
	today->tm_year += 1900;

	int iYear = today->tm_year * 10000;
	int iMonth = ( today->tm_mon + 1 ) * 100;
	int iDay = today->tm_mday;
	int iDateInfo = iYear + iMonth + iDay;

	if ( this->m_iKanturuBattleDate < iDateInfo )
	{
		this->m_iKanturuBattleDate = iDateInfo;
		this->m_iKanturuBattleCounter = 0;
	}

	this->m_iKanturuBattleCounter++;

	g_Log.Add("[Kanturu][Time Attack] Date:%d, Counter:%d",
		this->m_iKanturuBattleDate, this->m_iKanturuBattleCounter);
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


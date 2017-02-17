//////////////////////////////////////////////////////////////////////
// BloodCastle.cpp
// ------------------------------
// Decompiled by Deathway
// Date : 2007-05-09
// ------------------------------
// GS-N 0.99.60T 0x005004D0 - Completed
/* Disorder with same effect in CBloodCastle::LevelUp
		GCLevelUpMsgSend(gObj[iIndex].Index, gObj[iIndex].Level, gObj[iIndex].LevelUpPoint, 
			(int)((float)gObj[iIndex].unk104 + gObj[iIndex].fMaxLife), (int)((float)gObj[iIndex].unk108 + gObj[iIndex].fMaxMana),
			gObj[iIndex].iMaxBP + gObj[iIndex].unkE8, AddPoint, MaxAddPoint);
   Problem with global variables, there is a Zero ( 4 BYTES ) space
*/
// GS-N	1.00.18	0x005303F0	-	Completed

#include "stdafx.h"
#include "BloodCastle.h"
#include "ChaosCastle.h"
#include "GameMain.h"
#include "TLog.h"
#include "DSProtocol.h"
#include "Protocol.h"
#include "TNotice.h"
#include "winutil.h"
#include "ObjUseSkill.h"
#include "ChaosBox.h"
#include "CastleSiegeSync.h"
#include "CrywolfSync.h"
#include "CashShop.h"
#include "configread.h"
#include "BuffEffectSlot.h"
#include "MasterLevelSkillTreeSystem.h"
#include "QuestExpProgMng.h"
#include "BagManager.h"
#include "ItemSocketOptionSystem.h"
#include "gObjMonster.h"

CBloodCastle g_BloodCastle;

static const struct BLOOD_ZONE
{
	BYTE btStartX;
	BYTE btStartY;
	BYTE btEndX;
	BYTE btEndY;

} g_btCastleEntranceMapXY[MAX_BLOOD_CASTLE_LEVEL] = {

	13, 15, 15, 23,	// Blood Castle 1
	13, 15, 15, 23,	// Blood Castle 2
	13, 15, 15, 23,	// Blood Castle 3
	13, 15, 15, 23,	// Blood Castle 4
	13, 15, 15, 23,	// Blood Castle 5
	13, 15, 15, 23,	// Blood Castle 6
	13, 15, 15, 23,	// Blood Castle 7
	13, 15, 15, 23	// Blood Castle 8

}, g_btCastleBridgeMapXY[MAX_BLOOD_CASTLE_LEVEL] = {

	13, 70, 15, 75,	// Bridge of Blood Castle 1
	13, 70, 15, 75,	// Bridge of Blood Castle 2
	13, 70, 15, 75,	// Bridge of Blood Castle 3
	13, 70, 15, 75,	// Bridge of Blood Castle 4
	13, 70, 15, 75,	// Bridge of Blood Castle 5
	13, 70, 15, 75,	// Bridge of Blood Castle 6
	13, 70, 15, 75,	// Bridge of Blood Castle 7
	13, 70, 15, 75	// Bridge of Blood Castle 8

}, g_btCastleDoorMapXY[MAX_BLOOD_CASTLE_LEVEL][3]= {

	// Blood Castle 1
	13, 76, 15, 79,	// Door Itself
	11, 80, 25, 89,	// Zone Beginh Door
	 8, 80, 10, 83,	// Altar 

	// Blood Castle 2
	13, 76, 15, 79,	// Door Itself
	11, 80, 25, 89,	// Zone Beginh Door
	 8, 80, 10, 83,	// Altar 

	// Blood Castle 3
	13, 76, 15, 79,	// Door Itself
	11, 80, 25, 89,	// Zone Beginh Door
	 8, 80, 10, 83,	// Altar 

	// Blood Castle 4
	13, 76, 15, 79,	// Door Itself
	11, 80, 25, 89,	// Zone Beginh Door
	 8, 80, 10, 83,	// Altar 

	// Blood Castle 5
	13, 76, 15, 79,	// Door Itself
	11, 80, 25, 89,	// Zone Beginh Door
	 8, 80, 10, 83,	// Altar 

	// Blood Castle 6
	13, 76, 15, 79,	// Door Itself
	11, 80, 25, 89,	// Zone Beginh Door
	 8, 80, 10, 83,	// Altar 

	// Blood Castle 7
	13, 76, 15, 79,	// Door Itself
	11, 80, 25, 89,	// Zone Beginh Door
	 8, 80, 10, 83,	// Altar 

 	// Blood Castle 8
	13, 76, 15, 79,	// Door Itself
	11, 80, 25, 89,	// Zone Beginh Door
	 8, 80, 10, 83	// Altar 
};

static const struct ST_REWARD_ZEN
{
	int NormalCharacter;
	int SpecialCharacter;

} g_iQuestWinExpendZEN[MAX_BLOOD_CASTLE_LEVEL] = {

	20000,	10000,
	50000,	25000,
	100000,	50000,
	150000,	80000,
	200000,	100000,
	250000,	120000,
	300000,	140000,
	350000, 160000

};



/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/

CBloodCastle::CBloodCastle()
{
	this->m_bBC_EVENT_ENABLE = false;
	this->m_iBC_TIME_MIN_OPEN = 10;
	this->m_iBC_MONSTER_REGEN = 0;
	this->m_bBC_RESTRICT_FINISH_ENABLE = false;
	this->m_iBC_RESTRICT_FINISH_TIME = 0;

	this->m_iArchangelScrollDropRate = 0;
	this->m_iBloodBoneDropRate = 0;

	for ( int i =0; i<MAX_BLOOD_CASTLE_LEVEL ; i++ )
	{
		this->m_BridgeData[i].m_iBC_STATE = BC_STATE_NONE;
		this->m_BridgeData[i].m_iMapNumber = this->GetBridgeMapNumber(i);
		this->m_BridgeData[i].m_iBridgeIndex = i;
		this->m_BridgeData[i].m_i64_BC_REMAIN_MSEC = -1;
		this->m_BridgeData[i].m_i64_BC_TICK_COUNT = -1;
		this->m_BridgeData[i].m_iBC_REWARD_EXP = 1.0;
		this->m_BridgeData[i].m_iBC_CASTLE_BLOCK_INFO = 1000;
		this->m_iBC_BOSS_SCRIPT_CNT[i] = 0;
		this->m_iBC_GENERAL_SCRIPT_CNT[i] = 0;
		InitializeCriticalSection(&this->m_BridgeData[i].m_critUserData);

		for ( int j =0; j<MAX_BLOOD_CASTLE_SUB_BRIDGE ; j++ )
		{
			this->m_BridgeData[i].m_UserData[j].m_iIndex = -1;
		}

		this->ClearBridgeData(i);
	}
}



CBloodCastle::~CBloodCastle()
{
	for ( int i =0; i<MAX_BLOOD_CASTLE_LEVEL ; i++ )
	{
		DeleteCriticalSection(&this->m_BridgeData[i].m_critUserData);
	}
}



void CBloodCastle::Init()
{
	if (g_ConfigRead.server.GetStateFromEventTable(g_ConfigRead.server.GetServerType(), EV_TABLE_BC) == false)
	{
		this->SetEventEnable(false);
	}

	for (int i=0;i<MAX_BLOOD_CASTLE_LEVEL;i++)
	{
		if (this->IsEventEnable() == false)
		{
			this->SetState(i, BC_STATE_NONE);
		}

		else
		{
			this->SetState(i, BC_STATE_CLOSED);
		}
	}
}

void CBloodCastle::Load(char* filename)
{
	this->m_listBloodCastleOpenTime.clear();

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(filename);

	if (res.status != pugi::status_ok)
	{
		g_Log.MsgBox("[Blood Castle] Info file Load Fail [%s] [%s]", filename, res.description());
		return;
	}

	pugi::xml_node main = file.child("BloodCastle");

	bool bEnable = main.attribute("Enable").as_bool();

	if (g_ConfigRead.server.GetStateFromEventTable(g_ConfigRead.server.GetServerType(), EV_TABLE_BC) == false)
	{
		bEnable = false;
	}

	this->SetEventEnable(bEnable);

	this->m_bBC_RESTRICT_FINISH_ENABLE = main.attribute("EarlyFinishRestriction").as_bool();
	this->m_iBC_RESTRICT_FINISH_TIME = main.attribute("EarlyFinishMinTime").as_int();
	
	this->m_iArchangelScrollDropRate = main.attribute("ScrollofArchangelDropRate").as_int();
	this->m_iBloodBoneDropRate = main.attribute("BloodBoneDropRate").as_int();

	pugi::xml_node time = main.child("Time");

	this->m_iBC_TIME_MIN_OPEN = time.attribute("ToOpen").as_int();
	this->m_iBC_TIME_MIN_PLAY = time.attribute("PlayDuration").as_int();
	this->m_iBC_TIME_MIN_REST = time.attribute("ToClose").as_int();

	pugi::xml_node monster = main.child("Monster");

	this->m_iBC_MONSTER_REGEN = monster.attribute("TimeToRegen").as_int();

	pugi::xml_node schedule = main.child("Schedule");

	for (pugi::xml_node start = schedule.child("Start"); start; start = start.next_sibling())
	{
		BLOODCASTLE_START_TIME Schedule;

		Schedule.m_iHour = start.attribute("Hour").as_int();
		Schedule.m_iMinute = start.attribute("Minute").as_int();

		this->m_listBloodCastleOpenTime.push_back(Schedule);
	}

	pugi::xml_node event_settings = main.child("EventSettings");

	for (pugi::xml_node event_ = event_settings.child("Castle"); event_; event_ = event_.next_sibling())
	{		
		int iBridgeNum = event_.attribute("Level").as_int();

		if ( BC_BRIDGE_RANGE(iBridgeNum) != FALSE )
		{
			this->m_BridgeData[iBridgeNum].m_iCastleStatueHealth = event_.attribute("StatueHP").as_int();
			this->m_BridgeData[iBridgeNum].m_iCastleDoorHealth = event_.attribute("GateHP").as_int();
		}

		else
		{
			g_Log.MsgBox("Error - wrong Event Level (%d)", iBridgeNum);
			continue;
		}
	}

	pugi::xml_node reward_exp = main.child("RewardExpSettings");

	for (pugi::xml_node castle = reward_exp.child("Castle"); castle; castle = castle.next_sibling())
	{
		int iBridgeNum = castle.attribute("Level").as_int();

		if ( BC_BRIDGE_RANGE(iBridgeNum) != FALSE )
		{
			this->m_BridgeData[iBridgeNum].m_iBC_REWARD_EXP = castle.attribute("Multiplier").as_float();
		}
				
		else
		{
			g_Log.MsgBox("Error - wrong Event Level (%d)", iBridgeNum);
			continue;
		}
	}

	// private custom
	pugi::xml_node entry_level = main.child("EntryLevel");

	for (pugi::xml_node castle = entry_level.child("Castle"); castle; castle = castle.next_sibling())
	{
		int iBridgeNum = castle.attribute("Level").as_int();

		if (BC_BRIDGE_RANGE(iBridgeNum) != FALSE)
		{
			g_sttBLOODCASTLE_LEVEL[iBridgeNum].iLOWER_BOUND = castle.attribute("NormalMinLevel").as_int();
			g_sttBLOODCASTLE_LEVEL[iBridgeNum].iUPPER_BOUND = castle.attribute("NormalMaxLevel").as_int();
			g_sttBLOODCASTLE_LEVEL[iBridgeNum].iLOWER_BOUND_MAGUMSA = castle.attribute("SpecialMinLevel").as_int();
			g_sttBLOODCASTLE_LEVEL[iBridgeNum].iUPPER_BOUND_MAGUMSA = castle.attribute("SpecialMaxLevel").as_int();
		}
	}

	g_Log.Add("%s file load!", filename);
}

void CBloodCastle::LoadMonster(LPSTR filename)
{
	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(filename);

	if (res.status != pugi::status_ok)
	{
		g_Log.MsgBox("%s file load fail (%s)", filename, res.description());
		return;
	}

	pugi::xml_node main = file.child("BloodCastle");

	for (pugi::xml_node castle = main.child("Castle"); castle; castle = castle.next_sibling())
	{
		int iBridgeIndex = castle.attribute("Level").as_int();

		if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
		{
			g_Log.MsgBox("Error - wrong Castle Level (%d)", iBridgeIndex);
			continue;
		}

		this->m_BCMP_AngelKing[iBridgeIndex].Init();
		this->m_BCMP_CastleGate[iBridgeIndex].Init();
		this->m_BCMP_SaintStatue[iBridgeIndex].Init();

		for (int i = 0; i < 20; i++)
		{
			this->m_BCMP_BossMonster[iBridgeIndex][i].Init();
		}

		for (int i = 0; i < 100; i++)
		{
			this->m_BCMP_General[iBridgeIndex][i].Init();
		}

		pugi::xml_node archangel = castle.child("Archangel");

		this->m_BCMP_AngelKing[iBridgeIndex].m_Type			= archangel.attribute("Index").as_int();
		this->m_BCMP_AngelKing[iBridgeIndex].m_MapNumber	= archangel.attribute("MapNumber").as_int();
		this->m_BCMP_AngelKing[iBridgeIndex].m_Dis			= archangel.attribute("Distance").as_int();
		this->m_BCMP_AngelKing[iBridgeIndex].m_X			= archangel.attribute("StartX").as_int();
		this->m_BCMP_AngelKing[iBridgeIndex].m_Y			= archangel.attribute("StartY").as_int();
		this->m_BCMP_AngelKing[iBridgeIndex].m_Dir			= archangel.attribute("Dir").as_int();

		pugi::xml_node gate = castle.child("Gate");

		this->m_BCMP_CastleGate[iBridgeIndex].m_Type		= gate.attribute("Index").as_int();
		this->m_BCMP_CastleGate[iBridgeIndex].m_MapNumber	= gate.attribute("MapNumber").as_int();
		this->m_BCMP_CastleGate[iBridgeIndex].m_Dis			= gate.attribute("Distance").as_int();
		this->m_BCMP_CastleGate[iBridgeIndex].m_X			= gate.attribute("StartX").as_int();
		this->m_BCMP_CastleGate[iBridgeIndex].m_Y			= gate.attribute("StartY").as_int();
		this->m_BCMP_CastleGate[iBridgeIndex].m_Dir			= gate.attribute("Dir").as_int();

		pugi::xml_node statue = castle.child("Statue");

		this->m_BCMP_SaintStatue[iBridgeIndex].m_Type		= statue.attribute("Index").as_int();
		this->m_BCMP_SaintStatue[iBridgeIndex].m_MapNumber	= statue.attribute("MapNumber").as_int();
		this->m_BCMP_SaintStatue[iBridgeIndex].m_Dis		= statue.attribute("Distance").as_int();
		this->m_BCMP_SaintStatue[iBridgeIndex].m_X			= statue.attribute("StartX").as_int();
		this->m_BCMP_SaintStatue[iBridgeIndex].m_Y			= statue.attribute("StartY").as_int();
		this->m_BCMP_SaintStatue[iBridgeIndex].m_Dir		= statue.attribute("Dir").as_int();

		pugi::xml_node bosses = castle.child("Bosses");
		int iCount = 0;

		for (pugi::xml_node monster = bosses.child("Monster"); monster; monster = monster.next_sibling())
		{
			this->m_BCMP_BossMonster[iBridgeIndex][iCount].m_Type		= monster.attribute("Index").as_int();
			this->m_BCMP_BossMonster[iBridgeIndex][iCount].m_MapNumber	= monster.attribute("MapNumber").as_int();
			this->m_BCMP_BossMonster[iBridgeIndex][iCount].m_Dis		= monster.attribute("Distance").as_int();
			this->m_BCMP_BossMonster[iBridgeIndex][iCount].m_X			= monster.attribute("StartX").as_int();
			this->m_BCMP_BossMonster[iBridgeIndex][iCount].m_Y			= monster.attribute("StartY").as_int();
			this->m_BCMP_BossMonster[iBridgeIndex][iCount].m_Dir		= monster.attribute("Dir").as_int();

			if (BC_MAP_RANGE(this->m_BCMP_BossMonster[iBridgeIndex][iCount].m_MapNumber) == FALSE)
			{
				g_Log.MsgBox("[BloodCastle][LoadMonster] Boss Monster Map Number Invalid (%d)", this->m_BCMP_BossMonster[iBridgeIndex][iCount].m_MapNumber);
				continue;
			}

			iCount++;
		}

		if (iCount == 20 || iCount < 0)
		{
			g_Log.Add("[BloodCastle][LoadMonster] Boss Monster Count Invalid");
			iCount = 20;
		}

		this->m_iBC_BOSS_SCRIPT_CNT[iBridgeIndex] = iCount;

		pugi::xml_node general = castle.child("General");
		iCount = 0;

		for (pugi::xml_node monster = general.child("Monster"); monster; monster = monster.next_sibling())
		{
			this->m_BCMP_General[iBridgeIndex][iCount].m_Type		= monster.attribute("Index").as_int();
			this->m_BCMP_General[iBridgeIndex][iCount].m_MapNumber	= monster.attribute("MapNumber").as_int();
			this->m_BCMP_General[iBridgeIndex][iCount].m_Dis		= monster.attribute("Distance").as_int();
			this->m_BCMP_General[iBridgeIndex][iCount].m_X			= monster.attribute("StartX").as_int();
			this->m_BCMP_General[iBridgeIndex][iCount].m_Y			= monster.attribute("StartY").as_int();
			this->m_BCMP_General[iBridgeIndex][iCount].m_Dir		= monster.attribute("Dir").as_int();

			if (BC_MAP_RANGE(this->m_BCMP_General[iBridgeIndex][iCount].m_MapNumber) == FALSE)
			{
				g_Log.MsgBox("[BloodCastle][LoadMonster] General Monster Map Number Invalid (%d)", this->m_BCMP_General[iBridgeIndex][iCount].m_MapNumber);
				continue;
			}

			iCount++;
		}

		if (iCount == 100 || iCount < 0)
		{
			g_Log.Add("[BloodCastle][LoadMonster] General Monster Count Invalid");
			iCount = 100;
		}

		this->m_iBC_GENERAL_SCRIPT_CNT[iBridgeIndex] = iCount;
	}
}

void CBloodCastle::CheckSync(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	std::list<BLOODCASTLE_START_TIME>::iterator it;
	BLOODCASTLE_START_TIME WebzenVar1;
	BLOODCASTLE_START_TIME WebzenVar2;
	int BaseTime = 0; //7
	int CheckTime = 0; //8
	DWORD CurrentTime = 0; //9
	
	tm * today; //10
	time_t ltime; //11

	int loc12;

	this->m_BridgeData[iBridgeIndex].m_i64_BC_TICK_COUNT = GetTickCount64();

	if(this->m_listBloodCastleOpenTime.size() == 0)
	{
		g_Log.MsgBox("Error : Blood Castle StartTime size is 0");
		return;
	}

	time(&ltime);
	today = localtime(&ltime);

	
	CurrentTime = (today->tm_hour * 60) + today->tm_min;
	WebzenVar1 = *this->m_listBloodCastleOpenTime.begin();

	for( it = this->m_listBloodCastleOpenTime.begin(); it != this->m_listBloodCastleOpenTime.end(); ++it )
	{
		WebzenVar2 = *it;
		BaseTime = (WebzenVar1.m_iHour * 60) + WebzenVar1.m_iMinute;
		CheckTime =	(WebzenVar2.m_iHour * 60) + WebzenVar2.m_iMinute;

		if( BaseTime == CheckTime )
		{
			if( CurrentTime < CheckTime )
			{
				WebzenVar2 = *it;
				break;
			}
			continue;
		}
		
		if( CurrentTime >= BaseTime && CurrentTime < CheckTime )
		{
			break;
		}
		else
		{
			WebzenVar1 = *it;
		}
	}

	for(loc12 = 2;loc12--;)
	{
		if(it == this->m_listBloodCastleOpenTime.end())
		{
			it = this->m_listBloodCastleOpenTime.begin();

			WebzenVar2 = (*it);
		}

		CheckTime = WebzenVar2.m_iHour*60+WebzenVar2.m_iMinute;

		if(today->tm_hour <= WebzenVar2.m_iHour && CheckTime > CurrentTime)
		{
			this->m_BridgeData[iBridgeIndex].m_i64_BC_REMAIN_MSEC = ((CheckTime - CurrentTime)*60)*1000;
		}
		else
		{
			this->m_BridgeData[iBridgeIndex].m_i64_BC_REMAIN_MSEC = ((1440-CurrentTime+CheckTime)*60)*1000;
		}

		if ( this->m_BridgeData[iBridgeIndex].m_i64_BC_REMAIN_MSEC <= ( this->m_iBC_TIME_MIN_OPEN * 60 * 1000 ))
		{
			it++;

			if(it != this->m_listBloodCastleOpenTime.end())
			{
				WebzenVar2 = (*it);
			}
		}
		else
		{
			break;
		}
	}

	this->m_BridgeData[iBridgeIndex].m_i64_BC_REMAIN_MSEC -= today->tm_sec * 1000;

	g_Log.Add("[Blood Castle] (%d) Sync Open Time. [%I64d] min remain", iBridgeIndex+1, this->m_BridgeData[iBridgeIndex].m_i64_BC_REMAIN_MSEC/60000);
}




void CBloodCastle::ClearBridgeData(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	this->m_BridgeData[iBridgeIndex].m_iTOTAL_EXP = 0;
	this->m_BridgeData[iBridgeIndex].m_iBC_MONSTER_MAX_COUNT = 0;
	this->m_BridgeData[iBridgeIndex].m_iBC_MONSTER_KILL_COUNT = 0;
	this->m_BridgeData[iBridgeIndex].m_iBC_MONSTER_SUCCESS_MSG_COUNT = 0;
	this->m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_MAX_COUNT = 0;
	this->m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_KILL_COUNT = 0;
	this->m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_SUCCESS_MSG_COUNT = 0;
	this->m_BridgeData[iBridgeIndex].m_btBC_QUEST_ITEM_NUMBER = 0;
	this->m_BridgeData[iBridgeIndex].m_i64_BC_TICK_DOOR_OPEN = -1;
	this->m_BridgeData[iBridgeIndex].m_nBC_QUESTITEM_SERIAL = -1;
	this->m_BridgeData[iBridgeIndex].m_iBC_QUEST_ITEM_USER_INDEX = -1;
	this->m_BridgeData[iBridgeIndex].m_iMISSION_SUCCESS = -1;
	this->m_BridgeData[iBridgeIndex].m_iBC_NOTIFY_COUNT = -1;
	this->m_BridgeData[iBridgeIndex].m_bCASTLE_DOOR_LIVE = TRUE;
	this->m_BridgeData[iBridgeIndex].m_bBC_REWARDED = false;
	this->m_BridgeData[iBridgeIndex].m_bBC_MSG_BEFORE_ENTER = false;
	this->m_BridgeData[iBridgeIndex].m_bBC_MSG_BEFORE_PLAY = false;
	this->m_BridgeData[iBridgeIndex].m_bBC_MSG_BEFORE_END = false;
	this->m_BridgeData[iBridgeIndex].m_bBC_MSG_BEFORE_QUIT = false;
	this->m_BridgeData[iBridgeIndex].m_bBC_CAN_ENTER = false;
	this->m_BridgeData[iBridgeIndex].m_bBC_CAN_PARTY = false;
	this->m_BridgeData[iBridgeIndex].m_bBC_PLAY_START = false;
	this->m_BridgeData[iBridgeIndex].m_bBC_MONSTER_KILL_COMPLETE = false;
	this->m_BridgeData[iBridgeIndex].m_bBC_BOSS_MONSTER_KILL_COMPLETE = false;
	this->m_BridgeData[iBridgeIndex].m_bBC_DOOR_TERMINATE_COMPLETE = false;
	this->m_BridgeData[iBridgeIndex].m_iExtraEXP_Kill_Door_Index = -10;
	this->m_BridgeData[iBridgeIndex].m_iExtraEXP_Kill_Door_Party = -10;
	this->m_BridgeData[iBridgeIndex].m_iExtraEXP_Kill_Statue_Index = -10;
	this->m_BridgeData[iBridgeIndex].m_iExtraEXP_Kill_Statue_Party = -10;
	this->m_BridgeData[iBridgeIndex].m_iExtraEXP_Win_Quest_Index = -10;
	this->m_BridgeData[iBridgeIndex].m_iExtraEXP_Win_Quest_Party = -10;

	memset(this->m_BridgeData[iBridgeIndex].m_szKill_Door_CharName , 0, sizeof(this->m_BridgeData[iBridgeIndex].m_szKill_Door_CharName));
	memset(this->m_BridgeData[iBridgeIndex].m_szKill_Status_CharName, 0, sizeof(this->m_BridgeData[iBridgeIndex].m_szKill_Status_CharName));
	memset(this->m_BridgeData[iBridgeIndex].m_szWin_Quest_CharName, 0, sizeof(this->m_BridgeData[iBridgeIndex].m_szWin_Quest_CharName));
	memset(this->m_BridgeData[iBridgeIndex].m_szKill_Door_AccountID, 0, sizeof(this->m_BridgeData[iBridgeIndex].m_szKill_Door_AccountID));
	memset(this->m_BridgeData[iBridgeIndex].m_szKill_Status_AccountID, 0, sizeof(this->m_BridgeData[iBridgeIndex].m_szKill_Status_AccountID));
	memset(this->m_BridgeData[iBridgeIndex].m_szWin_Quest_AccountID, 0, sizeof(this->m_BridgeData[iBridgeIndex].m_szWin_Quest_AccountID));
	

	for ( int i=0;i<MAX_BLOOD_CASTLE_SUB_BRIDGE;i++)
	{
		if ( this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex != -1 )
		{
			gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_iBloodCastleEXP = 0;
			gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleIndex = -1;
			gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleSubIndex = -1;
			gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_bBloodCastleComplete = false;
		}

		this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iEXP = 0;
		this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iScore = 0;
		this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex = -1;
		this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState= 0;
		this->m_BridgeData[iBridgeIndex].m_UserData[i].m_bSendQuitMsg = false;
		this->m_BridgeData[iBridgeIndex].m_UserData[i].m_bLiveWhenDoorBreak = false;
		this->m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX = -1;
		this->m_BridgeData[iBridgeIndex].m_iBC_DOOR_MONSTER_INDEX = -1; //season 2.5 add-on
		this->m_BridgeData[iBridgeIndex].m_iBC_CASTLE_BLOCK_INFO = 1000;
	}
}



void CBloodCastle::SetState(int iBridgeIndex, int iBC_STATE)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	if ( iBC_STATE < BC_STATE_NONE || iBC_STATE > BC_STATE_PLAYEND )
	{
		return;
	}

	this->m_BridgeData[iBridgeIndex].m_iBC_STATE = iBC_STATE;

	switch ( this->m_BridgeData[iBridgeIndex].m_iBC_STATE )
	{
		case BC_STATE_NONE:
			this->SetState_None(iBridgeIndex);
			break;

		case BC_STATE_CLOSED:
			this->SetState_Closed(iBridgeIndex);
			break;

		case BC_STATE_PLAYING:
			this->SetState_Playing(iBridgeIndex);
			break;

		case BC_STATE_PLAYEND:
			this->SetState_PlayEnd(iBridgeIndex);
			break;
	}
}



void CBloodCastle::Run()
{
	if ( this->IsEventEnable() != false )
	{
		for (int iBridgeIndex=0;iBridgeIndex<MAX_BLOOD_CASTLE_LEVEL;iBridgeIndex++)
		{
			switch ( this->m_BridgeData[iBridgeIndex].m_iBC_STATE )
			{
				case BC_STATE_NONE:
					this->ProcState_None(iBridgeIndex);
					break;

				case BC_STATE_CLOSED:
					this->ProcState_Closed(iBridgeIndex);
					break;

				case BC_STATE_PLAYING:
					this->ProcState_Playing(iBridgeIndex);
					break;

				case BC_STATE_PLAYEND:
					this->ProcState_PlayEnd(iBridgeIndex);
					break;
			}
		}
	}
}





	
void CBloodCastle::ProcState_None(int iBridgeIndex)
{
	return;
}



void CBloodCastle::ProcState_Closed(int iBridgeIndex)
{
	ULONGLONG i64TICK_MSEC = GetTickCount64() - this->m_BridgeData[iBridgeIndex].m_i64_BC_TICK_COUNT;

	if ( i64TICK_MSEC >= 1000 )
	{
		this->m_BridgeData[iBridgeIndex].m_i64_BC_REMAIN_MSEC -= i64TICK_MSEC;
		this->m_BridgeData[iBridgeIndex].m_i64_BC_TICK_COUNT = GetTickCount64();

		if ( this->IsEventEnable() != FALSE )
		{
			if ( this->m_BridgeData[iBridgeIndex].m_i64_BC_REMAIN_MSEC <= ( this->m_iBC_TIME_MIN_OPEN * 60 * 1000 ) && this->m_BridgeData[iBridgeIndex].m_bBC_CAN_ENTER == false)
			{
				this->m_BridgeData[iBridgeIndex].m_bBC_CAN_ENTER = true;
				this->m_BridgeData[iBridgeIndex].m_bBC_CAN_PARTY = true;
			}

			if ( this->m_BridgeData[iBridgeIndex].m_i64_BC_REMAIN_MSEC <= ( this->m_iBC_TIME_MIN_OPEN * 60 * 1000 ) && this->m_BridgeData[iBridgeIndex].m_i64_BC_REMAIN_MSEC > 0 && (this->m_BridgeData[iBridgeIndex].m_i64_BC_REMAIN_MSEC/60000) !=this->m_BridgeData[iBridgeIndex].m_iBC_NOTIFY_COUNT)
			{
				this->m_BridgeData[iBridgeIndex].m_iBC_NOTIFY_COUNT = this->m_BridgeData[iBridgeIndex].m_i64_BC_REMAIN_MSEC / 60000;

				if ( iBridgeIndex == 0 )
				{
					PMSG_NOTICE pNotice;

					TNotice::MakeNoticeMsgEx(&pNotice, 0, Lang.GetText(0,65), this->m_BridgeData[iBridgeIndex].m_iBC_NOTIFY_COUNT+1);
					this->SendAllUserAnyMsg((BYTE *)&pNotice, pNotice.h.size);
				}
			}

			if ( this->m_BridgeData[iBridgeIndex].m_i64_BC_REMAIN_MSEC <= 30000 && this->m_BridgeData[iBridgeIndex].m_i64_BC_REMAIN_MSEC > 0 && this->m_BridgeData[iBridgeIndex].m_bBC_MSG_BEFORE_ENTER == false )
			{
				this->m_BridgeData[iBridgeIndex].m_bBC_MSG_BEFORE_ENTER = true;

				if ( iBridgeIndex == 0 )
				{
					PMSG_SET_DEVILSQUARE pMsg;

					PHeadSetB((LPBYTE)&pMsg, 0x92, sizeof(pMsg));
					pMsg.Type = 3;

					for (int i= g_ConfigRead.server.GetObjectStartUserIndex();i<g_ConfigRead.server.GetObjectMax();i++)
					{
						if ( gObj[i].Connected == PLAYER_PLAYING && gObj[i].Type == OBJ_USER)
						{
							if ( BC_MAP_RANGE(gObj[i].MapNumber) == FALSE )
							{
								if ( CC_MAP_RANGE(gObj[i].MapNumber) == FALSE )
								{
									if ( IT_MAP_RANGE(gObj[i].MapNumber) == FALSE )
									{
										if ( DG_MAP_RANGE(gObj[i].MapNumber) == FALSE )
										{
											if ( IMPERIAL_MAP_RANGE(gObj[i].MapNumber) == FALSE )
											{
												IOCP.DataSend(i, (UCHAR*)&pMsg, pMsg.h.size);
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}

	if ( this->m_BridgeData[iBridgeIndex].m_i64_BC_REMAIN_MSEC <= 0 )
	{
		if ( this->IsEventEnable() != FALSE )
		{
			this->SetState(iBridgeIndex, BC_STATE_PLAYING);
		}
		else
		{
			this->SetState(iBridgeIndex, BC_STATE_CLOSED);
		}
	}
}




void CBloodCastle::ProcState_Playing(int iBridgeIndex)
{
	ULONGLONG i64TICK_MSEC = GetTickCount64() - this->m_BridgeData[iBridgeIndex].m_i64_BC_TICK_COUNT;

	if ( i64TICK_MSEC >= 1000 )
	{
		this->m_BridgeData[iBridgeIndex].m_i64_BC_REMAIN_MSEC -= i64TICK_MSEC;
		this->m_BridgeData[iBridgeIndex].m_i64_BC_TICK_COUNT = GetTickCount64();

		if ( this->m_BridgeData[iBridgeIndex].m_i64_BC_REMAIN_MSEC <= ((this->m_iBC_TIME_MIN_PLAY*60-30)*1000) )
		{
			if ( this->m_BridgeData[iBridgeIndex].m_bBC_MSG_BEFORE_PLAY == false )
			{
				this->m_BridgeData[iBridgeIndex].m_bBC_MSG_BEFORE_PLAY = true;
				
				PMSG_SET_DEVILSQUARE pMsg;
				PHeadSetB((LPBYTE)&pMsg, 0x92, sizeof(pMsg));
				pMsg.Type = 4;
				this->SendBridgeAnyMsg((BYTE*)&pMsg, sizeof(pMsg), iBridgeIndex);
			}
		}

		if ( this->m_BridgeData[iBridgeIndex].m_bBC_MONSTER_KILL_COMPLETE != false	)
		{
			if ( this->m_BridgeData[iBridgeIndex].m_i64_BC_TICK_DOOR_OPEN != -1 )
			{
				if ( GetTickCount64() > this->m_BridgeData[iBridgeIndex].m_i64_BC_TICK_DOOR_OPEN )
				{
					this->ReleaseCastleBridge(iBridgeIndex);
					this->SendCastleBridgeBlockInfo(iBridgeIndex, 0);
					g_Log.Add("[Blood Castle] (%d) Bridge Change Bridge Attribute -> Open", iBridgeIndex+1);
					this->m_BridgeData[iBridgeIndex].m_i64_BC_TICK_DOOR_OPEN = -1; // Prevent multiple openings
					this->m_BridgeData[iBridgeIndex].m_iBC_CASTLE_BLOCK_INFO = 1002;

					if(this->m_BridgeData[iBridgeIndex].m_iBC_DOOR_MONSTER_INDEX == -1) //season 2.5 add-on
					{
						g_Log.Add("[Blood Castle] (%d) Door Not Subsist", iBridgeIndex+1);
					}
					else
					{
						g_Log.Add("[Blood Castle] (%d) Door Subsist (%d)(Name: %s)(Dieregen:%d)", iBridgeIndex+1, this->m_BridgeData[iBridgeIndex].m_iBC_DOOR_MONSTER_INDEX, gObj[this->m_BridgeData[iBridgeIndex].m_iBC_DOOR_MONSTER_INDEX].Name, gObj[this->m_BridgeData[iBridgeIndex].m_iBC_DOOR_MONSTER_INDEX].DieRegen);
					}
				}
			}
		}

		// Set Play Quest
		if ( this->m_BridgeData[iBridgeIndex].m_i64_BC_REMAIN_MSEC <= ((this->m_iBC_TIME_MIN_PLAY*60-60)*1000) && this->m_BridgeData[iBridgeIndex].m_bBC_PLAY_START == false )
		{
			PMSG_NOTICE pNotice;

			this->m_BridgeData[iBridgeIndex].m_i64_BC_REMAIN_MSEC = (this->m_iBC_TIME_MIN_PLAY*60)*1000;
			this->m_BridgeData[iBridgeIndex].m_bBC_CAN_PARTY = false;
			TNotice::MakeNoticeMsgEx((TNotice*)&pNotice, 0, Lang.GetText(0,66), iBridgeIndex+1);
			this->SendBridgeAnyMsg( (LPBYTE)&pNotice, pNotice.h.size, iBridgeIndex);
			this->ReleaseCastleEntrance(iBridgeIndex);
			this->SendCastleEntranceBlockInfo(iBridgeIndex, 0);
			this->m_BridgeData[iBridgeIndex].m_bBC_PLAY_START = true;
			this->SetMonster(iBridgeIndex);
			this->m_BridgeData[iBridgeIndex].m_iBC_CASTLE_BLOCK_INFO = 1001;
			this->SendNoticeState(iBridgeIndex, false);

			g_Log.Add("[Blood Castle] (%d) Blood Castle Quest Start", iBridgeIndex+1);
		}

		if ( this->m_BridgeData[iBridgeIndex].m_i64_BC_REMAIN_MSEC <= 30000 && this->m_BridgeData[iBridgeIndex].m_i64_BC_REMAIN_MSEC > 0 && this->m_BridgeData[iBridgeIndex].m_bBC_MSG_BEFORE_END == false)	// Set counter to kick
		{
			this->m_BridgeData[iBridgeIndex].m_bBC_MSG_BEFORE_END = true;
			PMSG_SET_DEVILSQUARE pMsg;
			PHeadSetB((LPBYTE)&pMsg, 0x92, sizeof(pMsg));
			pMsg.Type = 5;
			this->SendBridgeAnyMsg((BYTE *)&pMsg, sizeof(pMsg), iBridgeIndex);
		}
		
		if ( this->CheckEveryUserDie(iBridgeIndex) != false )
		{
			PMSG_NOTICE pNotice;

			TNotice::MakeNoticeMsg(&pNotice, 0, Lang.GetText(0,67));
			this->SendBridgeAnyMsg( (LPBYTE)&pNotice, pNotice.h.size, iBridgeIndex);

			g_Log.Add("[Blood Castle] (%d) Blood Castle Quest Fail Result -> Destroy Castle Door [%s][%s]",
				iBridgeIndex+1, this->m_BridgeData[iBridgeIndex].m_szKill_Door_AccountID,
				this->m_BridgeData[iBridgeIndex].m_szKill_Door_CharName);

			g_Log.Add("[Blood Castle] (%d) Blood Castle Quest Fail Result -> Destroy Saint Status [%s][%s]",
				iBridgeIndex+1, this->m_BridgeData[iBridgeIndex].m_szKill_Door_AccountID,
				this->m_BridgeData[iBridgeIndex].m_szKill_Status_CharName);
			this->GiveReward_Fail(iBridgeIndex);
			this->SetState(iBridgeIndex, BC_STATE_CLOSED);
			g_Log.Add("[Blood Castle] (%d) Blood Castle Quest Failed -> Every User Out", iBridgeIndex+1);
		}
		else
		{
			if ( this->m_BridgeData[iBridgeIndex].m_bBC_PLAY_START != false )
			{
				if ( this->m_BridgeData[iBridgeIndex].m_bBC_DOOR_TERMINATE_COMPLETE == false || this->m_BridgeData[iBridgeIndex].m_bBC_BOSS_MONSTER_KILL_COMPLETE != false )
				{
					this->SendNoticeState(iBridgeIndex, 1);
				}
				else
				{
					this->SendNoticeState(iBridgeIndex, 4);
				}
			}
		}

		if ( this->CheckWinnerExist(iBridgeIndex) == true )
		{
			if ( this->CheckWinnerValid(iBridgeIndex) == true )
			{
				if ( this->CheckWinnerPartyComplete(iBridgeIndex) == true )
				{
					this->GiveReward_Win(this->m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX, iBridgeIndex);
					this->SetState(iBridgeIndex, BC_STATE_PLAYEND);

					g_Log.Add("[Blood Castle] (%d) CheckWinnerPartyComplete(iBridgeIndex) == true",
						iBridgeIndex+1);

					return;
				}
			}
			else
			{
				this->GiveReward_Fail(iBridgeIndex);
				this->SetState(iBridgeIndex, BC_STATE_PLAYEND); 

				g_Log.Add("[Blood Castle] (%d) CheckWinnerValid(iBridgeIndex) == false",
					iBridgeIndex+1);

				return;
			}
		}
	}

	if ( this->m_BridgeData[iBridgeIndex].m_i64_BC_REMAIN_MSEC <= 0 )
	{
		if ( this->m_BridgeData[iBridgeIndex].m_bBC_REWARDED == false )
		{
			g_Log.Add("[Blood Castle] (%d) Blood Castle Quest Fail Result -> Destroy Castle Door [%s][%s]",
				iBridgeIndex+1, this->m_BridgeData[iBridgeIndex].m_szKill_Door_AccountID,
				this->m_BridgeData[iBridgeIndex].m_szKill_Door_CharName);

			g_Log.Add("[Blood Castle] (%d) Blood Castle Quest Fail Result -> Destroy Saint Status [%s][%s]",
				iBridgeIndex+1, this->m_BridgeData[iBridgeIndex].m_szKill_Status_AccountID,
				this->m_BridgeData[iBridgeIndex].m_szKill_Status_CharName);
			this->GiveReward_Fail(iBridgeIndex);

			g_Log.Add("[Blood Castle] (%d) Blood Castle Quest Failed -> Time Out", iBridgeIndex+1);
			
		}

		this->SetState(iBridgeIndex, BC_STATE_PLAYEND);
	}
}




void CBloodCastle::ProcState_PlayEnd(int iBridgeIndex)
{
	ULONGLONG i64TICK_MSEC = GetTickCount64() - this->m_BridgeData[iBridgeIndex].m_i64_BC_TICK_COUNT;

	if ( i64TICK_MSEC >= 1000 )
	{
		this->m_BridgeData[iBridgeIndex].m_i64_BC_REMAIN_MSEC -= i64TICK_MSEC;
		this->m_BridgeData[iBridgeIndex].m_i64_BC_TICK_COUNT = GetTickCount64();

		if ( this->m_BridgeData[iBridgeIndex].m_i64_BC_REMAIN_MSEC <= 30000 && this->m_BridgeData[iBridgeIndex].m_i64_BC_REMAIN_MSEC > 0 && this->m_BridgeData[iBridgeIndex]. m_bBC_MSG_BEFORE_QUIT == false)
		{
			this->m_BridgeData[iBridgeIndex]. m_bBC_MSG_BEFORE_QUIT = true;

			PMSG_SET_DEVILSQUARE pMsg;

			PHeadSetB((LPBYTE)&pMsg, 0x92, sizeof(pMsg));
			pMsg.Type = 6;

			this->SendBridgeAnyMsg((BYTE *)&pMsg, sizeof(pMsg), iBridgeIndex);
		}

	}

	if ( this->m_BridgeData[iBridgeIndex].m_i64_BC_REMAIN_MSEC <= 0 )
	{
		this->SetState(iBridgeIndex, BC_STATE_CLOSED);
	}
}




void CBloodCastle::SetState_None(int iBridgeIndex)
{
	if (BC_BRIDGE_RANGE(iBridgeIndex) == FALSE)
	{
		return;
	}

	this->m_BridgeData[iBridgeIndex].m_i64_BC_REMAIN_MSEC = -1;
	this->m_BridgeData[iBridgeIndex].m_i64_BC_TICK_COUNT = -1;
	this->SendNoticeState(iBridgeIndex, 2);
	this->ClearBridgeData(iBridgeIndex);
	this->ClearMonster(iBridgeIndex, 1);

	for (int n = g_ConfigRead.server.GetObjectStartUserIndex(); n < g_ConfigRead.server.GetObjectMax(); n++)
	{
		int iMapNumber = this->GetBridgeMapNumber(iBridgeIndex); //season3 add-on

		if (gObj[n].MapNumber == iMapNumber && gObj[n].Connected == PLAYER_PLAYING) //season3 changed
		{
			gObjMoveGate(n, 22);
		}
	}

	this->BlockCastleDoor(iBridgeIndex);
}



void CBloodCastle::SetState_Closed(int iBridgeIndex)
{
	if (BC_BRIDGE_RANGE(iBridgeIndex) == FALSE)
	{
		return;
	}

	this->SendNoticeState(iBridgeIndex, 2);
	this->ClearBridgeData(iBridgeIndex);
	this->ClearMonster(iBridgeIndex, 1);
	this->CheckAngelKingExist(iBridgeIndex);

	for (int n = g_ConfigRead.server.GetObjectStartUserIndex(); n<g_ConfigRead.server.GetObjectMax(); n++)
	{
		int iMapNumber = this->GetBridgeMapNumber(iBridgeIndex); //season3 add-on

		if (gObj[n].MapNumber == iMapNumber && gObj[n].Connected > PLAYER_LOGGED) //season3 changed
		{
			this->SearchUserDeleteQuestItem(n);
			gObjMoveGate(n, 22);
		}
	}

	this->BlockCastleDoor(iBridgeIndex);
	this->BlockCastleBridge(iBridgeIndex);
	this->BlockCastleEntrance(iBridgeIndex);
	this->CheckSync(iBridgeIndex);
	g_Log.Add("[Blood Castle] (%d) SetState CLOSED", iBridgeIndex + 1);
}




void CBloodCastle::SetState_Playing(int iBridgeIndex)
{
	if (BC_BRIDGE_RANGE(iBridgeIndex) == FALSE)
	{
		return;
	}

	this->m_BridgeData[iBridgeIndex].m_bBC_CAN_ENTER = false;
	this->m_BridgeData[iBridgeIndex].m_i64_BC_REMAIN_MSEC = this->m_iBC_TIME_MIN_PLAY * 60 * 1000;
	this->CheckUsersOnConnect(iBridgeIndex);

	PMSG_NOTICE pNotice;

	TNotice::MakeNoticeMsgEx(&pNotice, 1, Lang.GetText(0, 68), iBridgeIndex + 1, 60);
	this->SendBridgeAnyMsg((LPBYTE)&pNotice, pNotice.h.size, iBridgeIndex);

	PMSG_SERVERCMD ServerCmd;

	PHeadSubSetB((LPBYTE)&ServerCmd, 0xF3, 0x40, sizeof(ServerCmd));
	ServerCmd.CmdType = 1;
	ServerCmd.X = 45;
	ServerCmd.Y = 0;

	this->SendBridgeAnyMsg((BYTE *)&ServerCmd, ServerCmd.h.size, iBridgeIndex);

	for (int n = g_ConfigRead.server.GetObjectStartUserIndex(); n<g_ConfigRead.server.GetObjectMax(); n++)
	{
		if (gObj[n].MapNumber == this->GetBridgeMapNumber(iBridgeIndex) && gObj[n].Connected > PLAYER_LOGGED) //season3 changed
		{
			this->SearchUserDeleteQuestItem(n);
		}
	}

	g_Log.Add("[Blood Castle] (%d) SetState PLAYING", iBridgeIndex + 1);
}




void CBloodCastle::SetState_PlayEnd(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	this->SendNoticeState(iBridgeIndex, 2);
	this->ClearMonster(iBridgeIndex, 0);
	this->m_BridgeData[iBridgeIndex].m_bBC_CAN_ENTER = false;
	this->m_BridgeData[iBridgeIndex].m_i64_BC_REMAIN_MSEC = this->m_iBC_TIME_MIN_REST*60*1000;

	g_Log.Add("[Blood Castle] (%d) SetState PLAY-END", iBridgeIndex+1);

	for (int n=0;n<MAX_BLOOD_CASTLE_SUB_BRIDGE;n++)
	{
		if ( ObjectMaxRange(this->m_BridgeData[iBridgeIndex].m_UserData[n].m_iIndex) != FALSE )
		{
			this->SearchUserDeleteQuestItem(this->m_BridgeData[iBridgeIndex].m_UserData[n].m_iIndex);
		}
	}
}






int  CBloodCastle::GetCurrentState(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return -1;
	}

	return this->m_BridgeData[iBridgeIndex].m_iBC_STATE;
}




LONGLONG  CBloodCastle::GetCurrentRemainSec(int iBridgeIndex)
{
	return this->m_BridgeData[iBridgeIndex].m_i64_BC_REMAIN_MSEC / 1000;
}




int  CBloodCastle::CheckEnterLevel(int iIndex, int iLevel)	// RET : [2:Error][1:OverLevel][0:InLevel - Success][-1:UnderLevel]
{
	if ( ObjectMaxRange(iIndex) == FALSE )
	{
		return 2;
	}

	if ( gObj[iIndex].Type != OBJ_USER || gObj[iIndex].Connected <= PLAYER_LOGGED )
	{
		return 2;
	}

	if ( gObj[iIndex].Class == CLASS_DARKLORD || gObj[iIndex].Class == CLASS_MAGUMSA || gObj[iIndex].Class == CLASS_RAGEFIGHTER )
	{
		if ( gObj[iIndex].m_PlayerData->ChangeUP == 2)
		{
			if(iLevel != 8)
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}
		if ( gObj[iIndex].Level >= g_sttBLOODCASTLE_LEVEL[iLevel-1].iLOWER_BOUND_MAGUMSA && gObj[iIndex].Level <= g_sttBLOODCASTLE_LEVEL[iLevel-1].iUPPER_BOUND_MAGUMSA )
		{
			return 0;
		}

		if ( gObj[iIndex].Level < g_sttBLOODCASTLE_LEVEL[iLevel-1].iLOWER_BOUND_MAGUMSA )
		{
			return -1;
		}

		if ( gObj[iIndex].Level > g_sttBLOODCASTLE_LEVEL[iLevel-1].iUPPER_BOUND_MAGUMSA )
		{
			return 1;
		}
	}
	else
	{
		if ( gObj[iIndex].m_PlayerData->ChangeUP == 2)
		{
			if(iLevel != 8)
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}
		if ( gObj[iIndex].Level >= g_sttBLOODCASTLE_LEVEL[iLevel-1].iLOWER_BOUND && gObj[iIndex].Level <= g_sttBLOODCASTLE_LEVEL[iLevel-1].iUPPER_BOUND )
		{
			return 0;
		}

		if ( gObj[iIndex].Level < g_sttBLOODCASTLE_LEVEL[iLevel-1].iLOWER_BOUND )
		{
			return -1;
		}

		if ( gObj[iIndex].Level > g_sttBLOODCASTLE_LEVEL[iLevel-1].iUPPER_BOUND )
		{
			return 1;
		}
	}

	return 2;
}



bool CBloodCastle::CheckEnterFreeTicket(int iIndex)
{
	if ( !ObjectMaxRange(iIndex) )
		return false;

	if ( gObj[iIndex].Type != OBJ_USER || gObj[iIndex].Connected <= PLAYER_LOGGED )
		return false;

	for (int x=0;x<MAIN_INVENTORY_SIZE;x++)
	{
		if ( gObj[iIndex].pInventory[x].IsItem() == TRUE )
		{
			if ( gObj[iIndex].pInventory [x].m_Type == ITEMGET(13,47) )
			{
				return true;
			}
		}
	}

	return false;
}

bool CBloodCastle::BloodCastleChaosMix(int iIndex, int iLEVEL)
{
	if ( ObjectMaxRange(iIndex) == FALSE )
	{
		return false;
	}

	int iMapNumber = this->GetBridgeMapNumber(iLEVEL-1); //Season3 add-on

	if ( BC_MAP_RANGE(iMapNumber)  == FALSE ) //season3 changed
	{
		return false;
	}

	BOOL bMIX_RESULT = FALSE;

	PMSG_CHAOSMIXRESULT pMsg;

	PHeadSetB((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR; //
	gObj[iIndex].ChaosLock = TRUE;

	g_Log.Add("[Blood Castle] BloodCastle Ticket Mix Chaos Mix Start (Account:%s, Name:%s, Level:%d)", gObj[iIndex].AccountID, gObj[iIndex].Name, iLEVEL);

	char szTemp[64];
	wsprintf(szTemp, "BloodCastle Ticket Mix,%d", iLEVEL); //Season 2.5 add-on

	g_MixSystem.LogChaosItem(&gObj[iIndex], szTemp);

	int iMIX_SUCCESS_RATE = g_iBC_ChoasMixSuccessRate[iLEVEL - 1];

	if ( iMIX_SUCCESS_RATE < 0 || iMIX_SUCCESS_RATE > 100 )
	{
		IOCP.DataSend(iIndex, (LPBYTE)&pMsg, pMsg.h.size);
		g_Log.Add("[Blood Castle] BloodCastle Ticket Mix Chaos Mix Failed - MixRate Out of Bound (Account:%s, Name:%s, Level:%d)", gObj[iIndex].AccountID, gObj[iIndex].Name, iLEVEL);
		return false;
	}

	if ( g_CrywolfSync.GetOccupationState() == 0 && g_CrywolfSync.GetApplyBenefit() == TRUE )
	{
		iMIX_SUCCESS_RATE += g_CrywolfSync.GetPlusChaosRate();
	}

	if ( iMIX_SUCCESS_RATE > 80 )
	{
		iMIX_SUCCESS_RATE = 80;
	}

	if ( gObj[iIndex].ChaosSuccessRate > 10 )
	{
		pMsg.Result = 0xF0;
		gObj[iIndex].ChaosLock = FALSE;

		IOCP.DataSend(iIndex, (LPBYTE)&pMsg, pMsg.h.size);
	}

	iMIX_SUCCESS_RATE += gObj[iIndex].ChaosSuccessRate;
	int iMIX_NEED_MONEY = g_iBC_ChoasMixMoney[iLEVEL - 1];
	int iChaosTaxMoney = (int)((__int64)(iMIX_NEED_MONEY) * (__int64)(g_CastleSiegeSync.GetTaxRateChaos(iIndex)) / (__int64)100);

	if ( iChaosTaxMoney < 0 )
	{
		iChaosTaxMoney = 0;
	}

	iMIX_NEED_MONEY += iChaosTaxMoney;

	if ( iMIX_NEED_MONEY <  0 )
	{
		IOCP.DataSend(iIndex, (LPBYTE)&pMsg, pMsg.h.size);
		g_Log.Add("[Blood Castle] BloodCastle Ticket Mix Chaos Mix Failed - MixMoney < 0 (Account:%s, Name:%s, Level:%d)", gObj[iIndex].AccountID, gObj[iIndex].Name, iLEVEL);
		return false;
	}

	if ( (gObj[iIndex].m_PlayerData->Money - iMIX_NEED_MONEY) < 0 )
	{
		pMsg.Result = CB_BC_NOT_ENOUGH_ZEN;
		IOCP.DataSend(iIndex, (LPBYTE)&pMsg, pMsg.h.size);
		g_Log.Add("[Blood Castle] BloodCastle Ticket Mix Chaos Mix Failed - Not Enough Money (Account:%s, Name:%s, Level:%d)", gObj[iIndex].AccountID, gObj[iIndex].Name, iLEVEL);
		return false;
	}

	gObj[iIndex].m_PlayerData->Money -= iMIX_NEED_MONEY;
	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
	GSProtocol.GCMoneySend(iIndex, gObj[iIndex].m_PlayerData->Money);

	if ( (rand()%100) < iMIX_SUCCESS_RATE )
	{
		int item_num = ITEMGET(13,18);
		ItemSerialCreateSend(iIndex, -1, 0, 0, item_num, iLEVEL, 255, 0, 0, 0, -1, 0, 0, 0, 0, 0);
		g_Log.Add("[BloodCastle Ticket Mix] [%s][%s] CBMix Success %d Money : %d-%d", gObj[iIndex].AccountID, gObj[iIndex].Name, iMIX_SUCCESS_RATE, gObj[iIndex].m_PlayerData->Money, iMIX_NEED_MONEY);
	}
	else
	{
		g_MixSystem.ChaosBoxInit(&gObj[iIndex]);
		GSProtocol.GCUserChaosBoxSend(&gObj[iIndex], 0);
		IOCP.DataSend(iIndex, (LPBYTE)&pMsg, pMsg.h.size);
		g_Log.Add("[BloodCastle Ticket Mix] [%s][%s] CBMix Fail %d Money : %d-%d", gObj[iIndex].AccountID, gObj[iIndex].Name, iMIX_SUCCESS_RATE, gObj[iIndex].m_PlayerData->Money, iMIX_NEED_MONEY);
		return false;
	}

	::gObjInventoryCommit(iIndex);

	return true;
}



static const struct ST_BC_EVENT_SCORE
{
	int unk0;
	int unk4;
	int unk8;
	int unkC;
	int unk10;

} g_iBC_EventScore[MAX_BLOOD_CASTLE_LEVEL] = 
{
	600, 300, 1000, 800, 400,
	600, 300, 1000, 800, 400,
	600, 300, 1005, 800, 400,
	600, 300, 1005, 800, 400,         
	600, 300, 1005, 800, 400,
	600, 300, 1005, 800, 400,
	600, 300, 1005, 800, 400,
	600, 300, 1005, 800, 400
};

static const int g_iBC_EventScore_Fail[MAX_BLOOD_CASTLE_LEVEL] = { -300,-300,-300,-300,-300,-300,-300, -300 };	

static const struct  ST_BC_ADD_EXP
{
	int unk0;
	int unk4;
	int unk8;
	int unkC;

} g_iBC_Add_Exp[MAX_BLOOD_CASTLE_LEVEL] =
{
    20000, 20000, 5000, 160,
	50000, 50000, 10000, 180,
	80000, 80000, 15000, 200,
	90000, 90000, 20000, 220,
	100000, 100000, 25000, 240,
	110000, 110000, 30000, 260,
	120000, 120000, 35000, 280,
	130000, 130000, 40000, 300
};

static const int g_iBC_Party_EventPoint[MAX_USER_IN_PARTY]	= {5, 10, 15, 20, 30 };
static const int g_iBC_MONSTER_CHANGE_STATE[2][3] =
{
	//	DamageMin	DamageMax	Defense
	10,	20,	-14,
	20, 40, -32
};



int  CBloodCastle::CheckChoasMixItem(int iIndex)
{
	if ( ObjectMaxRange(iIndex) == FALSE )
	{
		return false;
	}

	int iCHAOS_MIX_LEVEL = 0;
	BOOL bIsChaosGemExist = FALSE;
	BOOL bIsAngelKingPaperExist = FALSE;
	BOOL bIsBloodBoneExist = FALSE;
	BOOL bIsOtherItemExist = FALSE;
	int iEventItemCount = 0;
	int iAngelKingPaperLevel = 0;
	int iBloodBoneLevel = 0;
	int iCharmOfLuckCount=0;

	for ( int i=0;i<CHAOS_BOX_SIZE;i++)
	{
		if ( gObj[iIndex].pChaosBox[i].IsItem() == TRUE )
		{
			if ( gObj[iIndex].pChaosBox[i].m_Type == ITEMGET(12,15) ) // Chaos
			{
				bIsChaosGemExist = TRUE;
			}
			else if ( gObj[iIndex].pChaosBox[i].m_Type == ITEMGET(13,16) ) //Scroll of Archangel
			{
				int iSCROLL_LEVEL = gObj[iIndex].pChaosBox[i].m_Level;
				iEventItemCount++;
				bIsAngelKingPaperExist = TRUE;
				iAngelKingPaperLevel = iSCROLL_LEVEL;
			}
			else if ( gObj[iIndex].pChaosBox[i].m_Type == ITEMGET(13,17) ) //Blood Bone
			{
				int iBLOOD_BONE_LEVEL = gObj[iIndex].pChaosBox[i].m_Level;
				iEventItemCount++;
				bIsBloodBoneExist = TRUE;
				iBloodBoneLevel = iBLOOD_BONE_LEVEL;
			}
			else if ( gObj[iIndex].pChaosBox[i].m_Type == ITEMGET(14,53) ) //Charm
			{
				iCharmOfLuckCount += gObj[iIndex].pChaosBox[i].m_Durability;
			}
			else
			{
				bIsOtherItemExist = TRUE;
			}
		}
	}

	gObj[iIndex].ChaosSuccessRate = iCharmOfLuckCount;

	if ( bIsOtherItemExist != FALSE )
	{
		return 0;
	}

	if ( bIsAngelKingPaperExist == FALSE && bIsBloodBoneExist == FALSE )
	{
		return 0;
	}

	if ( bIsAngelKingPaperExist == FALSE || bIsBloodBoneExist == FALSE )
	{
		return 11;
	}

	if ( iEventItemCount > 2 )
	{
		return 12;
	}

	if ( iCharmOfLuckCount > 10 )
	{
		return 15;
	}

	if ( iAngelKingPaperLevel != iBloodBoneLevel )
	{
		return 9;
	}

	if ( BC_BRIDGE_RANGE(iAngelKingPaperLevel-1) == FALSE )
	{
		return 9;
	}

	if ( BC_BRIDGE_RANGE(iBloodBoneLevel-1) == FALSE )
	{
		return 9;
	}

	if ( bIsChaosGemExist == FALSE )
	{
		return 10;
	}

	if ( gObj[iIndex].Class == CLASS_DARKLORD || gObj[iIndex].Class == CLASS_MAGUMSA || gObj[iIndex].Class == CLASS_RAGEFIGHTER )
	{
		if ( gObj[iIndex].Level < g_sttBLOODCASTLE_LEVEL[0].iLOWER_BOUND_MAGUMSA )
		{
			return 14;
		}
	}
	else
	{
		if ( gObj[iIndex].Level < g_sttBLOODCASTLE_LEVEL[0].iLOWER_BOUND )
		{
			return 14;
		}
	}

	if ( bIsChaosGemExist != FALSE && bIsAngelKingPaperExist != FALSE && bIsBloodBoneExist != FALSE )
	{
		return iAngelKingPaperLevel;
	}

	return 0;
}





int  CBloodCastle::CheckEnterItem(int iIndex)
{
	int iITEM_LEVEL = 0;

	if ( ObjectMaxRange(iIndex) == FALSE )
	{
		return 0;
	}

	if ( gObj[iIndex].Type != OBJ_USER || gObj[iIndex].Connected <= PLAYER_LOGGED )
	{
		return 0;
	}

	for ( int x=0;x<MAIN_INVENTORY_SIZE;x++)
	{
		if ( gObj[iIndex].pInventory[x].IsItem() == TRUE )
		{
			if ( gObj[iIndex].pInventory[x].m_Type == ITEMGET(13,18) ) // Invisibility Cloak
			{
				iITEM_LEVEL = gObj[iIndex].pInventory[x].m_Level;

				if ( CHECK_LIMIT(iITEM_LEVEL, MAX_CLOACK_LEVEL) == FALSE )
				{
					iITEM_LEVEL = 0;

				}

				if ( iITEM_LEVEL != 0 )
				{
					return iITEM_LEVEL;
				}
			}
			
			if ( gObj[iIndex].pInventory[x].m_Type == ITEMGET(13,47) ) // 
			{
				iITEM_LEVEL = 10;
			}
		}
	}

	return iITEM_LEVEL;
}





int  CBloodCastle::CheckQuestItem(int iIndex)
{
	int iITEM_LEVEL = -1;

	if ( ObjectMaxRange(iIndex) == FALSE )
	{
		return -1;
	}

	if ( BC_MAP_RANGE(gObj[iIndex].MapNumber) == FALSE )
	{
		return -1;
	}

	int iBridgeIndex = this->GetBridgeIndex(gObj[iIndex].MapNumber); //season3 add-on

	if ( this->m_BridgeData[iBridgeIndex].m_nBC_QUESTITEM_SERIAL == -1 )
	{
		return -1;
	}

	if ( gObj[iIndex].Type != OBJ_USER || gObj[iIndex].Connected <= PLAYER_LOGGED )
	{
		return -1;
	}

	for ( int x=0;x<INVENTORY_SIZE;x++)
	{
		if ( gObj[iIndex].pInventory[x].IsItem() == TRUE )
		{
			if ( gObj[iIndex].pInventory[x].m_Type == ITEMGET(13,19) ) // Absolute Weapon of Archangel QUEST ITEM
			{
				if ( gObj[iIndex].pInventory[x].m_Number	== this->m_BridgeData[iBridgeIndex].m_nBC_QUESTITEM_SERIAL )
				{
					iITEM_LEVEL = gObj[iIndex].pInventory[x].m_Level;

					if ( iITEM_LEVEL < 0 || iITEM_LEVEL > 2 )
					{
						iITEM_LEVEL = -1;
						
					}

					break;
				}
			}
		}
	}

	return iITEM_LEVEL;
}




bool CBloodCastle::CheckWalk(int iIndex, int iMoveX, int iMoveY)
{
	if ( ObjectMaxRange(iIndex) == FALSE )
	{
		return false;
	}

	if (MAX_MAP_RANGE(gObj[iIndex].MapNumber) == FALSE)
	{
		return false;
	}
	
	if ( BC_MAP_RANGE(gObj[iIndex].MapNumber) == FALSE )
	{
		return false;
	}

	if ( gObj[iIndex].Type != OBJ_USER || gObj[iIndex].Connected <= PLAYER_LOGGED )
	{
		return false;
	}

	if ( this->GetCurrentState(gObj[iIndex].m_cBloodCastleIndex ) == TRUE )
	{
		BYTE btMapAttr = MapC[gObj[iIndex].MapNumber].GetAttr(iMoveX, iMoveY);

		if ( (btMapAttr&1) != 1 )
		{
			return true;
		}
	}

	return false;
}




bool CBloodCastle::CheckCanEnter(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return false;
	}

	return this->m_BridgeData[iBridgeIndex].m_bBC_CAN_ENTER;
}




bool CBloodCastle::CheckCanParty(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return false;
	}

	return this->m_BridgeData[iBridgeIndex].m_bBC_CAN_PARTY;
}





bool CBloodCastle::CheckQuestItemSerial(int iBridgeIndex, CMapItem * lpItem)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return false;
	}

	if ( this->m_BridgeData[iBridgeIndex].m_nBC_QUESTITEM_SERIAL == -1 )
	{
		return false;
	}
	
	if ( lpItem->IsItem() == TRUE )
	{
		if ( lpItem->m_Type == ITEMGET(13,19) ) // Absolute Weapon
		{
			int iLEVEL = lpItem->m_Level;

			if ( BC_WEAPON_LEVEL_RANGE(iLEVEL) != FALSE )
			{
				if ( this->m_BridgeData[iBridgeIndex].m_nBC_QUESTITEM_SERIAL == lpItem->m_Number )
				{
					return true;
				}
			}
		}
	}
	

	return false;

}




bool CBloodCastle::CheckPlayStart(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return false;
	}

	return this->m_BridgeData[iBridgeIndex].m_bBC_PLAY_START;
}




int  CBloodCastle::GetRemainTime(int iBridgeIndex)
{
	int iREMAIN_MINUTE = 0;

	if ( this->GetCurrentState(iBridgeIndex) == 1 )
	{
		iREMAIN_MINUTE = (int)this->m_BridgeData[iBridgeIndex].m_i64_BC_REMAIN_MSEC / 60000 - this->m_iBC_TIME_MIN_OPEN + 1;
	}
	else
	{
		std::list<BLOODCASTLE_START_TIME>::iterator it; //loc3 -> ebp C
		BLOODCASTLE_START_TIME WebzenVar1; //loc 4-5 -> ebp 14
		BLOODCASTLE_START_TIME WebzenVar2; //loc 6-7 -> epb 18
		int BaseTime = 0; // loc8 -> ebp 1C
		int CheckTime = 0; // loc9 -> ebp 20
		DWORD CurrentTime = 0;	// loc10-> ebp 24
		
		tm * today; //loc11 -> ebp 28
		time_t ltime; //loc12 -> ebp 2C

		int loc13; //ebp 34

		time(&ltime); //
		today = localtime(&ltime); //
		
		CurrentTime = (today->tm_hour * 60) + today->tm_min;
		WebzenVar1 = *this->m_listBloodCastleOpenTime.begin();

		for( it = this->m_listBloodCastleOpenTime.begin(); it != this->m_listBloodCastleOpenTime.end(); ++it ) //for identical
		{
			WebzenVar2 = *it; // loc5-6
			BaseTime = (WebzenVar1.m_iHour * 60) + WebzenVar1.m_iMinute;
			CheckTime =	(WebzenVar2.m_iHour * 60) + WebzenVar2.m_iMinute;

			if( BaseTime == CheckTime )
			{
				if( CurrentTime < CheckTime )
				{
					WebzenVar2 = *it;
					break;
				}
				continue;
			}
			
			if( CurrentTime >= BaseTime && CurrentTime < CheckTime )
			{
				break;
			}
			else
			{
				WebzenVar1 = *it;
			}
		}

		for(loc13 = 2;loc13--;) //good ->func identical so far
		{
			if(it == this->m_listBloodCastleOpenTime.end())
			{
				it = this->m_listBloodCastleOpenTime.begin();

				WebzenVar2 = (*it);
			}

			CheckTime = WebzenVar2.m_iHour*60+WebzenVar2.m_iMinute;

			if(today->tm_hour <= WebzenVar2.m_iHour && CheckTime > CurrentTime)
			{
				iREMAIN_MINUTE = ((CheckTime - CurrentTime)*60)*1000;
			}
			else
			{
				iREMAIN_MINUTE = ((1440-CurrentTime+CheckTime)*60)*1000;
			}

			if ( iREMAIN_MINUTE <= ( this->m_iBC_TIME_MIN_OPEN * 60 * 1000 )) //should be if ( iREMAIN_MINUTE <= ( this->m_iBC_TIME_MIN_OPEN * 60 * 1000 ))
			{
				it++;

				if(it != this->m_listBloodCastleOpenTime.end())
				{
					WebzenVar2 = (*it);
				}
			}
			else
			{
				break;
			}
		}

		iREMAIN_MINUTE = (int)this->m_BridgeData[iBridgeIndex].m_i64_BC_REMAIN_MSEC - today->tm_sec * 1000;
	}

	return iREMAIN_MINUTE;
}




void CBloodCastle::ClearMonster(int iBridgeIndex, bool bClearCastleDoor)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	g_Log.Add("[Blood Castle] (%d) Perform to Delete Monsters (ClearDoor:%d)", iBridgeIndex + 1, bClearCastleDoor); //season 2.5 add-on

	int iCount = 0;

	for ( int n = 0; n < g_ConfigRead.server.GetObjectMaxMonster(); n++)	
	{
		if ( gObj[n].MapNumber == this->GetBridgeMapNumber(iBridgeIndex))
		{
			if ( bClearCastleDoor == false && gObj[n].Class == 131)
			{
				continue;
			}

			if ( gObj[n].Class == 232 )
			{
				continue;
			}

			gObjDel(n);
			iCount++;
		}
	}

	g_Log.Add("[Blood Castle] (%d) Deleted Monsters (Count:%d)", iBridgeIndex + 1, iCount); //season 2.5 add-on
}




void CBloodCastle::SetMonster(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	this->SetMonsterKillCount(iBridgeIndex);
	BYTE btMapNumber = this->GetBridgeMapNumber(iBridgeIndex);
	WORD wMonsterType = this->m_BCMP_CastleGate[iBridgeIndex].m_Type;
	BYTE btBloodCastleIndex = this->GetBridgeIndex(btMapNumber);

	int iIndex = ::gObjAddMonster(btMapNumber);

	if ( iIndex >= 0 )
	{
		gObj[iIndex].X = this->m_BCMP_CastleGate[iBridgeIndex].m_X;
		gObj[iIndex].Y = this->m_BCMP_CastleGate[iBridgeIndex].m_Y;
		gObj[iIndex].MapNumber = this->m_BCMP_CastleGate[iBridgeIndex].m_MapNumber;
		gObj[iIndex].TX = gObj[iIndex].X;
		gObj[iIndex].TY = gObj[iIndex].Y;
		gObj[iIndex].m_OldX = gObj[iIndex].X;
		gObj[iIndex].m_OldY = gObj[iIndex].Y;
		gObj[iIndex].Dir = this->m_BCMP_CastleGate[iBridgeIndex].m_Dir;
		gObj[iIndex].StartX = gObj[iIndex].X;
		gObj[iIndex].StartY = gObj[iIndex].Y;

		gObjSetMonster(iIndex, wMonsterType);

		gObj[iIndex].m_cBloodCastleIndex = btBloodCastleIndex;
		gObj[iIndex].Dir = 1;
		gObj[iIndex].m_PosNum = -1;
		gObj[iIndex].Live = TRUE;
		gObj[iIndex].DieRegen = FALSE;
		gObj[iIndex].m_State = 1;
		gObj[iIndex].MaxRegenTime = 0;
		gObj[iIndex].MaxLife = this->m_BridgeData[btBloodCastleIndex].m_iCastleStatueHealth;
		gObj[iIndex].Life = this->m_BridgeData[btBloodCastleIndex].m_iCastleStatueHealth;

		g_Log.Add("[Blood Castle] (%d) Create Door Successfuly : Index(%d)", btBloodCastleIndex+1, this->m_BridgeData[btBloodCastleIndex].m_iBC_DOOR_MONSTER_INDEX = iIndex); //season 2.5 add-on
	}
	
	else
	{
		g_Log.Add("[Blood Castle] (%d) Create Door Fail", btBloodCastleIndex+1); //season 2.5 add-on
	}

	for (int i = 0; i < this->m_iBC_GENERAL_SCRIPT_CNT[iBridgeIndex]; i++)
	{
		if ( BC_MAP_RANGE(this->m_BCMP_General[iBridgeIndex][i].m_MapNumber) != FALSE  )
		{
			WORD wIndex = this->m_BCMP_General[iBridgeIndex][i].m_Type;
			BYTE btMap = this->m_BCMP_General[iBridgeIndex][i].m_MapNumber;
			BYTE btBridgeIndex = this->GetBridgeIndex(btMap); //season3 changed
			
			if ( btBridgeIndex != iBridgeIndex )
			{
				continue;
			}

			if ( wIndex == 232 )
			{
				continue;
			}

			if ( wIndex == 131 )
			{
				continue;
			}

			if ( wIndex == 89 || wIndex == 95 || wIndex == 112  || wIndex == 118 || wIndex == 124 || wIndex == 130 || wIndex == 143 || wIndex == 433 ) //season3 changed
			{
				continue;
			}

			if ( BC_STATUE_RANGE(wIndex-132) != FALSE )
			{
				continue;
			}

			iIndex = gObjAddMonster(this->m_BCMP_General[iBridgeIndex][i].m_MapNumber);

			if ( iIndex >= 0 )
			{
				gObj[iIndex].m_PosNum = i;
				gObj[iIndex].X = this->m_BCMP_General[iBridgeIndex][i].m_X;
				gObj[iIndex].Y = this->m_BCMP_General[iBridgeIndex][i].m_Y;
				gObj[iIndex].MapNumber = this->m_BCMP_General[iBridgeIndex][i].m_MapNumber;
				gObj[iIndex].TX = gObj[iIndex].X;
				gObj[iIndex].TY = gObj[iIndex].Y;
				gObj[iIndex].m_OldX = gObj[iIndex].X;
				gObj[iIndex].m_OldY = gObj[iIndex].Y;
				gObj[iIndex].Dir = this->m_BCMP_General[iBridgeIndex][i].m_Dir;
				gObj[iIndex].StartX = gObj[iIndex].X;
				gObj[iIndex].StartY = gObj[iIndex].Y;
				gObj[iIndex].m_MoveRange = this->m_BCMP_General[iBridgeIndex][i].m_Dis;
				gObj[iIndex].DieRegen = FALSE;
				gObj[iIndex].m_State = 1;
				gObjSetMonster(iIndex, wIndex);
				gObj[iIndex].MaxRegenTime = this->m_iBC_MONSTER_REGEN;
				gObj[iIndex].m_cBloodCastleIndex = btBridgeIndex;
				gObj[iIndex].m_ItemRate = 100;
			}
		}

		else
		{
			g_Log.AddC(TColor::Red, "[Blood Castle] (%d) Create Monster Fail - Wrong Map (%d) Pos:%d", btBloodCastleIndex+1, this->m_BCMP_General[iBridgeIndex][i].m_MapNumber, i); //season 2.5 add-on
		}
	}
}




void CBloodCastle::SetBossMonster(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	for ( int x=0;x<this->m_iBC_BOSS_SCRIPT_CNT[iBridgeIndex];x++)
	{
		WORD wMonsterType = this->m_BCMP_BossMonster[iBridgeIndex][x].m_Type; //season3 changed
		BYTE btMapNumber = this->m_BCMP_BossMonster[iBridgeIndex][x].m_MapNumber;
		BYTE btBloodCastleIndex = this->GetBridgeIndex(btMapNumber); //season3 changed

		if ( btBloodCastleIndex != iBridgeIndex )
		{
			continue;
		}

		if ( wMonsterType == 89 || wMonsterType == 95 || wMonsterType == 112 || wMonsterType == 118 || wMonsterType == 124 || wMonsterType == 130 || wMonsterType == 143 || wMonsterType == 433 )
		{
			int iIndex = ::gObjAddMonster(btMapNumber);

			if ( iIndex >= 0 )
			{
				this->SetPosMonster(iBridgeIndex, iIndex, x, wMonsterType);
				gObjSetMonster(iIndex, wMonsterType);
				gObj[iIndex].MaxRegenTime = this->m_iBC_MONSTER_REGEN;
				gObj[iIndex].m_cBloodCastleIndex = iBridgeIndex;
				gObj[iIndex].m_ItemRate = 100;
				gObj[iIndex].Dir = rand() % 8;
			}
		}
	}
}



void CBloodCastle::SetSaintStatue(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	WORD wMonsterType = this->m_BCMP_SaintStatue[iBridgeIndex].m_Type; //season3 changed
	BYTE btMapNumber = this->m_BCMP_SaintStatue[iBridgeIndex].m_MapNumber;
	BYTE btBloodCastleIndex = this->GetBridgeIndex(btMapNumber); //season3 changed

	if ( BC_STATUE_RANGE(wMonsterType-132) != FALSE )
	{
		int iIndex = gObjAddMonster(btMapNumber);

		if ( iIndex >= 0 )
		{
			gObj[iIndex].X = this->m_BCMP_SaintStatue[iBridgeIndex].m_X;
			gObj[iIndex].Y = this->m_BCMP_SaintStatue[iBridgeIndex].m_Y;
			gObj[iIndex].MapNumber = this->m_BCMP_SaintStatue[iBridgeIndex].m_MapNumber;
			gObj[iIndex].TX = gObj[iIndex].X;
			gObj[iIndex].TY = gObj[iIndex].Y;
			gObj[iIndex].m_OldX = gObj[iIndex].X;
			gObj[iIndex].m_OldY = gObj[iIndex].Y;
			gObj[iIndex].Dir = this->m_BCMP_SaintStatue[iBridgeIndex].m_Dir;
			gObj[iIndex].StartX = gObj[iIndex].X;
			gObj[iIndex].StartY = gObj[iIndex].Y;
			gObj[iIndex].DieRegen = FALSE;
			::gObjSetMonster(iIndex, wMonsterType);
			gObj[iIndex].Class = rand() % 3 + 132;
			gObj[iIndex].m_cBloodCastleIndex = btBloodCastleIndex;
			gObj[iIndex].m_ItemRate = 100;
			gObj[iIndex].Dir = 1;
			gObj[iIndex].m_PosNum = -1;
			gObj[iIndex].Live = TRUE;
			gObj[iIndex].DieRegen = 0;
			gObj[iIndex].m_State = 1;
			gObj[iIndex].MaxRegenTime = 0;
			gObj[iIndex].MaxLife = this->m_BridgeData[btBloodCastleIndex].m_iCastleStatueHealth;
			gObj[iIndex].Life = this->m_BridgeData[btBloodCastleIndex].m_iCastleStatueHealth;

			g_Log.Add("[Blood Castle] (%d) Saint Status be created.", iBridgeIndex+1); //season 2.5 add-on
		}
	}

	else
	{
		g_Log.Add("[Blood Castle][Bug Tracer] (%d) It's not Saint Statue( Map:%d, BloodCastleIndex:%d, Type:%d )", iBridgeIndex+1, btMapNumber, this->GetBridgeIndex(btMapNumber), wMonsterType); //season3 changed
	}
}



int  CBloodCastle::LeaveUserBridge(int iBridgeIndex, int iBridgeSubIndex, int iUserIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return -1;
	}

	if ( BC_SUB_BRIDGE_RANGE(iBridgeSubIndex) == FALSE )
	{
		return -1;
	}

	int iRET_VAL = -1;

	::EnterCriticalSection(&this->m_BridgeData[iBridgeIndex].m_critUserData);

	if ( this->m_BridgeData[iBridgeIndex].m_UserData[iBridgeSubIndex].m_iIndex == iUserIndex )
	{
		iRET_VAL = iUserIndex;
		this->m_BridgeData[iBridgeIndex].m_UserData[iBridgeSubIndex].m_iIndex = -1;
		this->m_BridgeData[iBridgeIndex].m_UserData[iBridgeSubIndex].m_iEXP = 0;
		this->m_BridgeData[iBridgeIndex].m_UserData[iBridgeSubIndex].m_iScore = 0;
		this->m_BridgeData[iBridgeIndex].m_UserData[iBridgeSubIndex].m_iUserState = 0;
	}

	::LeaveCriticalSection(&this->m_BridgeData[iBridgeIndex].m_critUserData);

	return iRET_VAL;
}




int  CBloodCastle::EnterUserBridge(int iBridgeIndex, int iUserIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return -1;
	}

	int iRET_VAL = -1;

	::EnterCriticalSection(&this->m_BridgeData[iBridgeIndex].m_critUserData);

	for ( int i=0;i<MAX_BLOOD_CASTLE_SUB_BRIDGE;i++)
	{
		if ( this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex != -1 )
		{
			if ( this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex == iUserIndex )
			{
				iRET_VAL = i;
				break;
			}
		}

		if ( this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex == -1 )
		{
			iRET_VAL = i;
			this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex = iUserIndex;
			this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iEXP = 0;
			this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iScore = 0;
			this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState = 0;
			gObj[iUserIndex].m_bBloodCastleComplete = false;
			break;
		}
	}

	::LeaveCriticalSection(&this->m_BridgeData[iBridgeIndex].m_critUserData);

	return iRET_VAL;
}




int  CBloodCastle::LevelUp(int iIndex, int iAddExp)
{
	if ( ObjectMaxRange(iIndex) == FALSE )
	{
		return 0;
	}

	if ( gObj[iIndex].Type != OBJ_USER )
	{
		return 0;
	}

	if ( g_MasterLevelSkillTreeSystem.IsMasterLevelUser(&gObj[iIndex]) == true)
	{
		g_MasterLevelSkillTreeSystem.MasterLevelUp(&gObj[iIndex], iAddExp, 0, "Blood Castle");
		return 0;
	}

	int iLEFT_EXP = 0;

	g_Log.Add("[Blood Castle] Experience : [%s][%s](%d) Experience: %I64d + %d",	// #error gObj[iIndex].X -> gObj[iIndex].Y
		gObj[iIndex].AccountID,	gObj[iIndex].Name,
		gObj[iIndex].Level, gObj[iIndex].m_PlayerData->Experience,
		iAddExp);

	::gObjSetExpPetItem(iIndex, iAddExp);

	if ( gObj[iIndex].Level >= g_ConfigRead.data.common.UserMaxLevel )
	{
		::GSProtocol.GCServerMsgStringSend(Lang.GetText(0,45), gObj[iIndex].m_Index, 1);
		return 0;
	}

	if ( (gObj[iIndex].m_PlayerData->Experience + iAddExp) < gObj[iIndex].m_PlayerData->NextExp )
	{
		gObj[iIndex].m_PlayerData->Experience += iAddExp;
	}
	else
	{
		iLEFT_EXP = gObj[iIndex].m_PlayerData->Experience + iAddExp - gObj[iIndex].m_PlayerData->NextExp;
		gObj[iIndex].m_PlayerData->Experience = gObj[iIndex].m_PlayerData->NextExp;
		gObj[iIndex].Level++;

		if ( g_ConfigRead.data.reset.iBlockLevelUpPointAfterResets == -1 || gObj[iIndex].m_PlayerData->m_iResets < g_ConfigRead.data.reset.iBlockLevelUpPointAfterResets )
		{
			if ( gObj[iIndex].Class == CLASS_DARKLORD || gObj[iIndex].Class == CLASS_MAGUMSA || gObj[iIndex].Class == CLASS_RAGEFIGHTER || gObj[iIndex].Class == CLASS_GROWLANCER )
			{
				gObj[iIndex].m_PlayerData->LevelUpPoint += gLevelUpPointMGDL;
			}

			else
			{
				gObj[iIndex].m_PlayerData->LevelUpPoint += gLevelUpPointNormal;
			}

			if ( gObj[iIndex].m_PlayerData->PlusStatQuestClear != false )
			{
				gObj[iIndex].m_PlayerData->LevelUpPoint++;

				g_Log.Add("[%s][%s] LevelUp PlusStatQuest Clear AddStat %d",
					gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].m_PlayerData->LevelUpPoint);
			}
		}

		gObj[iIndex].MaxLife += DCInfo.DefClass[gObj[iIndex].Class].LevelLife;
		gObj[iIndex].MaxMana += DCInfo.DefClass[gObj[iIndex].Class].LevelMana;
		gObj[iIndex].Life = gObj[iIndex].MaxLife;
		gObj[iIndex].Mana = gObj[iIndex].MaxMana;
		gObjNextExpCal(&gObj[iIndex]);
		gObjSetBP(iIndex);

		/*short AddPoint = 0;
		short MaxAddPoint = 0;
		short MinusPoint = 0;
		short MaxMinusPoint = 0;

		gObjGetStatPointState(gObj[iIndex].m_Index, AddPoint, MaxAddPoint, MinusPoint, MaxMinusPoint);*/

		GSProtocol.GCLevelUpMsgSend(gObj[iIndex].m_Index, 1);//gObj[iIndex].Level, gObj[iIndex].LevelUpPoint, 
		//	(int)((float)gObj[iIndex].AddLife + gObj[iIndex].MaxLife), (int)((float)gObj[iIndex].AddMana + gObj[iIndex].MaxMana),
		//	gObj[iIndex].MaxBP + gObj[iIndex].AddBP, AddPoint, MaxAddPoint);
		gObjCalcMaxLifePower(gObj[iIndex].m_Index);
		g_Log.Add("Level Up [%s][%s][%d]", gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].Level);
	}

	//GJSetCharacterInfo(&gObj[iIndex], gObj[iIndex].m_Index, 0);
	
	return iLEFT_EXP;
}



void CBloodCastle::CheckUsersOnConnect(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	::EnterCriticalSection(&this->m_BridgeData[iBridgeIndex].m_critUserData);

	for ( int i=0;i<MAX_BLOOD_CASTLE_SUB_BRIDGE;i++)
	{
		if ( this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex != -1 )
		{
			if ( gObjIsConnected(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex) == FALSE )
			{
				this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex = -1;
			}
			else
			{
				if ( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].MapNumber != this->GetBridgeMapNumber(iBridgeIndex) ) //season3 changed
				{
					this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex = -1;
				}
			}
		}
	}

	::LeaveCriticalSection(&this->m_BridgeData[iBridgeIndex].m_critUserData);

}





bool CBloodCastle::AddExperience(int iIndex, int iEXP)
{
	if ( ObjectMaxRange(iIndex) == FALSE )
	{
		return false;
	}

	if ( BC_BRIDGE_RANGE(gObj[iIndex].m_cBloodCastleIndex) == FALSE )
	{
		return false;
	}

	if ( BC_SUB_BRIDGE_RANGE(gObj[iIndex].m_cBloodCastleSubIndex) == FALSE )
	{
		return false;
	}

	if ( iEXP > 0 )
	{
		this->m_BridgeData[gObj[iIndex].m_cBloodCastleIndex].m_UserData[gObj[iIndex].m_cBloodCastleSubIndex].m_iEXP += iEXP;
		gObj[iIndex].m_iBloodCastleEXP += iEXP;
	}

	return true;
}



void CBloodCastle::BlockCastleEntrance(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	for ( int i= ::g_btCastleEntranceMapXY[iBridgeIndex].btStartX;i<= ::g_btCastleEntranceMapXY[iBridgeIndex].btEndX;i++)
	{
		for ( int j= ::g_btCastleEntranceMapXY[iBridgeIndex].btStartY;j<= ::g_btCastleEntranceMapXY[iBridgeIndex].btEndY;j++)
		{
			int iMapNumber = this->GetBridgeMapNumber(iBridgeIndex); //season3 add-on

			MapC[iMapNumber].m_attrbuf[j * 256 + i] |= 4; //season3 changed
		}
	}
}




void CBloodCastle::ReleaseCastleEntrance(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	for ( int i= ::g_btCastleEntranceMapXY[iBridgeIndex].btStartX;i<= ::g_btCastleEntranceMapXY[iBridgeIndex].btEndX;i++)
	{
		for ( int j= ::g_btCastleEntranceMapXY[iBridgeIndex].btStartY;j<= ::g_btCastleEntranceMapXY[iBridgeIndex].btEndY;j++)
		{
			int iMapNumber = this->GetBridgeMapNumber(iBridgeIndex); //season3 add-on

			MapC[iMapNumber].m_attrbuf[j * 256 + i] &= ~4; //season3 changed
		}
	}
}





void CBloodCastle::BlockCastleBridge(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	for ( int i= ::g_btCastleBridgeMapXY[iBridgeIndex].btStartX;i<= ::g_btCastleBridgeMapXY[iBridgeIndex].btEndX;i++)
	{
		for ( int j= ::g_btCastleBridgeMapXY[iBridgeIndex].btStartY;j<= ::g_btCastleBridgeMapXY[iBridgeIndex].btEndY;j++)
		{
			int iMapNumber = this->GetBridgeMapNumber(iBridgeIndex); //season3 add-on

			MapC[iMapNumber].m_attrbuf[j * 256 + i] |= 8; //season3 changed
		}
	}
}



void CBloodCastle::ReleaseCastleBridge(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	for ( int i= ::g_btCastleBridgeMapXY[iBridgeIndex].btStartX;i<= ::g_btCastleBridgeMapXY[iBridgeIndex].btEndX;i++)
	{
		for ( int j= ::g_btCastleBridgeMapXY[iBridgeIndex].btStartY;j<= ::g_btCastleBridgeMapXY[iBridgeIndex].btEndY;j++)
		{
			int iMapNumber = this->GetBridgeMapNumber(iBridgeIndex); //season3 add-on

			MapC[iMapNumber].m_attrbuf[j * 256 + i] &= ~8; //season3 changed

			g_Log.Add("[Blood Castle] (%d) Castle Door Subsist", iBridgeIndex+1); //season 2.5 add-on

			g_Log.Add("[Blood Castle][Bug Tracer] ReleaseCastleBridge-> %d", iBridgeIndex+1);
		}
	}
}



void CBloodCastle::BlockCastleDoor(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	for ( int x=0;x<3;x++)
	{
		for ( int y=::g_btCastleDoorMapXY[iBridgeIndex][x].btStartX; y <= ::g_btCastleDoorMapXY[iBridgeIndex][x].btEndX ;y++)
		{
			for ( int z = ::g_btCastleDoorMapXY[iBridgeIndex][x].btStartY; z <= ::g_btCastleDoorMapXY[iBridgeIndex][x].btEndY ; z++)
			{
				int iMapNumber = this->GetBridgeMapNumber(iBridgeIndex); //season3 add-on

				MapC[iMapNumber].m_attrbuf[z * 256 + y] |= 4; //season3 changed
			}
		}
	}
}




void CBloodCastle::ReleaseCastleDoor(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	for ( int x=0;x<3;x++)
	{
		for ( int y=::g_btCastleDoorMapXY[iBridgeIndex][x].btStartX; y <= ::g_btCastleDoorMapXY[iBridgeIndex][x].btEndX ;y++)
		{
			for ( int z = ::g_btCastleDoorMapXY[iBridgeIndex][x].btStartY; z <= ::g_btCastleDoorMapXY[iBridgeIndex][x].btEndY ; z++)
			{
				int iMapNumber = this->GetBridgeMapNumber(iBridgeIndex); //season3 add-on

				MapC[iMapNumber].m_attrbuf[z * 256 + y] &= ~4; //season3 changed
			}
		}
	}
}




void CBloodCastle::BlockSector(int iMAP_NUM, int iSTART_X, int iSTART_Y, int iEND_X, int iEND_Y)
{
	if ( BC_MAP_RANGE(iMAP_NUM) == FALSE )
	{
		return;
	}

	for ( int i=iSTART_X;i<=iEND_X;i++)
	{
		for ( int j=iSTART_Y;j<=iEND_Y;j++ )
		{
			MapC[iMAP_NUM].m_attrbuf[j * 256 + i] |= 4;
		}
	}
}



void CBloodCastle::ReleaseSector(int iMAP_NUM, int iSTART_X, int iSTART_Y, int iEND_X, int iEND_Y)
{

	if ( BC_MAP_RANGE(iMAP_NUM) == FALSE )
	{
		return;
	}

	for ( int i=iSTART_X;i<=iEND_X;i++)
	{
		for ( int j=iSTART_Y;j<=iEND_Y;j++ )
		{
			MapC[iMAP_NUM].m_attrbuf[j * 256 + i] &= ~4;
		}
	}

}






void CBloodCastle::SendCastleEntranceBlockInfo(int iBridgeIndex, bool bLive)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	char cTEMP_BUF[256];
	PMSG_SETMAPATTR_COUNT * lpMsg = (PMSG_SETMAPATTR_COUNT *)cTEMP_BUF;

	PHeadSetB((LPBYTE)lpMsg, 0x46, sizeof(PMSG_SETMAPATTR_COUNT)+sizeof(PMSG_SETMAPATTR)*6);
	PMSG_SETMAPATTR * lpMsgBody = (PMSG_SETMAPATTR *)&cTEMP_BUF[7];
	lpMsg->btType = 0;
	lpMsg->btCount = 1;
	lpMsg->btMapAttr = 4;
	(bLive)?(lpMsg->btMapSetType=0) :( lpMsg->btMapSetType=1);

	lpMsgBody[0].btX = ::g_btCastleEntranceMapXY[iBridgeIndex].btStartX;
	lpMsgBody[0].btY = ::g_btCastleEntranceMapXY[iBridgeIndex].btStartY;
	lpMsgBody[1].btX   = ::g_btCastleEntranceMapXY[iBridgeIndex].btEndX;
	lpMsgBody[1].btY   = ::g_btCastleEntranceMapXY[iBridgeIndex].btEndY;

	for (int i= g_ConfigRead.server.GetObjectMaxMonster();i<g_ConfigRead.server.GetObjectMax();i++)
	{
		int iMapNumber = this->GetBridgeMapNumber(iBridgeIndex); //season3 add-on

		if ( gObj[i].MapNumber == iMapNumber ) //season3 changed
		{
			if ( gObj[i].Connected > PLAYER_LOGGED )
			{
				IOCP.DataSend(i,(LPBYTE)lpMsg, lpMsg->h.size);
			}
		}
	}

}




void CBloodCastle::SendCastleBridgeBlockInfo(int iBridgeIndex, bool bLive)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	char cTEMP_BUF[256];
	PMSG_SETMAPATTR_COUNT * lpMsg = (PMSG_SETMAPATTR_COUNT *)cTEMP_BUF;

	PHeadSetB((LPBYTE)lpMsg, 0x46, sizeof(PMSG_SETMAPATTR_COUNT)+sizeof(PMSG_SETMAPATTR)*6);
	PMSG_SETMAPATTR * lpMsgBody = (PMSG_SETMAPATTR *)&cTEMP_BUF[7];
	lpMsg->btType = 0;
	lpMsg->btCount = 1;
	lpMsg->btMapAttr = 8;
	(bLive)?lpMsg->btMapSetType=0:lpMsg->btMapSetType=1;

	lpMsgBody[0].btX = ::g_btCastleEntranceMapXY[iBridgeIndex].btStartX;
	lpMsgBody[0].btY = ::g_btCastleEntranceMapXY[iBridgeIndex].btStartY;
	lpMsgBody[1].btX   = ::g_btCastleEntranceMapXY[iBridgeIndex].btEndX;
	lpMsgBody[1].btY   = ::g_btCastleEntranceMapXY[iBridgeIndex].btEndY;

	for (int i= g_ConfigRead.server.GetObjectMaxMonster();i<g_ConfigRead.server.GetObjectMax();i++)
	{
		int iMapNumber = this->GetBridgeMapNumber(iBridgeIndex); //season3 add-on

		if ( gObj[i].MapNumber == iMapNumber ) //season3 changed
		{
			if ( gObj[i].Connected > PLAYER_LOGGED )
			{
				IOCP.DataSend(i, (LPBYTE)lpMsg, lpMsg->h.size);

				g_Log.Add("[Blood Castle][Bug Tracer] SendCastleBridgeBlockInfo-> Bridge:%d/Live:%d, (%d,%d,%d,%d)", iBridgeIndex+1, bLive, lpMsgBody[0].btX, lpMsgBody[0].btY, lpMsgBody[1].btX, lpMsgBody[1].btY);
			}
		}
	}
}



void CBloodCastle::SendCastleDoorBlockInfo(int iBridgeIndex, bool bLive)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	char cTEMP_BUF[256];
	PMSG_SETMAPATTR_COUNT * lpMsg = (PMSG_SETMAPATTR_COUNT *)cTEMP_BUF;

	PHeadSetB((LPBYTE)lpMsg, 0x46, sizeof(PMSG_SETMAPATTR_COUNT)+sizeof(PMSG_SETMAPATTR)*6);
	PMSG_SETMAPATTR * lpMsgBody = (PMSG_SETMAPATTR *)&cTEMP_BUF[7];
	lpMsg->btType = 0;
	lpMsg->btCount = 3;
	lpMsg->btMapAttr = 4;
	(bLive)?lpMsg->btMapSetType=0:lpMsg->btMapSetType=1;

	lpMsgBody[0].btX = ::g_btCastleDoorMapXY[iBridgeIndex][0].btStartX;
	lpMsgBody[0].btY = ::g_btCastleDoorMapXY[iBridgeIndex][0].btStartY;
	lpMsgBody[1].btX   = ::g_btCastleDoorMapXY[iBridgeIndex][0].btEndX;
	lpMsgBody[1].btY   = ::g_btCastleDoorMapXY[iBridgeIndex][0].btEndY;

	lpMsgBody[2].btX = ::g_btCastleDoorMapXY[iBridgeIndex][1].btStartX;
	lpMsgBody[2].btY = ::g_btCastleDoorMapXY[iBridgeIndex][1].btStartY;
	lpMsgBody[3].btX   = ::g_btCastleDoorMapXY[iBridgeIndex][1].btEndX;
	lpMsgBody[3].btY   = ::g_btCastleDoorMapXY[iBridgeIndex][1].btEndY;

	lpMsgBody[4].btX = ::g_btCastleDoorMapXY[iBridgeIndex][2].btStartX;
	lpMsgBody[4].btY = ::g_btCastleDoorMapXY[iBridgeIndex][2].btStartY;
	lpMsgBody[5].btX   = ::g_btCastleDoorMapXY[iBridgeIndex][2].btEndX;
	lpMsgBody[5].btY   = ::g_btCastleDoorMapXY[iBridgeIndex][2].btEndY;

	for (int i= g_ConfigRead.server.GetObjectMaxMonster();i<g_ConfigRead.server.GetObjectMax();i++)
	{
		int iMapNumber = this->GetBridgeMapNumber(iBridgeIndex); //season3 add-on

		if ( gObj[i].MapNumber == iMapNumber ) //season3 changed
		{
			if ( gObj[i].Connected > PLAYER_LOGGED )
			{
				IOCP.DataSend(i, (LPBYTE)lpMsg, lpMsg->h.size);
			}
		}
	}
}



void CBloodCastle::SendNoticeMessage(int iBridgeIndex, char * lpszMSG)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	PMSG_NOTICE pNotice;

	TNotice::MakeNoticeMsg( &pNotice, 0, lpszMSG);

	for ( int i=0;i<MAX_BLOOD_CASTLE_SUB_BRIDGE;i++)
	{
		if ( this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex != -1 )
		{
			if ( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Connected > PLAYER_LOGGED )
			{
				if ( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleIndex != -1 )
				{
					if ( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleSubIndex != -1 )
					{
						IOCP.DataSend(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, (UCHAR*)&pNotice, pNotice.h.size);
					}
				}
			}
		}
	}
}



void CBloodCastle::SendNoticeScore(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	PMSG_NOTICE pNotice;
	pNotice.type = 0;

	for ( int i=0;i<MAX_BLOOD_CASTLE_SUB_BRIDGE;i++)
	{
		if ( this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex != -1 )
		{
			if ( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Connected > PLAYER_LOGGED )
			{
				if ( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleIndex != -1 )
				{
					if ( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleSubIndex != -1 )
					{
						TNotice::MakeNoticeMsgEx(&pNotice, 0, Lang.GetText(0,69), iBridgeIndex+1, this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iEXP);
						IOCP.DataSend(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, (UCHAR*)&pNotice, pNotice.h.size);
					}
				}
			}
		}
	}
}



void CBloodCastle::SendNoticeState(int iBridgeIndex, int iPlayState)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	PMSG_STATEBLOODCASTLE pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x9B, sizeof(PMSG_STATEBLOODCASTLE));
	pMsg.btPlayState = iPlayState;
	pMsg.wRemainSec = (int)this->m_BridgeData[iBridgeIndex].m_i64_BC_REMAIN_MSEC / 1000;

	if ( iPlayState == 4 )
	{
		pMsg.wMaxKillMonster = this->m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_MAX_COUNT;
		pMsg.wCurKillMonster = this->m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_KILL_COUNT;
	}
	else
	{
		pMsg.wMaxKillMonster = this->m_BridgeData[iBridgeIndex].m_iBC_MONSTER_MAX_COUNT;
		pMsg.wCurKillMonster = this->m_BridgeData[iBridgeIndex].m_iBC_MONSTER_KILL_COUNT;
	}

	pMsg.wUserHaveWeapon = this->m_BridgeData[iBridgeIndex].m_iBC_QUEST_ITEM_USER_INDEX;
	pMsg.btWeaponNum = this->m_BridgeData[iBridgeIndex].m_btBC_QUEST_ITEM_NUMBER + 1;

	for ( int i=0;i<MAX_BLOOD_CASTLE_SUB_BRIDGE;i++)
	{
		if ( this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex != -1 )
		{
			if ( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Connected > PLAYER_LOGGED )
			{
				if ( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleIndex != -1 )
				{
					if ( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleSubIndex != -1 )
					{
						IOCP.DataSend(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, (UCHAR *)&pMsg, pMsg.h.size);
					}
				}
			}
		}
	}
}





bool CBloodCastle::CheckUserBridgeMember(int iBridgeIndex, int iIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return false;
	}

	if ( ObjectMaxRange(iIndex) == FALSE )
	{
		return false;
	}

	for ( int i=0;i<MAX_BLOOD_CASTLE_SUB_BRIDGE;i++)
	{
		if ( this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex == iIndex )
		{
			return true;
		}
	}

	return false;
}



int  CBloodCastle::GetAliveUserTotalEXP(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return 0;
	}

	int iRET_EXP = 0;

	for ( int i=0;i<MAX_BLOOD_CASTLE_SUB_BRIDGE;i++)
	{
		if ( this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex != -1 )
		{
			if ( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Connected > PLAYER_LOGGED )
			{
				if ( BC_MAP_RANGE(gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].MapNumber) != FALSE )
				{
					iRET_EXP += this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iEXP;
				}
			}
		}
	}

	return iRET_EXP;
}


void CBloodCastle::SearchUserDeleteQuestItem(int iIndex)
{
	if ( gObjIsConnected(iIndex) == FALSE )
	{
		return;
	}

	for ( int x=0;x<INVENTORY_SIZE;x++)
	{
		if ( gObj[iIndex].pInventory[x].IsItem() == TRUE )
		{
			if ( gObj[iIndex].pInventory[x].m_Type == ITEMGET(13,19) )
			{
				int iLEVEL = gObj[iIndex].pInventory[x].m_Level;

				if ( BC_WEAPON_LEVEL_RANGE(iLEVEL) != FALSE )
				{
					::gObjInventoryItemSet(iIndex, x, -1);
					::gObjInventoryDeleteItem(iIndex, x);
					::GSProtocol.GCInventoryItemDeleteSend(iIndex, x, TRUE);

					g_Log.Add("[Blood Castle] Force (Account:%s, Name:%s) to Delete Angel King's Weapon (%d) [Serial:%I64d]",
						gObj[iIndex].AccountID, gObj[iIndex].Name, iLEVEL, gObj[iIndex].pInventory[x].m_Number);
				}
			}
		}
	}
}



void CBloodCastle::SearchUserDropQuestItem(int iIndex)
{
	if ( ObjectMaxRange(iIndex) == FALSE )
	{
		return;
	}

	if ( gObj[iIndex].Type != OBJ_USER || gObj[iIndex].Connected <= PLAYER_LOGGED )
	{
		return;
	}

	for ( int x=0;x<INVENTORY_SIZE;x++)
	{
		if ( gObj[iIndex].pInventory[x].IsItem() == TRUE )
		{
			if ( gObj[iIndex].pInventory[x].m_Type == ITEMGET(13,19) )
			{
				int iLEVEL = gObj[iIndex].pInventory[x].m_Level;

				if ( BC_WEAPON_LEVEL_RANGE(iLEVEL) != FALSE )
				{
					BYTE pMsg[6];
					pMsg[5] = x;
					pMsg[3] = gObj[iIndex].X;
					pMsg[4] = gObj[iIndex].Y;

					int iBC_INDEX = this->GetBridgeIndex(gObj[iIndex].MapNumber); //season3 add-on

					this->DropItemDirectly(iBC_INDEX, gObj[iIndex].m_Index, ITEMGET(13, 19), x); //season3 add-on

					if ( BC_MAP_RANGE(gObj[iIndex].MapNumber) != FALSE )
					{
						int iBC_INDEX = this->GetBridgeIndex(gObj[iIndex].MapNumber);

						if ( this->m_BridgeData[iBC_INDEX].m_nBC_QUESTITEM_SERIAL != -1 )
						{
							if ( this->m_BridgeData[iBC_INDEX].m_nBC_QUESTITEM_SERIAL == gObj[iIndex].pInventory[x].m_Number )
							{
								this->m_BridgeData[iBC_INDEX].m_iBC_QUEST_ITEM_USER_INDEX = -1;
							}
						}

						g_Log.Add("[Blood Castle] Force (Account:%s, Name:%s) to Drop Angel King's Weapon (%d) [Serial:%I64d-%I64d]",
							gObj[iIndex].AccountID, gObj[iIndex].Name, iLEVEL, this->m_BridgeData[iBC_INDEX].m_nBC_QUESTITEM_SERIAL,
							gObj[iIndex].pInventory[x].m_Number);

					}
					else
					{
						g_Log.Add("[Blood Castle] Force (Account:%s, Name:%s) to Drop Angel King's Weapon (%d) [Serial:%I64d]",
							gObj[iIndex].AccountID, gObj[iIndex].Name, iLEVEL,
							gObj[iIndex].pInventory[x].m_Number);
					}
				}
			}
		}
	}
}



void CBloodCastle::SetUserState(int iIndex, int iState)
{
	if ( ObjectMaxRange(iIndex) == FALSE )
	{
		return;
	}

	if ( gObj[iIndex].Type != OBJ_USER )
	{
		return;
	}

	int iBridgeIndex = gObj[iIndex].m_cBloodCastleIndex;
	int iBridgeSubIndex = gObj[iIndex].m_cBloodCastleSubIndex;

	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	if ( BC_SUB_BRIDGE_RANGE(iBridgeSubIndex) == FALSE )
	{
		return;
	}

	switch ( iState )
	{
		case 0:
			this->m_BridgeData[iBridgeIndex].m_UserData[iBridgeSubIndex].m_iUserState = 0;
			break;

		case 1:
			this->m_BridgeData[iBridgeIndex].m_UserData[iBridgeSubIndex].m_iUserState = 1;
			break;

		case 2:
			if ( gObj[iIndex].PartyNumber >= 0 )
			{
				for ( int i=0;i<MAX_BLOOD_CASTLE_SUB_BRIDGE;i++)
				{
					if ( i == iBridgeSubIndex )
					{
						continue;
					}

					if(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex == -1) //season 4 add-on
					{
						continue;
					}

					if ( gObj[iIndex].PartyNumber == gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].PartyNumber )
					{
						if ( BC_MAP_RANGE( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex ].MapNumber ) != FALSE )
						{
							this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState = 3;
						}
						else
						{
							this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState = 4;
						}
					}
				}
			}

			this->m_BridgeData[iBridgeIndex].m_UserData[iBridgeSubIndex].m_iUserState = 2;
			this->m_BridgeData[iBridgeIndex].m_iMISSION_SUCCESS = iBridgeSubIndex;
			break;

		case 3:
			this->m_BridgeData[iBridgeIndex].m_UserData[iBridgeSubIndex].m_iUserState = 3;
			break;

		case 4:
			this->m_BridgeData[iBridgeIndex].m_UserData[iBridgeSubIndex].m_iUserState = 4;
			break;
	}
}


struct ST_BC_SCORE	// size == 0x18
{
	char CharName[MAX_ACCOUNT_LEN];	// unk0
	int  iSCORE;	// unkC
	int  iEXP;	// unk10
	int  iZEN;	// unk14
	

};

#pragma pack(1)

struct GCS_BC_GIVE_REWARD	//Send[C1:93] - #error WARNING (Deathway) - Check with Packet of DevilSquareGround.h Ranking 
{
	PHEADB PHeader;
	bool bWinner;
	BYTE btType;
	ST_BC_SCORE m_stBCCharScore[MAX_BLOOD_CASTLE_SUB_BRIDGE+1];
};

#pragma pack()

// ///////////////////////////////////
// START REVIEW HERE

void CBloodCastle::GiveReward_Win(int iIndex, int iBridgeIndex)
{
	this->FixUsersPlayStateWin(iBridgeIndex);
	char szNOTIFY_MSG[256];
	int iREWARD_EXP = 0;
	int iREWARD_ZEN = 0;
	int iREWARD_SCR = 0;
	int iLEFT_TIME = (int)this->m_BridgeData[iBridgeIndex].m_i64_BC_REMAIN_MSEC / 1000;
	int iALIVE_PARTYCOUNT = this->GetWinnerPartyCompleteCount(iBridgeIndex);
	int iADD_PARTYPOINT = this->GetWinnerPartyCompletePoint(iBridgeIndex);

	g_Log.Add("[Blood Castle] (%d) Winner Party Point - Alive Party Member : %d, Alive Party Point : %d",
		iBridgeIndex+1, iALIVE_PARTYCOUNT, iADD_PARTYPOINT);

	if ( ObjectMaxRange(iIndex) == FALSE )
	{
		return;
	}

	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}
	
	if ( this->m_BridgeData[iBridgeIndex].m_bBC_REWARDED != false )
	{
		return;
	}

	if ( gObj[iIndex].Connected > PLAYER_LOGGED )
	{
		gObj[iIndex].Name[MAX_ACCOUNT_LEN] = 0;
		wsprintf(szNOTIFY_MSG, Lang.GetText(0,70), gObj[iIndex].Name);
	}
	else
	{
		wsprintf(szNOTIFY_MSG, Lang.GetText(0,71));
	}

	if ( this->m_BridgeData[iBridgeIndex].m_iMISSION_SUCCESS != -1 )
	{
		int iTOTAL_EXP = this->GetAliveUserTotalEXP(iBridgeIndex);
		g_Log.Add("[Blood Castle] (%d) Quest Complete - All Alive User's Total EXP:%d",
			iBridgeIndex+1, iTOTAL_EXP);

		for ( int i=0;i<MAX_BLOOD_CASTLE_SUB_BRIDGE;i++)
		{
			if ( this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex == -1 )
			{
				continue;
			}

			if ( gObj[ this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex ].Connected < PLAYER_PLAYING )
			{
				continue;
			}

			if ( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleIndex == -1 || gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleSubIndex == -1 ||  gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleIndex != iBridgeIndex )
			{
				continue;
			}

			PMSG_NOTICE pNotice;
			TNotice::MakeNoticeMsg(&pNotice, 10, szNOTIFY_MSG);
			TNotice::SetNoticeProperty(&pNotice, 10, _ARGB(255, 128, 149, 196), 1, 0, 20);
			TNotice::SendNoticeToUser(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, (TNotice*)&pNotice);

			int iADD_EXP = 0;

			if ( (gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].PartyNumber != -1 && gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].PartyNumber == this->m_BridgeData[iBridgeIndex].m_iExtraEXP_Kill_Door_Party) || this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex == this->m_BridgeData[iBridgeIndex].m_iExtraEXP_Kill_Door_Index)
			{
				iADD_EXP += ::g_iBC_Add_Exp[iBridgeIndex].unk0;
			}
			else
			{
				if ( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].PartyNumber != -1 )
				{
					if ( this->m_BridgeData[iBridgeIndex].m_UserData[i].m_bLiveWhenDoorBreak != false )
					{
						iADD_EXP += ::g_iBC_Add_Exp[iBridgeIndex].unk0 / 2;
					}
				}
			}

			if ( (gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].PartyNumber != -1 && gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].PartyNumber == this->m_BridgeData[iBridgeIndex].m_iExtraEXP_Kill_Statue_Party) || this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex == this->m_BridgeData[iBridgeIndex].m_iExtraEXP_Kill_Statue_Index)
			{
				iADD_EXP += ::g_iBC_Add_Exp[iBridgeIndex].unk4;
			}

			if ( (gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].PartyNumber != -1 && gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].PartyNumber == this->m_BridgeData[iBridgeIndex].m_iExtraEXP_Win_Quest_Party) || this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex == this->m_BridgeData[iBridgeIndex].m_iExtraEXP_Win_Quest_Index)
			{
				iADD_EXP += ::g_iBC_Add_Exp[iBridgeIndex].unk8;
			}

			switch ( this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState  )
			{
				case 0:
					iADD_EXP += (int)this->m_BridgeData[iBridgeIndex].m_i64_BC_REMAIN_MSEC / 1000 * ::g_iBC_Add_Exp[iBridgeIndex].unkC;
					iREWARD_EXP = this->CalcSendRewardEXP(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, iADD_EXP);
					iREWARD_ZEN = this->CalcSendRewardZEN(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, ::g_iQuestWinExpendZEN[iBridgeIndex].SpecialCharacter);
					iREWARD_SCR = ::g_iBC_EventScore[iBridgeIndex].unk0;
					this->SendRewardScore(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, iREWARD_SCR, iLEFT_TIME, 0);

					g_Log.Add("[Blood Castle] (%d) Quest Complete Rewarded (Account:%s, Name:%s [State:%d], Exp:%d, ExtExp:%d, Zen:%d, Score:%d, LeftTime:%d, MapNumber:%d)",
						iBridgeIndex+1, gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].AccountID,
						gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Name,
						0, gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_iBloodCastleEXP,
						iREWARD_EXP, iREWARD_ZEN, iREWARD_SCR, iLEFT_TIME, gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].MapNumber);
					break;

				case 1:
					iADD_EXP += (int)this->m_BridgeData[iBridgeIndex].m_i64_BC_REMAIN_MSEC / 1000 * ::g_iBC_Add_Exp[iBridgeIndex].unkC;
					iREWARD_EXP = this->CalcSendRewardEXP(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, iADD_EXP);
					iREWARD_ZEN = this->CalcSendRewardZEN(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, ::g_iQuestWinExpendZEN[iBridgeIndex].SpecialCharacter);
					iREWARD_SCR = ::g_iBC_EventScore[iBridgeIndex].unk4;
					this->SendRewardScore(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, iREWARD_SCR, iLEFT_TIME, 0);

					g_Log.Add("[Blood Castle] (%d) Quest Complete Rewarded (Account:%s, Name:%s [State:%d], Exp:%d, ExtExp:%d, Zen:%d, Score:%d, LeftTime:%d, MapNumber:%d)",
						iBridgeIndex+1, gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].AccountID,
						gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Name,
						1, gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_iBloodCastleEXP,
						iREWARD_EXP, iREWARD_ZEN, iREWARD_SCR, iLEFT_TIME, gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].MapNumber);
					break;

				case 2:
					iADD_EXP += (int)this->m_BridgeData[iBridgeIndex].m_i64_BC_REMAIN_MSEC / 1000 * ::g_iBC_Add_Exp[iBridgeIndex].unkC;
					iREWARD_EXP = this->CalcSendRewardEXP(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, iADD_EXP);
					iREWARD_ZEN = this->CalcSendRewardZEN(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, ::g_iQuestWinExpendZEN[iBridgeIndex].NormalCharacter);
					iREWARD_SCR = ::g_iBC_EventScore[iBridgeIndex].unk8;
					iREWARD_SCR += iADD_PARTYPOINT;

					if ( BC_MAP_RANGE(gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].MapNumber ) != FALSE )
					{
						this->DropReward(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex);
					}

					this->SendRewardScore(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, iREWARD_SCR, iLEFT_TIME, iALIVE_PARTYCOUNT);

					g_Log.Add("[Blood Castle] (%d) Quest Complete Rewarded (Account:%s, Name:%s [State:%d], Exp:%d, ExtExp:%d, Zen:%d, Score:%d, LeftTime:%d, MapNumber:%d)",
						iBridgeIndex+1, gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].AccountID,
						gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Name,
						2, gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_iBloodCastleEXP,
						iREWARD_EXP, iREWARD_ZEN, iREWARD_SCR, iLEFT_TIME, gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].MapNumber);
					break;

				case 3:
					iADD_EXP += (int)this->m_BridgeData[iBridgeIndex].m_i64_BC_REMAIN_MSEC / 1000 * ::g_iBC_Add_Exp[iBridgeIndex].unkC;
					iREWARD_EXP = this->CalcSendRewardEXP(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, iADD_EXP);
					iREWARD_ZEN = this->CalcSendRewardZEN(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, ::g_iQuestWinExpendZEN[iBridgeIndex].NormalCharacter);
					iREWARD_SCR = ::g_iBC_EventScore[iBridgeIndex].unkC;
					iREWARD_SCR += iADD_PARTYPOINT;

					if ( BC_MAP_RANGE(gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].MapNumber ) != FALSE )
					{
						this->DropReward(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex);
					}

					this->SendRewardScore(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, iREWARD_SCR, iLEFT_TIME, iALIVE_PARTYCOUNT);

					g_Log.Add("[Blood Castle] (%d) Quest Complete Rewarded (Account:%s, Name:%s [State:%d], Exp:%d, ExtExp:%d, Zen:%d, Score:%d, LeftTime:%d, MapNumber:%d)",
						iBridgeIndex+1, gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].AccountID,
						gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Name,
						3, gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_iBloodCastleEXP,
						iREWARD_EXP, iREWARD_ZEN, iREWARD_SCR, iLEFT_TIME, gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].MapNumber);
					break;

				case 4:
					iADD_EXP += (int)this->m_BridgeData[iBridgeIndex].m_i64_BC_REMAIN_MSEC / 1000 * ::g_iBC_Add_Exp[iBridgeIndex].unkC;
					iREWARD_EXP = this->CalcSendRewardEXP(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, iADD_EXP);
					iREWARD_ZEN = this->CalcSendRewardZEN(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, ::g_iQuestWinExpendZEN[iBridgeIndex].NormalCharacter);
					iREWARD_SCR = ::g_iBC_EventScore[iBridgeIndex].unk10;
					this->SendRewardScore(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, iREWARD_SCR, iLEFT_TIME, iALIVE_PARTYCOUNT);

					g_Log.Add("[Blood Castle] (%d) Quest Complete Rewarded (Account:%s, Name:%s [State:%d], Exp:%d, ExtExp:%d, Zen:%d, Score:%d, LeftTime:%d, MapNumber:%d)",
						iBridgeIndex+1, gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].AccountID,
						gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Name,
						4, gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_iBloodCastleEXP,
						iREWARD_EXP, iREWARD_ZEN, iREWARD_SCR, iLEFT_TIME, gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].MapNumber);
					break;
			}

			GCS_BC_GIVE_REWARD pMsg;

			pMsg.bWinner = true;
			pMsg.btType = -1;
			memcpy(pMsg.m_stBCCharScore[0].CharName , gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Name, MAX_ACCOUNT_LEN);
			pMsg.m_stBCCharScore[0].iEXP = iREWARD_EXP;
			pMsg.m_stBCCharScore[0].iZEN = iREWARD_ZEN;
			pMsg.m_stBCCharScore[0].iSCORE = iREWARD_SCR;
			PHeadSetB((LPBYTE)&pMsg.PHeader, 0x93, sizeof(GCS_BC_GIVE_REWARD) - (sizeof(ST_BC_SCORE) * (MAX_BLOOD_CASTLE_SUB_BRIDGE -1)) );

			IOCP.DataSend(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, (UCHAR*)&pMsg, pMsg.PHeader.uSize);
			g_QuestExpProgMng.ChkUserQuestTypeEventMap(QUESTEXP_ASK_BLOODCASTLE_CLEAR, &gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex], gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleIndex, 0);
		}

		this->m_BridgeData[iBridgeIndex].m_bBC_REWARDED = true;
	}
}



void CBloodCastle::GiveReward_Fail(int iBridgeIndex)
{
	this->FixUsersPlayStateFail(iBridgeIndex);

	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	int iUserWhoGotUltimateWeapon = -1;
	iUserWhoGotUltimateWeapon = this->GetWhoGotUltimateWeapon(iBridgeIndex);

	if ( iUserWhoGotUltimateWeapon != -1 )
	{
		if ( ObjectMaxRange(iUserWhoGotUltimateWeapon) != FALSE )
		{
			g_Log.Add("[Blood Castle] (%d) (Account:%s, Name:%s) got Ultimate Weapon (%d) but Failed - Time Out",
				iBridgeIndex+1, gObj[iUserWhoGotUltimateWeapon].AccountID, gObj[iUserWhoGotUltimateWeapon].Name,
				this->m_BridgeData[iBridgeIndex].m_btBC_QUEST_ITEM_NUMBER);
		}
	}

	for ( int i=0;i<MAX_BLOOD_CASTLE_SUB_BRIDGE;i++)
	{
		if ( this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex == -1 )
		{
			continue;
		}

		if ( gObj[ this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex ].Connected < PLAYER_PLAYING )
		{
			continue;
		}

		if ( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleIndex == -1 || gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleSubIndex == -1 )
		{
			continue;
		}	

		int iADD_EXP = 0;

		if ( (gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].PartyNumber != -1 && gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].PartyNumber == this->m_BridgeData[iBridgeIndex].m_iExtraEXP_Kill_Door_Party) || this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex == this->m_BridgeData[iBridgeIndex].m_iExtraEXP_Kill_Door_Index)
		{
			iADD_EXP += ::g_iBC_Add_Exp[iBridgeIndex].unk0;
		}
		else
		{
			if ( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].PartyNumber != -1 )
			{
				if ( this->m_BridgeData[iBridgeIndex].m_UserData[i].m_bLiveWhenDoorBreak != false )
				{
					iADD_EXP += ::g_iBC_Add_Exp[iBridgeIndex].unk0 / 2;
				}
			}
		}

		if ( (gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].PartyNumber != -1 && gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].PartyNumber == this->m_BridgeData[iBridgeIndex].m_iExtraEXP_Kill_Statue_Party) || this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex == this->m_BridgeData[iBridgeIndex].m_iExtraEXP_Kill_Statue_Index)
		{
			iADD_EXP += ::g_iBC_Add_Exp[iBridgeIndex].unk4;
		}

		this->CalcSendRewardEXP(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, iADD_EXP);
		this->SendRewardScore(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, ::g_iBC_EventScore_Fail[iBridgeIndex], 0, 0);

		g_Log.Add("[Blood Castle] (%d) Quest Fail Rewarded (Account:%s, Name:%s, Score:%d, ExtEXP:%d, LeftTime:%d)",
			iBridgeIndex+1, gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].AccountID,
			gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Name,
			::g_iBC_EventScore_Fail[iBridgeIndex], iADD_EXP, 0);

		GCS_BC_GIVE_REWARD pMsg;

		pMsg.bWinner = false;
		pMsg.btType = -1;
		memcpy(pMsg.m_stBCCharScore[0].CharName, gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Name, MAX_ACCOUNT_LEN);
		pMsg.m_stBCCharScore[0].iEXP = iADD_EXP;
		pMsg.m_stBCCharScore[0].iZEN = 0;
		pMsg.m_stBCCharScore[0].iSCORE = g_iBC_EventScore_Fail[iBridgeIndex];
		PHeadSetB((LPBYTE)&pMsg.PHeader, 0x93, sizeof(GCS_BC_GIVE_REWARD) - (sizeof(ST_BC_SCORE) * (MAX_BLOOD_CASTLE_SUB_BRIDGE -1)) );

		if ( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleIndex != -1 && gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleSubIndex != -1 )
		{
			IOCP.DataSend(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, (UCHAR*)&pMsg, pMsg.PHeader.uSize);			

			g_Log.Add("[Blood Castle] (%d) Send User Quest Fail Message (Account:%s, Name:%s [State:%d], MapNumber:%d)",
				iBridgeIndex+1, gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].AccountID,
				gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Name, 
				this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState,
				gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].MapNumber);
		}	
	}
}



int  CBloodCastle::CalcSendRewardEXP(int iIndex, int iEXP)
{
	if ( iEXP <= 0 )
	{
		return 0;
	}

	UINT64 iRET_EXP = 0;
	UINT64 iCAL_EXP = iEXP;

	if ( g_CrywolfSync.GetOccupationState() == 1 && g_CrywolfSync.GetApplyPenalty() == TRUE )
	{
		iCAL_EXP = iCAL_EXP * g_CrywolfSync.GetGettingExpPenaltyRate() / 100; //season 2.5 changed
	}

	if ( ObjectMaxRange(iIndex) == FALSE )
	{
		return 0;
	}

	if ( gObj[iIndex].Connected != PLAYER_PLAYING )
	{
		return 0;
	}

	iRET_EXP = iCAL_EXP;

	if ( gObj[iIndex].Type == OBJ_USER )
	{
		while ( iCAL_EXP > 0 )
		{
			if ( iCAL_EXP > 0 )
			{
				CheckItemOptForGetExpExRenewal(&gObj[iIndex], 0, iCAL_EXP, 0, TRUE);

				iCAL_EXP = (iCAL_EXP) * this->m_BridgeData[gObj[iIndex].m_cBloodCastleIndex].m_iBC_REWARD_EXP; //season 4.5 add-on
				iRET_EXP = (iCAL_EXP);

				iCAL_EXP = this->LevelUp(iIndex, iCAL_EXP);
			}
		}

		if(g_MasterLevelSkillTreeSystem.IsMasterLevelUser(&gObj[iIndex]) == false) //season3 add-on
		{
			GSProtocol.GCKillPlayerMasterExpSend(iIndex, (WORD)-1, iRET_EXP, 0, 0);
		}
	}

	return iRET_EXP;
}


int  CBloodCastle::CalcSendRewardZEN(int iIndex, int iZEN)
{
	if ( iZEN <= 0 )
	{
		return 0;
	}

	int iRET_ZEN = 0;

	if ( ObjectMaxRange(iIndex) == FALSE )
	{
		return 0;
	}

	if ( gObj[iIndex].Connected != PLAYER_PLAYING )
	{
		return 0;
	}

	if ( gObjCheckMaxZen(iIndex, iZEN) == FALSE )
	{
		iRET_ZEN = MAX_ZEN - gObj[iIndex].m_PlayerData->Money;
		gObj[iIndex].m_PlayerData->Money += iRET_ZEN;

		return iRET_ZEN;
	}

	gObj[iIndex].m_PlayerData->Money += iZEN;
	iRET_ZEN = iZEN;
	GSProtocol.GCMoneySend(iIndex, gObj[iIndex].m_PlayerData->Money);

	return iRET_ZEN;
}

void CBloodCastle::DropReward(int iIndex)
{
	if ( ObjectMaxRange(iIndex) == FALSE )
	{
		return;
	}

	if ( ObjectMaxRange(iIndex) == FALSE )
	{
		return;
	}

	switch(gObj[iIndex].m_cBloodCastleIndex)
	{
		case 0:
			g_BagManager.UseBag_GremoryCase(iIndex, BAG_EVENT, EVENTBAG_BC1, iIndex, GC_STORAGE_CHARACTER, GC_REWARD_BLOOD_CASTLE, 30);
			break;
		case 1:
			g_BagManager.UseBag_GremoryCase(iIndex, BAG_EVENT, EVENTBAG_BC2, iIndex, GC_STORAGE_CHARACTER, GC_REWARD_BLOOD_CASTLE, 30);
			break;
		case 2:
			g_BagManager.UseBag_GremoryCase(iIndex, BAG_EVENT, EVENTBAG_BC3, iIndex, GC_STORAGE_CHARACTER, GC_REWARD_BLOOD_CASTLE, 30);
			break;
		case 3:
			g_BagManager.UseBag_GremoryCase(iIndex, BAG_EVENT, EVENTBAG_BC4, iIndex, GC_STORAGE_CHARACTER, GC_REWARD_BLOOD_CASTLE, 30);
			break;
		case 4:
			g_BagManager.UseBag_GremoryCase(iIndex, BAG_EVENT, EVENTBAG_BC5, iIndex, GC_STORAGE_CHARACTER, GC_REWARD_BLOOD_CASTLE, 30);
			break;
		case 5:
			g_BagManager.UseBag_GremoryCase(iIndex, BAG_EVENT, EVENTBAG_BC6, iIndex, GC_STORAGE_CHARACTER, GC_REWARD_BLOOD_CASTLE, 30);
			break;
		case 6:
			g_BagManager.UseBag_GremoryCase(iIndex, BAG_EVENT, EVENTBAG_BC7, iIndex, GC_STORAGE_CHARACTER, GC_REWARD_BLOOD_CASTLE, 30);
			break;
		case 7:
			g_BagManager.UseBag_GremoryCase(iIndex, BAG_EVENT, EVENTBAG_BC8, iIndex, GC_STORAGE_CHARACTER, GC_REWARD_BLOOD_CASTLE, 30);
			break;

	}

	g_CashShop.AddCoin(&gObj[iIndex], EVENT_BC);
}


struct PMSG_ANS_BLOODCASTLESCORE_5TH
{
	PBMSG_HEAD2 h;	// C1:0D
	char AccountID[10];	// 3
	char GameID[10];	// D
	int ServerCode;	// 18
	int Score;	// 1C
	int Class;	// 20
	int BridgeNum;	// 24
	int iLeftTime;	// 28
	int iAlivePartyCount;	// 2C
};



void CBloodCastle::SendRewardScore(int iIndex, int iSCORE, int iLeftTime, int iAlivePartyCount)
{
	if ( ObjectMaxRange(iIndex) == FALSE )
	{
		return;
	}

	PMSG_ANS_BLOODCASTLESCORE_5TH pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.headcode = 0xBD;
	pMsg.h.subcode = 0x03;
	pMsg.h.size = sizeof(pMsg);
	pMsg.Score = iSCORE;
	pMsg.BridgeNum = gObj[iIndex].m_cBloodCastleIndex;
	pMsg.Class = gObj[iIndex].Class;
	pMsg.ServerCode = g_ConfigRead.server.GetGameServerCode();
	pMsg.iLeftTime = iLeftTime;
	memcpy(pMsg.AccountID, gObj[iIndex].AccountID, MAX_ACCOUNT_LEN);
	memcpy(pMsg.GameID, gObj[iIndex].Name, MAX_ACCOUNT_LEN);
	pMsg.iAlivePartyCount = iAlivePartyCount;

	wsDataCli.DataSend(reinterpret_cast<char *>(&pMsg), pMsg.h.size);
}




void CBloodCastle::SendBridgeAnyMsg(BYTE * lpMsg, int iSize, int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	for ( int i=0;i<MAX_BLOOD_CASTLE_SUB_BRIDGE;i++)
	{
		if ( this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex != -1 )
		{
			if ( gObj[ this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex ].Connected == PLAYER_PLAYING )
			{
				if ( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleIndex != -1 && gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleSubIndex != -1 )
				{
					IOCP.DataSend(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, lpMsg, iSize);
				}
			}
		}
	}

}



void CBloodCastle::SendAllUserAnyMsg(BYTE * lpMsg, int iSize)
{
	for ( int i=g_ConfigRead.server.GetObjectStartUserIndex();i<g_ConfigRead.server.GetObjectMax();i++)
	{
		if ( gObj[i].Connected == PLAYER_PLAYING )
		{
			if ( gObj[i].Type == OBJ_USER )
			{
				if ( DG_MAP_RANGE(gObj[i].MapNumber) == FALSE )
				{
					IOCP.DataSend(i, lpMsg, iSize);
				}
			}
		}
	}
}




void CBloodCastle::SetMonsterKillCount(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	int iTOT_USER_COUNT = 0;
	int iLIVE_USER_COUNT = 0;
	int iKILL_USER_COUNT = 0;

	for ( int i=0;i<MAX_BLOOD_CASTLE_SUB_BRIDGE;i++)
	{
		if ( this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex != -1 )
		{
			iTOT_USER_COUNT++;

			if ( BC_MAP_RANGE(gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].MapNumber) != FALSE )
			{
				if ( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Connected > PLAYER_LOGGED )
				{
					iLIVE_USER_COUNT++;
				}
				else
				{
					iKILL_USER_COUNT++;
				}
			}
			else
			{
				iKILL_USER_COUNT++;
			}

		}
	}

	this->m_BridgeData[iBridgeIndex].m_iBC_MONSTER_MAX_COUNT = iLIVE_USER_COUNT * 40;
	this->m_BridgeData[iBridgeIndex].m_iBC_MONSTER_KILL_COUNT = 0;

	g_Log.Add("[Blood Castle] (%d) Monster Kill Count Set - Monster:%d, Current Monster:%d, USER TOT:%d, LIVE:%d, DEAD:%d",
		iBridgeIndex+1, this->m_BridgeData[iBridgeIndex].m_iBC_MONSTER_MAX_COUNT,
		this->m_BridgeData[iBridgeIndex].m_iBC_MONSTER_KILL_COUNT, 
		iTOT_USER_COUNT, iLIVE_USER_COUNT, iKILL_USER_COUNT);
}





bool CBloodCastle::CheckMonsterKillCount(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return false;
	}

	if( this->m_BridgeData[iBridgeIndex].m_iBC_MONSTER_KILL_COUNT >= this->m_BridgeData[iBridgeIndex].m_iBC_MONSTER_MAX_COUNT )
	{
		g_Log.Add("[Blood Castle][Bug Tracker] (%d) All Monster Kill Success(Max Kill Count:%d / Kill Count:%d)",
			iBridgeIndex+1, this->m_BridgeData[iBridgeIndex].m_iBC_MONSTER_MAX_COUNT,
			this->m_BridgeData[iBridgeIndex].m_iBC_MONSTER_KILL_COUNT);

		return true;
	}

	return false;
}



bool CBloodCastle::CheckMonsterKillSuccess(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return false;
	}

	return this->m_BridgeData[iBridgeIndex].m_bBC_MONSTER_KILL_COMPLETE;
}



bool CBloodCastle::CheckBossKillCount(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return false;
	}

	if ( this->m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_KILL_COUNT >= this->m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_MAX_COUNT )
	{
		g_Log.Add("[Blood Castle][Bug Tracker] (%d) All Boss Monster Kill Success(Max Boss Kill Count:%d / Boss Kill Count:%d)",
			iBridgeIndex+1, this->m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_MAX_COUNT,
			this->m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_KILL_COUNT);

		return true;
	}

	return false;
}



bool CBloodCastle::CheckBossKillSuccess(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return false;
	}

	return this->m_BridgeData[iBridgeIndex].m_bBC_BOSS_MONSTER_KILL_COMPLETE;
}



bool CBloodCastle::CheckEveryUserDie(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return false;
	}

	bool bRET_VAL = true;

	for ( int i=0;i<MAX_BLOOD_CASTLE_SUB_BRIDGE;i++)
	{
		if ( this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex != -1 )
		{
			if ( ObjectMaxRange(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex) == FALSE )
			{
				g_Log.Add("error-L3 : [Blood Castle] (%d) User Out of Bound UserIndex:%d, SubIndex:%d",
					iBridgeIndex+1, this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, i);
				
				continue;
			}

			if ( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].MapNumber == this->GetBridgeMapNumber(iBridgeIndex) ) //season3 changed
			{
				if ( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Connected > PLAYER_LOGGED )
				{
					bRET_VAL = false;
				}
				else
				{
					g_Log.Add("error-L3 : [Blood Castle] (%d) Connection Closed UserIndex:%d, SubIndex:%d", iBridgeIndex+1, this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, i);
				}
			}

			else
			{
				if ( this->m_BridgeData[iBridgeIndex].m_UserData[i].m_bSendQuitMsg == false )
				{
					this->m_BridgeData[iBridgeIndex].m_UserData[i].m_bSendQuitMsg = true;

					PMSG_STATEBLOODCASTLE pMsg;

					PHeadSetB((LPBYTE)&pMsg, 0x9B, sizeof(PMSG_STATEBLOODCASTLE));
					pMsg.btPlayState = 2;
					pMsg.wRemainSec = 0;
					pMsg.wMaxKillMonster = 0;
					pMsg.wCurKillMonster = 0;
					pMsg.wUserHaveWeapon = 0;
					pMsg.btWeaponNum = -1;

					IOCP.DataSend(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, (UCHAR*)&pMsg, pMsg.h.size);
				}
			}
		}
	}

	return bRET_VAL;
}



bool CBloodCastle::CheckAngelKingExist(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return false;
	}

	bool bRET_VAL = false;

	if ( this->m_BCMP_AngelKing[iBridgeIndex].m_Type == (WORD)-1 )
	{
		g_Log.Add("[Blood Castle] (%d) Angel King's Monster Position Doesn't Exist",
			iBridgeIndex+1);

		return false;
	}

	for ( int i=0;i<g_ConfigRead.server.GetObjectStartUserIndex();i++)
	{
		if ( gObj[i].Connected == PLAYER_PLAYING && gObj[i].Type == OBJ_NPC )
		{
			if ( gObj[i].Class == 232 )
			{
				if (gObj[i].MapNumber == this->GetBridgeMapNumber(iBridgeIndex) ) //season3 changed
				{
					bRET_VAL = true;
					break;
				}
			}
		}
	}

	if ( bRET_VAL == false )
	{
		int iIndex = gObjAddMonster(this->GetBridgeMapNumber(iBridgeIndex)); //season3 changed

		if ( iIndex >= 0 )
		{
			gObj[iIndex].X = this->m_BCMP_AngelKing[iBridgeIndex].m_X;
			gObj[iIndex].Y = this->m_BCMP_AngelKing[iBridgeIndex].m_Y;
			gObj[iIndex].MapNumber = this->m_BCMP_AngelKing[iBridgeIndex].m_MapNumber;
			gObj[iIndex].TX = gObj[iIndex].X;
			gObj[iIndex].TY = gObj[iIndex].Y;
			gObj[iIndex].m_OldX = gObj[iIndex].X;
			gObj[iIndex].m_OldY = gObj[iIndex].Y;
			gObj[iIndex].Dir = this->m_BCMP_AngelKing[iBridgeIndex].m_Dir;
			gObj[iIndex].StartX = gObj[iIndex].X;
			gObj[iIndex].StartY = gObj[iIndex].Y;
			gObjSetMonster(iIndex, 232);
			gObj[iIndex].m_cBloodCastleIndex = this->GetBridgeIndex(gObj[iIndex].MapNumber);
			bRET_VAL = true;
		}
		else
		{
			g_Log.Add("[Blood Castle] (%d) Angel King - gObjAdd() failed (no space to add monster, result:%d)",
				iBridgeIndex+1, iIndex);

			return false;
		}
	}

	return bRET_VAL;
}




int  CBloodCastle::GetWhoGotUltimateWeapon(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return -1;
	}

	int iBridgeUserIndex = -1;

	for ( int i=0;i<MAX_BLOOD_CASTLE_SUB_BRIDGE;i++)
	{
		int iIndex = this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex;

		if ( ObjectMaxRange(iIndex) == FALSE )
		{
			continue;
		}

		if( gObj[iIndex].Type != OBJ_USER || gObj[iIndex].Connected <= PLAYER_LOGGED )
		{
			continue;
		}

		if ( BC_MAP_RANGE( gObj[iIndex].MapNumber ) == FALSE )
		{
			continue;
		}

		for ( int x=0;x<INVENTORY_SIZE;x++)
		{
			if ( gObj[iIndex].pInventory[x].IsItem() == TRUE )
			{
				if ( gObj[iIndex].pInventory[x].m_Type == ITEMGET(13,19) )
				{
					int iLEVEL = gObj[iIndex].pInventory[x].m_Level;

					if ( BC_WEAPON_LEVEL_RANGE(iLEVEL) != FALSE )
					{
						if ( this->m_BridgeData[iBridgeIndex].m_nBC_QUESTITEM_SERIAL != -1 )
						{
							if ( this->m_BridgeData[iBridgeIndex].m_nBC_QUESTITEM_SERIAL == gObj[iIndex].pInventory[x].m_Number )
							{
								iBridgeUserIndex = iIndex;
								break;
							}
						}
					}
				}
			}
		}

	}

	return iBridgeUserIndex;
}




int  CBloodCastle::GetCurrentLiveUserCount(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return -1;
	}

	int iRetLiveUserCount = 0;

	for ( int i=0;i<MAX_BLOOD_CASTLE_SUB_BRIDGE;i++)
	{
		int iIndex = this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex;

		if ( ObjectMaxRange(iIndex) == FALSE )
		{
			continue;
		}

		if ( BC_MAP_RANGE( gObj[iIndex].MapNumber ) == FALSE )
		{
			continue;
		}

		if( gObj[iIndex].Type != OBJ_USER  )
		{
			continue;
		}

		if ( gObj[iIndex].Connected > PLAYER_LOGGED )
		{
			iRetLiveUserCount++;
		}
	}

	return iRetLiveUserCount;
}



BOOL CBloodCastle::DropItemDirectly(int iBridgeIndex, int iIndex, int iItemType, int iItemPos)
{
	if ( BC_MAP_RANGE(this->GetBridgeMapNumber(iBridgeIndex)) == FALSE )
	{
		g_Log.Add("[Blood Castle] [%s][%s] DropItemDirectly() failed (iBridgeIndex wrong - %d)",	gObj[iIndex].AccountID, gObj[iIndex].Name, iBridgeIndex);
		return FALSE;
	}

	PMSG_ITEMTHROW_RESULT pResult;

	PHeadSetB((LPBYTE)&pResult, 0x23, sizeof(pResult));
	pResult.Result = TRUE;
	pResult.Ipos = iItemPos;
	int map_num = gObj[iIndex].MapNumber;
	int type = gObj[iIndex].pInventory[iItemPos].m_Type;
	int level = gObj[iIndex].pInventory[iItemPos].m_Level;
	float dur = gObj[iIndex].pInventory[iItemPos].m_Durability;
	BOOL ret = gObj[iIndex].pInventory[iItemPos].IsItem();
	BYTE Option1 = gObj[iIndex].pInventory[iItemPos].m_Option1;
	BYTE Option2 = gObj[iIndex].pInventory[iItemPos].m_Option2;
	BYTE Option3 = gObj[iIndex].pInventory[iItemPos].m_Option3;
	BYTE NOption = gObj[iIndex].pInventory[iItemPos].m_NewOption;
	UINT64 s_num = gObj[iIndex].pInventory[iItemPos].m_Number;
	BYTE ItemExOption = g_kJewelOfHarmonySystem.GetItemStrengthenOption(&gObj[iIndex].pInventory[iItemPos]);
	BYTE ItemExLevel = g_kJewelOfHarmonySystem.GetItemOptionLevel(&gObj[iIndex].pInventory[iItemPos]);

	BYTE NewOption[MAX_EXOPTION_SIZE];
	::ItemIsBufExOption(NewOption, &gObj[iIndex].pInventory[iItemPos]);
	int PetLevel = gObj[iIndex].pInventory[iItemPos].m_PetItem_Level;
	UINT64 PetExp = gObj[iIndex].pInventory[iItemPos].m_PetItem_Exp;
	BYTE SOption = gObj[iIndex].pInventory[iItemPos].m_SetOption;
	BYTE ItemEffectEx = gObj[iIndex].pInventory[iItemPos].m_ItemOptionEx;
	UINT64 item_number = gObj[iIndex].pInventory[iItemPos].m_Number;
	char szItemName[50] = "Item";
	int aAntiLootIndex = -1;

	//Season 4 add-on
	BYTE SocketOption[5]; //
	SocketOption[0] = 0xFF;
	SocketOption[1] = 0xFF;
	SocketOption[2] = 0xFF;
	SocketOption[3] = 0xFF;
	SocketOption[4] = 0xFF;

	BYTE SocketIndex = 0; //

	g_SocketOptionSystem.GetSocketOption(&gObj[iIndex].pInventory[iItemPos], SocketOption, SocketIndex);

	if ( MapC[map_num].ItemDrop(type, level, dur, gObj[iIndex].X, gObj[iIndex].Y,Option1, Option2, Option3, NOption, SOption, item_number, aAntiLootIndex, PetLevel, PetExp, ItemEffectEx, SocketOption, SocketIndex, 0) == TRUE )
	{
		::gObjInventoryDeleteItem(iIndex, iItemPos);
		pResult.Result = TRUE;

		g_Log.Add("[%s][%s][%d]%d/%d Throw away Serial:%I64d [%s][%d][%d][%d][%d][%d] dur:[%d] Ex:[%d,%d,%d,%d,%d,%d,%d] Set:[%d] 380:[%d] HO:[%d,%d] E:[%d]", gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].MapNumber,
			gObj[iIndex].X, gObj[iIndex].Y, s_num, szItemName, type, level, Option1, Option2, Option3, (int)dur, NewOption[0],
			NewOption[1], NewOption[2], NewOption[3], NewOption[4], NewOption[5], NewOption[6], SOption,
			ItemEffectEx>>7, ItemExOption, ItemExLevel);
	}
	else
	{
		pResult.Result = FALSE;
	}

	IOCP.DataSend(iIndex, (UCHAR*)&pResult, pResult.h.size);

	if ( pResult.Result == TRUE )
	{
		if ( iItemPos < INVENTORY_BAG_START )
		{
			if ( iItemPos== 10 || iItemPos == 11 )
			{
				gObjUseSkill.SkillChangeUse(iIndex);
			}

			::gObjMakePreviewCharSet(iIndex);

			PMSG_USEREQUIPMENTCHANGED pMsg;

			PHeadSetB((LPBYTE)&pMsg, 0x25, sizeof(PMSG_USEREQUIPMENTCHANGED));
			pMsg.NumberH = SET_NUMBERH(iIndex);
			pMsg.NumberL = SET_NUMBERL(iIndex);
			ItemByteConvert(pMsg.ItemInfo, gObj[iIndex].pInventory[iItemPos]);
			pMsg.ItemInfo[I_OPTION] = iItemPos * 16; // iItemPos << 16;
			pMsg.ItemInfo[I_OPTION] |= LevelSmallConvert(iIndex, iItemPos) & 0x0F;
			pMsg.Element = gObj[iIndex].m_iPentagramMainAttribute;

			IOCP.DataSend(iIndex, (UCHAR*)&pMsg, pMsg.h.size);
			GSProtocol.MsgSendV2(&gObj[iIndex], (UCHAR*)&pMsg, pMsg.h.size);
		}
	}

	return (pResult.Result);
}



bool CBloodCastle::CheckUserHaveUlimateWeapon(int iIndex)
{
	if ( ObjectMaxRange(iIndex ) == FALSE )
	{
		return false;
	}

	if ( gObj[iIndex].Type != OBJ_USER || gObj[iIndex].Connected <= PLAYER_LOGGED )
	{
		return false;
	}

	bool bRetVal = false;

	for ( int x=0;x<INVENTORY_SIZE;x++)
	{
		if ( gObj[iIndex].pInventory[x].IsItem() == TRUE )
		{
			if ( gObj[iIndex].pInventory[x].m_Type == ITEMGET(0,19) || gObj[iIndex].pInventory[x].m_Type == ITEMGET(5,10) || gObj[iIndex].pInventory[x].m_Type == ITEMGET(4,18) )
			{
				bRetVal = true;
				break;
			}
		}
	}

	return bRetVal;
}

bool CBloodCastle::CheckWinnerExist(int iBridgeIndex)
{
	if ( !BC_BRIDGE_RANGE(iBridgeIndex))
		return false;

	if ( this->m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX != -1 )
		return true;

	return false;
}


bool CBloodCastle::CheckWinnerValid(int iBridgeIndex)
{
	if ( !BC_BRIDGE_RANGE(iBridgeIndex))
	{
		g_Log.Add("[Blood Castle] (%d) CBloodCastle::CheckWinnerValid() - !CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT)",
			iBridgeIndex+1);

		return false;
	}

	if ( this->m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX == -1 )
	{
		g_Log.Add("[Blood Castle] (%d) CBloodCastle::CheckWinnerValid() - m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX == -1",
			iBridgeIndex+1);

		return false;
	}

	if ( !gObjIsConnected(this->m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX) )
	{
		g_Log.Add("[Blood Castle] (%d) CBloodCastle::CheckWinnerValid() - !gObjIsConnected(m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX)",
			iBridgeIndex+1);

		return false;
	}

	if ( gObj[this->m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX].m_cBloodCastleIndex == -1 || gObj[this->m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX].m_cBloodCastleSubIndex == -1 || gObj[this->m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX].m_cBloodCastleIndex != iBridgeIndex )
	{
		g_Log.Add("[Blood Castle] (%d) CBloodCastle::CheckWinnerValid() - (gObj[m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX].m_cBloodCastleIndex == -1) ...",
			iBridgeIndex+1);

		return false;
	}

	if ( !BC_MAP_RANGE(gObj[this->m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX].MapNumber) )
	{
		g_Log.Add("[Blood Castle] (%d) CBloodCastle::CheckWinnerValid() - !CHECK_BLOODCASTLE(gObj[m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX].MapNumber)",
			iBridgeIndex+1);

		return false;
	}

	return true;
}
	


bool CBloodCastle::CheckUserWinnerParty(int iBridgeIndex, int iIndex)
{
	if ( !BC_BRIDGE_RANGE(iBridgeIndex))
		return false;

	if ( gObjIsConnected(this->m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX) == FALSE )
		return false;

	if ( gObjIsConnected(iIndex) == FALSE )
		return false;

	int iPartyIndex1 = gObj[iIndex].PartyNumber;
	int iPartyIndex2 = gObj[this->m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX].PartyNumber;

	if ( ObjectMaxRange(iPartyIndex1) != FALSE && iPartyIndex1 == iPartyIndex2 )
		return true;

	return false;
}


bool CBloodCastle::CheckPartyExist(int iIndex)
{
	if ( !gObjIsConnected(iIndex))
		return false;

	int iPartyIndex = gObj[iIndex].PartyNumber;
	int iUserIndex;

	if ( !ObjectMaxRange(iPartyIndex))
		return false;

	for ( int iPartyUserIndex =0;iPartyUserIndex<MAX_USER_IN_PARTY;iPartyUserIndex++)
	{
		iUserIndex = gParty.m_PartyS[iPartyIndex].Number[iPartyUserIndex];

		if ( gObjIsConnected(iUserIndex))
		{
			if ( BC_MAP_RANGE(gObj[iUserIndex].MapNumber) && BC_BRIDGE_RANGE(gObj[iUserIndex].m_cBloodCastleIndex) )
			{
				if ( gObj[iUserIndex].Live == 1 )
				{
					if ( gObj[iUserIndex].m_bBloodCastleComplete == false )
					{
						return true;
					}
				}
			}
		}
	}

	return false;
}

bool CBloodCastle::CheckWinnerPartyComplete(int iBridgeIndex)
{
	if ( !BC_BRIDGE_RANGE(iBridgeIndex) )
		return false;

	if ( !gObjIsConnected(this->m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX))
		return false;

	int iPartyIndex = gObj[this->m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX].PartyNumber;
	int iUserIndex;

	if ( !ObjectMaxRange(iPartyIndex))
		return true;	// #error why true??

	for ( int iPartyUserIndex =0;iPartyUserIndex<MAX_USER_IN_PARTY;iPartyUserIndex++)
	{
		iUserIndex = gParty.m_PartyS[iPartyIndex].Number[iPartyUserIndex];

		if ( gObjIsConnected(iUserIndex))
		{
			if ( BC_MAP_RANGE(gObj[iUserIndex].MapNumber) && BC_BRIDGE_RANGE(gObj[iUserIndex].m_cBloodCastleIndex) )
			{
				if ( gObj[iUserIndex].Live == 1 )
				{
					if ( gObj[iUserIndex].m_bBloodCastleComplete == false )
					{
						return false;
					}
				}
			}
		}
	}

	return true;
}


bool CBloodCastle::SetBridgeWinner(int iBridgeIndex, int iIndex)
{
	if ( !BC_BRIDGE_RANGE(iBridgeIndex) )
		return false;

	if ( gObjIsConnected(this->m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX) != 0 )
		return false;

	if ( !gObjIsConnected(iIndex))
		return false;

	if ( !BC_MAP_RANGE(gObj[iIndex].MapNumber))
		return false;

	this->m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX = iIndex;

	return true;
}


int CBloodCastle::GetWinnerPartyCompleteCount(int iBridgeIndex)
{
	if ( !BC_BRIDGE_RANGE(iBridgeIndex) )
		return false;

	if ( !gObjIsConnected(this->m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX))
		return false;

	int iPartyIndex = gObj[this->m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX].PartyNumber;
	
	if ( !ObjectMaxRange(iPartyIndex))
		return false;

	int iPartyComplete=0;
	int iUserIndex;

	for ( int iPartyUserIndex =0;iPartyUserIndex<MAX_USER_IN_PARTY;iPartyUserIndex++)
	{
		iUserIndex = gParty.m_PartyS[iPartyIndex].Number[iPartyUserIndex];

		if ( gObjIsConnected(iUserIndex))
		{
			if ( BC_MAP_RANGE(gObj[iUserIndex].MapNumber) && BC_BRIDGE_RANGE(gObj[iUserIndex].m_cBloodCastleIndex) )
			{
				if ( gObj[iUserIndex].Live == 1 )
				{
					if ( gObj[iUserIndex].m_bBloodCastleComplete == true )
					{
						iPartyComplete++;
					}
				}
			}
		}
	}

	return iPartyComplete;
}


int CBloodCastle::GetWinnerPartyCompletePoint(int iBridgeIndex)
{
	int iPartyComplete = this->GetWinnerPartyCompleteCount(iBridgeIndex);
	iPartyComplete--;

	if ( CHECK_LIMIT(iPartyComplete, MAX_USER_IN_PARTY) )
		return g_iBC_Party_EventPoint[iPartyComplete];

	return 0;
}

void CBloodCastle::ChangeMonsterState(int iBridgeIndex, int iIndex)
{
	if ( !BC_BRIDGE_RANGE(iBridgeIndex) )
		return;

	int iAddDamageMax = 0;
	int iAddDamageMin = 0;
	int iAddDefense = 0;

	if ( this->m_BridgeData[iBridgeIndex].m_i64_BC_REMAIN_MSEC < 300000 )
	{
		iAddDamageMin = g_iBC_MONSTER_CHANGE_STATE[1][0];
		iAddDamageMax = g_iBC_MONSTER_CHANGE_STATE[1][1];
		iAddDefense = g_iBC_MONSTER_CHANGE_STATE[1][2];
	}
	else if ( this->m_BridgeData[iBridgeIndex].m_i64_BC_REMAIN_MSEC < 600000 )
	{
		iAddDamageMin = g_iBC_MONSTER_CHANGE_STATE[0][0];
		iAddDamageMax = g_iBC_MONSTER_CHANGE_STATE[0][1];
		iAddDefense = g_iBC_MONSTER_CHANGE_STATE[0][2];
	}

	LPMONSTER_ATTRIBUTE lpMA = gMAttr.GetAttr(gObj[iIndex].Class);

	if ( lpMA == NULL )
		return;

	gObj[iIndex].m_AttackDamageMin = lpMA->m_DamageMin + iAddDamageMin;
	gObj[iIndex].m_AttackDamageMax = lpMA->m_DamageMax + iAddDamageMax;
	gObj[iIndex].m_Defense = lpMA->m_Defense + iAddDefense;
}



void CBloodCastle::FixUsersPlayStateWin(int iBridgeIndex)
{
	if ( !BC_BRIDGE_RANGE(iBridgeIndex) )
		return;

	if ( this->m_BridgeData[iBridgeIndex].m_iMISSION_SUCCESS != -1 )
	{
		for ( int i=0;i<MAX_BLOOD_CASTLE_SUB_BRIDGE;i++)
		{
			if ( this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex == -1 )
				continue;

			if ( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Connected < PLAYER_PLAYING )
				continue;

			if ( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleIndex == -1 ||
				 gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleSubIndex == -1 ||
				 gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleIndex != iBridgeIndex )
				continue;

			LPOBJ lpObj = &gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex];

			switch ( this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState )
			{
				case 0:
					if ( ObjectMaxRange(lpObj->PartyNumber) && lpObj->PartyNumber == gObj[this->m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX].PartyNumber )
					{
						if ( BC_MAP_RANGE(lpObj->MapNumber) && lpObj->Live == TRUE && lpObj->Life > 0.0 )
						{
							this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState = 3;
						}
						else
						{
							this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState = 4;
						}
					}
					else 
					{
						if ( !BC_MAP_RANGE(lpObj->MapNumber) || lpObj->Live == 0 || lpObj->Life <= 0.0 )
						{
							this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState = 1;
						}
					}
					break;
				case 1:
					if ( ObjectMaxRange(lpObj->PartyNumber) && lpObj->PartyNumber == gObj[this->m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX].PartyNumber )
					{
						this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState = 4;
					}
					break;
				case 3:
					if ( ObjectMaxRange(lpObj->PartyNumber) && lpObj->PartyNumber == gObj[this->m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX].PartyNumber )
					{
						if ( !BC_MAP_RANGE(lpObj->MapNumber) || lpObj->Live == 0 || lpObj->Life <= 0.0 )
						{
							this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState = 4;
						}
					}
					else 
					{
						if ( BC_MAP_RANGE(lpObj->MapNumber) && lpObj->Live == TRUE && lpObj->Life > 0.0 )
						{
							this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState = 0;
						}
						else
						{
							this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState = 1;
						}
					}
					break;
				case 4:
					if ( !ObjectMaxRange(lpObj->PartyNumber) || lpObj->PartyNumber != gObj[this->m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX].PartyNumber )
					{
						this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState = 1;
					}
					break;
			}

			g_Log.Add("[Blood Castle] (%d) [%s][%s] FixUsersPlayStateWin() - State : %d",
				iBridgeIndex+1, lpObj->AccountID, lpObj->Name, this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState);

		}
	}
}


void CBloodCastle::FixUsersPlayStateFail(int iBridgeIndex)
{
	if ( !BC_BRIDGE_RANGE(iBridgeIndex) )
		return;

	for ( int i=0;i<MAX_BLOOD_CASTLE_SUB_BRIDGE;i++)
	{
		if ( this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex == -1 )
			continue;

		if ( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Connected < PLAYER_PLAYING )
			continue;

		if ( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleIndex == -1 ||
			 gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleSubIndex == -1 ||
			 gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleIndex != iBridgeIndex )
			continue;

		LPOBJ lpObj = &gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex];

		switch ( this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState )
		{
			case 3:
				if ( !BC_MAP_RANGE(lpObj->MapNumber) || lpObj->Live == 0 || lpObj->Life <= 0.0 )
				{
					this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState = 1;
				}
				else
				{
					this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState = 0;
				}
				break;
			case 4:
				this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState = 1;
				break;
		}

		g_Log.Add("[Blood Castle] (%d) [%s][%s] FixUsersPlayStateFail() - State : %d",
			iBridgeIndex+1, lpObj->AccountID, lpObj->Name,
			this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState);
	}
}

int CBloodCastle::GetBridgeMapNumber(int iBridgeIndex) //00555F10
{
	int iMapNumber = 0;

	switch(iBridgeIndex)
	{
	case 0:
		iMapNumber = MAP_INDEX_BLOODCASTLE1;
		break;
	case 1:
		iMapNumber = MAP_INDEX_BLOODCASTLE2;
		break;
	case 2:
		iMapNumber = MAP_INDEX_BLOODCASTLE3;
		break;
	case 3:
		iMapNumber = MAP_INDEX_BLOODCASTLE4;
		break;
	case 4:
		iMapNumber = MAP_INDEX_BLOODCASTLE5;
		break;
	case 5:
		iMapNumber = MAP_INDEX_BLOODCASTLE6;
		break;
	case 6:
		iMapNumber = MAP_INDEX_BLOODCASTLE7;
		break;
	case 7:
		iMapNumber = MAP_INDEX_BLOODCASTLE8;
		break;
	}

	return iMapNumber;
}

//Identical
int CBloodCastle::GetBridgeIndex(int iMAP_NUM) //00555FE0
{
	int iBridgeIndex = -1;

	switch(iMAP_NUM)
	{
	case MAP_INDEX_BLOODCASTLE1:
		iBridgeIndex = 0;
		break;
	case MAP_INDEX_BLOODCASTLE2:
		iBridgeIndex = 1;
		break;
	case MAP_INDEX_BLOODCASTLE3:
		iBridgeIndex = 2;
		break;
	case MAP_INDEX_BLOODCASTLE4:
		iBridgeIndex = 3;
		break;
	case MAP_INDEX_BLOODCASTLE5:
		iBridgeIndex = 4;
		break;
	case MAP_INDEX_BLOODCASTLE6:
		iBridgeIndex = 5;
		break;
	case MAP_INDEX_BLOODCASTLE7:
		iBridgeIndex = 6;
		break;
	case MAP_INDEX_BLOODCASTLE8:
		iBridgeIndex = 7;
		break;
	}

	return iBridgeIndex;
}

//Identical
int CBloodCastle::GetItemMapNumberFirst(int iMAP_NUM) //005560F0
{
	int iMapNumber = iMAP_NUM;

	switch(iMAP_NUM)
	{
	case 238:
		iMapNumber = MAP_INDEX_BLOODCASTLE1;
		break;
	case 239:
		iMapNumber = MAP_INDEX_BLOODCASTLE2;
		break;
	case 240:
		iMapNumber = MAP_INDEX_BLOODCASTLE3;
		break;
	case 241:
		iMapNumber = MAP_INDEX_BLOODCASTLE4;
		break;
	case 242:
		iMapNumber = MAP_INDEX_BLOODCASTLE5;
		break;
	case 243:
		iMapNumber = MAP_INDEX_BLOODCASTLE6;
		break;
	case 244:
		iMapNumber = MAP_INDEX_BLOODCASTLE7;
		break;
	case 245:
		iMapNumber = MAP_INDEX_BLOODCASTLE8;
		break;
	}

	return iMapNumber;
}

//Identical
int CBloodCastle::GetItemMapNumberSecond(int iMAP_NUM) //005561C0
{
	int iMapNumber = iMAP_NUM;

	switch(iMAP_NUM)
	{
	case 246:
		iMapNumber = MAP_INDEX_BLOODCASTLE1;
		break;
	case 247:
		iMapNumber = MAP_INDEX_BLOODCASTLE2;
		break;
	case 248:
		iMapNumber = MAP_INDEX_BLOODCASTLE3;
		break;
	case 249:
		iMapNumber = MAP_INDEX_BLOODCASTLE4;
		break;
	case 250:
		iMapNumber = MAP_INDEX_BLOODCASTLE5;
		break;
	case 251:
		iMapNumber = MAP_INDEX_BLOODCASTLE6;
		break;
	case 252:
		iMapNumber = MAP_INDEX_BLOODCASTLE7;
		break;
	case 253:
		iMapNumber = MAP_INDEX_BLOODCASTLE8;
		break;
	}

	return iMapNumber;
}

bool CBloodCastle::ChangeUserIndex(int OldIndex, int NewIndex, int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return false;
	}

	EnterCriticalSection(&this->m_BridgeData[iBridgeIndex].m_critUserData);

	for(int i=0;i<MAX_BLOOD_CASTLE_SUB_BRIDGE;i++)
	{
		if(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex == OldIndex)
		{
			this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex = NewIndex;
			LeaveCriticalSection(&this->m_BridgeData[iBridgeIndex].m_critUserData);
			return true;
		}
	}

	LeaveCriticalSection(&this->m_BridgeData[iBridgeIndex].m_critUserData);
	return false;
}

void CBloodCastle::SendNoticeMessageToSpecificUser(int iBridgeIndex, int aIndex, int iPlayState)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	PMSG_STATEBLOODCASTLE pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x9B, sizeof(PMSG_STATEBLOODCASTLE));
	pMsg.btPlayState = iPlayState;
	pMsg.wRemainSec = (int)this->m_BridgeData[iBridgeIndex].m_i64_BC_REMAIN_MSEC / 1000;

	if ( iPlayState == 4 )
	{
		pMsg.wMaxKillMonster = this->m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_MAX_COUNT;
		pMsg.wCurKillMonster = this->m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_KILL_COUNT;
	}
	else
	{
		pMsg.wMaxKillMonster = this->m_BridgeData[iBridgeIndex].m_iBC_MONSTER_MAX_COUNT;
		pMsg.wCurKillMonster = this->m_BridgeData[iBridgeIndex].m_iBC_MONSTER_KILL_COUNT;
	}

	pMsg.wUserHaveWeapon = this->m_BridgeData[iBridgeIndex].m_iBC_QUEST_ITEM_USER_INDEX;
	pMsg.btWeaponNum = this->m_BridgeData[iBridgeIndex].m_btBC_QUEST_ITEM_NUMBER + 1;

	for (int i=0;i<MAX_BLOOD_CASTLE_SUB_BRIDGE;i++)
	{
		if ( this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex == aIndex )
		{
			if ( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Connected > PLAYER_LOGGED )
			{
				if ( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleIndex != -1 )
				{
					if ( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleSubIndex != -1 )
					{
						IOCP.DataSend(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, (UCHAR *)&pMsg, pMsg.h.size);
						return;
					}
				}
			}
		}
	}
}

bool CBloodCastle::SetCastleBlockInfo(int iBridgeIndex, int iCastleBlockInfo)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return false;
	}

	if ( iCastleBlockInfo < 0 || iCastleBlockInfo > 1003 )
	{
		return false;
	}

	this->m_BridgeData[iBridgeIndex].m_iBC_CASTLE_BLOCK_INFO = iCastleBlockInfo;

	return true;
}

int CBloodCastle::GetCastleBlockInfo(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return 0;
	}

	return this->m_BridgeData[iBridgeIndex].m_iBC_CASTLE_BLOCK_INFO;
}

int CBloodCastle::GetPlayUserCountRightNow(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return -1;
	}

	int Count = 0;

	for ( int i=0; i<MAX_BLOOD_CASTLE_SUB_BRIDGE;i++ )
	{
		int iIndex = this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex;

		if ( ObjectMaxRange(iIndex) == TRUE )
		{
			if ( BC_MAP_RANGE ( gObj[iIndex].MapNumber ) == TRUE )
			{
				if ( gObj[iIndex].Type == OBJ_USER )
				{
					if ( gObj[iIndex].Connected == PLAYER_PLAYING )
					{
						Count++;
					}
				}
			}
		}
	}
	return Count;
}

bool CBloodCastle::SetQuestItemSerialNumber(int iBridgeIndex, UINT64 iQuestItemSerial)
{
	if (BC_BRIDGE_RANGE(iBridgeIndex) == FALSE)
	{
		return false;
	}

	this->m_BridgeData[iBridgeIndex].m_nBC_QUESTITEM_SERIAL = iQuestItemSerial;
	return true;
}

UINT64 CBloodCastle::GetQuestItemSerialNumber(int iBridgeIndex)
{
	if (BC_BRIDGE_RANGE(iBridgeIndex) == FALSE)
	{
		return -1;
	}

	return this->m_BridgeData[iBridgeIndex].m_nBC_QUESTITEM_SERIAL;
}

void CBloodCastle::ThrowQuestItemByUser(int iBridgeIndex, UINT64 iQuestItemSerial)
{
	if (BC_BRIDGE_RANGE(iBridgeIndex) == false)
	{
		return;
	}

	int CurrQuestItemSerialNum = this->GetQuestItemSerialNumber(iBridgeIndex);

	if (CurrQuestItemSerialNum != -1)
	{
		if (iQuestItemSerial == CurrQuestItemSerialNum)
		{
			int QuestItemUserIndex = this->m_BridgeData[iBridgeIndex].m_iBC_QUEST_ITEM_USER_INDEX;

			if (QuestItemUserIndex != -1)
			{
				g_Log.Add("[Blood Castle] (%d) (Account:%s, Name:%s) Dropped Angel King's Weapon (%d)",
					iBridgeIndex + 1, gObj[QuestItemUserIndex].AccountID, gObj[QuestItemUserIndex].Name,
					this->m_BridgeData[iBridgeIndex].m_btBC_QUEST_ITEM_NUMBER);

				this->m_BridgeData[iBridgeIndex].m_iBC_QUEST_ITEM_USER_INDEX = -1;
				this->m_BridgeData[iBridgeIndex].m_btBC_QUEST_ITEM_NUMBER = 0;
			}
		}
	}
}

void CBloodCastle::CatchQuestItemByUser(int iBridgeIndex, int iUserIndex, int iItemLevel)
{
	if (BC_BRIDGE_RANGE(iBridgeIndex) == false)
	{
		return;
	}

	this->m_BridgeData[iBridgeIndex].m_iBC_QUEST_ITEM_USER_INDEX = gObj[iUserIndex].m_Index;
	char szTempMsg[128];

	switch (iItemLevel)
	{
	case 0:	// Staff of Archangel
		wsprintf(szTempMsg, Lang.GetText(0,75), gObj[iUserIndex].Name);
		this->SendNoticeMessage(iBridgeIndex, szTempMsg);
		break;

	case 1:	// Sword of Archangel
		wsprintf(szTempMsg, Lang.GetText(0,76), gObj[iUserIndex].Name);
		this->SendNoticeMessage(iBridgeIndex, szTempMsg);
		break;

	case 2:	// Crossbow of Archangel
		wsprintf(szTempMsg, Lang.GetText(0,77), gObj[iUserIndex].Name);
		this->SendNoticeMessage(iBridgeIndex, szTempMsg);
		break;

	default:
		szTempMsg[0] = 0;
	}

	g_Log.Add("[Blood Castle] (%d) (Account:%s, Name:%s) picked up Angel King's Weapon (%d)",
		iBridgeIndex + 1, gObj[iUserIndex].AccountID, gObj[iUserIndex].Name, iItemLevel);
}

bool CBloodCastle::NpcAngelKing(LPOBJ lpNpc, LPOBJ lpObj)
{
	int iITEM_LEVEL = 0;
	int iBLOODCASTLE_INDEX = lpObj->m_cBloodCastleIndex;

	if (BC_BRIDGE_RANGE(iBLOODCASTLE_INDEX) == FALSE)
	{
		GSProtocol.GCServerCmd(lpObj->m_Index, 1, 0x18, 0);
		return FALSE;
	}

	g_Log.Add("[Blood Castle] (%d) [%s][%s] Talk to Angel King",
		iBLOODCASTLE_INDEX + 1, lpObj->AccountID, lpObj->Name);

	if (this->m_BridgeData[iBLOODCASTLE_INDEX].m_bBC_REWARDED != false)
	{
		GSProtocol.GCServerCmd(lpObj->m_Index, 1, 0x2E, 0);
		return FALSE;
	}

	if (this->GetCurrentState(iBLOODCASTLE_INDEX) != 2 || this->CheckPlayStart(iBLOODCASTLE_INDEX) == false)
	{
		GSProtocol.GCServerCmd(lpObj->m_Index, 1, 0x18, 0);
		return FALSE;
	}

	if (lpObj->m_bBloodCastleComplete == true)
	{
		GSProtocol.GCServerCmd(lpObj->m_Index, 1, 0x2E, 0);
		return FALSE;
	}

	if (iITEM_LEVEL = iITEM_LEVEL = CHECK_LIMIT(this->CheckQuestItem(lpObj->m_Index), 3))
	{
		if (lpNpc->m_cBloodCastleIndex != iBLOODCASTLE_INDEX)
		{
			GSProtocol.GCServerCmd(lpObj->m_Index, 1, 0x18, 0);
			return FALSE;
		}

		if (this->CheckUserBridgeMember(iBLOODCASTLE_INDEX, lpObj->m_Index) == false)
		{
			GSProtocol.GCServerCmd(lpObj->m_Index, 1, 0x18, 0);
			return FALSE;
		}

		if (this->m_bBC_RESTRICT_FINISH_ENABLE == true)
		{
			if ((this->m_iBC_RESTRICT_FINISH_TIME * 1000) < (int)this->m_BridgeData[iBLOODCASTLE_INDEX].m_i64_BC_REMAIN_MSEC)
			{
				MsgOutput(lpObj->m_Index, Lang.GetText(0,406), this->m_iBC_RESTRICT_FINISH_TIME / 60);
				return FALSE;
			}
		}

		this->SetUserState(lpObj->m_Index, 2);
		this->m_BridgeData[iBLOODCASTLE_INDEX].m_iExtraEXP_Win_Quest_Party = lpObj->PartyNumber;
		this->m_BridgeData[iBLOODCASTLE_INDEX].m_iExtraEXP_Win_Quest_Index = lpObj->m_Index;
		memcpy(this->m_BridgeData[iBLOODCASTLE_INDEX].m_szWin_Quest_CharName, lpObj->Name, 10);
		memcpy(this->m_BridgeData[iBLOODCASTLE_INDEX].m_szWin_Quest_AccountID, lpObj->AccountID, 10);
		this->m_BridgeData[iBLOODCASTLE_INDEX].m_szWin_Quest_CharName[10] = 0;	// Zero String terminated
		this->m_BridgeData[iBLOODCASTLE_INDEX].m_szWin_Quest_AccountID[10] = 0;	// Zero String Terminated

		g_Log.Add("[Blood Castle] (%d) Blood Castle Quest Succeed Result -> Destroy Castle Door [%s][%s]",
			iBLOODCASTLE_INDEX + 1, this->m_BridgeData[iBLOODCASTLE_INDEX].m_szKill_Door_AccountID,
			this->m_BridgeData[iBLOODCASTLE_INDEX].m_szKill_Door_CharName);
		g_Log.Add("[Blood Castle] (%d) Blood Castle Quest Succeed Result -> Destroy Saint Status [%s][%s]",
			iBLOODCASTLE_INDEX + 1, this->m_BridgeData[iBLOODCASTLE_INDEX].m_szKill_Status_AccountID,	// #error Correct this
			this->m_BridgeData[iBLOODCASTLE_INDEX].m_szKill_Status_CharName);
		g_Log.Add("[Blood Castle] (%d) Blood Castle Quest Succeed Result -> Win Quest [%s][%s]",
			iBLOODCASTLE_INDEX + 1, this->m_BridgeData[iBLOODCASTLE_INDEX].m_szWin_Quest_AccountID,
			this->m_BridgeData[iBLOODCASTLE_INDEX].m_szWin_Quest_CharName);

		if (this->SetBridgeWinner(iBLOODCASTLE_INDEX, lpObj->m_Index) == TRUE)
		{
			lpObj->m_bBloodCastleComplete = true;

			if (this->CheckPartyExist(lpObj->m_Index) == FALSE)
			{
				GSProtocol.GCServerCmd(lpObj->m_Index, 1, 0x17, 0);

				g_Log.Add("[Blood Castle] (%d) (Account:%s, Name:%s) Succeed Blood Castle Quest",
					iBLOODCASTLE_INDEX + 1, lpObj->AccountID, lpObj->Name);
			}
			else
			{
				GSProtocol.GCServerCmd(lpObj->m_Index, 1, 0x17, 0);
				g_Log.Add("[Blood Castle] (%d) (Account:%s, Name:%s) Succeed Blood Castle Quest - More Party to register",
					iBLOODCASTLE_INDEX + 1, lpObj->AccountID, lpObj->Name);
			}
		}
		else
		{
			g_Log.Add("[Blood Castle] (%d) (Account:%s, Name:%s) Winner Already Exist !!",
				iBLOODCASTLE_INDEX + 1, lpObj->AccountID, lpObj->Name);

			return FALSE;
		}

		this->SearchUserDeleteQuestItem(lpObj->m_Index);
	}
	else
	{
		if (this->CheckUserWinnerParty(iBLOODCASTLE_INDEX, lpObj->m_Index) == TRUE)
		{
			lpObj->m_bBloodCastleComplete = true;

			if (this->CheckPartyExist(lpObj->m_Index) == FALSE)
			{
				GSProtocol.GCServerCmd(lpObj->m_Index, 1, 0x17, 0);

				g_Log.Add("[Blood Castle] (%d) (Account:%s, Name:%s) Succeed Blood Castle Quest",
					iBLOODCASTLE_INDEX + 1, lpObj->AccountID, lpObj->Name);
			}
			else
			{
				GSProtocol.GCServerCmd(lpObj->m_Index, 1, 0x17, 0);
				g_Log.Add("[Blood Castle] (%d) (Account:%s, Name:%s) Succeed Blood Castle Quest - More Party to register",
					iBLOODCASTLE_INDEX + 1, lpObj->AccountID, lpObj->Name);
			}

			return FALSE;
		}

		GSProtocol.GCServerCmd(lpObj->m_Index, 1, 0x18, 0);
	}

	return FALSE;
}

void CBloodCastle::KillMonsterProc(int iBridgeIndex, LPOBJ lpMonsterObj)
{
	if (BC_BRIDGE_RANGE(iBridgeIndex) == false)
	{
		return;
	}

	if (lpMonsterObj->Class == 89 || lpMonsterObj->Class == 95 || lpMonsterObj->Class == 112 || lpMonsterObj->Class == 118 || lpMonsterObj->Class == 124 || lpMonsterObj->Class == 130 || lpMonsterObj->Class == 143 || lpMonsterObj->Class == 433)
	{
		this->m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_KILL_COUNT++;
	}
	else
	{
		this->m_BridgeData[iBridgeIndex].m_iBC_MONSTER_KILL_COUNT++;
	}

	if (this->CheckMonsterKillCount(iBridgeIndex) != false)
	{
		if (this->m_BridgeData[iBridgeIndex].m_bBC_MONSTER_KILL_COMPLETE == false)
		{
			this->m_BridgeData[iBridgeIndex].m_bBC_MONSTER_KILL_COMPLETE = true;
			this->m_BridgeData[iBridgeIndex].m_iBC_MONSTER_MAX_COUNT = -1;

			PMSG_STATEBLOODCASTLE pMsg;

			PHeadSetB((LPBYTE)&pMsg, 0x9B, sizeof(PMSG_STATEBLOODCASTLE));

			pMsg.btPlayState = BC_STATE_PLAYEND;
			pMsg.wRemainSec = 0;
			pMsg.wMaxKillMonster = 0;
			pMsg.wCurKillMonster = 0;
			pMsg.wUserHaveWeapon = 0;
			pMsg.btWeaponNum = -1;

			this->SendBridgeAnyMsg((LPBYTE)&pMsg, pMsg.h.size, iBridgeIndex);
			this->ReleaseCastleBridge(iBridgeIndex);

			this->m_BridgeData[iBridgeIndex].m_i64_BC_TICK_DOOR_OPEN = GetTickCount64() + 3000;

			g_Log.Add("[Blood Castle] (%d) All of the Monster Terminated -> %d", iBridgeIndex + 1, this->m_BridgeData[iBridgeIndex].m_iBC_MONSTER_KILL_COUNT);

			this->m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_MAX_COUNT = this->GetCurrentLiveUserCount(iBridgeIndex) * 2;
			this->m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_KILL_COUNT = 0;

			if (this->m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_MAX_COUNT > 10)
			{
				this->m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_MAX_COUNT = 10;
			}
		}

		if (this->m_BridgeData[iBridgeIndex].m_iBC_MONSTER_SUCCESS_MSG_COUNT < 1)
		{
			this->m_BridgeData[iBridgeIndex].m_iBC_MONSTER_SUCCESS_MSG_COUNT++;
			this->SendNoticeMessage(iBridgeIndex, Lang.GetText(0,72));
		}

	}

	if (this->m_BridgeData[iBridgeIndex].m_bBC_MONSTER_KILL_COMPLETE != false)
	{
		if (this->CheckBossKillCount(iBridgeIndex) != false)
		{
			if (this->m_BridgeData[iBridgeIndex].m_bBC_BOSS_MONSTER_KILL_COMPLETE == false)
			{
				this->m_BridgeData[iBridgeIndex].m_bBC_BOSS_MONSTER_KILL_COMPLETE = true;
				this->m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_MAX_COUNT = -1;

				this->SetSaintStatue(iBridgeIndex);

				g_Log.Add("[Blood Castle] (%d) All of the Boss Monster Terminated -> %d", iBridgeIndex + 1, this->m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_KILL_COUNT);
			}

			if (this->m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_SUCCESS_MSG_COUNT < 1)
			{
				this->m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_SUCCESS_MSG_COUNT++;
				this->SendNoticeMessage(iBridgeIndex, Lang.GetText(0,81));
			}
		}
	}
}

void CBloodCastle::DestroyCastleDoor(int iBridgeIndex, LPOBJ lpDoorObj)
{
	if (BC_BRIDGE_RANGE(iBridgeIndex) == false)
	{
		return;
	}

	int TopHitUser = gObjMonsterTopHitDamageUser(lpDoorObj);

	if (TopHitUser != -1)
	{
		char szMsg[256];

		wsprintf(szMsg, Lang.GetText(0,79), gObj[TopHitUser].Name);

		this->SendNoticeMessage(iBridgeIndex, szMsg);

		this->m_BridgeData[iBridgeIndex].m_iExtraEXP_Kill_Door_Party = gObj[TopHitUser].PartyNumber;
		this->m_BridgeData[iBridgeIndex].m_iExtraEXP_Kill_Door_Index = TopHitUser;

		memcpy(this->m_BridgeData[iBridgeIndex].m_szKill_Door_CharName, gObj[TopHitUser].Name, 10);
		memcpy(this->m_BridgeData[iBridgeIndex].m_szKill_Door_AccountID, gObj[TopHitUser].AccountID, 10);

		this->m_BridgeData[iBridgeIndex].m_szKill_Door_CharName[10] = 0;
		this->m_BridgeData[iBridgeIndex].m_szKill_Door_AccountID[10] = 0;

		g_QuestExpProgMng.ChkUserQuestTypeEventMap(QUESTEXP_ASK_BLOODCASTLE_DOOR_KILL, &gObj[TopHitUser], this->GetBridgeIndex(gObj[TopHitUser].MapNumber), 1);

		g_Log.Add("[Blood Castle] (%d) Door Terminated -> %d [%s][%s]", iBridgeIndex + 1, this->m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_KILL_COUNT, gObj[TopHitUser].AccountID, gObj[TopHitUser].Name);
	}
	else
	{
		this->SendNoticeMessage(iBridgeIndex, Lang.GetText(0,73));
		g_Log.Add("[Blood Castle] (%d) Door Terminated -> %d)", iBridgeIndex + 1, this->m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_KILL_COUNT);
	}

	this->m_BridgeData[iBridgeIndex].m_bCASTLE_DOOR_LIVE = 0;
	this->m_BridgeData[iBridgeIndex].m_bBC_DOOR_TERMINATE_COMPLETE = 1;
	this->m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_MAX_COUNT = this->GetCurrentLiveUserCount(iBridgeIndex) * 2;

	this->m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_KILL_COUNT = 0;

	if (this->m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_MAX_COUNT > 10)
	{
		this->m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_MAX_COUNT = 10;
	}

	this->SendCastleDoorBlockInfo(iBridgeIndex, 0);
	this->ReleaseCastleDoor(iBridgeIndex);
	this->SetCastleBlockInfo(iBridgeIndex, 1003);

	if (this->GetCurrentState(iBridgeIndex) == 2)
	{
		this->SetBossMonster(iBridgeIndex);
	}

	for (int n = 0; n < MAX_BLOOD_CASTLE_SUB_BRIDGE; n++)
	{
		if (this->m_BridgeData[iBridgeIndex].m_UserData[n].m_iIndex != -1)
		{
			if (gObj[this->m_BridgeData[iBridgeIndex].m_UserData[n].m_iIndex].Connected > PLAYER_LOGGED)
			{
				this->m_BridgeData[iBridgeIndex].m_UserData[n].m_bLiveWhenDoorBreak = 1;
			}

			if (this->m_BridgeData[iBridgeIndex].m_iExtraEXP_Kill_Door_Party == gObj[this->m_BridgeData[iBridgeIndex].m_UserData[n].m_iIndex].PartyNumber)
			{
				OBJECTSTRUCT* tmp_user = &gObj[this->m_BridgeData[iBridgeIndex].m_UserData[n].m_iIndex];

				int BridgeIndex = this->GetBridgeIndex(tmp_user->MapNumber);
				g_QuestExpProgMng.ChkUserQuestTypeEventMap(QUESTEXP_ASK_BLOODCASTLE_DOOR_KILL, tmp_user, BridgeIndex, 1);
			}
		}
	}
}

void CBloodCastle::DestroySaintStatue(int iBridgeIndex, LPOBJ lpStatueObj)
{
	if (BC_BRIDGE_RANGE(iBridgeIndex) == false)
	{
		return;
	}

	int TopHitUser = gObjMonsterTopHitDamageUser(lpStatueObj);

	if (TopHitUser != -1)
	{
		char szMsg[256];

		wsprintf(szMsg, Lang.GetText(0,80), gObj[TopHitUser].Name);

		this->SendNoticeMessage(iBridgeIndex, szMsg);

		this->m_BridgeData[iBridgeIndex].m_iExtraEXP_Kill_Statue_Party = gObj[TopHitUser].PartyNumber;
		this->m_BridgeData[iBridgeIndex].m_iExtraEXP_Kill_Statue_Index = gObj[TopHitUser].m_Index;

		memcpy(this->m_BridgeData[iBridgeIndex].m_szKill_Status_CharName, gObj[TopHitUser].Name, 10);
		memcpy(this->m_BridgeData[iBridgeIndex].m_szKill_Status_AccountID, gObj[TopHitUser].AccountID, 10);

		this->m_BridgeData[iBridgeIndex].m_szKill_Status_CharName[10] = 0;
		this->m_BridgeData[iBridgeIndex].m_szKill_Status_AccountID[10] = 0;

		g_Log.Add("[Blood Castle] (%d) a stone statue Of Saint Terminated -> %d [%s][%s]", iBridgeIndex + 1, this->m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_KILL_COUNT, gObj[TopHitUser].AccountID, gObj[TopHitUser].Name);
	}
	else
	{
		this->SendNoticeMessage(iBridgeIndex, Lang.GetText(0,306));
		g_Log.Add("[Blood Castle] (%d) a stone statue Of Saint Terminated -> %d", iBridgeIndex + 1, this->m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_KILL_COUNT);
	}

	int level = lpStatueObj->Class - 132;

	int type = ItemGetNumberMake(13, 19);

	int iMapNumber;

	switch (lpStatueObj->MapNumber)
	{
	case MAP_INDEX_BLOODCASTLE1:
		iMapNumber = 246;
		break;
	case MAP_INDEX_BLOODCASTLE2:
		iMapNumber = 247;
		break;
	case MAP_INDEX_BLOODCASTLE3:
		iMapNumber = 248;
		break;
	case MAP_INDEX_BLOODCASTLE4:
		iMapNumber = 249;
		break;
	case MAP_INDEX_BLOODCASTLE5:
		iMapNumber = 250;
		break;
	case MAP_INDEX_BLOODCASTLE6:
		iMapNumber = 251;
		break;
	case MAP_INDEX_BLOODCASTLE7:
		iMapNumber = 252;
		break;
	case MAP_INDEX_BLOODCASTLE8:
		iMapNumber = 253;
		break;
	}

	ItemSerialCreateSend(lpStatueObj->m_Index, iMapNumber, lpStatueObj->X, lpStatueObj->Y, type, level, 0, 0, 0, 0, TopHitUser, 0, 0, 0, 0, 0);

	this->m_BridgeData[iBridgeIndex].m_btBC_QUEST_ITEM_NUMBER = level;

	g_Log.Add("[Blood Castle] (%d) Ultimate Weapon Dropped -> (%s)", iBridgeIndex + 1, (level != 0) ? ((level == 1) ? "Sword" : "Crossbow") : "Staff");

}

bool CBloodCastle::CheckCastleDoorLive(int iBridgeIndex)
{
	if (BC_BRIDGE_RANGE(iBridgeIndex) == FALSE)
	{
		return -1;
	}

	return this->m_BridgeData[iBridgeIndex].m_bCASTLE_DOOR_LIVE;
}

BLOODCASTLE_MONSTER_POSITION * CBloodCastle::GetMonsterPosData(int iPosNum, int iBridgeIndex, int iMonsterIndex)
{
	if (BC_BRIDGE_RANGE(iBridgeIndex) == FALSE)
	{
		return NULL;
	}

	if (!ObjectMaxRange(iMonsterIndex))
	{
		return NULL;
	}

	LPOBJ lpObj = &gObj[iMonsterIndex];

	if (lpObj->Class == 89 || lpObj->Class == 95 ||
		lpObj->Class == 112 || lpObj->Class == 118 ||
		lpObj->Class == 124 || lpObj->Class == 130 ||
		lpObj->Class == 143 || lpObj->Class == 433)
	{
		if (iPosNum < 0 || iPosNum >= 20)
		{
			return NULL;
		}

		return &this->m_BCMP_BossMonster[iBridgeIndex][iPosNum];
	}

	else
	{
		if (iPosNum < 0 || iPosNum >= 100)
		{
			return NULL;
		}

		return &this->m_BCMP_General[iBridgeIndex][iPosNum];
	}

	return NULL;
}

bool CBloodCastle::SetPosMonster(int iBridgeIndex, int iMonsterIndex, int iPosNum, int iMonsterClass)
{
	if (BC_BRIDGE_RANGE(iBridgeIndex) == FALSE)
	{
		return false;
	}

	if (!ObjectMaxRange(iMonsterIndex))
	{
		return false;
	}

	BLOODCASTLE_MONSTER_POSITION * lpPos = nullptr;
	LPOBJ lpObj = &gObj[iMonsterIndex];

	if (iMonsterClass == 89 || iMonsterClass == 95 ||
		iMonsterClass == 112 || iMonsterClass == 118 ||
		iMonsterClass == 124 || iMonsterClass == 130 ||
		iMonsterClass == 143 || iMonsterClass == 433)
	{
		if (iPosNum < 0 || iPosNum >= 20)
		{
			return false;
		}
		
		lpPos = &this->m_BCMP_BossMonster[iBridgeIndex][iPosNum];
	}

	else
	{
		if (iPosNum < 0 || iPosNum >= 100)
		{
			return false;
		}

		lpPos = &this->m_BCMP_General[iBridgeIndex][iPosNum];
	}

	if (lpPos == nullptr)
	{
		return false;
	}

	lpObj->m_PosNum = iPosNum;
	lpObj->X = lpPos->m_X;
	lpObj->Y = lpPos->m_Y;
	lpObj->MapNumber = lpPos->m_MapNumber;
	lpObj->TX = lpObj->X;
	lpObj->TY = lpObj->Y;
	lpObj->m_OldX = lpObj->X;
	lpObj->m_OldY = lpObj->Y;
	lpObj->Dir = lpPos->m_Dir;
	lpObj->StartX = lpObj->X;
	lpObj->StartY = lpObj->Y;

	if (this->GetPosition(lpPos, lpObj->MapNumber, lpObj->X, lpObj->Y) == FALSE)
	{
		g_Log.Add("[Blood Castle] [Invalid Location]: MAP: %d X: %d Y: %d POSNUM: %d", lpObj->MapNumber, lpPos->m_X, lpPos->m_Y, iPosNum);
		return FALSE;
	}

	lpObj->TX = lpObj->X;
	lpObj->TY = lpObj->Y;
	lpObj->m_OldX = lpObj->X;
	lpObj->m_OldY = lpObj->Y;
	lpObj->Dir = lpPos->m_Dir;
	lpObj->StartX = lpObj->X;
	lpObj->StartY = lpObj->Y;
	lpObj->m_iPentagramMainAttribute = 0;

	return true;

}

bool CBloodCastle::GetPosition(BLOODCASTLE_MONSTER_POSITION * lpPos, int iMapNumber, short & x, short & y)
{
	if (lpPos == nullptr)
	{
		g_Log.Add("ERROR : %s %d", __FILE__, __LINE__);
		return FALSE;
	}

	x = lpPos->m_X;
	y = lpPos->m_Y;

	return FALSE;
}


//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


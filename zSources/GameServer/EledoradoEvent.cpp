//////////////////////////////////////////////////////////////////////
// EledoradoEvent.cpp
// ------------------------------
// Decompiled by Deathway
// Date : 2007-05-09
// ------------------------------
#include "stdafx.h"
#include "EledoradoEvent.h"
#include "Gamemain.h"
#include "TLog.h"
#include "user.h"
#include "TNotice.h"
#include "configread.h"
#include "MapServerManager.h"

CEledoradoEvent gEledoradoEvent;

CEledoradoEvent::CEledoradoEvent()
{
	this->m_BossGoldDerconMapNumber[0] = -1;
	this->m_BossGoldDerconMapNumber[1] = -1;
	this->m_BossGoldDerconMapNumber[2] = -1;
	this->m_BossGoldDerconMapNumber[3] = -1;

	InitializeCriticalSection(&this->m_MonCriti);
}



CEledoradoEvent::~CEledoradoEvent()
{
	DeleteCriticalSection(&this->m_MonCriti);
}

void CEledoradoEvent::ClearMonster()
{
	EnterCriticalSection(&this->m_MonCriti);

	for (std::map<int, ELEDORADO_MONSTER_INFO>::iterator It = this->m_mapMonsterInfo.begin(); It != this->m_mapMonsterInfo.end(); It++)
	{
		gObjDel(It->second.m_Index);
	}

	this->m_mapMonsterInfo.clear();

	LeaveCriticalSection(&this->m_MonCriti);
}

void CEledoradoEvent::Init()
{
	this->ClearMonster();
	this->m_vtMonsterPos.clear();

	char * szFile = g_ConfigRead.GetPath("\\Events\\IGC_GoldenInvasion.xml");

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(szFile);

	if (res.status != pugi::status_ok)
	{
		g_Log.MsgBox("%s load fail (%s)", szFile, res.description());
		return;
	}

	pugi::xml_node main = file.child("GoldenInvasion");
	pugi::xml_node spot = main.child("Spot");

	for (pugi::xml_node spawn = spot.child("Spawn"); spawn; spawn = spawn.next_sibling())
	{
		ELEDORARO_MONSTER_POS m_Pos;

		m_Pos.m_Type = spawn.attribute("Index").as_int();

		m_Pos.m_MapNumber[0] = spawn.attribute("MapNumber1").as_int();
		m_Pos.m_MapNumber[1] = spawn.attribute("MapNumber2").as_int();
		m_Pos.m_MapNumber[2] = spawn.attribute("MapNumber3").as_int();
		m_Pos.m_MapNumber[3] = spawn.attribute("MapNumber4").as_int();

		m_Pos.m_Dis = spawn.attribute("Distance").as_int();
		m_Pos.m_X = spawn.attribute("StartX").as_int();
		m_Pos.m_Y = spawn.attribute("StartY").as_int();
		m_Pos.m_W = spawn.attribute("EndX").as_int();
		m_Pos.m_H = spawn.attribute("EndY").as_int();
		m_Pos.m_Dir = spawn.attribute("Dir").as_int();
		m_Pos.m_Count = spawn.attribute("Count").as_int();

		this->m_vtMonsterPos.insert(std::pair<int, ELEDORARO_MONSTER_POS>(m_Pos.m_Type, m_Pos));
	}
}

void CEledoradoEvent::SetEventState(int State)
{
	this->EventState = State;
}

void CEledoradoEvent::Run()
{
	return;
}

void CEledoradoEvent::RegenGoldGoblen()
{
	std::map<int, ELEDORARO_MONSTER_POS>::iterator It = this->m_vtMonsterPos.find(78);

	if (It == this->m_vtMonsterPos.end())
	{
		return;
	}

	int MapNumber;

	for (int i = 0; i < It->second.m_Count; i++)
	{
		MapNumber = this->GetMapNumber(It);

		if (g_MapServerManager.CheckMapCanMove(MapNumber) == FALSE)
		{
			return;
		}

		if (MapNumber == -1)
		{
			g_Log.AddC(TColor::Red, "%s MapNumber == -1", __FUNCTION__);
			return;
		}

		int result = gObjAddMonster(MapNumber);

		if (result == -1)
		{
			g_Log.AddC(TColor::Red, "%s result == -1", __FUNCTION__);
			return;
		}

		gObj[result].MapNumber = MapNumber;

		while (this->GetBoxPosition(MapNumber, It->second.m_X, It->second.m_Y, It->second.m_W, It->second.m_H, gObj[result].X, gObj[result].Y) == 0)
		{

		}

		gObj[result].m_PosNum = -1;
		gObj[result].TX = gObj[result].X;
		gObj[result].TY = gObj[result].Y;
		gObj[result].MTX = gObj[result].X;
		gObj[result].MTY = gObj[result].Y;
		gObj[result].m_OldX = gObj[result].X;
		gObj[result].m_OldY = gObj[result].Y;
		gObj[result].StartX = gObj[result].X;
		gObj[result].StartY = gObj[result].Y;

		if (It->second.m_Dir == (BYTE)-1)
		{
			gObj[result].Dir = rand() % 8;
		}

		else
		{
			gObj[result].Dir = It->second.m_Dir;
		}

		gObjSetMonster(result, It->second.m_Type);

		EnterCriticalSection(&this->m_MonCriti);

		ELEDORADO_MONSTER_INFO m_MonsterInfo;
		m_MonsterInfo.m_Index = result;

		this->m_mapMonsterInfo.insert(std::pair<int, ELEDORADO_MONSTER_INFO>(result, m_MonsterInfo));

		LeaveCriticalSection(&this->m_MonCriti);

		g_Log.Add("[Eledorado Event] Respawn Golden Goblin : %d, %d,%d",
			MapNumber, gObj[result].X, gObj[result].Y);
	}
}


void CEledoradoEvent::RegenTitan()
{
	std::map<int, ELEDORARO_MONSTER_POS>::iterator It = this->m_vtMonsterPos.find(53);

	if (It == this->m_vtMonsterPos.end())
	{
		return;
	}

	int MapNumber;

	for (int i = 0; i < It->second.m_Count; i++)
	{
		MapNumber = this->GetMapNumber(It);

		if (g_MapServerManager.CheckMapCanMove(MapNumber) == FALSE)
		{
			return;
		}

		if (MapNumber == -1)
		{
			g_Log.AddC(TColor::Red, "%s MapNumber == -1", __FUNCTION__);
			return;
		}

		int result = gObjAddMonster(MapNumber);

		if (result == -1)
		{
			g_Log.AddC(TColor::Red, "%s result == -1", __FUNCTION__);
			return;
		}

		gObj[result].MapNumber = MapNumber;

		while (this->GetBoxPosition(MapNumber, It->second.m_X, It->second.m_Y, It->second.m_W, It->second.m_H, gObj[result].X, gObj[result].Y) == 0)
		{

		}

		this->m_BossTitanMapNumber = gObj[result].MapNumber;
		this->m_BossTitanMapX = gObj[result].X;
		this->m_BossTitanMapY = gObj[result].Y;

		gObj[result].m_PosNum = -1;
		gObj[result].TX = gObj[result].X;
		gObj[result].TY = gObj[result].Y;
		gObj[result].MTX = gObj[result].X;
		gObj[result].MTY = gObj[result].Y;
		gObj[result].m_OldX = gObj[result].X;
		gObj[result].m_OldY = gObj[result].Y;
		gObj[result].StartX = gObj[result].X;
		gObj[result].StartY = gObj[result].Y;

		if (It->second.m_Dir == (BYTE)-1)
		{
			gObj[result].Dir = rand() % 8;
		}

		else
		{
			gObj[result].Dir = It->second.m_Dir;
		}

		gObjSetMonster(result, It->second.m_Type);

		EnterCriticalSection(&this->m_MonCriti);

		ELEDORADO_MONSTER_INFO m_MonsterInfo;
		m_MonsterInfo.m_Index = result;

		this->m_mapMonsterInfo.insert(std::pair<int, ELEDORADO_MONSTER_INFO>(result, m_MonsterInfo));

		LeaveCriticalSection(&this->m_MonCriti);

		g_Log.Add("[Eledorado Event] Respawn Golden Titan : %d, %d,%d",
			MapNumber, gObj[result].X, gObj[result].Y);
	}

	It = this->m_vtMonsterPos.find(54);

	if (It == this->m_vtMonsterPos.end())
	{
		return;
	}

	for (int i = 0; i < It->second.m_Count; i++)
	{
		MapNumber = this->m_BossTitanMapNumber;
		int result = gObjAddMonster(MapNumber);

		if (result == -1)
		{
			g_Log.AddC(TColor::Red, "%s result == -1", __FUNCTION__);
			return;
		}

		gObj[result].MapNumber = MapNumber;

		this->GetBoxPosition(MapNumber, this->m_BossTitanMapX - 4, this->m_BossTitanMapY - 4, this->m_BossTitanMapX + 4, this->m_BossTitanMapY + 4, gObj[result].X, gObj[result].Y);

		gObj[result].m_PosNum = -1;
		gObj[result].TX = gObj[result].X;
		gObj[result].TY = gObj[result].Y;
		gObj[result].MTX = gObj[result].X;
		gObj[result].MTY = gObj[result].Y;
		gObj[result].m_OldX = gObj[result].X;
		gObj[result].m_OldY = gObj[result].Y;
		gObj[result].StartX = gObj[result].X;
		gObj[result].StartY = gObj[result].Y;

		if (It->second.m_Dir == (BYTE)-1)
		{
			gObj[result].Dir = rand() % 8;
		}

		else
		{
			gObj[result].Dir = It->second.m_Dir;
		}

		EnterCriticalSection(&this->m_MonCriti);

		ELEDORADO_MONSTER_INFO m_MonsterInfo;
		m_MonsterInfo.m_Index = result;

		this->m_mapMonsterInfo.insert(std::pair<int, ELEDORADO_MONSTER_INFO>(result, m_MonsterInfo));

		LeaveCriticalSection(&this->m_MonCriti);

		gObjSetMonster(result, It->second.m_Type);
	}
}

void CEledoradoEvent::RegenGoldDercon()
{
	std::map<int, ELEDORARO_MONSTER_POS>::iterator It = this->m_vtMonsterPos.find(79);

	if (It == this->m_vtMonsterPos.end())
	{
		return;
	}

	int MapNumber = -1;
	int SelMap = -1;
	int count = 0;

	for (int i = 0; i < It->second.m_Count; i++)
	{
		MapNumber = this->GetMapNumber(It);

		if (g_MapServerManager.CheckMapCanMove(MapNumber) == FALSE)
		{
			return;
		}

		if (MapNumber == -1)
		{
			g_Log.AddC(TColor::Red, "%s MapNumber == -1", __FUNCTION__);
			return;
		}

		if (SelMap == -1)
		{
			SelMap = MapNumber;
		}

		else
		{
			MapNumber = SelMap;
		}

		int result = gObjAddMonster(MapNumber);

		if (result == -1)
		{
			g_Log.AddC(TColor::Red, "%s result == -1", __FUNCTION__);
			return;
		}

		gObj[result].MapNumber = MapNumber;

		while (this->GetBoxPosition(MapNumber, It->second.m_X, It->second.m_Y, It->second.m_W, It->second.m_H, gObj[result].X, gObj[result].Y) == 0)
		{

		}

		gObj[result].m_PosNum = -1;
		gObj[result].TX = gObj[result].X;
		gObj[result].TY = gObj[result].Y;
		gObj[result].MTX = gObj[result].X;
		gObj[result].MTY = gObj[result].Y;
		gObj[result].m_OldX = gObj[result].X;
		gObj[result].m_OldY = gObj[result].Y;
		gObj[result].StartX = gObj[result].X;
		gObj[result].StartY = gObj[result].Y;

		if (It->second.m_Dir == (BYTE)-1)
		{
			gObj[result].Dir = rand() % 8;
		}

		else
		{
			gObj[result].Dir = It->second.m_Dir;
		}

		gObjSetMonster(result, It->second.m_Type);

		EnterCriticalSection(&this->m_MonCriti);

		ELEDORADO_MONSTER_INFO m_MonsterInfo;
		m_MonsterInfo.m_Index = result;

		this->m_mapMonsterInfo.insert(std::pair<int, ELEDORADO_MONSTER_INFO>(result, m_MonsterInfo));

		LeaveCriticalSection(&this->m_MonCriti);

		g_Log.Add("[Eledorado Event] Respawn Golden Dragon : %d, %d,%d",
			MapNumber, gObj[result].X, gObj[result].Y);

		this->m_BossGoldDerconMapNumber[count] = MapNumber;
		gObj[result].m_BossGoldDerconMapNumber = MapNumber;	// #error change count by MapNumber
		count++;
	}

	if (MapNumber != -1)
	{
		char szTemp[256];
		strcpy(szTemp, Lang.GetMap(0, MapNumber));

		PMSG_NOTICE pNotice;
		TNotice::MakeNoticeMsgEx(&pNotice, 0, Lang.GetText(0, 64), szTemp);
		TNotice::SendNoticeToAllUser(&pNotice);
	}

	this->CheckGoldDercon(MapNumber);
}


void CEledoradoEvent::RegenDevilLizardKing()
{
	std::map<int, ELEDORARO_MONSTER_POS>::iterator It = this->m_vtMonsterPos.find(80);

	if (It == this->m_vtMonsterPos.end())
	{
		return;
	}

	int MapNumber;

	for (int i = 0; i < It->second.m_Count; i++)
	{
		MapNumber = this->GetMapNumber(It);

		if (g_MapServerManager.CheckMapCanMove(MapNumber) == FALSE)
		{
			return;
		}

		if (MapNumber == -1)
		{
			g_Log.AddC(TColor::Red, "%s MapNumber == -1", __FUNCTION__);
			return;
		}

		int result = gObjAddMonster(MapNumber);

		if (result == -1)
		{
			g_Log.AddC(TColor::Red, "%s result == -1", __FUNCTION__);
			return;
		}

		gObj[result].MapNumber = MapNumber;

		while (this->GetBoxPosition(MapNumber, It->second.m_X, It->second.m_Y, It->second.m_W, It->second.m_H, gObj[result].X, gObj[result].Y) == 0)
		{

		}

		this->m_BossDevilLizardKingMapNumber = gObj[result].MapNumber;
		this->m_BossDevilLizardKingMapX = gObj[result].X;
		this->m_BossDevilLizardKingMapY = gObj[result].Y;

		gObj[result].m_PosNum = -1;
		gObj[result].TX = gObj[result].X;
		gObj[result].TY = gObj[result].Y;
		gObj[result].MTX = gObj[result].X;
		gObj[result].MTY = gObj[result].Y;
		gObj[result].m_OldX = gObj[result].X;
		gObj[result].m_OldY = gObj[result].Y;
		gObj[result].StartX = gObj[result].X;
		gObj[result].StartY = gObj[result].Y;

		if (It->second.m_Dir == (BYTE)-1)
		{
			gObj[result].Dir = rand() % 8;
		}

		else
		{
			gObj[result].Dir = It->second.m_Dir;
		}

		gObjSetMonster(result, It->second.m_Type);

		EnterCriticalSection(&this->m_MonCriti);

		ELEDORADO_MONSTER_INFO m_MonsterInfo;
		m_MonsterInfo.m_Index = result;

		this->m_mapMonsterInfo.insert(std::pair<int, ELEDORADO_MONSTER_INFO>(result, m_MonsterInfo));

		LeaveCriticalSection(&this->m_MonCriti);

		g_Log.Add("[Eledorado Event] Respawn Golden LizardKing : %d, %d,%d",
			MapNumber, gObj[result].X, gObj[result].Y);
	}

	It = this->m_vtMonsterPos.find(81);

	if (It == this->m_vtMonsterPos.end())
	{
		return;
	}

	for (int i = 0; i < It->second.m_Count; i++)
	{
		MapNumber = this->m_BossDevilLizardKingMapNumber;
		int result = gObjAddMonster(MapNumber);

		if (result == -1)
		{
			g_Log.AddC(TColor::Red, "%s result == -1", __FUNCTION__);
			return;
		}

		gObj[result].MapNumber = MapNumber;

		this->GetBoxPosition(MapNumber, this->m_BossDevilLizardKingMapX - 4, this->m_BossDevilLizardKingMapY - 4,
			this->m_BossDevilLizardKingMapX + 4, this->m_BossDevilLizardKingMapY + 4, gObj[result].X, gObj[result].Y);

		gObj[result].m_PosNum = -1;
		gObj[result].TX = gObj[result].X;
		gObj[result].TY = gObj[result].Y;
		gObj[result].MTX = gObj[result].X;
		gObj[result].MTY = gObj[result].Y;
		gObj[result].m_OldX = gObj[result].X;
		gObj[result].m_OldY = gObj[result].Y;
		gObj[result].StartX = gObj[result].X;
		gObj[result].StartY = gObj[result].Y;

		if (It->second.m_Dir == (BYTE)-1)
		{
			gObj[result].Dir = rand() % 8;
		}

		else
		{
			gObj[result].Dir = It->second.m_Dir;
		}

		gObjSetMonster(result, It->second.m_Type);

		EnterCriticalSection(&this->m_MonCriti);

		ELEDORADO_MONSTER_INFO m_MonsterInfo;
		m_MonsterInfo.m_Index = result;

		this->m_mapMonsterInfo.insert(std::pair<int, ELEDORADO_MONSTER_INFO>(result, m_MonsterInfo));

		LeaveCriticalSection(&this->m_MonCriti);
	}
}

void CEledoradoEvent::RegenKantur()
{
	std::map<int, ELEDORARO_MONSTER_POS>::iterator It = this->m_vtMonsterPos.find(82);

	if (It == this->m_vtMonsterPos.end())
	{
		return;
	}

	int MapNumber;

	for (int i = 0; i < It->second.m_Count; i++)
	{
		MapNumber = this->GetMapNumber(It);

		if (g_MapServerManager.CheckMapCanMove(MapNumber) == FALSE)
		{
			return;
		}

		if (MapNumber == -1)
		{
			g_Log.AddC(TColor::Red, "%s MapNumber == -1", __FUNCTION__);
			return;
		}

		int result = gObjAddMonster(MapNumber);

		if (result == -1)
		{
			g_Log.AddC(TColor::Red, "%s result == -1", __FUNCTION__);
			return;
		}

		gObj[result].MapNumber = MapNumber;

		while (this->GetBoxPosition(MapNumber, It->second.m_X, It->second.m_Y, It->second.m_W, It->second.m_H, gObj[result].X, gObj[result].Y) == 0)
		{

		}

		this->m_BossKanturMapNumber = gObj[result].MapNumber;
		this->m_BossKanturMapX = gObj[result].X;
		this->m_BossKanturMapY = gObj[result].Y;

		gObj[result].m_PosNum = -1;
		gObj[result].TX = gObj[result].X;
		gObj[result].TY = gObj[result].Y;
		gObj[result].MTX = gObj[result].X;
		gObj[result].MTY = gObj[result].Y;
		gObj[result].m_OldX = gObj[result].X;
		gObj[result].m_OldY = gObj[result].Y;
		gObj[result].StartX = gObj[result].X;
		gObj[result].StartY = gObj[result].Y;

		if (It->second.m_Dir == (BYTE)-1)
		{
			gObj[result].Dir = rand() % 8;
		}

		else
		{
			gObj[result].Dir = It->second.m_Dir;
		}

		gObjSetMonster(result, It->second.m_Type);

		EnterCriticalSection(&this->m_MonCriti);

		ELEDORADO_MONSTER_INFO m_MonsterInfo;
		m_MonsterInfo.m_Index = result;

		this->m_mapMonsterInfo.insert(std::pair<int, ELEDORADO_MONSTER_INFO>(result, m_MonsterInfo));

		LeaveCriticalSection(&this->m_MonCriti);

		g_Log.Add("[Eledorado Event] Respawn Golden Tantalos : %d, %d,%d",
			MapNumber, gObj[result].X, gObj[result].Y);
	}

	It = this->m_vtMonsterPos.find(83);

	if (It == this->m_vtMonsterPos.end())
	{
		return;
	}

	for (int i = 0; i < It->second.m_Count; i++)
	{
		MapNumber = this->m_BossKanturMapNumber;
		int result = gObjAddMonster(MapNumber);

		if (result == -1)
		{
			g_Log.AddC(TColor::Red, "%s result == -1", __FUNCTION__);
			return;
		}

		gObj[result].MapNumber = MapNumber;

		this->GetBoxPosition(MapNumber, this->m_BossKanturMapX - 10, m_BossKanturMapY - 10,
			this->m_BossKanturMapX + 10, m_BossKanturMapY + 10, gObj[result].X, gObj[result].Y);

		gObj[result].m_PosNum = -1;
		gObj[result].TX = gObj[result].X;
		gObj[result].TY = gObj[result].Y;
		gObj[result].MTX = gObj[result].X;
		gObj[result].MTY = gObj[result].Y;
		gObj[result].m_OldX = gObj[result].X;
		gObj[result].m_OldY = gObj[result].Y;
		gObj[result].StartX = gObj[result].X;
		gObj[result].StartY = gObj[result].Y;

		if (It->second.m_Dir == (BYTE)-1)
		{
			gObj[result].Dir = rand() % 8;
		}

		else
		{
			gObj[result].Dir = It->second.m_Dir;
		}

		gObjSetMonster(result, It->second.m_Type);

		EnterCriticalSection(&this->m_MonCriti);

		ELEDORADO_MONSTER_INFO m_MonsterInfo;
		m_MonsterInfo.m_Index = result;

		this->m_mapMonsterInfo.insert(std::pair<int, ELEDORADO_MONSTER_INFO>(result, m_MonsterInfo));

		LeaveCriticalSection(&this->m_MonCriti);
	}
}

void CEledoradoEvent::CheckGoldDercon(int MapNumber)
{
	if (this->EventState == 0)
		return;

	BOOL EventOn = FALSE;
	int EventClearMapNumber = -1;

	for (int i = 0; i < 4; i++)
	{
		if (this->m_BossGoldDerconMapNumber[i] != -1)
		{
			if (this->m_BossGoldDerconMapNumber[i] == MapNumber)
			{
				EventOn = TRUE;
			}
			else
			{
				EventClearMapNumber = this->m_BossGoldDerconMapNumber[i];
			}
		}
	}

	if (EventClearMapNumber != -1)
	{
		GSProtocol.GCMapEventStateSend(EventClearMapNumber, 0, 3);
	}

	if (EventOn != FALSE)
	{
		GSProtocol.GCMapEventStateSend(MapNumber, 1, 3);
	}
	else
	{
		GSProtocol.GCMapEventStateSend(MapNumber, 0, 3);
	}
}

void CEledoradoEvent::Start_Menual()
{
	this->SetMenualStart(TRUE);
	this->ClearMonster();

	g_Log.Add("[Event Management] [Start] Eledorado Event!");

	this->RegenGoldGoblen();
	this->RegenTitan();
	this->RegenGoldDercon();;
	this->RegenDevilLizardKing();
	this->RegenKantur();
	this->RegenRabbit();
	this->RegenDarkKnight();
	this->RegenDevil();
	this->RegenDarkKnightAida();
	this->RegenCrust();
	this->RegenSatiros();
	this->RegenTwinTail();
	this->RegenIronKnight();
	this->RegenNeipin();
	this->RegenGreatDragon();

}

void CEledoradoEvent::End_Menual()
{
	this->SetMenualStart(FALSE);
	this->ClearMonster();
}

void CEledoradoEvent::RegenRabbit()
{
	std::map<int, ELEDORARO_MONSTER_POS>::iterator It = this->m_vtMonsterPos.find(502);

	if (It == this->m_vtMonsterPos.end())
	{
		return;
	}

	int MapNumber;

	for (int i = 0; i < It->second.m_Count; i++)
	{
		MapNumber = this->GetMapNumber(It);

		if (g_MapServerManager.CheckMapCanMove(MapNumber) == FALSE)
		{
			return;
		}

		if (MapNumber == -1)
		{
			g_Log.AddC(TColor::Red, "%s MapNumber == -1", __FUNCTION__);
			return;
		}

		int result = gObjAddMonster(MapNumber);

		if (result == -1)
		{
			g_Log.AddC(TColor::Red, "%s result == -1", __FUNCTION__);
			return;
		}

		gObj[result].MapNumber = MapNumber;

		while (this->GetBoxPosition(MapNumber, It->second.m_X, It->second.m_Y, It->second.m_W, It->second.m_H, gObj[result].X, gObj[result].Y) == 0)
		{

		}

		gObj[result].m_PosNum = -1;
		gObj[result].TX = gObj[result].X;
		gObj[result].TY = gObj[result].Y;
		gObj[result].MTX = gObj[result].X;
		gObj[result].MTY = gObj[result].Y;
		gObj[result].m_OldX = gObj[result].X;
		gObj[result].m_OldY = gObj[result].Y;
		gObj[result].StartX = gObj[result].X;
		gObj[result].StartY = gObj[result].Y;

		if (It->second.m_Dir == (BYTE)-1)
		{
			gObj[result].Dir = rand() % 8;
		}

		else
		{
			gObj[result].Dir = It->second.m_Dir;
		}

		gObjSetMonster(result, It->second.m_Type);

		EnterCriticalSection(&this->m_MonCriti);

		ELEDORADO_MONSTER_INFO m_MonsterInfo;
		m_MonsterInfo.m_Index = result;

		this->m_mapMonsterInfo.insert(std::pair<int, ELEDORADO_MONSTER_INFO>(result, m_MonsterInfo));

		LeaveCriticalSection(&this->m_MonCriti);

		g_Log.Add("[Eledorado Event] Respawn Golden Rabbit : %d, %d,%d",
			MapNumber, gObj[result].X, gObj[result].Y);
	}
}

void CEledoradoEvent::RegenDarkKnight()
{
	std::map<int, ELEDORARO_MONSTER_POS>::iterator It = this->m_vtMonsterPos.find(493);

	if (It == this->m_vtMonsterPos.end())
	{
		return;
	}

	int MapNumber;

	for (int i = 0; i < It->second.m_Count; i++)
	{
		MapNumber = this->GetMapNumber(It);

		if (g_MapServerManager.CheckMapCanMove(MapNumber) == FALSE)
		{
			return;
		}

		if (MapNumber == -1)
		{
			g_Log.AddC(TColor::Red, "%s MapNumber == -1", __FUNCTION__);
			return;
		}

		int result = gObjAddMonster(MapNumber);

		if (result == -1)
		{
			g_Log.AddC(TColor::Red, "%s result == -1", __FUNCTION__);
			return;
		}

		gObj[result].MapNumber = MapNumber;

		while (this->GetBoxPosition(MapNumber, It->second.m_X, It->second.m_Y, It->second.m_W, It->second.m_H, gObj[result].X, gObj[result].Y) == 0)
		{

		}

		gObj[result].m_PosNum = -1;
		gObj[result].TX = gObj[result].X;
		gObj[result].TY = gObj[result].Y;
		gObj[result].MTX = gObj[result].X;
		gObj[result].MTY = gObj[result].Y;
		gObj[result].m_OldX = gObj[result].X;
		gObj[result].m_OldY = gObj[result].Y;
		gObj[result].StartX = gObj[result].X;
		gObj[result].StartY = gObj[result].Y;

		if (It->second.m_Dir == (BYTE)-1)
		{
			gObj[result].Dir = rand() % 8;
		}

		else
		{
			gObj[result].Dir = It->second.m_Dir;
		}

		gObjSetMonster(result, It->second.m_Type);

		EnterCriticalSection(&this->m_MonCriti);

		ELEDORADO_MONSTER_INFO m_MonsterInfo;
		m_MonsterInfo.m_Index = result;

		this->m_mapMonsterInfo.insert(std::pair<int, ELEDORADO_MONSTER_INFO>(result, m_MonsterInfo));

		LeaveCriticalSection(&this->m_MonCriti);

		g_Log.Add("[Eledorado Event] Respawn Golden Dark Knight : %d, %d,%d",
			MapNumber, gObj[result].X, gObj[result].Y);
	}
}

void CEledoradoEvent::RegenDevil()
{
	std::map<int, ELEDORARO_MONSTER_POS>::iterator It = this->m_vtMonsterPos.find(494);

	if (It == this->m_vtMonsterPos.end())
	{
		return;
	}

	int MapNumber;

	for (int i = 0; i < It->second.m_Count; i++)
	{
		MapNumber = this->GetMapNumber(It);

		if (g_MapServerManager.CheckMapCanMove(MapNumber) == FALSE)
		{
			return;
		}

		if (MapNumber == -1)
		{
			g_Log.AddC(TColor::Red, "%s MapNumber == -1", __FUNCTION__);
			return;
		}

		int result = gObjAddMonster(MapNumber);

		if (result == -1)
		{
			g_Log.AddC(TColor::Red, "%s result == -1", __FUNCTION__);
			return;
		}

		gObj[result].MapNumber = MapNumber;

		while (this->GetBoxPosition(MapNumber, It->second.m_X, It->second.m_Y, It->second.m_W, It->second.m_H, gObj[result].X, gObj[result].Y) == 0)
		{

		}

		gObj[result].m_PosNum = -1;
		gObj[result].TX = gObj[result].X;
		gObj[result].TY = gObj[result].Y;
		gObj[result].MTX = gObj[result].X;
		gObj[result].MTY = gObj[result].Y;
		gObj[result].m_OldX = gObj[result].X;
		gObj[result].m_OldY = gObj[result].Y;
		gObj[result].StartX = gObj[result].X;
		gObj[result].StartY = gObj[result].Y;

		if (It->second.m_Dir == (BYTE)-1)
		{
			gObj[result].Dir = rand() % 8;
		}

		else
		{
			gObj[result].Dir = It->second.m_Dir;
		}

		gObjSetMonster(result, It->second.m_Type);

		EnterCriticalSection(&this->m_MonCriti);

		ELEDORADO_MONSTER_INFO m_MonsterInfo;
		m_MonsterInfo.m_Index = result;

		this->m_mapMonsterInfo.insert(std::pair<int, ELEDORADO_MONSTER_INFO>(result, m_MonsterInfo));

		LeaveCriticalSection(&this->m_MonCriti);

		g_Log.Add("[Eledorado Event] Respawn Golden Devil : %d, %d,%d",
			MapNumber, gObj[result].X, gObj[result].Y);
	}
}

void CEledoradoEvent::RegenDarkKnightAida()
{
	std::map<int, ELEDORARO_MONSTER_POS>::iterator It = this->m_vtMonsterPos.find(495);

	if (It == this->m_vtMonsterPos.end())
	{
		return;
	}

	int MapNumber;

	for (int i = 0; i < It->second.m_Count; i++)
	{
		MapNumber = this->GetMapNumber(It);

		if (g_MapServerManager.CheckMapCanMove(MapNumber) == FALSE)
		{
			return;
		}

		if (MapNumber == -1)
		{
			g_Log.AddC(TColor::Red, "%s MapNumber == -1", __FUNCTION__);
			return;
		}

		int result = gObjAddMonster(MapNumber);

		if (result == -1)
		{
			g_Log.AddC(TColor::Red, "%s result == -1", __FUNCTION__);
			return;
		}

		gObj[result].MapNumber = MapNumber;

		while (this->GetBoxPosition(MapNumber, It->second.m_X, It->second.m_Y, It->second.m_W, It->second.m_H, gObj[result].X, gObj[result].Y) == 0)
		{

		}

		gObj[result].m_PosNum = -1;
		gObj[result].TX = gObj[result].X;
		gObj[result].TY = gObj[result].Y;
		gObj[result].MTX = gObj[result].X;
		gObj[result].MTY = gObj[result].Y;
		gObj[result].m_OldX = gObj[result].X;
		gObj[result].m_OldY = gObj[result].Y;
		gObj[result].StartX = gObj[result].X;
		gObj[result].StartY = gObj[result].Y;

		if (It->second.m_Dir == (BYTE)-1)
		{
			gObj[result].Dir = rand() % 8;
		}

		else
		{
			gObj[result].Dir = It->second.m_Dir;
		}

		gObjSetMonster(result, It->second.m_Type);

		EnterCriticalSection(&this->m_MonCriti);

		ELEDORADO_MONSTER_INFO m_MonsterInfo;
		m_MonsterInfo.m_Index = result;

		this->m_mapMonsterInfo.insert(std::pair<int, ELEDORADO_MONSTER_INFO>(result, m_MonsterInfo));

		LeaveCriticalSection(&this->m_MonCriti);

		g_Log.Add("[Eledorado Event] Respawn Golden Dark Knight : %d, %d,%d",
			MapNumber, gObj[result].X, gObj[result].Y);
	}
}

void CEledoradoEvent::RegenCrust()
{
	std::map<int, ELEDORARO_MONSTER_POS>::iterator It = this->m_vtMonsterPos.find(496);

	if (It == this->m_vtMonsterPos.end())
	{
		return;
	}

	int MapNumber;

	for (int i = 0; i < It->second.m_Count; i++)
	{
		MapNumber = this->GetMapNumber(It);

		if (g_MapServerManager.CheckMapCanMove(MapNumber) == FALSE)
		{
			return;
		}

		if (MapNumber == -1)
		{
			g_Log.AddC(TColor::Red, "%s MapNumber == -1", __FUNCTION__);
			return;
		}

		int result = gObjAddMonster(MapNumber);

		if (result == -1)
		{
			g_Log.AddC(TColor::Red, "%s result == -1", __FUNCTION__);
			return;
		}

		gObj[result].MapNumber = MapNumber;

		while (this->GetBoxPosition(MapNumber, It->second.m_X, It->second.m_Y, It->second.m_W, It->second.m_H, gObj[result].X, gObj[result].Y) == 0)
		{

		}

		gObj[result].m_PosNum = -1;
		gObj[result].TX = gObj[result].X;
		gObj[result].TY = gObj[result].Y;
		gObj[result].MTX = gObj[result].X;
		gObj[result].MTY = gObj[result].Y;
		gObj[result].m_OldX = gObj[result].X;
		gObj[result].m_OldY = gObj[result].Y;
		gObj[result].StartX = gObj[result].X;
		gObj[result].StartY = gObj[result].Y;

		if (It->second.m_Dir == (BYTE)-1)
		{
			gObj[result].Dir = rand() % 8;
		}

		else
		{
			gObj[result].Dir = It->second.m_Dir;
		}

		gObjSetMonster(result, It->second.m_Type);

		EnterCriticalSection(&this->m_MonCriti);

		ELEDORADO_MONSTER_INFO m_MonsterInfo;
		m_MonsterInfo.m_Index = result;

		this->m_mapMonsterInfo.insert(std::pair<int, ELEDORADO_MONSTER_INFO>(result, m_MonsterInfo));

		LeaveCriticalSection(&this->m_MonCriti);

		g_Log.Add("[Eledorado Event] Respawn Golden Crust : %d, %d,%d",
			MapNumber, gObj[result].X, gObj[result].Y);
	}
}

void CEledoradoEvent::RegenSatiros()
{
	std::map<int, ELEDORARO_MONSTER_POS>::iterator It = this->m_vtMonsterPos.find(497);

	if (It == this->m_vtMonsterPos.end())
	{
		return;
	}

	int MapNumber;

	for (int i = 0; i < It->second.m_Count; i++)
	{
		MapNumber = this->GetMapNumber(It);

		if (g_MapServerManager.CheckMapCanMove(MapNumber) == FALSE)
		{
			return;
		}

		if (MapNumber == -1)
		{
			g_Log.AddC(TColor::Red, "%s MapNumber == -1", __FUNCTION__);
			return;
		}

		int result = gObjAddMonster(MapNumber);

		if (result == -1)
		{
			g_Log.AddC(TColor::Red, "%s result == -1", __FUNCTION__);
			return;
		}

		gObj[result].MapNumber = MapNumber;

		while (this->GetBoxPosition(MapNumber, It->second.m_X, It->second.m_Y, It->second.m_W, It->second.m_H, gObj[result].X, gObj[result].Y) == 0)
		{

		}

		gObj[result].m_PosNum = -1;
		gObj[result].TX = gObj[result].X;
		gObj[result].TY = gObj[result].Y;
		gObj[result].MTX = gObj[result].X;
		gObj[result].MTY = gObj[result].Y;
		gObj[result].m_OldX = gObj[result].X;
		gObj[result].m_OldY = gObj[result].Y;
		gObj[result].StartX = gObj[result].X;
		gObj[result].StartY = gObj[result].Y;

		if (It->second.m_Dir == (BYTE)-1)
		{
			gObj[result].Dir = rand() % 8;
		}

		else
		{
			gObj[result].Dir = It->second.m_Dir;
		}

		gObjSetMonster(result, It->second.m_Type);

		EnterCriticalSection(&this->m_MonCriti);

		ELEDORADO_MONSTER_INFO m_MonsterInfo;
		m_MonsterInfo.m_Index = result;

		this->m_mapMonsterInfo.insert(std::pair<int, ELEDORADO_MONSTER_INFO>(result, m_MonsterInfo));

		LeaveCriticalSection(&this->m_MonCriti);

		g_Log.Add("[Eledorado Event] Respawn Golden Satiros : %d, %d,%d",
			MapNumber, gObj[result].X, gObj[result].Y);
	}
}

void CEledoradoEvent::RegenTwinTail()
{
	std::map<int, ELEDORARO_MONSTER_POS>::iterator It = this->m_vtMonsterPos.find(498);

	if (It == this->m_vtMonsterPos.end())
	{
		return;
	}

	int MapNumber;

	for (int i = 0; i < It->second.m_Count; i++)
	{
		MapNumber = this->GetMapNumber(It);

		if (g_MapServerManager.CheckMapCanMove(MapNumber) == FALSE)
		{
			return;
		}

		if (MapNumber == -1)
		{
			g_Log.AddC(TColor::Red, "%s MapNumber == -1", __FUNCTION__);
			return;
		}

		int result = gObjAddMonster(MapNumber);

		if (result == -1)
		{
			g_Log.AddC(TColor::Red, "%s result == -1", __FUNCTION__);
			return;
		}

		gObj[result].MapNumber = MapNumber;

		while (this->GetBoxPosition(MapNumber, It->second.m_X, It->second.m_Y, It->second.m_W, It->second.m_H, gObj[result].X, gObj[result].Y) == 0)
		{

		}

		gObj[result].m_PosNum = -1;
		gObj[result].TX = gObj[result].X;
		gObj[result].TY = gObj[result].Y;
		gObj[result].MTX = gObj[result].X;
		gObj[result].MTY = gObj[result].Y;
		gObj[result].m_OldX = gObj[result].X;
		gObj[result].m_OldY = gObj[result].Y;
		gObj[result].StartX = gObj[result].X;
		gObj[result].StartY = gObj[result].Y;

		if (It->second.m_Dir == (BYTE)-1)
		{
			gObj[result].Dir = rand() % 8;
		}

		else
		{
			gObj[result].Dir = It->second.m_Dir;
		}

		gObjSetMonster(result, It->second.m_Type);

		EnterCriticalSection(&this->m_MonCriti);

		ELEDORADO_MONSTER_INFO m_MonsterInfo;
		m_MonsterInfo.m_Index = result;

		this->m_mapMonsterInfo.insert(std::pair<int, ELEDORADO_MONSTER_INFO>(result, m_MonsterInfo));

		LeaveCriticalSection(&this->m_MonCriti);

		g_Log.Add("[Eledorado Event] Respawn Golden TwinTail : %d, %d,%d",
			MapNumber, gObj[result].X, gObj[result].Y);
	}
}

void CEledoradoEvent::RegenIronKnight()
{
	std::map<int, ELEDORARO_MONSTER_POS>::iterator It = this->m_vtMonsterPos.find(499);

	if (It == this->m_vtMonsterPos.end())
	{
		return;
	}

	int MapNumber;

	for (int i = 0; i < It->second.m_Count; i++)
	{
		MapNumber = this->GetMapNumber(It);

		if (g_MapServerManager.CheckMapCanMove(MapNumber) == FALSE)
		{
			return;
		}

		if (MapNumber == -1)
		{
			g_Log.AddC(TColor::Red, "%s MapNumber == -1", __FUNCTION__);
			return;
		}

		int result = gObjAddMonster(MapNumber);

		if (result == -1)
		{
			g_Log.AddC(TColor::Red, "%s result == -1", __FUNCTION__);
			return;
		}

		gObj[result].MapNumber = MapNumber;

		while (this->GetBoxPosition(MapNumber, It->second.m_X, It->second.m_Y, It->second.m_W, It->second.m_H, gObj[result].X, gObj[result].Y) == 0)
		{

		}

		gObj[result].m_PosNum = -1;
		gObj[result].TX = gObj[result].X;
		gObj[result].TY = gObj[result].Y;
		gObj[result].MTX = gObj[result].X;
		gObj[result].MTY = gObj[result].Y;
		gObj[result].m_OldX = gObj[result].X;
		gObj[result].m_OldY = gObj[result].Y;
		gObj[result].StartX = gObj[result].X;
		gObj[result].StartY = gObj[result].Y;

		if (It->second.m_Dir == (BYTE)-1)
		{
			gObj[result].Dir = rand() % 8;
		}

		else
		{
			gObj[result].Dir = It->second.m_Dir;
		}

		gObjSetMonster(result, It->second.m_Type);

		EnterCriticalSection(&this->m_MonCriti);

		ELEDORADO_MONSTER_INFO m_MonsterInfo;
		m_MonsterInfo.m_Index = result;

		this->m_mapMonsterInfo.insert(std::pair<int, ELEDORADO_MONSTER_INFO>(result, m_MonsterInfo));

		LeaveCriticalSection(&this->m_MonCriti);

		g_Log.Add("[Eledorado Event] Respawn Golden Iron Knight : %d, %d,%d",
			MapNumber, gObj[result].X, gObj[result].Y);
	}
}

void CEledoradoEvent::RegenNeipin()
{
	std::map<int, ELEDORARO_MONSTER_POS>::iterator It = this->m_vtMonsterPos.find(500);

	if (It == this->m_vtMonsterPos.end())
	{
		return;
	}

	int MapNumber;

	for (int i = 0; i < It->second.m_Count; i++)
	{
		MapNumber = this->GetMapNumber(It);

		if (g_MapServerManager.CheckMapCanMove(MapNumber) == FALSE)
		{
			return;
		}

		if (MapNumber == -1)
		{
			g_Log.AddC(TColor::Red, "%s MapNumber == -1", __FUNCTION__);
			return;
		}

		int result = gObjAddMonster(MapNumber);

		if (result == -1)
		{
			g_Log.AddC(TColor::Red, "%s result == -1", __FUNCTION__);
			return;
		}

		gObj[result].MapNumber = MapNumber;

		while (this->GetBoxPosition(MapNumber, It->second.m_X, It->second.m_Y, It->second.m_W, It->second.m_H, gObj[result].X, gObj[result].Y) == 0)
		{

		}

		gObj[result].m_PosNum = -1;
		gObj[result].TX = gObj[result].X;
		gObj[result].TY = gObj[result].Y;
		gObj[result].MTX = gObj[result].X;
		gObj[result].MTY = gObj[result].Y;
		gObj[result].m_OldX = gObj[result].X;
		gObj[result].m_OldY = gObj[result].Y;
		gObj[result].StartX = gObj[result].X;
		gObj[result].StartY = gObj[result].Y;

		if (It->second.m_Dir == (BYTE)-1)
		{
			gObj[result].Dir = rand() % 8;
		}

		else
		{
			gObj[result].Dir = It->second.m_Dir;
		}

		gObjSetMonster(result, It->second.m_Type);

		EnterCriticalSection(&this->m_MonCriti);

		ELEDORADO_MONSTER_INFO m_MonsterInfo;
		m_MonsterInfo.m_Index = result;

		this->m_mapMonsterInfo.insert(std::pair<int, ELEDORADO_MONSTER_INFO>(result, m_MonsterInfo));

		LeaveCriticalSection(&this->m_MonCriti);

		g_Log.Add("[Eledorado Event] Respawn Golden Neipin : %d, %d,%d",
			MapNumber, gObj[result].X, gObj[result].Y);
	}
}

void CEledoradoEvent::RegenGreatDragon()
{
	std::map<int, ELEDORARO_MONSTER_POS>::iterator It = this->m_vtMonsterPos.find(501);

	if (It == this->m_vtMonsterPos.end())
	{
		return;
	}

	int MapNumber;

	for (int i = 0; i < It->second.m_Count; i++)
	{
		MapNumber = this->GetMapNumber(It);

		if (g_MapServerManager.CheckMapCanMove(MapNumber) == FALSE)
		{
			return;
		}

		if (MapNumber == -1)
		{
			g_Log.AddC(TColor::Red, "%s MapNumber == -1", __FUNCTION__);
			return;
		}

		int result = gObjAddMonster(MapNumber);

		if (result == -1)
		{
			g_Log.AddC(TColor::Red, "%s result == -1", __FUNCTION__);
			return;
		}

		gObj[result].MapNumber = MapNumber;

		while (this->GetBoxPosition(MapNumber, It->second.m_X, It->second.m_Y, It->second.m_W, It->second.m_H, gObj[result].X, gObj[result].Y) == 0)
		{

		}

		gObj[result].m_PosNum = -1;
		gObj[result].TX = gObj[result].X;
		gObj[result].TY = gObj[result].Y;
		gObj[result].MTX = gObj[result].X;
		gObj[result].MTY = gObj[result].Y;
		gObj[result].m_OldX = gObj[result].X;
		gObj[result].m_OldY = gObj[result].Y;
		gObj[result].StartX = gObj[result].X;
		gObj[result].StartY = gObj[result].Y;

		if (It->second.m_Dir == (BYTE)-1)
		{
			gObj[result].Dir = rand() % 8;
		}

		else
		{
			gObj[result].Dir = It->second.m_Dir;
		}

		gObjSetMonster(result, It->second.m_Type);

		EnterCriticalSection(&this->m_MonCriti);

		ELEDORADO_MONSTER_INFO m_MonsterInfo;
		m_MonsterInfo.m_Index = result;

		this->m_mapMonsterInfo.insert(std::pair<int, ELEDORADO_MONSTER_INFO>(result, m_MonsterInfo));

		LeaveCriticalSection(&this->m_MonCriti);

		g_Log.Add("[Eledorado Event] Respawn Golden GreatDragon : %d, %d,%d",
			MapNumber, gObj[result].X, gObj[result].Y);
	}
}

BOOL CEledoradoEvent::GetBoxPosition(int mapnumber, int ax, int ay, int aw, int ah, short &mx, short &my)
{
	int count = 100;
	int w;
	int h;
	int tx;
	int ty;
	BYTE attr;

	while (count-- != 0)
	{
		w = aw - ax;
		h = ah - ay;

		__try
		{
			tx = ax + (rand() % w);
			ty = ay + (rand() % h);
		}
		__except (w = 1, h = 1, 1)
		{

		}

		attr = MapC[mapnumber].GetAttr(tx, ty);

		if (((attr & 1) != 1) && ((attr & 4) != 4) && ((attr & 8) != 8))
		{
			mx = tx;
			my = ty;
			return TRUE;
		}
	}

	return false;
}

BYTE CEledoradoEvent::GetMapNumber(std::map<int, ELEDORARO_MONSTER_POS>::iterator Iter)
{
	int count = 100;
	BYTE MapNumber = -1;

	while (count-- != 0)
	{
		MapNumber = Iter->second.m_MapNumber[rand() % 4];

		if (MapNumber != (BYTE)-1)
		{
			break;
		}
	}

	if (MapNumber == (BYTE)-1)
	{
		MapNumber = Iter->second.m_MapNumber[0];
	}

	return MapNumber;
}

ELEDORARO_MONSTER_POS * CEledoradoEvent::GetMonsterPos(int iMonsterIndex)
{
	std::map<int, ELEDORARO_MONSTER_POS>::iterator It = this->m_vtMonsterPos.find(iMonsterIndex);

	if (It == this->m_vtMonsterPos.end())
	{
		return nullptr;
	}

	return &It->second;
}

bool CEledoradoEvent::IsEledoradoMonster(int iIndex)
{
	std::map<int, ELEDORADO_MONSTER_INFO>::iterator It = this->m_mapMonsterInfo.find(iIndex);

	if (It == this->m_mapMonsterInfo.end())
	{
		return false;
	}

	return true;
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


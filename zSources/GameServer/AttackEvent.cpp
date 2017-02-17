//////////////////////////////////////////////////////////////////////
// AttackEvent.cpp
// ------------------------------
// Decompiled by Deathway
// Date : 2007-05-09
// ------------------------------
// GS-N 0.99.60T 0x0045B180 - Completed
// GS-N 1.00.18	0x0046AA80 - Completed
#include "stdafx.h"
#include "AttackEvent.h"
#include "Gamemain.h"
#include "TLog.h"
#include "MapServerManager.h"

CAttackEvent::CAttackEvent()
{
	InitializeCriticalSection(&this->m_MonCriti);
	this->Init();
}

CAttackEvent::~CAttackEvent()
{
	DeleteCriticalSection(&this->m_MonCriti);
	return;
}

void CAttackEvent::ClearMonster()
{
	EnterCriticalSection(&this->m_MonCriti);

	for (std::map<int, ATTACKEVENT_MONSTER_INFO>::iterator It = this->m_mapMonsterInfo.begin(); It != this->m_mapMonsterInfo.end(); It++)
	{
		gObjDel(It->second.m_nIndex);
	}

	this->m_mapMonsterInfo.clear();

	LeaveCriticalSection(&this->m_MonCriti);
}

void CAttackEvent::LoadScript(char *szFile)
{
	this->m_mapMonsterPos.clear();

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(szFile);

	if (res.status != pugi::status_ok)
	{
		g_Log.MsgBox("%s file load fail (%s)", szFile, res.description());
		return;
	}

	pugi::xml_node main = file.child("AttackEvent");

	for (pugi::xml_node spawn = main.child("Spawn"); spawn; spawn = spawn.next_sibling())
	{
		ATTACKEVENT_MONSTER_POS m_Pos;

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

		this->m_mapMonsterPos.insert(std::pair<int, ATTACKEVENT_MONSTER_POS>(m_Pos.m_Type, m_Pos));
	}
}

void CAttackEvent::Init()
{
	this->EventState = false;
	this->EventStartTime = GetTickCount64();
	this->m_MapNumber = MAP_INDEX_RORENCIA;
	this->ClearMonster();
}

void CAttackEvent::Start()
{
	this->EventState = true;
	this->EventStartTime = GetTickCount64(); 
	this->ClearMonster();
}

void CAttackEvent::End()
{
	this->ClearMonster();
}

void CAttackEvent::Run()
{
	return;
}

void CAttackEvent::Active()
{
	std::map<int, ATTACKEVENT_MONSTER_POS>::iterator It = this->m_mapMonsterPos.find(53);

	if (It == this->m_mapMonsterPos.end())
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

		this->m_BossMap53 = gObj[result].MapNumber;
		this->m_BossMapX53 = gObj[result].X;
		this->m_BossMapY53 = gObj[result].Y;

		gObjSetMonster(result, It->second.m_Type);

		EnterCriticalSection(&this->m_MonCriti);

		ATTACKEVENT_MONSTER_INFO m_MonsterInfo;
		m_MonsterInfo.m_nIndex = result;

		this->m_mapMonsterInfo.insert(std::pair<int, ATTACKEVENT_MONSTER_INFO>(result, m_MonsterInfo));

		LeaveCriticalSection(&this->m_MonCriti);

		g_Log.Add("[AttackEvent] Create Boss %d: %d %d %d create ", It->second.m_Type, this->m_BossMap53, this->m_BossMapX53, this->m_BossMapY53);
	}

	It = this->m_mapMonsterPos.find(54);

	if (It == this->m_mapMonsterPos.end())
	{
		return;
	}

	for (int i = 0; i < It->second.m_Count; i++)
	{
		MapNumber = this->m_BossMap53;
		int result = gObjAddMonster(MapNumber);

		if (result == -1)
		{
			g_Log.AddC(TColor::Red, "%s result == -1", __FUNCTION__);
			return;
		}

		gObj[result].MapNumber = MapNumber;

		this->GetBoxPosition(MapNumber, this->m_BossMapX53 - 4, this->m_BossMapY53 - 4, this->m_BossMapX53 + 4, this->m_BossMapY53 + 4, gObj[result].X, gObj[result].Y);

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

		ATTACKEVENT_MONSTER_INFO m_MonsterInfo;
		m_MonsterInfo.m_nIndex = result;

		this->m_mapMonsterInfo.insert(std::pair<int, ATTACKEVENT_MONSTER_INFO>(result, m_MonsterInfo));

		LeaveCriticalSection(&this->m_MonCriti);
	}

	It = this->m_mapMonsterPos.find(55);

	if (It == this->m_mapMonsterPos.end())
	{
		return;
	}

	for (int i = 0; i < It->second.m_Count; i++)
	{
		MapNumber = this->GetMapNumber(It);

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

		this->m_BossMap55 = gObj[result].MapNumber;
		this->m_BossMapX55 = gObj[result].X;
		this->m_BossMapY55 = gObj[result].Y;

		gObjSetMonster(result, It->second.m_Type);

		EnterCriticalSection(&this->m_MonCriti);

		ATTACKEVENT_MONSTER_INFO m_MonsterInfo;
		m_MonsterInfo.m_nIndex = result;

		this->m_mapMonsterInfo.insert(std::pair<int, ATTACKEVENT_MONSTER_INFO>(result, m_MonsterInfo));

		LeaveCriticalSection(&this->m_MonCriti);

		g_Log.Add("[AttackEvent] Create Boss %d: %d %d %d create ", It->second.m_Type, this->m_BossMap55, this->m_BossMapX55, this->m_BossMapY55);
	}

	It = this->m_mapMonsterPos.find(56);

	if (It == this->m_mapMonsterPos.end())
	{
		return;
	}

	for (int i = 0; i < It->second.m_Count; i++)
	{
		MapNumber = this->m_BossMap55;
		int result = gObjAddMonster(MapNumber);

		if (result == -1)
		{
			g_Log.AddC(TColor::Red, "%s result == -1", __FUNCTION__);
			return;
		}

		gObj[result].MapNumber = MapNumber;

		this->GetBoxPosition(MapNumber, this->m_BossMapX55 - 4, this->m_BossMapY55 - 4, this->m_BossMapX55 + 4, this->m_BossMapY55 + 4, gObj[result].X, gObj[result].Y);

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

		ATTACKEVENT_MONSTER_INFO m_MonsterInfo;
		m_MonsterInfo.m_nIndex = result;

		this->m_mapMonsterInfo.insert(std::pair<int, ATTACKEVENT_MONSTER_INFO>(result, m_MonsterInfo));

		LeaveCriticalSection(&this->m_MonCriti);
	}
}





void CAttackEvent::Start_Menual() 
{
	this->SetMenualStart(TRUE);

	g_Log.Add("[Event Management] [Start] Attack Event!"); 
	this->EventState= false;
	this->Start();
	this->Active();
}



void CAttackEvent::End_Menual()
{
	this->SetMenualStart(FALSE);
	this->End();
}

BOOL CAttackEvent::GetBoxPosition(int mapnumber, int ax, int ay, int aw, int ah, short &mx, short &my)
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

BYTE CAttackEvent::GetMapNumber(std::map<int, ATTACKEVENT_MONSTER_POS>::iterator Iter)
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

ATTACKEVENT_MONSTER_POS * CAttackEvent::GetMonsterPos(int iMonsterIndex)
{
	std::map<int, ATTACKEVENT_MONSTER_POS>::iterator It = this->m_mapMonsterPos.find(iMonsterIndex);

	if (It == this->m_mapMonsterPos.end())
	{
		return nullptr;
	}

	return &It->second;
}

bool CAttackEvent::IsAttackEventMonster(int iIndex)
{
	std::map<int, ATTACKEVENT_MONSTER_INFO>::iterator It = this->m_mapMonsterInfo.find(iIndex);

	if (It == this->m_mapMonsterInfo.end())
	{
		return false;
	}

	return true;
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


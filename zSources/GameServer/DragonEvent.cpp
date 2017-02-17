//////////////////////////////////////////////////////////////////////
// DragonEvent.cpp
// ------------------------------
// Decompiled by Deathway
// Date : 2007-05-09
// ------------------------------
// GS-N 0.99.60T 45a900
// GS-N	1.00.18	JPN	0x0046A1F0	-	Completed
#include "stdafx.h"
#include "DragonEvent.h"
#include "GameMain.h"
#include "TLog.h"
#include "Protocol.h"
#include "MapServerManager.h"

CDragonEvent::CDragonEvent()
{
	InitializeCriticalSection(&this->m_MonCriti);
	this->Init();
}


CDragonEvent::~CDragonEvent()
{
	DeleteCriticalSection(&this->m_MonCriti);
}

void CDragonEvent::ClearMonster()
{
	EnterCriticalSection(&this->m_MonCriti);

	for (std::map<int, DRAGON_MONSTER_INFO>::iterator It = this->m_mapMonsterInfo.begin(); It != this->m_mapMonsterInfo.end(); It++)
	{
		gObjDel(It->second.m_nIndex);
	}

	this->m_mapMonsterInfo.clear();

	LeaveCriticalSection(&this->m_MonCriti);
}

void CDragonEvent::LoadScript(char *szFile)
{
	this->ClearMonster();
	this->m_mapEventInfo.clear();

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(szFile);

	if (res.status != pugi::status_ok)
	{
		g_Log.MsgBox("%s load fail (%s)", szFile, res.description());
		return;
	}

	DRAGON_EVENT_INFO m_Info;

	pugi::xml_node main = file.child("DragonEvent");

	for (pugi::xml_node monster = main.child("Monster"); monster; monster = monster.next_sibling())
	{
		m_Info.m_Type = monster.attribute("Index").as_int();
		m_Info.m_Dis = monster.attribute("Distance").as_int();
		m_Info.m_Count = monster.attribute("Count").as_int();

		for (pugi::xml_node map = monster.child("Map"); map; map = map.next_sibling())
		{
			DRAGON_MAP_INFO m_MapInfo;

			m_MapInfo.m_MapNumber = map.attribute("Number").as_int();

			for (pugi::xml_node spawn = map.child("Spawn"); spawn; spawn = spawn.next_sibling())
			{
				DRAGON_SPAWN_INFO m_Spawn;

				m_Spawn.m_X = spawn.attribute("StartX").as_int();
				m_Spawn.m_Y = spawn.attribute("StartY").as_int();
				m_Spawn.m_W = spawn.attribute("EndX").as_int();
				m_Spawn.m_H = spawn.attribute("EndY").as_int();

				m_MapInfo.m_SpawnInfo.push_back(m_Spawn);
			}

			m_Info.m_MapInfo.push_back(m_MapInfo);
		}

		this->m_mapEventInfo.insert(std::pair<int, DRAGON_EVENT_INFO>(m_Info.m_Type, m_Info)); 
	}
}

void CDragonEvent::Init()
{
	this->EventState= 0;
	this->EventStartTime = 0;
	this->m_MapNumber = 0;
	this->m_bMenualStart = 0;
	this->ClearMonster();
}

void CDragonEvent::Start()
{
	char szTemp[256];

	if ( this->EventState != 0 )
	{
		return;
	}

	this->Init();

	std::map<int, DRAGON_EVENT_INFO>::iterator It = this->m_mapEventInfo.find(44);

	if (It == this->m_mapEventInfo.end())
	{
		return;
	}

	int MapNumber = It->second.m_MapInfo[rand() % It->second.m_MapInfo.size()].m_MapNumber;

	this->EventState = 1;
	this->m_MapNumber = MapNumber;

	strcpy(szTemp, Lang.GetMap(0, MapNumber));
	strcat(szTemp, ", ");
	strcat(szTemp, Lang.GetText(0,63));

	GSProtocol.AllSendServerMsg(szTemp);
	this->EventStartTime = GetTickCount64();
}
	
	
void CDragonEvent::End()
{
	GSProtocol.GCMapEventStateSend(this->m_MapNumber, 0, 1);
	g_Log.Add("[Red Dragon] Event End");
	this->EventState=0;
	this->ClearMonster();
}

void CDragonEvent::DragonActive()
{
	std::map<int, DRAGON_EVENT_INFO>::iterator It = this->m_mapEventInfo.find(44);

	if (It == this->m_mapEventInfo.end())
	{
		return;
	}

	for (int i = 0; i < It->second.m_Count; i++)
	{

		if (g_MapServerManager.CheckMapCanMove(this->m_MapNumber) == FALSE)
		{
			return;
		}

		int result = gObjAddMonster(this->m_MapNumber);

		if (result == -1)
		{
			g_Log.AddC(TColor::Red, "%s result == -1", __FUNCTION__);
			return;
		}

		gObj[result].MapNumber = this->m_MapNumber;

		DRAGON_SPAWN_INFO SpawnInfo;
		bool bSpawnFind = false;

		for (std::vector<DRAGON_MAP_INFO>::iterator ItMap = It->second.m_MapInfo.begin(); ItMap != It->second.m_MapInfo.end(); ItMap++)
		{
			if (ItMap->m_MapNumber == this->m_MapNumber)
			{
				SpawnInfo = ItMap->m_SpawnInfo[rand() % ItMap->m_SpawnInfo.size()];
				bSpawnFind = true;
				break;
			}
		}

		if (bSpawnFind == false)
		{
			gObjDel(result);
			g_Log.AddC(TColor::Red, "%s bSpawnFind == false", __FUNCTION__);
			return;
		}

		this->GetBoxPosition(this->m_MapNumber, SpawnInfo.m_X, SpawnInfo.m_Y, SpawnInfo.m_W, SpawnInfo.m_H, gObj[result].X, gObj[result].Y);

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

		DRAGON_MONSTER_INFO m_MonsterInfo;
		m_MonsterInfo.m_nIndex = result;

		this->m_mapMonsterInfo.insert(std::pair<int, DRAGON_MONSTER_INFO>(result, m_MonsterInfo));

		LeaveCriticalSection(&this->m_MonCriti);
	}
}



void CDragonEvent::Run()
{
	if ( this->m_bMenualStart != FALSE )
	{
		return;
	}

	if ( this->EventState == 0 )
	{
		return;
	}

	if ( this->EventState == 1 )
	{
		if ( ( GetTickCount64() - this->EventStartTime ) > 3000 )
		{
			this->EventState = 2;
			GSProtocol.GCMapEventStateSend(this->m_MapNumber , 1, 1 );
			g_Log.Add("[Red Dragon] Event Start");
			this->DragonActive();
			this->EventStartTime = GetTickCount64();
			return;
		}
	}
	else
	{
		if ( this->EventState == 2 )
		{
			if ( (GetTickCount64() - this->EventStartTime ) > 300000 )
			{
				this->End();
			}
		}
	}
}



void CDragonEvent::Start_Menual()
{
	this->SetMenualStart(TRUE);
	g_Log.Add("[Event Management] [Start] Red Dragon Event");
	this->EventState=0;
	this->Start();
}

			

void CDragonEvent::End_Menual()
{
	this->SetMenualStart(FALSE);
	this->End();
}

DRAGON_EVENT_INFO * CDragonEvent::GetMonsterPos(int iMonsterIndex)
{
	std::map<int, DRAGON_EVENT_INFO>::iterator It = this->m_mapEventInfo.find(iMonsterIndex);

	if (It == this->m_mapEventInfo.end())
	{
		return nullptr;
	}

	return &It->second;
}

BOOL CDragonEvent::GetBoxPosition(int mapnumber, int ax, int ay, int aw, int ah, short & mx, short & my)
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

bool CDragonEvent::IsDragonEventMonster(int iIndex)
{
	std::map<int, DRAGON_MONSTER_INFO>::iterator It = this->m_mapMonsterInfo.find(iIndex);

	if (It == this->m_mapMonsterInfo.end())
	{
		return false;
	}

	return true;
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


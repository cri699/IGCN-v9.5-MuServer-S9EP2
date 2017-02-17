//////////////////////////////////////////////////////////////////////
// DragonEvent.h
// ------------------------------
// Decompiled by Deathway
// Date : 2007-05-09
// ------------------------------
#ifndef DRAGONEVENT_H
#define DRAGONEVENT_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct DRAGON_SPAWN_INFO
{
	BYTE m_X;	// 6
	BYTE m_Y;	// 7
	BYTE m_W;	// 9
	BYTE m_H;	// A
};

struct DRAGON_MAP_INFO
{
	BYTE m_MapNumber;
	std::vector<DRAGON_SPAWN_INFO> m_SpawnInfo;
};

struct DRAGON_EVENT_INFO
{
	WORD m_Type;
	BYTE m_Dis;
	BYTE m_Dir;
	BYTE m_Count;
	std::vector<DRAGON_MAP_INFO> m_MapInfo;
};

struct DRAGON_MONSTER_INFO
{
	int m_nIndex;

	DRAGON_MONSTER_INFO()
	{
		this->m_nIndex = -1;
	}
};

class CDragonEvent
{

public:

	CDragonEvent();
	virtual ~CDragonEvent();

	void ClearMonster();

	void LoadScript(char *szFile);

	BYTE GetState(){return this->EventState;}	// Dagon event line 30.h
	BYTE GetMapNumber(){return this->m_MapNumber;}	// Dragon event line 35.h 
	void Init();
	void Start();
	void End();
	void Run();
	void DragonActive();
	void SetMenualStart(BOOL bMenualStart){this->m_bMenualStart = bMenualStart;}
	void Start_Menual();
	void End_Menual();

	DRAGON_EVENT_INFO * GetMonsterPos(int iMonsterIndex);

	bool IsDragonEventMonster(int iIndex);

private:
	BOOL GetBoxPosition(int mapnumber, int ax, int ay, int aw, int ah, short &mx, short &my);

	BYTE EventState;	// 4
	ULONGLONG EventStartTime;	// 8
	BYTE m_MapNumber;	// C
	BOOL m_bMenualStart;	// 10
	std::map<int, DRAGON_EVENT_INFO> m_mapEventInfo;
	std::map<int, DRAGON_MONSTER_INFO> m_mapMonsterInfo;

	CRITICAL_SECTION m_MonCriti;
};

#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////


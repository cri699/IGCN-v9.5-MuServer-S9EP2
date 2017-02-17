//////////////////////////////////////////////////////////////////////
// AttackEvent.h
// ------------------------------
// Decompiled by Deathway
// Date : 2007-05-09
// ------------------------------
#ifndef ATTACKEVENT_H
#define ATTACKEVENT_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct ATTACKEVENT_MONSTER_POS
{
	WORD m_Type;	// 2
	BYTE m_MapNumber[4];	// 4
	BYTE m_Dis;	// 5
	BYTE m_X;	// 6
	BYTE m_Y;	// 7
	BYTE m_Dir;	// 8
	BYTE m_W;	// 9
	BYTE m_H;	// A
	WORD m_Count;
};

struct ATTACKEVENT_MONSTER_INFO
{
	int m_nIndex;

	ATTACKEVENT_MONSTER_INFO()
	{
		this->m_nIndex = -1;
	}
};

class CAttackEvent
{
public:

	CAttackEvent();
	virtual ~CAttackEvent();

	void LoadScript(char * szFile);

	void ClearMonster();

	void Init();
	void Start();
	void End();
	void Run();
	void Active();

	void Start_Menual();
	void SetMenualStart(BOOL bMenualStart){this->m_bMenualStart = bMenualStart;};	// line : 39
	void End_Menual();

	bool IsAttackEventMonster(int iIndex);

	ATTACKEVENT_MONSTER_POS * GetMonsterPos(int iMonsterIndex);

private:

	BOOL GetBoxPosition(int mapnumber, int ax, int ay, int aw, int ah, short &mx, short &my);
	BYTE GetMapNumber(std::map<int, ATTACKEVENT_MONSTER_POS>::iterator Iter);

	bool EventState;	// 4
	ULONGLONG EventStartTime;	// 8
	BYTE m_MapNumber;	// C
	int m_BossMap55;	// 10
	int m_BossMapX55;	// 14
	int m_BossMapY55;	// 18
	int m_BossMap53;	// 1C
	int m_BossMapX53;	// 20
	int m_BossMapY53;	// 24
	BOOL m_bMenualStart;	// 28
	std::map<int, ATTACKEVENT_MONSTER_POS> m_mapMonsterPos;
	std::map<int, ATTACKEVENT_MONSTER_INFO> m_mapMonsterInfo;

	CRITICAL_SECTION m_MonCriti;
};

#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

